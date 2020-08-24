#ifndef _NUMERICAL_LAGRANGE_INTERPOLATOR_H_
#define _NUMERICAL_LAGRANGE_INTERPOLATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/Interpolator.h"

namespace Numerical
{
    class LagrangeInterpolator : public Numerical::Interpolator
    {
    public:

        LagrangeInterpolator();
        virtual ~LagrangeInterpolator();

        /*using Numerical::Interpolator::execute;
        virtual Real execute(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        using Numerical::Interpolator::executeDerivative;
        virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        using Numerical::Interpolator::execute2ndDerivative;
        virtual Real execute2ndDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        virtual Real polynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const;
        virtual Real polynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const;
        virtual Real dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const;
        virtual Real dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const;
        virtual Real d2Polynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const;
        virtual Real d2Polynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const;

        //virtual Real executeDerivative(const OrderedPoints *points, UInt derivative, Real x) const;
        //virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, Real x) const;
        virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real x) const;
        virtual Real dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt derivative, UInt start, Real x) const;

    protected:

        virtual Real dNumerator(const OrderedPoints *points, UInt degree, UInt derivative, UInt size, UInt skip[], UInt start, Real x) const;*/

        virtual Real polynomial(UInt pol) const;
        virtual Real dPolynomial(UInt pol) const;
        virtual Real d2Polynomial(UInt pol) const;

        virtual Real dPolynomial(UInt pol, UInt derivative) const;

    protected:

        virtual Real dNumerator(UInt derivative, UInt size, UInt skip[]) const;

        virtual Real dExecute(UInt derivative);

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_LAGRANGE_INTERPOLATOR_H_
