#include "Data/BoxTree.h"

#include "Data/State.h"
#include "Data/BoxTreeCell.h"
#include "Data/Point.h"
#include "Data/Box.h"
#include "Data/RecursiveDecompositionTreeCell.h"
#include "Data/BoxTreeCell.h"

using namespace Data;

Data::BoxTree::BoxTree(ULInt id) :
	Data::RecursiveDecompositionTree(new Box(), NULL, id)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::UNKNOWN, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::BoxTree::BoxTree(BoxTreeCell *root, ULInt id) :
	Data::RecursiveDecompositionTree(new Box(), root, id)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::UNKNOWN, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::BoxTree::BoxTree(Box *box, BoxTreeCell *root, ULInt id) :
	Data::RecursiveDecompositionTree(box, root, id)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::UNKNOWN, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::BoxTree::BoxTree(Point *min, Point *max, BoxTreeCell *root, ULInt id) :
	Data::RecursiveDecompositionTree(new Box(min, max), root, id)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::UNKNOWN, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::BoxTree::~BoxTree()
{

}

bool Data::BoxTree::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::BoxTree::in(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->in(p) : false;
}

bool Data::BoxTree::optIn(const Point &p) const
{
	return this->optIn(&p);
}

bool Data::BoxTree::optIn(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->optIn(p) : false;
}

bool Data::BoxTree::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::BoxTree::on(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->on(p) : false;
}

bool Data::BoxTree::optOn(const Point &p) const
{
	return this->optOn(&p);
}

bool Data::BoxTree::optOn(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->optOn(p) : false;
}

bool Data::BoxTree::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::BoxTree::out(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->out(p) : false;
}

bool Data::BoxTree::optOut(const Point &p) const
{
	return this->optOut(&p);
}

bool Data::BoxTree::optOut(const Point *p) const
{
	return this->shape ? static_cast<Box *>(this->shape)->optOut(p) : false;
}

Real Data::BoxTree::size() const
{
	return this->shape ? static_cast<Box *>(this->shape)->size() : 0.0;
}

Real Data::BoxTree::size(UInt level) const
{
	return this->size()/std::pow(2.0, static_cast<Real>(level));
}

BoxTreeCell *Data::BoxTree::find(const Point &p) const
{
	return this->find(&p);
}

BoxTreeCell *Data::BoxTree::find(const Point *p) const
{
#if USE_EXPERIMENTAL_TREE
	return static_cast<BoxTreeCell *>(this->getRoot() ? static_cast<RecursiveDecompositionTreeCell *>(this->getRoot())->find(p) : NULL);
#else
	return static_cast<BoxTreeCell *>(this->root ? static_cast<RecursiveDecompositionTreeCell *>(this->root)->find(p) : NULL);
#endif //#if USE_EXPERIMENTAL_TREE
}

void Data::BoxTree::neighbors(const BoxTreeCell *c, GraphNodeList &neighs) const
{
	for (UInt i = 0; i < c->numSides(); i++)
	{
		this->neighbors(c, i, neighs);
	}
}

