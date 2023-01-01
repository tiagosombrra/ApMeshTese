#include "../../include/generator/generator_adaptive.h"

GeneratorAdaptive::GeneratorAdaptive() {}

#if USE_MPI
int GeneratorAdaptive::Execute(char* argv[], Timer* timer, MPI_Status status)
#else
int GeneratorAdaptive::Execute(char* argv[], Timer* timer)
#endif  // USE_MPI
{
  Geometry* geometry = new Geometry;

#if USE_MPI
  if (RANK_MPI == 0) {
    // estimativa e ordenação dos patches para carga de distribuiçao dos patches
    std::list<PatchBezier*> patches = EstimateChargeofPatches(geometry, timer);

    // distribuição dos patches entre os (n) processos de acordo com suas cargas
    // ordenadas
    patches = OrderPatchesDistribProcess(patches);

#if USE_PRINT_ESTIMATE
    // mostra a distribuição de patches para cada processo
    calculateEstimateProcessElements(SIZE_MPI, listBezierPt);
#endif  // USE_PRINT_ESTIMATE

    double patches_process0[patches.size() * 48];
    double patches_process[patches.size() * 48];

    int i = 0, izero = 0;

    for (int j = 0; j < SIZE_MPI; j++) {
      i = 0;

      for (std::list<PatchBezier*>::iterator it = patches.begin();
           it != patches.end(); it++) {
        if (j == (*it)->GetIdProcess()) {
          patches_process[i] = (*it)->GetPt00().GetX();
          i++;
          patches_process[i] = (*it)->GetPt00().GetY();
          i++;
          patches_process[i] = (*it)->GetPt00().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt10().GetX();
          i++;
          patches_process[i] = (*it)->GetPt10().GetY();
          i++;
          patches_process[i] = (*it)->GetPt10().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt20().GetX();
          i++;
          patches_process[i] = (*it)->GetPt20().GetY();
          i++;
          patches_process[i] = (*it)->GetPt20().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt30().GetX();
          i++;
          patches_process[i] = (*it)->GetPt30().GetY();
          i++;
          patches_process[i] = (*it)->GetPt30().GetZ();
          i++;

          patches_process[i] = (*it)->GetPt01().GetX();
          i++;
          patches_process[i] = (*it)->GetPt01().GetY();
          i++;
          patches_process[i] = (*it)->GetPt01().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt11().GetX();
          i++;
          patches_process[i] = (*it)->GetPt11().GetY();
          i++;
          patches_process[i] = (*it)->GetPt11().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt21().GetX();
          i++;
          patches_process[i] = (*it)->GetPt21().GetY();
          i++;
          patches_process[i] = (*it)->GetPt21().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt31().GetX();
          i++;
          patches_process[i] = (*it)->GetPt31().GetY();
          i++;
          patches_process[i] = (*it)->GetPt31().GetZ();
          i++;

          patches_process[i] = (*it)->GetPt02().GetX();
          i++;
          patches_process[i] = (*it)->GetPt02().GetY();
          i++;
          patches_process[i] = (*it)->GetPt02().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt12().GetX();
          i++;
          patches_process[i] = (*it)->GetPt12().GetY();
          i++;
          patches_process[i] = (*it)->GetPt12().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt22().GetX();
          i++;
          patches_process[i] = (*it)->GetPt22().GetY();
          i++;
          patches_process[i] = (*it)->GetPt22().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt32().GetX();
          i++;
          patches_process[i] = (*it)->GetPt32().GetY();
          i++;
          patches_process[i] = (*it)->GetPt32().GetZ();
          i++;

          patches_process[i] = (*it)->GetPt03().GetX();
          i++;
          patches_process[i] = (*it)->GetPt03().GetY();
          i++;
          patches_process[i] = (*it)->GetPt03().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt13().GetX();
          i++;
          patches_process[i] = (*it)->GetPt13().GetY();
          i++;
          patches_process[i] = (*it)->GetPt13().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt23().GetX();
          i++;
          patches_process[i] = (*it)->GetPt23().GetY();
          i++;
          patches_process[i] = (*it)->GetPt23().GetZ();
          i++;
          patches_process[i] = (*it)->GetPt33().GetX();
          i++;
          patches_process[i] = (*it)->GetPt33().GetY();
          i++;
          patches_process[i] = (*it)->GetPt33().GetZ();
          i++;
        }
      }

      if (j == 0) {
        izero = i;

        for (int m = 0; m < i; ++m) {
          patches_process0[m] = patches_process[m];
        }

      } else {
        timer->InitTimerParallel(RANK_MPI, 0, 0);  // Send
        MPI_Send(&TIME_READ_FILE, 1, MPI_DOUBLE, j, TAG_TIME, MPI_COMM_WORLD);
        MPI_Send(&i, 1, MPI_INT, j, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD);
        MPI_Send(&patches_process, i, MPI_DOUBLE, j, TAG_DOUBLE,
                 MPI_COMM_WORLD);
        timer->EndTimerParallel(RANK_MPI, 0, 0);  // Send
      }
    }

    // gerador de malha para o processo root
#if USE_OPENMP
    Generator(patches_process0, izero, timer, 1024, atoi(argv[1]),
              atoi(argv[2]));
#else
    Generator(patches_process0, izero, timer, 1024, atoi(argv[1]));
#endif

  } else {
    timer->InitTimerParallel(RANK_MPI, 0, 9);  // Recv

    MPI_Recv(&TIME_READ_FILE, 1, MPI_DOUBLE, 0, TAG_TIME, MPI_COMM_WORLD,
             &status);

    int size_patches = 0;
    MPI_Recv(&size_patches, 1, MPI_INT, 0, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD,
             &status);

    double patches[size_patches];
    MPI_Recv(&patches, size_patches, MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD,
             &status);

    timer->EndTimerParallel(RANK_MPI, 0, 9);  // Recv

    // gerador de malha para o processo (n)
#if USE_OPENMP
    Generator(patches, size_patches, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
    Generator(patches, size_patches, timer, 1024, atoi(argv[1]));
#endif
  }
#else

  Model model;
  PatchReader patch_reader;

  // Inclusão dos patches no Modelo
  Models3d models3d;
  model.SetGeometry(models3d.ModelUtahteapot(geometry));

  // if (argv[3]) {
  //   timer->InitTimerParallel(0, 0, 5);  // Leitura arquivo
  //   model.SetGeometry(patch_reader.ReaderFilePatches(geometry, argv[3]));
  //   timer->EndTimerParallel(0, 0, 5);  // Leitura arquivo
  // } else {
  //   Models3d models3d;
  //   model.SetGeometry(models3d.ModelUtahteapot(geometry));
  // }

#if USE_OPENMP
  Generator(model, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
  Generator(model, timer, 1024);
#endif  // USE_OPENMP

#endif  // USE_MPI
  return 0;
}

#if USE_MPI
std::list<PatchBezier*> GeneratorAdaptive::EstimateChargeofPatches(
    Geometry* geometry, Timer* timer) {
  ChargeEstimateProcess* charge_estimate_process = new ChargeEstimateProcess();
  std::list<PatchBezier*> patches =
      charge_estimate_process->ChargeEstimate(geometry, timer);
  delete charge_estimate_process;

  return patches;
}

std::vector<CurveAdaptive*> GeneratorAdaptive::CreateVectorOfCurves(
    std::list<PatchBezier*> patches) {
  CurveAdaptive* curve;
  std::vector<CurveAdaptive*> curves;

  for (std::list<PatchBezier*>::iterator it = patches.begin();
       it != patches.end(); it++) {
    if (curves.size() > 0) {
      if (VerifyCurve((*it)->GetPt00(), (*it)->GetPt10(), (*it)->GetPt20(),
                      (*it)->GetPt30(), curves)) {
        curve = new CurveAdaptiveParametricBezier(
            (*it)->GetPt00(), (*it)->GetPt10(), (*it)->GetPt20(),
            (*it)->GetPt30());
        curves.push_back(curve);
      }
      if (VerifyCurve((*it)->GetPt30(), (*it)->GetPt31(), (*it)->GetPt32(),
                      (*it)->GetPt33(), curves)) {
        curve = new CurveAdaptiveParametricBezier(
            (*it)->GetPt30(), (*it)->GetPt31(), (*it)->GetPt32(),
            (*it)->GetPt33());
        curves.push_back(curve);
      }
      if (VerifyCurve((*it)->GetPt03(), (*it)->GetPt13(), (*it)->GetPt23(),
                      (*it)->GetPt33(), curves)) {
        curve = new CurveAdaptiveParametricBezier(
            (*it)->GetPt03(), (*it)->GetPt13(), (*it)->GetPt23(),
            (*it)->GetPt33());
        curves.push_back(curve);
      }
      if (VerifyCurve((*it)->GetPt00(), (*it)->GetPt01(), (*it)->GetPt02(),
                      (*it)->GetPt03(), curves)) {
        curve = new CurveAdaptiveParametricBezier(
            (*it)->GetPt00(), (*it)->GetPt01(), (*it)->GetPt02(),
            (*it)->GetPt03());
        curves.push_back(curve);
      }
    } else if (curves.size() == 0) {
      curve =
          new CurveAdaptiveParametricBezier((*it)->GetPt00(), (*it)->GetPt10(),
                                            (*it)->GetPt20(), (*it)->GetPt30());
      curves.push_back(curve);
      curve =
          new CurveAdaptiveParametricBezier((*it)->GetPt30(), (*it)->GetPt31(),
                                            (*it)->GetPt32(), (*it)->GetPt33());
      curves.push_back(curve);
      curve =
          new CurveAdaptiveParametricBezier((*it)->GetPt03(), (*it)->GetPt13(),
                                            (*it)->GetPt23(), (*it)->GetPt33());
      curves.push_back(curve);
      curve =
          new CurveAdaptiveParametricBezier((*it)->GetPt00(), (*it)->GetPt01(),
                                            (*it)->GetPt02(), (*it)->GetPt03());
      curves.push_back(curve);
    }
  }

  return curves;
}

std::list<PatchBezier*> GeneratorAdaptive::OrderPatchesDistribProcess(
    std::list<PatchBezier*> patches) {
  std::vector<std::pair<double, int> > process_pairs;
  std::list<PatchBezier*> patches_order;

  if (SIZE_MPI > 1) {
    for (int i = 0; i < SIZE_MPI; i++) {
      process_pairs.push_back(std::make_pair(0.0, i));
    }

    for (std::list<PatchBezier*>::iterator it = patches.begin();
         it != patches.end(); it++) {
      std::sort(process_pairs.begin(), process_pairs.end());

      (*it)->SetIdProcess(process_pairs.front().second);

      process_pairs.front().first += (*it)->GetNumberTriangle();
    }

    for (int i = 0; i < SIZE_MPI; i++) {
      for (std::list<PatchBezier*>::iterator it = patches.begin();
           it != patches.end(); it++) {
        if ((*it)->GetIdProcess() == i) {
          patches_order.push_back((*it));
        }
      }
    }

    return patches_order;

  } else {
    for (std::list<PatchBezier*>::iterator it = patches.begin();
         it != patches.end(); it++) {
      (*it)->SetIdProcess(0);
    }

    return patches;
  }
}

bool GeneratorAdaptive::VerifyCurve(PointAdaptive p0, PointAdaptive p1,
                                    PointAdaptive p2, PointAdaptive p3,
                                    std::vector<CurveAdaptive*> curves) {
  for (vector<CurveAdaptive*>::iterator it = curves.begin(); it != curves.end();
       it++) {
    if (static_cast<CurveAdaptiveParametricBezier*>(*it)->GetPoint0().
        operator==(p0) &&
        static_cast<CurveAdaptiveParametricBezier*>(*it)->GetPoint1().
        operator==(p1) &&
        static_cast<CurveAdaptiveParametricBezier*>(*it)->GetPoint2().
        operator==(p2) &&
        static_cast<CurveAdaptiveParametricBezier*>(*it)->GetPoint3().
        operator==(p3)) {
      return false;
    }
  }

  return true;
}

void GeneratorAdaptive::CalculateEstimateProcessElements(
    int size_process, std::list<PatchBezier*> patches) {
  double estimate[size_process] = {};

  for (std::list<PatchBezier*>::iterator it = patches.begin();
       it != patches.end(); it++) {
    estimate[(*it)->GetIdProcess()] += (*it)->GetNumberTriangle();
  }

  for (int i = 0; i < SIZE_MPI; ++i) {
    cout << "Estimativa para o processo " << i << " = " << estimate[i] << endl;
  }
}

std::list<PatchBezier*>::iterator GeneratorAdaptive::GetIteratorListPatches(
    int size_patches, std::list<PatchBezier*> patches) {
  std::list<PatchBezier*>::iterator it = patches.begin();
  advance(it, size_patches);
  return it;
}

Geometry* GeneratorAdaptive::UnpakGeometry(double patches[], int size_patches) {
  Geometry* geometry = new Geometry;

  PointAdaptive* p00;
  PointAdaptive* p01;
  PointAdaptive* p02;
  PointAdaptive* p03;
  PointAdaptive* p10;
  PointAdaptive* p11;
  PointAdaptive* p12;
  PointAdaptive* p13;
  PointAdaptive* p20;
  PointAdaptive* p21;
  PointAdaptive* p22;
  PointAdaptive* p23;
  PointAdaptive* p30;
  PointAdaptive* p31;
  PointAdaptive* p32;
  PointAdaptive* p33;

  CurveAdaptive* patch_c1;
  CurveAdaptive* patch_c2;
  CurveAdaptive* patch_c3;
  CurveAdaptive* patch_c4;

  PatchBezier* patch_bezier;

  for (int i = 0; i < size_patches; i = i + 48) {
    p00 = new VertexAdaptive(patches[i], patches[i + 1], patches[i + 2]);
    p10 = new VertexAdaptive(patches[i + 3], patches[i + 4], patches[i + 5]);
    p20 = new VertexAdaptive(patches[i + 6], patches[i + 7], patches[i + 8]);
    p30 = new VertexAdaptive(patches[i + 9], patches[i + 10], patches[i + 11]);

    p01 = new VertexAdaptive(patches[i + 12], patches[i + 13], patches[i + 14]);
    p11 = new VertexAdaptive(patches[i + 15], patches[i + 16], patches[i + 17]);
    p21 = new VertexAdaptive(patches[i + 18], patches[i + 19], patches[i + 20]);
    p31 = new VertexAdaptive(patches[i + 21], patches[i + 22], patches[i + 23]);

    p02 = new VertexAdaptive(patches[i + 24], patches[i + 25], patches[i + 26]);
    p12 = new VertexAdaptive(patches[i + 27], patches[i + 28], patches[i + 29]);
    p22 = new VertexAdaptive(patches[i + 30], patches[i + 31], patches[i + 32]);
    p32 = new VertexAdaptive(patches[i + 33], patches[i + 34], patches[i + 35]);

    p03 = new VertexAdaptive(patches[i + 36], patches[i + 37], patches[i + 38]);
    p13 = new VertexAdaptive(patches[i + 39], patches[i + 40], patches[i + 41]);
    p23 = new VertexAdaptive(patches[i + 42], patches[i + 43], patches[i + 44]);
    p33 = new VertexAdaptive(patches[i + 45], patches[i + 46], patches[i + 47]);

    patch_c1 = new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
    patch_c2 = new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
    patch_c3 = new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
    patch_c4 = new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);

    if (geometry->VerifyCurveGeometry(p00, p10, p20, p30) == nullptr) {
      patch_c1 = new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
      geometry->InsertCurve(patch_c1);
    } else {
      patch_c1 = geometry->VerifyCurveGeometry(p00, p10, p20, p30);
    }

    if (geometry->VerifyCurveGeometry(p30, p31, p32, p33) == nullptr) {
      patch_c2 = new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
      geometry->InsertCurve(patch_c2);
    } else {
      patch_c2 = geometry->VerifyCurveGeometry(p30, p31, p32, p33);
    }

    if (geometry->VerifyCurveGeometry(p03, p13, p23, p33) == nullptr) {
      patch_c3 = new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
      geometry->InsertCurve(patch_c3);
    } else {
      patch_c3 = geometry->VerifyCurveGeometry(p03, p13, p23, p33);
    }

    if (geometry->VerifyCurveGeometry(p00, p01, p02, p03) == nullptr) {
      patch_c4 = new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);
      geometry->InsertCurve(patch_c4);
    } else {
      patch_c4 = geometry->VerifyCurveGeometry(p00, p01, p02, p03);
    }

    patch_bezier = new PatchBezier(patch_c1, patch_c2, patch_c3, patch_c4, *p11,
                                   *p21, *p12, *p22);
    patch_bezier->SetId(i / 48);

    geometry->InsertPatch(patch_bezier);
  }

  return geometry;
}

