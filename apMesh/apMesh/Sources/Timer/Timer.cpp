#include "../../Headers/Timer/Timer.h"

Timer::Timer()
{
    this->timerParallelInit.resize(1);
    this->timerParallelEnd.resize(1);
    this->timerParallel.resize(1);

    this->timerParallelInit[0].resize(1);
    this->timerParallelEnd[0].resize(1);
    this->timerParallel[0].resize(1);

    this->timerParallelInit[0][0].resize(11);
    this->timerParallelEnd[0][0].resize(11);
    this->timerParallel[0][0].resize(11);

    for (int i = 0; i < 11; ++i) {
       // this->timerParallelInit[0][0][i] = 0;
       // this->timerParallelEnd[0][0][i] = 0;
        this->timerParallel[0][0][i] = 0;
    }
}

Timer::Timer(int sizeRank, int sizeThread, int sizeType)
{
    sizeRank = (sizeRank == 0) ? 1: sizeRank;
    sizeThread = (sizeThread == 0) ? 1: sizeThread;

    this->timerParallelInit.resize(sizeRank);
    this->timerParallelEnd.resize(sizeRank);
    this->timerParallel.resize(sizeRank);

    for (int i = 0; i < sizeRank; ++i) {

        this->timerParallelInit[i].resize(sizeThread);
        this->timerParallelEnd[i].resize(sizeThread);
        this->timerParallel[i].resize(sizeThread);

        for (int j = 0; j < sizeThread; ++j) {

            this->timerParallelInit[i][j].resize(sizeType);
            this->timerParallelEnd[i][j].resize(sizeType);
            this->timerParallel[i][j].resize(sizeType);

            for (int l = 0; l < sizeType; ++l) {

               // this->timerParallelInit[i][j][l] = 0;
               // this->timerParallelEnd[i][j][l] = 0;
                this->timerParallel[i][j][l] = 0;
            }
        }
    }
}

bool Timer::openFile(string locationName)
{
    // Metodo de criação(caso não exista) ou abertura de arquivo(caso exista).

    // localização e nome do arquivo
    this->locationName = locationName;
    file.open(locationName.c_str(), fstream::out | fstream::ate);
    file << "Arquivo gerado pelo apMesh para medir o tempo detalaha de execução do programa" << endl << endl;

    return file.is_open();
}

void Timer::closeFile()
{
    file.close();
}

bool Timer::deleteFile(string nameFile)
{
    if (std::remove(nameFile.c_str()) != 0) {
        // perror("Error deleting file");
        return false;
    } else {
        // puts("File successfully deleted");
        return true;
    }
}

void Timer::initTimerParallel(int _rank, int _thread, int _type)
{

    //time_clock = clock();
    //this->timerParallelInit[_rank][_thread][_type] = (double)time_clock;
    timeval begin;
    gettimeofday(&begin, 0);
    this->timerParallelInit[_rank][_thread][_type] = begin;

}

void Timer::endTimerParallel(int _rank, int _thread, int _type)
{
//    time_clock = clock();
//    this->timerParallelEnd[_rank][_thread][_type] = (double)time_clock;
//    calculateTime(_rank, _thread, _type);
    timeval end;
    gettimeofday(&end, 0);
    this->timerParallelEnd[_rank][_thread][_type] = end;
    calculateTime(_rank, _thread, _type);
}

void Timer::calculateTime(int _rank, int _thread, int _type)
{
//    this->timerParallel[_rank][_thread][_type] += this->timerParallelEnd[_rank][_thread][_type]
//            / CLOCKS_PER_SEC - this->timerParallelInit[_rank][_thread][_type] / CLOCKS_PER_SEC;
    long seconds = (this->timerParallelEnd[_rank][_thread][_type]).tv_sec - (this->timerParallelInit[_rank][_thread][_type]).tv_sec;
    long microseconds = (this->timerParallelEnd[_rank][_thread][_type]).tv_usec - (this->timerParallelInit[_rank][_thread][_type]).tv_usec;
    this->timerParallel[_rank][_thread][_type] += seconds + microseconds*1e-6;

}

void Timer::printTime()
{
    vector<double> max = getMaxTime();
    cout << "Inicialização: " << max[0] << endl;
    cout << "Estimativa de carga: " << max[1] << endl;
    cout << "Geração da malha inicial: " << max[2] << endl;
    cout << "Adaptação das curvas: " << max[3] << endl;
    cout << "Adaptação do domínio: " << max[4] << endl;
    cout << "Quadtree: " << max[5] << endl;
    cout << "Avanço de front.: " << max[6] << endl;
    cout << "Calculo do erro: " << max[7] << endl;
    cout << "Overhead: " << max[8] << endl;
    cout << "SendRecv: " << max[9] << endl;
    cout << "Full: " << max[10] << endl<<endl;

}

double Timer::getRankThreadTime(int _rank, int _thread, int _type)
{
    return timerParallel[_rank][_thread][_type];
}

vector<double> Timer::getMaxTime()
{
    vector<double> max;
    max.resize(this->timerParallel[0][0].size());

    for (unsigned int i = 0; i < this->timerParallel[0][0].size(); ++i) {
        max[i] = 0;
    }

    for (unsigned int i = 0; i < this->timerParallel.size(); ++i) {
        for (unsigned int j = 0; j < timerParallel[i].size(); ++j) {
            for (unsigned int l = 0; l < this->timerParallel[i][j].size(); ++l) {
                if (this->timerParallel[i][j][l] > max[l]) {
                    max[l] = this->timerParallel[i][j][l];
                }
            }
        }
    }

    return max;
}

vector<vector<vector<timeval> > > Timer::getTimerParallelInit() const
{
    return timerParallelInit;
}

void Timer::setTimerParallelInit(const vector<vector<vector<timeval> > > &value)
{
    timerParallelInit = value;
}

vector<vector<vector<timeval> > > Timer::getTimerParallelEnd() const
{
    return timerParallelEnd;
}

void Timer::setTimerParallelEnd(const vector<vector<vector<timeval> > > &value)
{
    timerParallelEnd = value;
}

vector<vector<vector<double> > > Timer::getTimerParallel() const
{
    return timerParallel;
}

void Timer::setTimerParallel(const vector<vector<vector<double> > > &value)
{
    timerParallel = value;
}
