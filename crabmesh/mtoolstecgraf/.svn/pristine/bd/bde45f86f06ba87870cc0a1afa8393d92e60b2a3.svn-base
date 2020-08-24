#include "MJMesh/VertexBoxSearchTree.h"

#include "MJMesh/VertexBoxSearchTreeCell.h"

#if USE_EXPERIMENTAL_5

MJMesh::VertexBoxSearchTree2D::VertexBoxSearchTree2D(const Front *front,
	const Data::BoxTree *tree, MJMesh::VertexBoxSearchTreeCell2D *root, ULInt id) :
		Data::VertexBoxSearchTree2D(tree, root, id)
{
	this->setFront(front);
}

MJMesh::VertexBoxSearchTree2D::~VertexBoxSearchTree2D()
{

}

void MJMesh::VertexBoxSearchTree2D::setFront(const Front *front)
{
	this->front = front;
}

const MJMesh::Front *MJMesh::VertexBoxSearchTree2D::getFront() const
{
	return this->front;
}

Data::BoxSearchTreeCell *MJMesh::VertexBoxSearchTree2D::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::VertexBoxSearchTreeCell2D(c, static_cast<MJMesh::VertexBoxSearchTreeCell2D *>(parent), id);
}

MJMesh::VertexBoxSearchTree3D::VertexBoxSearchTree3D(const Front *front,
	const Data::BoxTree *tree, MJMesh::VertexBoxSearchTreeCell3D *root, ULInt id) :
		Data::VertexBoxSearchTree3D(tree, root, id)
{
	this->setFront(front);
}

MJMesh::VertexBoxSearchTree3D::~VertexBoxSearchTree3D()
{

}

void MJMesh::VertexBoxSearchTree3D::setFront(const Front *front)
{
	this->front = front;
}

const MJMesh::Front *MJMesh::VertexBoxSearchTree3D::getFront() const
{
	return this->front;
}

Data::BoxSearchTreeCell *MJMesh::VertexBoxSearchTree3D::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::VertexBoxSearchTreeCell3D(c, static_cast<MJMesh::VertexBoxSearchTreeCell3D *>(parent), id);
}

#else

MJMesh::VertexBoxSearchTree::VertexBoxSearchTree(const Front *front,
	const Data::BoxTree *tree, MJMesh::VertexBoxSearchTreeCell *root, ULInt id) :
		Data::VertexBoxSearchTree(tree, root, id)
{
	this->setFront(front);
}

MJMesh::VertexBoxSearchTree::~VertexBoxSearchTree()
{

}

void MJMesh::VertexBoxSearchTree::setFront(const Front *front)
{
	this->front = front;
}

const MJMesh::Front *MJMesh::VertexBoxSearchTree::getFront() const
{
	return this->front;
}

Data::BoxSearchTreeCell *MJMesh::VertexBoxSearchTree::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::VertexBoxSearchTreeCell(c, static_cast<MJMesh::VertexBoxSearchTreeCell *>(parent), id);
}

#endif //#if USE_EXPERIMENTAL_5
