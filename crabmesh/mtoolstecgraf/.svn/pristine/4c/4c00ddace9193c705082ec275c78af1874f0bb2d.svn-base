#include "Data/Edge2D.h"

#include "Data/Identifiable.h"
#include "Data/State.h"

using namespace Data;

Data::Edge2D::Edge2D(Point2D *p1, Point2D *p2) :
	Edge(p1, p2)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Edge2D::Edge2D(const Point2D &p1, const Point2D &p2) :
	Edge(NULL, NULL)
{
	this->setPoints(new Point2D(p1), new Point2D(p2));

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Edge2D::~Edge2D()
{

}

UInt Data::Edge2D::dimension() const
{
	return 2;
}

bool Data::Edge2D::intercept(const Edge2D &e) const
{
	return this->intercept(&e);
}

bool Data::Edge2D::intercept(const Edge2D *e) const
{
	return this->intercept(static_cast<Point2D *>(e->getP1()), static_cast<Point2D *>(e->getP2()));
}

bool Data::Edge2D::intercept(const Point2D &p1, const Point2D &p2) const
{
	return this->intercept(&p1, &p2);
}

bool Data::Edge2D::intercept(const Point2D *p1, const Point2D *p2) const
{
	Point2D *tp1 = static_cast<Point2D *>(this->getP1());
	Point2D *tp2 = static_cast<Point2D *>(this->getP2());

	/*Real min, max, pmin, pmax;

	//if the bounding boxes of the two edges do not intercept, then
	//	the two edges do not intercept

	if (tp1->getX() > tp2->getX())
	{
		min = tp2->getX();
		max = tp1->getX();
	}
	else
	{
		min = tp1->getX();
		max = tp2->getX();
	}

	if (p1->getX() > p2->getX())
	{
		pmin = p2->getX();
		pmax = p1->getX();
	}
	else
	{
		pmin = p1->getX();
		pmax = p2->getX();
	}

	if ((min > pmax) || (max < pmin))
	{
		return false;
	}

	if (tp1->getY() > tp2->getY())
	{
		min = tp2->getY();
		max = tp1->getY();
	}
	else
	{
		min = tp1->getY();
		max = tp2->getY();
	}

	if (p1->getY() > p2->getY())
	{
		pmin = p2->getY();
		pmax = p1->getY();
	}
	else
	{
		pmin = p1->getY();
		pmax = p2->getY();
	}

	if ((min > pmax) || (max < pmin))
	{
		return false;
	}*/

#if USE_EXPERIMENTAL_3
	Vector2D ab(this->vector2D());
#else
	Vector2D ab(this->vector());
#endif //#if USE_EXPERIMENTAL_3
	Vector2D ac(tp1, p1);
	Vector2D ad(tp1, p2);

    Vector2D cd(p1, p2);
	Vector2D ca(p1, tp1);
	Vector2D cb(p1, tp2);

    return ((ab.cross(ac)*ab.cross(ad) < 0.0) &&
			(cd.cross(ca)*cd.cross(cb) < 0.0));
	//return ((ab.cross(ac)*ab.cross(ad) < -Data::getTolerance()) &&
	//		(cd.cross(ca)*cd.cross(cb) < -Data::getTolerance()));
}

#if USE_EXACT
bool Data::Edge2D::eintercept(const Edge2D &e) const
{
	return this->eintercept(&e);
}

bool Data::Edge2D::eintercept(const Edge2D *e) const
{
	return this->eintercept(static_cast<Point2D *>(e->getP1()), static_cast<Point2D *>(e->getP2()));
}

bool Data::Edge2D::eintercept(const Point2D &p1, const Point2D &p2) const
{
	return this->eintercept(&p1, &p2);
}

bool Data::Edge2D::eintercept(const Point2D *p1, const Point2D *p2) const
{
	EPoint2D tp1 = static_cast<Point2D *>(this->getP1())->epoint2D();
	EPoint2D tp2 = static_cast<Point2D *>(this->getP2())->epoint2D();

	/*Real min, max, pmin, pmax;

	//if the bounding boxes of the two edges do not intercept, then
	//	the two edges do not intercept

	if (tp1->getX() > tp2->getX())
	{
		min = tp2->getX();
		max = tp1->getX();
	}
	else
	{
		min = tp1->getX();
		max = tp2->getX();
	}

	if (p1->getX() > p2->getX())
	{
		pmin = p2->getX();
		pmax = p1->getX();
	}
	else
	{
		pmin = p1->getX();
		pmax = p2->getX();
	}

	if ((min > pmax) || (max < pmin))
	{
		return false;
	}

	if (tp1->getY() > tp2->getY())
	{
		min = tp2->getY();
		max = tp1->getY();
	}
	else
	{
		min = tp1->getY();
		max = tp2->getY();
	}

	if (p1->getY() > p2->getY())
	{
		pmin = p2->getY();
		pmax = p1->getY();
	}
	else
	{
		pmin = p1->getY();
		pmax = p2->getY();
	}

	if ((min > pmax) || (max < pmin))
	{
		return false;
	}*/

	EPoint2D ep1 = p1->epoint2D();
	EPoint2D ep2 = p2->epoint2D();

	EVector2D ab(tp1, tp2);
	EVector2D ac(tp1, ep1);
	EVector2D ad(tp1, ep2);

    EVector2D cd(ep1, ep2);
	EVector2D ca(ep1, tp1);
	EVector2D cb(ep1, tp2);

    return ((ab.cross(ac)*ab.cross(ad) < 0.0) &&
			(cd.cross(ca)*cd.cross(cb) < 0.0));
}
#endif //#if USE_EXACT

Real Data::Edge2D::distance(const Point2D &p) const
{
	return this->distance(&p);
}

Real Data::Edge2D::distance(const Point2D *p) const
{
#if USE_EXPERIMENTAL_3
	Vector2D v = this->vector2D();
#else
	Vector2D v = this->vector();
#endif //#if USE_EXPERIMENTAL_3
	Vector2D u(this->getP1(), p);

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
		Vector2D v(this->getP2(), p);

		return v.norm();
	}

	return std::sqrt(u.dot(u) - t*t/d);

	/*Vector2D v(this->getP1(), p);

	Real l = this->length();
	Real vcos = this->vector().dot(v)/l;

	return ((vcos < Data::getTolerance()) ? this->getP1()->distance(p) :
			((vcos > l - Data::getTolerance()) ? this->getP2()->distance(p) : this->height(p)));*/
}

