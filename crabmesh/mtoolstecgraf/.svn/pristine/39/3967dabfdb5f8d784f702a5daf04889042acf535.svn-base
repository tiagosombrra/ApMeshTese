#ifndef _NUMERICAL_DERIVATIVE_FUNCTION_H_
#define _NUMERICAL_DERIVATIVE_FUNCTION_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

namespace Numerical
{
	class DerivativePolynomial :
	    public Numerical::IPolynomial
    {
    public:

        DerivativePolynomial(const IPolynomial *polynomial);
        virtual ~DerivativePolynomial();

        virtual void setPolynomial(const IPolynomial *polynomial);
        virtual const IPolynomial *getPolynomial() const;

        virtual UInt degree() const;
        virtual Real getCoefficient(UInt i) const;

        using Numerical::IPolynomial::f;

    protected:

        const IPolynomial *polynomial;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_DERIVATIVE_FUNCTION_H_
