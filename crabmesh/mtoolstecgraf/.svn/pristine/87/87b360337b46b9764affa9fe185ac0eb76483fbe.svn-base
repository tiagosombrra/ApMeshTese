#include "Data/Triangle2D.h"

#include "Data/Vector2D.h"
#include "Data/State.h"
#include "Data/Identifiable.h"

using namespace Data;

Data::Triangle2D::Triangle2D(Point2D *p1, Point2D *p2, Point2D *p3) :
	Triangle(p1, p2, p3)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Triangle2D::Triangle2D(const Point2D &p1, const Point2D &p2, const Point2D &p3) :
	Triangle(new Point2D(p1), new Point2D(p2), new Point2D(p3) )
{

}

Data::Triangle2D::~Triangle2D()
{

}

UInt Data::Triangle2D::dimension() const
{
	return 2;
}

bool Data::Triangle2D::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Triangle2D::in(const Point *p) const
{
	//return ((!this->box().out(p)) && (this->in(static_cast<const Point2D *>(p))));
	return ((!this->box().optOut(p)) && (this->in(static_cast<const Point2D *>(p))));
}

bool Data::Triangle2D::in(const Point2D &p) const
{
	return this->in(&p);
}

bool Data::Triangle2D::in(const Point2D *p) const
{
	/*R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);*/

#if USE_C__11
	Real l1, l2, l3;

	std::tie(l1, l2, l3) = this->barycentricCoordinates(p);
#else
	R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
#endif //#if USE_C__11

	//return ((l1 > 0.0) && (l2 > 0.0) && (l3 > 0.0));
	//return ((l1 >= 0.0) && (l2 >= 0.0) && (l3 >= 0.0));
	//return ((l1 > Data::getTolerance()) && (l2 > Data::getTolerance()) && (l3 > Data::getTolerance()));
	return ((l1 > Data::getTolerance()) && (l2 > Data::getTolerance()) && (l3 > Data::getTolerance()));
}

bool Data::Triangle2D::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Triangle2D::on(const Point *p) const
{
	return this->on(static_cast<const Point2D *>(p));
}

bool Data::Triangle2D::on(const Point2D &p) const
{
	return this->on(&p);
}

bool Data::Triangle2D::on(const Point2D *p) const
{
	return ((!this->in(p)) && (!this->out(p)));
}

bool Data::Triangle2D::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Triangle2D::out(const Point *p) const
{
	//return ((this->box().out(p)) || (this->out(static_cast<const Point2D *>(p))));
	return ((this->box().optOut(p)) || (this->out(static_cast<const Point2D *>(p))));
}

bool Data::Triangle2D::out(const Point2D &p) const
{
	return this->out(&p);
}

bool Data::Triangle2D::out(const Point2D *p) const
{
	/*R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);*/

#if USE_C__11
	Real l1, l2, l3;

	std::tie(l1, l2, l3) = this->barycentricCoordinates(p);
#else
	R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
#endif //#if USE_C__11

	//return ((l1 < 0.0) || (l2 < 0.0) || (l3 < 0.0));
	//return ((l1 <= 0.0) || (l2 <= 0.0) || (l3 <= 0.0));
	//return ((l1 < Data::getTolerance()) || (l2 < Data::getTolerance()) || (l3 < Data::getTolerance()));
	return ((l1 < -Data::getTolerance()) || (l2 < -Data::getTolerance()) || (l3 < -Data::getTolerance()));
}

R3Tuple Data::Triangle2D::barycentricCoordinates(const Point &p) const
{
	return this->barycentricCoordinates(&p);
}

R3Tuple Data::Triangle2D::barycentricCoordinates(const Point *p) const
{
	return this->barycentricCoordinates(static_cast<const Point2D *>(p));
}

R3Tuple Data::Triangle2D::barycentricCoordinates(const Point2D &p) const
{
	return this->barycentricCoordinates(&p);
}