Real Data::Edge2D::distance(Real x, Real y) const
{
	Point2D p(x, y);

	return this->distance(p);
}

Real Data::Edge2D::straightDistance(const Point &p) const
{
	return this->straightDistance(&p);
}

Real Data::Edge2D::straightDistance(const Point *p) const
{
	return this->straightDistance(Point2D(p));
}

Real Data::Edge2D::straightDistance(const Point2D &p) const
{
	return this->straightDistance(&p);
}

Real Data::Edge2D::straightDistance(const Point2D *p) const
{
	return this->height(p);
}

Real Data::Edge2D::straightDistance(Real x, Real y) const
{
	Point2D p(x, y);

	return this->straightDistance(p);
}

Real Data::Edge2D::height(const Point &p) const
{
	return this->height(&p);
}

Real Data::Edge2D::height(const Point *p) const
{
	return this->height(Point2D(p));
}

Real Data::Edge2D::height(const Point2D &p) const
{
	return this->height(&p);
}

Real Data::Edge2D::height(const Point2D *p) const
{
	/*Point2D *p1 = static_cast<Point2D *>(this->getP1());
	Point2D *p2 = static_cast<Point2D *>(this->getP2());

	return (2.0*p1->surface(p2, p)/this->length());*/

	Vector2D n = this->normal();
	Vector2D v(this->getP1(), p);

	return std::fabs(v.dot(n)/n.norm());
}

Real Data::Edge2D::height(Real x, Real y) const
{
	Point2D p(x, y);

	return this->height(p);
}

bool Data::Edge2D::left(const Point2D &p) const
{
	return this->left(&p);
}

bool Data::Edge2D::left(const Point2D *p) const
{
	Vector2D v(static_cast<Point2D *>(this->getP1()), p);

#if USE_EXPERIMENTAL_3
	return Vector2D(this->vector2D()).left(v);
#else
	return Vector2D(this->vector()).left(v);
#endif //#if USE_EXPERIMENTAL_3
}

bool Data::Edge2D::left(Real x, Real y) const
{
	Point2D p(x, y);

	return this->left(p);
}

bool Data::Edge2D::right(const Point2D &p) const
{
	return this->right(&p);
}

bool Data::Edge2D::right(const Point2D *p) const
{
	//Vector2D v(this->getP1(), p);
	Vector2D v(static_cast<Point2D *>(this->getP1()), p);

#if USE_EXPERIMENTAL_3
	return Vector2D(this->vector2D()).right(v);
#else
	return Vector2D(this->vector()).right(v);
#endif //#if USE_EXPERIMENTAL_3
}

bool Data::Edge2D::right(Real x, Real y) const
{
	Point2D p(x, y);

	return this->right(p);
}

bool Data::Edge2D::accordingToNormal(const Point2D &p, bool insideTest) const
{
	return this->accordingToNormal(&p, insideTest);
}

