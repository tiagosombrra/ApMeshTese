#include "Numerical/CholeskyFatorationLinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/SymmetricMatrix.h"
#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

/*ISymmetricMatrix *Numerical::CholeskyFatorationLinearSystemSolver::decompose(const IMatrix *posDefSymmetric, Real tolerance, bool &ok)
{
    ok = true;

    if (!posDefSymmetric->isSquare())
    {
        ok = false;

        return NULL;
    }

	SymmetricMatrix *g = new SymmetricMatrix(posDefSymmetric->numRows());

    for (UInt i = 0; i < posDefSymmetric->numRows(); i++)
    {
        for (UInt j = i; j < posDefSymmetric->numCols(); j++)
        {
            g->setElement(i, j, posDefSymmetric->getElement(i, j));
        }
    }

	for (UInt step = 0; step < posDefSymmetric->numRows(); step++)
    {
        //std::cout << g->text() << std::endl;

        Real pivot = g->getElement(step, step);

        Real sum = 0.0;

        for (UInt j = 0; j < step; j++)
        {
            Real val = g->getElement(step, j);

            sum += val*val;
        }

        Real r = pivot - sum;

        if (r < 0.0)
        {
            ok = false;

            delete g;

            return NULL;
        }

        if (std::fabs(r) <= tolerance)
        {
            ok = false;

            delete g;

            return NULL;
        }

        r = std::sqrt(r);

        g->setElement(step, step, r);

        for (UInt i = step + 1; i < g->numRows(); i++)
        {
            sum = 0.0;

            for (UInt j = 0; j < step; j++)
            {
                sum += g->getElement(i, j)*g->getElement(step, j);
            }

            Real val = (g->getElement(i, step) - sum)/r;

            g->setElement(i, step, val);
        }
    }

    //std::cout << g->text() << std::endl;

    return g;
}

ISymmetricMatrix *Numerical::CholeskyFatorationLinearSystemSolver::decompose(const IMatrix *posDefSymmetric, bool &ok)
{
    return this->decompose(posDefSymmetric, LinearSystemSolver::TOLERANCE, ok);
}

ISymmetricMatrix *Numerical::CholeskyFatorationLinearSystemSolver::decompose(const IMatrix *posDefSymmetric)
{
    bool ok;

    return this->decompose(posDefSymmetric, ok);
}

IColumnMatrix *Numerical::CholeskyFatorationLinearSystemSolver::execute(const ISymmetricMatrix *g, const IColumnMatrix *constants, bool &ok)
{
	if (g->numRows() != constants->numRows())
    {
        ok = false;

        return NULL;
    }

	IColumnMatrix *intermediate = this->executeForwardSubstitution(g, constants);

    //std::cout << intermediate->text() << std::endl;

	IColumnMatrix *result = this->executeBackwardSubstitution(g, intermediate);

	delete intermediate;

    return result;
}

IColumnMatrix *Numerical::CholeskyFatorationLinearSystemSolver::execute(const ISymmetricMatrix *g, const IColumnMatrix *constants)
{
    bool ok;

    return this->execute(g, constants, ok);
}

IColumnMatrix *Numerical::CholeskyFatorationLinearSystemSolver::execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants, Real tolerance, bool &ok)
{
    ISymmetricMatrix *g = this->decompose(posDefSymmetric, tolerance, ok);

    if (!ok)
    {
        if (g)
        {
            delete g;
        }

        return NULL;
    }

    IColumnMatrix *r = this->execute(g, constants);

    delete g;

    return r;
}

IColumnMatrix *Numerical::CholeskyFatorationLinearSystemSolver::execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants, bool &ok)
{
    return this->execute(posDefSymmetric, constants, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::CholeskyFatorationLinearSystemSolver::execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->execute(posDefSymmetric, constants, ok);
}*/

Numerical::CholeskyFatorationLinearSystemSolver::CholeskyFatorationLinearSystemSolver() :
    Numerical::LinearSystemSolver::LinearSystemSolver()
{
    this->fatoration = NULL;
    this->fatorationGot = false;
}

Numerical::CholeskyFatorationLinearSystemSolver::~CholeskyFatorationLinearSystemSolver()
{
    if (!this->fatorationGot)
    {
        delete this->fatoration;
    }
}

ISymmetricMatrix *Numerical::CholeskyFatorationLinearSystemSolver::getFatoration() const
{
    this->fatorationGot = true;

    return this->fatoration;
}

void Numerical::CholeskyFatorationLinearSystemSolver::execute()
{
    if ((!this->matrix) && (!this->fatoration))
    {
        this->ok = false;

        return;
    }

    //if ((this->matrix) && (!this->fatoration))
    if (this->matrix)
    {
        if (this->fatoration)
        {
            delete this->fatoration;

            this->fatoration = NULL;
        }

        this->decompose();

        if (!this->ok)
        {
            if (this->fatoration)
            {
                delete this->fatoration;

                this->fatoration = NULL;
            }

            return;
        }
    }

    if ((this->fatoration) && (this->constants))
    {
        this->solve();
    }
}

void Numerical::CholeskyFatorationLinearSystemSolver::decompose()
{
    this->ok = true;

    if (!this->matrix)
    {
        this->ok = false;

        return;
    }

    if (!this->matrix->isSquare())
    {
        this->ok = false;

        return;
    }

	this->fatoration = new SymmetricMatrix(this->matrix->numRows());

    for (UInt i = 0; i < this->matrix->numRows(); i++)
    {
        for (UInt j = i; j < this->matrix->numCols(); j++)
        {
            this->fatoration->setElement(i, j, this->matrix->getElement(i, j));
        }
    }

	for (UInt step = 0; step < this->matrix->numRows(); step++)
    {
        //std::cout << this->fatoration->text() << std::endl;

        Real pivot = this->fatoration->getElement(step, step);

        Real sum = 0.0;

        for (UInt j = 0; j < step; j++)
        {
            Real val = this->fatoration->getElement(step, j);

            sum += val*val;
        }

        Real r = pivot - sum;

        if (r < 0.0)
        {
            ok = false;

            delete this->fatoration;

            this->fatoration = NULL;

            return;
        }

        if (std::fabs(r) <= tolerance)
        {
            ok = false;

            delete this->fatoration;

            this->fatoration = NULL;

            return;
        }

        r = std::sqrt(r);

        this->fatoration->setElement(step, step, r);

        for (UInt i = step + 1; i < this->fatoration->numRows(); i++)
        {
            sum = 0.0;

            for (UInt j = 0; j < step; j++)
            {
                sum += this->fatoration->getElement(i, j)*this->fatoration->getElement(step, j);
            }

            Real val = (this->fatoration->getElement(i, step) - sum)/r;

            this->fatoration->setElement(i, step, val);
        }
    }

    //std::cout << this->fatoration->text() << std::endl;
}

void Numerical::CholeskyFatorationLinearSystemSolver::solve()
{
    this->ok = true;

    this->unknowns = NULL;

    if (this->fatoration->numRows() != this->constants->numRows())
    {
        this->ok = false;

        return;
    }

	IColumnMatrix *intermediate = this->forwardSubstitution(this->fatoration, this->constants);

    //std::cout << intermediate->text() << std::endl;

	IColumnMatrix *result = this->backwardSubstitution(this->fatoration, intermediate);

	delete intermediate;

    this->unknowns = result;
}

#endif //#if USE_NUMERICAL
