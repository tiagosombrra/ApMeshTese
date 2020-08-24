#include "Numerical/GaussLegendreIntegralCalculator.h"

#if USE_NUMERICAL

#include "Numerical/Function.h"
#include "Numerical/InterpolationFunction.h"
#include "Numerical/OrderedPoints.h"

using namespace Data;
using namespace Numerical;

const Real Numerical::GaussLegendreIntegralCalculator::alphas[8][8] =
{
	{0.0,            0.0,           0.0,           0.0,          0.0,          0.0,          0.0,          0.0,        },
	{-0.577350269,   0.577350269,   0.0,           0.0,          0.0,          0.0,          0.0,          0.0         },
	{-0.774596669,   0.0,           0.774596669,   0.0,          0.0,          0.0,          0.0,          0.0         },
	{-0.861136312,  -0.339981043,   0.339981043,   0.861136312,  0.0,          0.0,          0.0,          0.0         },
	{-0.9061798459, -0.5384693101,  0.0,           0.5384693101, 0.9061798459, 0.0,          0.0,          0.0         },
	{-0.9324695142, -0.6612093864, -0.2386191860,  0.2386191860, 0.6612093864, 0.9324695142, 0.0,          0.0         },
	{-0.9491079123, -0.7415311855, -0.4058451513,  0.0,          0.4058451513, 0.7415311855, 0.9491079123, 0.0         },
	{-0.9602898564, -0.7966664774, -0.5255324099, -0.1834346424, 0.1834346424, 0.5255324099, 0.7966664774, 0.9602898564}
};

const Real Numerical::GaussLegendreIntegralCalculator::weights[8][8] =
{
	{2.0,          0.0,          0.0,          0.0,          0.0,          0.0,          0.0,          0.0         },
	{1.0,          1.0,          0.0,          0.0,          0.0,          0.0,          0.0,          0.0         },
	{0.555555556,  0.888888889,  0.555555556,  0.0,          0.0,          0.0,          0.0,          0.0         },
	{0.347854845,  0.652145155,  0.652145155,  0.347854845,  0.0,          0.0,          0.0,          0.0         },
	{0.2369268850, 0.4786286704, 0.5688888888, 0.4786286704, 0.2369268850, 0.0,          0.0,          0.0         },
	{0.1713244923, 0.3607615730, 0.4679139345, 0.4679139345, 0.3607615730, 0.1713244923, 0.0,          0.0         },
	{0.1294849661, 0.2797053914, 0.3818300505, 0.4179591836, 0.3818300505, 0.2797053914, 0.1294849661, 0.0         },
	{0.1012285362, 0.2223810344, 0.3137066458, 0.3626837833, 0.3626837833, 0.3137066458, 0.2223810344, 0.1012285362}
};

/*Real Numerical::GaussLegendreIntegralCalculator::execute(IntegralFunction *function, UInt points)
{
	if (points < 1)
	{
		points = 1;
	}

	if (points > 8)
	{
		points = 8;
	}

	Real total = 0.0;

	Real a = function->min();
	Real b = function->max();
	Real halfAB = (a + b)*0.5;
	Real halfDist = (b - a)*0.5;

	for (UInt i = 0; i < points; i++)
	{
		Real alpha = this->alphas[points-1][i];
		Real weight = this->weights[points-1][i];

		Real x = halfAB + halfDist * alpha;

		Real f = function->f(x) * weight;

		total += f;
	}

	total *= halfDist;

	return total;
}

Real Numerical::GaussLegendreIntegralCalculator::execute(IntegralFunction *function)
{
	return execute(function, 4);
}*/

Numerical::GaussLegendreIntegralCalculator::GaussLegendreIntegralCalculator() :
    Numerical::FunctionBasedIntegralCalculator::FunctionBasedIntegralCalculator()
{
    this->numPoints = 4;
}

Numerical::GaussLegendreIntegralCalculator::~GaussLegendreIntegralCalculator()
{

}

void Numerical::GaussLegendreIntegralCalculator::setNumPoints(UInt numPoints)
{
    this->numPoints = numPoints;
}

UInt Numerical::GaussLegendreIntegralCalculator::getNumPoints() const
{
    return this->numPoints;
}

void Numerical::GaussLegendreIntegralCalculator::execute()
{
    if (this->numPoints < 1)
	{
		this->numPoints = 1;
	}

	if (this->numPoints > 8)
	{
		this->numPoints = 8;
	}

	Real total = 0.0;

	Real a = this->min;
	Real b = this->max;
	Real halfAB = (a + b)*0.5;
	Real halfDist = (b - a)*0.5;

	for (UInt i = 0; i < this->numPoints; i++)
	{
		Real alpha = this->alphas[this->numPoints-1][i];
		Real weight = this->weights[this->numPoints-1][i];

		Real x = halfAB + halfDist * alpha;

		Real f = function->f(x) * weight;

		total += f;
	}

	total *= halfDist;

	this->integral = total;
}

#endif //#if USE_NUMERICAL
