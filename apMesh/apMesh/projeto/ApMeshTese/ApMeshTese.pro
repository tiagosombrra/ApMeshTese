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
    ../../Sources/Adapter/Adaptador.cpp \
    ../../Sources/Adapter/AdaptadorPorCurvatura.cpp \
    ../../Sources/Crabmesh/Aft/AdvancingFront.cpp \
    ../../Sources/Crabmesh/Aft/Boundary.cpp \
    ../../Sources/Crabmesh/Aft/Quadtree.cpp \
    ../../Sources/Crabmesh/Aft/QuadtreeCell.cpp \
    ../../Sources/Crabmesh/Aft/Shape.cpp \
    ../../Sources/Crabmesh/Numerical/BisectionEquationRoot.cpp \
    ../../Sources/Crabmesh/Numerical/Function.cpp \
    ../../Sources/Crabmesh/Performer/IdManager.cpp \
    ../../Sources/Crabmesh/Performer/RangedIdManager.cpp \
    ../../Sources/Curvature/Adjacente.cpp \
    ../../Sources/Curvature/Curvatura.cpp \
    ../../Sources/Curvature/CurvaturaAnalitica.cpp \
    ../../Sources/Curvature/CurvaturaDiscreta.cpp \
    ../../Sources/Data/Curve/CurvParamBezier.cpp \
    ../../Sources/Data/Curve/CurvParamHermite.cpp \
    ../../Sources/Data/Curve/Curva.cpp \
    ../../Sources/Data/Curve/CurvaParametrica.cpp \
    ../../Sources/Data/Definitions.cpp \
    ../../Sources/Data/Edge.cpp \
    ../../Sources/Data/Elemento.cpp \
    ../../Sources/Data/Face.cpp \
    ../../Sources/Data/Geometria.cpp \
    ../../Sources/Data/Mesh/Malha.cpp \
    ../../Sources/Data/Mesh/SubMalha.cpp \
    ../../Sources/Data/Modelo.cpp \
    ../../Sources/Data/Noh.cpp \
    ../../Sources/Data/Patch/BezierPatch.cpp \
    ../../Sources/Data/Patch/CoonsPatch.cpp \
    ../../Sources/Data/Patch/HermitePatch.cpp \
    ../../Sources/Data/Patch/Patch.cpp \
    ../../Sources/Data/Ponto.cpp \
    ../../Sources/Data/Tree/BinTree.cpp \
    ../../Sources/Data/Triangulo.cpp \
    ../../Sources/Data/Vertex.cpp \
    ../../Sources/Data/Vertice.cpp \
    ../../Sources/Data/Vetor.cpp \
    ../../Sources/Estimate/ChargeEstimateProcess.cpp \
    ../../Sources/Generator/Gerador.cpp \
    ../../Sources/Generator/GeradorAdaptativo.cpp \
    ../../Sources/Generator/GeradorAdaptativoPorCurvatura.cpp \
    ../../Sources/IO/Arquivo.cpp \
    ../../Sources/IO/Modelos3d.cpp \
    ../../Sources/IO/PatchBezierReader.cpp \
    ../../Sources/IO/ReaderPatches.cpp \
    ../../Sources/IO/WriteOBJFile.cpp \
    ../../Sources/Parallel/ApMeshCommunicator.cpp \
    ../../Sources/Parallel/Communicator.cpp \
    ../../Sources/Parallel/MPICommunicator.cpp \
    ../../Sources/Parallel/MPIMessage.cpp \
    ../../Sources/Parallel/Message.cpp \
    ../../Sources/Parallel/NoCommunicator.cpp \
    ../../Sources/Parallel/NoThreadManager.cpp \
    ../../Sources/Parallel/OMPThreadManager.cpp \
    ../../Sources/Parallel/ParallelMeshGenerator.cpp \
    ../../Sources/Parallel/TMCommunicator.cpp \
    ../../Sources/Parallel/ThreadManager.cpp \
    ../../Sources/Parallel/Transferable.cpp \
    ../../Sources/Timer/Timer.cpp \
    ../../Sources/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../Headers/Adapter/Adaptador.h \
    ../../Headers/Adapter/AdaptadorPorCurvatura.h \
    ../../Headers/Crabmesh/Aft/AdvancingFront.h \
    ../../Headers/Crabmesh/Aft/Boundary.h \
    ../../Headers/Crabmesh/Aft/Quadtree.h \
    ../../Headers/Crabmesh/Aft/QuadtreeCell.h \
    ../../Headers/Crabmesh/Aft/Shape.h \
    ../../Headers/Crabmesh/Numerical/BisectionEquationRoot.h \
    ../../Headers/Crabmesh/Numerical/EquationRoot.h \
    ../../Headers/Crabmesh/Numerical/EquationRootFunction.h \
    ../../Headers/Crabmesh/Numerical/Function.h \
    ../../Headers/Crabmesh/Numerical/MultiVariableFunction.h \
    ../../Headers/Crabmesh/Performer/Definitions.h \
    ../../Headers/Crabmesh/Performer/IdManager.h \
    ../../Headers/Crabmesh/Performer/RangedIdManager.h \
    ../../Headers/Curvature/Adjacente.h \
    ../../Headers/Curvature/Curvatura.h \
    ../../Headers/Curvature/CurvaturaAnalitica.h \
    ../../Headers/Curvature/CurvaturaDiscreta.h \
    ../../Headers/Data/Curve/CurvParamBezier.h \
    ../../Headers/Data/Curve/CurvParamHermite.h \
    ../../Headers/Data/Curve/Curva.h \
    ../../Headers/Data/Curve/CurvaParametrica.h \
    ../../Headers/Data/Definitions.h \
    ../../Headers/Data/Edge.h \
    ../../Headers/Data/Elemento.h \
    ../../Headers/Data/Face.h \
    ../../Headers/Data/Geometria.h \
    ../../Headers/Data/Mesh/Malha.h \
    ../../Headers/Data/Mesh/SubMalha.h \
    ../../Headers/Data/Modelo.h \
    ../../Headers/Data/Noh.h \
    ../../Headers/Data/Patch/BezierPatch.h \
    ../../Headers/Data/Patch/CoonsPatch.h \
    ../../Headers/Data/Patch/HermitePatch.h \
    ../../Headers/Data/Patch/Patch.h \
    ../../Headers/Data/Ponto.h \
    ../../Headers/Data/Tree/BinTree.h \
    ../../Headers/Data/Triangulo.h \
    ../../Headers/Data/Vertex.h \
    ../../Headers/Data/Vertice.h \
    ../../Headers/Data/Vetor.h \
    ../../Headers/Estimate/ChargeEstimateProcess.h \
    ../../Headers/Generator/Gerador.h \
    ../../Headers/Generator/GeradorAdaptativo.h \
    ../../Headers/Generator/GeradorAdaptativoPorCurvatura.h \
    ../../Headers/IO/Arquivo.h \
    ../../Headers/IO/Modelos3d.h \
    ../../Headers/IO/PatchBezierReader.h \
    ../../Headers/IO/ReaderPatches.h \
    ../../Headers/IO/WriteOBJFIle.h \
    ../../Headers/Parallel/ApMeshCommunicator.h \
    ../../Headers/Parallel/Communicator.h \
    ../../Headers/Parallel/Definitions.h \
    ../../Headers/Parallel/MPICommunicator.h \
    ../../Headers/Parallel/MPIMessage.h \
    ../../Headers/Parallel/Message.h \
    ../../Headers/Parallel/NoCommunicator.h \
    ../../Headers/Parallel/NoThreadManager.h \
    ../../Headers/Parallel/OMPThreadManager.h \
    ../../Headers/Parallel/ParallelMeshGenerator.h \
    ../../Headers/Parallel/TMCommunicator.h \
    ../../Headers/Parallel/ThreadManager.h \
    ../../Headers/Parallel/Transferable.h \
    ../../Headers/Timer/Timer.h
