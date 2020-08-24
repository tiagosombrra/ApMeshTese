#include "Data/Point3D.h"

#include "Data/State.h"

using namespace Data;

Data::Point3D::Point3D(Real x, Real y, Real z) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(x, y, z);
}

Data::Point3D::Point3D(const Point3D &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Data::Point3D::Point3D(const Point &p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Data::Point3D::Point3D(const Point *p) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
}

Data::Point3D::Point3D(const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v.getCoord(0), v.getCoord(1), v.getCoord(2));
}

Data::Point3D::Point3D(const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(v->getCoord(0), v->getCoord(1), v->getCoord(2));
}

Data::Point3D::Point3D(const Point &p, const Vector &v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p.getCoord(0) + v.getCoord(0), p.getCoord(1) + v.getCoord(1), p.getCoord(2) + v.getCoord(2));
}

Data::Point3D::Point3D(const Point *p, const Vector *v) :
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	Point()
#else
	Point(3)
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 3;
	this->Point::coords = this->coords;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)

	this->setPosition(p->getCoord(0) + v->getCoord(0), p->getCoord(1) + v->getCoord(1), p->getCoord(2) + v->getCoord(2));
}

Data::Point3D::~Point3D()
{
#if (!USE_EXPERIMENTAL_3)
#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
	this->dim = 0;
	this->Point::coords = NULL;
#endif //#if ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2))
#endif //#if (!USE_EXPERIMENTAL_3)
}

