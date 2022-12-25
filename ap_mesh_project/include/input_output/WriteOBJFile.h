//
//  WriteOBJFIle.h
//  apMesh
//
//  Created by Tiago Guimarães Sombra on 05/12/15.
//  Copyright © 2015 TMeshSurf. All rights reserved.
//

#ifndef WriteOBJFIle_h
#define WriteOBJFIle_h

#include <ctime>
#include <fstream>
#include <sstream>

#include "../data/Triangulo.h"
#include "../data/mesh/Malha.h"

extern std::string NAME_MODEL;

class WriteOBJFIle {
 public:
  WriteOBJFIle();
  bool writeMeshOBJFile(Malha* malha, unsigned int passo, int process);
  void writeCurvaturePatches(std::vector<double> vecCurvaturePatches,
                             double maxValue);
  ~WriteOBJFIle();
};

#endif /* WriteOBJFIle_h */
