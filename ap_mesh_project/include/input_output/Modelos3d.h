#ifndef MODELOS3D_H
#define MODELOS3D_H

#include "../data/curve/CurvParamBezier.h"
#include "../data/curve/CurvParamHermite.h"
#include "../data/Geometria.h"
#include "../data/patch/BezierPatch.h"
#include "../data/Vertice.h"

class Modelos3d {
 public:
  Modelos3d();
  Geometria *modelBook(Geometria *geo);
  Geometria *modelPlanBook(Geometria *geo);
  Geometria *modelTresPatches(Geometria *geo);
  Geometria *modelDoisPatchesPlanosCurva1(Geometria *geo);
  Geometria *modelDoisPatchesPlanosCurva(Geometria *geo);
  Geometria *modelCurvaBezier(Geometria *geo);
  Geometria *modelParaboloide(Geometria *geo);
  Geometria *modelPneu(Geometria *geo);
  Geometria *modelLadoDescendente(Geometria *geo);
  Geometria *modelBaseCircular(Geometria *geo);
  Geometria *modelBaseQuadrada(Geometria *geo);
  Geometria *modelBordaCurva(Geometria *geo);
  Geometria *modelDoisPatches(Geometria *geo);
  Geometria *modelNariz(Geometria *geo);
  Geometria *modelUtahteapot(Geometria *geo);
};

#endif  // MODELOS3D_H