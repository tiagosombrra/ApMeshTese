#ifndef _PMTOOLS_MESH_GENERATOR_H_
#define _PMTOOLS_MESH_GENERATOR_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "Data/Box.h"
#include "Performer/MeshGenerator.h"
#include "OctTree.hpp"

namespace PMTools
{
	class MeshGenerator : public Performer::MeshGenerator
	{
	public:

		MeshGenerator();
		virtual ~MeshGenerator();

		virtual void setUseBoxTest(bool useBoxTest);
		virtual void setWorkerTask(bool workerTask);
		virtual void setEnableBackTracking(bool enableBackTracking);
		virtual void setBox(const Data::Box &box);
		virtual void setTreeBox(const Data::Box &treeBox);
		virtual void setRangeProportion(double rangeProportion);

		virtual void setFtree(const FTools::OctTree *ftree);

		virtual void setFront(MJMesh::Front *front);
		virtual void setBoundary(const MJMesh::Boundary *boundary);
		virtual void setMesh(Data::Mesh *mesh);

		virtual void setDeleteFels(bool deleteFels);

		virtual void execute();

	protected:

		virtual PMTools::MshRegion3D *makeMshRegion() const;
		virtual void convertFrontToMTools();
		virtual bool executeFrontAdvancing();
		virtual bool executeMeshBuilding();

	public:

		enum
		{
			SUCCESS = 0,
			NOT_INIT,
			INIT,

			ERROR = 100
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status error;
		static const Performer::Status success;

	protected:

		bool useBoxTest;
		bool workerTask;
		bool enableBackTracking;
		Data::Box box;
		Data::Box treeBox;
		double rangeProportion;

		MJMesh::Front *front;
		const MJMesh::Boundary *boundary;

		const FTools::OctTree *ftree;

		MshRegion3D *mshRegion;

		bool fromMesh;

		ULInt maxIdFrontVertex;
		ULInt maxIdVertex;
		Data::VertexVector vMap;

		int maxIdElement;
#if USE_C__11
		Data::ElementHashMap eMap;
#else
		Data::ElementVector eMap;
#endif //#if USE_C__11

        Data::VertexList verticesAddedToBoundary;
        Data::VertexList verticesNotAddedToBoundary;
        Data::FrontElementList felsAddedToBoundary[4];
        Data::FrontElementList felsNotAddedToBoundary[4];

		//used for PMTools2, true for PMTools
		bool deleteFels;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeOtherStuff;
		double timeMeshGeneration;
		double timeMeshImprovement;
		double timeToMToolsConversion;
		double timeFromMToolsConversion;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_MESH_GENERATOR_H_
