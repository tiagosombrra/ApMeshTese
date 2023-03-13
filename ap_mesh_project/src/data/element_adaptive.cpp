#include "../../include/data/element_adaptive.h"

#include <cstdint>

ElementAdaptive::ElementAdaptive() : id_(0), area_(0) {}

ElementAdaptive::~ElementAdaptive() {}

void ElementAdaptive::SetId(const std::uint64_t id) { this->id_ = id; }

std::uint64_t ElementAdaptive::GetId() { return this->id_; }

double ElementAdaptive::GetArea() const { return this->area_; }

VectorAdaptive ElementAdaptive::GetVectorNormal() const {
  return this->vector_normal_;
}
