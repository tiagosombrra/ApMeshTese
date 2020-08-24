#ifndef _PERFORMER_PRINCIPAL_COMPONENTS_BASIS_FINDER_H_
#define _PERFORMER_PRINCIPAL_COMPONENTS_BASIS_FINDER_H_

#include "Performer/Definitions.h"
#include "Performer/PrincipalComponentsAnalyzer.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Performer
{
    class PrincipalComponentsBasisFinder : public Performer::Builder
    {
    public:

        PrincipalComponentsBasisFinder();
        virtual ~PrincipalComponentsBasisFinder();

        virtual Numerical::BasisChangeMatrix *getMatrix() const;

        virtual void setBoundary(const Data::Boundary *boundary);
        virtual const Data::Boundary *getBoundary() const;

        virtual void setMode(enum Performer::CovarianceMatrixFinder::Mode mode);
        virtual enum Performer::CovarianceMatrixFinder::Mode getMode() const;

        virtual void setFrontElementCentroids(bool frontElementCentroids);
        virtual bool isFrontElementCentroids() const;

        virtual void setWeighted(bool weighted);
        virtual bool isWeighted() const;

        virtual Numerical::RowMatrix *getEigenvalues() const;
        virtual Numerical::DenseMatrix *getEigenvectors() const;
        virtual Numerical::ColumnMatrix *getMid() const;

        virtual void execute();

    protected:

        Numerical::BasisChangeMatrix *matrix;

        Performer::PrincipalComponentsAnalyzer pca;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _PERFORMER_PRINCIPAL_COMPONENTS_BASIS_FINDER_H_
