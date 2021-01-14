#ifndef READERPATCHES_H
#define READERPATCHES_H

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "BezierPatch.h"
#include "Ponto.h"
#include "HermitePatch.h"

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