GraphNodeList Data::BoxTree::neighbors(const BoxTreeCell *c) const
{
	GraphNodeList neighs;

	this->neighbors(c, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const ULInt id, GraphNodeList &neighs) const
{
	BoxTreeCell *c = static_cast<BoxTreeCell *>(this->node(id));

	if (c)
	{
		this->neighbors(c, neighs);
	}
}

GraphNodeList Data::BoxTree::neighbors(const ULInt id) const
{
	GraphNodeList neighs;

	this->neighbors(id, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const BoxTreeCell *c, UInt side, GraphNodeList &neighs) const
{
	UIntList stack;

	this->neighbors(c, side, stack, neighs);
}

GraphNodeList Data::BoxTree::neighbors(const BoxTreeCell *c, UInt side) const
{
	GraphNodeList neighs;

	this->neighbors(c, side, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const ULInt id, UInt side, GraphNodeList &neighs) const
{
	BoxTreeCell *c = static_cast<BoxTreeCell *>(this->node(id));

	if (c)
	{
		this->neighbors(c, side, neighs);
	}
}

GraphNodeList Data::BoxTree::neighbors(const ULInt id, UInt side) const
{
	GraphNodeList neighs;

	this->neighbors(id, side, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &neighs) const
{
	UIntVector sds = sides;

	std::sort(sds.begin(), sds.end());
	UIntVector::iterator it = std::unique(sds.begin(), sds.end());
	sds.resize(it - sds.begin());

	if ((sds.empty()) || (sds.size() > c->dimension()))
	{
		return;
	}

	if (sds.size() == 1)
	{
		this->neighbors(c, sds[0], neighs);

		return;
	}

	if (c->dimension() == 2)
	{
		if ((sds[0] > 2) || (sds[1] > 3) || (sds[1] == sds[0] + 2))
		{
			return;
		}

		if ((sds[0] == 0) && (sds[1] == 3))
		{
			sds[0] = 3;
			sds[1] = 0;
		}

		this->neighbors2D(c, sds, neighs);

		return;
	}

	if (c->dimension() == 3)
	{
		if (sds.size() == 2)
		{
			this->neighbors3D2(c, sds, neighs);

			return;
		}

		if (sds.size() == 3)
		{
			this->neighbors3D3(c, sds, neighs);

			return;
		}
	}
}

GraphNodeList Data::BoxTree::neighbors(const BoxTreeCell *c, const UIntVector &sides) const
{
	GraphNodeList neighs;

	this->neighbors(c, sides, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const ULInt id, const UIntVector &sides, GraphNodeList &neighs) const
{
	BoxTreeCell *c = static_cast<BoxTreeCell *>(this->node(id));

	if (c)
	{
		this->neighbors(c, sides, neighs);
	}
}

GraphNodeList Data::BoxTree::neighbors(const ULInt id, const UIntVector &sides) const
{
	GraphNodeList neighs;

	this->neighbors(id, sides, neighs);

	return neighs;
}

BoxTreeCell *Data::BoxTree::neighbor(const BoxTreeCell *c, UInt side) const
{
	UIntList stack;

	return this->neighbor(c, side, stack);
}

BoxTreeCell *Data::BoxTree::neighbor(const BoxTreeCell *c, UInt side, UIntList &stack) const
{
	if ((c->isRoot()) || (side > c->numSides()))
	{
		return NULL;
	}

	BoxTreeCell *p = static_cast<BoxTreeCell *>(c->parent());

	UInt child = c->position();

	if (c->dimension() == 1)
	{
		if (child != side)
		{
			stack.push_back(child);

			return this->neighbor(p, side, stack);
		}

		for (UIntList::iterator iter = stack.begin();
			 iter != stack.end(); iter++)
		{
			(*iter) = 1 - (*iter);
		}

		BoxTreeCell *sibling = static_cast<BoxTreeCell *>(p->child(1 - child));

		return this->child(sibling, stack);
	}

	if (c->dimension() == 2)
	{
		if ((side == (child + 2)%4) || (side == (child + 3)%4))
		{
			stack.push_back(child);

			return this->neighbor(p, side, stack);
		}

		UInt sib = (side == child) ? (side + 1)%4 : (side + 2)%4;

		BoxTreeCell *sibling = static_cast<BoxTreeCell *>(p->child(sib));

		for (UIntList::iterator iter = stack.begin();
			 iter != stack.end(); iter++)
		{
			(*iter) = ((side + 1)%4 == (*iter)) ? side : (side + 3)%4;
		}

		return this->child(sibling, stack);
	}

	if (c->dimension() == 3)
	{
		//OMG, what a horrible code! I really hope this works!
		//YEP, it works!

		if (((child == 0) && ((side == 3) || (side == 4) || (side == 5))) ||
			((child == 1) && ((side == 4) || (side == 5) || (side == 0))) ||
			((child == 2) && ((side == 5) || (side == 0) || (side == 1))) ||
			((child == 3) && ((side == 1) || (side == 3) || (side == 5))) ||
			((child == 4) && ((side == 2) || (side == 3) || (side == 4))) ||
			((child == 5) && ((side == 0) || (side == 2) || (side == 4))) ||
			((child == 6) && ((side == 0) || (side == 1) || (side == 2))) ||
			((child == 7) && ((side == 1) || (side == 2) || (side == 3))))
		{
			stack.push_back(child);

			return this->neighbor(p, side, stack);
		}

		BoxTreeCell *sibling = NULL;

		switch (child)
		{
		case 0:
			switch (side)
			{
			case 0:
				sibling = static_cast<BoxTreeCell *>(p->child(1));
				break;
			case 1:
				sibling = static_cast<BoxTreeCell *>(p->child(3));
				break;
			case 2:
				sibling = static_cast<BoxTreeCell *>(p->child(4));
				break;
			}
			break;
		case 1:
			switch (side)
			{
			case 1:
				sibling = static_cast<BoxTreeCell *>(p->child(2));
				break;
			case 2:
				sibling = static_cast<BoxTreeCell *>(p->child(5));
				break;
			case 3:
				sibling = static_cast<BoxTreeCell *>(p->child(0));
				break;
			}
			break;
		case 2:
			switch (side)
			{
			case 2:
				sibling = static_cast<BoxTreeCell *>(p->child(6));
				break;
			case 3:
				sibling = static_cast<BoxTreeCell *>(p->child(3));
				break;
			case 4:
				sibling = static_cast<BoxTreeCell *>(p->child(1));
				break;
			}
			break;
		case 3:
			switch (side)
			{
			case 2:
				sibling = static_cast<BoxTreeCell *>(p->child(7));
				break;
			case 4:
				sibling = static_cast<BoxTreeCell *>(p->child(0));
				break;
			case 0:
				sibling = static_cast<BoxTreeCell *>(p->child(2));
				break;
			}
			break;
		case 4:
			switch (side)
			{
			case 1:
				sibling = static_cast<BoxTreeCell *>(p->child(7));
				break;
			case 5:
				sibling = static_cast<BoxTreeCell *>(p->child(0));
				break;
			case 0:
				sibling = static_cast<BoxTreeCell *>(p->child(5));
				break;
			}
			break;
		case 5:
			switch (side)
			{
			case 1:
				sibling = static_cast<BoxTreeCell *>(p->child(6));
				break;
			case 3:
				sibling = static_cast<BoxTreeCell *>(p->child(4));
				break;
			case 5:
				sibling = static_cast<BoxTreeCell *>(p->child(1));
				break;
			}
			break;
		case 6:
			switch (side)
			{
			case 3:
				sibling = static_cast<BoxTreeCell *>(p->child(7));
				break;
			case 4:
				sibling = static_cast<BoxTreeCell *>(p->child(5));
				break;
			case 5:
				sibling = static_cast<BoxTreeCell *>(p->child(2));
				break;
			}
			break;
		case 7:
			switch (side)
			{
			case 4:
				sibling = static_cast<BoxTreeCell *>(p->child(4));
				break;
			case 5:
				sibling = static_cast<BoxTreeCell *>(p->child(3));
				break;
			case 0:
				sibling = static_cast<BoxTreeCell *>(p->child(6));
				break;
			}
			break;
		}

		if ((side == 2) || (side == 5))
		{
			for (UIntList::iterator iter = stack.begin();
				 iter != stack.end(); iter++)
			{
				(*iter) = ((*iter) + 4)%8;
			}
		}
		else
		{
			if ((side == 3) || (side == 4))
			{
				side--;
			}

			for (UIntList::iterator iter = stack.begin();
				 iter != stack.end(); iter++)
			{
				UInt complement = ((*iter) > 3) ? 4 : 0;

				(*iter) = complement + (((side + 1)%4 == (*iter)%4) ? side : (side + 3)%4);
			}
		}

		return this->child(sibling, stack);
	}

	return NULL;
}

BoxTreeCell *Data::BoxTree::neighbor3D2(const BoxTreeCell *c, const UIntVector &sides) const
{
	UIntList stack;

	return this->neighbor3D2(c, sides, stack);
}

BoxTreeCell *Data::BoxTree::neighbor3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack) const
{
	if (c->isRoot())
	{
		return NULL;
	}

	UIntVector callRec(2), callSib(4), callNeigh(2), children(2);

	if (!this->buildTableNeighbor3D2(sides, callRec, callSib, callNeigh, children))
	{
		return NULL;
	}

	BoxTreeCell *p = static_cast<BoxTreeCell *>(c->parent());

	UInt child = c->position();

	if ((child == callRec[0]) || (child == callRec[1]))
	{
		stack.push_back(child);

		return this->neighbor3D2(p, sides, stack);
	}

	BoxTreeCell *s = static_cast<BoxTreeCell *>(p->child(
		((child == callSib[0]) || (child == callSib[1]) || (child == callSib[2]) || (child == callSib[3])) ? callRec[0] : callRec[1]));

	UInt side =  ((child == callNeigh[0]) || (child == callNeigh[1])) ? 0 :
				(((child == callNeigh[2]) || (child == callNeigh[3])) ? 1 : 2);

	if (side < 2)
	{
		s = this->neighbor(s, sides[side]);

		if ((!s) || (s->level() < c->level()))
		{
			return NULL;
		}
	}

	for (UIntList::iterator iter = stack.begin();
		 iter != stack.end(); iter++)
	{
		(*iter) = ((*iter) == callRec[0]) ? children[0] : children[1];
	}

	return this->child(s, stack);
}

bool Data::BoxTree::buildTableNeighbor3D2(const UIntVector &sides, UIntVector &callRec, UIntVector &callSib, UIntVector &callNeigh, UIntVector &children) const
{
	if ((sides[0] == 0) && (sides[1] == 1))
	{
		callRec  [0] = 2; callRec  [1] = 6;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 2; callSib  [3] = 3;
		callNeigh[0] = 1; callNeigh[1] = 5; callNeigh[2] = 3; callNeigh[3] = 7;
		children [0] = 0; children [1] = 4;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 2))
	{
		callRec  [0] = 5; callRec  [1] = 6;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 4; callSib  [3] = 5;
		callNeigh[0] = 1; callNeigh[1] = 2; callNeigh[2] = 4; callNeigh[3] = 7;
		children [0] = 0; children [1] = 3;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 4))
	{
		callRec  [0] = 1; callRec  [1] = 5;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 2; callSib  [3] = 3;
		callNeigh[0] = 2; callNeigh[1] = 6; callNeigh[2] = 0; callNeigh[3] = 4;
		children [0] = 3; children [1] = 7;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 5))
	{
		callRec  [0] = 1; callRec  [1] = 2;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 4; callSib  [3] = 5;
		callNeigh[0] = 5; callNeigh[1] = 6; callNeigh[2] = 0; callNeigh[3] = 3;
		children [0] = 4; children [1] = 7;

		return true;
	}

	if ((sides[0] == 1) && (sides[1] == 2))
	{
		callRec  [0] = 6; callRec  [1] = 7;
		callSib  [0] = 1; callSib  [1] = 2; callSib  [2] = 5; callSib  [3] = 6;
		callNeigh[0] = 2; callNeigh[1] = 3; callNeigh[2] = 4; callNeigh[3] = 5;
		children [0] = 1; children [1] = 0;

		return true;
	}

	if ((sides[0] == 1) && (sides[1] == 3))
	{
		callRec  [0] = 3; callRec  [1] = 7;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 2; callSib  [3] = 3;
		callNeigh[0] = 2; callNeigh[1] = 6; callNeigh[2] = 0; callNeigh[3] = 4;
		children [0] = 1; children [1] = 5;

		return true;
	}

	if ((sides[0] == 1) && (sides[1] == 5))
	{
		callRec  [0] = 2; callRec  [1] = 3;
		callSib  [0] = 1; callSib  [1] = 2; callSib  [2] = 5; callSib  [3] = 6;
		callNeigh[0] = 6; callNeigh[1] = 7; callNeigh[2] = 0; callNeigh[3] = 1;
		children [0] = 5; children [1] = 4;

		return true;
	}

	if ((sides[0] == 2) && (sides[1] == 3))
	{
		callRec  [0] = 4; callRec  [1] = 7;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 4; callSib  [3] = 5;
		callNeigh[0] = 5; callNeigh[1] = 6; callNeigh[2] = 0; callNeigh[3] = 3;
		children [0] = 1; children [1] = 2;

		return true;
	}

	if ((sides[0] == 2) && (sides[1] == 4))
	{
		callRec  [0] = 4; callRec  [1] = 5;
		callSib  [0] = 0; callSib  [1] = 3; callSib  [2] = 4; callSib  [3] = 7;
		callNeigh[0] = 6; callNeigh[1] = 7; callNeigh[2] = 0; callNeigh[3] = 1;
		children [0] = 3; children [1] = 2;

		return true;
	}

	if ((sides[0] == 3) && (sides[1] == 4))
	{
		callRec  [0] = 0; callRec  [1] = 4;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 2; callSib  [3] = 3;
		callNeigh[0] = 3; callNeigh[1] = 7; callNeigh[2] = 1; callNeigh[3] = 5;
		children [0] = 2; children [1] = 6;

		return true;
	}

	if ((sides[0] == 3) && (sides[1] == 5))
	{
		callRec  [0] = 0; callRec  [1] = 3;
		callSib  [0] = 0; callSib  [1] = 1; callSib  [2] = 4; callSib  [3] = 5;
		callNeigh[0] = 4; callNeigh[1] = 7; callNeigh[2] = 1; callNeigh[3] = 2;
		children [0] = 5; children [1] = 6;

		return true;
	}

	if ((sides[0] == 4) && (sides[1] == 5))
	{
		callRec  [0] = 0; callRec  [1] = 1;
		callSib  [0] = 0; callSib  [1] = 3; callSib  [2] = 4; callSib  [3] = 7;
		callNeigh[0] = 4; callNeigh[1] = 5; callNeigh[2] = 2; callNeigh[3] = 3;
		children [0] = 7; children [1] = 6;

		return true;
	}

	return false;
}

bool Data::BoxTree::buildTableNeighbor3D3(const UIntVector &sides, UInt &callRec, UIntVector &callNeigh, UIntVector &callNeigh2, UInt &child) const
{
	if ((sides[0] == 0) && (sides[1] == 1) && (sides[2] == 2))
	{
		callRec       = 6;
		callNeigh [0] = 1; callNeigh [1] = 3; callNeigh [2] = 4;
		callNeigh2[0] = 2; callNeigh2[1] = 5; callNeigh2[2] = 7;
		child         = 0;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 1) && (sides[2] == 5))
	{
		callRec       = 2;
		callNeigh [0] = 5; callNeigh [1] = 7; callNeigh [2] = 0;
		callNeigh2[0] = 6; callNeigh2[1] = 1; callNeigh2[2] = 3;
		child         = 4;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 2) && (sides[2] == 4))
	{
		callRec       = 5;
		callNeigh [0] = 2; callNeigh [1] = 7; callNeigh [2] = 0;
		callNeigh2[0] = 6; callNeigh2[1] = 1; callNeigh2[2] = 4;
		child         = 3;

		return true;
	}

	if ((sides[0] == 0) && (sides[1] == 4) && (sides[2] == 5))
	{
		callRec       = 1;
		callNeigh [0] = 6; callNeigh [1] = 4; callNeigh [2] = 3;
		callNeigh2[0] = 5; callNeigh2[1] = 2; callNeigh2[2] = 0;
		child         = 7;

		return true;
	}

	if ((sides[0] == 1) && (sides[1] == 2) && (sides[2] == 3))
	{
		callRec       = 7;
		callNeigh [0] = 2; callNeigh [1] = 5; callNeigh [2] = 0;
		callNeigh2[0] = 6; callNeigh2[1] = 3; callNeigh2[2] = 4;
		child         = 1;

		return true;
	}

	if ((sides[0] == 1) && (sides[1] == 3) && (sides[2] == 5))
	{
		callRec       = 3;
		callNeigh [0] = 6; callNeigh [1] = 4; callNeigh [2] = 1;
		callNeigh2[0] = 7; callNeigh2[1] = 2; callNeigh2[2] = 0;
		child         = 5;

		return true;
	}

	if ((sides[0] == 2) && (sides[1] == 3) && (sides[2] == 4))
	{
		callRec       = 4;
		callNeigh [0] = 6; callNeigh [1] = 3; callNeigh [2] = 1;
		callNeigh2[0] = 7; callNeigh2[1] = 5; callNeigh2[2] = 0;
		child         = 2;

		return true;
	}

	if ((sides[0] == 3) && (sides[1] == 4) && (sides[2] == 5))
	{
		callRec       = 0;
		callNeigh [0] = 7; callNeigh [1] = 5; callNeigh [2] = 2;
		callNeigh2[0] = 4; callNeigh2[1] = 3; callNeigh2[2] = 1;
		child         = 6;

		return true;
	}

	return false;
}

