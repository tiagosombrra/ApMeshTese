#include "Numerical/NewtonCotesIntegralCalculator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"

using namespace Data;
using namespace Numerical;

/*Real Numerical::NewtonCotesIntegralCalculator::execute(IntegralFunction *function, Real distance, UInt numPoints)
{
    OrderedPoints *points = this->findPoints(function, distance);

    Real val = this->execute(points, numPoints);

    delete points;

    return val;
}

Real Numerical::NewtonCotesIntegralCalculator::execute(IntegralFunction *function, UInt numSubdivisions, UInt numPoints)
{
    OrderedPoints *points = this->findPoints(function, numSubdivisions);

    Real val = this->execute(points, numPoints);

    delete points;

    return val;
}*/

Numerical::NewtonCotesIntegralCalculator::NewtonCotesIntegralCalculator() :
    Numerical::PointBasedIntegralCalculator::PointBasedIntegralCalculator()
{
    this->numPoints = 0;
}

Numerical::NewtonCotesIntegralCalculator::~NewtonCotesIntegralCalculator()
{

}

void Numerical::NewtonCotesIntegralCalculator::setNumPoints(UInt numPoints)
{
    this->numPoints = numPoints;
}

UInt Numerical::NewtonCotesIntegralCalculator::getNumPoints() const
{
    return this->numPoints;
}

const Real Numerical::ClosedNewtonCotesIntegralCalculator::weights[4][6] =
{
	{1.0/ 2.0, 1.0,  1.0,  0.0,  0.0, 0.0},
	{1.0/ 3.0, 1.0,  4.0,  1.0,  0.0, 0.0},
	{3.0/ 8.0, 1.0,  3.0,  3.0,  1.0, 0.0},
	{2.0/45.0, 7.0, 32.0, 12.0, 32.0, 7.0}
};

/*Real Numerical::ClosedNewtonCotesIntegralCalculator::execute(OrderedPoints *points, UInt numPoints)
{
    if (points->getNumPoints() == 0)
    {
        return 0.0;
    }

    if (points->getNumPoints() == 1)
    {
        return points->y(0);
    }

    if (points->getNumPoints() < numPoints)
    {
        numPoints = points->getNumPoints();
    }

    if ((numPoints >= 2) && (numPoints <= 5))
    {
        return this->executeFrom(points, numPoints, 0);
    }

    return 0.0;
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::execute(OrderedPoints *points)
{
    return this->execute(points, 3);
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeFrom(OrderedPoints *points, UInt numPoints, UInt start) const
{
    UInt n = numPoints - 1;
    Real sum = 0.0;

    for (UInt i = start; i + n < points->getNumPoints(); i += n)
    {
        Real h = 0.0;

        for (UInt j = i; j < i + n; j++)
        {
            h += points->x(j + 1) - points->x(j);
        }

        h /= static_cast<Real>(n);

        Real s = 0.0;

        for (UInt j = i; j < i + n + 1; j++)
        {
            s += points->y(j)*this->weights[n - 1][j - i + 1];
        }

        sum += s*h*this->weights[n - 1][0];
    }

    UInt nd = (points->getNumPoints() - start - 1)%(n);

    if (nd != 0)
    {
        sum += this->executeFrom(points, nd + 1, points->getNumPoints() - nd - 1);
    }

    return sum;
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeTrapezoidalRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 2, 0);
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeSimpson13Rule(OrderedPoints *points) const
{
    return this->executeFrom(points, 3, 0);
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeSimpson38Rule(OrderedPoints *points) const
{
    return this->executeFrom(points, 4, 0);
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeBooleRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 5, 0);
}*/

Numerical::ClosedNewtonCotesIntegralCalculator::ClosedNewtonCotesIntegralCalculator() :
    Numerical::NewtonCotesIntegralCalculator::NewtonCotesIntegralCalculator()
{

}

