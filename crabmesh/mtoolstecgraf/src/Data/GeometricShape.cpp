#include "Data/GeometricShape.h"

#include "Data/Box.h"

#if USE_EXPERIMENTAL_3
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

using namespace Data;

Data::GeometricShape::GeometricShape()
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if (!USE_EXPERIMENTAL_4)
#if USE_ARRAY
	this->numPts = 0;
	this->points = NULL;
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if (!USE_EXPERIMENTAL_4)
Data::GeometricShape::GeometricShape(UInt numPoints)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->numPts = static_cast<UShort>(numPoints);
	this->points = new Point*[this->numPts];
#else
	this->points.resize(numPoints, NULL);
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_4)

Data::GeometricShape::~GeometricShape()
{
	/*while (!this->points.empty())
	{
		if (this->points.back())
		{
			delete this->points.back();
		}

		this->points.pop_back();
	}*/

	//this->points.clear();

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_4)
	if (this->points)
	{
		delete [] this->points;
	}
#endif //#if (!USE_EXPERIMENTAL_4)
#endif //#if USE_ARRAY
}

#if (!USE_EXPERIMENTAL_4)
void Data::GeometricShape::setPoint(UInt i, Point *p)
{
	//if (i < this->numPoints())
	{
		this->points[i] = p;
	}
}

Point *Data::GeometricShape::getPoint(UInt i) const
{
	//return (i < this->numPoints()) ? this->points[i] : NULL;
	return this->points[i];
}
#endif //#if (!USE_EXPERIMENTAL_4)

#if (!USE_EXPERIMENTAL_4)
UInt Data::GeometricShape::numPoints() const
{
#if USE_ARRAY
	return this->numPts;
#else
	return this->points.size();
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_4)

UInt Data::GeometricShape::dimension() const
{
#if USE_ARRAY
	/*if (!this->points)
	{
		return 0;
	}*/

#if USE_EXPERIMENTAL_4
	return ((this->numPoints() == 0) || (!this->getPoint(0))) ? 0 : this->getPoint(0)->dimension();
#else
	return (!this->points) ? 0 : this->points[0]->dimension();
#endif //#if USE_EXPERIMENTAL_4
#else

	/*UInt dim = 0;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (this->points[i]->dimension() > dim)
		{
			dim = this->points[i]->dimension();
		}
	}

	return dim;*/

	return this->points[0]->dimension();
#endif //#if USE_ARRAY
}

Real Data::GeometricShape::size() const
{
	return std::fabs(this->orientedSize());
}

#if USE_EXPERIMENTAL_3
Point2D Data::GeometricShape::mid2D() const
{
    Vector2D v;

    for (UInt i = 0; i < this->numPoints(); i++)
    {
#if USE_EXPERIMENTAL_4
        v.sum(this->getPoint(i));
#else
        v.sum(this->points[i]);
#endif //#if USE_EXPERIMENTAL_4
    }

    v.multiply(1.0/static_cast<Real>(this->numPoints()));

    return Point2D(v);
}

Point3D Data::GeometricShape::mid3D() const
{
    Vector3D v;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		v.sum(this->getPoint(i));
#else
		v.sum(this->points[i]);
#endif //#if USE_EXPERIMENTAL_4
	}

	v.multiply(1.0/static_cast<Real>(this->numPoints()));

	return Point3D(v);
}
#else
Point Data::GeometricShape::mid() const
{
	Vector v(this->dimension());

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		v.sum(this->points[i]);
	}

	v.multiply(1.0/static_cast<Real>(this->numPoints()));

	return Point(v);
}
#endif //#if USE_EXPERIMENTAL_3

Box Data::GeometricShape::box() const
{
	UInt dimension = this->dimension();

#if USE_EXPERIMENTAL_3
	Point2D min2d, max2d;
	Point3D min3d, max3d;

	Point &min = (dimension == 2) ? static_cast<Point &>(min2d) : static_cast<Point &>(min3d);
	Point &max = (dimension == 2) ? static_cast<Point &>(max2d) : static_cast<Point &>(max3d);
#else
	Point min(dimension);
	Point max(dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < dimension; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		for (UInt j = 0; j < dimension; j++)
		{
#if USE_EXPERIMENTAL_4
			Real coord = this->getPoint(i)->getCoord(j);
#else
			Real coord = this->points[i]->getCoord(j);
#endif //#if USE_EXPERIMENTAL_4

			if (coord < min.getCoord(j))
			{
				min.setCoord(j, coord);
			}

			if (coord > max.getCoord(j))
			{
				max.setCoord(j, coord);
			}
		}
	}

	return Box(min, max);
}

