#ifndef _NUMERICAL_ADAMS_BASHFORTH_ODE_SOLVER_H_
#define _NUMERICAL_ADAMS_BASHFORTH_ODE_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/ExplicitODESolver.h"

namespace Numerical
{
    class AdamsBashforthODESolver : public Numerical::ExplicitODESolver
    {
    public:

        AdamsBashforthODESolver();
        virtual ~AdamsBashforthODESolver();

        //using Numerical::ExplicitODESolver::execute;
        //virtual Real *execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer = NULL);

        virtual void execute();

        //virtual void executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[]);
        //virtual void executeStep(ODESystem *system, Real step, UInt numPoints, Real t, Real prev[], Real next[]);

        virtual void next(Real t, Real prev[], Real next[]);

    protected:

        static const Real coefficients[6][6];

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ADAMS_BASHFORTH_ODE_SOLVER_H_
