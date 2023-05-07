#include "../../include/generator/generator_adaptive.h"

#include <ostream>

#include "../../include/definitions.h"

extern int sizeMpi, rankMpi, maxThreads, kSteps;
extern double timeReadFile, kTolerance, kEpsylon;
extern std::string nameModel, numberProcess, writeMesh;
extern vector<std::shared_ptr<CurveAdaptiveParametric>> ptr_aux;

GeneratorAdaptive::GeneratorAdaptive() {}

#if USE_MPI
int GeneratorAdaptive::Execute(char* argv[], Timer& timer, MPI_Status status)
#else
int GeneratorAdaptive::Execute(char* argv[], Timer& timer)
#endif  // USE_MPI
{
  for (char** arg = argv; *arg != nullptr; ++arg) {
    std::cout << "argv[]:" << *arg << std::endl;
  }

  std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();

#if USE_MPI
  if (rankMpi == 0) {
    // estimativa e ordenação dos patches para carga de distribuiçao dos patches
    auto patches = EstimateChargeofPatches(geometry, timer);

    // distribuição dos patches entre os (n) processos de acordo com suas cargas
    // ordenadas
    patches = OrderPatchesDistribProcess(patches);

#if USE_PRINT_ESTIMATE
    // mostra a distribuição de patches para cada processo
    calculateEstimateProcessElements(sizeMpi, listBezierPt);
#endif  // USE_PRINT_ESTIMATE

    double patches_process0[patches.size() * 48];
    double patches_process[patches.size() * 48];

    int i = 0, izero = 0;

    for (int j = 0; j < sizeMpi; j++) {
      i = 0;
      for (const auto& patch : patches) {
        if (j == patch->GetIdProcess()) {
          patches_process[i] = patch->GetPt00().GetX();
          i++;
          patches_process[i] = patch->GetPt00().GetY();
          i++;
          patches_process[i] = patch->GetPt00().GetZ();
          i++;
          patches_process[i] = patch->GetPt10().GetX();
          i++;
          patches_process[i] = patch->GetPt10().GetY();
          i++;
          patches_process[i] = patch->GetPt10().GetZ();
          i++;
          patches_process[i] = patch->GetPt20().GetX();
          i++;
          patches_process[i] = patch->GetPt20().GetY();
          i++;
          patches_process[i] = patch->GetPt20().GetZ();
          i++;
          patches_process[i] = patch->GetPt30().GetX();
          i++;
          patches_process[i] = patch->GetPt30().GetY();
          i++;
          patches_process[i] = patch->GetPt30().GetZ();
          i++;

          patches_process[i] = patch->GetPt01().GetX();
          i++;
          patches_process[i] = patch->GetPt01().GetY();
          i++;
          patches_process[i] = patch->GetPt01().GetZ();
          i++;
          patches_process[i] = patch->GetPt11().GetX();
          i++;
          patches_process[i] = patch->GetPt11().GetY();
          i++;
          patches_process[i] = patch->GetPt11().GetZ();
          i++;
          patches_process[i] = patch->GetPt21().GetX();
          i++;
          patches_process[i] = patch->GetPt21().GetY();
          i++;
          patches_process[i] = patch->GetPt21().GetZ();
          i++;
          patches_process[i] = patch->GetPt31().GetX();
          i++;
          patches_process[i] = patch->GetPt31().GetY();
          i++;
          patches_process[i] = patch->GetPt31().GetZ();
          i++;

          patches_process[i] = patch->GetPt02().GetX();
          i++;
          patches_process[i] = patch->GetPt02().GetY();
          i++;
          patches_process[i] = patch->GetPt02().GetZ();
          i++;
          patches_process[i] = patch->GetPt12().GetX();
          i++;
          patches_process[i] = patch->GetPt12().GetY();
          i++;
          patches_process[i] = patch->GetPt12().GetZ();
          i++;
          patches_process[i] = patch->GetPt22().GetX();
          i++;
          patches_process[i] = patch->GetPt22().GetY();
          i++;
          patches_process[i] = patch->GetPt22().GetZ();
          i++;
          patches_process[i] = patch->GetPt32().GetX();
          i++;
          patches_process[i] = patch->GetPt32().GetY();
          i++;
          patches_process[i] = patch->GetPt32().GetZ();
          i++;

          patches_process[i] = patch->GetPt03().GetX();
          i++;
          patches_process[i] = patch->GetPt03().GetY();
          i++;
          patches_process[i] = patch->GetPt03().GetZ();
          i++;
          patches_process[i] = patch->GetPt13().GetX();
          i++;
          patches_process[i] = patch->GetPt13().GetY();
          i++;
          patches_process[i] = patch->GetPt13().GetZ();
          i++;
          patches_process[i] = patch->GetPt23().GetX();
          i++;
          patches_process[i] = patch->GetPt23().GetY();
          i++;
          patches_process[i] = patch->GetPt23().GetZ();
          i++;
          patches_process[i] = patch->GetPt33().GetX();
          i++;
          patches_process[i] = patch->GetPt33().GetY();
          i++;
          patches_process[i] = patch->GetPt33().GetZ();
          i++;
        }
      }

      if (j == 0) {
        izero = i;

        for (int m = 0; m < i; ++m) {
          patches_process0[m] = patches_process[m];
        }

      } else {
        timer.InitTimerParallel(rankMpi, 0, 0);  // Send
        MPI_Send(&timeReadFile, 1, MPI_DOUBLE, j, TAG_TIME, MPI_COMM_WORLD);
        MPI_Send(&i, 1, MPI_INT, j, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD);
        MPI_Send(&patches_process, i, MPI_DOUBLE, j, TAG_DOUBLE,
                 MPI_COMM_WORLD);
        timer.EndTimerParallel(rankMpi, 0, 0);  // Send
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
    timer.InitTimerParallel(rankMpi, 0, 9);  // Recv

    MPI_Recv(&timeReadFile, 1, MPI_DOUBLE, 0, TAG_TIME, MPI_COMM_WORLD,
             &status);

    int size_patches = 0;
    MPI_Recv(&size_patches, 1, MPI_INT, 0, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD,
             &status);

    double patches[size_patches];
    MPI_Recv(&patches, size_patches, MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD,
             &status);

    timer.EndTimerParallel(rankMpi, 0, 9);  // Recv

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
  models3d.ModelPlanBook(geometry);
  model.SetGeometry(geometry);

  //  if (argv[3]) {
  //    timer.InitTimerParallel(0, 0, 5);  // Leitura arquivo
  //    model.SetGeometry(patch_reader.ReaderFilePatches(geometry, argv[3]));
  //    timer.EndTimerParallel(0, 0, 5);  // Leitura arquivo
  //  } else {
  //    Models3d models3d;
  //    model.SetGeometry(models3d.ModelPneu(geometry));
  //  }

#if USE_OPENMP
  Generator(model, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
  Generator(model, timer, 1024);
#endif  // USE_OPENMP

#endif  // USE_MPI
  return 0;
}

#if USE_MPI
std::list<std::shared_ptr<PatchBezier>>
GeneratorAdaptive::EstimateChargeofPatches(std::shared_ptr<Geometry>& geometry,
                                           Timer& timer) {
  auto charge_estimate_process = std::make_shared<ChargeEstimateProcess>();
  std::list<std::shared_ptr<PatchBezier>> patches =
      charge_estimate_process->ChargeEstimate(geometry, timer);

  return patches;
}

std::vector<std::shared_ptr<CurveAdaptive>>
GeneratorAdaptive::CreateVectorOfCurves(
    std::list<std::shared_ptr<PatchBezier>> patches) {
  std::shared_ptr<CurveAdaptive> curve;
  std::vector<std::shared_ptr<CurveAdaptive>> curves;

  for (const auto& patch : patches) {
    if (curves.size() > 0) {
      if (VerifyCurve(patch->GetPt00(), patch->GetPt10(), patch->GetPt20(),
                      patch->GetPt30(), curves)) {
        curve = std::make_shared<CurveAdaptiveParametricBezier>(
            patch->GetPt00(), patch->GetPt10(), patch->GetPt20(),
            patch->GetPt30());
        curves.push_back(curve);
      }
      if (VerifyCurve(patch->GetPt30(), patch->GetPt31(), patch->GetPt32(),
                      patch->GetPt33(), curves)) {
        curve = std::make_shared<CurveAdaptiveParametricBezier>(
            patch->GetPt30(), patch->GetPt31(), patch->GetPt32(),
            patch->GetPt33());
        curves.push_back(curve);
      }
      if (VerifyCurve(patch->GetPt03(), patch->GetPt13(), patch->GetPt23(),
                      patch->GetPt33(), curves)) {
        curve = std::make_shared<CurveAdaptiveParametricBezier>(
            patch->GetPt03(), patch->GetPt13(), patch->GetPt23(),
            patch->GetPt33());
        curves.push_back(curve);
      }
      if (VerifyCurve(patch->GetPt00(), patch->GetPt01(), patch->GetPt02(),
                      patch->GetPt03(), curves)) {
        curve = std::make_shared<CurveAdaptiveParametricBezier>(
            patch->GetPt00(), patch->GetPt01(), patch->GetPt02(),
            patch->GetPt03());
        curves.push_back(curve);
      }
    } else if (curves.size() == 0) {
      curve = std::make_shared<CurveAdaptiveParametricBezier>(
          patch->GetPt00(), patch->GetPt10(), patch->GetPt20(),
          patch->GetPt30());
      curves.push_back(curve);
      curve = std::make_shared<CurveAdaptiveParametricBezier>(
          patch->GetPt30(), patch->GetPt31(), patch->GetPt32(),
          patch->GetPt33());
      curves.push_back(curve);
      curve = std::make_shared<CurveAdaptiveParametricBezier>(
          patch->GetPt03(), patch->GetPt13(), patch->GetPt23(),
          patch->GetPt33());
      curves.push_back(curve);
      curve = std::make_shared<CurveAdaptiveParametricBezier>(
          patch->GetPt00(), patch->GetPt01(), patch->GetPt02(),
          patch->GetPt03());
      curves.push_back(curve);
    }
  }

  return curves;
}

std::list<std::shared_ptr<PatchBezier>>
GeneratorAdaptive::OrderPatchesDistribProcess(
    std::list<std::shared_ptr<PatchBezier>>& patches) {
  std::vector<std::pair<double, int>> process_pairs;
  std::list<std::shared_ptr<PatchBezier>> patches_order;

  if (sizeMpi > 1) {
    for (int i = 0; i < sizeMpi; i++) {
      process_pairs.push_back(std::make_pair(0.0, i));
    }

    for (const auto& patch : patches) {
      std::sort(process_pairs.begin(), process_pairs.end());
      patch->SetIdProcess(process_pairs.front().second);
      process_pairs.front().first += patch->GetNumberTriangle();
    }

    for (int i = 0; i < sizeMpi; i++) {
      for (const auto& patch : patches) {
        if (patch->GetIdProcess() == i) {
          patches_order.push_back(patch);
        }
      }
    }

    return patches_order;

  } else {
    for (const auto& patch : patches) {
      patch->SetIdProcess(0);
    }

    return patches;
  }
}

bool GeneratorAdaptive::VerifyCurve(
    PointAdaptive p0, PointAdaptive p1, PointAdaptive p2, PointAdaptive p3,
    std::vector<std::shared_ptr<CurveAdaptive>> curves) {
  for (const auto& curve : curves) {
    if (std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(curve)
            ->GetPoint0()
            .
            operator==(p0) &&
        std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(curve)
            ->GetPoint1()
            .
            operator==(p1) &&
        std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(curve)
            ->GetPoint2()
            .
            operator==(p2) &&
        std::dynamic_pointer_cast<CurveAdaptiveParametricBezier>(curve)
            ->GetPoint3()
            .
            operator==(p3)) {
      return false;
    }
  }

  return true;
}

void GeneratorAdaptive::CalculateEstimateProcessElements(
    int size_process, std::list<std::shared_ptr<PatchBezier>> patches) {
  std::vector<double> estimate(size_process, 0.0);

  for (const auto& patch : patches) {
    estimate[patch->GetIdProcess()] += patch->GetNumberTriangle();
  }

  for (int i = 0; i < sizeMpi; ++i) {
    cout << "Estimativa para o processo " << i << " = " << estimate[i] << endl;
  }
}

std::list<std::shared_ptr<PatchBezier>>::iterator
GeneratorAdaptive::GetIteratorListPatches(
    int size_patches, std::list<std::shared_ptr<PatchBezier>> patches) {
  auto it = patches.begin();
  advance(it, size_patches);
  return it;
}

std::shared_ptr<Geometry> GeneratorAdaptive::UnpakGeometry(double patches[],
                                                           int size_patches) {
  auto geometry = std::make_shared<Geometry>();

  std::shared_ptr<CurveAdaptiveParametric> patch_c1;
  std::shared_ptr<CurveAdaptiveParametric> patch_c2;
  std::shared_ptr<CurveAdaptiveParametric> patch_c3;
  std::shared_ptr<CurveAdaptiveParametric> patch_c4;

  std::shared_ptr<PatchBezier> patch_bezier;

  for (int i = 0; i < size_patches; i = i + 48) {
    auto p00 = std::make_shared<VertexAdaptive>(patches[i], patches[i + 1],
                                                patches[i + 2]);
    auto p10 = std::make_shared<VertexAdaptive>(patches[i + 3], patches[i + 4],
                                                patches[i + 5]);
    auto p20 = std::make_shared<VertexAdaptive>(patches[i + 6], patches[i + 7],
                                                patches[i + 8]);
    auto p30 = std::make_shared<VertexAdaptive>(patches[i + 9], patches[i + 10],
                                                patches[i + 11]);

    auto p01 = std::make_shared<VertexAdaptive>(
        patches[i + 12], patches[i + 13], patches[i + 14]);
    auto p11 = std::make_shared<VertexAdaptive>(
        patches[i + 15], patches[i + 16], patches[i + 17]);
    auto p21 = std::make_shared<VertexAdaptive>(
        patches[i + 18], patches[i + 19], patches[i + 20]);
    auto p31 = std::make_shared<VertexAdaptive>(
        patches[i + 21], patches[i + 22], patches[i + 23]);

    auto p02 = std::make_shared<VertexAdaptive>(
        patches[i + 24], patches[i + 25], patches[i + 26]);
    auto p12 = std::make_shared<VertexAdaptive>(
        patches[i + 27], patches[i + 28], patches[i + 29]);
    auto p22 = std::make_shared<VertexAdaptive>(
        patches[i + 30], patches[i + 31], patches[i + 32]);
    auto p32 = std::make_shared<VertexAdaptive>(
        patches[i + 33], patches[i + 34], patches[i + 35]);

    auto p03 = std::make_shared<VertexAdaptive>(
        patches[i + 36], patches[i + 37], patches[i + 38]);
    auto p13 = std::make_shared<VertexAdaptive>(
        patches[i + 39], patches[i + 40], patches[i + 41]);
    auto p23 = std::make_shared<VertexAdaptive>(
        patches[i + 42], patches[i + 43], patches[i + 44]);
    auto p33 = std::make_shared<VertexAdaptive>(
        patches[i + 45], patches[i + 46], patches[i + 47]);

    patch_c1 =
        std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p10, *p20, *p30);
    patch_c2 =
        std::make_shared<CurveAdaptiveParametricBezier>(*p30, *p31, *p32, *p33);
    patch_c3 =
        std::make_shared<CurveAdaptiveParametricBezier>(*p03, *p13, *p23, *p33);
    patch_c4 =
        std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p01, *p02, *p03);

    if (geometry->VerifyCurveGeometry(*p00, *p10, *p20, *p30) == nullptr) {
      patch_c1 = std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p10,
                                                                 *p20, *p30);
      geometry->InsertCurve(patch_c1);
    } else {
      patch_c1 = geometry->VerifyCurveGeometry(*p00, *p10, *p20, *p30);
    }

    if (geometry->VerifyCurveGeometry(*p30, *p31, *p32, *p33) == nullptr) {
      patch_c2 = std::make_shared<CurveAdaptiveParametricBezier>(*p30, *p31,
                                                                 *p32, *p33);
      geometry->InsertCurve(patch_c2);
    } else {
      patch_c2 = geometry->VerifyCurveGeometry(*p30, *p31, *p32, *p33);
    }

    if (geometry->VerifyCurveGeometry(*p03, *p13, *p23, *p33) == nullptr) {
      patch_c3 = std::make_shared<CurveAdaptiveParametricBezier>(*p03, *p13,
                                                                 *p23, *p33);
      geometry->InsertCurve(patch_c3);
    } else {
      patch_c3 = geometry->VerifyCurveGeometry(*p03, *p13, *p23, *p33);
    }

    if (geometry->VerifyCurveGeometry(*p00, *p01, *p02, *p03) == nullptr) {
      patch_c4 = std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p01,
                                                                 *p02, *p03);
      geometry->InsertCurve(patch_c4);
    } else {
      patch_c4 = geometry->VerifyCurveGeometry(*p00, *p01, *p02, *p03);
    }

    patch_bezier = std::make_shared<PatchBezier>(
        patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21, *p12, *p22);
    patch_bezier->SetId(i / 48);

    geometry->InsertPatch(patch_bezier);
  }

  return geometry;
}

