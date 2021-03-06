/* Classe encarregada de fazer a geração adaptativa das malhas de acordo com as
curvaturas.*/

#include "../../Headers/Generator/GeradorAdaptativoPorCurvatura.h"

GeradorAdaptativoPorCurvatura::GeradorAdaptativoPorCurvatura()
{

}

#if USE_MPI
int GeradorAdaptativoPorCurvatura::execute(int argc, char *argv[], Timer *timer, MPI_Status status)
#else
int GeradorAdaptativoPorCurvatura::execute(int argc, char *argv[], Timer *timer)
#endif // USE_MPI
{
    Geometria *geo = new Geometria;

#if USE_MPI
    if (RANK_MPI == 0) {

        // estimativa e ordenação dos patches para carga de distribuiçao dos patches
        std::list<BezierPatch*> listBezierPt = estimateChargeofPatches(geo, timer, argv[3]);

        // distribuição dos patches entre os (n) processos de acordo com suas cargas ordenadas
        listBezierPt = orderPatchesDistribProcess(listBezierPt);


#if USE_PRINT_ESTIMATE
        // mostra a distribuição de patches para cada processo
        calculateEstimateProcessElements(SIZE_MPI, listBezierPt);
#endif //USE_PRINT_ESTIMATE

        double vecBezierPatchesOfProcess0[listBezierPt.size() * 48];
        double vecBezierPatches[listBezierPt.size() * 48];

        int i = 0, izero = 0;

        for (int j = 0; j < SIZE_MPI; j++) {

            i = 0;

            for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++) {

                if (j == (*it)->getIdProcess()) {

                    vecBezierPatches[i] = (*it)->getPt00().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt00().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt00().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt10().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt10().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt10().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt20().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt20().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt20().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt30().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt30().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt30().z;
                    i++;

                    vecBezierPatches[i] = (*it)->getPt01().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt01().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt01().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt11().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt11().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt11().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt21().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt21().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt21().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt31().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt31().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt31().z;
                    i++;

                    vecBezierPatches[i] = (*it)->getPt02().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt02().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt02().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt12().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt12().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt12().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt22().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt22().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt22().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt32().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt32().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt32().z;
                    i++;

                    vecBezierPatches[i] = (*it)->getPt03().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt03().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt03().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt13().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt13().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt13().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt23().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt23().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt23().z;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt33().x;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt33().y;
                    i++;
                    vecBezierPatches[i] = (*it)->getPt33().z;
                    i++;
                }
            }

            if (j == 0) {

                izero = i;

                for (int m = 0; m < i; ++m) {
                    vecBezierPatchesOfProcess0[m] = vecBezierPatches[m];
                }

            }else {
                timer->initTimerParallel(RANK_MPI,0,0); //Send
                MPI_Send(&TIME_READ_FILE, 1, MPI_DOUBLE, j, TAG_TIME, MPI_COMM_WORLD);
                MPI_Send(&i, 1, MPI_INT, j, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD);
                MPI_Send(&vecBezierPatches, i, MPI_DOUBLE, j, TAG_DOUBLE, MPI_COMM_WORLD);
                timer->endTimerParallel(RANK_MPI,0,0); //Send

            }
        }

        //gerador de malha para o processo root
#if USE_OPENMP
        generator(vecBezierPatchesOfProcess0, izero, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
        generator(vecBezierPatchesOfProcess0, izero, timer, 1024, atoi(argv[1]));
#endif

    }else{

        timer->initTimerParallel(RANK_MPI,0,9); //Recv

        MPI_Recv(&TIME_READ_FILE, 1, MPI_DOUBLE, 0, TAG_TIME, MPI_COMM_WORLD, &status);

        int sizePtBezierPatches = 0;
        MPI_Recv(&sizePtBezierPatches, 1, MPI_INT, 0, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD, &status);

        double ptBezierPatches[sizePtBezierPatches];
        MPI_Recv(&ptBezierPatches, sizePtBezierPatches, MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD, &status);

        timer->endTimerParallel(RANK_MPI,0,9); //Recv


        //gerador de malha para o processo (n)
#if USE_OPENMP
        generator(ptBezierPatches, sizePtBezierPatches, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
        generator(ptBezierPatches, sizePtBezierPatches, timer, 1024, atoi(argv[1]));
#endif
    }
#else

    Modelo modelo;
    ReaderPatches read;

    //Inclusão dos patches no Modelo
    if (argv[3])
    {
        timer->initTimerParallel(0,0,5); //Leitura arquivo
        modelo.setGeometria(read.readerPatches(geo, argv[3]));
        timer->endTimerParallel(0,0,5); //Leitura arquivo
    }
    else
    {
        Modelos3d modelos3d;
        modelo.setGeometria(modelos3d.modelLadoDescendente(geo));
    }

#if USE_OPENMP
    generator(modelo, timer, 1024, atoi(argv[1]), atoi(argv[2]));
#else
    generator(modelo, timer, 1024);
#endif // USE_OPENMP

#endif // USE_MPI
    return 0;
}

#if USE_MPI
std::list<BezierPatch *> GeradorAdaptativoPorCurvatura::estimateChargeofPatches(Geometria *geometria, Timer *timer, string entrada)
{
    ChargeEstimateProcess* cep = new ChargeEstimateProcess();
    std::list<BezierPatch*> listBezierPt = cep->chargeEstimateProcess(geometria, timer, entrada);
    delete cep;

    return listBezierPt;
}

std::vector<Curva*> GeradorAdaptativoPorCurvatura::createVectorOfCurves(std::list<BezierPatch *> listBezierPt)
{
    Curva* curva;
    std::vector<Curva*> curves;

    for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++) {

        if (curves.size() > 0) {
            if (verifyCurve((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30(), curves)) {
                curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33(), curves)) {
                curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33(), curves)) {
                curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
                curves.push_back(curva);
            }
            if (verifyCurve((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03(), curves)) {
                curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
                curves.push_back(curva);
            }
        } else if (curves.size() == 0) {
            curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
            curves.push_back(curva);
            curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
            curves.push_back(curva);
        }
    }

    return curves;
}

std::list<BezierPatch *> GeradorAdaptativoPorCurvatura::orderPatchesDistribProcess(std::list<BezierPatch *> listPatches)
{
    std::vector<std::pair<double, int> > vectorEstimativeProcessPair;
    std::list<BezierPatch*> listPatchesOrder;

    if (SIZE_MPI > 1) {

        for (int i = 0; i < SIZE_MPI; i++) {
            vectorEstimativeProcessPair.push_back(std::make_pair(0.0, i));
        }

        for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {

            std::sort(vectorEstimativeProcessPair.begin(), vectorEstimativeProcessPair.end());

            (*it)->setIdProcess(vectorEstimativeProcessPair.front().second);

            vectorEstimativeProcessPair.front().first += (*it)->getNumberTriangle();

        }

        for (int i = 0; i < SIZE_MPI; i++) {
            for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {

                if ((*it)->getIdProcess() == i) {

                    listPatchesOrder.push_back((*it));
                }
            }
        }

        return listPatchesOrder;

    } else {

        for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {
            (*it)->setIdProcess(0);
        }

        return listPatches;
    }
}

bool GeradorAdaptativoPorCurvatura::verifyCurve(Ponto p0, Ponto p1, Ponto p2, Ponto p3, std::vector<Curva*> curves)
{
    for (vector<Curva*>::iterator it = curves.begin(); it != curves.end(); it++) {
        if (static_cast<CurvParamBezier*>(*it)
                ->getP0()
                .operator==(p0) and static_cast<CurvParamBezier*>(*it)
                ->getP1()
                .operator==(p1) and static_cast<CurvParamBezier*>(*it)
                ->getP2()
                .operator==(p2) and static_cast<CurvParamBezier*>(*it)
                ->getP3()
                .operator==(p3)) {
            return false;
        }
    }

    return true;
}

void GeradorAdaptativoPorCurvatura::calculateEstimateProcessElements(int sizeProcess, std::list<BezierPatch *> listBezierPt)
{
    double estimate[sizeProcess] = { };

    for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++)
    {
        estimate[(*it)->getIdProcess()] += (*it)->getNumberTriangle();
    }

    for (int i = 0; i < SIZE_MPI; ++i) {
        cout<<"Estimativa para o processo "<<i<<" = "<<estimate[i]<<endl;
    }
}

