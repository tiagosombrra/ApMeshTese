#include "Tools/MeshSplitter.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"

Tools::MeshSplitter::MeshSplitter() :
	Performer::MeshSplitter::MeshSplitter(),
	Tools::Tool::Tool()
{

}

Tools::MeshSplitter::~MeshSplitter()
{

}

void Tools::MeshSplitter::execute()
{
	if (!this->mesh)
	{
		return;
	}

	this->startMeasure(static_cast<UInt>(this->mesh->verticesSize()));

	Performer::MeshSplitter::execute();

	this->endMeasure();
}

void Tools::MeshSplitter::addToVertices(Data::Vertex *v, Data::Mesh *mesh, Data::VertexList &vertices)
{
	this->addMeasure(1, 0);

	Performer::MeshSplitter::addToVertices(v, mesh, vertices);
}

void Tools::MeshSplitter::copy()
{
	if (!this->duplicate)
	{
		this->addMeasure(0, static_cast<UInt>(this->meshes.size()) + 1);
	}

	Performer::MeshSplitter::copy();

	if (!this->duplicate)
	{
		this->addMeasure(1, 0);
	}
}

void Tools::MeshSplitter::copy(Data::Mesh *mesh)
{
	if (this->duplicate)
	{
		this->addMeasure(0, static_cast<UInt>(mesh->verticesSize() + mesh->frontElementsSize() + mesh->size()));
	}
	else
	{
		this->addMeasure(1, 0);
	}

	Performer::MeshSplitter::copy(mesh);
}

Data::Mesh *Tools::MeshSplitter::makeMesh() const
{
	return new MJMesh::Mesh();
}

Data::Boundary *Tools::MeshSplitter::makeBoundary() const
{
	return new MJMesh::Boundary();
}

Data::Vertex *Tools::MeshSplitter::makeVertex(const Data::Vertex *vertex,
	const Performer::MeshSplitter::VMap &/*vMapSoFar*/) const
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

Data::FrontElement *Tools::MeshSplitter::makeFrontElement(const Data::FrontElement *frontElement,
	const Performer::MeshSplitter::VMap &vMap, const Performer::MeshSplitter::FEMap &/*felMapSoFar*/) const
{
	this->addMeasure(1, 0);

	UInt numVertices = frontElement->numVertices();

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
				frontElement->getId()));
}

Data::Element *Tools::MeshSplitter::makeElement(const Data::Element *element,
	const Performer::MeshSplitter::VMap &vMap, const Performer::MeshSplitter::FEMap &/*felMap*/, const Performer::MeshSplitter::EMap &/*elMapSoFar*/) const
{
	this->addMeasure(1, 0);

	UInt numVertices = element->numVertices();

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
				element->getId()));
}
