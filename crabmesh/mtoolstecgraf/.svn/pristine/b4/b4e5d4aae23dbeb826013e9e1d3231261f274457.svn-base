#ifndef _NUMERICAL_DENSE_MATRIX_H_
#define _NUMERICAL_DENSE_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
    class IDenseMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~IDenseMatrix();
    };

    template <UInt rows, UInt cols>
    class TDenseMatrix :
        public Numerical::IDenseMatrix,
        public Numerical::TMatrix<Real, rows, cols>
    {
    public:

        TDenseMatrix();
        TDenseMatrix(const TDenseMatrix<rows, cols> &m);
        virtual ~TDenseMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);
    };

    class DenseMatrix :
        public Numerical::IDenseMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        DenseMatrix(UInt rows, UInt cols);
        DenseMatrix(const DenseMatrix &m);
        virtual ~DenseMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual void resize(UInt rows, UInt cols);

		virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);
    };
}

template <Data::UInt rows, Data::UInt cols>
Numerical::TDenseMatrix<rows, cols>::TDenseMatrix() :
    Numerical::IDenseMatrix::IDenseMatrix(),
    Numerical::TMatrix<Real, rows, cols>::TMatrix()
{

}

template <Data::UInt rows, Data::UInt cols>
Numerical::TDenseMatrix<rows, cols>::TDenseMatrix(const TDenseMatrix<rows, cols> &m) :
    Numerical::IDenseMatrix::IDenseMatrix(),
    Numerical::TMatrix<Real, rows, cols>::TMatrix(m)
{

}

template <Data::UInt rows, Data::UInt cols>
Numerical::TDenseMatrix<rows, cols>::~TDenseMatrix()
{

}

template <Data::UInt rows, Data::UInt cols>
Data::UInt Numerical::TDenseMatrix<rows, cols>::numRows() const
{
    return rows;
}

template <Data::UInt rows, Data::UInt cols>
Data::UInt Numerical::TDenseMatrix<rows, cols>::numCols() const
{
    return cols;
}

template <Data::UInt rows, Data::UInt cols>
Data::Real &Numerical::TDenseMatrix<rows, cols>::element(UInt i, UInt j)
{
    return Numerical::TMatrix<Real, rows, cols>::element(i, j);
}

template <Data::UInt rows, Data::UInt cols>
Numerical::IMatrix *Numerical::TDenseMatrix<rows, cols>::clone() const
{
    return new TDenseMatrix<rows, cols>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_DENSE_MATRIX_H_
