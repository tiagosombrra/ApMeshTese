#ifndef PATCHBEZIERREADER_H
#define PATCHBEZIERREADER_H

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "../Data/Patch/BezierPatch.h"
#include "../Data/Ponto.h"
#include "../Data/Patch/HermitePatch.h"

extern std::string entrada;

class PatchBezierReader
{
    std::list<BezierPatch*> patches;
    std::list<HermitePatch*> patchesHermite;
    BezierPatch *patch;
    HermitePatch *patchHermite;

public:
    PatchBezierReader();
    std::list<BezierPatch *> parsePatchesBezier();
    std::list<BezierPatch *> LoaderRibFile();
    std::list<BezierPatch *> loaderBPFile(string filename);
    std::list<HermitePatch*> loaderBPFileHermite();
    std::list<BezierPatch *> loaderOBJFile();
	std::list<BezierPatch *> orderVectorToListBezierPatches(std::vector<double>);
    Ponto getPointVectorControlPoints(std::vector<Ponto>, unsigned long);
    
    
    ~PatchBezierReader();
};

#endif // PATCHBEZIERREADER_H
