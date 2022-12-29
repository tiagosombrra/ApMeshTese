#ifndef CHARGEESTIMATEPROCESS_H
#define CHARGEESTIMATEPROCESS_H

#include <algorithm>
#include <map>

#include "../adapter/adapter.h"
#include "../data/Geometria.h"
#include "../data/Ponto.h"
#include "../data/Vertice.h"
#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/mesh/mesh_adaptive.h"
#include "../data/patch/patch_bezier.h"
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
  std::list<PatchBezier *> chargeEstimateProcess(Geometry *, Timer *timer,
                                                 string INPUT_MODEL);
  std::vector<Ponto> interpolateControlPointsCurve(Ponto p0, Ponto p1, Ponto p2,
                                                   Ponto p3, double u,
                                                   double v);
  double calculateKaMedioPatch(PatchBezier *patch, int points);
  double calculateAreaPatch(PatchBezier *patch, int pointesGauusLegandre);
  double calculateAreaTriangleMedioRad(PatchBezier *patch);
  double calculateAreaTriangleMedio(PatchBezier *patch, Timer *timer, int grau);
  long int calculateNumbersTriangle(PatchBezier *patch, int grau);
  SubMesh *malhaInicialEstimativa(PatchCoons *patch, int grau);
  bool calculateErroEstimative(MeshAdaptive *malha, Timer *timer, int grau);
  // double calculateAreaPatchSegmets(BezierPatch *patch, int segments);
  // double calculateSegmentMedio(BezierPatch *patch);
  ~ChargeEstimateProcess();
  double menor_erro;
  int menor_grau;
  WriteOBJFIle write;
  std::vector<double> vecCurvature;
};

#endif  // CHARGEESTIMATEPROCESS_H
