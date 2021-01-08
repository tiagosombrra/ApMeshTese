#ifndef _NUMERICAL_HOUSEHOLDER_MATRIX_H_
#define _NUMERICAL_HOUSEHOLDER_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/SymmetricMatrix.h"

namespace Numerical
{
    class HouseholderMatrix : public Numerical::SymmetricMatrix
    {
    public:

        HouseholderMatrix();
        virtual ~HouseholderMatrix();

        virtual void setElement(UInt i, UInt j, Real element);
        virtual Real getElement(UInt i, UInt j) const;

        virtual UInt numRows() const;
        virtual UInt numCols() const;

        using SymmetricMatrix::numElements;
        virtual UInt numElements() const;

        virtual void reset(const IMatrix *matrix, UInt row, UInt col, bool above = false, bool below = true);

    protected:

        using Numerical::SymmetricMatrix::resize;

        using Numerical::SymmetricMatrix::element;
        virtual Real &element(UInt i, UInt j);

    protected:

        UShort type;
        UInt row;
        UInt realRows;
        UInt originalRows;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_HOUSEHOLDER_MATRIX_H_
