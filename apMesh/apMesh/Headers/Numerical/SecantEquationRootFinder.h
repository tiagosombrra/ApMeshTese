#ifndef _NUMERICAL_SECANT_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_SECANT_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EquationRootFinder.h"

namespace Numerical
{
	class SecantEquationRootFinder : public Numerical::EquationRootFinder
	{
	/*public:

		///by default, max = 1000, tolerance = 0.0001, testFunction = true, testStepSize = true
		///  if both testFunction and testStepSize are false, then they are converted to true
		virtual Real execute(SecantEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual Real execute(SecantEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual Real execute(SecantEquationRootFunction *function, bool &ok, UInt &numSteps);
		virtual Real execute(SecantEquationRootFunction *function, UInt &numSteps);
		virtual Real execute(SecantEquationRootFunction *function, bool &ok);
		virtual Real execute(SecantEquationRootFunction *function);
		virtual Real execute(EquationRootFunction *function);

		virtual Real next(SecantEquationRootFunction *function, Real x0, Real x1, Real fx0, Real fx1);*/

    public:

        SecantEquationRootFinder();
        virtual ~SecantEquationRootFinder();

        virtual void setInitial(Real initial);
        virtual Real getInitial() const;

        virtual void setInitial2(Real initial2);
        virtual Real getInitial2() const;

        virtual void execute();

    protected:

        virtual Real next(Real x0, Real x1, Real fx0, Real fx1) const;

    protected:

        Real initial;
        Real initial2;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_SECANTF_EQUATION_ROOT_FINDER_H_
