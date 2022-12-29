#include "../../include/input_output/PatchBezierReader.h"

PatchBezierReader::PatchBezierReader() {
  // patch = new BezierPatch();
}

std::list<PatchHermite*> PatchBezierReader::loaderBPFileHermite() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/"
      "apMesh/Modelos/blend/18_mountain.bp";
#else
  string filename = INPUT_MODEL;  //"../../INPUT_MODEL/mountain.bp"
#endif  // #ifdef __APPLE__

  patchHermite = new PatchHermite();

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
            patchesHermite.push_back(patchHermite);
            patchHermite = new PatchHermite();
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

  return patchesHermite;
}

std::list<PatchBezier*> PatchBezierReader::loaderBPFile(std::string filename) {
  patch = new PatchBezier();

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
                patch->SetPt00(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 1) {
                patch->SetPt10(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 2) {
                patch->SetPt20(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 3) {
                patch->SetPt30(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 4) {
                patch->SetPt01(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 5) {
                patch->SetPt11(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 6) {
                patch->SetPt21(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 7) {
                patch->SetPt31(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 8) {
                patch->SetPt02(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 9) {
                patch->SetPt12(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 10) {
                patch->SetPt22(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 11) {
                patch->SetPt32(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 12) {
                patch->SetPt03(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 13) {
                patch->SetPt13(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 14) {
                patch->SetPt23(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 15) {
                patch->SetPt33(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              }
            }

            p.erase(p.begin(), p.end());
            patches.push_back(patch);
            patch = new PatchBezier();
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

  return patches;
}

std::list<PatchBezier*> PatchBezierReader::loaderOBJFile() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/"
      "genBezierPatches/projeto/linux/saida/patch_estimativa_2.bp";
#else
  string filename = "../../INPUT_MODEL/uteapot.obj";
#endif  // #ifdef __APPLE__

  patch = new PatchBezier();

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
                patch->SetPt00(vectorControlPoints.at(i));
              } else if (i == 1) {
                patch->SetPt10(vectorControlPoints.at(i));
              } else if (i == 2) {
                patch->SetPt20(vectorControlPoints.at(i));
              } else if (i == 3) {
                patch->SetPt30(vectorControlPoints.at(i));
              } else if (i == 4) {
                patch->SetPt01(vectorControlPoints.at(i));
              } else if (i == 5) {
                patch->SetPt11(vectorControlPoints.at(i));
              } else if (i == 6) {
                patch->SetPt21(vectorControlPoints.at(i));
              } else if (i == 7) {
                patch->SetPt31(vectorControlPoints.at(i));
              } else if (i == 8) {
                patch->SetPt02(vectorControlPoints.at(i));
              } else if (i == 9) {
                patch->SetPt12(vectorControlPoints.at(i));
              } else if (i == 10) {
                patch->SetPt22(vectorControlPoints.at(i));
              } else if (i == 11) {
                patch->SetPt32(vectorControlPoints.at(i));
              } else if (i == 12) {
                patch->SetPt03(vectorControlPoints.at(i));
              } else if (i == 13) {
                patch->SetPt13(vectorControlPoints.at(i));
              } else if (i == 14) {
                patch->SetPt23(vectorControlPoints.at(i));
              } else if (i == 15) {
                patch->SetPt33(vectorControlPoints.at(i));
              }
            }

            p.erase(p.begin(), p.end());
            patches.push_back(patch);
            patch = new PatchBezier();
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

  return patches;
}

PatchBezierReader::~PatchBezierReader() {
  // delete patch;
}
std::list<PatchBezier*> PatchBezierReader::orderVectorToListBezierPatches(
    std::vector<double> vecDouble) {
  for (unsigned int i = 0; i < vecDouble.size(); i += i + 16) {
    patch = new PatchBezier();
  }

  return patches;
}

PointAdaptive PatchBezierReader::getPointVectorControlPoints(
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

std::list<PatchBezier*> PatchBezierReader::parsePatchesBezier() {
#if USE_INTERFACEQT

  QFileDialog dialog;
  QWidget* wid = new QWidget();
  QString filename = dialog.getOpenFileName(
      wid, QObject::tr("Adicionar um conjunto de patch"), QDir::currentPath(),
      QObject::tr("Arquivo de cenas (*.patches *.pt)"));
  dialog.close();

#else
  // setar manuamente a localização do arquivo de INPUT_MODEL gerado pelo
  // genMesh(.pt / .patches)

#ifdef __APPLE__
  string filename = "../../INPUT_MODEL/four_patches.pt";
#else
  string filename = "../../INPUT_MODEL/cone1.pt";
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
            patch->setId_bezierPatch(id_patch);
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
              patch->SetPt00(point3D);
              count_points++;

            } else if (name_patch == "pc10") {
              patch->SetPt10(point3D);
              count_points++;

            } else if (name_patch == "pc20") {
              patch->SetPt20(point3D);
              count_points++;

            } else if (name_patch == "pc30") {
              patch->SetPt30(point3D);
              count_points++;

            } else if (name_patch == "pc01") {
              patch->SetPt01(point3D);
              count_points++;

            } else if (name_patch == "pc11") {
              patch->SetPt11(point3D);
              count_points++;

            } else if (name_patch == "pc21") {
              patch->SetPt21(point3D);
              count_points++;

            } else if (name_patch == "pc31") {
              patch->SetPt31(point3D);
              count_points++;

            } else if (name_patch == "pc02") {
              patch->SetPt02(point3D);
              count_points++;

            } else if (name_patch == "pc12") {
              patch->SetPt12(point3D);
              count_points++;

            } else if (name_patch == "pc22") {
              patch->SetPt22(point3D);
              count_points++;

            } else if (name_patch == "pc32") {
              patch->SetPt32(point3D);
              count_points++;

            } else if (name_patch == "pc03") {
              patch->SetPt03(point3D);
              count_points++;

            } else if (name_patch == "pc13") {
              patch->SetPt13(point3D);
              count_points++;

            } else if (name_patch == "pc23") {
              patch->SetPt23(point3D);
              count_points++;

            } else if (name_patch == "pc33") {
              patch->SetPt33(point3D);
              count_points++;
            }

            name_patch = "";
          }

          if (count_points == 16) {
            count_points = 0;
            patches.push_back(patch);
            // patch = nullptr;
            patch = new PatchBezier();
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
  delete wid;
#endif  // #if USE_INTERFACEQT
  return patches;
}

std::list<PatchBezier*> PatchBezierReader::LoaderRibFile() {
#ifdef __APPLE__
  string filename =
      "/Users/tiagosombra/Dropbox/tiago/ufc/GitHub/TMeshSurfAux_Documentos/"
      "TMeshSurf_Aux/reunioes/objs/"
      "four_patches.pt";
#else
  string filename = "../INPUT_MODEL/uteapot.rib";
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

  return patches;
}
