#ifndef _NUMERICAL_DERIVATIVE_CALCULATOR_H_
#define _NUMERICAL_DERIVATIVE_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
	class DerivativeCalculator
	{
	public:

		//virtual Real execute(DerivativeFunction *function, Real x, UInt derivative = 1) = 0;
		virtual Real execute(Function *function, Real x, UInt derivative = 1) = 0;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_DERIVATIVE_CALCULATOR_H_
