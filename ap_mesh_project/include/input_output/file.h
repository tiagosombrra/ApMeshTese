#ifndef INPUT_OUTPUT_FILE_H
#define INPUT_OUTPUT_FILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../data/model.h"

class File {
 public:
  explicit File(const char* name);
  ~File();

  string GetName();
  void ReadFileTo();

 protected:
  void ReadCurves(const string& reading);   // lê todas as curvas do arquivo
  void ReadPatches(const string& reading);  // lê todos os patches do arquivo
  void CreateCurvesTo();                    // cria as curvas a partir da list
  void CreatePatchesTo();                   // cria os patches a partir da list
  std::vector<double> GetDoubles(const std::string& str);

  std::string name_;
  std::ifstream input_model_;
  std::vector<string> curves_;
  std::vector<string> patches_;
};
#endif  // INPUT_OUTPUT_FILE_H
