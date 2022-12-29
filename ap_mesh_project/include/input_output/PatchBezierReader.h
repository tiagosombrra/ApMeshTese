#ifndef PATCHBEZIERREADER_H
#define PATCHBEZIERREADER_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "../data/Ponto.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_hermite.h"
// #include "../../Libs/Boost/boost/filesystem.hpp"
// #include "../../Libs/Boost/boost/filesystem/fstream.hpp"
// #include "../../Libs/Boost/boost/archive/text_iarchive.hpp"

extern std::string INPUT_MODEL;
extern int RANK_MPI;

class PatchBezierReader {
  std::list<PatchBezier *> patches;
  std::list<PatchHermite *> patchesHermite;
  PatchBezier *patch;
  PatchHermite *patchHermite;

 public:
  PatchBezierReader();
  std::list<PatchBezier *> parsePatchesBezier();
  std::list<PatchBezier *> LoaderRibFile();
  std::list<PatchBezier *> loaderBPFile(string filename);
  std::list<PatchHermite *> loaderBPFileHermite();
  std::list<PatchBezier *> loaderOBJFile();
  std::list<PatchBezier *> orderVectorToListBezierPatches(std::vector<double>);
  Ponto getPointVectorControlPoints(std::vector<Ponto>, unsigned long);

  ~PatchBezierReader();
};

#endif  // PATCHBEZIERREADER_H
