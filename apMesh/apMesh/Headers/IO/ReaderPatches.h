#ifndef READERPATCHES_H
#define READERPATCHES_H

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "../Data/Patch/BezierPatch.h"
#include "../Data/Ponto.h"
#include "../Data/Patch/HermitePatch.h"

extern std::string entrada;

class ReaderPatches
{
public:

    std::list<BezierPatch*> patches;
    std::list<HermitePatch*> patchesHermite;
    BezierPatch *patch;
    HermitePatch *patchHermite;

    ReaderPatches();

    std::list<BezierPatch *> loaderBezierPatchFile();
    Ponto getPointVectorControlPoints(std::vector<Ponto> , unsigned long );



};

#endif // READERPATCHES_H
