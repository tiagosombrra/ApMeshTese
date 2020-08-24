#include "Data/OctTree.h"

using namespace Data;

Data::OctTree::OctTree(const Point3D &min, const Point3D &max, ULInt id) :
	BoxTree(new Point3D(min), new Point3D(max), new OctTreeCell(), id)
{

}

Data::OctTree::OctTree(Box *box, OctTreeCell *root, ULInt id) :
	BoxTree(box, root, id)
{

}

Data::OctTree::~OctTree()
{

}

bool Data::OctTree::in(Real x, Real y, Real z) const
{
	return this->in(Point3D(x, y, z));
}

bool Data::OctTree::optIn(Real x, Real y, Real z) const
{
	return this->optIn(Point3D(x, y, z));
}

bool Data::OctTree::on(Real x, Real y, Real z) const
{
	return this->on(Point3D(x, y, z));
}

bool Data::OctTree::optOn(Real x, Real y, Real z) const
{
	return this->optOn(Point3D(x, y, z));
}

bool Data::OctTree::out(Real x, Real y, Real z) const
{
	return this->out(Point3D(x, y, z));
}

bool Data::OctTree::optOut(Real x, Real y, Real z) const
{
	return this->optOut(Point3D(x, y, z));
}

GraphNode *Data::OctTree::node(ULInt id) const
{
	if (id == 0)
	{
		return this->getRoot();
	}

	UIntList positions;

	do
	{
		UInt position = static_cast<UInt>((id - 1)%8);

		id = (id - 1 - static_cast<ULInt>(position))/8;

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

/*GraphNodeVector Data::OctTree::neighbors(const BoxTreeCell *c) const
{
	OctTreeCell *octc = dynamic_cast<OctTreeCell *>(const_cast<BoxTreeCell *>(c));

	return this->neighbors(octc);
}*/

/*GraphNodeVector Data::OctTree::neighbors(const BoxTreeCell *c) const
{
	GraphNodeVector neighbors;

	if (!c)
	{
		return neighbors;
	}

	GraphNodeVector leaves = this->getLeaves();

	Point *cmin = c->getMin();
	Point *cmax = c->getMax();

	while (!leaves.empty())
	{
		OctTreeCell *l = (OctTreeCell *)leaves.back();
		leaves.pop_back();

		Point *lmin = l->getMin();
		Point *lmax = l->getMax();

		UInt dimension = 3;

		UInt hits = dimension;

		for (UInt i = 0; i < dimension; i++)
		{
			if ((std::fabs(lmax->getCoord(i) - cmin->getCoord(i)) < Data::getTolerance()) ||
				(std::fabs(lmin->getCoord(i) - cmax->getCoord(i)) < Data::getTolerance()))
			{
				hits = i;

				break;
			}
		}

		if (hits == dimension)
		{
			continue;
		}

		for (UInt i = 0; i < dimension; i++)
		{
			if (i == hits)
			{
				continue;
			}

			if ((cmin->getCoord(i) - lmax->getCoord(i) >= Data::getTolerance()) ||
				(cmax->getCoord(i) - lmin->getCoord(i) <= Data::getTolerance()))
			{
				hits = dimension;

				break;
			}
		}

		if (hits == dimension)
		{
			continue;
		}

		neighbors.push_back(l);
	}

	return neighbors;
}*/
