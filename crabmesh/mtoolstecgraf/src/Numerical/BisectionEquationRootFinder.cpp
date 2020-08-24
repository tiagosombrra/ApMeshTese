#include "Numerical/BisectionEquationRootFinder.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"

#if USE_GUI
#include "Data/Point3D.h"
#endif //#if USE_GUI

using namespace Data;
using namespace Numerical;

/*Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, UInt &numSteps)
{
	ok = true;

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	Real a = function->min();
	Real b = function->max();

	Real min = std::min(a, b);
	Real fmin = function->f(min);

	if ((testFunction) && (std::fabs(fmin) <= tolerance))
	{
	    numSteps = 0;

		return min;
	}

	Real max = std::max(a, b);
	Real fmax = function->f(max);

	if ((testFunction) && (std::fabs(fmax) <= tolerance))
	{
	    numSteps = 0;

		return max;
	}

	if (fmin*fmax >= 0.0)
	{
	    numSteps = maxSteps;

		ok = false;

		return this->next(function, min, max, fmin, fmax);
	}

	Real mid = 0.0, fmid = 0.0;

	//std::cout << "step\ta\tb\tfa\tfb\tm\tfm\tb-a" << std::endl;

	for (numSteps = 0; numSteps < maxSteps; numSteps++)
	{
		mid = this->next(function, min, max, fmin, fmax);

		if ((testStepSize) && ((max - min) <= tolerance))
		{
		    //std::cout << numSteps << "\t" << min << "\t" << max << "\t" << fmin << "\t" << fmax << "\t" << mid << "\t" << function->f(mid) << "\t" << max-min << std::endl;
			return mid;
		}

		fmid = function->f(mid);

		//std::cout << numSteps << "\t" << min << "\t" << max << "\t" << fmin << "\t" << fmax << "\t" << mid << "\t" << fmid << "\t" << max-min << std::endl;

		if ((testFunction) && (std::fabs(fmid) <= tolerance))
		{
			return mid;
		}

		if (fmin*fmid < 0.0)
		{
			max = mid;
			fmax = fmid;
		}
		else
		{
			min = mid;
			fmin = fmid;
		}
	}

	numSteps = maxSteps;

	ok = false;

	return mid;
}

Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function, UInt maxSteps, Real tolerance, bool &ok, UInt &numSteps)
{
	return this->execute(function, maxSteps, tolerance, true, true, ok, numSteps);
}

Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function, bool &ok, UInt &numSteps)
{
	return this->execute(function, EquationRootFinder::MAX_STEPS, EquationRootFinder::TOLERANCE, ok, numSteps);
}

Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function, UInt &numSteps)
{
    bool ok = true;

    return this->execute(function, ok, numSteps);
}

Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function, bool &ok)
{
    UInt numSteps = UINT_MAX;

    return this->execute(function, ok, numSteps);
}

Real Numerical::BisectionEquationRootFinder::execute(IntervalEquationRootFunction *function)
{
	bool ok = true;

	return this->execute(function, ok);
}

Real Numerical::BisectionEquationRootFinder::execute(EquationRootFunction *function)
{
    return this->execute(dynamic_cast<IntervalEquationRootFunction *>(function));
}

Real Numerical::BisectionEquationRootFinder::next(IntervalEquationRootFunction * *//*function*//*, Real min, Real max, Real *//*fmin*//*, Real *//*fmax*//*)
{
    return 0.5*(min + max);
}*/

Numerical::BisectionEquationRootFinder::BisectionEquationRootFinder() :
    Numerical::EquationRootFinder::EquationRootFinder()
{
    this->min = 0.0;
    this->max = 0.0;
}

Numerical::BisectionEquationRootFinder::~BisectionEquationRootFinder()
{

}

void Numerical::BisectionEquationRootFinder::setMin(Real min)
{
    this->min = min;
}

Real Numerical::BisectionEquationRootFinder::getMin() const
{
    return this->min;
}

void Numerical::BisectionEquationRootFinder::setMax(Real max)
{
    this->max = max;
}

Real Numerical::BisectionEquationRootFinder::getMax() const
{
    return this->max;
}

