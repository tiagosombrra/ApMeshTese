#ifndef OMPRUN_H
#define OMPRUN_H

#include "../Data/Definitions.h"
#include "../Timer/Timer.h"
#include "../Data/Modelo.h"
#include "../Data/Patch/BezierPatch.h"
#include "../IO/Modelos3d.h"
#include "../IO/ReaderPatches.h"
#include "../Generator/GeradorAdaptativoPorCurvatura.h"
class OmpRun
{
public:
    OmpRun();
    int execute(int argc, char *argv[], Timer *timer);

};

#endif // OMPRUN_H
