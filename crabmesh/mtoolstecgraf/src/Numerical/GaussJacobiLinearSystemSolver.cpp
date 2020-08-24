#include "Numerical/GaussJacobiLinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::GaussJacobiLinearSystemSolver::GaussJacobiLinearSystemSolver() :
    Numerical::FixedPointLinearSystemSolver::FixedPointLinearSystemSolver()
{

}

Numerical::GaussJacobiLinearSystemSolver::~GaussJacobiLinearSystemSolver()
{

}

//void Numerical::GaussJacobiLinearSystemSolver::next(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, IColumnMatrix *next) const
void Numerical::GaussJacobiLinearSystemSolver::next(const IColumnMatrix *guess, IColumnMatrix *next) const
{
    for (UInt i = 0; i < next->numRows(); i++)
    {
        Real sum = 0.0;

        for (UInt j = 0; j < this->matrix->numCols(); j++)
        {
            if (i == j)
            {
                continue;
            }

            sum += this->matrix->getElement(i, j)*guess->getElement(j);
        }

        Real val = (this->constants->getElement(i) - sum)/this->matrix->getElement(i, i);

        next->setElement(i, val);
    }
}

#endif //#if USE_NUMERICAL
