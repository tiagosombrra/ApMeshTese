#ifndef WriteOBJFIle_h
#define WriteOBJFIle_h

#include <ctime>
#include <fstream>
#include <sstream>

#include "../data/mesh/mesh_adaptive.h"
#include "../data/triangle_adaptive.h"

extern std::string NAME_MODEL;

class WriteOBJFile {
 public:
  WriteOBJFile();
  bool writeMeshOBJFile(MeshAdaptive* malha, unsigned int passo, int process);
  void writeCurvaturePatches(std::vector<double> vecCurvaturePatches,
                             double maxValue);
  ~WriteOBJFile();
};

#endif /* WriteOBJFIle_h */
