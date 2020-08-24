#include "Numerical/Function.h"

#if USE_NUMERICAL

#include "Numerical/DerivativePolynomial.h"

using namespace Data;
using namespace Numerical;

Numerical::Function::~Function()
{

}

Real Numerical::Function::f(const Real x[1])
{
	return this->f(x[0]);
}

UInt Numerical::IPolynomial::realDegree() const
{
    UInt deg = this->degree();

    for (UInt i = 0; i <= deg; i++)
    {
        if (this->getCoefficient(deg - i) != 0.0)
        {
            return deg - i;
        }
    }

    return 0;
}

UInt Numerical::IPolynomial::numCoefficients() const
{
    return this->degree() + 1;
}

Real Numerical::IPolynomial::f(Real x)
{
    UInt deg = this->degree();

    Real sum = this->getCoefficient(deg);

    for (UInt i = 0; i < deg; i++)
    {
        sum *= x;
        sum += this->getCoefficient(deg - i - 1);
    }

    return sum;
}

void Numerical::IPolynomial::descartesNumRoots(Data::UIntVector &positives, Data::UIntVector &negatives) const
{
    UInt numVariationsPositive = 0, numVariationsNegative = 0;
    Short prevPositive = 0, prevNegative = 0;

    UInt deg = this->degree();

    for (UInt i = 0; i <= deg; i++)
    {
        Real coeffPositive = this->getCoefficient(deg - i);
        Real coeffNegative = ((deg - i)%2 == 1) ? -coeffPositive : coeffPositive;

        if (coeffPositive > 0.0)
        {
            if (prevPositive == -1)
            {
                numVariationsPositive++;
            }

            prevPositive = 1;
        }
        else if (coeffPositive < 0.0)
        {
            if (prevPositive == 1)
            {
                numVariationsPositive++;
            }

            prevPositive = -1;
        }

        if (coeffNegative > 0.0)
        {
            if (prevNegative == -1)
            {
                numVariationsNegative++;
            }

            prevNegative = 1;
        }
        else if (coeffNegative < 0.0)
        {
            if (prevNegative == 1)
            {
                numVariationsNegative++;
            }

            prevNegative = -1;
        }
    }

    if (numVariationsPositive == 0)
    {
        positives.push_back(0);
    }
    else
    {
        for (UInt i = 0; i <= numVariationsPositive; i += 2)
        {
            positives.push_back(numVariationsPositive - i);
        }
    }

    if (numVariationsNegative == 0)
    {
        negatives.push_back(0);
    }
    else
    {
        for (UInt i = 0; i <= numVariationsNegative; i += 2)
        {
            negatives.push_back(numVariationsNegative - i);
        }
    }
}

void Numerical::IPolynomial::strumSequences(Real x0, Real x1, Data::RealVector &s0, Data::RealVector &s1) const
{
    DerivativePolynomial der(const_cast<IPolynomial *>(this));

    IPolynomial *first = const_cast<IPolynomial *>(this);
    IPolynomial *second = new Polynomial(der.degree());

    for (UInt i = 0; i <= der.degree(); i++)
    {
        static_cast<Numerical::Polynomial *>(second)->setCoefficient(i, der.getCoefficient(i));
    }

    s0.push_back(first->f(x0));
    s1.push_back(first->f(x1));
    s0.push_back(second->f(x0));
    s1.push_back(second->f(x1));

    while (second->realDegree() > 0)
    {
        IPolynomial *quotient = NULL, *remainder = NULL;

        first->division(second, quotient, remainder);

        delete quotient;

        s0.push_back(-remainder->f(x0));
        s1.push_back(-remainder->f(x1));

        if (first != this)
        {
            delete first;
        }

        first = second;
        second = remainder;
    }

    delete second;
}

UInt Numerical::IPolynomial::strumNumberRoots(Real x0, Real x1) const
{
    if ((const_cast<IPolynomial *>(this)->f(x0) == 0.0) || (const_cast<IPolynomial *>(this)->f(x1) == 0.0))
    {
        return UINT_MAX;
    }

    Data::RealVector v0, v1;

    this->strumSequences(x0, x1, v0, v1);

    UInt numVars0 = 0, numVars1 = 0;
    UInt prev0 = 0, prev1 = 0;

    for (UInt i = 1; i < v0.size(); i++)
    {
        if (v0[i]*v0[prev0] < 0.0)
        {
            numVars0++;
        }

        if (v0[i] != 0.0)
        {
            prev0 = i;
        }
    }

    for (UInt i = 1; i < v1.size(); i++)
    {
        if (v1[i]*v1[prev1] < 0.0)
        {
            numVars1++;
        }

        if (v1[i] != 0.0)
        {
            prev1 = i;
        }
    }

    return numVars0 > numVars1 ? numVars0 - numVars1 : 0;
}

