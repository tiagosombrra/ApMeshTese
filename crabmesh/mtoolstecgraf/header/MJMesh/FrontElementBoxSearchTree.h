#ifndef _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_H_
#define _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_H_

#include "MJMesh/Definitions.h"

#include "Data/FrontElementBoxSearchTree.h"

namespace MJMesh
{
#if USE_EXPERIMENTAL_5

	class FrontElementBoxSearchTree2D : public Data::FrontElementBoxSearchTree2D
	{
	public:

		FrontElementBoxSearchTree2D(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::FrontElementBoxSearchTreeCell2D *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree2D();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
	};

	class FrontElementBoxSearchTree3D : public Data::FrontElementBoxSearchTree3D
	{
	public:

		FrontElementBoxSearchTree3D(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::FrontElementBoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		FrontElementBoxSearchTree3D(const Boundary *boundary, const Data::BoxTree *tree = NULL,
			MJMesh::FrontElementBoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree3D();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

		virtual void setBoundary(const Boundary *boundary);
		virtual const Boundary *getBoundary() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
		const Boundary *boundary;
	};

#else

	class FrontElementBoxSearchTree : public Data::FrontElementBoxSearchTree
	{
	public:

		FrontElementBoxSearchTree(const Front *front, const Data::BoxTree *tree = NULL,
			MJMesh::FrontElementBoxSearchTreeCell *root = NULL, ULInt id = 0);
		virtual ~FrontElementBoxSearchTree();

		virtual void setFront(const Front *front);
		virtual const Front *getFront() const;

	protected:

		virtual Data::BoxSearchTreeCell *makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const;

	protected:

		const Front *front;
	};

#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _MJMESH_FRONT_ELEMENT_BOX_SEARCH_TREE_H_
