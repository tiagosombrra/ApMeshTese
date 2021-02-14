#include "../../Headers/Timer/Timer.h"

Timer::Timer(int sizeRank, int sizeThread, int sizeType)
{
    for (int i = 0; i < 11; i++) {
        this->timeStepsInit[i] = 0;
        this->timeStepsEnd[i] = 0;
        this->timeSteps[i] = 0;
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

void Timer::writeTimeFile(int process)
{
    stringstream name;

    cout << "Processo: " << process << endl<<endl;
    cout << "Inicialização: " << timeSteps[0] << endl;
    cout << "Estimativa de carga: " << timeSteps[1] << endl;
    cout << "Geração da malha inicial: " << timeSteps[2] << endl;
    cout << "Adaptação das curvas: " << timeSteps[3] << endl;
    cout << "Geração da malha: " << timeSteps[4] << endl;
    cout << "Quadtree: " << timeSteps[5] << endl;
    cout << "Avanço de front.: " << timeSteps[6] << endl;
    cout << "Calculo do erro: " << timeSteps[7] << endl;
    cout << "Overhead: " << timeSteps[8] << endl;
    cout << "SendRecv: " << timeSteps[9] << endl;
    cout << "Full: " << timeSteps[10] << endl<<endl;
}

void Timer::initTime(int type)
{
    time_clock = clock();
    timeStepsInit[type] = (double)time_clock;
}

void Timer::endTime(int type)
{
    time_clock = clock();
    timeStepsEnd[type] = (double)time_clock;
    calculateTime(type);
}

void Timer::calculateTime(int type)
{
    timeSteps[type] += timeStepsEnd[type] / CLOCKS_PER_SEC - timeStepsInit[type] / CLOCKS_PER_SEC;
    // cout << timeSteps[type] << endl;
}
