#include "Data/Wedge.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/State.h"
#include "Data/Identifiable.h"

using namespace Data;

Data::Wedge::Wedge(Point *p1, Point *p2, Point *p3, Point *p4,
	Point *p5, Point *p6) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
		//Polyhedron(0, 6)
		Polyhedron()
#else
		//Polyhedron(0, 6)
		Polyhedron(6)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 6;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	this->setPoints(p1, p2, p3, p4, p5, p6);
}

Data::Wedge::~Wedge()
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Wedge::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Wedge::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Wedge::numPoints() const
{
	return 6;
}

void Data::Wedge::setPoints(Point *p1, Point *p2, Point *p3, Point *p4,
	Point *p5, Point *p6)
{
	this->setP1(p1);
	this->setP2(p2);
	this->setP3(p3);
	this->setP4(p4);
	this->setP5(p5);
	this->setP6(p6);
}

void Data::Wedge::setP1(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Wedge::getP1() const
{
	//return this->getPoint(0);
	return this->points[0];
}

void Data::Wedge::setP2(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Wedge::getP2() const
{
	//return this->getPoint(1);
	return this->points[1];
}

void Data::Wedge::setP3(Point *p)
{
	//this->setPoint(2, p);
	this->points[2] = p;
}

Point *Data::Wedge::getP3() const
{
	//return this->getPoint(2);
	return this->points[2];
}

void Data::Wedge::setP4(Point *p)
{
	//this->setPoint(3, p);
	this->points[3] = p;
}

Point *Data::Wedge::getP4() const
{
	//return this->getPoint(3);
	return this->points[3];
}

void Data::Wedge::setP5(Point *p)
{
	//this->setPoint(4, p);
	this->points[4] = p;
}

Point *Data::Wedge::getP5() const
{
	//return this->getPoint(4);
	return this->points[4];
}

void Data::Wedge::setP6(Point *p)
{
	//this->setPoint(5, p);
	this->points[5] = p;
}

Point *Data::Wedge::getP6() const
{
	//return this->getPoint(5);
	return this->points[5];
}

#if USE_EXPERIMENTAL_4
Real Data::Wedge::surface() const
{
    return static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[1]), static_cast<Point3D *>(this->points[2])) +
           static_cast<Point3D *>(this->points[3])->surface(static_cast<Point3D *>(this->points[4]), static_cast<Point3D *>(this->points[5])) +
           static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[1]), static_cast<Point3D *>(this->points[4])) +
           static_cast<Point3D *>(this->points[0])->surface(static_cast<Point3D *>(this->points[4]), static_cast<Point3D *>(this->points[3])) +
           static_cast<Point3D *>(this->points[1])->surface(static_cast<Point3D *>(this->points[2]), static_cast<Point3D *>(this->points[5])) +
           static_cast<Point3D *>(this->points[1])->surface(static_cast<Point3D *>(this->points[5]), static_cast<Point3D *>(this->points[4])) +
           static_cast<Point3D *>(this->points[2])->surface(static_cast<Point3D *>(this->points[0]), static_cast<Point3D *>(this->points[3])) +
           static_cast<Point3D *>(this->points[2])->surface(static_cast<Point3D *>(this->points[3]), static_cast<Point3D *>(this->points[5]));
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Wedge::numSides() const
{
    return 5;
}

#if USE_EXPERIMENTAL
UInt Data::Wedge::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
#endif //#if USE_EXPERIMENTAL

bool Data::Wedge::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Wedge::in(const Point */*p*/) const
{
	return false;
}

bool Data::Wedge::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Wedge::on(const Point */*p*/) const
{
	return false;
	//return ((!this->in(p)) && (!this->out(p)));
}

bool Data::Wedge::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Wedge::out(const Point */*p*/) const
{
	return false;
}

Real Data::Wedge::volume() const
{
	return 0.0;
}

Real Data::Wedge::orientedVolume() const
{
	return 0.0;
}

