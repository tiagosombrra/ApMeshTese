#ifndef _NUMERICAL_COLUMN_MATRIX_H_
#define _NUMERICAL_COLUMN_MATRIX_H_

#include "Definitions.h"

#if USE_NUMERICAL

#include "IMatrix.h"
#include "TMatrix.h"

namespace Numerical
{
    class IColumnMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~IColumnMatrix();

        virtual UInt numCols() const;

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

    template <UInt rows>
    class TColumnMatrix :
        public Numerical::IColumnMatrix,
        public Numerical::TMatrix<Real, rows, 1>
    {
    public:

        TColumnMatrix();
        TColumnMatrix(const TColumnMatrix<rows> &m);
        virtual ~TColumnMatrix();

        virtual UInt numRows() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::IColumnMatrix::element;
        virtual Real &element(UInt i);
    };

    class ColumnMatrix :
        public Numerical::IColumnMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        ColumnMatrix(UInt rows);
        ColumnMatrix(const ColumnMatrix &m);
        virtual ~ColumnMatrix();

        virtual UInt numRows() const;

		virtual void resize(UInt rows);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::IColumnMatrix::element;
        virtual Real &element(UInt i);
    };
}

template <Data::UInt rows>
Numerical::TColumnMatrix<rows>::TColumnMatrix() :
    Numerical::IColumnMatrix::IColumnMatrix(),
    Numerical::TMatrix<Real, rows, 1>::TMatrix()
{

}

template <Data::UInt rows>
Numerical::TColumnMatrix<rows>::TColumnMatrix(const TColumnMatrix<rows> &m) :
    Numerical::IColumnMatrix::IColumnMatrix(),
    Numerical::TMatrix<Real, rows, 1>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TColumnMatrix<rows>::~TColumnMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TColumnMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::Real &Numerical::TColumnMatrix<rows>::element(UInt i)
{
    return Numerical::TMatrix<Real, rows, 1>::element(i, 0);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TColumnMatrix<rows>::clone() const
{
    return new TColumnMatrix<rows>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_COLUMN_MATRIX_H_
