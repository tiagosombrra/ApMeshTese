#include "Data/Spline/ISurface.h"

#if USE_SPLINES

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/State.h"
#include "Data/Spline/SplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::ISurface::ISurface() :
	Data::GeometricShape::GeometricShape()
{
#if USE_GUI
	Data::Colorable::setR(1.0);
	Data::Colorable::setG(1.0);
	Data::Colorable::setB(1.0);

	this->discretizationU = this->discretizationV = 4;
#endif //#if USE_GUI

	this->uFunction = NULL;
	this->vFunction = NULL;
}

Data::Spline::ISurface::~ISurface()
{
	if (this->uFunction)
	{
		delete this->uFunction;
	}

	if (this->vFunction)
	{
		delete this->vFunction;
	}
}

#if USE_EXPERIMENTAL_3
Point2D Data::Spline::ISurface::mid2D() const
{
    Vector2D v;

    for (UInt i = 0; i < this->numPoints(); i++)
    {
    	if (!this->getPoint(i))
		{
			continue;
		}

        v.sum(this->getPoint(i));
    }

    v.multiply(1.0/static_cast<Real>(this->numPoints()));

    return Point2D(v);
}

Point3D Data::Spline::ISurface::mid3D() const
{
    Vector3D v;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (!this->getPoint(i))
		{
			continue;
		}

		v.sum(this->getPoint(i));
	}

	v.multiply(1.0/static_cast<Real>(this->numPoints()));

	return Point3D(v);
}
#else
Point Data::Spline::ISurface::mid() const
{
	Vector v(this->dimension());

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (!this->getPoint(i))
		{
			continue;
		}

		v.sum(this->getPoint(i));
	}

	v.multiply(1.0/static_cast<Real>(this->numPoints()));

	return Point(v);
}
#endif //#if USE_EXPERIMENTAL_3

Box Data::Spline::ISurface::box() const
{
	UInt dimension = this->dimension();

#if USE_EXPERIMENTAL_3
	Point2D min2d, max2d;
	Point3D min3d, max3d;

	Point &min = (dimension == 2) ? static_cast<Point &>(min2d) : static_cast<Point &>(min3d);
	Point &max = (dimension == 2) ? static_cast<Point &>(max2d) : static_cast<Point &>(max3d);
#else
	Point min(dimension);
	Point max(dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < dimension; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (!this->getPoint(i))
		{
			continue;
		}

		for (UInt j = 0; j < dimension; j++)
		{
			Real coord = this->getPoint(i)->getCoord(j);

			if (coord < min.getCoord(j))
			{
				min.setCoord(j, coord);
			}

			if (coord > max.getCoord(j))
			{
				max.setCoord(j, coord);
			}
		}
	}

	return Box(min, max);
}

UInt Data::Spline::ISurface::numPoints() const
{
	return (this->numPointsV() == 0) ? 0 : this->numPointsU()*this->numPointsV();
}

UInt Data::Spline::ISurface::numPoints(UInt segmentCol, UInt segmentRow) const
{
	return this->numPointsU(segmentCol)*this->numPointsV(segmentRow);
}

UInt Data::Spline::ISurface::numPointsU(UInt /*segmentCol*/) const
{
	return this->numPointsU();
}

UInt Data::Spline::ISurface::numPointsV(UInt /*segmentRow*/) const
{
	return this->numPointsV();
}

void Data::Spline::ISurface::setPoint(UInt i, Point *p)
{
	this->setPoint(i/this->numPointsU(), i%this->numPointsU(), p);
}

Point *Data::Spline::ISurface::getPoint(UInt i) const
{
	return this->getPoint(i/this->numPointsU(), i%this->numPointsU());
}

