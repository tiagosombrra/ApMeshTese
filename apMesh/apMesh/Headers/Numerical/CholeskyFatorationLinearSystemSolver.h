#ifndef _NUMERICAL_CHOLESKY_FATORATION_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_CHOLESKY_FATORATION_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/LinearSystemSolver.h"

namespace Numerical
{
	class CholeskyFatorationLinearSystemSolver : public Numerical::LinearSystemSolver
	{
	public:

		/*virtual ISymmetricMatrix *decompose(const IMatrix *posDefSymmetric, Real tolerance, bool &ok);
		virtual ISymmetricMatrix *decompose(const IMatrix *posDefSymmetric, bool &ok);
		virtual ISymmetricMatrix *decompose(const IMatrix *posDefSymmetric);

		virtual IColumnMatrix *execute(const ISymmetricMatrix *g, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const ISymmetricMatrix *g, const IColumnMatrix *constants);

		virtual IColumnMatrix *execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants, Real tolerance, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *posDefSymmetric, const IColumnMatrix *constants);*/

		CholeskyFatorationLinearSystemSolver();
		virtual ~CholeskyFatorationLinearSystemSolver();

		virtual ISymmetricMatrix *getFatoration() const;

		virtual void execute();

		virtual void decompose();
		virtual void solve();

    protected:

        ISymmetricMatrix *fatoration;
        mutable bool fatorationGot;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_CHOLESKY_FATORATION_LINEAR_SYSTEM_SOLVER_H_
