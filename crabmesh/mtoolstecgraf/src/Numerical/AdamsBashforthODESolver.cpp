#include "Numerical/AdamsBashforthODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Numerical/RungeKuttaODESolver.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::AdamsBashforthODESolver::coefficients[6][6] =
{
    {   1.0,            0.0,            0.0,            0.0,            0.0,           0.0},
    {-  1.0/   2.0,     3.0/   2.0,     0.0,            0.0,            0.0,           0.0},
    {   5.0/  12.0, -  16.0/  12.0,    23.0/  12.0,     0.0,            0.0,           0.0},
    {-  9.0/  24.0,    37.0/  24.0, -  59.0/  24.0,    55.0/  24.0,     0.0,           0.0},
    { 251.0/ 720.0, -1274.0/ 720.0,  2616.0/ 720.0, -2774.0/ 720.0,  1901.0/ 720.0,    0.0},
    {-475.0/1440.0,  2877.0/1440.0, -7298.0/1440.0,  9982.0/1440.0, -7923.0/1440.0, 4277.0/1440.0}
};

Numerical::AdamsBashforthODESolver::AdamsBashforthODESolver() :
    Numerical::ExplicitODESolver::ExplicitODESolver()
{

}

Numerical::AdamsBashforthODESolver::~AdamsBashforthODESolver()
{

}

//Real *Numerical::AdamsBashforthODESolver::execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer)
void Numerical::AdamsBashforthODESolver::execute()
{
    if (this->numPoints < 1)
    {
        this->numPoints = 1;
    }

    if (this->numPoints > 6)
    {
        this->numPoints = 6;
    }

    /*if ((numPoints < 1) || (numPoints > 6))
    {
        return NULL;
    }*/

    UInt n = this->system->numFunctions();

    //Real min = system->min();
    //Real max = system->max();

    Real *next = new Real[n*numPoints];
    //Real *result = new Real[n];
    this->result = new Real[n];

    for (UInt i = 0; i < n; i++)
    {
        next[i] = this->system->initial(i);
    }

    if (numPoints > 1)
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
            init->next(this->min + static_cast<Real>(j-1)*step, &next[(j-1)*n], &next[j*n]);
        }

        if (!this->initializer)
        {
            delete init;
        }
    }

    for (Real t = this->min + static_cast<Real>(this->numPoints - 1)*this->step; t < this->max; t += this->step)
    {
        //this->executeStep(system, step, numPoints, t, next, result);
        this->next(t, next, this->result);

        if (t + step < this->max)
        {
            this->move(n, this->numPoints, next, this->result);
        }
    }

    delete [] next;

    //return result;
}

/*void Numerical::AdamsBashforthODESolver::executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[])
{
    this->executeStep(system, step, order, t, prev, next);
}*/

//void Numerical::AdamsBashforthODESolver::executeStep(ODESystem *system, Real step, UInt numPoints, Real t, Real prev[], Real next[])
void Numerical::AdamsBashforthODESolver::next(Real t, Real prev[], Real next[])
{
    UInt n = this->system->numFunctions();

    for (UInt i = 0; i < n; i++)
    {
        next[i] = 0.0;

        for (UInt j = 0; j < this->numPoints; j++)
        {
            next[i] += this->coefficients[this->numPoints - 1][j]*this->system->f(i, t - (this->numPoints - j - 1)*this->step, &prev[j*n]);
        }

        next[i] *= step;
        next[i] += prev[n*(this->numPoints - 1) + i];
    }
}

#endif //#if USE_NUMERICAL
