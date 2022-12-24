//
//  ReaderObjFiles.cpp
//  genBezierPatches
//
//  Created by Tiago Guimarães Sombra on 12/11/15.
//  Copyright © 2015 TMeshSurf. All rights reserved.
//

#include "../../header/io/ReaderObjFiles.h"

ReaderObjFiles::ReaderObjFiles() {}

ReaderObjFiles::~ReaderObjFiles() {}

VectorTuple48d ReaderObjFiles::readerObjOriginalTeapot(string inputObj) {
  VectorTuple48d d;
  vector<int> vecIdsPatches;
  vector<Vector3d> vecControlPoints;
  ifstream fin(inputObj);
  int sizePatches = 0;
  int cont = 0;
  int sizeVector = 0;
  bool change = false;

  if (fin) {
    while (fin) {
      string line;

      while (getline(fin, line)) {
        int v[16];
        double vv[3];

        istringstream iss(line);
        if (cont == 0) {
          iss >> sizePatches;
        } else if (cont == sizePatches + 1) {
          iss >> sizeVector;
          change = true;
        } else {
          if (!change) {
            iss >> v[0];
            iss >> v[1];
            iss >> v[2];
            iss >> v[3];
            iss >> v[4];
            iss >> v[5];
            iss >> v[6];
            iss >> v[7];
            iss >> v[8];
            iss >> v[9];
            iss >> v[10];
            iss >> v[11];
            iss >> v[12];
            iss >> v[13];
            iss >> v[14];
            iss >> v[15];
            vecIdsPatches.push_back(v[0]);
            vecIdsPatches.push_back(v[1]);
            vecIdsPatches.push_back(v[2]);
            vecIdsPatches.push_back(v[3]);
            vecIdsPatches.push_back(v[4]);
            vecIdsPatches.push_back(v[5]);
            vecIdsPatches.push_back(v[6]);
            vecIdsPatches.push_back(v[7]);
            vecIdsPatches.push_back(v[8]);
            vecIdsPatches.push_back(v[9]);
            vecIdsPatches.push_back(v[10]);
            vecIdsPatches.push_back(v[11]);
            vecIdsPatches.push_back(v[12]);
            vecIdsPatches.push_back(v[13]);
            vecIdsPatches.push_back(v[14]);
            vecIdsPatches.push_back(v[15]);
          } else if (change) {
            iss >> vv[0];
            iss >> vv[1];
            iss >> vv[2];
            Vector3d point3D(vv[0], vv[1], vv[2]);
            vecControlPoints.push_back(point3D);
          }
        }
        cont++;
      }
    }
  }

  for (int i = 0; i < sizePatches; ++i) {
    TuplePoint tuplePoint1(i, 1, vecControlPoints.at(vecIdsPatches.at(0)));
    vectorPoints.push_back(tuplePoint1);

    TuplePoint tuplePoint2(i, 2, vecControlPoints.at(vecIdsPatches.at(1)));
    vectorPoints.push_back(tuplePoint2);

    TuplePoint tuplePoint3(i, 3, vecControlPoints.at(vecIdsPatches.at(2)));
    vectorPoints.push_back(tuplePoint3);

    TuplePoint tuplePoint4(i, 4, vecControlPoints.at(vecIdsPatches.at(3)));
    vectorPoints.push_back(tuplePoint4);

    TuplePoint tuplePoint5(i, 5, vecControlPoints.at(vecIdsPatches.at(4)));
    vectorPoints.push_back(tuplePoint5);

    TuplePoint tuplePoint6(i, 6, vecControlPoints.at(vecIdsPatches.at(5)));
    vectorPoints.push_back(tuplePoint6);

    TuplePoint tuplePoint7(i, 7, vecControlPoints.at(vecIdsPatches.at(6)));
    vectorPoints.push_back(tuplePoint7);

    TuplePoint tuplePoint8(i, 8, vecControlPoints.at(vecIdsPatches.at(7)));
    vectorPoints.push_back(tuplePoint8);

    TuplePoint tuplePoint9(i, 9, vecControlPoints.at(vecIdsPatches.at(8)));
    vectorPoints.push_back(tuplePoint9);

    TuplePoint tuplePoint10(i, 10, vecControlPoints.at(vecIdsPatches.at(9)));
    vectorPoints.push_back(tuplePoint10);

    TuplePoint tuplePoint11(i, 11, vecControlPoints.at(vecIdsPatches.at(10)));
    vectorPoints.push_back(tuplePoint11);

    TuplePoint tuplePoint12(i, 12, vecControlPoints.at(vecIdsPatches.at(11)));
    vectorPoints.push_back(tuplePoint12);

    TuplePoint tuplePoint13(i, 13, vecControlPoints.at(vecIdsPatches.at(12)));
    vectorPoints.push_back(tuplePoint13);

    TuplePoint tuplePoint14(i, 14, vecControlPoints.at(vecIdsPatches.at(13)));
    vectorPoints.push_back(tuplePoint14);

    TuplePoint tuplePoint15(i, 15, vecControlPoints.at(vecIdsPatches.at(14)));
    vectorPoints.push_back(tuplePoint15);

    TuplePoint tuplePoint16(i, 0, vecControlPoints.at(vecIdsPatches.at(15)));
    vectorPoints.push_back(tuplePoint16);
  }

  if (!checkOrientationOfPatches()) {
    cout << "ERRO >>> LISTA CONTEM PATCHES COM ORIENTAÇÃO ERRADA." << endl;
  } else {
    cout << "SUCESSO >>> LISTA NÃO CONTEM ERRO NA ORIENTAÇÃO." << endl;
  }

  WritePTFile write;
  write.writeObjFile("teacupWrite.obj", vectorPoints);

  VectorTuple48d vectorTuple48d = convertVectorTuplePoints(vectorPoints);

  return vectorTuple48d;
}

