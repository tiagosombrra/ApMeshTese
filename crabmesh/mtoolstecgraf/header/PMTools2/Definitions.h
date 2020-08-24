#ifndef _PMTOOLS2_DEFINITIONS_H_
#define _PMTOOLS2_DEFINITIONS_H_

#include "PMJMesh2/Definitions.h"
#include "PMTools/Definitions.h"

#if USE_MTOOLS

#define USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION TRUE
#define DEBUG_PMTOOLS2_PROFILE_OCTREE TRUE

namespace PMTools2
{
    using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	class Communicator;

    class MainDrive;

    class ParallelMeshGenerator;
    class APrioriParallelMeshGenerator;
    class MeshGenerator;

    class WorkerMainDrive;
    class MToolsTask;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_DEFINITIONS_H_