#endif

#if USE_MPI
void GeneratorAdaptive::Generator(double patches[], int size_patches,
                                  Timer* timer, int id_range, int size_rank,
                                  int size_thread)
#else
void GeneratorAdaptive::Generator(Model& model, Timer* timer, int id_range,
                                  int size_rank, int size_thread)
#endif
{
#if USE_MPI
  this->communicator_ = new ApMeshCommunicator(true);
  Geometry* geometry = UnpakGeometry(patches, size_patches);
#elif USE_OPENMP
  this->communicator_ = new ApMeshCommunicator(true);
  Geometry* geometry = model.GetGeometry();
#else
  this->communicator_ = new Parallel::TMCommunicator(false);
  Geometry* geometry = model.GetGeometry();
#endif

  int size_patch = geometry->GetNumBerPatches();

  MeshAdaptive* mesh = new MeshAdaptive;
  mesh->ResizeSubMeshAdaptiveByPosition(size_patch);

  this->id_manager_ = nullptr;
  this->id_off_set_ = 0;
  this->id_range_ = id_range;

  this->step_ = 0;

  size_thread =
      size_thread > static_cast<Parallel::TMCommunicator*>(this->communicator_)
                        ->getMaxThreads()
          ? static_cast<Parallel::TMCommunicator*>(this->communicator_)
                ->getMaxThreads()
          : size_thread;

  ptr_aux.resize(size_thread, nullptr);

  if (this->id_managers_.empty()) {
    this->id_managers_.resize(size_thread, nullptr);
  }

  // Gerar a malha inicial
#if USE_OPENMP
  GeneratorInitialMesh(geometry, mesh, timer, size_thread, size_patch);
#else
  this->id_managers_[0] = this->MakeIdManager(communicator_, 0);
#if USE_MPI
  timer->InitTimerParallel(RANK_MPI, 0, 2);  // Malha inicial
#else
  timer->InitTimerParallel(0, 0, 2);    // Malha inicial
#endif  // USE_MPI

  GeneratorInitialMesh(geometry, mesh, timer, size_thread, size_patch);

#if USE_MPI
  timer->EndTimerParallel(RANK_MPI, 0, 2);   // Malha inicial
#else
  timer->EndTimerParallel(0, 0, 2);     // Malha inicial
#endif  // USE_MPI
#endif  // USE_OPENMP

  // Calcula o erro global para a malha inicial
#if USE_OPENMP
  if (size_patch > 1) {
    this->error_local_process_ =
        this->CalculateErrorGlobalOmp(mesh, timer, 0, size_thread);

  } else {
    this->error_local_process_ = this->ErrorGlobal(mesh, timer, 0, size_thread);
  }
#else
#if USE_MPI
  timer->InitTimerParallel(RANK_MPI, 0, 7);  // Calculo do erro Global
#else
  timer->InitTimerParallel(0, 0, 7);    // Calculo do erro Global
#endif  // USE_MPI
  this->error_local_process_ = this->ErrorGlobal(mesh, timer);
#if USE_MPI
  timer->EndTimerParallel(RANK_MPI, 0, 7);  // Calculo do erro Global
#else
  timer->EndTimerParallel(0, 0, 7);     // Calculo do erro Global
#endif  // USE_MPI
#endif  // USE_OPENMP

  this->error_step_.push_back(this->error_local_process_);

#if USE_SAVE_ERRO_MESH
  SaveErrorMesh(malha, step_);
#endif  // USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
  cout << "*************** ERRO " << this->step_ << " rank " << RANK_MPI
       << " = " << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
  cout << "*************** ERRO " << this->step_ << " = "
       << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#endif  // USE_MPI

  if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
    SaveMesh(mesh, step_);
  }

  // this->error_local_process_ = 1000;

  // Gerar malha enquanto o erro global for acima do erro desejado
  while (this->step_ < PASSOS) {
    if (this->error_local_process_ < EPSYLON) {
      break;
    }
    // #if USE_MPI
    //         timer->InitTimerParallel(RANK_MPI,0,9); // SendRecv
    //         MPI_Allreduce(&this->error_local_process_,
    //         &this->error_local_process_, 1, MPI_DOUBLE, MPI_SUM,
    //         MPI_COMM_WORLD); timer->EndTimerParallel(RANK_MPI,0,9); //
    //         SendRecv this->error_local_process_ = this->error_local_process_
    //         / sizeRank;
    // #endif //USE_MPI

    this->step_++;

    // Aloca uma nova malha
    mesh = new MeshAdaptive;
    mesh->ResizeSubMeshAdaptiveByPosition(geometry->GetNumBerPatches());
#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, 0, 3);  // Adaptação da curva
#else
    timer->InitTimerParallel(0, 0, 3);         // Adaptação da curva
#endif  // USE_MPI

    // Adapta as curvas pela curvatura da curva / Atualiza a discretização das
    // curvas
    AdaptCurve(geometry);

#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, 0, 3);  // Adaptação da curva
#else
    timer->EndTimerParallel(0, 0, 3);          // Adaptação da curva
#endif  // USE_MPI

    // Adapta as patches / Atualiza os patches
#if USE_OPENMP
    AdaptDomainOmp(geometry, mesh, timer, size_thread, size_patch);
#else
#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, 0, 4);  // Adaptação do domínio
#else
    timer->InitTimerParallel(0, 0, 4);  // Adaptação do domínio
#endif  // USE_MPI
    AdaptDomain(geometry, mesh);
#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, 0, 4);   // Adaptação do domínio
#else
    timer->EndTimerParallel(0, 0, 4);   // Adaptação do domínio
#endif  // USE_MPI
#endif  // USE_OPENMP

    // Calcula o erro global para da malha
#if USE_OPENMP
    if (size_patch > 1) {
      this->error_local_process_ =
          this->CalculateErrorGlobalOmp(mesh, timer, 0, size_thread);

    } else {
      this->error_local_process_ =
          this->ErrorGlobal(mesh, timer, 0, size_thread);
    }
#else
#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, 0, 7);  // Calculo do erro Global
#else
    timer->InitTimerParallel(0, 0, 7);  // Calculo do erro Global
#endif  // USE_MPI
    this->error_local_process_ = this->ErrorGlobal(mesh, timer);
#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, 0, 7);  // Calculo do erro Global
#else
    timer->EndTimerParallel(0, 0, 7);   // Calculo do erro Global
#endif  // USE_MPI
#endif

    this->error_step_.push_back(this->error_local_process_);

#if USE_SAVE_ERRO_MESH
    SaveErrorMesh(malha, step_);
#endif  // USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->step_ << " rank " << RANK_MPI
         << " = " << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->step_ << " = "
         << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#endif  // USE_MPI

    if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
      SaveMesh(mesh, step_);
    }
  }

