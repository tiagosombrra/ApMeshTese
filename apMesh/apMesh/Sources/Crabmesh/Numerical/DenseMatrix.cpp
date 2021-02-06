#include "../../../Headers/Crabmesh/Numerical/DenseMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::DenseMatrix::DenseMatrix(UInt rows, UInt cols) :
    Numerical::IDenseMatrix::IDenseMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(rows, cols)
{

}

Numerical::DenseMatrix::DenseMatrix(const DenseMatrix &m) :
    Numerical::IDenseMatrix::IDenseMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::IDenseMatrix::~IDenseMatrix()
{

}

Numerical::DenseMatrix::~DenseMatrix()
{

}

UInt Numerical::DenseMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::DenseMatrix::numCols() const
{
    return this->cols;
}

void Numerical::DenseMatrix::resize(UInt rows, UInt cols)
{
    this->reallocate(rows, cols);
}

Real &Numerical::DenseMatrix::element(UInt i, UInt j)
{
    return this->TDMatrix<Real>::element(i, j);
}

IMatrix *Numerical::DenseMatrix::clone() const
{
    return new DenseMatrix(*this);
}

#endif //#if USE_NUMERICAL
