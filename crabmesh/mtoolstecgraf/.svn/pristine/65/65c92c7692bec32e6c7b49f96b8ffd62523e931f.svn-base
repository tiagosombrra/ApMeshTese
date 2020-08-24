#ifndef _NUMERICAL_RUNGE_KUTTA_ODE_SOLVER_H_
#define _NUMERICAL_RUNGE_KUTTA_ODE_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/ODESolver.h"

namespace Numerical
{
    class RungeKuttaODESolver : public Numerical::SingleStepODESolver
    {
    public:

        RungeKuttaODESolver();
        virtual ~RungeKuttaODESolver();

        /*virtual Real *execute(ODESystem *system, Real step);
        virtual Real *execute(ODESystem *system, Real step, UInt order);

        virtual Real *executeEuler(ODESystem *system, Real step);
        virtual Real *executeHeun(ODESystem *system, Real step);
        virtual Real *executeMidpoint(ODESystem *system, Real step);
        virtual Real *executeRalston(ODESystem *system, Real step);
        virtual Real *execute3rdOrder(ODESystem *system, Real step);
        virtual Real *execute3rdOrder2(ODESystem *system, Real step);
        virtual Real *execute4thOrder(ODESystem *system, Real step);

        virtual Real *executeRungeKutta(ODESystem *system, Real step, UInt order, const Real *a, const Real *p, const Real *q);

        virtual void executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[]);
        virtual void executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[], Real *aux);

        virtual void executeStepEuler(ODESystem *system, Real step, Real t, Real prev[], Real next[]);
        virtual void executeStepHeun(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);
        virtual void executeStepMidpoint(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);
        virtual void executeStepRalston(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);
        virtual void executeStep3rdOrder(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);
        virtual void executeStep3rdOrder2(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);
        virtual void executeStep4thOrder(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux);

        virtual void executeStepRungeKutta(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux,
            UInt order, const Real *a, const Real *p, const Real *q);

        virtual Real *makeAuxiliaryVector(UInt order, UInt n);*/

        virtual void execute();

        virtual void executeEuler();
        virtual void executeHeun();
        virtual void executeMidpoint();
        virtual void executeRalston();
        virtual void execute3rdOrder();
        virtual void execute3rdOrder2();
        virtual void execute4thOrder();

        virtual void executeRungeKutta(const Real *a, const Real *p, const Real *q);

        virtual void next(Real t, Real prev[], Real next[]);
        virtual void next(Real t, Real prev[], Real next[], Real *aux);

        virtual void nextEuler(Real t, Real prev[], Real next[]);
        virtual void nextHeun(Real t, Real prev[], Real next[], Real *aux);
        virtual void nextMidpoint(Real t, Real prev[], Real next[], Real *aux);
        virtual void nextRalston(Real t, Real prev[], Real next[], Real *aux);
        virtual void next3rdOrder(Real t, Real prev[], Real next[], Real *aux);
        virtual void next3rdOrder2(Real t, Real prev[], Real next[], Real *aux);
        virtual void next4thOrder(Real t, Real prev[], Real next[], Real *aux);

        virtual void nextRungeKutta(Real t, Real prev[], Real next[], Real *aux, const Real *a, const Real *p, const Real *q);

        virtual Real *makeAuxiliaryVector();

    protected:

        static const Real aEuler[1];

        static const Real aHeun[2];
        static const Real pHeun[1];
        static const Real qHeun[1];

        static const Real aMidpoint[2];
        static const Real pMidpoint[1];
        static const Real qMidpoint[1];

        static const Real aRalston[2];
        static const Real pRalston[1];
        static const Real qRalston[1];

        static const Real a3rdOrder[3];
        static const Real p3rdOrder[2];
        static const Real q3rdOrder[3];

        static const Real a3rdOrder2[3];
        static const Real p3rdOrder2[2];
        static const Real q3rdOrder2[3];

        static const Real a4thOrder[4];
        static const Real p4thOrder[3];
        static const Real q4thOrder[6];

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_RUNGE_KUTTA_ODE_SOLVER_H_
