#ifndef _NUMERICAL_NEWTON_COTES_INTEGRAL_CALCULATOR_H_
#define _NUMERICAL_NEWTON_COTES_INTEGRAL_CALCULATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IntegralCalculator.h"

namespace Numerical
{
    class NewtonCotesIntegralCalculator : public Numerical::PointBasedIntegralCalculator
    {
    public:

        NewtonCotesIntegralCalculator();
        virtual ~NewtonCotesIntegralCalculator();

        //using Numerical::PointBasedIntegralCalculator::execute;
        //virtual Real execute(IntegralFunction *function, Real distance, UInt numPoints);
		//virtual Real execute(IntegralFunction *function, UInt numSubdivisions, UInt numPoints);
		//virtual Real execute(OrderedPoints *points, UInt numPoints) = 0;

		virtual void setNumPoints(UInt numPoints);
		virtual UInt getNumPoints() const;

    protected:

        UInt numPoints;
    };

	class ClosedNewtonCotesIntegralCalculator : public Numerical::NewtonCotesIntegralCalculator
	{
	public:

	    ClosedNewtonCotesIntegralCalculator();
	    virtual ~ClosedNewtonCotesIntegralCalculator();

		//using Numerical::NewtonCotesIntegralCalculator::execute;
		///points should be between 2 and 5
		///  3 is used by default
		//virtual Real execute(OrderedPoints *points, UInt numPoints);
		//virtual Real execute(OrderedPoints *points);

        //virtual Real executeTrapezoidalRule(OrderedPoints *points) const;
        //virtual Real executeSimpson13Rule(OrderedPoints *points) const;
        //virtual Real executeSimpson38Rule(OrderedPoints *points) const;
        //virtual Real executeBooleRule(OrderedPoints *points) const;

        //virtual Real executeFrom(OrderedPoints *points, UInt numPoints, UInt start) const;

        virtual void execute();

        virtual void executeTrapezoidalRule();
        virtual void executeSimpson13Rule();
        virtual void executeSimpson38Rule();
        virtual void executeBooleRule();

        virtual Real executeFrom(UInt numPoints, UInt start) const;

    protected:

		static const Real weights[4][6];

	};

	class OpenNewtonCotesIntegralCalculator : public Numerical::NewtonCotesIntegralCalculator
	{
	public:

	    OpenNewtonCotesIntegralCalculator();
	    virtual ~OpenNewtonCotesIntegralCalculator();

		//using Numerical::NewtonCotesIntegralCalculator::execute;
		///points should be between 1 and 4
		///  2 is used by default
		//virtual Real execute(OrderedPoints *points, UInt numPoints);
		//virtual Real execute(OrderedPoints *points);

        //virtual Real executeMidPointRule(OrderedPoints *points) const;
        //virtual Real execute2PointRule(OrderedPoints *points) const;
        //virtual Real execute3PointRule(OrderedPoints *points) const;
        //virtual Real execute4PointRule(OrderedPoints *points) const;

        //virtual Real executeFrom(OrderedPoints *points, UInt numPoints, UInt start) const;

        virtual void execute();

        virtual void executeMidPointRule();
        virtual void execute2PointRule();
        virtual void execute3PointRule();
        virtual void execute4PointRule();

        virtual Real executeFrom(UInt numPoints, UInt start) const;

    protected:

		static const Real weights[4][5];

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_NEWTON_COTES_INTEGRAL_CALCULATOR_H_
