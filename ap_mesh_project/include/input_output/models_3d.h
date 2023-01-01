#ifndef INPUT_OUTPUT_MODELS_3D_H
#define INPUT_OUTPUT_MODELS_3D_H

#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/curve/curve_adaptive_parametric_hermite.h"
#include "../data/geometry.h"
#include "../data/patch/patch_bezier.h"
#include "../data/vertex_adaptive.h"

class Models3d {
 public:
  Models3d();
  Geometry *ModelBook(Geometry *geometry);
  Geometry *ModelPlanBook(Geometry *geometry);
  Geometry *ModelTresPatches(Geometry *geometry);
  Geometry *ModelDoisPatchesPlanosCurva1(Geometry *geometry);
  Geometry *ModelDoisPatchesPlanosCurva(Geometry *geometry);
  Geometry *ModelCurvaBezier(Geometry *geometry);
  Geometry *ModelParaboloide(Geometry *geometry);
  Geometry *ModelPneu(Geometry *geometry);
  Geometry *ModelLadoDescendente(Geometry *geometry);
  Geometry *ModelBaseCircular(Geometry *geometry);
  Geometry *ModelBaseQuadrada(Geometry *geometry);
  Geometry *ModelBordaCurva(Geometry *geometry);
  Geometry *ModelDoisPatches(Geometry *geometry);
  Geometry *ModelNariz(Geometry *geometry);
  Geometry *ModelUtahteapot(Geometry *geometry);
};

#endif  // INPUT_OUTPUT_MODELS_3D_H
