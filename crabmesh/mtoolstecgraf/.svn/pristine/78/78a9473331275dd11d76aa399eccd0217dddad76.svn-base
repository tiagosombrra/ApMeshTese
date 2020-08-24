#include "Numerical/FixedPointEquationRootFinder.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"
#if USE_GUI
#include "Data/Point3D.h"
#endif //#if USE_GUI

using namespace Data;
using namespace Numerical;

/*Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
	ok = true;

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	Real init = function->initial();
	Real finit = function->f(init);

	if ((testFunction) && (std::fabs(finit) <= tolerance))
	{
	    numSteps = 0;

		return init;
	}

	Real next = 0.0, fnext = 0.0;

	//std::cout << "step\tx\tf(x)\tphi(x)" << std::endl;

	for (numSteps = 0; numSteps < maxSteps; numSteps++)
	{
		next = this->next(function, init, finit);

		//std::cout << numSteps << "\t" << init << "\t" << finit << "\t" << next << std::endl;

		if ((testStepSize) && (std::fabs(next - init) <= tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << next << std::endl;

			return next;
		}

		fnext = function->f(next);

		if ((testFunction) && (std::fabs(fnext) <= tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << next << "\t" << fnext << std::endl;

			return next;
		}

		init = next;
		finit = fnext;
	}

	numSteps = maxSteps;

	ok = false;

	return init;
}

Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->execute(function, maxSteps, tolerance, true, true, ok, numSteps);
}

Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function, bool &ok, UInt &numSteps)
{
	return this->execute(function, EquationRootFinder::MAX_STEPS, EquationRootFinder::TOLERANCE, ok, numSteps);
}

Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function, UInt &numSteps)
{
    bool ok = true;

    return this->execute(function, ok, numSteps);
}

Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->execute(function, ok, numSteps);
}

Real Numerical::FixedPointEquationRootFinder::execute(FixedPointEquationRootFunction *function)
{
	bool ok;

	return this->execute(function, ok);
}

Real Numerical::FixedPointEquationRootFinder::execute(EquationRootFunction *function)
{
    return this->execute(dynamic_cast<FixedPointEquationRootFunction *>(function));
}

Real Numerical::FixedPointEquationRootFinder::next(FixedPointEquationRootFunction *function, Real x, Real *//*fx*//*)
{
    return function->step()->f(x);
}*/

Numerical::FixedPointEquationRootFinder::FixedPointEquationRootFinder() :
    Numerical::EquationRootFinder::EquationRootFinder()
{
    this->step = NULL;
    this->initial = 0.0;
}

Numerical::FixedPointEquationRootFinder::~FixedPointEquationRootFinder()
{

}

void Numerical::FixedPointEquationRootFinder::setInitial(Real initial)
{
    this->initial = initial;
}

Real Numerical::FixedPointEquationRootFinder::getInitial() const
{
    return this->initial;
}

void Numerical::FixedPointEquationRootFinder::setStep(Function *step)
{
    this->step = step;
}

Function *Numerical::FixedPointEquationRootFinder::getStep() const
{
    return this->step;
}

void Numerical::FixedPointEquationRootFinder::execute()
{
#if USE_GUI
    this->points.clear();
#endif //#if USE_GUI

    this->ok = true;

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	Real init = this->initial;
	Real finit = this->function->f(init);

#if USE_GUI
    this->store(init, finit);
#endif //#if USE_GUI

	if ((this->testingFunction) && (std::fabs(finit) <= this->tolerance))
	{
	    this->numSteps = 0;

	    this->root = init;

		return;
	}

	Real next = 0.0, fnext = 0.0;

	//std::cout << "step\tx\tf(x)\tphi(x)" << std::endl;

	for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
	{
		next = this->next(init, finit);

		//std::cout << numSteps << "\t" << init << "\t" << finit << "\t" << next << std::endl;

		if ((this->testingStepSize) && (std::fabs(next - init) <= this->tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << next << std::endl;

			this->root = next;

#if USE_GUI
            this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI

			return;
		}

		fnext = this->function->f(next);

#if USE_GUI
        this->store(next, fnext);
#endif //#if USE_GUI

		if ((this->testingFunction) && (std::fabs(fnext) <= this->tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << next << "\t" << fnext << std::endl;

		    this->root = next;

			return;
		}

		init = next;
		finit = fnext;
	}

	this->numSteps = this->maxSteps;

	this->ok = false;

	this->root = init;

#if USE_GUI
    this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI
}

Real Numerical::FixedPointEquationRootFinder::next(Real x, Real /*fx*/) const
{
    return this->step->f(x);
}

#endif //#if USE_NUMERICAL
