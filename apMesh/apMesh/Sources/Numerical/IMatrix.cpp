#include "../../Numerical/IMatrix.h"

#if USE_NUMERICAL

#include "../../Numerical/PermutationMatrix.h"
#include "../../Numerical/ColumnMatrix.h"
#include "../../Numerical/RowMatrix.h"
#include "../../Data/OStream.h"

using namespace Data;
using namespace Numerical;

//const Real Numerical::IMatrix::zero = 0.0;

Numerical::IMatrix::~IMatrix()
{

}

UInt IMatrix::numRows() const
{
    return 0;
}

UInt IMatrix::numCols() const
{
 return 0;
}

Real Numerical::IMatrix::getElement(UInt i, UInt j) const
{
    return const_cast<IMatrix *>(this)->element(i, j);
}

void Numerical::IMatrix::setElement(UInt i, UInt j, Real element)
{
    this->element(i, j) = element;
}

bool Numerical::IMatrix::isSquare() const
{
    return (this->numRows() == this->numCols());
}

bool Numerical::IMatrix::isSymmetric() const
{
    if (!this->isSquare())
    {
        return false;
    }

    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = i+1; j < this->numCols(); j++)
        {
            if (this->getElement(i, j) != this->getElement(j, i))
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IMatrix::isTriangular() const
{
    bool lower = true, upper = true;

    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < this->numCols(); j++)
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

bool Numerical::IMatrix::isLowerTriangular() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = i+1; j < this->numCols(); j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IMatrix::isUpperTriangular() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < std::min(i, this->numCols()); j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IMatrix::isDiagonal() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < this->numCols(); j++)
        {
            if ((i != j) && (this->getElement(i, j) != 0.0))
            {
                return false;
            }
        }
    }

    return true;
}

bool Numerical::IMatrix::isColumn() const
{
    return (this->numCols() == 1);
}

bool Numerical::IMatrix::isRow() const
{
    return (this->numRows() == 1);
}

bool Numerical::IMatrix::isBand() const
{
    UInt min = std::min(this->numRows(), this->numCols());

    return ((this->lowerBandwidth() + 1 != min) || (this->upperBandwidth() + 1 != min));
}

bool Numerical::IMatrix::isIdentity() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < this->numCols(); j++)
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

bool Numerical::IMatrix::isZero() const
{
    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < this->numCols(); j++)
        {
            if (this->getElement(i, j) != 0.0)
            {
                return false;
            }
        }
    }

    return true;
}

UInt Numerical::IMatrix::numElements() const
{
    return this->numRows()*this->numCols();
}

UInt Numerical::IMatrix::lowerBandwidth() const
{
    UInt min = std::min(this->numRows(), this->numCols());

    UInt band = min - 1;

    for (UInt diag = 0; diag < min - 1; diag++, band--)
    {
        for (UInt j = 0; j <= diag; j++)
        {
            UInt i = min + j - diag - 1;

            if (this->getElement(i, j) != 0.0)
            {
                break;
            }
        }
    }

    if ((band == 0) || (this->isSquare()))
    {
        return band;
    }

    if (this->numRows() > this->numCols())
    {
        for (UInt i = min; i < this->numRows(); i++)
        {
            UInt max = std::min(this->numCols() - band + i, this->numCols());

            for (UInt j = 0; j < max; j++)
            {
                if (this->getElement(i, j) != 0.0)
                {
                    band = i - j;

                    break;
                }
            }
        }
    }

    return band;
}

UInt Numerical::IMatrix::upperBandwidth() const
{
    UInt min = std::min(this->numRows(), this->numCols());

    UInt band = min - 1;

    for (UInt diag = 0; diag < min - 1; diag++, band--)
    {
        for (UInt i = 0; i <= diag; i++)
        {
            UInt j = min + i - diag - 1;

            if (this->getElement(i, j) != 0.0)
            {
                break;
            }
        }
    }

    if ((band == 0) || (this->isSquare()))
    {
        return band;
    }

    if (this->numCols() > this->numRows())
    {
        for (UInt j = min; j < this->numCols(); j++)
        {
            UInt max = std::min(this->numRows() - band + j, this->numRows());

            for (UInt i = 0; i < max; i++)
            {
                if (this->getElement(i, j) != 0.0)
                {
                    band = j - i;

                    break;
                }
            }
        }
    }

    return band;
}

