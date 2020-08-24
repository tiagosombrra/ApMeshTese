#include "Data/BSPTree.h"

#include "Data/BSPTreeCell.h"
#include "Data/Box.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

using namespace Data;

Data::BSPTree::BSPTree(ULInt id) :
	Data::BoxTree::BoxTree(id)
{

}

Data::BSPTree::BSPTree(BSPTreeCell *root, ULInt id) :
	Data::BoxTree::BoxTree(root, id)
{

}

Data::BSPTree::BSPTree(Box *b, BSPTreeCell *root, ULInt id) :
	Data::BoxTree::BoxTree(b, root, id)
{

}

Data::BSPTree::BSPTree(Point *min, Point *max, BSPTreeCell *root, ULInt id) :
	Data::BoxTree::BoxTree(min, max, root, id)
{

}

Data::BSPTree::~BSPTree()
{

}

GraphNode *Data::BSPTree::node(ULInt id) const
{
	if (id == 0)
	{
		return this->getRoot();
	}

	UInt dimension = this->getShape()->dimension();

	UIntList positions;

	do
	{
		UInt position = static_cast<UInt>((id - 1)%(2*dimension));

		id = (id - 1 - position)/(2*dimension);

		positions.push_back(position);
	}
	while (id != 0);

	TreeNode *t = this->getRoot();

	while (!positions.empty())
	{
		UInt position = positions.back();

		positions.pop_back();

		if ((t->isLeaf()) ||
			(position/2 != static_cast<BSPTreeCell *>(t)->getSplitDirection()))
		{
			return NULL;
		}

		t = t->child(position%2);

		/*if (!t)
		{
			break;
		}*/
	}

	return t;
}

Real Data::BSPTree::size(UInt /*level*/) const
{
	return 0;
}

Data::BSPTreeCell *Data::BSPTree::commonParent(ULInt id1, ULInt id2) const
{
#if USE_EXPERIMENTAL_TREE
    return this->commonParent(id1, id2, static_cast<Data::BSPTreeCell *>(this->getRoot()));
#else
    return this->commonParent(id1, id2, static_cast<Data::BSPTreeCell *>(this->root));
#endif //#if USE_EXPERIMENTAL_TREE
}

Data::BSPTreeCell *Data::BSPTree::commonParent(ULInt id1, ULInt id2, Data::BSPTreeCell *base) const
{
    if (!base)
    {
        return NULL;
    }

    for (UInt i = 0; i < base->numChildren(); i++)
    {
        Data::BSPTreeCell *child = static_cast<Data::BSPTreeCell *>(base->child(i));

        if ((child->isDescendant(id1)) && (child->isDescendant(id2)))
        {
            return this->commonParent(id1, id2, child);
        }
    }

    return base;
}

void Data::BSPTree::children(const BoxTreeCell *c, UInt side, GraphNodeList &cells, const Data::Point *min, const Point *max, bool test) const
{
	UInt dimension = c->dimension();

	if (test)
	{
		for (UInt i = 0; i < dimension; i++)
		{
			if (i == (side%dimension))
			{
				continue;
			}

			Real tol = (c->box()->getMax()->getCoord(i) - c->box()->getMin()->getCoord(i))*Data::getTolerance();
			tol = std::min(Data::getTolerance(), tol);

			if (c->box()->getMax()->getCoord(i) < min->getCoord(i) + tol)
			{
				return;
			}

			if (c->box()->getMin()->getCoord(i) > max->getCoord(i) - tol)
			{
				return;
			}
		}
	}

	if (c->isLeaf())
	{
		cells.push_back(const_cast<BoxTreeCell *>(c));

		return;
	}

	UInt direction = static_cast<const Data::BSPTreeCell *>(c)->getSplitDirection();

	if ((side%dimension) == direction)
	{
		this->children(static_cast<BoxTreeCell *>(c->child(1 - side/dimension)), side, cells, min, max, false);

		return;
	}

	this->children(static_cast<BoxTreeCell *>(c->child(0)), side, cells, min, max);
	this->children(static_cast<BoxTreeCell *>(c->child(1)), side, cells, min, max);
}

void Data::BSPTree::neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &cells) const
{
	return this->neighbors(c, side, cells, c->box()->getMin(), c->box()->getMax());
}

void Data::BSPTree::neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &cells, const Data::Point *min, const Point *max) const
{
	if (c->isRoot())
	{
		return;
	}

	BSPTreeCell *p = static_cast<BSPTreeCell *>(c->parent());

	UInt child = c->position();

	UInt direction = p->getSplitDirection();

	UInt dimension = c->dimension();

	if (((side%dimension) == direction) &&
		((side/dimension) == child))
	{
		this->children(static_cast<BoxTreeCell *>(p->child(1 - child)), (side + dimension)%(2*dimension), cells, min, max);

		return;
	}

	this->neighbors(p, side, cells, min, max);
}

