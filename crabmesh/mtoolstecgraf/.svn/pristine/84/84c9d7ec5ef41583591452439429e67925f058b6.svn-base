#ifndef _NUMERICAL_PREDICTOR_CORRECTOR_ODE_SOLVER_H_
#define _NUMERICAL_PREDICTOR_CORRECTOR_ODE_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/ODESolver.h"

namespace Numerical
{
    class PredictorCorrectorODESolver : public Numerical::MultiStepODESolver
    {
    public:

        static const Real TOLERANCE;

    public:

        PredictorCorrectorODESolver();
        virtual ~PredictorCorrectorODESolver();

        virtual void setPredictor(MultiStepODESolver *predictor);
        virtual MultiStepODESolver *getPredictor() const;

        virtual void setCorrector(MultiStepODESolver *corrector);
        virtual MultiStepODESolver *getCorrector() const;

        virtual void setTolerance(Real tolerance);
        virtual Real getTolerance() const;

        //virtual Real *execute(ODESystem *system, Real step);
        //virtual Real *execute(ODESystem *system, Real step, Real tolerance);
        //virtual Real *execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer = NULL);
        //virtual Real *execute(ODESystem *system, Real step, UInt numPoints, Real tolerance,
        //    SingleStepODESolver *initializer = NULL, MultiStepODESolver *predictor = NULL, MultiStepODESolver *corrector = NULL);

        virtual void execute();

        //virtual void executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[]);

        virtual void next(Real t, Real prev[], Real next[]);

    //protected:

        //virtual void executeStep(ODESystem *system, Real step, UInt numPoints, Real t, Real prev[], Real next[], Real tolerance);

    protected:

        MultiStepODESolver *predictor;
        MultiStepODESolver *corrector;

        MultiStepODESolver *pred;
        MultiStepODESolver *correc;

        Real tolerance;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_PREDICTOR_CORRECTOR_ODE_SOLVER_H_
