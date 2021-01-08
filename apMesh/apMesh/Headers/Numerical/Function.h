#ifndef _NUMERICAL_FUNCTION_H_
#define _NUMERICAL_FUNCTION_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/MultiVariableFunction.h"

namespace Numerical
{
	class Function : public Numerical::MultiVariableFunction<1>
	{
	public:
	    virtual ~Function();

		virtual Real f(Real x) = 0;

		virtual Real f(const Real x[1]);
	};

	class IPolynomial : virtual public Numerical::Function
	{
    public:

	    virtual UInt realDegree() const;
	    virtual UInt degree() const = 0;
        virtual UInt numCoefficients() const;

        virtual Real getCoefficient(UInt i) const = 0;

        using Numerical::Function::f;
        virtual Real f(Real x);

        virtual void descartesNumRoots(Data::UIntVector &positives, Data::UIntVector &negatives) const;
        virtual void strumSequences(Real x0, Real x1, Data::RealVector &s0, Data::RealVector &s1) const;
        virtual UInt strumNumberRoots(Real x0, Real x1) const;
        virtual Real radiusRootRho() const;
        virtual Real radiusRootMax() const;

        virtual IPolynomial *negative() const;
        virtual void division(IPolynomial *divisor, IPolynomial *&quotient, IPolynomial *&remainder);
	};

	template <UInt deg>
	class TPolynomial : public Numerical::IPolynomial
	{
    public:

        TPolynomial<deg>();

        virtual UInt degree() const;
        virtual UInt numCoefficients() const;

        virtual void setCoefficient(UInt i, Real coefficient);
        virtual Real getCoefficient(UInt i) const;

        using Numerical::IPolynomial::f;
        virtual Real f(Real x);

    protected:

        Real coefficients[deg+1];
	};

	class Polynomial : public Numerical::IPolynomial
	{
    public:

        Polynomial(UInt degree);
        virtual ~Polynomial();

        virtual UInt degree() const;
        virtual UInt numCoefficients() const;

        virtual void setCoefficient(UInt i, Real coefficient);
        virtual Real getCoefficient(UInt i) const;

        using Numerical::IPolynomial::f;
        virtual Real f(Real x);

    protected:

        UInt deg;
        Real *coefficients;
	};
}

template <Data::UInt deg>
Numerical::TPolynomial<deg>::TPolynomial() :
    Numerical::IPolynomial::IPolynomial()
{
    for (Data::UInt i = 0; i <= deg; i++)
    {
        this->coefficients[i] = 0.0;
    }
}

template <Data::UInt deg>
Data::UInt Numerical::TPolynomial<deg>::degree() const
{
    return deg;
}

template <Data::UInt deg>
Data::UInt Numerical::TPolynomial<deg>::numCoefficients() const
{
    return deg + 1;
}

template <Data::UInt deg>
void Numerical::TPolynomial<deg>::setCoefficient(Data::UInt i, Real coefficient)
{
    this->coefficients[i] = coefficient;
}

template <Data::UInt deg>
Data::Real Numerical::TPolynomial<deg>::getCoefficient(Data::UInt i) const
{
    return (i > deg) ? 0.0 : this->coefficients[i];
}

template <Data::UInt deg>
Data::Real Numerical::TPolynomial<deg>::f(Real x)
{
    Data::Real sum = this->coefficients[deg];

    for (Data::UInt i = 0; i < deg; i++)
    {
        sum *= x;
        sum += this->coefficients[deg - i - 1];
    }

    return sum;
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_FUNCTION_H_
