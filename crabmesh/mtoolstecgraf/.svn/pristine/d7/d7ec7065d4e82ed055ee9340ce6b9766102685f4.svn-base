#include "PMJMesh/BoundaryTransferable.h"

#include "PMJMesh/Communicator.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Boundary.h"

#include "Performer/SequentialIdManager.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/GeometricShape.h"

using namespace PMJMesh;

PMJMesh::BoundaryTransferable::BoundaryTransferable() :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_BOUNDARY)
{
	this->idManager = NULL;

	this->setBoundary(NULL);

	this->dimension = 0;
}

PMJMesh::BoundaryTransferable::BoundaryTransferable(UInt dimension, Data::Boundary *boundary) :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_BOUNDARY)
{
	this->idManager = NULL;

	this->setBoundary(boundary);

	this->dimension = dimension;
}

PMJMesh::BoundaryTransferable::~BoundaryTransferable()
{
	//this->boundary = NULL;

	if (this->idManager)
	{
		delete this->idManager;
	}
}

void PMJMesh::BoundaryTransferable::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

Data::Boundary *PMJMesh::BoundaryTransferable::getBoundary() const
{
	return this->boundary;
}

void PMJMesh::BoundaryTransferable::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

Performer::IdManager *PMJMesh::BoundaryTransferable::getIdManager() const
{
	return this->idManager;
}

UInt PMJMesh::BoundaryTransferable::packSize(const Parallel::Communicator *comm, ULInt numVertices, ULInt numElements) const
{
	UInt size = 0;

	// unsigned - type,
	size += comm->packSize(1, Parallel::Communicator::UINT);
	// long - number of vertices, number of elements,
	//        ids of vertices, ids of front elements,
	//        ids of the (2 or 3) vertices for front elements
	size += comm->packSize(2 + static_cast<UInt>(numVertices) + (this->dimension + 1)*static_cast<UInt>(numElements), Parallel::Communicator::ULINT);
	// real - coordinates of points, tolerance
	size += comm->packSize(this->dimension*static_cast<UInt>(numVertices) + 1, Parallel::Communicator::REAL);

	return size;
}

UInt PMJMesh::BoundaryTransferable::packSize(const Parallel::Communicator *comm) const
{
	return this->packSize(comm, this->boundary->verticesSize(), this->boundary->size());
}

void PMJMesh::BoundaryTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	const Data::Boundary *boundary) const
{
	comm->pack(this->type, buffer, size, position);

	ULInt numVertices = boundary->verticesSize();

	comm->pack(numVertices, buffer, size, position);

	ULInt numElements = boundary->size();

	comm->pack(numElements, buffer, size, position);

	for (Data::VertexList::const_iterator iter = boundary->vBegin();
		 iter != boundary->vEnd(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		ULInt id = (*iter)->getId();

		comm->pack(id, buffer, size, position);

		for (UInt j = 0; j < this->dimension; j++)
		{
			Real coord = p->getCoord(j);

			comm->pack(coord, buffer, size, position);
		}
	}

	for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
		 iter != boundary->eEnd(); iter++)
	{
		ULInt id = (*iter)->getId();

		comm->pack(id, buffer, size, position);

		for (UInt j = 0; j < (*iter)->numVertices(); j++)
		{
			id = (*iter)->getVertex(j)->getId();

			comm->pack(id, buffer, size, position);
		}
	}

	comm->pack(Data::getTolerance(), buffer, size, position);
}

void PMJMesh::BoundaryTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	if (!this->boundary)
	{
		comm->pack(this->type, buffer, size, position);

		return;
	}

	//Data::VertexVector vertices = this->boundary->getVertices();
	//Data::VertexList vertices = this->boundary->getVertices();
	//Data::FrontElementVector elements = this->boundary->getElements();
	//Data::FrontElementList elements = this->boundary->getElements();

	//this->pack(comm, buffer, size, position, vertices, elements);
	this->pack(comm, buffer, size, position, this->boundary);
}

Parallel::Package PMJMesh::BoundaryTransferable::pack(const Parallel::Communicator *comm) const
{
	Int size = 0;
	char *buffer = NULL;
	Int position = 0;

	if (!this->boundary)
	{
		size += comm->packSize(1, Parallel::Communicator::UINT);

		buffer = new char[size];

		this->pack(comm, buffer, size, position);

		Parallel::Package p(size, buffer);

		return p;
	}

	size = this->packSize(comm, this->boundary->verticesSize(), this->boundary->size());

	buffer = new char[size];

	this->pack(comm, buffer, size, position, this->boundary);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::BoundaryTransferable::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	Int size = p.first;
	char *buffer = p.second;

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	this->boundary = new MJMesh::Boundary();

	ULInt numVertices = 0;

	comm->unpack(numVertices, buffer, size, position);

	ULInt numElements = 0;

	comm->unpack(numElements, buffer, size, position);

	{
		//decide what is the dimension

		this->dimension = 2;
		Int testSize = this->packSize(comm, numVertices, numElements);

		if (testSize != size)
		{
			this->dimension = 3;
		}
	}

	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

#if USE_C__11
	Data::VertexHashMap vMap;

	vMap.reserve(numVertices);
#else
	Data::VertexMap vMap;
#endif //#if USE_C__11

	for (ULInt i = 0; i < numVertices; i++)
	{
		Data::Point *p = (this->dimension == 2) ? static_cast<Data::Point *>(new Data::Point2D()) : static_cast<Data::Point *>(new Data::Point3D());

		ULInt id = 0;

		comm->unpack(id, buffer, size, position);

		if (id > this->idManager->getId(0))
		{
			this->idManager->setId(0, id);
		}

		for (UInt j = 0; j < this->dimension; j++)
		{
			Real coord = 0.0;

			comm->unpack(coord, buffer, size, position);

			p->setCoord(j, coord);
		}

		MJMesh::Vertex *v = new MJMesh::Vertex(p, id);

		vMap[id] = v;

		this->boundary->add(v);
	}

	for (ULInt i = 0; i < numElements; i++)
	{
		MJMesh::FrontElement *fel = NULL;

		ULInt id, idv1, idv2;

		comm->unpack(id, buffer, size, position);

		if (id > this->idManager->getId(1))
		{
			this->idManager->setId(1, id);
		}

		comm->unpack(idv1, buffer, size, position);
		comm->unpack(idv2, buffer, size, position);

		if (this->dimension == 2)
		{
			fel = new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]),
				static_cast<MJMesh::Vertex *>(vMap[idv2]), id);
		}
		else
		{
			ULInt idv3;

			comm->unpack(idv3, buffer, size, position);

			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]),
				static_cast<MJMesh::Vertex *>(vMap[idv2]),
				static_cast<MJMesh::Vertex *>(vMap[idv3]), id);
		}

		this->boundary->add(fel);
	}

	Real tolerance;

	comm->unpack(tolerance, buffer, size, position);

	Data::setTolerance(tolerance);
}