std::list<BezierPatch*>::iterator GeradorAdaptativoPorCurvatura::getIteratorListPatches(int numberPatches, std::list<BezierPatch *> listBezierPt)
{
    std::list<BezierPatch*>::iterator it = listBezierPt.begin();
    advance(it, numberPatches);
    return it;
}

Geometria *GeradorAdaptativoPorCurvatura::unpakGeometry(double listOfPatches[], int sizeOfListPatches)
{
    Geometria *geo = new Geometria;

    Ponto* p00;
    Ponto* p01;
    Ponto* p02;
    Ponto* p03;
    Ponto* p10;
    Ponto* p11;
    Ponto* p12;
    Ponto* p13;
    Ponto* p20;
    Ponto* p21;
    Ponto* p22;
    Ponto* p23;
    Ponto* p30;
    Ponto* p31;
    Ponto* p32;
    Ponto* p33;

    Curva* patch_c1;
    Curva* patch_c2;
    Curva* patch_c3;
    Curva* patch_c4;

    BezierPatch* bezierPatch;

    for (int i = 0; i < sizeOfListPatches; i = i + 48) {

        p00 = new Vertice(listOfPatches[i], listOfPatches[i + 1], listOfPatches[i + 2]);
        p10 = new Vertice(listOfPatches[i + 3], listOfPatches[i + 4], listOfPatches[i + 5]);
        p20 = new Vertice(listOfPatches[i + 6], listOfPatches[i + 7], listOfPatches[i + 8]);
        p30 = new Vertice(listOfPatches[i + 9], listOfPatches[i + 10], listOfPatches[i + 11]);

        p01 = new Vertice(listOfPatches[i + 12], listOfPatches[i + 13], listOfPatches[i + 14]);
        p11 = new Vertice(listOfPatches[i + 15], listOfPatches[i + 16], listOfPatches[i + 17]);
        p21 = new Vertice(listOfPatches[i + 18], listOfPatches[i + 19], listOfPatches[i + 20]);
        p31 = new Vertice(listOfPatches[i + 21], listOfPatches[i + 22], listOfPatches[i + 23]);

        p02 = new Vertice(listOfPatches[i + 24], listOfPatches[i + 25], listOfPatches[i + 26]);
        p12 = new Vertice(listOfPatches[i + 27], listOfPatches[i + 28], listOfPatches[i + 29]);
        p22 = new Vertice(listOfPatches[i + 30], listOfPatches[i + 31], listOfPatches[i + 32]);
        p32 = new Vertice(listOfPatches[i + 33], listOfPatches[i + 34], listOfPatches[i + 35]);

        p03 = new Vertice(listOfPatches[i + 36], listOfPatches[i + 37], listOfPatches[i + 38]);
        p13 = new Vertice(listOfPatches[i + 39], listOfPatches[i + 40], listOfPatches[i + 41]);
        p23 = new Vertice(listOfPatches[i + 42], listOfPatches[i + 43], listOfPatches[i + 44]);
        p33 = new Vertice(listOfPatches[i + 45], listOfPatches[i + 46], listOfPatches[i + 47]);

        patch_c1 = new CurvParamBezier(*p00, *p10, *p20, *p30);
        patch_c2 = new CurvParamBezier(*p30, *p31, *p32, *p33);
        patch_c3 = new CurvParamBezier(*p03, *p13, *p23, *p33);
        patch_c4 = new CurvParamBezier(*p00, *p01, *p02, *p03);

        if (geo->verifyCurveGeometria(p00, p10, p20, p30) == NULL) {
            patch_c1 = new CurvParamBezier(*p00, *p10, *p20, *p30);
            geo->insereCurva(patch_c1);
        } else {
            patch_c1 = geo->verifyCurveGeometria(p00, p10, p20, p30);
        }

        if (geo->verifyCurveGeometria(p30, p31, p32, p33) == NULL) {
            patch_c2 = new CurvParamBezier(*p30, *p31, *p32, *p33);
            geo->insereCurva(patch_c2);
        } else {
            patch_c2 = geo->verifyCurveGeometria(p30, p31, p32, p33);
        }

        if (geo->verifyCurveGeometria(p03, p13, p23, p33) == NULL) {
            patch_c3 = new CurvParamBezier(*p03, *p13, *p23, *p33);
            geo->insereCurva(patch_c3);
        } else {
            patch_c3 = geo->verifyCurveGeometria(p03, p13, p23, p33);
        }

        if (geo->verifyCurveGeometria(p00, p01, p02, p03) == NULL) {
            patch_c4 = new CurvParamBezier(*p00, *p01, *p02, *p03);
            geo->insereCurva(patch_c4);
        } else {
            patch_c4 = geo->verifyCurveGeometria(p00, p01, p02, p03);
        }

        bezierPatch = new BezierPatch(patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21, *p12, *p22);
        bezierPatch->setId(i / 48);

        geo->inserePatch(bezierPatch);

    }

    return geo;
}

#endif

