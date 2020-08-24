#ifndef _NUMERICAL_NEWTON_INTERPOLATOR_H_
#define _NUMERICAL_NEWTON_INTERPOLATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/Interpolator.h"

namespace Numerical
{
    class NewtonInterpolator : public Numerical::Interpolator
    {
    friend
        class Numerical::InterpolationFunction;

    public:

        NewtonInterpolator();
        virtual ~NewtonInterpolator();

        //using Numerical::Interpolator::execute;
        //virtual Real execute(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        //using Numerical::Interpolator::executeDerivative;
        //virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        //using Numerical::Interpolator::execute2ndDerivative;
        //virtual Real execute2ndDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        //virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real x) const;
        //virtual Real dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt derivative, UInt start, Real x) const;
        virtual Real dPolynomial(UInt pol, UInt derivative) const;

        virtual void buildDividedDifferencesMatrix(IMatrix &dividedDifferences) const;
        virtual void buildDividedDifferencesVector(Real *dividedDifferences) const;

    protected:

        //virtual Real dNumerator(const OrderedPoints *points, UInt pol, UInt derivative, UInt size, UInt skip[], UInt start, Real x) const;
        virtual Real dNumerator(UInt pol, UInt derivative, UInt size, UInt skip[]) const;

        //virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real *dividedDifferences, Real x) const;

        virtual Real dExecute(UInt derivative);
        virtual Real dExecute(UInt derivative, Real *dividedDifferences);

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_NEWTON_INTERPOLATOR_H_
