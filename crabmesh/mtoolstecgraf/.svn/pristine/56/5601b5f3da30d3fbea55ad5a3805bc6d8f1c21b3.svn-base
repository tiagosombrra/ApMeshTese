#include "Data/Point2D.h"

#include "Data/Vector2D.h"
#include "Data/State.h"
#include "Data/Identifiable.h"

using namespace Data;

Data::Point2D::Point2D(Real x, Real y) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(x, y);
}

Data::Point2D::Point2D(const Point2D &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1));
}

Data::Point2D::Point2D(const Point &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1));
}

Data::Point2D::Point2D(const Point *p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0), p->getCoord(1));
}

Data::Point2D::Point2D(const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1));
}

Data::Point2D::Point2D(const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v->getCoord(0), v->getCoord(1));
}

Data::Point2D::Point2D(const Point &p, const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0) + v.getCoord(0), p.getCoord(1) + v.getCoord(1));
}

Data::Point2D::Point2D(const Point *p, const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0) + v->getCoord(0), p->getCoord(1) + v->getCoord(1));
}

Data::Point2D::~Point2D()
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 0;
	this->Point::coords = NULL;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)
}

void Data::Point2D::setPosition(Real x, Real y)
{
	this->setX(x);
	this->setY(y);
}

void Data::Point2D::setX(Real x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

Real Data::Point2D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::Point2D::setY(Real y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

Real Data::Point2D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

UInt Data::Point2D::dimension() const
{
	return 2;
}

#if USE_EXPERIMENTAL_3
void Data::Point2D::setCoords(const Real coords[])
{
    for (UInt i = 0; i < 2; i++)
    {
        this->coords[i] = coords[i];
    }
}

void Data::Point2D::setCoords(const RealVector &coords)
{
    for (UInt i = 0; i < 2; i++)
    {
        this->coords[i] = coords[i];
    }
}

RealVector Data::Point2D::getCoords() const
{
    return RealVector(this->coords, this->coords + 2);
}

const Real *Data::Point2D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::Point2D::setCoord(UInt i, Real c)
{
    this->coords[i] = c;
}

Real Data::Point2D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

Real Data::Point2D::surface(const Point2D &p1, const Point2D &p2) const
{
	return this->surface(&p1, &p2);
}

Real Data::Point2D::surface(const Point2D *p1, const Point2D *p2) const
{
	return std::fabs(this->orientedSurface(p1, p2));
}

Real Data::Point2D::orientedSurface(const Point2D &p1, const Point2D &p2) const
{
	return this->orientedSurface(&p1, &p2);
}

Real Data::Point2D::orientedSurface(const Point2D *p1, const Point2D *p2) const
{
	Vector2D ab(this, p1);
	Vector2D ac(this, p2);

	return 0.5*ab.cross(ac);
}

bool Data::Point2D::clockWise(const Point2D &p1, const Point2D &p2) const
{
	return this->clockWise(&p1, &p2);
}

bool Data::Point2D::clockWise(const Point2D *p1, const Point2D *p2) const
{
	return (this->orientedSurface(p1, p2) > Data::getTolerance());
}

bool Data::Point2D::counterClockWise(const Point2D &p1, const Point2D &p2) const
{
	return this->counterClockWise(&p1, &p2);
}

bool Data::Point2D::counterClockWise(const Point2D *p1, const Point2D *p2) const
{
	return (this->orientedSurface(p1, p2) < -Data::getTolerance());
}

/*bool Data::Point2D::match(const Point2D &p) const
{
	return Point::match(p);
}

bool Data::Point2D::match(const Point2D *p) const
{
	return Point::match(p);
}*/

bool Data::Point2D::match(Real x, Real y) const
{
	return this->match(Point2D(x, y));
}

bool Data::Point2D::equal(Real x, Real y) const
{
	return Point::equal(Point2D(x, y));
}

#if USE_EXACT
EPoint2D Data::Point2D::epoint2D() const
{
	EPoint2D pt;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		pt.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	//pt.setSize(this->getSize());

	pt.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		pt.highlight();
	}
#endif //#if USE_GUI

	return pt;
}
#endif //#if USE_EXACT

#if USE_ARRAY
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
Point2D &Data::Point2D::operator=(const Point2D &p)
{
	if (this != &p)
	{
#if (!USE_EXPERIMENTAL_3)
		this->dim = 2;
		this->Point::coords = this->coords;
#endif //#if (!USE_EXPERIMENTAL_3)

		this->coords[0] = p.coords[0];
		this->coords[1] = p.coords[1];

#if USE_GUI
		this->setColor(p.r, p.g, p.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))

#if USE_EXPERIMENTAL_3
Point2D &Data::Point2D::operator=(const Point &p)
{
	if (this != &p)
	{
		this->coords[0] = p.getCoord(0);
		this->coords[1] = p.getCoord(1);

#if USE_GUI
		this->setColor(p.getR(), p.getG(), p.getB());
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Point2D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
		this->explodeAndShrink(this, true);

		glBegin(GL_POINTS);
			//glVertex(this->coords[0], this->coords[1]);
			glVertex(0.0, 0.0);
		glEnd();
	glPopMatrix();
}

UInt Data::Point2D::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = static_cast<GLfloat>(this->getR());
		color[1] = static_cast<GLfloat>(this->getG());
		color[2] = static_cast<GLfloat>(this->getB());
	}

	coord[0] = static_cast<GLfloat>(0.0);
	coord[1] = static_cast<GLfloat>(0.0);

	this->explodeAndShrink(coord, *this, true);

	return 1;
}
#endif //#if USE_GUI

#if USE_EXACT

Data::EPoint2D::EPoint2D(EReal x, EReal y) :
	EPoint(2)
{
	this->setPosition(x, y);
}

Data::EPoint2D::EPoint2D(const EPoint &p) :
	EPoint(2)
{
	this->setPosition(p.getCoord(0), p.getCoord(1));
}

Data::EPoint2D::EPoint2D(const EPoint *p) :
	EPoint(2)
{
	this->setPosition(p->getCoord(0), p->getCoord(1));
}

Data::EPoint2D::EPoint2D(const EVector &v) :
	EPoint(2)
{
	this->setPosition(v.getCoord(0), v.getCoord(1));
}

Data::EPoint2D::EPoint2D(const EVector *v) :
	EPoint(2)
{
	this->setPosition(v->getCoord(0), v->getCoord(1));
}

Data::EPoint2D::EPoint2D(const EPoint &p, const EVector &v) :
	EPoint(2)
{
	this->setPosition(p.getCoord(0) + v.getCoord(0), p.getCoord(1) + v.getCoord(1));
}

Data::EPoint2D::EPoint2D(const EPoint *p, const EVector *v) :
	EPoint(2)
{
	this->setPosition(p->getCoord(0) + v->getCoord(0), p->getCoord(1) + v->getCoord(1));
}

Data::EPoint2D::~EPoint2D()
{

}

void Data::EPoint2D::setPosition(EReal x, EReal y)
{
	this->setX(x);
	this->setY(y);
}

void Data::EPoint2D::setX(EReal x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

EReal Data::EPoint2D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::EPoint2D::setY(EReal y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

EReal Data::EPoint2D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

UInt Data::EPoint2D::dimension() const
{
	return 2;
}

EReal Data::EPoint2D::surface(const EPoint2D &p1, const EPoint2D &p2) const
{
	return this->surface(&p1, &p2);
}

EReal Data::EPoint2D::surface(const EPoint2D *p1, const EPoint2D *p2) const
{
	return std::fabs(this->orientedSurface(p1, p2));
}

EReal Data::EPoint2D::orientedSurface(const EPoint2D &p1, const EPoint2D &p2) const
{
	return this->orientedSurface(&p1, &p2);
}

EReal Data::EPoint2D::orientedSurface(const EPoint2D *p1, const EPoint2D *p2) const
{
	EVector2D ab(this, p1);
	EVector2D ac(this, p2);

	return 0.5*ab.cross(ac);
}

bool Data::EPoint2D::clockWise(const EPoint2D &p1, const EPoint2D &p2) const
{
	return this->clockWise(&p1, &p2);
}

bool Data::EPoint2D::clockWise(const EPoint2D *p1, const EPoint2D *p2) const
{
	return (this->orientedSurface(p1, p2) > 0.0);
}

bool Data::EPoint2D::counterClockWise(const EPoint2D &p1, const EPoint2D &p2) const
{
	return this->counterClockWise(&p1, &p2);
}

bool Data::EPoint2D::counterClockWise(const EPoint2D *p1, const EPoint2D *p2) const
{
	return (this->orientedSurface(p1, p2) < 0.0);
}

/*bool Data::EPoint2D::match(const EPoint2D &p) const
{
	return EPoint::match(p);
}

bool Data::EPoint2D::match(const EPoint2D *p) const
{
	return EPoint::match(p);
}*/

bool Data::EPoint2D::match(EReal x, EReal y) const
{
	return this->match(EPoint2D(x, y));
}

/*bool Data::EPoint2D::equal(EReal x, EReal y) const
{
	return EPoint::equal(EPoint2D(x, y));
}*/

Point2D Data::EPoint2D::point2D() const
{
	Point2D pt;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		pt.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	//pt.setSize(this->getSize());

	pt.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		pt.highlight();
	}
#endif //#if USE_GUI

	return pt;
}

#if USE_GUI
void Data::EPoint2D::draw() const
{
	this->point2D().draw();
}

UInt Data::EPoint2D::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
	return this->point2D().fill(coord, color, normal);
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