void Data::BoxTree::neighbors2D(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &neighs) const
{
	if (c->isRoot())
	{
		return;
	}

	BoxTreeCell *p = static_cast<BoxTreeCell *>(c->parent());

	UInt child = c->position();

	if (child == (sides[0] + 2)%4)
	{
		this->neighbors2D(p, sides, neighs);

		return;
	}

	BoxTreeCell *s = static_cast<BoxTreeCell *>(p->child((sides[0] + 2)%4));

	if ((child == (sides[0] + 1)%4) || (child == (sides[0] + 3)%4))
	{
		UInt side = (child == (sides[0] + 1)%4) ? 0 : 1;

		s = this->neighbor(s, sides[side]);

		if ((!s) || (s->level() < c->level()))
		{
			return;
		}
	}

	BoxTreeCell *n = this->child(s, sides[0]);

	neighs.push_back(n);
}

GraphNodeList Data::BoxTree::neighbors2D(const BoxTreeCell *c, const UIntVector &sides) const
{
	GraphNodeList neighs;

	this->neighbors2D(c, sides, neighs);

	return neighs;
}

void Data::BoxTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &neighs) const
{
	UIntList stack;

	this->neighbors3D2(c, sides, stack, neighs);
}

GraphNodeList Data::BoxTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides) const
{
	GraphNodeList neighs;

	this->neighbors3D2(c, sides, neighs);

	return neighs;
}

