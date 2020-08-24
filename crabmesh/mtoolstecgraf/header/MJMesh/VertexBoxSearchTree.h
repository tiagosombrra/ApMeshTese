#ifndef _MJMESH_VERTEX_BOX_SEARCH_TREE_H_
#define _MJMESH_VERTEX_BOX_SEARCH_TREE_H_

#include "MJMesh/Definitions.h"

#include "Data/VertexBoxSearchTree.h"

namespace MJMesh
{
#if USE_EXPERIMENTAL_5

	class VertexBoxSearchTree2D : public Data::VertexBoxSearchTree2D
	{
	public:

		VertexBoxSearchTree2D(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::VertexBoxSearchTreeCell2D *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree2D();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
	};

	class VertexBoxSearchTree3D : public Data::VertexBoxSearchTree3D
	{
	public:

		VertexBoxSearchTree3D(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::VertexBoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree3D();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
	};

#else

	class VertexBoxSearchTree : public Data::VertexBoxSearchTree
	{
	public:

		VertexBoxSearchTree(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::VertexBoxSearchTreeCell *root = NULL, ULInt id = 0);
		virtual ~VertexBoxSearchTree();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _MJMESH_VERTEX_BOX_SEARCH_TREE_H_
