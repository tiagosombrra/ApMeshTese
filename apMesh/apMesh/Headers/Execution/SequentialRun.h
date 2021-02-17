#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "../Data/Definitions.h"
#include "../Timer/Timer.h"
#include "../Data/Modelo.h"
#include "../Data/Patch/BezierPatch.h"
#include "../IO/Modelos3d.h"
#include "../IO/ReaderPatches.h"
#include "../Generator/GeradorAdaptativoPorCurvatura.h"


class SequentialRun
{
public:
    SequentialRun();
    int execute(char *argv[], Timer *timer);
    Geometria* readerPatches(Geometria *geo, string fileName);


};

#endif // SEQUENTIAL_H
