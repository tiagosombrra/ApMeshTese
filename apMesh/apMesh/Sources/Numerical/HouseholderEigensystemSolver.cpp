#include "Numerical/HouseholderEigensystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"
#include "Numerical/RowMatrix.h"
#include "Numerical/NewtonRaphsonEquationRootFinder.h"
#include "Numerical/FalsePositionEquationRootFinder.h"
#include "Numerical/BisectionEquationRootFinder.h"
#include "Numerical/HouseholderMatrixTridiagonalizer.h"

using namespace Data;
using namespace Numerical;

Numerical::HouseholderEigensystemSolver::HouseholderEigensystemSolver() :
    Numerical::EigensystemSolver::EigensystemSolver()
{
    this->eigenvalues = NULL;

    this->copy = false;
}

Numerical::HouseholderEigensystemSolver::~HouseholderEigensystemSolver()
{

}

void Numerical::HouseholderEigensystemSolver::setCopy(bool copy)
{
    this->copy = copy;
}

bool Numerical::HouseholderEigensystemSolver::isCopy() const
{
    return this->copy;
}

RowMatrix *Numerical::HouseholderEigensystemSolver::getEigenvalues() const
{
    return this->eigenvalues;
}

void Numerical::HouseholderEigensystemSolver::execute()
{
    if ((!this->matrix) || (this->matrix->numRows() == 0))
    {
        return;
    }

    HouseholderMatrixTridiagonalizer tridiagonalizer;

    tridiagonalizer.setMatrix(this->matrix);
    tridiagonalizer.setCopy(this->copy);

    tridiagonalizer.execute();

    IMatrix *tridiagonalized = tridiagonalizer.getTridiagonalized();

    class TridiagPolynomial : public Numerical::Function
    {
    public:

        IMatrix *matrix;
        UInt n;

        using Numerical::Function::f;
        Real f(Real x)
        {
            Real values[3];

            values[0] = 1.0;

            if (this->n == 0)
            {
                return values[0];
            }

            values[1] = this->matrix->getElement(0, 0) - x;

            if (this->n == 1)
            {
                return values[1];
            }

            for (UInt i = 2; i <= this->n; i++)
            {
                Real a = this->matrix->getElement(i - 1, i - 1);
                Real b = this->matrix->getElement(i - 1, i - 2);

                values[i%3] = (a - x)*values[(i - 1)%3] - b*b*values[(i - 2)%3];
            }

            return values[this->n%3];
        };
    };

    class TridiagPolynomialDeriv : public Numerical::Function
    {
    public:

        IMatrix *matrix;
        UInt n;

        using Numerical::Function::f;
        Real f(Real x)
        {
            Real values[3];
            Real derivs[3];

            values[0] = 1.0;
            derivs[0] = 0.0;

            if (this->n == 0)
            {
                return derivs[0];
            }

            values[1] = this->matrix->getElement(0, 0) - x;
            derivs[1] = -1.0;

            if (this->n == 1)
            {
                return derivs[1];
            }

            for (UInt i = 2; i <= this->n; i++)
            {
                Real a = this->matrix->getElement(i - 1, i - 1);
                Real b = this->matrix->getElement(i - 1, i - 2);

                values[i%3] = (a - x)*values[(i - 1)%3] - b*b*values[(i - 2)%3];
                derivs[i%3] = -values[(i - 1)%3] + (a - x)*derivs[(i - 1)%3] - b*b*derivs[(i - 2)%3];
            }

            return derivs[this->n%3];
        };
    };

    TridiagPolynomial pol;
    TridiagPolynomialDeriv deriv;

    pol.matrix = deriv.matrix = tridiagonalized;

    NewtonRaphsonEquationRootFinder nr;
    FalsePositionEquationRootFinder fp;
    BisectionEquationRootFinder bis;

    nr.setFunction(&pol);
    nr.setDerivative(&deriv);

    fp.setFunction(&pol);
    bis.setFunction(&pol);

    this->eigenvalues = new RowMatrix(tridiagonalized->numRows());

    //this->eigenvalues->setElement(0, tridiagonalized->getElement(0, 0));

    for (UInt i = 0; i < tridiagonalized->numRows(); i++)
    {
        pol.n = deriv.n = i + 1;

        Real prev = this->eigenvalues->getElement(0);

        for (UInt j = 0; j <= i; j++)
        {
            Real root = 0.0;

            if ((j == 0) || (j == i))
            {
                Real initial = 0.0;

                if (i > 0)
                {
                    if (std::fabs(pol.f(prev)) <= nr.getTolerance())
                    {
                        initial = prev;
                    }
                    else
                    {
                        if (j == 0)
                        {
                            initial = prev - 1.0;
                        }
                        else if (j == i)
                        {
                            initial = prev + 1.0;
                        }
                    }
                }

                nr.setInitial(initial);

                nr.execute();

                root = nr.getRoot();
            }
            else
            {
                fp.setMin(prev);
                fp.setMax(this->eigenvalues->getElement(j));

                fp.execute();

                if (fp.isOk())
                {
                    root = fp.getRoot();
                }
                else
                {
                    bis.setMin(fp.getMin());
                    bis.setMax(fp.getMax());

                    bis.execute();

                    root = bis.getRoot();
                }

                prev = this->eigenvalues->getElement(j);
            }

            this->eigenvalues->setElement(j, root);
        }
    }

    for (UInt i = 0; i < this->eigenvalues->numCols() - 1; i++)
    {
        Real next = this->eigenvalues->getElement(this->eigenvalues->numCols() - i - 1);
        Real prev = this->eigenvalues->getElement(this->eigenvalues->numCols() - i - 2);

        if (prev > next)
        {
            this->eigenvalues->setElement(this->eigenvalues->numCols() - i - 2, next);
        }
    }

    if (tridiagonalized != this->matrix)
    {
        delete tridiagonalized;
    }
}

#endif //#if USE_NUMERICAL
