#include "Numerical/ApproximationFunctions.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

using namespace Data;
using namespace Numerical;

Numerical::ApproximationFunctions::ApproximationFunctions()
{
    this->numFunctions = 0;
    this->functions = NULL;
}

Numerical::ApproximationFunctions::~ApproximationFunctions()
{
    delete [] this->functions;
}

void Numerical::ApproximationFunctions::setNumFunctions(UInt numFunctions)
{
    if (numFunctions == this->numFunctions)
    {
        return;
    }

    const Function **oldFunctions = this->functions;

    this->functions = new const Function*[numFunctions];

    if (this->numFunctions != 0)
    {
        UInt nf = std::min(numFunctions, this->numFunctions);

        for (UInt i = 0; i < nf; i++)
        {
            this->functions[i] = oldFunctions[i];
        }

        delete [] oldFunctions;
    }

    this->numFunctions = numFunctions;
}

UInt Numerical::ApproximationFunctions::getNumFunctions() const
{
    return this->numFunctions;
}

void Numerical::ApproximationFunctions::setFunction(UInt i, const Function *function)
{
    this->functions[i] = function;
}

const Function *Numerical::ApproximationFunctions::getFunction(UInt i) const
{
    return this->functions[i];
}

Real Numerical::ApproximationFunctions::function(UInt i, Real x) const
{
    return const_cast<Function *>(this->functions[i])->f(x);
}

#endif //#if USE_NUMERICAL
