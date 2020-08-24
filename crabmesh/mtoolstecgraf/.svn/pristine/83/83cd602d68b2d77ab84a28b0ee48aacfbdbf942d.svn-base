#ifndef _PMJMESH_VERTEX_BOX_SEARCH_TREE_H_
#define _PMJMESH_VERTEX_BOX_SEARCH_TREE_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/VertexBoxSearchTree.h"

namespace PMJMesh
{
#if USE_EXPERIMENTAL_5

	class VertexBoxSearchTree2D : public MJMesh::VertexBoxSearchTree2D
	{
	public:

		VertexBoxSearchTree2D(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~VertexBoxSearchTree2D();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::VertexBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;
	};

	class VertexBoxSearchTree3D : public MJMesh::VertexBoxSearchTree3D
	{
	public:

		VertexBoxSearchTree3D(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~VertexBoxSearchTree3D();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::VertexBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;
	};

#else

	class VertexBoxSearchTree : public MJMesh::VertexBoxSearchTree
	{
	public:

		VertexBoxSearchTree(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~VertexBoxSearchTree();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::VertexBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;

		UInt dimension;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _PMJMESH_VERTEX_BOX_SEARCH_TREE_H_