#endif

#if USE_MPI
void GeneratorAdaptive::Generator(double patches[], int size_patches,
                                  Timer& timer, int id_range,
                                  [[maybe_unused]] int size_rank,
                                  int size_thread)
#else
void GeneratorAdaptive::Generator(Model& model, Timer& timer, int id_range,
                                  int size_rank, int size_thread)
#endif
{
#if USE_MPI
  this->communicator_ = std::make_shared<ApMeshCommunicator>(true);
  auto geometry = UnpakGeometry(patches, size_patches);
#elif USE_OPENMP
  this->communicator_ = std::make_shared<ApMeshCommunicator>(true);
  auto geometry = model.GetGeometry();
#else
  this->communicator_ = std::make_shared<Parallel::TMCommunicator>(false);
  auto geometry = model.GetGeometry();
#endif

  int size_patch = geometry->GetNumberPatches();

  mesh_ = std::make_shared<MeshAdaptive>();
  mesh_->ResizeSubMeshAdaptiveByPosition(size_patch);

  this->id_manager_ = nullptr;
  this->id_off_set_ = 0;
  this->id_range_ = id_range;

  this->step_ = 0;

  maxThreads = size_thread =
      size_thread >
              static_cast<Parallel::TMCommunicator*>(this->communicator_.get())
                  ->getMaxThreads()
          ? static_cast<Parallel::TMCommunicator*>(this->communicator_.get())
                ->getMaxThreads()
          : size_thread;

  ptr_aux.resize(size_thread, nullptr);

  if (this->id_managers_.empty()) {
    this->id_managers_.resize(size_thread, nullptr);
  }

  // Gerar a malha inicial
#if USE_OPENMP
  GeneratorInitialMesh(geometry, mesh_, timer, size_thread, size_patch);
#else
  this->id_managers_[0] = this->MakeIdManager(communicator_, 0);
#if USE_MPI
  timer.InitTimerParallel(rankMpi, 0, 2);  // Malha inicial
#else
  timer.InitTimerParallel(0, 0, 2);    // Malha inicial
#endif  // USE_MPI

  GeneratorInitialMesh(geometry, mesh_, timer, size_thread, size_patch);

#if USE_MPI
  timer.EndTimerParallel(rankMpi, 0, 2);   // Malha inicial
#else
  timer.EndTimerParallel(0, 0, 2);     // Malha inicial
#endif  // USE_MPI
#endif  // USE_OPENMP

  // Calcula o erro global para a malha inicial
#if USE_OPENMP
  if (size_patch > 1) {
    this->error_local_process_ =
        this->CalculateErrorGlobalOmp(mesh_, timer, 0, size_thread);

  } else {
    this->error_local_process_ =
        this->ErrorGlobal(mesh_, timer, 0, size_thread);
  }
#else
#if USE_MPI
  timer.InitTimerParallel(rankMpi, 0, 7);  // Calculo do erro Global
#else
  timer.InitTimerParallel(0, 0, 7);    // Calculo do erro Global
#endif  // USE_MPI
  this->error_local_process_ = this->ErrorGlobal(mesh_, timer);
#if USE_MPI
  timer.EndTimerParallel(rankMpi, 0, 7);  // Calculo do erro Global
#else
  timer.EndTimerParallel(0, 0, 7);     // Calculo do erro Global
#endif  // USE_MPI
#endif  // USE_OPENMP

  this->error_step_.push_back(this->error_local_process_);

#if USE_SAVE_ERRO_MESH
  SaveErrorMesh(malha, step_);
#endif  // USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
  cout << "*************** ERRO " << this->step_ << " rank " << rankMpi << " = "
       << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
  cout << "*************** ERRO " << this->step_ << " = "
       << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#endif  // USE_MPI

  if (writeMesh == std::string("m") || writeMesh == std::string("q")) {
    SaveMesh(mesh_, step_);
  }

  // this->error_local_process_ = 1000;

  // Gerar malha enquanto o erro global for acima do erro desejado
  while (this->step_ < kSteps) {
    if (this->error_local_process_ < kEpsylon) {
      break;
    }
    // #if USE_MPI
    //         timer.InitTimerParallel(rankMpi,0,9); // SendRecv
    //         MPI_Allreduce(&this->error_local_process_,
    //         &this->error_local_process_, 1, MPI_DOUBLE, MPI_SUM,
    //         MPI_COMM_WORLD); timer.EndTimerParallel(rankMpi,0,9); //
    //         SendRecv this->error_local_process_ = this->error_local_process_
    //         / sizeRank;
    // #endif //USE_MPI

    this->step_++;

    // Aloca uma nova malha
    mesh_ = std::make_shared<MeshAdaptive>();
    mesh_->ResizeSubMeshAdaptiveByPosition(geometry->GetNumberPatches());
#if USE_MPI
    timer.InitTimerParallel(rankMpi, 0, 3);  // Adaptação da curva
#else
    timer.InitTimerParallel(0, 0, 3);        // Adaptação da curva
#endif  // USE_MPI

    // Adapta as curvas pela curvatura da curva / Atualiza a discretização das
    // curvas
    AdaptCurve(*geometry);

#if USE_MPI
    timer.EndTimerParallel(rankMpi, 0, 3);  // Adaptação da curva
#else
    timer.EndTimerParallel(0, 0, 3);         // Adaptação da curva
#endif  // USE_MPI

    // Adapta as patches / Atualiza os patches
#if USE_OPENMP
    AdaptDomainOmp(geometry, mesh_, timer, size_thread, size_patch);
#else
#if USE_MPI
    timer.InitTimerParallel(rankMpi, 0, 4);  // Adaptação do domínio
#else
    timer.InitTimerParallel(0, 0, 4);  // Adaptação do domínio
#endif  // USE_MPI
    AdaptDomain(*geometry, mesh_);
#if USE_MPI
    timer.EndTimerParallel(rankMpi, 0, 4);   // Adaptação do domínio
#else
    timer.EndTimerParallel(0, 0, 4);   // Adaptação do domínio
#endif  // USE_MPI
#endif  // USE_OPENMP

    // Calcula o erro global para da malha
#if USE_OPENMP
    if (size_patch > 1) {
      this->error_local_process_ =
          this->CalculateErrorGlobalOmp(mesh_, timer, 0, size_thread);
    } else {
      this->error_local_process_ =
          this->ErrorGlobal(mesh_, timer, 0, size_thread);
    }
#else
#if USE_MPI
    timer.InitTimerParallel(rankMpi, 0, 7);  // Calculo do erro Global
#else
    timer.InitTimerParallel(0, 0, 7);  // Calculo do erro Global
#endif  // USE_MPI
    this->error_local_process_ = this->ErrorGlobal(mesh_, timer);
#if USE_MPI
    timer.EndTimerParallel(rankMpi, 0, 7);  // Calculo do erro Global
#else
    timer.EndTimerParallel(0, 0, 7);   // Calculo do erro Global
#endif  // USE_MPI
#endif

    this->error_step_.push_back(this->error_local_process_);

#if USE_SAVE_ERRO_MESH
    SaveErrorMesh(malha, step_);
#endif  // USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->step_ << " rank " << rankMpi
         << " = " << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->step_ << " = "
         << this->error_local_process_ << endl;
#endif  // #if USE_PRINT_RESULTS
#endif  // USE_MPI

    if (writeMesh == std::string("m") || writeMesh == std::string("q")) {
      SaveMesh(mesh_, step_);
    }
  }

#if USE_MPI
  timer.EndTimerParallel(rankMpi, 0, 10);  // Full
  timer.PrintTime(rankMpi);
#endif  // USE_MPI

  // Escreve o(s) arquivo(s) com suas respectivas malhas em cada step_
#if USE_MPI
  for (unsigned int i = 0; i < error_step_.size(); ++i) {
    cout << "Erro do processo " << rankMpi << " no step_ " << i << " = "
         << error_step_[i] << endl;
  }

  if (writeMesh == std::string("m") || writeMesh == std::string("q")) {
    for (const auto& it : save_mesh_) {
      if (writeMesh == std::string("q")) {
        PrintElments(it.second, it.first, this->error_step_, rankMpi);
        WriteQualityMesh(it.second, it.first, this->error_step_, rankMpi);
      } else if (writeMesh == std::string("m")) {
        PrintElments(it.second, it.first, this->error_step_, rankMpi);
        WriteMesh(it.second, it.first, this->error_step_, rankMpi);
      }
    }
  }
#else

  timer.EndTimerParallel(0, 0, 10);  // Full
  timer.PrintTime();

  if (writeMesh == std::string("m") || writeMesh == std::string("q")) {
    for (const auto& mesh : save_mesh_) {
      if (writeMesh == std::string("q")) {
        PrintElments(mesh.second, mesh.first, this->error_step_);
        WriteQualityMesh(mesh.second, mesh.first, this->error_step_);
      } else {
        PrintElments(mesh.second, mesh.first, this->error_step_);
        WriteMesh(mesh.second, mesh.first, this->error_step_);
      }
    }
    for (unsigned int i = 0; i < error_step_.size(); ++i) {
      cout << "Erro no step_ " << i << " = " << error_step_[i] << endl;
    }
  }
#endif  // USE_MPI
}

