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

#include "Noh.h"

void Noh::insereElemento ( Elemento* elem )
{
	this->guarda_chuva.push_back ( elem );
}

Elemento* Noh::getElemento ( const unsigned int i )
{
	list <Elemento*>::iterator it = this->guarda_chuva.begin();
	advance ( it, i);

	return *it;
}

Noh::~Noh (  )
{
	// 1. apaga a lista de elementos
	this->guarda_chuva.clear ( );
}
