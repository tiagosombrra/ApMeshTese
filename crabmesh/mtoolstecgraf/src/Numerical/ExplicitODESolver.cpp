#include "Numerical/ExplicitODESolver.h"

#if USE_NUMERICAL

#include "Numerical/ODESystem.h"
#include "Data/OStream.h"

using namespace Data;
using namespace Numerical;

Numerical::ExplicitODESolver::ExplicitODESolver() :
    Numerical::MultiStepODESolver::MultiStepODESolver()
{

}

Numerical::ExplicitODESolver::~ExplicitODESolver()
{

}

/*Real *Numerical::ExplicitODESolver::execute(ODESystem *system, Real step)
{
    return this->execute(system, step, 4);
}*/

#endif //#if USE_NUMERICAL
