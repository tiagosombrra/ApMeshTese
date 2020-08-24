#include "Numerical/DiagonalMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::DiagonalMatrix::DiagonalMatrix(UInt rows) :
    Numerical::BandMatrix::BandMatrix(rows, 0, 0)
{

}

Numerical::DiagonalMatrix::DiagonalMatrix(const DiagonalMatrix &m) :
    Numerical::BandMatrix::BandMatrix(m)
{

}

Numerical::DiagonalMatrix::~DiagonalMatrix()
{

}

void Numerical::DiagonalMatrix::resize(UInt rows)
{
    this->resize(rows, 0, 0);
}

IMatrix *Numerical::DiagonalMatrix::clone() const
{
    return new DiagonalMatrix(*this);
}

#endif //#if USE_NUMERICAL
