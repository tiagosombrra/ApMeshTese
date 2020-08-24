#ifndef _NUMERICAL_EIGENSYSTEM_SOLVER_H_
#define _NUMERICAL_EIGENSYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class EigensystemSolver : public Performer::Builder
    {
    public:

        static const UInt MAX_STEPS;
        static const Real TOLERANCE;

    public:

        EigensystemSolver();
        virtual ~EigensystemSolver();

        virtual void setMatrix(IMatrix *matrix);
        virtual IMatrix *getMatrix() const;

    protected:

        IMatrix *matrix;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_EIGENSYSTEM_SOLVER_H_
