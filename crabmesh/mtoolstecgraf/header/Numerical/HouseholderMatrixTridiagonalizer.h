#ifndef _NUMERICAL_HOUSEHOLDER_MATRIX_TRIDIAGONALIZER_H_
#define _NUMERICAL_HOUSEHOLDER_MATRIX_TRIDIAGONALIZER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/MatrixTridiagonalizer.h"

namespace Numerical
{
    class HouseholderMatrixTridiagonalizer : public Numerical::MatrixTridiagonalizer
    {
    public:

        HouseholderMatrixTridiagonalizer();
        virtual ~HouseholderMatrixTridiagonalizer();

        virtual void execute();

    protected:

        virtual void tridiagonalize(UInt row, bool typeSymmetric, bool symmetric);
        virtual Real element(UInt row, UInt column, Real size);

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_HOUSEHOLDER_MATRIX_TRIDIAGONALIZER_H_
