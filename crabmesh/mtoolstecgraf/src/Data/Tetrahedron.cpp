#include "Data/Tetrahedron.h"

#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/State.h"
#include "Data/Identifiable.h"

using namespace Data;

Data::Tetrahedron::Tetrahedron(Point *p1, Point *p2, Point *p3, Point *p4) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	//Polyhedron(0, 4)
	Polyhedron()
#else
	//Polyhedron(4, 4)
	Polyhedron(4)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 4;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	this->setPoints(p1, p2, p3, p4);
}

#if (!USE_EXPERIMENTAL_4)
Data::Tetrahedron::Tetrahedron(Point *p1, Point *p2, Point *p3, Point *p4,
	Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4) :
#if USE_EXPERIMENTAL
	Polyhedron()
#else
	Polyhedron(4, 4)
#endif //
{
#if USE_EXPERIMENTAL
	this->numPts = 4;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL

	this->setPoints(p1, p2, p3, p4);

	this->setTriangles(t1, t2, t3, t4);
}
#endif //#if (!USE_EXPERIMENTAL_4)

Data::Tetrahedron::~Tetrahedron()
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Tetrahedron::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Tetrahedron::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Tetrahedron::numPoints() const
{
	return 4;
}

void Data::Tetrahedron::setPoints(Point *p1, Point *p2, Point *p3, Point *p4)
{
	this->setP1(p1);
	this->setP2(p2);
	this->setP3(p3);
	this->setP4(p4);
}

void Data::Tetrahedron::setP1(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Tetrahedron::getP1() const
{
	//return this->getPoint(0);
	return this->points[0];
}

void Data::Tetrahedron::setP2(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Tetrahedron::getP2() const
{
	//return this->getPoint(1);
	return this->points[1];
}

void Data::Tetrahedron::setP3(Point *p)
{
	//this->setPoint(2, p);
	this->points[2] = p;
}

Point *Data::Tetrahedron::getP3() const
{
	//return this->getPoint(2);
	return this->points[2];
}

void Data::Tetrahedron::setP4(Point *p)
{
	//this->setPoint(3, p);
	this->points[3] = p;
}

Point *Data::Tetrahedron::getP4() const
{
	//return this->getPoint(3);
	return this->points[3];
}

#if USE_EXPERIMENTAL_4
Real Data::Tetrahedron::surface() const
{
    return static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[1]), static_cast<Point3D *>(this->points[2])) +
           static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[1]), static_cast<Point3D *>(this->points[3])) +
           static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[2]), static_cast<Point3D *>(this->points[3])) +
           static_cast<Point3D *>(this->points[1])->surface(static_cast<Point3D *>(this->points[2]), static_cast<Point3D *>(this->points[3]));
}
#endif //#if USE_EXPERIMENTAL_4

#if (!USE_EXPERIMENTAL_4)
void Data::Tetrahedron::setTriangles(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4)
{
	this->polygons.resize(4);

	this->setT1(t1);
	this->setT2(t2);
	this->setT3(t3);
	this->setT4(t4);
}

void Data::Tetrahedron::setT1(Triangle *t)
{
	this->setPolygon(0, t);
}

Triangle *Data::Tetrahedron::getT1() const
{
	return static_cast<Triangle *>(this->getPolygon(0));
}

void Data::Tetrahedron::setT2(Triangle *t)
{
	this->setPolygon(1, t);
}

Triangle *Data::Tetrahedron::getT2() const
{
	return static_cast<Triangle *>(this->getPolygon(1));
}

void Data::Tetrahedron::setT3(Triangle *t)
{
	this->setPolygon(2, t);
}

Triangle *Data::Tetrahedron::getT3() const
{
	return static_cast<Triangle *>(this->getPolygon(2));
}

void Data::Tetrahedron::setT4(Triangle *t)
{
	this->setPolygon(3, t);
}

Triangle *Data::Tetrahedron::getT4() const
{
	return static_cast<Triangle *>(this->getPolygon(3));
}
#endif //#if (!USE_EXPERIMENTAL_4)

UInt Data::Tetrahedron::numSides() const
{
    return 4;
}

#if USE_EXPERIMENTAL
UInt Data::Tetrahedron::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
#endif //#if USE_EXPERIMENTAL

