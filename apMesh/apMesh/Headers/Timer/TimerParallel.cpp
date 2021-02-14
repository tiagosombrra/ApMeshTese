#include "TimerParallel.h"

template<int rank, int thread, int type>
TimerParallel<rank, thread, type>::TimerParallel()
{
    for (int i = 0; i < rank; ++i) {
        for (int j = 0; j < thread; ++j) {
            for (int l = 0; l < type; ++l) {
                this->vecTimerParallelInit[i][j][j] = 0;
                this->vecTimerParallelEnd[i][j][j] = 0;
                this->vecTimerParallel[i][j][j] = 0;
            }
        }
    }
}

template<int rank, int thread, int type>
void TimerParallel<rank, thread, type>::initTimerParallel(int _rank, int _thread, int _type)
{
    time_clock = clock();
    this->vecTimerParallelInit[_rank][_thread][_type] = (double)time_clock;
}

template<int rank, int thread, int type>
void TimerParallel<rank, thread, type>::endTimerParallel(int _rank, int _thread, int _type)
{
    time_clock = clock();
    this->vecTimerParallelEnd[_rank][_thread][_type] = (double)time_clock;
}

template<int rank, int thread, int type>
void TimerParallel<rank, thread, type>::calculateTime(int _rank, int _thread, int _type)
{
    this->vecTimerParallel[_rank][_thread][_type] += this->vecTimerParallelEnd[_rank][_thread][_type] / CLOCKS_PER_SEC - this->vecTimerParallelInit[_rank][_thread][_type] / CLOCKS_PER_SEC;

}

template<int rank, int thread, int type>
void TimerParallel<rank, thread, type>::printTime(int _rank, int _thread)
{
    cout << "Rank: " <<_rank<< " Thread: "<<_thread<< endl<<endl;
    cout << "Inicialização: " << this->vecTimerParallel[_rank][_thread][0] << endl;
    cout << "Estimativa de carga: " << this->vecTimerParallel[_rank][_thread][1] << endl;
    cout << "Geração da malha inicial: " << this->vecTimerParallel[_rank][_thread][2] << endl;
    cout << "Adaptação das curvas: " << this->vecTimerParallel[_rank][_thread][3] << endl;
    cout << "Adaptação do domínio: " << this->vecTimerParallel[_rank][_thread][4] << endl;
    cout << "Quadtree: " << this->vecTimerParallel[_rank][_thread][5] << endl;
    cout << "Avanço de front.: " << this->vecTimerParallel[_rank][_thread][6] << endl;
    cout << "Calculo do erro: " << this->vecTimerParallel[_rank][_thread][7] << endl;
    cout << "Overhead: " << this->vecTimerParallel[_rank][_thread][8] << endl;
    cout << "SendRecv: " << this->vecTimerParallel[_rank][_thread][9] << endl;
    cout << "Full: " << this->vecTimerParallel[_rank][_thread][10] << endl<<endl;

}
