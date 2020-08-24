#include "Data/Edge.h"

#include "Data/Identifiable.h"
#include "Data/State.h"

#if USE_EXPERIMENTAL_3
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL

using namespace Data;

Data::Edge::Edge(Point *p1, Point *p2) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	//this->points.resize(2, NULL);

	this->setPoints(p1, p2);
}

Data::Edge::Edge(const Point &p1, const Point &p2) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_EXPERIMENTAL_3
    UInt dim = p1.dimension();

	this->setPoints(
        (dim == 2) ? static_cast<Point *>(new Point2D(p1)) : static_cast<Point *>(new Point3D(p1)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p2)) : static_cast<Point *>(new Point3D(p2)));
#else
	this->setPoints(new Point(p1), new Point(p2));
#endif //#if USE_EXPERIMENTAL_3
}

Data::Edge::~Edge()
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Edge::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Edge::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Edge::numPoints() const
{
	return 2;
}

void Data::Edge::setPoints(Point *p1, Point *p2)
{
	this->setP1(p1);
	this->setP2(p2);
}

void Data::Edge::setP1(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Edge::getP1() const
{
	//return this->getPoint(0);
	return this->points[0];
}

void Data::Edge::setP2(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Edge::getP2() const
{
	//return this->getPoint(1);
	return this->points[1];
}

#if USE_EXPERIMENTAL
UInt Data::Edge::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
#endif //#if USE_EXPERIMENTAL

Real Data::Edge::length() const
{
#if USE_EXPERIMENTAL_3
    return (this->dimension() == 2) ?
        this->vector2D().norm() :
        this->vector3D().norm();
#else
	return this->vector().norm();
#endif //#if USE_EXPERIMENTAL_3
}

Real Data::Edge::size() const
{
	return this->length();
}

Real Data::Edge::orientedSize() const
{
	return this->size();
}

#if USE_EXPERIMENTAL_3
Vector2D Data::Edge::vector2D() const
{
    return Vector2D(this->getP1(), this->getP2());
}

Vector3D Data::Edge::vector3D() const
{
    /*if (this->dimension() == 2)
    {
        Vector3D v;

        v.setPosition(
            this->getP2()->getCoord(0) - this->getP1()->getCoord(0),
            this->getP2()->getCoord(1) - this->getP1()->getCoord(1),
            0.0)
    }*/

    return Vector3D(this->getP1(), this->getP2());
}
#else
Vector Data::Edge::vector() const
{
	return Vector(this->getP1(), this->getP2());
}
#endif //#if USE_EXPERIMENTAL_3

Real Data::Edge::height(const Point &p) const
{
	return this->height(&p);
}

Real Data::Edge::height(const Point *p) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Vector2D v = this->vector2D();

        v.multiply(v.dot(Vector2D(this->getP1(), p))/v.norm());

        Point2D q(this->getP1());

        q.sum(v);

        return p->distance(q);
    }

    Vector3D v = this->vector3D();

	v.multiply(v.dot(Vector3D(this->getP1(), p))/v.norm());

	Point3D q(this->getP1());

	q.sum(v);

	return p->distance(q);
#else
	Vector v = this->vector();

	v.multiply(v.dot(Vector(this->getP1(), p))/v.norm());

	Point q(this->getP1());

	q.sum(v);

	return p->distance(q);
#endif //#if USE_EXPERIMENTAL_3
}

Real Data::Edge::straightDistance(const Point &p) const
{
	return this->straightDistance(&p);
}

Real Data::Edge::straightDistance(const Point *p) const
{
	return this->height(p);
}

bool Data::Edge::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Edge::in(const Point *p) const
{
	Real d1 = p->distance(this->getP1());
	Real d2 = p->distance(this->getP2());

	return ((std::fabs(d1 + d2 - this->length()) < Data::getTolerance()));
}

bool Data::Edge::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Edge::on(const Point *p) const
{
	return ((p == this->getP1()) ||
			(p == this->getP2()) ||
			(p->equal(this->getP1())) ||
			(p->equal(this->getP2())));
}

bool Data::Edge::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Edge::out(const Point *p) const
{
	return !this->in(p);
}

Real Data::Edge::angle(const Point &p) const
{
	return this->angle(&p);
}

