
#ifndef DATA_NODE_ADAPTIVE_H
#define DATA_NODE_ADAPTIVE_H

#include <list>
#include <memory>

#include "element_adaptive.h"
#include "point_adaptive.h"

class NodeAdaptive : public PointAdaptive {
 public:
  NodeAdaptive();
  NodeAdaptive(double x, double y, double z);
  NodeAdaptive(double x, double y, double z, unsigned long position);
  explicit NodeAdaptive(const PointAdaptive &point);
  ~NodeAdaptive();

  void InsertElement(std::shared_ptr<ElementAdaptive> element);
  unsigned int GetNumberElements() const;
  std::shared_ptr<ElementAdaptive> GetElement(const unsigned int position);
  void ClearElements();

  const std::list<std::shared_ptr<ElementAdaptive>> &GetElements() const;
  void setElements(const std::list<std::shared_ptr<ElementAdaptive>> &elements);

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
  std::list<std::shared_ptr<ElementAdaptive>> elements_;

  double ga_;  // curvatura analítica gaussiana
  double gd_;  // curvatura discreta gaussiana
  double ha_;  // curvatura analítica média
  double hd_;  // curvatura discreta média
  // friend class CurvaturaDiscreta;
};

#endif  // DATA_NODE_ADAPTIVE_H