// VectorTuple48d ReaderObjFiles::fileReadingOBJBlender(string)
//{

//}

VectorTuple48d ReaderObjFiles::fileReadingOBJ_Surface_Order(string obj) {
  vector<Vector3d> vecPoints3d;
  ifstream fin(obj);
  cout << "INICIO >>> LEITURA" << endl;

  if (fin) {
    while (fin) {
      string line;
      double v[3];

      while (getline(fin, line)) {
        if (line[0] == 'v' and line[1] == ' ') {
          line = line.erase(0, 2);
          istringstream iss(line);

          iss >> v[0];
          iss >> v[1];
          iss >> v[2];

          Vector3d point3D(v[0], v[1], v[2]);
          vecPoints3d.push_back(point3D);
          // cout<<point3D<<endl;
        }
      }
    }
  }

  cout << vecPoints3d.size() << endl;

  //    int points = sqrt(vecPoints3d.size());
  //    int patches = (points - 1)/3;
  //    int patches_aux = patches;
  //    int value_x = 0;
  //    int value_y = 0;
  //    int value_y_aux = 0;

  int points = sqrt(vecPoints3d.size());
  int patches = ((points - 1) / 3) / 2;
  int patches_aux = patches;
  int value_x = 0;
  int value_y = 0;
  int value_y_aux = 0;

  for (int i = 0; i < pow(patches_aux, 2); ++i) {
    //        int value_00 = value_x + value_y*points;
    //        int value_01 = value_x + value_y*points+1;
    //        int value_02 = value_x + value_y*points+2;
    //        int value_03 = value_x + value_y*points+3;
    //        value_y++;

    //        int value_10 = value_x + value_y*points;
    //        int value_11 = value_x + value_y*points+1;
    //        int value_12 = value_x + value_y*points+2;
    //        int value_13 = value_x + value_y*points+3;
    //        value_y++;

    //        int value_20 = value_x + value_y*points;
    //        int value_21 = value_x + value_y*points+1;
    //        int value_22 = value_x + value_y*points+2;
    //        int value_23 = value_x + value_y*points+3;
    //        value_y++;

    //        int value_30 = value_x + value_y*points;
    //        int value_31 = value_x + value_y*points+1;
    //        int value_32 = value_x + value_y*points+2;
    //        int value_33 = value_x + value_y*points+3;

    //        value_x = value_x + 3;

    int value_00 = value_x + value_y * points;
    int value_01 = value_x + value_y * points + 2;
    int value_02 = value_x + value_y * points + 4;
    int value_03 = value_x + value_y * points + 6;
    value_y = value_y + 2;

    int value_10 = value_x + value_y * points;
    int value_11 = value_x + value_y * points + 2;
    int value_12 = value_x + value_y * points + 4;
    int value_13 = value_x + value_y * points + 6;
    value_y = value_y + 2;

    int value_20 = value_x + value_y * points;
    int value_21 = value_x + value_y * points + 2;
    int value_22 = value_x + value_y * points + 4;
    int value_23 = value_x + value_y * points + 6;
    value_y = value_y + 2;

    int value_30 = value_x + value_y * points;
    int value_31 = value_x + value_y * points + 2;
    int value_32 = value_x + value_y * points + 4;
    int value_33 = value_x + value_y * points + 6;

    value_x = value_x + 6;

    if (i == patches - 1) {
      value_y_aux = value_y_aux + 6;
      // value_y_aux = value_y_aux + 3;
      value_x = 0;
      patches = patches + patches_aux;
    }

    value_y = value_y_aux;

    TuplePoint tuplePoint15(i, 1, vecPoints3d.at(value_33));
    vectorPoints.push_back(tuplePoint15);

    TuplePoint tuplePoint11(i, 2, vecPoints3d.at(value_23));
    vectorPoints.push_back(tuplePoint11);

    TuplePoint tuplePoint7(i, 3, vecPoints3d.at(value_13));
    vectorPoints.push_back(tuplePoint7);

    TuplePoint tuplePoint3(i, 4, vecPoints3d.at(value_03));
    vectorPoints.push_back(tuplePoint3);

    TuplePoint tuplePoint14(i, 5, vecPoints3d.at(value_32));
    vectorPoints.push_back(tuplePoint14);

    TuplePoint tuplePoint10(i, 6, vecPoints3d.at(value_22));
    vectorPoints.push_back(tuplePoint10);

    TuplePoint tuplePoint6(i, 7, vecPoints3d.at(value_12));
    vectorPoints.push_back(tuplePoint6);

    TuplePoint tuplePoint2(i, 8, vecPoints3d.at(value_02));
    vectorPoints.push_back(tuplePoint2);

    TuplePoint tuplePoint13(i, 9, vecPoints3d.at(value_31));
    vectorPoints.push_back(tuplePoint13);

    TuplePoint tuplePoint9(i, 10, vecPoints3d.at(value_21));
    vectorPoints.push_back(tuplePoint9);

    TuplePoint tuplePoint5(i, 11, vecPoints3d.at(value_11));
    vectorPoints.push_back(tuplePoint5);

    TuplePoint tuplePoint1(i, 12, vecPoints3d.at(value_01));
    vectorPoints.push_back(tuplePoint1);

    TuplePoint tuplePoint12(i, 13, vecPoints3d.at(value_30));
    vectorPoints.push_back(tuplePoint12);

    TuplePoint tuplePoint8(i, 14, vecPoints3d.at(value_20));
    vectorPoints.push_back(tuplePoint8);

    TuplePoint tuplePoint4(i, 15, vecPoints3d.at(value_10));
    vectorPoints.push_back(tuplePoint4);

    TuplePoint tuplePoint(i, 0, vecPoints3d.at(value_00));
    vectorPoints.push_back(tuplePoint);
  }

  //                for (VectorTuplePoints::iterator it = vectorPoints.begin() ;
  //                it != vectorPoints.end(); ++it) {

  //                    cout<<"id do patch: "<<get<0>((*it))
  //                             <<" id do ponto: "<<get<1>(((*it)))%16
  //                             << " cood: "<<get<2>((*it))<< endl;
  //                }

  cout << "INICIO >>> ORIENTACAO" << endl;

  if (!checkOrientationOfPatches()) {
    cout << "ERRO >>> LISTA CONTEM PATCHES COM ORIENTAÇÃO ERRADA." << endl;
  } else {
    cout << "SUCESSO >>> LISTA NÃO CONTEM ERRO NA ORIENTAÇÃO." << endl;
  }

  VectorTuple48d vectorTuple48d = convertVectorTuplePoints(vectorPoints);

  return vectorTuple48d;
}

