#ifndef METHOD_SEQUENTIAL_H
#define METHOD_SEQUENTIAL_H

#include "../IO/PatchBezierReader.h"
#include "../Estimate/ChargeEstimateProcess.h"
#include "../Data/Modelo.h"
#include "../IO/Modelos3d.h"
#include "../Timer/Timer.h"

class Method_Sequential
{
public:
    Method_Sequential(int argc, char *argv[]);
    ~Method_Sequential();
    int execute(Timer *timer);

    std::list<BezierPatch*> listBezierPt;
    Modelo M;
	bool reader_patches_bezier;
	int id_exemplo;
	bool geraMalha;


};

#endif // METHOD_SEQUENTIAL_H
