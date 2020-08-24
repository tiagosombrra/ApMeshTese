#ifndef _PMJMESH_APRIORI_TASK_MANAGER_H_
#define _PMJMESH_APRIORI_TASK_MANAGER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/GlobalTaskManager.h"

namespace PMJMesh
{
	class APrioriTaskManager : public PMJMesh::GlobalTaskManager
	{
	public:

		APrioriTaskManager(bool sharedParallel, UInt dimension,
			MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
			bool heightDependentInLoad, bool smoothInSlavesEnabled, UInt numImproves, Real smoothingFactor,
#if (!USE_DELETE_FELS)
			UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			UInt smoothingLayer,	bool localBackTrackingEnabled,
			bool meshSorted, UInt maxBackTracks, enum PartitionerType partitionerType,
			enum MeshUpdateType updateType,
			bool useMean, Real partitioningThreshold, bool sorted, bool usingSearchTrees,
			UInt minRefinementLevel, bool skippingInnerInDecomposition,
			bool createInnerFrontInDecomposition,
			enum Parallel::AvailableLoadBalancers balancerType,
			Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm);
		virtual ~APrioriTaskManager();

		virtual void interceptPartitionFront(Data::Box *box, Data::TreeNode *root,//);
                                            std::list<Data::TreeNode *> &topBoundaryCells, std::list<Data::TreeNode *> &baseBoundaryCells,
                                            std::list<Data::TreeNode *> &leftBoundaryCells, std::list<Data::TreeNode *> &rightBoundaryCells);

		virtual void buildSubDomain(Data::Box *box, PMJMesh::Front *sub_front);

		virtual void execute();

	protected:

		//Markos
		virtual MJMesh::Vertex *findOrCreate(PMJMesh::Front *sub_front, Real x, Real y, Real z = 0.0);
		virtual void adjustSubfronts();

		static bool compareBoxY(Data::TreeNode *first, Data::TreeNode *second);
		static bool compareBoxX(Data::TreeNode *first, Data::TreeNode *second);

		virtual bool existEdgeInGlobal(MJMesh::FrontElement *e);

		virtual bool includeInModel(Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList element_front, Data::FrontElementList &rejected, MJMesh::FrontElement2D *e, MJMesh::Vertex *v1, MJMesh::Vertex *v2);

		virtual MJMesh::Vertex* nearestVertexSubFront(Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList element_front, MJMesh::FrontElement2D *e, MJMesh::Vertex* vertex);

		virtual MJMesh::Vertex* bestVertex(PMJMesh::Front *sub_front, Data::Edge2D *e);

		virtual bool boxEdgeTest(Data::Box *box, Data::TreeNode *root);

		virtual MJMesh::FrontElement2D * InitialEdge(Data::Box *box, Data::FrontElementList element_front, PMJMesh::Front *sub_front, Data::FrontElementList &rejected, MJMesh::Vertex *vMin, MJMesh::FrontElement2D *e, bool invert);

        virtual void removeDanglingEdges(Data::Box *box, PMJMesh::Front *sub_front, MJMesh::FrontElement *e);

        virtual void removeOutEdges(Data::Box *box, PMJMesh::Front *sub_front);

		virtual void cleanOutEdges(Data::Box *box, PMJMesh::Front *sub_front);

		//Markos
		virtual bool setTaskParameters(Parallel::Task *t,
			MJMesh::Boundary *boundary, Data::BoxTree *tree,
			Performer::IdManager *idManager) const;

		//Markos
		virtual LoadCalculator *makeLoadCalculator(bool initializing) const;
#if USE_C__11
		virtual TreePartitioner *makeTreePartitioner(const Data::RealHashMap *load, PMJMesh::LoadCalculator *loadCalculator) const;
#else
		virtual TreePartitioner *makeTreePartitioner(const Data::RealMap *load, PMJMesh::LoadCalculator *loadCalculator) const;
#endif //#if USE_C__11
		virtual FrontPartitioner *makeFrontPartitioner() const;

		/*//Markos
#if USE_NEW_FRONT_UPDATER
		virtual MeshUpdater *makeMeshUpdater() const;
#endif //#if USE_NEW_FRONT_UPDATER*/
	};
}

#endif //#ifndef _PMJMESH_APRIORI_TASK_MANAGER_H_
