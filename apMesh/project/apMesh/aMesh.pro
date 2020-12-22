#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T13:02:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = aMesh
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
LIBS += -fopenmp
QMAKE_CXXFLAGS += -fopenmp -pg -O3
QMAKE_LFLAGS += -fopenmp -pg -O3

SOURCES += main.cpp \
    AFT/AdvancingFront.cpp \
    AFT/Boundary.cpp \
    AFT/Quadtree.cpp \
    AFT/QuadtreeCell.cpp \
    Basics/Edge.cpp \
    Basics/Face.cpp \
    Basics/Shape.cpp \
    Basics/Vertex.cpp \
    Adaptador.cpp \
    AdaptadorPorCurvatura.cpp \
    Adjacente.cpp \
    Arquivo.cpp \
    BezierPatch.cpp \
    BinTree.cpp \
    CoonsPatch.cpp \
    Curva.cpp \
    CurvaParametrica.cpp \
    Curvatura.cpp \
    CurvaturaAnalitica.cpp \
    CurvaturaDiscreta.cpp \
    CurvParamBezier.cpp \
    CurvParamHermite.cpp \
    Elemento.cpp \
    Geometria.cpp \
    Gerador.cpp \
    GeradorAdaptativo.cpp \
    GeradorAdaptativoPorCurvatura.cpp \
    HermitePatch.cpp \
    Malha.cpp \
    Matriz.cpp \
    Modelo.cpp \
    Noh.cpp \
    Patch.cpp \
    Ponto.cpp \
    Renderizador.cpp \
    SubMalha.cpp \
    testeGeometria.cpp \
    Triangulo.cpp \
    Vertice.cpp \
    Vetor.cpp

HEADERS += \
    AFT/AdvancingFront.h \
    AFT/Boundary.h \
    AFT/Quadtree.h \
    AFT/QuadtreeCell.h \
    Basics/Definitions.h \
    Basics/Edge.h \
    Basics/Face.h \
    Basics/Shape.h \
    Basics/Vertex.h \
    Adaptador.h \
    AdaptadorPorCurvatura.h \
    Adjacente.h \
    Arquivo.h \
    BezierPatch.h \
    BinTree.h \
    CoonsPatch.h \
    Curva.h \
    CurvaParametrica.h \
    Curvatura.h \
    CurvaturaAnalitica.h \
    CurvaturaDiscreta.h \
    CurvParamBezier.h \
    CurvParamHermite.h \
    Elemento.h \
    Geometria.h \
    Gerador.h \
    GeradorAdaptativo.h \
    GeradorAdaptativoPorCurvatura.h \
    HalfEdge.h \
    HermitePatch.h \
    Interface.h \
    Malha.h \
    Matriz.h \
    Modelo.h \
    Noh.h \
    Patch.h \
    Ponto.h \
    QuadTree.h \
    Renderizador.h \
    SubMalha.h \
    Triangulo.h \
    Vertice.h \
    Vetor.h
