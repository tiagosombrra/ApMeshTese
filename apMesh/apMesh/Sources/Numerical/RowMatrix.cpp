#include "Numerical/RowMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::IRowMatrix::~IRowMatrix()
{

}

UInt Numerical::IRowMatrix::numRows() const
{
    return 1;
}

Real Numerical::IRowMatrix::getElement(UInt i) const
{
    return const_cast<IRowMatrix *>(this)->element(i);
}

void Numerical::IRowMatrix::setElement(UInt i, Real element)
{
    this->element(i) = element;
}

bool Numerical::IRowMatrix::isSquare() const
{
    return (this->numCols() == 1);
}

bool Numerical::IRowMatrix::isSymmetric() const
{
    return (this->numCols() == 1);
}

bool Numerical::IRowMatrix::isTriangular() const
{
    return true;
}

bool Numerical::IRowMatrix::isLowerTriangular() const
{
    return true;
}

bool Numerical::IRowMatrix::isUpperTriangular() const
{
    return true;
}

bool Numerical::IRowMatrix::isDiagonal() const
{
    return true;
}

bool Numerical::IRowMatrix::isColumn() const
{
    return (this->numCols() == 1);
}

bool Numerical::IRowMatrix::isRow() const
{
    return true;
}

bool Numerical::IRowMatrix::isBand() const
{
    return false;
}

bool Numerical::IRowMatrix::isIdentity() const
{
    return ((this->numCols() == 1) && (this->getElement(0) == 1.0));
}

UInt Numerical::IRowMatrix::numElements() const
{
    return this->numCols();
}

UInt Numerical::IRowMatrix::lowerBandwidth() const
{
    return 0;
}

UInt Numerical::IRowMatrix::upperBandwidth() const
{
    return 0;
}

Real Numerical::IRowMatrix::getElement(UInt /*i*/, UInt j) const
{
    return this->getElement(j);
}

void Numerical::IRowMatrix::setElement(UInt /*i*/, UInt j, Real element)
{
    this->setElement(j, element);
}

Real &Numerical::IRowMatrix::element(UInt /*i*/, UInt j)
{
    return this->element(j);
}

Numerical::RowMatrix::RowMatrix(UInt cols) :
    Numerical::IRowMatrix::IRowMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(1, cols)
{

}

Numerical::RowMatrix::RowMatrix(const RowMatrix &m) :
    Numerical::IRowMatrix::IRowMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::RowMatrix::~RowMatrix()
{

}

UInt Numerical::RowMatrix::numCols() const
{
    return this->cols;
}

void Numerical::RowMatrix::resize(UInt cols)
{
    this->reallocate(1, cols);
}

Real &Numerical::RowMatrix::element(UInt i)
{
    return this->TDMatrix<Real>::element(0, i);
}

IMatrix *Numerical::RowMatrix::clone() const
{
    return new RowMatrix(*this);
}

#endif //#if USE_NUMERICAL
