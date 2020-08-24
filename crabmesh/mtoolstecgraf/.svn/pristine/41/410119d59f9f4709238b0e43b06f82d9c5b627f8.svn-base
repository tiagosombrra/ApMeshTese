#include "Numerical/InterpolationFunction.h"

#if USE_NUMERICAL

#include "Numerical/NewtonInterpolator.h"
#include "Numerical/OrderedPoints.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/GaussianEliminationLinearSystemSolver.h"

using namespace Data;
using namespace Numerical;

Numerical::InterpolationFunction::InterpolationFunction(const OrderedPoints *points, Interpolator *interpolation)
{
    this->points = points;
    this->interpolation = interpolation;
    this->degr = this->points->getNumPoints() - 1;

    this->interp = NULL;
    this->dividedDiferences = NULL;
    this->coefs = NULL;
}

Numerical::InterpolationFunction::InterpolationFunction(const OrderedPoints *points, UInt degree, Interpolator *interpolation)
{
    this->points = points;
    this->interpolation = interpolation;
    this->degr = degree;

    this->interp = NULL;
    this->dividedDiferences = NULL;
    this->coefs = NULL;
}

Numerical::InterpolationFunction::~InterpolationFunction()
{
    if (this->interp)
    {
        delete this->interp;
    }

    if (this->dividedDiferences)
    {
        delete [] this->dividedDiferences;
    }

    if (this->coefs)
    {
        delete this->coefs;
    }
}

const OrderedPoints *Numerical::InterpolationFunction::getPoints()
{
    return this->points;
}

UInt Numerical::InterpolationFunction::degree() const
{
    return this->degr;
}

Real Numerical::InterpolationFunction::getCoefficient(UInt i) const
{
    if (this->degr != this->points->getNumPoints() - 1)
    {
        return 0.0;
    }

    if (i > this->degr)
    {
        return 0.0;
    }

    if (!this->coefs)
    {
        Numerical::DenseMatrix d(this->degr + 1, this->degr + 1);
        Numerical::ColumnMatrix c(this->degr + 1);

        for (UInt j = 0; j <= this->degr; j++)
        {
            for (UInt k = 0; k <= this->degr; k++)
            {
                Real val = (j == 0) ? 1.0 : d.getElement(j, k - 1) * this->points->x(j);

                d.setElement(j, k, val);
            }

            c.setElement(j, this->points->y(j));
        }

        Numerical::GaussianEliminationLinearSystemSolver ge;

        //this->coefs = ge.execute(&d, &c);

        ge.setMatrix(&d);
        ge.setConstants(&c);

        ge.execute();

        this->coefs = ge.getUnknowns();
    }

    return this->coefs->getElement(i);
}

Real Numerical::InterpolationFunction::f(Real x)
{
    if (this->interpolation)
    {
        this->interpolation->setPoints(this->points);
        this->interpolation->setDegree(this->degr);
        this->interpolation->setX(x);

        Numerical::NewtonInterpolator *newton = dynamic_cast<Numerical::NewtonInterpolator *>(this->interpolation);

        if (newton)
        {
            if ((!this->dividedDiferences) && (this->degr == this->points->getNumPoints() - 1))
            {
                this->dividedDiferences = new Real[this->degr - 1];

                //newton->buildDividedDifferencesVector(this->points, this->degr, 0, this->dividedDiferences);
                newton->buildDividedDifferencesVector(this->dividedDiferences);
            }

            if (this->dividedDiferences)
            {
                //return newton->executeDerivative(this->points, this->degr, 0, 0, this->dividedDiferences, x);
                return newton->dExecute(0, this->dividedDiferences);
            }
        }

        //return this->interpolation->execute(this->points, this->degr, x);
        this->interpolation->execute();

        return this->interpolation->getFx();
    }

    if (!this->interp)
    {
        this->interp = new Numerical::NewtonInterpolator();
    }

    this->interp->setPoints(this->points);
    this->interp->setDegree(this->degr);
    this->interp->setX(x);

    if ((!this->dividedDiferences) && (this->degr == this->points->getNumPoints() - 1))
    {
        this->dividedDiferences = new Real[this->degr - 1];

        //this->interp->buildDividedDifferencesVector(this->points, this->degr, 0, this->dividedDiferences);
        this->interp->buildDividedDifferencesVector(this->dividedDiferences);
    }

    if (this->dividedDiferences)
    {
        //return this->interp->executeDerivative(this->points, this->degr, 0, 0, this->dividedDiferences, x);
        return this->interp->dExecute(0, this->dividedDiferences);
    }

    return this->interp->dExecute(0);
}

#endif //#if USE_NUMERICAL
