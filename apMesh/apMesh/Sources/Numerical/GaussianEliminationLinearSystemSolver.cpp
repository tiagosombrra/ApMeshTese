#include "Numerical/GaussianEliminationLinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/PermutationMatrix.h"

using namespace Data;
using namespace Numerical;

/*IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants,
    bool copy, enum Pivoting pivoting, Real tolerance, bool &ok)
{
    ok = true;

	if ((matrix->numRows() != constants->numRows()) || (!matrix->isSquare()))
    {
        ok = false;

        return NULL;
    }

	IMatrix *m = NULL;
	IColumnMatrix *c = NULL;

	if (copy)
    {
        m = matrix->clone();
        c = static_cast<IColumnMatrix *>(constants->clone());
    }
    else
    {
        m = const_cast<IMatrix *>(matrix);
        c = const_cast<IColumnMatrix *>(constants);
    }

    Numerical::PermutationMatrix *pm = (pivoting == TOTAL) ? new Numerical::PermutationMatrix(matrix->numRows()) : NULL;

	for (UInt step = 0; step < m->numRows() - 1; step++)
    {
        UInt row = step, col = step;

        if (pivoting != NONE)
        {
            for (UInt i = step + 1; i < m->numRows(); i++)
            {
                if (std::fabs(m->getElement(i, col)) > std::fabs(m->getElement(row, col)))
                {
                    row = i;
                }
            }
        }

        if (pivoting == TOTAL)
        {
            for (UInt i = step; i < m->numRows(); i++)
            {
                for (UInt j = step + 1; j < m->numCols(); j++)
                {
                    if (std::fabs(m->getElement(i, j)) > std::fabs(m->getElement(row, col)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != step)
        {
            for (UInt j = step; j < m->numCols(); j++)
            {
                Real v = m->getElement(row, j);

                m->setElement(row, j, m->getElement(step, j));

                m->setElement(step, j, v);
            }

            Real v = c->getElement(row);

            c->setElement(row, c->getElement(step));

            c->setElement(step, v);
        }

        if (col != step)
        {
            pm->swapRows(step, col);

            for (UInt i = 0; i < m->numRows(); i++)
            {
                Real v = m->getElement(i, col);

                m->setElement(i, col, m->getElement(i, step));

                m->setElement(i, step, v);
            }
        }

        //std::cout << m->text() << std::endl;
        //std::cout << c->text() << std::endl;

        Real pivot = m->getElement(step, step);

        if (std::fabs(pivot) < tolerance)
        {
            ok = false;

            if (copy)
            {
                delete m;
                delete c;
            }

            if (pm)
            {
                delete pm;
            }

            return NULL;
        }

        for (UInt i = step + 1; i < m->numRows(); i++)
        {
            Real multiplyer = m->getElement(i, step)/pivot;

            m->setElement(i, step, 0.0);

            for (UInt j = step + 1; j < m->numCols(); j++)
            {
                Real val = m->getElement(i, j);

                val -= multiplyer*m->getElement(step, j);

                m->setElement(i, j, val);
            }

            Real val = c->getElement(i);

            val -= multiplyer*c->getElement(step);

            c->setElement(i, val);
        }
    }

    //std::cout << m->text() << std::endl;
    //std::cout << c->text() << std::endl;

	IColumnMatrix *result = this->executeBackwardSubstitution(m, c);

	if (pivoting == TOTAL)
    {
        pm->premultiply(result);

        delete pm;
    }

    if (copy)
    {
        delete m;
        delete c;
    }

    return result;
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, enum Pivoting pivoting, bool &ok)
{
    return this->execute(matrix, constants, copy, pivoting, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, bool &ok)
{
    return this->execute(matrix, constants, true, pivoting, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting)
{
    bool ok = true;

    return this->execute(matrix, constants, pivoting, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants,
    bool copy, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, copy, Numerical::GaussianEliminationLinearSystemSolver::NONE, tolerance, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok)
{
    return this->execute(matrix, constants, copy, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->execute(matrix, constants, true, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->execute(matrix, constants, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, copy, Numerical::GaussianEliminationLinearSystemSolver::PARTIAL, tolerance, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok)
{
    return this->executePartial(matrix, constants, copy, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->executePartial(matrix, constants, true, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->executePartial(matrix, constants, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, copy, Numerical::GaussianEliminationLinearSystemSolver::TOTAL, tolerance, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok)
{
    return this->executeTotal(matrix, constants, copy, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->executeTotal(matrix, constants, true, ok);
}

IColumnMatrix *Numerical::GaussianEliminationLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->executeTotal(matrix, constants, ok);
}*/

Numerical::GaussianEliminationLinearSystemSolver::GaussianEliminationLinearSystemSolver() :
    Numerical::LinearSystemSolver::LinearSystemSolver()
{
    this->pivoting = this->NONE;

    this->copy = true;
    this->skip = false;
}

Numerical::GaussianEliminationLinearSystemSolver::~GaussianEliminationLinearSystemSolver()
{

}

