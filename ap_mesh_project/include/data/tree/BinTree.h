/* Classe para leitura de arquivo
 */
#ifndef BinTree_h
#define BinTree_h

#include <iostream>
#include <list>
#include <tuple>

#include "../curve/curve_adaptive_parametric.h"

using namespace std;

struct BinTree {
  unsigned short nivel;  // nível d
  double ti;             // coordenada paramétrica do início da célula
  double tf;             // coordenada paramétrica do final da célula

  BinTree* pai;
  BinTree* filhoDir;
  BinTree* filhoEsq;
  BinTree* vizinhoEsq;
  BinTree* vizinhoDir;

  bool folha();      // diz se uma célula é folha
  bool raiz();       // diz se uma célula é a raiz
  double get_tam();  // retorna o tamanho da célula
  bool restringir(CurveAdaptiveParametric* curv);
  void subdividir(CurveAdaptiveParametric* curv);
  void subdividir(
      double t, double t_par,
      CurveAdaptiveParametric* curv);  // subdivide uma célula e define
                                       // suas duas células filhas
  BinTree* localiza(double t);  // retorna uma célula que contém ti <= t <=tf
  void percorre(BinTree* pt, list<double>& lista);
  list<double> rediscretizacao();  // retorna as coordenadas das folhas

  BinTree(double t_i = 0.0, double t_f = 1.0, BinTree* p = NULL);
  ~BinTree();
};
#endif
