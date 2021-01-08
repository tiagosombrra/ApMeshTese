#include "Numerical/FixedPointLinearSystemSolver.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"
#include "Numerical/PermutationMatrix.h"

using namespace Data;
using namespace Numerical;

/*bool Numerical::FixedPointLinearSystemSolver::testFunction(const IMatrix *matrix,
    const IColumnMatrix *constants, const IColumnMatrix *guess, Real tolerance, IColumnMatrix *fguess) const
{
    Numerical::IMatrix::multiply(matrix, guess, fguess);

    for (UInt i = 0; i < fguess->numRows(); i++)
    {
        Real diff = std::fabs(fguess->getElement(i) - constants->getElement(i));

        if (diff > tolerance)
        {
            return false;
        }
    }

    return true;
}

bool Numerical::FixedPointLinearSystemSolver::testStepSize(const IColumnMatrix *guess, const IColumnMatrix *next, Real tolerance) const
{
    for (UInt i = 0; i < guess->numRows(); i++)
    {
        Real diff = std::fabs(next->getElement(i) - guess->getElement(i));

        if (diff > tolerance)
        {
            return false;
        }
    }

    return true;
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess,
	UInt maxSteps, enum Pivoting pivoting, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
	ok = true;

	if ((!matrix->isSquare()) || (matrix->numRows() != constants->numRows()))
    {
        ok = false;

        return NULL;
    }

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	ColumnMatrix *init = new ColumnMatrix(constants->numRows());

    for (UInt i = 0; i < init->numRows(); i++)
    {
        init->setElement(i, guess ? guess->getElement(i) : 0.0);
    }

    ColumnMatrix *fnext = NULL;

	if (testFunction)
    {
        fnext = new ColumnMatrix(init->numRows());

        if (this->testFunction(matrix, constants, init, tolerance, fnext))
        {
            delete fnext;

            numSteps = 0;

            if (guess)
            {
                delete init;

                return const_cast<Numerical::IColumnMatrix *>(guess);
            }

            return init;
        }
    }

    ColumnMatrix *next = new ColumnMatrix(init->numRows());

    PermutationMatrix *pre = (pivoting != NONE) ? new PermutationMatrix(matrix->numRows()) : NULL;
    PermutationMatrix *post = (pivoting == TOTAL) ? new PermutationMatrix(matrix->numRows()) : NULL;

    IMatrix *m = const_cast<IMatrix *>(matrix);
    IColumnMatrix *c = const_cast<IColumnMatrix *>(constants);

    if (pivoting != NONE)
    {
        m = matrix->clone();
        c = static_cast<IColumnMatrix *>(constants->clone());

        this->pivot(m, pivoting, pre, post, tolerance);
        //this->pivot(m, pivoting, ROWS, pre, post, tolerance);

        pre->premultiply(c);

        delete pre;

        pre = NULL;
    }

    if (pivoting == TOTAL)
    {
        post->postmultiply(init);

        if (testFunction)
        {
            post->postmultiply(fnext);
        }
    }

	for (numSteps = 0; numSteps < maxSteps; numSteps++)
	{
		this->next(m, c, init, next);

		bool error = false;

		for (UInt i = 0; i < next->numRows(); i++)
        {
            if ((std::isinf(next->getElement(i))) || (std::isnan(next->getElement(i))))
            {
                error = true;

                break;
            }
        }

        if (error)
        {
            break;
        }

		//std::cout << numSteps << "\n\n" << init->text() << "\n" <<  next->text() << std::endl;

		if ((testStepSize) && (this->testStepSize(init, next, tolerance)))
        {
            if (pivoting == TOTAL)
            {
                post->premultiply(next);
            }

            delete init;

            if (fnext)
            {
                delete fnext;
            }

            if (m != matrix)
            {
                delete m;
            }

            if (c != constants)
            {
                delete c;
            }

            if (pre)
            {
                delete pre;
            }

            if (post)
            {
                delete post;
            }

            return next;
        }

		if ((testFunction) && (this->testFunction(m, c, next, tolerance, fnext)))
        {
            if (pivoting == TOTAL)
            {
                post->premultiply(next);
            }

            delete init;
            delete fnext;

            if (m != matrix)
            {
                delete m;
            }

            if (c != constants)
            {
                delete c;
            }

            if (pre)
            {
                delete pre;
            }

            if (post)
            {
                delete post;
            }

            return next;
        }

		std::swap(init, next);
	}

	numSteps = maxSteps;

	ok = false;

	delete next;

	if (fnext)
    {
        delete fnext;
    }

    if (m != matrix)
    {
        delete m;
    }

    if (c != constants)
    {
        delete c;
    }

    if (pre)
    {
        delete pre;
    }

    if (post)
    {
        delete post;
    }

    delete init;

	return NULL;
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
    return this->execute(matrix, constants, guess, maxSteps, NONE, tolerance, testFunction, testStepSize, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->execute(matrix, constants, guess, maxSteps, tolerance, true, true, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps)
{
	return this->execute(matrix, constants, guess, FixedPointLinearSystemSolver::MAX_STEPS, FixedPointLinearSystemSolver::TOLERANCE, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps)
{
    bool ok = true;

    return this->execute(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->execute(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess)
{
    UInt numSteps = UINT_MAX;
    bool ok;

    return this->execute(matrix, constants, guess, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::execute(const IMatrix *matrix, const IColumnMatrix *constants)
{
	return this->execute(matrix, constants, NULL);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
	return this->execute(matrix, constants, guess, maxSteps, PARTIAL, tolerance, testFunction, testStepSize, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->executePartial(matrix, constants, guess, maxSteps, tolerance, true, true, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps)
{
	return this->executePartial(matrix, constants, guess, FixedPointLinearSystemSolver::MAX_STEPS, FixedPointLinearSystemSolver::TOLERANCE, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps)
{
    bool ok = true;

    return this->executePartial(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->executePartial(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess)
{
    UInt numSteps = UINT_MAX;
    bool ok;

    return this->executePartial(matrix, constants, guess, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executePartial(const IMatrix *matrix, const IColumnMatrix *constants)
{
	return this->executePartial(matrix, constants, NULL);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
	return this->execute(matrix, constants, guess, maxSteps, TOTAL, tolerance, testFunction, testStepSize, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->executeTotal(matrix, constants, guess, maxSteps, tolerance, true, true, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps)
{
	return this->executeTotal(matrix, constants, guess, FixedPointLinearSystemSolver::MAX_STEPS, FixedPointLinearSystemSolver::TOLERANCE, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps)
{
    bool ok = true;

    return this->executeTotal(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->executeTotal(matrix, constants, NULL, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess)
{
    UInt numSteps = UINT_MAX;
    bool ok;

    return this->executeTotal(matrix, constants, guess, ok, numSteps);
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::executeTotal(const IMatrix *matrix, const IColumnMatrix *constants)
{
	return this->executeTotal(matrix, constants, NULL);
}

Real Numerical::FixedPointLinearSystemSolver::sumRowForSassenfeld(const IMatrix *matrix, UInt row, UInt insteadOfRow, const Real *beta) const
{
    Real sum = 0;

    for (UInt j = 0; j < insteadOfRow; j++)
    {
        sum += std::fabs(matrix->getElement(row, j))*(beta ? beta[j] : 1.0);
    }

    for (UInt j = insteadOfRow + 1; j < matrix->numCols(); j++)
    {
        sum += std::fabs(matrix->getElement(row, j));
    }

    return sum;
}

Real Numerical::FixedPointLinearSystemSolver::sumRowForSassenfeld(const IMatrix *matrix, UInt row, const Real *beta) const
{
    return this->sumRowForSassenfeld(matrix, row, row, beta);
}

bool Numerical::FixedPointLinearSystemSolver::satisfyRowsCriterion(const IMatrix *matrix) const
{
    if (!matrix->isSquare())
    {
        return false;
    }

    for (UInt i = 0; i < matrix->numRows(); i++)
    {
        Real sum = this->sumRowForSassenfeld(matrix, i);

        if (sum >= std::fabs(matrix->getElement(i, i)))
        {
            return false;
        }
    }

    return true;
}

bool Numerical::FixedPointLinearSystemSolver::satisfySassenfeldCriterion(const IMatrix *matrix) const
{
    if (!matrix->isSquare())
    {
        return false;
    }

    Real *beta = new Real[matrix->numRows()];

    for (UInt i = 0; i < matrix->numRows(); i++)
    {
        Real sum = this->sumRowForSassenfeld(matrix, i, beta);

        if (sum >= fabs(matrix->getElement(i, i)))
        {
            delete [] beta;

            return false;
        }

        beta[i] = sum/fabs(matrix->getElement(i, i));
    }

    delete [] beta;

    return true;
}

void Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, enum Pivoting pivoting, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    if (pivoting == NONE)
    {
        return;
    }

    for (UInt k = 0; k < matrix->numRows() - 1; k++)
    {
        UInt row = k, col = k;

        for (UInt i = k + 1; i < matrix->numRows(); i++)
        {
            if ((std::fabs(matrix->getElement(i, k)) > std::fabs(matrix->getElement(row, col))) &&
                (std::fabs(matrix->getElement(k, i)) > tolerance))
            {
                row = i;
            }
        }

        if (pivoting == TOTAL)
        {
            for (UInt i = k; i < matrix->numRows(); i++)
            {
                for (UInt j = k + 1; j < matrix->numCols(); j++)
                {
                    if ((std::fabs(matrix->getElement(i, j)) > std::fabs(matrix->getElement(row, col))) &&
                        (std::fabs(matrix->getElement(j, k)) > tolerance) &&
                        ((i == k) || (std::fabs(matrix->getElement(k, i)) > tolerance)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != k)
        {
            this->swapRows(matrix, pre, row, k);
        }

        if (col != k)
        {
            this->swapCols(matrix, post, col, k);
        }
    }
}

void Numerical::FixedPointLinearSystemSolver::pivotPartial(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const
{
    this->pivot(matrix, PARTIAL, pre, NULL, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivotTotal(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    this->pivot(matrix, TOTAL, pre, post, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivotPartialForRowsCriterion(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const
{
    this->pivot(matrix, PARTIAL, ROWS, pre, NULL, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivotTotalForRowsCriterion(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    this->pivot(matrix, TOTAL, ROWS, pre, post, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivotPartialForSassenfeldCriterion(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const
{
    this->pivot(matrix, PARTIAL, SASSENFELD, pre, NULL, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivotTotalForSassenfeldCriterion(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    this->pivot(matrix, TOTAL, SASSENFELD, pre, post, tolerance);
}

void Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, enum Pivoting pivoting, enum Criterion criterion, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    if (pivoting == NONE)
    {
        return;
    }

    if (criterion == NO_CRITERION)
    {
        this->pivot(matrix, pivoting, pre, post, tolerance);

        return;
    }

    Real *beta = (criterion == ROWS) ? NULL : new Real[matrix->numRows()];

    this->pivot(matrix, pivoting, criterion, 0, beta, pre, post, tolerance);

    if (beta)
    {
        delete beta;
    }
}

bool Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, enum Pivoting pivoting, enum Criterion criterion, UInt step, Real *beta,
    IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const
{
    UInt maxCol = (pivoting == PARTIAL) ? step + 1 : matrix->numCols();

    for (UInt col = step; col < maxCol; col++)
    {
        if ((pivoting == TOTAL) && (step != col))
        {
            this->swapCols(matrix, post, step, col);
        }

        for (UInt row = step; row < matrix->numRows(); row++)
        {
            if (std::fabs(matrix->getElement(row, step)) <= tolerance)
            {
                continue;
            }

            Real sum = this->sumRowForSassenfeld(matrix, row, step, beta);

            if (sum >= std::fabs(matrix->getElement(row, step)))
            {
                continue;
            }

            if (criterion == SASSENFELD)
            {
                beta[step] = sum/fabs(matrix->getElement(row, step));
            }

            if (step != row)
            {
                this->swapRows(matrix, pre, step, row);
            }

            if (this->pivot(matrix, pivoting, criterion, step + 1, beta, pre, post, tolerance))
            {
                return true;
            }

            if (step != row)
            {
                this->swapRows(matrix, pre, step, row);
            }
        }

        if ((pivoting == TOTAL) && (step != col))
        {
            this->swapCols(matrix, post, step, col);
        }
    }

    return false;
}

void Numerical::FixedPointLinearSystemSolver::swapRows(IMatrix *matrix, IPermutationMatrix *pre, UInt i, UInt j) const
{
    pre->swapRows(i, j);

    for (UInt k = 0; k < matrix->numCols(); k++)
    {
        Real v = matrix->getElement(i, k);

        matrix->setElement(i, k, matrix->getElement(j, k));

        matrix->setElement(j, k, v);
    }
}

void Numerical::FixedPointLinearSystemSolver::swapCols(IMatrix *matrix, IPermutationMatrix *post, UInt i, UInt j) const
{
    post->swapRows(i, j);

    for (UInt k = 0; k < matrix->numRows(); k++)
    {
        Real v = matrix->getElement(k, i);

        matrix->setElement(k, i, matrix->getElement(k, j));

        matrix->setElement(k, j, v);
    }
}*/

