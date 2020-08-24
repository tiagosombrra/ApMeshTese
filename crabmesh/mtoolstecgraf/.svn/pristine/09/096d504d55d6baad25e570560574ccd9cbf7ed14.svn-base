#ifndef _PERFORMER_NEIGHBOR_TREE_REFINER_H_
#define _PERFORMER_NEIGHBOR_TREE_REFINER_H_

#include "Performer/Definitions.h"

#include "Performer/TreeRefiner.h"

namespace Performer
{
	class NeighborTreeRefiner : public Performer::TreeRefiner
	{
	public:

		NeighborTreeRefiner(Data::BoxTree *tree = NULL, UInt difference = 1);
		virtual ~NeighborTreeRefiner();

		virtual void setDifference(UInt difference);
		virtual UInt getDifference() const;

		virtual Data::GraphNodeList findNeighbors(const Data::BoxTreeCell *c) const;

		virtual void refine(Data::TreeNode *n);
		virtual void refine(Data::BoxTreeCell *c);

		virtual void execute();

	protected:

		virtual void subdivide(Data::TreeNode *n);
		virtual void subdivide(Data::BoxTreeCell *c);

		virtual void refineChildren(Data::BoxTreeCell *c);

	public:

		static bool smaller(Data::BoxTreeCell *const &c1, Data::BoxTreeCell *const &c2);

	protected:

		typedef std::multiset<Data::BoxTreeCell *, bool (*)(Data::BoxTreeCell *const &, Data::BoxTreeCell *const &)> BoxTreeCellMultiSet;
#if USE_C__11
		typedef std::unordered_map<Data::BoxTreeCell *, BoxTreeCellMultiSet::iterator> BoxTreeCellMap;
#else
		typedef std::map<Data::BoxTreeCell *, BoxTreeCellMultiSet::iterator> BoxTreeCellMap;
#endif //#if USE_C__11

	protected:

		virtual BoxTreeCellMultiSet *makeSet() const;

		virtual void executeWithSet();

		virtual void fillSet();
		virtual void fillSet(Data::BoxTreeCell *cell);
		virtual void refineSet();

	protected:

		UInt difference;

		//bool refined;

	protected:

		BoxTreeCellMultiSet *cells;
		BoxTreeCellMap map;
	};
}

#endif //#ifndef _PERFORMER_MIN_TREE_REFINER_H_
