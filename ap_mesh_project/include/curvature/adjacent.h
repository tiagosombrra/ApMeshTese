#ifndef CURVATURE_ADJACENT_H
#define CURVATURE_ADJACENT_H

#include <cmath>
#include <iostream>
#include <list>
#include <memory>

#include "../data/node_adaptive.h"
#include "../data/triangle_adaptive.h"

// OBS: Essa classe foi implementada para triâgulos !!!
class Adjacent {
 public:
  bool ConfirmLeftAdjacency(const NodeAdaptive& noh,
                            const ElementAdaptive& first_element,
                            const ElementAdaptive& last_element);
  bool ConfirmRightAdjacency(const NodeAdaptive& noh,
                             const ElementAdaptive& first_element,
                             const ElementAdaptive& last_element);
  std::shared_ptr<ElementAdaptive> GetElementLeft(
      const NodeAdaptive& noh, std::shared_ptr<ElementAdaptive> element,
      std::list<std::shared_ptr<ElementAdaptive>>& list_element);
  std::shared_ptr<ElementAdaptive> GetElementRight(
      const NodeAdaptive& noh, std::shared_ptr<ElementAdaptive> element,
      std::list<std::shared_ptr<ElementAdaptive>>& list_element);
  int ConcavityElement(const NodeAdaptive& noh,
                       const ElementAdaptive& first_element,
                       const ElementAdaptive& next_element);
  double AngleElement(const ElementAdaptive& first_element,
                      const ElementAdaptive& next_element);
};
#endif  // CURVATURE_ADJACENT_H
