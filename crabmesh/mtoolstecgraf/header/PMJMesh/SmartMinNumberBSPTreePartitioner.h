#ifndef _PMJMESH_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_
#define _PMJMESH_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_

#include "PMJMesh/Definitions.h"
#include "PMJMesh/MinNumberBSPTreePartitioner.h"

namespace PMJMesh
{
	class SmartMinNumberBSPTreePartitioner : public PMJMesh::MinNumberBSPTreePartitioner
	{
	public:

		SmartMinNumberBSPTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator,
			bool usePartitioningThreshold, UInt minNumPartitions,
			UInt removeLayersInLoad = 0, bool proportional = true);
		virtual ~SmartMinNumberBSPTreePartitioner();

	protected:

		struct SplitCoord
		{
			UInt split;
			Real coord;
			Real load[2];
		};

#if USE_C__11
		typedef std::unordered_map<Data::BoxTreeCell *, Data::GraphNodeList> BSPBoxHashMap;
#else
		typedef std::map<Data::BoxTreeCell *, Data::GraphNodeList> BSPBoxMap;
#endif //#if USE_C__11

		struct Proportion
		{
			UInt min;
			UInt max;
		};

#if USE_C__11
		typedef std::unordered_map<ULInt, Proportion> ProportionHashMap;
#else
		typedef std::map<ULInt, Proportion> ProportionMap;
#endif //#if USE_C__11

	protected:

		virtual void setCurrentProportion(const Data::BoxTreeCell *cell);
		virtual void addProportion(const Data::BoxTreeCell *cell, UInt proportion);
		virtual void addChildrenProportion(const Data::BoxTreeCell *cell);
		virtual void findProportion(Real v0, Real v1, Real &prop0, Real &prop1) const;

		using PMJMesh::MinNumberBSPTreePartitioner::shouldSkip;
		virtual bool shouldSkip(const Data::GraphNodeList &cells) const;
		virtual bool shouldSkip(const Data::BoxTreeCell *partitionCell) const;

		virtual void buildTree();

		virtual bool isMin(const Data::BoxTreeCell *c, Real coord, UInt split) const;
		//virtual bool isMin(const Data::BoxTreeCell *c, const Data::Point &mid, UInt split) const;
		virtual bool isEqual(Real newLoad[2], Real oldLoad[2]);
		virtual bool isEqual(Real newLoad, Real oldLoad);
		virtual bool isBetter(Real newLoad[2], Real oldLoad[2]);
		virtual bool isBetter(Real newLoad, Real oldLoad);

		virtual bool split(const Data::GraphNodeList &list, UInt split, Real coord, Real limits[2], Data::GraphNodeList cells[2]) const;

		virtual Real findInitialSplitCoord(UInt split, Real reference, const Data::GraphNodeList &cells, Real min, Real max) const;

		virtual SplitCoord findSplitCoord(Data::BSPTreeCell *cell, Data::GraphNodeList cells[2]);
		virtual SplitCoord findSplitCoord(UInt split, Real coord, Real plimits[2],
			Data::GraphNodeList cells[2]);
		virtual SplitCoord findSplitCoord(UInt split, Real coord, Real plimits[2],
			Data::GraphNodeList cells[2], Data::GraphNodeList fixed[2], Real load[2], bool skipFixed[2]);

	protected:

#if USE_C__11
		BSPBoxHashMap bspMap;
#else
		BSPBoxMap bspMap;
#endif //#if USE_C__11

		UInt removeLayersInLoad;

		bool proportional;

#if USE_C__11
		ProportionHashMap proportions;
#else
		ProportionMap proportions;
#endif //#if USE_C__11

		Proportion current;

	};
}

#endif //#ifndef _PMJMESH_SMART_MIN_NUMBER_BSP_TREE_PARTITIONER_H_