Numerical::FixedPointLinearSystemSolver::FixedPointLinearSystemSolver() :
    Numerical::LinearSystemSolver::LinearSystemSolver()
{
    this->guess = NULL;

    this->pivoting = this->NONE;
    this->criterion = this->NO_CRITERION;

    this->testingFunction = true;
    this->testingStepSize = true;

    this->maxSteps = this->MAX_STEPS;
    this->numSteps = 0;
}

Numerical::FixedPointLinearSystemSolver::~FixedPointLinearSystemSolver()
{

}

void Numerical::FixedPointLinearSystemSolver::setGuess(IColumnMatrix *guess)
{
    this->guess = guess;
}

IColumnMatrix *Numerical::FixedPointLinearSystemSolver::getGuess() const
{
    return this->guess;
}

void Numerical::FixedPointLinearSystemSolver::setPivoting(enum Pivoting pivoting)
{
    this->pivoting = pivoting;
}

enum Numerical::LinearSystemSolver::Pivoting Numerical::FixedPointLinearSystemSolver::getPivoting() const
{
    return this->pivoting;
}

void Numerical::FixedPointLinearSystemSolver::setCriterion(enum Criterion criterion)
{
    this->criterion = criterion;
}

enum Numerical::FixedPointLinearSystemSolver::Criterion Numerical::FixedPointLinearSystemSolver::getCriterion() const
{
    return this->criterion;
}