#if USE_ARRAY
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
Wedge &Data::Wedge::operator=(const Wedge &b)
{
	if (this != &b)
	{
#if (!USE_EXPERIMENTAL_4)
		this->numPts = 6;
		this->GeometricShape::points = this->points;
#endif //#if (!USE_EXPERIMENTAL_4)

#if (!USE_EXPERIMENTAL_4)
		this->polygons = b.polygons;
#endif //#if (!USE_EXPERIMENTAL_4)

		UInt dim = b.dimension();

		for (UInt i = 0; i < 6; i++)
		{
			this->points[i] = (!b.points[i]) ? NULL :
				((dim == 2) ? static_cast<Point *>(new Point2D(b.points[i])) :
#if USE_EXPERIMENTAL_3
                              static_cast<Point *>(new Point3D(b.points[i])));
#else
				((dim == 3) ? static_cast<Point *>(new Point3D(b.points[i])) : new Point(b.points[i])));
#endif //#if USE_EXPERIMENTAL_3
		}
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
//void Data::Wedge::draw(bool fill) const
void Data::Wedge::draw() const
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

        if (this->isHighlighted())
		{
			glBegin(GL_TRIANGLES);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		Vector3D v1, v2, n;

		v1 = Vector3D(this->getP1(), this->getP3());
		v2 = Vector3D(this->getP1(), this->getP2());
		n = v1.cross(v2);

		n.normalize();

		glNormal(n.getX(), n.getY(), n.getZ());

		glVertex(this->getP1()->getCoord(0), this->getP1()->getCoord(1), this->getP1()->getCoord(2));
		glVertex(this->getP3()->getCoord(0), this->getP3()->getCoord(1), this->getP3()->getCoord(2));
		glVertex(this->getP2()->getCoord(0), this->getP2()->getCoord(1), this->getP2()->getCoord(2));

		glEnd();

		if (this->isHighlighted())
		{
			glBegin(GL_QUADS);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		v1 = Vector3D(this->getP1(), this->getP1());
		v2 = Vector3D(this->getP1(), this->getP4());
		n = v1.cross(v2);

		n.normalize();

		glNormal(n.getX(), n.getY(), n.getZ());

		glVertex(this->getP1()->getCoord(0), this->getP1()->getCoord(1), this->getP1()->getCoord(2));
		glVertex(this->getP2()->getCoord(0), this->getP2()->getCoord(1), this->getP2()->getCoord(2));
		glVertex(this->getP5()->getCoord(0), this->getP5()->getCoord(1), this->getP5()->getCoord(2));
		glVertex(this->getP4()->getCoord(0), this->getP4()->getCoord(1), this->getP4()->getCoord(2));

		glEnd();

		if (this->isHighlighted())
		{
			glBegin(GL_QUADS);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		v1 = Vector3D(this->getP2(), this->getP3());
		v2 = Vector3D(this->getP2(), this->getP5());
		n = v1.cross(v2);

		n.normalize();

		glNormal(n.getX(), n.getY(), n.getZ());

		glVertex(this->getP2()->getCoord(0), this->getP2()->getCoord(1), this->getP2()->getCoord(2));
		glVertex(this->getP3()->getCoord(0), this->getP3()->getCoord(1), this->getP3()->getCoord(2));
		glVertex(this->getP6()->getCoord(0), this->getP6()->getCoord(1), this->getP6()->getCoord(2));
		glVertex(this->getP5()->getCoord(0), this->getP5()->getCoord(1), this->getP5()->getCoord(2));

		glEnd();

		if (this->isHighlighted())
		{
			glBegin(GL_QUADS);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		v1 = Vector3D(this->getP3(), this->getP1());
		v2 = Vector3D(this->getP3(), this->getP6());
		n = v1.cross(v2);

		n.normalize();

		glNormal(n.getX(), n.getY(), n.getZ());

		glVertex(this->getP3()->getCoord(0), this->getP3()->getCoord(1), this->getP3()->getCoord(2));
		glVertex(this->getP1()->getCoord(0), this->getP1()->getCoord(1), this->getP1()->getCoord(2));
		glVertex(this->getP4()->getCoord(0), this->getP4()->getCoord(1), this->getP4()->getCoord(2));
		glVertex(this->getP6()->getCoord(0), this->getP6()->getCoord(1), this->getP6()->getCoord(2));

		glEnd();

        if (this->isHighlighted())
		{
			glBegin(GL_TRIANGLES);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		v1 = Vector3D(this->getP4(), this->getP5());
		v2 = Vector3D(this->getP4(), this->getP6());
		n = v1.cross(v2);

		n.normalize();

		glNormal(n.getX(), n.getY(), n.getZ());

		glVertex(this->getP4()->getCoord(0), this->getP4()->getCoord(1), this->getP4()->getCoord(2));
		glVertex(this->getP5()->getCoord(0), this->getP5()->getCoord(1), this->getP5()->getCoord(2));
		glVertex(this->getP6()->getCoord(0), this->getP6()->getCoord(1), this->getP6()->getCoord(2));

		glEnd();

    glPopMatrix();

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

UInt Data::Wedge::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = color[9]  = color[12] = color[15] = color[18] = color[21] = color[24] = color[27] = color[30] = color[33] =
		color[36] = color[39] = color[42] = color[45]  = color[48] = color[51] = color[54] = color[57] = color[60] = color[63] = color[66] = color[69] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = color[10] = color[13] = color[16] = color[19] = color[22] = color[25] = color[28] = color[31] = color[34] =
		color[37] = color[40] = color[43] = color[46] = color[49] = color[52] = color[55] = color[58] = color[61] = color[64] = color[67] = color[70] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = color[11] = color[14] = color[17] = color[20] = color[23] = color[26] = color[29] = color[32] = color[35] =
		color[38] = color[41] = color[44] = color[47] = color[50] = color[53] = color[56] = color[59] = color[62] = color[65] = color[68] = color[71] = static_cast<GLfloat>(this->getB());
	}

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());
	Point3D *p5 = static_cast<Point3D *>(this->getP5());
	Point3D *p6 = static_cast<Point3D *>(this->getP6());

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

		normal[18] = normal[21] = normal[24] = static_cast<GLfloat>(n.getX());
		normal[19] = normal[22] = normal[25] = static_cast<GLfloat>(n.getY());
		normal[20] = normal[23] = normal[26] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p2, p3);
		v2 = Vector3D(p2, p5);
		n = v1.cross(v2);

		n.normalize();

		normal[27] = normal[30] = normal[33] = static_cast<GLfloat>(n.getX());
		normal[28] = normal[31] = normal[34] = static_cast<GLfloat>(n.getY());
		normal[29] = normal[32] = normal[35] = static_cast<GLfloat>(n.getZ());

		normal[36] = normal[39] = normal[42] = static_cast<GLfloat>(n.getX());
		normal[37] = normal[40] = normal[43] = static_cast<GLfloat>(n.getY());
		normal[38] = normal[41] = normal[44] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p3, p1);
		v2 = Vector3D(p3, p6);
		n = v1.cross(v2);

		n.normalize();

		normal[45] = normal[48] = normal[51] = static_cast<GLfloat>(n.getX());
		normal[46] = normal[49] = normal[52] = static_cast<GLfloat>(n.getY());
		normal[47] = normal[50] = normal[53] = static_cast<GLfloat>(n.getZ());

		normal[54] = normal[57] = normal[60] = static_cast<GLfloat>(n.getX());
		normal[55] = normal[58] = normal[61] = static_cast<GLfloat>(n.getY());
		normal[56] = normal[59] = normal[62] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p4, p5);
		v2 = Vector3D(p4, p6);
		n = v1.cross(v2);

		n.normalize();

		normal[63] = normal[66] = normal[69] = static_cast<GLfloat>(n.getX());
		normal[64] = normal[67] = normal[70] = static_cast<GLfloat>(n.getY());
		normal[65] = normal[68] = normal[71] = static_cast<GLfloat>(n.getZ());
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

	//color[0] = color[3] = color[6] = 1.0;
	//color[1] = color[4] = color[7] = 0.0;
	//color[2] = color[5] = color[8] = 0.0;

	coord[9]  = static_cast<GLfloat>(p1->getX());
	coord[10] = static_cast<GLfloat>(p1->getY());
	coord[11] = static_cast<GLfloat>(p1->getZ());

	coord[12] = static_cast<GLfloat>(p2->getX());
	coord[13] = static_cast<GLfloat>(p2->getY());
	coord[14] = static_cast<GLfloat>(p2->getZ());

	coord[15] = static_cast<GLfloat>(p5->getX());
	coord[16] = static_cast<GLfloat>(p5->getY());
	coord[17] = static_cast<GLfloat>(p5->getZ());

	//color[9]  = color[12] = color[15] = 0.0;
	//color[10] = color[13] = color[16] = 1.0;
	//color[11] = color[14] = color[17] = 0.0;

	coord[18] = static_cast<GLfloat>(p1->getX());
	coord[19] = static_cast<GLfloat>(p1->getY());
	coord[20] = static_cast<GLfloat>(p1->getZ());

	coord[21] = static_cast<GLfloat>(p5->getX());
	coord[22] = static_cast<GLfloat>(p5->getY());
	coord[23] = static_cast<GLfloat>(p5->getZ());

	coord[24] = static_cast<GLfloat>(p4->getX());
	coord[25] = static_cast<GLfloat>(p4->getY());
	coord[26] = static_cast<GLfloat>(p4->getZ());

	//color[18] = color[21] = color[24] = 0.0;
	//color[19] = color[22] = color[25] = 0.0;
	//color[20] = color[23] = color[26] = 1.0;

	coord[27] = static_cast<GLfloat>(p2->getX());
	coord[28] = static_cast<GLfloat>(p2->getY());
	coord[29] = static_cast<GLfloat>(p2->getZ());

	coord[30] = static_cast<GLfloat>(p3->getX());
	coord[31] = static_cast<GLfloat>(p3->getY());
	coord[32] = static_cast<GLfloat>(p3->getZ());

	coord[33] = static_cast<GLfloat>(p6->getX());
	coord[34] = static_cast<GLfloat>(p6->getY());
	coord[35] = static_cast<GLfloat>(p6->getZ());

	//color[27] = color[30] = color[33] = 1.0;
	//color[28] = color[31] = color[34] = 1.0;
	//color[29] = color[32] = color[35] = 0.0;

	coord[36] = static_cast<GLfloat>(p2->getX());
	coord[37] = static_cast<GLfloat>(p2->getY());
	coord[38] = static_cast<GLfloat>(p2->getZ());

	coord[39] = static_cast<GLfloat>(p6->getX());
	coord[40] = static_cast<GLfloat>(p6->getY());
	coord[41] = static_cast<GLfloat>(p6->getZ());

	coord[42] = static_cast<GLfloat>(p5->getX());
	coord[43] = static_cast<GLfloat>(p5->getY());
	coord[44] = static_cast<GLfloat>(p5->getZ());

	//color[36] = color[39] = color[42] = 1.0;
	//color[37] = color[40] = color[43] = 0.0;
	//color[38] = color[41] = color[44] = 1.0;

	coord[45] = static_cast<GLfloat>(p3->getX());
	coord[46] = static_cast<GLfloat>(p3->getY());
	coord[47] = static_cast<GLfloat>(p3->getZ());

	coord[48] = static_cast<GLfloat>(p1->getX());
	coord[49] = static_cast<GLfloat>(p1->getY());
	coord[50] = static_cast<GLfloat>(p1->getZ());

	coord[51] = static_cast<GLfloat>(p4->getX());
	coord[52] = static_cast<GLfloat>(p4->getY());
	coord[53] = static_cast<GLfloat>(p4->getZ());

	//color[45] = color[48] = color[51] = 0.0;
	//color[46] = color[49] = color[52] = 1.0;
	//color[47] = color[50] = color[53] = 1.0;

	coord[54] = static_cast<GLfloat>(p3->getX());
	coord[55] = static_cast<GLfloat>(p3->getY());
	coord[56] = static_cast<GLfloat>(p3->getZ());

	coord[57] = static_cast<GLfloat>(p4->getX());
	coord[58] = static_cast<GLfloat>(p4->getY());
	coord[59] = static_cast<GLfloat>(p4->getZ());

	coord[60] = static_cast<GLfloat>(p6->getX());
	coord[61] = static_cast<GLfloat>(p6->getY());
	coord[62] = static_cast<GLfloat>(p6->getZ());

	//color[54] = color[57] = color[60] = 0.5;
	//color[55] = color[58] = color[61] = 0.0;
	//color[56] = color[59] = color[62] = 0.0;

	coord[63] = static_cast<GLfloat>(p4->getX());
	coord[64] = static_cast<GLfloat>(p4->getY());
	coord[65] = static_cast<GLfloat>(p4->getZ());

	coord[66] = static_cast<GLfloat>(p5->getX());
	coord[67] = static_cast<GLfloat>(p5->getY());
	coord[68] = static_cast<GLfloat>(p5->getZ());

	coord[69] = static_cast<GLfloat>(p6->getX());
	coord[70] = static_cast<GLfloat>(p6->getY());
	coord[71] = static_cast<GLfloat>(p6->getZ());

	//color[63] = color[66] = color[69] = 0.0;
	//color[64] = color[67] = color[70] = 0.5;
	//color[65] = color[68] = color[71] = 0.0;

#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Point2D mid = this->mid2D();

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
        this->explodeAndShrink(coord + 36, mid);
        this->explodeAndShrink(coord + 39, mid);
        this->explodeAndShrink(coord + 42, mid);
        this->explodeAndShrink(coord + 45, mid);
        this->explodeAndShrink(coord + 48, mid);
        this->explodeAndShrink(coord + 51, mid);
        this->explodeAndShrink(coord + 54, mid);
        this->explodeAndShrink(coord + 57, mid);
        this->explodeAndShrink(coord + 60, mid);
        this->explodeAndShrink(coord + 63, mid);
        this->explodeAndShrink(coord + 66, mid);
        this->explodeAndShrink(coord + 69, mid);
    }
    else
    {
        Point3D mid = this->mid3D();

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
        this->explodeAndShrink(coord + 36, mid);
        this->explodeAndShrink(coord + 39, mid);
        this->explodeAndShrink(coord + 42, mid);
        this->explodeAndShrink(coord + 45, mid);
        this->explodeAndShrink(coord + 48, mid);
        this->explodeAndShrink(coord + 51, mid);
        this->explodeAndShrink(coord + 54, mid);
        this->explodeAndShrink(coord + 57, mid);
        this->explodeAndShrink(coord + 60, mid);
        this->explodeAndShrink(coord + 63, mid);
        this->explodeAndShrink(coord + 66, mid);
        this->explodeAndShrink(coord + 69, mid);
    }
