#include "Numerical/TriangularMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ITriangularMatrix::~ITriangularMatrix()
{

}

bool Numerical::ITriangularMatrix::isSquare() const
{
    return true;
}

bool Numerical::ITriangularMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

bool Numerical::ITriangularMatrix::isTriangular() const
{
    return true;
}

UInt Numerical::ITriangularMatrix::numRows() const
{
    return this->numRows(this->numElements());
}

UInt Numerical::ITriangularMatrix::numCols() const
{
    return this->numRows();
}

UInt Numerical::ITriangularMatrix::numElements() const
{
    return this->numElements2(this->numRows());
}

UInt Numerical::ITriangularMatrix::numRows(UInt numElements)
{
    return (static_cast<UInt>(std::sqrt(1 + 8*numElements)) - 1)/2;
}

UInt Numerical::ITriangularMatrix::numCols(UInt numElements)
{
    return ::Numerical::ITriangularMatrix::numRows(numElements);
}

Numerical::ILowerTriangularMatrix::~ILowerTriangularMatrix()
{

}

Real Numerical::ILowerTriangularMatrix::getElement(UInt i, UInt j) const
{
    return (i >= j) ? const_cast<ILowerTriangularMatrix *>(this)->element(i, j) : 0.0;
}

void Numerical::ILowerTriangularMatrix::setElement(UInt i, UInt j, Real element)
{
    if (i >= j)
    {
        this->element(i, j) = element;
    }
}

bool Numerical::ILowerTriangularMatrix::isLowerTriangular() const
{
    return true;
}

Real &Numerical::ILowerTriangularMatrix::element(UInt i, UInt j)
{
    return this->element(this->numElements2(i) + j);
}

Numerical::IUpperTriangularMatrix::~IUpperTriangularMatrix()
{

}

Real Numerical::IUpperTriangularMatrix::getElement(UInt i, UInt j) const
{
    return (i <= j) ? const_cast<IUpperTriangularMatrix *>(this)->element(i, j) : 0.0;
}

void Numerical::IUpperTriangularMatrix::setElement(UInt i, UInt j, Real element)
{
    if (i <= j)
    {
        this->element(i, j) = element;
    }
}

bool Numerical::IUpperTriangularMatrix::isUpperTriangular() const
{
    return true;
}

Real &Numerical::IUpperTriangularMatrix::element(UInt i, UInt j)
{
    return this->element(i*this->numRows() - this->numElements2(i-1) + j - i);
}

Numerical::LowerTriangularMatrix::LowerTriangularMatrix(UInt rows) :
    Numerical::ILowerTriangularMatrix::ILowerTriangularMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(1, Numerical::ITriangularMatrix::numElements2(rows))
{

}

Numerical::LowerTriangularMatrix::LowerTriangularMatrix(const LowerTriangularMatrix &m) :
    Numerical::ILowerTriangularMatrix::ILowerTriangularMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::LowerTriangularMatrix::~LowerTriangularMatrix()
{

}

UInt Numerical::LowerTriangularMatrix::numElements() const
{
    return Numerical::TDMatrix<Real>::cols;
}

void Numerical::LowerTriangularMatrix::resize(UInt rows)
{
    this->reallocate(1, Numerical::ITriangularMatrix::numElements2(rows));
}

Real &Numerical::LowerTriangularMatrix::element(UInt i)
{
    return Numerical::TDMatrix<Real>::element(0, i);
}

IMatrix *Numerical::LowerTriangularMatrix::clone() const
{
    return new LowerTriangularMatrix(*this);
}

Numerical::UpperTriangularMatrix::UpperTriangularMatrix(UInt rows) :
    Numerical::IUpperTriangularMatrix::IUpperTriangularMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(1, Numerical::ITriangularMatrix::numElements2(rows))
{

}

Numerical::UpperTriangularMatrix::UpperTriangularMatrix(const UpperTriangularMatrix &m) :
    Numerical::IUpperTriangularMatrix::IUpperTriangularMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::UpperTriangularMatrix::~UpperTriangularMatrix()
{

}

UInt Numerical::UpperTriangularMatrix::numElements() const
{
    return Numerical::TDMatrix<Real>::cols;
}

void Numerical::UpperTriangularMatrix::resize(UInt rows)
{
    this->reallocate(1, Numerical::ITriangularMatrix::numElements2(rows));
}

Real &Numerical::UpperTriangularMatrix::element(UInt i)
{
    return Numerical::TDMatrix<Real>::element(0, i);
}

IMatrix *Numerical::UpperTriangularMatrix::clone() const
{
    return new UpperTriangularMatrix(*this);
}

#endif //#if USE_NUMERICAL