void Numerical::FixedPointLinearSystemSolver::setTestingFunction(bool testingFunction)
{
    this->testingFunction = testingFunction;
}

bool Numerical::FixedPointLinearSystemSolver::isTestingFunction() const
{
    return this->testingFunction;
}

void Numerical::FixedPointLinearSystemSolver::setTestingStepSize(bool testingStepSize)
{
    this->testingStepSize = testingStepSize;
}

bool Numerical::FixedPointLinearSystemSolver::getTestingStepSize() const
{
    return this->testingStepSize;
}

void Numerical::FixedPointLinearSystemSolver::setMaxSteps(UInt maxSteps)
{
    this->maxSteps = maxSteps;
}

UInt Numerical::FixedPointLinearSystemSolver::getMaxSteps() const
{
    return this->maxSteps;
}

UInt Numerical::FixedPointLinearSystemSolver::getNumSteps() const
{
    return this->numSteps;
}

void Numerical::FixedPointLinearSystemSolver::execute()
{
    this->ok = true;

    this->unknowns = NULL;

    if ((!this->matrix) || (!this->constants))
    {
        this->ok = false;

        return;
    }

	if ((!this->matrix->isSquare()) || (this->matrix->numRows() != this->constants->numRows()))
    {
        this->ok = false;

        return;
    }

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	ColumnMatrix *init = new ColumnMatrix(constants->numRows());

    for (UInt i = 0; i < init->numRows(); i++)
    {
        init->setElement(i, this->guess ? this->guess->getElement(i) : 0.0);
    }

    ColumnMatrix *fnext = NULL;

	if (this->testingFunction)
    {
        fnext = new ColumnMatrix(init->numRows());

        if (this->testFunction(init, fnext))
        {
            delete fnext;

            this->numSteps = 0;

            /*if (this->guess)
            {
                delete init;

                return const_cast<Numerical::IColumnMatrix *>(guess);
            }*/

            this->unknowns = init;

            return;
        }
    }

    ColumnMatrix *next = new ColumnMatrix(init->numRows());

    PermutationMatrix *pre = (this->pivoting != NONE) ? new PermutationMatrix(this->matrix->numRows()) : NULL;
    PermutationMatrix *post = (this->pivoting == TOTAL) ? new PermutationMatrix(this->matrix->numRows()) : NULL;

    IMatrix *m = const_cast<IMatrix *>(this->matrix);
    IColumnMatrix *c = const_cast<IColumnMatrix *>(this->constants);

    if (this->pivoting != NONE)
    {
        m = this->matrix->clone();
        c = static_cast<IColumnMatrix *>(this->constants->clone());

        //this->pivot(m, pre, post);
        this->pivot(m, this->criterion, pre, post);

        pre->premultiply(c);

        delete pre;

        pre = NULL;
    }

    if (this->pivoting == TOTAL)
    {
        post->postmultiply(init);

        if (this->testingFunction)
        {
            post->postmultiply(fnext);
        }
    }

	for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
	{
		this->next(init, next);

		bool error = false;

		for (UInt i = 0; i < next->numRows(); i++)
        {
            if ((std::isinf(next->getElement(i))) || (std::isnan(next->getElement(i))))
            {
                error = true;

                break;
            }
        }

        if (error)
        {
            break;
        }

		//std::cout << numSteps << "\n\n" << init->text() << "\n" <<  next->text() << std::endl;

		if ((this->testingStepSize) && (this->testStepSize(init, next)))
        {
            if (pivoting == TOTAL)
            {
                post->premultiply(next);
            }

            delete init;

            if (fnext)
            {
                delete fnext;
            }

            if (m != this->matrix)
            {
                delete m;
            }

            if (c != this->constants)
            {
                delete c;
            }

            if (pre)
            {
                delete pre;
            }

            if (post)
            {
                delete post;
            }

            this->unknowns = next;

            return;
        }

		if ((this->testingFunction) && (this->testFunction(next, fnext)))
        {
            if (this->pivoting == TOTAL)
            {
                post->premultiply(next);
            }

            delete init;
            delete fnext;

            if (m != this->matrix)
            {
                delete m;
            }

            if (c != this->constants)
            {
                delete c;
            }

            if (pre)
            {
                delete pre;
            }

            if (post)
            {
                delete post;
            }

            this->unknowns = next;

            return;
        }

		std::swap(init, next);
	}

	this->numSteps = this->maxSteps;

	this->ok = false;

	delete next;

	if (fnext)
    {
        delete fnext;
    }

    if (m != this->matrix)
    {
        delete m;
    }

    if (c != this->constants)
    {
        delete c;
    }

    if (pre)
    {
        delete pre;
    }

    if (post)
    {
        delete post;
    }

    delete init;
}