Numerical::ClosedNewtonCotesIntegralCalculator::~ClosedNewtonCotesIntegralCalculator()
{

}

void Numerical::ClosedNewtonCotesIntegralCalculator::execute()
{
    if (this->points->getNumPoints() == 0)
    {
        this->integral = 0.0;

        return;
    }

    if (this->points->getNumPoints() == 1)
    {
        this->integral = this->points->y(0);

        return;
    }

    if (this->points->getNumPoints() < this->numPoints)
    {
        this->numPoints = this->points->getNumPoints();
    }

    if (this->numPoints < 2)
    {
        this->numPoints = 2;
    }

    if (this->numPoints > 5)
    {
        this->numPoints = 5;
    }

    if ((this->numPoints >= 2) && (this->numPoints <= 5))
    {
        this->integral = this->executeFrom(this->numPoints, 0);

        return;
    }

    this->integral = 0.0;
}

void Numerical::ClosedNewtonCotesIntegralCalculator::executeTrapezoidalRule()
{
    this->numPoints = 2;

    this->execute();
}

void Numerical::ClosedNewtonCotesIntegralCalculator::executeSimpson13Rule()
{
    this->numPoints = 3;

    this->execute();
}

void Numerical::ClosedNewtonCotesIntegralCalculator::executeSimpson38Rule()
{
    this->numPoints = 4;

    this->execute();
}

void Numerical::ClosedNewtonCotesIntegralCalculator::executeBooleRule()
{
    this->numPoints = 5;

    this->execute();
}

Real Numerical::ClosedNewtonCotesIntegralCalculator::executeFrom(UInt numPoints, UInt start) const
{
    UInt n = numPoints - 1;
    Real sum = 0.0;

    for (UInt i = start; i + n < this->points->getNumPoints(); i += n)
    {
        Real h = 0.0;

        for (UInt j = i; j < i + n; j++)
        {
            h += this->points->x(j + 1) - this->points->x(j);
        }

        h /= static_cast<Real>(n);

        Real s = 0.0;

        for (UInt j = i; j < i + n + 1; j++)
        {
            s += this->points->y(j)*this->weights[n - 1][j - i + 1];
        }

        sum += s*h*this->weights[n - 1][0];
    }

    UInt nd = (this->points->getNumPoints() - start - 1)%(n);

    if (nd != 0)
    {
        sum += this->executeFrom(nd + 1, this->points->getNumPoints() - nd - 1);
    }

    return sum;
}

const Real Numerical::OpenNewtonCotesIntegralCalculator::weights[4][5] =
{
	{2.0     ,  1.0,  0.0,  0.0,  0.0},
	{3.0/ 2.0,  1.0,  1.0,  0.0,  0.0},
	{4.0/ 3.0,  2.0, -1.0,  2.0,  0.0},
	{5.0/24.0, 11.0,  1.0,  1.0, 11.0}
};

