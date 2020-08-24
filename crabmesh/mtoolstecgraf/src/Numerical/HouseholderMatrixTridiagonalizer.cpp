#include "Numerical/HouseholderMatrixTridiagonalizer.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/SymmetricMatrix.h"
#include "Numerical/HouseholderMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::HouseholderMatrixTridiagonalizer::HouseholderMatrixTridiagonalizer() :
    Numerical::MatrixTridiagonalizer::MatrixTridiagonalizer()
{

}

Numerical::HouseholderMatrixTridiagonalizer::~HouseholderMatrixTridiagonalizer()
{

}

void Numerical::HouseholderMatrixTridiagonalizer::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    this->tridiagonalized = this->copy ? this->matrix->clone() : this->matrix;

    bool typeSymmetric = (dynamic_cast<ISymmetricMatrix *>(this->tridiagonalized) != NULL);
    bool symmetric = typeSymmetric || this->tridiagonalized->isSymmetric();

    for (UInt i = 0; i < this->tridiagonalized->numRows() - 2; i++)
    {
        this->tridiagonalize(i, typeSymmetric, symmetric);
    }
}

void Numerical::HouseholderMatrixTridiagonalizer::tridiagonalize(UInt row, bool typeSymmetric, bool symmetric)
{
    //Real size = 0.0;
    UInt numRows = this->tridiagonalized->numRows();

    /*for (UInt i = row + 1; i < numRows; i++)
    {
        Real val = this->tridiagonalized->getElement(i, row);

        size += val*val;
    }

    size = std::sqrt(size);

    if (this->tridiagonalized->getElement(row + 1, row) < 0)
    {
        size = -size;
    }

    Real h = size*size + size*this->tridiagonalized->getElement(row + 1, row);*/

    //UInt pnr = numRows - row - 1;
    UInt pnr = numRows - row;

    /*SymmetricMatrix p(pnr);

    for (UInt i = 0; i < pnr; i++)
    {
        Real l = this->element(i, row, size);

        for (UInt j = i; j < pnr; j++)
        {
            Real m = this->element(j, row, size);

            Real v = -l*m/h;

            if (i == j)
            {
                v += 1.0;
            }

            p.setElement(i, j, v);
        }
    }*/

    HouseholderMatrix p;

    p.reset(this->tridiagonalized, row + 1, row, false, true);

    DenseMatrix aux(pnr, pnr);

    //Numerical::IMatrix::multiply(&p, this->tridiagonalized, &aux, 0, 0, row + 1, row + 1, 0, 0, pnr, pnr, pnr);
    //Numerical::IMatrix::multiply(&aux, &p, this->tridiagonalized, 0, 0, 0, 0, row + 1, row + 1, pnr, pnr, pnr);
    Numerical::IMatrix::multiply(&p, this->tridiagonalized, &aux, row, row, row, row, 0, 0, pnr, pnr, pnr);
    Numerical::IMatrix::multiply(&aux, &p, this->tridiagonalized, 0, 0, row, row, row, row, pnr, pnr, pnr);

    /*this->tridiagonalized->setElement(row + 1, row, -size);

    if ((!typeSymmetric) && (symmetric))
    {
        this->tridiagonalized->setElement(row, row + 1, -size);
    }*/

    //for (UInt i = 1; i < pnr; i++)
    for (UInt i = 1; i < pnr - 1; i++)
    {
        this->tridiagonalized->setElement(row + i + 1, row, 0.0);

        if ((!typeSymmetric) && (symmetric))
        {
            this->tridiagonalized->setElement(row, row + i + 1, 0.0);
        }
    }

    /*if ((!typeSymmetric) && (!symmetric))
    {
        aux.resize(row + 1, pnr);

        for (UInt i = 0; i < row + 1; i++)
        {
            for (UInt j = 0; j < pnr; j++)
            {
                aux.setElement(i, j, this->tridiagonalized->getElement(i, row + j + 1));
            }
        }

        Numerical::IMatrix::multiply(&aux, &p, this->tridiagonalized, 0, 0, 0, 0, 0, row + 1, row + 1, pnr, pnr);
    }*/
}

Real Numerical::HouseholderMatrixTridiagonalizer::element(UInt row, UInt column, Real size)
{
    Real el = this->tridiagonalized->getElement(column + row + 1, column);

    return (row == 0) ? el + size : el;
}

#endif //#if USE_NUMERICAL
