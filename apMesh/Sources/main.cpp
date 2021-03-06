#include "../Headers/Data/Patch/HermitePatch.h"
#include "../Headers/Data/Patch/BezierPatch.h"
#include "../Headers/Data/Vertice.h"
#include "../Headers/Data/Vetor.h"
#include "../Headers/Data/Curve/CurvParamHermite.h"
#include "../Headers/Data/Curve/CurvParamBezier.h"
#include "../Headers/Data/Modelo.h"
#include "../Headers/Generator/GeradorAdaptativoPorCurvatura.h"
#include "../Headers/IO/Modelos3d.h"
#include "../Headers/Data/Definitions.h"
#include "../Headers/IO/ReaderPatches.h"
#include "../Headers/Timer/Timer.h"
#include "../Headers/IO/PatchBezierReader.h"

#if USE_MPI
int RANK_MPI, SIZE_MPI;
#endif //#if USE_OPENMPI
int PASSOS = 2;
double TRIANGLE_MEDIO = 0.0;
double TOLERANCIA_ESTIMATIVE = 1.0;
double DELTA = 0.0001;
double TOLERANCIA = 0.0001;
double TOLERANCIA_CURVATURA = 0.0001;
double TOLERANCIA_AFT = 0.0001;
double PROPORCAO = 0.5; // proporção usada no avanço de fronteira
double SUAVIZACAO = 7; // número de vezes que se dará a suavização laplaciana
double FATOR_SUAVIZACAO = 0.5; // fator usado na suavização laplaciana
double EPSYLON = 5.0; //trashrold
double DISCRETIZACAO_CURVA = 1.414213562;
double DISCRETIZACAO_INTER = sqrt(DISCRETIZACAO_CURVA);
double TIME_READ_FILE = 0.0;

std::set<Ponto*> listAllPointsModel;
std::set<SubMalha*> listAllSubMalhaModel;
std::string nameModel;
std::string entrada;
std::string numberProcess;
std::string WRITE_MESH;

// argv[0] = "executavel: ./apmesh",
// argv[1] = "n° de process"
// argv[2] = "n° threads",
// argv[3] = "entrada",       OBS: Projects-> Comands line arguments -> ../../apMesh/Entrada/mountain_289_patches.bp
// argv[4] = "WRITE_MESH" (m)
// argv[5] = "nameModel"


int main(int argc, char **argv)
{

#if USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &SIZE_MPI);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK_MPI);
    MPI_Status status;

#endif

    // contador do tempo para carregar a malha na memória
    // (sizeRank, sizeThread, sizeType)
    Timer *timer = new Timer(atoi(argv[1]), atoi(argv[2]), 11);

    //cout<<atoi(argv[1])<<atoi(argv[2])<<endl;
    //cout<<RANK_MPI<<THREAD_ROOT<<endl;

    //writeMeshOn ou writeMeshOff da escrita da malha
    WRITE_MESH = argv[4];

    //nome do modelo para escrita da malha de saída
    nameModel = argv[5];

    // contador do tempo de inicialização em segundos em todos os processos
#if USE_MPI
#if USE_OPENMP
    timer->initTimerParallel(RANK_MPI, THREAD_ROOT, 10); // Full
#else
    timer->initTimerParallel(RANK_MPI, 0, 10); // Full
#endif
#elif USE_OPENMP
    timer->initTimerParallel(0, THREAD_ROOT, 10); // Full
#else
    timer->initTimerParallel(0, 0, 10); // Full
#endif

    GeradorAdaptativoPorCurvatura ger;

#if USE_MPI
    if (ger.execute(argc, argv, timer, status) == 0) {

        cout<<endl<<"Tempo do processo "<<RANK_MPI<<endl;

        timer->printTime(RANK_MPI);
        cout << "Método do processo "<<RANK_MPI<<" com "<<argv[2]<< " thread(s) finalizado com Sucesso!" << endl;
        delete timer;

        return MPI_Finalize();
    }
#else
    if (ger.execute(argc, argv, timer) == 0) {
        cout << "Método com "<<argv[1]<<" processo(s) e "<<argv[2]<< " thread(s) finalizado com Sucesso!" << endl;
        return 0;
    }else if(argc < 4){
        cout<<"Erro!!! Apenas"<<argc<<" parâmetros inseridos, quantidade correta é 5 parâmetros"<<endl;
        return -1;
    }else{
        cout << "Erro na execução no método main()"<<endl;
        return -1;
    }
#endif

}
