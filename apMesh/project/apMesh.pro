QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += ../apMesh/Headers/Adapter ../apMesh/Headers/Basic/ ../apMesh/Headers/Crabmesh/Aft/  ../apMesh/Headers/Crabmesh/Numerical/ ../apMesh/Headers/Curvature/ ../apMesh/Headers/Data/Curve/ ../apMesh/Headers/Data/Mesh/  ../apMesh/Headers/Data/Patch/  ../apMesh/Headers/Data/Tree/ ../apMesh/Headers/Data/ ../apMesh/Headers/Estimate/  ../apMesh/Headers/Generator/ ../apMesh/Headers/IO/ ../apMesh/Headers/TypeMethod/  ../apMesh/Headers/Timer/

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    amesh_pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../apMesh/Headers/Adapter/Adaptador.h \
    ../apMesh/Headers/Adapter/AdaptadorPorCurvatura.h \
    ../apMesh/Headers/Basic/Definitions.h \
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
    ../apMesh/Headers/Curvature/Adjacente.h \
    ../apMesh/Headers/Curvature/Curvatura.h \
    ../apMesh/Headers/Curvature/CurvaturaAnalitica.h \
    ../apMesh/Headers/Curvature/CurvaturaDiscreta.h \
    ../apMesh/Headers/Data/Curve/CurvParamBezier.h \
    ../apMesh/Headers/Data/Curve/CurvParamHermite.h \
    ../apMesh/Headers/Data/Curve/Curva.h \
    ../apMesh/Headers/Data/Curve/CurvaParametrica.h \
    ../apMesh/Headers/Data/Edge.h \
    ../apMesh/Headers/Data/Elemento.h \
    ../apMesh/Headers/Data/Face.h \
    ../apMesh/Headers/Data/Geometria.h \
    ../apMesh/Headers/Data/Matriz.h \
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
    ../apMesh/Headers/Generator/Gerador.h \
    ../apMesh/Headers/Generator/GeradorAdaptativo.h \
    ../apMesh/Headers/Generator/GeradorAdaptativoPorCurvatura.h \
    ../apMesh/Headers/IO/Arquivo.h

SOURCES += \
    ../apMesh/Sources/Adapter/Adaptador.cpp \
    ../apMesh/Sources/Adapter/AdaptadorPorCurvatura.cpp \
    ../apMesh/Sources/Crabmesh/Aft/AdvancingFront.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Boundary.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Quadtree.cpp \
    ../apMesh/Sources/Crabmesh/Aft/QuadtreeCell.cpp \
    ../apMesh/Sources/Crabmesh/Aft/Shape.cpp \
    ../apMesh/Sources/Crabmesh/Numerical/BisectionEquationRoot.cpp \
    ../apMesh/Sources/Crabmesh/Numerical/Function.cpp \
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
    ../apMesh/Sources/Data/Matriz.cpp \
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
    ../apMesh/Sources/main.cpp
