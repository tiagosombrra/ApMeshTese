#include "Numerical/Interpolator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"

using namespace Data;
using namespace Numerical;

Numerical::Interpolator::Interpolator() :
    Performer::Builder::Builder()
{
    this->points = NULL;
    this->degree = UINT_MAX;
    this->degreeSet = false;
    this->start = 0;
    this->x = 0.0;
    this->fx = 0.0;
}

Numerical::Interpolator::~Interpolator()
{

}

void Numerical::Interpolator::setPoints(const OrderedPoints *points)
{
    this->points = points;
}

const OrderedPoints *Numerical::Interpolator::getPoints() const
{
    return this->points;
}

void Numerical::Interpolator::setDegree(UInt degree)
{
    this->degree = degree;

    this->degreeSet = true;
}

UInt Numerical::Interpolator::getDegree() const
{
    return this->degree;
}

void Numerical::Interpolator::setX(Real x)
{
    this->x = x;
}

Real Numerical::Interpolator::getX() const
{
    return this->x;
}

Real Numerical::Interpolator::getFx() const
{
    return this->fx;
}

/*Real Numerical::Interpolator::execute(const OrderedPoints *points, Real x)
{
    return (points->getNumPoints() > 0) ? this->execute(points, points->getNumPoints() - 1, x) : 0.0;
}

Real Numerical::Interpolator::execute(const OrderedPoints *points, UInt degree, Real x) const
{
    UInt start = 0;

    bool ok = this->findStart(points, degree, x, start);

    return ok ? this->execute(points, degree, start, x) : 0.0;
}

Real Numerical::Interpolator::executeDerivative(const OrderedPoints *points, Real x)
{
    return (points->getNumPoints() > 0) ? this->executeDerivative(points, points->getNumPoints() - 1, x) : 0.0;
}

Real Numerical::Interpolator::executeDerivative(const OrderedPoints *points, UInt degree, Real x) const
{
    UInt start = 0;

    bool ok = this->findStart(points, degree, x, start);

    return ok ? this->executeDerivative(points, degree, start, x) : 0.0;
}

Real Numerical::Interpolator::executeDerivative(const OrderedPoints * *//*points*//*, UInt *//*degree*//*, UInt *//*start*//*, Real *//*x*//*) const
{
    return 0.0;
}

Real Numerical::Interpolator::execute2ndDerivative(const OrderedPoints *points, Real x)
{
    return (points->getNumPoints() > 0) ? this->execute2ndDerivative(points, points->getNumPoints() - 1, x) : 0.0;
}

Real Numerical::Interpolator::execute2ndDerivative(const OrderedPoints *points, UInt degree, Real x) const
{
    UInt start = 0;

    bool ok = this->findStart(points, degree, x, start);

    return ok ? this->execute2ndDerivative(points, degree, start, x) : 0.0;
}

Real Numerical::Interpolator::execute2ndDerivative(const OrderedPoints * *//*points*//*, UInt *//*degree*//*, UInt *//*start*//*, Real *//*x*//*) const
{
    return 0.0;
}*/

void Numerical::Interpolator::execute()
{
    this->executeNthDerivative(0);
}

void Numerical::Interpolator::executeDerivative()
{
    this->executeNthDerivative(1);
}

void Numerical::Interpolator::execute2ndDerivative()
{
    this->executeNthDerivative(2);
}

void Numerical::Interpolator::executeNthDerivative(UInt derivative)
{
    if (!this->points)
    {
        return;
    }

    if (!this->findStart())
    {
        return;
    }

    this->fx = this->dExecute(derivative);
}

//bool Numerical::Interpolator::findStart(const OrderedPoints *points, UInt &degree, Real x, UInt &start) const
bool Numerical::Interpolator::findStart()
{
    if (!this->degreeSet)
    {
        this->degree = UINT_MAX;
    }

    if (this->points->getNumPoints() == 0)
    {
        return false;
    }

    if ((this->x < this->points->x(0)) || (this->x > this->points->x(this->points->getNumPoints() - 1)))
    {
        return false;
    }

    if (this->degree > this->points->getNumPoints() - 1)
    {
        this->degree = this->points->getNumPoints() - 1;
    }

    if (this->degree == this->points->getNumPoints() - 1)
    {
        this->start = 0;

        return true;
    }

    UInt from = this->points->search(x);

    if ((this->degree == 1) || (from == 0))
    {
        this->start = from;

        return true;
    }

    UInt to = from + 1;

    for (UInt i = 1; i < this->degree; i++)
    {
        if (from == 0)
        {
            break;
        }

        if (to == this->points->getNumPoints() - 1)
        {
            --from;

            continue;
        }

        Real dprev = std::fabs(this->points->x(from - 1) - x);
        Real dnext = std::fabs(this->points->x(to + 1) - x);

        if (dprev < dnext)
        {
            --from;
        }
        else
        {
            ++to;
        }
    }

    this->start = from;

    return true;
}

#endif //#if USE_NUMERICAL