Real Data::Spline::ISurface::orientedSize() const
{
	Real size = 0;

	for (UInt i = 0; i < this->numSegmentsCol(); i++)
	{
		for (UInt j = 0; j < this->numSegmentsRow(); j++)
		{
			if (!this->haveAllPoints(i, j))
			{
				continue;
			}

#if USE_GUI
			UInt discretizationU = std::max(static_cast<UInt>(1), this->discretizationU*(this->numPointsU(i) - 1));
			UInt discretizationV = std::max(static_cast<UInt>(1), this->discretizationV*(this->numPointsV(j) - 1));
#else
			UInt discretizationU = std::max(static_cast<UInt>(1), 4*(this->numPointsU(i) - 1));
			UInt discretizationV = std::max(static_cast<UInt>(1), 4*(this->numPointsV(j) - 1));
#endif //#if USE_GUI

			Point3DMatrix pts;

			pts.resize(discretizationV + 1);

			Real incrU = 1.0/static_cast<Real>(discretizationU);
			Real incrV = 1.0/static_cast<Real>(discretizationV);

			for (UInt iv = 0; iv < discretizationV + 1; iv++)
			{
				pts[iv].resize(discretizationU + 1);

				for (UInt iu = 0; iu < discretizationU + 1; iu++)
				{
					pts[iv][iu] = this->p(i, j, iu*incrU, iv*incrV);
				}
			}

			Real area = 0.0;

			for (UInt iv = 0; iv < discretizationV; iv++)
			{
				for (UInt iu = 0; iu < discretizationU; iu++)
				{
					area += pts[iv][iu].surface(pts[iv + 1][iu], pts[iv + 1][iu + 1]);
					area += pts[iv][iu].surface(pts[iv + 1][iu + 1], pts[iv][iu + 1]);
				}
			}

			size += area;
		}
	}

	return size;
}

