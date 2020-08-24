#include "Performer/Clipper.h"

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Edge.h"
#include "Data/Polygon.h"
#include "Data/Polyhedron.h"

using namespace Performer;

Performer::Clipper::Clipper(UInt dimension, const Data::Box &box,
	const Data::GeometricShape *s, enum Type type) :
	Builder()
{
	this->setDimension(dimension);

	this->setBox(box);

	this->setShape(s, type);
}

Performer::Clipper::Clipper(UInt dimension, const Data::Point &min,
	const Data::Point &max, const Data::GeometricShape *s, enum Type type) :
	Builder()
{
	this->setDimension(dimension);

	this->setBox(min, max);

	this->setShape(s, type);
}

Performer::Clipper::~Clipper()
{

}

void Performer::Clipper::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void Performer::Clipper::setBox(const Data::Box &box)
{
#if USE_EXPERIMENTAL_3
	this->setBox(*box.getMin(), *box.getMax());
#else
	this->setBox(box.getMin(), box.getMax());
#endif //#if USE_EXPERIMENTAL_3
}

void Performer::Clipper::setBox(const Data::Point &min, const Data::Point &max)
{
	this->box.setBox(min, max);
}

void Performer::Clipper::setShape(const Data::GeometricShape *s, enum Type type)
{
	this->type = type;
	this->shape = s;
}

void Performer::Clipper::setShape(const Data::Edge *e)
{
	this->setShape(e, Clipper::EDGE);
}

void Performer::Clipper::setShape(const Data::Polygon *p)
{
	this->setShape(p, Clipper::POLYGON);
}

void Performer::Clipper::setShape(const Data::Polyhedron *p)
{
	this->setShape(p, Clipper::POLYHEDRON);
}

#if USE_EXPERIMENTAL_3
const Data::Point2DObjList &Performer::Clipper::getPoints2D() const
{
	return this->points2D;
}

const Data::Point3DObjList &Performer::Clipper::getPoints3D() const
{
	return this->points3D;
}
#else
const Data::PointObjList &Performer::Clipper::getPoints() const
{
	return this->points;
}
#endif //#if USE_EXPERIMENTAL_3

void Performer::Clipper::execute()
{
	if ((!this->shape) || (this->type == Clipper::UNKNOWN))
	{
		return;
	}

	if ((this->dimension != 2) && (this->dimension != 3))
	{
		return;
	}

	///TODO clip in polyhedra
	if (this->type == Clipper::POLYHEDRON)
	{
		return;
	}

#if USE_EXPERIMENTAL_3
	this->points2D.clear();
	this->points3D.clear();
#else
	this->points.clear();
#endif //#if USE_EXPERIMENTAL_3

	this->clip();
}

Real Performer::Clipper::interpolate(Real val1, Real val2, Real val) const
{
	return (val - val1)/(val2 - val1);
}

Data::Point *Performer::Clipper::interpolate(UInt coord, const Data::Point &p1,
	const Data::Point &p2, Real val) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension == 2)
    {
        Data::Vector2D v(p1, p2);

        v.multiply(this->interpolate(p1.getCoord(coord), p2.getCoord(coord), val));

        v.sum(p1);

        return new Data::Point2D(v);
    }

    Data::Vector3D v(p1, p2);

	v.multiply(this->interpolate(p1.getCoord(coord), p2.getCoord(coord), val));

	v.sum(p1);

	return new Data::Point3D(v);
#else
	Data::Vector v(p1, p2);

	v.multiply(this->interpolate(p1.getCoord(coord), p2.getCoord(coord), val));

	v.sum(p1);

	return (this->dimension == 2) ?
		static_cast<Data::Point *>(new Data::Point2D(v)) :
		static_cast<Data::Point *>(new Data::Point3D(v));
#endif //#if USE_EXPERIMENTAL_3
}

enum Data::Position Performer::Clipper::clipMin(UInt coord, const Data::Point &p, Real val) const
{
	Real pcoord = p.getCoord(coord);

