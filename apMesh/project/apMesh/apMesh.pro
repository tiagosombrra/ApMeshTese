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
    ../../apMesh/AFT/AdvancingFront.cpp \
    ../../apMesh/AFT/Boundary.cpp \
    ../../apMesh/AFT/Quadtree.cpp \
    ../../apMesh/AFT/QuadtreeCell.cpp \
    ../../apMesh/Adaptador.cpp \
    ../../apMesh/AdaptadorPorCurvatura.cpp \
    ../../apMesh/Adjacente.cpp \
    ../../apMesh/Arquivo.cpp \
    ../../apMesh/Basics/Edge.cpp \
    ../../apMesh/Basics/Face.cpp \
    ../../apMesh/Basics/Shape.cpp \
    ../../apMesh/Basics/Vertex.cpp \
    ../../apMesh/BezierPatch.cpp \
    ../../apMesh/BinTree.cpp \
    ../../apMesh/CoonsPatch.cpp \
    ../../apMesh/CurvParamBezier.cpp \
    ../../apMesh/CurvParamHermite.cpp \
    ../../apMesh/Curva.cpp \
    ../../apMesh/CurvaParametrica.cpp \
    ../../apMesh/Curvatura.cpp \
    ../../apMesh/CurvaturaAnalitica.cpp \
    ../../apMesh/CurvaturaDiscreta.cpp \
    ../../apMesh/Elemento.cpp \
    ../../apMesh/Geometria.cpp \
    ../../apMesh/Gerador.cpp \
    ../../apMesh/GeradorAdaptativo.cpp \
    ../../apMesh/GeradorAdaptativoPorCurvatura.cpp \
    ../../apMesh/HermitePatch.cpp \
    ../../apMesh/Malha.cpp \
    ../../apMesh/Matriz.cpp \
    ../../apMesh/Modelo.cpp \
    ../../apMesh/Noh.cpp \
    ../../apMesh/Numerical/BisectionEquationRoot.cpp \
    ../../apMesh/Numerical/Function.cpp \
    ../../apMesh/Patch.cpp \
    ../../apMesh/Ponto.cpp \
    ../../apMesh/SubMalha.cpp \
    ../../apMesh/Triangulo.cpp \
    ../../apMesh/Vertice.cpp \
    ../../apMesh/Vetor.cpp \
    ../../apMesh/main.cpp

TRANSLATIONS += \
    amesh_pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ../../aMesh/aMesh.pro

HEADERS += \
    ../../apMesh/AFT/AdvancingFront.h \
    ../../apMesh/AFT/Boundary.h \
    ../../apMesh/AFT/Quadtree.h \
    ../../apMesh/AFT/QuadtreeCell.h \
    ../../apMesh/Adaptador.h \
    ../../apMesh/AdaptadorPorCurvatura.h \
    ../../apMesh/Adjacente.h \
    ../../apMesh/Arquivo.h \
    ../../apMesh/Basics/Definitions.h \
    ../../apMesh/Basics/Edge.h \
    ../../apMesh/Basics/Face.h \
    ../../apMesh/Basics/Shape.h \
    ../../apMesh/Basics/Vertex.h \
    ../../apMesh/BezierPatch.h \
    ../../apMesh/BinTree.h \
    ../../apMesh/CoonsPatch.h \
    ../../apMesh/CurvParamBezier.h \
    ../../apMesh/CurvParamHermite.h \
    ../../apMesh/Curva.h \
    ../../apMesh/CurvaParametrica.h \
    ../../apMesh/Curvatura.h \
    ../../apMesh/CurvaturaAnalitica.h \
    ../../apMesh/CurvaturaDiscreta.h \
    ../../apMesh/Definitions.h \
    ../../apMesh/Elemento.h \
    ../../apMesh/Geometria.h \
    ../../apMesh/Gerador.h \
    ../../apMesh/GeradorAdaptativo.h \
    ../../apMesh/GeradorAdaptativoPorCurvatura.h \
    ../../apMesh/HermitePatch.h \
    ../../apMesh/Malha.h \
    ../../apMesh/Matriz.h \
    ../../apMesh/Modelo.h \
    ../../apMesh/Noh.h \
    ../../apMesh/Numerical/BisectionEquationRoot.h \
    ../../apMesh/Numerical/EquationRoot.h \
    ../../apMesh/Numerical/EquationRootFunction.h \
    ../../apMesh/Numerical/Function.h \
    ../../apMesh/Numerical/MultiVariableFunction.h \
    ../../apMesh/Patch.h \
    ../../apMesh/Ponto.h \
    ../../apMesh/SubMalha.h \
    ../../apMesh/Triangulo.h \
    ../../apMesh/Vertice.h \
    ../../apMesh/Vetor.h