VectorTuple48d ReaderObjFiles::fileReadingOBJ(string siqInput) {
  ifstream fin(siqInput);

  if (fin) {
    while (fin) {
      string line;
      double v[3];
      int id_patch;
      int id_point_patch;

      while (getline(fin, line)) {
        if (line[0] == 'o' || line[0] == 'g') {
          for (int i = 0; i < line.size(); i++) {
            if (line[i] == '.') {
              line = line.erase(0, i + 1);
              break;
            }
          }

          for (int i = 0; i < line.size(); i++) {
            if (line[i] == '_') {
              line = line.erase(i, line.size());
              istringstream iss(line);
              int n;
              iss >> n;

              id_point_patch = n;

              if (n % 16 == 0) {
                id_patch = n / 16;
              }
              break;

            } else {
              istringstream iss(line);
              int n;
              iss >> n;

              id_point_patch = n;

              if (n % 16 == 0) {
                id_patch = n / 16;
              }
              break;
            }
          }
        }

        if (line[0] == 'v') {
          line = line.erase(0, 2);
          istringstream iss(line);

          iss >> v[0];
          iss >> v[1];
          iss >> v[2];

          Vector3d point3D(v[0], v[1], v[2]);

          TuplePoint tuplePoint(id_patch, id_point_patch, point3D);
          vectorPoints.push_back(tuplePoint);
        }
      }
    }
  }

  sort(vectorPoints.begin(), vectorPoints.end(),
       [](const TuplePoint &lhs, const TuplePoint &rhs) -> bool {
         return get<1>(lhs) < get<1>(rhs);
       });

  //            for (VectorTuplePoints::iterator it = vectorPoints.begin() ; it
  //            != vectorPoints.end(); ++it) {
  //
  //                cout<<"id do patch: "<<get<0>((*it))
  //                         <<" id do ponto: "<<get<1>(((*it)))%16
  //                         << " cood: "<<get<2>((*it))<< endl;
  //            }

  if (!checkOrientationOfPatches()) {
    cout << "ERRO >>> LISTA CONTEM PATCHES COM ORIENTAÇÃO ERRADA." << endl;
  } else {
    cout << "SUCESSO >>> LISTA NÃO CONTEM ERRO NA ORIENTAÇÃO." << endl;
  }

  VectorTuple48d vectorTuple48d = convertVectorTuplePoints(vectorPoints);

  return vectorTuple48d;
}

