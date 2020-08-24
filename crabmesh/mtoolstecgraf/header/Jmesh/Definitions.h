#ifndef _JMESH_DEFINITIONS_H_
#define _JMESH_DEFINITIONS_H_

#include "MJMesh/Definitions.h"

namespace Jmesh
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

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
}

#endif //#ifndef _JMESH_DEFINITIONS_H_
