#ifndef _PMJMESH_MIN_TREE_REFINER_H_
#define _PMJMESH_MIN_TREE_REFINER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/MinTreeRefiner.h"

namespace PMJMesh
{
	class MinTreeRefiner : public MJMesh::MinTreeRefiner
	{
	public:

		MinTreeRefiner(UInt dimension, Data::BoxTree *tree,
			const MJMesh::Boundary *boundary, UInt minLevel, bool postponeMinRefinement,
			const Parallel::ThreadManager *tm);
		virtual ~MinTreeRefiner();

		virtual void setDimension(UInt dimension);

		virtual void setThreadManager(const Parallel::ThreadManager *tm);

		virtual void setPostponeMinRefinement(bool postponeMinRefinement);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
		virtual void setElementsMap(FEListHashMap *elementsMap);
		virtual FEListHashMap *getElementsMap() const;
#else
		virtual void setElementsMap(FEListMap *elementsMap);
		virtual FEListMap *getElementsMap() const;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

		virtual void execute();

	protected:

	    virtual UInt findMinLevel() const;

		virtual PMJMesh::AngleTreeCellClassifier *makeTCC() const;

		virtual Data::Position classifyChild(Data::BoxTreeCell *c, UInt i,
#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			UInt id,
#endif //#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			//const Data::FrontElementList &elements, Data::FrontElementList &intercepting) const;
			const Data::FrontElementList *elements, Data::FrontElementList &intercepting) const;

		using MJMesh::MinTreeRefiner::subdivide;
		virtual void subdivide(Data::BoxTreeCell *c);

		using MJMesh::MinTreeRefiner::refine;
		virtual void refine(Data::BoxTreeCell *c);
		virtual void refineChildren(Data::BoxTreeCell *c);

#if USE_OPENMP
#if (!USE_EXPERIMENTAL_TREE)
		virtual void unsetTree(Data::GraphNodeVector &leaves);
		virtual void setTree(Data::GraphNodeVector &leaves);
#endif //#if (!USE_EXPERIMENTAL_TREE)
#endif //#if USE_OPENMP
		using MJMesh::MinTreeRefiner::setTree;
#if (!USE_EXPERIMENTAL_TREE)
		virtual void addTree(Data::BoxTreeCell *c, Data::GraphNodeList &newLeaves);
#endif //#if (!USE_EXPERIMENTAL_TREE)

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_OPENMP
	protected:

		virtual void buildElementsMap() const;
		//virtual void buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList &elements) const;
		virtual void buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList *elements) const;
#endif //#if USE_OPENMP

	protected:

#if USE_OPENMP
#if USE_C__11
		typedef std::vector<FEListHashMap> FEListHashMapVector;
#endif //#if USE_C__11

		typedef std::vector<FEListMap> FEListMapVector;
#endif //#if USE_OPENMP

#if USE_C__11
		mutable FEListHashMap *elementsMap;
#else
		mutable FEListMap *elementsMap;
#endif //#if USE_C__11

#if USE_OPENMP
#if USE_C__11
		mutable FEListHashMapVector tempElementsMap;
#else
		mutable FEListMapVector tempElementsMap;
#endif //#if USE_C__11
#endif //#if USE_OPENMP
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

	protected:

		UInt dimension;

		const Parallel::ThreadManager *tm;

		Int nthreads;

		bool postponeMinRefinement;

		//Data::FrontElementList elements;
		const Data::FrontElementList *elements;

		PMJMesh::AngleTreeCellClassifier *tcc;

#if DEBUG_TIMING_PMJMESH
	public:

		mutable Real timePMJMeshGeneration, timePMJMeshClassification;
#endif //#if DEBUG_TIMING_PMJMESH
	};
}

#endif //#ifndef _PMJMESH_MIN_TREE_REFINER_H_
