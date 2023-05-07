#ifndef DATA_CURVE_CURVE_ADAPTIVE_H
#define DATA_CURVE_CURVE_ADAPTIVE_H

#include <list>
#include <memory>
#include <vector>

#include "../node_adaptive.h"
#include "../patch/patch.h"
#include "../triangle_adaptive.h"

class CurveAdaptive {
 public:
  CurveAdaptive();
  explicit CurveAdaptive(std::shared_ptr<CurveAdaptive> curve_adaptive);
  virtual ~CurveAdaptive();

  void SetId(unsigned int id);
  double GetLength();
  void InsertPoint(std::shared_ptr<PointAdaptive> point);
  unsigned int GetNumBerPoints() const;
  // retorna o i-ésimo ponto
  std::shared_ptr<PointAdaptive> GetPoint(const unsigned int position);
  // troca a lista de pontos
  void SetPoints(std::list<std::shared_ptr<PointAdaptive>> new_points);
  std::list<std::shared_ptr<PointAdaptive>> GetPoints();
  void InsertPatch(const std::shared_ptr<Patch>& patch);
  unsigned int GetNumBerPatches() const;
  // retorna o i-ésimo patch
  std::shared_ptr<Patch> GetPatch(const unsigned int position);
  // verifica se uma curva é de borda
  bool CheckIsOnBorder();

  // calcula o comprimento de curva de p1 a p2
  virtual double CalculateLengthPoints(const PointAdaptive&,
                                       const PointAdaptive&);
  // calcula o comprimento de curva até p
  virtual double CalculateLengthPoint(const PointAdaptive&);
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
  std::list<std::shared_ptr<PointAdaptive>> points_;
  // uma curva só com um patch é de borda
  std::vector<std::shared_ptr<Patch>> patches_;
};
#endif  // DATA_CURVE_CURVE_ADAPTIVE_H
