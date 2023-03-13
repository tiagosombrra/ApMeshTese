#ifndef DATA_TREE_BIN_TREE_H
#define DATA_TREE_BIN_TREE_H

#include <cstdint>
#include <list>
#include <memory>

#include "../curve/curve_adaptive_parametric.h"

class BinTree : public std::enable_shared_from_this<BinTree> {
 public:
  BinTree(double initial_param_coord = 0.0, double final_param_coord = 1.0,
          std::shared_ptr<BinTree> bin_tree = nullptr);
  ~BinTree() = default;
  std::uint64_t level_;         // nível d
  double initial_param_coord_;  // coordenada paramétrica do início da célula
  double final_param_coord_;    // coordenada paramétrica do final da célula

  std::shared_ptr<BinTree> bt_father_;
  std::shared_ptr<BinTree> bt_right_child_;
  std::shared_ptr<BinTree> bt_left_child_;
  std::shared_ptr<BinTree> bt_left_neighbor_;
  std::shared_ptr<BinTree> bt_right_neighbor_;

  bool IsLeaf();     // diz se uma célula é folha
  bool IsRoot();     // diz se uma célula é a raiz
  double GetSize();  // retorna o tamanho da célula
  bool Restrict(std::shared_ptr<CurveAdaptiveParametric> curve);
  void Subdivide(std::shared_ptr<CurveAdaptiveParametric> curve);
  // subdivide uma célula e define suas duas células filhas
  void Subdivide(double t, double t_par,
                 std::shared_ptr<CurveAdaptiveParametric> curve);
  // retorna uma célula que contém ti <= t <=tf
  std::shared_ptr<BinTree> Locate(double t);
  void Traverse(std::shared_ptr<BinTree> bin_tree, list<double>& coordinates);
  // retorna as coordenadas das folhas
  list<double> Rediscretization();
};

#endif  // DATA_TREE_BIN_TREE_H