#else
	Point mid = this->mid();

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
	this->explodeAndShrink(coord + 36, mid);
	this->explodeAndShrink(coord + 39, mid);
	this->explodeAndShrink(coord + 42, mid);
	this->explodeAndShrink(coord + 45, mid);
	this->explodeAndShrink(coord + 48, mid);
	this->explodeAndShrink(coord + 51, mid);
	this->explodeAndShrink(coord + 54, mid);
	this->explodeAndShrink(coord + 57, mid);
	this->explodeAndShrink(coord + 60, mid);
	this->explodeAndShrink(coord + 63, mid);
	this->explodeAndShrink(coord + 66, mid);
	this->explodeAndShrink(coord + 69, mid);
#endif //#if USE_EXPERIMENTAL_3

	return 24;
}

UInt Data::Wedge::qfill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = color[9]  = color[12] = color[15] = color[18] = color[21] = color[24] = color[27] = color[30] = color[33] =
		static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = color[10] = color[13] = color[16] = color[19] = color[22] = color[25] = color[28] = color[31] = color[34] =
		static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = color[11] = color[14] = color[17] = color[20] = color[23] = color[26] = color[29] = color[32] = color[35] =
		static_cast<GLfloat>(this->getB());
	}

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());
	Point3D *p5 = static_cast<Point3D *>(this->getP5());
	Point3D *p6 = static_cast<Point3D *>(this->getP6());

	if (normal)
	{
		Vector3D v1, v2, n;

		v1 = Vector3D(p1, p2);
		v2 = Vector3D(p1, p4);
		n = v1.cross(v2);

		n.normalize();

		normal[0] = normal[3] = normal[6] = normal[9]  = static_cast<GLfloat>(n.getX());
		normal[1] = normal[4] = normal[7] = normal[10] = static_cast<GLfloat>(n.getY());
		normal[2] = normal[5] = normal[8] = normal[11] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p2, p3);
		v2 = Vector3D(p2, p5);
		n = v1.cross(v2);

		n.normalize();

		normal[12] = normal[15] = normal[18] = normal[21] = static_cast<GLfloat>(n.getX());
		normal[13] = normal[16] = normal[19] = normal[22] = static_cast<GLfloat>(n.getY());
		normal[14] = normal[17] = normal[20] = normal[23] = static_cast<GLfloat>(n.getZ());

		v1 = Vector3D(p3, p1);
		v2 = Vector3D(p3, p6);
		n = v1.cross(v2);

		n.normalize();

		normal[24] = normal[27] = normal[30] = normal[33] = static_cast<GLfloat>(n.getX());
		normal[25] = normal[28] = normal[31] = normal[34] = static_cast<GLfloat>(n.getY());
		normal[26] = normal[29] = normal[32] = normal[35] = static_cast<GLfloat>(n.getZ());
	}

	coord[0] = static_cast<GLfloat>(p1->getX());
	coord[1] = static_cast<GLfloat>(p1->getY());
	coord[2] = static_cast<GLfloat>(p1->getZ());

	coord[3] = static_cast<GLfloat>(p2->getX());
	coord[4] = static_cast<GLfloat>(p2->getY());
	coord[5] = static_cast<GLfloat>(p2->getZ());

	coord[6] = static_cast<GLfloat>(p5->getX());
	coord[7] = static_cast<GLfloat>(p5->getY());
	coord[8] = static_cast<GLfloat>(p5->getZ());

	coord[9] = static_cast<GLfloat>(p4->getX());
	coord[10] = static_cast<GLfloat>(p4->getY());
	coord[11] = static_cast<GLfloat>(p4->getZ());

	//----

	coord[12] = static_cast<GLfloat>(p2->getX());
	coord[13] = static_cast<GLfloat>(p2->getY());
	coord[14] = static_cast<GLfloat>(p2->getZ());

	coord[15] = static_cast<GLfloat>(p3->getX());
	coord[16] = static_cast<GLfloat>(p3->getY());
	coord[17] = static_cast<GLfloat>(p3->getZ());

	coord[18] = static_cast<GLfloat>(p6->getX());
	coord[19] = static_cast<GLfloat>(p6->getY());
	coord[20] = static_cast<GLfloat>(p6->getZ());

	coord[21] = static_cast<GLfloat>(p5->getX());
	coord[22] = static_cast<GLfloat>(p5->getY());
	coord[23] = static_cast<GLfloat>(p5->getZ());

	//----

	coord[24] = static_cast<GLfloat>(p3->getX());
	coord[25] = static_cast<GLfloat>(p3->getY());
	coord[26] = static_cast<GLfloat>(p3->getZ());

	coord[27] = static_cast<GLfloat>(p1->getX());
	coord[28] = static_cast<GLfloat>(p1->getY());
	coord[29] = static_cast<GLfloat>(p1->getZ());

	coord[30] = static_cast<GLfloat>(p4->getX());
	coord[31] = static_cast<GLfloat>(p4->getY());
	coord[32] = static_cast<GLfloat>(p4->getZ());

	coord[33] = static_cast<GLfloat>(p6->getX());
	coord[34] = static_cast<GLfloat>(p6->getY());
	coord[35] = static_cast<GLfloat>(p6->getZ());

	//----

