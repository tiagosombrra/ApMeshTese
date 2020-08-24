#include "Numerical/EigensystemSolver.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

const UInt Numerical::EigensystemSolver::MAX_STEPS = 1000;
const Real Numerical::EigensystemSolver::TOLERANCE = 0.000001;

Numerical::EigensystemSolver::EigensystemSolver() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;
}

Numerical::EigensystemSolver::~EigensystemSolver()
{

}

void Numerical::EigensystemSolver::setMatrix(IMatrix *matrix)
{
    this->matrix = matrix;
}

IMatrix *Numerical::EigensystemSolver::getMatrix() const
{
    return this->matrix;
}

#endif //#if USE_NUMERICAL
