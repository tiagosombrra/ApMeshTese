#include "Data/Spline/BCurve.h"

#if USE_SPLINES

#include "Data/State.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Spline/BSplineFunction.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::BCurve::BCurve() :
	Data::Spline::Curve::Curve()
{

}

Data::Spline::BCurve::~BCurve()
{

}

void Data::Spline::BCurve::setDegree(UInt segment, UInt degree)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<BSplineFunction *>(this->function)->setDegree(degree, this->numPoints(segment));
}

UInt Data::Spline::BCurve::getDegree(UInt /*segment*/) const
{
	return this->getDegree();
}

void Data::Spline::BCurve::setDegree(UInt degree)
{
	this->setDegree(0, degree);
}

UInt Data::Spline::BCurve::getDegree() const
{
	return (!this->function) ? 0 : static_cast<BSplineFunction *>(this->function)->getDegree();
}

void Data::Spline::BCurve::setKnot(UInt /*segment*/, UInt i, Real knot)
{
	if (!this->function)
	{
		this->function = this->makeSplineFunction();
	}

	static_cast<BSplineFunction *>(this->function)->setKnot(i, knot);
}

Real Data::Spline::BCurve::getKnot(UInt /*segment*/, UInt i) const
{
	return (!this->function) ? 0.0 : static_cast<BSplineFunction *>(this->function)->getKnot(i);
}

UInt Data::Spline::BCurve::getKnotSize(UInt segment) const
{
	return (!this->function) ? 0 : static_cast<BSplineFunction *>(this->function)->getKnotSize(this->numPoints(segment));
}

void Data::Spline::BCurve::setKnot(UInt i, Real knot)
{
	for (UInt j = 0; j < this->numSegments(); j++)
	{
		UInt min = this->getMinKnot(j);
		UInt max = this->getMaxKnot(j);

		if (i >= max)
		{
			continue;
		}

		this->setKnot(j, i - min, knot);

		break;
	}
}

Real Data::Spline::BCurve::getKnot(UInt i) const
{
	return (!this->function) ? 0.0 : static_cast<BSplineFunction *>(this->function)->getKnot(i);
}

UInt Data::Spline::BCurve::getKnotSize() const
{
	return (!this->function) ? 0 : static_cast<BSplineFunction *>(this->function)->getKnotSize();
}

UInt Data::Spline::BCurve::getMinKnot(UInt segment) const
{
	return (segment == 0) ? 0 : this->getMaxKnot(segment - 1);
}

UInt Data::Spline::BCurve::getMaxKnot(UInt segment) const
{
	return this->getMinKnot(segment) + this->getKnotSize(segment);
}

void Data::Spline::BCurve::add(Point *p)
{
	Data::Spline::Curve::add(p);

	static_cast<BSplineFunction *>(this->function)->changeNumPoints(this->numPoints());
}

#if USE_GUI
void Data::Spline::BCurve::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
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

		for (UInt j = 0; j < this->numSegments(); j++)
		{
			if (this->numPoints(j) <= this->getDegree(j))
			{
				continue;
			}

			UInt discretization = this->discretization*(this->numPoints(j) - 1);

			Real incr = 1.0/static_cast<Real>(discretization);

			glBegin(GL_LINE_STRIP);

				for (UInt i = 0; i < discretization; i++)
				{
					Point3D p = this->p(j, i*incr);

					glVertex(p.getX(), p.getY(), p.getZ());
				}

				Point3D p = this->p(j, 1.0);

				glVertex(p.getX(), p.getY(), p.getZ());

			glEnd();
		}

	glPopMatrix();
}

std::string Data::Spline::BCurve::text() const
{
	std::stringstream s;

	s << "B, " << Data::Spline::Curve::text()
	  << ", degree: " << this->getDegree()
	  << ", knot:";

	for (UInt i = 0; i < this->getKnotSize(); i++)
	{
		s << " " << this->getKnot(i);
	}

	return s.str();
}
#endif //#if USE_GUI

SplineFunction *Data::Spline::BCurve::makeSplineFunction() const
{
	return new BSplineFunction();
}

#endif //#if USE_SPLINES
