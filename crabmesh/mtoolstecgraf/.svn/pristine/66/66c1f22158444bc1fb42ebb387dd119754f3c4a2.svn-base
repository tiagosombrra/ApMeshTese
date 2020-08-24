#include "Data/RecursiveDecompositionTreeCell.h"

#if USE_EXPERIMENTAL
#include "Data/RecursiveDecompositionTree.h"
#endif //#if USE_EXPERIMENTAL

using namespace Data;

#if USE_EXPERIMENTAL
Data::RecursiveDecompositionTreeCell::RecursiveDecompositionTreeCell(ULInt id) :
	TreeNode(id)
#if USE_GUI
	, Drawable()
#endif //#if USE_GUI
{
	this->shp = NULL;
}

Data::RecursiveDecompositionTreeCell::RecursiveDecompositionTreeCell(
	RecursiveDecompositionTree *tree, ULInt id) :
	TreeNode(tree, id)
#if USE_GUI
	, Drawable()
#endif //#if USE_GUI
{
	this->shp = NULL;
}
#endif //#if USE_EXPERIMENTAL

Data::RecursiveDecompositionTreeCell::RecursiveDecompositionTreeCell(
	RecursiveDecompositionTreeCell *parent, ULInt id) :
	TreeNode(parent, id)
#if USE_GUI
	, Drawable()
#endif //#if USE_GUI
{
	this->shp = NULL;
}

Data::RecursiveDecompositionTreeCell::~RecursiveDecompositionTreeCell()
{
	if (this->shp)
	{
		delete this->shp;
	}
}

bool Data::RecursiveDecompositionTreeCell::isSubdivided() const
{
	return (!this->isLeaf());
}

GeometricShape *Data::RecursiveDecompositionTreeCell::shape() const
{
	return (this->shp) ? this->shp : (this->shp = this->makeShape(), this->shp);

	/*if (!this->shp)
	{
		this->shp = this->makeShape();
	}

	return this->shp;*/
}

#if USE_ARRAY
RecursiveDecompositionTreeCell &Data::RecursiveDecompositionTreeCell::operator=(const RecursiveDecompositionTreeCell &c)
{
	if (this != &c)
	{
		TreeNode::operator=(c);

		this->shp = (c.shp) ? this->makeShape() : NULL;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
void Data::RecursiveDecompositionTreeCell::setR(Real r) const
{
	this->shape()->setR(r);

	/*return;

	if (this->shp)
	{
		this->shp->setR(r);
	}*/
}

Real Data::RecursiveDecompositionTreeCell::getR() const
{
	return this->shape()->getR();

	//return (this->shp) ? this->shp->getR() : 0.0;
}

void Data::RecursiveDecompositionTreeCell::setG(Real g) const
{
	this->shape()->setG(g);

	/*return;

	if (this->shp)
	{
		this->shp->setG(g);
	}*/
}

Real Data::RecursiveDecompositionTreeCell::getG() const
{
	return this->shape()->getG();

	//return (this->shp) ? this->shp->getG() : 0.0;
}

void Data::RecursiveDecompositionTreeCell::setB(Real b) const
{
	this->shape()->setB(b);

	/*return;

	if (this->shp)
	{
		this->shp->setB(b);
	}*/
}

Real Data::RecursiveDecompositionTreeCell::getB() const
{
	return this->shape()->getB();

	//return (this->shp) ? this->shp->getB() : 0.0;
}

void Data::RecursiveDecompositionTreeCell::highlight() const
{
	this->shape()->highlight();

	/*return;

	if (this->shp)
	{
		this->shp->highlight();
	}*/
}

void Data::RecursiveDecompositionTreeCell::unhighlight() const
{
	this->shape()->unhighlight();

	/*return;

	if (this->shp)
	{
		this->shp->unhighlight();
	}*/
}

bool Data::RecursiveDecompositionTreeCell::isHighlighted() const
{
	return this->shape()->isHighlighted();
}

void Data::RecursiveDecompositionTreeCell::draw() const
{
	if (this->isLeaf())
	{
		this->shape()->draw();
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		this->child(i)->draw();
	}
}
#endif //#if USE_GUI
