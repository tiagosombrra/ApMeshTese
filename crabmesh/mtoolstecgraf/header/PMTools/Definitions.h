#ifndef _PMTOOLS_DEFINITIONS_H_
#define _PMTOOLS_DEFINITIONS_H_

#include "MTools/Definitions.h"

#if USE_MTOOLS

#define USE_SORT_BY_ID_IN_CONVERSION FALSE

#include "PMJMesh/Definitions.h"

namespace PMTools
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	class MainDrive;

	class Communicator;
	class TreeGenerator;
	class MeshGenerator;
	class BackTrackingFrontAdvancer;

	class GlobalTaskManager;
	class WorkerMainDrive;
	class MToolsTask;

	class MshRegion3D;
	class AdvFront3D;
	class TetTopo;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)

#if DEBUG_DISABLE_IMPROVEMENT
	void setWorkerImprovementEnabled(bool enabled);
	void disableWorkerImprovement();
	void enableWorkerImprovement();

	bool isWorkerImprovementEnabled();
	bool isWorkerImprovementDisabled();

	void setMasterImprovementEnabled(bool enabled);
	void disableMasterImprovement();
	void enableMasterImprovement();

	bool isMasterImprovementEnabled();
	bool isMasterImprovementDisabled();
#endif //#if DEBUG_DISABLE_IMPROVEMENT
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_DEFINITIONS_H_
