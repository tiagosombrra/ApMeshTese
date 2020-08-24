#include "PMJMesh/MeshTransferable.h"

#include "PMJMesh/Communicator.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Mesh.h"
//#include "Performer/SequentialIdManager.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/GeometricShape.h"

using namespace PMJMesh;

PMJMesh::MeshTransferable::MeshTransferable() :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_MESH)
{
	//this->idManager = NULL;

	this->setMesh(NULL);

	this->dimension = 0;
}

PMJMesh::MeshTransferable::MeshTransferable(UInt dimension, Data::Mesh *mesh) :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_MESH)
{
	//this->idManager = NULL;

	this->setMesh(mesh);

	this->dimension = dimension;
}

PMJMesh::MeshTransferable::~MeshTransferable()
{
	//this->mesh = NULL;

	/*if (this->idManager)
	{
		delete this->idManager;
	}*/
}

void PMJMesh::MeshTransferable::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

Data::Mesh *PMJMesh::MeshTransferable::getMesh() const
{
	return this->mesh;
}

/*void PMJMesh::MeshTransferable::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

Performer::IdManager *PMJMesh::MeshTransferable::getIdManager() const
{
	return this->idManager;
}*/

UInt PMJMesh::MeshTransferable::packSize(const Parallel::Communicator *comm,
	ULInt numVertices, ULInt numFrontElements, ULInt numElements) const
{
	UInt size = 0;

	// unsigned - type
	size += comm->packSize(1, Parallel::Communicator::UINT);
	// long - number of vertices, number of front elements, number of elements
	//        ids of vertices,
	//		  ids of front elements, ids of the (2 or 3) vertices for front elements, ids of the two adjacent elements for front elements
	//		  ids of elements, ids of the (3 or 4) vertices for elements
	size += comm->packSize(3 + static_cast<UInt>(numVertices) +
		(1 + this->dimension + 2)*static_cast<UInt>(numFrontElements) +
		(1 + (this->dimension + 1))*static_cast<UInt>(numElements), Parallel::Communicator::ULINT);
	// real - coordinates of points
	size += comm->packSize(this->dimension*static_cast<UInt>(numVertices), Parallel::Communicator::REAL);

	return size;
}

UInt PMJMesh::MeshTransferable::packSize(const Parallel::Communicator *comm) const
{
	if (!this->mesh)
	{
		return comm->packSize(1, Parallel::Communicator::UINT);
	}

	return this->packSize(comm,
		this->mesh->verticesSize(),
		this->mesh->frontElementsSize(),
		this->mesh->size());
}

void PMJMesh::MeshTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	const Data::Mesh *mesh) const
{
	comm->pack(this->type, buffer, size, position);

	if (!mesh)
	{
		return;
	}

	comm->pack(mesh->verticesSize(), buffer, size, position);

	comm->pack(mesh->frontElementsSize(), buffer, size, position);

	comm->pack(mesh->size(), buffer, size, position);

	for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter++)
	{
		Data::Vertex *v = (*iter);

		comm->pack(v->getId(), buffer, size, position);

		for (UInt i = 0; i < this->dimension; i++)
		{
			comm->pack(v->getPoint()->getCoord(i), buffer, size, position);
		}
	}

	for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		comm->pack(fel->getId(), buffer, size, position);

		for (UInt i = 0; i < this->dimension; i++)
		{
			comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
		}

		Data::ElementList adj = static_cast<MJMesh::FrontElement *>(fel)->getAdjacency();

		for (UInt i = 0; i < 2; i++)
		{
			ULInt id = 0;

			if (!adj.empty())
			{
				id = adj.front()->getId();

				adj.pop_front();
			}

			comm->pack(id, buffer, size, position);
		}
	}

	for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter++)
	{
		Data::Element *el = (*iter);

		comm->pack(el->getId(), buffer, size, position);

		for (UInt i = 0; i < this->dimension + 1; i++)
		{
			comm->pack(el->getVertex(i)->getId(), buffer, size, position);
		}
	}
}

void PMJMesh::MeshTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	if (!this->mesh)
	{
		comm->pack(this->type, buffer, size, position);

		return;
	}

	this->pack(comm, buffer, size, position, this->mesh);
}

