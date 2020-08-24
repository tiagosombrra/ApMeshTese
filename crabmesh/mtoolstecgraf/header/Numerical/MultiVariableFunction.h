#ifndef _NUMERICAL_MULTI_VARIABLE_FUNCTION_H_
#define _NUMERICAL_MULTI_VARIABLE_FUNCTION_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
	template <UInt n>
	class MultiVariableFunction
	{
	public:

		virtual UInt numFunctions() const;
		virtual Real f(const Real x[n]) = 0;
	};
}

template <Data::UInt n>
Data::UInt Numerical::MultiVariableFunction<n>::numFunctions() const
{
    return n;
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_MULTI_VARIABLE_FUNCTION_H_
