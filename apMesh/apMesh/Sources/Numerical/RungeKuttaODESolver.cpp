#include "Numerical/RungeKuttaODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::RungeKuttaODESolver::aEuler[1] = {1.0};

const Real Numerical::RungeKuttaODESolver::aHeun[2] = {0.5, 0.5};
const Real Numerical::RungeKuttaODESolver::pHeun[1] = {1.0};
const Real Numerical::RungeKuttaODESolver::qHeun[1] = {1.0};

const Real Numerical::RungeKuttaODESolver::aMidpoint[2] = {0.0, 1.0};
const Real Numerical::RungeKuttaODESolver::pMidpoint[1] = {0.5};
const Real Numerical::RungeKuttaODESolver::qMidpoint[1] = {0.5};

const Real Numerical::RungeKuttaODESolver::aRalston[2] = {1.0/3.0, 2.0/3.0};
const Real Numerical::RungeKuttaODESolver::pRalston[1] = {3.0/4.0};
const Real Numerical::RungeKuttaODESolver::qRalston[1] = {3.0/4.0};

const Real Numerical::RungeKuttaODESolver::a3rdOrder[3] = {1.0/6.0, 4.0/6.0, 1.0/6.0};
const Real Numerical::RungeKuttaODESolver::p3rdOrder[2] = {0.5, 1.0};
const Real Numerical::RungeKuttaODESolver::q3rdOrder[3] = {0.5, -1.0, 2.0};

const Real Numerical::RungeKuttaODESolver::a3rdOrder2[3] = {2.0/9.0, 3.0/9.0, 4.0/9.0};
const Real Numerical::RungeKuttaODESolver::p3rdOrder2[2] = {1.0/2.0, 3.0/4.0};
const Real Numerical::RungeKuttaODESolver::q3rdOrder2[3] = {1.0/2.0, 0.0, 3.0/4.0};

const Real Numerical::RungeKuttaODESolver::a4thOrder[4] = {1.0/6.0, 2.0/6.0, 2.0/6.0, 1.0/6.0};
const Real Numerical::RungeKuttaODESolver::p4thOrder[3] = {0.5, 0.5, 1.0};
const Real Numerical::RungeKuttaODESolver::q4thOrder[6] = {0.5, 0.0, 0.5, 0.0, 0.0, 1.0};

Numerical::RungeKuttaODESolver::RungeKuttaODESolver() :
    Numerical::SingleStepODESolver::SingleStepODESolver()
{

}

Numerical::RungeKuttaODESolver::~RungeKuttaODESolver()
{

}

/*Real *Numerical::RungeKuttaODESolver::execute(ODESystem *system, Real step)
{
    return this->execute(system, step, 4);
}

Real *Numerical::RungeKuttaODESolver::execute(ODESystem *system, Real step, UInt order)
{
    switch (order)
    {
    case 0:
    case 1:
        return this->executeEuler(system, step);
    case 2:
        return this->executeHeun(system, step);
    case 3:
        return this->execute3rdOrder(system, step);
    default:
        return this->execute4thOrder(system, step);
    }

    return NULL;
}*/

void Numerical::RungeKuttaODESolver::execute()
{
    switch (this->order)
    {
    case 0:
    case 1:
        this->executeEuler();
        break;
    case 2:
        this->executeHeun();
        break;
    case 3:
        this->execute3rdOrder();
        break;
    default:
        this->execute4thOrder();
        break;
    }
}

//Real *Numerical::RungeKuttaODESolver::executeEuler(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::executeEuler()
{
    this->order = 1;

    //return this->executeRungeKutta(system, step, 1, this->aEuler, NULL, NULL);
    this->executeRungeKutta(this->aEuler, NULL, NULL);
}

//Real *Numerical::RungeKuttaODESolver::executeHeun(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::executeHeun()
{
    this->order = 2;

    //return this->executeRungeKutta(system, step, 2, this->aHeun, this->pHeun, this->qHeun);
    this->executeRungeKutta(this->aHeun, this->pHeun, this->qHeun);
}

//Real *Numerical::RungeKuttaODESolver::executeMidpoint(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::executeMidpoint()
{
    this->order = 2;

    //return this->executeRungeKutta(system, step, 2, this->aMidpoint, this->pMidpoint, this->qMidpoint);
    this->executeRungeKutta(this->aMidpoint, this->pMidpoint, this->qMidpoint);
}

