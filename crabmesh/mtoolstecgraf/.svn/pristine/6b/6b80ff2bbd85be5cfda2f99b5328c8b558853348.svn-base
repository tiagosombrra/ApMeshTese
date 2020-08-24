#include "Data/Vector2D.h"

#include "Data/Point2D.h"

using namespace Data;

Data::Vector2D::Vector2D(Real x, Real y) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(x, y);
}

Data::Vector2D::Vector2D(const Vector2D &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1));
}

Data::Vector2D::Vector2D(const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1));
}

Data::Vector2D::Vector2D(const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v->getCoord(0), v->getCoord(1));
}

Data::Vector2D::Vector2D(const Point &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1));
}

Data::Vector2D::Vector2D(const Point *p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0), p->getCoord(1));
}

Data::Vector2D::Vector2D(const Point &p1, const Point &p2) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p2.getCoord(0) - p1.getCoord(0), p2.getCoord(1) - p1.getCoord(1));
}

Data::Vector2D::Vector2D(const Point *p1, const Point *p2) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(2)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 2;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p2->getCoord(0) - p1->getCoord(0), p2->getCoord(1) - p1->getCoord(1));
}

Data::Vector2D::~Vector2D()
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	this->dim = 0;
	this->Vector::coords = NULL;
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
#endif //#if (!USE_EXPERIMENTAL_3)
}

void Data::Vector2D::setPosition(Real x, Real y)
{
	this->setX(x);
	this->setY(y);
}

void Data::Vector2D::setX(Real x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

Real Data::Vector2D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::Vector2D::setY(Real y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

Real Data::Vector2D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

UInt Data::Vector2D::dimension() const
{
	return 2;
}

#if USE_EXPERIMENTAL_3
void Data::Vector2D::setCoords(const Real coords[])
{
    for (UInt i = 0; i < 2; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::Vector2D::setCoords(const RealVector &coords)
{
    for (UInt i = 0; i < 2; i++)
	{
		this->coords[i] = coords[i];
	}
}

RealVector Data::Vector2D::getCoords() const
{
    return RealVector(this->coords, this->coords + 2);
}

const Real *Data::Vector2D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::Vector2D::setCoord(UInt i, Real c)
{
    this->coords[i] = c;
}

Real Data::Vector2D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

Real Data::Vector2D::cross(const Vector2D &v) const
{
	return this->cross(&v);
}

Real Data::Vector2D::cross(const Vector2D *v) const
{
	return (this->getX()*v->getY() - this->getY()*v->getX());
}

Real Data::Vector2D::scaledCross(const Vector2D &v) const
{
	return this->scaledCross(&v);
}

Real Data::Vector2D::scaledCross(const Vector2D *v) const
{
	Real norm = this->norm();

	if (norm < 1.0)
	{
		Vector2D v2(this);

		v2.multiply(2.0/norm);

		norm = v->norm();

		if (norm < 1.0)
		{
			Vector2D v3(v);

			v3.multiply(2.0/norm);

			return v2.cross(v3);
		}

		return v2.cross(v);
	}

	norm = v->norm();

	if (norm < 1.0)
	{
		Vector2D v3(v);

		v3.multiply(2.0/norm);

		return this->cross(v3);
	}

	return this->cross(v);
}

Real Data::Vector2D::orientedAngle() const
{
	static const Vector2D v(1.0, 0.0);

    Real ang = this->angle(v);

    return (this->getY() >= 0.0) ? ang : 2.0*M_PI - ang;
}

Real Data::Vector2D::orientedAngle(const Vector2D &v) const
{
	return this->orientedAngle(&v);
}

Real Data::Vector2D::orientedAngle(const Vector2D *v) const
{
	return (v->orientedAngle() - this->orientedAngle());
}

bool Data::Vector2D::left(const Vector2D &v) const
{
	return this->left(&v);
}

bool Data::Vector2D::left(const Vector2D *v) const
{
	//return (this->cross(v) > Data::getTolerance());
	return (this->scaledCross(v) > Data::getTolerance());
}

bool Data::Vector2D::left(const Point2D &p) const
{
	return this->left(&p);
}

bool Data::Vector2D::left(const Point2D *p) const
{
	Vector2D v(p);

	return this->left(v);
}

bool Data::Vector2D::right(const Vector2D &v) const
{
	return this->right(&v);
}

bool Data::Vector2D::right(const Vector2D *v) const
{
	//return (this->cross(v) < -Data::getTolerance());
	return (this->scaledCross(v) < -Data::getTolerance());
}

bool Data::Vector2D::right(const Point2D &p) const
{
	return this->right(&p);
}

bool Data::Vector2D::right(const Point2D *p) const
{
	Vector2D v(p);

	return this->right(v);
}

#if USE_EXACT
EVector2D Data::Vector2D::evector2D() const
{
	EVector2D v;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		v.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	v.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		v.highlight();
	}
#endif //#if USE_GUI

	return v;
}
#endif //#if USE_EXACT

#if USE_ARRAY
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
Vector2D &Data::Vector2D::operator=(const Vector2D &v)
{
	if (this != &v)
	{
#if (!USE_EXPERIMENTAL_3)
		this->dim = 2;
		this->Vector::coords = this->coords;
#endif //#if (!USE_EXPERIMENTAL_3)

		this->coords[0] = v.coords[0];
		this->coords[1] = v.coords[1];

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))

#if USE_EXPERIMENTAL_3
Vector2D &Data::Vector2D::operator=(const Vector &v)
{
	if (this != &v)
	{
		this->coords[0] = v.getCoord(0);
		this->coords[1] = v.getCoord(1);

#if USE_GUI
		this->setColor(v.getR(), v.getG(), v.getB());
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Vector2D::draw() const
{
	glColor(this->getR(), this->getG(), this->getB());

	glBegin(GL_LINES);
		glVertex(0.0, 0.0);
		glVertex(this->coords[0], this->coords[1]);
	glEnd();

	glBegin(GL_POINTS);
		glVertex(0.0, 0.0);
	glEnd();
}
#endif //#if USE_GUI

#if USE_EXACT

Data::EVector2D::EVector2D(EReal x, EReal y) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(x, y);
}

Data::EVector2D::EVector2D(const EVector &v) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(v.getCoord(0), v.getCoord(1));
}

Data::EVector2D::EVector2D(const EVector *v) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(v->getCoord(0), v->getCoord(1));
}

Data::EVector2D::EVector2D(const EPoint &p) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p.getCoord(0), p.getCoord(1));
}

