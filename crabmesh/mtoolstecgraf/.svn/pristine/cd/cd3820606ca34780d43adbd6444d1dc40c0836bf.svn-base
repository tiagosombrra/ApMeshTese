#include "Data/BinTree.h"

#include "Data/BinTreeCell.h"
#include "Data/GeometricShape.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

Data::BinTree::BinTree(const Point &min, const Point &max, ULInt id) :
#if USE_EXPERIMENTAL_3
	BoxTree(new Point2D(min), new Point2D(max), new BinTreeCell(), id)
#else
	BoxTree(new Point(min), new Point(max), new BinTreeCell(), id)
#endif //#if USE_EXPERIMENTAL_3
{

}

Data::BinTree::BinTree(Real min, Real max, ULInt id) :
	BoxTree(new BinTreeCell(), id)
{
	this->getShape()->getPoint(0)->setCoord(0, min);
	this->getShape()->getPoint(1)->setCoord(0, max);
#if USE_EXPERIMENTAL_3
    this->getShape()->getPoint(0)->setCoord(1, 0.0);
	this->getShape()->getPoint(1)->setCoord(1, 0.0);
#endif //#if USE_EXPERIMENTAL_3
}

Data::BinTree::~BinTree()
{

}

GraphNode *Data::BinTree::node(ULInt id) const
{
	if (id == 0)
	{
		return this->getRoot();
	}

	UIntVector positions;

	do
	{
		UInt position = static_cast<UInt>((id - 1)%2);

		id = (id - 1 - static_cast<ULInt>(position))/2;

		positions.push_back(position);
	}
	while (id != 0);

	TreeNode *t = this->getRoot();

	while (!positions.empty())
	{
		UInt position = positions.back();

		positions.pop_back();

		t = t->child(position);

		if (!t)
		{
			break;
		}
	}

	return t;
}

bool Data::BinTree::in(Real x) const
{
///TODO class Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->in(p);
}

bool Data::BinTree::optIn(Real x) const
{
///TODO class Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optIn(p);
}

bool Data::BinTree::on(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->on(p);
}

bool Data::BinTree::optOn(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optOn(p);
}

bool Data::BinTree::out(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->out(p);
}

bool Data::BinTree::optOut(Real x) const
{
///TODO Point1D
#if USE_EXPERIMENTAL_3
	Point2D p;
#else
	Point p(1);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);

	return this->optOut(p);
}
