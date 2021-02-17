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
        this->timerParallelInit[0][0][i] = 0;
        this->timerParallelEnd[0][0][i] = 0;
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

                this->timerParallelInit[i][j][l] = 0;
                this->timerParallelEnd[i][j][l] = 0;
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
    time_clock = clock();
    this->timerParallelInit[_rank][_thread][_type] = (double)time_clock;
}

void Timer::endTimerParallel(int _rank, int _thread, int _type)
{
    time_clock = clock();
    this->timerParallelEnd[_rank][_thread][_type] = (double)time_clock;
    calculateTime(_rank, _thread, _type);
}

void Timer::calculateTime(int _rank, int _thread, int _type)
{
    this->timerParallel[_rank][_thread][_type] += this->timerParallelEnd[_rank][_thread][_type]
            / CLOCKS_PER_SEC - this->timerParallelInit[_rank][_thread][_type] / CLOCKS_PER_SEC;

}

void Timer::printTime(int _rank, int _thread)
{
    cout << "Rank: " <<_rank<< " Thread: "<<_thread<<endl;
    cout << "Inicialização: " << this->timerParallel[_rank][_thread][0] << endl;
    cout << "Estimativa de carga: " << this->timerParallel[_rank][_thread][1] << endl;
    cout << "Geração da malha inicial: " << this->timerParallel[_rank][_thread][2] << endl;
    cout << "Adaptação das curvas: " << this->timerParallel[_rank][_thread][3] << endl;
    cout << "Adaptação do domínio: " << this->timerParallel[_rank][_thread][4] << endl;
    cout << "Quadtree: " << this->timerParallel[_rank][_thread][5] << endl;
    cout << "Avanço de front.: " << this->timerParallel[_rank][_thread][6] << endl;
    cout << "Calculo do erro: " << this->timerParallel[_rank][_thread][7] << endl;
    cout << "Overhead: " << this->timerParallel[_rank][_thread][8] << endl;
    cout << "SendRecv: " << this->timerParallel[_rank][_thread][9] << endl;
    cout << "Full: " << this->timerParallel[_rank][_thread][10] << endl<<endl;

}

double Timer::getRankThreadTime(int _rank, int _thread, int _type)
{
    return timerParallel[_rank][_thread][_type];
}

vector<vector<vector<double> > > Timer::getTimerParallelInit() const
{
    return timerParallelInit;
}

void Timer::setTimerParallelInit(const vector<vector<vector<double> > > &value)
{
    timerParallelInit = value;
}

vector<vector<vector<double> > > Timer::getTimerParallelEnd() const
{
    return timerParallelEnd;
}

void Timer::setTimerParallelEnd(const vector<vector<vector<double> > > &value)
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