void Data::Point3D::setPosition(Real x, Real y, Real z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Data::Point3D::setX(Real x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

Real Data::Point3D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::Point3D::setY(Real y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

Real Data::Point3D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

void Data::Point3D::setZ(Real z)
{
	//this->setCoord(2, z);
	this->coords[2] = z;
}

Real Data::Point3D::getZ() const
{
	//return this->getCoord(2);
	return this->coords[2];
}

UInt Data::Point3D::dimension() const
{
	return 3;
}

#if USE_EXPERIMENTAL_3
void Data::Point3D::setCoords(const Real coords[])
{
    for (UInt i = 0; i < 3; i++)
    {
        this->coords[i] = coords[i];
    }
}

void Data::Point3D::setCoords(const RealVector &coords)
{
    for (UInt i = 0; i < 3; i++)
    {
        this->coords[i] = coords[i];
    }
}

RealVector Data::Point3D::getCoords() const
{
    return RealVector(this->coords, this->coords + 3);
}

const Real *Data::Point3D::getCoordsArray() const
{
    return this->coords;
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
void Data::Point3D::setCoord(UInt i, Real c)
{
    this->coords[i] = c;
}

Real Data::Point3D::getCoord(UInt i) const
{
    return this->coords[i];
}
#endif //#if USE_EXPERIMENTAL_3

Real Data::Point3D::surface(const Point3D &p1, const Point3D &p2) const
{
	return surface(&p1, &p2);
}

Real Data::Point3D::surface(const Point3D *p1, const Point3D *p2) const
{
	Vector3D ab(this, p1);
	Vector3D ac(this, p2);

	return 0.5*ab.cross(ac).norm();
}

Real Data::Point3D::volume(const Point3D &p1, const Point3D &p2, const Point3D &p3) const
{
	return this->volume(&p1, &p2, &p3);
}

Real Data::Point3D::volume(const Point3D *p1, const Point3D *p2, const Point3D *p3) const
{
	return std::fabs(this->orientedVolume(p1, p2, p3));
}

Real Data::Point3D::orientedVolume(const Point3D &p1, const Point3D &p2, const Point3D &p3) const
{
	return this->orientedVolume(&p1, &p2, &p3);
}

Real Data::Point3D::orientedVolume(const Point3D *p1, const Point3D *p2, const Point3D *p3) const
{
	Vector3D ab(this, p1);
	Vector3D ac(this, p2);
	Vector3D ad(this, p3);

	return ad.dot(ab.cross(ac))/6.0;
}

bool Data::Point3D::match(Real x, Real y, Real z)
{
	return Point::match(Point3D(x, y, z));
}

bool Data::Point3D::equal(Real x, Real y, Real z)
{
	return Point::equal(Point3D(x, y, z));
}

void Data::Point3D::project(const Point3D &point, const Vector3D &normal)
{
	Vector3D aux(&point, this);

	Real dot = aux.dot(normal);

	aux = normal;

	aux.multiply(-dot);

	this->sum(aux);
}

#if USE_EXACT
EPoint3D Data::Point3D::epoint3D() const
{
	EPoint3D pt;

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
Point3D &Data::Point3D::operator=(const Point3D &p)
{
	if (this != &p)
	{
#if (!USE_EXPERIMENTAL_3)
		this->dim = 3;
		this->Point::coords = this->coords;
#endif //#if (!USE_EXPERIMENTAL_3)

		this->coords[0] = p.coords[0];
		this->coords[1] = p.coords[1];
		this->coords[2] = p.coords[2];

#if USE_GUI
		this->setColor(p.r, p.g, p.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))

#if USE_EXPERIMENTAL_3
Point3D &Data::Point3D::operator=(const Point &p)
{
	if (this != &p)
	{
		this->coords[0] = p.getCoord(0);
		this->coords[1] = p.getCoord(1);
		this->coords[2] = p.getCoord(2);

#if USE_GUI
		this->setColor(p.getR(), p.getG(), p.getB());
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Point3D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	bool lighting = false;

	if (glIsEnabled(GL_LIGHTING))
	{
		glDisable(GL_LIGHTING);

		lighting = true;
	}

	glPushMatrix();
		this->explodeAndShrink(this, true);

		glBegin(GL_POINTS);
			//glVertex(this->coords[0], this->coords[1], this->coords[2]);
			glVertex(0.0, 0.0, 0.0);
		glEnd();
	glPopMatrix();

	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
}

UInt Data::Point3D::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
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
	coord[2] = static_cast<GLfloat>(0.0);

	this->explodeAndShrink(coord, *this, true);

	return 1;
}
#endif //#if USE_GUI

#if USE_EXACT

Data::EPoint3D::EPoint3D(EReal x, EReal y, EReal z) :
	EPoint(3)
{
	this->setPosition(x, y, z);
}

Data::EPoint3D::EPoint3D(const EPoint &p) :
	EPoint(3)
{
	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Data::EPoint3D::EPoint3D(const EPoint *p) :
	EPoint(3)
{
	this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
}

Data::EPoint3D::EPoint3D(const EVector &v) :
	EPoint(3)
{
	this->setPosition(v.getCoord(0), v.getCoord(1), v.getCoord(2));
}

Data::EPoint3D::EPoint3D(const EVector *v) :
	EPoint(3)
{
	this->setPosition(v->getCoord(0), v->getCoord(1), v->getCoord(2));
}

Data::EPoint3D::EPoint3D(const EPoint &p, const EVector &v) :
	EPoint(3)
{
	this->setPosition(p.getCoord(0) + v.getCoord(0), p.getCoord(1) + v.getCoord(1), p.getCoord(2) + v.getCoord(2));
}

Data::EPoint3D::EPoint3D(const EPoint *p, const EVector *v) :
	EPoint(3)
{
	this->setPosition(p->getCoord(0) + v->getCoord(0), p->getCoord(1) + v->getCoord(1), p->getCoord(2) + v->getCoord(2));
}

Data::EPoint3D::~EPoint3D()
{

}

void Data::EPoint3D::setPosition(EReal x, EReal y, EReal z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Data::EPoint3D::setX(EReal x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

EReal Data::EPoint3D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Data::EPoint3D::setY(EReal y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

EReal Data::EPoint3D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

void Data::EPoint3D::setZ(EReal z)
{
	//this->setCoord(2, z);
	this->coords[2] = z;
}

EReal Data::EPoint3D::getZ() const
{
	//return this->getCoord(2);
	return this->coords[2];
}

UInt Data::EPoint3D::dimension() const
{
	return 3;
}

EReal Data::EPoint3D::surface(const EPoint3D &p1, const EPoint3D &p2) const
{
	return surface(&p1, &p2);
}

EReal Data::EPoint3D::surface(const EPoint3D *p1, const EPoint3D *p2) const
{
	EVector3D ab(this, p1);
	EVector3D ac(this, p2);

	return 0.5*ab.cross(ac).norm();
}

EReal Data::EPoint3D::volume(const EPoint3D &p1, const EPoint3D &p2, const EPoint3D &p3) const
{
	return this->volume(&p1, &p2, &p3);
}

EReal Data::EPoint3D::volume(const EPoint3D *p1, const EPoint3D *p2, const EPoint3D *p3) const
{
	return std::fabs(this->orientedVolume(p1, p2, p3));
}

EReal Data::EPoint3D::orientedVolume(const EPoint3D &p1, const EPoint3D &p2, const EPoint3D &p3) const
{
	return this->orientedVolume(&p1, &p2, &p3);
}

EReal Data::EPoint3D::orientedVolume(const EPoint3D *p1, const EPoint3D *p2, const EPoint3D *p3) const
{
	EVector3D ab(this, p1);
	EVector3D ac(this, p2);
	EVector3D ad(this, p3);

	return ad.dot(ab.cross(ac))/6.0;
}

bool Data::EPoint3D::match(EReal x, EReal y, EReal z)
{
	return EPoint::match(EPoint3D(x, y, z));
}

/*bool Data::EPoint3D::equal(EReal x, EReal y, EReal z)
{
	return EPoint::equal(EPoint3D(x, y, z));
}*/

Point3D Data::EPoint3D::point3D() const
{
	Point3D pt;

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
void Data::EPoint3D::draw() const
{
	this->point3D().draw();
}

UInt Data::EPoint3D::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
	return this->point3D().fill(coord, color, normal);
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
