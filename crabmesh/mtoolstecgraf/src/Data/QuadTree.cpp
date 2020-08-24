#include "Data/QuadTree.h"

using namespace Data;

Data::QuadTree::QuadTree(const Point2D &min, const Point2D &max, ULInt id) :
	BoxTree(new Point2D(min), new Point2D(max), new QuadTreeCell(), id)
{

}

Data::QuadTree::QuadTree(Box *box, QuadTreeCell *root, ULInt id) :
	BoxTree(box, root, id)
{

}

Data::QuadTree::~QuadTree()
{

}

bool Data::QuadTree::in(Real x, Real y) const
{
	return this->in(Point2D(x, y));
}

bool Data::QuadTree::optIn(Real x, Real y) const
{
	return this->optIn(Point2D(x, y));
}

bool Data::QuadTree::on(Real x, Real y) const
{
	return this->on(Point2D(x, y));
}

bool Data::QuadTree::optOn(Real x, Real y) const
{
	return this->optOn(Point2D(x, y));
}

bool Data::QuadTree::out(Real x, Real y) const
{
	return this->out(Point2D(x, y));
}

bool Data::QuadTree::optOut(Real x, Real y) const
{
	return this->optOut(Point2D(x, y));
}

GraphNode *Data::QuadTree::node(ULInt id) const
{
	if (id == 0)
	{
		return this->getRoot();
	}

	UIntList positions;

	do
	{
		UInt position = static_cast<UInt>((id - 1)%4);

		id = (id - 1 - static_cast<ULInt>(position))/4;

		positions.push_back(position);
	}
	while (id != 0);

	TreeNode *t = this->getRoot();

	while (!positions.empty())
	{
		UInt position = positions.back();

		positions.pop_back();

		if (t->isLeaf())
		{
			return NULL;
		}

		t = t->child(position);

		/*if (!t)
		{
			break;
		}*/
	}

	return t;
}
