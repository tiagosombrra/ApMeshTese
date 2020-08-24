#ifndef _NUMERICAL_FIXED_POINT_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_FIXED_POINT_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EquationRootFinder.h"

namespace Numerical
{
	class FixedPointEquationRootFinder : public Numerical::EquationRootFinder
	{
	/*public:

		///by default, max = 1000, tolerance = 0.0001, testFunction = true, testStepSize = true
		///  if both testFunction and testStepSize are false, then they are converted to true
		virtual Real execute(FixedPointEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual Real execute(FixedPointEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual Real execute(FixedPointEquationRootFunction *function, bool &ok, UInt &numSteps);
		virtual Real execute(FixedPointEquationRootFunction *function, UInt &numSteps);
		virtual Real execute(FixedPointEquationRootFunction *function, bool &ok);
		virtual Real execute(FixedPointEquationRootFunction *function);
		virtual Real execute(EquationRootFunction *function);

		virtual Real next(FixedPointEquationRootFunction *function, Real x, Real fx);*/

    public:

        FixedPointEquationRootFinder();
        virtual ~FixedPointEquationRootFinder();

        virtual void setInitial(Real initial);
        virtual Real getInitial() const;

        virtual void setStep(Function *step);
        virtual Function *getStep() const;

        virtual void execute();

    protected:

        virtual Real next(Real x, Real fx) const;

    protected:

        Real initial;
        Function *step;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_FIXED_POINT_EQUATION_ROOT_FINDER_H_