/*Real Numerical::OpenNewtonCotesIntegralCalculator::execute(OrderedPoints *points, UInt numPoints)
{
    if (points->getNumPoints() <= 2)
    {
        return 0.0;
    }

    if (points->getNumPoints() - 2 < numPoints)
    {
        numPoints = points->getNumPoints() - 2;
    }

    if ((numPoints >= 1) && (numPoints <= 4))
    {
        return this->executeFrom(points, numPoints, 0);
    }

    return 0.0;
}

Real Numerical::OpenNewtonCotesIntegralCalculator::execute(OrderedPoints *points)
{
    return this->execute(points, 2);
}

Real Numerical::OpenNewtonCotesIntegralCalculator::executeFrom(OrderedPoints *points, UInt numPoints, UInt start) const
{
    UInt n = numPoints;
    Real sum = 0.0;

    for (UInt i = start + 1; i + n < points->getNumPoints(); i += n + 1)
    {
        Real h = 0.0;

        for (UInt j = i - 1; j < i + n; j++)
        {
            h += points->x(j + 1) - points->x(j);
        }

        h /= static_cast<Real>(n + 1);

        Real s = 0.0;

        for (UInt j = i; j < i + n; j++)
        {
            s += points->y(j)*this->weights[n - 1][j - i + 1];
        }

        sum += s*h*this->weights[n - 1][0];
    }

    UInt nd = (points->getNumPoints() - start - 1)%(n + 1);

    if (nd == 1)
    {
        sum += 0.5*this->executeFrom(points, 1, points->getNumPoints() - 3);
    }
    else if (nd > 1)
    {
        sum += this->executeFrom(points, nd - 1, points->getNumPoints() - nd - 1);
    }

    return sum;
}

Real Numerical::OpenNewtonCotesIntegralCalculator::executeMidPointRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 1, 0);
}

Real Numerical::OpenNewtonCotesIntegralCalculator::execute2PointRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 2, 0);
}

Real Numerical::OpenNewtonCotesIntegralCalculator::execute3PointRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 3, 0);
}

Real Numerical::OpenNewtonCotesIntegralCalculator::execute4PointRule(OrderedPoints *points) const
{
    return this->executeFrom(points, 4, 0);
}*/

Numerical::OpenNewtonCotesIntegralCalculator::OpenNewtonCotesIntegralCalculator() :
    Numerical::NewtonCotesIntegralCalculator::NewtonCotesIntegralCalculator()
{

}

Numerical::OpenNewtonCotesIntegralCalculator::~OpenNewtonCotesIntegralCalculator()
{

}

void Numerical::OpenNewtonCotesIntegralCalculator::execute()
{
    if (this->points->getNumPoints() <= 2)
    {
        this->integral = 0.0;

        return;
    }

    if (this->points->getNumPoints() - 2 < this->numPoints)
    {
        this->numPoints = this->points->getNumPoints() - 2;
    }

    if (this->numPoints < 1)
    {
        this->numPoints = 1;
    }

    if (this->numPoints > 4)
    {
        this->numPoints = 4;
    }

    if ((this->numPoints >= 1) && (this->numPoints <= 4))
    {
        this->integral = this->executeFrom(this->numPoints, 0);

        return;
    }

    this->integral = 0.0;
}

void Numerical::OpenNewtonCotesIntegralCalculator::executeMidPointRule()
{
    this->numPoints = 1;

    this->execute();
}

void Numerical::OpenNewtonCotesIntegralCalculator::execute2PointRule()
{
    this->numPoints = 2;

    this->execute();
}

void Numerical::OpenNewtonCotesIntegralCalculator::execute3PointRule()
{
    this->numPoints = 3;

    this->execute();
}

void Numerical::OpenNewtonCotesIntegralCalculator::execute4PointRule()
{
    this->numPoints = 4;

    this->execute();
}

Real Numerical::OpenNewtonCotesIntegralCalculator::executeFrom(UInt numPoints, UInt start) const
{
    UInt n = numPoints;
    Real sum = 0.0;

    for (UInt i = start + 1; i + n < this->points->getNumPoints(); i += n + 1)
    {
        Real h = 0.0;

        for (UInt j = i - 1; j < i + n; j++)
        {
            h += this->points->x(j + 1) - this->points->x(j);
        }

        h /= static_cast<Real>(n + 1);

        Real s = 0.0;

        for (UInt j = i; j < i + n; j++)
        {
            s += this->points->y(j)*this->weights[n - 1][j - i + 1];
        }

        sum += s*h*this->weights[n - 1][0];
    }

    UInt nd = (this->points->getNumPoints() - start - 1)%(n + 1);

    if (nd == 1)
    {
        sum += 0.5*this->executeFrom(1, this->points->getNumPoints() - 3);
    }
    else if (nd > 1)
    {
        sum += this->executeFrom(nd - 1, this->points->getNumPoints() - nd - 1);
    }

    return sum;
}

#endif //#if USE_NUMERICAL