#if USE_MPI
  timer->EndTimerParallel(RANK_MPI, 0, 10);  // Full
#endif                                       // USE_MPI

  // Escreve o(s) arquivo(s) com suas respectivas malhas em cada step_
#if USE_MPI
  for (unsigned int i = 0; i < error_step_.size(); ++i) {
    cout << "Erro do processo " << RANK_MPI << " no step_ " << i << " = "
         << error_step_[i] << endl;
  }

  if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
    for (MeshVector::iterator it = save_mesh_.begin(); it != save_mesh_.end();
         it++) {
      if (WRITE_MESH == std::string("q")) {
        PrintElments((*it).second, (*it).first, this->error_step_, RANK_MPI);
        WriteQualityMesh((*it).second, (*it).first, this->error_step_,
                         RANK_MPI);
      } else if (WRITE_MESH == std::string("m")) {
        PrintElments((*it).second, (*it).first, this->error_step_, RANK_MPI);
        WriteMesh((*it).second, (*it).first, this->error_step_, RANK_MPI);
      }
    }
  }
#else

  timer->EndTimerParallel(0, 0, 10);  // Full
  timer->PrintTime();

  if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
    for (MeshVector::iterator it = save_mesh_.begin(); it != save_mesh_.end();
         it++) {
      if (WRITE_MESH == std::string("q")) {
        PrintElments((*it).second, (*it).first, this->error_step_);
        WriteQualityMesh((*it).second, (*it).first, this->error_step_);
      } else {
        PrintElments((*it).second, (*it).first, this->error_step_);
        WriteMesh((*it).second, (*it).first, this->error_step_);
      }
    }
    for (unsigned int i = 0; i < error_step_.size(); ++i) {
      cout << "Erro no step_ " << i << " = " << error_step_[i] << endl;
    }
  }
#endif  // USE_MPI
}

void GeneratorAdaptive::AdaptCurve(Geometry* geometry) {
  list<PointAdaptive*> new_points[geometry->GetNumBerCurves()];
  map<PointAdaptive*, PointAdaptive*> map_points;

  for (unsigned int i = 0; i < geometry->GetNumBerCurves(); ++i) {
    new_points[i] = adapter.AdaptCurveByCurve(geometry->GetCurve(i), map_points,
                                              this->id_managers_[0], 1);
    geometry->GetCurve(i)->SetPoints(new_points[i]);
    new_points[i] = adapter.AdaptCurveBySurface(
        geometry->GetCurve(i), map_points, this->id_managers_[0], 1);
    geometry->GetCurve(i)->SetPoints(new_points[i]);
  }
}

void GeneratorAdaptive::AdaptDomain(Geometry* geometry, MeshAdaptive* mesh) {
  for (unsigned int i = 0; i < geometry->GetNumBerPatches(); ++i) {
    PatchCoons* p = static_cast<PatchCoons*>(geometry->GetPatch(i));
    SubMesh* sub_mesh = adapter.AdaptDomain(p, this->id_managers_[0], 1);
    sub_mesh->SetPatch(p);
    mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
    geometry->GetPatch(i)->SetSubMesh(mesh->GetSubMeshAdaptiveByPosition(i));
  }
}

