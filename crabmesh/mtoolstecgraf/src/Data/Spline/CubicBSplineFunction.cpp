#include "Data/Spline/CubicBSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::CubicBSplineFunction::CubicBSplineFunction() :
	Data::Spline::BSplineFunction::BSplineFunction()
{
	this->setDegree(3, 0);
}

Data::Spline::CubicBSplineFunction::~CubicBSplineFunction()
{

}

void Data::Spline::CubicBSplineFunction::setDegree(UInt /*degree*/, UInt n)
{
	Data::Spline::BSplineFunction::setDegree(3, n);
}

void Data::Spline::CubicBSplineFunction::changeNumPoints(UInt n)
{
	RealVector oldKnot = this->knot;

	UInt oldSize = static_cast<UInt>(oldKnot.size());

	UInt newSize = this->getKnotSize(n);

	this->knot.resize(newSize);

	if (oldSize == 0)
	{
		this->refill(0, 0, NULL, 0, newSize, true);
	}
	else
	{
		this->refill(0, oldSize, oldKnot.data(), 0, newSize, false);
	}
}

Real Data::Spline::CubicBSplineFunction::blend(UInt i, UInt n, Real t) const
{
	return this->blend(0, i, n, t);
}

Real Data::Spline::CubicBSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	return this->dBlend(0, i, n, t);
}

Real Data::Spline::CubicBSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	return this->d2Blend(0, i, n, t);
}

Real Data::Spline::CubicBSplineFunction::blend(UInt min, UInt i, UInt n, Real t) const
{
	return this->b(min, i, this->deg + 1, n, this->interpolate(min, n, t));
}

Real Data::Spline::CubicBSplineFunction::dBlend(UInt min, UInt i, UInt n, Real t) const
{
	return this->db(min, i, this->deg + 1, n, this->interpolate(min, n, t));
}

Real Data::Spline::CubicBSplineFunction::d2Blend(UInt min, UInt i, UInt n, Real t) const
{
	return this->d2b(min, i, this->deg + 1, n, this->interpolate(min, n, t));
}

Real Data::Spline::CubicBSplineFunction::interpolate(UInt n, Real t) const
{
	return this->interpolate(0, n, t);
}

Real Data::Spline::CubicBSplineFunction::interpolate(UInt min, UInt n, Real t) const
{
	Real tmin = this->knot[min + this->deg];
	Real tmax = this->knot[min + n];

	return tmin + t*(tmax - tmin);
}

Real Data::Spline::CubicBSplineFunction::b(UInt i, UInt d, UInt n, Real t) const
{
	return this->b(0, i, d, n, t);
}

Real Data::Spline::CubicBSplineFunction::db(UInt i, UInt d, UInt n, Real t) const
{
	return this->db(0, i, d, n, t);
}

Real Data::Spline::CubicBSplineFunction::d2b(UInt i, UInt d, UInt n, Real t) const
{
	return this->d2b(0, i, d, n, t);
}

Real Data::Spline::CubicBSplineFunction::b(UInt min, UInt i, UInt d, UInt n, Real t) const
{
	if (d == 1)
	{
		if (t < this->knot[min + i])
		{
			return 0.0;
		}

		if (t > this->knot[min + i + 1])
		{
			return 0.0;
		}

		if (/*(min + i == this->numPoints(segment)) &&*/
			(t >= this->knot[min + i + 1]) &&
			(this->knot[min + i + 1] >= this->knot[min + this->getKnotSize(n) - 1]))
		{
			return 1.0;
		}

		if (t >= this->knot[min + i + 1])
		{
			return 0.0;
		}

		return 1.0;
	}

	Real n1 = t - this->knot[min + i];
	Real d1 = this->knot[min + i + d - 1] - this->knot[min + i];

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = this->knot[min + i + d] - t;
	Real d2 = this->knot[min + i + d] - this->knot[min + i + 1];

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	return this->b(min, i, d - 1, n, t)*n1/d1 +
		   this->b(min, i + 1, d - 1, n, t)*n2/d2;
}

Real Data::Spline::CubicBSplineFunction::db(UInt min, UInt i, UInt d, UInt n, Real t) const
{
	if (d == 1)
	{
		return 0.0;
	}

	Real n1 = 1.0;
	Real d1 = this->knot[min + i + d - 1] - this->knot[min + i];

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = -1.0;
	Real d2 = this->knot[min + i + d] - this->knot[min + i + 1];

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	return static_cast<Real>(d - 1)*(this->b(min, i, d - 1, n, t)*n1/d1 +
									 this->b(min, i + 1, d - 1, n, t)*n2/d2);
}

Real Data::Spline::CubicBSplineFunction::d2b(UInt min, UInt i, UInt d, UInt n, Real t) const
{
	if (d <= 2)
	{
		return 0.0;
	}

	Real n1 = 1.0;
	Real d1 = (this->knot[min + i + d - 1] - this->knot[min + i])*
			  (this->knot[min + i + d - 2] - this->knot[min + i]);

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = -((this->knot[min + i + d] - this->knot[min + i + 1]) + (this->knot[min + i + d - 1] - this->knot[min + i]));
	Real d2 = (this->knot[min + i + d - 1] - this->knot[min + i + 1])*
			  (this->knot[min + i + d - 1] - this->knot[min + i])*
			  (this->knot[min + i + d] - this->knot[min + i + 1]);

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	Real n3 = 1.0;
	Real d3 = (this->knot[min + i + d] - this->knot[min + i + 1])*
			  (this->knot[min + i + d] - this->knot[min + i + 2]);

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d3 = 1.0;
		n3 = 0.0;
	}

	return static_cast<Real>(d - 1)*static_cast<Real>(d - 2)*(this->b(min, i, d - 2, n, t)*n1/d1 +
															  this->b(min, i + 1, d - 2, n, t)*n2/d2 +
															  this->b(min, i + 2, d - 2, n, t)*n3/d3);
}

#endif //#if USE_SPLINES
