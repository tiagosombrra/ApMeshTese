#include "Data/GeometricSearchTreeNode.h"

using namespace Data;

Data::GeometricSearchTreeNode::GeometricSearchTreeNode(
	GeometricSearchTreeNode *parent, ULInt id)
	: TreeNode(parent, id)
{

}

Data::GeometricSearchTreeNode::~GeometricSearchTreeNode()
{
	//this->clear();
}

void Data::GeometricSearchTreeNode::clear()
{
	if (this->isLeaf())
	{
		this->shapes.clear();

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<GeometricSearchTreeNode *>(this->child(i))->clear();
	}
}

GeometricSearchTreeNode::ShapeSet Data::GeometricSearchTreeNode::allShapes() const
{
	if (this->isLeaf())
	{
		return this->shapes;
	}

	GeometricSearchTreeNode::ShapeSet shapes;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		GeometricSearchTreeNode::ShapeSet s = static_cast<GeometricSearchTreeNode *>(this->child(i))->allShapes();

		shapes.insert(s.begin(), s.end());
	}

	return shapes;
}

#if USE_ARRAY
GeometricSearchTreeNode &Data::GeometricSearchTreeNode::operator=(const GeometricSearchTreeNode &n)
{
	if (this != &n)
	{
		TreeNode::operator=(n);

		this->shapes = n.shapes;
	}

	return *this;
}
#endif //#if USE_ARRAY
