#ifndef CHARGEESTIMATEPROCESS_H
#define CHARGEESTIMATEPROCESS_H

#include <algorithm>
#include <map>

#include "../adapter/adapter.h"
#include "../data/Geometria.h"
#include "../data/Ponto.h"
#include "../data/Vertice.h"
#include "../data/curve/CurvParamBezier.h"
#include "../data/curve/curve_adaptive.h"
#include "../data/mesh/Malha.h"
#include "../data/patch/BezierPatch.h"
#include "../generator/GeradorAdaptativoPorCurvatura.h"
#include "../input_output/PatchBezierReader.h"
#include "../input_output/WriteOBJFile.h"
#include "../timer/Timer.h"

extern double TIME_READ_FILE;
extern double TOLERANCIA_ESTIMATIVE;
extern double TRIANGLE_MEDIO;
extern std::string WRITE_MESH;
class ChargeEstimateProcess {
 public:
  ChargeEstimateProcess();
  std::list<BezierPatch *> chargeEstimateProcess(Geometria *, Timer *timer,
                                                 string INPUT_MODEL);
  std::vector<Ponto> interpolateControlPointsCurve(Ponto p0, Ponto p1, Ponto p2,
                                                   Ponto p3, double u,
                                                   double v);
  double calculateKaMedioPatch(BezierPatch *patch, int points);
  double calculateAreaPatch(BezierPatch *patch, int pointesGauusLegandre);
  double calculateAreaTriangleMedioRad(BezierPatch *patch);
  double calculateAreaTriangleMedio(BezierPatch *patch, Timer *timer, int grau);
  long int calculateNumbersTriangle(BezierPatch *patch, int grau);
  SubMalha *malhaInicialEstimativa(CoonsPatch *patch, int grau);
  bool calculateErroEstimative(Malha *malha, Timer *timer, int grau);
  // double calculateAreaPatchSegmets(BezierPatch *patch, int segments);
  // double calculateSegmentMedio(BezierPatch *patch);
  ~ChargeEstimateProcess();
  double menor_erro;
  int menor_grau;
  WriteOBJFIle write;
  std::vector<double> vecCurvature;
};

#endif  // CHARGEESTIMATEPROCESS_H