bool Data::Spline::ISurface::in(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ISurface::in(const Point */*p*/) const
{
	return false;
}

bool Data::Spline::ISurface::on(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ISurface::on(const Point */*p*/) const
{
	return false;
}

bool Data::Spline::ISurface::out(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ISurface::out(const Point */*p*/) const
{
	return false;
}

UInt Data::Spline::ISurface::numSegmentsRow() const
{
	return 1;
}

UInt Data::Spline::ISurface::numSegmentsCol() const
{
	return 1;
}

Point *Data::Spline::ISurface::point(UInt segmentCol, UInt segmentRow, UInt i, UInt j) const
{
	if (this->getPoint(i, j))
	{
		return this->getPoint(i, j);
	}

	if (i == 0)
	{
		return NULL;
	}

	return this->point(segmentCol, segmentRow, i - 1, j);
}

bool Data::Spline::ISurface::haveAllPoints(UInt segmentCol, UInt segmentRow) const
{
	for (UInt i = 0; i < this->numPointsV(segmentRow); i++)
	{
		for (UInt j = 0; j < this->numPointsU(segmentCol); j++)
		{
			if (!this->point(segmentCol, segmentRow, i, j))
			{
				return false;
			}
		}
	}

	return true;
}

void Data::Spline::ISurface::move(UInt i, UInt j, Real x, Real y, Real z)
{
	this->getPoint(i, j)->setCoord(0, x);
	this->getPoint(i, j)->setCoord(1, y);

	if (this->getPoint(i, j)->dimension() > 2)
	{
		this->getPoint(i, j)->setCoord(2, z);
	}
}

Point3D Data::Spline::ISurface::p(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->blendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->blendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

#if USE_NUMERICAL
Point3D Data::Spline::ISurface::dup(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->blendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->dBlendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

Point3D Data::Spline::ISurface::dvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->dBlendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->blendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

Point3D Data::Spline::ISurface::d2uup(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->blendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->d2BlendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

Point3D Data::Spline::ISurface::d2uvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->dBlendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->dBlendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

Point3D Data::Spline::ISurface::d2vvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	UInt numPtsU = this->numPointsU(segmentCol);
	UInt numPtsV = this->numPointsV(segmentRow);

	Real blendsU[numPtsU], blendsV[numPtsV];

	for (UInt i = 0; i < numPtsV; i++)
	{
		blendsV[i] = this->d2BlendV(segmentRow, i, v);
	}

	for (UInt j = 0; j < numPtsU; j++)
	{
		blendsU[j] = this->blendU(segmentCol, j, u);
	}

	Vector3D sum;

	for (UInt i = 0; i < numPtsV; i++)
	{
		for (UInt j = 0; j < numPtsU; j++)
		{
			Vector3D pt(this->pt3d(this->point(segmentCol, segmentRow, i, j)));

			pt.multiply(blendsU[j]*blendsV[i]);

			sum.sum(pt);
		}
	}

	return Point3D(sum);
}

Vector3D Data::Spline::ISurface::normal(UInt segmentRow, UInt segmentCol, Real u, Real v) const
{
	if (!this->haveAllPoints(segmentCol, segmentRow))
	{
		return Point3D();
	}

	Vector3D du(this->dup(segmentCol, segmentRow, u, v));
	Vector3D dv(this->dvp(segmentCol, segmentRow, u, v));

	return du.cross(dv);
}
#endif //#if USE_NUMERICAL

Real Data::Spline::ISurface::blendU(UInt segmentCol, UInt j, Real u) const
{
	return (!this->uFunction) ? 0.0 : this->uFunction->blend(j, this->numPointsU(segmentCol), u);
}

Real Data::Spline::ISurface::blendV(UInt segmentRow, UInt i, Real v) const
{
	return (!this->vFunction) ? 0.0 : this->vFunction->blend(i, this->numPointsV(segmentRow), v);
}

#if USE_NUMERICAL
Real Data::Spline::ISurface::dBlendU(UInt segmentCol, UInt j, Real u) const
{
	return (!this->uFunction) ? 0.0 : this->uFunction->dBlend(j, this->numPointsU(segmentCol), u);
}

Real Data::Spline::ISurface::dBlendV(UInt segmentRow, UInt i, Real v) const
{
	return (!this->vFunction) ? 0.0 : this->vFunction->dBlend(i, this->numPointsV(segmentRow), v);
}

Real Data::Spline::ISurface::d2BlendU(UInt segmentCol, UInt j, Real u) const
{
	return (!this->uFunction) ? 0.0 : this->uFunction->d2Blend(j, this->numPointsU(segmentCol), u);
}

Real Data::Spline::ISurface::d2BlendV(UInt segmentRow, UInt i, Real v) const
{
	return (!this->vFunction) ? 0.0 : this->vFunction->d2Blend(i, this->numPointsV(segmentRow), v);
}
#endif //#if USE_NUMERICAL

Point3D Data::Spline::ISurface::pt3d(const Point *p) const
{
	if (p->dimension() == 1)
	{
		return Point3D(p->getCoord(0));
	}

	if (p->dimension() == 2)
	{
		return Point3D(p->getCoord(0), p->getCoord(1));
	}

	if (p->dimension() == 3)
	{
		return Point3D(p);
	}

	return Point3D();
}

#if USE_GUI
void Data::Spline::ISurface::setR(Real r) const
{
	Data::Colorable::setR(r);

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->getPoint(i, j))
			{
				this->getPoint(i, j)->setR(r);
			}
		}
	}
}

void Data::Spline::ISurface::setG(Real g) const
{
	Data::Colorable::setG(g);

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->getPoint(i, j))
			{
				this->getPoint(i, j)->setG(g);
			}
		}
	}
}

void Data::Spline::ISurface::setB(Real b) const
{
	Data::Colorable::setB(b);

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->getPoint(i, j))
			{
				this->getPoint(i, j)->setB(b);
			}
		}
	}
}

void Data::Spline::ISurface::highlight() const
{
	Data::Colorable::highlight();

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->getPoint(i, j))
			{
				this->getPoint(i, j)->highlight();
			}
		}
	}
}

void Data::Spline::ISurface::unhighlight() const
{
	Data::Colorable::unhighlight();

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->getPoint(i, j))
			{
				this->getPoint(i, j)->unhighlight();
			}
		}
	}
}

void Data::Spline::ISurface::setDiscretization(UInt discretizationU, UInt discretizationV) const
{
	this->discretizationU = discretizationU;
	this->discretizationV = discretizationV;
}

