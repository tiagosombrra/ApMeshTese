#include "Data/Quadrilateral.h"

#include "Data/Point2D.h"
#include "Data/Vector2D.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Edge.h"
#include "Data/Triangle2D.h"
#include "Data/Triangle3D.h"

#include "Data/Identifiable.h"
#include "Data/State.h"

using namespace Data;

Data::Quadrilateral::Quadrilateral(Point *p1, Point *p2, Point *p3, Point *p4) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	Polygon()
#else
	Polygon(4)
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

Data::Quadrilateral::Quadrilateral(const Point& p1, const Point &p2, const Point &p3, const Point &p4) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	Polygon()
#else
	Polygon(4)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 4;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_EXPERIMENTAL_3
    UInt dim = p1.dimension();

	this->setPoints(
        (dim == 2) ? static_cast<Point *>(new Point2D(p1)) : static_cast<Point *>(new Point3D(p1)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p2)) : static_cast<Point *>(new Point3D(p2)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p3)) : static_cast<Point *>(new Point3D(p3)),
        (dim == 2) ? static_cast<Point *>(new Point2D(p4)) : static_cast<Point *>(new Point3D(p4)));
#else
	this->setPoints(new Point(p1), new Point(p2), new Point(p3), new Point(p4));
#endif //#if USE_EXPERIMENTAL_3
}

Data::Quadrilateral::~Quadrilateral()
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Quadrilateral::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Quadrilateral::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Quadrilateral::numPoints() const
{
	return 4;
}

void Data::Quadrilateral::setPoints(Point *p1, Point *p2, Point *p3, Point *p4)
{
	this->setP1(p1);
	this->setP2(p2);
	this->setP3(p3);
	this->setP4(p4);
}

