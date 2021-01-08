// Universidade Federal do Ceará
// Graduação em Ciências da Computação
// Métodos Numéricos I
// Prof. Joaquim Bento Cavalcante
// Alunos:	Leonardo Sampaio Rocha
//				Daniel Márcio Batista de Siqueira
// Definição da classe Matriz (dimensões genéricas)
// Setembro de 2004
// Modificado em Dezembro de 2008
//				Ricardo Lenz César

#include "Matriz.h"
#include <cstdlib>
#include <iostream>

using namespace std;



void Matriz :: swap( double & x, double & y ) const
{
	double aux;

	aux = x;
	x = y;
	y = aux;
}



// linhas
int Matriz :: get_m() const
{
	return m;
}

// colunas
int Matriz :: get_n() const
{
	return n;
}


double Matriz :: getElem(int linha, int coluna) const
{
	/*if ( linha < 0 || coluna < 0 || linha >= m || coluna >= n )
	{
		cout << "*** ERRO Matriz::getElem: linha " << linha
			<< ", coluna " << coluna << " invalidos!" << endl;

		return 0.0;
	}*/

	return A[m * coluna + linha];
}


void Matriz :: setElem(int linha, int coluna, double elem)
{
	/*if ( linha < 0 || coluna < 0 || linha >= m || coluna >= n )
	{
		cout << "*** ERRO Matriz::setElem: linha " << linha
			<< ", coluna " << coluna << " invalidos!" << endl;

		return;
	}*/

	A[m * coluna + linha] = elem;
}



void Matriz :: trocaLinha(int linha1, int linha2)
{
	if ( !(
		(0 <= linha1) && ( linha1 < get_m() ) &&
		(0 <= linha2) && ( linha2 < get_m() )
		)
	)
		return;

	for (int col = 0; col < get_n(); col++)
		swap(
			A[m * col + linha1],
			A[m * col + linha2]
		);
}


void Matriz :: trocaColuna(int coluna1, int coluna2)
{
	if ( !(
		(0 <= coluna1) && ( coluna1 < get_n() ) &&
		(0 <= coluna2) && ( coluna2 < get_n() )
		)
	)
		return;

	for (int lin = 0; lin < get_m(); lin++)
		swap(
			A[m * coluna1 + lin],
			A[m * coluna2 + lin]
		);
}


void Matriz :: somaLinha(int linha1, int linha2, double coef)
{
	for ( int col = 0; col < get_n(); col++ )
		setElem( linha1, col,
			getElem( linha2, col ) * coef
		);
}


Matriz operator* (const Matriz & A, const Matriz & B)
{
	if ( A.get_n() != B.get_m() )
		cout << "*** ERRO: Matriz * Matriz de tamanho invalido" << endl;

	Matriz C ( A.get_m(), B.get_n() );

	for (int i = 0; i < A.get_m(); i++)
		for (int j = 0; j < B.get_n(); j++)
			for (int k = 0; k < B.get_m(); k++)
			{
				C.setElem( i, j,
					C.getElem( i, j ) +
					A.getElem( i, k ) * B.getElem( k, j )
				);
			}

	return C;
}



Matriz operator* (double x, const Matriz & B)
{
	Matriz C ( B.get_m(), B.get_n() );

	for (int i = 0; i < B.get_m(); i++)
		for (int j = 0; j < B.get_n(); j++)
			C.setElem( i, j,
				x * B.getElem( i, j )
			);

	return C;
}


Matriz operator+ (const Matriz & A, const Matriz & B)
{

	if (
		A.get_m() != B.get_m() &&
		A.get_n() != B.get_n()
	)
		return A;

	Matriz C ( A.get_m(), B.get_n() );

	for (int i = 0; i < A.get_m(); i++)
		for (int j = 0; j < B.get_n(); j++)
			C.setElem( i, j,
				A.getElem( i , j ) + B.getElem( i ,j )
			);

	return C;
}


void Matriz :: copiaDe( const Matriz & fonte )
{
	this->m = fonte.m;
	this->n = fonte.n;
	this->nome = fonte.nome;

	if ( this->A ) delete A;

	this->A = (double *) malloc( m * n * sizeof( double ) );

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			setElem( i, j, fonte.getElem( i, j ) );
}


const Matriz &  Matriz :: operator= (const Matriz & A)
{
	copiaDe( A );
	return *this;
}


Matriz Matriz :: transposta() const
{
	Matriz transposta ( n, m );
	int i,j;

	for(i = 0; i < n; i++)
		for(j = 0; j < m; j++)
			transposta.setElem( i, j, this->getElem(j, i) );

	return transposta;
}


Matriz :: Matriz(int linhas, int colunas, const char * nome)
{
	this->m = linhas;
	this->n = colunas;
	this->A = (double *) malloc( linhas * colunas * sizeof( double ) );

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			setElem( i, j, 0.0 );


	////////

	this->nome = nome;

#ifdef DEBUG_MATRIZ

	if ( this->nome )
		cout << "Matriz " << nome << " criada ("
			<< linhas << " x " << colunas << ")" << endl;
	else
		cout << "Matriz criada ("
			<< linhas << " x " << colunas << ")" << endl;
#endif

}


Matriz :: Matriz()
{
	this->m = 0;
	this->n = 0;
	this->A = NULL;
	this->nome = NULL;

#ifdef DEBUG_MATRIZ
	cout << "Matriz vazia criada" << endl;
#endif

}


Matriz :: Matriz( const Matriz & M )
{
	this->A = NULL;

	copiaDe( M );
}


Matriz :: Matriz( const Matriz * M )
{
	this->A = NULL;

	copiaDe( *M );
}


Matriz Matriz :: identidade() const
{
	Matriz *I;

	I = new Matriz(m, m);

	for (int k = 0; k < m; k++)
		I->setElem( k, k, 1.0 );

	return *I;
}


Matriz :: ~Matriz()
{

#ifdef DEBUG_MATRIZ
	if ( this->nome )
		cout << "Destrutor Matriz " << nome << endl;
	else
		cout << "Destrutor Matriz" << endl;
#endif

	free ( A );
}