bool Data::Tetrahedron::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Tetrahedron::in(const Point *p) const
{
	/*if (this->dimension() != 3)
	{
		return false;
	}*/

	/*R4Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);
	Real l4 = std::get<3>(t);*/

#if USE_C__11
	Real l1, l2, l3, l4;

	std::tie(l1, l2, l3, l4) = this->barycentricCoordinates(p);
#else
	R4Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
	Real l4 = t.values[3];
#endif //#if USE_C__11

	return ((l1 > 0.0) && (l2 > 0.0) && (l3 > 0.0) && (l4 > 0.0));
	//return ((l1 >= 0.0) && (l2 >= 0.0) && (l3 >= 0.0) && (l4 >= 0.0));
	//return ((l1 > Shape::getTolerance()) && (l2 > Shape::getTolerance()) &&
	//		(l3 > Shape::getTolerance()) && (l4 > Shape::getTolerance()));
}

bool Data::Tetrahedron::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Tetrahedron::on(const Point *p) const
{
	return ((!this->in(p)) && (!this->out(p)));

	/*return ((this->getT1()->in(p)) ||
			(this->getT1()->on(p)) ||
			(this->getT2()->in(p)) ||
			(this->getT2()->on(p)) ||
			(this->getT3()->in(p)) ||
			(this->getT3()->on(p)) ||
			(this->getT4()->in(p)) ||
			(this->getT4()->on(p)));*/
}

bool Data::Tetrahedron::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Tetrahedron::out(const Point *p) const
{
	/*if (this->dimension() != 3)
	{
		return false;
	}*/

	/*R4Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);
	Real l4 = std::get<3>(t);*/

#if USE_C__11
	Real l1, l2, l3, l4;

	std::tie(l1, l2, l3, l4) = this->barycentricCoordinates(p);
#else
	R4Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
	Real l4 = t.values[3];
#endif //#if USE_C__11

	return ((l1 < 0.0) || (l2 < 0.0) || (l3 < 0.0) || (l4 < 0.0));
	//return ((l1 <= 0.0) || (l2 <= 0.0) || (l3 <= 0.0) || (l4 <= 0.0));
	//return ((l1 < Shape::getTolerance()) || (l2 < Shape::getTolerance()) ||
	//		(l3 < Shape::getTolerance()) || (l4 < Shape::getTolerance()));
}

R4Tuple Data::Tetrahedron::barycentricCoordinates(const Point &p) const
{
	return barycentricCoordinates(&p);
}

R4Tuple Data::Tetrahedron::barycentricCoordinates(const Point *p) const
{
	/*if (this->dimension() != 3)
	{
		return std::make_tuple(0.0, 0.0, 0.0, 0.0);
	}*/

	Real vInv = 1.0/this->orientedVolume();

	Real l1, l2, l3, l4;
	l1 = l2 = l3 = l4 = 0.0;

	const Point3D *newp = static_cast<const Point3D *>(p);
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	l1 = newp->orientedVolume(p2, p3, p4)*vInv;
	l2 = p1->orientedVolume(newp, p3, p4)*vInv;
	l3 = p1->orientedVolume(p2, newp, p4)*vInv;
	l4 = p1->orientedVolume(p2, p3, newp)*vInv;

#if USE_C__11
	return std::make_tuple(l1, l2, l3, l4);
#else
	R4Tuple t;

	t.values[0] = l1;
	t.values[1] = l2;
	t.values[2] = l3;
	t.values[3] = l4;

	return t;
#endif //#if USE_C__11
}

Real Data::Tetrahedron::volume() const
{
	/*if (this->dimension() != 3)
	{
		return 0.0;
	}*/

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	return p1->volume(p2, p3, p4);
}

Real Data::Tetrahedron::orientedVolume() const
{
	/*if (this->dimension() != 3)
	{
		return 0.0;
	}*/

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	return p1->orientedVolume(p2, p3, p4);
}

Real Data::Tetrahedron::gamma()
{
	Real v = this->volume();

	if (v < std::pow(Data::getTolerance(), this->dimension()))
	{
		return 0.0;
	}

	/*Real d1 = this->getP1()->distance(this->getP2());
	Real d2 = this->getP1()->distance(this->getP3());
	Real d3 = this->getP1()->distance(this->getP4());
    Real d4 = this->getP2()->distance(this->getP3());
    Real d5 = this->getP2()->distance(this->getP4());
    Real d6 = this->getP3()->distance(this->getP4());

    return std::pow(std::sqrt((d1*d1 + d2*d2 + d3*d3 + d4*d4 + d5*d5 + d6*d6)/6.0), 3.0)/v;*/

    Real sum = 0.0;

    for (UInt i = 0; i < this->numPoints() - 1; i++)
    {
    	for (UInt j = i + 1; j < this->numPoints(); j++)
    	{
    		//Real d = this->getPoint(i)->distance(this->getPoint(j));

    		//sum += d*d;

    		sum += this->getPoint(i)->squaredDistance(this->getPoint(j));
    	}
    }

    return std::pow(std::sqrt(sum/6.0), 3.0)/v;
}

