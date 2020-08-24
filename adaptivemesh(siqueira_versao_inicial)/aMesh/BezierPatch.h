/* Classe que define um patch de Bezier
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da dissertação de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef BezierPatch_h
#define BezierPatch_h

#include "CoonsPatch.h"

// As curvas devem ser definidas da esquerda para a direita, de baixo para
// cima em relação ao Patch !!!

class BezierPatch : public CoonsPatch
{
      // Pontos de controle ///////////////////////////////
      //
		Ponto Pt03;   Ponto Pt13;	Ponto Pt23;   Ponto Pt33;
		Ponto Pt02;   Ponto Pt12;	Ponto Pt22;   Ponto Pt32;
		Ponto Pt01;   Ponto Pt11;	Ponto Pt21;   Ponto Pt31;
		Ponto Pt00;   Ponto Pt10;	Ponto Pt20;   Ponto Pt30;
		//
		/////////////////////////////////////////////////////


		Matriz* U;  // matriz do parâmetro u
		Matriz* Gx; // Matriz geométrica (x)
		Matriz* Gy; // Matriz geométrica (y)
		Matriz* Gz; // Matriz geométrica (z)
		Matriz* V;  // matriz do parâmetro v
		//Matriz* B;  // matriz de Bezier
		static const Matriz* B;  // matriz de Bezier

		Ponto calculaPonto_u_v ( ); // faz a multiplicação das matrizes

	public :
		Matriz & getU  (  ) const { return *( this->U  ); };
		Matriz & getGx (  ) const { return *( this->Gx ); };
		Matriz & getGy (  ) const { return *( this->Gy ); };
		Matriz & getGz (  ) const { return *( this->Gz ); };
		Matriz & getV  (  ) const { return *( this->V  ); };
		const Matriz & getB  (  ) const { return *( this->B ); };

		Ponto getPt00 (  ) const { return this->Pt00; };
		Ponto getPt01 (  ) const { return this->Pt01; };
		Ponto getPt02 (  ) const { return this->Pt02; };
		Ponto getPt03 (  ) const { return this->Pt03; };

		Ponto getPt10 (  ) const { return this->Pt10; };
		Ponto getPt11 (  ) const { return this->Pt11; };
		Ponto getPt12 (  ) const { return this->Pt12; };
		Ponto getPt13 (  ) const { return this->Pt13; };

		Ponto getPt20 (  ) const { return this->Pt20; };
		Ponto getPt21 (  ) const { return this->Pt21; };
		Ponto getPt22 (  ) const { return this->Pt22; };
		Ponto getPt23 (  ) const { return this->Pt23; };

		Ponto getPt30 (  ) const { return this->Pt30; };
		Ponto getPt31 (  ) const { return this->Pt31; };
		Ponto getPt32 (  ) const { return this->Pt32; };
		Ponto getPt33 (  ) const { return this->Pt33; };

		void mostraMatrizes ( );

		// encontra as coordenadas parâmetricas u, v de um dado ponto p no patch,
		// ou a projeção desse ponto na superfície
		tuple < double, double > encontrar_u_v ( const Ponto& p );

		// encontra o ponto p na curva dado um parâmetro p
		virtual Ponto parametrizar ( double u, double v );

		// calcula as derivadas parciais nas direçõe u e v
		virtual Vetor Qu  ( double u, double v );
		virtual Vetor Qv  ( double u, double v );
		virtual Vetor Quu ( double u, double v );
		virtual Vetor Quv ( double u, double v );
		virtual Vetor Qvu ( double u, double v );
		virtual Vetor Qvv ( double u, double v );

		// calcula as derivadas parciais de um ponto p
		virtual Vetor Qu  ( const Ponto& p );
		virtual Vetor Qv  ( const Ponto& p );
		virtual Vetor Quu ( const Ponto& p );
		virtual Vetor Quv ( const Ponto& p );
		virtual Vetor Qvu ( const Ponto& p );
		virtual Vetor Qvv ( const Ponto& p );

		// Ordem das curvas:
		//		C3
		//	C4		C2
		//		C1
		BezierPatch ( Curva* C1, Curva* C2, Curva* C3, Curva* C4,
							Ponto Pt_11, Ponto Pt_21, Ponto Pt_12, Ponto Pt_22,
							bool sinal_c1 = true,
							bool sinal_c2 = true,
							bool sinal_c3 = true,
							bool sinal_c4 = true );

		BezierPatch (  Ponto Pt_00, Ponto Pt_01, Ponto Pt_02, Ponto Pt_03,
		               Ponto Pt_10, Ponto Pt_11, Ponto Pt_12, Ponto Pt_13,
		               Ponto Pt_20, Ponto Pt_21, Ponto Pt_22, Ponto Pt_23,
					   Ponto Pt_30, Ponto Pt_31, Ponto Pt_32, Ponto Pt_33,
							bool sinal_c1 = true,
							bool sinal_c2 = true,
							bool sinal_c3 = true,
							bool sinal_c4 = true );

		BezierPatch ( BezierPatch * );

		~BezierPatch ( );
};
#endif
