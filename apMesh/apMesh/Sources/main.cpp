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
#include "../Headers/Execution/SequentialRun.h"
#include "../Headers/Execution/OmpRun.h"
#include "../Headers/Execution/MpiRun.h"
#include "../Headers/Execution/ParallelRun.h"


double TRIANGLE_MEDIO = 0.0;
double TOLERANCIA_ESTIMATIVE = 1.0;

double DELTA = 0.0001;
double TOLERANCIA = 0.0001;
double TOLERANCIA_CURVATURA = 0.0001;
double TOLERANCIA_AFT = 0.0001;
double PROPORCAO = 0.5; // proporção usada no avanço de fronteira
double SUAVIZACAO = 7; // número de vezes que se dará a suavização laplaciana
double FATOR_SUAVIZACAO = 0.5; // fator usado na suavização laplaciana
double EPSYLON = 0.003;
double DISCRETIZACAO_CURVA = 1.414213562;
double DISCRETIZACAO_INTER = sqrt(DISCRETIZACAO_CURVA);
//int NUM_THREADS = 8;

std::set<Ponto*> listAllPointsModel;
std::set<SubMalha*> listAllSubMalhaModel;
#if USE_MPI
int RANK_MPI, SIZE_MPI;
#endif //#if USE_OPENMPI

std::string nameModel;
std::string entrada;
std::string numberProcess;
std::string WRITE_MESH;

// argv[0] = "executavel: ./apmesh",
// argv[1] = "seq ou omp ou mpi ou par",
// argv[2] = "n° de process"
// argv[3] = "n° threads",
// argv[4] = "entrada",       OBS: Projects-> Comands line arguments -> ../../apMesh/Entrada/mountain_289_patches.bp



int main(int argc, char **argv)
{
    //retorno do programa
    int finish = -1;

    // contador do tempo para carregar a malha na memória
    // (sizeRank, sizeThread, sizeType)
    Timer *timer = new Timer(atoi(argv[2]), atoi(argv[3]), 11);

    // contador do tempo de inicialização em segundos em todos os processos
    timer->initTimerParallel(0, 0, 10); // Full
    timer->initTimerParallel(0, 0, 0);  // Inicialização

    if (argv[1] == string("seq")) {
        //sequencial
        SequentialRun seq;
        if (seq.execute(argv, timer) == 0) {
            cout << "Método Sequencial Finalizado com Sucesso!" << endl;
            finish = 0;
        } else {
            cout << "Erro no Método Sequencial!" << endl;
        }

    } else if(argv[1] == string("omp")) {
        //paralelo
        OmpRun omp;
        if (omp.execute(argc, argv, timer) == 0) {
            cout << "Método Omp Finalizado com Sucesso!" << endl;
            finish = 0;
        } else {
            cout << "Erro no Método Omp!" << endl;
        }

    } else if(argv[1] == string("mpi")) {
        //paralelo
        MpiRun mpi;
        if (mpi.execute(argc, argv, timer) == 0) {
            cout << "Método Mpi Finalizado com Sucesso!" << endl;
            finish = 0;
        } else {
            cout << "Erro no Método Mpi!" << endl;
        }

    } else if(argv[1] == string("par")) {
        //paralelo
        ParallelRun par;
        if (par.execute(argc, argv, timer) == 0) {
            cout << "Método Paralelo Finalizado com Sucesso!" << endl;
            finish = 0;
        } else {
            cout << "Erro no Método Paralelo!" << endl;
        }

    } else {
        cout<<"Erro na inserção dos parâmetros de entrada"<<endl;
    }

    return finish;
}
