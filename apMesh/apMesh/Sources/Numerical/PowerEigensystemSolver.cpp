#include "Numerical/PowerEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::PowerEigensystemSolver::PowerEigensystemSolver() :
    Numerical::EigensystemSolver::EigensystemSolver()
{
    this->guess = NULL;

    this->copy = false;

    this->tolerance = this->TOLERANCE;

    this->maxSteps = this->MAX_STEPS;
    this->numSteps = 0;
    this->testingEigenvalue = true;
    this->testingEigenvector = true;

    this->eigenvector = NULL;
    this->eigenvalue = 0.0;
}

Numerical::PowerEigensystemSolver::~PowerEigensystemSolver()
{

}

void Numerical::PowerEigensystemSolver::setGuess(IColumnMatrix *guess)
{
    this->guess = guess;
}

IColumnMatrix *Numerical::PowerEigensystemSolver::getGuess() const
{
    return this->guess;
}

void Numerical::PowerEigensystemSolver::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::PowerEigensystemSolver::getTolerance() const
{
    return this->tolerance;
}

void Numerical::PowerEigensystemSolver::setCopy(bool copy)
{
    this->copy = copy;
}

bool Numerical::PowerEigensystemSolver::isCopy() const
{
    return this->copy;
}

void Numerical::PowerEigensystemSolver::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::PowerEigensystemSolver::getMaxSteps() const
{
    return this->maxSteps;
}

void Numerical::PowerEigensystemSolver::setTestingEigenvalue(bool testingEigenvalue)
{
    this->testingEigenvalue = testingEigenvalue;
}

bool Numerical::PowerEigensystemSolver::isTestingEigenvalue() const
{
    return this->testingEigenvalue;
}

void Numerical::PowerEigensystemSolver::setTestingEigenvector(bool testingEigenvector)
{
    this->testingEigenvector = testingEigenvector;
}

bool Numerical::PowerEigensystemSolver::isTestingEigenvector() const
{
    return this->testingEigenvector;
}

UInt Numerical::PowerEigensystemSolver::getNumSteps() const
{
    return this->numSteps;
}

IColumnMatrix *Numerical::PowerEigensystemSolver::getEigenvector() const
{
    return this->eigenvector;
}

Real Numerical::PowerEigensystemSolver::getEigenvalue() const
{
    return this->eigenvalue;
}

#endif //#if USE_NUMERICAL
