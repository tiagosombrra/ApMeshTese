#ifndef _NUMERICAL_DIAGONAL_MATRIX_H_
#define _NUMERICAL_DIAGONAL_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/BandMatrix.h"

namespace Numerical
{
	template <UInt rows>
    class TDiagonalMatrix :
        public Numerical::TBandMatrix<rows, 0, 0>
    {
    public:

        TDiagonalMatrix();
        TDiagonalMatrix(const TDiagonalMatrix<rows> &m);
        virtual ~TDiagonalMatrix();

        virtual IMatrix *clone() const;

    };

    class DiagonalMatrix :
        public Numerical::BandMatrix
    {
    public:

        DiagonalMatrix(UInt rows);
        DiagonalMatrix(const DiagonalMatrix &m);
        virtual ~DiagonalMatrix();

        virtual void resize(UInt rows);

        virtual IMatrix *clone() const;

    protected:

        using Numerical::BandMatrix::resize;

    };
}

template <Data::UInt rows>
Numerical::TDiagonalMatrix<rows>::TDiagonalMatrix() :
    Numerical::TBandMatrix<rows, 0, 0>::TBandMatrix()
{

}

template <Data::UInt rows>
Numerical::TDiagonalMatrix<rows>::TDiagonalMatrix(const TDiagonalMatrix<rows> &m) :
    Numerical::TBandMatrix<rows, 0, 0>::TBandMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TDiagonalMatrix<rows>::~TDiagonalMatrix()
{

}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TDiagonalMatrix<rows>::clone() const
{
    return new TDiagonalMatrix<rows>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_DIAGONAL_MATRIX_H_
