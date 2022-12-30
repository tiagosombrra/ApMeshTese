#ifndef DATA_ELEMENT_ADAPTIVE_H
#define DATA_ELEMENT_ADAPTIVE_H

#include "Vetor.h"

class Noh;

class ElementAdaptive {
 public:
  ElementAdaptive();
  virtual ~ElementAdaptive();

  unsigned long GetId();
  void SetId(unsigned long id);
  double GetArea() const;
  Vetor GetVectorNormal() const;

  virtual void CalculateArea() = 0;
  virtual void CalculateNormal() = 0;
  virtual Noh GetNoh(unsigned const int) const = 0;
  virtual double GetAngle(const Noh& n) = 0;

 protected:
  unsigned long id_;
  double area_;
  Vetor vector_normal_;
};

#endif  // DATA_ELEMENT_ADAPTIVE_H
