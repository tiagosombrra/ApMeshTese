// Copyright (c) 2023 Tiago Sombra
#ifndef AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_
#define AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_

#include <memory>

#include "../data/curve/curve_adaptive_parametric_bezier.h"
#include "../data/curve/curve_adaptive_parametric_hermite.h"
#include "../data/geometry.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_hermite.h"
#include "../data/vertex_adaptive.h"

class Models3d {
 public:
  Models3d();
  void ModelPlanBook(std::shared_ptr<Geometry> &geometry);
  void ModelTresPatches(std::shared_ptr<Geometry> &geometry);
  void ModelDoisPatchesPlanosCurva1(std::shared_ptr<Geometry> &geometry);
  void ModelDoisPatchesPlanosCurva(std::shared_ptr<Geometry> &geometry);
  void ModelCurvaBezier(std::shared_ptr<Geometry> &geometry);
  void ModelParaboloide(std::shared_ptr<Geometry> &geometry);
  void ModelPneu(std::shared_ptr<Geometry> &geometry);
  void ModelLadoDescendente(std::shared_ptr<Geometry> &geometry);
  void ModelBaseCircular(std::shared_ptr<Geometry> &geometry);
  void ModelBaseQuadrada(std::shared_ptr<Geometry> &geometry);
  void ModelBordaCurva(std::shared_ptr<Geometry> &geometry);
  void ModelDoisPatches(std::shared_ptr<Geometry> &geometry);
  void ModelNariz(std::shared_ptr<Geometry> &geometry);
  void ModelUtahteapot(std::shared_ptr<Geometry> &geometry);
};

#endif  // AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_MODELS_3D_H_