#if (!USE_EXPERIMENTAL_4)
void Data::Tetrahedron::makeTriangles()
{
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	Triangle3D *t1 = new Triangle3D(p1, p3, p2);

	Triangle3D *t2 = new Triangle3D(p1, p2, p4);

	Triangle3D *t3 = new Triangle3D(p2, p3, p4);

	Triangle3D *t4 = new Triangle3D(p3, p1, p4);

	this->setTriangles(t1, t2, t3, t4);
}
#endif //#if (!USE_EXPERIMENTAL_4)

Real Data::Tetrahedron::circumradius() const
{
	Data::Vector3D a(this->getP1(), this->getP2());
	Data::Vector3D b(this->getP1(), this->getP3());
	Data::Vector3D c(this->getP1(), this->getP4());

	Data::Vector3D ab = a.cross(b);
	Data::Vector3D bc = b.cross(c);
	Data::Vector3D ca = c.cross(a);

	Real v6 = std::fabs(a.dot(bc));

	bc.multiply(a.squaredNorm());
	ca.multiply(b.squaredNorm());
	ab.multiply(c.squaredNorm());

	Data::Vector3D sum = bc;
	sum.sum(ca);
	sum.sum(ab);

	return sum.norm()/(2.0*v6);
}

#if USE_EXPERIMENTAL_3
Point3D Data::Tetrahedron::circumcenter() const
#else
Point Data::Tetrahedron::circumcenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	/*if (this->dimension() != 3)
	{
		return Point();
	}*/

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	if (!p1 || !p2 || !p3 || !p4)
	{
#if USE_EXPERIMENTAL_3
		return Point3D();
#else
		return Point();
#endif //#if USE_EXPERIMENTAL_3
	}

	//found in http://en.wikipedia.org/wiki/Tetrahedron

	Vector3D a(p1, p2);
	Vector3D b(p1, p3);
	Vector3D c(p1, p4);

	Vector3D v1 = b.cross(c);
	Vector3D v2 = c.cross(a);
	Vector3D v3 = a.cross(b);

	//Real d = 12.0*this->volume();
	Real d = 2.0*v1.dot(a);

	v1.multiply(a.dot(a));
	v2.multiply(b.dot(b));
	v3.multiply(c.dot(c));

	v1.sum(v2);
	v1.sum(v3);

	v1.multiply(1.0/d);

	Point3D p(p1);

	p.sum(v1);

	return p;
}

Real Data::Tetrahedron::inradius() const
{
	Data::Vector3D a(this->getP1(), this->getP2());
	Data::Vector3D b(this->getP1(), this->getP3());
	Data::Vector3D c(this->getP1(), this->getP4());

	Data::Vector3D ab = a.cross(b);
	Data::Vector3D bc = b.cross(c);
	Data::Vector3D ca = c.cross(a);

	Real v6 = std::fabs(a.dot(bc));

	Data::Vector3D sum = bc;
	sum.sum(ca);
	sum.sum(ab);

	return v6/(bc.norm() + ca.norm() + ab.norm() + sum.norm());
}

#if USE_EXPERIMENTAL_3
Point3D Data::Tetrahedron::incenter() const
#else
Point Data::Tetrahedron::incenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	/*if (this->dimension() != 3)
	{
		return Point();
	}*/

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	if (!p1 || !p2 || !p3 || !p4)
	{
#if USE_EXPERIMENTAL_3
		return Point3D();
#else
		return Point();
#endif //#if USE_EXPERIMENTAL_3
	}

	//found in http://en.wikipedia.org/wiki/Tetrahedron

	Vector3D a(p1, p2);
	Vector3D b(p1, p3);
	Vector3D c(p1, p4);

	Vector3D v1 = b.cross(c);
	Vector3D v2 = c.cross(a);
	Vector3D v3 = a.cross(b);

	Real v1norm = v1.norm();
	Real v2norm = v2.norm();
	Real v3norm = v3.norm();

	v1.sum(v2);
	v1.sum(v3);

	Real d = 1.0/(v1norm + v2norm + v3norm + v1.norm());

	a.multiply(v1norm);
	b.multiply(v2norm);
	c.multiply(v3norm);

	a.sum(b);
	a.sum(c);

	a.multiply(d);

	Point3D p(p1);

	p.sum(a);

	return p;

	/*//found in http://en.wikipedia.org/wiki/Incircle

	Real a = p1->surface(p2, p3);
    Real b = p4->surface(p1, p2);
    Real c = p4->surface(p2, p3);
    Real d = p4->surface(p3, p1);

    Real sum = a + b + c + d;

    Real x = (a*p4->getX() + b*p3->getX() + c*p1->getX() + d*p2->getX())/sum;
    Real y = (a*p4->getY() + b*p3->getY() + c*p1->getY() + d*p2->getY())/sum;
    Real z = (a*p4->getZ() + b*p3->getZ() + c*p1->getZ() + d*p2->getZ())/sum;

    Point3D p(x, y, z);

    return p;*/
}

