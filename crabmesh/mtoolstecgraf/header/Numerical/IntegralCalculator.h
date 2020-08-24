#ifndef _NUMERICAL_INTEGRAL_CALCULATOR_H_
#define _NUMERICAL_INTEGRAL_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
	class IntegralCalculator : public Performer::Builder
	{
	public:

	    IntegralCalculator();
	    virtual ~IntegralCalculator();

	    virtual Real getIntegral() const;

		//virtual Real execute(OrderedPoints *points) = 0;
		//virtual Real execute(IntegralFunction *function) = 0;

    protected:

        Real integral;

	};

	class FunctionBasedIntegralCalculator : public IntegralCalculator
	{
    public:

        FunctionBasedIntegralCalculator();
        virtual ~FunctionBasedIntegralCalculator();

        virtual void setFunction(Function *function);
        virtual Function *getFunction() const;

        virtual void setFunction(OrderedPoints *points);

        virtual void setMin(Real min);
        virtual Real getMin() const;

        virtual void setMax(Real max);
        virtual Real getMax() const;

        //using Numerical::IntegralCalculator::execute;
        // creates an interpolation
        //virtual Real execute(OrderedPoints *points);

    protected:

        Function *function;
        Real min;
        Real max;

        bool fromPoints;

	};

	class PointBasedIntegralCalculator : public IntegralCalculator
	{
    //public:
        ///by default, DISTANCE = 0.1, NUM_POINTS = 10

	    //static const Real DISTANCE;
	    //static const UInt NUM_SUBDIVISIONS;

    public:

        PointBasedIntegralCalculator();
        virtual ~PointBasedIntegralCalculator();

        virtual void setPoints(OrderedPoints *points);
        virtual OrderedPoints *getPoints() const;

        virtual void setPoints(Function *function, Real min, Real max, Real distance);
        virtual void setPoints(Function *function, Real min, Real max, UInt numSubdivisions);

        //using Numerical::IntegralCalculator::execute;

		// creates equally-spaced points
		//virtual Real execute(IntegralFunction *function); //calls with NUM_SUBDIVISIONS
		//virtual Real execute(IntegralFunction *function, Real distance);
		//virtual Real execute(IntegralFunction *function, UInt numSubdivisions);

    //protected:

        //virtual OrderedPoints *findPoints(IntegralFunction *function, Real distance) const;
        //virtual OrderedPoints *findPoints(IntegralFunction *function, UInt numSubdivisions) const;

    protected:

        OrderedPoints *points;

        bool fromFunction;
	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_INTEGRAL_CALCULATOR_H_