void Data::BSPTree::neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const
{
	Point2D p;

	if ((sides[0] == 0) || (sides[1] == 0))
	{
		p.setCoord(0, c->box()->getMax()->getCoord(0));
	}
	else
	{
		p.setCoord(0, c->box()->getMin()->getCoord(0));
	}

	if ((sides[0] == 1) || (sides[1] == 1))
	{
		p.setCoord(1, c->box()->getMax()->getCoord(1));
	}
	else
	{
		p.setCoord(1, c->box()->getMin()->getCoord(1));
	}

	this->neighbors2D(c, sides, cells, p);
}

void Data::BSPTree::neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point2D &pt) const
{
	if (c->isRoot())
	{
		return;
	}

	BSPTreeCell *p = static_cast<BSPTreeCell *>(c->parent());

	UInt child = c->position();

	UInt direction = p->getSplitDirection();

	if (((sides[0] == 0) && (((direction == 0) && (child == 1)) || ((direction == 1) && (child == 1)))) ||
		((sides[0] == 1) && (((direction == 0) && (child == 0)) || ((direction == 1) && (child == 1)))) ||
		((sides[0] == 2) && (((direction == 0) && (child == 0)) || ((direction == 1) && (child == 0)))) ||
		((sides[0] == 3) && (((direction == 0) && (child == 1)) || ((direction == 1) && (child == 0)))))
	{
		this->neighbors2D(p, sides, cells, pt);

		return;
	}

	Data::Point2D aux = pt;
	//Data::GraphNodeList neighs;

	switch (sides[0])
	{
	case 0:
		if (direction == 0)
		{
			aux.setCoord(0, aux.getCoord(0) + Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 1, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 1, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.front());
			}*/
		}
		else
		{
			aux.setCoord(1, aux.getCoord(1) + Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 0, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 0, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.front());
			}*/
		}
		break;
	case 1:
		if (direction == 0)
		{
			aux.setCoord(0, aux.getCoord(0) - Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 1, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 1, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.back());
			}*/
		}
		else
		{
			aux.setCoord(1, aux.getCoord(1) + Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 2, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 2, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.front());
			}*/
		}
		break;
	case 2:
		if (direction == 0)
		{
			aux.setCoord(0, aux.getCoord(0) - Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 3, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 3, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.back());
			}*/
		}
		else
		{
			aux.setCoord(1, aux.getCoord(1) - Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 2, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 2, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.back());
			}*/
		}
		break;
	case 3:
		if (direction == 0)
		{
			aux.setCoord(0, aux.getCoord(0) + Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 3, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 3, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.front());
			}*/
		}
		else
		{
			aux.setCoord(1, aux.getCoord(1) - Data::getTolerance());

			this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 0, cells, &aux, &aux);
			/*this->neighbors(static_cast<BoxTreeCell *>(p->child(1 - child)), 0, neighs);

			if (!neighs.empty())
			{
				cells.push_back(neighs.back());
			}*/
		}
		break;
	}
}

void Data::BSPTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const
{
	this->neighbors3D2(c, sides, cells, c->box()->getMin(), c->box()->getMax());
}