void Data::BoxTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack, GraphNodeList &neighs) const
{
	if (c->isRoot())
	{
		return;
	}

	UIntVector callRec(2), callSib(4), callNeigh(2), children(2);

	if (!this->buildTableNeighbor3D2(sides, callRec, callSib, callNeigh, children))
	{
		return;
	}

	BoxTreeCell *n = this->neighbor3D2(c, sides, stack);

	if (!n)
	{
		return;
	}

	this->children(n, children, stack, neighs);
}

GraphNodeList Data::BoxTree::neighbors3D2(const BoxTreeCell *c, const UIntVector &sides, UIntList &stack) const
{
	GraphNodeList neighs;

	this->neighbors3D2(c, sides, stack, neighs);

	return neighs;
}

void Data::BoxTree::neighbors3D3(const BoxTreeCell *c, const UIntVector &sides, GraphNodeList &neighs) const
{
	if (c->isRoot())
	{
		return;
	}

	UInt callRec;
	UIntVector callNeigh(2);
	UIntVector callNeigh2(2);
	UInt ch;

	if (!this->buildTableNeighbor3D3(sides, callRec, callNeigh, callNeigh2, ch))
	{
		return;
	}

	BoxTreeCell *p = static_cast<BoxTreeCell *>(c->parent());

	UInt child = c->position();

	if (child == callRec)
	{
		this->neighbors3D3(p, sides, neighs);

		return;
	}

	BoxTreeCell *s = static_cast<BoxTreeCell *>(p->child(callRec));

	if ((child == callNeigh[0]) ||
		(child == callNeigh[1]) ||
		(child == callNeigh[2]))
	{
		UInt side = (child == callNeigh[0]) ? 0 : ((child == callNeigh[1]) ? 1 : 2);

		s = this->neighbor(s, sides[side]);

		if ((!s) || (s->level() < c->level()))
		{
			return;
		}
	}

	if ((child == callNeigh2[0]) ||
		(child == callNeigh2[1]) ||
		(child == callNeigh2[2]))
	{
		UIntVector sds(2);

		sds[0] = ((child == callNeigh2[0]) || (child == callNeigh2[1])) ? 0 : 1;
		sds[1] =  (child == callNeigh2[0]) ? 1 : 2;

		sds[0] = sides[sds[0]];
		sds[1] = sides[sds[1]];

		s = this->neighbor3D2(s, sds);

		if ((!s) || (s->level() < c->level()))
		{
			return;
		}
	}

	BoxTreeCell *n = this->child(s, ch);

	neighs.push_back(n);
}

