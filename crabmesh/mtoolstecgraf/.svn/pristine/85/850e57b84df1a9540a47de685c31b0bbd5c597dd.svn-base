#include "MJMesh/FrontElementBoxSearchTreeCell.h"

#include "MJMesh/FrontElementBoxSearchTree.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"

#if USE_EXPERIMENTAL_5

MJMesh::FrontElementBoxSearchTreeCell2D::FrontElementBoxSearchTreeCell2D(
	Data::BoxTreeCell *cell, MJMesh::FrontElementBoxSearchTreeCell2D *parent, ULInt id) :
		Data::FrontElementBoxSearchTreeCell2D(cell, parent, id)
{

}

MJMesh::FrontElementBoxSearchTreeCell2D::~FrontElementBoxSearchTreeCell2D()
{

}

bool MJMesh::FrontElementBoxSearchTreeCell2D::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);

		fel->addFrontData(static_cast<MJMesh::FrontElementBoxSearchTree2D *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::FrontElementBoxSearchTreeCell2D::remove(const Data::Identifiable *s)
{
	const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&fel->getFrontSTNodes(static_cast<MJMesh::FrontElementBoxSearchTree2D *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::FrontElementBoxSearchTreeCell2D *>(static_cast<const MJMesh::FrontElementBoxSearchTreeCell2D *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::FrontElementBoxSearchTreeCell2D::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	return static_cast<MJMesh::Edge2D *>(static_cast<const MJMesh::FrontElement *>(s)->getShape())->getBox()->optIntercept(box);
}

MJMesh::FrontElementBoxSearchTreeCell3D::FrontElementBoxSearchTreeCell3D(
	Data::BoxTreeCell *cell, MJMesh::FrontElementBoxSearchTreeCell3D *parent, ULInt id) :
		Data::FrontElementBoxSearchTreeCell3D(cell, parent, id)
{

}

MJMesh::FrontElementBoxSearchTreeCell3D::~FrontElementBoxSearchTreeCell3D()
{

}

bool MJMesh::FrontElementBoxSearchTreeCell3D::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);

		fel->addFrontData(static_cast<MJMesh::FrontElementBoxSearchTree3D *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::FrontElementBoxSearchTreeCell3D::remove(const Data::Identifiable *s)
{
	const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&fel->getFrontSTNodes(static_cast<MJMesh::FrontElementBoxSearchTree3D *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::FrontElementBoxSearchTreeCell3D *>(static_cast<const MJMesh::FrontElementBoxSearchTreeCell3D *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::FrontElementBoxSearchTreeCell3D::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	return static_cast<MJMesh::Triangle3D *>(static_cast<const MJMesh::FrontElement *>(s)->getShape())->getBox()->optIntercept(box);
}

#else

MJMesh::FrontElementBoxSearchTreeCell::FrontElementBoxSearchTreeCell(
	Data::BoxTreeCell *cell, MJMesh::FrontElementBoxSearchTreeCell *parent, ULInt id) :
		Data::FrontElementBoxSearchTreeCell(cell, parent, id)
{

}

MJMesh::FrontElementBoxSearchTreeCell::~FrontElementBoxSearchTreeCell()
{

}

bool MJMesh::FrontElementBoxSearchTreeCell::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);

		fel->addFrontData(static_cast<MJMesh::FrontElementBoxSearchTree *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::FrontElementBoxSearchTreeCell::remove(const Data::Identifiable *s)
{
	const MJMesh::FrontElement *fel = static_cast<const MJMesh::FrontElement *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&fel->getFrontSTNodes(static_cast<MJMesh::FrontElementBoxSearchTree *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::FrontElementBoxSearchTreeCell *>(static_cast<const MJMesh::FrontElementBoxSearchTreeCell *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::FrontElementBoxSearchTreeCell::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	Data::GeometricShape *shape = static_cast<const MJMesh::FrontElement *>(s)->getShape();

	return (shape->dimension() == 2) ?
		static_cast<MJMesh::Edge2D *>(shape)->getBox()->optIntercept(box) :
		static_cast<MJMesh::Triangle3D *>(shape)->getBox()->optIntercept(box);
}

#endif //#if USE_EXPERIMENTAL_5
