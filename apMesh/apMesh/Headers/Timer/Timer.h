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

class Timer
{
public:
    Timer();
    bool openFile(string);
    void writeTimeFile(int process=-1);
    void closeFile();
    bool deleteFile(string);
    // Contagem do tempo em segundos da inicialização em cada processo
    void initTimeInitialization();
    void endTimeInitialization();
    // Contagem do tempo em segundos da estimativa de carga no processo 0
    void initTimeEstimative();
    void endTimeEstimative();
    // Contagem do tempo em segundos da geração da malha inicial em cada processo
    void initTimeGenerationMeshInitial();
    void endTimeGenerationMeshInitial();
    // Contagem do tempo em segundos da adaptação das curvas em cada processo
    void initTimeAdaptCurves();
    void endTimeAdaptCurves();
    // Contagem do tempo em segundos da geração da malha em cada processo
    void initTimeGenerationMesh();
    void endTimeGenerationMesh();
    // Contagem do tempo em segundos da geração da quadtree em cada processo
    void initTimeGenerationQuadtree();
    void endTimeGenerationQuadtree();
    // Contagem do tempo em segundos da geração do avanço de fronteira em cada processo
    void initTimeGenerationAdv();
    void endTimeGenerationAdv();
    // Contagem do tempo em segundos do calculo do erro em cada processo
    void initTimeGenerationErro();
    void endTimeGenerationErro();
    // Contagem do tempo em segundos ociosos entre os contadores acimas
    void initTimeOverHead();
    void endTimeOverHead();
    // Contagem do tempo em segundos entre os Sends e Recvs
    void initTimeSendRecv();
    void endTimeSendRecv();

    void initTime(int type);
    void endTime(int type);

    void calculateTime(int type);

    void SetTypeRun(string typeRun);
    string GetTypeRun();

private:
    //[0]= Inicialização
    //[1]= Estimativa de carga
    //[2]= Geração da Malha Inicial
    //[3]= Adaptação das curvas
    //[4]= Geração da malha
    //[5]= Quadtree
    //[6]= Avanço de front.
    //[7]= Calculo do erro
    //[8]= Overhead
    //[9]= Timer send and recv process
    //[10]= Full
    double timeStepsInit[11];
    double timeStepsEnd[11];
    double timeSteps[11];
    string locationName;
    fstream file;
    clock_t time_clock;
    string typeRun;
};

#endif // TIMER_H