void GeneratorAdaptive::AdaptCurve(Geometry& geometry) {
  std::list<std::shared_ptr<PointAdaptive>>
      new_points[geometry.GetNumberCurves()];
  map<std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive>>
      map_points;

  for (unsigned int i = 0; i < geometry.GetNumberCurves(); ++i) {
    auto curve_ptr = geometry.GetCurve(i);
    auto id_manager_ptr =
        std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
    new_points[i] =
        adapter_.AdaptCurveByCurve(curve_ptr, map_points, id_manager_ptr, 1);

    cout << "AdaptCurveByCurve curva: " << i
         << " newpoint[i] size: " << new_points[i].size() << endl;
    geometry.GetCurve(i)->SetPoints(new_points[i]);
    //    new_points[i] =
    //        adapter_.AdaptCurveBySurface(curve_ptr, map_points,
    //        id_manager_ptr, 1);
    //    geometry.GetCurve(i)->SetPoints(new_points[i]);
    cout << "AdaptCurveBySurface curva: " << i
         << " newpoint[i] size: " << new_points[i].size() << endl;
  }
}

void GeneratorAdaptive::AdaptDomain(Geometry& geometry,
                                    std::shared_ptr<MeshAdaptive>& mesh) {
  for (unsigned int i = 0; i < geometry.GetNumberPatches(); ++i) {
    auto patch = static_pointer_cast<PatchCoons>(geometry.GetPatch(i));
    auto id_manager_ptr =
        std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
    auto new_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
    auto sub_mesh = adapter_.AdaptDomain(patch, id_manager_ptr, 1);
    sub_mesh->SetPatch(patch);
    mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
    geometry.GetPatch(i)->SetSubMesh(new_mesh);
  }
}

