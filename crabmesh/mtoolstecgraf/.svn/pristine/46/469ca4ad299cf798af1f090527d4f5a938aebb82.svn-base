#ifndef _NUMERICAL_LU_DECOMPOSITION_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_LU_DECOMPOSITION_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/GaussianEliminationLinearSystemSolver.h"

namespace Numerical
{
	//class LUDecompositionLinearSystemSolver : public Numerical::LinearSystemSolver
	class LUDecompositionLinearSystemSolver : public Numerical::GaussianEliminationLinearSystemSolver
	{
    /*public:

        enum Pivoting
        {
            NONE = 0,
            PARTIAL,
            TOTAL
        };

	public:

		virtual ILUMatrix *decompose(const IMatrix *matrix, enum Pivoting pivoting, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance, bool &ok);
		virtual ILUMatrix *decompose(const IMatrix *matrix, enum Pivoting pivoting, IPermutationMatrix *pre, IPermutationMatrix *post, bool &ok);
		virtual ILUMatrix *decompose(const IMatrix *matrix, enum Pivoting pivoting, IPermutationMatrix *pre, IPermutationMatrix *post);

		virtual IColumnMatrix *execute(const ILUMatrix *lu, const IColumnMatrix *constants, enum Pivoting pivoting, const IPermutationMatrix *pre, const IPermutationMatrix *post, bool &ok);
		virtual IColumnMatrix *execute(const ILUMatrix *lu, const IColumnMatrix *constants, enum Pivoting pivoting, const IPermutationMatrix *pre, const IPermutationMatrix *post);

		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, Real tolerance, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting);

		virtual ILUMatrix *decompose(const IMatrix *matrix, Real tolerance, bool &ok);
		virtual ILUMatrix *decompose(const IMatrix *matrix, bool &ok);
		virtual ILUMatrix *decompose(const IMatrix *matrix);

		virtual IColumnMatrix *execute(const ILUMatrix *lu, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const ILUMatrix *lu, const IColumnMatrix *constants);

		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual ILUMatrix *decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre, Real tolerance, bool &ok);
		virtual ILUMatrix *decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre, bool &ok);
		virtual ILUMatrix *decomposePartial(const IMatrix *matrix, IPermutationMatrix *pre);

		virtual IColumnMatrix *executePartial(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre, bool &ok);
		virtual IColumnMatrix *executePartial(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre);

		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual ILUMatrix *decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, Real tolerance, bool &ok);
		virtual ILUMatrix *decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post, bool &ok);
		virtual ILUMatrix *decomposeTotal(const IMatrix *matrix, IPermutationMatrix *pre, IPermutationMatrix *post);

		virtual IColumnMatrix *executeTotal(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre, const IPermutationMatrix *post, bool &ok);
		virtual IColumnMatrix *executeTotal(const ILUMatrix *lu, const IColumnMatrix *constants, const IPermutationMatrix *pre, const IPermutationMatrix *post);

		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, Real tolerance, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants);*/

    public:

        LUDecompositionLinearSystemSolver();
        virtual ~LUDecompositionLinearSystemSolver();

        virtual void setLU(ILUMatrix *lu);
        virtual ILUMatrix *getLU() const;

        virtual void execute();

        virtual void decompose();
        virtual void solve();

    protected:

        using Numerical::GaussianEliminationLinearSystemSolver::setCopy;
        using Numerical::GaussianEliminationLinearSystemSolver::isCopy;

    protected:


        ILUMatrix *lu;
        mutable bool luGot;

        PermutationMatrix *pre;
        PermutationMatrix *post;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_LU_DECOMPOSITION_LINEAR_SYSTEM_SOLVER_H_