#if USE_MPI
void GeradorAdaptativoPorCurvatura::generator(double listOfPatches[], int sizeOfListPatches, Timer *timer, int idrange, int sizeRank, int sizeThread)
#else
void GeradorAdaptativoPorCurvatura::generator(Modelo &modelo, Timer *timer, int idrange, int sizeRank, int sizeThread)
#endif
{
#if USE_MPI
    this->comm = new ApMeshCommunicator(true);
    Geometria *geo = unpakGeometry(listOfPatches, sizeOfListPatches);
#elif USE_OPENMP
    this->comm = new ApMeshCommunicator(true);
    Geometria* geo = modelo.getGeometria ( );
#else
    this->comm = new Parallel::TMCommunicator(false);
    Geometria* geo = modelo.getGeometria ( );
#endif

    int sizePatch = geo->getNumDePatches();

    Malha* malha = new Malha;
    malha->resizeSubmalha(sizePatch);

    this->idManager = NULL;
    this->idoffset = 0;
    this->idrange = idrange;

    this->passo = 0;

    sizeThread = sizeThread > static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads() ?
                static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads() : sizeThread;

    ptr_aux.resize(sizeThread,NULL);

    if (this->idManagers.empty())
    {
        this->idManagers.resize(sizeThread, NULL);
    }

    //Gerar a malha inicial
#if USE_OPENMP
    generatorInitialMesh(geo, malha, timer, sizeThread, sizePatch);
#else
    this->idManagers[0] = this->makeIdManager(comm, 0);
#if USE_MPI
    timer->initTimerParallel(RANK_MPI, 0, 2); // Malha inicial
#else
    timer->initTimerParallel(0, 0, 2); // Malha inicial
#endif //USE_MPI

    generatorInitialMesh(geo, malha, timer, sizeThread, sizePatch);

#if USE_MPI
    timer->endTimerParallel(RANK_MPI, 0, 2); // Malha inicial
#else
    timer->endTimerParallel(0, 0, 2); // Malha inicial
#endif //USE_MPI
#endif //USE_OPENMP

    //Calcula o erro global para a malha inicial
#if USE_OPENMP
    if (sizePatch > 1) {
        this->erro = this->erroGlobalOmp( malha, timer, 0, sizeThread);

    } else{
        this->erro = this->erroGlobal( malha, timer, 0, sizeThread);
    }
#else
#if USE_MPI
    timer->initTimerParallel(RANK_MPI, 0, 7); // Calculo do erro Global
#else
    timer->initTimerParallel(0, 0, 7); // Calculo do erro Global
#endif // USE_MPI
    this->erro = this->erroGlobal(malha, timer);
#if USE_MPI
    timer->endTimerParallel(RANK_MPI, 0, 7); // Calculo do erro Global
#else
    timer->endTimerParallel(0, 0, 7); // Calculo do erro Global
#endif // USE_MPI
#endif // USE_OPENMP

    this->erroPasso.push_back(this->erro);

#if USE_SAVE_ERRO_MESH
    salvaErroMalha(malha, passo);
#endif //USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->passo << " rank " << RANK_MPI << " = " << this->erro << endl;
#endif //#if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
    cout << "*************** ERRO " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_RESULTS
#endif //USE_MPI

    if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
        salvaMalha(malha, passo);
    }

    // this->erro = 1000;

    //Gerar malha enquanto o erro global for acima do erro desejado
    while (this->passo < PASSOS)
    {

        if (this->erro < EPSYLON) {
            break;
        }
        //#if USE_MPI
        //        timer->initTimerParallel(RANK_MPI,0,9); // SendRecv
        //        MPI_Allreduce(&this->erro, &this->erro, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        //        timer->endTimerParallel(RANK_MPI,0,9); // SendRecv
        //        this->erro = this->erro / sizeRank;
        //#endif //USE_MPI

        this->passo++;

        //Aloca uma nova malha
        malha = new Malha;
        malha->resizeSubmalha(geo->getNumDePatches());
#if USE_MPI
        timer->initTimerParallel(RANK_MPI, 0 , 3); // Adaptação da curva
#else
        timer->initTimerParallel(0, 0, 3); // Adaptação da curva
#endif //USE_MPI

        //Adapta as curvas pela curvatura da curva / Atualiza a discretização das curvas
        adaptCurve(geo);

#if USE_MPI
        timer->endTimerParallel(RANK_MPI, 0 , 3); // Adaptação da curva
#else
        timer->endTimerParallel(0, 0, 3); // Adaptação da curva
#endif //USE_MPI

        // Adapta as patches / Atualiza os patches
#if USE_OPENMP
        adaptDomainOmp(geo, malha, timer, sizeThread, sizePatch);
#else
#if USE_MPI
        timer->initTimerParallel(RANK_MPI, 0 , 4); // Adaptação do domínio
#else
        timer->initTimerParallel(0, 0 , 4); // Adaptação do domínio
#endif //USE_MPI
        adaptDomain(geo, malha);
#if USE_MPI
        timer->endTimerParallel(RANK_MPI, 0 , 4); // Adaptação do domínio
#else
        timer->endTimerParallel(0, 0 , 4); // Adaptação do domínio
#endif //USE_MPI
#endif //USE_OPENMP

        //Calcula o erro global para da malha
#if USE_OPENMP
        if (sizePatch > 1) {
            this->erro = this->erroGlobalOmp( malha, timer, 0, sizeThread);

        } else{
            this->erro = this->erroGlobal( malha, timer, 0, sizeThread);
        }
#else
#if USE_MPI
        timer->initTimerParallel(RANK_MPI, 0, 7); // Calculo do erro Global
#else
        timer->initTimerParallel(0, 0, 7); // Calculo do erro Global
#endif //USE_MPI
        this->erro = this->erroGlobal(malha, timer);
#if USE_MPI
        timer->endTimerParallel(RANK_MPI, 0, 7); // Calculo do erro Global
#else
        timer->endTimerParallel(0, 0, 7); // Calculo do erro Global
#endif //USE_MPI
#endif

        this->erroPasso.push_back(this->erro);

#if USE_SAVE_ERRO_MESH
        salvaErroMalha(malha, passo);
#endif //USE_SAVE_ERRO_MESH

#if USE_MPI
#if USE_PRINT_RESULTS
        cout << "*************** ERRO " << this->passo << " rank " << RANK_MPI << " = " << this->erro << endl;
#endif //#if USE_PRINT_RESULTS
#else
#if USE_PRINT_RESULTS
        cout << "*************** ERRO " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_RESULTS
#endif //USE_MPI

        if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
            salvaMalha(malha, passo);
        }
    }

#if USE_MPI
    timer->endTimerParallel(RANK_MPI, 0, 10); // Full
#endif #endif //USE_MPI

    //Escreve o(s) arquivo(s) com suas respectivas malhas em cada passo
#if USE_MPI
    for (unsigned int i = 0; i < erroPasso.size(); ++i) {
        cout<<"Erro do processo "<<RANK_MPI<< " no passo "<<i<<" = "<< erroPasso[i]<<endl;
    }

    if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
        for (MeshVector::iterator it = saveMesh.begin(); it != saveMesh.end(); it++) {
            if (WRITE_MESH == std::string("q")) {
                printElments((*it).second, (*it).first, this->erroPasso, RANK_MPI);
                writeQualityMesh((*it).second, (*it).first, this->erroPasso, RANK_MPI);
            } else if (WRITE_MESH == std::string("m")){
                printElments((*it).second, (*it).first, this->erroPasso, RANK_MPI);
                escreveMalha((*it).second, (*it).first, this->erroPasso, RANK_MPI);
            }
        }

    }
#else

    timer->endTimerParallel(0,0,10); //Full
    timer->printTime();

    if (WRITE_MESH == std::string("m") || WRITE_MESH == std::string("q")) {
        for (MeshVector::iterator it = saveMesh.begin(); it != saveMesh.end(); it++) {
            if (WRITE_MESH == std::string("q")) {
                printElments((*it).second, (*it).first, this->erroPasso);
                writeQualityMesh((*it).second, (*it).first, this->erroPasso);
            } else{
                printElments((*it).second, (*it).first, this->erroPasso);
                escreveMalha((*it).second, (*it).first, this->erroPasso);
            }
        }
        for (unsigned int i = 0; i < erroPasso.size(); ++i) {
            cout<<"Erro no passo "<<i<<" = "<< erroPasso[i]<<endl;
        }
    }
#endif //USE_MPI
}

void GeradorAdaptativoPorCurvatura::adaptCurve(Geometria *geo)
{
    list<Ponto *> novosPontos[geo->getNumDeCurvas( )];
    map<Ponto *, Ponto *> mapaPontos;

    for (unsigned int i = 0; i < geo->getNumDeCurvas( ); ++i )
    {
        novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurva( geo->getCurva(i), mapaPontos, this->idManagers[0], 1);
        geo->getCurva(i)->setPontos(novosPontos[i]);
        novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficie( geo->getCurva(i), mapaPontos, this->idManagers[0], 1);
        geo->getCurva(i)->setPontos(novosPontos[i]);
    }
}

void GeradorAdaptativoPorCurvatura::adaptDomain(Geometria *geo, Malha *malha)
{
    for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
    {
        CoonsPatch *p = static_cast < CoonsPatch* > (geo->getPatch(i));
        SubMalha* sub = AdaptadorPorCurvatura::adaptaDominio( p, this->idManagers[0], 1);
        sub->setPatch(p);
        malha->insereSubMalha(sub, i);
        geo->getPatch(i)->setMalha(malha->getSubMalha(i));
    }
}