void Data::Quadrilateral::setP1(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Quadrilateral::getP1() const
{
	//return (this->numPoints() > 0) ? this->getPoint(0) : NULL;
	return this->points[0];
}

void Data::Quadrilateral::setP2(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Quadrilateral::getP2() const
{
	//return (this->numPoints() > 1) ? this->getPoint(1) : NULL;
	return this->points[1];
}

void Data::Quadrilateral::setP3(Point *p)
{
	//this->setPoint(2, p);
	this->points[2] = p;
}

Point *Data::Quadrilateral::getP3() const
{
	//return (this->numPoints() > 2) ? this->getPoint(2) : NULL;
	return this->points[2];
}

void Data::Quadrilateral::setP4(Point *p)
{
	//this->setPoint(3, p);
	this->points[3] = p;
}

Point *Data::Quadrilateral::getP4() const
{
	//return (this->numPoints() > 2) ? this->getPoint(3) : NULL;
	return this->points[3];
}

UInt Data::Quadrilateral::numSides() const
{
    return 4;
}

#if USE_EXPERIMENTAL
UInt Data::Quadrilateral::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
#endif //#if USE_EXPERIMENTAL

bool Data::Quadrilateral::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Quadrilateral::in(const Point */*p*/) const
{
	return false;
}

bool Data::Quadrilateral::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Quadrilateral::on(const Point */*p*/) const
{
	//return ((!this->in(p)) && (!this->out(p)));
	return false;
}

bool Data::Quadrilateral::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Quadrilateral::out(const Point */*p*/) const
{
	return false;
}

Real Data::Quadrilateral::surface() const
{
	return std::fabs(this->orientedSurface());
}

Real Data::Quadrilateral::orientedSurface() const
{
	if (this->dimension() == 2)
	{
		Data::Triangle2D t1(static_cast<Data::Point2D *>(this->getP1()), static_cast<Data::Point2D *>(this->getP2()), static_cast<Data::Point2D *>(this->getP3()));
		Data::Triangle2D t2(static_cast<Data::Point2D *>(this->getP1()), static_cast<Data::Point2D *>(this->getP3()), static_cast<Data::Point2D *>(this->getP4()));

		return t1.orientedSurface() + t2.orientedSurface();
	}

	Data::Triangle3D t1(static_cast<Data::Point3D *>(this->getP1()), static_cast<Data::Point3D *>(this->getP2()), static_cast<Data::Point3D *>(this->getP3()));
	Data::Triangle3D t2(static_cast<Data::Point3D *>(this->getP1()), static_cast<Data::Point3D *>(this->getP3()), static_cast<Data::Point3D *>(this->getP4()));

	return t1.orientedSurface() + t2.orientedSurface();
}

bool Data::Quadrilateral::match(const Quadrilateral &q) const
{
	return this->match(*(q.getP1()), *(q.getP2()), *(q.getP3()), *(q.getP4()));
}

bool Data::Quadrilateral::match(const Quadrilateral *t) const
{
	return this->match(t->getP1(), t->getP2(), t->getP3(), t->getP4());
}

bool Data::Quadrilateral::match(const Point &p1, const Point &p2, const Point &p3, const Point &p4) const
{
	return (((this->getP1()->match(p1)) && (this->getP2()->match(p2)) && (this->getP3()->match(p3)) && (this->getP4()->match(p4))) ||
			((this->getP1()->match(p1)) && (this->getP2()->match(p4)) && (this->getP3()->match(p3)) && (this->getP4()->match(p2))) ||
			((this->getP1()->match(p2)) && (this->getP2()->match(p3)) && (this->getP3()->match(p4)) && (this->getP4()->match(p1))) ||
			((this->getP1()->match(p2)) && (this->getP2()->match(p1)) && (this->getP3()->match(p4)) && (this->getP4()->match(p3))) ||
			((this->getP1()->match(p3)) && (this->getP2()->match(p4)) && (this->getP3()->match(p1)) && (this->getP4()->match(p2))) ||
			((this->getP1()->match(p3)) && (this->getP2()->match(p2)) && (this->getP3()->match(p1)) && (this->getP4()->match(p4))) ||
			((this->getP1()->match(p4)) && (this->getP2()->match(p1)) && (this->getP3()->match(p2)) && (this->getP4()->match(p3))) ||
			((this->getP1()->match(p4)) && (this->getP2()->match(p3)) && (this->getP3()->match(p2)) && (this->getP4()->match(p1))));
}

bool Data::Quadrilateral::match(const Point *p1, const Point *p2, const Point *p3, const Point *p4) const
{
	return (((this->getP1() == p1) && (this->getP2() == p2) && (this->getP3() == p3) && (this->getP4() == p4)) ||
			((this->getP1() == p1) && (this->getP2() == p4) && (this->getP3() == p3) && (this->getP4() == p2)) ||
			((this->getP1() == p2) && (this->getP2() == p3) && (this->getP3() == p4) && (this->getP4() == p1)) ||
			((this->getP1() == p2) && (this->getP2() == p1) && (this->getP3() == p4) && (this->getP4() == p3)) ||
			((this->getP1() == p3) && (this->getP2() == p4) && (this->getP3() == p1) && (this->getP4() == p2)) ||
			((this->getP1() == p3) && (this->getP2() == p2) && (this->getP3() == p1) && (this->getP4() == p4)) ||
			((this->getP1() == p4) && (this->getP2() == p1) && (this->getP3() == p2) && (this->getP4() == p3)) ||
			((this->getP1() == p4) && (this->getP2() == p3) && (this->getP3() == p2) && (this->getP4() == p1)));
}

#if USE_ARRAY
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
Quadrilateral &Data::Quadrilateral::operator=(const Quadrilateral &q)
{
	if (this != &q)
	{
#if (!USE_EXPERIMENTAL_4)
		this->numPts = 4;
		this->GeometricShape::points = this->points;
#endif //#if (!USE_EXPERIMENTAL_4)

		UInt dim = q.dimension();

		for (UInt i = 0; i < 4; i++)
		{
			this->points[i] = (!q.points[i]) ? NULL :
				((dim == 2) ? static_cast<Point *>(new Point2D(q.points[i])) :
#if USE_EXPERIMENTAL_3
                              static_cast<Point *>(new Point3D(q.points[i])));
#else
				((dim == 3) ? static_cast<Point *>(new Point3D(q.points[i])) : new Point(q.points[i])));
#endif //#if USE_EXPERIMENTAL_3
		}
	}

	return *this;
}
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Quadrilateral::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	bool changed = false;
	int modes[2];

	glColor(this->getR(), this->getG(), this->getB());

	if (this->dimension() == 2)
	{
		Point2D *p1 = static_cast<Point2D *>(this->getP1());
		Point2D *p2 = static_cast<Point2D *>(this->getP2());
		Point2D *p3 = static_cast<Point2D *>(this->getP3());
		Point2D *p4 = static_cast<Point2D *>(this->getP4());

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

			glBegin(GL_QUADS);

				glVertex(p1->getX(), p1->getY());
				glVertex(p2->getX(), p2->getY());
				glVertex(p3->getX(), p3->getY());
				glVertex(p4->getX(), p4->getY());

			glEnd();
		glPopMatrix();
	}
	else if (this->dimension() == 3)
	{
		Point3D *p1 = static_cast<Point3D *>(this->getP1());
		Point3D *p2 = static_cast<Point3D *>(this->getP2());
		Point3D *p3 = static_cast<Point3D *>(this->getP3());
		Point3D *p4 = static_cast<Point3D *>(this->getP4());

		Vector3D n = this->normal();
		glNormal(n.getX(), n.getY(), n.getZ());

		glGetIntegerv(GL_POLYGON_MODE, modes);

		bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

		if ((this->isHighlighted()) && (wireframe))
		{
			changed = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glPushMatrix();
#if USE_EXPERIMENTAL_3
			this->explodeAndShrink(this->mid3D());
#else
			this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

			glBegin(GL_QUADS);
				glVertex(p1->getX(), p1->getY(), p1->getZ());
				glVertex(p2->getX(), p2->getY(), p2->getZ());
				glVertex(p3->getX(), p3->getY(), p3->getZ());
				glVertex(p4->getX(), p4->getY(), p4->getZ());
			glEnd();

			if (!wireframe)
			{
				GLboolean lighting = glIsEnabled(GL_LIGHTING);

				if (lighting)
				{
					glDisable(GL_LIGHTING);
				}

				glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

				glBegin(GL_LINE_LOOP);
					glVertex(p1->getX(), p1->getY(), p1->getZ());
					glVertex(p2->getX(), p2->getY(), p2->getZ());
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

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

UInt Data::Quadrilateral::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    return this->fill(coord, color, normal, NULL);
}

UInt Data::Quadrilateral::fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const
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

	if (this->dimension() == 2)
	{
		coord[0] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getY());

		coord[2] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getX());
		coord[3] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getY());

		coord[4] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getX());
		coord[5] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getY());

		coord[6] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getX());
		coord[7] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getY());

		coord[8] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getX());
		coord[9] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getY());

		coord[10] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP4())->getX());
		coord[11] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP4())->getY());

