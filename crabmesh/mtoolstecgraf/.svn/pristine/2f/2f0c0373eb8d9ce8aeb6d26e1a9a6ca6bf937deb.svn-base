#include "Numerical/SteffensenEquationRootFinder.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

using namespace Data;
using namespace Numerical;

Numerical::SteffensenEquationRootFinder::SteffensenEquationRootFinder() :
    Numerical::FixedPointEquationRootFinder::FixedPointEquationRootFinder()
{

}

Numerical::SteffensenEquationRootFinder::~SteffensenEquationRootFinder()
{

}

//Real Numerical::SteffensenEquationRootFinder::next(FixedPointEquationRootFunction *function, Real x, Real fx)
Real Numerical::SteffensenEquationRootFinder::next(Real x, Real fx) const
{
    //return x - fx*fx/(function->f(x + fx) - fx);
    return x - fx*fx/(this->function->f(x + fx) - fx);
}

#endif //#if USE_NUMERICAL
