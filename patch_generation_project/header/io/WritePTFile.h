#ifndef WRITEPTFILE_H
#define WRITEPTFILE_H

#include "../basic/Definitions.h"
#include <iostream>
#include <fstream>
#include <iomanip>

class WritePTFile
{
public:
    WritePTFile();
    void writerPTFile(std::string, VectorTuple48d);
    void writeBPFile(std::string, BP);
    void writeObjFile(std::string, VectorTuplePoints);

};

#endif // WRITEPTFILE_H
