#include "Numerical/MatrixTridiagonalizer.h"

#if USE_NUMERICAL

#include "Numerical/BandMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::MatrixTridiagonalizer::MatrixTridiagonalizer() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;

    this->tridiagonalized = NULL;
    this->tridiagonalizedGot = false;

    this->copy = false;

    this->tridiagonal = NULL;
}

Numerical::MatrixTridiagonalizer::~MatrixTridiagonalizer()
{
    if ((!this->tridiagonalizedGot) && (this->tridiagonalized) && (this->tridiagonalized != this->matrix))
    {
        delete this->tridiagonalized;
    }
}

void Numerical::MatrixTridiagonalizer::setMatrix(IMatrix *matrix)
{
    this->matrix = matrix;
}

IMatrix *Numerical::MatrixTridiagonalizer::getMatrix() const
{
    return this->matrix;
}

void Numerical::MatrixTridiagonalizer::setCopy(bool copy)
{
    this->copy = copy;
}

bool Numerical::MatrixTridiagonalizer::isCopy() const
{
    return this->copy;
}

IMatrix *Numerical::MatrixTridiagonalizer::getTridiagonalized() const
{
    this->tridiagonalizedGot = true;

    return this->tridiagonalized;
}

TridiagonalMatrix *Numerical::MatrixTridiagonalizer::getTridiagonal() const
{
    if (!this->tridiagonalized)
    {
        return NULL;
    }

    UInt nr = this->tridiagonalized->numRows();

    this->tridiagonal = new TridiagonalMatrix(nr);

    for (UInt i = 0; i < nr; i++)
    {
        for (UInt j = 0; j < 3; j++)
        {
            if (((i == 0) && (j == 0)) ||
                ((i == nr - 1) && (j == 2)))
            {
                continue;
            }

            this->tridiagonal->setElement(i, i + j - 1, this->tridiagonalized->getElement(i, i + j - 1));
        }
    }

    return this->tridiagonal;
}

#endif //#if USE_NUMERICAL
