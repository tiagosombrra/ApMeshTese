#ifndef _NUMERICAL_ODE_SYSTEM_H_
#define _NUMERICAL_ODE_SYSTEM_H_

#include "Data/Definitions.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/MultiVariableFunction.h"

namespace Numerical
{
    //template <UInt n>
    //class ODESystem : public MultiVariableFunction<n>
    class ODESystem
    {
    public:

        virtual UInt numFunctions() = 0;

        //virtual Real min() = 0;
        //virtual Real max() = 0;

        virtual Real initial(UInt i) = 0;
        virtual Real f(UInt i, Real t, Real *x) = 0;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ODE_SYSTEM_H_