#if USE_EXPERIMENTAL_3
		Point2D mid = this->mid2D();
#else
		Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

		this->explodeAndShrink(coord, mid);
		this->explodeAndShrink(coord + 2, mid);
		this->explodeAndShrink(coord + 4, mid);
		this->explodeAndShrink(coord + 6, mid);
		this->explodeAndShrink(coord + 8, mid);
		this->explodeAndShrink(coord + 10, mid);
	}
	else if (this->dimension() == 3)
	{
		if (normal)
		{
			if (adjacency)
            {
                Vector3D n[4];

                for (UInt i = 0; i < 4; i++)
                {
                    if (adjacency[i].empty())
                    {
                        n[i] = this->normal();

                        continue;
                    }

                    for (Data::PolygonList::iterator iter = adjacency[i].begin();
                         iter != adjacency[i].end(); iter++)
                    {
                        Data::Vector3D normal = (*iter)->normal();

                        normal.normalize();

                        UInt j = 0;

                        for (j = 0; j < (*iter)->numPoints(); j++)
                        {
                            if ((*iter)->getPoint(j) == this->getPoint(i))
                            {
                                break;
                            }
                        }

                        if (j == (*iter)->numPoints())
                        {
                            n[i].sum(normal);

                            continue;
                        }

                        UInt next = (j + 1)%(*iter)->numPoints();
                        UInt prev = (j + (*iter)->numPoints() - 1)%(*iter)->numPoints();

                        Data::Vector3D v1(this->getPoint(i), (*iter)->getPoint(next));
                        Data::Vector3D v2(this->getPoint(i), (*iter)->getPoint(prev));

                        Real ang = v1.angle(v2);

                        normal.multiply(ang);

                        n[i].sum(normal);
                    }
                }

                n[0].normalize();
                n[1].normalize();
                n[2].normalize();
                n[3].normalize();

                normal[0] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getX()) : static_cast<GLfloat>(n[0].getX());
                normal[1] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getY()) : static_cast<GLfloat>(n[0].getY());
                normal[2] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getZ()) : static_cast<GLfloat>(n[0].getZ());

                normal[3] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getX()) : static_cast<GLfloat>(n[1].getX());
                normal[4] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getY()) : static_cast<GLfloat>(n[1].getY());
                normal[5] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getZ()) : static_cast<GLfloat>(n[1].getZ());

                normal[6] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getX()) : static_cast<GLfloat>(n[2].getX());
                normal[7] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getY()) : static_cast<GLfloat>(n[2].getY());
                normal[8] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getZ()) : static_cast<GLfloat>(n[2].getZ());

                normal[9]  = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getX()) : static_cast<GLfloat>(n[0].getX());
                normal[10] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getY()) : static_cast<GLfloat>(n[0].getY());
                normal[11] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getZ()) : static_cast<GLfloat>(n[0].getZ());

                normal[12] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getX()) : static_cast<GLfloat>(n[2].getX());
                normal[13] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getY()) : static_cast<GLfloat>(n[2].getY());
                normal[14] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getZ()) : static_cast<GLfloat>(n[2].getZ());

                normal[15] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getX()) : static_cast<GLfloat>(n[3].getX());
                normal[16] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getY()) : static_cast<GLfloat>(n[3].getY());
                normal[17] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getZ()) : static_cast<GLfloat>(n[3].getZ());
            }
            else
            {
                Vector3D n = this->normal();

                normal[0] = normal[3] = normal[6] = normal[9] =  normal[12] = normal[15] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getX()) : static_cast<GLfloat>(n.getX());
                normal[1] = normal[4] = normal[7] = normal[10] = normal[13] = normal[16] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getY()) : static_cast<GLfloat>(n.getY());
                normal[2] = normal[5] = normal[8] = normal[11] = normal[14] = normal[17] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getZ()) : static_cast<GLfloat>(n.getZ());
            }
		}

		coord[0] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getY());
		coord[2] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getZ());

		coord[3] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getX());
		coord[4] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getY());
		coord[5] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getZ());

		coord[6] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getX());
		coord[7] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getY());
		coord[8] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getZ());

		coord[9] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getX());
		coord[10] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getY());
		coord[11] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getZ());

		coord[12] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getX());
		coord[13] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getY());
		coord[14] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getZ());

		coord[15] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getX());
		coord[16] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getY());
		coord[17] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getZ());

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
	}

	return 6;
}

