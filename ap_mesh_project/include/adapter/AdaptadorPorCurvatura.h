/* Classe encarregada de fazer a adaptação das malhas baseando-se nas curvaturas
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef AdaptadorPorCurvatura_h
#define AdaptadorPorCurvatura_h

#include <cmath>
#include <cstdlib>
#include <map>

#include "../crab_mesh/aft/AdvancingFront.h"
#include "../crab_mesh/aft/Boundary.h"
#include "../crab_mesh/performer/RangedIdManager.h"
#include "../curvature/CurvaturaAnalitica.h"
#include "../curvature/CurvaturaDiscreta.h"
#include "../data/curve/CurvaParametrica.h"
#include "../data/Definitions.h"
#include "../data/Edge.h"
#include "../data/Face.h"
#include "../data/mesh/SubMalha.h"
#include "../data/patch/CoonsPatch.h"
#include "../data/tree/BinTree.h"
#include "../data/Vertex.h"
#include "../parallel/TMCommunicator.h"

class AdaptadorPorCurvatura {
#if USE_OPENMP
  static list<Ponto*> adaptaCurvaByCurvaOmp(Curva* c,
                                            Performer::IdManager* idManager,
                                            double fator_dis = 1.0);
  static list<Ponto*> adaptaCurvaBySuperficieOmp(
      Curva* c, Performer::IdManager* idManager, double fator_dis = 1.0);
  static SubMalha* adaptaDominioOmp(CoonsPatch* patch,
                                    Performer::IdManager* idManager,
                                    double fator_dis = 1.0);
#endif  //#USE_OPENMP

  static list<Ponto*> adaptaCurvaByCurva(Curva*, map<Ponto*, Ponto*>&,
                                         Performer::IdManager* idManager,
                                         double fator_dis = 1.0);
  static list<Ponto*> adaptaCurvaBySuperficie(Curva* c, map<Ponto*, Ponto*>&,
                                              Performer::IdManager* idManager,
                                              double fator_dis = 1.0);

  static tuple<double, double> curvatura(CurvaturaAnalitica& ka,
                                         CurvaturaDiscreta& kd);
  static double novoTamanho(const double, const double, const double,
                            const double);
  static void adaptaCurvas(CoonsPatch& patch, const unsigned int);
  static SubMalha* adaptaDominio(CoonsPatch* patch,
                                 Performer::IdManager* idManager,
                                 double fator_dis = 1.0);
  virtual void fazNada() = 0;

  friend class GeradorAdaptativoPorCurvatura;
};

#endif