#if USE_OPENMP
SubMalha *GeradorAdaptativoPorCurvatura::malhaInicialOmp(CoonsPatch *patch, Performer::IdManager *idManager)
{
    Curva* c1 = patch->getCurva ( 0 );
    Curva* c2 = patch->getCurva ( 1 );
    Curva* c3 = patch->getCurva ( 2 );
    Curva* c4 = patch->getCurva ( 3 );

    // 1. verifica quais curvas ainda não foram discretizadas
    if ( c1->getNumDePontos ( ) ) c1 = NULL; // c1 já foi trabalhada no patch vizinho
    if ( c2->getNumDePontos ( ) ) c2 = NULL; // c2 já foi trabalhada no patch vizinho
    if ( c3->getNumDePontos ( ) ) c3 = NULL; // c3 já foi trabalhada no patch vizinho
    if ( c4->getNumDePontos ( ) ) c4 = NULL; // c4 já foi trabalhada no patch vizinho

    SubMalha* sub = new SubMalha;

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região

    for ( double v = 0.0; v <= 1.0; v += 1 )
    {
        for ( double u = 0.0; u <= 1.0; u += 1 )
        {
            Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
            p->id = idManager->next(0);

            if ( v == 0 and c1 ) // p está na curva 1 (c1 = NULL)
                c1-> inserePonto ( p );
            else if ( v == 1 and c3 ) // p está na curva 3
                c3-> inserePonto ( p );

            if ( u == 0 and c4 ) // p está na curva 4
                c4-> inserePonto ( p );
            else if ( u == 1 and c2 ) // p está na curva 2
                c2-> inserePonto ( p );

            sub->insereNoh ( static_cast < Noh* > ( p ) );
        }
    }

    Ponto* p = new Noh ( patch->parametrizar ( 0.5, 0.5 ) );
    p->id = idManager->next(0);
    sub->insereNoh ( static_cast < Noh* > ( p ) );


    Elemento* e1 = new Triangulo (	sub->getNoh ( 0 ),
                                    sub->getNoh ( 1 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e1)->p1 = make_tuple ( 0, 0 );
    ((Triangulo*)e1)->p2 = make_tuple ( 1, 0 );
    ((Triangulo*)e1)->p3 = make_tuple ( 0.5, 0.5 );
    e1->setId (/*idManager->next(1)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  */ idManager->next(1));
    sub->insereElemento ( e1);

    Elemento* e2 = new Triangulo (	sub->getNoh ( 1 ),
                                    sub->getNoh ( 3 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e2)->p1 = make_tuple ( 1, 0 );
    ((Triangulo*)e2)->p2 = make_tuple ( 1, 1 );
    ((Triangulo*)e2)->p3 = make_tuple ( 0.5, 0.5 );
    e2->setId ( /*idManager->next(1)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        */ idManager->next(1));
    sub->insereElemento ( e2);

    Elemento* e3 = new Triangulo (	sub->getNoh ( 3 ),
                                    sub->getNoh ( 2 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e3)->p1 = make_tuple ( 1, 1 );
    ((Triangulo*)e3)->p2 = make_tuple ( 0, 1 );
    ((Triangulo*)e3)->p3 = make_tuple ( 0.5, 0.5 );
    e3->setId ( /*idManager->next(1)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        */ idManager->next(1));
    sub->insereElemento ( e3);

    Elemento* e4 = new Triangulo (	sub->getNoh ( 2 ),
                                    sub->getNoh ( 0 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e4)->p1 = make_tuple ( 0, 1 );
    ((Triangulo*)e4)->p2 = make_tuple ( 0, 0 );
    ((Triangulo*)e4)->p3 = make_tuple ( 0.5, 0.5 );
    e4->setId ( /*idManager->next(1)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        */ idManager->next(1));
    sub->insereElemento ( e4);
    //==============================================================================*/

    // 5. define a submalha do patch
    patch->setMalha ( sub );
    sub->setPatch ( patch );

    return sub;
}

double GeradorAdaptativoPorCurvatura::erroGlobalOmp(Malha *malha, Timer *timer, int rank, int sizeThread)
{
    unsigned int Ns = 0; // número de submalhas
    double Nj = 0.0; // erro global da malha

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha (OMP)
#pragma omp parallel num_threads(sizeThread) firstprivate(Ns) reduction(+ :Nj)
    {

#if USE_MPI
        timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
        timer->initTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI
#pragma omp for
        for ( unsigned int i = 0; i < Ns; ++i )
        {
            SubMalha* sub = malha->getSubMalha ( i );
            unsigned int Nv = sub->getNumDeNos ( );
            double curvPower = 0.0;
            double Njs = 0.0;

            // Calcula o erro relativo para cada nó e soma a Nj
            //#pragma omp parallel for num_threads(nThreads) firstprivate(Ns) reduction(+ :Nj)
            for ( unsigned int j = 0; j < Nv; ++j )
            {
#if USE_MPI
                timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
                timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 6); //MediaGauss
#else
                timer->endTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
                timer->initTimerParallel(0,omp_get_thread_num(), 6); //MediaGauss
#endif //USE_MPI

                Ponto *n = sub->getNoh ( j );
                Patch *p = sub->getPatch (  );
                CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
                CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
                double Ga = ka.gauss();
                double Gd = kd.gauss();
                double Ha = ka.media();
                double Hd = kd.media();

                //tratamento para erro -nan
                if (std::isnan(Gd)) {
                    cout<<"-nan Gd"<<endl;
                    Ga = 0.0;
                    Gd = 0.0;
                }

                // atualiza as curvaturas do nó ( para que não sejam recalculadas na
                // adaptação das curvas e do domínio )
                ((Noh*)n)->Ga = Ga;
                ((Noh*)n)->Gd = Gd;
                ((Noh*)n)->Ha = Ha;
                ((Noh*)n)->Hd = Hd;
#if USE_MPI
                timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 6); //MediaGauss
                timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
                timer->endTimerParallel(0,omp_get_thread_num(), 6); //MediaGauss
                timer->initTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI

                double power = 0.0;
                double diff = 0.0;

                if ( fabs ( Ga ) >= TOLERANCIA)
                {
                    diff = Gd - Ga;
                    power = pow( diff, 2);
                    Njs += power;
                    curvPower += pow(Ga, 2);
                }
                else if ( fabs ( Ha ) >= TOLERANCIA )
                {
                    diff = Hd - Ha;
                    power = pow( diff, 2);
                    Njs += power;
                    curvPower += pow(Ha, 2);
                }
            }

            if (Njs > 0.0 && curvPower > 0.0) {
                Njs =(double) sqrt( Njs / curvPower) / Nv;
            }

            if(!std::isnan(Njs))
                Nj += Njs;
            else
                cout<<"Njs -nan"<<endl;

        } //Parallel for

#if USE_MPI
        timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
        timer->endTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI
    }

    Nj /= Ns; // o erro global é a média do erro das submalhas

    return Nj;
}

int GeradorAdaptativoPorCurvatura::generatorOmp(Modelo &modelo, Timer *timer, int idrange, int sizeRank, int sizeThread)

{
    this->idManager = NULL;
    this->idoffset = 0;
    this->idrange = idrange;
    this->comm = new ApMeshCommunicator(true);


#if USE_MPI
    Int nProcesses = 1;
    Int rank = 0;
    nProcesses = this->comm->numProcesses();
    rank = this->comm->rank();
#endif //#if USE_MPI

    Geometria* geo = modelo.getGeometria ( );
    int sizePatch = geo->getNumDePatches();

    Malha* malha = new Malha;
    malha->resizeSubmalha(sizePatch);

    this->passo = 0;

    //sizeThread = static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads();

    ptr_aux.resize(sizeThread,NULL);

    if (this->idManagers.empty())
    {
        this->idManagers.resize(sizeThread, NULL);
    }

#pragma omp parallel num_threads(sizeThread) shared(malha, geo, sizePatch)
    {
        Int id = comm->threadId();


        if (!this->idManagers[id])
        {
            this->idManagers[id] = this->makeIdManagerOmp(comm, id);
        }

        timer->initTimerParallel(0, id, 2); // Malha inicial

        // 1. Gera a malha inicial
#pragma omp for
        for (int i = 0; i < sizePatch; ++i )
        {
            CoonsPatch *patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
            SubMalha *sub = this->malhaInicialOmp( static_cast < CoonsPatch* > ( patch ), this->idManagers[id]);
            malha->insereSubMalha (sub,i);
        }

        timer->endTimerParallel(0, id, 2); // Malha inicial
    }

    // 2. Insere a malha inicial no modelo ( que guarda todas as malhas geradas )
    modelo.insereMalha ( malha );

    // 3. Calcula o erro global para a malha inicial
    this->erro = this->erroGlobalOmp( malha, timer, 0, sizeThread);


#if USE_PRINT_ERRO
    cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_ERRO

#if USE_SAVE_MESH
    escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH

    this->erro = 1.0;

    // 4. enquanto o erro global de uma malha gerada não for menor que o desejado
    while ( this->erro > EPSYLON )
    {
        if (passo >= 2)
        {
            break;
        }

        this->passo++;

        // 4.1. Aloca uma nova malha
        malha = new Malha;
        malha->resizeSubmalha(sizePatch);

        list<Ponto *> novosPontos[geo->getNumDeCurvas()];

        // map<Ponto *, Ponto *> mapaPontos;

        int sizeCurvas = geo->getNumDeCurvas ( );

        //
        //        sizeThread = 1;
        //#pragma omp parallel num_threads(sizeThread) shared(geo, sizeCurvas, sizePatch, malha, novosPontos)
        //        {
        //            Int id = comm->threadId();
        //            //           this->idManagers[id] = this->makeIdManagerOmp(comm, id);

        //            // 4.2. Adapta as curvas pela curvatura da curva
        //#pragma omp for firstprivate(ptr_aux)
        //            for (int i = 0; i < sizeCurvas; ++i )
        //            {
        //                novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurvaOmp( geo->getCurva( i ), this->idManagers[id], 1);
        //                geo->getCurva(i)->setPontos(novosPontos[i]);
        //                novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficieOmp( geo->getCurva( i ), this->idManagers[id], 1);
        //                geo->getCurva(i)->setPontos(novosPontos[i]);
        //                ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
        //            }

        //        }


        //4.2. Adapta as curvas pela curvatura da curva / 4.3. Atualiza a discretização das curvas
        map<Ponto *, Ponto *> mapaPontos;
        timer->initTimerParallel(0, 0, 3); //adpt. das curvas

        for (int i = 0; i < sizeCurvas; ++i )
        {
            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaByCurva( geo->getCurva( i ), mapaPontos, this->idManagers[0], 1);
            geo->getCurva( i )->setPontos(novosPontos[i]);
            novosPontos[i] = AdaptadorPorCurvatura::adaptaCurvaBySuperficie( geo->getCurva( i ), mapaPontos, this->idManagers[0], 1);
            geo->getCurva( i )->setPontos(novosPontos[i]);
            // ((CurvaParametrica*)geo->getCurva(i))->ordenaLista ( );
        }

        timer->endTimerParallel(0, 0, 3); //adpt. das cruvas

#pragma omp parallel num_threads(sizeThread) shared(geo, sizePatch, malha)
        {
            Int id = comm->threadId();
            //((Performer::RangedIdManager *)this->idManagers[id])->setMin(1,0) ;

            timer->initTimerParallel(0, id, 4); //adpt. do domínio

            // 4.3. Adapta as patches
#pragma omp for
            for (int i = 0; i < sizePatch; ++i )
            {
                CoonsPatch *p = static_cast < CoonsPatch* > ( geo->getPatch( i ) );
                SubMalha* sub = AdaptadorPorCurvatura::adaptaDominioOmp ( p, this->idManagers[id], 1);
                sub->setPatch(p);
                malha->insereSubMalha(sub, i);
                geo->getPatch(i)->setMalha(malha->getSubMalha(i));
            }

            timer->endTimerParallel(0, id, 4); //adpt. do domínio
        }

        //        // 4.5. Atualiza os patches
        //        for ( unsigned int i = 0; i < geo->getNumDePatches ( ); ++i )
        //        {
        //            geo->getPatch( i )->setMalha(malha->getSubMalha( i ));
        //        }

        // 4.6. Insere a malha gerada no modelo ( que guarda todas as malhas geradas )
        modelo.insereMalha ( malha );

        // 4.7. Escreve um artigo "neutral file" da malha gerada

#if USE_SAVE_MESH
        escreveMalha(malha, passo);
#endif //#USE_SAVE_MESH


        // 4.7. Calcula o erro global para a malha
        this->erro = this->erroGlobalOmp( malha, timer, 0, sizeThread );

#if USE_PRINT_ERRO
        cout << "ERRO  " << this->passo << " = " << this->erro << endl;
#endif //#if USE_PRINT_COMENT
    }

    return 0;
}

void GeradorAdaptativoPorCurvatura::adaptDomainOmp(Geometria *geo, Malha *malha, Timer *timer, int sizeThread, int sizePatch)
{
#pragma omp parallel num_threads(sizeThread) shared(geo, sizePatch, malha)
    {
        Int id = comm->threadId();

#if USE_MPI
        timer->initTimerParallel(RANK_MPI, id, 4); // Adaptação do domínio
#else
        timer->initTimerParallel(0, id, 4); // Adaptação do domínio
#endif //USE_MPI

        // 4.3. Adapta as patches
#pragma omp for
        for (int i = 0; i < sizePatch; ++i )
        {
            CoonsPatch *p = static_cast < CoonsPatch* > ( geo->getPatch( i ) );
            SubMalha* sub = AdaptadorPorCurvatura::adaptaDominioOmp ( p, this->idManagers[id], 1);
            sub->setPatch(p);
            malha->insereSubMalha(sub, i);
            geo->getPatch(i)->setMalha(malha->getSubMalha(i));
        }
#if USE_MPI
        timer->endTimerParallel(RANK_MPI, id, 4); // Adaptação do domínio
#else
        timer->endTimerParallel(0, id, 4); // Adaptação do domínio
#endif //USE_MPI
    }
}

#endif //USE_OPENMP

SubMalha* GeradorAdaptativoPorCurvatura::malhaInicial(CoonsPatch* patch, Performer::IdManager *idManager)
{

    Curva* c1 = patch->getCurva ( 0 );
    Curva* c2 = patch->getCurva ( 1 );
    Curva* c3 = patch->getCurva ( 2 );
    Curva* c4 = patch->getCurva ( 3 );


    // 1. verifica quais curvas ainda não foram discretizadas
    if ( c1->getNumDePontos ( ) ) c1 = NULL; // c1 já foi trabalhada no patch vizinho
    if ( c2->getNumDePontos ( ) ) c2 = NULL; // c2 já foi trabalhada no patch vizinho
    if ( c3->getNumDePontos ( ) ) c3 = NULL; // c3 já foi trabalhada no patch vizinho
    if ( c4->getNumDePontos ( ) ) c4 = NULL; // c4 já foi trabalhada no patch vizinho

    SubMalha* sub = new SubMalha;

    //========================= Malha Grosseira ====================================
    // 2. divide cada patch em 9 regiões e gera os nós dos extremos de cada região
    for ( double v = 0.0; v <= 1.0; v += 1 )
    {
        for ( double u = 0.0; u <= 1.0; u += 1 )
        {
            //			cout << "u = " << u << " v = " << v << endl;
            Ponto* p = new Noh ( patch->parametrizar ( u, v ) );
            p->id = idManager->next(0);

            //			cout << "ponto " << p->id << " " <<  p->x << " " << p->y << " " << p->z << endl;
            //			cout << "====" << endl;

            if ( v == 0 and c1 ) // p está na curva 1 (c1 = NULL)
                c1-> inserePonto ( p );
            else if ( v == 1 and c3 ) // p está na curva 3
                c3-> inserePonto ( p );

            if ( u == 0 and c4 ) // p está na curva 4
                c4-> inserePonto ( p );
            else if ( u == 1 and c2 ) // p está na curva 2
                c2-> inserePonto ( p );

            sub->insereNoh ( static_cast < Noh* > ( p ) );
        }
    }

    Ponto* p = new Noh ( patch->parametrizar ( 0.5, 0.5 ) );
    sub->insereNoh ( static_cast < Noh* > ( p ) );
    p->id = idManager->next(0);

    Elemento* e1 = new Triangulo (	sub->getNoh ( 0 ),
                                    sub->getNoh ( 1 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e1)->p1 = make_tuple ( 0, 0 );
    ((Triangulo*)e1)->p2 = make_tuple ( 1, 0 );
    ((Triangulo*)e1)->p3 = make_tuple ( 0.5, 0.5 );
    e1->setId ( idManager->next(1));
    sub->insereElemento ( e1 );

    Elemento* e2 = new Triangulo (	sub->getNoh ( 1 ),
                                    sub->getNoh ( 3 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e2)->p1 = make_tuple ( 1, 0 );
    ((Triangulo*)e2)->p2 = make_tuple ( 1, 1 );
    ((Triangulo*)e2)->p3 = make_tuple ( 0.5, 0.5 );
    e2->setId ( idManager->next(1));
    sub->insereElemento ( e2 );

    Elemento* e3 = new Triangulo (	sub->getNoh ( 3 ),
                                    sub->getNoh ( 2 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e3)->p1 = make_tuple ( 1, 1 );
    ((Triangulo*)e3)->p2 = make_tuple ( 0, 1 );
    ((Triangulo*)e3)->p3 = make_tuple ( 0.5, 0.5 );
    e3->setId ( idManager->next(1));
    sub->insereElemento ( e3 );

    Elemento* e4 = new Triangulo (	sub->getNoh ( 2 ),
                                    sub->getNoh ( 0 ),
                                    sub->getNoh ( 4 ) );
    ((Triangulo*)e4)->p1 = make_tuple ( 0, 1 );
    ((Triangulo*)e4)->p2 = make_tuple ( 0, 0 );
    ((Triangulo*)e4)->p3 = make_tuple ( 0.5, 0.5 );
    e4->setId ( idManager->next(1));
    sub->insereElemento ( e4 );
    //==============================================================================*/

    // 5. define a submalha do patch
    patch->setMalha ( sub );
    sub->setPatch ( patch );

    return sub;
}

// gera a malha inicial e insere na lista de malhas do modelo
// a lista de pontos da curva é preenchida durante a geração da malha inicial

// calcula o erro global da malha
double GeradorAdaptativoPorCurvatura::erroGlobal(Malha* malha, Timer *timer, int rank, int sizeThread)
{

    unsigned int Ns = 0; // número de submalhas
    unsigned int Nv = 0; // número de vértices
    double Njs = 0; // erro global da submalha
    double curvPower = 0.0;
    double Nj = 0; // erro global da malha
    SubMalha* sub = 0;

    Ns = malha->getNumDeSubMalhas ( );

#if USE_MPI
    timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
    timer->initTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI

    // Calcula o erro global de cada submalha
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        sub = malha->getSubMalha ( i );
        Nv = sub->getNumDeNos ( );
        Njs = 0.0;
        curvPower = 0.0;

        // Calcula o erro relativo para cada nó e soma a Nj
        for ( unsigned int j = 0; j < Nv; ++j )
        {
#if USE_MPI
            timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
            timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 6); //MediaGauss
#else
            timer->endTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
            timer->initTimerParallel(0,omp_get_thread_num(), 6); //MediaGauss
#endif //USE_MPI

            Ponto *n = sub->getNoh ( j );
            Patch *p = sub->getPatch (  );
            CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
            CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();
            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;
#if USE_MPI
            timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 6); //MediaGauss
            timer->initTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
            timer->endTimerParallel(0,omp_get_thread_num(), 6); //MediaGauss
            timer->initTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI
            double power = 0.0;
            double diff = 0.0;

            if ( fabs ( Ga ) >= TOLERANCIA )
            {
                diff = Gd - Ga;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
            }
            else if ( fabs ( Ha ) >= TOLERANCIA )
            {
                diff = Hd - Ha;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
            }
        }

        if (Njs > 0.0 && curvPower > 0.0) {
            Njs =(double) sqrt( Njs / curvPower) / Nv;
        }

        Nj += Njs;
    }


    Nj /= Ns; // o erro global é a média do erro das submalhas

#if USE_MPI
    timer->endTimerParallel(RANK_MPI,omp_get_thread_num(), 7); //calculo do erro global
#else
    timer->endTimerParallel(0,omp_get_thread_num(), 7); //calculo do erro global
#endif //USE_MPI

    return Nj;
}

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManager(const Parallel::TMCommunicator *comm, Int id) const
{
    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, /*this->idManager->getId(0)*/ 0 + procOffset + threadOffset);
    manager->setMin(1, /*this->idManager->getId(1)*/ 0 + procOffset + threadOffset);

    return manager;
}

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManagerOmp(const Parallel::TMCommunicator *comm, Int id) const
{
    Int iNoh, iElemet;
    if (this->idManagers[id]) {
        iNoh = this->idManagers[id]->getId(0);
        iElemet = this->idManagers[id]->getId(1);
    } else {
        iNoh = 0;
        iElemet = 0;
    }

    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, iNoh + procOffset + threadOffset);
    manager->setMin(1, iElemet + procOffset + threadOffset);

    return manager;
}

