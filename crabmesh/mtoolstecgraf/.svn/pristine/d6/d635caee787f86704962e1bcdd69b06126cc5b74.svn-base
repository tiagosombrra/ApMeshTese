#include "Numerical/RombergIntegralCalculator.h"

#if USE_NUMERICAL

#include "Data/OStream.h"
#include "Numerical/NewtonCotesIntegralCalculator.h"
#include "Numerical/GaussLegendreIntegralCalculator.h"
#include "Numerical/Function.h"

using namespace Data;
using namespace Numerical;

const UInt Numerical::RombergIntegralCalculator::MAX_STEPS = 5;
const Real Numerical::RombergIntegralCalculator::TOLERANCE = 0.000001;

/*Real Numerical::RombergIntegralCalculator::executeIntegral(IntegralFunction *function, NewtonCotesIntegralCalculator *nc, GaussLegendreIntegralCalculator *gl, UInt numPoints, UInt numSubdivisions)
{
    if (nc)
    {
        return nc->execute(function, numSubdivisions, numPoints);
    }

    Real sum = 0.0;
    Real min = function->min();
    Real max = function->max();
    Real dist = (max - min)/static_cast<Real>(numSubdivisions);

    for (UInt i = 0; i < numSubdivisions; i++)
    {
        IntervalIntegralFunction f;

        Real a = min + static_cast<Real>(i)*dist;
        Real b = (i == numSubdivisions - 1) ? max : a + dist;

        f.setFunction(function);
        f.setMin(a);
        f.setMax(b);

        sum += gl->execute(&f, numPoints);
    }

    return sum;
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps, Real tolerance, bool testTolerance)
{
    NewtonCotesIntegralCalculator *nc = dynamic_cast<NewtonCotesIntegralCalculator *>(integral);
    GaussLegendreIntegralCalculator *gl = dynamic_cast<GaussLegendreIntegralCalculator *>(integral);

    if ((!nc) && (!gl))
    {
        return 0.0;
    }

    if (maxSteps < 1)
    {
        return 0.0;
    }

    Real lines[2][maxSteps];

    UInt numSubdivisions = 1;

    //TableStream str(8, 4);

    for (UInt i = 0; i < maxSteps; i++, numSubdivisions *= 2)
    {
        UInt row = i%2;
        UInt prow = (i+1)%2;

        for (UInt j = 0; j < i + 1; j++)
        {
            if (j == 0)
            {
                if ((nc) && dynamic_cast<ClosedNewtonCotesIntegralCalculator *>(nc) && (numPoints == 2) && (i > 0))
                {
                    lines[row][j] = 0.0;

                    Real a = function->min();
                    Real h = (function->max() - a)/static_cast<Real>(numSubdivisions);
                    Real hp = 2.0*h;

                    UInt l = static_cast<UInt>(std::pow(2, i - 1));

                    for (UInt k = 1; k <= l; k++)
                    {
                        lines[row][j] += function->f(a + static_cast<Real>(2.0*k - 1)*h);
                    }

                    lines[row][j] *= hp;
                    lines[row][j] += lines[prow][j];
                    lines[row][j] *= 0.5;

                    continue;
                }

                lines[row][j] = this->executeIntegral(function, nc, gl, numPoints, numSubdivisions);

                continue;
            }

            lines[row][j] = lines[row][j-1] + (1.0/(std::pow(4.0, j) - 1.0))*(lines[row][j-1] - lines[prow][j-1]);
        }

        //for (UInt j = 0; j < i + 1; j++)
        //{
            //str << lines[row][j];
        //}

        //str.endl();

        if ((testTolerance) && (i > 0))
        {
            if ((std::fabs(lines[row][i] - lines[row][i-1]) <= tolerance) || (std::fabs(lines[row][i] - lines[prow][i-1]) <= tolerance))
            {
                return lines[row][i];
            }
        }
    }

    //std::cout << str.str() << std::endl;

    return lines[(maxSteps - 1)%2][maxSteps - 1];
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps, Real tolerance)
{
    return this->execute(function, integral, numPoints, maxSteps, tolerance, true);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, Real tolerance)
{
    return this->execute(function, integral, numPoints, this->MAX_STEPS, tolerance, true);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints, UInt maxSteps)
{
    return this->execute(function, integral, numPoints, maxSteps, this->TOLERANCE, false);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, IntegralCalculator *integral, UInt numPoints)
{
    return this->execute(function, integral, numPoints, this->MAX_STEPS, this->TOLERANCE, true);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, UInt maxSteps, Real tolerance)
{
    ClosedNewtonCotesIntegralCalculator nc;

    return this->execute(function, &nc, 2, maxSteps, tolerance);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, Real tolerance)
{
    ClosedNewtonCotesIntegralCalculator nc;

    return this->execute(function, &nc, 2, tolerance);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function, UInt maxSteps)
{
    ClosedNewtonCotesIntegralCalculator nc;

    return this->execute(function, &nc, 2, maxSteps);
}

Real Numerical::RombergIntegralCalculator::execute(IntegralFunction *function)
{
    return this->execute(function, this->MAX_STEPS, this->TOLERANCE);
}*/

