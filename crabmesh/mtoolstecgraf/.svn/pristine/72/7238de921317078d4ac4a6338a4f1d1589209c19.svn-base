#include "Numerical/FalsePositionEquationRootFinder.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::FalsePositionEquationRootFinder::FalsePositionEquationRootFinder() :
    Numerical::BisectionEquationRootFinder::BisectionEquationRootFinder()
{
    this->weightDecreaseCount = 0;
    this->countA = this->countB = 0;
}

Numerical::FalsePositionEquationRootFinder::~FalsePositionEquationRootFinder()
{

}

void Numerical::FalsePositionEquationRootFinder::setWeightDecreaseCount(UInt weightDecreaseCount)
{
    this->weightDecreaseCount = weightDecreaseCount;
}

UInt Numerical::FalsePositionEquationRootFinder::getWeightDecreaseCount() const
{
    return this->weightDecreaseCount;
}

void Numerical::FalsePositionEquationRootFinder::execute()
{
    this->countA = this->countB = 0;

    this->Numerical::BisectionEquationRootFinder::execute();
}

Real Numerical::FalsePositionEquationRootFinder::next(Real a, Real b, Real fa, Real fb) const
{
    //return (min*std::fabs(fmax) + max*std::fabs(fmin))/(std::fabs(fmax) + std::fabs(fmin));
    return (a*std::fabs(fb) + b*std::fabs(fa))/(std::fabs(fb) + std::fabs(fa));
}

void Numerical::FalsePositionEquationRootFinder::narrowDownRange(Real &a, Real &b, Real &fa, Real &fb, Real &m, Real &fm) const
{
    if (fa*fm < 0.0)
    {
        b = m;
        fb = fm;

        if (this->weightDecreaseCount > 0)
        {
            this->countA++;
            this->countB = 0;

            if (this->countA == this->weightDecreaseCount)
            {
                fa /= 2;

                this->countA = 0;
            }
        }
    }
    else
    {
        a = m;
        fa = fm;

        if (this->weightDecreaseCount > 0)
        {
            this->countA = 0;
            this->countB++;

            if (this->countB == this->weightDecreaseCount)
            {
                fb /= 2;

                this->countB = 0;
            }
        }
    }
}

Numerical::ClosestFalsePositionEquationRootFinder::ClosestFalsePositionEquationRootFinder() :
    Numerical::ClosestBisectionEquationRootFinder::ClosestBisectionEquationRootFinder(),
    Numerical::FalsePositionEquationRootFinder::FalsePositionEquationRootFinder()
{

}

Numerical::ClosestFalsePositionEquationRootFinder::~ClosestFalsePositionEquationRootFinder()
{

}

void Numerical::ClosestFalsePositionEquationRootFinder::execute()
{
    this->Numerical::ClosestBisectionEquationRootFinder::execute();
}

Numerical::MaxFalsePositionEquationRootFinder::MaxFalsePositionEquationRootFinder() :
    Numerical::MaxBisectionEquationRootFinder::MaxBisectionEquationRootFinder(),
    Numerical::FalsePositionEquationRootFinder::FalsePositionEquationRootFinder()
{

}

Numerical::MaxFalsePositionEquationRootFinder::~MaxFalsePositionEquationRootFinder()
{

}

void Numerical::MaxFalsePositionEquationRootFinder::execute()
{
    this->Numerical::MaxBisectionEquationRootFinder::execute();
}

Numerical::MinFalsePositionEquationRootFinder::MinFalsePositionEquationRootFinder() :
    Numerical::MinBisectionEquationRootFinder::MinBisectionEquationRootFinder(),
    Numerical::FalsePositionEquationRootFinder::FalsePositionEquationRootFinder()
{

}

Numerical::MinFalsePositionEquationRootFinder::~MinFalsePositionEquationRootFinder()
{

}

void Numerical::MinFalsePositionEquationRootFinder::execute()
{
    this->Numerical::MinBisectionEquationRootFinder::execute();
}

#endif //#if USE_NUMERICAL