#if USE_OPENMP
std::shared_ptr<SubMesh> GeneratorAdaptive::GeneratorInitialMeshOmp(
    std::shared_ptr<PatchCoons>& patch,
    std::shared_ptr<Performer::IdManager>& id_manager) {
  auto c1 = patch->GetCurve(0);
  auto c2 = patch->GetCurve(1);
  auto c3 = patch->GetCurve(2);
  auto c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = nullptr;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = nullptr;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = nullptr;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = nullptr;  // c4 já foi trabalhada no patch vizinho

  auto sub_mesh = std::make_shared<SubMesh>();

  //========================= Malha Grosseira
  //====================================
  // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região

  for (double v = 0.0; v <= 1.0; v += 1) {
    for (double u = 0.0; u <= 1.0; u += 1) {
      auto point = std::make_shared<NodeAdaptive>(patch->Parameterize(u, v));
      point->SetId(id_manager->next(0));

      if (v == 0 && c1)  // p está na curva 1 (c1 = nullptr)
        c1->InsertPoint(point);
      else if (v == 1 && c3)  // p está na curva 3
        c3->InsertPoint(point);

      if (u == 0 && c4)  // p está na curva 4
        c4->InsertPoint(point);
      else if (u == 1 && c2)  // p está na curva 2
        c2->InsertPoint(point);

      auto noh = static_pointer_cast<NodeAdaptive>(point);
      sub_mesh->SetNoh(noh);
    }
  }

  auto point = std::make_shared<NodeAdaptive>(patch->Parameterize(0.5, 0.5));
  point->SetId(id_manager->next(0));
  auto noh = static_pointer_cast<NodeAdaptive>(point);
  sub_mesh->SetNoh(noh);

  auto e1 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(0), sub_mesh->GetNoh(1), sub_mesh->GetNoh(4));
  e1->SetParametersN1(make_tuple(0, 0));
  e1->SetParametersN2(make_tuple(1, 0));
  e1->SetParametersN3(make_tuple(0.5, 0.5));
  e1->SetId(id_manager->next(1));
  sub_mesh->SetElement(e1);

  auto e2 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(1), sub_mesh->GetNoh(3), sub_mesh->GetNoh(4));
  e2->SetParametersN1(make_tuple(1, 0));
  e2->SetParametersN2(make_tuple(1, 1));
  e2->SetParametersN3(make_tuple(0.5, 0.5));
  e2->SetId(id_manager->next(1));
  sub_mesh->SetElement(e2);

  auto e3 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(3), sub_mesh->GetNoh(2), sub_mesh->GetNoh(4));
  e3->SetParametersN1(make_tuple(1, 1));
  e3->SetParametersN2(make_tuple(0, 1));
  e3->SetParametersN3(make_tuple(0.5, 0.5));
  e3->SetId(id_manager->next(1));
  sub_mesh->SetElement(e3);

  auto e4 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(2), sub_mesh->GetNoh(0), sub_mesh->GetNoh(4));
  e4->SetParametersN1(make_tuple(0, 1));
  e4->SetParametersN2(make_tuple(0, 0));
  e4->SetParametersN3(make_tuple(0.5, 0.5));
  e4->SetId(id_manager->next(1));
  sub_mesh->SetElement(e4);
  //==============================================================================*/

  // 5. define a submalha do patch
  patch->SetSubMesh(sub_mesh);
  sub_mesh->SetPatch(patch);

  return sub_mesh;
}

