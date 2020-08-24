#include "Numerical/IdentityMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

//const Real Numerical::IdentityMatrix::one = 1.0;

Numerical::IdentityMatrix::IdentityMatrix(UInt rows) :
    Numerical::IMatrix::IMatrix()
{
    this->rows = rows;
}

Numerical::IdentityMatrix::IdentityMatrix(const IdentityMatrix &m) :
    Numerical::IMatrix::IMatrix()
{
    this->rows = m.rows;
}

Numerical::IdentityMatrix::~IdentityMatrix()
{

}

UInt Numerical::IdentityMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::IdentityMatrix::numCols() const
{
    return this->rows;
}

Real Numerical::IdentityMatrix::getElement(UInt i, UInt j) const
{
    return (i == j) ? 1.0 : 0.0;
}

bool Numerical::IdentityMatrix::isSquare() const
{
    return true;
}

bool Numerical::IdentityMatrix::isSymmetric() const
{
    return true;
}

bool Numerical::IdentityMatrix::isTriangular() const
{
    return true;
}

bool Numerical::IdentityMatrix::isLowerTriangular() const
{
    return true;
}

bool Numerical::IdentityMatrix::isUpperTriangular() const
{
    return true;
}

bool Numerical::IdentityMatrix::isDiagonal() const
{
    return true;
}

bool Numerical::IdentityMatrix::isBand() const
{
    return true;
}

bool Numerical::IdentityMatrix::isIdentity() const
{
    return true;
}

bool Numerical::IdentityMatrix::isZero() const
{
    return false;
}

UInt Numerical::IdentityMatrix::numElements() const
{
    return 0;
}

UInt Numerical::IdentityMatrix::lowerBandwidth() const
{
    return 0;
}

UInt Numerical::IdentityMatrix::upperBandwidth() const
{
    return 0;
}

void Numerical::IdentityMatrix::setElement(UInt /*i*/, UInt /*j*/, Real /*element*/)
{

}

Real &Numerical::IdentityMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    return (i == j) ? one : zero;
}

IMatrix *Numerical::IdentityMatrix::clone() const
{
    return new IdentityMatrix(*this);
}

#endif //#if USE_NUMERICAL
