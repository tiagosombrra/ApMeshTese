CONFIG += c++11 console
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -O3 -fopenmp     #-g  -pg    -std=gnu++0x#-march=native #-m64 #-mavx #-O0 valgrind
QMAKE_LFLAGS += -O3 -fopenmp       #-g  -pg   -std=gnu++0x#-march=native #-m64 #-mavx

# MPI Settings
QMAKE_CXX = mpicxx
QMAKE_CXX_RELEASE = $$QMAKE_CXX
QMAKE_CXX_DEBUG = $$QMAKE_CXX
QMAKE_LINK = $$QMAKE_CXX
QMAKE_CC = mpicc

QMAKE_CFLAGS += $$system(mpicc --showme:compile)
QMAKE_LFLAGS += $$system(mpicxx --showme:link)
QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/adapter/adapter.cpp \
    ../src/crab_mesh/aft/advancing_front.cpp \
    ../src/crab_mesh/aft/boundary.cpp \
    ../src/crab_mesh/aft/quadtree.cpp \
    ../src/crab_mesh/aft/quadtree_cell.cpp \
    ../src/crab_mesh/aft/shape.cpp \
    ../src/crab_mesh/numerical/bisection_equation_root.cpp \
    ../src/crab_mesh/numerical/function.cpp \
    ../src/crab_mesh/performer/id_manager.cpp \
    ../src/crab_mesh/performer/ranged_id_manager.cpp \
    ../src/curvature/adjacent.cpp \
    ../src/curvature/curvature_analytical.cpp \
    ../src/curvature/curvature_discrete.cpp \
    ../src/data/curve/curve_adaptive.cpp \
    ../src/data/curve/curve_adaptive_parametric.cpp \
    ../src/data/curve/curve_adaptive_parametric_bezier.cpp \
    ../src/data/curve/curve_adaptive_parametric_hermite.cpp \
    ../src/data/Definitions.cpp \
    ../src/data/Edge.cpp \
    ../src/data/Elemento.cpp \
    ../src/data/Face.cpp \
    ../src/data/Geometria.cpp \
    ../src/data/mesh/mesh_adaptive.cpp \
    ../src/data/mesh/sub_mesh.cpp \
    ../src/data/Modelo.cpp \
    ../src/data/Noh.cpp \
    ../src/data/patch/patch_bezier.cpp \
    ../src/data/patch/patch_coons.cpp \
    ../src/data/patch/patch_hermite.cpp \
    ../src/data/patch/patch.cpp \
    ../src/data/Ponto.cpp \
    ../src/data/tree/BinTree.cpp \
    ../src/data/Triangulo.cpp \
    ../src/data/Vertex.cpp \
    ../src/data/Vertice.cpp \
    ../src/data/Vetor.cpp \
    ../src/estimate/ChargeEstimateProcess.cpp \
    ../src/generator/Gerador.cpp \
    ../src/generator/GeradorAdaptativo.cpp \
    ../src/generator/GeradorAdaptativoPorCurvatura.cpp \
    ../src/input_output/Arquivo.cpp \
    ../src/input_output/Modelos3d.cpp \
    ../src/input_output/PatchBezierReader.cpp \
    ../src/input_output/ReaderPatches.cpp \
    ../src/input_output/WriteOBJFile.cpp \
    ../src/parallel/ApMeshCommunicator.cpp \
    ../src/parallel/Communicator.cpp \
    ../src/parallel/Message.cpp \
    ../src/parallel/MPICommunicator.cpp \
    ../src/parallel/MPIMessage.cpp \
    ../src/parallel/NoCommunicator.cpp \
    ../src/parallel/NoThreadManager.cpp \
    ../src/parallel/OMPThreadManager.cpp \
    ../src/parallel/ParallelMeshGenerator.cpp \
    ../src/parallel/ThreadManager.cpp \
    ../src/parallel/TMCommunicator.cpp \
    ../src/parallel/Transferable.cpp \
    ../src/timer/Timer.cpp \
    ../src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../include/adapter/adapter.h \
    ../include/crab_mesh/aft/advancing_front.h \
    ../include/crab_mesh/aft/boundary.h \
    ../include/crab_mesh/aft/quadtree.h \
    ../include/crab_mesh/aft/quadtree_cell.h \
    ../include/crab_mesh/aft/shape.h \
    ../include/crab_mesh/numerical/bisection_equation_root.h \
    ../include/crab_mesh/numerical/equation_root.h \
    ../include/crab_mesh/numerical/equation_root_function.h \
    ../include/crab_mesh/numerical/function.h \
    ../include/crab_mesh/numerical/multi_variable_function.h \
    ../include/crab_mesh/performer/definitions.h \
    ../include/crab_mesh/performer/id_manager.h \
    ../include/crab_mesh/performer/ranged_id_manager.h \
    ../include/curvature/adjacent.h \
    ../include/curvature/curvature.h \
    ../include/curvature/curvature_analytical.h \
    ../include/curvature/curvature_discrete.h \
    ../include/data/curve/curve_adaptive.h \
    ../include/data/curve/curve_adaptive_parametric.h \
    ../include/data/curve/curve_adaptive_parametric_bezier.h \
    ../include/data/curve/curve_adaptive_parametric_hermite.h \
    ../include/data/Definitions.h \
    ../include/data/Edge.h \
    ../include/data/Elemento.h \
    ../include/data/Face.h \
    ../include/data/Geometria.h \
    ../include/data/mesh/mesh_adaptive.h \
    ../include/data/mesh/sub_mesh.h \
    ../include/data/Modelo.h \
    ../include/data/Noh.h \
    ../include/data/patch/patch_bezier.h \
    ../include/data/patch/patch_coons.h \
    ../include/data/patch/patch_hermite.h \
    ../include/data/patch/patch.h \
    ../include/data/Ponto.h \
    ../include/data/Tree/BinTree.h \
    ../include/data/Triangulo.h \
    ../include/data/Vertex.h \
    ../include/data/Vertice.h \
    ../include/data/Vetor.h \
    ../include/estimate/ChargeEstimateProcess.h \
    ../include/generator/Gerador.h \
    ../include/generator/GeradorAdaptativo.h \
    ../include/generator/GeradorAdaptativoPorCurvatura.h \
    ../include/input_output/Arquivo.h \
    ../include/input_output/Modelos3d.h \
    ../include/input_output/PatchBezierReader.h \
    ../include/input_output/ReaderPatches.h \
    ../include/input_output/WriteOBJFIle.h \
    ../include/parallel/ApMeshCommunicator.h \
    ../include/parallel/Communicator.h \
    ../include/parallel/Definitions.h \
    ../include/parallel/Message.h \
    ../include/parallel/MPICommunicator.h \
    ../include/parallel/MPIMessage.h \
    ../include/parallel/NoCommunicator.h \
    ../include/parallel/NoThreadManager.h \
    ../include/parallel/OMPThreadManager.h \
    ../include/parallel/ParallelMeshGenerator.h \
    ../include/parallel/ThreadManager.h \
    ../include/parallel/TMCommunicator.h \
    ../include/parallel/Transferable.h \
    ../include/timer/Timer.h
