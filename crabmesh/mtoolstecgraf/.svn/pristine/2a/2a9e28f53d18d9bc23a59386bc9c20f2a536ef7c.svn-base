#ifndef _NUMERICAL_IMATRIX_H_
#define _NUMERICAL_IMATRIX_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
	class IMatrix
	{
    public:

        virtual ~IMatrix();

		virtual UInt numRows() const = 0;
		virtual UInt numCols() const = 0;
		virtual Real getElement(UInt i, UInt j) const;
		virtual void setElement(UInt i, UInt j, Real element);

		virtual bool isSquare() const;
		virtual bool isSymmetric() const;
		virtual bool isTriangular() const;
		virtual bool isLowerTriangular() const;
		virtual bool isUpperTriangular() const;
		virtual bool isDiagonal() const;
		virtual bool isColumn() const;
		virtual bool isRow() const;
		virtual bool isBand() const;
		virtual bool isIdentity() const;
		virtual bool isZero() const;

		virtual UInt numElements() const;

		virtual UInt lowerBandwidth() const;
		virtual UInt upperBandwidth() const;

		virtual std::string text() const;

		virtual IMatrix *clone() const = 0;

    protected:

        virtual Real &element(UInt i, UInt j) = 0;

    public:

        static bool isMultipliable(const IMatrix *first, const IMatrix *second);
        static bool isMultipliable(const IMatrix &first, const IMatrix &second);
        static bool isSummable(const IMatrix *first, const IMatrix *second);
        static bool isSummable(const IMatrix &first, const IMatrix &second);

        static void multiply(const IMatrix *first, const IMatrix *second, IMatrix *result);
        static void multiply(const IMatrix &first, const IMatrix &second, IMatrix &result);
        static void multiply(const IMatrix *first, const IMatrix *second, IMatrix *result, UInt frow, UInt fcol, UInt srow, UInt scol, UInt rrow, UInt rcol, UInt fnrows, UInt sncols, UInt nelems);
        static void multiply(const IMatrix &first, const IMatrix &second, IMatrix &result, UInt frow, UInt fcol, UInt srow, UInt scol, UInt rrow, UInt rcol, UInt fnrows, UInt sncols, UInt nelems);

        static void multiply(const IPermutationMatrix *first, const IMatrix *second, IMatrix *result);
        static void multiply(const IPermutationMatrix &first, const IMatrix &second, IMatrix &result);
        static void multiply(const IMatrix *first, const IPermutationMatrix *second, IMatrix *result);
        static void multiply(const IMatrix &first, const IPermutationMatrix &second, IMatrix &result);

        static void multiply(const IColumnMatrix *first, const IRowMatrix *second, Real &value);
        static void multiply(const IColumnMatrix &first, const IRowMatrix &second, Real &value);
        static void multiply(const IColumnMatrix *first, const IColumnMatrix *second, Real &value);
        static void multiply(const IColumnMatrix &first, const IColumnMatrix &second, Real &value);
        static void multiply(const IRowMatrix *first, const IRowMatrix *second, Real &value);
        static void multiply(const IRowMatrix &first, const IRowMatrix &second, Real &value);

        static void multiply(const IMatrix *first, Real constant, IMatrix *result);
        static void multiply(const IMatrix &first, Real constant, IMatrix &result);
        static void multiply(const IMatrix *first, Real constant, IMatrix *result, UInt frow, UInt fcol, UInt rrow, UInt rcol, UInt fnrows, UInt fncols);
        static void multiply(const IMatrix &first, Real constant, IMatrix &result, UInt frow, UInt fcol, UInt rrow, UInt rcol, UInt fnrows, UInt fncols);

        static void sum(const IMatrix *first, const IMatrix *second, IMatrix *result);
        static void sum(const IMatrix &first, const IMatrix &second, IMatrix &result);
        static void sum(const IMatrix *first, const IMatrix *second, IMatrix *result, UInt rmin, UInt rmax, UInt cmin, UInt cmax);
        static void sum(const IMatrix &first, const IMatrix &second, IMatrix &result, UInt rmin, UInt rmax, UInt cmin, UInt cmax);

        static void copy(const IMatrix *from, IMatrix *to);
        static void copy(const IMatrix &from, IMatrix &to);
        static void copy(const IMatrix *from, IMatrix *to, UInt rmin, UInt rmax, UInt cmin, UInt cmax);
        static void copy(const IMatrix &from, IMatrix &to, UInt rmin, UInt rmax, UInt cmin, UInt cmax);

    /*public:

        static const Real zero;*/

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_IMATRIX_H_
