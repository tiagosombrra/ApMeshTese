CONFIG += c++11 console
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -O3 -fopenmp  #-g  -pg    -std=gnu++0x#-march=native #-m64 #-mavx #-O0 valgrind
QMAKE_LFLAGS += -O3 -fopenmp  #-g  -pg   -std=gnu++0x#-march=native #-m64 #-mavx

## MPI Settings
#QMAKE_CXX = mpicxx
#QMAKE_CXX_RELEASE = $$QMAKE_CXX
#QMAKE_CXX_DEBUG = $$QMAKE_CXX
#QMAKE_LINK = $$QMAKE_CXX
#QMAKE_CC = mpicc

#QMAKE_CFLAGS += $$system(mpicc --showme:compile)
#QMAKE_LFLAGS += $$system(mpicxx --showme:link)
#QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

DEFINES += QT_DEPRECATED_WARNINGS
TRANSLATIONS += \
    amesh_pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../apMesh/Headers/Adapter/Adaptador.h \
    ../apMesh/Headers/Adapter/AdaptadorPorCurvatura.h \
    ../apMesh/Headers/Crabmesh/Aft/AdvancingFront.h \
    ../apMesh/Headers/Crabmesh/Aft/Boundary.h \
    ../apMesh/Headers/Crabmesh/Aft/Quadtree.h \
    ../apMesh/Headers/Crabmesh/Aft/QuadtreeCell.h \
    ../apMesh/Headers/Crabmesh/Aft/Shape.h \
    ../apMesh/Headers/Crabmesh/Numerical/BisectionEquationRoot.h \
    ../apMesh/Headers/Crabmesh/Numerical/EquationRoot.h \
    ../apMesh/Headers/Crabmesh/Numerical/EquationRootFunction.h \
    ../apMesh/Headers/Crabmesh/Numerical/Function.h \
    ../apMesh/Headers/Crabmesh/Numerical/MultiVariableFunction.h \
    ../apMesh/Headers/Crabmesh/Performer/Definitions.h \
    ../apMesh/Headers/Crabmesh/Performer/IdManager.h \
    ../apMesh/Headers/Crabmesh/Performer/RangedIdManager.h \
    ../apMesh/Headers/Curvature/Adjacente.h \
    ../apMesh/Headers/Curvature/Curvatura.h \
    ../apMesh/Headers/Curvature/CurvaturaAnalitica.h \
    ../apMesh/Headers/Curvature/CurvaturaDiscreta.h \
    ../apMesh/Headers/Data/Curve/CurvParamBezier.h \
    ../apMesh/Headers/Data/Curve/CurvParamHermite.h \
    ../apMesh/Headers/Data/Curve/Curva.h \
    ../apMesh/Headers/Data/Curve/CurvaParametrica.h \
    ../apMesh/Headers/Data/Definitions.h \
    ../apMesh/Headers/Data/Edge.h \
    ../apMesh/Headers/Data/Elemento.h \
    ../apMesh/Headers/Data/Face.h \
    ../apMesh/Headers/Data/Geometria.h \
    ../apMesh/Headers/Data/Mesh/Malha.h \
    ../apMesh/Headers/Data/Mesh/SubMalha.h \
    ../apMesh/Headers/Data/Modelo.h \
    ../apMesh/Headers/Data/Noh.h \
    ../apMesh/Headers/Data/Patch/BezierPatch.h \
    ../apMesh/Headers/Data/Patch/CoonsPatch.h \
    ../apMesh/Headers/Data/Patch/HermitePatch.h \
    ../apMesh/Headers/Data/Patch/Patch.h \
    ../apMesh/Headers/Data/Ponto.h \
    ../apMesh/Headers/Data/Tree/BinTree.h \
    ../apMesh/Headers/Data/Triangulo.h \
    ../apMesh/Headers/Data/Vertex.h \
    ../apMesh/Headers/Data/Vertice.h \
    ../apMesh/Headers/Data/Vetor.h \
    ../apMesh/Headers/Estimate/ChargeEstimateProcess.h \
    ../apMesh/Headers/Execution/MpiRun.h \
    ../apMesh/Headers/Execution/OmpRun.h \
    ../apMesh/Headers/Execution/ParallelRun.h \
    ../apMesh/Headers/Execution/SequentialRun.h \
    ../apMesh/Headers/Generator/Gerador.h \
    ../apMesh/Headers/Generator/GeradorAdaptativo.h \
    ../apMesh/Headers/Generator/GeradorAdaptativoPorCurvatura.h \
    ../apMesh/Headers/IO/Arquivo.h \
    ../apMesh/Headers/IO/Modelos3d.h \
    ../apMesh/Headers/IO/PatchBezierReader.h \
    ../apMesh/Headers/IO/ReaderPatches.h \ \
    ../apMesh/Headers/IO/WriteOBJFIle.h \
    ../apMesh/Headers/Parallel/ApMeshCommunicator.h \
    ../apMesh/Headers/Parallel/Communicator.h \
    ../apMesh/Headers/Parallel/Definitions.h \
    ../apMesh/Headers/Parallel/MPICommunicator.h \
    ../apMesh/Headers/Parallel/MPIMessage.h \
    ../apMesh/Headers/Parallel/Message.h \
    ../apMesh/Headers/Parallel/NoCommunicator.h \
    ../apMesh/Headers/Parallel/NoThreadManager.h \
    ../apMesh/Headers/Parallel/OMPThreadManager.h \
    ../apMesh/Headers/Parallel/ParallelMeshGenerator.h \
    ../apMesh/Headers/Parallel/TMCommunicator.h \
    ../apMesh/Headers/Parallel/ThreadManager.h \
    ../apMesh/Headers/Parallel/Transferable.h \
    ../apMesh/Headers/Timer/Timer.h

