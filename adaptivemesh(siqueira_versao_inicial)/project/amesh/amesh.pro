QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ../../aMesh/AFT/AdvancingFront.cpp \
    ../../aMesh/AFT/Boundary.cpp \
    ../../aMesh/AFT/Quadtree.cpp \
    ../../aMesh/AFT/QuadtreeCell.cpp \
    ../../aMesh/Adaptador.cpp \
    ../../aMesh/AdaptadorPorCurvatura.cpp \
    ../../aMesh/Adjacente.cpp \
    ../../aMesh/Arquivo.cpp \
    ../../aMesh/Basics/Edge.cpp \
    ../../aMesh/Basics/Face.cpp \
    ../../aMesh/Basics/Shape.cpp \
    ../../aMesh/Basics/Vertex.cpp \
    ../../aMesh/BezierPatch.cpp \
    ../../aMesh/BinTree.cpp \
    ../../aMesh/CoonsPatch.cpp \
    ../../aMesh/CurvParamBezier.cpp \
    ../../aMesh/CurvParamHermite.cpp \
    ../../aMesh/Curva.cpp \
    ../../aMesh/CurvaParametrica.cpp \
    ../../aMesh/Curvatura.cpp \
    ../../aMesh/CurvaturaAnalitica.cpp \
    ../../aMesh/CurvaturaDiscreta.cpp \
    ../../aMesh/Elemento.cpp \
    ../../aMesh/Geometria.cpp \
    ../../aMesh/Gerador.cpp \
    ../../aMesh/GeradorAdaptativo.cpp \
    ../../aMesh/GeradorAdaptativoPorCurvatura.cpp \
    ../../aMesh/HermitePatch.cpp \
    ../../aMesh/Malha.cpp \
    ../../aMesh/Matriz.cpp \
    ../../aMesh/Modelo.cpp \
    ../../aMesh/Noh.cpp \
    ../../aMesh/Patch.cpp \
    ../../aMesh/Ponto.cpp \
    ../../aMesh/Renderizador.cpp \
    ../../aMesh/SubMalha.cpp \
    ../../aMesh/Triangulo.cpp \
    ../../aMesh/Vertice.cpp \
    ../../aMesh/Vetor.cpp \
    ../../aMesh/main.cpp \
    ../../aMesh/numerical/BisectionEquationRoot.cpp \
    ../../aMesh/numerical/Function.cpp

TRANSLATIONS += \
    amesh_pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ../../aMesh/aMesh.pro

HEADERS += \
    ../../aMesh/AFT/AdvancingFront.h \
    ../../aMesh/AFT/Boundary.h \
    ../../aMesh/AFT/Quadtree.h \
    ../../aMesh/AFT/QuadtreeCell.h \
    ../../aMesh/Adaptador.h \
    ../../aMesh/AdaptadorPorCurvatura.h \
    ../../aMesh/Adjacente.h \
    ../../aMesh/Arquivo.h \
    ../../aMesh/Basics/Definitions.h \
    ../../aMesh/Basics/Edge.h \
    ../../aMesh/Basics/Face.h \
    ../../aMesh/Basics/Shape.h \
    ../../aMesh/Basics/Vertex.h \
    ../../aMesh/BezierPatch.h \
    ../../aMesh/BinTree.h \
    ../../aMesh/CoonsPatch.h \
    ../../aMesh/CurvParamBezier.h \
    ../../aMesh/CurvParamHermite.h \
    ../../aMesh/Curva.h \
    ../../aMesh/CurvaParametrica.h \
    ../../aMesh/Curvatura.h \
    ../../aMesh/CurvaturaAnalitica.h \
    ../../aMesh/CurvaturaDiscreta.h \
    ../../aMesh/Definitions.h \
    ../../aMesh/Elemento.h \
    ../../aMesh/Geometria.h \
    ../../aMesh/Gerador.h \
    ../../aMesh/GeradorAdaptativo.h \
    ../../aMesh/GeradorAdaptativoPorCurvatura.h \
    ../../aMesh/HalfEdge.h \
    ../../aMesh/HermitePatch.h \
    ../../aMesh/Interface.h \
    ../../aMesh/Malha.h \
    ../../aMesh/Matriz.h \
    ../../aMesh/Modelo.h \
    ../../aMesh/Noh.h \
    ../../aMesh/Patch.h \
    ../../aMesh/Ponto.h \
    ../../aMesh/QuadTree.h \
    ../../aMesh/Renderizador.h \
    ../../aMesh/SubMalha.h \
    ../../aMesh/Triangulo.h \
    ../../aMesh/Vertice.h \
    ../../aMesh/Vetor.h \
    ../../aMesh/numerical/BisectionEquationRoot.h \
    ../../aMesh/numerical/EquationRoot.h \
    ../../aMesh/numerical/EquationRootFunction.h \
    ../../aMesh/numerical/Function.h \
    ../../aMesh/numerical/MultiVariableFunction.h
