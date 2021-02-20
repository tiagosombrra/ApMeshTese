#include "../../Headers/Execution/SequentialRun.h"

SequentialRun::SequentialRun()
{

}

int SequentialRun::execute(char *argv[], Timer *timer)
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
        M.setGeometria ( modelos3d.modelLadoDescendente(geo) );
    }

    // fim do contador do tempo para carregar a malha na memória
    timer->endTimerParallel(0, 0, 0); // inicialização

    //Gerador da malha
    GeradorAdaptativoPorCurvatura adaptiveMesh (M, timer, 8);

    //fim do contador do tempo total
    timer->endTimerParallel(0, 0, 10); // Full

#if USE_PRINT_TIME
    timer->printTime();
#endif //#if USE_PRINT_TIME

#if USE_PRINT_ERRO
    cout << "Erro global final = " << adaptiveMesh.erro << endl;
#endif //#if USE_PRINT_ERRO

    return 0;
}
