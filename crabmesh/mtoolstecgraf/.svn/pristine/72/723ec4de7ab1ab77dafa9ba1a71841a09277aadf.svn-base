#include "Numerical/LeastSquaresApproximator.h"

#if USE_NUMERICAL

#include "Numerical/OrderedPoints.h"
#include "Numerical/ApproximationFunctions.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/GaussianEliminationLinearSystemSolver.h"

using namespace Data;
using namespace Numerical;

Numerical::LeastSquaresApproximator::LeastSquaresApproximator() :
    Numerical::Approximator::Approximator()
{
    this->points = NULL;
    this->functions = NULL;

    this->solver = NULL;
    this->createdSolver = false;

    this->x = 0.0;
    this->fx = 0.0;
}

Numerical::LeastSquaresApproximator::~LeastSquaresApproximator()
{
    if (this->createdSolver)
    {
        delete this->solver;
    }
}

void Numerical::LeastSquaresApproximator::setPoints(const OrderedPoints *points)
{
    this->points = points;
}

const OrderedPoints *Numerical::LeastSquaresApproximator::getPoints() const
{
    return this->points;
}

void Numerical::LeastSquaresApproximator::setFunctions(const ApproximationFunctions *functions)
{
    this->functions = functions;
}

const ApproximationFunctions *Numerical::LeastSquaresApproximator::getFunctions() const
{
    return this->functions;
}

void Numerical::LeastSquaresApproximator::setSolver(LinearSystemSolver *solver)
{
    this->solver = solver;
}

LinearSystemSolver *Numerical::LeastSquaresApproximator::getSolver() const
{
    return this->solver;
}

void Numerical::LeastSquaresApproximator::setX(Real x)
{
    this->x = x;
}

Real Numerical::LeastSquaresApproximator::getX() const
{
    return this->x;
}

Real Numerical::LeastSquaresApproximator::getFx() const
{
    return this->fx;
}

/*void Numerical::LeastSquaresApproximator::fill(const OrderedPoints *points, const ApproximationFunctions *functions, IMatrix *matrix, IColumnMatrix *constants) const
{
    UInt nfs = functions->getNumFunctions();
    UInt nps = points->getNumPoints();

    for (UInt i = 0; i < nfs; i++)
    {
        for (UInt j = i; j < nfs; j++)
        {
            Real sum = 0.0;

            for (UInt k = 0; k < nps; k++)
            {
                sum += functions->function(i, points->x(k))*functions->function(j, points->x(k));
            }

            matrix->setElement(i, j, sum);

            if (i != j)
            {
                matrix->setElement(j, i, sum);
            }
        }

        Real sum = 0.0;

        for (UInt k = 0; k < nps; k++)
        {
            sum += functions->function(i, points->x(k))*points->y(k);
        }

        constants->setElement(i, sum);
    }
}

IColumnMatrix *Numerical::LeastSquaresApproximator::findCoefficients(const IMatrix *matrix, const IColumnMatrix *constants, LinearSystemSolver *solver) const
{
    //std::cout << "matrix:\n" << matrix->text() << "constants:\n" << constants->text();

    //return solver->execute(matrix, constants);

    solver->setMatrix(matrix);
    solver->setConstants(constants);

    solver->execute();

    return solver->getUnknowns();
}

IColumnMatrix *Numerical::LeastSquaresApproximator::findCoefficients(const OrderedPoints *points, const ApproximationFunctions *functions, LinearSystemSolver *solver) const
{
    DenseMatrix coefficientes(functions->getNumFunctions(), functions->getNumFunctions());
    ColumnMatrix constants(functions->getNumFunctions());

    this->fill(points, functions, &coefficientes, &constants);

    return this->findCoefficients(&coefficientes, &constants, solver);
}

Real Numerical::LeastSquaresApproximator::execute(const ApproximationFunctions *functions, const IColumnMatrix *coefficients, Real x) const
{
    Real sum = 0.0;

    for (UInt i = 0; i < functions->getNumFunctions(); i++)
    {
        sum += coefficients->getElement(i)*functions->function(i, x);
    }

    return sum;
}

Real Numerical::LeastSquaresApproximator::execute(const OrderedPoints *points, const ApproximationFunctions *functions, LinearSystemSolver *solver, Real x) const
{
    IColumnMatrix *coefficients = this->findCoefficients(points, functions, solver);

    //std::cout << "coefficients:\n" << coefficients->text();

    Real result = this->execute(functions, coefficients, x);

    delete coefficients;

    return result;
}*/

void Numerical::LeastSquaresApproximator::execute()
{
    IColumnMatrix *coefficients = this->findCoefficients();

    //std::cout << "coefficients:\n" << coefficients->text();

    this->fx = this->calculate(coefficients);

    delete coefficients;
}

void Numerical::LeastSquaresApproximator::fill(IMatrix *matrix, IColumnMatrix *constants) const
{
    UInt nfs = this->functions->getNumFunctions();
    UInt nps = this->points->getNumPoints();

    for (UInt i = 0; i < nfs; i++)
    {
        for (UInt j = i; j < nfs; j++)
        {
            Real sum = 0.0;

            for (UInt k = 0; k < nps; k++)
            {
                sum += this->functions->function(i, this->points->x(k))*this->functions->function(j, this->points->x(k));
            }

            matrix->setElement(i, j, sum);

            if (i != j)
            {
                matrix->setElement(j, i, sum);
            }
        }

        Real sum = 0.0;

        for (UInt k = 0; k < nps; k++)
        {
            sum += this->functions->function(i, this->points->x(k))*this->points->y(k);
        }

        constants->setElement(i, sum);
    }
}

IColumnMatrix *Numerical::LeastSquaresApproximator::findCoefficients(const IMatrix *matrix, const IColumnMatrix *constants) const
{
    if (!this->solver)
    {
        this->solver = new GaussianEliminationLinearSystemSolver();

        this->createdSolver = true;
    }

    this->solver->setMatrix(matrix);
    this->solver->setConstants(constants);

    this->solver->execute();

    return this->solver->getUnknowns();
}

IColumnMatrix *Numerical::LeastSquaresApproximator::findCoefficients() const
{
    DenseMatrix coefficientes(this->functions->getNumFunctions(), this->functions->getNumFunctions());
    ColumnMatrix constants(this->functions->getNumFunctions());

    this->fill(&coefficientes, &constants);

    return this->findCoefficients(&coefficientes, &constants);
}

Real Numerical::LeastSquaresApproximator::calculate(const IColumnMatrix *coefficients) const
{
    Real sum = 0.0;

    for (UInt i = 0; i < this->functions->getNumFunctions(); i++)
    {
        sum += coefficients->getElement(i)*this->functions->function(i, this->x);
    }

    return sum;
}

#endif //#if USE_NUMERICAL
