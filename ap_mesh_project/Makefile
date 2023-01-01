PROGRAMA = ap_mesh

FONTES = src/adapter/adapter.cpp \
src/crab_mesh/aft/advancing_front.cpp \
src/crab_mesh/aft/boundary.cpp \
src/crab_mesh/aft/edge.cpp \
src/crab_mesh/aft/face.cpp \
src/crab_mesh/aft/quadtree_cell.cpp \
src/crab_mesh/aft/quadtree.cpp \
src/crab_mesh/aft/shape.cpp \
src/crab_mesh/aft/vertex.cpp \
src/crab_mesh/numerical/bisection_equation_root.cpp \
src/crab_mesh/numerical/function.cpp \
src/crab_mesh/performer/id_manager.cpp \
src/crab_mesh/performer/ranged_id_manager.cpp \
src/curvature/adjacent.cpp \
src/curvature/curvature_analytical.cpp \
src/curvature/curvature_discrete.cpp \
src/data/curve/curve_adaptive_parametric_bezier.cpp \
src/data/curve/curve_adaptive_parametric_hermite.cpp \
src/data/curve/curve_adaptive_parametric.cpp \
src/data/curve/curve_adaptive.cpp \
src/data/definitions.cpp \
src/data/element_adaptive.cpp \
src/data/geometry.cpp \
src/data/mesh/mesh_adaptive.cpp \
src/data/mesh/sub_mesh.cpp \
src/data/model.cpp \
src/data/node_adaptive.cpp \
src/data/patch/patch_bezier.cpp \
src/data/patch/patch_coons.cpp \
src/data/patch/patch_hermite.cpp \
src/data/patch/patch.cpp \
src/data/point_adaptive.cpp \
src/data/tree/bin_tree.cpp \
src/data/triangle_adaptive.cpp \
src/data/vector_adaptive.cpp \
src/data/vertex_adaptive.cpp \
src/estimate/charge_estimate_process.cpp \
src/generator/generator_adaptive.cpp \
src/input_output/file.cpp \
src/input_output/models_3d.cpp \
src/input_output/patch_reader.cpp \
src/input_output/write_obj_file.cpp \
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
src/timer/timer.cpp \
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

SUBDIR_ROOTS := src
DIRS := . $(shell find $(SUBDIR_ROOTS) -type d)
GARBAGE_PATTERNS := *.o
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

$(PROGRAMA): $(OBJETOS)
	@echo "gerando o executavel..."
	$(MPICXX) $(INCPATH) $(CXXFLAGS_OMP) $(LIBPATH) $(OBJETOS) -o $@

.cpp.o:
	@echo "gerando os .o..."
	$(MPICXX) $(INCPATH) -c $(CXXFLAGS_OMP) $< -o $@

clean:
	rm -rfv $(GARBAGE) $(PROGRAMA)

exec: 
	./$(PROGRAMA)