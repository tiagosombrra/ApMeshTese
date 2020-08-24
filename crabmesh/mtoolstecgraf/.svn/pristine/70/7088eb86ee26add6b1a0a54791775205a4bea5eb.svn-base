#include "Data/Spline/LagrangeChebyshevSplineFunction.h"

#include "Numerical/ChebyshevPoints.h"

#if USE_SPLINES
#if USE_NUMERICAL

using namespace Data;
using namespace Data::Spline;

Data::Spline::LagrangeChebyshevSplineFunction::LagrangeChebyshevSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{
	this->deg = 0;
}

Data::Spline::LagrangeChebyshevSplineFunction::~LagrangeChebyshevSplineFunction()
{

}

void Data::Spline::LagrangeChebyshevSplineFunction::setDeg(UInt deg) const
{
	this->deg = deg;
}

UInt Data::Spline::LagrangeChebyshevSplineFunction::degree() const
{
	return this->deg;
}

Real Data::Spline::LagrangeChebyshevSplineFunction::blend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 1.0;
	}

	Real diffs[n+1], num = 1.0, den = 1.0;

	Real pti = Numerical::ChebyshevPoints::point(i, n, 0.0, 1.0);

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real ptj = Numerical::ChebyshevPoints::point(j, n, 0.0, 1.0);

		diffs[j] = t - ptj;

		den *= (pti - ptj);
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

Real Data::Spline::LagrangeChebyshevSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 0.0;
	}

	Real diffs[n+1], num = 0.0, den = 1.0;

	Real pti = Numerical::ChebyshevPoints::point(i, n, 0.0, 1.0);

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real ptj = Numerical::ChebyshevPoints::point(j, n, 0.0, 1.0);

		diffs[j] = t - ptj;

		den *= (pti - ptj);
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

Real Data::Spline::LagrangeChebyshevSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	n = n - 1;

	this->setDeg(n);

	if (n == 0)
	{
		return 0.0;
	}

	Real diffs[n+1], num = 0.0, den = 1.0;

	Real pti = Numerical::ChebyshevPoints::point(i, n, 0.0, 1.0);

	for (UInt j = 0; j <= n; j++)
	{
		if (j == i)
		{
			diffs[j] = 1.0;

			continue;
		}

		Real ptj = Numerical::ChebyshevPoints::point(j, n, 0.0, 1.0);

		diffs[j] = t - ptj;

		den *= (pti - ptj);
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

#endif //#if USE_NUMERICAL
#endif //#if USE_SPLINES
