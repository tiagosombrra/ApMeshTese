#ifndef _DATA_VERTEX_BOX_SEARCH_TREE_H_
#define _DATA_VERTEX_BOX_SEARCH_TREE_H_

#include "Data/Definitions.h"
#include "Data/BoxSearchTree.h"
#include "Data/VertexBoxSearchTreeCell.h"

namespace Data
{
#if USE_EXPERIMENTAL_5
	class VertexBoxSearchTree2D : public Data::BoxSearchTree2D
	{
	public:

		VertexBoxSearchTree2D(const BoxTree *tree = NULL,
			VertexBoxSearchTreeCell2D *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree2D();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};

	class VertexBoxSearchTree3D : public Data::BoxSearchTree3D
	{
	public:

		VertexBoxSearchTree3D(const BoxTree *tree = NULL,
			VertexBoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree3D();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};

#else

	class VertexBoxSearchTree : public Data::BoxSearchTree
	{
	public:

		VertexBoxSearchTree(const BoxTree *tree = NULL,
			VertexBoxSearchTreeCell *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree();

	protected:

		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _DATA_VERTEX_BOX_SEARCH_TREE_H_
