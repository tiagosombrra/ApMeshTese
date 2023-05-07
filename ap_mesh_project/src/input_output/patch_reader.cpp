#include "../../include/input_output/patch_reader.h"

#include "../../include/definitions.h"

extern std::string inputModel;

PatchReader::PatchReader() {
  // patch = new BezierPatch();
}

std::list<std::shared_ptr<PatchHermite>> PatchReader::LoaderBPFileHermite() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/"
      "apMesh/Modelos/blend/18_mountain.bp";
#else
  string filename = inputModel;  //"../../inputModel/mountain.bp"
#endif  // #ifdef __APPLE__

  patch_hermite_ = std::make_shared<PatchHermite>();

  if (!filename.empty()) {
    std::ifstream fin(filename);

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;
        std::vector<PointAdaptive> vectorControlPoints;
        std::vector<long> p;
        long id_ponto = 0;

        while (std::getline(fin, line)) {
          if (line[0] == 'v') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            // int id_ponto;
            // iss >> id_ponto;

            // line = line.erase(0, 1);
            double n;
            while (iss >> n) {
              v.push_back(n);
            }

            PointAdaptive ponto(v[0], v[1], v[2], id_ponto);
            id_ponto++;
            vectorControlPoints.push_back(ponto);
            v.erase(v.begin(), v.end());
          }

          if (line[0] == 'p') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            long n;
            while (iss >> n) {
              p.push_back(n);
            }

            for (unsigned int i = 0; i < p.size(); i++) {
              //                            if (i == 0) {
              //                                patchHermite->setPt00(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 1) {
              //                                patchHermite->setPt10(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 2) {
              //                                patchHermite->setPt20(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 3) {
              //                                patchHermite->setPt30(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 4) {
              //                                patchHermite->setPt01(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 5) {
              //                                patchHermite->setPt11(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 6) {
              //                                patchHermite->setPt21(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 7) {
              //                                patchHermite->setPt31(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 8) {
              //                                patchHermite->setPt02(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 9) {
              //                                patchHermite->setPt12(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 10) {
              //                                patchHermite->setPt22(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 11) {
              //                                patchHermite->setPt32(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 12) {
              //                                patchHermite->setPt03(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 13) {
              //                                patchHermite->setPt13(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 14) {
              //                                patchHermite->setPt23(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            } else if (i == 15) {
              //                                patchHermite->setPt33(getPointVectorControlPoints(vectorControlPoints,
              //                                p[i]));
              //                            }
            }

            p.erase(p.begin(), p.end());
            patches_hermite_.push_back(patch_hermite_);
            patch_hermite_ = std::make_shared<PatchHermite>();
          }
        }
      }
    } else {
      std::cout << "Error: file .bp is not readable." << std::endl;
    }

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .bp" << std::endl;
  }

  return patches_hermite_;
}