bool Numerical::FixedPointLinearSystemSolver::testFunction(const IColumnMatrix *guess, IColumnMatrix *fguess) const
{
    Numerical::IMatrix::multiply(this->matrix, guess, fguess);

    for (UInt i = 0; i < fguess->numRows(); i++)
    {
        Real diff = std::fabs(fguess->getElement(i) - this->constants->getElement(i));

        if (diff > this->tolerance)
        {
            return false;
        }
    }

    return true;
}

bool Numerical::FixedPointLinearSystemSolver::testStepSize(const IColumnMatrix *guess, const IColumnMatrix *next) const
{
    for (UInt i = 0; i < guess->numRows(); i++)
    {
        Real diff = std::fabs(next->getElement(i) - guess->getElement(i));

        if (diff > this->tolerance)
        {
            return false;
        }
    }

    return true;
}

bool Numerical::FixedPointLinearSystemSolver::isSatisfyingRowsCriterion(const IMatrix *matrix)
{
    if (!matrix->isSquare())
    {
        return false;
    }

    for (UInt i = 0; i < matrix->numRows(); i++)
    {
        Real sum = sumRowForSassenfeld(matrix, i);

        if (sum >= std::fabs(matrix->getElement(i, i)))
        {
            return false;
        }
    }

    return true;
}

