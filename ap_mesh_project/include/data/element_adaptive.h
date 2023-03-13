#ifndef DATA_ELEMENT_ADAPTIVE_H
#define DATA_ELEMENT_ADAPTIVE_H

#include <cstdint>

#include "vector_adaptive.h"

class NodeAdaptive;

class ElementAdaptive {
 public:
  ElementAdaptive();
  virtual ~ElementAdaptive();

  std::uint64_t GetId();
  void SetId(const std::uint64_t id);
  double GetArea() const;
  VectorAdaptive GetVectorNormal() const;

  virtual void CalculateArea() = 0;
  virtual void CalculateNormal() = 0;
  virtual NodeAdaptive GetNoh(const std::uint64_t) const = 0;
  virtual double GetAngle(const NodeAdaptive& n) = 0;

 protected:
  std::uint64_t id_;
  double area_;
  VectorAdaptive vector_normal_;
};

#endif  // DATA_ELEMENT_ADAPTIVE_H