#if USE_EXPERIMENTAL_3
	Point3D mid = this->mid3D();

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
#else
	Point mid = this->mid();

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
#endif //#if USE_EXPERIMENTAL_3

	return 12;
}

UInt Data::Wedge::tfill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = color[9]  = color[12] = color[15] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = color[10] = color[13] = color[16] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = color[11] = color[14] = color[17] = static_cast<GLfloat>(this->getB());
	}

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());
	Point3D *p4 = static_cast<Point3D *>(this->getP4());
	Point3D *p5 = static_cast<Point3D *>(this->getP5());
	Point3D *p6 = static_cast<Point3D *>(this->getP6());

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

		v1 = Vector3D(p4, p5);
		v2 = Vector3D(p4, p6);
		n = v1.cross(v2);

		n.normalize();

		normal[9]  = normal[12] = normal[15] = static_cast<GLfloat>(n.getX());
		normal[10] = normal[13] = normal[16] = static_cast<GLfloat>(n.getY());
		normal[11] = normal[14] = normal[17] = static_cast<GLfloat>(n.getZ());
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

	//---

	coord[9] = static_cast<GLfloat>(p4->getX());
	coord[10] = static_cast<GLfloat>(p4->getY());
	coord[11] = static_cast<GLfloat>(p4->getZ());

	coord[12] = static_cast<GLfloat>(p5->getX());
	coord[13] = static_cast<GLfloat>(p5->getY());
	coord[14] = static_cast<GLfloat>(p5->getZ());

	coord[15] = static_cast<GLfloat>(p6->getX());
	coord[16] = static_cast<GLfloat>(p6->getY());
	coord[17] = static_cast<GLfloat>(p6->getZ());

	//---