#if USE_OPENMP
SubMesh* GeneratorAdaptive::GeneratorInitialMeshOmp(
    PatchCoons* patch, Performer::IdManager* id_manager) {
  CurveAdaptive* c1 = patch->GetCurve(0);
  CurveAdaptive* c2 = patch->GetCurve(1);
  CurveAdaptive* c3 = patch->GetCurve(2);
  CurveAdaptive* c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = nullptr;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = nullptr;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = nullptr;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = nullptr;  // c4 já foi trabalhada no patch vizinho

  SubMesh* sub_mesh = new SubMesh;

  //========================= Malha Grosseira
  //====================================
  // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região

  for (double v = 0.0; v <= 1.0; v += 1) {
    for (double u = 0.0; u <= 1.0; u += 1) {
      PointAdaptive* point = new NodeAdaptive(patch->Parameterize(u, v));
      point->SetId(id_manager->next(0));

      if (v == 0 && c1)  // p está na curva 1 (c1 = nullptr)
        c1->InsertPoint(point);
      else if (v == 1 && c3)  // p está na curva 3
        c3->InsertPoint(point);

      if (u == 0 && c4)  // p está na curva 4
        c4->InsertPoint(point);
      else if (u == 1 && c2)  // p está na curva 2
        c2->InsertPoint(point);

      sub_mesh->SetNoh(static_cast<NodeAdaptive*>(point));
    }
  }

  PointAdaptive* point = new NodeAdaptive(patch->Parameterize(0.5, 0.5));
  point->SetId(id_manager->next(0));
  sub_mesh->SetNoh(static_cast<NodeAdaptive*>(point));

  ElementAdaptive* e1 = new TriangleAdaptive(
      sub_mesh->GetNoh(0), sub_mesh->GetNoh(1), sub_mesh->GetNoh(4));
  ((TriangleAdaptive*)e1)->SetParametersN1(make_tuple(0, 0));
  ((TriangleAdaptive*)e1)->SetParametersN2(make_tuple(1, 0));
  ((TriangleAdaptive*)e1)->SetParametersN3(make_tuple(0.5, 0.5));
  e1->SetId(id_manager->next(1));
  sub_mesh->SetElement(e1);

  ElementAdaptive* e2 = new TriangleAdaptive(
      sub_mesh->GetNoh(1), sub_mesh->GetNoh(3), sub_mesh->GetNoh(4));
  ((TriangleAdaptive*)e2)->SetParametersN1(make_tuple(1, 0));
  ((TriangleAdaptive*)e2)->SetParametersN2(make_tuple(1, 1));
  ((TriangleAdaptive*)e2)->SetParametersN3(make_tuple(0.5, 0.5));
  e2->SetId(id_manager->next(1));
  sub_mesh->SetElement(e2);

  ElementAdaptive* e3 = new TriangleAdaptive(
      sub_mesh->GetNoh(3), sub_mesh->GetNoh(2), sub_mesh->GetNoh(4));
  ((TriangleAdaptive*)e3)->SetParametersN1(make_tuple(1, 1));
  ((TriangleAdaptive*)e3)->SetParametersN2(make_tuple(0, 1));
  ((TriangleAdaptive*)e3)->SetParametersN3(make_tuple(0.5, 0.5));
  e3->SetId(id_manager->next(1));
  sub_mesh->SetElement(e3);

  ElementAdaptive* e4 = new TriangleAdaptive(
      sub_mesh->GetNoh(2), sub_mesh->GetNoh(0), sub_mesh->GetNoh(4));
  ((TriangleAdaptive*)e4)->SetParametersN1(make_tuple(0, 1));
  ((TriangleAdaptive*)e4)->SetParametersN2(make_tuple(0, 0));
  ((TriangleAdaptive*)e4)->SetParametersN3(make_tuple(0.5, 0.5));
  e4->SetId(id_manager->next(1));
  sub_mesh->SetElement(e4);
  //==============================================================================*/

  // 5. define a submalha do patch
  patch->SetSubMesh(sub_mesh);
  sub_mesh->SetPatch(patch);

  return sub_mesh;
}

double GeneratorAdaptive::CalculateErrorGlobalOmp(MeshAdaptive* mesh,
                                                  Timer* timer, int rank,
                                                  int size_thread) {
  unsigned int Ns = 0;  // número de submalhas
  double Nj = 0.0;      // erro global da malha

  Ns = mesh->GetNumberSubMeshesAdaptive();

  // Calcula o erro global de cada submalha (OMP)
#pragma omp parallel num_threads(size_thread) firstprivate(Ns) reduction(+ : Nj)
  {
#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                             7);  // calculo do erro global
#else
    timer->InitTimerParallel(0, omp_get_thread_num(),
                             7);  // calculo do erro global
#endif  // USE_MPI
#pragma omp for
    for (unsigned int i = 0; i < Ns; ++i) {
      SubMesh* sub = mesh->GetSubMeshAdaptiveByPosition(i);
      unsigned int Nv = sub->GetNumberNos();
      double curvPower = 0.0;
      double Njs = 0.0;

      // Calcula o erro relativo para cada nó e soma a Nj
      // #pragma omp parallel for num_threads(nThreads) firstprivate(Ns)
      // reduction(+ :Nj)
      for (unsigned int j = 0; j < Nv; ++j) {
#if USE_MPI
        timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(),
                                7);  // calculo do erro global
        timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                                 6);  // MediaGauss
#else
        timer->EndTimerParallel(0, omp_get_thread_num(),
                                7);  // calculo do erro global
        timer->InitTimerParallel(0, omp_get_thread_num(), 6);  // MediaGauss
#endif  // USE_MPI

        PointAdaptive* n = sub->GetNoh(j);
        Patch* p = sub->GetPatch();
        CurvatureAnalytical ka(*(static_cast<NodeAdaptive*>(n)),
                               *(static_cast<PatchCoons*>(p)));
        CurvatureDiscrete kd(*(static_cast<NodeAdaptive*>(n)));
        double Ga = ka.CalculateGaussCurvature();
        double Gd = kd.CalculateGaussCurvature();
        double Ha = ka.CalculateMeanCurvature();
        double Hd = kd.CalculateMeanCurvature();

        // tratamento para erro -nan
        if (std::isnan(Gd)) {
          cout << "-nan Gd" << endl;
          Ga = 0.0;
          Gd = 0.0;
        }

        // atualiza as curvaturas do nó ( para que não sejam recalculadas na
        // adaptação das curvas e do domínio )
        ((NodeAdaptive*)n)->SetGa(Ga);
        ((NodeAdaptive*)n)->SetGd(Gd);
        ((NodeAdaptive*)n)->SetHa(Ha);
        ((NodeAdaptive*)n)->SetHd(Hd);
#if USE_MPI
        timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(),
                                6);  // MediaGauss
        timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                                 7);  // calculo do erro global
#else
        timer->EndTimerParallel(0, omp_get_thread_num(), 6);   // MediaGauss
        timer->InitTimerParallel(0, omp_get_thread_num(),
                                 7);  // calculo do erro global
#endif  // USE_MPI

        double power = 0.0;
        double diff = 0.0;

        if (fabs(Ga) >= TOLERANCIA) {
          diff = Gd - Ga;
          power = pow(diff, 2);
          Njs += power;
          curvPower += pow(Ga, 2);
        } else if (fabs(Ha) >= TOLERANCIA) {
          diff = Hd - Ha;
          power = pow(diff, 2);
          Njs += power;
          curvPower += pow(Ha, 2);
        }
      }

      if (Njs > 0.0 && curvPower > 0.0) {
        Njs = (double)sqrt(Njs / curvPower) / Nv;
      }

      if (!std::isnan(Njs))
        Nj += Njs;
      else
        cout << "Njs -nan" << endl;

    }  // Parallel for

#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(),
                            7);  // calculo do erro global
#else
    timer->EndTimerParallel(0, omp_get_thread_num(),
                            7);          // calculo do erro global
#endif  // USE_MPI
  }

  Nj /= Ns;  // o erro global é a média do erro das submalhas

  return Nj;
}

int GeneratorAdaptive::GeneratorOmp(Model& model, Timer* timer, int id_range,
                                    int size_rank, int size_thread)

