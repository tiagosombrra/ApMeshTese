#ifndef _NUMERICAL_SYMMETRIC_MATRIX_H_
#define _NUMERICAL_SYMMETRIC_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
    class ISymmetricMatrix : virtual public Numerical::IMatrix
    {
    public:

        virtual ~ISymmetricMatrix();

        virtual void setElement(UInt i, UInt j, Real element);
        virtual Real getElement(UInt i, UInt j) const;

        virtual bool isSquare() const;
		virtual bool isSymmetric() const;

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt numElements() const;

        static constexpr UInt numElements2(UInt rows)
        {
            return rows*(rows + 1)/2;
        };

    protected:

        static UInt numRows(UInt numElements);
        static UInt numCols(UInt numElements);

        virtual Real &element(UInt i, UInt j);
        virtual Real &element(UInt i) = 0;

    };

    template <UInt rows>
    class TSymmetricMatrix :
        public Numerical::ISymmetricMatrix,
        public Numerical::TMatrix<Real, 1, Numerical::ISymmetricMatrix::numElements2(rows)>
    {
    public:

        TSymmetricMatrix();
        TSymmetricMatrix(const TSymmetricMatrix<rows> &m);
        virtual ~TSymmetricMatrix();

        virtual UInt numRows() const;
        virtual UInt numCols() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::ISymmetricMatrix::element;
        virtual Real &element(UInt i);
    };

    class SymmetricMatrix :
        public Numerical::ISymmetricMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        SymmetricMatrix(UInt rows);
        SymmetricMatrix(const SymmetricMatrix &m);
        virtual ~SymmetricMatrix();

        virtual UInt numElements() const;

		virtual void resize(UInt rows);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::ISymmetricMatrix::element;
        virtual Real &element(UInt i);

    };
}

template <Data::UInt rows>
Numerical::TSymmetricMatrix<rows>::TSymmetricMatrix() :
    Numerical::ISymmetricMatrix::ISymmetricMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ISymmetricMatrix::numElements2(rows)>::TMatrix()
{

}

template <Data::UInt rows>
Numerical::TSymmetricMatrix<rows>::TSymmetricMatrix(const TSymmetricMatrix<rows> &m) :
    Numerical::ISymmetricMatrix::ISymmetricMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ISymmetricMatrix::numElements2(rows)>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TSymmetricMatrix<rows>::~TSymmetricMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TSymmetricMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt Numerical::TSymmetricMatrix<rows>::numCols() const
{
    return rows;
}

template <Data::UInt rows>
Data::Real &Numerical::TSymmetricMatrix<rows>::element(UInt i)
{
    return Numerical::TMatrix<Real, 1, Numerical::ISymmetricMatrix::numElements2(rows)>::element(0, i);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TSymmetricMatrix<rows>::clone() const
{
    return new TSymmetricMatrix<rows>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_SYMMETRIC_MATRIX_H_
