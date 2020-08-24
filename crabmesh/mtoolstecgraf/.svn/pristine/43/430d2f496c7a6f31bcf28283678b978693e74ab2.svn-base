#include "Numerical/SymmetricMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ISymmetricMatrix::~ISymmetricMatrix()
{

}

Real Numerical::ISymmetricMatrix::getElement(UInt i, UInt j) const
{
    return (i <= j) ? const_cast<ISymmetricMatrix *>(this)->element(i, j) : const_cast<ISymmetricMatrix *>(this)->element(j, i);
}

void Numerical::ISymmetricMatrix::setElement(UInt i, UInt j, Real element)
{
    if (i <= j)
    {
        this->element(i, j) = element;
    }
    else
    {
        this->element(j, i) = element;
    }
}

UInt Numerical::ISymmetricMatrix::numRows() const
{
    return this->numRows(this->numElements());
}

UInt Numerical::ISymmetricMatrix::numCols() const
{
    return this->numRows();
}

bool Numerical::ISymmetricMatrix::isSquare() const
{
    return true;
}

bool Numerical::ISymmetricMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

UInt Numerical::ISymmetricMatrix::numElements() const
{
    return this->numElements(this->numRows());
}

UInt Numerical::ISymmetricMatrix::numRows(UInt numElements)
{
    return (static_cast<UInt>(std::sqrt(1 + 8*numElements)) - 1)/2;
}

UInt Numerical::ISymmetricMatrix::numCols(UInt numElements)
{
    return ::Numerical::ISymmetricMatrix::numRows(numElements);
}

/*constexpr UInt Numerical::ISymmetricMatrix::numElements(UInt rows)
{
    return rows*(rows + 1)/2;
}*/

Real &Numerical::ISymmetricMatrix::element(UInt i, UInt j)
{
    return this->element(i*this->numRows() - this->numElements(i-1) + j - i);
}

Numerical::SymmetricMatrix::SymmetricMatrix(UInt rows) :
    Numerical::ISymmetricMatrix::ISymmetricMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(1, Numerical::ISymmetricMatrix::numElements(rows))
{

}

Numerical::SymmetricMatrix::SymmetricMatrix(const SymmetricMatrix &m) :
    Numerical::ISymmetricMatrix::ISymmetricMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::SymmetricMatrix::~SymmetricMatrix()
{

}

UInt Numerical::SymmetricMatrix::numElements() const
{
    return Numerical::TDMatrix<Real>::cols;
}

void Numerical::SymmetricMatrix::resize(UInt rows)
{
    this->reallocate(1, Numerical::ISymmetricMatrix::numElements(rows));
}

Real &Numerical::SymmetricMatrix::element(UInt i)
{
    return Numerical::TDMatrix<Real>::element(0, i);
}

IMatrix *Numerical::SymmetricMatrix::clone() const
{
    return new SymmetricMatrix(*this);
}

#endif //#if USE_NUMERICAL