double GeneratorAdaptive::CalculateErrorGlobalOmp(
    std::shared_ptr<MeshAdaptive>& mesh, Timer& timer,
    [[maybe_unused]] int rank, int size_thread) {
  unsigned int Ns = 0;  // número de submalhas
  double Nj = 0.0;      // erro global da malha

  Ns = mesh->GetNumberSubMeshesAdaptive();

  // Calcula o erro global de cada submalha (OMP)
#pragma omp parallel num_threads(size_thread) firstprivate(Ns) reduction(+ : Nj)
  {
#if USE_MPI
    timer.InitTimerParallel(rankMpi, omp_get_thread_num(),
                            7);  // calculo do erro global
#else
    timer.InitTimerParallel(0, omp_get_thread_num(),
                            7);  // calculo do erro global
#endif  // USE_MPI
#pragma omp for
    for (unsigned int i = 0; i < Ns; ++i) {
      auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
      unsigned int Nv = sub_mesh->GetNumberNos();
      double curvPower = 0.0;
      double Njs = 0.0;

      // Calcula o erro relativo para cada nó e soma a Nj
      // #pragma omp parallel for num_threads(nThreads) firstprivate(Ns)
      // reduction(+ :Nj)
      for (unsigned int j = 0; j < Nv; ++j) {
#if USE_MPI
        timer.EndTimerParallel(rankMpi, omp_get_thread_num(),
                               7);  // calculo do erro global
        timer.InitTimerParallel(rankMpi, omp_get_thread_num(),
                                6);  // MediaGauss
#else
        timer.EndTimerParallel(0, omp_get_thread_num(),
                               7);  // calculo do erro global
        timer.InitTimerParallel(0, omp_get_thread_num(), 6);  // MediaGauss
#endif  // USE_MPI

        auto noh = sub_mesh->GetNoh(j);
        auto patch = sub_mesh->GetPatch();
        CurvatureAnalytical ka(
            *std::static_pointer_cast<PointAdaptive>(noh).get(),
            *std::static_pointer_cast<PatchCoons>(patch).get());
        CurvatureDiscrete kd(*noh.get());
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
        noh->SetGa(Ga);
        noh->SetGd(Gd);
        noh->SetHa(Ha);
        noh->SetHd(Hd);

#if USE_MPI
        timer.EndTimerParallel(rankMpi, omp_get_thread_num(),
                               6);  // MediaGauss
        timer.InitTimerParallel(rankMpi, omp_get_thread_num(),
                                7);  // calculo do erro global
#else
        timer.EndTimerParallel(0, omp_get_thread_num(), 6);   // MediaGauss
        timer.InitTimerParallel(0, omp_get_thread_num(),
                                7);  // calculo do erro global
#endif  // USE_MPI

        double power = 0.0;
        double diff = 0.0;

        if (fabs(Ga) >= kTolerance) {
          diff = Gd - Ga;
          power = pow(diff, 2);
          Njs += power;
          curvPower += pow(Ga, 2);
        } else if (fabs(Ha) >= kTolerance) {
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
    timer.EndTimerParallel(rankMpi, omp_get_thread_num(),
                           7);  // calculo do erro global
#else
    timer.EndTimerParallel(0, omp_get_thread_num(),
                           7);          // calculo do erro global
#endif  // USE_MPI
  }

  Nj /= Ns;  // o erro global é a média do erro das submalhas

  return Nj;
}

int GeneratorAdaptive::GeneratorOmp(Model& model, Timer& timer, int id_range,
                                    [[maybe_unused]] int size_rank,
                                    int size_thread)

{
  this->id_manager_ = nullptr;
  this->id_off_set_ = 0;
  this->id_range_ = id_range;
  this->communicator_ = std::make_shared<ApMeshCommunicator>(true);

#if USE_MPI
  [[maybe_unused]] Int nProcesses = 1;
  [[maybe_unused]] Int rank = 0;
  nProcesses = this->communicator_->numProcesses();
  rank = this->communicator_->rank();
#endif  // #if USE_MPI

  auto geometry = model.GetGeometry();
  int sizePatch = geometry->GetNumberPatches();

  auto mesh = std::make_shared<MeshAdaptive>();
  mesh->ResizeSubMeshAdaptiveByPosition(sizePatch);

  this->step_ = 0;

  // sizeThread = static_cast<Parallel::TMCommunicator
  // *>(this->comm)->getMaxThreads();

  ptr_aux.resize(size_thread, nullptr);

  if (this->id_managers_.empty()) {
    this->id_managers_.resize(size_thread, nullptr);
  }

#pragma omp parallel num_threads(size_thread) shared(mesh, geometry, sizePatch)
  {
    Int id = communicator_->threadId();

    if (!this->id_managers_[id]) {
      this->id_managers_[id] = this->MakeIdManagerOmp(communicator_, id).get();
    }

    timer.InitTimerParallel(0, id, 2);  // Malha inicial

    // 1. Gera a malha inicial
#pragma omp for
    for (int i = 0; i < sizePatch; ++i) {
      auto patch = static_pointer_cast<PatchCoons>(geometry->GetPatch(i));
      auto id_manager_ptr =
          std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
      auto sub = this->GeneratorInitialMeshOmp(patch, id_manager_ptr);
      mesh->InsertSubMeshAdaptiveByPosition(sub, i);
    }

    timer.EndTimerParallel(0, id, 2);  // Malha inicial
  }

  // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
  model.InsertMeshAdaptive(mesh);

  // 3. Calcula o erro global para a malha inicial
  this->error_local_process_ =
      this->CalculateErrorGlobalOmp(mesh, timer, 0, size_thread);

#if USE_PRINT_ERRO
  cout << "ERRO  " << this->step_ << " = " << this->error_local_process_
       << endl;
#endif  // #if USE_PRINT_ERRO

#if USE_SAVE_MESH
  WriteMesh(mesh, step_);
#endif  // #USE_SAVE_MESH

  this->error_local_process_ = 1.0;

  // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
  while (this->error_local_process_ > kEpsylon) {
    if (step_ >= 2) {
      break;
    }

    this->step_++;

    // 4.1. Aloca uma nova malha
    mesh = std::make_shared<MeshAdaptive>();
    mesh->ResizeSubMeshAdaptiveByPosition(sizePatch);

    std::list<std::shared_ptr<PointAdaptive>>
        new_points[geometry->GetNumberCurves()];

    // map<Ponto *, Ponto *> mapaPontos;

    int sizeCurvas = geometry->GetNumberCurves();

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
    map<std::shared_ptr<PointAdaptive>, std::shared_ptr<PointAdaptive>>
        map_points;
    timer.InitTimerParallel(0, 0, 3);  // adpt. das curvas

    for (int i = 0; i < sizeCurvas; ++i) {
      auto curve_ptr = std::static_pointer_cast<CurveAdaptiveParametric>(
          geometry->GetCurve(i));
      auto id_manager_ptr =
          std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
      new_points[i] =
          adapter_.AdaptCurveByCurve(curve_ptr, map_points, id_manager_ptr, 1);
      geometry->GetCurve(i)->SetPoints(new_points[i]);
      new_points[i] = adapter_.AdaptCurveBySurface(curve_ptr, map_points,
                                                   id_manager_ptr, 1);
      geometry->GetCurve(i)->SetPoints(new_points[i]);
      // ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
    }

    timer.EndTimerParallel(0, 0, 3);  // adpt. das cruvas

#pragma omp parallel num_threads(size_thread) shared(geometry, sizePatch, mesh)
    {
      Int id = communicator_->threadId();
      //((Performer::RangedIdManager *)this->idManagers[id])->setMin(1,0) ;

      timer.InitTimerParallel(0, id, 4);  // adpt. do domínio

      // 4.3. Adapta as patches
#pragma omp for
      for (int i = 0; i < sizePatch; ++i) {
        auto patch = static_pointer_cast<PatchCoons>(geometry->GetPatch(i));
        auto id_manager_ptr =
            std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
        auto sub_mesh = adapter_.AdaptDomainOmp(patch, id_manager_ptr, 1);
        sub_mesh->SetPatch(patch);
        mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
        auto new_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
        geometry->GetPatch(i)->SetSubMesh(new_mesh);
      }

      timer.EndTimerParallel(0, id, 4);  // adpt. do domínio
    }

    //        // 4.5. Atualiza os patches
    //        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
    //        {
    //            geo->getPatch( i )->setMalha(malha->getSubMalha( i ));
    //        }

    // 4.6. Insere a malha gerada no modelo ( que guarda todas as malhas geradas
    // )
    model.InsertMeshAdaptive(mesh);

    // 4.7. Escreve um artigo "neutral file" da malha gerada

#if USE_SAVE_MESH
    WriteMesh(malha, step_);
#endif  // #USE_SAVE_MESH

    // 4.7. Calcula o erro global para a malha
    this->error_local_process_ =
        this->CalculateErrorGlobalOmp(mesh, timer, 0, size_thread);

#if USE_PRINT_ERRO
    cout << "ERRO  " << this->step_ << " = " << this->error_local_process_
         << endl;
#endif  // #if USE_PRINT_COMENT
  }

  return 0;
}

void GeneratorAdaptive::AdaptDomainOmp(std::shared_ptr<Geometry>& geometry,
                                       std::shared_ptr<MeshAdaptive>& mesh,
                                       Timer& timer, int sizeThread,
                                       int sizePatch) {
#pragma omp parallel num_threads(sizeThread) shared(geometry, sizePatch, mesh)
  {
    Int id = communicator_->threadId();

#if USE_MPI
    timer.InitTimerParallel(rankMpi, id, 4);  // Adaptação do domínio
#else
    timer.InitTimerParallel(0, id, 4);  // Adaptação do domínio
#endif  // USE_MPI

    // 4.3. Adapta as patches
#pragma omp for
    for (int i = 0; i < sizePatch; ++i) {
      auto patch = static_pointer_cast<PatchCoons>(geometry->GetPatch(i));
      auto id_manager_ptr =
          std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
      auto sub_mesh = adapter_.AdaptDomainOmp(patch, id_manager_ptr, 1);
      sub_mesh->SetPatch(patch);
      mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
      auto new_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
      geometry->GetPatch(i)->SetSubMesh(new_mesh);
    }
#if USE_MPI
    timer.EndTimerParallel(rankMpi, id, 4);  // Adaptação do domínio
#else
    timer.EndTimerParallel(0, id, 4);   // Adaptação do domínio
#endif  // USE_MPI
  }
}

#endif  // USE_OPENMP

std::shared_ptr<SubMesh> GeneratorAdaptive::InitialMesh(
    std::shared_ptr<PatchCoons>& patch,
    std::shared_ptr<Performer::IdManager>& idManager) {
  auto c1 = patch->GetCurve(0);
  auto c2 = patch->GetCurve(1);
  auto c3 = patch->GetCurve(2);
  auto c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = nullptr;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = nullptr;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = nullptr;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = nullptr;  // c4 já foi trabalhada no patch vizinho

  auto sub_mesh = std::make_shared<SubMesh>();

  //========================= Malha Grosseira
  //====================================
  // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região
  for (double v = 0.0; v <= 1.0; v += 1) {
    for (double u = 0.0; u <= 1.0; u += 1) {
      //			cout << "u = " << u << " v = " << v << endl;
      auto point = std::make_shared<NodeAdaptive>(patch->Parameterize(u, v));
      point->SetId(idManager->next(0));

      //			cout << "ponto " << p->id << " " <<  p->x << " "
      //<< p->y << " " << p->z << endl; 			cout << "===="
      //<< endl;

      if (v == 0 && c1)  // p está na curva 1 (c1 = nullptr)
        c1->InsertPoint(point);
      else if (v == 1 && c3)  // p está na curva 3
        c3->InsertPoint(point);

      if (u == 0 && c4)  // p está na curva 4
        c4->InsertPoint(point);
      else if (u == 1 && c2)  // p está na curva 2
        c2->InsertPoint(point);
      auto noh = static_pointer_cast<NodeAdaptive>(point);
      sub_mesh->SetNoh(noh);
    }
  }

  auto point = std::make_shared<NodeAdaptive>(patch->Parameterize(0.5, 0.5));
  auto noh = static_pointer_cast<NodeAdaptive>(point);
  sub_mesh->SetNoh(noh);
  point->SetId(idManager->next(0));

  auto e1 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(0), sub_mesh->GetNoh(1), sub_mesh->GetNoh(4));
  e1->SetParametersN1(make_tuple(0, 0));
  e1->SetParametersN2(make_tuple(1, 0));
  e1->SetParametersN3(make_tuple(0.5, 0.5));
  e1->SetId(idManager->next(1));
  sub_mesh->SetElement(e1);

  auto e2 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(1), sub_mesh->GetNoh(3), sub_mesh->GetNoh(4));
  e2->SetParametersN1(make_tuple(1, 0));
  e2->SetParametersN2(make_tuple(1, 1));
  e2->SetParametersN3(make_tuple(0.5, 0.5));
  e2->SetId(idManager->next(1));
  sub_mesh->SetElement(e2);

  auto e3 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(3), sub_mesh->GetNoh(2), sub_mesh->GetNoh(4));
  e3->SetParametersN1(make_tuple(1, 1));
  e3->SetParametersN2(make_tuple(0, 1));
  e3->SetParametersN3(make_tuple(0.5, 0.5));
  e3->SetId(idManager->next(1));
  sub_mesh->SetElement(e3);

  auto e4 = std::make_shared<TriangleAdaptive>(
      sub_mesh->GetNoh(2), sub_mesh->GetNoh(0), sub_mesh->GetNoh(4));
  e4->SetParametersN1(make_tuple(0, 1));
  e4->SetParametersN2(make_tuple(0, 0));
  e4->SetParametersN3(make_tuple(0.5, 0.5));
  e4->SetId(idManager->next(1));
  sub_mesh->SetElement(e4);
  //==============================================================================*/

  // 5. define a submalha do patch
  patch->SetSubMesh(sub_mesh);
  sub_mesh->SetPatch(patch);

  return sub_mesh;
}

