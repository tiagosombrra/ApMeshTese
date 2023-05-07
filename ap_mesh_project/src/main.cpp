#include <memory>
#include <string>

#include "../include/data/curve/curve_adaptive_parametric_bezier.h"
#include "../include/data/curve/curve_adaptive_parametric_hermite.h"
#include "../include/data/model.h"
#include "../include/data/patch/patch_bezier.h"
#include "../include/data/patch/patch_hermite.h"
#include "../include/data/vector_adaptive.h"
#include "../include/data/vertex_adaptive.h"
#include "../include/definitions.h"
#include "../include/generator/generator_adaptive.h"
#include "../include/input_output/models_3d.h"
#include "../include/input_output/patch_reader.h"
#include "../include/timer/timer.h"

// Global const and variables

int kSteps = 3;
double kEstimativeTolerance = 1.0;

// distância entre um parâmetro e outro
double kDelta = 0.0001;

// distância máxima entre dois pontos
double kTolerance = 0.0001;

double kToleranceCurvature = 0.0001;
double kToleranceAft = 0.0001;

// proporção usada no avanço de fronteira (antes era 0.5)
double kRatioAft = 1.0;

// número de vezes que se dará a suavização laplaciana
double kSmoothingLaplacianNumber = 7;

// fator usado na suavização laplaciana
double kSmoothingLaplacianFactor = 0.5;

// trashrold
double kEpsylon = 0.0000001;
double kDiscretizationCurveFactor = 1.414213562;

// i maximo do método findUV()
unsigned int kImax = 50000;

double kMaxTime = 999999999.0;

#if USE_MPI
int rankMpi, sizeMpi;
#endif  // #if USE_MPI

double timeReadFile = 0.0;
int maxThreads = 1;

std::string nameModel;
std::string inputModel;
std::string numberProcess;
std::string writeMesh;
std::string useTemplate;

// extern int sizeMpi, rankMpi, maxThreads;
// extern std::string inputModel, writeMesh, nameModel, useTemplate;

// argv[0] = "executavel: ./apmesh",
// argv[1] = "n° de process"
// argv[2] = "n° threads",
// argv[3] = "inputModel",       OBS: Projects-> Comands line arguments ->
// ../../apMesh/inputModel/mountain_289_patches.bp
// argv[4] = "writeMesh" (m)
// argv[5] = "nameModel"
// argv[6] = "useTemplate" -> y or n

int main(int argc, char **argv) {
#if USE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeMpi);
  MPI_Comm_rank(MPI_COMM_WORLD, &rankMpi);
  MPI_Status status;

  if (atoi(argv[1]) < sizeMpi) {
    sizeMpi = atoi(argv[1]);
  }

#endif

  if (argc < 7) {
    cout << "Erro!!! Number of parameters is incorrect (" << argc
         << ") , correct number is 7." << endl;
    return -1;
  }

  // contador do tempo para carregar a malha na memória
  // (sizeRank, sizeThread, sizeType)
  std::unique_ptr<Timer> timer =
      std::make_unique<Timer>(atoi(argv[1]), atoi(argv[2]), 11);

  // cout<<atoi(argv[1])<<atoi(argv[2])<<endl;
  // cout<<rankMpi<<THREAD_ROOT<<endl;
  inputModel = argv[3];

  // writeMeshOn ou writeMeshOff da escrita da malha
  writeMesh = argv[4];

  // nome do modelo para escrita da malha de saída
  nameModel = argv[5];

  // habilita e desabilita a geração por templates
  useTemplate = argv[6];

  // contador do tempo de inicialização em segundos em todos os processos
#if USE_MPI
#if USE_OPENMP
  timer->InitTimerParallel(rankMpi, THREAD_ROOT, 10);  // Full
#else
  timer->InitTimerParallel(rankMpi, 0, 10);  // Full
#endif
#elif USE_OPENMP
  timer->InitTimerParallel(0, THREAD_ROOT, 10);  // Full
#else
  timer->InitTimerParallel(0, 0, 10);  // Full
#endif

  GeneratorAdaptive generator;

#if USE_MPI
  if (generator.Execute(argv, *timer, status) == 0) {
    cout << "Método do processo " << rankMpi << " com " << argv[2]
         << " thread(s) finalizado com Sucesso!" << endl;

    return MPI_Finalize();
  } else {
    cout << endl << "Erro na execução generator.Execute()." << rankMpi << endl;
  }
#else
  if (generator.Execute(argv, *timer.get()) == 0) {
    cout << "Método com " << argv[1] << " processo(s) e " << maxThreads
         << " thread(s) finalizado com Sucesso!" << endl;
    return 0;
  } else {
    cout << "Erro na execução no método main()" << endl;
    return -1;
  }
#endif
}
