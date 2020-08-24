#ifndef _NUMERICAL_SPARSE_MATRIX_H_
#define _NUMERICAL_SPARSE_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/TMatrix.h"

namespace Numerical
{
    class SparseMatrixElement
    {
    public:
        SparseMatrixElement();
        ~SparseMatrixElement();

    public:

        UInt col;
        Real element;
    };

    class ISparseMatrix : public Numerical::IMatrix
    {
    public:

        virtual ~ISparseMatrix();

		virtual Real getElement(UInt i, UInt j) const;

        virtual UInt capacity() const = 0;
        using Numerical::IMatrix::numElements;
		virtual UInt numElements(UInt row) const = 0;

    protected:

		virtual UInt minPosition(UInt row) const = 0;
		virtual UInt maxPosition(UInt row) const = 0;
		virtual UInt &column(UInt position) = 0;

		virtual bool search(UInt i, UInt j, UInt &position) const;

		virtual void incrementSize(UInt row) = 0;
		virtual void decrementSize(UInt row) = 0;

		virtual Real &element(UInt i, UInt j);
		virtual Real &element(UInt i) = 0;

    };

    template <UInt rows, UInt cols, UInt cap>
    class TSparseMatrix :
        public Numerical::ISparseMatrix,
        public Numerical::TMatrix<SparseMatrixElement, 1, cap>
    {
    public:

        TSparseMatrix();
        TSparseMatrix(const TSparseMatrix<rows, cols, cap> &m);
        virtual ~TSparseMatrix();

        virtual void setElement(UInt i, UInt j, Real element);

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt capacity() const;
		virtual UInt numElements(UInt row) const;

		virtual UInt numElements() const;

		virtual IMatrix *clone() const;

    protected:

        virtual UInt minPosition(UInt row) const;
		virtual UInt maxPosition(UInt row) const;
		virtual UInt &column(UInt position);

		virtual void incrementSize(UInt row);
		virtual void decrementSize(UInt row);

        using Numerical::ISparseMatrix::element;
        virtual Real &element(UInt i) = 0;

    protected:

        UInt sizes[rows];
    };

    class SparseMatrix :
        public Numerical::ISparseMatrix,
        public Numerical::TDMatrix<SparseMatrixElement>
    {
    public:

        SparseMatrix(UInt rows, UInt cols, UInt capacity = 0);
        SparseMatrix(const SparseMatrix &m);
        virtual ~SparseMatrix();

		virtual void setElement(UInt i, UInt j, Real element);

        virtual UInt numRows() const;
		virtual UInt numCols() const;

		virtual UInt capacity() const;
		virtual UInt numElements(UInt row) const;

		virtual UInt numElements() const;

		virtual void reserve(UInt capacity);
		virtual void resize(UInt rows, UInt cols, UInt capacity);

		virtual IMatrix *clone() const;

    protected:

        virtual UInt minPosition(UInt row) const;
		virtual UInt maxPosition(UInt row) const;
		virtual UInt &column(UInt position);

		virtual void incrementSize(UInt row);
		virtual void decrementSize(UInt row);

        using Numerical::ISparseMatrix::element;
        virtual Real &element(UInt i);

    protected:

        UInt *sizes;

        UInt rows;
        UInt cols;
    };
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Numerical::TSparseMatrix<rows, cols, cap>::TSparseMatrix() :
    Numerical::ISparseMatrix::ISparseMatrix(),
    Numerical::TMatrix<SparseMatrixElement, 1, cap>::TMatrix()
{
    for (UInt i = 0; i < rows; i++)
    {
        this->sizes[i] = 0;
    }
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Numerical::TSparseMatrix<rows, cols, cap>::TSparseMatrix(const TSparseMatrix<rows, cols, cap> &m) :
    Numerical::ISparseMatrix::ISparseMatrix(),
    Numerical::TMatrix<SparseMatrixElement, 1, cap>::TMatrix(m)
{
    for (UInt i = 0; i < rows; i++)
    {
        this->sizes[i] = m.sizes[i];
    }
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Numerical::TSparseMatrix<rows, cols, cap>::~TSparseMatrix()
{

}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
void Numerical::TSparseMatrix<rows, cols, cap>::setElement(Data::UInt i, Data::UInt j, Data::Real element)
{
    UInt pos = 0;

    bool found = this->search(i, j, pos);

    if (found)
    {
        if (element != 0.0)
        {
            this->element(pos) = element;
            this->column(pos) = j;

            return;
        }

        UInt numElems = this->numElements();

        for (UInt k = pos; k < numElems; k++)
        {
            this->element(k) = this->element(k + 1);
            this->column(k) = this->column(k+1);
        }

        for (UInt k = i; k < rows; k++)
        {
            this->decrementSize(k);
        }

        return;
    }

    if (element == 0.0)
    {
        return;
    }

    UInt numElems = this->numElements();

    if (numElems >= this->capacity())
    {
        return;
    }

    for (UInt k = pos; k < numElems; k++)
    {
        this->element(numElems - k) = this->element(numElems - k - 1);
        this->column(numElems - k) = this->column(numElems - k - 1);
    }

    this->element(pos) = element;
    this->column(pos) = j;

    for (UInt k = i; k < rows; k++)
    {
        this->incrementSize(k);
    }
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::numRows() const
{
    return rows;
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::numCols() const
{
    return cols;
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::capacity() const
{
    return cap;
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::numElements(Data::UInt row) const
{
    return (row == 0) ? this->sizes[row] : this->sizes[row] - this->sizes[row - 1];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::numElements() const
{
    return this->sizes[rows-1];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::minPosition(UInt row) const
{
    return (row == 0) ? 0 : this->sizes[row - 1];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt Numerical::TSparseMatrix<rows, cols, cap>::maxPosition(UInt row) const
{
    return this->sizes[row];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::UInt &Numerical::TSparseMatrix<rows, cols, cap>::column(UInt position)
{
    return this->Numerical::TMatrix<SparseMatrixElement, 1, cap>::element(0, position).col;
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
void Numerical::TSparseMatrix<rows, cols, cap>::incrementSize(UInt row)
{
    ++this->sizes[row];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
void Numerical::TSparseMatrix<rows, cols, cap>::decrementSize(UInt row)
{
    --this->sizes[row];
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Data::Real &Numerical::TSparseMatrix<rows, cols, cap>::element(UInt i)
{
    return this->Numerical::TMatrix<SparseMatrixElement, 1, cap>::element(0, i).element;
}

template <Data::UInt rows, Data::UInt cols, Data::UInt cap>
Numerical::IMatrix *Numerical::TSparseMatrix<rows, cols, cap>::clone() const
{
    return new TSparseMatrix<rows, cols, cap>(*this);
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_SPARSE_MATRIX_H_
