#ifndef _NUMERICAL_ORDERED_POINTS_H_
#define _NUMERICAL_ORDERED_POINTS_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Numerical
{
    class OrderedPoints
    {
    public:

        OrderedPoints();
        virtual ~OrderedPoints();

        virtual void setNumPoints(UInt numPoints);
        virtual UInt getNumPoints() const;

        virtual void setPoint(UInt i, Real x, Real y);

        virtual Real x(UInt i) const;
        virtual Real y(UInt i) const;

        virtual UInt search(Real x) const;
        virtual UInt search(Real x, UInt min, UInt max) const;

        virtual bool isEquallySpaced() const;
        virtual bool isEquallySpaced(Real tolerance) const;

    protected:

        Real *points[2];
        UInt numPoints;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_ORDERED_POINTS_H_
