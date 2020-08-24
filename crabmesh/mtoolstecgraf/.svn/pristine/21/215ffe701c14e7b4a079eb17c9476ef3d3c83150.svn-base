#ifndef _PMJMESH_WORKER_MAIN_DRIVE_H_
#define _PMJMESH_WORKER_MAIN_DRIVE_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/MainDrive.h"
//debug
#include "Data/Identifiable.h"
//endebug

namespace PMJMesh
{
	class WorkerMainDrive : public MJMesh::MainDrive, public Data::Identifiable
	{
	public:

		WorkerMainDrive(Parallel::Communicator *comm, UInt status = 0);
		virtual ~WorkerMainDrive();

		virtual void setComm(Parallel::Communicator *comm);

		virtual void setDimension(UInt dimension);

#if USE_GUI
		using MJMesh::MainDrive::setFront;
#endif //#if USE_GUI

		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual void setTree(Data::BoxTree *tree);
		virtual void setFront(PMJMesh::Front *front);
		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void setBox(Data::Box *box);

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual void setTestingBox(bool testingBox);
		virtual void setRangeProportion(Real rangeProportion);
		virtual void setSmoothInSlavesEnabled(bool smoothInSlavesEnabled);
#if (!USE_DELETE_FELS)
		virtual void setUnsmoothingLayer(UInt unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
		virtual void setMinRefinementLevel(UInt minRefinementLevel);
#if (!USE_DELETE_FELS)
		virtual void setRemoveBoundaryFelsForSmoothing(bool removeBoundaryFelsForSmoothing);
#endif //#if (!USE_DELETE_FELS)
		virtual void setClassifyTreeCells(bool classifyTreeCells);
		virtual void setFindingCommonFront(bool findingCommonFront);

#if USE_NEW_FRONT_UPDATER
		virtual void setOldFront(PMJMesh::Front *oldFront);
		virtual PMJMesh::Front *getOldFront() const;

		virtual void setNewFront(PMJMesh::Front *newFront);
		virtual PMJMesh::Front *getNewFront() const;

		virtual void setCommonFront(PMJMesh::Front *commonFront);
		virtual PMJMesh::Front *getCommonFront() const;
#else
		virtual void setOldFront(const Data::FrontElementList &oldFront);
		virtual const Data::FrontElementList &getOldFront() const;

		virtual void setNewFront(const Data::FrontElementList &newFront);
		virtual const Data::FrontElementList &getNewFront() const;

		virtual void setCommonFront(const Data::FrontElementList &commonFront);
		virtual const Data::FrontElementList &getCommonFront() const;
#endif //#if USE_NEW_FRONT_UPDATER

		virtual void execute();
		/*virtual void execute1();
		virtual void execute2();
		virtual void execute3();
		virtual void execute4();
		virtual void executer();*/

#if USE_THREAD_COLOR
		static void getMeshColor(Real &r, Real &g, Real &b, Parallel::ThreadManager *tm, Int process = -1);
		virtual void colorizeMesh(Int process = -1) const;
#endif //#if USE_THREAD_COLOR

	public:

		enum
		{
			ERROR_MIN_TREE_REFINEMENT = ERROR_MESH_NOT_GENERATED_BACK_TRACKING + 1,
			ERROR_FRONT_GEOMETRY_ADVANCER,
			ERROR_FRONT_TOPOLOGY_ADVANCER,
			ERROR_FRONT_BACK_TRACKING_ADVANCER,
			ERROR_MESH_IMPROVER,
			ERROR_TREE_CLASSIFIER,
			ERROR_TREE_DISREFINER
		};

		static const Performer::Status errorMinTreeRefinement;
		static const Performer::Status errorFrontGeometryAdvancer;
		static const Performer::Status errorFrontTopologyAdvancer;
		static const Performer::Status errorFrontBackTrackingAdvancer;
		static const Performer::Status errorMeshImprover;
		static const Performer::Status errorTreeClassifier;
		static const Performer::Status errorTreeDisrefiner;

	protected:

		virtual std::string debug() const;
		virtual std::string debug(const Data::BoxTreeCell *cell) const;

		virtual void findCommonFront();
		virtual void invertNewFront();

		virtual bool executeTreeMinRefiner(bool del);
		virtual bool executeFrontGeometryAdvancer();
		virtual bool executeFrontTopologyAdvancer();
		virtual bool executeFrontBackTrackingAdvancer();
		virtual bool executeMeshImprover();
		virtual bool executeTreeClassifier();
		virtual bool executeTreeDisrefiner();

		virtual MJMesh::MinTreeRefiner *makeTreeMinRefiner() const;
		virtual MJMesh::GeometryFrontAdvancer *makeFrontGeometryAdvancer() const;
		virtual MJMesh::TopologyFrontAdvancer *makeFrontTopologyAdvancer() const;
		virtual MJMesh::BackTrackingFrontAdvancer *makeFrontBackTrackingAdvancer() const;
		virtual MJMesh::MeshImprover *makeMeshImprover() const;
		virtual Performer::TreeCellClassifier *makeTreeCellClassifier() const;
		virtual Performer::TreeRefiner *makeTreeMinDisrefiner() const;

	protected:

		Data::Box *box;

		//for tree refinement and disrefinement
		Data::GraphNodeList *refinedCells;

		//for mesh generation
		bool testingBox;
		Real rangeProportion;

		//for mesh improvement
		bool smoothInSlavesEnabled;
#if (!USE_DELETE_FELS)
		UInt unsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)
		UInt minRefinementLevel;
#if (!USE_DELETE_FELS)
		bool removeBoundaryFelsForSmoothing;
#endif //#if (!USE_DELETE_FELS)

		//for mesh improvement and tree cell classification
		bool inverted;

		//for tree cell classification
		bool classifyTreeCells;

#if USE_DELETE_FELS
        mutable Data::VertexList oldInnerVertices;
#endif //#if USE_DELETE_FELS

        bool findingCommonFront;

#if USE_NEW_FRONT_UPDATER
		PMJMesh::Front *oldFront;
		PMJMesh::Front *commonFront;
#else
		Data::FrontElementList oldFront;
		Data::FrontElementList newFront;
		Data::FrontElementList commonFront;
#endif //#if USE_NEW_FRONT_UPDATER
		//Data::FrontElementList crossing; //this is for a priori only (for now)

		Performer::TreeCellClassifier *classifier;
		Performer::TreeRefiner *minTreeDisrefiner;

		Parallel::Communicator *comm;

		/*typedef std::map<Data::FrontElement *, Data::ElementList> ElementListMap;
		ElementListMap oldFrontElements;*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeRefinement;
		double timeTrees;
		double timeGeometry;
		double timeTopology;
		double timeBackTracking;
		double timeClassify;
		double timeImprover;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	};
}

#endif //#ifndef _PMJMESH_WORKER_MAIN_DRIVE_H_
