#ifndef _NUMERICAL_STEFFENSEN_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_STEFFENSEN_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/FixedPointEquationRootFinder.h"

namespace Numerical
{
	class SteffensenEquationRootFinder : public Numerical::FixedPointEquationRootFinder
	{
	public:

	    SteffensenEquationRootFinder();
	    virtual ~SteffensenEquationRootFinder();

		//virtual Real next(FixedPointEquationRootFunction *function, Real x, Real fx);
		virtual Real next(Real x, Real fx) const;

    protected:

        using FixedPointEquationRootFinder::setStep;
        using FixedPointEquationRootFinder::getStep;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_STEFFENSEN_EQUATION_ROOT_FINDER_H_
