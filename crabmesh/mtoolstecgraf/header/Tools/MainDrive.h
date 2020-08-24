#ifndef _TOOLS_MAIN_DRIVE_H_
#define _TOOLS_MAIN_DRIVE_H_

#include "Tools/Definitions.h"
#include "Performer/MainDrive.h"
#include "TecGraf/Definitions.h"

#include "Tools/MeshInterceptionChecker.h"
#include "Tools/MeshTopologyChecker.h"
#include "Tools/BoundaryWaterTightnessChecker.h"
#include "Tools/CrackRemover.h"
#include "Tools/BoundaryConversor.h"
#include "Tools/BoundaryNormalFixer.h"
#include "Tools/BoundaryNormalInverter.h"
#include "Tools/UniformBoundaryAdapter.h"
#include "Tools/BoundaryLaplacianSmoother.h"
#include "Tools/SizeBoundaryAdapter.h"
#include "Tools/EdgeSwapBoundaryAdapter.h"
#include "Tools/EdgeCollapseBoundaryAdapter.h"
#include "Tools/EdgeBoundaryAdapter.h"
#include "Tools/InnerSurfacesIdentifier.h"
#include "Tools/SilhouetteIdentifier.h"
#include "Tools/SurfacesIdentifier.h"
#include "Tools/VisibilityIdentifier.h"
#include "Tools/FrontElementVisibilityIdentifier.h"
#include "Tools/VisibilityBasedVertexFinder.h"
#include "Tools/CovarianceMatrixFinder.h"
#include "Tools/PrincipalComponentsAnalyzer.h"
#include "Tools/HotellingTransformer.h"
#include "Tools/MeshMerger.h"
#include "Tools/MeshSplitter.h"
#include "Tools/MeshColorizer.h"
#include "Tools/VertexMerger.h"
#include "Tools/BoundaryCreator.h"
#include "Tools/MeshTransformer.h"
#include "Tools/TecGrafBoundaryBuilder.h"

namespace Tools
{
	class MainDrive : public Performer::MainDrive
	{
	public:

		MainDrive();
		virtual ~MainDrive();

		virtual void setTool(Tool *tool);
		virtual Tool *getTool() const;

		virtual void addTool(Tool *tool);
		virtual Tool *getLastTool() const;

		virtual UInt getDimension() const;

		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual MJMesh::Boundary *getBoundary() const;

		virtual void setMesh(MJMesh::Mesh *mesh);
		virtual MJMesh::Mesh *getMesh() const;

#if USE_TECGRAF
		virtual void setModel(TecGraf::Model *model);
		virtual TecGraf::Model *getModel() const;
#endif //#if USE_TECGRAF

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual void execute();

		virtual void clear();

	protected:

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &outputFileName) const;

		virtual bool loadOutputFile(const std::string &inputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;

		virtual void findQuality();

    protected:

        static const UInt QUALITY_RANGE;

	protected:

		Tool *tool;

		ToolList tools;

		UInt dimension;

		MJMesh::Boundary *boundary;
		MJMesh::Mesh *mesh;

#if USE_TECGRAF
		TecGraf::Model *model;
#endif //#if USE_TECGRAF

		Performer::BoundaryBuilder *boundaryBuilder;

		Data::Mesh::Quality quality;

	};
}

#endif //#ifndef _TOOLS_MAIN_DRIVE_H_
