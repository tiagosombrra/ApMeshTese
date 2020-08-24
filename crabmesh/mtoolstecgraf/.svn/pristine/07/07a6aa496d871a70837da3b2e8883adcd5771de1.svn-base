#include "MJMesh/VertexBoxSearchTreeCell.h"

#include "MJMesh/VertexBoxSearchTree.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"

#if USE_EXPERIMENTAL_5

MJMesh::VertexBoxSearchTreeCell2D::VertexBoxSearchTreeCell2D(
	Data::BoxTreeCell *cell, MJMesh::VertexBoxSearchTreeCell2D *parent, ULInt id) :
		Data::VertexBoxSearchTreeCell2D(cell, parent, id)
{

}

MJMesh::VertexBoxSearchTreeCell2D::~VertexBoxSearchTreeCell2D()
{

}

bool MJMesh::VertexBoxSearchTreeCell2D::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);

		v->addFrontData(static_cast<MJMesh::VertexBoxSearchTree2D *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::VertexBoxSearchTreeCell2D::remove(const Data::Identifiable *s)
{
	const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&v->getFrontSTNodes(static_cast<MJMesh::VertexBoxSearchTree2D *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::VertexBoxSearchTreeCell2D *>(static_cast<const MJMesh::VertexBoxSearchTreeCell2D *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::VertexBoxSearchTreeCell2D::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	return !box->optOut((static_cast<const Data::Vertex *>(s))->getPoint());
}

MJMesh::VertexBoxSearchTreeCell3D::VertexBoxSearchTreeCell3D(
	Data::BoxTreeCell *cell, MJMesh::VertexBoxSearchTreeCell3D *parent, ULInt id) :
		Data::VertexBoxSearchTreeCell3D(cell, parent, id)
{

}

MJMesh::VertexBoxSearchTreeCell3D::~VertexBoxSearchTreeCell3D()
{

}

bool MJMesh::VertexBoxSearchTreeCell3D::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);

		v->addFrontData(static_cast<MJMesh::VertexBoxSearchTree3D *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::VertexBoxSearchTreeCell3D::remove(const Data::Identifiable *s)
{
	const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&v->getFrontSTNodes(static_cast<MJMesh::VertexBoxSearchTree3D *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::VertexBoxSearchTreeCell3D *>(static_cast<const MJMesh::VertexBoxSearchTreeCell3D *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::VertexBoxSearchTreeCell3D::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	return !box->optOut((static_cast<const Data::Vertex *>(s))->getPoint());
}

#else

MJMesh::VertexBoxSearchTreeCell::VertexBoxSearchTreeCell(
	Data::BoxTreeCell *cell, MJMesh::VertexBoxSearchTreeCell *parent, ULInt id) :
		Data::VertexBoxSearchTreeCell(cell, parent, id)
{

}

MJMesh::VertexBoxSearchTreeCell::~VertexBoxSearchTreeCell()
{

}

bool MJMesh::VertexBoxSearchTreeCell::add(const Data::Identifiable *s)
{
	if ((!this->cell) || (!this->inRange(s)))
	{
		return false;
	}

	if (this->isLeaf())
	{
		const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);

		v->addFrontData(static_cast<MJMesh::VertexBoxSearchTree *>(this->getTree())->getFront(),
			this, this->shapes.insert(const_cast<Identifiable *>(s)).first);

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

bool MJMesh::VertexBoxSearchTreeCell::remove(const Data::Identifiable *s)
{
	const MJMesh::Vertex *v = static_cast<const MJMesh::Vertex *>(s);
	MJMesh::SpecificData::SearchTreeDataList *cells =
		&v->getFrontSTNodes(static_cast<MJMesh::VertexBoxSearchTree *>(this->getTree())->getFront());

	if (cells->empty())
	{
		return false;
	}

	while (!cells->empty())
	{
		const_cast<MJMesh::VertexBoxSearchTreeCell *>(static_cast<const MJMesh::VertexBoxSearchTreeCell *>(cells->front().stnode))->shapes.erase(cells->front().stnodeIt);

		cells->pop_front();
	}

	return true;
}

bool MJMesh::VertexBoxSearchTreeCell::inRange(const Data::Box *box, const Data::Identifiable *s) const
{
	return !box->optOut((static_cast<const Data::Vertex *>(s))->getPoint());
}

#endif //#if USE_EXPERIMENTAL_5
