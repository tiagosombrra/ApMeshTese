#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include "../Data/Definitions.h"
#include <time.h>

using namespace std;

extern std::string nameModel;
extern std::string numberProcess;

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

class Timer
{

public:

    Timer();
    bool openFile(string);
    void writeTimeFile(int process=-1);
    void closeFile();
    bool deleteFile(string);

    void initTime(int type);
    void endTime(int type);
    void calculateTime(int type);


private:

    double timeStepsInit[11];
    double timeStepsEnd[11];
    double timeSteps[11];

    string locationName;
    fstream file;
    clock_t time_clock;
};

#endif // TIMER_H