VectorTuple48d ReaderObjFiles::fileReadingRIB(string objInput) {
  VectorTuple48d vectorTuple48d;

  int idPatch = -1;

  ifstream fin(objInput);

  if (fin) {
    while (fin) {
      string line;
      Vector48d vector48d;

      while (getline(fin, line)) {
        if (line[0] == 'P' && line[1] == 'a') {
          for (int i = 0; i < line.size(); i++) {
            if (line[i] == '[') {
              line = line.erase(0, i + 1);
              break;
            }
          }
        }

        istringstream iss(line);
        iss >> vector48d(9, 0);
        iss >> vector48d(10, 0);
        iss >> vector48d(11, 0);
        iss >> vector48d(6, 0);
        iss >> vector48d(7, 0);
        iss >> vector48d(8, 0);
        iss >> vector48d(3, 0);
        iss >> vector48d(4, 0);
        iss >> vector48d(5, 0);
        iss >> vector48d(0, 0);
        iss >> vector48d(1, 0);
        iss >> vector48d(2, 0);

        iss >> vector48d(21, 0);
        iss >> vector48d(22, 0);
        iss >> vector48d(23, 0);
        iss >> vector48d(18, 0);
        iss >> vector48d(19, 0);
        iss >> vector48d(20, 0);
        iss >> vector48d(15, 0);
        iss >> vector48d(16, 0);
        iss >> vector48d(17, 0);
        iss >> vector48d(12, 0);
        iss >> vector48d(13, 0);
        iss >> vector48d(14, 0);

        iss >> vector48d(33, 0);
        iss >> vector48d(34, 0);
        iss >> vector48d(35, 0);
        iss >> vector48d(30, 0);
        iss >> vector48d(31, 0);
        iss >> vector48d(32, 0);
        iss >> vector48d(27, 0);
        iss >> vector48d(28, 0);
        iss >> vector48d(29, 0);
        iss >> vector48d(24, 0);
        iss >> vector48d(25, 0);
        iss >> vector48d(26, 0);

        iss >> vector48d(45, 0);
        iss >> vector48d(46, 0);
        iss >> vector48d(47, 0);
        iss >> vector48d(42, 0);
        iss >> vector48d(43, 0);
        iss >> vector48d(44, 0);
        iss >> vector48d(39, 0);
        iss >> vector48d(40, 0);
        iss >> vector48d(41, 0);
        iss >> vector48d(36, 0);
        iss >> vector48d(37, 0);
        iss >> vector48d(38, 0);

        idPatch++;

        TupleVector48d tupleVector48d(idPatch, vector48d);
        vectorTuple48d.push_back(tupleVector48d);
      }
    }
  }

  vectorPoints = convertVectorTuplePoints(vectorTuple48d);

  if (!checkOrientationOfPatches()) {
    cout << "ERRO >>> LISTA CONTEM PATCHES COM ORIENTAÇÃO ERRADA." << endl;
  }

  return vectorTuple48d;
}

