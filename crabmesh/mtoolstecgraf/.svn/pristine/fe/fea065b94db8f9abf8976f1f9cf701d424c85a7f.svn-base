#ifndef _MTOOLS_DEFINITIONS_H_
#define _MTOOLS_DEFINITIONS_H_

//#ifndef USE_MTOOLS
//#define USE_MTOOLS TRUE
//#endif //#ifndef USE_MTOOLS

#include "MJMesh/Definitions.h"

#if USE_MTOOLS

#define DEBUG_DISABLE_IMPROVEMENT FALSE

namespace MTools
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

	class TreeGenerator;

	class MshRegion3D;
	class AdvFront3D;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)

#if DEBUG_DISABLE_IMPROVEMENT
	void setImprovementEnabled(bool enabled);
	void disableImprovement();
	void enableImprovement();

	bool isImprovementEnabled();
	bool isImprovementDisabled();
#endif //#if DEBUG_DISABLE_IMPROVEMENT
}

#endif //#if USE_MTOOLS

#endif //#ifndef _MTOOLS_DEFINITIONS_H_
