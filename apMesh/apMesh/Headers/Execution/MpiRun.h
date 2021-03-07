#ifndef MPIRUN_H
#define MPIRUN_H

#include "../Timer/Timer.h"
#include "../Data/Definitions.h"
#include "../Generator/GeradorAdaptativoPorCurvatura.h"
#include "../Estimate/ChargeEstimateProcess.h"

extern int SIZE_MPI, RANK_MPI;

class MpiRun
{
public:
    MpiRun();
    ~MpiRun();
    int execute(int argc, char *argv[], Timer *timer);
    void estimateChargeofPatches(Timer *timer, std::string entrada);
    void createVectorOfCurves();
    bool verifyCurve(Ponto p0, Ponto p1, Ponto p2, Ponto p3);
    std::list<BezierPatch *> orderPatchesDistribProcess(std::list<BezierPatch*>);
    std::list<BezierPatch *>::iterator getIteratorListPatches(int numberPatches);


    std::list<BezierPatch*> listBezierPt;
    std::vector<Curva*> curves;

    double erroMeshProcess;
    int passo;

    Modelo modelo;
    Geometria *geo;
    CoonsPatch *patch;
    Malha *malha;
};

#endif // MPIRUN_H
