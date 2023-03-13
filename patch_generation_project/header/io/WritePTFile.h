#ifndef WRITEPTFILE_H
#define WRITEPTFILE_H

#include <fstream>
#include <iomanip>
#include <iostream>

#include "../basic/Definitions.h"

class WritePTFile {
 public:
  WritePTFile();
  void writerPTFile(std::string, VectorTuple48d);
  void writeBPFile(std::string, BP);
  void writeObjFile(std::string, VectorTuplePoints);
};

#endif  // WRITEPTFILE_H
