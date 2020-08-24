#include "Data/Spline/SplineFunction.h"

#if USE_SPLINES

#include "Numerical/NewtonDerivativeCalculator.h"
#include "Numerical/Function.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::SplineFunction::SplineFunction()
{

}

Data::Spline::SplineFunction::~SplineFunction()
{

}

#if USE_NUMERICAL
Real Data::Spline::SplineFunction::dBlend(UInt i, UInt n, Real t) const
{
	//struct SplineDerivativeFunction : public Numerical::DerivativeFunction
	struct SplineDerivativeFunction : public Numerical::Function
	{
		UInt i;
		UInt n;
		const Data::Spline::SplineFunction *function;

		//using Numerical::DerivativeFunction::f;
		using Numerical::Function::f;
		Real f(Real x)
		{
			return this->function->blend(i, n, x);
		};
	};

	SplineDerivativeFunction f;

	f.i = i;
	f.n = n;
	f.function = this;

	Numerical::CombinedNewtonDerivativeCalculator derivative;

	return derivative.execute(&f, t, 1, 0.0, 1.0);

	/*Real delta = 0.0001;

	if (t - delta*0.5 < 0.0)
	{
		Real blend   = this->blend(i, n, t);
		Real forward = this->blend(i, n, t + delta);

		return forward - blend;
	}

	if (t + delta*0.5 > 1.0)
	{
		Real blend    = this->blend(i, n, t);
		Real backward = this->blend(i, n, t - delta);

		return blend - backward;
	}

    Real forward  = this->blend(i, n, t + delta*0.5);
    Real backward = this->blend(i, n, t - delta*0.5);

    return forward - backward;*/
}

Real Data::Spline::SplineFunction::d2Blend(UInt i, UInt n, Real t) const
{
	//struct SplineDerivativeFunction : public Numerical::DerivativeFunction
	struct SplineDerivativeFunction : public Numerical::Function
	{
		UInt i;
		UInt n;
		const Data::Spline::SplineFunction *function;

		//using Numerical::DerivativeFunction::f;
		using Numerical::Function::f;
		Real f(Real x)
		{
			return this->function->blend(i, n, x);
		};
	};

	SplineDerivativeFunction f;

	f.i = i;
	f.n = n;
	f.function = this;

	Numerical::CombinedNewtonDerivativeCalculator derivative;

	return derivative.execute(&f, t, 2, 0.0, 1.0);

	/*Real delta = 0.0001;

	if (t - delta < 0.0)
	{
		Real blend    = this->blend(i, n, t);
		Real forward  = this->blend(i, n, t + delta);
		Real forward2 = this->blend(i, n, t + 2.0*delta);

		return (forward2 - 2.0*forward + blend)/(delta*delta);
	}

	if (t + delta > 1.0)
	{
		Real blend     = this->blend(i, n, t);
		Real backward  = this->blend(i, n, t - delta);
		Real backward2 = this->blend(i, n, t - 2.0*delta);

		return (blend - 2.0*backward + backward2)/(delta*delta);
	}

    Real forward  = this->blend(i, n, t + delta);
    Real blend    = this->blend(i, n, t);
    Real backward = this->blend(i, n, t - delta);

    return (forward - 2.0*blend + backward)/(delta*delta);*/
}
#endif //#if USE_NUMERICAL

#endif //#if USE_SPLINES