//Real *Numerical::RungeKuttaODESolver::executeRalston(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::executeRalston()
{
    this->order = 2;

    //return this->executeRungeKutta(system, step, 2, this->aRalston, this->pRalston, this->qRalston);
    this->executeRungeKutta(this->aRalston, this->pRalston, this->qRalston);
}

//Real *Numerical::RungeKuttaODESolver::execute3rdOrder(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::execute3rdOrder()
{
    this->order = 3;

    //return this->executeRungeKutta(system, step, 3, this->a3rdOrder, this->p3rdOrder, this->q3rdOrder);
    this->executeRungeKutta(this->a3rdOrder, this->p3rdOrder, this->q3rdOrder);
}

//Real *Numerical::RungeKuttaODESolver::execute3rdOrder2(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::execute3rdOrder2()
{
    this->order = 3;

    //return this->executeRungeKutta(system, step, 3, this->a3rdOrder2, this->p3rdOrder2, this->q3rdOrder2);
    this->executeRungeKutta(this->a3rdOrder2, this->p3rdOrder2, this->q3rdOrder2);
}

//Real *Numerical::RungeKuttaODESolver::execute4thOrder(ODESystem *system, Real step)
void Numerical::RungeKuttaODESolver::execute4thOrder()
{
    this->order = 4;

    //return this->executeRungeKutta(system, step, 4, this->a4thOrder, this->p4thOrder, this->q4thOrder);
    this->executeRungeKutta(this->a4thOrder, this->p4thOrder, this->q4thOrder);
}

//Real *Numerical::RungeKuttaODESolver::executeRungeKutta(ODESystem *system, Real step, UInt order, const Real *a, const Real *p, const Real *q)
void Numerical::RungeKuttaODESolver::executeRungeKutta(const Real *a, const Real *p, const Real *q)
{
    UInt n = this->system->numFunctions();

    //Real min = system->min();
    //Real max = system->max();

    this->result = new Real[n];
    Real *next = new Real[n];
    Real *aux = (this->order > 1) ? this->makeAuxiliaryVector() : NULL;

    for (UInt i = 0; i < n; i++)
    {
        this->result[i] = this->system->initial(i);
    }

    for (Real t = this->min; t < this->max; t += this->step)
    {
        //this->executeStepRungeKutta(system, step, t, result, next, aux, order, a, p, q);
        this->nextRungeKutta(t, this->result, next, aux, a, p, q);

        Real *tmp = this->result;
        this->result = next;
        next = tmp;
    }

    delete [] next;

    if (aux)
    {
        delete [] aux;
    }

    //return result;
}

//void Numerical::RungeKuttaODESolver::executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[])
void Numerical::RungeKuttaODESolver::next(Real t, Real prev[], Real next[])
{
    Real *aux = this->makeAuxiliaryVector();

    //this->executeStep(t, prev, next, aux);
    this->next(t, prev, next, aux);

    if (aux)
    {
        delete [] aux;
    }
}

//void Numerical::RungeKuttaODESolver::executeStep(ODESystem *system, Real step, Real t, UInt order, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::next(Real t, Real prev[], Real next[], Real *aux)
{
    switch (this->order)
    {
    case 0:
    case 1:
        //return this->executeStepEuler(system, step, t, prev, next);
        this->nextEuler(t, prev, next);
        break;
    case 2:
        //return this->executeStepHeun(system, step, t, prev, next, aux);
        this->nextHeun(t, prev, next, aux);
        break;
    case 3:
        //return this->executeStep3rdOrder(system, step, t, prev, next, aux);
        this->next3rdOrder(t, prev, next, aux);
        break;
    default:
        //return this->executeStep4thOrder(system, step, t, prev, next, aux);
        this->next4thOrder(t, prev, next, aux);
        break;
    }
}

//void Numerical::RungeKuttaODESolver::executeStepEuler(ODESystem *system, Real step, Real t, Real prev[], Real next[])
void Numerical::RungeKuttaODESolver::nextEuler(Real t, Real prev[], Real next[])
{
    this->order = 1;

    //this->executeStepRungeKutta(system, step, t, prev, next, NULL, 1, this->aEuler, NULL, NULL);
    this->nextRungeKutta(t, prev, next, NULL, this->aEuler, NULL, NULL);
}