UInt Data::Spline::ISurface::getDiscretizationU() const
{
	return this->discretizationU;
}

UInt Data::Spline::ISurface::getDiscretizationV() const
{
	return this->discretizationV;
}

void Data::Spline::ISurface::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	bool changed = false;
	int modes[2];

	glGetIntegerv(GL_POLYGON_MODE, modes);

	bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

	if ((this->isHighlighted()) && (wireframe))
	{
		changed = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

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

		for (UInt i = 0; i < this->numSegmentsCol(); i++)
		{
			for (UInt j = 0; j < this->numSegmentsRow(); j++)
			{
				if (!this->haveAllPoints(i, j))
				{
					continue;
				}

				UInt discretizationU = std::max(static_cast<UInt>(1), this->discretizationU*(this->numPointsU(i) - 1));
				UInt discretizationV = std::max(static_cast<UInt>(1), this->discretizationV*(this->numPointsV(j) - 1));

				Point3DMatrix pts;
#if USE_NUMERICAL
				Vector3DMatrix normals;
#endif //#if USE_NUMERICAL

				pts.resize(discretizationV + 1);
#if USE_NUMERICAL
				normals.resize(discretizationV + 1);
#endif //#if USE_NUMERICAL

				Real incrU = 1.0/static_cast<Real>(discretizationU);
				Real incrV = 1.0/static_cast<Real>(discretizationV);

				for (UInt iv = 0; iv < discretizationV + 1; iv++)
				{
					pts[iv].resize(discretizationU + 1);
#if USE_NUMERICAL
					normals[iv].resize(discretizationU + 1);
#endif //#if USE_NUMERICAL

					for (UInt iu = 0; iu < discretizationU + 1; iu++)
					{
						pts[iv][iu] = this->p(i, j, iu*incrU, iv*incrV);
#if USE_NUMERICAL
						normals[iv][iu] = this->normal(i, j, iu*incrU, iv*incrV);

						normals[iv][iu].normalize();
#endif //#if USE_NUMERICAL
					}
				}

				glBegin(GL_QUADS);

				for (UInt iv = 0; iv < discretizationV; iv++)
				{
					for (UInt iu = 0; iu < discretizationU; iu++)
					{
#if USE_NUMERICAL
						glNormal(normals[iv][iu].getX(), normals[iv][iu].getY(), normals[iv][iu].getZ());
#endif //#if USE_NUMERICAL
						glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());

#if USE_NUMERICAL
						glNormal(normals[iv+1][iu].getX(), normals[iv+1][iu].getY(), normals[iv+1][iu].getZ());
#endif //#if USE_NUMERICAL
						glVertex(pts[iv+1][iu].getX(), pts[iv+1][iu].getY(), pts[iv+1][iu].getZ());

#if USE_NUMERICAL
						glNormal(normals[iv+1][iu+1].getX(), normals[iv+1][iu+1].getY(), normals[iv+1][iu+1].getZ());
#endif //#if USE_NUMERICAL
						glVertex(pts[iv+1][iu+1].getX(), pts[iv+1][iu+1].getY(), pts[iv+1][iu+1].getZ());

#if USE_NUMERICAL
						glNormal(normals[iv][iu+1].getX(), normals[iv][iu+1].getY(), normals[iv][iu+1].getZ());
#endif //#if USE_NUMERICAL
						glVertex(pts[iv][iu+1].getX(), pts[iv][iu+1].getY(), pts[iv][iu+1].getZ());
					}
				}

				glEnd();

				if (!wireframe)
				{
					GLboolean lighting = glIsEnabled(GL_LIGHTING);

					if (lighting)
					{
						glDisable(GL_LIGHTING);
					}

					for (UInt iv = 0; iv < discretizationV + 1; iv++)
					{
						glBegin(GL_LINE_STRIP);

						for (UInt iu = 0; iu < discretizationU + 1; iu++)
						{
							glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());
						}

						glEnd();
					}

					for (UInt iu = 0; iu < discretizationU + 1; iu++)
					{
						glBegin(GL_LINE_STRIP);

						for (UInt iv = 0; iv < discretizationV + 1; iv++)
						{
							glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());
						}

						glEnd();
					}

					if (lighting)
					{
						glEnable(GL_LIGHTING);
					}
				}
			}
		}

	glPopMatrix();

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

