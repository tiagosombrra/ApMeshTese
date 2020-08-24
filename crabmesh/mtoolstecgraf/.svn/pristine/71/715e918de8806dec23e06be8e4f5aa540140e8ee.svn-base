#include "Data/Spline/BSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::BSplineFunction::BSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{
	this->deg = 0;

	this->setDegree(1, 0);
}

Data::Spline::BSplineFunction::~BSplineFunction()
{

}

UInt Data::Spline::BSplineFunction::degree() const
{
	return this->getDegree();
}

void Data::Spline::BSplineFunction::setDegree(UInt degree, UInt n)
{
	if (degree < 1)
	{
		degree = 1;
	}

	RealVector oldKnot = this->knot;

	this->deg = degree;

	UInt newSize = this->getKnotSize(n);

	this->knot.resize(newSize);

	if (oldKnot.size() == 0)
	{
		this->refill(0, 0, NULL, 0, newSize, true);
	}
	else
	{
		this->refill(0, static_cast<UInt>(oldKnot.size()), oldKnot.data(), 0, newSize, true);
	}
}

UInt Data::Spline::BSplineFunction::getDegree() const
{
	return this->deg;
}

void Data::Spline::BSplineFunction::setKnot(UInt i, Real knot)
{
	this->knot[i] = knot;

	for (UInt j = i + 1; j < this->getKnotSize(); j++)
	{
		if (this->knot[j] < knot)
		{
			this->knot[j] = knot;
		}
	}

	for (UInt j = 0; j < i; j++)
	{
		if (this->knot[j] > knot)
		{
			this->knot[j] = knot;
		}
	}
}

Real Data::Spline::BSplineFunction::getKnot(UInt i) const
{
	return this->knot[i];
}

UInt Data::Spline::BSplineFunction::getKnotSize(UInt n) const
{
	return this->deg + n + 1;
}

UInt Data::Spline::BSplineFunction::getKnotSize() const
{
	return static_cast<UInt>(this->knot.size());
}

void Data::Spline::BSplineFunction::changeNumPoints(UInt n)
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
		this->refill(0, oldSize, oldKnot.data(), 0, newSize, true);
	}
}

Real Data::Spline::BSplineFunction::blend(UInt i, UInt n, Real t) const
{
	return this->b(i, this->deg + 1, n, this->interpolate(n, t));
}

Real Data::Spline::BSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	return this->db(i, this->deg + 1, n, this->interpolate(n, t));
}

Real Data::Spline::BSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	return this->d2b(i, this->deg + 1, n, this->interpolate(n, t));
}

Real Data::Spline::BSplineFunction::interpolate(UInt n, Real t) const
{
	Real min = this->knot[this->deg];
	Real max = this->knot[n];

	return min + t*(max - min);
}

void Data::Spline::BSplineFunction::refill(UInt /*oldMin*/, UInt oldMax, Real */*old*/, UInt newMin, UInt newMax, bool remake)
{
	if (remake)
	{
		UInt size = newMax - newMin;

		Real incr = 1.0/static_cast<Real>(size - 1);

		this->knot[newMin] = 0.0;
		this->knot[newMin + size - 1] = 1.0;

		for (UInt i = 1; i < size - 1; i++)
		{
			this->knot[newMin + i] = i*incr;
		}
	}
	else
	{
		/*for (UInt i = newMin; i < newMax; i++)
		{
			UInt oldi = oldMin + i;

			if (oldi >= oldMax)
			{
				oldi = oldMax - 1;
			}

			this->knot[i] = old[oldi];
		}*/

		Real diff = (oldMax == 1) ? 1.0 : this->knot[oldMax - 1] - this->knot[oldMax - 2];

		for (UInt i = 0; i < newMax - oldMax; i++)
		{
			this->knot[oldMax + i] = this->knot[oldMax - 1] + static_cast<Real>(i + 1)*diff;
		}
	}
}

Real Data::Spline::BSplineFunction::b(UInt i, UInt d, UInt n, Real t) const
{
	if (d == 1)
	{
		if (t < this->knot[i])
		{
			return 0.0;
		}

		if (t > this->knot[i + 1])
		{
			return 0.0;
		}

		if (/*(i == this->numPoints(segment)) &&*/
			(t >= this->knot[i + 1]) &&
			(this->knot[i + 1] >= this->knot[this->getKnotSize(n) - 1]))
		{
			return 1.0;
		}

		if (t >= this->knot[i + 1])
		{
			return 0.0;
		}

		return 1.0;
	}

	Real n1 = t - this->knot[i];
	Real d1 = this->knot[i + d - 1] - this->knot[i];

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = this->knot[i + d] - t;
	Real d2 = this->knot[i + d] - this->knot[i + 1];

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	return this->b(i, d - 1, n, t)*n1/d1 +
		   this->b(i + 1, d - 1, n, t)*n2/d2;
}

Real Data::Spline::BSplineFunction::db(UInt i, UInt d, UInt n, Real t) const
{
	if (d == 1)
	{
		return 0.0;
	}

	Real n1 = 1.0;
	Real d1 = this->knot[i + d - 1] - this->knot[i];

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = -1.0;
	Real d2 = this->knot[i + d] - this->knot[i + 1];

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	return static_cast<Real>(d - 1)*(this->b(i, d - 1, n, t)*n1/d1 +
									 this->b(i + 1, d - 1, n, t)*n2/d2);
}

Real Data::Spline::BSplineFunction::d2b(UInt i, UInt d, UInt n, Real t) const
{
	if (d <= 2)
	{
		return 0.0;
	}

	Real n1 = 1.0;
	Real d1 = (this->knot[i + d - 1] - this->knot[i])*
			  (this->knot[i + d - 2] - this->knot[i]);

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d1 = 1.0;
		n1 = 0.0;
	}

	Real n2 = -((this->knot[i + d] - this->knot[i + 1]) + (this->knot[i + d - 1] - this->knot[i]));
	Real d2 = (this->knot[i + d - 1] - this->knot[i + 1])*
			  (this->knot[i + d - 1] - this->knot[i])*
			  (this->knot[i + d] - this->knot[i + 1]);

	if (std::fabs(d2) <= Data::getTolerance())
	{
		d2 = 1.0;
		n2 = 0.0;
	}

	Real n3 = 1.0;
	Real d3 = (this->knot[i + d] - this->knot[i + 1])*
			  (this->knot[i + d] - this->knot[i + 2]);

	if (std::fabs(d1) <= Data::getTolerance())
	{
		d3 = 1.0;
		n3 = 0.0;
	}

	return static_cast<Real>(d - 1)*static_cast<Real>(d - 2)*(this->b(i, d - 2, n, t)*n1/d1 +
															  this->b(i + 1, d - 2, n, t)*n2/d2 +
															  this->b(i + 2, d - 2, n, t)*n3/d3);
}

#endif //#if USE_SPLINES
