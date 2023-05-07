#ifndef INPUT_OUTPUT_WRITE_OBJ_FILE_H
#define INPUT_OUTPUT_WRITE_OBJ_FILE_H

#include <ctime>
#include <fstream>
#include <sstream>

#include "../data/mesh/mesh_adaptive.h"
#include "../data/triangle_adaptive.h"

class WriteOBJFile {
 public:
  WriteOBJFile();
  bool WriteMeshOBJFile(MeshAdaptive* mesh, unsigned int step, int process);
  void WriteCurvaturePatches(std::vector<double> patches, double max_value);
  ~WriteOBJFile();
};

#endif  // INPUT_OUTPUT_WRITE_OBJ_FILE_H
