#ifndef _NUMERICAL_LEAST_SQUARES_APPROXIMATOR_H_
#define _NUMERICAL_LEAST_SQUARES_APPROXIMATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/Approximator.h"

namespace Numerical
{
    class LeastSquaresApproximator : public Numerical::Approximator
    {
    public:

        LeastSquaresApproximator();
        virtual ~LeastSquaresApproximator();

        virtual void setPoints(const OrderedPoints *points);
        virtual const OrderedPoints *getPoints() const;

        virtual void setFunctions(const ApproximationFunctions *functions);
        virtual const ApproximationFunctions *getFunctions() const;

        virtual void setSolver(LinearSystemSolver *solver);
        virtual LinearSystemSolver *getSolver() const;

        virtual void setX(Real x);
        virtual Real getX() const;

        virtual Real getFx() const;

        /*virtual void fill(const OrderedPoints *points, const ApproximationFunctions *functions, IMatrix *matrix, IColumnMatrix *constants) const;

        virtual IColumnMatrix *findCoefficients(const IMatrix *matrix, const IColumnMatrix *constants, LinearSystemSolver *solver) const;
        virtual IColumnMatrix *findCoefficients(const OrderedPoints *points, const ApproximationFunctions *functions, LinearSystemSolver *solver) const;

        virtual Real execute(const ApproximationFunctions *functions, const IColumnMatrix *coefficients, Real x) const;

        virtual Real execute(const OrderedPoints *points, const ApproximationFunctions *functions, LinearSystemSolver *solver, Real x) const;*/

        virtual void execute();

        virtual void fill(IMatrix *matrix, IColumnMatrix *constants) const;

        virtual IColumnMatrix *findCoefficients(const IMatrix *matrix, const IColumnMatrix *constants) const;
        virtual IColumnMatrix *findCoefficients() const;

        virtual Real calculate(const IColumnMatrix *coefficients) const;

    protected:

        const OrderedPoints *points;
        const ApproximationFunctions *functions;

        mutable LinearSystemSolver *solver;
        mutable bool createdSolver;

        Real x;
        Real fx;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_LEAST_SQUARES_APPROXIMATOR_H_
