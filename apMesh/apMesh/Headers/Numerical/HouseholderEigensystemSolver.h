#ifndef _NUMERICAL_HOUSEHOLDER_EIGENSYSTEM_SOLVER_H_
#define _NUMERICAL_HOUSEHOLDER_EIGENSYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EigensystemSolver.h"

namespace Numerical
{
    class HouseholderEigensystemSolver : public Numerical::EigensystemSolver
    {
    public:

        HouseholderEigensystemSolver();
        virtual ~HouseholderEigensystemSolver();

        virtual void setCopy(bool copy);
        virtual bool isCopy() const;

        virtual RowMatrix *getEigenvalues() const;

        virtual void execute();

    protected:

        RowMatrix *eigenvalues;

        bool copy;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_HOUSEHOLDER_EIGENSYSTEM_SOLVER_H_
