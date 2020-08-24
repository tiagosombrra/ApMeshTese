#include "Numerical/NewtonRaphsonEquationRootFinder.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

using namespace Data;
using namespace Numerical;

Numerical::NewtonRaphsonEquationRootFinder::NewtonRaphsonEquationRootFinder() :
    Numerical::FixedPointEquationRootFinder::FixedPointEquationRootFinder()
{

}

Numerical::NewtonRaphsonEquationRootFinder::~NewtonRaphsonEquationRootFinder()
{

}

void Numerical::NewtonRaphsonEquationRootFinder::setDerivative(Function *derivative)
{
    this->step = derivative;
}

Function *Numerical::NewtonRaphsonEquationRootFinder::getDerivative() const
{
    return this->step;
}

//Real Numerical::NewtonRaphsonEquationRootFinder::next(FixedPointEquationRootFunction *function, Real x, Real fx)
Real Numerical::NewtonRaphsonEquationRootFinder::next(Real x, Real fx) const
{
    //return x - fx/static_cast<NewtonRaphsonEquationRootFunction *>(function)->derivative()->f(x);
    return x - fx/this->step->f(x);
}

#endif //#if USE_NUMERICAL