GraphNodeList Data::BoxTree::neighbors3D3(const BoxTreeCell *c, const UIntVector &sides) const
{
	GraphNodeList neighs;

	this->neighbors3D3(c, sides, neighs);

	return neighs;
}

void Data::BoxTree::neighbors(const BoxTreeCell *c, UInt side, UIntList &stack, GraphNodeList &neighs) const
{
	if ((c->isRoot()) || (side > c->numSides()))
	{
		return;
	}

	BoxTreeCell *n = this->neighbor(c, side, stack);

	if (!n)
	{
		return;
	}

	if (c->dimension() == 1)
	{
		n = this->child(n, side);

		neighs.push_back(n);

		return;
	}

	UIntVector children(static_cast<UInt>(std::pow(2, c->dimension() - 1)), 0);

	if (c->dimension() == 2)
	{
		children[0] = (side + 3)%4;
		children[1] = side;
	}
	else if (c->dimension() == 3)
	{
		switch (side)
		{
		case 0:
			children[0] = 0;
			children[1] = 3;
			children[2] = 4;
			children[3] = 7;
			break;
		case 1:
			children[0] = 0;
			children[1] = 1;
			children[2] = 4;
			children[3] = 5;
			break;
		case 2:
			children[0] = 0;
			children[1] = 1;
			children[2] = 2;
			children[3] = 3;
			break;
		case 3:
			children[0] = 1;
			children[1] = 2;
			children[2] = 5;
			children[3] = 6;
			break;
		case 4:
			children[0] = 2;
			children[1] = 3;
			children[2] = 6;
			children[3] = 7;
			break;
		case 5:
			children[0] = 4;
			children[1] = 5;
			children[2] = 6;
			children[3] = 7;
			break;
		}
	}
	else
	{
		return;
	}

	this->children(n, children, stack, neighs);
}

