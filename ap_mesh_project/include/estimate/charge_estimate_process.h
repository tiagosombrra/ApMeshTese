#ifndef ESTIMATE_CHARGE_ESTIMATE_PROCESS_H
#define ESTIMATE_CHARGE_ESTIMATE_PROCESS_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "../adapter/adapter.h"
#include "../data/curve/curve_adaptive.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/geometry.h"
#include "../data/mesh/mesh_adaptive.h"
#include "../data/patch/patch_bezier.h"
#include "../data/point_adaptive.h"
#include "../data/vertex_adaptive.h"
#include "../input_output/patch_reader.h"
#include "../input_output/write_obj_file.h"
#include "../timer/timer.h"

extern double TIME_READ_FILE;
extern double ESTIMATIVE_TOLERANCE;
extern double TRIANGLE_MEDIO;
extern std::string WRITE_MESH;

class ChargeEstimateProcess {
 public:
  ChargeEstimateProcess();
  ~ChargeEstimateProcess() = default;

  std::list<std::shared_ptr<PatchBezier>> ChargeEstimate(
      std::shared_ptr<Geometry> geometry, std::shared_ptr<Timer> &timer);
  std::vector<PointAdaptive> InterpolateControlPointsCurve(
      const PointAdaptive &p0, const PointAdaptive &p1, const PointAdaptive &p2,
      const PointAdaptive &p3, const double u, const double v);
  double CalculateKaMedioPatch(std::shared_ptr<PatchBezier> patch, int points);
  double CalculateAreaPatch(std::shared_ptr<PatchBezier> patch,
                            int pointsGaussLegandre);
  double CalculateAreaTriangleMedioRad(std::shared_ptr<PatchBezier> patch);
  double CalculateAreaTriangleMedio(std::shared_ptr<PatchBezier> patch,
                                    std::shared_ptr<Timer> &timer, int degree);
  std::uint64_t CalculateNumbersTriangle(std::shared_ptr<PatchBezier> patch,
                                         int degree);
  std::shared_ptr<SubMesh> InitialMeshEstimate(
      std::shared_ptr<PatchCoons> patch, int degree);
  bool CalculateErroEstimative(std::shared_ptr<MeshAdaptive> mesh,
                               std::shared_ptr<Timer> &timer, int degree);

  double minor_error_;
  int minor_degree_;
  WriteOBJFile write_obj_file_;
  std::vector<double> curvatures_;
};

#endif  // ESTIMATE_CHARGE_ESTIMATE_PROCESS_H
