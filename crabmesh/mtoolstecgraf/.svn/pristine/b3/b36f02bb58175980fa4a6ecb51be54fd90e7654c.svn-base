#include "Data/Edge3D.h"

#include "Data/Identifiable.h"
#include "Data/State.h"

using namespace Data;

Data::Edge3D::Edge3D(Point3D *p1, Point3D *p2) :
	Edge(p1, p2)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Edge3D::Edge3D(const Point3D &p1, const Point3D &p2) :
	Edge(NULL, NULL)
{
	this->setPoints(new Point3D(p1), new Point3D(p2));

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Edge3D::~Edge3D()
{

}

UInt Data::Edge3D::dimension() const
{
	return 3;
}

bool Data::Edge3D::intercept(const Edge3D &e) const
{
	return this->intercept(&e);
}

bool Data::Edge3D::intercept(const Edge3D *e) const
{
	return this->intercept(static_cast<Point3D *>(e->getP1()), static_cast<Point3D *>(e->getP2()));
}

bool Data::Edge3D::intercept(const Point3D &p1, const Point3D &p2) const
{
	return this->intercept(&p1, &p2);
}

bool Data::Edge3D::intercept(const Point3D *p1, const Point3D *p2) const
{
	Point3D *tp1 = static_cast<Point3D *>(this->getP1());
	Point3D *tp2 = static_cast<Point3D *>(this->getP2());

#if USE_EXPERIMENTAL_3
	Vector3D ab(this->vector3D());
#else
	Vector3D ab(this->vector());
#endif //#if USE_EXPERIMENTAL_3
	Vector3D ac(tp1, p1);
	Vector3D ad(tp1, p2);

    Vector3D cd(p1, p2);
	Vector3D ca(p1, tp1);
	Vector3D cb(p1, tp2);

	return ((ab.cross(ac).dot(ab.cross(ad)) < 0.0) &&
			(cd.cross(ca).dot(cd.cross(cb)) < 0.0));
	//return ((ab.cross(ac).dot(ab.cross(ad)) < -Data::getTolerance()) &&
	//		(cd.cross(ca).dot(cd.cross(cb)) < -Data::getTolerance()));*/
}

#if USE_EXACT
bool Data::Edge3D::eintercept(const Edge3D &e) const
{
	return this->eintercept(&e);
}

bool Data::Edge3D::eintercept(const Edge3D *e) const
{
	return this->eintercept(static_cast<Point3D *>(e->getP1()), static_cast<Point3D *>(e->getP2()));
}

bool Data::Edge3D::eintercept(const Point3D &p1, const Point3D &p2) const
{
	return this->eintercept(&p1, &p2);
}

bool Data::Edge3D::eintercept(const Point3D *p1, const Point3D *p2) const
{
	EPoint3D tp1 = static_cast<Point3D *>(this->getP1())->epoint3D();
	EPoint3D tp2 = static_cast<Point3D *>(this->getP2())->epoint3D();

	EPoint3D ep1 = p1->epoint3D();
	EPoint3D ep2 = p2->epoint3D();

	EVector3D ab(tp1, tp2);
	EVector3D ac(tp1, ep1);
	EVector3D ad(tp1, ep2);

    EVector3D cd(ep1, ep2);
	EVector3D ca(ep1, tp1);
	EVector3D cb(ep1, tp2);

    return ((ab.cross(ac).dot(ab.cross(ad)) < 0.0) &&
			(cd.cross(ca).dot(cd.cross(cb)) < 0.0));
}
#endif //#if USE_EXACT

Real Data::Edge3D::distance(const Point3D &p) const
{
	return this->distance(&p);
}

Real Data::Edge3D::distance(const Point3D *p) const
{
#if USE_EXPERIMENTAL_3
	Vector3D v = this->vector3D();
#else
	Vector3D v = this->vector();
#endif //#if USE_EXPERIMENTAL_3
	Vector3D u(this->getP1(), p);

	Real t = v.dot(u);

	//if t is smaller than 0, then p is closest to this->p1
	if (t < Data::getTolerance())
	{
		return u.norm();
	}

	Real d = v.dot(v);

	//if t is greater than d, then p is closest to this->p2
	if (t > d - Data::getTolerance())
	{
		Vector3D v(this->getP2(), p);

		return v.norm();
	}

	return std::sqrt(u.dot(u) - t*t/d);

	/*Vector3D v(this->getP1(), p);

	Real l = this->length();
	Real vcos = this->vector().dot(v)/l;

	return ((vcos < Data::getTolerance()) ? this->getP1()->distance(p) :
			((vcos > l - Data::getTolerance()) ? this->getP2()->distance(p) : this->height(p)));*/
}

Real Data::Edge3D::distance(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->distance(p);
}

Real Data::Edge3D::straightDistance(const Point &p) const
{
	return this->straightDistance(&p);
}

Real Data::Edge3D::straightDistance(const Point *p) const
{
	return this->straightDistance(Point3D(p));
}

Real Data::Edge3D::straightDistance(const Point3D &p) const
{
	return this->straightDistance(&p);
}

Real Data::Edge3D::straightDistance(const Point3D *p) const
{
	return this->height(p);
}

Real Data::Edge3D::straightDistance(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->straightDistance(p);
}

Real Data::Edge3D::height(const Point &p) const
{
	return this->height(&p);
}

Real Data::Edge3D::height(const Point *p) const
{
	return this->height(Point3D(p));
}

Real Data::Edge3D::height(const Point3D &p) const
{
	return this->height(&p);
}

Real Data::Edge3D::height(const Point3D *p) const
{
	/*Vector3D n = this->normal();
	Vector3D v(this->getP1(), p);

	return std::fabs(v.dot(n)/n.norm());*/

#if USE_EXPERIMENTAL_3
	Vector3D v = this->vector3D();
#else
	Vector3D v = this->vector();
#endif //#if USE_EXPERIMENTAL_3
	Vector3D u(this->getP1(), p);

	Real t = v.dot(u);

	Real d = v.dot(v);

	return std::sqrt(u.dot(u) - t*t/d);
}

Real Data::Edge3D::height(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->height(p);
}

Real Data::Edge3D::angle(const Point3D &p1, const Point3D &p2) const
{
	return this->angle(&p1, &p2);
}

Real Data::Edge3D::angle(const Point3D *p1, const Point3D *p2) const
{
#if USE_EXPERIMENTAL_3
	Vector3D ab(this->vector3D());
#else
	Vector3D ab = this->vector();
#endif //#if USE_EXPERIMENTAL_3
	Vector3D ac;

    if (this->getP1()->match(p1))
    {
        ac = Vector3D(p1, p2);
    }
    else if (this->getP1()->match(p2))
    {
        ac = Vector3D(p2, p1);
    }
    else if (this->getP2()->match(p1))
    {
    	ac = Vector3D(p1, p2);
    	ab.multiply(-1.0);
    }
    else if (this->getP2()->match(p2))
    {
        ac = Vector3D(p2, p1);
    	ab.multiply(-1.0);
    }
    else
    {
        return -1.0;
    }

    return ab.angle(ac);
}

Real Data::Edge3D::angle(const Edge3D &e) const
{
	return this->angle(&e);
}

Real Data::Edge3D::angle(const Edge3D *e) const
{
	return this->angle(static_cast<Point3D *>(e->getP1()), static_cast<Point3D *>(e->getP2()));
}

Real Data::Edge3D::angle(const Point3D &p) const
{
	return this->angle(&p);
}

Real Data::Edge3D::angle(const Point3D *p) const
{
	/*
	             *   p
	            / \  -----> returns this angle
	           /   \
	          /     \
	         /       \
	        /         \
	       /           \
	      /             \
	     *---------------*
	           this
	*/

	Vector3D v1(p, static_cast<Point3D *>(this->getP1()));
	Vector3D v2(p, static_cast<Point3D *>(this->getP2()));

	return v1.angle(v2);
}

#if USE_GUI
//void Data::Edge3D::draw(bool /*fill*/) const
void Data::Edge3D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(this->mid3D());
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		glBegin(GL_LINES);
			glVertex(static_cast<Point3D *>(this->getP1())->getX(), static_cast<Point3D *>(this->getP1())->getY(), static_cast<Point3D *>(this->getP1())->getZ());
			glVertex(static_cast<Point3D *>(this->getP2())->getX(), static_cast<Point3D *>(this->getP2())->getY(), static_cast<Point3D *>(this->getP2())->getZ());
		glEnd();
	glPopMatrix();
}

UInt Data::Edge3D::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = static_cast<GLfloat>(this->getB());
	}

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

	return 2;
}
#endif //#if USE_GUI