Data::EVector2D::EVector2D(const EPoint *p) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p->getCoord(0), p->getCoord(1));
}

Data::EVector2D::EVector2D(const EPoint &p1, const EPoint &p2) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p2.getCoord(0) - p1.getCoord(0), p2.getCoord(1) - p1.getCoord(1));
}

Data::EVector2D::EVector2D(const EPoint *p1, const EPoint *p2) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(2)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p2->getCoord(0) - p1->getCoord(0), p2->getCoord(1) - p1->getCoord(1));
}

Data::EVector2D::~EVector2D()
{

}

void Data::EVector2D::setPosition(EReal x, EReal y)
{
	this->setX(x);
	this->setY(y);
}

void Data::EVector2D::setX(EReal x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

EReal Data::EVector2D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::EVector2D::setY(EReal y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

EReal Data::EVector2D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

UInt Data::EVector2D::dimension() const
{
	return 2;
}

#if USE_EXPERIMENTAL_3
void Data::EVector2D::setCoords(const EReal coords[])
{
    for (UInt i = 0; i < 2; i++)
    {
        this->coords[i] = coords[i];
    }
}

void Data::EVector2D::setCoords(const ERealVector &coords)
{
    for (UInt i = 0; i < 2; i++)
    {
        this->coords[i] = coords[i];
    }
}

ERealVector Data::EVector2D::getCoords() const
{
    return ERealVector(this->coords, this->coords + 2);
}

const EReal *Data::EVector2D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::EVector2D::setCoord(UInt i, EReal c)
{
    this->coords[i] = c;
}

EReal Data::EVector2D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

EReal Data::EVector2D::cross(const EVector2D &v) const
{
	return this->cross(&v);
}

EReal Data::EVector2D::cross(const EVector2D *v) const
{
	return (this->getX()*v->getY() - this->getY()*v->getX());
}

EReal Data::EVector2D::scaledCross(const EVector2D &v) const
{
	return this->scaledCross(&v);
}

EReal Data::EVector2D::scaledCross(const EVector2D *v) const
{
	EReal norm = this->norm();

	if (norm < 1.0)
	{
		EVector2D v2(this);

		v2.multiply(2.0/norm);

		norm = v->norm();

		if (norm < 1.0)
		{
			EVector2D v3(v);

			v3.multiply(2.0/norm);

			return v2.cross(v3);
		}

		return v2.cross(v);
	}

	norm = v->norm();

	if (norm < 1.0)
	{
		EVector2D v3(v);

		v3.multiply(2.0/norm);

		return this->cross(v3);
	}

	return this->cross(v);
}

EReal Data::EVector2D::orientedAngle() const
{
	static const EVector2D v(EReal(1.0), EReal(0.0));

    EReal ang = this->angle(v);

    return (this->getY() >= 0.0) ? ang : 2.0*M_PI - ang;
}

EReal Data::EVector2D::orientedAngle(const EVector2D &v) const
{
	return this->orientedAngle(&v);
}

EReal Data::EVector2D::orientedAngle(const EVector2D *v) const
{
	return (v->orientedAngle() - this->orientedAngle());
}

bool Data::EVector2D::left(const EVector2D &v) const
{
	return this->left(&v);
}

bool Data::EVector2D::left(const EVector2D *v) const
{
	return (this->scaledCross(v) > 0.0);
}

bool Data::EVector2D::left(const EPoint2D &p) const
{
	return this->left(&p);
}

bool Data::EVector2D::left(const EPoint2D *p) const
{
	EVector2D v(p);

	return this->left(v);
}

bool Data::EVector2D::right(const EVector2D &v) const
{
	return this->right(&v);
}

bool Data::EVector2D::right(const EVector2D *v) const
{
	return (this->scaledCross(v) < 0.0);
}

bool Data::EVector2D::right(const EPoint2D &p) const
{
	return this->right(&p);
}

bool Data::EVector2D::right(const EPoint2D *p) const
{
	EVector2D v(p);

	return this->right(v);
}

Vector2D Data::EVector2D::vector2D() const
{
	Vector2D v;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		v.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	v.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		v.highlight();
	}
#endif //#if USE_GUI

	return v;
}

#if USE_ARRAY
#if USE_EXPERIMENTAL_3
EVector2D &Data::EVector2D::operator=(const EVector2D &v)
{
    if (this != &v)
	{
		this->coords[0] = v.coords[0];
		this->coords[1] = v.coords[1];

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::EVector2D::draw() const
{
	this->vector2D().draw();
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
