#ifndef INPUT_OUTPUT_PATCH_READER_H
#define INPUT_OUTPUT_PATCH_READER_H

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>

#include "../data/model.h"
#include "../data/patch/patch_bezier.h"
#include "../data/patch/patch_hermite.h"
#include "../data/point_adaptive.h"

class PatchReader {
 public:
  PatchReader();
  ~PatchReader();

  std::list<std::shared_ptr<PatchBezier>> ParsePatchesBezier();
  std::list<std::shared_ptr<PatchBezier>> LoaderRibFile();
  std::list<std::shared_ptr<PatchBezier>> LoaderBPFile(string filename);
  std::list<std::shared_ptr<PatchHermite>> LoaderBPFileHermite();
  std::list<std::shared_ptr<PatchBezier>> LoaderOBJFile();
  std::list<std::shared_ptr<PatchBezier>> OrderVectorToListBezierPatches(
      std::vector<double>);
  PointAdaptive GetPointVectorControlPoints(std::vector<PointAdaptive>,
                                            unsigned long);
  std::list<std::shared_ptr<PatchBezier>> LoaderBezierPatchFile(
      string fileName);
  std::shared_ptr<Geometry> ReaderFilePatches(
      std::shared_ptr<Geometry> &geometry, string fileName);

 protected:
  std::list<std::shared_ptr<PatchBezier>> patches_;
  std::list<std::shared_ptr<PatchHermite>> patches_hermite_;
  std::shared_ptr<PatchBezier> patch_;
  std::shared_ptr<PatchHermite> patch_hermite_;
};

#endif  // INPUT_OUTPUT_PATCH_READER_H