Performer::IdManager *GeradorAdaptativoPorCurvatura::makeIdManagerElementOmp(const Parallel::TMCommunicator *comm, Int id) const
{
    Int iNoh;
    if (this->idManagers[id]) {
        iNoh = this->idManagers[id]->getId(0);
    } else {
        iNoh = 0;
    }

    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 2);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);
    manager->setMin(0, iNoh + procOffset + threadOffset);
    manager->setMin(1, 0 + procOffset + threadOffset);

    return manager;
}

void GeradorAdaptativoPorCurvatura::salvarErroMalha(Malha *malha)
{
    cout << "Salvando a Malha com "<< malha->getNumDeSubMalhas ( )<<" subMalhas"<< endl;

    unsigned int Ns = 0; // número de submalhas
    unsigned int Nv = 0; // número de vértices
    double Njs = 0; // erro global da submalha
    double curvPower = 0.0;
    //double Nj = 0; // erro global da malha
    SubMalha* sub = 0;


    // Escreve arquivo com as curvaturas
    stringstream nome;
    nome << passo;
    nome << "erro";
    nome << passo;
    nome << ".txt";

    ofstream arquivo(nome.str().c_str());

    Ns = malha->getNumDeSubMalhas ( );

    // Calcula o erro global de cada submalha
    for ( unsigned int i = 0; i < Ns; ++i )
    {
        sub = malha->getSubMalha ( i );
        Nv = sub->getNumDeNos ( );
        Njs = 0.0;
        curvPower = 0.0;

        // Calcula o erro relativo para cada nó e soma a Nj
        for ( unsigned int j = 0; j < Nv; ++j )
        {
            Ponto *n = sub->getNoh ( j );
            Patch *p = sub->getPatch (  );
            CurvaturaAnalitica ka (	*( static_cast < Noh* > ( n ) ), *( static_cast < CoonsPatch* > ( p ) ) );
            CurvaturaDiscreta kd ( *( static_cast < Noh* > ( n ) ) );
            double Ga = ka.gauss();
            double Gd = kd.gauss();
            double Ha = ka.media();
            double Hd = kd.media();
            // atualiza as curvaturas do nó ( para que não sejam recalculadas na
            // adaptação das curvas e do domínio )
            ((Noh*)n)->Ga = Ga;
            ((Noh*)n)->Gd = Gd;
            ((Noh*)n)->Ha = Ha;
            ((Noh*)n)->Hd = Hd;

            arquivo << "P " << n->id << ": ( " << n->x << ", " << n->y << ", " << n->z << ")" << endl;
            tuple <double, double> t_n = ((HermitePatch*)p)->encontrar_u_v ( *n );
            arquivo << "\tu = " << get<0>(t_n) << " v = " << get<1>(t_n) << endl;
            unsigned int num = ((Noh*)n)->getNumDeElementos();
            arquivo << "\t" << num << " elementos incidentes:";
            for (unsigned int i = 0; i < num; ++i)
            {
                Elemento* elem = ((Noh*)n)->getElemento(i);
                arquivo << " T-" << elem->getId();
            }
            arquivo << endl;
            arquivo << "\tGd = " << ((Noh*)n)->Gd << " Ga = " << ((Noh*)n)->Ga << endl;
            arquivo << "\tHd = " << ((Noh*)n)->Hd << " Ha = " << ((Noh*)n)->Ha << endl;

            double power = 0.0;
            double diff = 0.0;

            if ( fabs ( Ga ) >= TOLERANCIA )
            {
                diff = Gd - Ga;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ga, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Gd << " Ca = " << ((Noh*)n)->Ga << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if ( fabs(diff) <= TOLERANCIA )
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            }
            else if ( fabs ( Ha ) >= TOLERANCIA )
            {
                diff = Hd - Ha;
                power = pow( diff, 2);
                Njs += power;
                curvPower += pow(Ha, 2);
                arquivo << "\tCd = " << ((Noh*)n)->Hd << " Ca = " << ((Noh*)n)->Ha << endl;
                arquivo << "\t|Cd - Ca| = " << fabs(diff) << endl;
                if ( fabs(diff) <= TOLERANCIA )
                    arquivo << "\tdiferença menor que tolerância!!" << endl;
            }

            arquivo << endl;


        }

        if (Njs > 0.0 && curvPower > 0.0) {
            Njs =(double) sqrt( Njs / curvPower) / Nv;
        }
    }

    arquivo.close();

    cout << "Malha salva com sucesso!!!" << endl;
}