bool Data::Edge2D::accordingToNormal(const Point2D *p, bool insideTest) const
{
	//return (insideTest) ? !this->right(p) : this->left(p);

	//this is here to keep conformity with
	//  Data::Triangle3D::accordingToNormal(const Point3D *, bool insideTest)
	Vector2D n = this->normal();
	//Vector2D v(static_cast<Point2D *>(this->getP1()), p);
#if USE_EXPERIMENTAL_3
	Vector2D v(this->mid2D(), *p);
#else
	Vector2D v(this->mid(), p);
#endif //#if USE_EXPERIMENTAL_3

	//n is already normalized
	//n.normalize();
	v.normalize();

	//since both n and v are normalized, the cosAngle need not to be called, because
	//  n.cosAngle(v) = n.dot(v)/(n.norm()*v.norm())
	//return (insideTest) ? n.cosAngle(v) > -Data::getTolerance() : n.cosAngle(v) > Data::getTolerance();
	return (insideTest) ? (n.dot(v) > -Data::getTolerance()) : (n.dot(v) > Data::getTolerance());
}

Real Data::Edge2D::angle(const Point2D &p1, const Point2D &p2) const
{
	return this->angle(&p1, &p2);
}

Real Data::Edge2D::angle(const Point2D *p1, const Point2D *p2) const
{
#if USE_EXPERIMENTAL_3
	Vector2D ab(this->vector2D());
#else
	Vector2D ab = this->vector();
#endif //#if USE_EXPERIMENTAL_3
	Vector2D ac;

    if (this->getP1()->match(p1))
    {
        ac = Vector2D(p1, p2);
    }
    else if (this->getP1()->match(p2))
    {
        ac = Vector2D(p2, p1);
    }
    else if (this->getP2()->match(p1))
    {
    	ac = Vector2D(p1, p2);
    	ab.multiply(-1.0);
    }
    else if (this->getP2()->match(p2))
    {
        ac = Vector2D(p2, p1);
    	ab.multiply(-1.0);
    }
    else
    {
        return -1.0;
    }

    return ab.angle(ac);
}

Real Data::Edge2D::angle(const Edge2D &e) const
{
	return this->angle(&e);
}

Real Data::Edge2D::angle(const Edge2D *e) const
{
	return this->angle(static_cast<Point2D *>(e->getP1()), static_cast<Point2D *>(e->getP2()));
}

Real Data::Edge2D::angle(const Point2D &p) const
{
	return this->angle(&p);
}

Real Data::Edge2D::angle(const Point2D *p) const
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

	Vector2D v1(p, static_cast<Point2D *>(this->getP1()));
	Vector2D v2(p, static_cast<Point2D *>(this->getP2()));

	return v1.angle(v2);
}

Real Data::Edge2D::orientedAngle(const Point2D &p, bool insideTest) const
{
	return this->orientedAngle(&p, insideTest);
}

Real Data::Edge2D::orientedAngle(const Point2D *p, bool insideTest) const
{
	Real angle = this->angle(p);

	return (this->accordingToNormal(p, insideTest)) ? angle : -angle;

	/*if (this->accordingToNormal(p, insideTest))
	{
		return angle;
	}

	return -angle;*/
}

Vector2D Data::Edge2D::normal() const
{
	Point2D *tp1 = static_cast<Point2D *>(this->getP1());
	Point2D *tp2 = static_cast<Point2D *>(this->getP2());

    if (tp1->match(tp2))
	{
		return Vector2D();
	}

	if (std::fabs(tp1->getX() - tp2->getX()) < Data::getTolerance())
	{
		if (tp1->getY() > tp2->getY())
		{
			return Vector2D(1.0, 0.0);
		}

		return Vector2D(-1.0, 0.0);
	}

	if (std::fabs(tp1->getY() - tp2->getY()) < Data::getTolerance())
	{
		if (tp1->getX() > tp2->getX())
		{
			return Vector2D(0.0, -1.0);
		}

		return Vector2D(0.0, 1.0);
	}

	Vector2D normal(tp1->getY() - tp2->getY(), tp2->getX() - tp1->getX());

	normal.normalize();

    return normal;
}

#if USE_GUI
//void Data::Edge2D::draw(bool /*fill*/) const
void Data::Edge2D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(this->mid2D());
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		glBegin(GL_LINES);
			glVertex(static_cast<Point2D *>(this->getP1())->getX(), static_cast<Point2D *>(this->getP1())->getY());
			glVertex(static_cast<Point2D *>(this->getP2())->getX(), static_cast<Point2D *>(this->getP2())->getY());
		glEnd();
	glPopMatrix();
}

UInt Data::Edge2D::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
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

	return 2;
}
#endif //#if USE_GUI
