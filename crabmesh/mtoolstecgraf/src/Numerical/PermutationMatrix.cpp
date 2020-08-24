#include "Numerical/PermutationMatrix.h"

#if USE_NUMERICAL

#include "Numerical/DenseMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::IPermutationMatrix::~IPermutationMatrix()
{

}

Real Numerical::IPermutationMatrix::getElement(UInt i, UInt j) const
{
    return (this->column(i) == j) ? 1.0 : 0.0;
}

void Numerical::IPermutationMatrix::swapRows(UInt i1, UInt i2)
{
    std::swap(this->element(i1), this->element(i2));
}

void Numerical::IPermutationMatrix::swapCols(UInt j1, UInt j2)
{
    UInt i1 = this->numRows(), i2 = this->numRows();

    for (UInt i = 0; i < this->numRows(); i++)
    {
        if (this->column(i) == j1)
        {
            i1 = i;
        }

        if (this->column(i) == j2)
        {
            i2 = i;
        }

        if ((i1 < this->numRows()) && (i2 < this->numRows()))
        {
            break;
        }
    }

    this->element(i2) = j1;
    this->element(i1) = j2;
}

UInt Numerical::IPermutationMatrix::column(UInt row) const
{
    return const_cast<Numerical::IPermutationMatrix *>(this)->element(row);
}

UInt Numerical::IPermutationMatrix::row(UInt column) const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        if (this->column(i) == column)
        {
            return i;
        }
    }

    return this->numRows();
}

bool Numerical::IPermutationMatrix::isSquare() const
{
    return true;
}

bool Numerical::IPermutationMatrix::isSymmetric() const
{
    return this->isDiagonal();
}

bool Numerical::IPermutationMatrix::isTriangular() const
{
    return this->isDiagonal();
}

bool Numerical::IPermutationMatrix::isLowerTriangular() const
{
    return this->isDiagonal();
}

bool Numerical::IPermutationMatrix::isUpperTriangular() const
{
    return this->isDiagonal();
}

bool Numerical::IPermutationMatrix::isDiagonal() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        if (this->column(i) != i)
        {
            return false;
        }
    }

    return true;
}

bool Numerical::IPermutationMatrix::isIdentity() const
{
    return this->isDiagonal();
}

UInt Numerical::IPermutationMatrix::numElements() const
{
    return this->numRows();
}

void Numerical::IPermutationMatrix::premultiply(Numerical::IMatrix *matrix) const
{
    UInt size = this->numRows();

    Data::BoolVector used(size, false);

    UInt row = 0;

    for (UInt i = 0; i < size; i++)
    {
        while (used[row])
        {
            ++row;
        }

        UInt n = this->column(row);

        used[row] = true;

        if (used[n])
        {
            row = n;
        }

        if (row == n)
        {
            continue;
        }

        for (UInt j = 0; j < matrix->numCols(); j++)
        {
            Real v = matrix->getElement(row, j);
            matrix->setElement(row, j, matrix->getElement(n, j));
            matrix->setElement(n, j, v);
        }

        row = n;
    }
}

void Numerical::IPermutationMatrix::postmultiply(Numerical::IMatrix *matrix) const
{
    UInt size = this->numCols();

    Data::BoolVector used(size, false);

    UInt col = 0;

    for (UInt j = 0; j < size; j++)
    {
        while (used[col])
        {
            ++col;
        }

        UInt n = this->row(col);

        used[col] = true;

        if (used[n])
        {
            col = n;
        }

        if (col == n)
        {
            continue;
        }

        for (UInt i = 0; i < matrix->numRows(); i++)
        {
            Real v = matrix->getElement(i, col);
            matrix->setElement(i, col, matrix->getElement(i, n));
            matrix->setElement(i, n, v);
        }

        col = n;
    }
}

void Numerical::IPermutationMatrix::setElement(UInt /*i*/, UInt /*j*/, Real /*element*/)
{

}

Real &Numerical::IPermutationMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    return (this->column(i) == j) ? one : zero;
}

Numerical::PermutationMatrix::PermutationMatrix(UInt rows) :
    Numerical::IPermutationMatrix::IPermutationMatrix(),
    Numerical::TDMatrix<UInt>::TDMatrix(rows, 1)
{
    for (UInt i = 0; i < rows; i++)
    {
        this->element(i) = i;
    }
}

Numerical::PermutationMatrix::PermutationMatrix(const PermutationMatrix &m) :
    Numerical::IPermutationMatrix::IPermutationMatrix(),
    Numerical::TDMatrix<UInt>::TDMatrix(m)
{

}

Numerical::PermutationMatrix::~PermutationMatrix()
{

}

UInt Numerical::PermutationMatrix::numRows() const
{
    return this->Numerical::TDMatrix<UInt>::rows;
}

UInt Numerical::PermutationMatrix::numCols() const
{
    return this->Numerical::TDMatrix<UInt>::rows;
}

void Numerical::PermutationMatrix::resize(UInt rows)
{
    UInt prevRows = this->numRows();

    this->reallocate(rows, 1);

    for (UInt i = prevRows; i < rows; i++)
    {
        this->element(i) = i;
    }
}

UInt &Numerical::PermutationMatrix::element(UInt i)
{
    return this->Numerical::TDMatrix<UInt>::element(i, 0);
}

IMatrix *Numerical::PermutationMatrix::clone() const
{
    return new PermutationMatrix(*this);
}

#endif //#if USE_NUMERICAL
