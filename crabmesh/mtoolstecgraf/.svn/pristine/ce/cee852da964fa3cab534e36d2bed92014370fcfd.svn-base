#ifndef _NUMERICAL_POWER_EIGENSYSTEM_SOLVER_H_
#define _NUMERICAL_POWER_EIGENSYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/EigensystemSolver.h"

namespace Numerical
{
    class PowerEigensystemSolver : public EigensystemSolver
    {
    public:

        PowerEigensystemSolver();
        virtual ~PowerEigensystemSolver();

        virtual void setGuess(IColumnMatrix *guess);
        virtual IColumnMatrix *getGuess() const;

        virtual void setTolerance(Real tolerance);
        virtual Real getTolerance() const;

        virtual void setCopy(bool copy);
        virtual bool isCopy() const;

        virtual void setMaxSteps(UInt maxSteps);
        virtual UInt getMaxSteps() const;

        virtual void setTestingEigenvalue(bool testingEigenvalue);
        virtual bool isTestingEigenvalue() const;

        virtual void setTestingEigenvector(bool testingEigenvector);
        virtual bool isTestingEigenvector() const;

        virtual UInt getNumSteps() const;

        virtual IColumnMatrix *getEigenvector() const;

        virtual Real getEigenvalue() const;

    protected:

        IColumnMatrix *guess;

        Real tolerance;

        bool copy;

        UInt maxSteps;
        UInt numSteps;
        bool testingEigenvalue;
        bool testingEigenvector;

        IColumnMatrix *eigenvector;
        Real eigenvalue;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_POWER_EIGENSYSTEM_SOLVER_H_
