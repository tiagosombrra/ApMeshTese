#include "../../../Headers/Crabmesh/Numerical/ColumnMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::IColumnMatrix::~IColumnMatrix()
{

}

UInt Numerical::IColumnMatrix::numCols() const
{
    return 1;
}

Real Numerical::IColumnMatrix::getElement(UInt i) const
{
    return const_cast<IColumnMatrix *>(this)->element(i);
}

void Numerical::IColumnMatrix::setElement(UInt i, Real element)
{
    this->element(i) = element;
}

bool Numerical::IColumnMatrix::isSquare() const
{
    return (this->numRows() == 1);
}

bool Numerical::IColumnMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

bool Numerical::IColumnMatrix::isTriangular() const
{
    return true;
}

bool Numerical::IColumnMatrix::isLowerTriangular() const
{
    return true;
}

bool Numerical::IColumnMatrix::isUpperTriangular() const
{
    return true;
}

bool Numerical::IColumnMatrix::isDiagonal() const
{
    return true;
}

bool Numerical::IColumnMatrix::isColumn() const
{
    return true;
}

bool Numerical::IColumnMatrix::isRow() const
{
    return (this->numRows() == 1);
}

bool Numerical::IColumnMatrix::isBand() const
{
    return false;
}

bool Numerical::IColumnMatrix::isIdentity() const
{
    return ((this->numRows() == 1) && (this->getElement(0) == 1.0));
}

UInt Numerical::IColumnMatrix::numElements() const
{
    return this->numRows();
}

UInt Numerical::IColumnMatrix::lowerBandwidth() const
{
    return 0;
}

UInt Numerical::IColumnMatrix::upperBandwidth() const
{
    return 0;
}

Real Numerical::IColumnMatrix::getElement(UInt i, UInt /*j*/) const
{
    return this->getElement(i);
}

void Numerical::IColumnMatrix::setElement(UInt i, UInt /*j*/, Real element)
{
    this->setElement(i, element);
}

Real &Numerical::IColumnMatrix::element(UInt i, UInt /*j*/)
{
    return this->element(i);
}

Numerical::ColumnMatrix::ColumnMatrix(UInt rows) :
    Numerical::IColumnMatrix::IColumnMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(rows, 1)
{

}

Numerical::ColumnMatrix::ColumnMatrix(const ColumnMatrix &m) :
    Numerical::IColumnMatrix::IColumnMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::ColumnMatrix::~ColumnMatrix()
{

}

UInt Numerical::ColumnMatrix::numRows() const
{
    return this->rows;
}

void Numerical::ColumnMatrix::resize(UInt rows)
{
    this->reallocate(rows, 1);
}

Real &Numerical::ColumnMatrix::element(UInt i)
{
    return this->TDMatrix<Real>::element(i, 0);
}

IMatrix *Numerical::ColumnMatrix::clone() const
{
    return new ColumnMatrix(*this);
}

#endif //#if USE_NUMERICAL
