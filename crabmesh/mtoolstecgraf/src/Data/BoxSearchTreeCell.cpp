#include "Data/BoxSearchTreeCell.h"

#include "Data/BoxTreeCell.h"
#include "Data/BoxSearchTree.h"
#include "Data/BoxTree.h"
#include "Data/Box.h"
#include "Data/QuadTreeCell.h"
#include "Data/OctTreeCell.h"

using namespace Data;

Data::BoxSearchTreeCell::BoxSearchTreeCell(BoxTreeCell *cell, BoxSearchTreeCell *parent, ULInt id)
	: GeometricSearchTreeNode(parent, id)
{
	this->setCell(cell);
}

Data::BoxSearchTreeCell::~BoxSearchTreeCell()
{
	//debug
	//std::stringstream str;
	//str << "will now delete box search tree cell " << (this->cell ? this->cell->getId() : 0) << std::endl;
	//std::cout << str.str();
	//end debug

#if (!USE_EXPERIMENTAL_5)
	if (!this->getTree())
	{
		this->cell = NULL;

		for (UInt i = 0; i < this->numChildren(); i++)
		{
			this->child(i)->setTree(NULL);
		}
	}
#endif //#if (!USE_EXPERIMENTAL_5)

	if (this->cell)
	{
		delete this->cell;
	}
}

void Data::BoxSearchTreeCell::setCell(BoxTreeCell *cell)
{
	this->cell = cell;
}

BoxTreeCell *Data::BoxSearchTreeCell::getCell() const
{
	return this->cell;
}

//gambiarra
void Data::BoxSearchTreeCell::makeCell()
{
	if (this->cell)
	{
		UInt dim = this->cell->dimension();

		if (dim == 2)
		{
			this->cell = new QuadTreeCell(this->parent() ?
				static_cast<QuadTreeCell *>(static_cast<BoxSearchTreeCell *>(this->parent())->getCell()) : NULL, this->cell->getId());
		}
		else
		{
			this->cell = new OctTreeCell(this->parent() ?
				static_cast<OctTreeCell *>(static_cast<BoxSearchTreeCell *>(this->parent())->getCell()) : NULL, this->cell->getId());
		}
	}
}
//end gambiarra

/*GraphNodeSet Data::BoxSearchTreeCell::cellsInRange(const Box *box) const
{
	Data::GraphNodeSet cells;

	this->cellsInRange(box, cells);

	return cells;
}*/

bool Data::BoxSearchTreeCell::inRange(const Identifiable *s) const
{
	/*if (!this->cell->box())
	{
		std::cout << "ERROR in Data::BoxSearchTreeCell::inRange:" << this->cell->box() << ", id = " << this->getId() << ", cell->id = " << this->cell->getId() << ", s->id = " << s->getId() << std::endl;

		return false;
	}*/

	return this->inRange(this->cell->box(), s);
}

bool Data::BoxSearchTreeCell::add(const Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		this->shapes.insert(const_cast<Identifiable *>(s));

		/*//debug
		s->setColor(0.0, 1.0, 0.0);
		//this->cell->Data::Box::highlight();
		//endebug*/

		return true;
	}

	bool added = false;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		if (static_cast<BoxSearchTreeCell *>(this->child(i))->add(s))
		{
			added = true;
		}
	}

	return added;
}

bool Data::BoxSearchTreeCell::remove(const Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		this->shapes.erase(const_cast<Identifiable *>(s));

		/*//debug
		s->setColor(1.0, 1.0, 1.0);

		if (this->shapes.empty())
		{
			this->cell->Data::Box::unhighlight();
		}
		//endebug*/

		return true;
	}

	bool removed = false;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		if (static_cast<BoxSearchTreeCell *>(this->child(i))->remove(s))
		{
			removed = true;
		}
	}

	return removed;
}

#if USE_GUI
void Data::BoxSearchTreeCell::draw() const
{
	if (!this->getTree())
	{
		return;
	}

	if (this->cell)
	{
		if (this->isLeaf())
		{
			/*bool high = this->cell->Box::isHighlighted();

			if (high)
			{
				this->cell->Box::unhighlight();
			}*/

			this->cell->box()->draw();

			/*if (high)
			{
				this->cell->Box::highlight();
			}*/
		}
		else
		{
			for (UInt i = 0; i < this->numChildren(); i++)
			{
				this->child(i)->draw();
			}
		}
	}
}
#endif //#if USE_GUI

