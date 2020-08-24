#ifndef _NUMERICAL_GAUSS_LEGENDRE_INTEGRAL_CALCULATOR_H_
#define _NUMERICAL_GAUSS_LEGENDRE_INTEGRAL_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IntegralCalculator.h"

namespace Numerical
{
	class GaussLegendreIntegralCalculator : public Numerical::FunctionBasedIntegralCalculator
	{
	public:

	    GaussLegendreIntegralCalculator();
	    virtual ~GaussLegendreIntegralCalculator();

		//using Numerical::FunctionBasedIntegralCalculator::execute;
		///points should be between 1 and 8
		///  4 is used by default
		//virtual Real execute(IntegralFunction *function, UInt points);
		//virtual Real execute(IntegralFunction *function);

		virtual void setNumPoints(UInt numPoints);
		virtual UInt getNumPoints() const;

		virtual void execute();

    protected:

        UInt numPoints;

	protected:

		static const Real alphas[8][8];
		static const Real weights[8][8];

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_GAUSS_LEGENDRE_INTEGRAL_CALCULATOR_H_