void Data::Spline::ISurface::drawPoints() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glColor(this->getR(), this->getG(), this->getB());

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (!this->getPoint(i, j))
			{
				continue;
			}

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

			this->getPoint(i, j)->draw();
		}
	}
}

void Data::Spline::ISurface::drawPolygon() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	bool changed = false;
	int modes[2];

	glGetIntegerv(GL_POLYGON_MODE, modes);

	bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

	if ((this->isHighlighted()) && (wireframe))
	{
		changed = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

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

		for (UInt i = 0; i < this->numSegmentsCol(); i++)
		{
			for (UInt j = 0; j < this->numSegmentsRow(); j++)
			{
				if (!this->haveAllPoints(i, j))
				{
					continue;
				}

				UInt numPtsU = this->numPointsU(i);
				UInt numPtsV = this->numPointsV(j);

				Point3DMatrix pts;
				Vector3DMatrix normalsF;
				Vector3DMatrix normals;

				pts.resize(numPtsV);
				normalsF.resize(numPtsV-1);
				normals.resize(numPtsV);

				for (UInt iv = 0; iv < numPtsV; iv++)
				{
					pts[iv].resize(numPtsU);
					normals[iv].resize(numPtsU);

					if (iv < numPtsV - 1)
					{
						normalsF[iv].resize(numPtsU-1);
					}

					for (UInt iu = 0; iu < numPtsU; iu++)
					{
						pts[iv][iu] = this->pt3d(this->point(i, j, iv, iu));

						if ((iu > 0) && (iv > 0))
						{
							Vector3D du(pts[iv-1][iu-1], pts[iv-1][iu]);
							Vector3D dv(pts[iv-1][iu-1], pts[iv][iu-1]);

							normalsF[iv-1][iu-1] = du.cross(dv);

							normalsF[iv-1][iu-1].normalize();
						}
					}
				}

				if ((numPtsV > 1) && (numPtsU > 1))
				{
					for (UInt iv = 0; iv < numPtsV; iv++)
					{
						for (UInt iu = 0; iu < numPtsU; iu++)
						{
							if ((iv == 0) && (iu == 0))
							{
								normals[iv][iu] = normalsF[iv][iu];

								continue;
							}

							if ((iv == 0) && (iu == numPtsU - 1))
							{
								normals[iv][iu] = normalsF[iv][iu-1];

								continue;
							}

							if ((iv == numPtsV - 1) && (iu == 0))
							{
								normals[iv][iu] = normalsF[iv-1][iu];

								continue;
							}

							if ((iv == numPtsV - 1) && (iu == numPtsU - 1))
							{
								normals[iv][iu] = normalsF[iv-1][iu-1];

								continue;
							}

							if (iv == 0)
							{
								normals[iv][iu] = normalsF[iv][iu-1];
								normals[iv][iu].sum(normalsF[iv][iu]);

								normals[iv][iu].normalize();

								continue;
							}

							if (iv == numPtsV - 1)
							{
								normals[iv][iu] = normalsF[iv-1][iu-1];
								normals[iv][iu].sum(normalsF[iv-1][iu]);

								normals[iv][iu].normalize();

								continue;
							}

							if (iu == 0)
							{
								normals[iv][iu] = normalsF[iv-1][iu];
								normals[iv][iu].sum(normalsF[iv][iu]);

								normals[iv][iu].normalize();

								continue;
							}

							if (iu == numPtsU - 1)
							{
								normals[iv][iu] = normalsF[iv-1][iu-1];
								normals[iv][iu].sum(normalsF[iv][iu-1]);

								normals[iv][iu].normalize();

								continue;
							}

							normals[iv][iu] = normalsF[iv-1][iu-1];
							normals[iv][iu].sum(normalsF[iv][iu-1]);
							normals[iv][iu].sum(normalsF[iv-1][iu]);
							normals[iv][iu].sum(normalsF[iv][iu]);

							normals[iv][iu].normalize();
						}
					}

					glBegin(GL_QUADS);

					for (UInt iv = 0; iv < numPtsV - 1; iv++)
					{
						for (UInt iu = 0; iu < numPtsU - 1; iu++)
						{
							glNormal(normals[iv][iu].getX(), normals[iv][iu].getY(), normals[iv][iu].getZ());
							glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());

							glNormal(normals[iv+1][iu].getX(), normals[iv+1][iu].getY(), normals[iv+1][iu].getZ());
							glVertex(pts[iv+1][iu].getX(), pts[iv+1][iu].getY(), pts[iv+1][iu].getZ());

							glNormal(normals[iv+1][iu+1].getX(), normals[iv+1][iu+1].getY(), normals[iv+1][iu+1].getZ());
							glVertex(pts[iv+1][iu+1].getX(), pts[iv+1][iu+1].getY(), pts[iv+1][iu+1].getZ());

							glNormal(normals[iv][iu+1].getX(), normals[iv][iu+1].getY(), normals[iv][iu+1].getZ());
							glVertex(pts[iv][iu+1].getX(), pts[iv][iu+1].getY(), pts[iv][iu+1].getZ());
						}
					}

					glEnd();
				}

				if (!wireframe)
				{
					GLboolean lighting = glIsEnabled(GL_LIGHTING);

					if (lighting)
					{
						glDisable(GL_LIGHTING);
					}

					for (UInt iv = 0; iv < numPtsV; iv++)
					{
						glBegin(GL_LINE_STRIP);

						for (UInt iu = 0; iu < numPtsU; iu++)
						{
							glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());
						}

						glEnd();
					}

					for (UInt iu = 0; iu < numPtsU; iu++)
					{
						glBegin(GL_LINE_STRIP);

						for (UInt iv = 0; iv < numPtsV; iv++)
						{
							glVertex(pts[iv][iu].getX(), pts[iv][iu].getY(), pts[iv][iu].getZ());
						}

						glEnd();
					}

					if (lighting)
					{
						glEnable(GL_LIGHTING);
					}
				}
			}
		}

	glPopMatrix();

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