VectorTuple48d ReaderObjFiles::fileReadingPatchesSiqueira(string objInput) {
  ifstream fin(objInput);

  if (fin) {
    while (fin) {
      string line;
      double v[3];
      int id_patch = 0;
      int id_point_patch = 0;

      while (getline(fin, line)) {
        size_t found = line.find("Vertice (");

        if (found != std::string::npos) {
          line = line.erase(0, found);

          found = line.find_first_of("Vertice(); ");
          while (found != std::string::npos) {
            line[found] = ' ';
            found = line.find_first_of("Vertice(); ", found + 1);
          }

          std::string::iterator end_pos =
              std::remove(line.begin(), line.end(), ' ');
          line.erase(end_pos, line.end());

          found = line.find_first_of(',');
          while (found != std::string::npos) {
            line[found] = ' ';
            found = line.find_first_of(',', found + 1);
          }

        } else {
          line = line.erase(0, line.size());
          std::string::iterator end_pos =
              std::remove(line.begin(), line.end(), ' ');
          line.erase(end_pos, line.end());
        }

        found = line.find_first_of("-0123456789");
        if (found != std::string::npos) {
          istringstream iss(line);
          iss >> v[0];
          iss >> v[1];
          iss >> v[2];

          Vector3d point3D(v[0], v[1], v[2]);

          TuplePoint tuplePoint(id_patch, id_point_patch, point3D);
          vectorPoints.push_back(tuplePoint);

          id_point_patch++;
          if (id_point_patch % 16 == 0) {
            id_patch = id_point_patch / 16;
          }
        }
      }
    }
  }

  //    sort(vectorPoints.begin(),vectorPoints.end(),[](const TuplePoint &lhs,
  //    const TuplePoint &rhs) -> bool
  //    {
  //        return get<1>(lhs) < get<1>(rhs);
  //    });

  //    for (VectorTuplePoints::iterator it = vectorPoints.begin() ; it !=
  //    vectorPoints.end(); ++it) {

  //        cout<<"id do patch: "<<get<0>((*it))
  //           <<" id do ponto: "<<get<1>(((*it)))%16
  //          << " cood: "<<get<2>((*it))<< endl;
  //    }

  if (!checkOrientationOfPatches()) {
    cout << "ERRO >>> LISTA CONTEM PATCHES COM ORIENTAÇÃO ERRADA." << endl;
  }

  VectorTuple48d vectorTuple48d = convertVectorTuplePoints(vectorPoints);

  return vectorTuple48d;
}

