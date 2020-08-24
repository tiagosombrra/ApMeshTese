#ifndef _NUMERICAL_JACOBI_ROTATION_MATRIX_H_
#define _NUMERICAL_JACOBI_ROTATION_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"

namespace Numerical
{
    class JacobiRotationMatrix : public Numerical::IMatrix
    {
    public:

        JacobiRotationMatrix();
        JacobiRotationMatrix(const JacobiRotationMatrix &m);
        virtual ~JacobiRotationMatrix();

        virtual void setElement(UInt i, UInt j, Real element);
        virtual Real getElement(UInt i, UInt j) const;

        virtual UInt numRows() const;
		virtual UInt numCols() const;

        virtual void reset(const IMatrix *matrix, UInt row, UInt col);

        virtual void applyOn(IMatrix *matrix) const;

        virtual void postmultiplyOn(IMatrix *matrix) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    protected:

        Real cos;
        Real sin;

        UInt row;
        UInt col;

        UInt rows;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_JACOBI_ROTATION_MATRIX_H_
