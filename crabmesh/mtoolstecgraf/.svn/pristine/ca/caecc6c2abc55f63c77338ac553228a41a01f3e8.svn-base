#ifndef _PERFORMER_HOTELLING_TRANSFORMER_H_
#define _PERFORMER_HOTELLING_TRANSFORMER_H_

#include "Performer/Definitions.h"
#include "Performer/PrincipalComponentsBasisFinder.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

namespace Performer
{
    class HotellingTransformer : public Performer::Builder
    {
    public:

        HotellingTransformer();
        virtual ~HotellingTransformer();

        virtual void setMesh(Data::Mesh *mesh);
        virtual Data::Mesh *getMesh() const;

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
        virtual void executeBack();

    protected:

        virtual void executePrincipalComponentsBasisFinder();
        virtual void executeTransform();

    protected:

        Data::Mesh *mesh;

        PrincipalComponentsBasisFinder pcbf;

    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _PERFORMER_HOTELLING_TRANSFORMER_H_