GraphNodeList Data::BoxTree::neighbors(const BoxTreeCell *c, UInt side, UIntList &stack) const
{
	GraphNodeList neighs;

	this->neighbors(c, side, stack, neighs);

	return neighs;
}

BoxTreeCell *Data::BoxTree::child(const BoxTreeCell *c, UIntList &stack) const
{
	if ((c->isLeaf()) || (stack.empty()))
	{
		return const_cast<BoxTreeCell *>(c);
	}

	UInt child = stack.back();

	stack.pop_back();

	return this->child(static_cast<BoxTreeCell *>(c->child(child)), stack);
}

BoxTreeCell *Data::BoxTree::child(const BoxTreeCell *c, UInt child) const
{
	BoxTreeCell *cell = const_cast<BoxTreeCell *>(c);

	while (!cell->isLeaf())
	{
		cell = static_cast<BoxTreeCell *>(cell->child(child));
	}

	return cell;
}

void Data::BoxTree::children(const BoxTreeCell *c, UInt side, GraphNodeList &cells) const
{
	if (side > c->numSides())
	{
		return;
	}

	if (c->dimension() == 1)
	{
		UIntVector ch(1, side);

		this->children(c, ch, cells);

		return;
	}

	if (c->dimension() == 2)
	{
		UIntVector ch(2);

		ch[0] = (side + 1)%4;
		ch[1] = (side + 2)%4;

		this->children(c, ch, cells);

		return;
	}

	if (c->dimension() == 3)
	{
		UIntVector ch(4);

		switch (side)
		{
		case 0:
			ch[0] = 1;
			ch[1] = 2;
			ch[2] = 5;
			ch[3] = 6;
			break;
		case 1:
			ch[0] = 2;
			ch[1] = 3;
			ch[2] = 6;
			ch[3] = 7;
			break;
		case 2:
			ch[0] = 4;
			ch[1] = 5;
			ch[2] = 6;
			ch[3] = 7;
			break;
		case 3:
			ch[0] = 0;
			ch[1] = 3;
			ch[2] = 4;
			ch[3] = 7;
			break;
		case 4:
			ch[0] = 0;
			ch[1] = 1;
			ch[2] = 4;
			ch[3] = 5;
			break;
		case 5:
			ch[0] = 0;
			ch[1] = 1;
			ch[2] = 2;
			ch[3] = 3;
			break;
		}

		this->children(c, ch, cells);

		return;
	}
}

