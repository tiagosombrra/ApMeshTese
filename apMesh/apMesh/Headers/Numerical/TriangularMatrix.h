#ifndef _NUMERICAL_TRIANGULAR_MATRIX_H_
#define _NUMERICAL_TRIANGULAR_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
    class ITriangularMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~ITriangularMatrix();

        virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt numElements() const;

    protected:

        static constexpr UInt numElements2(UInt rows);
        static UInt numRows(UInt numElements);
        static UInt numCols(UInt numElements);

        using Numerical::IMatrix::element;
        virtual Real &element(UInt i) = 0;

    };

    class ILowerTriangularMatrix : public Numerical::ITriangularMatrix
    {
    public:

        virtual ~ILowerTriangularMatrix();

        virtual Real getElement(UInt i, UInt j) const;
        virtual void setElement(UInt i, UInt j, Real element);

		virtual bool isLowerTriangular() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i, UInt j);
    };

    class IUpperTriangularMatrix : public Numerical::ITriangularMatrix
    {
    public:

        virtual ~IUpperTriangularMatrix();

        virtual Real getElement(UInt i, UInt j) const;
        virtual void setElement(UInt i, UInt j, Real element);

		virtual bool isUpperTriangular() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i, UInt j);
    };


    template <UInt rows>
    class TLowerTriangularMatrix :
        public Numerical::ILowerTriangularMatrix,
        public Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>
    {
    public:

        TLowerTriangularMatrix();
        TLowerTriangularMatrix(const TLowerTriangularMatrix<rows> &m);
        virtual ~TLowerTriangularMatrix();

        virtual UInt numRows() const;
        virtual UInt numCols() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i);
    };

    class LowerTriangularMatrix :
        public Numerical::ILowerTriangularMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        LowerTriangularMatrix(UInt rows);
        LowerTriangularMatrix(const LowerTriangularMatrix &m);
        virtual ~LowerTriangularMatrix();

        virtual UInt numElements() const;

		virtual void resize(UInt rows);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i);

    };

    template <UInt rows>
    class TUpperTriangularMatrix :
        public Numerical::IUpperTriangularMatrix,
        public Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>
    {
    public:

        TUpperTriangularMatrix();
        TUpperTriangularMatrix(const TUpperTriangularMatrix<rows> &m);
        virtual ~TUpperTriangularMatrix();

        virtual UInt numRows() const;
        virtual UInt numCols() const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i);
    };

    class UpperTriangularMatrix :
        public Numerical::IUpperTriangularMatrix,
        public Numerical::TDMatrix<Real>
    {
    public:

        UpperTriangularMatrix(UInt rows);
        UpperTriangularMatrix(const UpperTriangularMatrix &m);
        virtual ~UpperTriangularMatrix();

        virtual UInt numElements() const;

		virtual void resize(UInt rows);

		virtual IMatrix *clone() const;

    protected:

        using Numerical::ITriangularMatrix::element;
        virtual Real &element(UInt i);

    };
}

template <Data::UInt rows>
Numerical::TLowerTriangularMatrix<rows>::TLowerTriangularMatrix() :
    Numerical::ILowerTriangularMatrix::ILowerTriangularMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::TMatrix()
{

}

template <Data::UInt rows>
Numerical::TLowerTriangularMatrix<rows>::TLowerTriangularMatrix(const TLowerTriangularMatrix<rows> &m) :
    Numerical::ILowerTriangularMatrix::ILowerTriangularMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TLowerTriangularMatrix<rows>::~TLowerTriangularMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TLowerTriangularMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt Numerical::TLowerTriangularMatrix<rows>::numCols() const
{
    return rows;
}

template <Data::UInt rows>
Data::Real &Numerical::TLowerTriangularMatrix<rows>::element(UInt i)
{
    return Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::element(0, i);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TLowerTriangularMatrix<rows>::clone() const
{
    return new TLowerTriangularMatrix<rows>(*this);
}

template <Data::UInt rows>
Numerical::TUpperTriangularMatrix<rows>::TUpperTriangularMatrix() :
    Numerical::IUpperTriangularMatrix::IUpperTriangularMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::TMatrix()
{

}

template <Data::UInt rows>
Numerical::TUpperTriangularMatrix<rows>::TUpperTriangularMatrix(const TUpperTriangularMatrix<rows> &m) :
    Numerical::IUpperTriangularMatrix::IUpperTriangularMatrix(),
    Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::TMatrix(m)
{

}

template <Data::UInt rows>
Numerical::TUpperTriangularMatrix<rows>::~TUpperTriangularMatrix()
{

}

template <Data::UInt rows>
Data::UInt Numerical::TUpperTriangularMatrix<rows>::numRows() const
{
    return rows;
}

template <Data::UInt rows>
Data::UInt Numerical::TUpperTriangularMatrix<rows>::numCols() const
{
    return rows;
}

template <Data::UInt rows>
Data::Real &Numerical::TUpperTriangularMatrix<rows>::element(UInt i)
{
    return Numerical::TMatrix<Real, 1, Numerical::ITriangularMatrix::numElements2(rows)>::element(0, i);
}

template <Data::UInt rows>
Numerical::IMatrix *Numerical::TUpperTriangularMatrix<rows>::clone() const
{
    return new TUpperTriangularMatrix<rows>(*this);
}

constexpr Data::UInt Numerical::ITriangularMatrix::numElements2(Data::UInt rows)
{
    return rows*(rows + 1)/2;
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_TRIANGULAR_MATRIX_H_
