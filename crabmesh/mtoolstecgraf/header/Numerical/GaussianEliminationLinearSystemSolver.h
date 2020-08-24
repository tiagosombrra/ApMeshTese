#ifndef _NUMERICAL_GAUSSIAN_ELIMINATION_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_GAUSSIAN_ELIMINATION_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/LinearSystemSolver.h"

namespace Numerical
{
	class GaussianEliminationLinearSystemSolver : public Numerical::LinearSystemSolver
	{
	public:

	    GaussianEliminationLinearSystemSolver();
	    virtual ~GaussianEliminationLinearSystemSolver();

	    virtual void setPivoting(enum Pivoting pivoting);
	    virtual enum Pivoting getPivoting() const;

	    virtual void setCopy(bool copy);
	    virtual bool isCopy() const;

	    virtual void setSkip(bool skip);
	    virtual bool isSkip() const;

		/*///by default, max = 1000, tolerance = 0.0001
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, enum Pivoting pivoting, Real tolerance, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, enum Pivoting pivoting, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, enum Pivoting pivoting);

		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, Real tolerance, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, Real tolerance, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executePartial(const IMatrix *matrix, const IColumnMatrix *constants);

		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, Real tolerance, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool copy, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants, bool &ok);
		virtual IColumnMatrix *executeTotal(const IMatrix *matrix, const IColumnMatrix *constants);*/

		virtual void execute();

    protected:

        enum Pivoting pivoting;

        bool copy;
        bool skip;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_GAUSSIAN_ELIMINATION_LINEAR_SYSTEM_SOLVER_H_
