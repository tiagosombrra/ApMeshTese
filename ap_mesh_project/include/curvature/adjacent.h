#ifndef CURVATURE_ADJACENT_H
#define CURVATURE_ADJACENT_H

#include <cmath>
#include <iostream>
#include <list>

#include "../data/noh.h"
#include "../data/triangle_adaptive.h"

// OBS: Essa classe foi implementada para triâgulos !!!
class Adjacent {
 public:
  bool ConfirmLeftAdjacency(const Noh& noh,
                            const ElementAdaptive& first_element,
                            const ElementAdaptive& last_element);
  bool ConfirmRightAdjacency(const Noh& noh,
                             const ElementAdaptive& first_element,
                             const ElementAdaptive& last_element);
  ElementAdaptive* GetElementLeft(const Noh& noh, ElementAdaptive* element,
                                  std::list<ElementAdaptive*>& list_element);
  ElementAdaptive* GetElementRight(const Noh& noh, ElementAdaptive* element,
                                   std::list<ElementAdaptive*>& list_element);
  int ConcavityElement(const Noh& noh, const ElementAdaptive& first_element,
                       const ElementAdaptive& next_element);
  double AngleElement(const ElementAdaptive& first_element,
                      const ElementAdaptive& next_element);
};
#endif  // CURVATURE_ADJACENT_H
