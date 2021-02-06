#ifndef _NUMERICAL_ROW_MATRIX_H_
#define _NUMERICAL_ROW_MATRIX_H_

#include "Definitions.h"

#if USE_NUMERICAL

#include "IMatrix.h"
#include "TMatrix.h"

namespace Numerical
{
    class IRowMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~IRowMatrix();

        virtual UInt numRows() const;

        virtual Real getElement(UInt i) const;
		virtual void setElement(UInt i, Real element);

		virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
		virtual bool isLowerTriangular() const;
		virtual bool isUpperTriangular() const;
		virtual bool isDiagonal() const;
		virtual bool isColumn() const;
		virtual bool isRow() const;
		virtual bool isBand() const;
		virtual bool isIdentity() const;

		virtual UInt numElements() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

    protected:

		virtual Real getElement(UInt i, UInt j) const;
		virtual void setElement(UInt i, UInt j, Real element);

        virtual Real &element(UInt i, UInt j);
        virtual Real &element(UInt i) = 0;
    };

    template <UInt cols>
    class TRowMatrix :
        public Numerical::IRowMatrix,
        public Numerical::TMatrix<Real, 1, cols>
    {
    public:

        TRowMatrix();
        TRowMatrix(const TRowMatrix<cols> &m);
        virtual ~TRowMatrix();

        virtual UInt numCols() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::IRowMatrix::element;
        virtual Real &element(UInt i);
    };

    class RowMatrix :
        public Numerical::IRowMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        RowMatrix(UInt cols);
        RowMatrix(const RowMatrix &m);
        virtual ~RowMatrix();

        virtual UInt numCols() const;

		virtual void resize(UInt cols);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::IRowMatrix::element;
        virtual Real &element(UInt i);
    };
}

template <Data::UInt cols>
Numerical::TRowMatrix<cols>::TRowMatrix() :
    Numerical::IRowMatrix::IRowMatrix(),
    Numerical::TMatrix<Real, 1, cols>::TMatrix()
{

}

template <Data::UInt cols>
Numerical::TRowMatrix<cols>::TRowMatrix(const TRowMatrix<cols> &m) :
    Numerical::IRowMatrix::IRowMatrix(),
    Numerical::TMatrix<Real, 1, cols>::TMatrix(m)
{

}

template <Data::UInt cols>
Numerical::TRowMatrix<cols>::~TRowMatrix()
{

}

template <Data::UInt cols>
Data::UInt Numerical::TRowMatrix<cols>::numCols() const
{
    return cols;
}

template <Data::UInt cols>
Data::Real &Numerical::TRowMatrix<cols>::element(UInt i)
{
    return Numerical::TMatrix<Real, 1, cols>::element(0, i);
}

template <Data::UInt cols>
Numerical::IMatrix *Numerical::TRowMatrix<cols>::clone() const
{
    return new TRowMatrix<cols>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ROW_MATRIX_H_
