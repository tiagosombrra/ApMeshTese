#include "Numerical/RegularPowerEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::RegularPowerEigensystemSolver::RegularPowerEigensystemSolver() :
    Numerical::PowerEigensystemSolver::PowerEigensystemSolver()
{

}

Numerical::RegularPowerEigensystemSolver::~RegularPowerEigensystemSolver()
{

}

void Numerical::RegularPowerEigensystemSolver::execute()
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

    IColumnMatrix *nextEigenvector = static_cast<IColumnMatrix *>(this->eigenvector->clone());

    for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
    {
        Numerical::IMatrix::multiply(this->matrix, this->eigenvector, nextEigenvector);

        Real num, den;

        Numerical::IMatrix::multiply(nextEigenvector, nextEigenvector, num);
        Numerical::IMatrix::multiply(nextEigenvector, this->eigenvector, den);
        Real nextEigenvalue = num/den;

        Numerical::IMatrix::multiply(nextEigenvector, 1.0/nextEigenvalue, nextEigenvector);

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

    delete nextEigenvector;
}

#endif //#if USE_NUMERICAL
