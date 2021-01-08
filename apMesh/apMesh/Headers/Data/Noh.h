/* Classe que define um nó
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef Noh_h
#define Noh_h

#include <iostream>
#include <list>
using namespace std;

#include "Ponto.h"
#include "Elemento.h"

struct Noh : public Ponto
{
		list <Elemento*> guarda_chuva; // lista de elementos no nó
		
		double Ga; // curvatura analítica gaussiana
		double Gd; // curvatura discreta gaussiana
		double Ha; // curvatura analítica média
		double Hd; // curvatura discreta média
	
		void insereElemento ( Elemento* elem );
		unsigned int getNumDeElementos (  ) const { return ( int ) guarda_chuva.size(); };
		Elemento* getElemento ( const unsigned int i ); // retorna o i-ésimo elemento
		void apagaGuardaChuva() { guarda_chuva.clear(); };
		
		Noh (  ) {  };
		Noh ( double _x, double _y, double _z ) : Ponto ( _x, _y, _z ) {};
		Noh ( double _x, double _y, double _z, unsigned long i ) : Ponto ( _x, _y, _z, i ) {}; 
		Noh ( const Ponto& p ) { this->id = p.id; this->x = p.x; this->y = p.y; this->z = p.z; };
		
		~Noh (  );
		
		friend class CurvaturaDiscreta;
};

#endif
