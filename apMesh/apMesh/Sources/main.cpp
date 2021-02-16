#include "../Headers/Data/Patch/HermitePatch.h"
#include "../Headers/Data/Patch/BezierPatch.h"
#include "../Headers/Data/Vertice.h"
#include "../Headers/Data/Vetor.h"
#include "../Headers/Data/Curve/CurvParamHermite.h"
#include "../Headers/Data/Curve/CurvParamBezier.h"
#include "../Headers/Data/Modelo.h"
#include "../Headers/Generator/GeradorAdaptativoPorCurvatura.h"
#include "../Headers/IO/Modelos3d.h"
#include "../Headers/Data/Definitions.h"
#include "../Headers/IO/ReaderPatches.h"
#include "../Headers/Timer/Timer.h"

double TRIANGLE_MEDIO = 0.0;
double TOLERANCIA_ESTIMATIVE = 1.0;

double DELTA = 0.0001;
double TOLERANCIA = 0.0001;
double TOLERANCIA_CURVATURA = 0.001;
double TOLERANCIA_AFT = 0.001;
double PROPORCAO = 0.5; // proporção usada no avanço de fronteira
double SUAVIZACAO = 7; // número de vezes que se dará a suavização laplaciana
double FATOR_SUAVIZACAO = 0.5; // fator usado na suavização laplaciana
double EPSYLON = 0.003;
double DISCRETIZACAO_CURVA = 1.414213562;
double DISCRETIZACAO_INTER = sqrt(DISCRETIZACAO_CURVA);
//int NUM_THREADS = 8;

std::set<Ponto*> listAllPointsModel;
std::set<SubMalha*> listAllSubMalhaModel;
#if USE_MPI
int RANK_MPI, SIZE_MPI;
#endif //#if USE_OPENMPI

std::string nameModel;
std::string entrada;
std::string numberProcess;
std::string WRITE_MESH;

// argv[0] = "executavel: ./apmesh",
// argv[1] = "entrada",       OBS: Projects-> Comands line arguments -> ../../apMesh/Entrada/mountain_289_patches.bp


int main(int argc, char **argv)
{
    // Criação e inicialização do contador de tempo com todos os valores 0(zero)
    Timer* timer = new Timer( );

    // contador do tempo para carregar a malha na memória
    timer->initTime(0); // inicialização

    bool geraMalha = true;
    Modelo M;
    std::list<BezierPatch*> listBezierPt;
    Geometria *geo = new Geometria;


    if (argc == 1)
    {
        Modelos3d modelos3d;
        M.setGeometria ( modelos3d.modelDoisPatches(geo) );
    }
    else
    {
        entrada = argv[1];
        ReaderPatches* pt = new ReaderPatches();
        listBezierPt = pt->loaderBezierPatchFile();
        delete pt;

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

        for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++) {

            p00 = new Vertice((*it)->getPt00().x, (*it)->getPt00().y, (*it)->getPt00().z);
            p00->setId((*it)->getPt00().id);
            p10 = new Vertice((*it)->getPt10().x, (*it)->getPt10().y, (*it)->getPt10().z);
            p10->setId((*it)->getPt10().id);
            p20 = new Vertice((*it)->getPt20().x, (*it)->getPt20().y, (*it)->getPt20().z);
            p20->setId((*it)->getPt20().id);
            p30 = new Vertice((*it)->getPt30().x, (*it)->getPt30().y, (*it)->getPt30().z);
            p30->setId((*it)->getPt30().id);

            p01 = new Vertice((*it)->getPt01().x, (*it)->getPt01().y, (*it)->getPt01().z);
            p01->setId((*it)->getPt01().id);
            p11 = new Vertice((*it)->getPt11().x, (*it)->getPt11().y, (*it)->getPt11().z);
            p11->setId((*it)->getPt11().id);
            p21 = new Vertice((*it)->getPt21().x, (*it)->getPt21().y, (*it)->getPt21().z);
            p21->setId((*it)->getPt21().id);
            p31 = new Vertice((*it)->getPt31().x, (*it)->getPt31().y, (*it)->getPt31().z);
            p31->setId((*it)->getPt31().id);

            p02 = new Vertice((*it)->getPt02().x, (*it)->getPt02().y, (*it)->getPt02().z);
            p02->setId((*it)->getPt02().id);
            p12 = new Vertice((*it)->getPt12().x, (*it)->getPt12().y, (*it)->getPt12().z);
            p12->setId((*it)->getPt12().id);
            p22 = new Vertice((*it)->getPt22().x, (*it)->getPt22().y, (*it)->getPt22().z);
            p22->setId((*it)->getPt22().id);
            p32 = new Vertice((*it)->getPt32().x, (*it)->getPt32().y, (*it)->getPt32().z);
            p32->setId((*it)->getPt32().id);

            p03 = new Vertice((*it)->getPt03().x, (*it)->getPt03().y, (*it)->getPt03().z);
            p03->setId((*it)->getPt03().id);
            p13 = new Vertice((*it)->getPt13().x, (*it)->getPt13().y, (*it)->getPt13().z);
            p13->setId((*it)->getPt13().id);
            p23 = new Vertice((*it)->getPt23().x, (*it)->getPt23().y, (*it)->getPt23().z);
            p23->setId((*it)->getPt23().id);
            p33 = new Vertice((*it)->getPt33().x, (*it)->getPt33().y, (*it)->getPt33().z);
            p33->setId((*it)->getPt33().id);

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

            (*it) = new BezierPatch(patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21, *p12, *p22);
            //            (*it) = new BezierPatch(*p00, *p01, *p02, *p03,
            //                                    *p10, *p11, *p12, *p13,
            //                                    *p20, *p21, *p22, *p23,
            //                                    *p30, *p31, *p12, *p13,);
            geo->inserePatch((*it));
        }

        M.setGeometria(geo);
    }

    if ( geraMalha )
    {
        // fim do contador do tempo para carregar a malha na memória
        timer->endTime(0); // inicialização

        // contador do tempo de inicialização em segundos em todos os processos
        timer->initTime(10); // Full

#if USE_OPENMP
    GeradorAdaptativoPorCurvatura adaptiveMesh (M, timer, 8);
#else
    GeradorAdaptativoPorCurvatura adaptiveMesh (M, timer, 8);
#endif //#USE_OPENMP


        timer->endTime(10); // Full

#if USE_PRINT_TIME
        timer->writeTimeFile();
#endif //#if USE_PRINT_TIME

#if USE_PRINT_ERRO
        cout << "Erro global final = " << adaptiveMesh.erro << endl;
#endif //#if USE_PRINT_ERRO

    }
    else
    {
        cout <<"Erro ao ler modelo, por favor inserir modelo ou verificar se é válido"<<endl;
    }

    return 0;
}
