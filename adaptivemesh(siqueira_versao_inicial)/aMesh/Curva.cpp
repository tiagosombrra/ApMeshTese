/* Classe que define uma curva
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da tese de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "Curva.h"

void Curva::inserePonto ( Ponto* p )
{
	this->pontos.push_back ( p );
}

Ponto* Curva::getPonto ( const unsigned int i )
{
	list <Ponto*>::iterator it = this->pontos.begin();
	advance ( it, i );
	
	return *it;
}

void Curva::setPontos ( list < Ponto* > novaLista )
{
	this->pontos = novaLista;
}

void Curva::inserePatch ( Patch* p )
{
	this->patches.push_back ( p );
}

Patch* Curva::getPatch ( const unsigned int i )
{
	return (i < this->patches.size()) ? this->patches[i] : NULL;
}

// verifica se uma curva é de borda
bool Curva::estaNaBorda (  )
{
	if ( this->getNumDePatches() == 1 )
		return true;
	
	return false;
}


Curva::Curva( Curva * antiga )
{
	this->Id = antiga->Id;
	this->L = antiga->L;
	this->pontos = antiga->pontos;
}


Curva::~Curva (  )
{
	// 1. apaga a lista de pontos
	while ( !this->pontos.empty ( ) )
	{
		Ponto* P = this->pontos.back ( );
		this->pontos.pop_back ( );
		delete P;               
	}

	// 3. apaga a lista de pontos
	this->pontos.clear ( );
	
	// 4. apaga a lista de patches
	this->patches.clear ( );
}
