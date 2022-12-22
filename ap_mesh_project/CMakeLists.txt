cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(APMesh VERSION 1.0)

if(UNIX)
    message("This is a Unix System")
elseif(WIN32)
    message("This is a Windows System")
endif()

set(CMAKE_CXX_STANDARD 17)
message("CXX Standard: ${CMAKE_CXX_STANDARD}")

set (CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -W -Wall")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package (MPI REQUIRED)

set(HEADERS
include/adapter/adapter.h
include/crab_mesh/aft/advancing_front.h
include/crab_mesh/aft/boundary.h
include/crab_mesh/aft/quadtree.h
include/crab_mesh/aft/quadtree_cell.h
include/crab_mesh/aft/shape.h
include/crab_mesh/numerical/bisection_equation_root.h
include/crab_mesh/numerical/function.h 
include/crab_mesh/numerical/multi_variable_function.h 
include/crab_mesh/performer/definitions.h 
include/crab_mesh/performer/id_manager.h
include/crab_mesh/performer/ranged_id_manager.h
include/curvature/adjacent.h
include/curvature/curvature.h
include/curvature/curvature_analytical.h
include/curvature/curvature_discrete.h
include/data/curve/Curva.h
include/data/curve/CurvaParametrica.h
include/data/curve/CurvParamBezier.h
include/data/curve/CurvParamHermite.h
include/data/Definitions.h
include/data/Edge.h
include/data/Elemento.h
include/data/Face.h
include/data/Geometria.h
include/data/mesh/Malha.h
include/data/mesh/SubMalha.h
include/data/Modelo.h
include/data/Noh.h
include/data/patch/BezierPatch.h
include/data/patch/CoonsPatch.h
include/data/patch/HermitePatch.h
include/data/patch/Patch.h
include/data/Ponto.h
include/data/tree/BinTree.h
include/data/Triangulo.h
include/data/Vertex.h
include/data/Vertice.h
include/data/Vetor.h
include/estimate/ChargeEstimateProcess.h
include/generator/Gerador.h
include/generator/GeradorAdaptativo.h
include/generator/GeradorAdaptativoPorCurvatura.h
include/input_output/Arquivo.h
include/input_output/Modelos3d.h
include/input_output/PatchBezierReader.h
include/input_output/ReaderPatches.h
include/input_output/WriteOBJFile.h
include/parallel/ApMeshCommunicator.h
include/parallel/Communicator.h
include/parallel/Message.h
include/parallel/MPICommunicator.h
include/parallel/MPIMessage.h
include/parallel/NoCommunicator.h
include/parallel/NoThreadManager.h
include/parallel/OMPThreadManager.h
include/parallel/ParallelMeshGenerator.h
include/parallel/ThreadManager.h
include/parallel/TMCommunicator.h
include/parallel/Transferable.h
include/timer/Timer.h)

set(SOURCES
src/adapter/adapter.cpp
src/crab_mesh/aft/advancing_front.cpp
src/crab_mesh/aft/boundary.cpp
src/crab_mesh/aft/quadtree.cpp
src/crab_mesh/aft/quadtree_cell.cpp
src/crab_mesh/aft/shape.cpp
src/crab_mesh/numerical/bisection_equation_root.cpp
src/crab_mesh/numerical/function.cpp
src/crab_mesh/performer/id_manager.cpp
src/crab_mesh/performer/ranged_id_manager.cpp
src/curvature/adjacent.cpp
src/curvature/curvature_analytical.cpp
src/curvature/curvature_discrete.cpp
src/data/curve/Curva.cpp
src/data/curve/CurvaParametrica.cpp
src/data/curve/CurvParamBezier.cpp
src/data/curve/CurvParamHermite.cpp
src/data/Definitions.cpp
src/data/Edge.cpp
src/data/Elemento.cpp
src/data/Face.cpp
src/data/Geometria.cpp
src/data/mesh/Malha.cpp
src/data/mesh/SubMalha.cpp
src/data/Modelo.cpp
src/data/Noh.cpp
src/data/patch/BezierPatch.cpp
src/data/patch/CoonsPatch.cpp
src/data/patch/HermitePatch.cpp
src/data/patch/Patch.cpp
src/data/Ponto.cpp
src/data/tree/BinTree.cpp
src/data/Triangulo.cpp
src/data/Vertex.cpp
src/data/Vertice.cpp
src/data/Vetor.cpp
src/estimate/ChargeEstimateProcess.cpp
src/generator/Gerador.cpp
src/generator/GeradorAdaptativo.cpp
src/generator/GeradorAdaptativoPorCurvatura.cpp
src/input_output/Arquivo.cpp
src/input_output/Modelos3d.cpp
src/input_output/PatchBezierReader.cpp
src/input_output/ReaderPatches.cpp
src/input_output/WriteOBJFile.cpp
src/parallel/ApMeshCommunicator.cpp
src/parallel/Communicator.cpp
src/parallel/Message.cpp
src/parallel/MPICommunicator.cpp
src/parallel/MPIMessage.cpp
src/parallel/NoCommunicator.cpp
src/parallel/NoThreadManager.cpp
src/parallel/OMPThreadManager.cpp
src/parallel/ParallelMeshGenerator.cpp
src/parallel/ThreadManager.cpp
src/parallel/TMCommunicator.cpp
src/parallel/Transferable.cpp
src/timer/Timer.cpp
src/main.cpp)


include(CTest)
enable_testing()

add_executable(APMesh ${SOURCES} ${HEADERS})

target_link_libraries(APMesh PUBLIC MPI::MPI_CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
