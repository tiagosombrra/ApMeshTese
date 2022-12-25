#ifndef DATA_CURVE_CURVE_ADAPTIVE_H
#define DATA_CURVE_CURVE_ADAPTIVE_H

#include <list>
#include <vector>

#include "../Noh.h"
#include "../Triangulo.h"
#include "../patch/Patch.h"

class CurveAdaptive {
 public:
  CurveAdaptive();
  explicit CurveAdaptive(CurveAdaptive*);
  virtual ~CurveAdaptive();

  void SetId(unsigned int id);
  double GetLength();
  void InsertPoint(Ponto* point);
  unsigned int GetNumBerPoints() const;
  // retorna o i-ésimo ponto
  Ponto* GetPoint(const unsigned int position);
  // troca a lista de pontos
  void SetPoints(list<Ponto*> new_points);
  list<Ponto*>& GetPoints();
  void InsertPatch(Patch* patch);
  unsigned int GetNumBerPatches() const;
  // retorna o i-ésimo patch
  Patch* GetPatch(const unsigned int position);
  // verifica se uma curva é de borda
  bool CheckIsOnBorder();

  // calcula o comprimento de curva de p1 a p2
  virtual double CalculateLengthPoints(const Ponto&, const Ponto&);
  // calcula o comprimento de curva até p
  virtual double CalculateLengthPoint(const Ponto&);
  // calcula o comprimento total "L" da curva
  virtual void CalculateLengthCurve();
  // calcula a curvatuta da curva
  virtual double CalculateCurvature(double);

 protected:
  // identificador da curva
  unsigned long id_;
  // comprimento total da curva
  double length_;
  // será usada na discretização
  list<Ponto*> points_;
  // uma curva só com um patch é de borda
  vector<Patch*> patches_;
};
#endif  // DATA_CURVE_CURVE_ADAPTIVE_H
