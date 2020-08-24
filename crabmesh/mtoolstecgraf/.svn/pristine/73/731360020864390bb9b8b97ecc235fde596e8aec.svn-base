#include "Numerical/NewtonInterpolator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/TriangularMatrix.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

Numerical::NewtonInterpolator::NewtonInterpolator() :
    Numerical::Interpolator::Interpolator()
{

}

Numerical::NewtonInterpolator::~NewtonInterpolator()
{

}

void Numerical::NewtonInterpolator::buildDividedDifferencesMatrix(IMatrix &dividedDifferences) const
{
    for (UInt i = 0; i < this->degree; i++)
    {
        for (UInt j = 0; j < this->degree - i; j++)
        {
            Real first = (i == 0) ? this->points->y(this->start + j) : dividedDifferences.getElement(i + j - 1, i - 1);
            Real second = (i == 0) ? this->points->y(this->start + j + 1) : dividedDifferences.getElement(i + j, i - 1);

            dividedDifferences.setElement(i + j, i, (second - first)/(this->points->x(this->start + j + i + 1) - this->points->x(this->start + j)));
        }
    }

    //std::cout << dividedDifferences.text() << std::endl;
}

void Numerical::NewtonInterpolator::buildDividedDifferencesVector(Real *dividedDifferences) const
{
    for (UInt i = 0; i < this->degree; i++)
    {
        for (UInt j = this->degree - i; j > 0; j--)
        {
            Real first = (i == 0) ? this->points->y(this->start + j - 1) : dividedDifferences[i + j - 2];
            Real second = (i == 0) ? this->points->y(this->start + j) : dividedDifferences[i + j - 1];

            dividedDifferences[i + j - 1] = (second - first)/(this->points->x(this->start + j + i) - this->points->x(this->start + j - 1));
        }
    }
}

//Real Numerical::NewtonInterpolator::dNumerator(const OrderedPoints *points, UInt pol, UInt derivative, UInt size, UInt skip[], UInt start, Real x) const
Real Numerical::NewtonInterpolator::dNumerator(UInt pol, UInt derivative, UInt size, UInt skip[]) const
{
    Real val = (derivative == 0) ? 1.0 : 0.0;

    for (UInt i = 0; i < pol; i++)
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

            //val += this->dNumerator(points, pol, derivative - 1, size + 1, skip, start, x);
            val += this->dNumerator(pol, derivative - 1, size + 1, skip);
        }
    }

    return val;
}

/*Real Numerical::NewtonInterpolator::execute(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    return this->executeDerivative(points, degree, 0, start, x);
}

Real Numerical::NewtonInterpolator::executeDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    return this->executeDerivative(points, degree, 1, start, x);
}

Real Numerical::NewtonInterpolator::execute2ndDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const
{
    return this->executeDerivative(points, degree, 2, start, x);
}*/

//Real Numerical::NewtonInterpolator::executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real x) const
Real Numerical::NewtonInterpolator::dExecute(UInt derivative)
{
    Real dividedDifferences[this->degree];

    this->buildDividedDifferencesVector(dividedDifferences);

    return this->dExecute(derivative, dividedDifferences);
}

//Real Numerical::NewtonInterpolator::executeDerivative(const OrderedPoints *points, UInt degree, UInt derivative, UInt start, Real *dividedDifferences, Real x) const
Real Numerical::NewtonInterpolator::dExecute(UInt derivative, Real *dividedDifferences)
{
    Real val = 0.0;

    for (UInt i = 0; i < this->degree + 1; i++)
    {
        //Real dd = (i == 0) ? points->y(start) : dividedDifferences.getElement(i - 1, i - 1);
        Real dd = (i == 0) ? this->points->y(start) : dividedDifferences[i - 1];

        val += dd*this->dPolynomial(i, derivative);
    }

    return val;
}

//Real Numerical::NewtonInterpolator::dPolynomial(const OrderedPoints *points, UInt degree, UInt pol, UInt derivative, UInt start, Real x) const
Real Numerical::NewtonInterpolator::dPolynomial(UInt pol, UInt derivative) const
{
    if (derivative > this->degree)
    {
        return 0.0;
    }

    UInt skip[degree];

    return this->dNumerator(pol, derivative, 0, skip);
}

#endif //#if USE_NUMERICAL
