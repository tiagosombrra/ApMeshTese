#include "Numerical/LUDecompositionLinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/IMatrix.h"
#include "Numerical/LUMatrix.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/PermutationMatrix.h"

using namespace Data;
using namespace Numerical;

/*ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix, enum Pivoting pivoting,
    IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance, bool &ok)
{
    ok = true;

    if (!matrix->isSquare())
    {
        ok = false;

        return NULL;
    }

    if (((pivoting != NONE) && ((!pre) || (pre->numRows() != matrix->numRows()))) ||
        ((pivoting == TOTAL) && ((!post) || (post->numRows() != matrix->numRows()))))
    {
        ok = false;

        return NULL;
    }

	LUMatrix *lu = new LUMatrix(matrix);

    Numerical::IMatrix::copy(matrix, lu);

	for (UInt step = 0; step < matrix->numRows() - 1; step++)
    {
        UInt row = step, col = step;

        if (pivoting != NONE)
        {
            for (UInt i = step + 1; i < lu->numRows(); i++)
            {
                if (std::fabs(lu->getElement(i, step)) > std::fabs(lu->getElement(row, col)))
                {
                    row = i;
                }
            }
        }

        if (pivoting == TOTAL)
        {
            for (UInt i = step; i < lu->numRows(); i++)
            {
                for (UInt j = step + 1; j < lu->numCols(); j++)
                {
                    if (std::fabs(lu->getElement(i, j)) > std::fabs(lu->getElement(row, col)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != step)
        {
            pre->swapRows(step, row);

            for (UInt j = 0; j < lu->numCols(); j++)
            {
                Real v = lu->getElement(row, j);

                lu->setElement(row, j, lu->getElement(step, j));

                lu->setElement(step, j, v);
            }
        }

        if (col != step)
        {
            post->swapRows(step, col);

            for (UInt i = 0; i < lu->numRows(); i++)
            {
                Real v = lu->getElement(i, col);

                lu->setElement(i, col, lu->getElement(i, step));

                lu->setElement(i, step, v);
            }
        }

        //std::cout << lu->text() << std::endl;

        Real pivot = lu->getElement(step, step);

        if (std::fabs(pivot) < tolerance)
        {
            ok = false;

            delete lu;

            return NULL;
        }

        for (UInt i = step + 1; i < lu->numRows(); i++)
        {
            Real multiplyer = lu->getElement(i, step)/pivot;

            lu->setElement(i, step, multiplyer);

            for (UInt j = step + 1; j < lu->numCols(); j++)
            {
                Real val = lu->getElement(i, j);

                val -= multiplyer*lu->getElement(step, j);

                lu->setElement(i, j, val);
            }
        }
    }

    //std::cout << lu->text() << std::endl;

    lu->makeLower();
    lu->makeUpper();

    return lu;
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix, enum Pivoting pivoting,
    IPermutationMatrix *pre, IPermutationMatrix *post, bool &ok)
{
    return this->decompose(matrix, pivoting, pre, post, LinearSystemSolver::TOLERANCE, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix, enum Pivoting pivoting,
    IPermutationMatrix *pre, IPermutationMatrix *post)
{
    bool ok;

    return this->decompose(matrix, pivoting, pre, post, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const ILUMatrix *lu, const IColumnMatrix *constants, enum Pivoting pivoting,
    const IPermutationMatrix *pre, const IPermutationMatrix *post, bool &ok)
{
    if (lu->numRows() != constants->numRows())
    {
        ok = false;

        return NULL;
    }

    IColumnMatrix *c = const_cast<IColumnMatrix *>(constants);

    if (pivoting != NONE)
    {
        c = static_cast<IColumnMatrix *>(constants->clone());

        pre->premultiply(c);
    }

	IColumnMatrix *intermediate = this->executeForwardSubstitution(lu->getLower(), c);

    //std::cout << intermediate->text() << std::endl;

	IColumnMatrix *result = this->executeBackwardSubstitution(lu->getUpper(), intermediate);

	if (pivoting == TOTAL)
    {
        post->premultiply(result);
    }

	if (c != constants)
    {
        delete c;
    }

	delete intermediate;

    return result;
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const ILUMatrix *lu, const IColumnMatrix *constants, enum Pivoting pivoting,
    const IPermutationMatrix *pre, const IPermutationMatrix *post)
{
    bool ok;

    return this->execute(lu, constants, pivoting, pre, post, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, Real tolerance, bool &ok)
{
    PermutationMatrix *pre = (pivoting != NONE) ? new PermutationMatrix(matrix->numRows()) : NULL;
    PermutationMatrix *post = (pivoting == TOTAL) ? new PermutationMatrix(matrix->numRows()) : NULL;

    ILUMatrix *lu = this->decompose(matrix, pivoting, pre, post, tolerance, ok);

    if (!ok)
    {
        if (lu)
        {
            delete lu;
        }

        if (pre)
        {
            delete pre;
        }

        if (post)
        {
            delete post;
        }

        return NULL;
    }

    IColumnMatrix *r = this->execute(lu, constants, pivoting, pre, post);

    delete lu;

    if (pre)
    {
        delete pre;
    }

    if (post)
    {
        delete post;
    }

    return r;
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, bool &ok)
{
    return this->execute(matrix, constants, pivoting, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting)
{
    bool ok = true;

    return this->execute(matrix, constants, pivoting, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix, Real tolerance, bool &ok)
{
    return this->decompose(matrix, NONE, NULL, NULL, tolerance, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix, bool &ok)
{
    return this->decompose(matrix, LinearSystemSolver::TOLERANCE, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decompose(const IMatrix *matrix)
{
    bool ok;

    return this->decompose(matrix, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const ILUMatrix *lu, const IColumnMatrix *constants, bool &ok)
{
	return this->execute(lu, constants, NONE, NULL, NULL, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const ILUMatrix *lu, const IColumnMatrix *constants)
{
    bool ok;

    return this->execute(lu, constants, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, NONE, tolerance, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->execute(matrix, constants, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->execute(matrix, constants, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre, Real tolerance, bool &ok)
{
    return this->decompose(matrix, PARTIAL, pre, NULL, tolerance, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre, bool &ok)
{
    return this->decomposePartial(matrix, pre, LinearSystemSolver::TOLERANCE, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre)
{
    bool ok;

    return this->decomposePartial(matrix, pre, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executePartial(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre, bool &ok)
{
    return this->execute(lu, constants, PARTIAL, pre, NULL, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executePartial(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre)
{
    bool ok;

    return this->executePartial(lu, constants, pre, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, PARTIAL, tolerance, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->executePartial(matrix, constants, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->executePartial(matrix, constants, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance, bool &ok)
{
    return this->decompose(matrix, TOTAL, pre, post, tolerance, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, bool &ok)
{
    return this->decomposeTotal(matrix, pre, post, LinearSystemSolver::TOLERANCE, ok);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post)
{
    bool ok;

    return this->decomposeTotal(matrix, pre, post, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executeTotal(const ILUMatrix *lu, const IColumnMatrix *constants,
    const IPermutationMatrix *pre, const IPermutationMatrix *post, bool &ok)
{
    return this->execute(lu, constants, TOTAL, pre, post, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executeTotal(const ILUMatrix *lu, const IColumnMatrix *constants,
    const IPermutationMatrix *pre, const IPermutationMatrix *post)
{
    bool ok;

    return this->executeTotal(lu, constants, pre, post, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok)
{
    return this->execute(matrix, constants, TOTAL, tolerance, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    return this->executeTotal(matrix, constants, LinearSystemSolver::TOLERANCE, ok);
}

IColumnMatrix *Numerical::LUDecompositionLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants)
{
    bool ok = true;

    return this->executeTotal(matrix, constants, ok);
}*/

