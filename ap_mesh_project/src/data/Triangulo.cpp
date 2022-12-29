/* Classe que define um triangulo
 */
#include "../../include/data/Triangulo.h"

extern double TOLERANCIA;

Triangulo::Triangulo(Noh* v1, Noh* v2, Noh* v3) {
  this->n1 = v1;
  this->n2 = v2;
  this->n3 = v3;

  this->n1->insereElemento(this);
  this->n2->insereElemento(this);
  this->n3->insereElemento(this);

  this->angN1 = setAnguloN1();
  this->angN2 = setAnguloN2();
  this->angN3 = setAnguloN3();

  CalculateNormal();
  CalculateArea();
}

Triangulo::~Triangulo() {}

// retorna o i-ésimo nó
Noh Triangulo::GetNoh(unsigned const int i) const {
  if (i == 1)
    return *(this->n1);
  else if (i == 2)
    return *(this->n2);
  else if (i == 3)
    return *(this->n3);
  else
    cout << "Índice inválido para Noh Triangulo::GetNoh ( " << i << " )!"
         << endl;

  Noh* n = NULL;

  return *n;
}

// calcula o ângulo no nó 1
double Triangulo::setAnguloN1() {
  Vetor vet_1(*(this->n1), *(this->n2));
  Vetor vet_2(*(this->n1), *(this->n3));

  return vet_1.anguloCom(vet_2);
}

// calcula o ângulo no nó 2
double Triangulo::setAnguloN2() {
  Vetor vet_1(*(this->n2), *(this->n3));
  Vetor vet_2(*(this->n2), *(this->n1));

  return vet_1.anguloCom(vet_2);
}

// calcula o ângulo no nó 3
double Triangulo::setAnguloN3() {
  Vetor vet_1(*(this->n3), *(this->n1));
  Vetor vet_2(*(this->n3), *(this->n2));

  return vet_1.anguloCom(vet_2);
}

// retorna o ângulo do nó n
double Triangulo::GetAngle(const Noh& n) {
  if (this->n1->distanciaPara(n) <= TOLERANCIA) return this->angN1;

  if (this->n2->distanciaPara(n) <= TOLERANCIA) return this->angN2;

  if (this->n3->distanciaPara(n) <= TOLERANCIA) return this->angN3;

  return -1;  // erro
}

// calcula a área do triângulo
void Triangulo::CalculateArea() {
  this->area_ = 0.5 * (this->vector_normal_.modulo());
}

// calcula a normal do triângulo
void Triangulo::CalculateNormal() {
  Vetor vet_1(*(this->n1), *(this->n2));
  Vetor vet_2(*(this->n1), *(this->n3));

  this->vector_normal_ = vet_1 * vet_2;
}

void Triangulo::substituir(Noh* velho, Noh* novo) {
  if (this->n1 == velho)
    this->n1 = novo;
  else if (this->n2 == velho)
    this->n2 = novo;
  else if (this->n3 == velho)
    this->n3 = novo;
}

double Triangulo::quality() {
  double a = n1->distanciaPara(static_cast<Ponto>(*n2));
  double b = n2->distanciaPara(static_cast<Ponto>(*n3));
  double c = n3->distanciaPara(static_cast<Ponto>(*n1));

  // cout<<"a "<<a<<"b "<<b<<"c "<<c<<endl;

  double s = 0.5 * (a + b + c);
  double d = (s - a) * (s - b) * (s - c);

  double rInsc = std::sqrt(d / s);
  double rCirc = 0.25 * a * b * c / sqrt(s * d);

  return 2.0 * rInsc / rCirc;
}
