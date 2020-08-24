#include "MJMesh/AnglePointInObjectDecider.h"

#include "Data/FrontElement.h"
#include "Data/Triangle3D.h"
#include "Data/Edge2D.h"
#include "Data/Exact.h"

using namespace MJMesh;

MJMesh::AnglePointInObjectDecider::AnglePointInObjectDecider(UInt dimension,
	Data::Point *point, const Data::FrontElementList *fels,
	bool canBeUnknown, UInt surfaceDirection) :
	Performer::AnglePointInObjectDecider(dimension, point)
{
	this->setFels(fels);

	this->canBeUnknown = canBeUnknown;
	this->surfaceDirection = surfaceDirection;
}

MJMesh::AnglePointInObjectDecider::~AnglePointInObjectDecider()
{
	//this->fels = NULL;
}

void MJMesh::AnglePointInObjectDecider::setFels(const Data::FrontElementList *fels)
{
	this->fels = fels;
}

#if (!USE_EXACT)
bool smallerfabs(Real v1, Real v2)
{
	return std::fabs(v1) < std::fabs(v2);
}
#endif //#if (!USE_EXACT)

Real MJMesh::AnglePointInObjectDecider::angle(const Data::GeometricShape *shape) const
{
	if ((this->dimension == 3) && (this->surfaceDirection >= 3))
	{
		return static_cast<const Data::Triangle3D *>(shape)->orientedSolidAngle(this->point, false);
	}

	if (this->surfaceDirection < 3)
    {
        Data::Point2D p1, p2, tp;

        for (UInt i = 1; i < this->dimension; i++)
        {
            UInt j = (i + this->surfaceDirection)%this->dimension;

            p1.setCoord(i - 1, shape->getPoint(0)->getCoord(j));
            p2.setCoord(i - 1, shape->getPoint(1)->getCoord(j));
            tp.setCoord(i - 1, this->point->getCoord(j));
        }

        Data::Edge2D e(&p1, &p2);

        Real angle = e.angle(tp);

        Data::Vector2D n = e.normal();

#if USE_EXPERIMENTAL_3
        Data::Point2D m = e.mid2D();
#else
        Data::Point2D m = e.mid();
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
        Data::Vector2D v(&m, &tp);
#else
        Data::Vector v(m, tp);
#endif //#if USE_EXPERIMENTAL_3

        e.setPoints(NULL, NULL);

        Real dot = n.dot(v)/v.norm();

        return (dot > 0.0) ? angle : -angle;
    }

	Real angle = static_cast<const Data::Edge2D *>(shape)->angle(this->point);

	Data::Vector2D n = static_cast<const Data::Edge2D *>(shape)->normal();

#if USE_EXPERIMENTAL_3
	Data::Point2D m = static_cast<const Data::Edge2D *>(shape)->mid2D();
#else
	Data::Point2D m = static_cast<const Data::Edge2D *>(shape)->mid();
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
	Data::Vector2D v(&m, this->point);
#else
	Data::Vector v(m, this->point);
#endif //#if USE_EXPERIMENTAL_3

	//v.normalize();

	Real dot = n.dot(v)/v.norm();

	return (dot > 0.0) ? angle : -angle;
}

void MJMesh::AnglePointInObjectDecider::decide()
{
	Real testAngle = ((this->dimension == 2) || (this->surfaceDirection < 3)) ? M_PI : 2.0*M_PI;

	if ((this->canBeUnknown) &&
		(((std::fabs(this->ang) > 0.1) && (std::fabs(this->ang) < 2.0*testAngle - 0.1)) ||
		 (std::fabs(this->ang) > 2*testAngle + 0.1)))
	{
		this->decision = Data::UNKNOWN;

		return;
	}

	this->decision = (std::fabs(this->ang) > testAngle) ? Data::IN : Data::OUT;
}

void MJMesh::AnglePointInObjectDecider::execute()
{
	if ((!this->fels) || (!this->point) ||
		((this->dimension != 2) && (this->dimension != 3)))
	{
		return;
	}

	this->ang = 0.0;

#if USE_EXACT
	Data::EReal ang = 0.0;

	for (Data::FrontElementList::const_iterator iter = this->fels->begin();
		 iter != this->fels->end(); iter++)
	{
		ang += this->angle((*iter)->getShape());
	}

	this->ang = ang;
#else
	Data::RealVector angles(this->fels->size(), 0.0);
	UInt i = 0;

	for (Data::FrontElementList::const_iterator iter = this->fels->begin();
		 iter != this->fels->end(); iter++, i++)
	{
		angles[i] = this->angle((*iter)->getShape());
	}

	std::sort(angles.begin(), angles.end(), smallerfabs);

	for (Data::RealVector::iterator iter = angles.begin();
		 iter != angles.end(); iter++)
	{
		this->ang += (*iter);
	}
#endif //#if USE_EXACT

	this->decide();
}
