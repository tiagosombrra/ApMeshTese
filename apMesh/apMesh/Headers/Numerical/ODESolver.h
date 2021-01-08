#ifndef _NUMERICAL_ODE_SOLVER_H_
#define _NUMERICAL_ODE_SOLVER_H_

#include "Data/Definitions.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class ODESolver : public Performer::Builder
    {
    public:

        ODESolver();
        virtual ~ODESolver();

        virtual void setSystem(ODESystem *system);
        virtual ODESystem *getSystem() const;

        virtual void setStep(Real step);
        virtual Real getStep() const;

        virtual void setMin(Real min);
        virtual Real getMin() const;

        virtual void setMax(Real max);
        virtual Real getMax() const;

        virtual void setNumSteps(UInt numSteps);
        virtual UInt getNumSteps() const;

        virtual Real *getResult() const;

        //virtual Real *execute(ODESystem *system, Real step) = 0;

        //virtual void executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[]) = 0;
        virtual void next(Real t, Real prev[], Real next[]) = 0;

    protected:

        ODESystem *system;

        Real step;
        UInt numSteps;

        Real min;
        Real max;

        Real *result;

    };

    class SingleStepODESolver : public Numerical::ODESolver
    {
    public:

        SingleStepODESolver();
        virtual ~SingleStepODESolver();

        virtual void setOrder(UInt order);
        virtual UInt getOrder() const;

        //using Numerical::ODESolver::execute;
        //virtual Real *execute(ODESystem *system, Real step, UInt order) = 0;

    protected:

        UInt order;
    };

    class MultiStepODESolver : public Numerical::ODESolver
    {
    public:

        MultiStepODESolver();
        virtual ~MultiStepODESolver();

        virtual void setNumPoints(UInt numPoints);
        virtual UInt getNumPoints() const;

        virtual void setInitializer(SingleStepODESolver *initializer);
        virtual SingleStepODESolver *getInitializer() const;

        //using Numerical::ODESolver::execute;
        //virtual Real *execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer = NULL) = 0;

        static void moveLast(UInt numFunctions, UInt numPoints, Real prev[], Real next[]);
        static void move(UInt numFunctions, UInt numPoints, Real prev[], Real next[] = NULL);

    protected:

        UInt numPoints;

        SingleStepODESolver *initializer;
        bool createdInitializer;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ODE_SOLVER_H_
