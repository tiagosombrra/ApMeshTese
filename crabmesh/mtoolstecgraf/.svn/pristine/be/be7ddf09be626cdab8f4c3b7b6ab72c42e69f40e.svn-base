#include "Numerical/SecantEquationRootFinder.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"
#if USE_GUI
#include "Data/Point3D.h"
#endif //#if USE_GUI

using namespace Data;
using namespace Numerical;

/*Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function,
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

	Real init2 = function->secondInitial();

	if ((testStepSize) && (std::fabs(init2 - init) <= tolerance))
    {
        numSteps = 0;

        return init2;
    }

	Real finit2 = function->f(init2);

	if ((testFunction) && (std::fabs(finit2) <= tolerance))
	{
	    numSteps = 0;

		return init2;
	}

    Real next = 0.0, fnext = 0.0;

    std::cout << "step\txk-1\txk\tf(xk-1)\tf(xk)\tphi(x)" << std::endl;

	for (numSteps = 0; numSteps < maxSteps; numSteps++)
	{
		next = this->next(function, init, init2, finit, finit2);

		std::cout << numSteps << "\t" << init << "\t" << init2 << "\t" << finit << "\t" << finit2 << "\t" << next << std::endl;

		if ((testStepSize) && (std::fabs(next - init2) <= tolerance))
		{
		    std::cout << numSteps+1 << "\t" << next << std::endl;

			return next;
		}

		fnext = function->f(next);

		if ((testFunction) && (std::fabs(fnext) <= tolerance))
		{
		    std::cout << numSteps+1 << "\t" << init2 << "\t" << next << "\t" << finit2 << "\t" << fnext << std::endl;

			return next;
		}

		init = init2;
		finit = finit2;

		init2 = next;
		finit2 = fnext;
	}

	numSteps = maxSteps;

	ok = false;

	return init2;
}

Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->execute(function, maxSteps, tolerance, true, true, ok, numSteps);
}

Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function, bool &ok, UInt &numSteps)
{
	return this->execute(function, EquationRootFinder::MAX_STEPS, EquationRootFinder::TOLERANCE, ok, numSteps);
}

Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function, UInt &numSteps)
{
    bool ok = true;

    return this->execute(function, ok, numSteps);
}

Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->execute(function, ok, numSteps);
}

Real Numerical::SecantEquationRootFinder::execute(SecantEquationRootFunction *function)
{
	bool ok;

	return this->execute(function, ok);
}

Real Numerical::SecantEquationRootFinder::execute(EquationRootFunction *function)
{
    return this->execute(dynamic_cast<SecantEquationRootFunction *>(function));
}

Real Numerical::SecantEquationRootFinder::next(SecantEquationRootFunction * *//*function*//*, Real x0, Real x1, Real fx0, Real fx1)
{
    return (x0*fx1 - x1*fx0)/(fx1 - fx0);
}*/

Numerical::SecantEquationRootFinder::SecantEquationRootFinder() :
    Numerical::EquationRootFinder::EquationRootFinder()
{
    this->initial = 0.0;
    this->initial2 = 0.0;
}

Numerical::SecantEquationRootFinder::~SecantEquationRootFinder()
{

}

void Numerical::SecantEquationRootFinder::setInitial(Real initial)
{
    this->initial = initial;
}

Real Numerical::SecantEquationRootFinder::getInitial() const
{
    return this->initial;
}

void Numerical::SecantEquationRootFinder::setInitial2(Real initial2)
{
    this->initial2 = initial2;
}

Real Numerical::SecantEquationRootFinder::getInitial2() const
{
    return this->initial2;
}

void Numerical::SecantEquationRootFinder::execute()
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

	if ((this->testingFunction) && (std::fabs(finit) <= tolerance))
	{
	    this->numSteps = 0;

		this->root = init;

		return;
	}

	Real init2 = this->initial2;

	if ((this->testingStepSize) && (std::fabs(init2 - init) <= tolerance))
    {
        this->numSteps = 0;

        this->root = init2;

#if USE_GUI
        this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI

        return;
    }

	Real finit2 = this->function->f(init2);

#if USE_GUI
    this->store(init2, finit2);
#endif //#if USE_GUI

	if ((this->testingFunction) && (std::fabs(finit2) <= tolerance))
	{
	    this->numSteps = 0;

		this->root = init2;

		return;
	}

    Real next = 0.0, fnext = 0.0;

    //std::cout << "step\txk-1\txk\tf(xk-1)\tf(xk)\tphi(x)" << std::endl;

	for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
	{
		next = this->next(init, init2, finit, finit2);

		//std::cout << numSteps << "\t" << init << "\t" << init2 << "\t" << finit << "\t" << finit2 << "\t" << next << std::endl;

		if ((this->testingStepSize) && (std::fabs(next - init2) <= tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << next << std::endl;

			this->root = next;

#if USE_GUI
            this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI

			return;
		}

		fnext = function->f(next);

#if USE_GUI
        this->store(next, fnext);
#endif //#if USE_GUI

		if ((this->testingFunction) && (std::fabs(fnext) <= tolerance))
		{
		    //std::cout << numSteps+1 << "\t" << init2 << "\t" << next << "\t" << finit2 << "\t" << fnext << std::endl;

			this->root = next;

			return;
		}

		init = init2;
		finit = finit2;

		init2 = next;
		finit2 = fnext;
	}

	this->numSteps = this->maxSteps;

	this->ok = false;

	this->root = init2;

#if USE_GUI
    this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI
}

Real Numerical::SecantEquationRootFinder::next(Real x0, Real x1, Real fx0, Real fx1) const
{
    return (x0*fx1 - x1*fx0)/(fx1 - fx0);
}

#endif //#if USE_NUMERICAL
