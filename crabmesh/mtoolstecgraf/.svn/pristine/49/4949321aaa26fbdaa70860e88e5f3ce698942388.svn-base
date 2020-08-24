#include "Data/RecursiveDecompositionTree.h"

#include "Data/GeometricShape.h"
#include "Data/RecursiveDecompositionTreeCell.h"
#include "Data/ListDisplayable.h"
#include "Data/GeometricShape.h"
#include "Data/RecursiveDecompositionTreeCell.h"

using namespace Data;

Data::RecursiveDecompositionTree::RecursiveDecompositionTree(GeometricShape *shape,
	RecursiveDecompositionTreeCell *root, ULInt id) :
	Data::Tree(root, id)
#if USE_GUI
	, Data::ListDisplayable()
#endif //#if USE_GUI
{
	this->setShape(shape);
}

Data::RecursiveDecompositionTree::~RecursiveDecompositionTree()
{
	if (this->shape)
	{
		delete this->shape;
	}
}

void Data::RecursiveDecompositionTree::setShape(GeometricShape *shape)
{
	this->shape = shape;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

GeometricShape *Data::RecursiveDecompositionTree::getShape() const
{
	return this->shape;
}

#if USE_GUI
void Data::RecursiveDecompositionTree::setNodes(const GraphNodeList &nodes)
{
	Data::Tree::setNodes(nodes);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::RecursiveDecompositionTree::add(GraphNode *n)
{
	Data::Tree::add(n);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::RecursiveDecompositionTree::remove(GraphNode *n)
{
	Data::Tree::remove(n);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::RecursiveDecompositionTree::setR(Real r) const
{
	if (this->shape)
	{
		this->shape->setR(r);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

Real Data::RecursiveDecompositionTree::getR() const
{
	return (this->shape) ? this->shape->getR() : 0.0;
}

void Data::RecursiveDecompositionTree::setG(Real g) const
{
	if (this->shape)
	{
		this->shape->setG(g);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

Real Data::RecursiveDecompositionTree::getG() const
{
	return (this->shape) ? this->shape->getG() : 0.0;
}

void Data::RecursiveDecompositionTree::setB(Real b) const
{
	if (this->shape)
	{
		this->shape->setB(b);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

Real Data::RecursiveDecompositionTree::getB() const
{
	return (this->shape) ? this->shape->getB() : 0.0;
}

void Data::RecursiveDecompositionTree::highlight() const
{
	if (this->shape)
	{
		this->shape->highlight();

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI
	}
}

void Data::RecursiveDecompositionTree::unhighlight() const
{
	if (this->shape)
	{
		this->shape->unhighlight();

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI
	}
}

bool Data::RecursiveDecompositionTree::isHighlighted() const
{
	return (this->shape) ? this->shape->isHighlighted() : false;
}

void Data::RecursiveDecompositionTree::draw() const
{
	if (this->shape)
	{
		this->shape->draw();
	}
}

std::string Data::RecursiveDecompositionTree::text() const
{
	return (this->shape) ? this->shape->text() : "";
}
#endif //#if USE_GUI
