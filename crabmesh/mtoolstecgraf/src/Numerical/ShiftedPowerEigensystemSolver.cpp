#include "Numerical/ShiftedPowerEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::ShiftedPowerEigensystemSolver::ShiftedPowerEigensystemSolver() :
    Numerical::InversePowerEigensystemSolver::InversePowerEigensystemSolver()
{
    this->shift = 0.0;
}

Numerical::ShiftedPowerEigensystemSolver::~ShiftedPowerEigensystemSolver()
{

}

void Numerical::ShiftedPowerEigensystemSolver::setShift(Real shift)
{
    this->shift = shift;
}

Real Numerical::ShiftedPowerEigensystemSolver::getShift() const
{
    return this->shift;
}

void Numerical::ShiftedPowerEigensystemSolver::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    Numerical::IMatrix *original = this->matrix;

    if (this->copy)
    {
        this->matrix = this->matrix->clone();
    }

    for (UInt i = 0; i < this->matrix->numCols(); i++)
    {
        this->matrix->setElement(i, i, this->matrix->getElement(i, i) - this->shift);
    }

    InversePowerEigensystemSolver::execute();

    this->eigenvalue += this->shift;

    if (original != this->matrix)
    {
        delete this->matrix;

        this->matrix = original;
    }
}

#endif //#if USE_NUMERICAL