void Numerical::BisectionEquationRootFinder::execute()
{
#if USE_GUI
    this->points.clear();
#endif //#if USE_GUI

    this->ok = true;

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	Real a = std::min(this->min, this->max);
	Real fa = this->function->f(a);

#if USE_GUI
    this->store(a, fa);
#endif //#if USE_GUI

	if ((this->testingFunction) && (std::fabs(fa) <= this->tolerance))
	{
	    this->numSteps = 0;

		this->root = a;

		return;
	}

	Real b = std::max(this->min, this->max);
	Real fb = this->function->f(b);

#if USE_GUI
    this->store(b, fb);
#endif //#if USE_GUI

	if ((this->testingFunction) && (std::fabs(fb) <= this->tolerance))
	{
	    this->numSteps = 0;

		this->root = b;

		return;
	}

	if (fa*fb >= 0.0)
	{
	    this->numSteps = this->maxSteps;

		this->ok = false;

		this->root = this->next(a, b, fa, fb);

#if USE_GUI
        this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI

		return;
	}

	Real m = 0.0, fm = 0.0;

	//std::cout << "step\ta\tb\tfa\tfb\tm\tfm\tb-a" << std::endl;

	for (this->numSteps = 0; this->numSteps < this->maxSteps; this->numSteps++)
	{
		m = this->next(a, b, fa, fb);

		if ((this->testingStepSize) && ((b - a) <= this->tolerance))
		{
		    //std::cout << numSteps << "\t" << min << "\t" << max << "\t" << fmin << "\t" << fmax << "\t" << mid << "\t" << function->f(mid) << "\t" << max-min << std::endl;
			this->root = m;

#if USE_GUI
            this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI

			return;
		}

		fm = this->function->f(m);

#if USE_GUI
        this->store(m, fm);
#endif //#if USE_GUI

		//std::cout << numSteps << "\t" << min << "\t" << max << "\t" << fmin << "\t" << fmax << "\t" << mid << "\t" << fmid << "\t" << max-min << std::endl;

		if ((this->testingFunction) && (std::fabs(fm) <= this->tolerance))
		{
			this->root = m;

			return;
		}

		this->narrowDownRange(a, b, fa, fb, m, fm);
	}

	this->numSteps = this->maxSteps;

	this->ok = false;

	this->root = m;

#if USE_GUI
    this->store(this->root, this->function->f(this->root));
#endif //#if USE_GUI
}

Real Numerical::BisectionEquationRootFinder::next(Real a, Real b, Real /*fa*/, Real /*fb*/) const
{
    return 0.5*(a + b);
}

void Numerical::BisectionEquationRootFinder::narrowDownRange(Real &a, Real &b, Real &fa, Real &fb, Real &m, Real &fm) const
{
    if (fa*fm < 0.0)
    {
        b = m;
        fb = fm;
    }
    else
    {
        a = m;
        fa = fm;
    }
}

/*Real Numerical::ClosestBisectionEquationRootFinder::recursiveExecute(IntervalEquationRootFunction *function,
	Real min, Real max, Real fmin, Real fmax,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval)
{
	ok = true;

	Real mid = this->next(function, min, max, fmin, fmax);

	if (maxSteps == 0)
	{
		ok = false;

		fval = 0.0;

		return mid;
	}

	Real fmid = function->f(mid);

	if ((testStepSize) && ((max - min) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	if ((testFunction) && (std::fabs(fmid) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if (std::fabs(fmin) <= std::fabs(fmax) + tolerance)
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if (std::fabs(fmin) + tolerance >= std::fabs(fmax))
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - mid) < tolerance))
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - mid) < tolerance))
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (std::fabs(fval2) < std::fabs(fval1))) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (std::fabs(fmid) < std::fabs(fval1))
		{
			fval = fmid;

			return mid;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fmid;

	return mid;
}

Real Numerical::ClosestBisectionEquationRootFinder::execute(IntervalEquationRootFunction *function,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok)
{
	ok = true;

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	Real a = function->min();
	Real b = function->max();

	Real min = std::min(a, b);
	Real fmin = function->f(min);

	if ((testFunction) && (std::fabs(fmin) <= tolerance))
	{
		return min;
	}

	Real max = std::max(a, b);
	Real fmax = function->f(max);

	if ((testFunction) && (std::fabs(fmax) <= tolerance))
	{
		return max;
	}

	Real fval = 0.0;

	Real val = this->recursiveExecute(function, min, max, fmin, fmax, maxSteps, tolerance, testFunction, testStepSize, ok, fval);
	//Real val = this->recursiveExecute(function, min, max, fmin, fmax, maxSteps, tolerance, true, false, ok, fval);

	if (std::fabs(fmax) < std::fabs(fmin))
	{
		fmin = fmax;

		min = max;
	}

	if (ok)
	{
		if (std::fabs(fmin) < std::fabs(fval))
		{
			return min;
		}

		return val;
	}

	return min;
}*/