void GeradorAdaptativoPorCurvatura::escreveMalha(Malha *malha, int passo)
{
    stringstream nome;
    nome << passo;
    nome << "malha";
    nome << passo;
    nome << ".pos";

    ofstream arq(nome.str().c_str());

    arq << "%HEADER" << endl
        << "Arquivo gerado pelo gerador de malhas de superficie" << endl << endl;

    arq << "%HEADER.VERSION" << endl
        << "0-005 - Oct/93" << endl << endl
        << "%HEADER.ANALYSIS" << endl
        << "\'shell\'" << endl << endl;


    unsigned long int Nv, Nt;
    Nv = Nt = 0;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        Nv += sub->getNumDeNos();
        Nt += sub->getNumDeElementos();
    }

    arq << "%NODE" << endl
        << Nv << endl << endl;

    arq << "%NODE.COORD" << endl
        << Nv << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeNos(); j++)
        {
            Noh *n = sub->getNoh(j);

            arq << n->id << " "
                << n->x << " "
                << n->y << " "
                << n->z << endl;
        }
    }

    arq << endl;

    arq << "%MATERIAL" << endl
        << "1" << endl << endl
        << "%MATERIAL.LABEL" << endl
        << "1" << endl
        << "1\t\'m1\'" << endl << endl
        << "%MATERIAL.ISOTROPIC" << endl
        << "1" << endl
        << "1\t1000.0\t0.0" << endl << endl
        << "%THICKNESS" << endl
        << "1" << endl
        << "1\t1.0" << endl << endl
        << "%INTEGRATION.ORDER" << endl
        << "1" << endl
        << "1\t3\t1\t1\t3\t1\t1" << endl << endl;

    arq << "%ELEMENT" << endl
        << Nt << endl << endl;

    arq << "%ELEMENT.T3" << endl
        << Nt << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++)
    {
        SubMalha *sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeElementos(); j++)
        {
            Triangulo *t = (Triangulo *)sub->getElemento(j);

            arq << t->getId() << " "
                << "1 1 1 "
                << t->getN(1).id << " "
                << t->getN(2).id << " "
                << t->getN(3).id << endl;
        }
    }

    arq << endl;
    arq << "%END";

    arq.flush();

    arq.close();

    //  cout << "escreveu o arquivo para o passo " << passo << endl;
}

