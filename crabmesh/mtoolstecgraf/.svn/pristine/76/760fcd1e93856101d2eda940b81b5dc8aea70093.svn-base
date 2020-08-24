#ifndef _NUMERICAL_ZERO_MATRIX_H_
#define _NUMERICAL_ZERO_MATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"

namespace Numerical
{
	class ZeroMatrix : public Numerical::IMatrix
	{
    public:

        ZeroMatrix(UInt rows);
        ZeroMatrix(UInt rows, UInt cols);
        ZeroMatrix(const ZeroMatrix &m);
        virtual ~ZeroMatrix();

		virtual UInt numRows() const;
		virtual UInt numCols() const;
		virtual Real getElement(UInt i, UInt j) const;

		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
		virtual bool isLowerTriangular() const;
		virtual bool isUpperTriangular() const;
		virtual bool isDiagonal() const;
		virtual bool isBand() const;
		virtual bool isIdentity() const;
		virtual bool isZero() const;

		virtual UInt numElements() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

		virtual IMatrix *clone() const;

    protected:

		virtual void setElement(UInt i, UInt j, Real element);
        virtual Real &element(UInt i, UInt j);

    protected:

        UInt rows;
        UInt cols;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ZERO_MATRIX_H_