	return (pcoord < val - Data::getTolerance()) ? Data::OUT :
		   ((pcoord > val + Data::getTolerance()) ? Data::IN : Data::ON);

	/*if (p.getCoord(coord) < val - Data::getTolerance())
	{
		return Data::OUT;
	}

	if (p.getCoord(coord) > val + Data::getTolerance())
	{
		return Data::IN;
	}

	return Data::ON;*/
}

enum Data::Position Performer::Clipper::clipMax(UInt coord, const Data::Point &p, Real val) const
{
	Real pcoord = p.getCoord(coord);

	return (pcoord > val + Data::getTolerance()) ? Data::OUT :
		   ((pcoord < val - Data::getTolerance()) ? Data::IN : Data::ON);

	/*if (p.getCoord(coord) > val + Data::getTolerance())
	{
		return Data::OUT;
	}

	if (p.getCoord(coord) < val - Data::getTolerance())
	{
		return Data::IN;
	}

	return Data::ON;*/
}

enum Data::Position Performer::Clipper::clip(UInt coord, const Data::Point &p, Real val, bool min) const
{
	return (min) ? this->clipMin(coord, p, val) : this->clipMax(coord, p, val);
}

Data::Point *Performer::Clipper::clip(UInt coord, const Data::Point &p1,
	const Data::Point &p2, Real val,
	enum Data::Position pos1, enum Data::Position pos2) const
{
	return (((pos1 == Data::OUT) && (pos2 == Data::OUT)) ||
			((pos1 != Data::OUT) && (pos2 != Data::OUT)) ||
			((pos1 == Data::ON ) || (pos2 == Data::ON ))) ?
			NULL :
			this->interpolate(coord, p1, p2, val);

	///leave this commented code here for the sake of documentation
	/*//both points are out
	if ((pos1 == Data::OUT) &&
		(pos2 == Data::OUT))
	{
		return NULL;
	}

	//both points are either in or on
	if ((pos1 != Data::OUT) &&
		(pos2 != Data::OUT))
	{
		return NULL;
	}

	//one is out and the other is either in on on

	//one of the points is on (consequently, the other is out)
	if ((pos1 == Data::ON) ||
		(pos2 == Data::ON))
	{
		return NULL;
	}

	//one point is out and the other is in
	return this->interpolate(coord, p1, p2, val);*/
}

