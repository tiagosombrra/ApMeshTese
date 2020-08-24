#ifndef _NUMERICAL_FALSE_POSITION_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_FALSE_POSITION_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/BisectionEquationRootFinder.h"

namespace Numerical
{
	class FalsePositionEquationRootFinder : virtual public Numerical::BisectionEquationRootFinder
	{
	public:

	    FalsePositionEquationRootFinder();
	    virtual ~FalsePositionEquationRootFinder();

		virtual void setWeightDecreaseCount(UInt weightDecreaseCount);
		virtual UInt getWeightDecreaseCount() const;

		virtual void execute();

    protected:

        virtual Real next(Real a, Real b, Real fa, Real fb) const;
        virtual void narrowDownRange(Real &a, Real &b, Real &fa, Real &fb, Real &m, Real &fm) const;

    protected:

        UInt weightDecreaseCount;

        mutable UInt countA, countB;

	};

	class ClosestFalsePositionEquationRootFinder :
	    public Numerical::ClosestBisectionEquationRootFinder,
	    public Numerical::FalsePositionEquationRootFinder
	{
	public:

	    ClosestFalsePositionEquationRootFinder();
	    virtual ~ClosestFalsePositionEquationRootFinder();

	    virtual void execute();

		using Numerical::FalsePositionEquationRootFinder::next;
	};

	class MaxFalsePositionEquationRootFinder :
	    public Numerical::MaxBisectionEquationRootFinder,
	    public Numerical::FalsePositionEquationRootFinder
	{
	public:

	    MaxFalsePositionEquationRootFinder();
	    virtual ~MaxFalsePositionEquationRootFinder();

	    virtual void execute();

		using Numerical::FalsePositionEquationRootFinder::next;
	};

	class MinFalsePositionEquationRootFinder :
	    public Numerical::MinBisectionEquationRootFinder,
	    public Numerical::FalsePositionEquationRootFinder
	{
	public:

	    MinFalsePositionEquationRootFinder();
	    virtual ~MinFalsePositionEquationRootFinder();

	    virtual void execute();

		using Numerical::FalsePositionEquationRootFinder::next;
	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_FALSE_POSITION_EQUATION_ROOT_FINDER_H_
