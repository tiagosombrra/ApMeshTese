#include "Numerical/EquationRootFinder.h"

#if USE_NUMERICAL

#if USE_GUI
#include "Data/Point3D.h"
#endif //#if USE_GUI

using namespace Data;
using namespace Numerical;

const UInt Numerical::EquationRootFinder::MAX_STEPS = 1000;
const Real Numerical::EquationRootFinder::TOLERANCE = 0.000001;

Numerical::EquationRootFinder::EquationRootFinder() : Performer::Builder::Builder()
{
    this->function = NULL;
    this->root = 0.0;

    this->testingFunction = true;
    this->testingStepSize = true;

    this->maxSteps = this->MAX_STEPS;
    this->tolerance = this->TOLERANCE;

    this->ok = true;
    this->numSteps = 0;

#if USE_GUI
    this->storingPoints = false;
#endif //#if USE_GUI
}

Numerical::EquationRootFinder::~EquationRootFinder()
{

}

void Numerical::EquationRootFinder::setFunction(Function *function)
{
    this->function = function;
}

Function *Numerical::EquationRootFinder::getFunction() const
{
    return this->function;
}

Real Numerical::EquationRootFinder::getRoot() const
{
    return this->root;
}

void Numerical::EquationRootFinder::setTestingFunction(bool testingFunction)
{
    this->testingFunction = testingFunction;
}

bool Numerical::EquationRootFinder::isTestingFunction() const
{
    return this->testingFunction;
}

void Numerical::EquationRootFinder::setTestingStepSize(bool testingStepSize)
{
    this->testingStepSize = testingStepSize;
}

bool Numerical::EquationRootFinder::isTestingStepSize() const
{
    return this->testingStepSize;
}

void Numerical::EquationRootFinder::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::EquationRootFinder::getMaxSteps() const
{
    return this->maxSteps;
}

void Numerical::EquationRootFinder::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::EquationRootFinder::getTolerance() const
{
    return this->tolerance;
}

bool Numerical::EquationRootFinder::isOk() const
{
    return this->ok;
}

UInt Numerical::EquationRootFinder::getNumSteps() const
{
    return this->numSteps;
}

#if USE_GUI
void Numerical::EquationRootFinder::setStoringPoints(bool storingPoints)
{
    this->storingPoints = storingPoints;
}

bool Numerical::EquationRootFinder::isStoringPoints() const
{
    return this->storingPoints;
}

Data::Point3DObjVector Numerical::EquationRootFinder::getPoints() const
{
    return this->points;
}

void Numerical::EquationRootFinder::store(Real x, Real y)
{
    if (this->storingPoints)
    {
        this->points.push_back(Data::Point3D(x, y, 0.0));
    }
}
#endif //#if USE_GUI

#endif //#if USE_NUMERICAL
