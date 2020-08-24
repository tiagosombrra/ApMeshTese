#ifndef _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_H_
#define _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/FrontElementBoxSearchTree.h"

namespace PMJMesh
{
#if USE_EXPERIMENTAL_5

	class FrontElementBoxSearchTree2D : public MJMesh::FrontElementBoxSearchTree2D
	{
	public:

		FrontElementBoxSearchTree2D(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~FrontElementBoxSearchTree2D();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		//virtual bool build(BoxTreeCell *cell, BoxSearchTreeCell *scell);
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::FrontElementBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;
	};

	class FrontElementBoxSearchTree3D : public MJMesh::FrontElementBoxSearchTree3D
	{
	public:

		FrontElementBoxSearchTree3D(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~FrontElementBoxSearchTree3D();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		//virtual bool build(BoxTreeCell *cell, BoxSearchTreeCell *scell);
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::FrontElementBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;
	};

#else

	class FrontElementBoxSearchTree : public MJMesh::FrontElementBoxSearchTree
	{
	public:

		FrontElementBoxSearchTree(const MJMesh::Front *front, const Data::BoxTree *tree,
			const Data::Box *box);
		virtual ~FrontElementBoxSearchTree();

		virtual void setBox(const Data::Box *box);

		virtual bool build();

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

#if USE_UNIQUE_SEARCH_TREE
		//virtual bool build(BoxTreeCell *cell, BoxSearchTreeCell *scell);
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#else
		//using MJMesh::FrontElementBoxSearchTree::build;
		virtual bool build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *scell);
#endif //#if USE_UNIQUE_SEARCH_TREE

	protected:

		const Data::Box *box;

		bool cellInBox;

		UInt dimension;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _PMJMESH_FRONT_ELEMENT_SEARCH_TREE_H_