void Performer::Clipper::clip(UInt coord, Real val, bool min)
{
	if (this->type == Clipper::EDGE)
	{
#if USE_EXPERIMENTAL_3
		Data::Point2D p1 = this->points2D.front();
#else
		Data::Point p1 = this->points.front();
#endif //#if USE_EXPERIMENTAL_3

		enum Data::Position pos1 = this->clip(coord, p1, val, min);

		if (pos1 == Data::OUT)
		{
#if USE_EXPERIMENTAL_3
			this->points2D.pop_front();
#else
			this->points.pop_front();
#endif //#if USE_EXPERIMENTAL_3
		}

#if USE_EXPERIMENTAL_3
		if (!this->points2D.empty())
#else
		if (!this->points.empty())
#endif //#if USE_EXPERIMENTAL_3
		{
#if USE_EXPERIMENTAL_3
			Data::Point2D p2 = this->points2D.back();
#else
			Data::Point p2 = this->points.back();
#endif //#if USE_EXPERIMENTAL_3

			enum Data::Position pos2 = this->clip(coord, p2, val, min);

			if (pos2 == Data::OUT)
			{
#if USE_EXPERIMENTAL_3
				this->points2D.pop_back();
#else
				this->points.pop_back();
#endif //#if USE_EXPERIMENTAL_3
			}

#if USE_EXPERIMENTAL_3
			if (!this->points2D.empty())
#else
			if (!this->points.empty())
#endif //#if USE_EXPERIMENTAL_3
			{
				Data::Point *p = this->clip(coord, p1, p2, val, pos1, pos2);

				if (p)
				{
					if (pos1 == Data::OUT)
					{
#if USE_EXPERIMENTAL_3
						this->points2D.push_front(*p);
#else
						this->points.push_front(*p);
#endif //#if USE_EXPERIMENTAL_3
					}
					else
					{
#if USE_EXPERIMENTAL_3
						this->points2D.push_back(*p);
#else
						this->points.push_back(*p);
#endif //#if USE_EXPERIMENTAL_3
					}

					delete p;
				}
			}
		}

		/*//both points are either in or on
		if ((pos1 != Clipper::OUT) &&
			(pos2 != Clipper::OUT))
		{
			return;
		}

		//both points are out
		if ((pos1 == Clipper::OUT) &&
			(pos2 == Clipper::OUT))
		{
			this->points.clear();

			return;
		}

		//one is out and the other is either in on on

		if (pos1 == Clipper::OUT)
		{
			this->points.pop_front();

			//the first point is out and the second is on
			if (pos2 == Clipper::ON)
			{
				return;
			}
		}

		if (pos2 == Clipper::OUT)
		{
			this->points.pop_back();

			//the second point is out and the first os on
			if (pos1 == Clipper::ON)
			{
				return;
			}
		}

		//one point is out and the other is in
		Data::Point *p = this->interpolate(coord, p1, p2, val);

		if (pos1 == Clipper::OUT)
		{
			this->points.push_front(*p);
		}
		else
		{
			this->points.push_back(*p);
		}

		delete p;*/
	}
	else if (this->type == Clipper::POLYGON)
	{
		Data::PointVector newPoints;
		PositionVector positions;

#if USE_EXPERIMENTAL_3
		Data::Point3D p1 = this->points3D.front();
#else
		Data::Point p1 = this->points.front();
#endif //#if USE_EXPERIMENTAL_3
		UInt prev = 0;

		positions.push_back(this->clip(coord, p1, val, min));

#if USE_EXPERIMENTAL_3
		for (Data::Point3DObjList::iterator iter = ++this->points3D.begin();
			 iter != this->points3D.end(); iter++, prev++)
#else
		for (Data::PointObjList::iterator iter = ++this->points.begin();
			 iter != this->points.end(); iter++, prev++)
#endif //#if USE_EXPERIMENTAL_3
		{
			//Data::Point p2 = (*iter);

			//positions.push_back(((prev + 1) == this->points.size()) ? positions[0] : this->clip(coord, p2, val, min));
#if USE_EXPERIMENTAL_3
			positions.push_back(((prev + 1) == this->points3D.size()) ? positions[0] : this->clip(coord, (*iter), val, min));
#else
			positions.push_back(((prev + 1) == this->points.size()) ? positions[0] : this->clip(coord, (*iter), val, min));
#endif //#if USE_EXPERIMENTAL_3

			enum Data::Position pos1 = positions[prev];
			enum Data::Position pos2 = positions[prev + 1];

			//newPoints.push_back(this->clip(coord, p1, p2, val, pos1, pos2));
			newPoints.push_back(this->clip(coord, p1, (*iter), val, pos1, pos2));

			//p1 = p2;
			p1 = (*iter);

			/*//both points are either in or on
			if ((pos1 != Clipper::OUT) &&
				(pos2 != Clipper::OUT))
			{
				newPoints.push_back(NULL);

				continue;
			}

			//both points are out
			if ((pos1 == Clipper::OUT) &&
				(pos2 == Clipper::OUT))
			{
				newPoints.push_back(NULL);

				continue;
			}

			//one is out and the other is either in on on

			if (pos1 == Clipper::OUT)
			{
				//the first point is out and the second is on
				if (pos2 == Clipper::ON)
				{
					newPoints.push_back(NULL);

					continue;
				}
			}

			if (pos2 == Clipper::OUT)
			{
				//the second point is out and the first os on
				if (pos1 == Clipper::ON)
				{
					newPoints.push_back(NULL);

					continue;
				}
			}

			//one point is out and the other is in
			Data::Point *p = this->interpolate(coord, p1, p2, val);

			newPoints.push_back(p);*/
		}

#if USE_EXPERIMENTAL_3
		Data::Point3DObjList::iterator iter = this->points3D.begin();
#else
		Data::PointObjList::iterator iter = this->points.begin();
#endif //#if USE_EXPERIMENTAL_3

		for (UInt i = 0; i < positions.size() - 1; i++)
		{
			if (newPoints[i])
			{
				iter++;
#if USE_EXPERIMENTAL_3
				iter = this->points3D.insert(iter, *newPoints[i]);
#else
				iter = this->points.insert(iter, *newPoints[i]);
#endif //#if USE_EXPERIMENTAL_3
				iter--;
			}

			if (positions[i] == Data::OUT)
			{
#if USE_EXPERIMENTAL_3
				iter = this->points3D.erase(iter);
#else
				iter = this->points.erase(iter);
#endif //#if USE_EXPERIMENTAL_3
			}
			else
			{
				iter++;
			}

			if (newPoints[i])
			{
				iter++;
			}
		}

		if (positions.back() == Data::OUT)
		{
#if USE_EXPERIMENTAL_3
			this->points3D.pop_back();
#else
			this->points.pop_back();
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
			if (!this->points3D.empty())
#else
			if (!this->points.empty())
#endif //#if USE_EXPERIMENTAL_3
			{
#if USE_EXPERIMENTAL_3
				this->points3D.push_back(this->points3D.front());
#else
				this->points.push_back(this->points.front());
#endif //#if USE_EXPERIMENTAL_3
			}
		}

		while (!newPoints.empty())
		{
			Data::Point *p = newPoints.back();

			newPoints.pop_back();

			if (p)
			{
				delete p;
			}
		}
	}
}

