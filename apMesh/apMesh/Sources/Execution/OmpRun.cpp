#include "../../Headers/Execution/OmpRun.h"

OmpRun::OmpRun()
{

}

int OmpRun::execute(int argc, char *argv[], Timer *timer)
{
    Modelo M;
    Geometria *geo = new Geometria;
    ReaderPatches read;

    //Inclusão dos patches no Modelo
    if (argv[4])
    {
        M.setGeometria(read.readerPatches(geo, argv[4]));
    }
    else
    {
        Modelos3d modelos3d;
        M.setGeometria ( modelos3d.modelPlanBook(geo) );
    }

    // fim do contador do tempo para carregar a malha na memória
    timer->endTimerParallel(0, 0, 0); // inicialização

    // contador do tempo total
    timer->initTimerParallel(0, 0, 10); // Full

    //Gerador da malha
#if USE_OPENMP
    GeradorAdaptativoPorCurvatura adaptiveMesh;
            adaptiveMesh.generatorOmp(M, timer, 1024, atoi(argv[2]), atoi(argv[3]));

#endif //USE_OPENMP

    //fim do contador do tempo total
    timer->endTimerParallel(0, 0, 10); // Full

#if USE_PRINT_TIME
    timer->printTime();
#endif //#if USE_PRINT_TIME

#if USE_PRINT_ERRO
#if USE_OPENMP
    cout << "Erro global final = " << adaptiveMesh.erro << endl;
#endif //USE_OPENMP
#endif //#if USE_PRINT_ERRO

    return 0;
}
