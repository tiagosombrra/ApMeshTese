#include "Performer/AngleTreeCellClassifier.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/BoxTreeCell.h"
#include "Data/FrontElement.h"
#include "Performer/Clipper.h"
#include "Performer/AnglePointInObjectDecider.h"

Performer::AngleTreeCellClassifier::AngleTreeCellClassifier(UInt dimension,
	const Data::Boundary *boundary, const Data::BoxTree *tree) :
	TreeCellClassifier(boundary, tree)
{
	this->setDimension(dimension);
}

/*Performer::AngleTreeCellClassifier::AngleTreeCellClassifier(UInt dimension,
	const Data::Front *front, const Data::BoxTree *tree) :
	TreeCellClassifier(front, tree)
{
	this->setDimension(dimension);
}*/

Performer::AngleTreeCellClassifier::~AngleTreeCellClassifier()
{

}

void Performer::AngleTreeCellClassifier::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

Data::Point *Performer::AngleTreeCellClassifier::mid(const Data::BoxTreeCell *cell) const
{
	Data::Point *p = NULL;

	if (this->dimension == 2)
	{
#if USE_EXPERIMENTAL_3
		p = new Data::Point2D(cell->mid2D());
#else
		p = new Data::Point2D(cell->mid());
#endif //#if USE_EXPERIMENTAL_3
	}
	else
	{
#if USE_EXPERIMENTAL_3
		p = new Data::Point3D(cell->mid3D());
#else
		p = new Data::Point3D(cell->mid());
#endif //#if USE_EXPERIMENTAL_3
	}

	return p;
}

#if USE_EXPERIMENTAL_3
Data::Point2DObjList Performer::AngleTreeCellClassifier::clip2D(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s, Clipper::EDGE);

	clipper.execute();

	return clipper.getPoints2D();
}

Data::Point3DObjList Performer::AngleTreeCellClassifier::clip3D(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s, Clipper::POLYGON);

	clipper.execute();

	return clipper.getPoints3D();
}
#else
Data::PointObjList Performer::AngleTreeCellClassifier::clip(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s,
		(this->dimension == 2) ? Clipper::EDGE : Clipper::POLYGON);

	clipper.execute();

	return clipper.getPoints();
}
#endif //#if USE_EXPERIMENTAL_3

bool Performer::AngleTreeCellClassifier::out(const Data::Box &box, const Data::FrontElement *e) const
{
	//test if the two bounding boxes intercept
	//if (!box.intercept(e->getShape()->box()))
	if (!box.optIntercept(e->getShape()->box()))
	{
		//the two bounding do not intercept
		return true;
	}

#if USE_EXPERIMENTAL_3
	return (this->dimension == 2) ?
        this->clip2D(&box, e->getShape()).empty() :
        this->clip3D(&box, e->getShape()).empty();
	//Data::PointObjList points = this->clip(&box, e->getShape());

	//return points.empty();
#else
	Data::PointObjList points = this->clip(&box, e->getShape());

	return points.empty();
#endif //#if USE_EXPERIMENTAL_3
}

bool Performer::AngleTreeCellClassifier::on(const Data::BoxTreeCell *cell,
	//const Data::FrontElementList &elements, Data::FrontElementList &intercepting,
	const Data::FrontElementList *elements, Data::FrontElementList &intercepting,
	bool clipTest) const
{
	//for (Data::FrontElementList::const_iterator iter = elements.begin(), end = elements.end();
	for (Data::FrontElementList::const_iterator iter = elements->begin(), end = elements->end();
		 iter != end; ++iter)
	{
		Data::FrontElement *fel = (*iter);

		//if (fel->getShape()->box().intercept(cell->box()))
		if (fel->getShape()->box().optIntercept(cell->box()))
		{
			intercepting.push_back(fel);
		}
	}

	/*if (!clipTest)
	{
		return !intercepting.empty();
	}*/

	if (clipTest)
	{
		for (Data::FrontElementList::iterator iter = intercepting.begin(), end = intercepting.end();
			 iter != end; ++iter)
		{
#if USE_EXPERIMENTAL_3
            bool empty = (this->dimension == 2) ?
                this->clip2D(cell->box(), (*iter)->getShape()).empty() :
                this->clip3D(cell->box(), (*iter)->getShape()).empty();

			if (!empty)
#else
			if (!this->clip(cell->box(), (*iter)->getShape()).empty())
#endif //#if USE_EXPERIMENTAL_3
			{
				return true;
			}
		}
	}

	return false;
}