GraphNodeList Data::BoxTree::children(const BoxTreeCell *c, UInt side) const
{
	GraphNodeList cells;

	this->children(c, side, cells);

	return cells;
}

void Data::BoxTree::children(const BoxTreeCell *c, const UIntVector &children, GraphNodeList &cells) const
{
	if (c->isLeaf())
	{
		cells.push_back(const_cast<BoxTreeCell *>(c));

		return;
	}

	for (UInt i = 0; i < children.size(); i++)
	{
		this->children(static_cast<BoxTreeCell *>(c->child(children[i])), children, cells);
	}
}

GraphNodeList Data::BoxTree::children(const BoxTreeCell *c, const UIntVector &children) const
{
	GraphNodeList cells;

	this->children(c, children, cells);

	return cells;
}

void Data::BoxTree::children(const BoxTreeCell *c, const UIntVector &children, UIntList &stack, GraphNodeList &cells) const
{
	if (c->isLeaf())
	{
		cells.push_back(const_cast<BoxTreeCell *>(c));

		return;
	}

	if (stack.empty())
	{
		this->children(c, children, cells);

		return;
	}

	UInt child = stack.back();

	stack.pop_back();

	this->children(static_cast<BoxTreeCell *>(c->child(child)), children, stack, cells);
}

GraphNodeList Data::BoxTree::children(const BoxTreeCell *c, const UIntVector &children, UIntList &stack) const
{
	GraphNodeList cells;

	this->children(c, children, stack, cells);

	return cells;
}

