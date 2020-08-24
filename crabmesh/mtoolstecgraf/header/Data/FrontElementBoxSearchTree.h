#ifndef _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_H_
#define _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_H_

#include "Data/Definitions.h"
#include "Data/BoxSearchTree.h"
#include "Data/FrontElementBoxSearchTreeCell.h"

namespace Data
{
#if USE_EXPERIMENTAL_5

	class FrontElementBoxSearchTree2D : public Data::BoxSearchTree2D
	{
	public:

		FrontElementBoxSearchTree2D(const BoxTree *tree = NULL,
			FrontElementBoxSearchTreeCell2D *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree2D();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};

	class FrontElementBoxSearchTree3D : public Data::BoxSearchTree3D
	{
	public:

		FrontElementBoxSearchTree3D(const BoxTree *tree = NULL,
			FrontElementBoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree3D();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};

#else

	class FrontElementBoxSearchTree : public Data::BoxSearchTree
	{
	public:

		FrontElementBoxSearchTree(const BoxTree *tree = NULL,
			FrontElementBoxSearchTreeCell *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};
#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _DATA_FRONT_ELEMENT_BOX_SEARCH_TREE_H_
