//// Universidade Federal do Cear�
//// Gradua��o em Ci�ncias da Computa��o
//// M�todos Num�ricos I
//// Prof. Joaquim Bento Cavalcante
//// Alunos:	Leonardo Sampaio Rocha
////				Daniel M�rcio Batista de Siqueira
//// Defini��o da classe Matriz (dimens�es gen�ricas)
//// Setembro de 2004
//// Modificado em Dezembro de 2008
////				Ricardo Lenz C�sar

//#ifndef Matriz_h
//#define Matriz_h


//// comente a linha debaixo pra desativar o debug
//// das matrizes.
////
////#define DEBUG_MATRIZ

//class Matriz
//{
//    private:
//        const char* nome;

//    protected:

//        // m x n   =   linhas x colunas
//        int m;
//        int n;

//        double * A;

//    public:

//        void swap(double & x, double & y) const;
//        void copiaDe(const Matriz & fonte);

//        // m = linhas
//        int get_m() const;

//        // n = colunas
//        int get_n() const;


//        double getElem(int linha, int coluna) const;
//        void setElem(int linha, int coluna, double elem);


//        // operadores
//        //
//        friend Matriz operator* (const Matriz & A, const Matriz & B);
//        friend Matriz operator* (double x, const Matriz & B);
//        friend Matriz operator+ (const Matriz & A, const Matriz & B);
//        const Matriz & operator= (const Matriz & A);
//        //
//        //////////

//        Matriz identidade() const;

//        void trocaLinha(int linha1, int linha2);
//        void trocaColuna(int coluna1, int coluna2);

//        void somaLinha(int linha1, int linha2, double coef);
//        Matriz transposta() const;

//        Matriz(int linhas, int colunas, const char * nome = 0);
//        Matriz(const Matriz * M);
//        Matriz(const Matriz & M);
//        Matriz();

//        ~Matriz();
//};

//#endif
