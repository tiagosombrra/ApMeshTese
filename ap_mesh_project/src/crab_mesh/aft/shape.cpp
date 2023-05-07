#include "../../../include/crab_mesh/aft/shape.h"

Shape::Shape(long int id) { setId(id); }

Shape::~Shape() {}

void Shape::setId(long int id) { this->id = id; }

long int Shape::getId() { return id; }
