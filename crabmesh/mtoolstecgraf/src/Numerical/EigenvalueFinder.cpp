#include "Numerical/EigenvalueFinder.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::EigenvalueFinder::EigenvalueFinder() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;
    this->eigenvector = NULL;
    this->eigenvalue = 0.0;
}

Numerical::EigenvalueFinder::~EigenvalueFinder()
{

}

void Numerical::EigenvalueFinder::setMatrix(const IMatrix *matrix)
{
    this->matrix = matrix;
}

const IMatrix *Numerical::EigenvalueFinder::getMatrix() const
{
    return this->matrix;
}

void Numerical::EigenvalueFinder::setEigenvector(const IColumnMatrix *eigenvector)
{
    this->eigenvector = eigenvector;
}

const IColumnMatrix *Numerical::EigenvalueFinder::getEigenvector() const
{
    return this->eigenvector;
}

Real Numerical::EigenvalueFinder::getEigenvalue() const
{
    return this->eigenvalue;
}

void Numerical::EigenvalueFinder::execute()
{
    if ((!this->matrix) || (!this->eigenvector) ||
        (!this->matrix->isSquare()) || (this->matrix->numCols() != this->eigenvector->numRows()))
    {
        return;
    }

    ColumnMatrix *result = new ColumnMatrix(this->eigenvector->numRows());

    IMatrix::multiply(this->matrix, this->eigenvector, result);

    Real s1 = 0.0, s2 = 0.0;

    for (UInt i = 0; i < this->eigenvector->numRows(); i++)
    {
        Real v1 = this->eigenvector->getElement(i);
        Real v2 = result->getElement(i);

        s1 += v1*v1;
        s2 += v2*v2;
    }

    this->eigenvalue = std::sqrt(s2)/std::sqrt(s1);

    if (this->eigenvector->getElement(0)*result->getElement(0) < 0.0)
    {
        this->eigenvalue = -this->eigenvalue;
    }
}

#endif //#if USE_NUMERICAL
