#ifndef _NUMERICAL_FIXED_POINT_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_FIXED_POINT_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/LinearSystemSolver.h"

namespace Numerical
{
	class FixedPointLinearSystemSolver : public Numerical::LinearSystemSolver
	{
    public:

        /*enum Pivoting
        {
            NONE = 0,
            PARTIAL,
            TOTAL
        };*/

        enum Criterion
        {
            NO_CRITERION = 0,
            ROWS,
            SASSENFELD
        };

	/*public:

		virtual bool testFunction(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, Real tolerance, IColumnMatrix *fguess) const;
		virtual bool testStepSize(const IColumnMatrix *guess, const IColumnMatrix *next, Real tolerance) const;

		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, enum Pivoting pivoting, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);

		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, bool &ok, UInt &numSteps);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, UInt &numSteps);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual void next(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, IColumnMatrix *next) const = 0;

		virtual bool satisfyRowsCriterion(const IMatrix *matrix) const;
		virtual bool satisfySassenfeldCriterion(const IMatrix *matrix) const;

		virtual void pivot(IMatrix *matrix, enum Pivoting pivoting, enum Criterion criterion, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const;

		virtual void pivot(IMatrix *matrix, enum Pivoting pivoting, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const;
		virtual void pivotPartial(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const;
		virtual void pivotTotal(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const;

		virtual void pivotPartialForRowsCriterion(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const;
		virtual void pivotTotalForRowsCriterion(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const;
		virtual void pivotPartialForSassenfeldCriterion(IMatrix *matrix, IPermutationMatrix *pre, Real tolerance) const;
		virtual void pivotTotalForSassenfeldCriterion(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance) const;*/

    public:

        FixedPointLinearSystemSolver();
        virtual ~FixedPointLinearSystemSolver();

        virtual void setGuess(IColumnMatrix *guess);
        virtual IColumnMatrix *getGuess() const;

        virtual void setPivoting(enum Pivoting pivoting);
        virtual enum Pivoting getPivoting() const;

        virtual void setCriterion(enum Criterion criterion);
        virtual enum Criterion getCriterion() const;

        virtual void setTestingFunction(bool testingFunction);
        virtual bool isTestingFunction() const;

        virtual void setTestingStepSize(bool testingStepSize);
        virtual bool getTestingStepSize() const;

        virtual void setMaxSteps(UInt maxSteps);
        virtual UInt getMaxSteps() const;

        virtual UInt getNumSteps() const;

		virtual void execute();

        virtual bool testFunction(const IColumnMatrix *guess, IColumnMatrix *fguess) const;
		virtual bool testStepSize(const IColumnMatrix *guess, const IColumnMatrix *next) const;

		static bool isSatisfyingRowsCriterion(const IMatrix *matrix);
		static bool isSatisfyingSassenfeldCriterion(const IMatrix *matrix);

    protected:

		virtual void next(const IColumnMatrix *guess, IColumnMatrix *next) const = 0;

        static Real sumRowForSassenfeld(const IMatrix *matrix, UInt row, UInt insteadOfRow, const Real *beta = NULL);
        static Real sumRowForSassenfeld(const IMatrix *matrix, UInt row, const Real *beta = NULL);

        virtual void swapRows(IMatrix *matrix, IPermutationMatrix *pre, UInt i, UInt j) const;
        virtual void swapCols(IMatrix *matrix, IPermutationMatrix *post, UInt i, UInt j) const;

        virtual void pivot(IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post) const;
        virtual void pivot(IMatrix *matrix, enum Criterion criterion, IPermutationMatrix *pre, IPermutationMatrix *post) const;
		virtual bool pivot(IMatrix *matrix, enum Criterion criterion, UInt step, Real *beta, IPermutationMatrix *pre, IPermutationMatrix *post) const;

    protected:

        IColumnMatrix *guess;

        enum Pivoting pivoting;
        enum Criterion criterion;

        bool testingFunction;
        bool testingStepSize;

        UInt maxSteps;
        UInt numSteps;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_FIXED_POINT_LINEAR_SYSTEM_SOLVER_H_
