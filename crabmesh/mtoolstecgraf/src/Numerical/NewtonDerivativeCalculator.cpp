#include "Numerical/NewtonDerivativeCalculator.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::NewtonDerivativeCalculator::DELTA = 0.0001;

//Real Numerical::NewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative)
Real Numerical::NewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative)
{
	return this->execute(function, x, derivative, DELTA);
}

//Real Numerical::ForwardNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real delta)
Real Numerical::ForwardNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real delta)
{
	if (derivative == 1)
	{
		Real f = function->f(x);
		Real fd = function->f(x + delta);

		//return fd - f;
		return (fd - f)/delta;
	}

	if (derivative == 2)
	{
		Real f   = function->f(x);
		Real fd  = function->f(x + delta);
		Real fd2 = function->f(x + 2.0*delta);

		//return fd2 - 2.0*fd + f;
		return (fd2 - 2.0*fd + f)/(delta*delta);
	}

	Real sum = 0.0;
	Real sign = 1.0;
	Real coef = 1.0;

	for (UInt i = 0; i <= derivative; i++)
	{
		Real d = static_cast<Real>(derivative - i) * delta;
		Real f = function->f(x + d);

		sum += sign*coef*f;

		sign *= -1.0;
		coef *= static_cast<Real>(derivative - i)/static_cast<Real>(i + 1);
	}

	//return sum;
	return sum/pow(delta, derivative);
}

//Real Numerical::BackwardNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real delta)
Real Numerical::BackwardNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real delta)
{
	if (derivative == 1)
	{
		Real f = function->f(x);
		Real bd = function->f(x - delta);

		//return f - bd;
		return (f - bd)/delta;
	}

	if (derivative == 2)
	{
		Real f   = function->f(x);
		Real bd  = function->f(x - delta);
		Real bd2 = function->f(x - 2.0*delta);

		//return f - 2.0*bd + bd2;
		return (f - 2.0*bd + bd2)/(delta*delta);
	}

	Real sum = 0.0;
	Real sign = 1.0;
	Real coef = 1.0;

	for (UInt i = 0; i <= derivative; i++)
	{
		Real d = static_cast<Real>(i) * delta;
		Real f = function->f(x - d);

		sum += sign*coef*f;

		sign *= -1.0;
		coef *= static_cast<Real>(derivative - i)/static_cast<Real>(i + 1);
	}

	//return sum;
	return sum/pow(delta, derivative);
}

//Real Numerical::CentralNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real delta)
Real Numerical::CentralNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real delta)
{
	if (derivative == 1)
	{
		Real bd = function->f(x - delta*0.5);
		Real fd = function->f(x + delta*0.5);

		//return fd - bd;
		return (fd - bd)/delta;
	}

	if (derivative == 2)
	{
		Real bd = function->f(x - delta);
		Real f  = function->f(x);
		Real fd = function->f(x + delta);

		//return fd - 2.0*f + bd;
		return (fd - 2.0*f + bd)/(delta*delta);
	}

	UInt s = ((derivative%2) == 0) ? 0 : 1;
	Real sum = 0.0;
	Real sign = 1.0;
	Real coef = 1.0;

	for (UInt i = 0; i <= derivative; i++)
	{
		Real d = static_cast<Real>((derivative/2 + s - i)) * delta * 0.5;
		Real f = function->f(x + d);

		sum += sign*coef*f;

		sign *= -1.0;
		coef *= static_cast<Real>(derivative - i)/static_cast<Real>(i + 1);
	}

	//return sum;
	return sum/pow(delta, derivative);
}

//Real Numerical::CombinedNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real delta)
Real Numerical::CombinedNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real delta)
{
	return this->execute(function, x, derivative, delta, 0.0, 1.0);
}

//Real Numerical::CombinedNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real min, Real max)
Real Numerical::CombinedNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real min, Real max)
{
	return this->execute(function, x, derivative, DELTA, min, max);
}

//Real Numerical::CombinedNewtonDerivativeCalculator::execute(DerivativeFunction *function, Real x, UInt derivative, Real delta, Real min, Real max)
Real Numerical::CombinedNewtonDerivativeCalculator::execute(Function *function, Real x, UInt derivative, Real delta, Real min, Real max)
{
	Real d = static_cast<Real>(derivative)*delta*0.5;

	if (x - d < min)
	{
		ForwardNewtonDerivativeCalculator f;

		return f.execute(function, x, derivative, delta);
	}

	if (x + d > max)
	{
		BackwardNewtonDerivativeCalculator b;

		return b.execute(function, x, derivative, delta);
	}

	CentralNewtonDerivativeCalculator c;

	return c.execute(function, x, derivative, delta);
}

#endif //#if USE_NUMERICAL
