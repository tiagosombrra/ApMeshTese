#include "Performer/HotellingTransformer.h"

#if USE_NUMERICAL

#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Numerical/TransformationMatrix.h"

Performer::HotellingTransformer::HotellingTransformer() :
    Performer::PrincipalComponentsBasisFinder::Builder()
{
    this->mesh = NULL;
}

Performer::HotellingTransformer::~HotellingTransformer()
{
    delete this->pcbf.getMatrix();
}

void Performer::HotellingTransformer::setMesh(Data::Mesh *mesh)
{
    this->mesh = mesh;
}

Data::Mesh *Performer::HotellingTransformer::getMesh() const
{
    return this->mesh;
}

Numerical::BasisChangeMatrix *Performer::HotellingTransformer::getMatrix() const
{
    return this->pcbf.getMatrix();
}

void Performer::HotellingTransformer::setBoundary(const Data::Boundary *boundary)
{
    this->pcbf.setBoundary(boundary);
}

const Data::Boundary *Performer::HotellingTransformer::getBoundary() const
{
    return this->pcbf.getBoundary();
}

void Performer::HotellingTransformer::setMode(enum Performer::CovarianceMatrixFinder::Mode mode)
{
    this->pcbf.setMode(mode);
}

enum Performer::CovarianceMatrixFinder::Mode Performer::HotellingTransformer::getMode() const
{
    return this->pcbf.getMode();
}

void Performer::HotellingTransformer::setFrontElementCentroids(bool frontElementCentroids)
{
    this->pcbf.setFrontElementCentroids(frontElementCentroids);
}

bool Performer::HotellingTransformer::isFrontElementCentroids() const
{
    return this->pcbf.isFrontElementCentroids();
}

void Performer::HotellingTransformer::setWeighted(bool weighted)
{
    this->pcbf.setWeighted(weighted);
}

bool Performer::HotellingTransformer::isWeighted() const
{
    return this->pcbf.isWeighted();
}

Numerical::RowMatrix *Performer::HotellingTransformer::getEigenvalues() const
{
    return this->pcbf.getEigenvalues();
}

Numerical::DenseMatrix *Performer::HotellingTransformer::getEigenvectors() const
{
    return this->pcbf.getEigenvectors();
}

Numerical::ColumnMatrix *Performer::HotellingTransformer::getMid() const
{
    return this->pcbf.getMid();
}

void Performer::HotellingTransformer::executePrincipalComponentsBasisFinder()
{
    this->pcbf.execute();
}

void Performer::HotellingTransformer::executeTransform()
{
    Numerical::BasisChangeMatrix *matrix = this->getMatrix();

    if ((!this->getMid()) || (!this->getEigenvalues()) || (!this->getEigenvectors()) || (!matrix))
    {
        return;
    }

    UInt dim = this->getBoundary()->dimension();

#if USE_GUI
    this->getBoundary()->freeze();
#endif //#if USE_GUI

    for (Data::Boundary::ConstVertexIterator iter = this->getBoundary()->vBegin();
         iter != this->getBoundary()->vEnd(); iter++)
    {
        Data::Point *p = (*iter)->getPoint();

        if (dim == 2)
        {
            matrix->applyBackTo(static_cast<Data::Point2D *>(p));
        }
        else
        {
            matrix->applyBackTo(static_cast<Data::Point3D *>(p));
        }
    }

#if USE_GUI
    this->getBoundary()->unfreeze();
#endif //#if USE_GUI
}

void Performer::HotellingTransformer::execute()
{
    this->executePrincipalComponentsBasisFinder();

    this->executeTransform();
}

void Performer::HotellingTransformer::executeBack()
{
    Numerical::BasisChangeMatrix *matrix = this->getMatrix();

    if ((!this->getMid()) || (!this->getEigenvalues()) || (!this->getEigenvectors()) || (!matrix))
    {
        return;
    }

    if (this->mesh)
    {
#if USE_GUI
        this->mesh->freeze();
#endif //#if USE_GUI

        UInt dim = this->mesh->dimension();

        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
             iter != this->mesh->vEnd(); iter++)
        {
            Data::Point *p = (*iter)->getPoint();

            if (dim == 2)
            {
                matrix->applyTo(static_cast<Data::Point2D *>(p));
            }
            else
            {
                matrix->applyTo(static_cast<Data::Point3D *>(p));
            }
        }

#if USE_GUI
        this->mesh->unfreeze();
#endif //#if USE_GUI
    }
    else
    {
#if USE_GUI
        this->getBoundary()->freeze();
#endif //#if USE_GUI

        UInt dim = this->getBoundary()->dimension();

        for (Data::Boundary::ConstVertexIterator iter = this->getBoundary()->vBegin();
             iter != this->getBoundary()->vEnd(); iter++)
        {
            Data::Point *p = (*iter)->getPoint();

            if (dim == 2)
            {
                matrix->applyTo(static_cast<Data::Point2D *>(p));
            }
            else
            {
                matrix->applyTo(static_cast<Data::Point3D *>(p));
            }
        }

#if USE_GUI
        this->getBoundary()->unfreeze();
#endif //#if USE_GUI
    }
}

#endif //#if USE_NUMERICAL

