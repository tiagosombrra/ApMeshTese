PROGRAMA = teste

FONTES =	Modelo.cpp \
			Geometria.cpp \
			Curva.cpp \
			CurvaParametrica.cpp \
			CurvParamHermite.cpp \
			CurvParamBezier.cpp \
			Patch.cpp \
			CoonsPatch.cpp \
			HermitePatch.cpp \
			BezierPatch.cpp \
			Ponto.cpp \
			Vertice.cpp \
			Matriz.cpp \
			Vetor.cpp \
			Malha.cpp \
			SubMalha.cpp \
			Noh.cpp \
			Elemento.cpp \
			Triangulo.cpp \
			Curvatura.cpp \
			CurvaturaAnalitica.cpp \
			CurvaturaDiscreta.cpp \
			Adjacente.cpp \
			Gerador.cpp \
			GeradorAdaptativo.cpp \
			GeradorAdaptativoPorCurvatura.cpp \
			Adaptador.cpp \
			AdaptadorPorCurvatura.cpp \
			BinTree.cpp \
			AFT/AdvancingFront.cpp \
			AFT/Boundary.cpp \
			AFT/Quadtree.cpp \
			AFT/QuadtreeCell.cpp \
			Basics/Face.cpp \
			Basics/Edge.cpp \
			Basics/Vertex.cpp \
			Basics/Shape.cpp \
			main.cpp

OBJETOS = $(FONTES:.cpp=.o)

#CFLAGS = -c -g3 -Wall -lm -pipe -std=c++0x -pg -fopenmp
CFLAGS = -c -O3 -Wall -lm -pipe -std=c++0x -pg
LDFLAGS = -pg -O3

COMPILADOR = g++


all: $(FONTES) $(PROGRAMA)

$(PROGRAMA): $(OBJETOS)
	$(COMPILADOR) $(LDFLAGS) $(OBJETOS) -o $@

.cpp.o:
	$(COMPILADOR) $(CFLAGS) $< -o $@

clean:
	rm -rfv *.o AFT/*.o Basics/*.o $(PROGRAMA)

exec:
	./$(PROGRAMA)