UInt Data::Quadrilateral::qfill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    return this->qfill(coord, color, normal, NULL);
}

UInt Data::Quadrilateral::qfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = color[9]  = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = color[10] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = color[11] = static_cast<GLfloat>(this->getB());
	}

	if (this->dimension() == 2)
	{
		coord[0] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP1())->getY());

		coord[2] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getX());
		coord[3] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP2())->getY());

		coord[4] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getX());
		coord[5] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP3())->getY());

		coord[6] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP4())->getX());
		coord[7] = static_cast<GLfloat>(static_cast<Point2D *>(this->getP4())->getY());

#if USE_EXPERIMENTAL_3
		Point2D mid = this->mid2D();
#else
		Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

		this->explodeAndShrink(coord, mid);
		this->explodeAndShrink(coord + 2, mid);
		this->explodeAndShrink(coord + 4, mid);
		this->explodeAndShrink(coord + 6, mid);
	}
	else if (this->dimension() == 3)
	{
		if (normal)
		{
			if (adjacency)
            {
                Vector3D n[4];

                for (UInt i = 0; i < 4; i++)
                {
                    if (adjacency[i].empty())
                    {
                        n[i] = this->normal();

                        continue;
                    }

                    for (Data::PolygonList::iterator iter = adjacency[i].begin();
                         iter != adjacency[i].end(); iter++)
                    {
                        Data::Vector3D normal = (*iter)->normal();

                        normal.normalize();

                        UInt j = 0;

                        for (j = 0; j < (*iter)->numPoints(); j++)
                        {
                            if ((*iter)->getPoint(j) == this->getPoint(i))
                            {
                                break;
                            }
                        }

                        if (j == (*iter)->numPoints())
                        {
                            n[i].sum(normal);

                            continue;
                        }

                        UInt next = (j + 1)%(*iter)->numPoints();
                        UInt prev = (j + (*iter)->numPoints() - 1)%(*iter)->numPoints();

                        Data::Vector3D v1(this->getPoint(i), (*iter)->getPoint(next));
                        Data::Vector3D v2(this->getPoint(i), (*iter)->getPoint(prev));

                        Real ang = v1.angle(v2);

                        normal.multiply(ang);

                        n[i].sum(normal);
                    }
                }

                n[0].normalize();
                n[1].normalize();
                n[2].normalize();
                n[3].normalize();

                normal[0] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getX()) : static_cast<GLfloat>(n[0].getX());
                normal[1] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getY()) : static_cast<GLfloat>(n[0].getY());
                normal[2] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[0].getZ()) : static_cast<GLfloat>(n[0].getZ());

                normal[3] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getX()) : static_cast<GLfloat>(n[1].getX());
                normal[4] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getY()) : static_cast<GLfloat>(n[1].getY());
                normal[5] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[1].getZ()) : static_cast<GLfloat>(n[1].getZ());

                normal[6] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getX()) : static_cast<GLfloat>(n[2].getX());
                normal[7] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getY()) : static_cast<GLfloat>(n[2].getY());
                normal[8] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[2].getZ()) : static_cast<GLfloat>(n[2].getZ());

                normal[9]  = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getX()) : static_cast<GLfloat>(n[3].getX());
                normal[10] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getY()) : static_cast<GLfloat>(n[3].getY());
                normal[11] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[3].getZ()) : static_cast<GLfloat>(n[3].getZ());
            }
            else
            {
                Vector3D n = this->normal();

                normal[0] = normal[3] = normal[6] = normal[9] =  (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getX()) : static_cast<GLfloat>(n.getX());
                normal[1] = normal[4] = normal[7] = normal[10] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getY()) : static_cast<GLfloat>(n.getY());
                normal[2] = normal[5] = normal[8] = normal[11] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getZ()) : static_cast<GLfloat>(n.getZ());
            }
		}

		coord[0] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getX());
		coord[1] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getY());
		coord[2] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getZ());

		coord[3] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getX());
		coord[4] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getY());
		coord[5] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getZ());

		coord[6] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getX());
		coord[7] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getY());
		coord[8] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getZ());

		coord[9] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getX());
		coord[10] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getY());
		coord[11] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP4())->getZ());

#if USE_EXPERIMENTAL_3
		Point3D mid = this->mid3D();
#else
		Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

		this->explodeAndShrink(coord, mid);
		this->explodeAndShrink(coord + 3, mid);
		this->explodeAndShrink(coord + 6, mid);
		this->explodeAndShrink(coord + 9, mid);
	}

	return 4;
}

bool Data::Quadrilateral::isInVisibleSpace() const
{
	return (!this->isBoxInVisibleSpace()) ? false :
			((Data::getState().isPlaneInclusive()) ? this->isPointInVisibleSpace() : this->arePointsInVisibleSpace());
}
#endif //#if USE_GUI
