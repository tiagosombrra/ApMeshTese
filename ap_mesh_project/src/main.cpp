#include <string>

#include "../include/data/curve/curve_adaptive_parametric_bezier.h"
#include "../include/data/curve/curve_adaptive_parametric_hermite.h"
#include "../include/data/definitions.h"
#include "../include/data/model.h"
#include "../include/data/patch/patch_bezier.h"
#include "../include/data/patch/patch_hermite.h"
#include "../include/data/vector_adaptive.h"
#include "../include/data/vertex_adaptive.h"
#include "../include/generator/generator_adaptive.h"
#include "../include/input_output/models_3d.h"
#include "../include/input_output/patch_reader.h"
#include "../include/timer/timer.h"

#if USE_MPI
int RANK_MPI, SIZE_MPI;
#endif  // #if USE_OPENMPI
int STEPS = 3;
double TRIANGLE_MEDIO = 0.0;
double ESTIMATIVE_TOLERANCE = 1.0;
// distância entre um parâmetro e outro
double DELTA = 0.0001;
// distância máxima entre dois pontos
double TOLERANCE = 0.0001;
double TOLERANCE_CURVATURE = 0.0001;
double TOLERANCE_AFT = 0.0001;
// proporção usada no avanço de fronteira (antes era 0.5)
double RATIO_AFT = 1.0;
// número de vezes que se dará a suavização laplaciana
double SMOOTHING_LAPLACIAN_NUMBER = 7;
// fator usado na suavização laplaciana
double SMOOTHING_LAPLACIAN_FACTOR = 0.5;
double EPSYLON = 0.0000001;  // trashrold
double DISCRETIZATION_CURVE_FACTOR = 1.414213562;
double DISCRETIZATION_CURVE_FACTOR_INTERNAL = sqrt(DISCRETIZATION_CURVE_FACTOR);
double TIME_READ_FILE = 0.0;
unsigned int I_MAX = 50000;  // i maximo do método findUV()
double MAX_TIME = 999999999;
int MAX_THREADS = 1;
std::set<PointAdaptive *> LIST_ALL_POINTS_MODEL;
std::set<SubMesh *> LIST_ALL_SUB_MESH_MODEL;
std::string NAME_MODEL;
std::string INPUT_MODEL;
std::string NUMBER_PROCESS;
std::string WRITE_MESH;
std::string USE_TEMPLATE;

// argv[0] = "executavel: ./apmesh",
// argv[1] = "n° de process"
// argv[2] = "n° threads",
// argv[3] = "INPUT_MODEL",       OBS: Projects-> Comands line arguments ->
// ../../apMesh/INPUT_MODEL/mountain_289_patches.bp
// argv[4] = "WRITE_MESH" (m)
// argv[5] = "NAME_MODEL"
// argv[6] = "USE_TEMPLATE" -> y or n

int main(int argc, char **argv) {
#if USE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &SIZE_MPI);
  MPI_Comm_rank(MPI_COMM_WORLD, &RANK_MPI);
  MPI_Status status;

  if (atoi(argv[1]) < SIZE_MPI) {
    SIZE_MPI = atoi(argv[1]);
  }

#endif

  if (argc < 7) {
    cout << "Erro!!! Number of parameters is incorrect (" << argc
         << ") , correct number is 7." << endl;
    return -1;
  }

  // contador do tempo para carregar a malha na memória
  // (sizeRank, sizeThread, sizeType)
  Timer *timer = new Timer(atoi(argv[1]), atoi(argv[2]), 11);

  // cout<<atoi(argv[1])<<atoi(argv[2])<<endl;
  // cout<<RANK_MPI<<THREAD_ROOT<<endl;
  INPUT_MODEL = argv[3];

  // writeMeshOn ou writeMeshOff da escrita da malha
  WRITE_MESH = argv[4];

  // nome do modelo para escrita da malha de saída
  NAME_MODEL = argv[5];

  // habilita e desabilita a geração por templates
  USE_TEMPLATE = argv[6];

  // contador do tempo de inicialização em segundos em todos os processos
#if USE_MPI
#if USE_OPENMP
  timer->InitTimerParallel(RANK_MPI, THREAD_ROOT, 10);  // Full
#else
  timer->InitTimerParallel(RANK_MPI, 0, 10);  // Full
#endif
#elif USE_OPENMP
  timer->InitTimerParallel(0, THREAD_ROOT, 10);  // Full
#else
  timer->InitTimerParallel(0, 0, 10);  // Full
#endif

  GeneratorAdaptive generator;

#if USE_MPI
  if (generator.Execute(argv, timer, status) == 0) {
    cout << "Método do processo " << RANK_MPI << " com " << argv[2]
         << " thread(s) finalizado com Sucesso!" << endl;

    return MPI_Finalize();
  } else {
    cout << endl << "Erro na execução generator.Execute()." << RANK_MPI << endl;
  }
#else
  if (generator.Execute(argv, timer) == 0) {
    cout << "Método com " << argv[1] << " processo(s) e " << MAX_THREADS
         << " thread(s) finalizado com Sucesso!" << endl;

    return 0;
  } else if (argc < 4) {
    cout << "Erro!!! Apenas" << argc
         << " parâmetros inseridos, quantidade correta é 5 parâmetros" << endl;

    return -1;
  } else {
    cout << "Erro na execução no método main()" << endl;

    return -1;
  }
#endif
}
