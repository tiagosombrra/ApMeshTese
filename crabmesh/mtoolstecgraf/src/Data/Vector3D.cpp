#include "Data/Vector3D.h"

#include "Data/Point3D.h"

using namespace Data;

Data::Vector3D::Vector3D(Real x, Real y, Real z) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(x, y, z);
}

Data::Vector3D::Vector3D(const Vector3D &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1), v.getCoord(2));
}

Data::Vector3D::Vector3D(const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1), v.getCoord(2));
}

Data::Vector3D::Vector3D(const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v->getCoord(0), v->getCoord(1), v->getCoord(2));
}

Data::Vector3D::Vector3D(const Point &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Data::Vector3D::Vector3D(const Point *p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
}

Data::Vector3D::Vector3D(const Point &p1, const Point &p2) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p2.getCoord(0) - p1.getCoord(0), p2.getCoord(1) - p1.getCoord(1), p2.getCoord(2) - p1.getCoord(2));
}

Data::Vector3D::Vector3D(const Point *p1, const Point *p2) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Vector()
#else
	Vector(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Vector::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p2->getCoord(0) - p1->getCoord(0), p2->getCoord(1) - p1->getCoord(1), p2->getCoord(2) - p1->getCoord(2));
}

Data::Vector3D::~Vector3D()
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 0;
	this->Vector::coords = NULL;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)
}

void Data::Vector3D::setPosition(Real x, Real y, Real z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Data::Vector3D::setX(Real x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

Real Data::Vector3D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::Vector3D::setY(Real y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

Real Data::Vector3D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

void Data::Vector3D::setZ(Real z)
{
	//this->setCoord(2, z);
	this->coords[2] = z;
}

Real Data::Vector3D::getZ() const
{
	//return this->getCoord(2);
	return this->coords[2];
}

UInt Data::Vector3D::dimension() const
{
	return 3;
}

#if USE_EXPERIMENTAL_3
void Data::Vector3D::setCoords(const Real coords[])
{
    for (UInt i = 0; i < 3; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::Vector3D::setCoords(const RealVector &coords)
{
    for (UInt i = 0; i < 3; i++)
	{
		this->coords[i] = coords[i];
	}
}

RealVector Data::Vector3D::getCoords() const
{
    return RealVector(this->coords, this->coords + 3);
}

const Real *Data::Vector3D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::Vector3D::setCoord(UInt i, Real c)
{
    this->coords[i] = c;
}

Real Data::Vector3D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

Vector3D Data::Vector3D::cross(const Vector3D &v) const
{
	return this->cross(&v);
}

Vector3D Data::Vector3D::cross(const Vector3D *v) const
{
	Vector3D c(this->getY()*v->getZ() - this->getZ()*v->getY(),
			   this->getZ()*v->getX() - this->getX()*v->getZ(),
			   this->getX()*v->getY() - this->getY()*v->getX());

	return c;
}

#if USE_EXACT
EVector3D Data::Vector3D::evector3D() const
{
	EVector3D v;

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
Vector3D &Data::Vector3D::operator=(const Vector3D &v)
{
	if (this != &v)
	{
#if (!USE_EXPERIMENTAL_3)
		this->dim = 3;
		this->Vector::coords = this->coords;
#endif //#if (!USE_EXPERIMENTAL_3)

		this->coords[0] = v.coords[0];
		this->coords[1] = v.coords[1];
		this->coords[2] = v.coords[2];

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))

#if USE_EXPERIMENTAL_3
Vector3D &Data::Vector3D::operator=(const Vector &v)
{
	if (this != &v)
	{
		this->coords[0] = v.getCoord(0);
		this->coords[1] = v.getCoord(1);
		this->coords[2] = v.getCoord(2);

#if USE_GUI
		this->setColor(v.getR(), v.getG(), v.getB());
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Vector3D::draw() const
{
	glColor(this->getR(), this->getG(), this->getB());

	glBegin(GL_LINES);
		glVertex(0.0, 0.0, 0.0);
		glVertex(this->coords[0], this->coords[1], this->coords[2]);
	glEnd();

	glBegin(GL_POINTS);
		glVertex(0.0, 0.0, 0.0);
	glEnd();
}
#endif //#if USE_GUI

#if USE_EXACT

Data::EVector3D::EVector3D(EReal x, EReal y, EReal z) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(x, y, z);
}

Data::EVector3D::EVector3D(const EVector &v) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(v.getCoord(0), v.getCoord(1), v.getCoord(2));
}

Data::EVector3D::EVector3D(const EVector *v) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(v->getCoord(0), v->getCoord(1), v->getCoord(2));
}

Data::EVector3D::EVector3D(const EPoint &p) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Data::EVector3D::EVector3D(const EPoint *p) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
}

Data::EVector3D::EVector3D(const EPoint &p1, const EPoint &p2) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p2.getCoord(0) - p1.getCoord(0), p2.getCoord(1) - p1.getCoord(1), p2.getCoord(2) - p1.getCoord(2));
}

Data::EVector3D::EVector3D(const EPoint *p1, const EPoint *p2) :
#if USE_EXPERIMENTAL_3
	EVector()
#else
	EVector(3)
#endif //#if USE_EXPERIMENTAL_3
{
	this->setPosition(p2->getCoord(0) - p1->getCoord(0), p2->getCoord(1) - p1->getCoord(1), p2->getCoord(2) - p1->getCoord(2));
}

Data::EVector3D::~EVector3D()
{

}

void Data::EVector3D::setPosition(EReal x, EReal y, EReal z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Data::EVector3D::setX(EReal x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

EReal Data::EVector3D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::EVector3D::setY(EReal y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

EReal Data::EVector3D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

void Data::EVector3D::setZ(EReal z)
{
	//this->setCoord(2, z);
	this->coords[2] = z;
}

EReal Data::EVector3D::getZ() const
{
	//return this->getCoord(2);
	return this->coords[2];
}

UInt Data::EVector3D::dimension() const
{
	return 3;
}

#if USE_EXPERIMENTAL_3
void Data::EVector3D::setCoords(const EReal coords[])
{
    for (UInt i = 0; i < 3; i++)
    {
        this->coords[i] = coords[i];
    }
}

void Data::EVector3D::setCoords(const ERealVector &coords)
{
    for (UInt i = 0; i < 3; i++)
    {
        this->coords[i] = coords[i];
    }
}

ERealVector Data::EVector3D::getCoords() const
{
    return ERealVector(this->coords, this->coords + 3);
}

const EReal *Data::EVector3D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::EVector3D::setCoord(UInt i, EReal c)
{
    this->coords[i] = c;
}

EReal Data::EVector3D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

EVector3D Data::EVector3D::cross(const EVector3D &v) const
{
	return this->cross(&v);
}

EVector3D Data::EVector3D::cross(const EVector3D *v) const
{
	EVector3D c(this->getY()*v->getZ() - this->getZ()*v->getY(),
			    this->getZ()*v->getX() - this->getX()*v->getZ(),
			    this->getX()*v->getY() - this->getY()*v->getX());

	return c;
}

Vector3D Data::EVector3D::vector3D() const
{
	Vector3D v;

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
EVector3D &Data::EVector3D::operator=(const EVector3D &v)
{
    if (this != &v)
	{
		this->coords[0] = v.coords[0];
		this->coords[1] = v.coords[1];
		this->coords[2] = v.coords[2];

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::EVector3D::draw() const
{
	this->vector3D().draw();
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
