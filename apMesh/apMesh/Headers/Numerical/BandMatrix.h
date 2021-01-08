#ifndef _NUMERICAL_BAND_MATRIX_H_
#define _NUMERICAL_BAND_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
	class IBandMatrix : public Numerical::IMatrix
	{
    public:

        virtual ~IBandMatrix();

        virtual Real getElement(UInt i, UInt j) const;
        virtual void setElement(UInt i, UInt j, Real element);

		virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
		virtual bool isLowerTriangular() const;
		virtual bool isUpperTriangular() const;
		virtual bool isDiagonal() const;
		virtual bool isBand() const;
		virtual bool isIdentity() const;
		virtual bool isZero() const;

		virtual UInt numElements() const;

        virtual UInt minCol(UInt i) const;
        virtual UInt maxCol(UInt i) const;

	};

	template <UInt rows, UInt lowerBand, UInt upperBand>
    class TBandMatrix :
        public Numerical::IBandMatrix,
        public Numerical::TMatrix<Real, rows, lowerBand + upperBand + 1>
    {
    public:

        TBandMatrix();
        TBandMatrix(const TBandMatrix<rows, lowerBand, upperBand> &m);
        virtual ~TBandMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

		virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);
    };

    class BandMatrix :
        public Numerical::IBandMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        BandMatrix(UInt rows, UInt lowerBand, UInt upperBand);
        BandMatrix(const BandMatrix &m);
        virtual ~BandMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

		virtual void resize(UInt rows, UInt lowerBand, UInt upperBand);

		virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    protected:

        UInt lowerBand;
        UInt upperBand;
    };

    class TridiagonalMatrix : public BandMatrix
    {
    public:

        TridiagonalMatrix(UInt rows);
        TridiagonalMatrix(const TridiagonalMatrix &m);
        virtual ~TridiagonalMatrix();

        virtual IMatrix *clone() const;

        virtual void resize(UInt rows);

    protected:

        using BandMatrix::resize;
    };
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Numerical::TBandMatrix<rows, lowerBand, upperBand>::TBandMatrix() :
    Numerical::IBandMatrix::IBandMatrix(),
    Numerical::TMatrix<Real, rows, lowerBand + upperBand + 1>::TMatrix()
{

}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Numerical::TBandMatrix<rows, lowerBand, upperBand>::TBandMatrix(const TBandMatrix<rows, lowerBand, upperBand> &m) :
    Numerical::IBandMatrix::IBandMatrix(),
    Numerical::TMatrix<Real, rows, lowerBand + upperBand + 1>::TMatrix(m)
{

}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Numerical::TBandMatrix<rows, lowerBand, upperBand>::~TBandMatrix()
{

}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Data::UInt Numerical::TBandMatrix<rows, lowerBand, upperBand>::numRows() const
{
    return rows;
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Data::UInt Numerical::TBandMatrix<rows, lowerBand, upperBand>::numCols() const
{
    return rows;
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Data::UInt Numerical::TBandMatrix<rows, lowerBand, upperBand>::lowerBandwidth() const
{
    return lowerBand;
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Data::UInt Numerical::TBandMatrix<rows, lowerBand, upperBand>::upperBandwidth() const
{
    return upperBand;
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Data::Real &Numerical::TBandMatrix<rows, lowerBand, upperBand>::element(UInt i, UInt j)
{
    return Numerical::TMatrix<Real, rows, lowerBand + upperBand + 1>::element(i, j);
}

template <Data::UInt rows, Data::UInt lowerBand, Data::UInt upperBand>
Numerical::IMatrix *Numerical::TBandMatrix<rows, lowerBand, upperBand>::clone() const
{
    return new TBandMatrix<rows, lowerBand, upperBand>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_BAND_MATRIX_H_
