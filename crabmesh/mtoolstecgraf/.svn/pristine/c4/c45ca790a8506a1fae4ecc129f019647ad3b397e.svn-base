#include "Numerical/PredictorCorrectorODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Numerical/RungeKuttaODESolver.h"
#include "Numerical/AdamsBashforthODESolver.h"
#include "Numerical/AdamsMoultonODESolver.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::PredictorCorrectorODESolver::TOLERANCE = 0.000001;

Numerical::PredictorCorrectorODESolver::PredictorCorrectorODESolver()
{
    this->pred = NULL;
    this->correc = NULL;

    this->predictor = NULL;
    this->corrector = NULL;

    this->tolerance = this->TOLERANCE;
}

Numerical::PredictorCorrectorODESolver::~PredictorCorrectorODESolver()
{
    if (this->pred)
    {
        delete this->pred;
    }

    if (this->correc)
    {
        delete this->correc;
    }
}

void Numerical::PredictorCorrectorODESolver::setPredictor(MultiStepODESolver *predictor)
{
    this->predictor = predictor;
}

MultiStepODESolver *Numerical::PredictorCorrectorODESolver::getPredictor() const
{
    return this->predictor;
}

void Numerical::PredictorCorrectorODESolver::setCorrector(MultiStepODESolver *corrector)
{
    this->corrector = corrector;
}

MultiStepODESolver *Numerical::PredictorCorrectorODESolver::getCorrector() const
{
    return this->corrector;
}

void Numerical::PredictorCorrectorODESolver::setTolerance(Real tolerance)
{
    this->tolerance = tolerance;
}

Real Numerical::PredictorCorrectorODESolver::getTolerance() const
{
    return this->tolerance;
}

/*Real *Numerical::PredictorCorrectorODESolver::execute(ODESystem *system, Real step)
{
    return this->execute(system, step, this->TOLERANCE);
}

Real *Numerical::PredictorCorrectorODESolver::execute(ODESystem *system, Real step, Real tolerance)
{
    return this->execute(system, step, 4, tolerance);
}

Real *Numerical::PredictorCorrectorODESolver::execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer)
{
    return this->execute(system, step, numPoints, this->TOLERANCE, initializer);
}*/

//Real *Numerical::PredictorCorrectorODESolver::execute(ODESystem *system, Real step, UInt numPoints, Real tolerance,
//    SingleStepODESolver *initializer, MultiStepODESolver *predictor, MultiStepODESolver *corrector)
void Numerical::PredictorCorrectorODESolver::execute()
{
    /*if ((numPoints < 2) || (numPoints > 4))
    {
        return NULL;
    }*/

    UInt n = this->system->numFunctions();

    //Real min = system->min();
    //Real max = system->max();

    Real *next = new Real[n*this->numPoints];
    //Real *result = new Real[n];
    this->result = new Real[n];

    for (UInt i = 0; i < n; i++)
    {
        next[i] = this->system->initial(i);
    }

    if (this->numPoints > 1)
    {
        SingleStepODESolver *init = this->initializer;

        if (!this->initializer)
        {
            init = new Numerical::RungeKuttaODESolver();
        }

        init->setSystem(this->system);
        init->setStep(this->step);
        init->setOrder(this->numPoints);

        for (UInt j = 1; j < this->numPoints; j++)
        {
            //init->executeStep(system, step, min + static_cast<Real>(j-1)*step, numPoints, &next[(j-1)*n], &next[j*n]);
            init->next(min + static_cast<Real>(j-1)*step, &next[(j-1)*n], &next[j*n]);
        }

        if (!this->initializer)
        {
            delete init;
        }
    }

    this->pred = this->predictor;

    if (!this->predictor)
    {
        this->pred = new Numerical::AdamsBashforthODESolver();
    }

    this->pred->setSystem(this->system);
    this->pred->setStep(this->step);
    this->pred->setNumPoints(this->numPoints);

    this->correc = this->corrector;

    if (!this->corrector)
    {
        this->correc = new Numerical::AdamsMoultonODESolver();
    }

    this->correc->setSystem(this->system);
    this->correc->setStep(this->step);
    this->correc->setNumPoints(this->numPoints - 1);

    for (Real t = this->min + static_cast<Real>(this->numPoints - 1)*this->step; t < this->max; t += this->step)
    {
        //this->executeStep(system, step, numPoints, t, next, result, tolerance);
        this->next(t, next, result);
    }

    if (!this->predictor)
    {
        delete this->pred;

        this->pred = NULL;
    }

    if (!this->corrector)
    {
        delete this->correc;

        this->correc = NULL;
    }

    delete [] next;
}

/*void Numerical::PredictorCorrectorODESolver::executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[])
{
    this->executeStep(system, step, order, t, prev, next, this->TOLERANCE);
}*/

//void Numerical::PredictorCorrectorODESolver::executeStep(ODESystem *system, Real step, UInt numPoints, Real t, Real prev[], Real next[], Real tolerance)
void Numerical::PredictorCorrectorODESolver::next(Real t, Real prev[], Real next[])
{
    if (!this->pred)
    {
        this->pred = new Numerical::AdamsBashforthODESolver();
    }

    this->pred->setSystem(this->system);
    this->pred->setStep(this->step);
    this->pred->setNumPoints(this->numPoints);

    if (!this->correc)
    {
        this->correc = new Numerical::AdamsMoultonODESolver();
    }

    this->correc->setSystem(this->system);
    this->correc->setStep(this->step);
    this->correc->setNumPoints(this->numPoints - 1);

    UInt n = this->system->numFunctions();

    //this->pred->executeStep(system, step, t, numPoints, prev, next);
    this->pred->next(t, prev, next);

    this->move(n, this->numPoints, prev, next);

    Real max = 0.0;

    do
    {
        max = 0.0;

        //this->correc->executeStep(system, step, t, numPoints-1, prev, next);
        this->correc->next(t, prev, next);

        for (UInt j = 0; j < n; j++)
        {
            UInt pj = (this->numPoints - 1)*n + j;

            Real dif = std::fabs(prev[pj] - next[j]);

            if (dif > max)
            {
                max = dif;
            }

            prev[pj] = next[j];
        }
    }
    while (max > this->tolerance);
}

#endif //#if USE_NUMERICAL