Numerical::RombergIntegralCalculator::RombergIntegralCalculator() :
    Numerical::FunctionBasedIntegralCalculator::FunctionBasedIntegralCalculator()
{
    this->calculator = NULL;
    this->createdCalculator = false;

    this->maxSteps = this->MAX_STEPS;
    this->tolerance = this->TOLERANCE;

    this->testingTolerance = true;
}

Numerical::RombergIntegralCalculator::~RombergIntegralCalculator()
{
    if (this->createdCalculator)
    {
        delete this->calculator;
    }
}

void Numerical::RombergIntegralCalculator::setCalculator(IntegralCalculator *calculator)
{
    this->calculator = calculator;
}

IntegralCalculator *Numerical::RombergIntegralCalculator::getCalculator() const
{
    return this->calculator;
}

void Numerical::RombergIntegralCalculator::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::RombergIntegralCalculator::getMaxSteps() const
{
    return this->maxSteps;
}

void Numerical::RombergIntegralCalculator::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::RombergIntegralCalculator::getTolerance() const
{
    return this->tolerance;
}

void Numerical::RombergIntegralCalculator::setTestingTolerance(bool testingTolerance)
{
    this->testingTolerance = testingTolerance;
}

bool Numerical::RombergIntegralCalculator::isTestingTolerance() const
{
    return this->testingTolerance;
}

void Numerical::RombergIntegralCalculator::execute()
{
    /*NewtonCotesIntegralCalculator *nc = dynamic_cast<NewtonCotesIntegralCalculator *>(integral);
    GaussLegendreIntegralCalculator *gl = dynamic_cast<GaussLegendreIntegralCalculator *>(integral);

    if ((!nc) && (!gl))
    {
        return 0.0;
    }*/

    if (this->maxSteps < 1)
    {
        //return 0.0;
        this->maxSteps = 1;
    }

    if (!this->calculator)
    {
        this->calculator = new ClosedNewtonCotesIntegralCalculator();

        static_cast<ClosedNewtonCotesIntegralCalculator *>(this->calculator)->setNumPoints(2);

        this->createdCalculator = true;
    }

    Real lines[2][this->maxSteps];

    UInt numSubdivisions = 1;

    for (UInt i = 0; i < this->maxSteps; i++, numSubdivisions *= 2)
    {
        UInt row = i%2;
        UInt prow = (i+1)%2;

        for (UInt j = 0; j < i + 1; j++)
        {
            if (j == 0)
            {
                //if ((nc) && dynamic_cast<ClosedNewtonCotesIntegralCalculator *>(nc) && (numPoints == 2) && (i > 0))
                if ((this->createdCalculator) && (i > 0))
                {
                    lines[row][j] = 0.0;

                    Real a = this->min;
                    Real h = (this->max - a)/static_cast<Real>(numSubdivisions);
                    Real hp = 2.0*h;

                    UInt l = static_cast<UInt>(std::pow(2, i - 1));

                    for (UInt k = 1; k <= l; k++)
                    {
                        lines[row][j] += function->f(a + static_cast<Real>(2.0*k - 1)*h);
                    }

                    lines[row][j] *= hp;
                    lines[row][j] += lines[prow][j];
                    lines[row][j] *= 0.5;

                    continue;
                }

                lines[row][j] = this->sumIntegrals(numSubdivisions);

                continue;
            }

            lines[row][j] = lines[row][j-1] + (1.0/(std::pow(4.0, j) - 1.0))*(lines[row][j-1] - lines[prow][j-1]);
        }

        if ((this->testingTolerance) && (i > 0))
        {
            if ((std::fabs(lines[row][i] - lines[row][i-1]) <= this->tolerance) || (std::fabs(lines[row][i] - lines[prow][i-1]) <= this->tolerance))
            {
                this->integral = lines[row][i];

                return;
            }
        }
    }

    //std::cout << str.str() << std::endl;

    this->integral = lines[(this->maxSteps - 1)%2][this->maxSteps - 1];
}

Real Numerical::RombergIntegralCalculator::sumIntegrals(UInt numSubdivisions)
{
    PointBasedIntegralCalculator *pb = dynamic_cast<PointBasedIntegralCalculator *>(this->calculator);

    if (pb)
    {
        pb->setPoints(this->function, this->min, this->max, numSubdivisions);

        pb->execute();

        return pb->getIntegral();
    }

    FunctionBasedIntegralCalculator *fb = dynamic_cast<FunctionBasedIntegralCalculator *>(this->calculator);

    Real sum = 0.0;
    Real dist = (this->max - this->min)/static_cast<Real>(numSubdivisions);

    fb->setFunction(this->function);

    for (UInt i = 0; i < numSubdivisions; i++)
    {
        fb->setMin(this->min + static_cast<Real>(i)*dist);
        fb->setMax((i == numSubdivisions - 1) ? this->max : this->min + dist);

        fb->execute();

        sum += fb->getIntegral();
    }

    return sum;
}

#endif //#if USE_NUMERICAL
