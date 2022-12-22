PROGRAMA = ap_mesh

FONTES = src/adapter/adapter.cpp \
src/crab_mesh/aft/advancing_front.cpp \
src/crab_mesh/aft/boundary.cpp \
src/crab_mesh/aft/quadtree.cpp \
src/crab_mesh/aft/quadtree_cell.cpp \
src/crab_mesh/aft/shape.cpp \
src/crab_mesh/numerical/bisection_equation_root.cpp \
src/crab_mesh/numerical/function.cpp \
src/crab_mesh/performer/id_manager.cpp \
src/crab_mesh/performer/ranged_id_manager.cpp \
src/curvature/Adjacente.cpp \
src/curvature/Curvatura.cpp \
src/curvature/CurvaturaAnalitica.cpp \
src/curvature/CurvaturaDiscreta.cpp \
src/data/curve/Curva.cpp \
src/data/curve/CurvaParametrica.cpp \
src/data/curve/CurvParamBezier.cpp \
src/data/curve/CurvParamHermite.cpp \
src/data/Definitions.cpp \
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
src/estimate/ChargeEstimateProcess.cpp \
src/generator/Gerador.cpp \
src/generator/GeradorAdaptativo.cpp \
src/generator/GeradorAdaptativoPorCurvatura.cpp \
src/input_output/Arquivo.cpp \
src/input_output/Modelos3d.cpp \
src/input_output/PatchBezierReader.cpp \
src/input_output/ReaderPatches.cpp \
src/input_output/WriteOBJFile.cpp \
src/parallel/ApMeshCommunicator.cpp \
src/parallel/Communicator.cpp \
src/parallel/Message.cpp \
src/parallel/MPICommunicator.cpp \
src/parallel/MPIMessage.cpp \
src/parallel/NoCommunicator.cpp \
src/parallel/NoThreadManager.cpp \
src/parallel/OMPThreadManager.cpp \
src/parallel/ParallelMeshGenerator.cpp \
src/parallel/ThreadManager.cpp \
src/parallel/TMCommunicator.cpp \
src/parallel/Transferable.cpp \
src/timer/Timer.cpp \
src/main.cpp

OBJETOS = $(FONTES:.cpp=.o)

CC            = gcc
CXX           = g++
MPICC         = mpicc
MPICXX        = mpicxx

INCPATH = -Iinclude/adapter -Iinclude/crab_mesh/aft -Iinclude/crab_mesh/numerical -Iinclude/curvature -Iinclude/data -Iinclude/data/curve -Iinclude/data/mesh -Iinclude/data/patch -Iinclude/data/tree -Iinclude/estimate -Iinclude/generator -Iinclude/input_output -Iinclude/parallel -Iinclude/timer

LIBPATH = -Lsrc/adapter -Lsrc/crab_mesh/aft -Lsrc/crab_mesh/numerical -Lsrc/curvature -Lsrc/data -Lsrc/data/curve -Lsrc/data/mesh -Lsrc/data/patch -Lsrc/data/tree -Lsrc/estimate -Lsrc/generator -Lsrc/input_output -Lsrc/parallel -Lsrc/timer

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
	rm -rfv *.o src/*.o src/adapter/*.o src/crab_mesh/aft/*.o  src/crab_mesh/numerical/*.o src/crab_mesh/performer/*.o src/curvature/*.o src/data/*.o src/data/curve/*.o src/data/mesh/*.o src/data/patch/*.o src/data/tree/*.o src/Estimate/*.o src/generator/*.o src/input_output/*.o src/parallel/*.o src/timer/*.o $(PROGRAMA)

exec: 
	./$(PROGRAMA)