bool ReaderObjFiles::checkOrientationOfPatches() {
  vector<IdPatchPoint> vecIdPatchPoint;
  IdPatchPoint idPtatchPoint;
  checkOrientacionPatches = true;

  for (VectorTuplePoints::iterator it = vectorPoints.begin();
       it != vectorPoints.end(); ++it) {
    int id_patch = get<0>((*it));
    int id_point = get<1>((*it)) % 16;
    Vector3d point3D = get<2>((*it));

    for (VectorTuplePoints::iterator it = vectorPoints.begin();
         it != vectorPoints.end(); ++it) {
      int idPatch = get<0>((*it));
      int idPoint = get<1>((*it)) % 16;
      Vector3d point = get<2>((*it));

      if (id_patch != idPatch && point3D == point) {
        idPtatchPoint.first = idPatch;
        idPtatchPoint.second = idPoint;
        vecIdPatchPoint.push_back(idPtatchPoint);
      }
    }

    if (vecIdPatchPoint.size() > 0) {
      TuplePachtVerify tuplePachtVerify(id_patch, point3D, vecIdPatchPoint);
      vectorTuplePachtVerify.push_back(tuplePachtVerify);

      for (vector<IdPatchPoint>::iterator it = vecIdPatchPoint.begin();
           it != vecIdPatchPoint.end(); ++it) {
        if (!verifyNeighbor(id_point, (*it).second)) {
          cout << "Erro de orientação no ponto: " << point3D << endl;
          cout << "patch_orig: " << id_patch << " point_orig: " << id_point
               << endl;
          cout << "patch_dest: " << (*it).first
               << " point_dest: " << (*it).second << endl;

          checkOrientacionPatches = false;
        }
      }

      while (!vecIdPatchPoint.empty()) {
        vecIdPatchPoint.pop_back();
      }
    }
  }

  return checkOrientacionPatches;
}

