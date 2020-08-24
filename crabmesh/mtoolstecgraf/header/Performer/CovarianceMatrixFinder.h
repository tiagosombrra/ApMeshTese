#ifndef _PERFORMER_COVARIANCE_MATRIX_FINDER_H_
#define _PERFORMER_COVARIANCE_MATRIX_FINDER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Performer
{
    class CovarianceMatrixFinder : public Performer::Builder
    {
    public:

        enum Mode
        {
            NORMAL = 0,
            ACCUMULATE
#if USE_EXACT
            , PSEUDO_EXACT
#endif //#if USE_EXACT
        };

    public:

        CovarianceMatrixFinder();
        virtual ~CovarianceMatrixFinder();

        virtual void setBoundary(const Data::Boundary *boundary);
        virtual const Data::Boundary *getBoundary() const;

        virtual void setMode(enum Mode mode);
        virtual enum Mode getMode() const;

        virtual void setFrontElementCentroids(bool frontElementCentroids);
        virtual bool isFrontElementCentroids() const;

        virtual void setWeighted(bool weighted);
        virtual bool isWeighted() const;

        virtual Numerical::SymmetricMatrix *getMatrix() const;
        virtual void getMid(Real mid[3]) const;

        virtual void execute();

    protected:

        Numerical::SymmetricMatrix *makeMatrix(UInt dimension) const;

        virtual Real weight(const Data::Vertex *v) const;
        virtual Real weight(const Data::FrontElement *fel) const;

        virtual void centroid(const Data::FrontElement *fel, UInt dimension, Real pt[]) const;

        virtual void average(UInt dimension, Real average[], const Real pt[], Real weight, Real n = 0) const;
        virtual void endAverage(UInt dimension, Real average[], Real size) const;

        virtual void covariance(UInt dimension, Real average[], const Real pt[], Real weight, Real n = 0) const;
        virtual void endCovariance(UInt dimension, Real size) const;
#if USE_EXACT
        virtual Data::EReal eweight(const Data::Vertex *v) const;
        virtual Data::EReal eweight(const Data::FrontElement *fel) const;

        virtual void centroid(const Data::FrontElement *fel, UInt dimension, Data::EReal pt[]) const;

        virtual void average(UInt dimension, Data::EReal average[], const Real pt[], const Data::EReal &weight) const;
        virtual void average(UInt dimension, Data::EReal average[], const Data::EReal pt[], const Data::EReal &weight) const;
        virtual void endAverage(UInt dimension, Data::EReal average[], const Data::EReal &size) const;

        virtual void covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Real pt[], const Data::EReal &weight) const;
        virtual void covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Data::EReal pt[], const Data::EReal &weight) const;
        virtual void endCovariance(UInt dimension, Data::EReal matrix[], const Data::EReal &size) const;
#endif //#if USE_EXACT

    protected:

        const Data::Boundary *boundary;

        enum Mode mode;

        bool frontElementCentroids;
        bool weighted;

        Real mid[3];

        Numerical::SymmetricMatrix *matrix;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _PERFORMER_COVARIANCE_MATRIX_FINDER_H_