std::list<std::shared_ptr<PatchBezier>> PatchReader::LoaderBPFile(
    std::string filename) {
  patch_ = std::make_shared<PatchBezier>();

  if (!filename.empty()) {
    std::ifstream fin(filename);
    // boost::filesystem::ifstream fin(filename);

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;
        std::vector<PointAdaptive> vectorControlPoints;
        std::vector<long> p;
        long id_ponto = 0;

        while (std::getline(fin, line)) {
          if (line[0] == 'v') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            // boost::archive::text_iarchive iss(&line);
            double n;
            while (iss >> n) {
              v.push_back(n);
            }

            PointAdaptive ponto(v[0], v[1], v[2], id_ponto);
            id_ponto++;
            vectorControlPoints.push_back(ponto);
            v.erase(v.begin(), v.end());
          }

          if (line[0] == 'p') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            long n;
            while (iss >> n) {
              p.push_back(n);
            }

            for (unsigned int i = 0; i < p.size(); i++) {
              if (i == 0) {
                patch_->SetPt00(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 1) {
                patch_->SetPt10(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 2) {
                patch_->SetPt20(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 3) {
                patch_->SetPt30(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 4) {
                patch_->SetPt01(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 5) {
                patch_->SetPt11(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 6) {
                patch_->SetPt21(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 7) {
                patch_->SetPt31(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 8) {
                patch_->SetPt02(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 9) {
                patch_->SetPt12(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 10) {
                patch_->SetPt22(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 11) {
                patch_->SetPt32(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 12) {
                patch_->SetPt03(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 13) {
                patch_->SetPt13(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 14) {
                patch_->SetPt23(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 15) {
                patch_->SetPt33(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              }
            }

            p.erase(p.begin(), p.end());
            patches_.push_back(patch_);
            patch_ = std::make_shared<PatchBezier>();
          }
        }
      }
    } else {
      std::cout << "Error: file .bp is not readable." << std::endl;
    }

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .bp" << std::endl;
  }

  return patches_;
}

std::list<std::shared_ptr<PatchBezier>> PatchReader::LoaderOBJFile() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/"
      "genBezierPatches/projeto/linux/saida/patch_estimativa_2.bp";
#else
  string filename = "../../inputModel/uteapot.obj";
#endif  // #ifdef __APPLE__

  patch_ = std::make_shared<PatchBezier>();

  if (!filename.empty()) {
    std::ifstream fin(filename);

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;
        std::vector<PointAdaptive> vectorControlPoints;
        std::vector<long> p;
        long id_Ponto = 0;

        while (std::getline(fin, line)) {
          if (line[0] == 'v') {
            line = line.erase(0, 1);
            std::istringstream iss(line);

            double n;
            while (iss >> n) {
              v.push_back(n);
            }

            PointAdaptive ponto(v[0], v[1], v[2], id_Ponto);
            id_Ponto++;
            vectorControlPoints.push_back(ponto);
            v.erase(v.begin(), v.end());
          }

          if (line[0] == 's' and line[1] == 'u' and line[2] == 'r' and
              line[3] == 'f') {
            line = line.erase(0, 12);
            std::istringstream iss(line);
            long n;
            while (iss >> n) {
              p.push_back(abs(n));
            }

            for (unsigned int i = 0; i < p.size(); i++) {
              if (i == 0) {
                patch_->SetPt00(vectorControlPoints.at(i));
              } else if (i == 1) {
                patch_->SetPt10(vectorControlPoints.at(i));
              } else if (i == 2) {
                patch_->SetPt20(vectorControlPoints.at(i));
              } else if (i == 3) {
                patch_->SetPt30(vectorControlPoints.at(i));
              } else if (i == 4) {
                patch_->SetPt01(vectorControlPoints.at(i));
              } else if (i == 5) {
                patch_->SetPt11(vectorControlPoints.at(i));
              } else if (i == 6) {
                patch_->SetPt21(vectorControlPoints.at(i));
              } else if (i == 7) {
                patch_->SetPt31(vectorControlPoints.at(i));
              } else if (i == 8) {
                patch_->SetPt02(vectorControlPoints.at(i));
              } else if (i == 9) {
                patch_->SetPt12(vectorControlPoints.at(i));
              } else if (i == 10) {
                patch_->SetPt22(vectorControlPoints.at(i));
              } else if (i == 11) {
                patch_->SetPt32(vectorControlPoints.at(i));
              } else if (i == 12) {
                patch_->SetPt03(vectorControlPoints.at(i));
              } else if (i == 13) {
                patch_->SetPt13(vectorControlPoints.at(i));
              } else if (i == 14) {
                patch_->SetPt23(vectorControlPoints.at(i));
              } else if (i == 15) {
                patch_->SetPt33(vectorControlPoints.at(i));
              }
            }

            p.erase(p.begin(), p.end());
            patches_.push_back(patch_);
            patch_ = std::make_shared<PatchBezier>();
          }
        }
      }
    } else {
      std::cout << "Error: file .obj is not readable." << std::endl;
    }

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .obj" << std::endl;
  }

  return patches_;
}

PatchReader::~PatchReader() {
  // delete patch;
}
std::list<std::shared_ptr<PatchBezier>>
PatchReader::OrderVectorToListBezierPatches(std::vector<double> vecDouble) {
  for (unsigned int i = 0; i < vecDouble.size(); i += i + 16) {
    patch_ = std::make_shared<PatchBezier>();
  }

  return patches_;
}

PointAdaptive PatchReader::GetPointVectorControlPoints(
    std::vector<PointAdaptive> vectorPoints, unsigned long idPoint) {
  for (std::vector<PointAdaptive>::iterator it = vectorPoints.begin();
       it != vectorPoints.end(); ++it) {
    if ((*it).GetId() == idPoint) {
      return (*it);
    }
  }

  PointAdaptive ponto;
  return ponto;
}

std::list<std::shared_ptr<PatchBezier>> PatchReader::ParsePatchesBezier() {
#if USE_INTERFACEQT

  QFileDialog dialog;
  QWidget* wid = new QWidget();
  QString filename = dialog.getOpenFileName(
      wid, QObject::tr("Adicionar um conjunto de patch"), QDir::currentPath(),
      QObject::tr("Arquivo de cenas (*.patches *.pt)"));
  dialog.close();

#else
  // setar manuamente a localização do arquivo de inputModel gerado pelo
  // genMesh(.pt / .patches)

#ifdef __APPLE__
  string filename = "../../inputModel/four_patches.pt";
#else
  string filename = "../../inputModel/cone1.pt";
#endif  // #ifdef __APPLE__

#endif  // #if USE_INTERFACEQT

#if USE_INTERFACEQT
  if (!filename.isNull()) {
    std::ifstream fin(filename.toUtf8().constData());

#else
  if (!filename.empty()) {
    std::ifstream fin(filename);

#endif  // #if USE_INTERFACEQT

    int count_points = 0;
    long id_point = 0;
    // bool delPatch = true;

    patch_ = std::make_shared<PatchBezier>();

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;

        while (std::getline(fin, line)) {
          //  delPatch = true;

          if (line[0] == 'p' && line[1] == 'a') {
            //  delPatch = false;

            line = line.erase(0, 6);
            std::istringstream iss(line);
            int id_patch;
            iss >> id_patch;
            patch_->SetIdPatchBezier(id_patch);
          }

          if (line[0] == 'p' && line[1] == 'c') {
            //  delPatch = false;

            std::string name_patch = "";
            for (unsigned int i = 0; i < line.size(); i++) {
              if (line[i] != ' ') {
                name_patch.push_back(line[i]);
              } else {
                break;
              }
            }

            line = line.erase(0, 4);
            std::istringstream iss(line);
            double n;

            while (iss >> n) {
              v.push_back(n);
            }
            PointAdaptive point3D(v[0], v[1], v[2], id_point);

            id_point++;

            v.erase(v.begin(), v.end());

            if (name_patch == "pc00") {
              patch_->SetPt00(point3D);
              count_points++;

            } else if (name_patch == "pc10") {
              patch_->SetPt10(point3D);
              count_points++;

            } else if (name_patch == "pc20") {
              patch_->SetPt20(point3D);
              count_points++;

            } else if (name_patch == "pc30") {
              patch_->SetPt30(point3D);
              count_points++;

            } else if (name_patch == "pc01") {
              patch_->SetPt01(point3D);
              count_points++;

            } else if (name_patch == "pc11") {
              patch_->SetPt11(point3D);
              count_points++;

            } else if (name_patch == "pc21") {
              patch_->SetPt21(point3D);
              count_points++;

            } else if (name_patch == "pc31") {
              patch_->SetPt31(point3D);
              count_points++;

            } else if (name_patch == "pc02") {
              patch_->SetPt02(point3D);
              count_points++;

            } else if (name_patch == "pc12") {
              patch_->SetPt12(point3D);
              count_points++;

            } else if (name_patch == "pc22") {
              patch_->SetPt22(point3D);
              count_points++;

            } else if (name_patch == "pc32") {
              patch_->SetPt32(point3D);
              count_points++;

            } else if (name_patch == "pc03") {
              patch_->SetPt03(point3D);
              count_points++;

            } else if (name_patch == "pc13") {
              patch_->SetPt13(point3D);
              count_points++;

            } else if (name_patch == "pc23") {
              patch_->SetPt23(point3D);
              count_points++;

            } else if (name_patch == "pc33") {
              patch_->SetPt33(point3D);
              count_points++;
            }

            name_patch = "";
          }

          if (count_points == 16) {
            count_points = 0;
            patches_.push_back(patch_);
            patch_ = std::make_shared<PatchBezier>();
          }
        }
      }
    } else
      std::cout << "Error: file .obj is not readable." << std::endl;

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .obj" << std::endl;
  }

#if USE_INTERFACEQT
  // delete wid;
#endif  // #if USE_INTERFACEQT
  return patches_;
}

std::list<std::shared_ptr<PatchBezier>> PatchReader::LoaderRibFile() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/GitHub/TMeshSurfAux_Documentos/"
      "TMeshSurf_Aux/reunioes/objs/"
      "four_patches.pt";
#else
  string filename = "../inputModel/uteapot.rib";
#endif  // #ifdef __APPLE__

  std::vector<double> v;

  if (!filename.empty()) {
    std::ifstream fin(filename);

    if (fin) {
      while (fin) {
        std::string line;

        while (std::getline(fin, line)) {
          if (line[0] == 'P' && line[1] == 'a' && line[2] == 't' &&
              line[3] == 'c' && line[4] == 'h') {
            line = line.erase(0, 21);

            std::istringstream iss(line);
            double n;

            while (iss >> n) {
              v.push_back(n);
            }
          }
        }
      }
    } else
      std::cout << "Error: file .obj is not readable." << std::endl;

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .obj" << std::endl;
  }

  cout << "tamanho de v: " << v.size() / (16 * 3) << endl;

  return patches_;
}

std::list<std::shared_ptr<PatchBezier>> PatchReader::LoaderBezierPatchFile(
    string fileName) {
  patch_ = std::make_shared<PatchBezier>();

  if (!fileName.empty()) {
    std::ifstream fin(fileName);

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;
        std::vector<PointAdaptive> vectorControlPoints;
        std::vector<long> p;
        long id_ponto = 0;

        while (std::getline(fin, line)) {
          if (line[0] == 'v') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            // int id_ponto;
            // iss >> id_ponto;

            // line = line.erase(0, 1);
            double n;
            while (iss >> n) {
              v.push_back(n);
            }

            PointAdaptive ponto(v[0], v[1], v[2], id_ponto);
            id_ponto++;
            vectorControlPoints.push_back(ponto);
            v.erase(v.begin(), v.end());
          }

          if (line[0] == 'p') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            long n;
            while (iss >> n) {
              p.push_back(n);
            }

            for (unsigned int i = 0; i < p.size(); i++) {
              if (i == 0) {
                patch_->SetPt00(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 1) {
                patch_->SetPt10(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 2) {
                patch_->SetPt20(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 3) {
                patch_->SetPt30(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 4) {
                patch_->SetPt01(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 5) {
                patch_->SetPt11(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 6) {
                patch_->SetPt21(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 7) {
                patch_->SetPt31(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 8) {
                patch_->SetPt02(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 9) {
                patch_->SetPt12(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 10) {
                patch_->SetPt22(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 11) {
                patch_->SetPt32(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 12) {
                patch_->SetPt03(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 13) {
                patch_->SetPt13(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 14) {
                patch_->SetPt23(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 15) {
                patch_->SetPt33(
                    GetPointVectorControlPoints(vectorControlPoints, p[i]));
              }
            }

            p.erase(p.begin(), p.end());
            patches_.push_back(patch_);
            patch_ = std::make_shared<PatchBezier>();
          }
        }
      }
    } else {
      std::cout << "Error: file .bp is not readable." << std::endl;
    }

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .bp" << std::endl;
  }

  return patches_;
}

std::shared_ptr<Geometry> PatchReader::ReaderFilePatches(
    std::shared_ptr<Geometry>& geometry, string file_name) {
  auto patches = LoaderBezierPatchFile(file_name);

  std::shared_ptr<CurveAdaptiveParametric> patch_c1;
  std::shared_ptr<CurveAdaptiveParametric> patch_c2;
  std::shared_ptr<CurveAdaptiveParametric> patch_c3;
  std::shared_ptr<CurveAdaptiveParametric> patch_c4;

  std::shared_ptr<PatchBezier> patch_bezier;

  for (auto& patch : patches) {
    auto p00 = std::make_shared<VertexAdaptive>(patch->GetPt00().GetX(),
                                                patch->GetPt00().GetY(),
                                                patch->GetPt00().GetZ());
    p00->SetId(patch->GetPt00().GetId());

    auto p10 = std::make_shared<VertexAdaptive>(patch->GetPt10().GetX(),
                                                patch->GetPt10().GetY(),
                                                patch->GetPt10().GetZ());
    p10->SetId(patch->GetPt10().GetId());

    auto p20 = std::make_shared<VertexAdaptive>(patch->GetPt20().GetX(),
                                                patch->GetPt20().GetY(),
                                                patch->GetPt20().GetZ());
    p20->SetId(patch->GetPt20().GetId());

    auto p30 = std::make_shared<VertexAdaptive>(patch->GetPt30().GetX(),
                                                patch->GetPt30().GetY(),
                                                patch->GetPt30().GetZ());
    p30->SetId(patch->GetPt30().GetId());

    auto p01 = std::make_shared<VertexAdaptive>(patch->GetPt01().GetX(),
                                                patch->GetPt01().GetY(),
                                                patch->GetPt01().GetZ());
    p01->SetId(patch->GetPt01().GetId());

    auto p11 = std::make_shared<VertexAdaptive>(patch->GetPt11().GetX(),
                                                patch->GetPt11().GetY(),
                                                patch->GetPt11().GetZ());
    p11->SetId(patch->GetPt11().GetId());

    auto p21 = std::make_shared<VertexAdaptive>(patch->GetPt21().GetX(),
                                                patch->GetPt21().GetY(),
                                                patch->GetPt21().GetZ());
    p21->SetId(patch->GetPt21().GetId());

    auto p31 = std::make_shared<VertexAdaptive>(patch->GetPt31().GetX(),
                                                patch->GetPt31().GetY(),
                                                patch->GetPt31().GetZ());
    p31->SetId(patch->GetPt31().GetId());

    auto p02 = std::make_shared<VertexAdaptive>(patch->GetPt02().GetX(),
                                                patch->GetPt02().GetY(),
                                                patch->GetPt02().GetZ());
    p02->SetId(patch->GetPt02().GetId());

    auto p12 = std::make_shared<VertexAdaptive>(patch->GetPt12().GetX(),
                                                patch->GetPt12().GetY(),
                                                patch->GetPt12().GetZ());
    p12->SetId(patch->GetPt12().GetId());

    auto p22 = std::make_shared<VertexAdaptive>(patch->GetPt22().GetX(),
                                                patch->GetPt22().GetY(),
                                                patch->GetPt22().GetZ());
    p22->SetId(patch->GetPt22().GetId());

    auto p32 = std::make_shared<VertexAdaptive>(patch->GetPt32().GetX(),
                                                patch->GetPt32().GetY(),
                                                patch->GetPt32().GetZ());
    p32->SetId(patch->GetPt32().GetId());

    auto p03 = std::make_shared<VertexAdaptive>(patch->GetPt03().GetX(),
                                                patch->GetPt03().GetY(),
                                                patch->GetPt03().GetZ());
    p03->SetId(patch->GetPt03().GetId());

    auto p13 = std::make_shared<VertexAdaptive>(patch->GetPt13().GetX(),
                                                patch->GetPt13().GetY(),
                                                patch->GetPt13().GetZ());
    p13->SetId(patch->GetPt13().GetId());

    auto p23 = std::make_shared<VertexAdaptive>(patch->GetPt23().GetX(),
                                                patch->GetPt23().GetY(),
                                                patch->GetPt23().GetZ());
    p23->SetId(patch->GetPt23().GetId());

    auto p33 = std::make_shared<VertexAdaptive>(patch->GetPt33().GetX(),
                                                patch->GetPt33().GetY(),
                                                patch->GetPt33().GetZ());
    p33->SetId(patch->GetPt33().GetId());

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

    patch = std::make_shared<PatchBezier>(patch_c1, patch_c2, patch_c3,
                                          patch_c4, *p11, *p21, *p12, *p22);

    geometry->InsertPatch(patch);
  }

  return geometry;
}