// gera a malha inicial e insere na lista de malhas do modelo
// a lista de pontos da curva é preenchida durante a geração da malha inicial

// calcula o erro global da malha
double GeneratorAdaptive::ErrorGlobal(std::shared_ptr<MeshAdaptive>& mesh,
                                      Timer& timer, [[maybe_unused]] int rank,
                                      [[maybe_unused]] int sizeThread) {
  [[maybe_unused]] unsigned int Ns = 0;  // número de submalhas
  [[maybe_unused]] unsigned int Nv = 0;  // número de vértices
  [[maybe_unused]] double Njs = 0;       // erro global da submalha
  [[maybe_unused]] double curvPower = 0.0;
  double Nj = 0;  // erro global da malha
  auto sub_mesh = std::make_shared<SubMesh>();

  Ns = mesh->GetNumberSubMeshesAdaptive();

#if USE_MPI
#if USE_OPENMP
  // Seção de código para MPI + OpenMP
  // calculo do erro global
  timer.InitTimerParallel(rankMpi, omp_get_thread_num(), 7);
#else
  // Seção de código para MPI sem OpenMP
  // calculo do erro global
  timer.InitTimerParallel(rankMpi, 0, 7);
#endif
#else
#if USE_OPENMP
  // Seção de código para OpenMP sem MPI
  // calculo do erro global
  timer.InitTimerParallel(0, omp_get_thread_num(), 7);
#else
  // Seção de código sem MPI e sem OpenMP
  // calculo do erro global
  timer.InitTimerParallel(0, 0, 7);
#endif
#endif

  // Calcula o erro global de cada submalha
  for (unsigned int i = 0; i < Ns; ++i) {
    sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
    Nv = sub_mesh->GetNumberNos();
    Njs = 0.0;
    curvPower = 0.0;

    // Calcula o erro relativo para cada nó e soma a Nj
    for (unsigned int j = 0; j < Nv; ++j) {
#if USE_MPI
#if USE_OPENMP
      // Seção de código para MPI + OpenMP
      // calculo do erro global
      timer.EndTimerParallel(rankMpi, omp_get_thread_num(), 7);
      // MediaGauss
      timer.InitTimerParallel(rankMpi, omp_get_thread_num(), 6);
#else
      // Seção de código para MPI sem OpenMP
      // calculo do erro global
      timer.EndTimerParallel(rankMpi, 0, 7);
      // MediaGauss
      timer.InitTimerParallel(rankMpi, 0, 6);
#endif
#else
#if USE_OPENMP
      // Seção de código para OpenMP sem MPI
      // calculo do erro global
      timer.EndTimerParallel(0, omp_get_thread_num(), 7);
      // MediaGauss
      timer.InitTimerParallel(0, omp_get_thread_num(), 6);
#else
      // Seção de código sem MPI e sem OpenMP
      // calculo do erro global
      timer.EndTimerParallel(0, 0, 7);
      // MediaGauss
      timer.InitTimerParallel(0, 0, 6);
#endif
#endif

      auto noh = sub_mesh->GetNoh(j);
      auto patch = sub_mesh->GetPatch();
      CurvatureAnalytical ka(
          *std::static_pointer_cast<PointAdaptive>(noh).get(),
          *std::static_pointer_cast<PatchCoons>(patch).get());
      CurvatureDiscrete kd(*noh.get());
      double Ga = ka.CalculateGaussCurvature();
      double Gd = kd.CalculateGaussCurvature();
      double Ha = ka.CalculateMeanCurvature();
      double Hd = kd.CalculateMeanCurvature();
      // atualiza as curvaturas do nó ( para que não sejam recalculadas na
      // adaptação das curvas e do domínio )
      noh->SetGa(Ga);
      noh->SetGd(Gd);
      noh->SetHa(Ha);
      noh->SetHd(Hd);

#if USE_MPI
#if USE_OPENMP
      // Seção de código para MPI + OpenMP
      // MediaGauss
      timer.EndTimerParallel(rankMpi, omp_get_thread_num(), 6);
      // calculo do erro global
      timer.InitTimerParallel(rankMpi, omp_get_thread_num(), 7);
#else
      // Seção de código para MPI sem OpenMP
      // MediaGauss
      timer.EndTimerParallel(rankMpi, 0, 6);
      // calculo do erro global
      timer.InitTimerParallel(rankMpi, 0, 7);
#endif
#else
#if USE_OPENMP
      // Seção de código para OpenMP sem MPI
      // MediaGauss
      timer.EndTimerParallel(0, omp_get_thread_num(), 6);
      // calculo do erro global
      timer.InitTimerParallel(0, omp_get_thread_num(), 7);
#else
      // Seção de código sem MPI e sem OpenMP
      // calculo do erro global
      timer.EndTimerParallel(0, 0, 7);
      // MediaGauss
      timer.InitTimerParallel(0, 0, 6);
#endif
#endif

      double power = 0.0;
      double diff = 0.0;

      if (fabs(Ga) >= kTolerance) {
        diff = Gd - Ga;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ga, 2);
      } else if (fabs(Ha) >= kTolerance) {
        diff = Hd - Ha;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ha, 2);
      }
    }

    if (Njs > 0.0 && curvPower > 0.0) {
      Njs = static_cast<double>(sqrt(Njs / curvPower) / Nv);
    }

    Nj += Njs;
  }

  // o erro global é a média do erro das submalhas
  Nj /= Ns;

