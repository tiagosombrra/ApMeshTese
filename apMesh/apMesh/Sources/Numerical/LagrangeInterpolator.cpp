#include "Numerical/LagrangeInterpolator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"

using namespace Data;
using namespace Numerical;

Numerical::LagrangeInterpolator::LagrangeInterpolator() :
    Numerical::Interpolator::Interpolator()
{

}

Numerical::LagrangeInterpolator::~LagrangeInterpolator()
{

}

//Real Numerical::LagrangeInterpolator::polynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const
Real Numerical::LagrangeInterpolator::polynomial(UInt pol) const
{
    //return this->polynomial(points, degree, pol, 0, x);
    return this->dPolynomial(pol, 0);
}

/*Real Numerical::LagrangeInterpolator::polynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const
{
    return this->dPolynomial(points, degree, pol, 0, start, x);
}*/

//Real Numerical::LagrangeInterpolator::dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const
Real Numerical::LagrangeInterpolator::dPolynomial(UInt pol) const
{
    //return this->dPolynomial(points, degree, pol, 0, x);
    return this->dPolynomial(pol, 1);
}

/*Real Numerical::LagrangeInterpolator::dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const
{
    return this->dPolynomial(points, degree, pol, 1, start, x);
}*/

//Real Numerical::LagrangeInterpolator::d2Polynomial(const OrderedPoints *points, UInt degree, UInt pol, Real x) const
Real Numerical::LagrangeInterpolator::d2Polynomial(UInt pol) const
{
    //return this->d2Polynomial(points, degree, pol, 0, x);
    return this->dPolynomial(pol, 2);
}

/*Real Numerical::LagrangeInterpolator::d2Polynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt start, Real x) const
{
    return this->dPolynomial(points, degree, pol, 2, start, x);
}*/

//Real Numerical::LagrangeInterpolator::dNumerator(const OrderedPoints *points, UInt degree, UInt derivative, UInt size, UInt skip[], UInt start, Real x) const
Real Numerical::LagrangeInterpolator::dNumerator(UInt derivative, UInt size, UInt skip[]) const
{
    Real val = (derivative == 0) ? 1.0 : 0.0;

    for (UInt i = 0; i < this->degree + 1; i++)
    {
        bool count = true;

        for (UInt j = 0; j < size; j++)
        {
            if (i == skip[j])
            {
                count = false;

                break;
            }
        }

        if (!count)
        {
            continue;
        }

        if (derivative == 0)
        {
            val *= (this->x - this->points->x(this->start + i));
        }
        else
        {
            skip[size] = i;

            val += this->dNumerator(derivative - 1, size + 1, skip);
        }
    }

    return val;
}

/*Real Numerical::LagrangeInterpolator::execute(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    Real val = 0.0;

    for (UInt i = 0; i < degree + 1; i++)
    {
        val += points->y(start + i)*this->polynomial(points, degree, i, start, x);
    }

    return val;
}

Real Numerical::LagrangeInterpolator::executeDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    Real val = 0.0;

    for (UInt i = 0; i < degree + 1; i++)
    {
        val += points->y(start + i)*this->dPolynomial(points, degree, i, start, x);
    }

    return val;
}

Real Numerical::LagrangeInterpolator::execute2ndDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    Real val = 0.0;

    for (UInt i = 0; i < degree + 1; i++)
    {
        val += points->y(start + i)*this->d2Polynomial(points, degree, i, start, x);
    }

    return val;
}*/

//Real Numerical::LagrangeInterpolator::executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real x) const
Real Numerical::LagrangeInterpolator::dExecute(const UInt derivative)
{
    Real val = 0.0;

    for (UInt i = 0; i < this->degree + 1; i++)
    {
        val += points->y(this->start + i)*this->dPolynomial(i, derivative);
    }

    return val;
}

//Real Numerical::LagrangeInterpolator::dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt derivative, UInt start, Real x) const
Real Numerical::LagrangeInterpolator::dPolynomial(UInt pol, UInt derivative) const
{
    if (derivative > this->degree)
    {
        return 0.0;
    }

    Real den = 1.0;

    for (UInt i = 0; i < this->degree + 1; i++)
    {
        if (i == pol)
        {
            continue;
        }

        den *= (this->points->x(this->start + pol) - this->points->x(this->start + i));
    }

    UInt skip[this->degree + 1];

    skip[0] = pol;

    Real num = this->dNumerator(derivative, 1, skip);

    return num/den;
}

#endif //#if USE_NUMERICAL
