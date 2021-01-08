#include "Numerical/HouseholderMatrix.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::HouseholderMatrix::HouseholderMatrix() :
    Numerical::SymmetricMatrix::SymmetricMatrix(1)
{
    this->type = 0;
    this->row = 0;
    this->realRows = 0;
    this->originalRows = 0;
}

Numerical::HouseholderMatrix::~HouseholderMatrix()
{

}

void Numerical::HouseholderMatrix::setElement(UInt i, UInt j, Real element)
{
    switch (this->type)
    {
    case 1:
        if ((i >= this->row) && (j >= this->row))
        {
            SymmetricMatrix::setElement(i - this->row, j - this->row, element);
        }
        break;
    case 2:
        if ((i <= this->row) && (j <= this->row))
        {
            SymmetricMatrix::setElement(i, j, element);
        }
        break;
    case 3:
        SymmetricMatrix::setElement(i, j, element);
        break;
    }
}

Real Numerical::HouseholderMatrix::getElement(UInt i, UInt j) const
{
    switch (this->type)
    {
    case 1:
        return ((i < this->row) || (j < this->row)) ?
            ((i == j) ? 1.0 : 0.0) : SymmetricMatrix::getElement(i - this->row, j - this->row);
    case 2:
        return ((i > this->row) || (j > this->row)) ?
            ((i == j) ? 1.0 : 0.0) : SymmetricMatrix::getElement(i, j);
    case 3:
        return SymmetricMatrix::getElement(i, j);
    default:
        return 0.0;
    }

    return 0.0;
}

UInt Numerical::HouseholderMatrix::numRows() const
{
    return this->originalRows;
}

UInt Numerical::HouseholderMatrix::numCols() const
{
    return this->numRows();
}

UInt Numerical::HouseholderMatrix::numElements() const
{
    return Numerical::ISymmetricMatrix::numElements2(this->realRows);
}

void Numerical::HouseholderMatrix::reset(const IMatrix *matrix, UInt row, UInt col, bool above, bool below)
{
    if ((!above) && (!below))
    {
        this->type = 0;

        return;
    }

    if ((below) && (!above))
    {
        this->type = 1;
    }
    else if ((!below) && (above))
    {
        this->type = 2;
    }
    else
    {
        this->type = 3;
    }

    this->row = row;

    this->originalRows = matrix->numRows();

    Real s = 0.0;

    UInt min = above ? 0 : this->row;
    UInt max = below ? this->originalRows : this->row + 1;

    this->realRows = max - min;

    this->resize(this->realRows);

    for (UInt i = min; i < max; i++)
    {
        Real v = matrix->getElement(i, col);

        s += v*v;
    }

    s = std::sqrt(s);

    ColumnMatrix u(this->realRows);

    Real s2 = 0.0;

    for (UInt i = 0; i < this->realRows; i++)
    {
        Real v = matrix->getElement(min + i, col);

        if (min + i == this->row)
        {
            v -= s;
        }

        s2 += v*v;

        u.setElement(i, v);
    }

    s = std::sqrt(s2);

    s = (s <= Data::getTolerance()) ? 0.0 : 1.0/s;

    Numerical::IMatrix::multiply(u, s, u);

    for (UInt i = 0; i < this->realRows; i++)
    {
        for (UInt j = i; j < this->realRows; j++)
        {
            Real v = -2.0*u.getElement(i)*u.getElement(j);

            if (i == j)
            {
                v += 1.0;
            }

            this->setElement(min + i, min + j, v);
        }
    }
}

Real &Numerical::HouseholderMatrix::element(UInt i, UInt j)
{
    return this->element(i*this->realRows - Numerical::ISymmetricMatrix::numElements2(i-1) + j - i);
}

/*Real &Numerical::HouseholderMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    if (this->type == 0)
    {
        return zero;
    }

    if (this->type == 3)
    {
        return SymmetricMatrix::element(i, j);
    }

    if (this->type == 1)
    {
        if ((i < this->row) || (j < this->row))
        {
            return (i == j) : one : zero;
        }

        return SymmetricMatrix::element(i - this->row, j - this->row);
    }

    if (this->type == 2)
    {
        if ((i > this->row) || (j > this->row))
        {
            return (i == j) : one : zero;
        }

        return SymmetricMatrix::element(i, j);
    }
}*/

#endif //#if USE_NUMERICAL