#if USE_MPI
#if USE_OPENMP
  // Seção de código para MPI + OpenMP
  // calculo do erro global
  timer.EndTimerParallel(rankMpi, omp_get_thread_num(), 7);
#else
  // Seção de código para MPI sem OpenMP
  // calculo do erro global
  timer.EndTimerParallel(rankMpi, 0, 7);
#endif
#else
#if USE_OPENMP
  // Seção de código para OpenMP sem MPI
  // calculo do erro global
  timer.EndTimerParallel(0, omp_get_thread_num(), 7);
#else
  // Seção de código sem MPI e sem OpenMP
  // calculo do erro global
  timer.EndTimerParallel(0, 0, 7);
#endif
#endif

  return Nj;
}

std::shared_ptr<Performer::IdManager> GeneratorAdaptive::MakeIdManager(
    const std::shared_ptr<Parallel::TMCommunicator>& comm, Int id) const {
  UInt numProcs = comm->numProcesses();
  UInt rank = comm->rank();

  ULInt procOffset = rank * this->id_range_;

  this->id_off_set_ = numProcs * this->id_range_;
  ULInt tidrange = this->id_range_ / comm->getMaxThreads();

  auto manager = std::make_shared<Performer::RangedIdManager>(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0,
                  /*this->idManager->getId(0)*/ 0 + procOffset + threadOffset);
  manager->setMin(1,
                  /*this->idManager->getId(1)*/ 0 + procOffset + threadOffset);

  return manager;
}

std::shared_ptr<Performer::IdManager> GeneratorAdaptive::MakeIdManagerOmp(
    const std::shared_ptr<Parallel::TMCommunicator>& comm, Int id) const {
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

  auto manager = std::make_shared<Performer::RangedIdManager>(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0, iNoh + procOffset + threadOffset);
  manager->setMin(1, iElemet + procOffset + threadOffset);

  return manager;
}

std::shared_ptr<Performer::IdManager>
GeneratorAdaptive::MakeIdManagerElementOmp(
    const std::shared_ptr<Parallel::TMCommunicator>& comm, Int id) const {
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

  auto manager = std::make_shared<Performer::RangedIdManager>(1, 1, 1, 1, 2);

  ULInt threadOffset = id * tidrange;

  manager->setRange(tidrange);
  manager->setOffset(this->id_off_set_);
  manager->setMin(0, iNoh + procOffset + threadOffset);
  manager->setMin(1, 0 + procOffset + threadOffset);

  return manager;
}

