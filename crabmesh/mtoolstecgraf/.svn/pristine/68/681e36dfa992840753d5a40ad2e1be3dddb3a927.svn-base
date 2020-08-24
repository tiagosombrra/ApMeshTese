#include "Numerical/LinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

const UInt Numerical::LinearSystemSolver::MAX_STEPS = 1000;
const Real Numerical::LinearSystemSolver::TOLERANCE = 0.000001;

/*IColumnMatrix *Numerical::LinearSystemSolver::executeBackwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants)
{
    ColumnMatrix *result = new ColumnMatrix(constants->numRows());

    for (UInt i = constants->numRows(); i > 0; i--)
    {
        Real sum = 0.0;

        for (UInt j = i; j < constants->numRows(); j++)
        {
            sum += upperTriangular->getElement(i-1, j)*result->getElement(j);
        }

        result->setElement(i-1, (constants->getElement(i-1) - sum)/upperTriangular->getElement(i-1, i-1));
    }

    return result;
}

IColumnMatrix *Numerical::LinearSystemSolver::executeForwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants)
{
    ColumnMatrix *result = new ColumnMatrix(constants->numRows());

    for (UInt i = 0; i < constants->numRows(); i++)
    {
        Real sum = 0.0;

        for (UInt j = 0; j < i; j++)
        {
            sum += lowerTriangular->getElement(i, j)*result->getElement(j);
        }

        result->setElement(i, (constants->getElement(i) - sum)/lowerTriangular->getElement(i, i));
    }

    return result;
}*/

Numerical::LinearSystemSolver::LinearSystemSolver() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;
    this->constants = NULL;
    this->unknowns = NULL;

    this->tolerance = this->TOLERANCE;

    this->ok = true;
}

Numerical::LinearSystemSolver::~LinearSystemSolver()
{

}

void Numerical::LinearSystemSolver::setMatrix(const IMatrix *matrix)
{
    this->matrix = matrix;
}

const IMatrix *Numerical::LinearSystemSolver::getMatrix() const
{
    return this->matrix;
}

void Numerical::LinearSystemSolver::setConstants(const IColumnMatrix *constants)
{
    this->constants = constants;
}

const IColumnMatrix *Numerical::LinearSystemSolver::getConstants() const
{
    return this->constants;
}

void Numerical::LinearSystemSolver::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::LinearSystemSolver::getTolerance() const
{
    return this->tolerance;
}

bool Numerical::LinearSystemSolver::isOk() const
{
    return this->ok;
}

IColumnMatrix *Numerical::LinearSystemSolver::getUnknowns() const
{
    return this->unknowns;
}

IColumnMatrix *Numerical::LinearSystemSolver::backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants)
{
    ColumnMatrix *result = new ColumnMatrix(constants->numRows());

    this->backwardSubstitution(upperTriangular, constants, result);

    return result;
}

IColumnMatrix *Numerical::LinearSystemSolver::backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, bool guess)
{
    ColumnMatrix *result = new ColumnMatrix(constants->numRows());

    this->backwardSubstitution(upperTriangular, constants, guess, result);

    return result;
}

void Numerical::LinearSystemSolver::backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, IColumnMatrix *unknowns)
{
    /*for (UInt i = constants->numRows(); i > 0; i--)
    {
        Real sum = 0.0;

        for (UInt j = i; j < constants->numRows(); j++)
        {
            sum += upperTriangular->getElement(i-1, j)*unknowns->getElement(j);
        }

        unknowns->setElement(i-1, (constants->getElement(i-1) - sum)/upperTriangular->getElement(i-1, i-1));
    }*/

    this->backwardSubstitution(upperTriangular, constants, false, unknowns);
}

void Numerical::LinearSystemSolver::backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, bool guess, IColumnMatrix *unknowns)
{
    for (UInt i = constants->numRows(); i > 0; i--)
    {
        if ((guess) && (std::fabs(upperTriangular->getElement(i-1, i-1)) <= Data::getTolerance()))
        {
            unknowns->setElement(i-1, 1.0);

            continue;
        }

        Real sum = 0.0;

        for (UInt j = i; j < constants->numRows(); j++)
        {
            sum += upperTriangular->getElement(i-1, j)*unknowns->getElement(j);
        }

        unknowns->setElement(i-1, (constants->getElement(i-1) - sum)/upperTriangular->getElement(i-1, i-1));
    }
}

IColumnMatrix *Numerical::LinearSystemSolver::forwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants)
{
    ColumnMatrix *result = new ColumnMatrix(constants->numRows());

    this->forwardSubstitution(lowerTriangular, constants, result);

    return result;
}

void Numerical::LinearSystemSolver::forwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants, IColumnMatrix *unknowns)
{
    for (UInt i = 0; i < constants->numRows(); i++)
    {
        Real sum = 0.0;

        for (UInt j = 0; j < i; j++)
        {
            sum += lowerTriangular->getElement(i, j)*unknowns->getElement(j);
        }

        unknowns->setElement(i, (constants->getElement(i) - sum)/lowerTriangular->getElement(i, i));
    }
}

#endif //#if USE_NUMERICAL