bool Performer::AngleTreeCellClassifier::on(const Data::BoxTreeCell *cell,
	//const Data::FrontElementList &elements, bool clipTest) const
	const Data::FrontElementList *elements, bool clipTest) const
{
	Data::FrontElementList intercepting;

	return this->on(cell, elements, intercepting, clipTest);
}

bool Performer::AngleTreeCellClassifier::onRemoving(const Data::BoxTreeCell *cell,
	Data::FrontElementList &elements, Data::FrontElementList &intercepting, bool clipTest) const
{
	bool anyIn = false;

	for (Data::FrontElementList::iterator iter = elements.begin();
		 iter != elements.end();)
	{
		Data::FrontElement *fel = (*iter);

		//if (fel->getShape()->box().intercept(cell->box()))
		if (fel->getShape()->box().optIntercept(cell->box()))
		{
			intercepting.push_back(fel);

			bool allIn = false;

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				Data::Vertex *v = fel->getVertex(i);

				if (!cell->box()->in(v->getPoint()))
				{
					allIn = false;

					break;
				}
			}

			if (allIn)
			{
				iter = elements.erase(iter);

				anyIn = true;

				continue;
			}
		}

		iter++;
	}

	/*if (!clipTest)
	{
		return !intercepting.empty();
	}*/

	if (clipTest)
	{
		if (anyIn)
		{
			return true;
		}

		for (Data::FrontElementList::iterator iter = intercepting.begin(), end = intercepting.end();
			 iter != end; ++iter)
		{
#if USE_EXPERIMENTAL_3
            bool empty = (this->dimension == 2) ?
                this->clip2D(cell->box(), (*iter)->getShape()).empty() :
                this->clip3D(cell->box(), (*iter)->getShape()).empty();

			if (!empty)
#else
			if (!this->clip(cell->box(), (*iter)->getShape()).empty())
#endif //#if USE_EXPERIMENTAL_3
			{
				return true;
			}
		}
	}

	return false;
}

bool Performer::AngleTreeCellClassifier::onRemoving(const Data::BoxTreeCell *cell,
	Data::FrontElementList &elements, bool clipTest) const
{
	Data::FrontElementList intercepting;

	return this->onRemoving(cell, elements, intercepting, clipTest);
}

enum Data::Position Performer::AngleTreeCellClassifier::classify(Data::Point *p,
	//const Data::FrontElementList &elements, bool canBeUnknown) const
	const Data::FrontElementList *elements, bool canBeUnknown) const
{
	//AnglePointInObjectDecider *apiod = this->makeApiod(p, &elements, canBeUnknown);
	AnglePointInObjectDecider *apiod = this->makeApiod(p, elements, canBeUnknown);

	apiod->execute();

	enum Data::Position pos = apiod->getDecision();

	delete p;

	delete apiod;

	return pos;
}

void Performer::AngleTreeCellClassifier::classifyChildren(
	//const Data::BoxTreeCell *cell, const Data::FrontElementList &elements)
	const Data::BoxTreeCell *cell, const Data::FrontElementList *elements)
{
	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->classify(static_cast<Data::BoxTreeCell *>(cell->child(i)), elements);
	}
}

enum Data::Position Performer::AngleTreeCellClassifier::classify(const Data::BoxTreeCell *cell,
	//const Data::FrontElementList &elements)
	const Data::FrontElementList *elements)
{
	if (this->on(cell, elements))
	{
		this->setPosition(cell, Data::ON);

		this->classifyChildren(cell, elements);

		return Data::ON;
	}

	enum Data::Position pos = this->classify(this->mid(cell), elements);

	this->setChildrenPosition(cell, pos);

	return pos;
}
