#ifndef _TECGRAF_DEFINITIONS_H_
#define _TECGRAF_DEFINITIONS_H_

#include "MJMesh/Definitions.h"

#if USE_TECGRAF

namespace TecGraf
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	// TecGraf::Data

	class Surface;
	class OrientedSurface;
	class Region;
	class Model;

	// TecGraf::Performer

	class BoundaryBuilder;

#if USE_TECGRAF_MSH

#if USE_TECGRAF_MSH_2D
	namespace Msh2D
	{
		enum Algorithms
		{
			UNKNOWN = 0,

			BILINEAR = 1,
			TRY_BILINEAR,

			COLL_BILINEAR,

			LOFT,

			COLL_LOFT,

			TRILINEAR,
			TRY_TRILINEAR,

			BOUND_CONTRACTION,

			BOUND_CONTRACTION_WARP,

			QUAD_BOUND,

			SHAPE,

			EDGE

#if USE_QUADS
			, QUAD_SEAM,
			QUAD_SEAM_EDGE,

			TEMPLATE
#endif //#if USE_QUADS
		};

		class MainDrive;

#if (!USE_GUI)
		std::string help(const std::string &programName);

		int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
	}
#endif //#if USE_TECGRAF_MSH_2D

#if USE_TECGRAF_MSH_3D
	namespace Msh3D
	{
		enum Algorithms
		{
			UNKNOWN = 0,

			MAPP = 1,

			CURVE_SWEEP,

			EXTRUSION,

			MAPPING_CURVE,

			MAPPING_MESH,

			SWEEPING_MESH,

			GENERATION,

			TEMPLATE
		};

		class MainDrive;

#if (!USE_GUI)
		std::string help(const std::string &programName);

		int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
	}
#endif //#if USE_TECGRAF_MSH_3D

#if USE_TECGRAF_MSH_SURF
	namespace MshSurf
	{
		enum Algorithms
		{
			UNKNOWN = 0,

			EDGE = 1,

			CREATE_3D_BIL,

			SURF_3D_ANALYTICAL,
			SURF_3D_SUPPORT,

			CREATE_2D,

			CREATE_2D_EDGE,

			EDGE_2D,

			TEMPLATE
		};

		class MainDrive;

#if (!USE_GUI)
		std::string help(const std::string &programName);

		int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
	}
#endif //#if USE_TECGRAF_MSH_SURF

#endif //USE_TECGRAF_MSH

	typedef std::list<Surface *> SurfaceList;
	typedef std::list<OrientedSurface *> OrientedSurfaceList;
	typedef std::list<Region *> RegionList;

#if USE_C__11
	typedef std::unordered_set<Surface *> SurfaceHash;
#endif //#if USE_C__11

	typedef std::set<Surface *> SurfaceSet;
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TECGRAF_DEFINITIONS_H_
