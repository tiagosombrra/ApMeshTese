#ifndef INPUT_OUTPUT_MODELS_3D_H
#define INPUT_OUTPUT_MODELS_3D_H

#include <memory>

#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/curve/curve_adaptive_parametric_hermite.h"
#include "../data/geometry.h"
#include "../data/patch/patch_bezier.h"
#include "../data/vertex_adaptive.h"

class Models3d {
 public:
  Models3d();
  std::shared_ptr<Geometry> ModelBook(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelPlanBook(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelTresPatches(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelDoisPatchesPlanosCurva1(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelDoisPatchesPlanosCurva(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelCurvaBezier(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelParaboloide(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelPneu(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelLadoDescendente(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelBaseCircular(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelBaseQuadrada(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelBordaCurva(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelDoisPatches(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelNariz(std::shared_ptr<Geometry> geometry);
  std::shared_ptr<Geometry> ModelUtahteapot(std::shared_ptr<Geometry> geometry);
};

#endif  // INPUT_OUTPUT_MODELS_3D_H
