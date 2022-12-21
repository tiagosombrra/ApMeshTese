#ifndef PATCHBEZIERREADER_H
#define PATCHBEZIERREADER_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "../data/patch/BezierPatch.h"
#include "../data/patch/HermitePatch.h"
#include "../data/Ponto.h"
//#include "../../Libs/Boost/boost/filesystem.hpp"
//#include "../../Libs/Boost/boost/filesystem/fstream.hpp"
//#include "../../Libs/Boost/boost/archive/text_iarchive.hpp"

extern std::string entrada;
extern int RANK_MPI;

class PatchBezierReader {
  std::list<BezierPatch *> patches;
  std::list<HermitePatch *> patchesHermite;
  BezierPatch *patch;
  HermitePatch *patchHermite;

 public:
  PatchBezierReader();
  std::list<BezierPatch *> parsePatchesBezier();
  std::list<BezierPatch *> LoaderRibFile();
  std::list<BezierPatch *> loaderBPFile(string filename);
  std::list<HermitePatch *> loaderBPFileHermite();
  std::list<BezierPatch *> loaderOBJFile();
  std::list<BezierPatch *> orderVectorToListBezierPatches(std::vector<double>);
  Ponto getPointVectorControlPoints(std::vector<Ponto>, unsigned long);

  ~PatchBezierReader();
};

#endif  // PATCHBEZIERREADER_H
