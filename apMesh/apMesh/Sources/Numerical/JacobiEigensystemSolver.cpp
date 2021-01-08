#include "Numerical/JacobiEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/RowMatrix.h"
#include "Numerical/IdentityMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/JacobiRotationMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::JacobiEigensystemSolver::JacobiEigensystemSolver() :
    Numerical::EigensystemSolver::EigensystemSolver()
{
    this->eigenvalues = NULL;
    this->eigenvectors = NULL;

    this->tolerance = this->TOLERANCE;

    this->maxSteps = this->MAX_STEPS;
    this->numSteps = 0;
}

Numerical::JacobiEigensystemSolver::~JacobiEigensystemSolver()
{

}

void Numerical::JacobiEigensystemSolver::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::JacobiEigensystemSolver::getTolerance() const
{
    return this->tolerance;
}

void Numerical::JacobiEigensystemSolver::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::JacobiEigensystemSolver::getMaxSteps() const
{
    return this->maxSteps;
}

UInt Numerical::JacobiEigensystemSolver::getNumSteps() const
{
    return this->numSteps;
}

RowMatrix *Numerical::JacobiEigensystemSolver::getEigenvalues() const
{
    return this->eigenvalues;
}

DenseMatrix *Numerical::JacobiEigensystemSolver::getEigenvectors() const
{
    return this->eigenvectors;
}

void Numerical::JacobiEigensystemSolver::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    UInt numRows = this->matrix->numRows();

    this->eigenvalues = new RowMatrix(numRows);
    this->eigenvectors = new DenseMatrix(numRows, numRows);

    IMatrix *aux = this->matrix->clone();

    IdentityMatrix identity(numRows);

    Numerical::IMatrix::copy(&identity, this->eigenvectors);

    for (UInt i = 0; i < numRows; i++)
    {
        this->eigenvalues->setElement(i, 0.0);
    }

    JacobiRotationMatrix jacobi;

    //std::cout << "aux:" << std::endl << aux->text() << std::endl;
    //std::cout << "eigenvalues:" << std::endl << this->eigenvalues->text() << std::endl;
    //std::cout << "eigenvectors:" << std::endl << this->eigenvectors->text() << std::endl;

    for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
    {
        for (UInt i = 0; i < numRows - 1; i++)
        {
            for (UInt j = i + 1; j < numRows; j++)
            {
                jacobi.reset(aux, i, j);

                jacobi.applyOn(aux);
                jacobi.postmultiplyOn(this->eigenvectors);

                //std::cout << "jacobi for " << i << ", " << j << ":" << std::endl << jacobi.text() << std::endl;
                //std::cout << "aux:" << std::endl << aux->text() << std::endl;
            }
        }

        Real maxdif = 0.0;

        for (UInt i = 0; i < numRows; i++)
        {
            Real dif = std::fabs(aux->getElement(i, i) - this->eigenvalues->getElement(i));

            if (dif > maxdif)
            {
                maxdif = dif;
            }

            this->eigenvalues->setElement(i, aux->getElement(i, i));
        }

        //std::cout << "eigenvalues:" << std::endl << this->eigenvalues->text() << std::endl;
        //std::cout << "eigenvectors:" << std::endl << this->eigenvectors->text() << std::endl;

        if (maxdif <= this->tolerance)
        {
            break;
        }
    }

    delete aux;
}

#endif //#if USE_NUMERICAL
