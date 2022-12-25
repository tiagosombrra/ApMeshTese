#include <string>

#include "../include/data/Definitions.h"
#include "../include/data/Modelo.h"
#include "../include/data/Vertice.h"
#include "../include/data/Vetor.h"
#include "../include/data/curve/curve_adaptive_parametric_bezier.h"
#include "../include/data/curve/curve_adaptive_parametric_hermite.h"
#include "../include/data/patch/BezierPatch.h"
#include "../include/data/patch/HermitePatch.h"
#include "../include/generator/GeradorAdaptativoPorCurvatura.h"
#include "../include/input_output/Modelos3d.h"
#include "../include/input_output/PatchBezierReader.h"
#include "../include/input_output/ReaderPatches.h"
#include "../include/timer/Timer.h"

#if USE_MPI
int RANK_MPI, SIZE_MPI;
#endif  // #if USE_OPENMPI
int PASSOS = 4;
double TRIANGLE_MEDIO = 0.0;
double TOLERANCIA_ESTIMATIVE = 1.0;
// distância entre um parâmetro e outro
double DELTA = 0.0001;
// distância máxima entre dois pontos
double TOLERANCIA = 0.0001;
double TOLERANCIA_CURVATURA = 0.0001;
double TOLERANCIA_AFT = 0.0001;
double PROPORCAO =
    1.0;  // proporção usada no avanço de fronteira (antes era 0.5)
double SUAVIZACAO = 7;  // número de vezes que se dará a suavização laplaciana
double FATOR_SUAVIZACAO = 0.5;  // fator usado na suavização laplaciana
double EPSYLON = 0.0000001;     // trashrold
double DISCRETIZACAO_CURVA = 1.414213562;
double DISCRETIZACAO_INTER = sqrt(DISCRETIZACAO_CURVA);
double TIME_READ_FILE = 0.0;

std::set<Ponto *> LIST_ALL_POINTS_MODEL;
std::set<SubMalha *> LIST_ALL_SUB_MESH_MODEL;
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

  // writeMeshOn ou writeMeshOff da escrita da malha
  WRITE_MESH = argv[4];

  // nome do modelo para escrita da malha de saída
  NAME_MODEL = argv[5];

  // habilita e desabilita a geração por templates
  USE_TEMPLATE = argv[6];

  // contador do tempo de inicialização em segundos em todos os processos
#if USE_MPI
#if USE_OPENMP
  timer->initTimerParallel(RANK_MPI, THREAD_ROOT, 10);  // Full
#else
  timer->initTimerParallel(RANK_MPI, 0, 10);  // Full
#endif
#elif USE_OPENMP
  timer->initTimerParallel(0, THREAD_ROOT, 10);  // Full
#else
  timer->initTimerParallel(0, 0, 10);  // Full
#endif

  GeradorAdaptativoPorCurvatura ger;

#if USE_MPI
  if (ger.execute(argc, argv, timer, status) == 0) {
    cout << endl << "Tempo do processo " << RANK_MPI << endl;

    timer->printTime(RANK_MPI);
    cout << "Método do processo " << RANK_MPI << " com " << argv[2]
         << " thread(s) finalizado com Sucesso!" << endl;
    delete timer;

    return MPI_Finalize();
  }
#else
  if (ger.execute(argc, argv, timer) == 0) {
    cout << "Método com " << argv[1] << " processo(s) e " << argv[2]
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