{
  this->id_manager_ = nullptr;
  this->id_off_set_ = 0;
  this->id_range_ = id_range;
  this->communicator_ = new ApMeshCommunicator(true);

#if USE_MPI
  Int nProcesses = 1;
  Int rank = 0;
  nProcesses = this->communicator_->numProcesses();
  rank = this->communicator_->rank();
#endif  // #if USE_MPI

  Geometry* geo = model.GetGeometry();
  int sizePatch = geo->GetNumBerPatches();

  MeshAdaptive* malha = new MeshAdaptive;
  malha->ResizeSubMeshAdaptiveByPosition(sizePatch);

  this->step_ = 0;

  // sizeThread = static_cast<Parallel::TMCommunicator
  // *>(this->comm)->getMaxThreads();

  ptr_aux.resize(size_thread, nullptr);

  if (this->id_managers_.empty()) {
    this->id_managers_.resize(size_thread, nullptr);
  }

#pragma omp parallel num_threads(size_thread) shared(malha, geo, sizePatch)
  {
    Int id = communicator_->threadId();

    if (!this->id_managers_[id]) {
      this->id_managers_[id] = this->MakeIdManagerOmp(communicator_, id);
    }

    timer->InitTimerParallel(0, id, 2);  // Malha inicial

    // 1. Gera a malha inicial
#pragma omp for
    for (int i = 0; i < sizePatch; ++i) {
      PatchCoons* patch = static_cast<PatchCoons*>(geo->GetPatch(i));
      SubMesh* sub = this->GeneratorInitialMeshOmp(
          static_cast<PatchCoons*>(patch), this->id_managers_[id]);
      malha->InsertSubMeshAdaptiveByPosition(sub, i);
    }

    timer->EndTimerParallel(0, id, 2);  // Malha inicial
  }

  // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
  model.InsertMeshAdaptive(malha);

  // 3. Calcula o erro global para a malha inicial
  this->error_local_process_ =
      this->CalculateErrorGlobalOmp(malha, timer, 0, size_thread);

#if USE_PRINT_ERRO
  cout << "ERRO  " << this->step_ << " = " << this->error_local_process_
       << endl;
#endif  // #if USE_PRINT_ERRO

#if USE_SAVE_MESH
  WriteMesh(malha, step_);
#endif  // #USE_SAVE_MESH

  this->error_local_process_ = 1.0;

  // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
  while (this->error_local_process_ > EPSYLON) {
    if (step_ >= 2) {
      break;
    }

    this->step_++;

    // 4.1. Aloca uma nova malha
    malha = new MeshAdaptive;
    malha->ResizeSubMeshAdaptiveByPosition(sizePatch);

    list<PointAdaptive*> novosPontos[geo->GetNumBerCurves()];

    // map<Ponto *, Ponto *> mapaPontos;

    int sizeCurvas = geo->GetNumBerCurves();

    //
    //        sizeThread = 1;
    // #pragma omp parallel num_threads(sizeThread) shared(geo, sizeCurvas,
    // sizePatch, malha, novosPontos)
    //        {
    //            Int id = comm->threadId();
    //            //           this->idManagers[id] =
    //            this->makeIdManagerOmp(comm, id);

    //            // 4.2. Adapta as curvas pela curvatura da curva
    // #pragma omp for firstprivate(ptr_aux)
    //            for (int i = 0; i < sizeCurvas; ++i )
    //            {
    //                novosPontos[i] =
    //                AdaptadorPorCurvatura::adaptaCurvaByCurvaOmp(
    //                geo->getCurva( i ), this->idManagers[id], 1);
    //                geo->getCurva(i)->setPontos(novosPontos[i]);
    //                novosPontos[i] =
    //                AdaptadorPorCurvatura::adaptaCurvaBySuperficieOmp(
    //                geo->getCurva( i ), this->idManagers[id], 1);
    //                geo->getCurva(i)->setPontos(novosPontos[i]);
    //                ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
    //            }

    //        }

    // 4.2. Adapta as curvas pela curvatura da curva / 4.3. Atualiza a
    // discretização das curvas
    map<PointAdaptive*, PointAdaptive*> mapaPontos;
    timer->InitTimerParallel(0, 0, 3);  // adpt. das curvas

    for (int i = 0; i < sizeCurvas; ++i) {
      novosPontos[i] = adapter.AdaptCurveByCurve(geo->GetCurve(i), mapaPontos,
                                                 this->id_managers_[0], 1);
      geo->GetCurve(i)->SetPoints(novosPontos[i]);
      novosPontos[i] = adapter.AdaptCurveBySurface(geo->GetCurve(i), mapaPontos,
                                                   this->id_managers_[0], 1);
      geo->GetCurve(i)->SetPoints(novosPontos[i]);
      // ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
    }

    timer->EndTimerParallel(0, 0, 3);  // adpt. das cruvas

#pragma omp parallel num_threads(size_thread) shared(geo, sizePatch, malha)
    {
      Int id = communicator_->threadId();
      //((Performer::RangedIdManager *)this->idManagers[id])->setMin(1,0) ;

      timer->InitTimerParallel(0, id, 4);  // adpt. do domínio

      // 4.3. Adapta as patches
#pragma omp for
      for (int i = 0; i < sizePatch; ++i) {
        PatchCoons* p = static_cast<PatchCoons*>(geo->GetPatch(i));
        SubMesh* sub = adapter.AdaptDomainOmp(p, this->id_managers_[id], 1);
        sub->SetPatch(p);
        malha->InsertSubMeshAdaptiveByPosition(sub, i);
        geo->GetPatch(i)->SetSubMesh(malha->GetSubMeshAdaptiveByPosition(i));
      }

      timer->EndTimerParallel(0, id, 4);  // adpt. do domínio
    }

    //        // 4.5. Atualiza os patches
    //        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
    //        {
    //            geo->getPatch( i )->setMalha(malha->getSubMalha( i ));
    //        }

    // 4.6. Insere a malha gerada no modelo ( que guarda todas as malhas geradas
    // )
    model.InsertMeshAdaptive(malha);

    // 4.7. Escreve um artigo "neutral file" da malha gerada

#if USE_SAVE_MESH
    WriteMesh(malha, step_);
#endif  // #USE_SAVE_MESH

    // 4.7. Calcula o erro global para a malha
    this->error_local_process_ =
        this->CalculateErrorGlobalOmp(malha, timer, 0, size_thread);

#if USE_PRINT_ERRO
    cout << "ERRO  " << this->step_ << " = " << this->error_local_process_
         << endl;
#endif  // #if USE_PRINT_COMENT
  }

  return 0;
}

void GeneratorAdaptive::AdaptDomainOmp(Geometry* geo, MeshAdaptive* malha,
                                       Timer* timer, int sizeThread,
                                       int sizePatch) {
#pragma omp parallel num_threads(sizeThread) shared(geo, sizePatch, malha)
  {
    Int id = communicator_->threadId();

#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, id, 4);  // Adaptação do domínio
#else
    timer->InitTimerParallel(0, id, 4);  // Adaptação do domínio
#endif  // USE_MPI

    // 4.3. Adapta as patches
#pragma omp for
    for (int i = 0; i < sizePatch; ++i) {
      PatchCoons* p = static_cast<PatchCoons*>(geo->GetPatch(i));
      SubMesh* sub = adapter.AdaptDomainOmp(p, this->id_managers_[id], 1);
      sub->SetPatch(p);
      malha->InsertSubMeshAdaptiveByPosition(sub, i);
      geo->GetPatch(i)->SetSubMesh(malha->GetSubMeshAdaptiveByPosition(i));
    }
#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, id, 4);  // Adaptação do domínio
#else
    timer->EndTimerParallel(0, id, 4);   // Adaptação do domínio
#endif  // USE_MPI
  }
}

#endif  // USE_OPENMP

SubMesh* GeneratorAdaptive::InitialMesh(PatchCoons* patch,
                                        Performer::IdManager* idManager) {
  CurveAdaptive* c1 = patch->GetCurve(0);
  CurveAdaptive* c2 = patch->GetCurve(1);
  CurveAdaptive* c3 = patch->GetCurve(2);
  CurveAdaptive* c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = nullptr;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = nullptr;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = nullptr;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = nullptr;  // c4 já foi trabalhada no patch vizinho

  SubMesh* sub = new SubMesh;

  //========================= Malha Grosseira
  //====================================
  // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região
  for (double v = 0.0; v <= 1.0; v += 1) {
    for (double u = 0.0; u <= 1.0; u += 1) {
      //			cout << "u = " << u << " v = " << v << endl;
      PointAdaptive* p = new NodeAdaptive(patch->Parameterize(u, v));
      p->SetId(idManager->next(0));

      //			cout << "ponto " << p->id << " " <<  p->x << " "
      //<< p->y << " " << p->z << endl; 			cout << "===="
      //<< endl;

      if (v == 0 && c1)  // p está na curva 1 (c1 = nullptr)
        c1->InsertPoint(p);
      else if (v == 1 && c3)  // p está na curva 3
        c3->InsertPoint(p);

      if (u == 0 && c4)  // p está na curva 4
        c4->InsertPoint(p);
      else if (u == 1 && c2)  // p está na curva 2
        c2->InsertPoint(p);

      sub->SetNoh(static_cast<NodeAdaptive*>(p));
    }
  }

  PointAdaptive* p = new NodeAdaptive(patch->Parameterize(0.5, 0.5));
  sub->SetNoh(static_cast<NodeAdaptive*>(p));
  p->SetId(idManager->next(0));

  ElementAdaptive* e1 =
      new TriangleAdaptive(sub->GetNoh(0), sub->GetNoh(1), sub->GetNoh(4));
  ((TriangleAdaptive*)e1)->SetParametersN1(make_tuple(0, 0));
  ((TriangleAdaptive*)e1)->SetParametersN2(make_tuple(1, 0));
  ((TriangleAdaptive*)e1)->SetParametersN3(make_tuple(0.5, 0.5));
  e1->SetId(idManager->next(1));
  sub->SetElement(e1);

  ElementAdaptive* e2 =
      new TriangleAdaptive(sub->GetNoh(1), sub->GetNoh(3), sub->GetNoh(4));
  ((TriangleAdaptive*)e2)->SetParametersN1(make_tuple(1, 0));
  ((TriangleAdaptive*)e2)->SetParametersN2(make_tuple(1, 1));
  ((TriangleAdaptive*)e2)->SetParametersN3(make_tuple(0.5, 0.5));
  e2->SetId(idManager->next(1));
  sub->SetElement(e2);

  ElementAdaptive* e3 =
      new TriangleAdaptive(sub->GetNoh(3), sub->GetNoh(2), sub->GetNoh(4));
  ((TriangleAdaptive*)e3)->SetParametersN1(make_tuple(1, 1));
  ((TriangleAdaptive*)e3)->SetParametersN2(make_tuple(0, 1));
  ((TriangleAdaptive*)e3)->SetParametersN3(make_tuple(0.5, 0.5));
  e3->SetId(idManager->next(1));
  sub->SetElement(e3);

  ElementAdaptive* e4 =
      new TriangleAdaptive(sub->GetNoh(2), sub->GetNoh(0), sub->GetNoh(4));
  ((TriangleAdaptive*)e4)->SetParametersN1(make_tuple(0, 1));
  ((TriangleAdaptive*)e4)->SetParametersN2(make_tuple(0, 0));
  ((TriangleAdaptive*)e4)->SetParametersN3(make_tuple(0.5, 0.5));
  e4->SetId(idManager->next(1));
  sub->SetElement(e4);
  //==============================================================================*/

  // 5. define a submalha do patch
  patch->SetSubMesh(sub);
  sub->SetPatch(patch);

  return sub;
}

