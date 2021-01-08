#include "Numerical/LUMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ILUMatrix::ILUMatrix(const IMatrix *original)
{
    this->setOriginal(original);
    this->lower = NULL;
    this->upper = NULL;
}

Numerical::ILUMatrix::~ILUMatrix()
{
    if (this->lower)
    {
        delete this->lower;
    }

    if (this->upper)
    {
        delete this->upper;
    }
}

void Numerical::ILUMatrix::setOriginal(const IMatrix *original)
{
    this->original = original;
}

const IMatrix *Numerical::ILUMatrix::getOriginal() const
{
    return this->original;
}

UInt Numerical::ILUMatrix::numRows() const
{
    return this->original->numRows();
}

UInt Numerical::ILUMatrix::numCols() const
{
    return this->original->numCols();
}

bool Numerical::ILUMatrix::isSquare() const
{
    return true;
}

bool Numerical::ILUMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

bool Numerical::ILUMatrix::isTriangular() const
{
    return false;
}

bool Numerical::ILUMatrix::isLowerTriangular() const
{
    return false;
}

bool Numerical::ILUMatrix::isUpperTriangular() const
{
    return false;
}

bool Numerical::ILUMatrix::isDiagonal() const
{
    return (this->numRows() == 1);
}

bool Numerical::ILUMatrix::isColumn() const
{
    return (this->numRows() == 1);
}

bool Numerical::ILUMatrix::isRow() const
{
    return (this->numRows() == 1);
}

bool Numerical::ILUMatrix::isBand() const
{
    return false;
}

bool Numerical::ILUMatrix::isIdentity() const
{
    return false;
}

bool Numerical::ILUMatrix::isZero() const
{
    return false;
}

UInt Numerical::ILUMatrix::lowerBandwidth() const
{
    return this->numRows();
}

UInt Numerical::ILUMatrix::upperBandwidth() const
{
    return this->numRows();
}

const LMatrix *Numerical::ILUMatrix::makeLower()
{
    if (this->lower)
    {
        return this->lower;
    }

    this->lower = new Numerical::LMatrix(this);

    return this->lower;
}

const LMatrix *Numerical::ILUMatrix::getLower() const
{
    return this->lower;
}

const UMatrix *Numerical::ILUMatrix::makeUpper()
{
    if (this->upper)
    {
        return this->upper;
    }

    this->upper = new Numerical::UMatrix(this);

    return this->upper;
}

const UMatrix *Numerical::ILUMatrix::getUpper() const
{
    return this->upper;
}

Numerical::LUMatrix::LUMatrix(const IMatrix *original) :
    Numerical::ILUMatrix::ILUMatrix(original),
    Numerical::TDMatrix<Real>::TDMatrix(original->numRows(), original->numCols())
{

}

Numerical::LUMatrix::LUMatrix(const LUMatrix &m) :
    Numerical::ILUMatrix::ILUMatrix(m.original),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{

}

Numerical::LUMatrix::~LUMatrix()
{

}

void Numerical::LUMatrix::resize(UInt rows, UInt cols)
{
    this->reallocate(rows, cols);
}

Real &Numerical::LUMatrix::element(UInt i, UInt j)
{
    return this->TDMatrix<Real>::element(i, j);
}

IMatrix *Numerical::LUMatrix::clone() const
{
    return new LUMatrix(*this);
}

Numerical::LMatrix::LMatrix(ILUMatrix *lu) :
    Numerical::ILowerTriangularMatrix()
{
    this->lu = lu;
}

Numerical::LMatrix::LMatrix(const LMatrix &m) :
    Numerical::ILowerTriangularMatrix()
{
    this->lu = m.lu;
}

Numerical::LMatrix::~LMatrix()
{

}

Real Numerical::LMatrix::getElement(UInt i, UInt j) const
{
    return (i == j) ? 1.0 : ((i > j) ? const_cast<LMatrix *>(this)->element(i, j) : 0.0);
}

bool Numerical::LMatrix::isSquare() const
{
    return true;
}

bool Numerical::LMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

bool Numerical::LMatrix::isTriangular() const
{
    return true;
}

bool Numerical::LMatrix::isLowerTriangular() const
{
    return true;
}

bool Numerical::LMatrix::isUpperTriangular() const
{
    return false;
}

UInt Numerical::LMatrix::numRows() const
{
    return this->lu->numRows();
}

UInt Numerical::LMatrix::numCols() const
{
    return this->lu->numCols();
}

UInt Numerical::LMatrix::numElements() const
{
    return 0;
}

ILUMatrix *Numerical::LMatrix::getLUMatrix() const
{
    return this->lu;
}

const IMatrix *Numerical::LMatrix::getOriginal() const
{
    return this->lu->getOriginal();
}

void Numerical::LMatrix::setElement(UInt /*i*/, UInt /*j*/, Real /*val*/)
{

}

Real &Numerical::LMatrix::element(UInt i, UInt j)
{
    return this->lu->element(i, j);
}

Real &Numerical::LMatrix::element(UInt i)
{
    UInt i2 = this->Numerical::ILowerTriangularMatrix::numRows(i);

    UInt j = i - this->Numerical::ILowerTriangularMatrix::numElements2(i2);

    return this->lu->element(i2, j);
}

IMatrix *Numerical::LMatrix::clone() const
{
    return new LMatrix(*this);
}

Numerical::UMatrix::UMatrix(ILUMatrix *lu) :
    Numerical::IUpperTriangularMatrix()
{
    this->lu = lu;
}

Numerical::UMatrix::UMatrix(const UMatrix &m) :
    Numerical::IUpperTriangularMatrix()
{
    this->lu = m.lu;
}

Numerical::UMatrix::~UMatrix()
{

}

Real Numerical::UMatrix::getElement(UInt i, UInt j) const
{
    return (i <= j) ? const_cast<UMatrix *>(this)->element(i, j) : 0.0;
}

bool Numerical::UMatrix::isSquare() const
{
    return true;
}

bool Numerical::UMatrix::isSymmetric() const
{
    return (this->numRows() == 1);
}

bool Numerical::UMatrix::isTriangular() const
{
    return true;
}

bool Numerical::UMatrix::isLowerTriangular() const
{
    return false;
}

bool Numerical::UMatrix::isUpperTriangular() const
{
    return true;
}

UInt Numerical::UMatrix::numRows() const
{
    return this->lu->numRows();
}

UInt Numerical::UMatrix::numCols() const
{
    return this->lu->numCols();
}

UInt Numerical::UMatrix::numElements() const
{
    return 0;
}

ILUMatrix *Numerical::UMatrix::getLUMatrix() const
{
    return this->lu;
}

const IMatrix *Numerical::UMatrix::getOriginal() const
{
    return this->lu->getOriginal();
}

void Numerical::UMatrix::setElement(UInt /*i*/, UInt /*j*/, Real /*val*/)
{

}

Real &Numerical::UMatrix::element(UInt i, UInt j)
{
    return this->lu->element(i, j);
}

Real &Numerical::UMatrix::element(UInt i)
{
    UInt numElems = this->Numerical::IUpperTriangularMatrix::numElements2(this->numRows());
    UInt numRows = this->Numerical::IUpperTriangularMatrix::numRows(numElems - 1 - i);

    UInt i2 = this->numRows() - numRows - 1;
    UInt j = i - i2*(numRows - 1) + this->Numerical::IUpperTriangularMatrix::numElements2(i2 - 1);

    return this->lu->element(i2, j);
}

IMatrix *Numerical::UMatrix::clone() const
{
    return new UMatrix(*this);
}

#endif //#if USE_NUMERICAL
