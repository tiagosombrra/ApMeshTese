/* Classe para leitura de um ponto
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "Ponto.h"
using namespace std;


bool Ponto::operator== ( const Ponto& p )const
{
	if (	( fabs( this->x - p.x ) <= 0.000001 ) and
			( fabs( this->y - p.y ) <= 0.000001 ) and
			( fabs( this->z - p.z ) <= 0.000001 )
	) return true;
	
	return false;
}


double Ponto::distanciaPara ( const Ponto& p ) const
{ 
	return (
		sqrt(
			pow( ( p.x - this->x ), 2.0) +
			pow( ( p.y - this->y ), 2.0) +
			pow( ( p.z - this->z ), 2.0)
		)
	);
}
