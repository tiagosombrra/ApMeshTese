PROGRAMA = apMesh

FONTES = Sources/Adapter/Adaptador.cpp \
    Sources/Adapter/AdaptadorPorCurvatura.cpp \
    Sources/Crabmesh/Aft/AdvancingFront.cpp \
    Sources/Crabmesh/Aft/Boundary.cpp \
    Sources/Crabmesh/Aft/Quadtree.cpp \
    Sources/Crabmesh/Aft/QuadtreeCell.cpp \
    Sources/Crabmesh/Aft/Shape.cpp \
    Sources/Crabmesh/Numerical/BisectionEquationRoot.cpp \
    Sources/Crabmesh/Numerical/Function.cpp \
    Sources/Curvature/Adjacente.cpp \
    Sources/Curvature/Curvatura.cpp \
    Sources/Curvature/CurvaturaAnalitica.cpp \
    Sources/Curvature/CurvaturaDiscreta.cpp \
    Sources/Data/Curve/CurvParamBezier.cpp \
    Sources/Data/Curve/CurvParamHermite.cpp \
    Sources/Data/Curve/Curva.cpp \
    Sources/Data/Curve/CurvaParametrica.cpp \
    Sources/Data/Edge.cpp \
    Sources/Data/Elemento.cpp \
    Sources/Data/Face.cpp \
    Sources/Data/Geometria.cpp \
    Sources/Data/Mesh/Malha.cpp \
    Sources/Data/Mesh/SubMalha.cpp \
    Sources/Data/Modelo.cpp \
    Sources/Data/Noh.cpp \
    Sources/Data/Patch/BezierPatch.cpp \
    Sources/Data/Patch/CoonsPatch.cpp \
    Sources/Data/Patch/HermitePatch.cpp \
    Sources/Data/Patch/Patch.cpp \
    Sources/Data/Ponto.cpp \
    Sources/Data/Tree/BinTree.cpp \
    Sources/Data/Triangulo.cpp \
    Sources/Data/Vertex.cpp \
    Sources/Data/Vertice.cpp \
    Sources/Data/Vetor.cpp \
    Sources/Generator/Gerador.cpp \
    Sources/Generator/GeradorAdaptativo.cpp \
    Sources/Generator/GeradorAdaptativoPorCurvatura.cpp \
    Sources/IO/Arquivo.cpp \
    Sources/IO/Modelos3d.cpp \
    Sources/IO/ReaderPatches.cpp \
    Sources/Timer/Timer.cpp \
    Sources/main.cpp

OBJETOS = $(FONTES:.cpp=.o)

CC            = gcc
CXX           = g++

CFLAGS        = -O3 -fopenmp -g -std=gnu++11 -Wall -W #-D_REENTRANT -fPIC
CXXFLAGS      = -O3 -fopenmp -g -std=gnu++11 -Wall -W #-D_REENTRANT -fPIC #-pg
LFLAGS        = -O3 -fopenmp #-pg

#CFLAGS = -c -g3 -Wall -lm -pipe -std=c++0x -pg -fopenmp
#CFLAGS = -c -O3 -Wall -lm -pipe -std=c++0x -pg
#LDFLAGS = -pg -O3 -fopenmp

COMPILADOR = g++


all: $(FONTES) $(PROGRAMA)

$(PROGRAMA): $(OBJETOS)
	$(COMPILADOR) $(LFLAGS) $(OBJETOS) -o $@

.cpp.o:
	#$(COMPILADOR) $(CFLAGS) $< -o $@
	$(CXX) -c $(CXXFLAGS) $< -o $@
clean:
	rm -rfv *.o Sources/Adapter/*.o Sources/Crabmesh/Aft/*.o  Sources/Crabmesh/Numerical/*.o Sources/Curvature/*.o Sources/Data/*.o Sources/Data/Curve/*.o Sources/Data/Mesh/*.o Sources/Data/Patch/*.o Sources/Data/Tree/*.o Sources/Generator/*.o Sources/IO/*.o $(PROGRAMA)

exec:
	./$(PROGRAMA)
	
	
	