void Data::BSPTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point *min, const Data::Point *max, bool test) const
{
	if (c->isRoot())
	{
		return;
	}

	UInt dimension = c->dimension();

	if (test)
	{
		for (UInt i = 0; i < dimension; i++)
		{
			if ((i == (sides[0]%dimension)) || (i == (sides[1]%dimension)))
			{
				continue;
			}

			Real tol = (c->box()->getMax()->getCoord(i) - c->box()->getMin()->getCoord(i))*Data::getTolerance();
			tol = std::min(Data::getTolerance(), tol);

			if (c->box()->getMax()->getCoord(i) < min->getCoord(i) + tol)
			{
				return;
			}

			if (c->box()->getMin()->getCoord(i) > max->getCoord(i) - tol)
			{
				return;
			}
		}
	}

	BSPTreeCell *p = static_cast<BSPTreeCell *>(c->parent());

	UInt child = c->position();

	UInt direction = p->getSplitDirection();

	if (((sides[0] == 0) && (sides[1] == 1) && (((direction == 0) && (child == 1)) || ((direction == 1) && (child == 1)))) ||
		((sides[0] == 0) && (sides[1] == 2) && (((direction == 0) && (child == 1)) || ((direction == 2) && (child == 1)))) ||
		((sides[0] == 0) && (sides[1] == 4) && (((direction == 0) && (child == 1)) || ((direction == 1) && (child == 0)))) ||
		((sides[0] == 0) && (sides[1] == 5) && (((direction == 0) && (child == 1)) || ((direction == 2) && (child == 0)))) ||
		((sides[0] == 1) && (sides[1] == 2) && (((direction == 1) && (child == 1)) || ((direction == 2) && (child == 1)))) ||
		((sides[0] == 1) && (sides[1] == 3) && (((direction == 0) && (child == 0)) || ((direction == 1) && (child == 1)))) ||
		((sides[0] == 1) && (sides[1] == 5) && (((direction == 1) && (child == 1)) || ((direction == 2) && (child == 0)))) ||
		((sides[0] == 2) && (sides[1] == 3) && (((direction == 0) && (child == 0)) || ((direction == 2) && (child == 1)))) ||
		((sides[0] == 2) && (sides[1] == 4) && (((direction == 1) && (child == 0)) || ((direction == 2) && (child == 1)))) ||
		((sides[0] == 3) && (sides[1] == 4) && (((direction == 0) && (child == 0)) || ((direction == 1) && (child == 0)))) ||
		((sides[0] == 3) && (sides[1] == 5) && (((direction == 0) && (child == 0)) || ((direction == 2) && (child == 0)))) ||
		((sides[0] == 4) && (sides[1] == 5) && (((direction == 1) && (child == 0)) || ((direction == 2) && (child == 0)))))
	{
		this->neighbors3D2(p, sides, cells, min, max, false);

		return;
	}

	if (((sides[0] == 0) && (sides[1] == 1) && (direction == 2)) ||
		((sides[0] == 0) && (sides[1] == 2) && (direction == 1)) ||
		((sides[0] == 0) && (sides[1] == 4) && (direction == 2)) ||
		((sides[0] == 0) && (sides[1] == 5) && (direction == 1)) ||
		((sides[0] == 1) && (sides[1] == 2) && (direction == 0)) ||
		((sides[0] == 1) && (sides[1] == 3) && (direction == 2)) ||
		((sides[0] == 1) && (sides[1] == 5) && (direction == 0)) ||
		((sides[0] == 2) && (sides[1] == 3) && (direction == 1)) ||
		((sides[0] == 2) && (sides[1] == 4) && (direction == 0)) ||
		((sides[0] == 3) && (sides[1] == 4) && (direction == 2)) ||
		((sides[0] == 3) && (sides[1] == 5) && (direction == 1)) ||
		((sides[0] == 4) && (sides[1] == 5) && (direction == 0)))
	{
		this->neighbors3D2(p, sides, cells, min, max);

		return;
	}

	Data::Point3D min2 = *min;
	Data::Point3D max2 = *max;

	UInt dir = ((sides[0]%3) == direction) ? sides[1] : sides[0];

	if (child == 0)
	{
		max2.setCoord(direction, max2.getCoord(direction) + Data::getTolerance());
		min2.setCoord(direction, max2.getCoord(direction));
	}
	else
	{
		min2.setCoord(direction, min2.getCoord(direction) - Data::getTolerance());
		max2.setCoord(direction, min2.getCoord(direction));
	}

	this->neighbors(static_cast<BSPTreeCell *>(p->child(1 - child)), dir, cells, &min2, &max2);
}

void Data::BSPTree::neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells) const
{
	Point3D p;

	if ((sides[0] == 0) || (sides[1] == 0) || (sides[2] == 0))
	{
		p.setCoord(0, c->box()->getMax()->getCoord(0));
	}
	else
	{
		p.setCoord(0, c->box()->getMin()->getCoord(0));
	}

	if ((sides[0] == 1) || (sides[1] == 1) || (sides[2] == 1))
	{
		p.setCoord(1, c->box()->getMax()->getCoord(1));
	}
	else
	{
		p.setCoord(1, c->box()->getMin()->getCoord(1));
	}

	if ((sides[0] == 2) || (sides[1] == 2) || (sides[2] == 2))
	{
		p.setCoord(2, c->box()->getMax()->getCoord(2));
	}
	else
	{
		p.setCoord(2, c->box()->getMin()->getCoord(2));
	}

	this->neighbors3D3(c, sides, cells, p);
}

void Data::BSPTree::neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &cells, const Data::Point3D &pt) const
{
	if (c->isRoot())
	{
		return;
	}

	UInt dimension = this->getShape()->dimension();

	BSPTreeCell *p = static_cast<BSPTreeCell *>(c->parent());

	UInt child = c->position();

	UInt direction = p->getSplitDirection();

	UInt ind = 3;

	for (UInt i = 0; i < 3; i++)
	{
		if ((sides[i]%dimension) == direction)
		{
			if ((sides[i]/dimension) == child)
			{
				ind = i;
			}

			break;
		}
	}

	if (ind == 3)
	{
		this->neighbors3D3(p, sides, cells, pt);

		return;
	}

	UIntVector sds(2);

	for (UInt i = 0, j = 0; i < 3; i++)
	{
		if (i == ind)
		{
			continue;
		}

		sds[j++] = sides[i];
	}

	Data::Point3D min(pt), max(pt);

	BoxTreeCell *sib = static_cast<BoxTreeCell *>(p->child(1 - child));

	Real tol = (sib->box()->getMax()->getCoord(ind) - sib->box()->getMin()->getCoord(ind))*Data::getTolerance();
	tol = std::min(Data::getTolerance(), tol);

	if (sides[ind] < dimension)
	{
		max.setCoord(sides[ind]%dimension, max.getCoord(sides[ind]%dimension) + tol);
	}
	else
	{
		min.setCoord(sides[ind]%dimension, min.getCoord(sides[ind]%dimension) - tol);
	}

	this->neighbors3D2(sib, sds, cells, &min, &max);
}