// gera a malha inicial e insere na lista de malhas do modelo
// a lista de pontos da curva é preenchida durante a geração da malha inicial

// calcula o erro global da malha
double GeneratorAdaptive::ErrorGlobal(MeshAdaptive* malha, Timer* timer,
                                      int rank, int sizeThread) {
  unsigned int Ns = 0;  // número de submalhas
  unsigned int Nv = 0;  // número de vértices
  double Njs = 0;       // erro global da submalha
  double curvPower = 0.0;
  double Nj = 0;  // erro global da malha
  SubMesh* sub = 0;

  Ns = malha->GetNumberSubMeshesAdaptive();

#if USE_MPI
  timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                           7);  // calculo do erro global
#else
  timer->InitTimerParallel(0, omp_get_thread_num(),
                           7);  // calculo do erro global
#endif  // USE_MPI

  // Calcula o erro global de cada submalha
  for (unsigned int i = 0; i < Ns; ++i) {
    sub = malha->GetSubMeshAdaptiveByPosition(i);
    Nv = sub->GetNumberNos();
    Njs = 0.0;
    curvPower = 0.0;

    // Calcula o erro relativo para cada nó e soma a Nj
    for (unsigned int j = 0; j < Nv; ++j) {
#if USE_MPI
      timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(),
                              7);  // calculo do erro global
      timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                               6);  // MediaGauss
#else
      timer->EndTimerParallel(0, omp_get_thread_num(),
                              7);  // calculo do erro global
      timer->InitTimerParallel(0, omp_get_thread_num(), 6);  // MediaGauss
#endif  // USE_MPI

      PointAdaptive* n = sub->GetNoh(j);
      Patch* p = sub->GetPatch();
      CurvatureAnalytical ka(*(static_cast<NodeAdaptive*>(n)),
                             *(static_cast<PatchCoons*>(p)));
      CurvatureDiscrete kd(*(static_cast<NodeAdaptive*>(n)));
      double Ga = ka.CalculateGaussCurvature();
      double Gd = kd.CalculateGaussCurvature();
      double Ha = ka.CalculateMeanCurvature();
      double Hd = kd.CalculateMeanCurvature();
      // atualiza as curvaturas do nó ( para que não sejam recalculadas na
      // adaptação das curvas e do domínio )
      ((NodeAdaptive*)n)->SetGa(Ga);
      ((NodeAdaptive*)n)->SetGd(Gd);
      ((NodeAdaptive*)n)->SetHa(Ha);
      ((NodeAdaptive*)n)->SetHd(Hd);
#if USE_MPI
      timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(), 6);  // MediaGauss
      timer->InitTimerParallel(RANK_MPI, omp_get_thread_num(),
                               7);  // calculo do erro global
#else
      timer->EndTimerParallel(0, omp_get_thread_num(), 6);   // MediaGauss
      timer->InitTimerParallel(0, omp_get_thread_num(),
                               7);  // calculo do erro global
#endif  // USE_MPI
      double power = 0.0;
      double diff = 0.0;

      if (fabs(Ga) >= TOLERANCIA) {
        diff = Gd - Ga;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ga, 2);
      } else if (fabs(Ha) >= TOLERANCIA) {
        diff = Hd - Ha;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ha, 2);
      }
    }

    if (Njs > 0.0 && curvPower > 0.0) {
      Njs = (double)sqrt(Njs / curvPower) / Nv;
    }

    Nj += Njs;
  }

  Nj /= Ns;  // o erro global é a média do erro das submalhas

#if USE_MPI
  timer->EndTimerParallel(RANK_MPI, omp_get_thread_num(),
                          7);  // calculo do erro global
#else
  timer->EndTimerParallel(0, omp_get_thread_num(), 7);  // calculo do erro
                                                        // global
#endif  // USE_MPI

  return Nj;
}

Performer::IdManager* GeneratorAdaptive::MakeIdManager(
    const Parallel::TMCommunicator* comm, Int id) const {
  UInt numProcs = comm->numProcesses();
  UInt rank = comm->rank();

  ULInt procOffset = rank * this->id_range_;

  this->id_off_set_ = numProcs * this->id_range_;
  ULInt tidrange = this->id_range_ / comm->getMaxThreads();

  Performer::RangedIdManager* manager =
      new Performer::RangedIdManager(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0,
                  /*this->idManager->getId(0)*/ 0 + procOffset + threadOffset);
  manager->setMin(1,
                  /*this->idManager->getId(1)*/ 0 + procOffset + threadOffset);

  return manager;
}

Performer::IdManager* GeneratorAdaptive::MakeIdManagerOmp(
    const Parallel::TMCommunicator* comm, Int id) const {
  Int iNoh, iElemet;
  if (this->id_managers_[id]) {
    iNoh = this->id_managers_[id]->getId(0);
    iElemet = this->id_managers_[id]->getId(1);
  } else {
    iNoh = 0;
    iElemet = 0;
  }

  UInt numProcs = comm->numProcesses();
  UInt rank = comm->rank();

  ULInt procOffset = rank * this->id_range_;

  this->id_off_set_ = numProcs * this->id_range_;
  ULInt tidrange = this->id_range_ / comm->getMaxThreads();

  Performer::RangedIdManager* manager =
      new Performer::RangedIdManager(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0, iNoh + procOffset + threadOffset);
  manager->setMin(1, iElemet + procOffset + threadOffset);

  return manager;
}

Performer::IdManager* GeneratorAdaptive::MakeIdManagerElementOmp(
    const Parallel::TMCommunicator* comm, Int id) const {
  Int iNoh;
  if (this->id_managers_[id]) {
    iNoh = this->id_managers_[id]->getId(0);
  } else {
    iNoh = 0;
  }

  UInt numProcs = comm->numProcesses();
  UInt rank = comm->rank();

  ULInt procOffset = rank * this->id_range_;

  this->id_off_set_ = numProcs * this->id_range_;
  ULInt tidrange = this->id_range_ / comm->getMaxThreads();

  Performer::RangedIdManager* manager =
      new Performer::RangedIdManager(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0, iNoh + procOffset + threadOffset);
  manager->setMin(1, 0 + procOffset + threadOffset);

  return manager;
}

void GeneratorAdaptive::SaveErrorMesh(MeshAdaptive* malha) {
  cout << "Salvando a Malha com " << malha->GetNumberSubMeshesAdaptive()
       << " subMalhas" << endl;

  unsigned int Ns = 0;  // número de submalhas
  unsigned int Nv = 0;  // número de vértices
  double Njs = 0;       // erro global da submalha
  double curvPower = 0.0;
  // double Nj = 0; // erro global da malha
  SubMesh* sub = 0;

  // Escreve arquivo com as curvaturas
  stringstream nome;
  nome << step_;
  nome << "erro";
  nome << step_;
  nome << ".log";

  ofstream arquivo(nome.str().c_str());

  Ns = malha->GetNumberSubMeshesAdaptive();

  // Calcula o erro global de cada submalha
  for (unsigned int i = 0; i < Ns; ++i) {
    sub = malha->GetSubMeshAdaptiveByPosition(i);
    Nv = sub->GetNumberNos();
    Njs = 0.0;
    curvPower = 0.0;

    // Calcula o erro relativo para cada nó e soma a Nj
    for (unsigned int j = 0; j < Nv; ++j) {
      PointAdaptive* n = sub->GetNoh(j);
      Patch* p = sub->GetPatch();
      CurvatureAnalytical ka(*(static_cast<NodeAdaptive*>(n)),
                             *(static_cast<PatchCoons*>(p)));
      CurvatureDiscrete kd(*(static_cast<NodeAdaptive*>(n)));
      double Ga = ka.CalculateGaussCurvature();
      double Gd = kd.CalculateGaussCurvature();
      double Ha = ka.CalculateMeanCurvature();
      double Hd = kd.CalculateMeanCurvature();
      // atualiza as curvaturas do nó ( para que não sejam recalculadas na
      // adaptação das curvas e do domínio )
      ((NodeAdaptive*)n)->SetGa(Ga);
      ((NodeAdaptive*)n)->SetGd(Gd);
      ((NodeAdaptive*)n)->SetHa(Ha);
      ((NodeAdaptive*)n)->SetHd(Hd);

      arquivo << "P " << n->GetId() << ": ( " << n->GetX() << ", " << n->GetY()
              << ", " << n->GetZ() << ")" << endl;
      tuple<double, double> t_n = ((PatchHermite*)p)->FindUV(*n);
      arquivo << "\tu = " << get<0>(t_n) << " v = " << get<1>(t_n) << endl;
      unsigned int num = ((NodeAdaptive*)n)->GetNumberElements();
      arquivo << "\t" << num << " elementos incidentes:";
      for (unsigned int i = 0; i < num; ++i) {
        ElementAdaptive* elem = ((NodeAdaptive*)n)->GetElement(i);
        arquivo << " T-" << elem->GetId();
      }
      arquivo << endl;
      arquivo << "\tGd = " << ((NodeAdaptive*)n)->GetGd()
              << " Ga = " << ((NodeAdaptive*)n)->GetGa() << endl;
      arquivo << "\tHd = " << ((NodeAdaptive*)n)->GetHd()
              << " Ha = " << ((NodeAdaptive*)n)->GetHa() << endl;

      double power = 0.0;
      double diff = 0.0;

      if (fabs(Ga) >= TOLERANCIA) {
        diff = Gd - Ga;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ga, 2);
        arquivo << "\tCd = " << ((NodeAdaptive*)n)->GetGd()
                << " Ca = " << ((NodeAdaptive*)n)->GetGa() << endl;
        arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
        if (fabs(diff) <= TOLERANCIA)
          arquivo << "\tdiferença menor que tolerância!!" << endl;
      } else if (fabs(Ha) >= TOLERANCIA) {
        diff = Hd - Ha;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ha, 2);
        arquivo << "\tCd = " << ((NodeAdaptive*)n)->GetHd()
                << " Ca = " << ((NodeAdaptive*)n)->GetHa() << endl;
        arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
        if (fabs(diff) <= TOLERANCIA)
          arquivo << "\tdiferença menor que tolerância!!" << endl;
      }

      arquivo << endl;
    }

    if (Njs > 0.0 && curvPower > 0.0) {
      Njs = (double)sqrt(Njs / curvPower) / Nv;
    }
  }

  arquivo.close();

  cout << "Malha salva com sucesso!!!" << endl;
}

