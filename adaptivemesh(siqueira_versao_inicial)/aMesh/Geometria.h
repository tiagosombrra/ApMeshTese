/* Classe que define uma geometria
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#ifndef Geometria_h
#define Geometria_h

#include <vector>
#include <map>
using namespace std;

#include "Curva.h"
#include "HermitePatch.h"

class Geometria
{
		vector <Curva*> curvas;
		vector <Patch*> patches;

	public :
		void insereCurva ( Curva* c );
		unsigned int getNumDeCurvas (  ) const { return ( int ) curvas.size(); };
		Curva* getCurva ( const unsigned int i ); // retorna a i-ésima curva

		void inserePatch ( Patch* p );
		unsigned int getNumDePatches (  ) const { return ( int ) patches.size(); };
		Patch* getPatch ( const unsigned int i ); // retorna o i-ésimo patch

		Geometria ( ) {};
		Geometria ( Geometria* );
		~Geometria (  );
};
#endif
