
#ifndef DATA_NOH_H
#define DATA_NOH_H

#include <list>

#include "element_adaptive.h"
#include "point_adaptive.h"

class Noh : public PointAdaptive {
 public:
  Noh();
  Noh(double x, double y, double z);
  Noh(double x, double y, double z, unsigned long position);
  explicit Noh(const PointAdaptive &point);
  ~Noh();

  void InsertElement(ElementAdaptive *element);
  unsigned int GetNumberElements() const;
  ElementAdaptive *GetElement(
      const unsigned int position);  // retorna o i-ésimo elemento
  void ClearElements();

  const std::list<ElementAdaptive *> &GetElements() const;
  void setElements(const std::list<ElementAdaptive *> &elements);

  double GetGa() const;
  void SetGa(double ga);

  double GetGd() const;
  void SetGd(double gd);

  double GetHa() const;
  void SetHa(double ha);

  double GetHd() const;
  void SetHd(double hd);

 protected:
  // lista de elementos no nó
  std::list<ElementAdaptive *> elements_;

  double ga_;  // curvatura analítica gaussiana
  double gd_;  // curvatura discreta gaussiana
  double ha_;  // curvatura analítica média
  double hd_;  // curvatura discreta média
  // friend class CurvaturaDiscreta;
};

#endif  // DATA_NOH_H
