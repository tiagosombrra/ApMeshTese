#include "Numerical/AdamsMoultonODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Numerical/RungeKuttaODESolver.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::AdamsMoultonODESolver::coefficients[5][6] =
{
    {  1.0/   2.0,    1.0/   2.0,    0.0,           0.0,           0.0,          0.0},
    {- 1.0/  12.0,    8.0/  12.0,    5.0/  12.0,    0.0,           0.0,          0.0},
    {  1.0/  24.0, -  5.0/  24.0,   19.0/  24.0,    9.0/  24.0,    0.0,          0.0},
    {-19.0/ 720.0,  106.0/ 720.0, -264.0/ 720.0,  646.0/ 720.0,  251.0/ 720.0,   0.0},
    { 27.0/1440.0, -173.0/1440.0,  482.0/1440.0, -798.0/1440.0, 1427.0/1440.0, 475.0/1440.0}
};

Numerical::AdamsMoultonODESolver::AdamsMoultonODESolver() :
    Numerical::ImplicitODESolver::ImplicitODESolver()
{

}

Numerical::AdamsMoultonODESolver::~AdamsMoultonODESolver()
{

}

//Real *Numerical::AdamsMoultonODESolver::execute(ODESystem *system, Real step, UInt numPoints, SingleStepODESolver *initializer)
void Numerical::AdamsMoultonODESolver::execute()
{
    /*if ((numPoints < 2) || (numPoints > 6))
    {
        return NULL;
    }

    numPoints--;*/

    if (this->numPoints < 1)
    {
        this->numPoints = 1;
    }

    if (this->numPoints > 5)
    {
        this->numPoints = 5;
    }

    /*if ((numPoints < 1) || (numPoints > 5))
    {
        return NULL;
    }*/

    UInt n = this->system->numFunctions();

    //Real min = this->system->min();
    //Real max = this->system->max();

    Real *next = new Real[n*(this->numPoints+1)];
    //Real *result = new Real[n];
    this->result = new Real[n];

    for (UInt i = 0; i < n; i++)
    {
        next[i] = this->system->initial(i);
    }

    SingleStepODESolver *init = this->initializer;

    if (!this->initializer)
    {
        init = new Numerical::RungeKuttaODESolver();
    }

    init->setSystem(this->system);
    init->setStep(this->step);
    init->setOrder(this->numPoints);

    if (this->numPoints > 1)
    {
        for (UInt j = 1; j < this->numPoints; j++)
        {
            //init->executeStep(system, step, min + static_cast<Real>(j-1)*step, numPoints, &next[(j-1)*n], &next[j*n]);
            init->next(this->min + static_cast<Real>(j-1)*step, &next[(j-1)*n], &next[j*n]);
        }
    }

    for (Real t = this->min + static_cast<Real>(this->numPoints - 1)*this->step; t < this->max; t += this->step)
    {
        //init->executeStep(system, step, t, numPoints, &next[(numPoints-1)*n], &next[numPoints*n]);
        init->next(t, &next[(numPoints-1)*n], &next[numPoints*n]);

        //this->executeStep(system, step, numPoints, t, next, result);
        this->next(t, next, result);

        if (t + this->step < this->max)
        {
            this->moveLast(n, this->numPoints+1, next, this->result);

            this->move(n, this->numPoints+1, next);
        }
    }

    if (!initializer)
    {
        delete init;
    }

    delete [] next;
}

/*void Numerical::AdamsMoultonODESolver::executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[])
{
    this->executeStep(system, step, order, t, prev, next);
}*/

//void Numerical::AdamsMoultonODESolver::executeStep(ODESystem *system, Real step, UInt numPoints, Real t, Real prev[], Real next[])
void Numerical::AdamsMoultonODESolver::next(Real t, Real prev[], Real next[])
{
    UInt n = this->system->numFunctions();

    for (UInt i = 0; i < n; i++)
    {
        next[i] = 0.0;

        for (UInt j = 0; j <= this->numPoints; j++)
        {
            next[i] += this->coefficients[this->numPoints - 1][j]*this->system->f(i, t - (this->numPoints - j - 1)*this->step, &prev[j*n]);
        }

        next[i] *= this->step;
        next[i] += prev[n*(this->numPoints - 1) + i];
    }
}

#endif //#if USE_NUMERICAL
