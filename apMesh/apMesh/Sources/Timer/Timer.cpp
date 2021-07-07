#include "../../Headers/Timer/Timer.h"

Timer::Timer()
{
    this->timerParallelInit.resize(1);
    this->timerParallelEnd.resize(1);
    this->timerParallelInitMpi.resize(1);
    this->timerParallelEndMpi.resize(1);
    this->timerParallel.resize(1);

    this->timerParallelInit[0].resize(1);
    this->timerParallelEnd[0].resize(1);
    this->timerParallelInitMpi[0].resize(1);
    this->timerParallelEndMpi[0].resize(1);
    this->timerParallel[0].resize(1);

    this->timerParallelInit[0][0].resize(11);
    this->timerParallelEnd[0][0].resize(11);
    this->timerParallelInitMpi[0][0].resize(11);
    this->timerParallelEndMpi[0][0].resize(11);
    this->timerParallel[0][0].resize(11);

    for (int i = 0; i < 11; i++) {
        this->timerParallel[0][0][i] = 0.0;
    }
}

Timer::Timer(int sizeRank, int sizeThread, int sizeType)
{
    sizeRank = (sizeRank == 0) ? 1: sizeRank;
    sizeThread = (sizeThread == 0) ? 1: sizeThread;

    this->timerParallelInit.resize(sizeRank);
    this->timerParallelEnd.resize(sizeRank);
    this->timerParallelInitMpi.resize(sizeRank);
    this->timerParallelEndMpi.resize(sizeRank);
    this->timerParallel.resize(sizeRank);

    for (int i = 0; i < sizeRank; i++) {

        this->timerParallelInit[i].resize(sizeThread);
        this->timerParallelEnd[i].resize(sizeThread);
        this->timerParallelInitMpi[i].resize(sizeThread);
        this->timerParallelEndMpi[i].resize(sizeThread);
        this->timerParallel[i].resize(sizeThread);

        for (int j = 0; j < sizeThread; j++) {

            this->timerParallelInit[i][j].resize(sizeType);
            this->timerParallelEnd[i][j].resize(sizeType);
            this->timerParallelInitMpi[i][j].resize(sizeType);
            this->timerParallelEndMpi[i][j].resize(sizeType);
            this->timerParallel[i][j].resize(sizeType);

            for (int l = 0; l < sizeType; l++) {

                this->timerParallel[i][j][l] = 0.0;
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
    //#if USE_MPI
    //    this->timerParallelInitMpi[_rank][_thread][_type] = MPI_Wtime();
    //#else
    timeval begin;
    gettimeofday(&begin, 0);
    this->timerParallelInit[_rank][_thread][_type] = begin;
    //#endif //USE_MPI

}

void Timer::endTimerParallel(int _rank, int _thread, int _type)
{
    //#if USE_MPI
    //    this->timerParallelEndMpi[_rank][_thread][_type] = MPI_Wtime();
    //    calculateTime(_rank, _thread, _type);
    //#else
    timeval end;
    gettimeofday(&end, 0);
    this->timerParallelEnd[_rank][_thread][_type] = end;
    calculateTime(_rank, _thread, _type);
    //#endif //USE_MPI

}

void Timer::calculateTime(int _rank, int _thread, int _type)
{
    //#if USE_MPI
    //    this->timerParallel[_rank][_thread][_type] += (this->timerParallelEndMpi[_rank][_thread][_type])
    //                                                - (this->timerParallelInitMpi[_rank][_thread][_type]);
    //#else
    long seconds = (this->timerParallelEnd[_rank][_thread][_type]).tv_sec - (this->timerParallelInit[_rank][_thread][_type]).tv_sec;
    long microseconds = (this->timerParallelEnd[_rank][_thread][_type]).tv_usec - (this->timerParallelInit[_rank][_thread][_type]).tv_usec;
    this->timerParallel[_rank][_thread][_type] += seconds + microseconds*1e-6;
    //#endif //USE_MPI
}


void Timer::printTime()
{
    vector<double> max = getMaxTime();
    cout << max[0] << " Send"<<endl; //Send
    cout << max[1] << " Estimativa"<<endl; //Estimativa de carga
    cout << max[2] << " MalhaInicial"<<endl; //Geração da malha inicial
    cout << max[3] << " AdpCurvas"<<endl; //Adaptação das curvas
    cout << max[4] << " AdpDominio"<<endl; //Adaptação do domínio
    cout << max[5] << " Leitura arquivo"<<endl; //Leitura arquivo
    cout << max[6] << " MediaGauss"<<endl; //MediaGauss
    cout << max[7] << " CalcErro"<<endl; //Calculo do erro
    cout << max[10] - max[9] - max[7] - max[6] - max[5] - max[4] - max[3] - max[2] - max[1] - max[0] <<" Overhead"<<endl; //Overhead
    cout << max[9] << " Recv"<<endl; //Recv
    cout << max[10] - max[5] << " Full"<<endl; //Full

}

void Timer::printTime(int rank_process)
{
    if (rank_process == 0) {
        vector<double> max = getMaxTime();
        cout << max[0] << " Send"<<endl; //Send
        cout << max[1] << " Estimativa"<<endl; //Estimativa de carga
        cout << max[2] << " MalhaInicial"<<endl; //Geração da malha inicial
        cout << max[3] << " AdpCurvas"<<endl; //Adaptação das curvas
        cout << max[4] << " AdpDominio"<<endl; //Adaptação do domínio
        cout << max[5] << " Leitura arquivo"<<endl; //Leitura arquivo
        cout << max[6] << " MediaGauss"<<endl; //MediaGauss
        cout << max[7] << " CalcErro"<<endl; //Calculo do erro
        cout << max[10] - max[9] - max[7] - max[6] - max[5] - max[4] - max[3] - max[2] - max[1] - max[0] <<" Overhead"<<endl; //Overhead
        cout << max[9] << " Recv"<<endl; //Recv
        cout << max[10] - max[5] << " Full"<<endl; //Full

    } else{
        vector<double> max = getMaxTime();
        cout << max[0] << " Send"<<endl; //Send
        cout << max[1] << " Estimativa"<<endl; //Estimativa de carga
        cout << max[2] << " MalhaInicial"<<endl; //Geração da malha inicial
        cout << max[3] << " AdpCurvas"<<endl; //Adaptação das curvas
        cout << max[4] << " AdpDominio"<<endl; //Adaptação do domínio
        cout << max[5] << " Leitura arquivo copy Process 0"<<endl; //Leitura arquivo
        cout << max[6] << " MediaGauss"<<endl; //MediaGauss
        cout << max[7] << " CalcErro"<<endl; //Calculo do erro
        cout << max[10] - max[9] - max[7] - max[6] - max[5] - max[4] - max[3] - max[2] - max[1] - max[0] <<" Overhead"<<endl; //Overhead
        cout << max[9] - TIME_READ_FILE<< " Recv"<<endl; //Recv
        cout << max[10] - TIME_READ_FILE << " Full"<<endl; //Full
    }
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

vector<double> Timer::getMinTime()
{
    vector<double> min;
    min.resize(this->timerParallel[0][0].size());

    for (unsigned int i = 0; i < this->timerParallel[0][0].size(); ++i) {
        min[i] = 999999;
    }

    for (unsigned int i = 0; i < this->timerParallel.size(); ++i) {
        for (unsigned int j = 0; j < timerParallel[i].size(); ++j) {
            for (unsigned int l = 0; l < this->timerParallel[i][j].size(); ++l) {
                if (this->timerParallel[i][j][l] < min[l]) {
                    min[l] = this->timerParallel[i][j][l];
                }
            }
        }
    }

    return min;
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
