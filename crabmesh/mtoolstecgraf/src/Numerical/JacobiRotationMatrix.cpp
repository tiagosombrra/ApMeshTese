#include "Numerical/JacobiRotationMatrix.h"

#if USE_NUMERICAL

#include "Numerical/DenseMatrix.h"
#include "Numerical/SymmetricMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::JacobiRotationMatrix::JacobiRotationMatrix() :
    Numerical::IMatrix::IMatrix()
{
    this->cos = 0.0;
    this->sin = 0.0;

    this->row = 0;
    this->col = 0;

    this->rows = 0;
}

Numerical::JacobiRotationMatrix::JacobiRotationMatrix(const JacobiRotationMatrix &m) :
    Numerical::IMatrix::IMatrix(m)
{
    this->cos = m.cos;
    this->sin = m.sin;

    this->row = m.row;
    this->col = m.col;

    this->rows = m.rows;
}

Numerical::JacobiRotationMatrix::~JacobiRotationMatrix()
{

}

void Numerical::JacobiRotationMatrix::setElement(UInt i, UInt j, Real element)
{
    if (((i == this->row) || (i == this->col)) &&
        ((j == this->row) || (j == this->col)))
    {
        if (i > j)
        {
            this->element(j, i) = -element;
        }
        else
        {
            this->element(i, j) = element;
        }
    }
}

Real Numerical::JacobiRotationMatrix::getElement(UInt i, UInt j) const
{
    if (((i == this->row) || (i == this->col)) &&
        ((j == this->row) || (j == this->col)))
    {
        return const_cast<JacobiRotationMatrix *>(this)->element(i, j);
    }

    return (i == j) ? 1.0 : 0.0;
}

UInt Numerical::JacobiRotationMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::JacobiRotationMatrix::numCols() const
{
    return this->numRows();
}

void Numerical::JacobiRotationMatrix::reset(const IMatrix *matrix, UInt row, UInt col)
{
    if (!matrix)
    {
        return;
    }

    this->rows = matrix->numRows();

    this->row = row;
    this->col = col;

    if (std::fabs(matrix->getElement(row, col)) < Data::getTolerance())
    {
        this->cos = 1.0;
        this->sin = 0.0;

        return;
    }

    Real theta = (matrix->getElement(col, col) - matrix->getElement(row, row))/(2.0*matrix->getElement(row, col));

    Real theta2 = theta*theta;

    Real t = (theta > 0.0) ? 1.0 : -1.0;;

    if ((std::isnan(theta2)) || (std::isinf(theta2)))
    {
        t = 1.0/(2.0*theta);
    }
    else
    {
        t /= std::fabs(theta) + std::sqrt(theta2 + 1.0);
    }

    this->cos = 1.0/std::sqrt(t*t + 1.0);
    this->sin = this->cos*t;
}

Real &Numerical::JacobiRotationMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    if (i == j)
    {
        if ((i == this->row) || (i == this->col))
        {
            return this->cos;
        }

        return one;
    }

    if ((i == this->row) && (j == this->col))
    {
        return this->sin;
    }

    if ((i == this->col) && (j == this->row))
    {
        static Real msin;

        msin = -this->sin;

        return msin;
    }

    return zero;
}

void Numerical::JacobiRotationMatrix::applyOn(IMatrix *matrix) const
{
    Real tau = this->sin/(1.0 + this->cos);

    Numerical::DenseMatrix m(this->rows, 2);

    bool typeSymmetric = dynamic_cast<SymmetricMatrix *>(matrix);

    for (UInt i = 0; i < this->rows; i++)
    {
        m.setElement(i, 0, matrix->getElement(i, this->row));
        m.setElement(i, 1, matrix->getElement(i, this->col));
    }

    for (UInt i = 0; i < this->rows; i++)
    {
        if ((i == this->row) || (i == this->col))
        {
            continue;
        }

        matrix->setElement(i, this->row, matrix->getElement(i, this->row) - this->sin*(m.getElement(i, 1) + tau*m.getElement(i, 0)));
        matrix->setElement(i, this->col, matrix->getElement(i, this->col) + this->sin*(m.getElement(i, 0) - tau*m.getElement(i, 1)));

        if (!typeSymmetric)
        {
            matrix->setElement(this->row, i, matrix->getElement(i, this->row));
            matrix->setElement(this->col, i, matrix->getElement(i, this->col));
        }
    }

    Real t = this->sin/this->cos;

    matrix->setElement(this->row, this->row, matrix->getElement(this->row, this->row) - t*matrix->getElement(this->row, this->col));
    matrix->setElement(this->col, this->col, matrix->getElement(this->col, this->col) + t*matrix->getElement(this->row, this->col));

    matrix->setElement(this->row, this->col, 0.0);
    if (!typeSymmetric)
    {
        matrix->setElement(this->col, this->row, 0.0);
    }
}

void Numerical::JacobiRotationMatrix::postmultiplyOn(IMatrix *matrix) const
{
    for (UInt i = 0; i < this->rows; i++)
    {
        Real a = matrix->getElement(i, this->row);
        Real b = matrix->getElement(i, this->col);

        matrix->setElement(i, this->row, a*this->cos - b*this->sin);
        matrix->setElement(i, this->col, a*this->sin + b*this->cos);
    }
}

IMatrix *Numerical::JacobiRotationMatrix::clone() const
{
    return new JacobiRotationMatrix(*this);
}

#endif //#if USE_NUMERICAL
