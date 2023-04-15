// Copyright (c) 2023 Tiago Sombra
#ifndef AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_
#define AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_

#include <memory>

#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/curve/curve_adaptive_parametric_hermite.h"
#include "../data/geometry.h"
#include "../data/patch/patch_bezier.h"
#include "../data/vertex_adaptive.h"

class Models3d {
 public:
  Models3d();
  void ModelBook(std::unique_ptr<Geometry> &geometry);
  void ModelPlanBook(std::unique_ptr<Geometry> &geometry);
  void ModelTresPatches(std::unique_ptr<Geometry> &geometry);
  void ModelDoisPatchesPlanosCurva1(std::unique_ptr<Geometry> &geometry);
  void ModelDoisPatchesPlanosCurva(std::unique_ptr<Geometry> &geometry);
  void ModelCurvaBezier(std::unique_ptr<Geometry> &geometry);
  void ModelParaboloide(std::unique_ptr<Geometry> &geometry);
  void ModelPneu(std::unique_ptr<Geometry> &geometry);
  void ModelLadoDescendente(std::unique_ptr<Geometry> &geometry);
  void ModelBaseCircular(std::unique_ptr<Geometry> &geometry);
  void ModelBaseQuadrada(std::unique_ptr<Geometry> &geometry);
  void ModelBordaCurva(std::unique_ptr<Geometry> &geometry);
  void ModelDoisPatches(std::unique_ptr<Geometry> &geometry);
  void ModelNariz(std::unique_ptr<Geometry> &geometry);
  void ModelUtahteapot(std::unique_ptr<Geometry> &geometry);
};

#endif  // AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_
