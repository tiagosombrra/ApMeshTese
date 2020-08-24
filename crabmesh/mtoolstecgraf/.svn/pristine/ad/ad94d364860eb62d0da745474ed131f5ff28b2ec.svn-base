#include "Tools/BoundaryCreator.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"

Tools::BoundaryCreator::BoundaryCreator(Data::Mesh *mesh) :
	Performer::BoundaryCreator::BoundaryCreator(mesh),
	Tools::Tool::Tool()
{
    this->surface = false;
}

Tools::BoundaryCreator::~BoundaryCreator()
{

}

void Tools::BoundaryCreator::execute()
{
	if (!this->mesh)
	{
		return;
	}

	this->startMeasure(static_cast<UInt>(this->mesh->size()));

	Performer::BoundaryCreator::execute();

	this->endMeasure();
}

void Tools::BoundaryCreator::execute2D(Data::Element *e, Performer::BoundaryCreator::DoneList2D &done)
{
	this->addMeasure(1, 0);

	Performer::BoundaryCreator::execute2D(e, done);
}

void Tools::BoundaryCreator::execute3D()
{
    if (static_cast<MJMesh::Mesh *>(this->mesh)->isSurface())
    {
        this->surface = true;

        this->execute2D();

        return;
    }

    Performer::BoundaryCreator::execute3D();
}

void Tools::BoundaryCreator::execute3D(Data::Element *e, Performer::BoundaryCreator::DoneList3D &done)
{
	this->addMeasure(1, 0);

	Performer::BoundaryCreator::execute3D(e, done);
}

Data::Boundary *Tools::BoundaryCreator::makeBoundary() const
{
	return new MJMesh::Boundary();
}

Data::FrontElement *Tools::BoundaryCreator::makeFrontElement(const Data::VertexVector &vertices, Data::Element *e) const
{
	MJMesh::FrontElement *fel = NULL;

	if (this->surface)
    {
        fel = new MJMesh::FrontElementSurface(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]));
    }
	else if (vertices.size() == 2)
	{
		fel = new MJMesh::FrontElement2D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]));
	}
	else if (vertices.size() == 3)
	{
		fel = new MJMesh::FrontElement3D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]),
			static_cast<MJMesh::Vertex *>(vertices[2]));
	}
#if USE_QUADS
	else if (vertices.size() == 4)
	{
		fel = new MJMesh::FrontElement3D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]),
			static_cast<MJMesh::Vertex *>(vertices[2]),
			static_cast<MJMesh::Vertex *>(vertices[3]));
	}
#endif //#if USE_QUADS

	if (fel)
	{
		fel->add(static_cast<MJMesh::Element *>(e));
	}

	return fel;
}
