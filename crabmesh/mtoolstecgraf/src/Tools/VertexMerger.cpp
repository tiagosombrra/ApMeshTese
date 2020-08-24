#include "Tools/VertexMerger.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"

Tools::VertexMerger::VertexMerger() :
	Performer::VertexMerger::VertexMerger(),
	Tools::Tool::Tool()
{

}

Tools::VertexMerger::~VertexMerger()
{

}

void Tools::VertexMerger::execute()
{
	if ((!this->mesh) && (!this->boundary))
	{
		return;
	}

	this->startMeasure(static_cast<UInt>((this->mesh ? this->mesh->verticesSize() : this->boundary->verticesSize())) + 1);

	Performer::VertexMerger::execute();

	this->endMeasure();
}

void Tools::VertexMerger::mergeMesh(Data::Vertex *v, Data::Mesh::VertexIterator iter)
{
	this->addMeasure(1, 0);

	Performer::VertexMerger::mergeMesh(v, iter);
}

void Tools::VertexMerger::mergeBoundary(Data::Vertex *v, Data::Boundary::VertexIterator iter)
{
	this->addMeasure(1, 0);

	Performer::VertexMerger::mergeBoundary(v, iter);
}

void Tools::VertexMerger::replace(Data::FrontElement *fel, Data::Vertex *vOld, Data::Vertex *vNew) const
{
	static_cast<MJMesh::Vertex *>(vOld)->remove(fel);
	static_cast<MJMesh::Vertex *>(vNew)->add(fel);

	Performer::VertexMerger::replace(fel, vOld, vNew);
}

void Tools::VertexMerger::replace(Data::Element *e, Data::Vertex *vOld, Data::Vertex *vNew) const
{
	static_cast<MJMesh::Vertex *>(vOld)->remove(e);
	static_cast<MJMesh::Vertex *>(vNew)->add(e);

	Performer::VertexMerger::replace(e, vOld, vNew);
}