Numerical::LUDecompositionLinearSystemSolver::LUDecompositionLinearSystemSolver() :
    Numerical::GaussianEliminationLinearSystemSolver::GaussianEliminationLinearSystemSolver()
{
    this->lu = NULL;
    this->luGot = false;

    this->pre = NULL;
    this->post = NULL;
}

Numerical::LUDecompositionLinearSystemSolver::~LUDecompositionLinearSystemSolver()
{
    if (!this->luGot)
    {
        delete this->lu;
    }

    if (this->pre)
    {
        delete this->pre;
    }

    if (this->post)
    {
        delete this->post;
    }
}

void Numerical::LUDecompositionLinearSystemSolver::setLU(ILUMatrix *lu)
{
    this->lu = lu;

    this->luGot = (this->lu != NULL);
}

ILUMatrix *Numerical::LUDecompositionLinearSystemSolver::getLU() const
{
    this->luGot = true;

    return this->lu;
}

void Numerical::LUDecompositionLinearSystemSolver::execute()
{
    if ((!this->matrix) && (!this->lu))
    {
        this->ok = false;

        return;
    }

    if (this->matrix)
    {
        if (this->lu)
        {
            delete this->lu;

            this->lu = NULL;
        }

        this->pre = (this->pivoting != NONE) ? new PermutationMatrix(this->matrix->numRows()) : NULL;
        this->post = (this->pivoting == TOTAL) ? new PermutationMatrix(this->matrix->numRows()) : NULL;

        this->decompose();

        if (!this->ok)
        {
            if (this->lu)
            {
                delete this->lu;

                this->lu = NULL;
            }

            if (this->pre)
            {
                delete this->pre;

                this->pre = NULL;
            }

            if (this->post)
            {
                delete this->post;

                this->post = NULL;
            }

            return;
        }
    }

    if ((this->lu) && (this->constants))
    {
        this->solve();
    }
}