bool Data::GeometricShape::haveMatching(const Point &p) const
{
    for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		if (p.match(this->getPoint(i)))
#else
		if (p.match(this->points[i]))
#endif //#if USE_EXPERIMENTAL_4
		{
			return true;
		}
	}

	return false;
}

bool Data::GeometricShape::have(const Point &p) const
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		if (p.equal(this->getPoint(i)))
#else
		if (p.equal(this->points[i]))
#endif //#if USE_EXPERIMENTAL_4
		{
			return true;
		}
	}

	return false;
}

bool Data::GeometricShape::have(const Point *p) const
{
	//return std::find(this->points.begin(), this->points.end(), p) != this->points.end();

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		if (this->getPoint(i) == p)
#else
		if (this->points[i] == p)
#endif //#if USE_EXPERIMENTAL_4
		{
			return true;
		}
	}

	return false;
}

bool Data::GeometricShape::match(const GeometricShape &s) const
{
	if (this->numPoints() != s.numPoints())
	{
		return false;
	}

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		if ((this->getPoint(i) != s.getPoint(i)) &&
			(!this->getPoint(i)->match(s.getPoint(i))))
#else
		if ((this->points[i] != s.points[i]) &&
			(!this->points[i]->match(s.points[i])))
#endif //#if USE_EXPERIMENTAL_4
		{
			return false;
		}
	}

	return true;
}

bool Data::GeometricShape::match(const GeometricShape *s) const
{
	return ((this == s) || (this->match(*s)));
}

bool Data::GeometricShape::equal(const GeometricShape &s) const
{
	if (this->numPoints() != s.numPoints())
	{
		return false;
	}

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		bool found = false;

		for (UInt j = 0; j < s.numPoints(); j++)
		{
#if USE_EXPERIMENTAL_4
			if ((this->getPoint(i) == s.getPoint(j)) ||
				(this->getPoint(i)->equal(s.getPoint(j))))
#else
			if ((this->points[i] == s.points[j]) ||
				(this->points[i]->equal(s.points[j])))
#endif //#if USE_EXPERIMENTAL_4
			{
				found = true;

				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}

bool Data::GeometricShape::equal(const GeometricShape *s) const
{
	return ((this == s) || (this->equal(*s)));
}

bool Data::GeometricShape::smaller(const GeometricShape *s) const
{
	return (this->size() < s->size());
}

bool Data::GeometricShape::larger(const GeometricShape *s) const
{
	return (this->size() > s->size());
}

bool Data::GeometricShape::translate(const Vector &v)
{
	return this->translate(&v);
}

bool Data::GeometricShape::translate(const Vector *v)
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		this->getPoint(i)->sum(v);
#else
		this->points[i]->sum(v);
#endif //#if USE_EXPERIMENTAL_4
	}

	return true;
}

bool Data::GeometricShape::scale(const Vector &v)
{
	return this->scale(&v);
}

bool Data::GeometricShape::scale(const Vector *v)
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
		for (UInt j = 0; j < v->dimension(); j++)
		{
#if USE_EXPERIMENTAL_4
			this->getPoint(i)->setCoord(j, this->getPoint(i)->getCoord(j)*v->getCoord(j));
#else
			this->points[i]->setCoord(j, this->points[i]->getCoord(j)*v->getCoord(j));
#endif //#if USE_EXPERIMENTAL_4
		}
	}

	return true;
}

bool Data::GeometricShape::scale(Real s)
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		for (UInt j = 0; j < this->getPoint(i)->dimension(); j++)
#else
		for (UInt j = 0; j < this->points[i]->dimension(); j++)
#endif //#if USE_EXPERIMENTAL_4
		{
#if USE_EXPERIMENTAL_4
			this->getPoint(i)->setCoord(j, this->getPoint(i)->getCoord(j)*s);
#else
			this->points[i]->setCoord(j, this->points[i]->getCoord(j)*s);
#endif //#if USE_EXPERIMENTAL_4
		}
	}

	return true;
}

bool Data::GeometricShape::localScale(const Vector &v)
{
	return this->localScale(&v);
}

bool Data::GeometricShape::localScale(const Vector *v)
{
#if USE_EXPERIMENTAL_3
	UInt dimension = this->dimension();

	Point2D m2d;
	Point3D m3d;

	Point &m = (dimension == 2) ? static_cast<Point &>(m3d) : static_cast<Point &>(m2d);

	VECTORp(mid, dimension, m);
#else
    Data::Vector mid = this->mid();
#endif

	mid.invert();

	if (!this->translate(mid))
	{
		return false;
	}

	bool result = this->scale(v);

	mid.invert();

	this->translate(mid);

	return result;
}

