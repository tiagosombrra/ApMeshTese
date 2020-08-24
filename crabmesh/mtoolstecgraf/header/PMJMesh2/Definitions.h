#ifndef _PMJMESH2_DEFINITIONS_H_
#define _PMJMESH2_DEFINITIONS_H_

#include "PMJMesh/Definitions.h"

#define DEBUG_PMJMESH2_PROFILE ((USE_TIMING) && (USE_TIME_MEASUREMENTS) && (TRUE))
#define DEBUG_CELL_COUNTER TRUE
#define USE_REMOVED_LAYERS_PROPORTIONS FALSE
#define USE_FILE_BASED_BSP FALSE

namespace PMJMesh2
{
    using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	class PartitionBSPTree;
	class PartitionBSPTreeCell;

	class InputData;

	class Communicator;

    class MainDrive;

    class PartitionBSPTreeGenerator;
    class GeometryBSPTreeGenerator;
    class LoadBSPTreeGenerator;
#if USE_FILE_BASED_BSP
    class FileBSPTreeGenerator;
#endif //#if USE_FILE_BASED_BSP

    class FrontPartitioner;
    class APrioriFrontPartitioner;
    class APrioriSupportMeshLaplacianSmoother;
    class ParallelMeshGenerator;
#if USE_TECGRAF_MSH_SURF
    class APrioriParallelMeshGenerator;
#endif //#if USE_TECGRAF_MSH_SURF
    //class MeshGenerator;
    class AuxiliaryFrontGenerator;

    class CrossingReplacer;
    class APrioriCrossingReplacer;
    class Task;

    class WorkerMainDrive;
    class MJMeshTask;

    enum PartitionerType
    {
        GEOMETRY_BASED_BSP = 1,
        LOAD_BASED_BSP
#if USE_FILE_BASED_BSP
        , FILE_BASED_BSP
#endif //#if USE_FILE_BASED_BSP
    };

    enum InterfaceMeshGenerationType
    {
        A_POSTERIORI = 1,
#if USE_TECGRAF_MSH_SURF
        A_PRIORI,
        A_PRIORI_INTERNAL //Not implemented yet
#endif //#if USE_TECGRAF_MSH_SURF
    };

    enum MeshUpdateType
    {
        FULL = 1,
        PARTIAL,
        EMPTY //Not implemented yet
    };

    enum FinalMeshState
    {
        JOINED = 1,
        DISTRIBUTED,
        REDISTRIBUTED //Not implemented yet
    };

    /*enum //transferable objects
	{
		TRANSFERABLE_MESSAGE = 0,
		TRANSFERABLE_INPUT_DATA = 1,
		TRANSFERABLE_BOUNDARY,
		TRANSFERABLE_TREE,
		TRANSFERABLE_MESH,
		TRANSFERABLE_PARTITION,
		TRANSFERABLE_MJMESH_TASK
	};*/

#if DEBUG_CELL_COUNTER
    class CellCounter;
#endif //#if DEBUG_CELL_COUNTER

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
}

#endif //#ifndef _PMJMESH2_DEFINITIONS_H_