std::string Numerical::IMatrix::text() const
{
    Data::TableStream str(10, 6);

    for (UInt i = 0; i < this->numRows(); i++)
    {
        for (UInt j = 0; j < this->numCols(); j++)
        {
            str << this->getElement(i, j);
        }

        str.endl();
    }

    return str.str();
}

IMatrix *IMatrix::clone() const
{

}

Real &IMatrix::element(UInt i, UInt j)
{

}

bool Numerical::IMatrix::isMultipliable(const IMatrix *first, const IMatrix *second)
{
    return (first->numCols() == second->numRows());
}

bool Numerical::IMatrix::isMultipliable(const IMatrix &first, const IMatrix &second)
{
    return Numerical::IMatrix::isMultipliable(&first, &second);
}

bool Numerical::IMatrix::isSummable(const IMatrix *first, const IMatrix *second)
{
    return ((first->numRows() == second->numRows()) && (first->numCols() == second->numCols()));
}

bool Numerical::IMatrix::isSummable(const IMatrix &first, const IMatrix &second)
{
    return Numerical::IMatrix::isSummable(&first, &second);
}

void Numerical::IMatrix::multiply(const IMatrix *first, const IMatrix *second, IMatrix *result)
{
    /*for (UInt i = 0; i < first->numRows(); i++)
    {
        for (UInt j = 0; j < second->numCols(); j++)
        {
            Real mult = 0.0;

            for (UInt k = 0; k < first->numCols(); k++)
            {
                mult += first->getElement(i, k)*second->getElement(k, j);
            }

            result->setElement(i, j, mult);
        }
    }*/

    Numerical::IMatrix::multiply(first, second, result, 0, 0, 0, 0, 0, 0, first->numRows(), second->numCols(), first->numCols());
}

void Numerical::IMatrix::multiply(const IMatrix &first, const IMatrix &second, IMatrix &result)
{
    Numerical::IMatrix::multiply(&first, &second, &result);
}

void Numerical::IMatrix::multiply(const IMatrix *first, const IMatrix *second, IMatrix *result,
    UInt frow, UInt fcol, UInt srow, UInt scol, UInt rrow, UInt rcol, UInt fnrows, UInt sncols, UInt nelems)
{
    for (UInt i = 0; i < fnrows; i++)
    {
        for (UInt j = 0; j < sncols; j++)
        {
            Real mult = 0.0;

            for (UInt k = 0; k < nelems; k++)
            {
                mult += first->getElement(frow + i, fcol + k)*second->getElement(srow + k, scol + j);
            }

            result->setElement(rrow + i, rcol + j, mult);
        }
    }
}

void Numerical::IMatrix::multiply(const IMatrix &first, const IMatrix &second, IMatrix &result,
    UInt frow, UInt fcol, UInt srow, UInt scol, UInt rrow, UInt rcol, UInt fnrows, UInt sncols, UInt nelems)
{
    Numerical::IMatrix::multiply(&first, &second, &result, frow, fcol, srow, scol, rrow, rcol, fnrows, sncols, nelems);
}

void Numerical::IMatrix::multiply(const IPermutationMatrix *first, const IMatrix *second, IMatrix *result)
{
    for (UInt row = 0; row < result->numRows(); row++)
    {
        UInt oldRow = first->column(row);

        for (UInt col = 0; col < result->numCols(); col++)
        {
            result->setElement(row, col, second->getElement(oldRow, col));
        }
    }
}

void Numerical::IMatrix::multiply(const IPermutationMatrix &first, const IMatrix &second, IMatrix &result)
{
    Numerical::IMatrix::multiply(&first, &second, &result);
}

void Numerical::IMatrix::multiply(const IMatrix *first, const IPermutationMatrix *second, IMatrix *result)
{
    for (UInt col = 0; col < result->numCols(); col++)
    {
        UInt oldCol = second->row(col);

        for (UInt row = 0; row < result->numRows(); row++)
        {
            result->setElement(row, col, first->getElement(row, oldCol));
        }
    }
}

void Numerical::IMatrix::multiply(const IMatrix &first, const IPermutationMatrix &second, IMatrix &result)
{
    Numerical::IMatrix::multiply(&first, &second, &result);
}

void Numerical::IMatrix::multiply(const IColumnMatrix *first, const IRowMatrix *second, Real &value)
{
    value = 0.0;

    for (UInt i = 0; i < first->numRows(); i++)
    {
        value += first->getElement(i)*second->getElement(i);
    }
}