//void Numerical::RungeKuttaODESolver::executeStepHeun(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::nextHeun(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 2;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 2, this->aHeun, this->pHeun, this->qHeun);
    this->nextRungeKutta(t, prev, next, aux, this->aHeun, this->pHeun, this->qHeun);
}

//void Numerical::RungeKuttaODESolver::executeStepMidpoint(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::nextMidpoint(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 2;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 2, this->aMidpoint, this->pMidpoint, this->qMidpoint);
    this->nextRungeKutta(t, prev, next, aux, this->aMidpoint, this->pMidpoint, this->qMidpoint);
}

//void Numerical::RungeKuttaODESolver::executeStepRalston(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::nextRalston(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 2;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 2, this->aRalston, this->pRalston, this->qRalston);
    this->nextRungeKutta(t, prev, next, aux, this->aRalston, this->pRalston, this->qRalston);
}

//void Numerical::RungeKuttaODESolver::executeStep3rdOrder(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::next3rdOrder(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 3;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 3, this->a3rdOrder, this->p3rdOrder, this->q3rdOrder);
    this->nextRungeKutta(t, prev, next, aux, this->a3rdOrder, this->p3rdOrder, this->q3rdOrder);
}

//void Numerical::RungeKuttaODESolver::executeStep3rdOrder2(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::next3rdOrder2(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 3;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 3, this->a3rdOrder2, this->p3rdOrder2, this->q3rdOrder2);
    this->nextRungeKutta(t, prev, next, aux, this->a3rdOrder2, this->p3rdOrder2, this->q3rdOrder2);
}

//void Numerical::RungeKuttaODESolver::executeStep4thOrder(ODESystem *system, Real step, Real t, Real prev[], Real next[], Real *aux)
void Numerical::RungeKuttaODESolver::next4thOrder(Real t, Real prev[], Real next[], Real *aux)
{
    this->order = 4;

    //this->executeStepRungeKutta(system, step, t, prev, next, aux, 4, this->a4thOrder, this->p4thOrder, this->q4thOrder);
    this->nextRungeKutta(t, prev, next, aux, this->a4thOrder, this->p4thOrder, this->q4thOrder);
}

//void Numerical::RungeKuttaODESolver::executeStepRungeKutta(ODESystem *system,
//    Real step, Real t, Real prev[], Real next[], Real *aux,
//    UInt order, const Real *a, const Real *p, const Real *q)
void Numerical::RungeKuttaODESolver::nextRungeKutta(Real t,
    Real prev[], Real next[], Real *aux,
    const Real *a, const Real *p, const Real *q)
{
    UInt n = this->system->numFunctions();

    Real *k[this->order-1];
    Real *y[this->order-1];

    for (UInt j = 1; j < this->order; j++)
    {
        k[j-1] = &aux[2*(j - 1)*n];
        y[j-1] = &aux[(2*j - 1)*n];
    }

    for (UInt j = 1; j < this->order; j++)
    {
        for (UInt i = 0; i < n; i++)
        {
            k[j-1][i] = (j == 1) ? this->system->f(i, t, prev) : this->system->f(i, t + p[j-1]*this->step, y[j-2]);

            y[j-1][i] = 0.0;

            for (UInt l = 0; l < j; l++)
            {
                y[j-1][i] += q[(j-1)*(j)/2 + l]*k[l][i];
            }

            y[j-1][i] *= this->step;
            y[j-1][i] += prev[i];
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        Real klast = (this->order == 1) ? 0.0 : this->system->f(i, t + p[this->order-2]*this->step, y[this->order-2]);

        Real s = 0.0;

        for (UInt j = 0; j < this->order; j++)
        {
            s += a[j]*((this->order == 1) ? this->system->f(i, t, prev) : ((j == this->order - 1) ? klast : k[j][i]));
        }

        next[i] = prev[i] + s*this->step;
    }
}

//Real *Numerical::RungeKuttaODESolver::makeAuxiliaryVector(UInt order, UInt n)
Real *Numerical::RungeKuttaODESolver::makeAuxiliaryVector()
{
    return (this->order < 2) ? NULL : new Real[2*(this->order-1)*this->system->numFunctions()];
}

#endif //#if USE_NUMERICAL
