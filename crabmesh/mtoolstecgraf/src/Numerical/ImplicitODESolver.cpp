#include "Numerical/ImplicitODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

Numerical::ImplicitODESolver::ImplicitODESolver() :
    Numerical::MultiStepODESolver::MultiStepODESolver()
{

}

Numerical::ImplicitODESolver::~ImplicitODESolver()
{

}

/*Real *Numerical::ImplicitODESolver::execute(ODESystem *system, Real step)
{
    return this->execute(system, step, 4);
}*/

#endif //#if USE_NUMERICAL
