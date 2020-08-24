#ifndef _PMJMESH_TREE_GENERATOR_H_
#define _PMJMESH_TREE_GENERATOR_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/TreeGenerator.h"

namespace PMJMesh
{
	class TreeGenerator : public MJMesh::TreeGenerator
	{
	public:

		TreeGenerator(UInt dimension, MJMesh::Boundary *boundary, Real factor
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			, const Parallel::ThreadManager *tm
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			);
		virtual ~TreeGenerator();

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		virtual void setThreadManager(const Parallel::ThreadManager *tm);
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
		virtual void setElementsMap(FEListHashMap *elementsMap) const;
		virtual FEListHashMap *getElementsMap() const;
#else
		virtual void setElementsMap(FEListMap *elementsMap) const;
		virtual FEListMap *getElementsMap() const;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

		virtual void execute();

		//virtual bool subdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e);

	protected:

		//virtual Data::FrontElementList findElements() const;
		virtual const Data::FrontElementList *findElements() const;

		virtual Data::Tree *makeTree(const Data::Point2D &min, const Data::Point2D &max) const;
		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		using MJMesh::TreeGenerator::subdivide;
		virtual void subdivide(Data::BoxTreeCell *c) const;
		virtual void classifySubdividedCell(Data::BoxTreeCell *c) const;

		//virtual void findOnCells(const Data::BoxTreeCell *c, const Data::FrontElementList &elements) const;
		virtual void findOnCells(const Data::BoxTreeCell *c, const Data::FrontElementList *elements) const;

		virtual PMJMesh::AngleTreeCellClassifier *makeTCC() const;

		virtual void classify() const;

#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)
		virtual bool classifyByOnNeighbor(const Data::BoxTreeCell *c) const;
		virtual void classifyUnknowns(const Data::FrontElementList *elements) const;
#else
		//virtual enum Data::Position classify(const Data::BoxTreeCell *c, const Data::FrontElementList &elements) const;
		virtual enum Data::Position classify(const Data::BoxTreeCell *c, const Data::FrontElementList *elements) const;
#endif//#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)

		virtual void setNeighborsPosition(const Data::BoxTreeCell *c, enum Data::Position pos) const;

	protected:

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		const Parallel::ThreadManager *tm;
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

		PMJMesh::AngleTreeCellClassifier *tcc;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
		mutable FEListHashMap *elementsMap;
#else
		mutable FEListMap *elementsMap;
#endif //#if USE_C__11
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)
		mutable Data::GraphNodeList unknowns;
#endif //#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)

#if DEBUG_TIMING_PMJMESH
	public:

		mutable Real timePMJMeshGeneration, timePMJMeshClassification;
#endif //#if DEBUG_TIMING_PMJMESH
	};
}

#endif //#ifndef _PMJMESH_TREE_GENERATOR_H_
