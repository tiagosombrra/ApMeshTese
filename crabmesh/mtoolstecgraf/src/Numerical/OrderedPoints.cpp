#include "Numerical/OrderedPoints.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::OrderedPoints::OrderedPoints()
{
    this->numPoints = 0;
    this->points[0] = this->points[1] = NULL;
}

Numerical::OrderedPoints::~OrderedPoints()
{
    delete [] this->points[0];
    delete [] this->points[1];
}

void Numerical::OrderedPoints::setNumPoints(UInt numPoints)
{
    if (numPoints == this->numPoints)
    {
        return;
    }

    Real *oldPoints[2];
    oldPoints[0] = this->points[0];
    oldPoints[1] = this->points[1];

    this->points[0] = new Real[numPoints];
    this->points[1] = new Real[numPoints];

    if (this->numPoints != 0)
    {
        UInt np = std::min(numPoints, this->numPoints);

        for (UInt i = 0; i < np; i++)
        {
            this->points[0][i] = oldPoints[0][i];
            this->points[1][i] = oldPoints[1][i];
        }

        delete [] oldPoints[0];
        delete [] oldPoints[1];
    }

    this->numPoints = numPoints;
}

UInt Numerical::OrderedPoints::getNumPoints() const
{
    return this->numPoints;
}

void Numerical::OrderedPoints::setPoint(UInt i, Real x, Real y)
{
    this->points[0][i] = x;
    this->points[1][i] = y;
}

Real Numerical::OrderedPoints::x(UInt i) const
{
    return this->points[0][i];
}

Real Numerical::OrderedPoints::y(UInt i) const
{
    return this->points[1][i];
}

UInt Numerical::OrderedPoints::search(Real x, UInt min, UInt max) const
{
    UInt mid = (min + max)/2;

    if ((this->points[0][mid] <= x) && (this->points[0][mid + 1] >= x))
    {
        return mid;
    }

    return (this->points[0][mid] > x) ? this->search(x, min, mid) : this->search(x, mid, max);
}

UInt Numerical::OrderedPoints::search(Real x) const
{
    if ((this->points[0][0] <= x) && (this->points[0][1] >= x))
    {
        return 0;
    }

    if ((this->points[0][this->numPoints - 2] <= x) && (this->points[0][this->numPoints - 1] >= x))
    {
        return this->numPoints - 2;
    }

    return this->search(x, 0, this->numPoints);
}

bool Numerical::OrderedPoints::isEquallySpaced() const
{
    return this->isEquallySpaced(Data::getTolerance());
}

bool Numerical::OrderedPoints::isEquallySpaced(Real tolerance) const
{
    if (this->numPoints <= 2)
    {
        return true;
    }

    Real distance = points[0][1] - this->points[0][0];

    for (UInt i = 2; i < this->numPoints; i++)
    {
        Real d = points[0][i] - this->points[0][i - 1];

        if (std::fabs(d - distance) > tolerance)
        {
            return false;
        }
    }

    return true;
}

#endif //#if USE_NUMERICAL
