#ifndef INPUT_OUTPUT_FILE_H
#define INPUT_OUTPUT_FILE_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "../data/model.h"

class File {
 public:
  File(const char* name);
  ~File();

  string GetName();
  void ReadFileTo();

 protected:
  void ReadCurves(const string reading);   // lê todas as curvas do arquivo
  void ReadPatches(const string reading);  // lê todos os patches do arquivo
  void CreateCurvesTo();                   // cria as curvas a partir da list
  void CreatePatchesTo();                  // cria os patches a partir da list
  char* ConvertString(string font);

  string name_;
  ifstream INPUT_MODEL;
  list<string> curves_;
  list<string> patches_;
};
#endif  // INPUT_OUTPUT_FILE_H
