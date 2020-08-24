#ifndef _NUMERICAL_INTERPOLATION_FUNCTION_H_
#define _NUMERICAL_INTERPOLATION_FUNCTION_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/Interpolator.h"
#include "Numerical/Function.h"

namespace Numerical
{
    class InterpolationFunction : public IPolynomial
    {
    public:

        InterpolationFunction(const OrderedPoints *points, Interpolator *interpolation = NULL);
        InterpolationFunction(const OrderedPoints *points, UInt degree, Interpolator *interpolation = NULL);
        virtual ~InterpolationFunction();

        virtual const OrderedPoints *getPoints();

        virtual UInt degree() const;

        virtual Real getCoefficient(UInt i) const;

        using Numerical::IPolynomial::f;
        virtual Real f(Real x);

    protected:

        const OrderedPoints *points;
        UInt degr;
        Interpolator *interpolation;

        NewtonInterpolator *interp;
        Real *dividedDiferences;
        mutable IColumnMatrix *coefs;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_INTERPOLATION_FUNCTION_H_
