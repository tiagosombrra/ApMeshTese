#ifndef _NUMERICAL_GAUSS_JACOBI_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_GAUSS_JACOBI_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Numerical/FixedPointLinearSystemSolver.h"

namespace Numerical
{
	class GaussJacobiLinearSystemSolver : public Numerical::FixedPointLinearSystemSolver
	{
	public:

		GaussJacobiLinearSystemSolver();
		virtual ~GaussJacobiLinearSystemSolver();

		virtual void next(const IColumnMatrix *guess, IColumnMatrix *next) const;
		//virtual void next(const IMatrix *matrix, const IColumnMatrix *constants, const IColumnMatrix *guess, IColumnMatrix *next) const;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_GAUSS_JACOBI_LINEAR_SYSTEM_SOLVER_H_