Real Data::Edge::angle(const Point *p) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Vector2D v1(p, this->getP1());
        Vector2D v2(p, this->getP2());

        return v1.angle(v2);
    }

    Vector3D v1(p, this->getP1());
	Vector3D v2(p, this->getP2());

	return v1.angle(v2);
#else
	Vector v1(p, this->getP1());
	Vector v2(p, this->getP2());

	return v1.angle(v2);
#endif //#if USE_EXPERIMENTAL_3
}

bool Data::Edge::match(const Edge &e) const
{
	return this->match(*(e.getP1()), *(e.getP2()));
}

bool Data::Edge::match(const Edge *e) const
{
	return this->match(e->getP1(), e->getP2());
}

bool Data::Edge::match(const Point &p1, const Point &p2) const
{
	return (((this->getP1()->match(p1)) && (this->getP2()->match(p2))) ||
			((this->getP1()->match(p2)) && (this->getP2()->match(p1))));
}

bool Data::Edge::match(const Point *p1, const Point *p2) const
{
	return (((this->getP1() == p1) && (this->getP2() == p2)) ||
			((this->getP1() == p2) && (this->getP2() == p1)));
}

bool Data::Edge::equal(const Edge &e) const
{
	return this->equal(&e);
}

bool Data::Edge::equal(const Edge *e) const
{
	return this->equal(e->getP1(), e->getP2());
}

bool Data::Edge::equal(const Point &p1, const Point &p2) const
{
	return (((this->getP1()->equal(p1)) && (this->getP2()->equal(p2))) ||
			((this->getP1()->equal(p2)) && (this->getP2()->equal(p1))));
}

bool Data::Edge::equal(const Point *p1, const Point *p2) const
{
	return ((this->match(p1, p2)) || (this->equal(*p1, *p2)));
}

#if USE_ARRAY
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
Edge &Data::Edge::operator=(const Edge &e)
{
	if (this != &e)
	{
#if USE_EXPERIMENTAL
#if (!USE_EXPERIMENTAL_4)
		this->numPts = 2;
		this->GeometricShape::points = this->points;
#endif //#if (!USE_EXPERIMENTAL_4)

		UInt dim = e.dimension();

		this->points[0] = (!e.points[0]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(e.points[0])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(e.points[0])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(e.points[0])) : new Point(e.points[0])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[1] = (!e.points[1]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(e.points[1])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(e.points[1])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(e.points[1])) : new Point(e.points[1])));
#endif //#if USE_EXPERIMENTAL_3
#else
		GeometricShape::operator=(e);
#endif //#if USE_EXPERIMENTAL
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Edge::highlight() const
{
	Colorable::highlight();
}

void Data::Edge::unhighlight() const
{
	Colorable::unhighlight();

	//this->setWidth(1.0);
}

//void Data::Edge::draw(bool /*fill*/) const
void Data::Edge::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
		else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		glBegin(GL_LINES);
			glVertex(this->getP1()->getCoord(0), this->getP1()->getCoord(1), this->getP1()->getCoord(2));
			glVertex(this->getP2()->getCoord(0), this->getP2()->getCoord(1), this->getP2()->getCoord(2));
		glEnd();
	glPopMatrix();
}

UInt Data::Edge::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if ((this->dimension() != 2) && (this->dimension() != 3))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = static_cast<GLfloat>(this->getB());
	}

	if (this->dimension() == 2)
	{
		coord[0] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getY());

		coord[2] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getX());
		coord[3] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getY());

#if USE_EXPERIMENTAL_3
		Point2D mid = this->mid2D();
#else
		Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

		this->explodeAndShrink(coord, mid);
		this->explodeAndShrink(coord + 2, mid);
	}
	else
	{
		coord[0] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getY());
		coord[2] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getZ());

		coord[3] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getX());
		coord[4] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getY());
		coord[5] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getZ());

#if USE_EXPERIMENTAL_3
		Point3D mid = this->mid3D();
#else
		Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

		this->explodeAndShrink(coord, mid);
		this->explodeAndShrink(coord + 3, mid);
	}

	return 2;
}

std::string Data::Edge::text() const
{
	std::stringstream str;

	if (this->getP1())
	{
		str << this->getP1()->text();
	}
	else
	{
		str << "-1";
	}

	str << " ";

	if (this->getP2())
	{
		str << this->getP2()->text();
	}
	else
	{
		str << "-1";
	}

	return str.str();
}
#endif //#if USE_GUI
