/* Classe que define um elemento
 */
#include "../../include/data/Elemento.h"

void Elemento::setId(unsigned long i) { this->id = i; }

unsigned long Elemento::getId() { return this->id; }

double Elemento::getArea() const { return this->area; }

Vetor Elemento::getNormal() const { return this->normal; }

Elemento::~Elemento() {}
