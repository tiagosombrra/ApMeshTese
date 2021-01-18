#include "../../Headers/Timer/Timer.h"

Timer::Timer()
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

//    if (typeRun == std::string("seq")) {
//        name << "timer_sequencial";
//    } else if (typeRun == std::string("par")) {
//        name << nameModel;
//        name << "_n.process_";
//        name << numberProcess;
//        name << "timer_parallel_process_";
//        name << process;
//    }
//
//    name << ".txt";
//
//    if (openFile(name.str())) {
//        //[0]= Inicialização
//        //[1]= Estimativa de carga
//        //[2]= Geração da Malha Inicial
//        //[3]= Adaptação das curvas
//        //[4]= Geração da malha
//        //[5]= Quadtree
//        //[6]= Avanço de front.
//        //[7]= Calculo do erro
//        //[8]= Overhead
//        //[9]= Timer send and recv process
//        //[10]= Full
//        file << "Inicialização: " << timeSteps[0] << endl;
//        file << "Estimativa de carga: " << timeSteps[1] << endl;
//        file << "Geração da malha inicial: " << timeSteps[2] << endl;
//        file << "Adaptação das curvas: " << timeSteps[3] << endl;
//        file << "Geração da malha: " << timeSteps[4] << endl;
//        file << "Quadtree: " << timeSteps[5] << endl;
//        file << "Avanço de front.: " << timeSteps[6] << endl;
//        file << "Calculo do erro: " << timeSteps[7] << endl;
//        file << "Overhead: " << timeSteps[8] << endl;
//        file << "SendRecv: " << timeSteps[9] << endl;
//        file << "Full: " << timeSteps[10] << endl;
//
//        if (typeRun == std::string("seq")) {
//            cout << "Arquivo com detalhamento dos tempos salvo com sucesso!" << endl;
//        } else if (typeRun == std::string("par")) {
//            cout << "Arquivo com detalhamento dos tempos do processo: " << process << " salvo com sucesso!" << endl;
//        }
//
//    } else {
//
//        if (typeRun == std::string("seq")) {
//            cout << "Erro ao salvar arquivo com detalhamento dos tempos" << endl;
//        } else if (typeRun == std::string("par")) {
//            cout << "Erro ao salvar arquivo com detalhamento dos tempos do processo: " << process << endl;
//        }
//    }
}

void Timer::endTimeInitialization()
{
    //#if USE_OPENMPI
    //    timeStepsEnd[0] = MPI_Wtime();
    //#else //#if USE_OPENMPI
    time_clock = clock();
    timeStepsEnd[0] = (double)time_clock;
    //#endif //#if USE_OPENMPI
}

void Timer::initTimeInitialization()
{
    //#if USE_OPENMPI
    //    timeStepsInit[0] = MPI_Wtime();
    //#else //#if USE_OPENMPI
    time_clock = clock();
    timeStepsInit[0] = (double)time_clock;
    //#endif //#if USE_OPENMPI
}

void Timer::endTimeEstimative()
{
    //#if USE_OPENMPI
    //#else //#if USE_OPENMPI
    //#endif //#if USE_OPENMPI
    //    timeStepsEnd[1] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[1] = (double)time_clock;
}

void Timer::initTimeEstimative()
{
    //#if USE_OPENMPI
    //#else //#if USE_OPENMPI
    //#endif //#if USE_OPENMPI
    //    timeStepsInit[1] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[1] = (double)time_clock;
}

void Timer::endTimeGenerationMeshInitial()
{
    //#if USE_OPENMPI
    //#else //#if USE_OPENMPI
    //#endif //#if USE_OPENMPI
    //    timeStepsEnd[2] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[2] = (double)time_clock;
}

void Timer::initTimeGenerationMeshInitial()
{
    //#if USE_OPENMPI
    //#else //#if USE_OPENMPI
    //#endif //#if USE_OPENMPI
    //    timeStepsInit[2] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[2] = (double)time_clock;
}

void Timer::endTimeAdaptCurves()
{
    // timeStepsEnd[3] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[3] = (double)time_clock;
}

void Timer::initTimeAdaptCurves()
{
    // timeStepsInit[3] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[3] = (double)time_clock;
}
void Timer::endTimeGenerationMesh()
{
    // timeStepsEnd[4] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[4] = (double)time_clock;
}

void Timer::initTimeGenerationMesh()
{
    // timeStepsInit[4] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[4] = (double)time_clock;
}

void Timer::endTimeGenerationQuadtree()
{
    // timeStepsEnd[5] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[5] = (double)time_clock;
}

void Timer::initTimeGenerationQuadtree()
{
    // timeStepsInit[5] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[5] = (double)time_clock;
}
void Timer::endTimeGenerationAdv()
{
    //  timeStepsEnd[6] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[6] = (double)time_clock;
}

void Timer::initTimeGenerationAdv()
{
    // timeStepsInit[6] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[6] = (double)time_clock;
}

void Timer::endTimeGenerationErro()
{
    // timeStepsEnd[7] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[7] = (double)time_clock;
}

void Timer::initTimeGenerationErro()
{
    //  timeStepsInit[7] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[7] = (double)time_clock;
}

void Timer::endTimeOverHead()
{
    // timeStepsEnd[8] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[8] = (double)time_clock;
}

void Timer::initTimeOverHead()
{
    // timeStepsInit[8] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[8] = (double)time_clock;
}

void Timer::endTimeSendRecv()
{
    // timeStepsEnd[9] = MPI_Wtime();
    time_clock = clock();
    timeStepsEnd[9] = (double)time_clock;
}

void Timer::initTimeSendRecv()
{
    //  timeStepsInit[9] = MPI_Wtime();
    time_clock = clock();
    timeStepsInit[9] = (double)time_clock;
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

void Timer::SetTypeRun(string typeRun)
{
    this->typeRun = typeRun;
}
string Timer::GetTypeRun()
{
    return typeRun;
}
