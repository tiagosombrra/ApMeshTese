#include "Data/Spline/LagrangeSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::LagrangeSplineFunction::LagrangeSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{
	this->deg = 0;
}

Data::Spline::LagrangeSplineFunction::~LagrangeSplineFunction()
{

}

void Data::Spline::LagrangeSplineFunction::setDeg(UInt deg) const
{
	this->deg = deg;
}

UInt Data::Spline::LagrangeSplineFunction::degree() const
{
	return this->deg;
}

Real Data::Spline::LagrangeSplineFunction::blend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 1.0;
	}

	Real d = 1.0/static_cast<Real>(n);
	Real diffs[n+1], num = 1.0, den = 1.0;

	Real id = static_cast<Real>(i)*d;

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real jd = static_cast<Real>(j)*d;

		diffs[j] = t - jd;

		den *= (id - jd);
	}

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			continue;
		}

		num *= diffs[j];
	}

	return num/den;
}

Real Data::Spline::LagrangeSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 0.0;
	}

	Real d = 1.0/static_cast<Real>(n);
	Real diffs[n+1], num = 0.0, den = 1.0;

	Real id = static_cast<Real>(i)*d;

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real jd = static_cast<Real>(j)*d;

		diffs[j] = t - jd;

		den *= (id - jd);
	}

	for (UInt k = 0; k <= n; k++)
	{
		if (k == i)
		{
			continue;
		}

		Real mult = 1.0;

		for (UInt j = 0; j <= n; j++)
		{
			if ((j == i) || (j == k))
			{
				continue;
			}

			mult *= diffs[j];
		}

		num += mult;
	}

	return num/den;
}

Real Data::Spline::LagrangeSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 0.0;
	}

	Real d = 1.0/static_cast<Real>(n);
	Real diffs[n+1], num = 0.0, den = 1.0;

	Real id = static_cast<Real>(i)*d;

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real jd = static_cast<Real>(j)*d;

		diffs[j] = t - jd;

		den *= (id - jd);
	}

	for (UInt l = 0; l <= n; l++)
	{
		if (l == i)
		{
			continue;
		}

		Real sum = 0.0;

		for (UInt k = 0; k <= n; k++)
		{
			if ((k == i) || (k == l))
			{
				continue;
			}

			Real mult = 1.0;

			for (UInt j = 0; j <= n; j++)
			{
				if ((j == i) || (j == k) || (j == l))
				{
					continue;
				}

				mult *= diffs[j];
			}

			sum += mult;
		}

		num += sum;
	}

	return num/den;
}

#endif //#if USE_SPLINES