bool ReaderObjFiles::verifyNeighbor(int id_point, int idPoint) {
  verifyPointPatch = false;

  switch (id_point) {
    case 0: {
      if (idPoint == 13 || idPoint == 4 || idPoint == 1) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 15: {
      if (idPoint == 3) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 14: {
      if (idPoint == 2) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 13: {
      if (idPoint == 4 || idPoint == 1 || idPoint == 0) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 12: {
      if (idPoint == 9) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 9: {
      if (idPoint == 12) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 8: {
      if (idPoint == 5) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 5: {
      if (idPoint == 8) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 4: {
      if (idPoint == 13 || idPoint == 1 || idPoint == 0) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 3: {
      if (idPoint == 15) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 2: {
      if (idPoint == 14) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    case 1: {
      if (idPoint == 13 || idPoint == 4 || idPoint == 0) {
        verifyPointPatch = true;
      } else {
        verifyPointPatch = false;
      }
      break;
    }

    default:
      break;
  }

  return verifyPointPatch;
}

VectorTuple48d ReaderObjFiles::convertVectorTuplePoints(
    VectorTuplePoints vecTuplePoints) {
  cout << "INICIO >>> CONVERSAO" << endl;

  VectorTuple48d vecTuple48d;

  int idPatch = 0;

  for (unsigned int i = 0; i < vecTuplePoints.size(); i = i + 16) {
    Vector48d vector48d;

    vector48d << get<2>(vecTuplePoints.at(i)), get<2>(vecTuplePoints.at(i + 1)),
        get<2>(vecTuplePoints.at(i + 2)), get<2>(vecTuplePoints.at(i + 3)),
        get<2>(vecTuplePoints.at(i + 4)), get<2>(vecTuplePoints.at(i + 5)),
        get<2>(vecTuplePoints.at(i + 6)), get<2>(vecTuplePoints.at(i + 7)),
        get<2>(vecTuplePoints.at(i + 8)), get<2>(vecTuplePoints.at(i + 9)),
        get<2>(vecTuplePoints.at(i + 10)), get<2>(vecTuplePoints.at(i + 11)),
        get<2>(vecTuplePoints.at(i + 12)), get<2>(vecTuplePoints.at(i + 13)),
        get<2>(vecTuplePoints.at(i + 14)), get<2>(vecTuplePoints.at(i + 15));

    TupleVector48d tupleVector48d(idPatch, vector48d);

    vecTuple48d.push_back(tupleVector48d);

    idPatch++;
  }

  //    for (VectorTuple16d::iterator it = vecTuple16d.begin() ; it !=
  //    vecTuple16d.end(); ++it) {
  //
  //        cout<<"id do patch: "<<get<0>((*it))<< " cood: "<<get<1>((*it))<<
  //        endl;
  //    }

  cout << "FIM >>> CONVERSAO" << endl;

  return vecTuple48d;
}

VectorTuplePoints ReaderObjFiles::convertVectorTuplePoints(
    VectorTuple48d vecTuplePoints48) {
  VectorTuplePoints vecTuplePoints;

  for (VectorTuple48d::iterator it = vecTuplePoints48.begin();
       it != vecTuplePoints48.end(); ++it) {
    int j = 1;

    for (int i = 0; i < 48; i = i + 3) {
      Vector3d vec3d;
      vec3d << get<1>(*it)(i, 0), get<1>(*it)(i + 1, 0), get<1>(*it)(i + 2, 0);

      TuplePoint tuplePoint(get<0>(*it), j % 16, vec3d);
      j++;
      vecTuplePoints.push_back(tuplePoint);
    }
  }

  //                for (VectorTuplePoints::iterator it = vecTuplePoints.begin()
  //                ; it != vecTuplePoints.end(); ++it) {

  //                    cout<<"id do patch: "<<get<0>((*it))
  //                             <<" id do ponto: "<<get<1>(((*it)))
  //                             << " cood: "<<get<2>((*it))<< endl;
  //                }

  return vecTuplePoints;
}

BP ReaderObjFiles::convertBP(VectorTuple48d vectorTuple48d) {
  cout << "INICIO >>> CONVERTBP" << endl;

  MapControlPoints mapControlPoints;
  PatchesControlPoints patchesControlPoints;
  int idControlPoint = 0;

  for (VectorTuple48d::iterator it = vectorTuple48d.begin();
       it != vectorTuple48d.end(); ++it) {
    PatchControlPoints patchControlPoints;
    int j = 0;
    for (int i = 0; i < 48; i = i + 3) {
      Vector3d vec3d;
      vec3d << get<1>(*it)(i, 0), get<1>(*it)(i + 1, 0), get<1>(*it)(i + 2, 0);

      int idFindControlPoint = findControlPointMap(mapControlPoints, vec3d);
      if (idFindControlPoint > -1) {
        patchControlPoints(j, 0) = idFindControlPoint;
        j++;
      } else {
        mapControlPoints.insert(make_pair(idControlPoint, vec3d));
        patchControlPoints(j, 0) = idControlPoint;
        j++;
        idControlPoint++;
      }
    }
    patchesControlPoints.push_back(patchControlPoints);
  }
  cout << "FIM >>> CONVERTBP" << endl;

  return make_tuple(mapControlPoints, patchesControlPoints);
}

int ReaderObjFiles::findControlPointMap(MapControlPoints mapControlPoints,
                                        Vector3d vec3d) {
  for (MapControlPoints::const_iterator it = mapControlPoints.begin();
       it != mapControlPoints.end(); it++) {
    if (it->second == vec3d) {
      return it->first;
    }
  }

  return -1;
}
