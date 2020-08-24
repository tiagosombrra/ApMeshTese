#include "PMJMesh2/APrioriSupportMeshLaplacianSmoother.h"

#include "MJMesh/Boundary.h"

#include "Data/Mesh.h"
#include "Data/FrontElement.h"
#include "Data/Vertex.h"
#include "Data/Triangle3D.h"
#include "Data/Vector3D.h"
#include "Data/Point3D.h"

#include "Tools/BoundaryCreator.h"

PMJMesh2::APrioriSupportMeshLaplacianSmoother::APrioriSupportMeshLaplacianSmoother(
    Data::Mesh *mesh, Data::Boundary *boundary, bool testAdjacency, Real factor, UInt maxCount, UInt minMoved, bool sortPoints) :
    MJMesh::LaplacianSmoother::LaplacianSmoother(3, mesh, testAdjacency, factor, sortPoints)
{
    this->setBoundary(boundary);
    this->setMaxCount(maxCount);
    this->setMinMoved(minMoved);
}

PMJMesh2::APrioriSupportMeshLaplacianSmoother::~APrioriSupportMeshLaplacianSmoother()
{

}

void PMJMesh2::APrioriSupportMeshLaplacianSmoother::setBoundary(Data::Boundary *boundary)
{
    this->boundary = boundary;
}

void PMJMesh2::APrioriSupportMeshLaplacianSmoother::setMaxCount(UInt maxCount)
{
    this->maxCount = maxCount;
}

void PMJMesh2::APrioriSupportMeshLaplacianSmoother::setMinMoved(UInt minMoved)
{
    this->minMoved = minMoved;
}

void PMJMesh2::APrioriSupportMeshLaplacianSmoother::execute()
{
    Real maxedge = 0.0;
    Real minedge = REAL_MAX;

    for (Data::Boundary::FrontElementIterator it = this->boundary->eBegin();
         it != this->boundary->eEnd(); ++it)
    {
        Real d = (*it)->getShape()->size();

        if (d > maxedge)
        {
            maxedge = d;
        }

        if (d < minedge)
        {
            minedge = d;
        }
    }

    //Real maxdist = 0.5*(minedge + maxedge);
    Real maxdist = minedge;


    UInt side = 0;



    Data::Triangle3D *t = static_cast<Data::Triangle3D *>((*this->mesh->eBegin())->getShape());

    Data::Vector3D n = t->normal();

    if ((n.getCoord(0) > n.getCoord(1)) && (n.getCoord(0) > n.getCoord(2)))
    {
        side = 0;
    }
    else if ((n.getCoord(1) > n.getCoord(0)) && (n.getCoord(1) > n.getCoord(2)))
    {
        side = 1;
    }
    else
    {
        side = 2;
    }

    Real coordPlane = t->getPoint(0)->getCoord(side);


    Tools::BoundaryCreator creator;

    MJMesh::Boundary *supMeshBoundary = new MJMesh::Boundary();

    creator.setMesh(this->mesh);
    creator.setBoundary(supMeshBoundary);
    creator.setOnlyVertices(true);

    creator.execute();

    UInt moved = this->minMoved;

    UInt count = 0;

    //this->mesh->sortMesh();
    //this->boundary->sortBoundary();

    while ((moved >= this->minMoved) && ((maxCount == 0) || (count < this->maxCount)))
    {
        moved = 0;
        count++;

        for (Data::Mesh::VertexIterator it = this->mesh->vBegin(); it != this->mesh->vEnd(); it++)
        {
            Data::Vertex *v = (*it);

            if (supMeshBoundary->have(v))
            {
                continue;
            }

            Data::Point3D *p = static_cast<Data::Point3D *>(v->getPoint());

            Data::PointSet pts;

            for (Data::Boundary::VertexIterator it2 = this->boundary->vBegin();
                it2 != this->boundary->vEnd(); ++it2)
            {
                Data::Vertex *v2 = (*it2);
                Data::Point3D *p2 = static_cast<Data::Point3D *>(v2->getPoint());

                Real dist = p2->distance(p);

                Real distPlane = std::fabs(p2->getCoord(side) - coordPlane);

                if (dist > distPlane + maxdist + Data::getTolerance() )
                {
                    continue;
                }

                pts.insert(p2);
            }

            /*if ((count < maxCount) && (pts.empty()))
            {
                continue;
            }*/

            Data::ElementList elements;

            this->findAdjacentElements(v, elements);

            this->findAdjacency(elements, v, pts);

            Real pos[3], disp[3];

            for (UInt i = 0; i < 3; i++)
            {
                pos[i] = p->getCoord(i);
                disp[i] = 0.0;
            }

            this->findDisplacement(p, pos, pts, disp);

            ///TRUNCANDO VALORES APOS A 16 CASA
            for (UInt j = 0; j < 3; j++)
            {
                long int i = disp[j] * 1.0e15;
                disp[j] = i / 1.0e15;
            }

            for (UInt j = 0; j < 3; j++)
            {
                p->setCoord(j, pos[j] + disp[j]);
            }

            if ((this->testAdjacency) && (!this->isAdjacencyOk(v, elements)))
            {
                v->getPoint()->setCoords(pos);
            }
            else
            {
                moved++;
            }
        }
    }

    supMeshBoundary->clear();

    delete supMeshBoundary;

    //return false;
}
