#ifndef MODELOS3D_H
#define MODELOS3D_H

#include "../data/Geometria.h"
#include "../data/Vertice.h"
#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/curve/curve_adaptive_parametric_hermite.h"
#include "../data/patch/patch_bezier.h"

class Modelos3d {
 public:
  Modelos3d();
  Geometry *modelBook(Geometry *geo);
  Geometry *modelPlanBook(Geometry *geo);
  Geometry *modelTresPatches(Geometry *geo);
  Geometry *modelDoisPatchesPlanosCurva1(Geometry *geo);
  Geometry *modelDoisPatchesPlanosCurva(Geometry *geo);
  Geometry *modelCurvaBezier(Geometry *geo);
  Geometry *modelParaboloide(Geometry *geo);
  Geometry *modelPneu(Geometry *geo);
  Geometry *modelLadoDescendente(Geometry *geo);
  Geometry *modelBaseCircular(Geometry *geo);
  Geometry *modelBaseQuadrada(Geometry *geo);
  Geometry *modelBordaCurva(Geometry *geo);
  Geometry *modelDoisPatches(Geometry *geo);
  Geometry *modelNariz(Geometry *geo);
  Geometry *modelUtahteapot(Geometry *geo);
};

#endif  // MODELOS3D_H