bool Numerical::FixedPointLinearSystemSolver::isSatisfyingSassenfeldCriterion(const IMatrix *matrix)
{
    if (!matrix->isSquare())
    {
        return false;
    }

    Real *beta = new Real[matrix->numRows()];

    for (UInt i = 0; i < matrix->numRows(); i++)
    {
        Real sum = sumRowForSassenfeld(matrix, i, beta);

        if (sum >= fabs(matrix->getElement(i, i)))
        {
            delete [] beta;

            return false;
        }

        beta[i] = sum/fabs(matrix->getElement(i, i));
    }

    delete [] beta;

    return true;
}

Real Numerical::FixedPointLinearSystemSolver::sumRowForSassenfeld(const IMatrix *matrix, UInt row, UInt insteadOfRow, const Real *beta)
{
    Real sum = 0;

    for (UInt j = 0; j < insteadOfRow; j++)
    {
        sum += std::fabs(matrix->getElement(row, j))*(beta ? beta[j] : 1.0);
    }

    for (UInt j = insteadOfRow + 1; j < matrix->numCols(); j++)
    {
        sum += std::fabs(matrix->getElement(row, j));
    }

    return sum;
}

Real Numerical::FixedPointLinearSystemSolver::sumRowForSassenfeld(const IMatrix *matrix, UInt row, const Real *beta)
{
    return sumRowForSassenfeld(matrix, row, row, beta);
}