void GeradorAdaptativoPorCurvatura::escreveMalha(Malha* malha, int passo, vector<double> erroPasso, int rank)
{
    stringstream nome;
    if (rank == -1) {
        nome << nameModel;
        nome << "_passo_";
        nome << passo;
        nome << "_malha_";
        nome << passo;
        nome << ".pos";
    } else {
        nome << nameModel;
        nome << "_n.process_";
        nome << numberProcess;
        nome << "_passo_";
        nome << passo;
        nome << "_malha_";
        nome << passo;
        nome << "_rank_";
        nome << rank;
        nome << ".pos";
    }

    ofstream arq(nome.str().c_str());

    arq << "%HEADER" << endl << "Arquivo gerado pelo gerador de malhas de superficie" << endl;

    unsigned long int Nv, Nt;
    Nv = Nt = 0;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        Nv += sub->getNumDeNos();
        Nt += sub->getNumDeElementos();
    }

    arq << "%HEADER.VERSION" << endl << "0-005 - Oct/93" << endl << endl << "%HEADER.ANALYSIS" << endl << "\'shell\'"
        << endl << endl;

    arq << "erro global em cada passo" << endl;
    int n_pas = 0;
    for (vector<double>::iterator it = erroPasso.begin(); it != erroPasso.end(); it++) {
        arq << "passo: " << n_pas << " erro:" << (*it)<< endl;
        n_pas ++;
    }
    arq << endl;

    arq << "%NODE" << endl
        << Nv << endl << endl;

    arq << "%NODE.COORD" << endl
        << Nv << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeNos(); j++) {
            Noh* n = sub->getNoh(j);

            arq << n->id << " " << n->x << " " << n->y << " " << n->z << endl;
        }
    }

    arq << endl;

    arq << "%MATERIAL" << endl << "1" << endl << endl << "%MATERIAL.LABEL" << endl << "1" << endl << "1\t\'m1\'" << endl
        << endl << "%MATERIAL.ISOTROPIC" << endl << "1" << endl << "1\t1000.0\t0.0" << endl << endl << "%THICKNESS"
        << endl << "1" << endl << "1\t1.0" << endl << endl << "%INTEGRATION.ORDER" << endl << "1" << endl
        << "1\t3\t1\t1\t3\t1\t1" << endl << endl;

    arq << "%ELEMENT" << endl << Nt << endl << endl;

    arq << "%ELEMENT.T3" << endl << Nt << endl;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeElementos(); j++) {
            Triangulo* t = (Triangulo*)sub->getElemento(j);

            arq << t->getId() << " "
                << "1 1 1 " << t->getN(1).id << " " << t->getN(2).id << " " << t->getN(3).id << endl;
        }
    }

    arq << endl;
    arq << "%END";

    arq.flush();

    arq.close();

    cout << "escreveu o arquivo para o passo " << passo << endl;


    //Análise dos Elementos da Malha Gerada

    // cout<< "INIT >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
    stringstream nameFile;

    nameFile << nameModel;
    nameFile << "_n.process_";
    nameFile << numberProcess;
    nameFile << "_passo_";
    nameFile << passo;
    nameFile << "_qualite_";
    nameFile << passo;
    nameFile << "_rank_";
    nameFile << rank;
    nameFile << ".txt";

    ofstream file(nameFile.str().c_str());

    file << "File Analise qualidade" << endl << endl;

    std::vector<double> vec_0_10;
    std::vector<double> vec_10_20;
    std::vector<double> vec_20_30;
    std::vector<double> vec_30_40;
    std::vector<double> vec_40_50;
    std::vector<double> vec_50_60;
    std::vector<double> vec_60_70;
    std::vector<double> vec_70_80;
    std::vector<double> vec_80_90;
    std::vector<double> vec_90_100;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeElementos(); j++) {

            Triangulo* t = (Triangulo*)sub->getElemento(j);

            double value = t->quality();

            if (0.0 <= value and value <= 0.1) {
                vec_0_10.push_back(value);
            } else if (0.1 < value and value <= 0.2) {
                vec_10_20.push_back(value);
            } else if (0.2 < value and value <= 0.3) {
                vec_20_30.push_back(value);
            } else if (0.3 < value and value <= 0.4) {
                vec_30_40.push_back(value);
            } else if (0.4 < value and value <= 0.5) {
                vec_40_50.push_back(value);
            } else if (0.5 < value and value <= 0.6) {
                vec_50_60.push_back(value);
            } else if (0.6 < value and value <= 0.7) {
                vec_60_70.push_back(value);
            } else if (0.7 < value and value <= 0.8) {
                vec_70_80.push_back(value);
            } else if (0.8 < value and value <= 0.9) {
                vec_80_90.push_back(value);
            } else if (0.9 < value and value <= 1) {
                vec_90_100.push_back(value);
            }

        }
    }

    file <<"Quantidade elementos em cada caso de 0 a 1"<<endl;
    file << vec_0_10.size()<< endl;
    file << vec_10_20.size()<< endl;
    file << vec_20_30.size()<< endl;
    file << vec_30_40.size()<< endl;
    file << vec_40_50.size()<< endl;
    file << vec_50_60.size()<< endl;
    file << vec_60_70.size()<< endl;
    file << vec_70_80.size()<< endl;
    file << vec_80_90.size()<< endl;
    file << vec_90_100.size()<< endl << endl;

    file <<"Porcetagem elementos em cada caso de 0 a 1"<<endl;

    long porc = vec_0_10.size() + vec_10_20.size() + vec_20_30.size() + vec_30_40.size() + vec_40_50.size() + vec_50_60.size() + vec_60_70.size() + vec_70_80.size() + vec_80_90.size() + vec_90_100.size();

    file << vec_0_10.size()*100/(double)porc<<endl;
    file << vec_10_20.size()*100/(double)porc<<endl;
    file << vec_20_30.size()*100/(double)porc<<endl;
    file << vec_30_40.size()*100/(double)porc<<endl;
    file << vec_40_50.size()*100/(double)porc<<endl;
    file << vec_50_60.size()*100/(double)porc<<endl;
    file << vec_60_70.size()*100/(double)porc<<endl;
    file << vec_70_80.size()*100/(double)porc<<endl;
    file << vec_80_90.size()*100/(double)porc<<endl;
    file << vec_90_100.size()*100/(double)porc<<endl;

    file.flush();

    file.close();

    //cout<< "END >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
}

