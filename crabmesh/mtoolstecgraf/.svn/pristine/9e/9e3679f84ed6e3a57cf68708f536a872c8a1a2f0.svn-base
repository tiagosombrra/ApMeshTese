#include "MJMesh/HotellingTransformer.h"

#if USE_NUMERICAL

#include "Data/Vector2D.h"
#include "Numerical/TransformationMatrix.h"

#if USE_ATTRIBS
#include "MJMesh/Boundary.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Edge3D.h"
#include "MJMesh/Triangle3D.h"
#endif //#if USE_ATTRIBS

MJMesh::HotellingTransformer::HotellingTransformer() :
    Performer::HotellingTransformer::HotellingTransformer()
{

}

MJMesh::HotellingTransformer::~HotellingTransformer()
{

}

void MJMesh::HotellingTransformer::executePrincipalComponentsBasisFinder()
{
    this->Performer::HotellingTransformer::executePrincipalComponentsBasisFinder();

    UInt dim = this->getBoundary()->dimension();

    Numerical::BasisChangeMatrix *matrix = this->getMatrix();

    Real x[3], y[3];

    matrix->getX(x);
    matrix->getY(y);

    if (dim == 2)
    {
        Data::Vector2D v1(x[0], x[1]);
        Data::Vector2D v2(y[0], y[1]);

        Real cross = v1.cross(v2);

        if (cross < 0)
        {
            x[0] = -x[0];
            x[1] = -x[1];

            matrix->setX(x);
        }
    }
    else
    {
        Real z[3];

        matrix->getZ(z);

        Data::Vector3D v1(x[0], x[1], x[2]);
        Data::Vector3D v2(y[0], y[1], y[2]);
        Data::Vector3D v3(z[0], z[1], z[2]);

        Data::Vector3D v = v1.cross(v2);

        if (v.dot(v3) < 0)
        {
            x[0] = -x[0];
            x[1] = -x[1];
            x[2] = -x[2];

            matrix->setX(x);
        }
    }
}

void MJMesh::HotellingTransformer::execute()
{
    this->Performer::HotellingTransformer::execute();

#if USE_GUI
    this->getBoundary()->clearLists();
#endif //#if USE_GUI

#if USE_ATTRIBS
    if ((!this->getMid()) || (!this->getEigenvalues()) || (!this->getEigenvectors()) || (!this->getMatrix()))
    {
        return;
    }

    UInt dim = this->getBoundary()->dimension();
    bool surface = static_cast<const MJMesh::Boundary *>(this->getBoundary())->isSurfaceBoundary();

    for (Data::Boundary::ConstFrontElementIterator iter = this->getBoundary()->eBegin();
         iter != this->getBoundary()->eEnd(); iter++)
    {
        if (dim == 2)
        {
            static_cast<MJMesh::Edge2D *>((*iter)->getShape())->clearAttribs();
        }
        else if (surface)
        {
            static_cast<MJMesh::Edge3D *>((*iter)->getShape())->clearAttribs();
        }
        else
        {
            static_cast<MJMesh::Triangle3D *>((*iter)->getShape())->clearAttribs();
        }
    }
#endif //#if USE_ATTRIBS
}

void MJMesh::HotellingTransformer::executeBack()
{
    this->Performer::HotellingTransformer::executeBack();

#if USE_ATTRIBS
    if ((!this->getMid()) || (!this->getEigenvalues()) || (!this->getEigenvectors()) || (!this->getMatrix()))
    {
        return;
    }

    if (this->mesh)
    {
        UInt dim = this->mesh->dimension();
        bool surface = static_cast<const MJMesh::Mesh *>(this->mesh)->isSurface();

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++)
        {
            if (dim == 2)
            {
                static_cast<MJMesh::Edge2D *>((*iter)->getShape())->clearAttribs();
            }
            else if (surface)
            {
                static_cast<MJMesh::Edge3D *>((*iter)->getShape())->clearAttribs();
            }
            else
            {
                static_cast<MJMesh::Triangle3D *>((*iter)->getShape())->clearAttribs();
            }
        }
    }
    else
    {
        UInt dim = this->getBoundary()->dimension();
        bool surface = static_cast<const MJMesh::Boundary *>(this->getBoundary())->isSurfaceBoundary();

        for (Data::Boundary::ConstFrontElementIterator iter = this->getBoundary()->eBegin();
             iter != this->getBoundary()->eEnd(); iter++)
        {
            if (dim == 2)
            {
                static_cast<MJMesh::Edge2D *>((*iter)->getShape())->clearAttribs();
            }
            else if (surface)
            {
                static_cast<MJMesh::Edge3D *>((*iter)->getShape())->clearAttribs();
            }
            else
            {
                static_cast<MJMesh::Triangle3D *>((*iter)->getShape())->clearAttribs();
            }
        }
    }
#endif //#if USE_ATTRIBS
}

#endif //#if USE_NUMERICAL
