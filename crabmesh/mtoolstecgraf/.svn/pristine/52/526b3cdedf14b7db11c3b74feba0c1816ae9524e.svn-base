#include "PMJMesh/HotellingTransformer.h"

#if USE_NUMERICAL

#include "Data/Vector2D.h"
#include "Numerical/TransformationMatrix.h"

#if USE_ATTRIBS
#include "MJMesh/Boundary.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Edge3D.h"
#include "MJMesh/Triangle3D.h"
#endif //#if USE_ATTRIBS

PMJMesh::HotellingTransformer::HotellingTransformer() :
    MJMesh::HotellingTransformer::HotellingTransformer()
{

}

PMJMesh::HotellingTransformer::~HotellingTransformer()
{

}

void PMJMesh::HotellingTransformer::addMesh(Data::Mesh *mesh)
{
    this->meshes.push_back(mesh);
}

void PMJMesh::HotellingTransformer::executeBackOnMeshes()
{
    Numerical::BasisChangeMatrix *matrix = this->getMatrix();

    if ((!this->getMid()) || (!this->getEigenvalues()) || (!this->getEigenvectors()) || (!matrix))
    {
        return;
    }

    UInt dim = (this->mesh) ? this->mesh->dimension() : this->getBoundary()->dimension();

    for (Data::MeshList::iterator iter = this->meshes.begin();
         iter != this->meshes.end(); iter++)
    {
        Data::Mesh *mesh = (*iter);

#if USE_GUI
        mesh->freeze();
#endif //#if USE_GUI

        for (Data::Mesh::VertexIterator iter2 = mesh->vBegin();
             iter2 != mesh->vEnd(); iter2++)
        {
            Data::Vertex *v = (*iter2);

            bool found = false;

            if (this->mesh)
            {
                if (this->mesh->have(v))
                {
                    found = true;
                }
            }
            else if (this->getBoundary())
            {
                if (this->getBoundary()->have(v))
                {
                    found = true;
                }
            }

            if (found)
            {
                continue;
            }

            for (Data::MeshList::iterator iter3 = this->meshes.begin();
                 iter3 != iter; iter3++)
            {
                if ((*iter3)->have(v))
                {
                    found = true;

                    break;
                }
            }

            if (found)
            {
                continue;
            }

            Data::Point *p = v->getPoint();

            if (dim == 2)
            {
                matrix->applyTo(static_cast<Data::Point2D *>(p));
            }
            else
            {
                matrix->applyTo(static_cast<Data::Point3D *>(p));
            }
        }

#if USE_ATTRIBS
        bool surface = static_cast<const MJMesh::Mesh *>(mesh)->isSurface();

        for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
             iter != mesh->feEnd(); iter++)
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

#if USE_GUI
        mesh->unfreeze();
#endif //#if USE_GUI
    }
}

#endif //#if USE_NUMERICAL

