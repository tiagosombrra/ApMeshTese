#ifndef _NUMERICAL_EQUATION_ROOT_FINDER_H_
#define _NUMERICAL_EQUATION_ROOT_FINDER_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
	class EquationRootFinder : public Performer::Builder
	{
	public:

	    ///by default, MAX_STEPS = 1000, TOLERANCE = 0.000001

	    static const UInt MAX_STEPS;
	    static const Real TOLERANCE;

    public:

        EquationRootFinder();
        virtual ~EquationRootFinder();

        virtual void setFunction(Function *function);
        virtual Function *getFunction() const;

        virtual Real getRoot() const;

        virtual void setTestingFunction(bool testingFunction);
        virtual bool isTestingFunction() const;

        virtual void setTestingStepSize(bool testingStepSize);
        virtual bool isTestingStepSize() const;

        virtual void setMaxSteps(UInt maxSteps);
        virtual UInt getMaxSteps() const;

        virtual void setTolerance(Real tolerance);
        virtual Real getTolerance() const;

        virtual bool isOk() const;
        virtual UInt getNumSteps() const;

		//virtual Real execute(EquationRootFunction *function) = 0;

#if USE_GUI
        virtual void setStoringPoints(bool storingPoints);
        virtual bool isStoringPoints() const;

        virtual Data::Point3DObjVector getPoints() const;
#endif //#if USE_GUI

#if USE_GUI
        virtual void store(Real x, Real y);
#endif //#if USE_GUI

    protected:

        Function *function;
        Real root;

        bool testingFunction;
        bool testingStepSize;

        UInt maxSteps;
        Real tolerance;

        bool ok;
        UInt numSteps;

#if USE_GUI
        bool storingPoints;

        Data::Point3DObjVector points;
#endif //#if USE_GUI

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_EQUATION_ROOT_FINDER_H_