#if USE_NUMERICAL
void Data::Spline::ISurface::drawBases() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));
	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

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

		for (UInt i = 0; i < this->numSegmentsCol(); i++)
		{
			for (UInt j = 0; j < this->numSegmentsRow(); j++)
			{
				if (!this->haveAllPoints(i, j))
				{
					continue;
				}

				UInt discretizationU = std::max(static_cast<UInt>(1), this->discretizationU*(this->numPointsU(i) - 1));
				UInt discretizationV = std::max(static_cast<UInt>(1), this->discretizationV*(this->numPointsV(j) - 1));

				Real incrU = 1.0/static_cast<Real>(discretizationU);
				Real incrV = 1.0/static_cast<Real>(discretizationV);

				for (UInt iv = 0; iv < discretizationV; iv++)
				{
					for (UInt iu = 0; iu < discretizationU; iu++)
					{
						Point3D p = this->p(i, j, iu*incrU, iv*incrV);
						Vector3D dup(this->dup(i, j, iu*incrU, iv*incrV));
						dup.normalize();
						Vector3D dvp(this->dvp(i, j, iu*incrU, iv*incrV));
						dvp.normalize();
						Vector3D normal = dup.cross(dvp);

						glPushMatrix();
							glTranslate(p.getX(), p.getY(), p.getZ());

							glBegin(GL_POINTS);
								glVertex(0.0, 0.0, 0.0);
							glEnd();

							glBegin(GL_LINES);
								glVertex(0.0, 0.0, 0.0);
								glVertex(dup.getX(), dup.getY(), dup.getZ());

								glVertex(0.0, 0.0, 0.0);
								glVertex(dvp.getX(), dvp.getY(), dvp.getZ());

								glVertex(0.0, 0.0, 0.0);
								glVertex(normal.getX(), normal.getY(), normal.getZ());
							glEnd();
						glPopMatrix();
					}

					Point3D p = this->p(i, j, 1.0, iv*incrV);
					Vector3D dup(this->dup(i, j, 1.0, iv*incrV));
					dup.normalize();
					Vector3D dvp(this->dvp(i, j, 1.0, iv*incrV));
					dvp.normalize();
					Vector3D normal = dup.cross(dvp);

					glPushMatrix();
						glTranslate(p.getX(), p.getY(), p.getZ());

						glBegin(GL_POINTS);
							glVertex(0.0, 0.0, 0.0);
						glEnd();

						glBegin(GL_LINES);
							glVertex(0.0, 0.0, 0.0);
							glVertex(dup.getX(), dup.getY(), dup.getZ());

							glVertex(0.0, 0.0, 0.0);
							glVertex(dvp.getX(), dvp.getY(), dvp.getZ());

							glVertex(0.0, 0.0, 0.0);
							glVertex(normal.getX(), normal.getY(), normal.getZ());
						glEnd();
					glPopMatrix();
				}

				for (UInt iu = 0; iu < discretizationU; iu++)
				{
					Point3D p = this->p(i, j, iu*incrU, 1.0);
					Vector3D dup(this->dup(i, j, iu*incrU, 1.0));
					dup.normalize();
					Vector3D dvp(this->dvp(i, j, iu*incrU, 1.0));
					dvp.normalize();
					Vector3D normal = dup.cross(dvp);

					glPushMatrix();
						glTranslate(p.getX(), p.getY(), p.getZ());

						glBegin(GL_POINTS);
							glVertex(0.0, 0.0, 0.0);
						glEnd();

						glBegin(GL_LINES);
							glVertex(0.0, 0.0, 0.0);
							glVertex(dup.getX(), dup.getY(), dup.getZ());

							glVertex(0.0, 0.0, 0.0);
							glVertex(dvp.getX(), dvp.getY(), dvp.getZ());

							glVertex(0.0, 0.0, 0.0);
							glVertex(normal.getX(), normal.getY(), normal.getZ());
						glEnd();
					glPopMatrix();
				}

				Point3D p = this->p(i, j, 1.0, 1.0);
				Vector3D dup(this->dup(i, j, 1.0, 1.0));
				dup.normalize();
				Vector3D dvp(this->dvp(i, j, 1.0, 1.0));
				dvp.normalize();
				Vector3D normal = dup.cross(dvp);

				glPushMatrix();
					glTranslate(p.getX(), p.getY(), p.getZ());

					glBegin(GL_POINTS);
						glVertex(0.0, 0.0, 0.0);
					glEnd();

					glBegin(GL_LINES);
						glVertex(0.0, 0.0, 0.0);
						glVertex(dup.getX(), dup.getY(), dup.getZ());

						glVertex(0.0, 0.0, 0.0);
						glVertex(dvp.getX(), dvp.getY(), dvp.getZ());

						glVertex(0.0, 0.0, 0.0);
						glVertex(normal.getX(), normal.getY(), normal.getZ());
					glEnd();
				glPopMatrix();
			}
		}

	glPopMatrix();
}
#endif //#if USE_NUMERICAL

std::string Data::Spline::ISurface::text() const
{
	std::stringstream s;

	s << "columns: " << this->numSegmentsCol() << ", rows: " << this->numSegmentsRow()
	  << ", points in column: " << this->numPointsV() << ", points in row: " << this->numPointsU() << ": ";

	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (!this->getPoint(i, j))
			{
				continue;
			}

			s << "(i,j) " << this->getPoint(i, j)->text();

			if ((i < this->numPointsV() - 1) || (j < this->numPointsU() - 1))
			{
				s << ", ";
			}
		}
	}

	return s.str();
}
#endif //#if USE_GUI

/*SplineFunction *Data::Spline::ISurface::makeUSplineFunction() const
{
	return NULL;
}

SplineFunction *Data::Spline::ISurface::makeVSplineFunction() const
{
	return NULL;
}*/

#endif //#if USE_SPLINES