Numerical::ClosestBisectionEquationRootFinder::ClosestBisectionEquationRootFinder() :
    Numerical::BisectionEquationRootFinder::BisectionEquationRootFinder()
{
    this->ignoringSigns = true;
}

Numerical::ClosestBisectionEquationRootFinder::~ClosestBisectionEquationRootFinder()
{

}

void Numerical::ClosestBisectionEquationRootFinder::setIgnoringSigns(bool ignoringSigns)
{
    this->ignoringSigns = ignoringSigns;
}

bool Numerical::ClosestBisectionEquationRootFinder::isIgnoringSigns() const
{
    return this->ignoringSigns;
}

void Numerical::ClosestBisectionEquationRootFinder::execute()
{
    this->ok = true;
    this->numSteps = 0;

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	Real a = std::min(this->min, this->max);
	Real fa = function->f(a);

	if ((this->testingFunction) && (std::fabs(fa) <= this->tolerance))
	{
		this->root = a;

		return;
	}

	Real b = std::max(this->min, this->max);
	Real fb = function->f(b);

	if ((this->testingFunction) && (std::fabs(fb) <= this->tolerance))
	{
		this->root = b;

		return;
	}

	Real fval = 0.0;

	Real val = this->recurse(a, b, fa, fb, 0, this->ok, fval);

	if (std::fabs(fb) < std::fabs(fa))
	{
		fa = fb;

		a = b;
	}

	if (this->ok)
	{
		if (std::fabs(fa) < std::fabs(fb))
		{
		    this->root = a;

			return;
		}

		this->root = val;

		return;
	}

	this->root = a;
}

