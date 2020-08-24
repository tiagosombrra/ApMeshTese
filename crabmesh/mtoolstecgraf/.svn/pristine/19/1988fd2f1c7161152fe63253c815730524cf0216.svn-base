#ifndef _NUMERICAL_LU_MATRIX_H_
#define _NUMERICAL_LU_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"
#include "Numerical/TriangularMatrix.h"

namespace Numerical
{
    class ILUMatrix : public Numerical::IMatrix
    {
    friend class LMatrix;
    friend class UMatrix;

    public:

        ILUMatrix(const IMatrix *original);
        virtual ~ILUMatrix();

		virtual void setOriginal(const IMatrix *original);
		virtual const IMatrix *getOriginal() const;

		virtual UInt numRows() const;
		virtual UInt numCols() const;

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
		virtual bool isZero() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

		virtual const LMatrix *makeLower();
		virtual const LMatrix *getLower() const;

		virtual const UMatrix *makeUpper();
		virtual const UMatrix *getUpper() const;

    protected:

        const IMatrix *original;
        const LMatrix *lower;
        const UMatrix *upper;

    };

    template <UInt rows>
    class TLUMatrix :
        public Numerical::ILUMatrix,
        public Numerical::TMatrix<Real, rows, rows>
    {
    public:

        TLUMatrix(const IMatrix *original);
        TLUMatrix(const TLUMatrix<rows> &m);
        virtual ~TLUMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);
    };

    class LUMatrix :
        public Numerical::ILUMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        LUMatrix(const IMatrix *original);
        LUMatrix(const LUMatrix &m);
        virtual ~LUMatrix();

        virtual void resize(UInt rows, UInt cols);

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };

    class LMatrix :
        public Numerical::ILowerTriangularMatrix
    {
    public:

        LMatrix(ILUMatrix *lu);
        LMatrix(const LMatrix &m);
        ~LMatrix();

        virtual Real getElement(UInt i, UInt j) const;

        virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
        virtual bool isLowerTriangular() const;
        virtual bool isUpperTriangular() const;

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt numElements() const;

		virtual ILUMatrix *getLUMatrix() const;
		virtual const IMatrix *getOriginal() const;

		virtual IMatrix *clone() const;

    protected:

        virtual void setElement(UInt i, UInt j, Real val);

        virtual Real &element(UInt i, UInt j);
        virtual Real &element(UInt i);

    protected:

        ILUMatrix *lu;
    };

    class UMatrix :
        public Numerical::IUpperTriangularMatrix
    {
    public:

        UMatrix(ILUMatrix *lu);
        UMatrix(const UMatrix &u);
        ~UMatrix();

        virtual Real getElement(UInt i, UInt j) const;

        virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
        virtual bool isLowerTriangular() const;
        virtual bool isUpperTriangular() const;

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt numElements() const;

		virtual ILUMatrix *getLUMatrix() const;
		virtual const IMatrix *getOriginal() const;

		virtual IMatrix *clone() const;

    protected:

        virtual void setElement(UInt i, UInt j, Real val);

        virtual Real &element(UInt i, UInt j);
        virtual Real &element(UInt i);

    protected:

        ILUMatrix *lu;
    };
}

template <Data::UInt rows>
Numerical::TLUMatrix<rows>::TLUMatrix(const IMatrix *original) :
    Numerical::ILUMatrix::ILUMatrix(original),
    Numerical::TMatrix<Real, rows, rows>::TMatrix()
{

}

template <Data::UInt rows>
Numerical::TLUMatrix<rows>::TLUMatrix(const TLUMatrix<rows> &m) :
    Numerical::ILUMatrix::ILUMatrix(m.original),
    Numerical::TMatrix<Real, rows, rows>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TLUMatrix<rows>::~TLUMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TLUMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt Numerical::TLUMatrix<rows>::numCols() const
{
    return rows;
}

template <Data::UInt rows>
Data::Real &Numerical::TLUMatrix<rows>::element(UInt i, UInt j)
{
    return Numerical::TMatrix<Real, rows, rows>::element(i, j);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TLUMatrix<rows>::clone() const
{
    return new TLUMatrix<rows>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_TRIANGULAR_MATRIX_H_