#if USE_ARRAY
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
Tetrahedron &Data::Tetrahedron::operator=(const Tetrahedron &t)
{
	if (this != &t)
	{
#if (!USE_EXPERIMENTAL_4)
		this->numPts = 4;
		this->GeometricShape::points = this->points;

		this->polygons = t.polygons;
#endif //#if (!USE_EXPERIMENTAL_4)

		UInt dim = t.dimension();

		this->points[0] = (!t.points[0]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[0])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[0])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[0])) : new Point(t.points[0])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[1] = (!t.points[1]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[1])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[1])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[1])) : new Point(t.points[1])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[2] = (!t.points[2]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[2])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[2])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[2])) : new Point(t.points[2])));
#endif //#if USE_EXPERIMENTAL_3
		this->points[3] = (!t.points[3]) ? NULL :
			((dim == 2) ? static_cast<Point *>(new Point2D(t.points[3])) :
#if USE_EXPERIMENTAL_3
                          static_cast<Point *>(new Point3D(t.points[3])));
#else
			((dim == 3) ? static_cast<Point *>(new Point3D(t.points[3])) : new Point(t.points[3])));
#endif //#if USE_EXPERIMENTAL_3
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
#if (!USE_EXPERIMENTAL_4)
void Data::Tetrahedron::setColor(Real r, Real g, Real b) const
{
	Colorable::setColor(r, g, b);

	if (this->getT1())
	{
		this->getT1()->setColor(r, g, b);
	}

	if (this->getT2())
	{
		this->getT2()->setColor(r, g, b);
	}

	if (this->getT3())
	{
		this->getT3()->setColor(r, g, b);
	}

	if (this->getT4())
	{
		this->getT4()->setColor(r, g, b);
	}
}
#endif //#if (!USE_EXPERIMENTAL_4)

