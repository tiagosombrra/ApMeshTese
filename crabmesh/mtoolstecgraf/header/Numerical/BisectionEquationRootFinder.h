#ifndef _NUMERICAL_BISECTION_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_BISECTION_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EquationRootFinder.h"

namespace Numerical
{
	class BisectionEquationRootFinder : public Numerical::EquationRootFinder
	{
	/*public:

		///by default, max = 1000, tolerance = 0.0001, testFunction = true, testStepSize = true
		///  if both testFunction and testStepSize are false, then they are converted to true
		virtual Real execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual Real execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual Real execute(IntervalEquationRootFunction *function, bool &ok, UInt &numSteps);
		virtual Real execute(IntervalEquationRootFunction *function, UInt &numSteps);
		virtual Real execute(IntervalEquationRootFunction *function, bool &ok);
		virtual Real execute(IntervalEquationRootFunction *function);
		virtual Real execute(EquationRootFunction *function);

		virtual Real next(IntervalEquationRootFunction *function, Real min, Real max, Real fmin, Real fmax);*/

    public:

        BisectionEquationRootFinder();
        virtual ~BisectionEquationRootFinder();

        virtual void setMin(Real min);
        virtual Real getMin() const;

        virtual void setMax(Real max);
        virtual Real getMax() const;

        virtual void execute();

    protected:

        virtual Real next(Real a, Real b, Real fa, Real fb) const;
        virtual void narrowDownRange(Real &a, Real &b, Real &fa, Real &fb, Real &m, Real &fm) const;

    protected:

        Real min;
        Real max;

	};

	class ClosestBisectionEquationRootFinder : virtual public Numerical::BisectionEquationRootFinder
	{
	public:

	    ClosestBisectionEquationRootFinder();
	    virtual ~ClosestBisectionEquationRootFinder();

		//using Numerical::BisectionEquationRootFinder::execute;
		//virtual Real execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok);

		virtual void setIgnoringSigns(bool ignoringSigns);
		virtual bool isIgnoringSigns() const;

		virtual void execute();

	protected:

		/*virtual Real recursiveExecute(IntervalEquationRootFunction *function, Real min, Real max, Real fmin, Real fmax,
			UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval);*/

        virtual Real recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval);

    protected:

        bool ignoringSigns;

	};

	class MaxBisectionEquationRootFinder : virtual public Numerical::BisectionEquationRootFinder
	{
	public:

	    MaxBisectionEquationRootFinder();
	    virtual ~MaxBisectionEquationRootFinder();

		//using Numerical::BisectionEquationRootFinder::execute;
		//virtual Real execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok);
		virtual void execute();

	protected:

		/*virtual Real recursiveExecute(IntervalEquationRootFunction *function, Real min, Real max, Real fmin, Real fmax,
			UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval);*/

        virtual Real recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval);
	};

	class MinBisectionEquationRootFinder : virtual public Numerical::BisectionEquationRootFinder
	{
	public:

	    MinBisectionEquationRootFinder();
	    virtual ~MinBisectionEquationRootFinder();

		//using Numerical::BisectionEquationRootFinder::execute;
		//virtual Real execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok);

		virtual void execute();

	protected:

		/*virtual Real recursiveExecute(IntervalEquationRootFunction *function, Real min, Real max, Real fmin, Real fmax,
			UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval);*/

        virtual Real recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval);
	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_BISECTION_EQUATION_ROOT_FINDER_H_