void Data::BoxSearchTreeCell::inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const
{
	//if (!this->cell->intercept(box))
	if (!this->cell->optIntercept(box))
	{
		return;
	}

	if (this->isLeaf())
	{
		for (GeometricSearchTreeNode::ShapeSet::const_iterator iter = this->shapes.begin();
			 iter != this->shapes.end(); ++iter)
		{
			Identifiable *s = (*iter);

			if (this->inRange(box, s))
			{
				shapes.insert(s);
			}
		}

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<BoxSearchTreeCell *>(this->child(i))->inRange(box, shapes);
	}
}

GeometricSearchTreeNode::ShapeSet Data::BoxSearchTreeCell::inRange(const Box *box) const
{
	GeometricSearchTreeNode::ShapeSet shapes;

	this->inRange(box, shapes);

	return shapes;

	/*Data::GraphNodeSet cells = this->cellsInRange(box);

	IdentifiableSet shapes;

	for (Data::GraphNodeSet::iterator iter = cells.begin();
		 iter != cells.end(); iter++)
	{
		BoxSearchTreeCell *c = static_cast<BoxSearchTreeCell *>(*iter);

		//debug
		//c->cell->Data::Box::highlight();
		//endebug

		for (IdentifiableSet::iterator iter2 = c->shapes.begin();
			 iter2 != c->shapes.end(); iter2++)
		{
			Identifiable *s = (*iter2);

			if (this->inRange(box, s))
			{
				shapes.insert(s);
			}
		}
	}

	return shapes;*/
}

/*void Data::BoxSearchTreeCell::cellsInRange(const Box *box, GraphNodeSet &cells) const
{
	*//*if (this->isLeaf())
	{
		if (cells.count((GraphNode *)this) > 0)
		{
			return;
		}

		//if (!this->cell->intercept(box))
		if (!this->cell->optIntercept(box))
		{
			return;
		}

		cells.insert((GraphNode *)this);

		GraphNodeVector neighs = ((BoxSearchTree *)this->getTree())->neighbors(this);

		for (UInt i = 0; i < neighs.size(); i++)
		{
			if (neighs[i])
			{
				((BoxSearchTreeCell *)neighs[i])->cellsInRange(box, cells);
			}
		}

		return;
	}*//*

	//this performs as follows
	// 1 -> if the cell is a leaf and intercepts the box, add it to the list
	// 2 -> if the cell is not a leaf but intercepts the box, recurse to the
	//      children
	// 3 -> if the cell does not intercept the box, return.

	//if (!this->cell->intercept(box))
	if (!this->cell->optIntercept(box))
	{
		return;
	}

	if (this->isLeaf())
	{
		cells.insert(const_cast<BoxSearchTreeCell *>(this));

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<BoxSearchTreeCell *>(this->child(i))->cellsInRange(box, cells);
	}

	*//*return;

	//this performs as follows:
	// 1 -> if the cell is a leaf and intercepts the box, add it to the list
	// 1.1 -> find the neighbors of this cell and add them to the list, if
	//        they intercept the box
	// 2 -> if the cell is a leaf, return the list
	// 3 -> it the cell is not a leaf, then
	// 3.1 -> if the list is empty, recurse to the children
	// 3.2 -> if the list is not empty, the cells were already found. therefore,
	//        return the list.
	if (!cells.empty())
	{
		return;
	}

	//if (!this->cell->intercept(box))
	if (!this->cell->optIntercept(box))
	{
		return;
	}

	if (this->isLeaf())
	{
		cells.insert(const_cast<BoxSearchTreeCell *>(this));

		Data::GraphNodeList aux;

		GraphNodeVector neighs = static_cast<BoxSearchTree *>(this->getTree())->neighbors(this);

		aux.insert(aux.end(), neighs.begin(), neighs.end());

		while (!aux.empty())
		{
			Data::BoxSearchTreeCell *c = static_cast<Data::BoxSearchTreeCell *>(aux.front());

			aux.pop_front();

			if ((!c) ||
				//(!c->cell->intercept(box)) ||
				(!c->cell->optIntercept(box)) ||
				(cells.count(c) > 0))
			{
				continue;
			}

			cells.insert(c);

			GraphNodeVector neighs = static_cast<BoxSearchTree *>(this->getTree())->neighbors(c);

			aux.insert(aux.end(), neighs.begin(), neighs.end());
		}

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<BoxSearchTreeCell *>(this->child(i))->cellsInRange(box, cells);

		if (!cells.empty())
		{
			break;
		}
	}*//*
}*/