R3Tuple Data::Triangle2D::barycentricCoordinates(const Point2D *p) const
{
	Real sInv = 1.0/this->orientedSurface();

	Point2D *p1 = static_cast<Point2D *>(this->getP1());
	Point2D *p2 = static_cast<Point2D *>(this->getP2());
	Point2D *p3 = static_cast<Point2D *>(this->getP3());

	Real l1 = p->orientedSurface(p2, p3)*sInv;
	Real l2 = p1->orientedSurface(p, p3)*sInv;
	Real l3 = p1->orientedSurface(p2, p)*sInv;

#if USE_C__11
	return std::make_tuple(l1, l2, l3);
#else
	R3Tuple t;
	t.values[0] = l1;
	t.values[1] = l2;
	t.values[2] = l3;

	return t;
#endif //#if USE_C__11
}

Real Data::Triangle2D::surface() const
{
	return std::fabs(this->orientedSurface());
}

Real Data::Triangle2D::orientedSurface() const
{
	Point2D *p1 = static_cast<Point2D *>(this->getP1());
	Point2D *p2 = static_cast<Point2D *>(this->getP2());
	Point2D *p3 = static_cast<Point2D *>(this->getP3());

	return p1->orientedSurface(p2, p3);
}

#if USE_EXPERIMENTAL_3
Point2D Data::Triangle2D::circumcenter2D() const
#else
Point Data::Triangle2D::circumcenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	Point2D *p1 = static_cast<Point2D *>(this->getP1());
	Point2D *p2 = static_cast<Point2D *>(this->getP2());
	Point2D *p3 = static_cast<Point2D *>(this->getP3());

	if (!p1 || !p2 || !p3)
	{
#if USE_EXPERIMENTAL_3
		return Point2D();
#else
		return Point();
#endif //#if USE_EXPERIMENTAL_3
	}

	//found in http://en.wikipedia.org/wiki/Circumcenter

	Vector2D b(p1, p2);
	Vector2D c(p1, p3);

	Real bdot = b.dot(b);
	Real cdot = c.dot(c);

	Real d = 2.0*b.cross(c);

	Real x = (c.getY()*bdot - b.getY()*cdot)/d;
	Real y = (b.getX()*cdot - c.getX()*bdot)/d;

	Point2D p(x + p1->getX(), y + p1->getY());

	return p;

	/*Vector v1(p1);
	Vector v2(p2);
	Vector v3(p3);

	Real dot1 = v1.dot(v1);
	Real dot2 = v2.dot(v2);
	Real dot3 = v3.dot(v3);

	Real detNum1 = dot1*p2->getY() + dot2*p3->getY() + dot3*p1->getY() -
					(dot3*p2->getY() + dot2*p1->getY() + dot1*p3->getY());
	Real detNum2 = dot1*p3->getX() + dot2*p1->getX() + dot3*p2->getX() -
					(dot3*p1->getX() + dot2*p3->getX() + dot1*p2->getX());
	Real den = 2.0*(p1->getX()*p2->getY() + p2->getX()*p3->getY() + p3->getX()*p1->getY() -
					 (p3->getX()*p2->getY() + p1->getX()*p3->getY() + p2->getX()*p1->getY()));

	Point2D p(detNum1/den, detNum2/den);

	return p;*/

	/*Real x1, y1, x2, y2, x3, y3, x, y;
    Real x12PlusY12, x22PlusY22, x32PlusY32;
    Real detNum1, detNum2, detDen, den;

    x1 = p1->getX();
    y1 = p1->getY();
    x2 = p2->getX();
    y2 = p2->getY();
    x3 = p3->getX();
    y3 = p3->getY();

    x12PlusY12 = x1*x1 + y1*y1;
    x22PlusY22 = x2*x2 + y2*y2;
    x32PlusY32 = x3*x3 + y3*y3;

    detNum1 = x12PlusY12*y2 + x22PlusY22*y3 + x32PlusY32*y1 - x32PlusY32*y2 - x22PlusY22*y1 - x12PlusY12*y3;
    detNum2 = x12PlusY12*x3 + x22PlusY22*x1 + x32PlusY32*x2 - x32PlusY32*x1 - x22PlusY22*x3 - x12PlusY12*x2;
    detDen = x1*y2 + x2*y3 + x3*y1 - x3*y2 - x1*y3 - x2*y1;
    den = 2.0*detDen;

    x = detNum1/den;
    y = detNum2/den;

    Point2D center(x, y);

    return center;*/
}

