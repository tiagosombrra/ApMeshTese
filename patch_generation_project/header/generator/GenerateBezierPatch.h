#ifndef GENERATEBEZIERPATCH_H
#define GENERATEBEZIERPATCH_H

// Verificação do compilador
#ifdef __APPLE__
#include "Eigen/Dense"
#include "Eigen/Eigen"
#elif __linux__
#include "../../lib/eigen/Eigen/Dense"
#include "../../lib/eigen/Eigen/Eigen"
#else
#error "Unknown compiler"
#endif
#include "../basic/Definitions.h"

using namespace std;

class GenerateBezierPatch {
 public:
  GenerateBezierPatch();
  VectorTuple48d calculateControlPoints(VectorTuple48d);
  void calculateMatrixN();
  void calculateLineMatrixN(double, double, int);
  double unitBernstein(double, int, int);
  double bernstein(int, int);
  int factorial(int);

  VectorTuple48d reducePatch(TupleVector48d);
  VectorTuple48d reduceAllPatches(VectorTuple48d, int);

  void addPatchesVectorTuplePointsReduce(VectorTuple48d);
  void removePatchesVectorTuplePointsReduce(int);

  Vector3d calcReduceMidPoint(Vector3d p0, Vector3d p1, Vector3d p2,
                              Vector3d p3);
  Vector3d calcReduceNearMarge(Vector3d p0, Vector3d p1);
  Vector3d calcReduceNearMidPoint(Vector3d p0, Vector3d p1, Vector3d p2);

 private:
  Matrix16d matrix16d;
  double matrixN[16][16];
  VectorTuple48d vectorTuplePointsReduce;

  Vector3d r00, r10, r30, r40, r50, r70, r80;
  Vector3d r01, r11, r31, r41, r51, r71, r81;
  Vector3d tr02, tr12, tr32, tr42, tr52, tr72, tr82;
  Vector3d r03, r13, r33, r43, r53, r73, r83;
  Vector3d r04, r14, r34, r44, r54, r74, r84;
  Vector3d r05, r15, r35, r45, r55, r75, r85;
  Vector3d tr06, tr16, tr36, tr46, tr56, tr76, tr86;
  Vector3d r07, r17, r37, r47, r57, r77, r87;
  Vector3d r08, r18, r38, r48, r58, r78, r88;
};

#endif  // GENERATEBEZIERPATCH_H