SOURCES += \
    ../apMesh/Sources/Data/Definitions.cpp \
    ../apMesh/Sources/Estimate/ChargeEstimateProcess.cpp \
    ../apMesh/Sources/Execution/MpiRun.cpp \
    ../apMesh/Sources/Execution/OmpRun.cpp \
    ../apMesh/Sources/Execution/ParallelRun.cpp \
    ../apMesh/Sources/Execution/SequentialRun.cpp \
    ../apMesh/Sources/IO/PatchBezierReader.cpp \
    ../apMesh/Sources/IO/WriteOBJFile.cpp \
    ../apMesh/Sources/Parallel/ApMeshCommunicator.cpp \
    ../apMesh/Sources/Parallel/Communicator.cpp \
    ../apMesh/Sources/Parallel/MPICommunicator.cpp \
    ../apMesh/Sources/Parallel/MPIMessage.cpp \
    ../apMesh/Sources/Parallel/Message.cpp \
    ../apMesh/Sources/Parallel/NoCommunicator.cpp \
    ../apMesh/Sources/Parallel/NoThreadManager.cpp \
    ../apMesh/Sources/Parallel/OMPThreadManager.cpp \
    ../apMesh/Sources/Parallel/ParallelMeshGenerator.cpp \
    ../apMesh/Sources/Adapter/Adaptador.cpp \
    ../apMesh/Sources/Adapter/AdaptadorPorCurvatura.cpp \
    ../apMesh/Sources/Crabmesh/Aft/AdvancingFront.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Boundary.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Quadtree.cpp \
    ../apMesh/Sources/Crabmesh/Aft/QuadtreeCell.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Shape.cpp \
    ../apMesh/Sources/Crabmesh/Numerical/BisectionEquationRoot.cpp \
    ../apMesh/Sources/Crabmesh/Numerical/Function.cpp \
    ../apMesh/Sources/Crabmesh/Performer/IdManager.cpp \
    ../apMesh/Sources/Crabmesh/Performer/RangedIdManager.cpp \
    ../apMesh/Sources/Curvature/Adjacente.cpp \
    ../apMesh/Sources/Curvature/Curvatura.cpp \
    ../apMesh/Sources/Curvature/CurvaturaAnalitica.cpp \
    ../apMesh/Sources/Curvature/CurvaturaDiscreta.cpp \
    ../apMesh/Sources/Data/Curve/CurvParamBezier.cpp \
    ../apMesh/Sources/Data/Curve/CurvParamHermite.cpp \
    ../apMesh/Sources/Data/Curve/Curva.cpp \
    ../apMesh/Sources/Data/Curve/CurvaParametrica.cpp \
    ../apMesh/Sources/Data/Edge.cpp \
    ../apMesh/Sources/Data/Elemento.cpp \
    ../apMesh/Sources/Data/Face.cpp \
    ../apMesh/Sources/Data/Geometria.cpp \
    ../apMesh/Sources/Data/Mesh/Malha.cpp \
    ../apMesh/Sources/Data/Mesh/SubMalha.cpp \
    ../apMesh/Sources/Data/Modelo.cpp \
    ../apMesh/Sources/Data/Noh.cpp \
    ../apMesh/Sources/Data/Patch/BezierPatch.cpp \
    ../apMesh/Sources/Data/Patch/CoonsPatch.cpp \
    ../apMesh/Sources/Data/Patch/HermitePatch.cpp \
    ../apMesh/Sources/Data/Patch/Patch.cpp \
    ../apMesh/Sources/Data/Ponto.cpp \
    ../apMesh/Sources/Data/Tree/BinTree.cpp \
    ../apMesh/Sources/Data/Triangulo.cpp \
    ../apMesh/Sources/Data/Vertex.cpp \
    ../apMesh/Sources/Data/Vertice.cpp \
    ../apMesh/Sources/Data/Vetor.cpp \
    ../apMesh/Sources/Generator/Gerador.cpp \
    ../apMesh/Sources/Generator/GeradorAdaptativo.cpp \
    ../apMesh/Sources/Generator/GeradorAdaptativoPorCurvatura.cpp \
    ../apMesh/Sources/IO/Arquivo.cpp \
    ../apMesh/Sources/IO/Modelos3d.cpp \
    ../apMesh/Sources/IO/ReaderPatches.cpp \
    ../apMesh/Sources/Parallel/TMCommunicator.cpp \
    ../apMesh/Sources/Parallel/ThreadManager.cpp \
    ../apMesh/Sources/Parallel/Transferable.cpp \
    ../apMesh/Sources/Timer/Timer.cpp \
    ../apMesh/Sources/main.cpp \