void GeradorAdaptativoPorCurvatura::writeQualityMesh(Malha *malha, int passo, vector<double> erroPasso, int rank)
{
    //Análise dos Elementos da Malha Gerada

    // cout<< "INIT >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
    stringstream nameFile;

    nameFile << nameModel;
    nameFile << "_n.process_";
    nameFile << numberProcess;
    nameFile << "_passo_";
    nameFile << passo;
    nameFile << "_qualite_";
    nameFile << passo;
    nameFile << "_rank_";
    nameFile << rank;
    nameFile << ".txt";

    ofstream file(nameFile.str().c_str());

    file << "File Analise qualidade" << endl << endl;

    std::vector<double> vec_0_10;
    std::vector<double> vec_10_20;
    std::vector<double> vec_20_30;
    std::vector<double> vec_30_40;
    std::vector<double> vec_40_50;
    std::vector<double> vec_50_60;
    std::vector<double> vec_60_70;
    std::vector<double> vec_70_80;
    std::vector<double> vec_80_90;
    std::vector<double> vec_90_100;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        for (unsigned int j = 0; j < sub->getNumDeElementos(); j++) {

            Triangulo* t = (Triangulo*)sub->getElemento(j);

            double value = t->quality();

            if (0.0 <= value and value <= 0.1) {
                vec_0_10.push_back(value);
            } else if (0.1 < value and value <= 0.2) {
                vec_10_20.push_back(value);
            } else if (0.2 < value and value <= 0.3) {
                vec_20_30.push_back(value);
            } else if (0.3 < value and value <= 0.4) {
                vec_30_40.push_back(value);
            } else if (0.4 < value and value <= 0.5) {
                vec_40_50.push_back(value);
            } else if (0.5 < value and value <= 0.6) {
                vec_50_60.push_back(value);
            } else if (0.6 < value and value <= 0.7) {
                vec_60_70.push_back(value);
            } else if (0.7 < value and value <= 0.8) {
                vec_70_80.push_back(value);
            } else if (0.8 < value and value <= 0.9) {
                vec_80_90.push_back(value);
            } else if (0.9 < value and value <= 1) {
                vec_90_100.push_back(value);
            }

        }
    }

    file <<"Quantidade elementos em cada caso de 0 a 1"<<endl;
    file << vec_0_10.size()<< endl;
    file << vec_10_20.size()<< endl;
    file << vec_20_30.size()<< endl;
    file << vec_30_40.size()<< endl;
    file << vec_40_50.size()<< endl;
    file << vec_50_60.size()<< endl;
    file << vec_60_70.size()<< endl;
    file << vec_70_80.size()<< endl;
    file << vec_80_90.size()<< endl;
    file << vec_90_100.size()<< endl << endl;

    file <<"Porcetagem elementos em cada caso de 0 a 1"<<endl;

    long porc = vec_0_10.size() + vec_10_20.size() + vec_20_30.size() + vec_30_40.size() + vec_40_50.size() + vec_50_60.size() + vec_60_70.size() + vec_70_80.size() + vec_80_90.size() + vec_90_100.size();

    file << vec_0_10.size()*100/(double)porc<<endl;
    file << vec_10_20.size()*100/(double)porc<<endl;
    file << vec_20_30.size()*100/(double)porc<<endl;
    file << vec_30_40.size()*100/(double)porc<<endl;
    file << vec_40_50.size()*100/(double)porc<<endl;
    file << vec_50_60.size()*100/(double)porc<<endl;
    file << vec_60_70.size()*100/(double)porc<<endl;
    file << vec_70_80.size()*100/(double)porc<<endl;
    file << vec_80_90.size()*100/(double)porc<<endl;
    file << vec_90_100.size()*100/(double)porc<<endl;

    file.flush();

    file.close();

    // cout<< "END >> ANÁLISE DOS ELEMENTOS DA MALHA GERADA"<< endl;
}

void GeradorAdaptativoPorCurvatura::salvaMalha(Malha* malha, int passo)
{
    saveMesh.push_back(make_pair(passo, malha));
}

void GeradorAdaptativoPorCurvatura::salvaErroMalha(Malha* malha, int passo)
{
    saveErroMesh.push_back(make_pair(passo, malha));
}

void escreveElementos( int passo, SubMalha *sub, int i )
{
    stringstream nome;
    nome << passo;
    nome << "submalha-";
    nome << i;
    nome << ".txt";

    ofstream arq(nome.str().c_str());

    for ( unsigned int k = 0; k < sub->getNumDeElementos(); ++k)
    {
        Elemento *elem = sub->getElemento( k );

        Noh n1 ( elem->getN ( 1 ) );
        Noh n2 ( elem->getN ( 2 ) );
        Noh n3 ( elem->getN ( 3 ) );

        tuple <double, double> t1 = ((Triangulo*)elem)->p1;
        tuple <double, double> t2 = ((Triangulo*)elem)->p2;
        tuple <double, double> t3 = ((Triangulo*)elem)->p3;

        arq	<< "T-" << elem->getId() << ":\n\t"
            << "área = " << elem->getArea() << ";\n\t"
            << "normal = " << elem->getNormal().x << ", " << elem->getNormal().y << ", " << elem->getNormal().z << "\n\t"
            << "n-" << n1.id << "( " << get<0>(t1) << " , " << get<1>(t1) <<  ") ângulo = " << elem->getAngulo( n1 ) << ";\n\t"
            << "n-" << n2.id << "( " << get<0>(t2) << " , " << get<1>(t2) <<  ") ângulo = " << elem->getAngulo( n2 ) << ";\n\t"
            << "n-" << n3.id << "( " << get<0>(t3) << " , " << get<1>(t3) <<  ") ângulo = " << elem->getAngulo( n3 ) << endl;
    }

    arq.flush();

    arq.close();

    cout << "escreveu o arquivo com os elementos da submalha " << i << " para o passo " << passo << endl;
}

void GeradorAdaptativoPorCurvatura::generatorInitialMesh(Geometria *geo, Malha *malha,Timer *timer, int sizeThread, int sizePatch)
{
#if USE_OPENMP
#pragma omp parallel num_threads(sizeThread) shared(malha, geo, sizePatch)
    {
        Int id = comm->threadId();


        if (!this->idManagers[id])
        {
            this->idManagers[id] = this->makeIdManagerOmp(comm, id);
        }
#if USE_MPI
        timer->initTimerParallel(RANK_MPI, id , 2); // Malha inicial
#else
        timer->initTimerParallel(0, id , 2); // Malha inicial
#endif //USE_MPI

        // 1. Gera a malha inicial
#pragma omp for
        for (int i = 0; i < sizePatch; ++i )
        {
            CoonsPatch *patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
            SubMalha *sub = this->malhaInicialOmp( static_cast < CoonsPatch* > ( patch ), this->idManagers[id]);
            malha->insereSubMalha (sub,i);
        }

#if USE_MPI
        timer->endTimerParallel(RANK_MPI, id , 2); // Malha inicial
#else
        timer->endTimerParallel(0, id , 2); // Malha inicial
#endif //USE_MPI

    }
#else
    for (int i = 0; i < sizePatch; ++i )
    {
        CoonsPatch *patch = static_cast < CoonsPatch* > ( geo->getPatch ( i ) );
        SubMalha *sub = this->malhaInicial( static_cast < CoonsPatch* > ( patch ), this->idManagers[0]);
        malha->insereSubMalha (sub,i);
    }
#endif //USE_OPENMP
}

void GeradorAdaptativoPorCurvatura::printElments(Malha *malha, int passo, vector<double> erroPasso, int rank)
{
    unsigned long int Nv, Nt;
    Nv = Nt = 0;

    for (unsigned int i = 0; i < malha->getNumDeSubMalhas(); i++) {
        SubMalha* sub = malha->getSubMalha(i);

        Nv += sub->getNumDeNos();
        Nt += sub->getNumDeElementos();
    }

    cout << "#elementos_" << nameModel << "_n.process_" << numberProcess << "_passo_" << passo << "_rank_" << rank << " = " << Nt << endl;
}