#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Point2D mid = this->mid2D();

        this->explodeAndShrink(coord, mid);
        this->explodeAndShrink(coord + 3, mid);
        this->explodeAndShrink(coord + 6, mid);
        this->explodeAndShrink(coord + 9, mid);
        this->explodeAndShrink(coord + 12, mid);
        this->explodeAndShrink(coord + 15, mid);
    }
    else
    {
        Point3D mid = this->mid3D();

        this->explodeAndShrink(coord, mid);
        this->explodeAndShrink(coord + 3, mid);
        this->explodeAndShrink(coord + 6, mid);
        this->explodeAndShrink(coord + 9, mid);
        this->explodeAndShrink(coord + 12, mid);
        this->explodeAndShrink(coord + 15, mid);
    }
#else
	Point mid = this->mid();

	this->explodeAndShrink(coord, mid);
	this->explodeAndShrink(coord + 3, mid);
	this->explodeAndShrink(coord + 6, mid);
	this->explodeAndShrink(coord + 9, mid);
	this->explodeAndShrink(coord + 12, mid);
	this->explodeAndShrink(coord + 15, mid);
#endif //#if USE_EXPERIMENTAL_3

	return 6;
}

bool Data::Wedge::isInVisibleSpace() const
{
	return (!this->isBoxInVisibleSpace()) ? false :
			((Data::getState().isPlaneInclusive()) ? this->isPointInVisibleSpace() : this->arePointsInVisibleSpace());
}
#endif //#if USE_GUI
