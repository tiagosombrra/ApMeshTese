PROGRAMA = ap_mesh

FONTES = src/data/Definitions.cpp \
src/Estimate/ChargeEstimateProcess.cpp \
src/input_output/PatchBezierReader.cpp \
src/input_output/WriteOBJFile.cpp \
src/parallel/ApMeshCommunicator.cpp \
src/parallel/Communicator.cpp \
src/parallel/MPICommunicator.cpp \
src/parallel/MPIMessage.cpp \
src/parallel/Message.cpp \
src/parallel/NoCommunicator.cpp \
src/parallel/NoThreadManager.cpp \
src/parallel/OMPThreadManager.cpp \
src/parallel/ParallelMeshGenerator.cpp \
src/adapter/AdaptadorPorCurvatura.cpp \
src/crabmesh/aft/AdvancingFront.cpp \
src/crabmesh/aft/Boundary.cpp \
src/crabmesh/aft/Quadtree.cpp \
src/crabmesh/aft/QuadtreeCell.cpp \
src/crabmesh/aft/Shape.cpp \
src/crabmesh/numerical/BisectionEquationRoot.cpp \
src/crabmesh/numerical/Function.cpp \
src/crabmesh/performer/IdManager.cpp \
src/crabmesh/performer/RangedIdManager.cpp \
src/curvature/Adjacente.cpp \
src/curvature/Curvatura.cpp \
src/curvature/CurvaturaAnalitica.cpp \
src/curvature/CurvaturaDiscreta.cpp \
src/data/curve/CurvParamBezier.cpp \
src/data/curve/CurvParamHermite.cpp \
src/data/curve/Curva.cpp \
src/data/curve/CurvaParametrica.cpp \
src/data/Edge.cpp \
src/data/Elemento.cpp \
src/data/Face.cpp \
src/data/Geometria.cpp \
src/data/mesh/Malha.cpp \
src/data/mesh/SubMalha.cpp \
src/data/Modelo.cpp \
src/data/Noh.cpp \
src/data/patch/BezierPatch.cpp \
src/data/patch/CoonsPatch.cpp \
src/data/patch/HermitePatch.cpp \
src/data/patch/Patch.cpp \
src/data/Ponto.cpp \
src/data/tree/BinTree.cpp \
src/data/Triangulo.cpp \
src/data/Vertex.cpp \
src/data/Vertice.cpp \
src/data/Vetor.cpp \
src/generator/Gerador.cpp \
src/generator/GeradorAdaptativo.cpp \
src/generator/GeradorAdaptativoPorCurvatura.cpp \
src/input_output/Arquivo.cpp \
src/input_output/Modelos3d.cpp \
src/input_output/ReaderPatches.cpp \
src/parallel/TMCommunicator.cpp \
src/parallel/ThreadManager.cpp \
src/parallel/Transferable.cpp \
src/timer/Timer.cpp \
src/main.cpp

OBJETOS = $(FONTES:.cpp=.o)

CC            = gcc
CXX           = g++
MPICC         = mpicc
MPICXX        = mpicxx

INCPATH = -Iinclude/adapter -Iinclude/crabmesh/aft -Iinclude/crabmesh/numerical -Iinclude/curvature -Iinclude/data -Iinclude/data/curve -Iinclude/data/mesh -Iinclude/data/patch -Iinclude/data/tree -Iinclude/estimate -Iinclude/generator -Iinclude/input_output -Iinclude/parallel -Iinclude/timer

LIBPATH = -Lsrc/adapter -Lsrc/crabmesh/aft -Lsrc/crabmesh/numerical -Lsrc/curvature -Lsrc/data -Lsrc/data/curve -Lsrc/data/mesh -Lsrc/data/patch -Lsrc/data/tree -Lsrc/estimate -Lsrc/generator -Lsrc/input_output -Lsrc/parallel -Lsrc/timer

CXXFLAGS_EXTRA = -O3 -W -Wall
CXXFLAGS      = -std=c++17 $(CXXFLAGS_EXTRA)
CXXFLAGS_OMP  = -fopenmp  $(CXXFLAGS) 

$(PROGRAMA): $(OBJETOS)
	@echo "gerando o executavel..."
	$(MPICXX) $(INCPATH) $(CXXFLAGS_OMP) $(LIBPATH) $(OBJETOS) -o $@

.cpp.o:
	@echo "gerando os .o..."
	$(MPICXX) $(INCPATH) -c $(CXXFLAGS_OMP) $< -o $@

clean:
	rm -rfv *.o src/*.o src/adapter/*.o src/crabmesh/aft/*.o  src/crabmesh/numerical/*.o src/crabmesh/performer/*.o src/curvature/*.o src/data/*.o src/data/curve/*.o src/data/mesh/*.o src/data/patch/*.o src/data/tree/*.o src/Estimate/*.o src/generator/*.o src/input_output/*.o src/parallel/*.o src/timer/*.o $(PROGRAMA)

exec: 
	./$(PROGRAMA)
accn