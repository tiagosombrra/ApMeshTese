#ifndef _TOOLS_DEFINITIONS_H_
#define _TOOLS_DEFINITIONS_H_

#include "MJMesh/Definitions.h"

namespace Tools
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	class Tool;
	class ExternalTool;

	class CracksIdentifier;
	class AdjacenciesIdentifier;

	class MeshInterceptionChecker;
	class MeshTopologyChecker;
	class BoundaryWaterTightnessChecker;

	class CrackRemover;
	class BoundaryConversor;
	class BoundaryNormalFixer;
	class BoundaryNormalInverter;
	class UniformBoundaryAdapter;
	class BoundaryLaplacianSmoother;
	class SizeBoundaryAdapter;
	class EdgeSwapBoundaryAdapter;
	class EdgeCollapseBoundaryAdapter;
	class EdgeBoundaryAdapter;
	class InnerSurfacesIdentifier;
	class SilhouetteIdentifier;
	class SurfacesIdentifier;

	class VisibilityIdentifier;
	class FrontElementVisibilityIdentifier;
	class VisibilityBasedVertexFinder;

	class MeshMerger;
	class MeshSplitter;
	class VertexMerger;
	class BoundaryCreator;
#if USE_GUI
	class MeshColorizer;
#endif //#if USE_GUI

	class MeshTransformer;

#if USE_TECGRAF
	class TecGrafBoundaryBuilder;
#endif //#if USE_TECGRAF

#if USE_NUMERICAL
    class CovarianceMatrixFinder;
    class PrincipalComponentsAnalyzer;
    class HotellingTransformer;
#endif //#if USE_NUMERICAL

	class MainDrive;

	enum ToolType
	{
		UNKNOWN = 0,

		CRACKS_IDENTIFIER,
		ADJACENCIES_IDENTIFIER,

		CHECK_MESH_INTERCEPTION,
		CHECK_MESH_TOPOLOGY,
		CHECK_WATER_TIGHTNESS,

		BOUNDARY_CRACK_REMOVER,
		BOUNDARY_CONVERSOR,
		BOUNDARY_NORMAL_FIXER,
		BOUNDARY_NORMAL_INVERTER,
		BOUNDARY_INNER_SURFACES_IDENTIFIER,

		REFINEMENT_BOUNDARY_UNIFORM,
		REFINEMENT_BOUNDARY_LAPLACIAN_SMOOTHER,
		REFINEMENT_BOUNDARY_SIZE,
		REFINEMENT_BOUNDARY_EDGE_SWAP,
		REFINEMENT_BOUNDARY_EDGE_COLLAPSE,

		MESH_MERGER,
		MESH_SPLITTER,
		MESH_VERTEX_MERGER,
		MESH_BOUNDARY_CREATOR,

		TRANSFORMER_TRANSLATION,
		TRANSFORMER_SCALE,
		TRANSFORMER_ROTATION

#if USE_TECGRAF
		, TECGRAF_BOUNDARY_BUILDER
#endif //#if USE_TECGRAF

#if USE_NUMERICAL
        , COVARIANCE_MATRIX_FINDER
        , PRINCIPAL_COMPONENTS_ANALYZER
        , HOTELLING_TRANSFORMER
#endif //#if USE_NUMERICAL
	};

	typedef std::list<Tool *> ToolList;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)
}

#endif //#ifndef _TOOLS_DEFINITIONS_H_
