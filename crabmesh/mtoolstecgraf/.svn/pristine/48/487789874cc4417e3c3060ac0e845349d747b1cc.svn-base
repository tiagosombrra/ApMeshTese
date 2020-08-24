#include "Numerical/ZeroMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ZeroMatrix::ZeroMatrix(UInt rows) :
    Numerical::IMatrix::IMatrix()
{
    this->rows = this->cols = rows;
}

Numerical::ZeroMatrix::ZeroMatrix(UInt rows, UInt cols) :
    Numerical::IMatrix::IMatrix()
{
    this->rows = rows;
    this->cols = cols;
}

Numerical::ZeroMatrix::ZeroMatrix(const ZeroMatrix &m) :
    Numerical::IMatrix::IMatrix()
{
    this->rows = m.rows;
    this->cols = m.cols;
}

Numerical::ZeroMatrix::~ZeroMatrix()
{

}

UInt Numerical::ZeroMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::ZeroMatrix::numCols() const
{
    return this->cols;
}

Real Numerical::ZeroMatrix::getElement(UInt /*i*/, UInt /*j*/) const
{
    return 0.0;
}

bool Numerical::ZeroMatrix::isSymmetric() const
{
    return this->isSquare();
}

bool Numerical::ZeroMatrix::isTriangular() const
{
    return this->isSquare();
}

bool Numerical::ZeroMatrix::isLowerTriangular() const
{
    return this->isSquare();
}

bool Numerical::ZeroMatrix::isUpperTriangular() const
{
    return this->isSquare();
}

bool Numerical::ZeroMatrix::isDiagonal() const
{
    return this->isSquare();
}

bool Numerical::ZeroMatrix::isBand() const
{
    return true;
}

bool Numerical::ZeroMatrix::isIdentity() const
{
    return false;
}

bool Numerical::ZeroMatrix::isZero() const
{
    return false;
}

UInt Numerical::ZeroMatrix::numElements() const
{
    return 0;
}

UInt Numerical::ZeroMatrix::lowerBandwidth() const
{
    return 0;
}

UInt Numerical::ZeroMatrix::upperBandwidth() const
{
    return 0;
}

void Numerical::ZeroMatrix::setElement(UInt /*i*/, UInt /*j*/, Real /*element*/)
{

}

Real &Numerical::ZeroMatrix::element(UInt /*i*/, UInt /*j*/)
{
    static Real zero = 0.0;

    return zero;
}

IMatrix *Numerical::ZeroMatrix::clone() const
{
    return new ZeroMatrix(*this);
}

#endif //#if USE_NUMERICAL