void Numerical::FixedPointLinearSystemSolver::swapRows(IMatrix *matrix, IPermutationMatrix *pre, UInt i, UInt j) const
{
    pre->swapRows(i, j);

    for (UInt k = 0; k < matrix->numCols(); k++)
    {
        Real v = matrix->getElement(i, k);

        matrix->setElement(i, k, matrix->getElement(j, k));

        matrix->setElement(j, k, v);
    }
}

void Numerical::FixedPointLinearSystemSolver::swapCols(IMatrix *matrix, IPermutationMatrix *post, UInt i, UInt j) const
{
    post->swapRows(i, j);

    for (UInt k = 0; k < matrix->numRows(); k++)
    {
        Real v = matrix->getElement(k, i);

        matrix->setElement(k, i, matrix->getElement(k, j));

        matrix->setElement(k, j, v);
    }
}

void Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post) const
{
    if (this->pivoting == NONE)
    {
        return;
    }

    for (UInt k = 0; k < matrix->numRows() - 1; k++)
    {
        UInt row = k, col = k;

        for (UInt i = k + 1; i < matrix->numRows(); i++)
        {
            if ((std::fabs(matrix->getElement(i, k)) > std::fabs(matrix->getElement(row, col))) &&
                (std::fabs(matrix->getElement(k, i)) > this->tolerance))
            {
                row = i;
            }
        }

        if (this->pivoting == TOTAL)
        {
            for (UInt i = k; i < matrix->numRows(); i++)
            {
                for (UInt j = k + 1; j < matrix->numCols(); j++)
                {
                    if ((std::fabs(matrix->getElement(i, j)) > std::fabs(matrix->getElement(row, col))) &&
                        (std::fabs(matrix->getElement(j, k)) > this->tolerance) &&
                        ((i == k) || (std::fabs(matrix->getElement(k, i)) > this->tolerance)))
                    {
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != k)
        {
            this->swapRows(matrix, pre, row, k);
        }

        if (col != k)
        {
            this->swapCols(matrix, post, col, k);
        }
    }
}

void Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, enum Criterion criterion, IPermutationMatrix *pre, IPermutationMatrix *post) const
{
    if (this->pivoting == NONE)
    {
        return;
    }

    if (criterion == NO_CRITERION)
    {
        this->pivot(matrix, pre, post);

        return;
    }

    Real *beta = (criterion == ROWS) ? NULL : new Real[matrix->numRows()];

    this->pivot(matrix, criterion, 0, beta, pre, post);

    if (beta)
    {
        delete beta;
    }
}

bool Numerical::FixedPointLinearSystemSolver::pivot(IMatrix *matrix, enum Criterion criterion, UInt step, Real *beta, IPermutationMatrix *pre, IPermutationMatrix *post) const
{
    UInt maxCol = (this->pivoting == PARTIAL) ? step + 1 : matrix->numCols();

    for (UInt col = step; col < maxCol; col++)
    {
        if ((this->pivoting == TOTAL) && (step != col))
        {
            this->swapCols(matrix, post, step, col);
        }

        for (UInt row = step; row < matrix->numRows(); row++)
        {
            if (std::fabs(matrix->getElement(row, step)) <= this->tolerance)
            {
                continue;
            }

            Real sum = this->sumRowForSassenfeld(matrix, row, step, beta);

            if (sum >= std::fabs(matrix->getElement(row, step)))
            {
                continue;
            }

            if (criterion == SASSENFELD)
            {
                beta[step] = sum/fabs(matrix->getElement(row, step));
            }

            if (step != row)
            {
                this->swapRows(matrix, pre, step, row);
            }

            if (this->pivot(matrix, criterion, step + 1, beta, pre, post))
            {
                return true;
            }

            if (step != row)
            {
                this->swapRows(matrix, pre, step, row);
            }
        }

        if ((this->pivoting == TOTAL) && (step != col))
        {
            this->swapCols(matrix, post, step, col);
        }
    }

    return false;
}

#endif //#if USE_NUMERICAL
