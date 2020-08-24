#ifndef _NUMERICAL_EIGENVALUE_FINDER_H_
#define _NUMERICAL_EIGENVALUE_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class EigenvalueFinder : public Performer::Builder
    {
    public:

        EigenvalueFinder();
        virtual ~EigenvalueFinder();

        virtual void setMatrix(const IMatrix *matrix);
        virtual const IMatrix *getMatrix() const;

        virtual void setEigenvector(const IColumnMatrix *eigenvector);
        virtual const IColumnMatrix *getEigenvector() const;

        virtual Real getEigenvalue() const;

        virtual void execute();

    protected:

        const IMatrix *matrix;
        const IColumnMatrix *eigenvector;

        Real eigenvalue;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_EIGENVALUE_FINDER_H_
