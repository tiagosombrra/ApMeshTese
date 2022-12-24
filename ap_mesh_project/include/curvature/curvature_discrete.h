#ifndef CURVATURE_CURVATURE_DISCRETE_H
#define CURVATURE_CURVATURE_DISCRETE_H

#include <cmath>
#include <cstdlib>
#include <list>

#include "../data/Elemento.h"
#include "../data/Noh.h"
#include "adjacent.h"
#include "curvature.h"

class CurvatureDiscrete : public Curvature {
 public:
  CurvatureDiscrete(const Noh& n);
  double CalculateMeanCurvature() override;
  double CalculateGaussCurvature() override;

 private:
  void AdjacencySort(const Noh& noh);

  Noh noh_;
  // lista dos elementos de 'm' incidentes em 'n'
  list<Elemento*> list_elements_;
  // área dos elementos incidentes em 'n'
  double a_;
  // assume M_PI ou 2 * M_PI
  double factor_disc_;
  // soma dos ângulos em 'n'
  double sum_phi_;
  Adjacent adjacent_;
};

#endif  // CURVATURE_CURVATURE_DISCRETE_H
