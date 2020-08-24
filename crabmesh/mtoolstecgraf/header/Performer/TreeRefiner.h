#ifndef _PERFORMER_TREE_REFINER_H_
#define _PERFORMER_TREE_REFINER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class TreeRefiner : public Performer::Builder
	{
	public:

		TreeRefiner(Data::Tree *tree = NULL);
		virtual ~TreeRefiner();

		virtual void setTree(Data::Tree *tree);

		virtual void refine(Data::TreeNode *n) = 0;

	protected:

		virtual void subdivide(Data::TreeNode *n) = 0;

	protected:

		Data::Tree *tree;
	};
}

#endif //#ifndef _PERFORMER_TREE_REFINER_H_
