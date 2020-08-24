#ifndef _NUMERICAL_NEWTON_DERIVATIVE_CALCULATOR_H_
#define _NUMERICAL_NEWTON_DERIVATIVE_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/DerivativeCalculator.h"

namespace Numerical
{
	class NewtonDerivativeCalculator : public DerivativeCalculator
	{
	public:

		///by default, delta = 0.0001
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta) = 0;
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative = 1);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta) = 0;
		virtual Real execute(Function *function, Real x, UInt derivative = 1);

	protected:

		static const Real DELTA;
	};

	class ForwardNewtonDerivativeCalculator : public NewtonDerivativeCalculator
	{
	public:

		using NewtonDerivativeCalculator::execute;
		///by default, delta = 0.0001
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta);

	};

	class BackwardNewtonDerivativeCalculator : public NewtonDerivativeCalculator
	{
	public:

		using NewtonDerivativeCalculator::execute;
		///by default, delta = 0.0001
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta);

	};

	class CentralNewtonDerivativeCalculator : public NewtonDerivativeCalculator
	{
	public:

		using NewtonDerivativeCalculator::execute;
		///by default, delta = 0.0001
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta);

	};

	class CombinedNewtonDerivativeCalculator : public NewtonDerivativeCalculator
	{
	public:

		using NewtonDerivativeCalculator::execute;
		///by default, delta = 0.0001
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta);
		///by default, min = 0.0 and max = 1.0
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real min, Real max);
		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative, Real delta, Real min, Real max);
		virtual Real execute(Function *function, Real x, UInt derivative, Real min, Real max);
		virtual Real execute(Function *function, Real x, UInt derivative, Real delta, Real min, Real max);

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_NEWTON_DERIVATIVE_CALCULATOR_H_
