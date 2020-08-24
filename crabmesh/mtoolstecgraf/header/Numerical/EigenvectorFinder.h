#ifndef _NUMERICAL_EIGENVECTOR_FINDER_H_
#define _NUMERICAL_EIGENVECTOR_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class EigenvectorFinder : public Performer::Builder
    {
    public:

        EigenvectorFinder();
        virtual ~EigenvectorFinder();

        virtual void setMatrix(const IMatrix *matrix);
        virtual const IMatrix *getMatrix() const;

        virtual void setEigenvalue(Real eigenvalue);
        virtual Real getEigenvalue() const;

        virtual IColumnMatrix *getEigenvector() const;

        virtual void execute();

    protected:

        const IMatrix *matrix;
        Real eigenvalue;

        IColumnMatrix *eigenvector;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_EIGENVECTOR_FINDER_H_
