/* Classe que define uma curva paramétrica
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef CurvaParametrica_h
#define CurvaParametrica_h

#include <iostream>
#include <list>
using namespace std;

#include "Vertice.h"
#include "Matriz.h"
#include "Vetor.h"
#include "Curva.h"
#include "numerical/EquationRootFunction.h"
#include "numerical/BisectionEquationRoot.h"

class CurvaParametrica : public Curva
{ 
	protected :
		Ponto P0; // ponto inicial
		Ponto P1; // ponto final
		
		// matrizes geométricas
		Matriz* Gx;
		Matriz* Gy;
		Matriz* Gz; 
		
		Matriz* M; // matriz característica da curva
		Matriz* T; // matriz dos parâmetros
			
		// retorna o ponto calculado por T x M x Gx,y,z
		Ponto calculaPonto_t ( );
	
		double comprimento(double t1, double t2, int parts, int pontos);
		double pontoMedioBissecao(double t1, double t2);
	
	public :
		list < double > parametros; // parâmetros dos pontos
	
		Matriz& getGx (  ) const { return *(this->Gx); };
		Matriz& getGy (  ) const { return *(this->Gy); };
		Matriz& getGz (  ) const { return *(this->Gz); };
		Matriz& getM  (  ) const { return *(this->M ); };
		Matriz& getT  (  ) const { return *(this->T ); };
		
		Ponto getP0 (  ) const { return this->P0; };
		Ponto getP1 (  ) const { return this->P1; };
		
		void  setGx ( Matriz* G_x ){ this->Gx = G_x; };
		void  setGy ( Matriz* G_y ){ this->Gy = G_y; };
		void  setGz ( Matriz* G_z ){ this->Gz = G_z; };
		
		void  setM ( Matriz* M_ ){ this->M = M_; };
		void  setT ( Matriz* T_ ){ this->T = T_; };


		void setP0 ( const Ponto& p ) { this->P0 = p; };
		void setP1 ( const Ponto& p ) { this->P1 = p; calcular_L (  ); };
		void setPontos ( const Ponto& p0, const Ponto& p1 ) { this->P0 = p0; this->P1 = p1; };
						
		// calcula o comprimento de curva de p1 a p2
		virtual double calcularTamanho ( const Ponto& p1, const Ponto& p2 );
		// calcula o comprimento de curva até p
		virtual double calcularTamanho ( const Ponto& p );
		// calcula comprimento total "L" da curva
		virtual void calcular_L (  );
		// calcula a curvatuta da curva
		virtual double calcularCurvatura(double t);
		// calcula o comprimento de curva entre os parâmetros t1 e t2
		double comprimento ( double t1, double t2 );
		
		// encontra o parâmetro t de um dado ponto p na curva, ou do ponto na
		// curva mais próximo de p
		double encontrar_t ( const Ponto& p );
			
		// encontra o ponto p na curva dado um parâmetro t
		Ponto parametrizar ( double t );
		
		// calcula o gradiente no parâmetro t
		Vetor Qt ( double t );
		
		// retorna o ponto que fica na metade da curva
		Ponto pontoMedio ( const Ponto& p1, const Ponto& p2 );
		
		// retorna o ponto que fica na metade da curva
		double pontoMedio ( const double, const double );
		
		// ordena a lista de pontos de acordo com suas coordenadas paramétricas
		void ordenaLista (  );
		
		void atualizarParametros ( list < double > novaLista );
		//void atualizarLista ( );
		
		// IMPORTANTE: Ao usar o construtor default, não esquecer de setar
		// os pontos inicial e final e os vetores tangentes inicial e final !!!		
		CurvaParametrica (  );
		
		// Esse construtor é mais recomendado !!! Evita erros ao esquecer de setar
		// os valores indicados na observação do construtor default !!!
		CurvaParametrica ( Ponto p0, Ponto p1 );
		
		CurvaParametrica ( CurvaParametrica* );
		
		~CurvaParametrica (  );
};
#endif
