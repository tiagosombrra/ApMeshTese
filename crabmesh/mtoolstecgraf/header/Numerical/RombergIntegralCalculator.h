#ifndef _NUMERICAL_ROMBERG_INTEGRAL_CALCULATOR_H_
#define _NUMERICAL_ROMBERG_INTEGRAL_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IntegralCalculator.h"

namespace Numerical
{
	class RombergIntegralCalculator : public Numerical::FunctionBasedIntegralCalculator
	{
    public:

        ///by default, MAX_STEPS = 5, TOLERANCE = 0.000001

	    static const UInt MAX_STEPS;
	    static const Real TOLERANCE;

	public:

	    RombergIntegralCalculator();
	    virtual ~RombergIntegralCalculator();

	    virtual void setCalculator(IntegralCalculator *calculator);
	    virtual IntegralCalculator *getCalculator() const;

	    virtual void setMaxSteps(UInt maxSteps);
	    virtual UInt getMaxSteps() const;

	    virtual void setTolerance(Real tolerance);
	    virtual Real getTolerance() const;

	    virtual void setTestingTolerance(bool testingTolerance);
	    virtual bool isTestingTolerance() const;

		/*using Numerical::FunctionBasedIntegralCalculator::execute;
        virtual Real execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps, Real tolerance, bool testTolerance);
		virtual Real execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps, Real tolerance);
		virtual Real execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, Real tolerance);
		virtual Real execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps);
		virtual Real execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints);
		virtual Real execute(IntegralFunction *function, UInt maxSteps, Real tolerance);
		virtual Real execute(IntegralFunction *function, Real tolerance);
		virtual Real execute(IntegralFunction *function, UInt maxSteps);
		virtual Real execute(IntegralFunction *function);*/

		virtual void execute();

    //protected:

        //virtual Real executeIntegral(IntegralFunction *function, NewtonCotesIntegralCalculator *nc, GaussLegendreIntegralCalculator *gl, UInt numPoints, UInt numSubdivisions);
        virtual Real sumIntegrals(UInt numSubdivisions);

    protected:

        IntegralCalculator *calculator;
        bool createdCalculator;

        UInt maxSteps;
        Real tolerance;

        bool testingTolerance;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ROMBERG_INTEGRAL_CALCULATOR_H_
