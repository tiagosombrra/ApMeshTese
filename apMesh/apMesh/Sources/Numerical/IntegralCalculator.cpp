#include "Numerical/IntegralCalculator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"
#include "Numerical/InterpolationFunction.h"

using namespace Data;
using namespace Numerical;

//const Real Numerical::PointBasedIntegralCalculator::DISTANCE = 0.1;
//const UInt Numerical::PointBasedIntegralCalculator::NUM_SUBDIVISIONS = 10;

Numerical::IntegralCalculator::IntegralCalculator() :
    Performer::Builder::Builder()
{
    this->integral = 0.0;
}

Numerical::IntegralCalculator::~IntegralCalculator()
{

}

Real Numerical::IntegralCalculator::getIntegral() const
{
    return this->integral;
}

Numerical::FunctionBasedIntegralCalculator::FunctionBasedIntegralCalculator() :
    Numerical::IntegralCalculator::IntegralCalculator()
{
    this->function = NULL;
    this->min = 0.0;
    this->max = 0.0;

    this->fromPoints = false;
}

Numerical::FunctionBasedIntegralCalculator::~FunctionBasedIntegralCalculator()
{
    if (this->fromPoints)
    {
        //dynamic_cast<InterpolationFunction *>(this->function)->setPoints(NULL);

        delete this->function;
    }
}

void Numerical::FunctionBasedIntegralCalculator::setFunction(Function *function)
{
    this->function = function;

    this->fromPoints = false;
}

Function *Numerical::FunctionBasedIntegralCalculator::getFunction() const
{
    return this->function;
}

void Numerical::FunctionBasedIntegralCalculator::setFunction(OrderedPoints *points)
{
    this->function = new InterpolationFunction(points);

    this->min = points->x(0);
    this->max = points->x(points->getNumPoints() - 1);
}

void Numerical::FunctionBasedIntegralCalculator::setMin(Real min)
{
    this->min = min;
}

Real Numerical::FunctionBasedIntegralCalculator::getMin() const
{
    return this->min;
}

void Numerical::FunctionBasedIntegralCalculator::setMax(Real max)
{
    this->max = max;
}

Real Numerical::FunctionBasedIntegralCalculator::getMax() const
{
    return this->max;
}

/*Real Numerical::FunctionBasedIntegralCalculator::execute(OrderedPoints *points)
{
    Numerical::InterpolationFunction function(points);

    IntegralInterpolationFunction integralFunction;

    integralFunction.function = &function;

    return this->execute(&integralFunction);
}*/

/*Real Numerical::PointBasedIntegralCalculator::execute(IntegralFunction *function)
{
    return this->execute(function, this->NUM_SUBDIVISIONS);
}

Real Numerical::PointBasedIntegralCalculator::execute(IntegralFunction *function, Real distance)
{
    OrderedPoints *points = this->findPoints(function, distance);

    Real val = this->execute(points);

    delete points;

    return val;
}

Real Numerical::PointBasedIntegralCalculator::execute(IntegralFunction *function, UInt numSubdivisions)
{
    OrderedPoints *points = this->findPoints(function, numSubdivisions);

    Real val = this->execute(points);

    delete points;

    return val;
}

OrderedPoints *Numerical::PointBasedIntegralCalculator::findPoints(IntegralFunction *function, Real distance) const
{
    OrderedPoints *points = new OrderedPoints();

    Real min = function->min();
    Real max = function->max();

    UInt np = static_cast<UInt>((max - min)/distance + 0.5);

    points->setNumPoints(np);

    UInt i = 0;

    for (Real x = min; x <= max; x += distance, i++)
    {
        Real y = function->f(x);

        if (i >= np)
        {
            np = i + 1;

            points->setNumPoints(np);
        }

        points->setPoint(i, x, y);
    }

    if (i + 1 < np)
    {
        np = i + 1;

        points->setNumPoints(np);
    }

    return points;
}

OrderedPoints *Numerical::PointBasedIntegralCalculator::findPoints(IntegralFunction *function, UInt numSubdivisions) const
{
    OrderedPoints *points = new OrderedPoints();

    UInt numPoints = numSubdivisions + 1;

    Real min = function->min();
    Real max = function->max();

    points->setNumPoints(numPoints);

    Real distance = (max - min)/static_cast<Real>(numSubdivisions);

    for (UInt i = 0; i < numPoints; i++)
    {
        Real x = (i < numPoints/2) ? min + static_cast<Real>(i)*distance : max - static_cast<Real>(numPoints - i - 1)*distance ;
        Real y = function->f(x);

        points->setPoint(i, x, y);
    }

    return points;
}*/

Numerical::PointBasedIntegralCalculator::PointBasedIntegralCalculator() :
    Numerical::IntegralCalculator::IntegralCalculator()
{
    this->points = NULL;

    this->fromFunction = false;
}

Numerical::PointBasedIntegralCalculator::~PointBasedIntegralCalculator()
{
    if (this->fromFunction)
    {
        delete this->points;
    }
}

void Numerical::PointBasedIntegralCalculator::setPoints(OrderedPoints *points)
{
    this->points = points;

    this->fromFunction = false;
}

OrderedPoints *Numerical::PointBasedIntegralCalculator::getPoints() const
{
    return this->points;
}

void Numerical::PointBasedIntegralCalculator::setPoints(Function *function, Real min, Real max, Real distance)
{
    this->points = new OrderedPoints();

    this->fromFunction = true;

    UInt np = static_cast<UInt>((max - min)/distance + 0.5);

    this->points->setNumPoints(np);

    UInt i = 0;

    for (Real x = min; x <= max; x += distance, i++)
    {
        Real y = function->f(x);

        if (i >= np)
        {
            np = i + 1;

            points->setNumPoints(np);
        }

        this->points->setPoint(i, x, y);
    }

    if (i + 1 < np)
    {
        np = i + 1;

        this->points->setNumPoints(np);
    }
}

void Numerical::PointBasedIntegralCalculator::setPoints(Function *function, Real min, Real max,UInt numSubdivisions)
{
    this->points = new OrderedPoints();

    this->fromFunction = true;

    UInt numPoints = numSubdivisions + 1;

    this->points->setNumPoints(numPoints);

    Real distance = (max - min)/static_cast<Real>(numSubdivisions);

    for (UInt i = 0; i < numPoints; i++)
    {
        Real x = (i < numPoints/2) ? min + static_cast<Real>(i)*distance : max - static_cast<Real>(numPoints - i - 1)*distance ;
        Real y = function->f(x);

        this->points->setPoint(i, x, y);
    }
}

#endif //#if USE_NUMERICAL
