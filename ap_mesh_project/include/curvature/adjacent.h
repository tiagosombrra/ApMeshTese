#ifndef CURVATURE_ADJACENT_H
#define CURVATURE_ADJACENT_H

#include <cmath>
#include <iostream>
#include <list>

#include "../data/Noh.h"
#include "../data/Triangulo.h"

// OBS: Essa classe foi implementada para triâgulos !!!
class Adjacent {
 public:
  bool ConfirmLeftAdjacency(const Noh& noh, const Elemento& first_element,
                            const Elemento& last_element);
  bool ConfirmRightAdjacency(const Noh& noh, const Elemento& first_element,
                             const Elemento& last_element);
  Elemento* GetElementLeft(const Noh& noh, Elemento* element,
                           list<Elemento*>& list_element);
  Elemento* GetElementRight(const Noh& noh, Elemento* element,
                            list<Elemento*>& list_element);
  int ConcavityElement(const Noh& noh, const Elemento& first_element,
                       const Elemento& next_element);
  double AngleElement(const Elemento& first_element,
                      const Elemento& next_element);
};
#endif  // CURVATURE_ADJACENT_H
