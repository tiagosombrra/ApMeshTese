#include "Data/FrontElementADTreeCell.h"

#include "Data/Box.h"
#include "Data/FrontElement.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

Data::FrontElementADTreeCell::FrontElementADTreeCell(FrontElementADTreeCell *parent) :
	Data::AlternatingDigitalTreeCell::AlternatingDigitalTreeCell(parent)
{

}

Data::FrontElementADTreeCell::~FrontElementADTreeCell()
{

}

void Data::FrontElementADTreeCell::build(const IdentifiableList &shapes)
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
	Box **boxes = new Box*[shapes.size()];

	UInt size = 0;

#if USE_EXPERIMENTAL_3
    UInt dimension = static_cast<FrontElement *>(shapes.front())->getShape()->dimension();
#endif //#if USE_EXPERIMENTAL_3

	for (IdentifiableList::const_iterator iter = shapes.begin();
		 iter != shapes.end(); iter++, size++)
	{
		shs[size] = (*iter);
		boxes[size] = new Box(static_cast<FrontElement *>((*iter))->getShape()->box());
#if USE_EXPERIMENTAL_3
		pts[size] = (dimension == 2) ?
            static_cast<Point *>(new Point2D(boxes[size]->mid2D())) :
            static_cast<Point *>(new Point3D(boxes[size]->mid3D()));
#else
		pts[size] = new Point(boxes[size]->mid());
#endif //#if USE_EXPERIMENTAL_3
	}

	this->build(size, shs, pts, boxes);

	for (UInt i = 0; i < size; i++)
	{
		delete pts[i];
		delete boxes[i];
	}

	delete [] shs;
	delete [] pts;
	delete [] boxes;
}

void Data::FrontElementADTreeCell::build(const FrontElementList &fels)
{
	if (fels.empty())
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

	Identifiable **shs = new Identifiable*[fels.size()];
	Point **pts = new Point*[fels.size()];
	Box **boxes = new Box*[fels.size()];

	UInt size = 0;

#if USE_EXPERIMENTAL_3
    UInt dimension = static_cast<FrontElement *>(fels.front())->getShape()->dimension();
#endif //#if USE_EXPERIMENTAL_3

	for (FrontElementList::const_iterator iter = fels.begin();
		 iter != fels.end(); iter++, size++)
	{
		shs[size] = (*iter);
		boxes[size] = new Box((*iter)->getShape()->box());
#if USE_EXPERIMENTAL_3
		pts[size] = (dimension == 2) ?
            static_cast<Point *>(new Point2D(boxes[size]->mid2D())) :
            static_cast<Point *>(new Point3D(boxes[size]->mid3D()));
#else
		pts[size] = new Point(boxes[size]->mid());
#endif //#if USE_EXPERIMENTAL_3
	}

	this->build(size, shs, pts, boxes);

	for (UInt i = 0; i < size; i++)
	{
		delete pts[i];
		delete boxes[i];
	}

	delete [] shs;
	delete [] pts;
	delete [] boxes;
}

bool Data::FrontElementADTreeCell::add(const Identifiable *s)
{
	Data::Box box = static_cast<const FrontElement *>(s)->getShape()->box();
#if USE_EXPERIMENTAL_3
	if (box.dimension() == 2)
    {
        Data::Point2D mid = box.mid2D();

        return this->add(s, &mid, &box);
    }

	Data::Point3D mid = box.mid3D();

    return this->add(s, &mid, &box);
#else
	Data::Point mid = box.mid();

	return this->add(s, &mid, &box);
#endif //#if USE_EXPERIMENTAL_3
}

bool Data::FrontElementADTreeCell::remove(const Identifiable *s)
{
	Data::Box box = static_cast<const FrontElement *>(s)->getShape()->box();
#if USE_EXPERIMENTAL_3
	if (box.dimension() == 2)
    {
        Data::Point2D mid = box.mid2D();

        return this->remove(s, &mid, &box);
    }

	Data::Point3D mid = box.mid3D();

    return this->remove(s, &mid, &box);
#else
	Data::Point mid = box.mid();

	return this->remove(s, &mid, &box);
#endif //#if USE_EXPERIMENTAL_3
}

AlternatingDigitalTreeCell *Data::FrontElementADTreeCell::makeChild()
{
	return new FrontElementADTreeCell(this);
}

void Data::FrontElementADTreeCell::set(const Identifiable *shape, const Point */*pt*/, const Box */*box*/)
{
	this->setShape(shape);
}

void Data::FrontElementADTreeCell::set(const AlternatingDigitalTreeCell *other)
{
	Identifiable *shape = other->getShape();

	if (shape)
	{
		this->setShape(shape);
	}
}

void Data::FrontElementADTreeCell::inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const
{
	this->inRangeBox(box, shapes);
}

void Data::FrontElementADTreeCell::adjustGlobalWithBoxOrMid()
{
	for (UInt i = 0; i < this->box.dimension(); i++)
	{
        Real coord = this->box.getMin()->getCoord(i);

        if (coord < this->global.getMin()->getCoord(i))
        {
        	this->global.getMin()->setCoord(i, coord);
        }

        coord = this->box.getMax()->getCoord(i);

        if (coord > this->global.getMax()->getCoord(i))
		{
			this->global.getMax()->setCoord(i, coord);
		}
    }
}