#if USE_EXPERIMENTAL_3
Point3D Data::Triangle2D::circumcenter3D() const
{
    return Point3D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
Point2D Data::Triangle2D::incenter2D() const
#else
Point Data::Triangle2D::incenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	Point2D *p1 = static_cast<Point2D *>(this->getP1());
	Point2D *p2 = static_cast<Point2D *>(this->getP2());
	Point2D *p3 = static_cast<Point2D *>(this->getP3());

	if (!p1 || !p2 || !p3)
	{
#if USE_EXPERIMENTAL_3
		return Point2D();
#else
		return Point();
#endif //#if USE_EXPERIMENTAL_3
	}

	//found in http://en.wikipedia.org/wiki/Incircle

	Real a = p1->distance(p2);
    Real b = p2->distance(p3);
    Real c = p3->distance(p1);

    Real sum = a + b + c;

    Real x = (a*p3->getX() + b*p1->getX() + c*p2->getX())/sum;
    Real y = (a*p3->getY() + b*p1->getY() + c*p2->getY())/sum;

    Point2D p(x, y);

    return p;

    /*Real a, b, c, sum, x, y;

    a = v[0]->distance(v[1]);
    b = v[1]->distance(v[2]);
    c = v[2]->distance(v[0]);

    sum = a + b + c;

    x = (a*v[2]->getX() + b*v[0]->getX() + c*v[1]->getX())/sum;
    y = (a*v[2]->getY() + b*v[0]->getY() + c*v[1]->getY())/sum;

    Vertex *center = new Vertex(x, y);

    return center;*/
}

#if USE_EXPERIMENTAL_3
Point3D Data::Triangle2D::incenter3D() const
{
    return Point3D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_GUI
void Data::Triangle2D::highlight() const
{
	Colorable::highlight();
}

void Data::Triangle2D::unhighlight() const
{
	Colorable::unhighlight();
}

//void Data::Triangle2D::draw(bool fill) const
void Data::Triangle2D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	/*if ((fill) && (!this->isHighlighted()))
	{
		return;
	}*/

	bool changed = false;
	int modes[2];

	glColor(this->getR(), this->getG(), this->getB());

	/*if ((fill) || (this->isHighlighted()))
	{
		if (this->isHighlighted())
		{
			glGetIntegerv(GL_POLYGON_MODE, modes);

			if ((modes[0] != GL_FILL) || (modes[1] != GL_FILL))
			{
				changed = true;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		glBegin(GL_TRIANGLES);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
	}*/

	if (this->isHighlighted())
	{
		glGetIntegerv(GL_POLYGON_MODE, modes);

		if ((modes[0] != GL_FILL) || (modes[1] != GL_FILL))
		{
			changed = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(this->mid2D());
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		glBegin(GL_TRIANGLES);

			Point2D *p1 = static_cast<Point2D *>(this->getP1());
			Point2D *p2 = static_cast<Point2D *>(this->getP2());
			Point2D *p3 = static_cast<Point2D *>(this->getP3());

			glVertex(p1->getX(), p1->getY());
			glVertex(p2->getX(), p2->getY());
			glVertex(p3->getX(), p3->getY());

		glEnd();
	glPopMatrix();

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

UInt Data::Triangle2D::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = static_cast<GLfloat>(this->getB());
	}

	coord[0] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getX());
	coord[1] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getY());

	coord[2] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getX());
	coord[3] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getY());

	coord[4] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getX());
	coord[5] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getY());

#if USE_EXPERIMENTAL_3
	Point2D mid = this->mid2D();
#else
	Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

	this->explodeAndShrink(coord, mid);
	this->explodeAndShrink(coord + 2, mid);
	this->explodeAndShrink(coord + 4, mid);

	return 3;
}
#endif //#if USE_GUI
