// Copyright (c) 2023 Tiago Guimarães Sombra. Todos os direitos reservados.
#ifndef AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_WRITE_OBJ_FILE_H_
#define AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_WRITE_OBJ_FILE_H_

#include <chrono>
#include <ctime>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../data/mesh/mesh_adaptive.h"
#include "../data/triangle_adaptive.h"

extern std::string NAME_MODEL;

class WriteOBJFile {
 public:
  WriteOBJFile();
  bool WriteMeshOBJFile(std::shared_ptr<MeshAdaptive> mesh, unsigned int step,
                        int process);
  void WriteCurvaturePatches(std::vector<double> patches, double max_value);
  ~WriteOBJFile();
};

#endif  // AP_MESH_PROJECT_INCLUDE_INPUT_OUTPUT_WRITE_OBJ_FILE_H_
