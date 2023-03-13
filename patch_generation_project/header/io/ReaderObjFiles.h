//
//  ReaderObjFiles.h
//  genBezierPatches
//
//  Created by Tiago Guimarães Sombra on 12/11/15.
//  Copyright © 2015 TMeshSurf. All rights reserved.
//

#ifndef ReaderObjFiles_h
#define ReaderObjFiles_h

// Verificação de sistema operacional
#ifdef __APPLE__
#include <Eigen/Eigen>

#include "Eigen/Dense"
#elif __linux__
#include "../../lib/eigen/Eigen/Dense"
#include "../../lib/eigen/Eigen/Eigen"
#else
#error "Unknown compiler"
#endif

#include <fstream>
#include <iostream>
#include <sstream>

#include "../basic/Definitions.h"
#include "WritePTFile.h"

using namespace std;

class ReaderObjFiles {
 public:
  ReaderObjFiles();
  ~ReaderObjFiles();
  VectorTuple48d fileReadingOBJ_Surface_Order(string);
  VectorTuple48d fileReadingOBJ(string);
  VectorTuple48d fileReadingRIB(string);
  VectorTuple48d fileReadingPatchesSiqueira(string);
  VectorTuple48d readerObjOriginalTeapot(string);
  VectorTuple48d fileReadingOBJBlender(string);

  bool checkOrientationOfPatches();
  bool verifyNeighbor(int id_point, int idPoint);
  VectorTuple48d convertVectorTuplePoints(VectorTuplePoints);
  VectorTuplePoints convertVectorTuplePoints(VectorTuple48d);
  BP convertBP(VectorTuple48d);

  int findControlPointMap(MapControlPoints, Vector3d);

 private:
  VectorTuplePoints vectorPoints;
  bool checkOrientacionPatches;
  bool verifyPointPatch;
  VectorTuplePachtVerify vectorTuplePachtVerify;
};

#endif /* ReaderObjFiles_h */
