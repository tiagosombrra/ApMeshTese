#include "Tools/MeshMerger.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"

Tools::MeshMerger::MeshMerger() :
	Performer::MeshMerger::MeshMerger(),
	Tools::Tool::Tool()
{

}

Tools::MeshMerger::~MeshMerger()
{

}

void Tools::MeshMerger::execute()
{
	this->startMeasure();

	Performer::MeshMerger::execute();

	this->endMeasure();
}

void Tools::MeshMerger::merge(const Data::Mesh *mesh)
{
	if (!mesh)
	{
		return;
	}

	this->addMeasure(0, static_cast<UInt>(mesh->verticesSize() + mesh->frontElementsSize() + mesh->size()));

#if USE_C__11
    this->feAdjacency.reserve(mesh->size());
#endif //#if USE_C__11

	Performer::MeshMerger::merge(mesh);

	this->feAdjacency.clear();
}

Data::Mesh *Tools::MeshMerger::makeMesh() const
{
	return new MJMesh::Mesh();
}

Data::Boundary *Tools::MeshMerger::makeBoundary() const
{
	return new MJMesh::Boundary();
}

Data::Vertex *Tools::MeshMerger::makeVertex(const Data::Vertex *vertex,
	const Performer::MeshMerger::VMap &/*vMapSoFar*/) const
{
	this->addMeasure(1, 0);

	Data::Point *oldp = vertex->getPoint();

	Data::Point *newp = (oldp->dimension() == 2) ? static_cast<Data::Point *>(new Data::Point2D(oldp)) :
#if USE_EXPERIMENTAL_3
        static_cast<Data::Point *>(new Data::Point3D(oldp));
#else
		((oldp->dimension() == 3) ? static_cast<Data::Point *>(new Data::Point3D(oldp)) : new Data::Point(oldp));
#endif //#if USE_EXPERIMENTAL_3

	return new MJMesh::Vertex(newp, vertex->getId());
}

Data::FrontElement *Tools::MeshMerger::makeFrontElement(const Data::FrontElement *frontElement,
	const Performer::MeshMerger::VMap &vMap, const Performer::MeshMerger::FEMap &/*felMapSoFar*/) const
{
	this->addMeasure(1, 0);

    Data::FrontElement *newFel = NULL;

	UInt dimension = frontElement->getShape()->dimension();

	if (dimension == 2)
    {
        newFel = new MJMesh::FrontElement2D(
            static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
            frontElement->getId());
    }
    else
    {
        UInt numVertices = frontElement->numVertices();

        if (numVertices == 2)
        {
            newFel = new MJMesh::FrontElementSurface(
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
                frontElement->getId());
        }
        else if (numVertices == 3)
        {
            newFel = new MJMesh::FrontElement3D(
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(2)->getId())).second),
                frontElement->getId());
        }
#if USE_QUADS
        else if (numVertices == 4)
        {
            newFel = new MJMesh::FrontElement3D(
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(2)->getId())).second),
                static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(3)->getId())).second),
                frontElement->getId());
        }
#endif //#if USE_QUADS
    }

    if (newFel)
    {
        for (Data::ElementList::const_iterator iter = static_cast<const MJMesh::FrontElement *>(frontElement)->eBegin();
             iter != static_cast<const MJMesh::FrontElement *>(frontElement)->eEnd(); iter++)
        {
            this->feAdjacency[(*iter)->getId()].push_back(const_cast<Data::FrontElement *>(newFel));
        }
    }

    return newFel;

	/*UInt numVertices = frontElement->numVertices();

	return (numVertices == 2) ?
		static_cast<MJMesh::FrontElement *>(
			new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
				frontElement->getId())) :
		static_cast<MJMesh::FrontElement *>(
			new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(1)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(frontElement->getVertex(2)->getId())).second),
				frontElement->getId()));*/
}

Data::Element *Tools::MeshMerger::makeElement(const Data::Element *element,
	const Performer::MeshMerger::VMap &vMap, const Performer::MeshMerger::FEMap &/*felMap*/, const Performer::MeshMerger::EMap &/*elMapSoFar*/) const
{
	this->addMeasure(1, 0);

	UInt dimension = element->getShape()->dimension();
	UInt numVertices = element->numVertices();

	MJMesh::Element *newEl = NULL;

	if (dimension == 2)
    {
        if (numVertices == 3)
        {
            newEl = new MJMesh::Element2D(
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
				element->getId());
        }
#if USE_QUADS
        else if (numVertices == 4)
        {
            newEl = new MJMesh::Element2D(
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
				element->getId());
        }
#endif //#if USE_QUADS
    }
    else if (numVertices == 3)
    {
        newEl = new MJMesh::ElementSurface(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            element->getId());
    }
#if USE_QUADS
    else if ((numVertices == 4) && (dynamic_cast<const MJMesh::ElementSurface *>(element)))
    {
        newEl = new MJMesh::ElementSurface(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
            element->getId());
    }
#endif //#if USE_QUADS
    else if (numVertices == 4)
    {
        newEl = new MJMesh::Element3D(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
            element->getId());
    }
#if USE_BRICKS
    else if (numVertices == 5)
    {
        newEl = new MJMesh::Element3D(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(4)->getId())).second),
            element->getId());
    }
    else if (numVertices == 6)
    {
        newEl = new MJMesh::Element3D(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(4)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(5)->getId())).second),
            element->getId());
    }
    else if (numVertices == 8)
    {
        newEl = new MJMesh::Element3D(
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(4)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(5)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(6)->getId())).second),
            static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(7)->getId())).second),
            element->getId());
    }
#endif //#if USE_BRICKS

    if (newEl)
    {
        FEAdjacency::iterator iter = this->feAdjacency.find(element->getId());

        if (iter != this->feAdjacency.end())
        {
            for (Data::FrontElementList::iterator iter2 = (*iter).second.begin();
                 iter2 != (*iter).second.end(); iter2++)
            {
                static_cast<MJMesh::FrontElement *>((*iter2))->add(newEl);
            }
        }
    }

    return newEl;

	/*UInt numVertices = element->numVertices();

	return (numVertices == 3) ?
		static_cast<MJMesh::Element *>(
			new MJMesh::Element2D(
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
				element->getId())) :
		static_cast<MJMesh::Element *>(
			new MJMesh::Element3D(
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(0)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(1)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(2)->getId())).second),
				static_cast<MJMesh::Vertex *>((*vMap.find(element->getVertex(3)->getId())).second),
				element->getId()));*/
}
