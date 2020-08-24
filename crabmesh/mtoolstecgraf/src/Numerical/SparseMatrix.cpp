#include "Numerical/SparseMatrix.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::SparseMatrixElement::SparseMatrixElement()
{
    this->col = -1;
    this->element = 0.0;
}

Numerical::SparseMatrixElement::~SparseMatrixElement()
{

}

Numerical::ISparseMatrix::~ISparseMatrix()
{

}

Real Numerical::ISparseMatrix::getElement(UInt i, UInt j) const
{
    UInt pos = 0;

    return (this->search(i, j, pos)) ? const_cast<Numerical::ISparseMatrix *>(this)->element(pos) : 0.0;
}

Real &Numerical::ISparseMatrix::element(UInt /*i*/, UInt /*j*/)
{
    static Real zero = 0.0;

    return zero;
}

bool Numerical::ISparseMatrix::search(UInt i, UInt j, UInt &position) const
{
    UInt min = this->minPosition(i);
    UInt max = this->maxPosition(i);

    position = min;

    if (max == min)
    {
        return false;
    }

    UInt col = const_cast<Numerical::ISparseMatrix *>(this)->column(position);

    if (col == j)
    {
        return true;
    }

    position = max - 1;

    if (max == min + 1)
    {
        ++position;

        return false;
    }

    col = const_cast<Numerical::ISparseMatrix *>(this)->column(position);

    if (col == j)
    {
        return true;
    }

    while (min < max)
    {
        position = (min + max)/2;

        col = const_cast<Numerical::ISparseMatrix *>(this)->column(position);

        if (col == j)
        {
            return true;
        }

        if (col > j)
        {
            max = position;
        }
        else
        {
            min = ++position;
        }
    }

    return false;
}

Numerical::SparseMatrix::SparseMatrix(UInt rows, UInt cols, UInt capacity) :
    Numerical::ISparseMatrix::ISparseMatrix(),
    Numerical::TDMatrix<SparseMatrixElement>::TDMatrix(1, capacity)
{
    this->rows = rows;
    this->cols = cols;

    this->sizes = new UInt[this->rows];

    for (UInt i = 0; i < this->rows; i++)
    {
        this->sizes[i] = 0;
    }
}

Numerical::SparseMatrix::SparseMatrix(const SparseMatrix &m) :
    Numerical::ISparseMatrix::ISparseMatrix(),
    Numerical::TDMatrix<SparseMatrixElement>::TDMatrix(m)
{
    this->rows = m.rows;
    this->cols = m.cols;

    this->sizes = new UInt[this->rows];

    for (UInt i = 0; i < this->rows; i++)
    {
        this->sizes[i] = m.sizes[i];
    }
}

Numerical::SparseMatrix::~SparseMatrix()
{
    delete [] this->sizes;
}

void Numerical::SparseMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((i >= this->numRows()) || (j >= this->numCols()))
    {
        return;
    }

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
            this->column(k) = this->column(k + 1);
        }

        for (UInt k = i; k < rows; k++)
        {
            this->decrementSize(k);
        }

        UInt newCapacacity = this->capacity()/2;

        if (newCapacacity == 0)
        {
            newCapacacity = 1;
        }

        if (this->numElements() < newCapacacity)
        {
            this->reallocate(1, newCapacacity);
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
        UInt newCapacity = 2*this->capacity();

        if (newCapacity/this->numCols() > 0)
        {
            newCapacity = this->capacity() + this->numCols();
        }

        UInt maxCapacity = this->numRows()*this->numCols();

        if (newCapacity > maxCapacity)
        {
            newCapacity = maxCapacity;
        }

        this->reallocate(1, newCapacity);
    }

    for (UInt k = pos; k < numElems; k++)
    {
        this->element(numElems - k + pos) = this->element(numElems - k + pos - 1);
        this->column(numElems - k + pos) = this->column(numElems - k + pos - 1);
    }

    this->element(pos) = element;
    this->column(pos) = j;

    for (UInt k = i; k < rows; k++)
    {
        this->incrementSize(k);
    }
}

UInt Numerical::SparseMatrix::numRows() const
{
    return this->rows;
}

UInt Numerical::SparseMatrix::numCols() const
{
    return this->cols;
}

UInt Numerical::SparseMatrix::capacity() const
{
    return this->Numerical::TDMatrix<SparseMatrixElement>::cols;
}

UInt Numerical::SparseMatrix::numElements(UInt row) const
{
    return (row == 0) ? this->sizes[row] : this->sizes[row] - this->sizes[row - 1];
}

UInt Numerical::SparseMatrix::numElements() const
{
    return this->sizes[rows-1];
}

void Numerical::SparseMatrix::reserve(UInt capacity)
{
    if (capacity == 0)
    {
        capacity = 1;
    }

    if (capacity < this->capacity())
    {
        return;
    }

    UInt maxCapacity = this->numRows()*this->numCols();

    if (capacity > maxCapacity)
    {
        capacity = maxCapacity;
    }

    this->reallocate(1, capacity);
}

void Numerical::SparseMatrix::resize(UInt rows, UInt cols, UInt capacity)
{
    if (capacity == 0)
    {
        capacity = 1;
    }

    if (capacity > rows*cols)
    {
        capacity = rows*cols;
    }

    if ((capacity < this->capacity()) && (this->capacity() < rows*cols))
    {
        capacity = this->capacity();
    }

    this->reallocate(1, capacity);

    this->cols = cols;

    if (this->rows != rows)
    {
        UInt *sizes = new UInt[rows];

        UInt min = std::min(this->rows, rows);

        for (UInt i = 0; i < min; i++)
        {
            sizes[i] = this->sizes[i];
        }

        for (UInt i = min; i < rows; i++)
        {
            sizes[i] = sizes[min - 1];
        }

        delete [] this->sizes;

        this->sizes = sizes;

        this->rows = rows;
    }
}

UInt Numerical::SparseMatrix::minPosition(UInt row) const
{
    return (row == 0) ? 0 : this->sizes[row - 1];
}

UInt Numerical::SparseMatrix::maxPosition(UInt row) const
{
    return this->sizes[row];
}

UInt &Numerical::SparseMatrix::column(UInt position)
{
    return const_cast<Numerical::SparseMatrix *>(this)->Numerical::TDMatrix<SparseMatrixElement>::element(0, position).col;
}

void Numerical::SparseMatrix::incrementSize(UInt row)
{
    ++this->sizes[row];
}

void Numerical::SparseMatrix::decrementSize(UInt row)
{
    --this->sizes[row];
}

Real &Numerical::SparseMatrix::element(UInt i)
{
    return this->Numerical::TDMatrix<SparseMatrixElement>::element(0, i).element;
}

IMatrix *Numerical::SparseMatrix::clone() const
{
    return new SparseMatrix(*this);
}

#endif //#if USE_NUMERICAL