void GeneratorAdaptive::SaveErrorMesh(
    const std::shared_ptr<MeshAdaptive>& mesh) {
  cout << "Salvando a Malha com " << mesh->GetNumberSubMeshesAdaptive()
       << " subMalhas" << endl;

  unsigned int Ns = 0;  // número de submalhas
  unsigned int Nv = 0;  // número de vértices
  double Njs = 0;       // erro global da submalha
  double curvPower = 0.0;
  // double Nj = 0; // erro global da malha
  auto sub_mesh = std::make_shared<SubMesh>();

  // Escreve arquivo com as curvaturas
  stringstream nome;
  nome << step_;
  nome << "erro";
  nome << step_;
  nome << ".log";

  ofstream arquivo(nome.str().c_str());

  Ns = mesh->GetNumberSubMeshesAdaptive();

  // Calcula o erro global de cada submalha
  for (unsigned int i = 0; i < Ns; ++i) {
    sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);
    Nv = sub_mesh->GetNumberNos();
    Njs = 0.0;
    curvPower = 0.0;

    // Calcula o erro relativo para cada nó e soma a Nj
    for (unsigned int j = 0; j < Nv; ++j) {
      auto noh = sub_mesh->GetNoh(j);
      auto patch = sub_mesh->GetPatch();
      CurvatureAnalytical ka(
          *std::static_pointer_cast<PointAdaptive>(noh).get(),
          *std::static_pointer_cast<PatchCoons>(patch).get());
      CurvatureDiscrete kd(*noh.get());
      double Ga = ka.CalculateGaussCurvature();
      double Gd = kd.CalculateGaussCurvature();
      double Ha = ka.CalculateMeanCurvature();
      double Hd = kd.CalculateMeanCurvature();
      // atualiza as curvaturas do nó ( para que não sejam recalculadas na
      // adaptação das curvas e do domínio )
      noh->SetGa(Ga);
      noh->SetGd(Gd);
      noh->SetHa(Ha);
      noh->SetHd(Hd);

      arquivo << "P " << noh->GetId() << ": ( " << noh->GetX() << ", "
              << noh->GetY() << ", " << noh->GetZ() << ")" << endl;
      tuple<double, double> t_n =
          (static_pointer_cast<PatchHermite>(patch).get())->FindUV(*noh.get());
      arquivo << "\tu = " << get<0>(t_n) << " v = " << get<1>(t_n) << endl;
      unsigned int num = noh->GetNumberElements();
      arquivo << "\t" << num << " elementos incidentes:";
      for (unsigned int i = 0; i < num; ++i) {
        auto elem = noh->GetElement(i);
        arquivo << " T-" << elem->GetId();
      }
      arquivo << endl;
      arquivo << "\tGd = " << noh->GetGd() << " Ga = " << noh->GetGa() << endl;
      arquivo << "\tHd = " << noh->GetHd() << " Ha = " << noh->GetHa() << endl;

      double power = 0.0;
      double diff = 0.0;

      if (fabs(Ga) >= kTolerance) {
        diff = Gd - Ga;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ga, 2);
        arquivo << "\tCd = " << noh->GetGd() << " Ca = " << noh->GetGa()
                << endl;
        arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
        if (fabs(diff) <= kTolerance)
          arquivo << "\tdiferença menor que tolerância!!" << endl;
      } else if (fabs(Ha) >= kTolerance) {
        diff = Hd - Ha;
        power = pow(diff, 2);
        Njs += power;
        curvPower += pow(Ha, 2);
        arquivo << "\tCd = " << noh->GetHd() << " Ca = " << noh->GetHa()
                << endl;
        arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
        if (fabs(diff) <= kTolerance)
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

void GeneratorAdaptive::WriteMesh(const std::shared_ptr<MeshAdaptive>& malha,
                                  int step_) {
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
    auto sub_mesh = malha->GetSubMeshAdaptiveByPosition(i);

    Nv += sub_mesh->GetNumberNos();
    Nt += sub_mesh->GetNumberElements();
  }

  arq << "%NODE" << endl << Nv << endl << endl;

  arq << "%NODE.COORD" << endl << Nv << endl;

  for (unsigned int i = 0; i < malha->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberNos(); j++) {
      auto noh = sub_mesh->GetNoh(j);

      arq << noh->GetId() << " " << noh->GetX() << " " << noh->GetY() << " "
          << noh->GetZ() << endl;
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
    auto sub_mesh = malha->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberElements(); j++) {
      auto t =
          std::static_pointer_cast<TriangleAdaptive>(sub_mesh->GetElement(j));

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

void GeneratorAdaptive::WriteMesh(const std::shared_ptr<MeshAdaptive>& mesh,
                                  int step_, vector<double> step_error,
                                  int rank) {
  stringstream nome;
  if (rank == -1) {
    nome << nameModel;
    nome << "_passo_";
    nome << step_;
    nome << "_malha_";
    nome << step_;
    nome << ".pos";
  } else {
    nome << nameModel;
    nome << "_n.process_";
    nome << numberProcess;
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

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    Nv += sub_mesh->GetNumberNos();
    Nt += sub_mesh->GetNumberElements();
  }

  arq << "%HEADER.VERSION" << endl
      << "0-005 - Oct/93" << endl
      << endl
      << "%HEADER.ANALYSIS" << endl
      << "\'shell\'" << endl
      << endl;

  arq << "erro global em cada step_" << endl;
  int n_pas = 0;
  for (const auto& erro : step_error) {
    arq << "step_: " << n_pas << " erro:" << erro << endl;
    n_pas++;
  }
  arq << endl;

  arq << "%NODE" << endl << Nv << endl << endl;

  arq << "%NODE.COORD" << endl << Nv << endl;

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberNos(); j++) {
      auto noh = sub_mesh->GetNoh(j);

      arq << noh->GetId() << " " << noh->GetX() << " " << noh->GetY() << " "
          << noh->GetZ() << endl;
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

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberElements(); j++) {
      auto t =
          std::static_pointer_cast<TriangleAdaptive>(sub_mesh->GetElement(j));

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

  nameFile << nameModel;
  nameFile << "_n.process_";
  nameFile << numberProcess;
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

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberElements(); j++) {
      auto t =
          std::static_pointer_cast<TriangleAdaptive>(sub_mesh->GetElement(j));

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

void GeneratorAdaptive::WriteQualityMesh(
    const std::shared_ptr<MeshAdaptive>& mesh, int step_,
    [[maybe_unused]] vector<double> step_error, int rank) {
  // Análise dos Elementos da Malha Gerada

  // cout<< "INIT >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
  stringstream nameFile;

  nameFile << nameModel;
  nameFile << "_n.process_";
  nameFile << numberProcess;
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

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    for (unsigned int j = 0; j < sub_mesh->GetNumberElements(); j++) {
      auto t =
          std::static_pointer_cast<TriangleAdaptive>(sub_mesh->GetElement(j));

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

void GeneratorAdaptive::SaveMesh(const std::shared_ptr<MeshAdaptive>& malha,
                                 int step_) {
  save_mesh_.push_back(make_pair(step_, malha));
}

void GeneratorAdaptive::SaveErrorMesh(
    const std::shared_ptr<MeshAdaptive>& malha, int step_) {
  save_error_mesh_.push_back(make_pair(step_, malha));
}

void escreveElementos(int step_, std::shared_ptr<SubMesh>& sub_mesh, int i) {
  stringstream nome;
  nome << step_;
  nome << "submalha-";
  nome << i;
  nome << ".log";

  ofstream arq(nome.str().c_str());

  for (unsigned int k = 0; k < sub_mesh->GetNumberElements(); ++k) {
    auto elem = sub_mesh->GetElement(k);

    NodeAdaptive n1(elem->GetNoh(1));
    NodeAdaptive n2(elem->GetNoh(2));
    NodeAdaptive n3(elem->GetNoh(3));

    tuple<double, double> t1 =
        std::static_pointer_cast<TriangleAdaptive>(elem)->GetParametersN1();
    tuple<double, double> t2 =
        std::static_pointer_cast<TriangleAdaptive>(elem)->GetParametersN2();
    tuple<double, double> t3 =
        std::static_pointer_cast<TriangleAdaptive>(elem)->GetParametersN3();

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

void GeneratorAdaptive::GeneratorInitialMesh(
    std::shared_ptr<Geometry>& geometry, std::shared_ptr<MeshAdaptive>& mesh,
    Timer& timer, int size_thread, int size_patch) {
#if USE_OPENMP
#pragma omp parallel num_threads(size_thread) shared(mesh, geometry, size_patch)
  {
    Int id = communicator_->threadId();

    if (!this->id_managers_[id]) {
      this->id_managers_[id] = this->MakeIdManagerOmp(communicator_, id).get();
    }
#if USE_MPI
    timer.InitTimerParallel(rankMpi, id, 2);  // Malha inicial
#else
    timer.InitTimerParallel(0, id, 2);  // Malha inicial
#endif  // USE_MPI

    // 1. Gera a mesh inicial
#pragma omp for
    for (int i = 0; i < size_patch; ++i) {
      auto patch = static_pointer_cast<PatchCoons>(geometry->GetPatch(i));
      auto id_manager_ptr =
          std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
      auto sub_mesh = this->GeneratorInitialMeshOmp(patch, id_manager_ptr);
      mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
    }

#if USE_MPI
    timer.EndTimerParallel(rankMpi, id, 2);  // Malha inicial
#else
    timer.EndTimerParallel(0, id, 2);   // Malha inicial
#endif  // USE_MPI
  }
#else
  for (int i = 0; i < size_patch; ++i) {
    auto patch = static_pointer_cast<PatchCoons>(geometry->GetPatch(i));
    auto id_manager_ptr =
        std::shared_ptr<Performer::IdManager>(this->id_managers_[0]);
    auto sub_mesh = this->InitialMesh(patch, id_manager_ptr);
    mesh->InsertSubMeshAdaptiveByPosition(sub_mesh, i);
  }
#endif  // USE_OPENMP
}

void GeneratorAdaptive::PrintElments(const std::shared_ptr<MeshAdaptive>& mesh,
                                     int step_,
                                     [[maybe_unused]] vector<double> step_error,
                                     int rank) {
  [[maybe_unused]] unsigned long int Nv = 0, Nt = 0;

  for (unsigned int i = 0; i < mesh->GetNumberSubMeshesAdaptive(); i++) {
    auto sub_mesh = mesh->GetSubMeshAdaptiveByPosition(i);

    Nv += sub_mesh->GetNumberNos();
    Nt += sub_mesh->GetNumberElements();
  }

  cout << "#elementos_" << nameModel << "_n.process_" << numberProcess
       << "_passo_" << step_ << "_rank_" << rank << " = " << Nt << endl;
}
