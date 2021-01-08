#ifndef _NUMERICAL_QR_EIGENSYSTEM_SOLVER_H_
#define _NUMERICAL_QR_EIGENSYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EigensystemSolver.h"

namespace Numerical
{
    class QREigensystemSolver : public Numerical::EigensystemSolver
    {
    public:

        QREigensystemSolver();
        virtual ~QREigensystemSolver();

        virtual void setTolerance(Real tolerance);
        virtual Real getTolerance() const;

        virtual void setMaxSteps(UInt maxSteps);
        virtual UInt getMaxSteps() const;

        virtual UInt getNumSteps() const;

        virtual RowMatrix *getEigenvalues() const;

        virtual void execute();

    protected:

        RowMatrix *eigenvalues;

        Real tolerance;

        UInt maxSteps;
        UInt numSteps;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_QR_EIGENSYSTEM_SOLVER_H_
