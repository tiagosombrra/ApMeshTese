#ifndef _PMJMESH_NEIGHBOR_TREE_REFINER_H_
#define _PMJMESH_NEIGHBOR_TREE_REFINER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/NeighborTreeRefiner.h"

namespace PMJMesh
{
	class NeighborTreeRefiner : public MJMesh::NeighborTreeRefiner
	{
	public:

		NeighborTreeRefiner(UInt dimension, Data::BoxTree *tree,
			const MJMesh::Boundary *boundary);
		virtual ~NeighborTreeRefiner();

		virtual void setDimension(UInt dimension);

		virtual void setBoundary(const MJMesh::Boundary *boundary);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
		virtual void setElementsMap(FEListHashMap *elementsMap);
		virtual FEListHashMap *getElementsMap() const;
#else
		virtual void setElementsMap(FEListMap *elementsMap);
		virtual FEListMap *getElementsMap() const;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

		using MJMesh::NeighborTreeRefiner::refine;
		virtual void refine(Data::BoxTreeCell *c);

		virtual void execute();

	protected:

		virtual PMJMesh::AngleTreeCellClassifier *makeTCC() const;

		using MJMesh::NeighborTreeRefiner::fillSet;
		virtual void fillSet(Data::BoxTreeCell *cell);

		virtual Data::Position classifyChild(Data::BoxTreeCell *c, UInt i,
			//const Data::FrontElementList &elements, Data::FrontElementList &intercepting) const;
			const Data::FrontElementList *elements, Data::FrontElementList &intercepting) const;

		using MJMesh::NeighborTreeRefiner::subdivide;
		virtual void subdivide(Data::BoxTreeCell *c);

		virtual void refineChildren(Data::BoxTreeCell *c);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	protected:

		virtual void buildElementsMap() const;
		//virtual void buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList &elements) const;
		virtual void buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList *elements) const;

	protected:

#if USE_C__11
		mutable FEListHashMap *elementsMap;
#else
		mutable FEListMap *elementsMap;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

	protected:

		UInt dimension;

		const MJMesh::Boundary *boundary;

		//Data::FrontElementList elements;
		const Data::FrontElementList *elements;

		PMJMesh::AngleTreeCellClassifier *tcc;

#if DEBUG_TIMING_PMJMESH
	public:

		mutable Real timePMJMeshGeneration, timePMJMeshClassification;
#endif //#if DEBUG_TIMING_PMJMESH
	};
}

#endif //#ifndef PMJMESH_NEIGHBOR_TREE_REFINER_H_