void Numerical::GaussianEliminationLinearSystemSolver::setPivoting(enum Pivoting pivoting)
{
    this->pivoting = pivoting;
}

enum Numerical::GaussianEliminationLinearSystemSolver::Pivoting Numerical::GaussianEliminationLinearSystemSolver::getPivoting() const
{
    return this->pivoting;
}

void Numerical::GaussianEliminationLinearSystemSolver::setCopy(bool copy)
{
    this->copy = copy;
}

bool Numerical::GaussianEliminationLinearSystemSolver::isCopy() const
{
    return this->copy;
}

void Numerical::GaussianEliminationLinearSystemSolver::setSkip(bool skip)
{
    this->skip = skip;
}

bool Numerical::GaussianEliminationLinearSystemSolver::isSkip() const
{
    return this->skip;
}

void Numerical::GaussianEliminationLinearSystemSolver::execute()
{
    this->ok = true;

    this->unknowns = NULL;

    if ((!this->matrix) || (!this->constants))
    {
        this->ok = false;

        return;
    }

	if ((this->matrix->numRows() != this->constants->numRows()) || (!this->matrix->isSquare()))
    {
        ok = false;

        return;
    }

	IMatrix *m = NULL;
	IColumnMatrix *c = NULL;

	if (this->copy)
    {
        m = this->matrix->clone();
        c = static_cast<IColumnMatrix *>(this->constants->clone());
    }
    else
    {
        m = const_cast<IMatrix *>(this->matrix);
        c = const_cast<IColumnMatrix *>(this->constants);
    }

    Numerical::PermutationMatrix *pm = (this->pivoting == TOTAL) ? new Numerical::PermutationMatrix(this->matrix->numRows()) : NULL;

	for (UInt step = 0; step < m->numRows() - 1; step++)
    {
        UInt row = step, col = step;

        if (this->pivoting != NONE)
        {
            for (UInt i = step + 1; i < m->numRows(); i++)
            {
                if (std::fabs(m->getElement(i, col)) > std::fabs(m->getElement(row, col)))
                {
                    row = i;
                }
            }
        }

        if (this->pivoting == TOTAL)
        {
            for (UInt i = step; i < m->numRows(); i++)
            {
                for (UInt j = step + 1; j < m->numCols(); j++)
                {
                    if (std::fabs(m->getElement(i, j)) > std::fabs(m->getElement(row, col)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != step)
        {
            for (UInt j = step; j < m->numCols(); j++)
            {
                Real v = m->getElement(row, j);

                m->setElement(row, j, m->getElement(step, j));

                m->setElement(step, j, v);
            }

            Real v = c->getElement(row);

            c->setElement(row, c->getElement(step));

            c->setElement(step, v);
        }

        if (col != step)
        {
            pm->swapRows(step, col);

            for (UInt i = 0; i < m->numRows(); i++)
            {
                Real v = m->getElement(i, col);

                m->setElement(i, col, m->getElement(i, step));

                m->setElement(i, step, v);
            }
        }

        //std::cout << m->text() << std::endl;
        //std::cout << c->text() << std::endl;

        Real pivot = m->getElement(step, step);

        if (std::fabs(pivot) <= this->tolerance)
        {
            if (this->skip)
            {
                m->setElement(step, step, 0.0);

                continue;
            }

            this->ok = false;

            if (this->copy)
            {
                delete m;
                delete c;
            }

            if (pm)
            {
                delete pm;
            }

            return;
        }

        for (UInt i = step + 1; i < m->numRows(); i++)
        {
            Real multiplyer = m->getElement(i, step)/pivot;

            m->setElement(i, step, 0.0);

            if (std::fabs(multiplyer) <= this->tolerance)
            {
                continue;
            }

            for (UInt j = step + 1; j < m->numCols(); j++)
            {
                Real val = m->getElement(i, j);

                val -= multiplyer*m->getElement(step, j);

                m->setElement(i, j, val);
            }

            Real val = c->getElement(i);

            val -= multiplyer*c->getElement(step);

            c->setElement(i, val);
        }
    }

    //std::cout << m->text() << std::endl;
    //std::cout << c->text() << std::endl;

    if (std::fabs(m->getElement(m->numRows() - 1, m->numCols() - 1)) <= this->tolerance)
    {
        if (this->skip)
        {
            m->setElement(m->numRows() - 1, m->numCols() - 1, 0.0);
        }
        else
        {
            this->ok = false;

            if (this->copy)
            {
                delete m;
                delete c;
            }

            if (pm)
            {
                delete pm;
            }

            return;
        }
    }

	IColumnMatrix *result = this->backwardSubstitution(m, c, this->skip);

	if (this->pivoting == TOTAL)
    {
        pm->premultiply(result);

        delete pm;
    }

    if (this->copy)
    {
        delete m;
        delete c;
    }

    this->unknowns = result;
}

#endif //#if USE_NUMERICAL
