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
include/crab_mesh/aft/edge.h
include/crab_mesh/aft/face.h
include/crab_mesh/aft/quadtree_cell.h
include/crab_mesh/aft/quadtree.h
include/crab_mesh/aft/shape.h
include/crab_mesh/aft/vertex.h
include/crab_mesh/numerical/bisection_equation_root.h
include/crab_mesh/numerical/function.h 
include/crab_mesh/numerical/multi_variable_function.h 
include/crab_mesh/performer/definitions.h 
include/crab_mesh/performer/id_manager.h
include/crab_mesh/performer/ranged_id_manager.h
include/curvature/adjacent.h
include/curvature/curvature_analytical.h
include/curvature/curvature_discrete.h
include/curvature/curvature.h
include/data/curve/curve_adaptive_parametric_bezier.h
include/data/curve/curve_adaptive_parametric_hermite.h
include/data/curve/curve_adaptive_parametric.h
include/data/curve/curve_adaptive.h
include/data/definitions.h
include/data/element_adaptive.h
include/data/geometry.h
include/data/mesh/mesh_adaptive.h
include/data/mesh/sub_mesh.h
include/data/model.h
include/data/node_adaptive.h
include/data/patch/patch_bezier.h
include/data/patch/patch_coons.h
include/data/patch/patch_hermite.h
include/data/patch/patch.h
include/data/point_adaptive.h
include/data/tree/bin_tree.h
include/data/triangle_adaptive.h
include/data/vector_adaptive.h
include/data/vertex_adaptive.h
include/estimate/charge_estimate_process.h
include/generator/generator_adaptive.h
include/input_output/file.h
include/input_output/models_3d.h
include/input_output/patch_reader.h
include/input_output/write_obj_file.h
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
include/timer/timer.h)

set(SOURCES
src/adapter/adapter.cpp
src/crab_mesh/aft/advancing_front.cpp
src/crab_mesh/aft/boundary.cpp
src/crab_mesh/aft/edge.cpp
src/crab_mesh/aft/face.cpp
src/crab_mesh/aft/quadtree_cell.cpp
src/crab_mesh/aft/quadtree.cpp
src/crab_mesh/aft/shape.cpp
src/crab_mesh/aft/vertex.cpp
src/crab_mesh/numerical/bisection_equation_root.cpp
src/crab_mesh/numerical/function.cpp
src/crab_mesh/performer/id_manager.cpp
src/crab_mesh/performer/ranged_id_manager.cpp
src/curvature/adjacent.cpp
src/curvature/curvature_analytical.cpp
src/curvature/curvature_discrete.cpp
src/data/curve/curve_adaptive_parametric_bezier.cpp
src/data/curve/curve_adaptive_parametric_hermite.cpp
src/data/curve/curve_adaptive_parametric.cpp
src/data/curve/curve_adaptive.cpp
src/data/definitions.cpp
src/data/element_adaptive.cpp
src/data/geometry.cpp
src/data/mesh/mesh_adaptive.cpp
src/data/mesh/sub_mesh.cpp
src/data/model.cpp
src/data/node_adaptive.cpp
src/data/patch/patch_bezier.cpp
src/data/patch/patch_coons.cpp
src/data/patch/patch_hermite.cpp
src/data/patch/patch.cpp
src/data/point_adaptive.cpp
src/data/tree/bin_tree.cpp
src/data/triangle_adaptive.cpp
src/data/vector_adaptive.cpp
src/data/vertex_adaptive.cpp
src/estimate/charge_estimate_process.cpp
src/generator/generator_adaptive.cpp
src/input_output/file.cpp
src/input_output/models_3d.cpp
src/input_output/patch_reader.cpp
src/input_output/write_obj_file.cpp
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
src/timer/timer.cpp
src/main.cpp)


include(CTest)
enable_testing()

add_executable(APMesh ${SOURCES} ${HEADERS})

target_link_libraries(APMesh PUBLIC MPI::MPI_CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
