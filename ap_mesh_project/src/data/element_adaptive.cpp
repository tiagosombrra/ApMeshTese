#include "../../include/data/element_adaptive.h"

ElementAdaptive::ElementAdaptive() {}

ElementAdaptive::~ElementAdaptive() {}

void ElementAdaptive::SetId(unsigned long id) { this->id_ = id; }

unsigned long ElementAdaptive::GetId() { return this->id_; }

double ElementAdaptive::GetArea() const { return this->area_; }

Vetor ElementAdaptive::GetVectorNormal() const { return this->vector_normal_; }
