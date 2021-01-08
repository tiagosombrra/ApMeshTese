#ifndef _NUMERICAL_TMATRIX_H_
#define _NUMERICAL_TMATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
    template <typename Type, UInt rows, UInt cols>
    class TMatrix
    {
    public:

        TMatrix();
        TMatrix(const TMatrix<Type, rows, cols> &m);

        //Type element(UInt i, UInt j) const;
        Type &element(UInt i, UInt j);

    protected:

        void copyFrom(const TMatrix<Type, rows, cols> &m);

    protected:

        Type elements[rows][cols];
    };

    template <typename Type>
    class TDMatrix
    {
    public:

        TDMatrix(UInt rows, UInt cols);
        TDMatrix(const TDMatrix<Type> &m);
        virtual ~TDMatrix();

        void allocate();
        void deallocate();
        void reallocate(UInt rows, UInt cols);

        Type &element(UInt i, UInt j);

    protected:

        void copyFrom(const TDMatrix<Type> &m);

    protected:

        UInt rows;
        UInt cols;

        Type *elements;
    };
}

template <typename Type, Data::UInt rows, Data::UInt cols>
Numerical::TMatrix<Type, rows, cols>::TMatrix()
{

}

template <typename Type, Data::UInt rows, Data::UInt cols>
Numerical::TMatrix<Type, rows, cols>::TMatrix(const TMatrix<Type, rows, cols> &m)
{
    this->copyFrom(m);
}

/*template <typename Type, Data::UInt rows, Data::UInt cols>
Type Numerical::TMatrix<Type, rows, cols>::element(UInt i, UInt j) const
{
    //return this->elements[i*cols + j];
    return this->elements[i][j];
}*/

template <typename Type, Data::UInt rows, Data::UInt cols>
Type &Numerical::TMatrix<Type, rows, cols>::element(UInt i, UInt j)
{
    //return this->elements[i*cols + j];
    return this->elements[i][j];
}

template <typename Type, Data::UInt rows, Data::UInt cols>
void Numerical::TMatrix<Type, rows, cols>::copyFrom(const TMatrix<Type, rows, cols> &m)
{
    /*for (UInt i = 0; i < rows*cols; i++)
    {
        this->elements[i] = m.elements[i];
    }*/

    for (UInt i = 0; i < rows; i++)
    {
        for (UInt j = 0; j < cols; j++)
        {
            this->elements[i][j] = m.elements[i][j];
        }
    }
}

template <typename Type>
Numerical::TDMatrix<Type>::TDMatrix(UInt rows, UInt cols)
{
    this->rows = rows;
    this->cols = cols;
    this->elements = NULL;

    this->allocate();
}

template <typename Type>
Numerical::TDMatrix<Type>::TDMatrix(const TDMatrix<Type> &m)
{
    this->rows = m.rows;
    this->cols = m.cols;
    this->elements = NULL;

    this->allocate();

    this->copyFrom(m);
}

template <typename Type>
Numerical::TDMatrix<Type>::~TDMatrix()
{
    this->deallocate();
}

template <typename Type>
void Numerical::TDMatrix<Type>::allocate()
{
    this->elements = new Type[this->rows*this->cols];
}

template <typename Type>
void Numerical::TDMatrix<Type>::deallocate()
{
    if (this->elements != NULL)
    {
        delete [] this->elements;

        this->elements = NULL;
    }
}

template <typename Type>
void Numerical::TDMatrix<Type>::reallocate(UInt rows, UInt cols)
{
    UInt tsize = this->rows*this->cols;
    UInt osize = rows*cols;

    if (tsize == osize)
    {
        return;
    }

    Type *elements = new Type[osize];

    UInt minrows = std::min(this->rows, rows);
    UInt mincols = std::min(this->cols, cols);

    for (UInt i = 0; i < minrows; i++)
    {
        for (UInt j = 0; j < mincols; j++)
        {
            elements[i*cols + j] = this->elements[i*this->cols + j];
        }
    }

    delete [] this->elements;

    this->elements = elements;

    this->rows = rows;
    this->cols = cols;
}

template <typename Type>
Type &Numerical::TDMatrix<Type>::element(UInt i, UInt j)
{
    return this->elements[i*this->cols + j];
}

template <typename Type>
void Numerical::TDMatrix<Type>::copyFrom(const TDMatrix<Type> &m)
{
    this->reallocate(m.rows, m.cols);

    for (UInt i = 0; i < this->rows*this->cols; i++)
    {
        this->elements[i] = m.elements[i];
    }
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_TMATRIX_H_
