#include "Data/Spline/BezierSplineFunction.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::BezierSplineFunction::BezierSplineFunction() :
	Data::Spline::SplineFunction::SplineFunction()
{

}

Data::Spline::BezierSplineFunction::~BezierSplineFunction()
{

}

UInt Data::Spline::BezierSplineFunction::degree() const
{
	return this->coefficients.empty() ? 0 : static_cast<UInt>(this->coefficients.size()) - 1;
}

Real Data::Spline::BezierSplineFunction::blend(UInt i, UInt n, Real t) const
{
	this->findCoefficients(n);

	n = n - 1;

	Real b1 = std::pow(      t, static_cast<Real>(i));
	Real b2 = std::pow(1.0 - t, static_cast<Real>(n - i));

	return this->coefficients[i]*b1*b2;
}

Real Data::Spline::BezierSplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	this->findCoefficients(n);

	n = n - 1;

	Real b1 = (i <= 0) ? 1.0 : std::pow(      t, static_cast<Real>(i));
	Real b2 = (i >= n) ? 1.0 : std::pow(1.0 - t, static_cast<Real>(n - i));

	Real db1 = (i <= 0) ? 0.0 : static_cast<Real>(i)    *std::pow(      t, static_cast<Real>(i - 1));
	Real db2 = (i >= n) ? 0.0 : static_cast<Real>(n - i)*std::pow(1.0 - t, static_cast<Real>(n - i - 1))*(-1.0);

	return coefficients[i]*(db1*b2 + b1*db2);
}

Real Data::Spline::BezierSplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	this->findCoefficients(n);

	n = n - 1;

	Real b1 = (i <= 0) ? 1.0 : std::pow(      t, static_cast<Real>(i));
	Real b2 = (i >= n) ? 1.0 : std::pow(1.0 - t, static_cast<Real>(n - i));

	Real db1 = (i <= 0) ? 0.0 : (i <= 1    ) ? 1.0 : static_cast<Real>(i)    *std::pow(      t, static_cast<Real>(i - 1));
	Real db2 = (i >= n) ? 0.0 : (i >= n - 1) ? 1.0 : static_cast<Real>(n - i)*std::pow(1.0 - t, static_cast<Real>(n - i - 1))*(-1.0);

	Real d2b1 = (i <= 1    ) ? 0.0 : static_cast<Real>(i)    *static_cast<Real>(i - 1)    *std::pow(      t, static_cast<Real>(i - 2));
	Real d2b2 = (i >= n - 1) ? 0.0 : static_cast<Real>(n - i)*static_cast<Real>(n - i - 1)*std::pow(1.0 - t, static_cast<Real>(n - i - 2));

	return this->coefficients[i]*(d2b1*b2 + 2.0*db1*db2 + b1*d2b2);
}

void Data::Spline::BezierSplineFunction::findCoefficients(UInt n) const
{
	if (static_cast<UInt>(this->coefficients.size()) == n)
	{
		return;
	}

	n = n - 1;

	this->coefficients.resize(n + 1);

	for (UInt k = 0; k <= n; k++)
	{
		this->coefficients[k] = 1.0;

		for (UInt i = n; i >= k + 1; i--)
		{
			this->coefficients[k] *= i;
		}

		for (UInt i = n - k; i >= 2; i--)
		{
			this->coefficients[k] /= i;
		}
	}
}

#endif //#if USE_SPLINES