bool Data::GeometricShape::localScale(Real s)
{
#if USE_EXPERIMENTAL_3
	UInt dimension = this->dimension();

	Point2D m2d;
	Point3D m3d;

	Point &m = (dimension == 2) ? static_cast<Point &>(m3d) : static_cast<Point &>(m2d);

	VECTORp(mid, dimension, m);
#else
    Data::Vector mid = this->mid();
#endif

	mid.invert();

	if (!this->translate(mid))
	{
		return false;
	}

	bool result = this->scale(s);

	mid.invert();

	this->translate(mid);

	return result;
}

#if (!USE_EXPERIMENTAL_4)
void Data::GeometricShape::add(Point *p)
{
#if USE_ARRAY
	if (!this->points)
	{
		this->points = new Point*[1];

		this->points[0] = p;

		this->numPts = 1;

		return;
	}

	Point **points = this->points;

	this->points = new Point*[this->numPts + 1];

	for (UInt i = 0; i < this->numPts; i++)
	{
		this->points[i] = points[i];
	}

	delete [] points;

	this->points[this->numPts++] = p;
#else
	this->points.push_back(p);
#endif //#if USE_ARRAY
}

void Data::GeometricShape::remove(Point *p)
{
#if USE_ARRAY
	if (!this->points)
	{
		return;
	}

	UInt found = this->numPts;

	for (UInt i = 0; i < this->numPts; i++)
	{
		if (this->points[i] == p)
		{
			found = i;

			break;
		}
	}

	if (found < this->numPts)
	{
		Point **points = this->points;

		this->points = new Point*[this->numPts - 1];

		for (UInt i = 0; i < found; i++)
		{
			this->points[i] = points[i];
		}

		for (UInt i = found; i < static_cast<UShort>(this->numPts - 1); i++)
		{
			this->points[i] = points[i + 1];
		}

		delete [] points;

		this->numPts--;
	}
#else
	PointVector::iterator it = std::find(this->points.begin(), this->points.end(), p);

	if (it != this->points.end())
	{
		this->points.erase(it);
	}
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_4)
GeometricShape &Data::GeometricShape::operator=(const GeometricShape &s)
{
	if (this != &s)
	{
		if (this->numPts != s.numPts)
		{
			if (this->points)
			{
				delete [] this->points;
			}

			this->numPts = s.numPts;

			this->points = new Point*[this->numPts];
		}

		/*if (this->points)
		{
			delete [] this->points;
		}

		this->numPts = s.numPts;

		this->points = new Point*[this->numPts];*/

		for (UInt i = 0; i < this->numPts; i++)
		{
			this->points[i] = s.points[i];
		}

#if USE_GUI
		this->setColor(s.r, s.g, s.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if (!USE_EXPERIMENTAL_4)
#endif //#if USE_ARRAY

#if USE_GUI
void Data::GeometricShape::highlight() const
{
	Colorable::highlight();

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		this->getPoint(i)->highlight();
#else
		this->points[i]->highlight();
#endif //#if USE_EXPERIMENTAL_4
	}
}

void Data::GeometricShape::unhighlight() const
{
	Colorable::unhighlight();

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_4
		this->getPoint(i)->unhighlight();
#else
		this->points[i]->unhighlight();
#endif //#if USE_EXPERIMENTAL_4
	}
}

/*void Data::GeometricShape::draw() const
{
	this->draw(true);
}*/

bool Data::GeometricShape::isInVisibleSpace() const
{
	return this->isBoxInVisibleSpace();
}

bool Data::GeometricShape::isBoxInVisibleSpace() const
{
	Box box = this->box();

	Point *min = Data::getState().isPlaneInclusive() ? box.getMin() : box.getMax();
	Point *max = Data::getState().isPlaneInclusive() ? box.getMax() : box.getMin();

	for (UInt i = 0; i < 3; i++)
	{
		if (((Data::getState().isPlaneEnabled(2*i    )) && (min->getCoord(i) > Data::getState().getPlaneCoord(2*i    ))) ||
			((Data::getState().isPlaneEnabled(2*i + 1)) && (max->getCoord(i) < Data::getState().getPlaneCoord(2*i + 1))))
		{
			return false;
		}
	}

	return true;
}

bool Data::GeometricShape::isPointInVisibleSpace() const
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (Data::getState().isInVisibleSpace(this->getPoint(i)))
		{
			return true;
		}
	}

	return false;
}

bool Data::GeometricShape::arePointsInVisibleSpace() const
{
	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (!Data::getState().isInVisibleSpace(this->getPoint(i)))
		{
			return false;
		}
	}

	return true;
}
#endif //#if USE_GUI
