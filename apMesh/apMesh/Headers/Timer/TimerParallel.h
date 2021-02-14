#ifndef TIMERPARALLEL_H
#define TIMERPARALLEL_H

#include "../Data/Definitions.h"

//[0]= Inicialização
//[1]= Estimativa de carga
//[2]= Geração da Malha Inicial
//[3]= Adaptação das curvas
//[4]= Adaptação do domínio
//[5]= Quadtree
//[6]= Avanço de front.
//[7]= Calculo do erro
//[8]= Overhead
//[9]= Timer send and recv process
//[10]= Full

template<int rank = 1, int thread = 1, int type = 11>
class TimerParallel
{

public:
    TimerParallel();

    void initTimerParallel(int _rank, int _thread, int _type);
    void endTimerParallel(int _rank, int _thread, int _type);
    void calculateTime(int _rank, int _thread, int _type);
    void printTime(int _rank, int _thread);



private:
    double vecTimerParallelInit[rank][thread][type];
    double vecTimerParallelEnd[rank][thread][type];
    double vecTimerParallel[rank][thread][type];

    clock_t time_clock;
};

#endif // TIMERPARALLEL_H
