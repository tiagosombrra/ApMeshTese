#ifndef _NUMERICAL_LINEAR_SYSTEM_SOLVER_H_
#define _NUMERICAL_LINEAR_SYSTEM_SOLVER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
	class LinearSystemSolver : public Performer::Builder
	{
	public:

	    ///by default, MAX_STEPS = 1000, TOLERANCE = 0.000001

	    static const UInt MAX_STEPS;
	    static const Real TOLERANCE;

	    enum Pivoting
        {
            NONE = 0,
            PARTIAL,
            TOTAL
        };

    public:

        LinearSystemSolver();
        virtual ~LinearSystemSolver();

		//virtual IColumnMatrix *execute(const IMatrix *matrix, const IColumnMatrix *constants) = 0;

		//virtual IColumnMatrix *executeBackwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants);
		//virtual IColumnMatrix *executeForwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants);

		virtual void setMatrix(const IMatrix *matrix);
		virtual const IMatrix *getMatrix() const;

		virtual void setConstants(const IColumnMatrix *constants);
		virtual const IColumnMatrix *getConstants() const;

		virtual void setTolerance(Real tolerance);
	    virtual Real getTolerance() const;

	    virtual bool isOk() const;

	    virtual IColumnMatrix *getUnknowns() const;

		virtual IColumnMatrix *backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants);
		virtual IColumnMatrix *backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, bool guess);
		virtual void backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, IColumnMatrix *unknowns);
		virtual void backwardSubstitution(const IMatrix *upperTriangular, const IColumnMatrix *constants, bool guess, IColumnMatrix *unknowns);

		virtual IColumnMatrix *forwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants);
		virtual void forwardSubstitution(const IMatrix *lowerTriangular, const IColumnMatrix *constants, IColumnMatrix *unknowns);

    protected:

        const IMatrix *matrix;
        const IColumnMatrix *constants;

        IColumnMatrix *unknowns;

        Real tolerance;
        bool ok;

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_LINEAR_SYSTEM_SOLVER_H_