Parallel::Package PMJMesh::MeshTransferable::pack(const Parallel::Communicator *comm) const
{
	Int size = 0;
	char *buffer = NULL;
	Int position = 0;

	if (!this->mesh)
	{
		size += this->packSize(comm);

		buffer = new char[size];

		this->pack(comm, buffer, size, position);

		Parallel::Package p(size, buffer);

		return p;
	}

	size = this->packSize(comm, this->mesh->verticesSize(), this->mesh->frontElementsSize(), this->mesh->size());

	buffer = new char[size];

	this->pack(comm, buffer, size, position, this->mesh);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::MeshTransferable::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	Int size = p.first;
	char *buffer = p.second;

	if (size == comm->packSize(1, Parallel::Communicator::UINT))
	{
		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		return;
	}

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	this->mesh = new MJMesh::Mesh();

	ULInt numVertices = 0;

	comm->unpack(numVertices, buffer, size, position);

	ULInt numFrontElements = 0;

	comm->unpack(numFrontElements, buffer, size, position);

	ULInt numElements = 0;

	comm->unpack(numElements, buffer, size, position);

	{
		//decide what is the dimension

		this->dimension = 2;

		Int testSize = this->packSize(comm, numVertices, numFrontElements, numElements);

		if (testSize != size)
		{
			this->dimension = 3;
		}
	}

	/*if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 3);
	}*/

#if USE_C__11
	Data::VertexHashMap vMap;

	vMap.reserve(numVertices);
#else
	Data::VertexMap vMap;
#endif //#if USE_C__11
	//Data::FrontElementMap felMap;

	for (ULInt i = 0; i < numVertices; i++)
	{
		Data::Point *p = (this->dimension == 2) ? static_cast<Data::Point *>(new Data::Point2D()) : static_cast<Data::Point *>(new Data::Point3D());

		ULInt id = 0;

		comm->unpack(id, buffer, size, position);

		/*if ((this->idManager) && (id > this->idManager->getId(0)))
		{
			this->idManager->setId(0, id);
		}*/

		for (UInt j = 0; j < this->dimension; j++)
		{
			Real coord = 0.0;

			comm->unpack(coord, buffer, size, position);

			p->setCoord(j, coord);
		}

		MJMesh::Vertex *v = new MJMesh::Vertex(p, id);

		vMap[id] = v;

		this->mesh->add(v);
	}

#if USE_C__11
	FEListHashMap mapAdjEls;

	mapAdjEls.reserve(numElements);
#else
	FEListMap mapAdjEls;
#endif //#if USE_C__11

	for (ULInt i = 0; i < numFrontElements; i++)
	{
		ULInt id = 0, idv[dimension], idAdj[2];

		comm->unpack(id, buffer, size, position);

		/*if ((this->idManager) && (id > this->idManager->getId(1)))
		{
			this->idManager->setId(1, id);
		}*/

		for (UInt j = 0; j < dimension; j++)
		{
			comm->unpack(idv[j], buffer, size, position);
		}

		comm->unpack(idAdj[0], buffer, size, position);
		comm->unpack(idAdj[1], buffer, size, position);

		MJMesh::FrontElement *fel = (dimension == 2) ?
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
				static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

		if (idAdj[0] != 0)
		{
			mapAdjEls[idAdj[0]].push_back(fel);

			if (idAdj[1] != 0)
			{
				mapAdjEls[idAdj[1]].push_back(fel);
			}
		}

		//felMap[id] = fel;

		this->mesh->add(fel);
	}

	for (ULInt i = 0; i < numElements; i++)
	{
		ULInt id, idv[dimension + 1];

		comm->unpack(id, buffer, size, position);

		/*if ((this->idManager) && (id > this->idManager->getId(2)))
		{
			this->idManager->setId(2, id);
		}*/

		for (UInt j = 0; j < dimension + 1; j++)
		{
			comm->unpack(idv[j], buffer, size, position);
		}

		MJMesh::Element *el = (dimension == 2) ?
			static_cast<MJMesh::Element *>(new MJMesh::Element2D(
				static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
				static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id)) :
			static_cast<MJMesh::Element *>(new MJMesh::Element3D(
				static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
				static_cast<MJMesh::Vertex *>(vMap[idv[2]]), static_cast<MJMesh::Vertex *>(vMap[idv[3]]), id));

		this->mesh->add(el);

#if USE_C__11
		FEListHashMap::iterator iter = mapAdjEls.find(id);
#else
		FEListMap::iterator iter = mapAdjEls.find(id);
#endif //#if USE_C__11

		if (iter == mapAdjEls.end())
		{
			continue;
		}

		while (!(*iter).second.empty())
		{
			static_cast<MJMesh::FrontElement *>((*iter).second.front())->add(el);

			(*iter).second.pop_front();
		}

		mapAdjEls.erase(iter);
	}
}
