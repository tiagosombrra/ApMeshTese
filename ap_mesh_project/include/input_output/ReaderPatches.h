#ifndef READERPATCHES_H
#define READERPATCHES_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "../data/Modelo.h"
#include "../data/Ponto.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_hermite.h"

extern std::string INPUT_MODEL;

class ReaderPatches {
 public:
  std::list<PatchBezier *> patches;
  std::list<PatchHermite *> patchesHermite;
  PatchBezier *patch;
  PatchHermite *patchHermite;

  ReaderPatches();

  std::list<PatchBezier *> loaderBezierPatchFile(string fileName);
  Ponto getPointVectorControlPoints(std::vector<Ponto>, unsigned long);

  Geometry *readerPatches(Geometry *geo, string fileName);
};

#endif  // READERPATCHES_H
