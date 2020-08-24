#ifndef _NUMERICAL_SHIFTED_POWER_EIGENSYSTEM_SOLVER_H_
#define _NUMERICAL_SHIFTED_POWER_EIGENSYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/InversePowerEigensystemSolver.h"

namespace Numerical
{
    class ShiftedPowerEigensystemSolver : public Numerical::InversePowerEigensystemSolver
    {
    public:

        ShiftedPowerEigensystemSolver();
        virtual ~ShiftedPowerEigensystemSolver();

        virtual void setShift(Real shift);
        virtual Real getShift() const;

        virtual void execute();

    protected:

        Real shift;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_SHIFTED_POWER_EIGENSYSTEM_SOLVER_H_
