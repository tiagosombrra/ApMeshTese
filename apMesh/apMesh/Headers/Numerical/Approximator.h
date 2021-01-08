#ifndef _NUMERICAL_APPROXIMATOR_H_
#define _NUMERICAL_APPROXIMATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class Approximator : Performer::Builder
    {
    public:

        Approximator();
        virtual ~Approximator();

    protected:

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_APPROXIMATOR_H_