#if USE_GUI
void Data::BoxTree::draw() const
{
	if (!this->hasLists())
	{
		this->createList();
	}

	if (this->hasList())
	{
		if (this->isDrawing())
		{
			return;
		}

		this->drawList();

		return;
	}

	this->beginList();

#if USE_EXPERIMENTAL_TREE
	if (this->getRoot())
#else
	if (this->root)
#endif //#if USE_EXPERIMENTAL_TREE
	{
		/*bool enabled = glIsEnabled(GL_LIGHTING);

		if (enabled)
		{
			glDisable(GL_LIGHTING);
		}*/

#if USE_EXPERIMENTAL_TREE
		static_cast<BoxTreeCell *>(this->getRoot())->draw();
#else
		static_cast<BoxTreeCell *>(this->root)->draw();
#endif //#if USE_EXPERIMENTAL_TREE

		/*if (enabled)
		{
			glEnable(GL_LIGHTING);
		}*/
	}

	this->endList();
}

std::string Data::BoxTree::text() const
{
#if USE_EXPERIMENTAL_TREE
	return (this->getRoot()) ? static_cast<BoxTreeCell *>(this->getRoot())->text() : "";
#else
	return (this->root) ? static_cast<BoxTreeCell *>(this->root)->text() : "";
#endif //#if USE_EXPERIMENTAL_TREE
}
#endif //#if USE_GUI
