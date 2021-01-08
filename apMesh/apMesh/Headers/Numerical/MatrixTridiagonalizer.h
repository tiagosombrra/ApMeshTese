#ifndef _NUMERICAL_MATRIX_TRIDIAGONALIZER_H_
#define _NUMERICAL_MATRIX_TRIDIAGONALIZER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class MatrixTridiagonalizer : public Performer::Builder
    {
    public:

        MatrixTridiagonalizer();
        virtual ~MatrixTridiagonalizer();

        virtual void setMatrix(IMatrix *matrix);
        virtual IMatrix *getMatrix() const;

        virtual void setCopy(bool copy);
        virtual bool isCopy() const;

        virtual IMatrix *getTridiagonalized() const;
        virtual TridiagonalMatrix *getTridiagonal() const;

    protected:

        IMatrix *matrix;

        IMatrix *tridiagonalized;
        mutable bool tridiagonalizedGot;

        bool copy;

        mutable TridiagonalMatrix *tridiagonal;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_MATRIX_TRIDIAGONALIZER_H_
