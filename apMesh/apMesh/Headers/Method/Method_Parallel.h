#ifndef METHOD_PARALLEL_H
#define METHOD_PARALLEL_H

#include "../Data/Definitions.h"
#include "../Estimate/ChargeEstimateProcess.h"
#if USE_OPENMPI
#include "mpi.h"
#endif //#if USE_OPENMPI
#include "../Data/Modelo.h"
#include "../Data/Mesh/Malha.h"
#include "../Timer/Timer.h"

//modificado por markos
//static unsigned long int idv1 = 1;
//static unsigned long int ide1 = 1;
//end modificado por markos

extern int SIZE_MPI, RANK_MPI;

class Method_Parallel
{
public:
    Method_Parallel();
    std::list<BezierPatch*>::iterator getIteratorListPatches(int numberPatches);
    int execute(int argc, char *argv[], Timer *timer);
    //Method_Parallel(int argc, char *argv[], int);
	std::list<BezierPatch*> orderPatchesDistribProcess(std::list<BezierPatch*>);
    ~Method_Parallel();
	
	bool verifyCurve(Ponto, Ponto, Ponto, Ponto);
	
    void estimateChargeofPatches(Timer* timer);
	void createVectorOfCurves();
	void adapterCurves();
	
	SubMalha* malhaInicial(CoonsPatch* patch);
	double erroGlobal(Malha* malha);
	
	std::list<BezierPatch*> listBezierPt;
	std::vector<Curva*> curves;
	
	double erroMeshProcess;
	int passo;
	
    Modelo modelo;
	Geometria *geo;
	CoonsPatch *patch;
	Malha *malha;
};

#endif // METHOD_PARALLEL_H