void GeneratorAdaptive::WriteMesh(MeshAdaptive* malha, int step_) {
  stringstream nome;
  nome << step_;
  nome << "malha";
  nome << step_;
  nome << ".pos";

  ofstream arq(nome.str().c_str());

  arq << "%HEADER" << endl
      << "Arquivo gerado pelo gerador de malhas de superficie" << endl
      << endl;

  arq << "%HEADER.VERSION" << endl
      << "0-005 - Oct/93" << endl
      << endl
      << "%HEADER.ANALYSIS" << endl
      << "\'shell\'" << endl
      << endl;

  unsigned long int Nv, Nt;
  Nv = Nt = 0;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    Nv += sub->GetNumberNos();
    Nt += sub->GetNumberElements();
  }

  arq << "%NODE" << endl << Nv << endl << endl;

  arq << "%NODE.COORD" << endl << Nv << endl;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberNos(); j++) {
      NodeAdaptive* n = sub->GetNoh(j);

      arq << n->GetId() << " " << n->GetX() << " " << n->GetY() << " "
          << n->GetZ() << endl;
    }
  }

  arq << endl;

  arq << "%MATERIAL" << endl
      << "1" << endl
      << endl
      << "%MATERIAL.LABEL" << endl
      << "1" << endl
      << "1\t\'m1\'" << endl
      << endl
      << "%MATERIAL.ISOTROPIC" << endl
      << "1" << endl
      << "1\t1000.0\t0.0" << endl
      << endl
      << "%THICKNESS" << endl
      << "1" << endl
      << "1\t1.0" << endl
      << endl
      << "%INTEGRATION.ORDER" << endl
      << "1" << endl
      << "1\t3\t1\t1\t3\t1\t1" << endl
      << endl;

  arq << "%ELEMENT" << endl << Nt << endl << endl;

  arq << "%ELEMENT.T3" << endl << Nt << endl;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberElements(); j++) {
      TriangleAdaptive* t = (TriangleAdaptive*)sub->GetElement(j);

      arq << t->GetId() << " "
          << "1 1 1 " << t->GetNoh(1).GetId() << " " << t->GetNoh(2).GetId()
          << " " << t->GetNoh(3).GetId() << endl;
    }
  }

  arq << endl;
  arq << "%END";

  arq.flush();

  arq.close();

  //  cout << "escreveu o arquivo para o step_ " << step_ << endl;
}

void GeneratorAdaptive::WriteMesh(MeshAdaptive* malha, int step_,
                                  vector<double> erroPasso, int rank) {
  stringstream nome;
  if (rank == -1) {
    nome << NAME_MODEL;
    nome << "_passo_";
    nome << step_;
    nome << "_malha_";
    nome << step_;
    nome << ".pos";
  } else {
    nome << NAME_MODEL;
    nome << "_n.process_";
    nome << NUMBER_PROCESS;
    nome << "_passo_";
    nome << step_;
    nome << "_malha_";
    nome << step_;
    nome << "_rank_";
    nome << rank;
    nome << ".pos";
  }

  ofstream arq(nome.str().c_str());

  arq << "%HEADER" << endl
      << "Arquivo gerado pelo gerador de malhas de superficie" << endl;

  unsigned long int Nv, Nt;
  Nv = Nt = 0;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    Nv += sub->GetNumberNos();
    Nt += sub->GetNumberElements();
  }

  arq << "%HEADER.VERSION" << endl
      << "0-005 - Oct/93" << endl
      << endl
      << "%HEADER.ANALYSIS" << endl
      << "\'shell\'" << endl
      << endl;

  arq << "erro global em cada step_" << endl;
  int n_pas = 0;
  for (vector<double>::iterator it = erroPasso.begin(); it != erroPasso.end();
       it++) {
    arq << "step_: " << n_pas << " erro:" << (*it) << endl;
    n_pas++;
  }
  arq << endl;

  arq << "%NODE" << endl << Nv << endl << endl;

  arq << "%NODE.COORD" << endl << Nv << endl;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberNos(); j++) {
      NodeAdaptive* n = sub->GetNoh(j);

      arq << n->GetId() << " " << n->GetX() << " " << n->GetY() << " "
          << n->GetZ() << endl;
    }
  }

  arq << endl;

  arq << "%MATERIAL" << endl
      << "1" << endl
      << endl
      << "%MATERIAL.LABEL" << endl
      << "1" << endl
      << "1\t\'m1\'" << endl
      << endl
      << "%MATERIAL.ISOTROPIC" << endl
      << "1" << endl
      << "1\t1000.0\t0.0" << endl
      << endl
      << "%THICKNESS" << endl
      << "1" << endl
      << "1\t1.0" << endl
      << endl
      << "%INTEGRATION.ORDER" << endl
      << "1" << endl
      << "1\t3\t1\t1\t3\t1\t1" << endl
      << endl;

  arq << "%ELEMENT" << endl << Nt << endl << endl;

  arq << "%ELEMENT.T3" << endl << Nt << endl;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberElements(); j++) {
      TriangleAdaptive* t = (TriangleAdaptive*)sub->GetElement(j);

      arq << t->GetId() << " "
          << "1 1 1 " << t->GetNoh(1).GetId() << " " << t->GetNoh(2).GetId()
          << " " << t->GetNoh(3).GetId() << endl;
    }
  }

  arq << endl;
  arq << "%END";

  arq.flush();

  arq.close();

  cout << "escreveu o arquivo para o step_ " << step_ << endl;

  // Análise dos Elementos da Malha Gerada

  // cout<< "INIT >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
  stringstream nameFile;

  nameFile << NAME_MODEL;
  nameFile << "_n.process_";
  nameFile << NUMBER_PROCESS;
  nameFile << "_passo_";
  nameFile << step_;
  nameFile << "_qualite_";
  nameFile << step_;
  nameFile << "_rank_";
  nameFile << rank;
  nameFile << ".log";

  ofstream file(nameFile.str().c_str());

  file << "File Analise qualidade" << endl << endl;

  std::vector<double> vec_0_10;
  std::vector<double> vec_10_20;
  std::vector<double> vec_20_30;
  std::vector<double> vec_30_40;
  std::vector<double> vec_40_50;
  std::vector<double> vec_50_60;
  std::vector<double> vec_60_70;
  std::vector<double> vec_70_80;
  std::vector<double> vec_80_90;
  std::vector<double> vec_90_100;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberElements(); j++) {
      TriangleAdaptive* t = (TriangleAdaptive*)sub->GetElement(j);

      double value = t->CalculateQualityTriangle();

      if (0.0 <= value && value <= 0.1) {
        vec_0_10.push_back(value);
      } else if (0.1 < value && value <= 0.2) {
        vec_10_20.push_back(value);
      } else if (0.2 < value && value <= 0.3) {
        vec_20_30.push_back(value);
      } else if (0.3 < value && value <= 0.4) {
        vec_30_40.push_back(value);
      } else if (0.4 < value && value <= 0.5) {
        vec_40_50.push_back(value);
      } else if (0.5 < value && value <= 0.6) {
        vec_50_60.push_back(value);
      } else if (0.6 < value && value <= 0.7) {
        vec_60_70.push_back(value);
      } else if (0.7 < value && value <= 0.8) {
        vec_70_80.push_back(value);
      } else if (0.8 < value && value <= 0.9) {
        vec_80_90.push_back(value);
      } else if (0.9 < value && value <= 1) {
        vec_90_100.push_back(value);
      }
    }
  }

  file << "Quantidade elementos em cada caso de 0 a 1" << endl;
  file << vec_0_10.size() << endl;
  file << vec_10_20.size() << endl;
  file << vec_20_30.size() << endl;
  file << vec_30_40.size() << endl;
  file << vec_40_50.size() << endl;
  file << vec_50_60.size() << endl;
  file << vec_60_70.size() << endl;
  file << vec_70_80.size() << endl;
  file << vec_80_90.size() << endl;
  file << vec_90_100.size() << endl << endl;

  file << "Porcetagem elementos em cada caso de 0 a 1" << endl;

  long porc = vec_0_10.size() + vec_10_20.size() + vec_20_30.size() +
              vec_30_40.size() + vec_40_50.size() + vec_50_60.size() +
              vec_60_70.size() + vec_70_80.size() + vec_80_90.size() +
              vec_90_100.size();

  file << vec_0_10.size() * 100 / (double)porc << endl;
  file << vec_10_20.size() * 100 / (double)porc << endl;
  file << vec_20_30.size() * 100 / (double)porc << endl;
  file << vec_30_40.size() * 100 / (double)porc << endl;
  file << vec_40_50.size() * 100 / (double)porc << endl;
  file << vec_50_60.size() * 100 / (double)porc << endl;
  file << vec_60_70.size() * 100 / (double)porc << endl;
  file << vec_70_80.size() * 100 / (double)porc << endl;
  file << vec_80_90.size() * 100 / (double)porc << endl;
  file << vec_90_100.size() * 100 / (double)porc << endl;

  file.flush();

  file.close();

  // cout<< "END >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
}

