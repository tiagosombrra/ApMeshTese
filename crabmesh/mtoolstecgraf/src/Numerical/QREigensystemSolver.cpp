#include "Numerical/QREigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/IdentityMatrix.h"
#include "Numerical/RowMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/HouseholderMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::QREigensystemSolver::QREigensystemSolver() :
    Numerical::EigensystemSolver::EigensystemSolver()
{
    this->eigenvalues = NULL;

    this->numSteps = 0;
    this->maxSteps = this->MAX_STEPS;

    this->tolerance = this->TOLERANCE;
}

Numerical::QREigensystemSolver::~QREigensystemSolver()
{

}

void Numerical::QREigensystemSolver::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::QREigensystemSolver::getTolerance() const
{
    return this->tolerance;
}

void Numerical::QREigensystemSolver::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::QREigensystemSolver::getMaxSteps() const
{
    return this->maxSteps;
}

UInt Numerical::QREigensystemSolver::getNumSteps() const
{
    return this->numSteps;
}

RowMatrix *Numerical::QREigensystemSolver::getEigenvalues() const
{
    return this->eigenvalues;
}

void Numerical::QREigensystemSolver::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    //std::cout << "original:" << std::endl;
    //std::cout << this->matrix->text() << std::endl;

    UInt numRows = this->matrix->numRows();

    this->eigenvalues = new RowMatrix(numRows);

    for (UInt i = 0; i < numRows; i++)
    {
        this->eigenvalues->setElement(i, 0.0);
    }

    DenseMatrix aux(numRows, numRows);
    DenseMatrix r(numRows, numRows);
    DenseMatrix q(numRows, numRows);

    IdentityMatrix identity(numRows);

    Numerical::IMatrix::copy(this->matrix, &r);
    Numerical::IMatrix::copy(identity, q);

    HouseholderMatrix householder;

    for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
    {
        //std::cout << "STEP " << this->numSteps << ":" << std::endl;

        for (UInt row = 0; row < numRows - 1; row++)
        {
            //std::cout << "for col " << row << ":" << std::endl;

            householder.reset(&r, row, row, false, true);

            Numerical::IMatrix::multiply(householder, r, aux, row, row, row, row, row, row, numRows - row, numRows - row, numRows - row);
            //Numerical::IMatrix::multiply(householder, r, aux);
            Numerical::IMatrix::copy(aux, r, row, numRows, row, numRows);
            //Numerical::IMatrix::copy(aux, r);

            for (UInt i = row + 1; i < numRows; i++)
            {
                if (std::fabs(r.getElement(i, row)) <= this->tolerance)
                {
                    r.setElement(i, row, 0.0);
                }

                if (std::fabs(r.getElement(row, i)) <= this->tolerance)
                {
                    r.setElement(row, i, 0.0);
                }
            }

            Numerical::IMatrix::multiply(q, householder, aux, 0, row, row, row, 0, row, numRows, numRows - row, numRows - row);
            //Numerical::IMatrix::multiply(q, householder, aux);
            Numerical::IMatrix::copy(aux, q, 0, numRows, row, numRows);
            //Numerical::IMatrix::copy(aux, q);

            //std::cout << householder.text() << std::endl;
            //std::cout << q.text() << std::endl;
            //std::cout << r.text() << std::endl;
        }

        Real maxdif = 0.0;

        for (UInt i = 0; i < numRows; i++)
        {
            Real dif = std::fabs(r.getElement(i, i) - this->eigenvalues->getElement(i));

            if (dif > maxdif)
            {
                maxdif = dif;
            }

            this->eigenvalues->setElement(i, r.getElement(i, i));
        }

        //Numerical::IMatrix::multiply(q, r, aux);
        //std::cout << "QR:" << std::endl;
        //std::cout << aux.text() << std::endl;

        if (maxdif <= this->tolerance)
        {
            break;
        }

        Numerical::IMatrix::multiply(r, q, aux);

        Numerical::IMatrix::copy(aux, r);
        Numerical::IMatrix::copy(identity, q);
    }
}

#endif //#if USE_NUMERICAL
