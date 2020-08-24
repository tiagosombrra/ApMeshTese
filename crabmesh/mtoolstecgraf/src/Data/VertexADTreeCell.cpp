#include "Data/VertexADTreeCell.h"

#include "Data/Vertex.h"

using namespace Data;

Data::VertexADTreeCell::VertexADTreeCell(VertexADTreeCell *parent) :
	Data::AlternatingDigitalTreeCell::AlternatingDigitalTreeCell(parent)
{

}

Data::VertexADTreeCell::~VertexADTreeCell()
{

}

void Data::VertexADTreeCell::build(const IdentifiableList &shapes)
{
	if (shapes.empty())
	{
		if (!this->shapes.empty())
		{
			this->shapes.clear();
		}

#if USE_EXPERIMENTAL_3
		this->mid = NULL;
#else
		this->mid = Point();
#endif //#if USE_EXPERIMENTAL_3
		this->box = Data::Box();
		this->global = Data::Box();
		this->active = true;

		return;
	}

	Identifiable **shs = new Identifiable*[shapes.size()];
	Point **pts = new Point*[shapes.size()];

	UInt size = 0;

	for (IdentifiableList::const_iterator iter = shapes.begin();
		 iter != shapes.end(); iter++, size++)
	{
		shs[size] = (*iter);
		pts[size] = static_cast<Vertex *>((*iter))->getPoint();
	}

	this->build(size, shs, pts);

	delete [] shs;
	delete [] pts;
}

void Data::VertexADTreeCell::build(const VertexList &vertices)
{
	if (vertices.empty())
	{
		if (!this->shapes.empty())
		{
			this->shapes.clear();
		}

#if USE_EXPERIMENTAL_3
		this->mid = NULL;
#else
		this->mid = Point();
#endif //#if USE_EXPERIMENTAL_3
		this->box = Data::Box();
		this->global = Data::Box();
		this->active = true;

		return;
	}

	Identifiable **shs = new Identifiable*[vertices.size()];
	Point **pts = new Point*[vertices.size()];

	UInt size = 0;

	for (VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++, size++)
	{
		shs[size] = (*iter);
		pts[size] = (*iter)->getPoint();
	}

	this->build(size, shs, pts);

	delete [] shs;
	delete [] pts;
}

bool Data::VertexADTreeCell::add(const Identifiable *s)
{
	return this->add(s, static_cast<const Vertex *>(s)->getPoint());
}

bool Data::VertexADTreeCell::remove(const Identifiable *s)
{
	return this->remove(s, static_cast<const Vertex *>(s)->getPoint());
}

AlternatingDigitalTreeCell *Data::VertexADTreeCell::makeChild()
{
	return new VertexADTreeCell(this);
}

void Data::VertexADTreeCell::set(const Identifiable *shape, const Point */*pt*/, const Box */*box*/)
{
	this->setShape(shape);
}

void Data::VertexADTreeCell::set(const AlternatingDigitalTreeCell *other)
{
	Identifiable *shape = other->getShape();

	if (shape)
	{
		this->setShape(shape);
	}
}

void Data::VertexADTreeCell::inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const
{
	this->inRangeMid(box, shapes);
}

void Data::VertexADTreeCell::adjustGlobalWithBoxOrMid()
{
#if USE_EXPERIMENTAL_3
	for (UInt i = 0; i < this->mid->dimension(); i++)
#else
	for (UInt i = 0; i < this->mid.dimension(); i++)
#endif //#if USE_EXPERIMENTAL_3
	{
#if USE_EXPERIMENTAL_3
        Real coord = this->mid->getCoord(i);
#else
        Real coord = this->mid.getCoord(i);
#endif //#if USE_EXPERIMENTAL_3

        if (coord < this->global.getMin()->getCoord(i))
        {
        	this->global.getMin()->setCoord(i, coord);
        }

        if (coord > this->global.getMax()->getCoord(i))
		{
			this->global.getMax()->setCoord(i, coord);
		}
    }
}