//void Data::Tetrahedron::draw(bool fill) const
void Data::Tetrahedron::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	/*if ((fill) &&
		((!this->isHighlighted()) ||
		 (this->dimension() != 3)))
	{
		return;
	}*/

	bool changed = false;
	int modes[2];

	glGetIntegerv(GL_POLYGON_MODE, modes);

	bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

	if ((this->isHighlighted()) && (wireframe))
	{
		changed = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

#if (!USE_EXPERIMENTAL_4)
	if ((!this->getT1()) || (!this->getT2()) || (!this->getT3()) || (!this->getT4()))
#endif //#if (!USE_EXPERIMENTAL_4)
	{
		glPushMatrix();
#if USE_EXPERIMENTAL_3
			this->explodeAndShrink(this->mid3D());
#else
			this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

			glColor(this->getR(), this->getG(), this->getB());

			Point3D *p1 = static_cast<Point3D *>(this->getP1());
			Point3D *p2 = static_cast<Point3D *>(this->getP2());
			Point3D *p3 = static_cast<Point3D *>(this->getP3());
			Point3D *p4 = static_cast<Point3D *>(this->getP4());

			/*Data::Triangle3D t1(p1, p3, p2);
			Data::Triangle3D t2(p1, p2, p4);
			Data::Triangle3D t3(p2, p3, p4);
			Data::Triangle3D t4(p3, p1, p4);

			t1.setColor(this->getR(), this->getG(), this->getB());
			t2.setColor(this->getR(), this->getG(), this->getB());
			t3.setColor(this->getR(), this->getG(), this->getB());
			t4.setColor(this->getR(), this->getG(), this->getB());

			t1.draw();
			t2.draw();
			t3.draw();
			t4.draw();*/
			/*t1.draw(fill);
			t2.draw(fill);
			t3.draw(fill);
			t4.draw(fill);*/

			Vector3D v1, v2, cross;

			glBegin(GL_TRIANGLES);
				v1 = Data::Vector3D(p1, p3);
				v2 = Data::Vector3D(p1, p2);
				cross = v1.cross(v2);

				cross.normalize();

				glNormal(cross.getX(), cross.getY(), cross.getZ());

				glVertex(p1->getX(), p1->getY(), p1->getZ());
				glVertex(p3->getX(), p3->getY(), p3->getZ());
				glVertex(p2->getX(), p2->getY(), p2->getZ());

				v1 = Data::Vector3D(p1, p2);
				v2 = Data::Vector3D(p1, p4);
				cross = v1.cross(v2);

				cross.normalize();

				glNormal(cross.getX(), cross.getY(), cross.getZ());

				glVertex(p1->getX(), p1->getY(), p1->getZ());
				glVertex(p2->getX(), p2->getY(), p2->getZ());
				glVertex(p4->getX(), p4->getY(), p4->getZ());

				v1 = Data::Vector3D(p2, p3);
				v2 = Data::Vector3D(p2, p4);
				cross = v1.cross(v2);

				cross.normalize();

				glNormal(cross.getX(), cross.getY(), cross.getZ());

				glVertex(p2->getX(), p2->getY(), p2->getZ());
				glVertex(p3->getX(), p3->getY(), p3->getZ());
				glVertex(p4->getX(), p4->getY(), p4->getZ());

				v1 = Data::Vector3D(p3, p1);
				v2 = Data::Vector3D(p3, p4);
				cross = v1.cross(v2);

				cross.normalize();

				glNormal(cross.getX(), cross.getY(), cross.getZ());

				glVertex(p3->getX(), p3->getY(), p3->getZ());
				glVertex(p1->getX(), p1->getY(), p1->getZ());
				glVertex(p4->getX(), p4->getY(), p4->getZ());
			glEnd();

			if (!wireframe)
			{
				GLboolean lighting = glIsEnabled(GL_LIGHTING);

				if (!lighting)
				{
					glDisable(GL_LIGHTING);
				}

				glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

				glBegin(GL_LINES);
					glVertex(p1->getX(), p1->getY(), p1->getZ());
					glVertex(p2->getX(), p2->getY(), p2->getZ());

					glVertex(p2->getX(), p2->getY(), p2->getZ());
					glVertex(p3->getX(), p3->getY(), p3->getZ());

					glVertex(p3->getX(), p3->getY(), p3->getZ());
					glVertex(p1->getX(), p1->getY(), p1->getZ());

					glVertex(p1->getX(), p1->getY(), p1->getZ());
					glVertex(p4->getX(), p4->getY(), p4->getZ());

					glVertex(p2->getX(), p2->getY(), p2->getZ());
					glVertex(p4->getX(), p4->getY(), p4->getZ());

					glVertex(p3->getX(), p3->getY(), p3->getZ());
					glVertex(p4->getX(), p4->getY(), p4->getZ());
				glEnd();

				if (lighting)
				{
					glEnable(GL_LIGHTING);
				}
			}

		glPopMatrix();
	}
#if (!USE_EXPERIMENTAL_4)
	else
	{
		this->getT1()->draw();
		this->getT2()->draw();
		this->getT3()->draw();
		this->getT4()->draw();
		/*this->getT1()->draw(fill);
		this->getT2()->draw(fill);
		this->getT3()->draw(fill);
		this->getT4()->draw(fill);*/
	}
#endif //#if (!USE_EXPERIMENTAL_4)

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

UInt Data::Tetrahedron::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = color[9]  = color[12] = color[15] = color[18] = color[21] = color[24] = color[27] = color[30] = color[33] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = color[10] = color[13] = color[16] = color[19] = color[22] = color[25] = color[28] = color[31] = color[34] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = color[11] = color[14] = color[17] = color[20] = color[23] = color[26] = color[29] = color[32] = color[35] = static_cast<GLfloat>(this->getB());
	}

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());

	if (normal)
	{
		Vector3D v1, v2, n;

		v1 = Vector3D(p1, p3);
		v2 = Vector3D(p1, p2);
		n = v1.cross(v2);

		n.normalize();

		normal[0] = normal[3] = normal[6] = static_cast<GLfloat>(n.getX());
		normal[1] = normal[4] = normal[7] = static_cast<GLfloat>(n.getY());
		normal[2] = normal[5] = normal[8] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p1, p2);
		v2 = Vector3D(p1, p4);
		n = v1.cross(v2);

		n.normalize();

		normal[9]  = normal[12] = normal[15] = static_cast<GLfloat>(n.getX());
		normal[10] = normal[13] = normal[16] = static_cast<GLfloat>(n.getY());
		normal[11] = normal[14] = normal[17] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p2, p3);
		v2 = Vector3D(p2, p4);
		n = v1.cross(v2);

		n.normalize();

		normal[18] = normal[21] = normal[24] = static_cast<GLfloat>(n.getX());
		normal[19] = normal[22] = normal[25] = static_cast<GLfloat>(n.getY());
		normal[20] = normal[23] = normal[26] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p3, p1);
		v2 = Vector3D(p3, p4);
		n = v1.cross(v2);

		n.normalize();

		normal[27] = normal[30] = normal[33] = static_cast<GLfloat>(n.getX());
		normal[28] = normal[31] = normal[34] = static_cast<GLfloat>(n.getY());
		normal[29] = normal[32] = normal[35] = static_cast<GLfloat>(n.getZ());
	}

	coord[0] = static_cast<GLfloat>(p1->getX());
	coord[1] = static_cast<GLfloat>(p1->getY());
	coord[2] = static_cast<GLfloat>(p1->getZ());

	coord[3] = static_cast<GLfloat>(p3->getX());
	coord[4] = static_cast<GLfloat>(p3->getY());
	coord[5] = static_cast<GLfloat>(p3->getZ());

	coord[6] = static_cast<GLfloat>(p2->getX());
	coord[7] = static_cast<GLfloat>(p2->getY());
	coord[8] = static_cast<GLfloat>(p2->getZ());

	coord[9]  = static_cast<GLfloat>(p1->getX());
	coord[10] = static_cast<GLfloat>(p1->getY());
	coord[11] = static_cast<GLfloat>(p1->getZ());

	coord[12] = static_cast<GLfloat>(p2->getX());
	coord[13] = static_cast<GLfloat>(p2->getY());
	coord[14] = static_cast<GLfloat>(p2->getZ());

	coord[15] = static_cast<GLfloat>(p4->getX());
	coord[16] = static_cast<GLfloat>(p4->getY());
	coord[17] = static_cast<GLfloat>(p4->getZ());

	coord[18] = static_cast<GLfloat>(p2->getX());
	coord[19] = static_cast<GLfloat>(p2->getY());
	coord[20] = static_cast<GLfloat>(p2->getZ());

	coord[21] = static_cast<GLfloat>(p3->getX());
	coord[22] = static_cast<GLfloat>(p3->getY());
	coord[23] = static_cast<GLfloat>(p3->getZ());

	coord[24] = static_cast<GLfloat>(p4->getX());
	coord[25] = static_cast<GLfloat>(p4->getY());
	coord[26] = static_cast<GLfloat>(p4->getZ());

	coord[27] = static_cast<GLfloat>(p3->getX());
	coord[28] = static_cast<GLfloat>(p3->getY());
	coord[29] = static_cast<GLfloat>(p3->getZ());

	coord[30] = static_cast<GLfloat>(p1->getX());
	coord[31] = static_cast<GLfloat>(p1->getY());
	coord[32] = static_cast<GLfloat>(p1->getZ());

	coord[33] = static_cast<GLfloat>(p4->getX());
	coord[34] = static_cast<GLfloat>(p4->getY());
	coord[35] = static_cast<GLfloat>(p4->getZ());

#if USE_EXPERIMENTAL_3
	Point3D mid = this->mid3D();
#else
	Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

	this->explodeAndShrink(coord, mid);
	this->explodeAndShrink(coord + 3, mid);
	this->explodeAndShrink(coord + 6, mid);
	this->explodeAndShrink(coord + 9, mid);
	this->explodeAndShrink(coord + 12, mid);
	this->explodeAndShrink(coord + 15, mid);
	this->explodeAndShrink(coord + 18, mid);
	this->explodeAndShrink(coord + 21, mid);
	this->explodeAndShrink(coord + 24, mid);
	this->explodeAndShrink(coord + 27, mid);
	this->explodeAndShrink(coord + 30, mid);
	this->explodeAndShrink(coord + 33, mid);

	return 12;
}

UInt Data::Tetrahedron::tfill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    return this->fill(coord, color, normal);
}

bool Data::Tetrahedron::isInVisibleSpace() const
{
	return (!this->isBoxInVisibleSpace()) ? false :
			((Data::getState().isPlaneInclusive()) ? this->isPointInVisibleSpace() : this->arePointsInVisibleSpace());
}
#endif //#if USE_GUI
