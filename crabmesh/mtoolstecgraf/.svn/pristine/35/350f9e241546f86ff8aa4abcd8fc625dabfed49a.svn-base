#include "Numerical/ChebyshevPoints.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::ChebyshevPoints::ChebyshevPoints()
{

}

Numerical::ChebyshevPoints::~ChebyshevPoints()
{

}

Real Numerical::ChebyshevPoints::point(UInt i, UInt degree, Real min, Real max)
{
    Real ang = M_PI*(2.0*i + 1.0)/(2.0*degree + 2.0);

    ang = (ang < 0) ? 0 : (ang > 2.0*M_PI) ? 2.0*M_PI : ang;

    Real pt = std::cos(ang);

    return (min + max)*0.5 + (max - min)*0.5*pt;
}

#endif //#if USE_NUMERICAL
