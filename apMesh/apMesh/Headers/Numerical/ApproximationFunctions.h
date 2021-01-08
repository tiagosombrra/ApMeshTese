#ifndef _NUMERICAL_APPROXIMATION_FUNCTIONS_H_
#define _NUMERICAL_APPROXIMATION_FUNCTIONS_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
    class ApproximationFunctions
    {
    public:

        ApproximationFunctions();
        virtual ~ApproximationFunctions();

        virtual void setNumFunctions(UInt numFunctions);
        virtual UInt getNumFunctions() const;

        virtual void setFunction(UInt i, const Function *function);
        virtual const Function *getFunction(UInt i) const;

        virtual Real function(UInt i, Real x) const;

    protected:

        const Function **functions;
        UInt numFunctions;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_APPROXIMATION_FUNCTIONS_H_