Real Numerical::ClosestBisectionEquationRootFinder::recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval)
{
    ok = true;

	Real m = this->next(a, b, fa, fb);

	if (numSteps == this->maxSteps)
	{
		ok = false;

		fval = 0.0;

		this->numSteps = this->maxSteps;

		return m;
	}

	Real fm = this->function->f(m);

	if ((this->testingStepSize) && ((b - a) <= this->tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	if ((this->testingFunction) && (std::fabs(fm) <= tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if ((std::fabs(fa) <= std::fabs(fb) + this->tolerance) &&
        ((this->ignoringSigns) || (fa*fm < 0.0) || (fm*fb >= 0.0)))
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if ((std::fabs(fa) + this->tolerance >= std::fabs(fb)) &&
        ((this->ignoringSigns) || (fa*fm >= 0.0) || (fm*fb < 0.0)))
	{
		tested2 = true;

		val2 = this->recurse(m, b, fm, fb, numSteps + 1, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - m) < tolerance) /*&&
        ((this->ignoringSigns) || (fa*fm < 0.0) || (fm*fb >= 0.0))*/)
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - m) < tolerance) /*&&
        ((this->ignoringSigns) || (fa*fm >= 0.0) || (fm*fb < 0.0))*/)
	{
		tested2 = true;

		val2 = this->recurse(m, b, fm, fb, numSteps + 1, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (std::fabs(fval2) < std::fabs(fval1))) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (std::fabs(fm) < std::fabs(fval1))
		{
			fval = fm;

			return m;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fm;

	return m;
}

/*Real Numerical::MaxBisectionEquationRootFinder::recursiveExecute(IntervalEquationRootFunction *function,
	Real min, Real max, Real fmin, Real fmax,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval)
{
	ok = true;

	Real mid = this->next(function, min, max, fmin, fmax);

	Real fmid = function->f(mid);

	if ((testFunction) && (std::fabs(fmax - fmin) <= 2.0*tolerance) &&
		(std::fabs(fmax - fmid) <= tolerance) && (std::fabs(fmin - fmid) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	if (maxSteps == 0)
	{
		ok = false;

		fval = fmid;

		return mid;
	}

	if ((testStepSize) && ((max - min) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if (fmin + tolerance >= fmax)
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if (fmin <= fmax + tolerance)
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - mid) < tolerance))
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - mid) < tolerance))
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (fval2 > fval1)) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (fmid > fval1)
		{
			fval = fmid;

			return mid;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fmid;

	return mid;
}

Real Numerical::MaxBisectionEquationRootFinder::execute(IntervalEquationRootFunction *function,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok)
{
	ok = true;

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	Real a = function->min();
	Real b = function->max();

	Real min = std::min(a, b);
	Real fmin = function->f(min);

	Real max = std::max(a, b);
	Real fmax = function->f(max);

	Real fval = 0.0;

	Real val = this->recursiveExecute(function, min, max, fmin, fmax, maxSteps, tolerance, testFunction, testStepSize, ok, fval);

	if (fmin > fmax)
	{
		fmax = fmin;

		max = min;
	}

	if (ok)
	{
		if (fmax > fval)
		{
			return max;
		}

		return val;
	}

	return max;
}*/

Numerical::MaxBisectionEquationRootFinder::MaxBisectionEquationRootFinder() :
    Numerical::BisectionEquationRootFinder::BisectionEquationRootFinder()
{

}

Numerical::MaxBisectionEquationRootFinder::~MaxBisectionEquationRootFinder()
{

}

void Numerical::MaxBisectionEquationRootFinder::execute()
{
    this->ok = true;

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	Real a = std::min(this->min, this->max);
	Real fa = function->f(a);

	Real b = std::max(this->min, this->max);
	Real fb = function->f(b);

	Real fval = 0.0;

	Real val = this->recurse(a, b, fa, fb, 0, this->ok, fval);

	if (fa > fb)
	{
		fb = fa;

		b = a;
	}

	if (this->ok)
	{
		if (fb > fval)
		{
		    this->root = b;

			return;
		}

		this->root = val;

		return;
	}

	this->root = b;
}

Real Numerical::MaxBisectionEquationRootFinder::recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval)
{
    ok = true;

	Real m = this->next(a, b, fa, fb);

	Real fm = function->f(m);

	if ((this->testingFunction) && (std::fabs(fb - fa) <= 2.0*this->tolerance) &&
		(std::fabs(fb - fm) <= this->tolerance) && (std::fabs(fa - fm) <= this->tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	if (numSteps == this->maxSteps)
	{
		ok = false;

		fval = fm;

		this->numSteps = this->maxSteps;

		return m;
	}

	if ((this->testingStepSize) && ((b - a) <= tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if (fa + this->tolerance >= fb)
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if (fa <= fb + this->tolerance)
	{
		tested2 = true;

		val2 = this->recurse(m, b, fb, fb, numSteps + 1, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - m) < this->tolerance))
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - m) < tolerance))
	{
		tested2 = true;

		val2 = this->recurse(m, b, fm, fb, numSteps + 1, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (fval2 > fval1)) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (fm > fval1)
		{
			fval = fm;

			return m;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fm;

	return m;
}

/*Real Numerical::MinBisectionEquationRootFinder::recursiveExecute(IntervalEquationRootFunction *function,
	Real min, Real max, Real fmin, Real fmax,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok, Real &fval)
{
	ok = true;

	Real mid = this->next(function, min, max, fmin, fmax);

	Real fmid = function->f(mid);

	if ((testFunction) && (std::fabs(fmax - fmin) <= tolerance) &&
		(std::fabs(fmax - fmid) <= tolerance) && (std::fabs(fmin - fmid) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	if (maxSteps == 0)
	{
		ok = false;

		fval = fmid;

		return mid;
	}

	if ((testStepSize) && ((max - min) <= tolerance))
	{
		fval = fmid;

		return mid;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if (fmin <= fmax + tolerance)
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if (fmin + tolerance >= fmax)
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - mid) < tolerance))
	{
		tested1 = true;

		val1 = this->recursiveExecute(function, min, mid, fmin, fmid, maxSteps - 1, tolerance, testFunction, testStepSize, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - mid) < tolerance))
	{
		tested2 = true;

		val2 = this->recursiveExecute(function, mid, max, fmid, fmax, maxSteps - 1, tolerance, testFunction, testStepSize, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (fval2 < fval1)) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (fmid < fval1)
		{
			fval = fmid;

			return mid;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fmid;

	return mid;
}

Real Numerical::MinBisectionEquationRootFinder::execute(IntervalEquationRootFunction *function,
	UInt maxSteps, Real tolerance, bool testFunction, bool testStepSize, bool &ok)
{
	ok = true;

	if ((!testFunction) && (!testStepSize))
	{
		testFunction = testStepSize = true;
	}

	Real a = function->min();
	Real b = function->max();

	Real min = std::min(a, b);
	Real fmin = function->f(min);

	Real max = std::max(a, b);
	Real fmax = function->f(max);

	Real fval = 0.0;

	Real val = this->recursiveExecute(function, min, max, fmin, fmax, maxSteps, tolerance, testFunction, testStepSize, ok, fval);

	if (fmax < fmin)
	{
		fmin = fmax;

		min = max;
	}

	if (ok)
	{
		if (fmin < fval)
		{
			return min;
		}

		return val;
	}

	return min;
}*/

Numerical::MinBisectionEquationRootFinder::MinBisectionEquationRootFinder() :
    Numerical::BisectionEquationRootFinder::BisectionEquationRootFinder()
{

}

Numerical::MinBisectionEquationRootFinder::~MinBisectionEquationRootFinder()
{

}

void Numerical::MinBisectionEquationRootFinder::execute()
{
    this->ok = true;

	if ((!this->testingFunction) && (!this->testingStepSize))
	{
		this->testingFunction = this->testingStepSize = true;
	}

	Real a = std::min(this->min, this->max);
	Real fa = function->f(a);

	Real b = std::max(this->min, this->max);
	Real fb = function->f(b);

	Real fval = 0.0;

	Real val = this->recurse(a, b, fa, fb, 0, this->ok, fval);

	if (fb < fa)
	{
		fa = fb;

		a = b;
	}

	if (this->ok)
	{
		if (fa < fval)
		{
		    this->root = min;

			return;
		}

		this->root = val;

		return;
	}

	this->root = min;
}

Real Numerical::MinBisectionEquationRootFinder::recurse(Real a, Real b, Real fa, Real fb, UInt numSteps, bool &ok, Real &fval)
{
    ok = true;

	Real m = this->next(a, b, fa, fb);

	Real fm = function->f(m);

	if ((this->testingFunction) && (std::fabs(fb - fa) <= tolerance) &&
		(std::fabs(fb - fm) <= tolerance) && (std::fabs(fa - fm) <= tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	if (numSteps == this->maxSteps)
	{
		ok = false;

		fval = fm;

		this->numSteps = this->maxSteps;

		return m;
	}

	if ((this->testingStepSize) && ((b - a) <= tolerance))
	{
		fval = fm;

		this->numSteps = numSteps;

		return m;
	}

	bool tested1 = false, ok1 = false;
	Real val1 = 0.0, fval1 = 0.0;

	if (fa <= fb + tolerance)
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	bool tested2 = false, ok2 = false;
	Real val2 = 0.0, fval2 = 0.0;

	if (fa + tolerance >= fb)
	{
		tested2 = true;

		val2 = this->recurse(m, b, fm, fb, numSteps + 1, ok2, fval2);
	}

	if ((!tested1) && (std::fabs(val2 - m) < tolerance))
	{
		tested1 = true;

		val1 = this->recurse(a, m, fa, fm, numSteps + 1, ok1, fval1);
	}

	if ((!tested2) && (std::fabs(val1 - m) < tolerance))
	{
		tested2 = true;

		val2 = this->recurse(m, b, fm, fb, numSteps + 1, ok2, fval2);
	}

	if (ok2)
	{
		if (((ok1) && (fval2 < fval1)) ||
			(!ok1))
		{
			fval1 = fval2;

			val1 = val2;

			ok1 = true;
		}
	}

	if (ok1)
	{
		if (fm < fval1)
		{
			fval = fm;

			return m;
		}

		fval = fval1;

		return val1;
	}

	ok = false;

	fval = fm;

	return m;
}

#endif //#if USE_NUMERICAL