#if USE_ARRAY
BoxSearchTreeCell &Data::BoxSearchTreeCell::operator=(const BoxSearchTreeCell &c)
{
	if (this != &c)
	{
		GeometricSearchTreeNode::operator=(c);

		this->cell = c.cell;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_EXPERIMENTAL_5

Data::BoxSearchTreeCell2D::BoxSearchTreeCell2D(BoxTreeCell *cell, BoxSearchTreeCell2D *parent, ULInt id) :
    Data::BoxSearchTreeCell::BoxSearchTreeCell(cell, parent, id)
{
    this->adjacents[0] = parent;

    this->adjacents[1] = this->adjacents[2] = this->adjacents[3] = this->adjacents[4] = NULL;

    if (parent)
    {
        parent->add(this);
    }
}

Data::BoxSearchTreeCell2D::~BoxSearchTreeCell2D()
{
    if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

	if (!this->getTree())
	{
		this->cell = NULL;

		for (UInt i = 0; i < this->numChildren(); i++)
		{
			this->child(i)->setTree(NULL);
		}
	}

    for (UInt i = 1; i < 5; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];
		}
	}
}

UInt Data::BoxSearchTreeCell2D::numAdjacents() const
{
    UInt count = 1;

    for (UInt i = 1; i < 5; i++)
    {
        if (this->adjacents[i])
        {
            count++;
        }
    }

    return count;
}

void Data::BoxSearchTreeCell2D::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::BoxSearchTreeCell2D::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::BoxSearchTreeCell2D::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 5);
}

void Data::BoxSearchTreeCell2D::add(GraphNode *n)
{
    for (UInt i = 1; i < 5; i++)
    {
        if (!this->adjacents[i])
        {
            this->adjacents[i] = n;

            break;
        }
    }
}

void Data::BoxSearchTreeCell2D::remove(GraphNode *n)
{
    if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

    for (UInt i = 1; i < 5; i++)
    {
        if (this->adjacents[i] == n)
        {
            this->adjacents[i] = NULL;

            break;
        }
    }
}

#if USE_ARRAY
BoxSearchTreeCell2D &Data::BoxSearchTreeCell2D::operator=(const BoxSearchTreeCell2D &c)
{
    if (this != &c)
    {
        Data::BoxSearchTreeCell::operator=(c);

        for (UInt i = 0; i < 5; i++)
        {
            this->adjacents[i] = c.adjacents[i];
        }
    }

    return *this;
}
#endif //#if USE_ARRAY

Data::BoxSearchTreeCell3D::BoxSearchTreeCell3D(BoxTreeCell *cell, BoxSearchTreeCell3D *parent, ULInt id) :
    Data::BoxSearchTreeCell::BoxSearchTreeCell(cell, parent, id)
{
    this->adjacents[0] = parent;

    this->adjacents[1] = this->adjacents[2] = this->adjacents[3] = this->adjacents[4] =
        this->adjacents[5] = this->adjacents[6] = this->adjacents[7] = this->adjacents[8] = NULL;

    if (parent)
    {
        parent->add(this);
    }
}

Data::BoxSearchTreeCell3D::~BoxSearchTreeCell3D()
{
    if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

	if (!this->getTree())
	{
		this->cell = NULL;

		for (UInt i = 0; i < this->numChildren(); i++)
		{
			this->child(i)->setTree(NULL);
		}
	}

    for (UInt i = 1; i < 9; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];
		}
	}
}

UInt Data::BoxSearchTreeCell3D::numAdjacents() const
{
    UInt count = 1;

    for (UInt i = 1; i < 9; i++)
    {
        if (this->adjacents[i])
        {
            count++;
        }
    }

    return count;
}

void Data::BoxSearchTreeCell3D::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::BoxSearchTreeCell3D::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::BoxSearchTreeCell3D::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 9);
}

void Data::BoxSearchTreeCell3D::add(GraphNode *n)
{
    for (UInt i = 1; i < 9; i++)
    {
        if (!this->adjacents[i])
        {
            this->adjacents[i] = n;

            break;
        }
    }
}

void Data::BoxSearchTreeCell3D::remove(GraphNode *n)
{
    for (UInt i = 1; i < 9; i++)
    {
        if (this->adjacents[i] == n)
        {
            this->adjacents[i] = NULL;

            break;
        }
    }
}

#if USE_ARRAY
BoxSearchTreeCell3D &Data::BoxSearchTreeCell3D::operator=(const BoxSearchTreeCell3D &c)
{
    if (this != &c)
    {
        Data::BoxSearchTreeCell::operator=(c);

        for (UInt i = 0; i < 9; i++)
        {
            this->adjacents[i] = c.adjacents[i];
        }
    }

    return *this;
}
#endif //#if USE_ARRAY

#endif //#if USE_EXPERIMENTAL_5
