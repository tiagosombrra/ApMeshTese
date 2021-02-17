#include "../../Headers/Execution/SequentialRun.h"

SequentialRun::SequentialRun()
{

}

int SequentialRun::execute(char *argv[], Timer *timer)
{
    Modelo M;
    Geometria *geo = new Geometria;

    //Inclusão dos patches no Modelo
    if (argv[4])
    {
        M.setGeometria(readerPatches(geo, argv[4]));
    }
    else
    {
        Modelos3d modelos3d;
        M.setGeometria ( modelos3d.modelPlanBook(geo) );
    }

    // fim do contador do tempo para carregar a malha na memória
    timer->endTimerParallel(0, 0, 0); // inicialização

    // contador do tempo total
    timer->initTimerParallel(0, 0, 10); // Full

    //Gerador da malha
    GeradorAdaptativoPorCurvatura adaptiveMesh (M, timer, 8);

    //fim do contador do tempo total
    timer->endTimerParallel(0, 0, 10); // Full

#if USE_PRINT_TIME
    timer->printTime(0,0);
#endif //#if USE_PRINT_TIME

#if USE_PRINT_ERRO
    cout << "Erro global final = " << adaptiveMesh.erro << endl;
#endif //#if USE_PRINT_ERRO

    return 0;
}

Geometria *SequentialRun::readerPatches(Geometria *geo, string fileName)
{
    ReaderPatches* pt = new ReaderPatches();
    std::list<BezierPatch*> listBezierPt = pt->loaderBezierPatchFile(fileName);
    delete pt;

    Ponto* p00; Ponto* p01; Ponto* p02; Ponto* p03;
    Ponto* p10; Ponto* p11; Ponto* p12; Ponto* p13;
    Ponto* p20; Ponto* p21; Ponto* p22; Ponto* p23;
    Ponto* p30; Ponto* p31; Ponto* p32; Ponto* p33;
    Curva* patch_c1; Curva* patch_c2; Curva* patch_c3; Curva* patch_c4;

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

        geo->inserePatch((*it));
    }

    return geo;
}
