#include "Numerical/ODESolver.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ODESolver::ODESolver() :
    Performer::Builder::Builder()
{
    this->system = NULL;

    this->step = 0.0;

    this->min = 0.0;
    this->max = 0.0;

    this->result = NULL;
}

Numerical::ODESolver::~ODESolver()
{

}

void Numerical::ODESolver::setSystem(ODESystem *system)
{
    this->system = system;
}

ODESystem *Numerical::ODESolver::getSystem() const
{
    return this->system;
}

void Numerical::ODESolver::setStep(Real step)
{
    this->step = step;

    Real dif = this->max - this->min;

    if (std::fabs(dif) > Data::getTolerance())
    {
        this->numSteps = static_cast<UInt>(std::ceil(dif/this->step));
    }
}

Real Numerical::ODESolver::getStep() const
{
    return this->step;
}

void Numerical::ODESolver::setMin(Real min)
{
    this->min = min;
}

Real Numerical::ODESolver::getMin() const
{
    return this->min;
}

void Numerical::ODESolver::setMax(Real max)
{
    this->max = max;
}

Real Numerical::ODESolver::getMax() const
{
    return this->max;
}

void Numerical::ODESolver::setNumSteps(UInt numSteps)
{
    this->numSteps = numSteps;

    Real dif = this->max - this->min;

    if (std::fabs(dif) > Data::getTolerance())
    {
        this->step = dif/static_cast<Real>(this->numSteps);
    }
}

UInt Numerical::ODESolver::getNumSteps() const
{
    return this->numSteps;
}

Real *Numerical::ODESolver::getResult() const
{
    return this->result;
}

Numerical::SingleStepODESolver::SingleStepODESolver() :
    Numerical::ODESolver::ODESolver()
{
    this->order = 0;
}

Numerical::SingleStepODESolver::~SingleStepODESolver()
{

}

void Numerical::SingleStepODESolver::setOrder(UInt order)
{
    this->order = order;
}

UInt Numerical::SingleStepODESolver::getOrder() const
{
    return this->order;
}

Numerical::MultiStepODESolver::MultiStepODESolver() :
    Numerical::ODESolver::ODESolver()
{
    this->numPoints = 0;

    this->initializer = NULL;
    this->createdInitializer = false;
}

Numerical::MultiStepODESolver::~MultiStepODESolver()
{
    if (this->createdInitializer)
    {
        delete this->initializer;
    }
}

void Numerical::MultiStepODESolver::setNumPoints(UInt numPoints)
{
    this->numPoints = numPoints;
}

UInt Numerical::MultiStepODESolver::getNumPoints() const
{
    return this->numPoints;
}

void Numerical::MultiStepODESolver::setInitializer(SingleStepODESolver *initializer)
{
    this->initializer = initializer;

    if (this->initializer)
    {
        this->createdInitializer = false;
    }
}

SingleStepODESolver *Numerical::MultiStepODESolver::getInitializer() const
{
    return this->initializer;
}

void Numerical::MultiStepODESolver::moveLast(UInt numFunctions, UInt numPoints, Real prev[], Real next[])
{
    Numerical::MultiStepODESolver::move(numFunctions, 1, &prev[(numPoints-1)*numFunctions], next);
}

void Numerical::MultiStepODESolver::move(UInt numFunctions, UInt numPoints, Real prev[], Real next[])
{
    for (UInt i = 0; i < numPoints - 1; i++)
    {
        for (UInt j = 0; j < numFunctions; j++)
        {
            prev[i*numFunctions + j] = prev[(i+1)*numFunctions + j];
        }
    }

    if (next)
    {
        for (UInt j = 0; j < numFunctions; j++)
        {
            prev[(numPoints - 1)*numFunctions + j] = next[j];
        }
    }
}

#endif //#if USE_NUMERICAL
