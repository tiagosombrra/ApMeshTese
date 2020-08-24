#include "MJMesh/FrontElementBoxSearchTree.h"

#include "MJMesh/FrontElementBoxSearchTreeCell.h"

#if USE_EXPERIMENTAL_5

MJMesh::FrontElementBoxSearchTree2D::FrontElementBoxSearchTree2D(const Front *front,
	const Data::BoxTree *tree,
	MJMesh::FrontElementBoxSearchTreeCell2D *root, ULInt id) :
		Data::FrontElementBoxSearchTree2D(tree, root, id)
{
	this->setFront(front);
}

MJMesh::FrontElementBoxSearchTree2D::~FrontElementBoxSearchTree2D()
{

}

void MJMesh::FrontElementBoxSearchTree2D::setFront(const Front *front)
{
	this->front = front;
}

const MJMesh::Front *MJMesh::FrontElementBoxSearchTree2D::getFront() const
{
	return this->front;
}

Data::BoxSearchTreeCell *MJMesh::FrontElementBoxSearchTree2D::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::FrontElementBoxSearchTreeCell2D(c, static_cast<MJMesh::FrontElementBoxSearchTreeCell2D *>(parent), id);
}

MJMesh::FrontElementBoxSearchTree3D::FrontElementBoxSearchTree3D(const Front *front,
	const Data::BoxTree *tree,
	MJMesh::FrontElementBoxSearchTreeCell3D *root, ULInt id) :
		Data::FrontElementBoxSearchTree3D(tree, root, id)
{
	this->setFront(front);
}

MJMesh::FrontElementBoxSearchTree3D::FrontElementBoxSearchTree3D(const Boundary *boundary,
	const Data::BoxTree *tree,
	MJMesh::FrontElementBoxSearchTreeCell3D *root, ULInt id) :
		Data::FrontElementBoxSearchTree3D(tree, root, id)
{
	this->setBoundary(boundary);
}

MJMesh::FrontElementBoxSearchTree3D::~FrontElementBoxSearchTree3D()
{

}

void MJMesh::FrontElementBoxSearchTree3D::setFront(const Front *front)
{
	this->front = front;
	this->boundary = NULL;
}

const MJMesh::Front *MJMesh::FrontElementBoxSearchTree3D::getFront() const
{
	return this->front;
}

void MJMesh::FrontElementBoxSearchTree3D::setBoundary(const Boundary *boundary)
{
	this->front = NULL;
	this->boundary = boundary;
}

const MJMesh::Boundary *MJMesh::FrontElementBoxSearchTree3D::getBoundary() const
{
	return this->boundary;
}

Data::BoxSearchTreeCell *MJMesh::FrontElementBoxSearchTree3D::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::FrontElementBoxSearchTreeCell3D(c, static_cast<MJMesh::FrontElementBoxSearchTreeCell3D *>(parent), id);
}

#else

MJMesh::FrontElementBoxSearchTree::FrontElementBoxSearchTree(const Front *front,
	const Data::BoxTree *tree,
	MJMesh::FrontElementBoxSearchTreeCell *root, ULInt id) :
		Data::FrontElementBoxSearchTree(tree, root, id)
{
	this->setFront(front);
}

MJMesh::FrontElementBoxSearchTree::~FrontElementBoxSearchTree()
{

}

void MJMesh::FrontElementBoxSearchTree::setFront(const Front *front)
{
	this->front = front;
}

const MJMesh::Front *MJMesh::FrontElementBoxSearchTree::getFront() const
{
	return this->front;
}

Data::BoxSearchTreeCell *MJMesh::FrontElementBoxSearchTree::makeTreeCell(Data::BoxTreeCell *c, Data::BoxSearchTreeCell *parent, ULInt id) const
{
	return new MJMesh::FrontElementBoxSearchTreeCell(c, static_cast<MJMesh::FrontElementBoxSearchTreeCell *>(parent), id);
}

#endif //#if USE_EXPERIMENTAL_5
