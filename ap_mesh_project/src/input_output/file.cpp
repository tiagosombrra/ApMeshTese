#include "../../include/input_output/file.h"

File::File(const char *name) {
  this->input_model_.open(name);
  if (this->input_model_.fail())
    cout << "não abriu o arquivo em disco!" << endl;
  else
    this->name_ = name;
}

File::~File() { this->input_model_.close(); }

// ler as linhas que definem as curvas
void File::ReadCurves(const string &reading) {
  // "P" de point_1_ ou P2, os pontos inicial e final da curva
  // "D" de DP1 ou DP2, as derivadas nos pontos inicial e final
  if (reading[0] == 'P' or reading[0] == 'D') this->curves_.push_back(reading);
}

// ler as linhas que definem os patches
void File::ReadPatches(const string &reading) {
  // "D" de DEFINE_PATCH
  if (reading[0] == 'D') this->patches_.push_back(reading);
}

string File::GetName() { return name_; }

std::vector<double> File::GetDoubles(const std::string &str) {
  std::vector<double> result;
  std::string sub;

  size_t start = 0, end = 0;
  while (end != std::string::npos) {
    end = str.find_first_of(" <>,", start);
    sub = str.substr(start, end - start);

    if (!sub.empty()) {
      double value = std::stod(sub, nullptr);
      result.push_back(value);
    }

    start = str.find_first_not_of(" <>,", end);
  }

  return result;
}

// criar as curvas
void File::CreateCurvesTo() {
  for (const auto &curve : curves_) {
    auto points = GetDoubles(curve);
    // substituir pelo construtor de curvas
    cout << "\nContrui uma curva com ponto inicial ( " << points[0] << ", "
         << points[1] << ", " << points[2] << ")\n"
         << "ponto final: (" << points[3] << ", " << points[4] << ", "
         << points[5] << ")\n"
         << "Derivada no ponto inicial: (" << points[6] << ", " << points[7]
         << ", " << points[8] << ")\n"
         << "Derivada no ponto final: (" << points[9] << ", " << points[10]
         << ", " << points[11] << ")" << endl;
  }
}

// criar os patches
void File::CreatePatchesTo() {
  for (const auto &patch : patches_) {
    auto points = GetDoubles(patch);
    for (const auto &point : points) {
      cout << "point_patch: " << point << endl;
    }
  }
}

// ler um arquivo para definir um Modelo
void File::ReadFileTo() {
  string line;  // linha lida do arquivo

  string init_curves = "CURVAS_HERMITE";        // inicio do bloco de curvas
  bool read_curves = false;                     // pode ler uma linha de curva
  string end_of_curves = "FIM_CURVAS_HERMITE";  // fim do bloco de curvas

  string init_patches = "PATCHS_HERMITE";  // inicio do bloco de patches
  bool read_patches = false;               // pode ler uma linha de patch
  string end_of_patches = "FIM_DE_PATCHS_HERMITES";  // fim do bloco de patches

  while (input_model_.good()) {
    getline(input_model_, line);  // pega uma linha do arquivo

    if (line == init_curves)  // começa a definição das curvas no arquivo
    {
      read_curves = true;  // você pode ler as linhas e definir os pontos e suas
      // derivadas das curvas
      continue;
    } else if (line == end_of_curves)  // não há mais definições de curvas
    {
      read_curves = false;  // pare de definir os pontos e derivadas
      continue;
    } else if (line == "")  // ignore linhas em branco
      continue;
    else if (read_curves)  // se está dentro de um bloco de definição de curvas
      ReadCurves(line);    // leia a linha e forme uma curva
    else if (line == init_patches)  // começa a definição dos patches no arquivo
    {
      read_patches = true;  // você pode ler as linhas e definir os patches
      continue;
    } else if (line == end_of_patches)  // não há mais definições de patches
    {
      read_patches = false;  // pare de definir os patches
      break;
    } else if (read_patches)  // leia a linha e defina um patch
      ReadPatches(line);
  }  // fim do while
  CreateCurvesTo();
  CreatePatchesTo();
}
