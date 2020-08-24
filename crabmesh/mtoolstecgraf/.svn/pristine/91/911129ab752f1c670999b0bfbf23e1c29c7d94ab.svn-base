#include "Data/GeometricSearchTree.h"

using namespace Data;

Data::GeometricSearchTree::GeometricSearchTree(GeometricSearchTreeNode *root, ULInt id) :
	Tree(root, id)
{
	this->added = 0;
	this->removed = 0;
	this->addedSinceLastRebuild = 0;
	this->removedSinceLastRebuild = 0;
}

Data::GeometricSearchTree::~GeometricSearchTree()
{

}

bool Data::GeometricSearchTree::add(const Identifiable *s)
{
	if ((this->getRoot()) && (static_cast<GeometricSearchTreeNode *>(this->getRoot())->add(s)))
	{
		this->added++;
		this->addedSinceLastRebuild++;

		return true;
	}

	return false;
}

bool GeometricSearchTree::remove(const Identifiable *s)
{
	if ((this->getRoot()) && (static_cast<GeometricSearchTreeNode *>(this->getRoot())->remove(s)))
	{
		this->removed++;
		this->removedSinceLastRebuild++;

		return true;
	}

	return false;
}

IdentifiableList Data::GeometricSearchTree::allShapes() const
{
	IdentifiableList shapes;

	if (this->getRoot())
	{
		GeometricSearchTreeNode::ShapeSet s = static_cast<GeometricSearchTreeNode *>(this->getRoot())->allShapes();

		shapes.insert(shapes.end(), s.begin(), s.end());
	}

	return shapes;
}

IdentifiableList Data::GeometricSearchTree::inRange(const Box &box) const
{
	IdentifiableList shapes;

	if (this->getRoot())
	{
		GeometricSearchTreeNode::ShapeSet s = static_cast<GeometricSearchTreeNode *>(this->getRoot())->inRange(&box);

		shapes.insert(shapes.end(), s.begin(), s.end());
	}

	return shapes;
}

void Data::GeometricSearchTree::clear()
{
	if (this->getRoot())
	{
		static_cast<GeometricSearchTreeNode *>(this->getRoot())->clear();
	}
}

bool Data::GeometricSearchTree::rebuild()
{
	IdentifiableList shapes = this->allShapes();

	if (this->rebuildTreeNodes())
	{
		this->addedSinceLastRebuild = 0;
		this->removedSinceLastRebuild = 0;

		while (!shapes.empty())
		{
			this->add(shapes.back());

			shapes.pop_back();
		}

		return true;
	}

	return false;
}
