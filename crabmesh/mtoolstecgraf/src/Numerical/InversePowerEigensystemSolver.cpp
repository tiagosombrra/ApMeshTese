#include "Numerical/InversePowerEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"
#include "Numerical/LUDecompositionLinearSystemSolver.h"

using namespace Data;
using namespace Numerical;

Numerical::InversePowerEigensystemSolver::InversePowerEigensystemSolver() :
    Numerical::PowerEigensystemSolver::PowerEigensystemSolver()
{

}

Numerical::InversePowerEigensystemSolver::~InversePowerEigensystemSolver()
{

}

void Numerical::InversePowerEigensystemSolver::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    if (this->guess)
    {
        if (this->matrix->numCols() != this->guess->numRows())
        {
            return;
        }

        if (this->copy)
        {
            this->eigenvector = static_cast<IColumnMatrix *>(this->guess->clone());
        }
        else
        {
            this->eigenvector = this->guess;
        }
    }
    else
    {
        this->eigenvector = new ColumnMatrix(this->matrix->numCols());

        for (UInt i = 0; i < this->eigenvector->numRows(); i++)
        {
            this->eigenvector->setElement(i, 1.0);
        }
    }

    this->eigenvalue = 1.0;

    //IColumnMatrix *nextEigenvector = static_cast<IColumnMatrix *>(this->eigenvector->clone());
    IColumnMatrix *nextEigenvector = NULL;

    LUDecompositionLinearSystemSolver lusolver;

    lusolver.setMatrix(this->matrix);

    lusolver.execute();

    for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
    {
        //Numerical::IMatrix::multiply(this->matrix, this->eigenvector, nextEigenvector);

        lusolver.setConstants(this->eigenvector);

        lusolver.solve();

        if (nextEigenvector)
        {
            delete nextEigenvector;
        }

        nextEigenvector = lusolver.getUnknowns();

        Real num, den;

        Numerical::IMatrix::multiply(nextEigenvector, this->eigenvector, num);
        Numerical::IMatrix::multiply(nextEigenvector, nextEigenvector, den);
        Real nextEigenvalue = num/den;

        Numerical::IMatrix::multiply(nextEigenvector, nextEigenvalue, nextEigenvector);

        Real dif = std::fabs(nextEigenvalue - this->eigenvalue);

        this->eigenvalue = nextEigenvalue;

        std::swap(this->eigenvector, nextEigenvector);

        if (this->testingEigenvector)
        {
            Real maxdif = 0.0;

            for (UInt i = 0; i < this->eigenvector->numRows(); i++)
            {
                Real d = std::fabs(this->eigenvector->getElement(i) - nextEigenvector->getElement(i));

                if (d > maxdif)
                {
                    maxdif = d;
                }
            }

            if ((!this->testingEigenvalue) && (maxdif <= this->tolerance))
            {
                break;
            }

            if (maxdif > dif)
            {
                dif = maxdif;
            }
        }

        if ((this->testingEigenvalue) && (dif <= this->tolerance))
        {
            break;
        }
    }

    Real size = 0.0;

    for (UInt i = 0; i < this->eigenvector->numRows(); i++)
    {
        Real v = this->eigenvector->getElement(i);

        size += v*v;
    }

    size = std::sqrt(size);

    for (UInt i = 0; i < this->eigenvector->numRows(); i++)
    {
        this->eigenvector->setElement(i, this->eigenvector->getElement(i)/size);
    }
}

#endif //#if USE_NUMERICAL