void Performer::Clipper::clip()
{
	for (UInt i = 0; i < this->shape->numPoints(); i++)
	{
		if (this->dimension == 2)
		{
			Data::Point2D p(this->shape->getPoint(i));

#if USE_EXPERIMENTAL_3
			this->points2D.push_back(p);
#else
			this->points.push_back(p);
#endif //#if USE_EXPERIMENTAL_3
		}
		else
		{
			Data::Point3D p(this->shape->getPoint(i));

#if USE_EXPERIMENTAL_3
			this->points3D.push_back(p);
#else
			this->points.push_back(p);
#endif //#if USE_EXPERIMENTAL_3
		}
	}

	if (this->type == Clipper::POLYGON)
	{
#if USE_EXPERIMENTAL_3
		this->points3D.push_back(this->points3D.front());
#else
		this->points.push_back(this->points.front());
#endif //#if USE_EXPERIMENTAL_3
	}

	for (UInt i = 0; i < this->dimension; i++)
	{
		this->clip(i, this->box.getMin()->getCoord(i), true);

#if USE_EXPERIMENTAL_3
		if ((this->dimension == 2) ? this->points2D.empty() : this->points3D.empty())
#else
		if (this->points.empty())
#endif //#if USE_EXPERIMENTAL_3
		{
			break;
		}

		this->clip(i, this->box.getMax()->getCoord(i), false);

#if USE_EXPERIMENTAL_3
		if ((this->dimension == 2) ? this->points2D.empty() : this->points3D.empty())
#else
		if (this->points.empty())
#endif //#if USE_EXPERIMENTAL_3
		{
			break;
		}
	}

#if USE_EXPERIMENTAL_3
	if ((this->type == Clipper::POLYGON) && (!this->points3D.empty()))
#else
	if ((this->type == Clipper::POLYGON) && (!this->points.empty()))
#endif //#if USE_EXPERIMENTAL_3
	{
#if USE_EXPERIMENTAL_3
		this->points3D.pop_back();
#else
		this->points.pop_back();
#endif //#if USE_EXPERIMENTAL_3
	}
}