Real Numerical::IPolynomial::radiusRootRho() const
{
    UInt deg = this->realDegree();

    if (deg == 0)
    {
        return 0.0;
    }

    Real a0 = std::fabs(this->getCoefficient(0));

    if (a0 == 0.0)
    {
        return 0.0;
    }

    Real a1 = std::fabs(this->getCoefficient(1));

    if (deg == 1)
    {
        return a0/a1;
    }

    Real an = std::fabs(this->getCoefficient(deg));

    Real rho2 = std::pow(a0/an, 1.0/static_cast<double>(deg));

    if (a1 == 0.0)
    {
        return rho2;
    }

    Real rho1 = deg*a0/a1;

    return std::min(rho1, rho2);
}

Real Numerical::IPolynomial::radiusRootMax() const
{
    UInt deg = this->realDegree();

    if (deg == 0)
    {
        return 0.0;
    }

    Real an = std::fabs(this->getCoefficient(deg));

    Real max = std::fabs(this->getCoefficient(0))/an;

    for (UInt i = 1; i < deg; i++)
    {
        Real val = std::fabs(this->getCoefficient(i))/an;

        if (val > max)
        {
            max = val;
        }
    }

    return max + 1.0;
}

Numerical::IPolynomial *Numerical::IPolynomial::negative() const
{
    UInt deg = this->degree();

    Polynomial *negative = new Polynomial(deg);

    for (UInt i = 0; i <= deg; i++)
    {
        negative->setCoefficient(i, (i%2 == 0) ? this->getCoefficient(i) : -this->getCoefficient(i));
    }

    return negative;
}

void Numerical::IPolynomial::division(IPolynomial *divisor, IPolynomial *&quotient, IPolynomial *&remainder)
{
    quotient = NULL;
    remainder = NULL;

    UInt tdegree = this->realDegree();
    UInt ddegree = divisor->realDegree();

    if (tdegree < ddegree)
    {
        return;
    }

    UInt qdegree = tdegree - ddegree;
    UInt rdegree = tdegree;

    Polynomial *q = new Polynomial(qdegree);
    Polynomial *r = new Polynomial(tdegree);

    quotient = q;
    remainder = r;

    for (UInt i = 0; i <= rdegree; i++)
    {
        r->setCoefficient(i, this->getCoefficient(i));
    }

    Real dcoeff = divisor->getCoefficient(ddegree);

    for (UInt i = 0; i <= tdegree - ddegree; i++)
    {
        Real coeff = r->getCoefficient(rdegree)/dcoeff;

        q->setCoefficient(qdegree, coeff);
        r->setCoefficient(rdegree, 0.0);

        for (UInt i = 0; i < ddegree; i++)
        {
            Real rcoeff = r->getCoefficient(rdegree-i-1);

            r->setCoefficient(rdegree-i-1, rcoeff - coeff*divisor->getCoefficient(ddegree-i-1));
        }

        rdegree--;
        qdegree--;
    }
}

Numerical::Polynomial::Polynomial(UInt deg) :
    Numerical::IPolynomial::IPolynomial()
{
    this->deg = deg;

    this->coefficients = new Real[this->deg+1];

    for (Data::UInt i = 0; i <= this->deg; i++)
    {
        this->coefficients[i] = 0.0;
    }
}

Numerical::Polynomial::~Polynomial()
{
    delete [] this->coefficients;
}

Data::UInt Numerical::Polynomial::degree() const
{
    return this->deg;
}

Data::UInt Numerical::Polynomial::numCoefficients() const
{
    return this->deg + 1;
}

void Numerical::Polynomial::setCoefficient(Data::UInt i, Real coefficient)
{
    this->coefficients[i] = coefficient;
}

Data::Real Numerical::Polynomial::getCoefficient(Data::UInt i) const
{
    return (i > this->deg) ? 0.0 : this->coefficients[i];
}

Data::Real Numerical::Polynomial::f(Real x)
{
    Data::Real sum = this->coefficients[this->deg];

    for (Data::UInt i = 0; i < this->deg; i++)
    {
        sum *= x;
        sum += this->coefficients[this->deg - i - 1];
    }

    return sum;
}

#endif //#if USE_NUMERICAL
