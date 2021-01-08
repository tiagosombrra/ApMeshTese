#ifndef _NUMERICAL_INTERPOLATOR_H_
#define _NUMERICAL_INTERPOLATOR_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Performer/Builder.h"

namespace Numerical
{
    class Interpolator : public Performer::Builder
    {
    public:

        Interpolator();
        virtual ~Interpolator();

        virtual void setPoints(const OrderedPoints *points);
        virtual const OrderedPoints *getPoints() const;

        virtual void setDegree(UInt degree);
        virtual UInt getDegree() const;

        virtual void setX(Real x);
        virtual Real getX() const;

        virtual Real getFx() const;

        /*virtual Real execute(const OrderedPoints *points, Real x);
        virtual Real execute(const OrderedPoints *points, UInt degree, Real x) const;
        virtual Real execute(const OrderedPoints *points, UInt degree, UInt start, Real x) const = 0;

        virtual Real executeDerivative(const OrderedPoints *points, Real x);
        virtual Real executeDerivative(const OrderedPoints *points, UInt degree, Real x) const;
        virtual Real executeDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

        virtual Real execute2ndDerivative(const OrderedPoints *points, Real x);
        virtual Real execute2ndDerivative(const OrderedPoints *points, UInt degree, Real x) const;
        virtual Real execute2ndDerivative(const OrderedPoints *points, UInt degree, UInt start, Real x) const;

    public:

        virtual bool findStart(const OrderedPoints *points, UInt &degree, Real x, UInt &start) const;*/

        virtual void execute();
        virtual void executeDerivative();
        virtual void execute2ndDerivative();
        virtual void executeNthDerivative(UInt derivative);

        virtual bool findStart();

    public:

        virtual Real dExecute(UInt derivative) = 0;

    protected:

        const OrderedPoints *points;

        UInt degree;
        bool degreeSet;

        UInt start;

        Real x;

        Real fx;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_INTERPOLATOR_H_