void GeneratorAdaptive::WriteQualityMesh(MeshAdaptive* malha, int step_,
                                         vector<double> erroPasso, int rank) {
  // Análise dos Elementos da Malha Gerada

  // cout<< "INIT >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
  stringstream nameFile;

  nameFile << NAME_MODEL;
  nameFile << "_n.process_";
  nameFile << NUMBER_PROCESS;
  nameFile << "_passo_";
  nameFile << step_;
  nameFile << "_qualite_";
  nameFile << step_;
  nameFile << "_rank_";
  nameFile << rank;
  nameFile << ".log";

  ofstream file(nameFile.str().c_str());

  file << "File Analise qualidade" << endl << endl;

  std::vector<double> vec_0_10;
  std::vector<double> vec_10_20;
  std::vector<double> vec_20_30;
  std::vector<double> vec_30_40;
  std::vector<double> vec_40_50;
  std::vector<double> vec_50_60;
  std::vector<double> vec_60_70;
  std::vector<double> vec_70_80;
  std::vector<double> vec_80_90;
  std::vector<double> vec_90_100;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub->GetNumberElements(); j++) {
      TriangleAdaptive* t = (TriangleAdaptive*)sub->GetElement(j);

      double value = t->CalculateQualityTriangle();

      if (0.0 <= value && value <= 0.1) {
        vec_0_10.push_back(value);
      } else if (0.1 < value && value <= 0.2) {
        vec_10_20.push_back(value);
      } else if (0.2 < value && value <= 0.3) {
        vec_20_30.push_back(value);
      } else if (0.3 < value && value <= 0.4) {
        vec_30_40.push_back(value);
      } else if (0.4 < value && value <= 0.5) {
        vec_40_50.push_back(value);
      } else if (0.5 < value && value <= 0.6) {
        vec_50_60.push_back(value);
      } else if (0.6 < value && value <= 0.7) {
        vec_60_70.push_back(value);
      } else if (0.7 < value && value <= 0.8) {
        vec_70_80.push_back(value);
      } else if (0.8 < value && value <= 0.9) {
        vec_80_90.push_back(value);
      } else if (0.9 < value && value <= 1) {
        vec_90_100.push_back(value);
      }
    }
  }

  file << "Quantidade elementos em cada caso de 0 a 1" << endl;
  file << vec_0_10.size() << endl;
  file << vec_10_20.size() << endl;
  file << vec_20_30.size() << endl;
  file << vec_30_40.size() << endl;
  file << vec_40_50.size() << endl;
  file << vec_50_60.size() << endl;
  file << vec_60_70.size() << endl;
  file << vec_70_80.size() << endl;
  file << vec_80_90.size() << endl;
  file << vec_90_100.size() << endl << endl;

  file << "Porcetagem elementos em cada caso de 0 a 1" << endl;

  long porc = vec_0_10.size() + vec_10_20.size() + vec_20_30.size() +
              vec_30_40.size() + vec_40_50.size() + vec_50_60.size() +
              vec_60_70.size() + vec_70_80.size() + vec_80_90.size() +
              vec_90_100.size();

  file << vec_0_10.size() * 100 / (double)porc << endl;
  file << vec_10_20.size() * 100 / (double)porc << endl;
  file << vec_20_30.size() * 100 / (double)porc << endl;
  file << vec_30_40.size() * 100 / (double)porc << endl;
  file << vec_40_50.size() * 100 / (double)porc << endl;
  file << vec_50_60.size() * 100 / (double)porc << endl;
  file << vec_60_70.size() * 100 / (double)porc << endl;
  file << vec_70_80.size() * 100 / (double)porc << endl;
  file << vec_80_90.size() * 100 / (double)porc << endl;
  file << vec_90_100.size() * 100 / (double)porc << endl;

  file.flush();

  file.close();

  // cout<< "END >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
}

void GeneratorAdaptive::SaveMesh(MeshAdaptive* malha, int step_) {
  save_mesh_.push_back(make_pair(step_, malha));
}

void GeneratorAdaptive::SaveErrorMesh(MeshAdaptive* malha, int step_) {
  save_error_mesh_.push_back(make_pair(step_, malha));
}

void escreveElementos(int step_, SubMesh* sub, int i) {
  stringstream nome;
  nome << step_;
  nome << "submalha-";
  nome << i;
  nome << ".log";

  ofstream arq(nome.str().c_str());

  for (unsigned int k = 0; k < sub->GetNumberElements(); ++k) {
    ElementAdaptive* elem = sub->GetElement(k);

    NodeAdaptive n1(elem->GetNoh(1));
    NodeAdaptive n2(elem->GetNoh(2));
    NodeAdaptive n3(elem->GetNoh(3));

    tuple<double, double> t1 = ((TriangleAdaptive*)elem)->GetParametersN1();
    tuple<double, double> t2 = ((TriangleAdaptive*)elem)->GetParametersN2();
    tuple<double, double> t3 = ((TriangleAdaptive*)elem)->GetParametersN3();

    arq << "T-" << elem->GetId() << ":\n\t"
        << "área = " << elem->GetArea() << ";\n\t"
        << "normal = " << elem->GetVectorNormal().GetX() << ", "
        << elem->GetVectorNormal().GetY() << ", "
        << elem->GetVectorNormal().GetZ() << "\n\t"
        << "n-" << n1.GetId() << "( " << get<0>(t1) << " , " << get<1>(t1)
        << ") ângulo = " << elem->GetAngle(n1) << ";\n\t"
        << "n-" << n2.GetId() << "( " << get<0>(t2) << " , " << get<1>(t2)
        << ") ângulo = " << elem->GetAngle(n2) << ";\n\t"
        << "n-" << n3.GetId() << "( " << get<0>(t3) << " , " << get<1>(t3)
        << ") ângulo = " << elem->GetAngle(n3) << endl;
  }

  arq.flush();

  arq.close();

  cout << "escreveu o arquivo com os elementos da submalha " << i
       << " para o step_ " << step_ << endl;
}

void GeneratorAdaptive::GeneratorInitialMesh(Geometry* geo, MeshAdaptive* malha,
                                             Timer* timer, int sizeThread,
                                             int sizePatch) {
#if USE_OPENMP
#pragma omp parallel num_threads(sizeThread) shared(malha, geo, sizePatch)
  {
    Int id = communicator_->threadId();

    if (!this->id_managers_[id]) {
      this->id_managers_[id] = this->MakeIdManagerOmp(communicator_, id);
    }
#if USE_MPI
    timer->InitTimerParallel(RANK_MPI, id, 2);  // Malha inicial
#else
    timer->InitTimerParallel(0, id, 2);  // Malha inicial
#endif  // USE_MPI

    // 1. Gera a malha inicial
#pragma omp for
    for (int i = 0; i < sizePatch; ++i) {
      PatchCoons* patch = static_cast<PatchCoons*>(geo->GetPatch(i));
      SubMesh* sub = this->GeneratorInitialMeshOmp(
          static_cast<PatchCoons*>(patch), this->id_managers_[id]);
      malha->InsertSubMeshAdaptiveByPosition(sub, i);
    }

#if USE_MPI
    timer->EndTimerParallel(RANK_MPI, id, 2);  // Malha inicial
#else
    timer->EndTimerParallel(0, id, 2);   // Malha inicial
#endif  // USE_MPI
  }
#else
  for (int i = 0; i < sizePatch; ++i) {
    PatchCoons* patch = static_cast<PatchCoons*>(geo->GetPatch(i));
    SubMesh* sub = this->InitialMesh(static_cast<PatchCoons*>(patch),
                                     this->id_managers_[0]);
    malha->InsertSubMeshAdaptiveByPosition(sub, i);
  }
#endif  // USE_OPENMP
}

void GeneratorAdaptive::PrintElments(MeshAdaptive* malha, int step_,
                                     vector<double> erroPasso, int rank) {
  unsigned long int Nv, Nt;
  Nv = Nt = 0;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    SubMesh* sub = malha->GetSubMeshAdaptiveByPosition(i);

    Nv += sub->GetNumberNos();
    Nt += sub->GetNumberElements();
  }

  cout << "#elementos_" << NAME_MODEL << "_n.process_" << NUMBER_PROCESS
       << "_passo_" << step_ << "_rank_" << rank << " = " << Nt << endl;
}
