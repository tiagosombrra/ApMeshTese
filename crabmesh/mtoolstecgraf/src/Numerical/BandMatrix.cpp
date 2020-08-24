#include "Numerical/BandMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::IBandMatrix::~IBandMatrix()
{

}

Real Numerical::IBandMatrix::getElement(UInt i, UInt j) const
{
    UInt mincol = this->minCol(i);
    UInt maxcol = this->maxCol(i);

    return ((mincol <= j) && (j < maxcol)) ? const_cast<IBandMatrix *>(this)->element(i, j - mincol) : 0.0;
}

void Numerical::IBandMatrix::setElement(UInt i, UInt j, Real element)
{
    UInt mincol = this->minCol(i);
    UInt maxcol = this->maxCol(i);

    if ((mincol <= j) && (j < maxcol))
    {
        this->element(i, j - mincol) = element;
    }
}

bool Numerical::IBandMatrix::isSquare() const
{
    return true;
}

bool Numerical::IBandMatrix::isSymmetric() const
{
    if (this->lowerBandwidth() != this->upperBandwidth())
    {
        return false;
    }

    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt maxcol = this->maxCol(i);

        for (UInt j = i+1; j < maxcol; j++)
        {
            if (this->getElement(i, j) != this->getElement(j, i))
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IBandMatrix::isTriangular() const
{
    if ((this->lowerBandwidth() == 0) || (this->upperBandwidth() == 0))
    {
        return true;
    }

    bool lower = true, upper = true;

    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt mincol = this->minCol(i);
        UInt maxcol = this->maxCol(i);

        for (UInt j = mincol; j < maxcol; j++)
        {
            if ((i < j) && (this->getElement(i, j) != 0.0))
            {
                lower = false;
            }

            if ((i > j) && (this->getElement(i, j) != 0.0))
            {
                upper = false;
            }

            if ((!lower) && (!upper))
            {
                return false;
            }
        }
    }

    return ((lower) || (upper));
}

bool Numerical::IBandMatrix::isLowerTriangular() const
{
    if (this->upperBandwidth() == 0)
    {
        return true;
    }

    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt maxcol = this->maxCol(i);

        for (UInt j = i+1; j < maxcol; j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IBandMatrix::isUpperTriangular() const
{
    if (this->lowerBandwidth() == 0)
    {
        return true;
    }

    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt mincol = this->minCol(i);

        for (UInt j = mincol; j < std::min(i, this->numCols()); j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IBandMatrix::isDiagonal() const
{
    if ((this->lowerBandwidth() == 0) && (this->upperBandwidth() == 0))
    {
        return true;
    }

    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt mincol = this->minCol(i);
        UInt maxcol = this->maxCol(i);

        for (UInt j = mincol; j < maxcol; j++)
        {
            if ((i != j) && (this->getElement(i, j) != 0.0))
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IBandMatrix::isBand() const
{
    return ((this->lowerBandwidth() + 1 != this->numRows()) || (this->upperBandwidth() + 1 != this->numRows()));
}

bool Numerical::IBandMatrix::isIdentity() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt mincol = this->minCol(i);
        UInt maxcol = this->maxCol(i);

        for (UInt j = mincol; j < maxcol; j++)
        {
            if ((i != j) && (this->getElement(i, j) != 0.0))
            {
                return false;
            }

            if ((i == j) && (this->getElement(i, j) != 1.0))
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IBandMatrix::isZero() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        UInt mincol = this->minCol(i);
        UInt maxcol = this->maxCol(i);

        for (UInt j = mincol; j < maxcol; j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

UInt Numerical::IBandMatrix::numElements() const
{
    return this->numRows()*(this->lowerBandwidth() + this->upperBandwidth() + 1);
}

UInt Numerical::IBandMatrix::minCol(UInt i) const
{
    return (i < (this->lowerBandwidth() + 1)) ? 0 : (i - this->lowerBandwidth());
}

UInt Numerical::IBandMatrix::maxCol(UInt i) const
{
    return (i < (this->numCols() - this->upperBandwidth())) ? i + this->upperBandwidth() + 1: this->numCols();
}

Numerical::BandMatrix::BandMatrix(UInt rows, UInt lowerBand, UInt upperBand) :
    Numerical::IBandMatrix::IBandMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(rows, lowerBand + upperBand + 1)
{
    this->lowerBand = lowerBand;
    this->upperBand = upperBand;
}

Numerical::BandMatrix::BandMatrix(const BandMatrix &m) :
    Numerical::IBandMatrix::IBandMatrix(),
    Numerical::TDMatrix<Real>::TDMatrix(m)
{
    this->lowerBand = m.lowerBand;
    this->upperBand = m.upperBand;
}

Numerical::BandMatrix::~BandMatrix()
{

}

UInt Numerical::BandMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::BandMatrix::numCols() const
{
    return this->rows;
}

UInt Numerical::BandMatrix::lowerBandwidth() const
{
    return this->lowerBand;
}

UInt Numerical::BandMatrix::upperBandwidth() const
{
    return this->upperBand;
}

void Numerical::BandMatrix::resize(UInt rows, UInt lowerBand, UInt upperBand)
{
    this->reallocate(rows, lowerBand + upperBand + 1);

    this->lowerBand = lowerBand;
    this->upperBand = upperBand;
}

Real &Numerical::BandMatrix::element(UInt i, UInt j)
{
    return this->TDMatrix<Real>::element(i, j);
}

IMatrix *Numerical::BandMatrix::clone() const
{
    return new BandMatrix(*this);
}

Numerical::TridiagonalMatrix::TridiagonalMatrix(UInt rows) :
    Numerical::BandMatrix::BandMatrix(rows, 1, 1)
{

}

Numerical::TridiagonalMatrix::TridiagonalMatrix(const TridiagonalMatrix &m) :
    Numerical::BandMatrix::BandMatrix(m)
{

}

Numerical::TridiagonalMatrix::~TridiagonalMatrix()
{

}

IMatrix *Numerical::TridiagonalMatrix::clone() const
{
    return new TridiagonalMatrix(*this);
}

void Numerical::TridiagonalMatrix::resize(UInt rows)
{
    this->resize(rows, 1, 1);
}

#endif //#if USE_NUMERICAL
