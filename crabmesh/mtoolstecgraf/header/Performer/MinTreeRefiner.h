#ifndef _PERFORMER_MIN_TREE_REFINER_H_
#define _PERFORMER_MIN_TREE_REFINER_H_

#include "Performer/Definitions.h"
#include "Performer/TreeRefiner.h"

namespace Performer
{
	class MinTreeRefiner : public Performer::TreeRefiner
	{
	public:

		MinTreeRefiner(Data::BoxTree *tree = NULL, UInt minLevel = 0);
		virtual ~MinTreeRefiner();

		virtual void setMinLevel(UInt minLevel);
		virtual UInt getMinLevel() const;

		virtual void refine(Data::TreeNode *n);
		virtual void refine(Data::BoxTreeCell *c);

		virtual UInt findMinLevel() const = 0;

		virtual void execute();

	protected:

		virtual void subdivide(Data::TreeNode *n);
		virtual void subdivide(Data::BoxTreeCell *c);

		virtual void refineChildren(Data::BoxTreeCell *c);

	protected:

		UInt minLevel;
	};
}

#endif //#ifndef _PERFORMER_MIN_TREE_REFINER_H_