void Numerical::LUDecompositionLinearSystemSolver::decompose()
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

    if ((this->pivoting != NONE) && (!this->pre))
    {
        this->pre =  new PermutationMatrix(this->matrix->numRows());
    }

    if ((this->pivoting == TOTAL) && (!this->post))
    {
        this->post =  new PermutationMatrix(this->matrix->numRows());
    }

    if (((this->pivoting != NONE) && ((!this->pre) || (this->pre->numRows() != this->matrix->numRows()))) ||
        ((this->pivoting == TOTAL) && ((!this->post) || (this->post->numRows() != this->matrix->numRows()))))
    {
        this->ok = false;

        return;
    }

	this->lu = new LUMatrix(this->matrix);

    Numerical::IMatrix::copy(this->matrix, lu);

	for (UInt step = 0; step < this->matrix->numRows() - 1; step++)
    {
        UInt row = step, col = step;

        if (this->pivoting != NONE)
        {
            for (UInt i = step + 1; i < this->lu->numRows(); i++)
            {
                if (std::fabs(this->lu->getElement(i, step)) > std::fabs(this->lu->getElement(row, col)))
                {
                    row = i;
                }
            }
        }

        if (this->pivoting == TOTAL)
        {
            for (UInt i = step; i < this->lu->numRows(); i++)
            {
                for (UInt j = step + 1; j < this->lu->numCols(); j++)
                {
                    if (std::fabs(this->lu->getElement(i, j)) > std::fabs(this->lu->getElement(row, col)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != step)
        {
            this->pre->swapRows(step, row);

            for (UInt j = 0; j < this->lu->numCols(); j++)
            {
                Real v = this->lu->getElement(row, j);

                this->lu->setElement(row, j, this->lu->getElement(step, j));

                this->lu->setElement(step, j, v);
            }
        }

        if (col != step)
        {
            this->post->swapRows(step, col);

            for (UInt i = 0; i < this->lu->numRows(); i++)
            {
                Real v = this->lu->getElement(i, col);

                this->lu->setElement(i, col, lu->getElement(i, step));

                this->lu->setElement(i, step, v);
            }
        }

        //std::cout << lu->text() << std::endl;

        Real pivot = this->lu->getElement(step, step);

        if (std::fabs(pivot) < this->tolerance)
        {
            this->ok = false;

            delete this->lu;

            this->lu = NULL;

            return;
        }

        for (UInt i = step + 1; i < this->lu->numRows(); i++)
        {
            Real multiplyer = this->lu->getElement(i, step)/pivot;

            this->lu->setElement(i, step, multiplyer);

            for (UInt j = step + 1; j < this->lu->numCols(); j++)
            {
                Real val = this->lu->getElement(i, j);

                val -= multiplyer*this->lu->getElement(step, j);

                this->lu->setElement(i, j, val);
            }
        }
    }

    //std::cout << lu->text() << std::endl;

    this->lu->makeLower();
    this->lu->makeUpper();
}

void Numerical::LUDecompositionLinearSystemSolver::solve()
{
    this->ok = true;

    this->unknowns = NULL;

    if ((!this->lu) || (!this->constants))
    {
        this->ok = false;

        return;
    }

    if (this->lu->numRows() != this->constants->numRows())
    {
        this->ok = false;

        return;
    }

    IColumnMatrix *c = const_cast<IColumnMatrix *>(this->constants);

    if (this->pivoting != NONE)
    {
        c = static_cast<IColumnMatrix *>(this->constants->clone());

        this->pre->premultiply(c);
    }

	IColumnMatrix *intermediate = this->forwardSubstitution(lu->getLower(), c);

    //std::cout << intermediate->text() << std::endl;

	IColumnMatrix *result = this->backwardSubstitution(lu->getUpper(), intermediate);

	if (this->pivoting == TOTAL)
    {
        this->post->premultiply(result);
    }

	if (c != this->constants)
    {
        delete c;
    }

	delete intermediate;

    this->unknowns = result;
}

#endif //#if USE_NUMERICAL
