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


//Verificação do compilador
#ifdef __APPLE__
#include "Eigen/Eigen"
#include "Eigen/Dense"
#elif __linux__
#include "Eigen/Eigen"
#include "Eigen/Dense"
#else
#   error "Unknown compiler"
#endif

#include "Definitions.h"
#include <iostream>
#include <list>
#include "Vertice.h"
#include "Matriz.h"
#include "Vetor.h"
#include "Curva.h"
#include "EquationRootFunction.h"
#include "BisectionEquationRoot.h"

using namespace std;

class CurvaParametrica : public Curva
{ 
	protected :
		Ponto P0; // ponto inicial
        Ponto P1; // ponto final
		
		// matrizes geométricas
        Matrix41d Gx;
        Matrix41d Gy;
        Matrix41d Gz;
		
        Matrix4d M; // matriz característica da curva
        Matrix14d T; // matriz dos parâmetros
			
        // retorna o ponto calculado por T x M x Gx,y,z
		Ponto calculaPonto_t ( );
	
		double comprimento(double t1, double t2, int parts, int pontos);
		double pontoMedioBissecao(double t1, double t2);
	
	public :
		list < double > parametros; // parâmetros dos pontos
	
        Matrix41d getGx (  ) const;
        Matrix41d getGy (  ) const;
        Matrix41d getGz (  ) const;
        Matrix4d getM  (  ) const;
        Matrix14d getT  (  ) const;
		
        Ponto getP0 (  ) const;
        Ponto getP1 (  ) const;
		
        void  setGx ( Matrix41d G_x );
        void  setGy ( Matrix41d G_y );
        void  setGz ( Matrix41d G_z );
		
        void  setM ( Matrix4d M_ );
        void  setT ( Matrix14d T_ );


        void setP0 ( const Ponto& p );
        void setP1 ( const Ponto& p );
        void setPontos ( const Ponto& p0, const Ponto& p1 );
						
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
