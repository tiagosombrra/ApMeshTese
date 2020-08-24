#ifndef _NUMERICAL_PERMUTATION_MATRIX_H_
#define _NUMERICAL_PERMUTATION_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
    class IPermutationMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~IPermutationMatrix();

        virtual Real getElement(UInt i, UInt j) const;

		virtual void swapRows(UInt i1, UInt i2);
		virtual void swapCols(UInt j1, UInt j2);

		virtual UInt column(UInt row) const;
		virtual UInt row(UInt column) const;

		virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
		virtual bool isLowerTriangular() const;
		virtual bool isUpperTriangular() const;
		virtual bool isDiagonal() const;
		virtual bool isIdentity() const;

		virtual UInt numElements() const;

		virtual void premultiply(Numerical::IMatrix *matrix) const;
		virtual void postmultiply(Numerical::IMatrix *matrix) const;

    protected:

		virtual void setElement(UInt i, UInt j, Real element);

		virtual Real &element(UInt i, UInt j);
		virtual UInt &element(UInt i) = 0;

    };

    template <UInt rows>
    class TPermutationMatrix :
        public Numerical::IPermutationMatrix,
        public Numerical::TMatrix<UInt, rows, 1>
    {
    public:

        TPermutationMatrix();
        TPermutationMatrix(const TPermutationMatrix<rows> &m);
        virtual ~TPermutationMatrix();

        virtual UInt numRows() const;
        virtual UInt numCols() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::IPermutationMatrix::element;
        virtual UInt &element(UInt i);

    };

    class PermutationMatrix :
        public Numerical::IPermutationMatrix,
        public Numerical::TDMatrix<UInt>
    {
    public:

        PermutationMatrix(UInt rows);
        PermutationMatrix(const PermutationMatrix &m);
        virtual ~PermutationMatrix();

        virtual UInt numRows() const;
        virtual UInt numCols() const;

		virtual void resize(UInt rows);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::IPermutationMatrix::element;
        virtual UInt &element(UInt i);

    };
}

template <Data::UInt rows>
Numerical::TPermutationMatrix<rows>::TPermutationMatrix() :
    Numerical::IPermutationMatrix::IPermutationMatrix(),
    Numerical::TMatrix<UInt, rows, 1>::TMatrix()
{
    for (UInt i = 0; i < rows; i++)
    {
        this->element(i) = i;
    }
}

template <Data::UInt rows>
Numerical::TPermutationMatrix<rows>::TPermutationMatrix(const TPermutationMatrix<rows> &m) :
    Numerical::IPermutationMatrix::IPermutationMatrix(),
    Numerical::TMatrix<UInt, rows, 1>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TPermutationMatrix<rows>::~TPermutationMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TPermutationMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt Numerical::TPermutationMatrix<rows>::numCols() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt &Numerical::TPermutationMatrix<rows>::element(UInt i)
{
    return Numerical::TMatrix<UInt, rows, 1>::element(i, 0);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TPermutationMatrix<rows>::clone() const
{
    return new TPermutationMatrix<rows>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_PERMUTATION_MATRIX_H_