void Numerical::IMatrix::multiply(const IColumnMatrix &first, const IRowMatrix &second, Real &value)
{
    Numerical::IMatrix::multiply(&first, &second, value);
}

void Numerical::IMatrix::multiply(const IColumnMatrix *first, const IColumnMatrix *second, Real &value)
{
    value = 0.0;

    for (UInt i = 0; i < first->numRows(); i++)
    {
        value += first->getElement(i)*second->getElement(i);
    }
}

void Numerical::IMatrix::multiply(const IColumnMatrix &first, const IColumnMatrix &second, Real &value)
{
    Numerical::IMatrix::multiply(&first, &second, value);
}

void Numerical::IMatrix::multiply(const IRowMatrix *first, const IRowMatrix *second, Real &value)
{
    value = 0.0;

    for (UInt i = 0; i < first->numCols(); i++)
    {
        value += first->getElement(i)*second->getElement(i);
    }
}

void Numerical::IMatrix::multiply(const IRowMatrix &first, const IRowMatrix &second, Real &value)
{
    Numerical::IMatrix::multiply(&first, &second, value);
}

void Numerical::IMatrix::multiply(const IMatrix *first, Real constant, IMatrix *result)
{
    Numerical::IMatrix::multiply(first, constant, result, 0, 0, 0, 0, first->numRows(), first->numCols());
}

void Numerical::IMatrix::multiply(const IMatrix &first, Real constant, IMatrix &result)
{
    Numerical::IMatrix::multiply(&first, constant, &result);
}

void Numerical::IMatrix::multiply(const IMatrix *first, Real constant, IMatrix *result, UInt frow, UInt fcol, UInt rrow, UInt rcol, UInt fnrows, UInt fncols)
{
    for (UInt i = 0; i < fnrows; i++)
    {
        for (UInt j = 0; j < fncols; j++)
        {
            Real mult = first->getElement(frow + i, fcol + j)*constant;

            result->setElement(rrow + i, rcol + j, mult);
        }
    }
}

void Numerical::IMatrix::multiply(const IMatrix &first, Real constant, IMatrix &result, UInt frow, UInt fcol, UInt rrow, UInt rcol, UInt fnrows, UInt fncols)
{
    Numerical::IMatrix::multiply(&first, constant, &result, frow, fcol, rrow, rcol, fnrows, fncols);
}

void Numerical::IMatrix::sum(const IMatrix *first, const IMatrix *second, IMatrix *result)
{
    Numerical::IMatrix::sum(first, second, result, 0, first->numRows(), 0, first->numCols());
}

void Numerical::IMatrix::sum(const IMatrix &first, const IMatrix &second, IMatrix &result)
{
    Numerical::IMatrix::sum(&first, &second, &result);
}

void Numerical::IMatrix::sum(const IMatrix *first, const IMatrix *second, IMatrix *result, UInt rmin, UInt rmax, UInt cmin, UInt cmax)
{
    for (UInt i = rmin; i < rmax; i++)
    {
        for (UInt j = cmin; j < cmax; j++)
        {
            Real sum = first->getElement(i, j) + second->getElement(i, j);

            result->setElement(i, j, sum);
        }
    }
}

void Numerical::IMatrix::sum(const IMatrix &first, const IMatrix &second, IMatrix &result, UInt rmin, UInt rmax, UInt cmin, UInt cmax)
{
    Numerical::IMatrix::sum(&first, &second, &result, rmin, rmax, cmin, cmax);
}

void Numerical::IMatrix::copy(const IMatrix *from, IMatrix *to)
{
    Numerical::IMatrix::copy(from, to, 0, to->numRows(), 0, to->numCols());
}

void Numerical::IMatrix::copy(const IMatrix &from, IMatrix &to)
{
    Numerical::IMatrix::copy(&from, &to);
}

void Numerical::IMatrix::copy(const IMatrix *from, IMatrix *to, UInt rmin, UInt rmax, UInt cmin, UInt cmax)
{
    for (UInt i = rmin; i < rmax; i++)
    {
        for (UInt j = cmin; j < cmax; j++)
        {
            to->setElement(i, j, from->getElement(i, j));
        }
    }
}

void Numerical::IMatrix::copy(const IMatrix &from, IMatrix &to, UInt rmin, UInt rmax, UInt cmin, UInt cmax)
{
    Numerical::IMatrix::copy(&from, &to, rmin, rmax, cmin, cmax);
}

#endif //#if USE_NUMERICAL
