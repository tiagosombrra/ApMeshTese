#ifndef READERPATCHES_H
#define READERPATCHES_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "../data/model.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_hermite.h"
#include "../data/point_adaptive.h"

extern std::string INPUT_MODEL;

class ReaderPatches {
 public:
  std::list<PatchBezier *> patches;
  std::list<PatchHermite *> patchesHermite;
  PatchBezier *patch;
  PatchHermite *patchHermite;

  ReaderPatches();

  std::list<PatchBezier *> loaderBezierPatchFile(string fileName);
  PointAdaptive getPointVectorControlPoints(std::vector<PointAdaptive>,
                                            unsigned long);

  Geometry *readerPatches(Geometry *geo, string fileName);
};

#endif  // READERPATCHES_H
