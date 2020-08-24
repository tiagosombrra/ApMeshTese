#include "PMJMesh/MJMeshTask.h"

#include "PMJMesh/Communicator.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Vertex.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"

using namespace PMJMesh;

PMJMesh::MJMeshTask::MJMeshTask() :
	Parallel::Task(TRANSFERABLE_MJMESH_TASK)
{
	this->taskType = TASK_MJMESH_MAIN_DRIVE;
}

PMJMesh::MJMeshTask::MJMeshTask(PMJMesh::Partition *partition,
	PMJMesh::WorkerMainDrive *builder, PMJMesh::Communicator *comm) :
	Parallel::Task(TRANSFERABLE_MJMESH_TASK, comm, builder)
{
	this->taskType = TASK_MJMESH_MAIN_DRIVE;

	this->setPartition(partition);
}

PMJMesh::MJMeshTask::~MJMeshTask()
{
	if (this->status == this->finished)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->setSubfront(NULL);
	}
}

void PMJMesh::MJMeshTask::setPartition(Parallel::Partition *p)
{
	Parallel::Task::setPartition(p);

	if (p)
	{
		if (static_cast<PMJMesh::Partition *>(this->partition)->getSubfront())
		{
			this->status = ready;
		}

		this->id = p->getId();
	}
}

void PMJMesh::MJMeshTask::setMesh(MJMesh::Mesh *mesh)
{
	if (this->builder)
	{
		dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->setMesh(mesh);
	}
}

MJMesh::Mesh *PMJMesh::MJMeshTask::getMesh() const
{
	if (this->builder)
	{
		return const_cast<MJMesh::Mesh *>(dynamic_cast<const PMJMesh::WorkerMainDrive *>(this->builder)->getMesh());
	}

	return NULL;
}

PMJMesh::Front *PMJMesh::MJMeshTask::getFront() const
{
	if (this->partition)
	{
		return static_cast<PMJMesh::Partition *>(this->partition)->getSubfront();
	}

	if (this->builder)
	{
		return const_cast<PMJMesh::Front *>(static_cast<const PMJMesh::Front *>(dynamic_cast<const PMJMesh::WorkerMainDrive *>(this->builder)->getFront()));
	}

	return NULL;
}

Real PMJMesh::MJMeshTask::load() const
{
	return ((this->partition) && ((!static_cast<PMJMesh::Partition *>(this->partition)->getSubfront()) ||
		 (static_cast<PMJMesh::Partition *>(this->partition)->getSubfront()->isEmpty()))) ? 0.0 : Parallel::Task::load();
}

void PMJMesh::MJMeshTask::makeBuilder()
{
	if (this->builder)
	{
		delete this->builder;
	}

	this->builder = new PMJMesh::WorkerMainDrive(this->comm);
}

void PMJMesh::MJMeshTask::resizeGenerated(UInt size)
{
	this->generated.resize(size, false);
}

void PMJMesh::MJMeshTask::setGenerated(UInt shift, bool generated)
{
	this->generated[shift] = generated;
}

bool PMJMesh::MJMeshTask::isGenerated(UInt shift) const
{
	return (shift < this->generated.size()) ? this->generated[shift] : false;
}

bool PMJMesh::MJMeshTask::isCompletelyGenerated() const
{
	for (UInt i = 0; i < this->generated.size(); i++)
	{
		if (!this->generated[i])
		{
			return false;
		}
	}

	return true;
}

UInt PMJMesh::MJMeshTask::packSize(const Parallel::Communicator *comm,
	ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
	ULInt numFrontVertices, ULInt numFrontFrontElements) const
{
	UInt size = 0;

	//unsigned - type, tasktype
	size += comm->packSize(2, Communicator::UINT);

	//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

	//long - id
	size += comm->packSize(1, Communicator::ULINT);

	//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

	//if only partition exists, than pack only the partition
	//if only builder exists, than pack the dimension, the mesh and the front
	//if both exist, pack the dimension, the box, the mesh and the front
	//if builder will be packed:
	// if only mesh exists, pack its vertices, elements and front elements
	// if only front exists, pack its vertices and front elements
	// if both exist, pack mesh vertices, elements and front elements and
	//  point out the vertices and front elements belonging to the front

	//bool - partition exists, builder exists
	size += comm->packSize(2, Communicator::BOOL);

	//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

	if ((this->partition) || (this->builder))
	{
		if (!this->builder)
		{
			//partition size
			size += comm->packSize(1, Communicator::INT);

			//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

			//partition
			size += this->partition->packSize(comm);

			//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;
		}
		else
		{
			//unsigned - builder status, dimension
			size += comm->packSize(2, Communicator::UINT);

			//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

			UInt dimension = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->getDimension();

			if (this->partition)
			{
				//const Data::Box *box = static_cast<PMJMesh::Partition *>(this->partition)->getBox();

				//box.setBox(NULL, NULL);

				//real - partition->box->min->coords, partition->box->max->coords,
				//       load
				size += comm->packSize(2*dimension + 1, Communicator::REAL);

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;
			}

			//bool - mesh exists, front exists
			size += comm->packSize(2, Communicator::BOOL);

			//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

			MJMesh::Mesh *mesh = this->getMesh();

			if (mesh)
			{
				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

				//long - numvertices, numfrontelements, numelements,
				// vertices->id, frontelements->ids, frontelements->vertices->id,
				// frontelements->adjacency->id
				// elements->ids, elements->vertices->id
				size += comm->packSize(3 + static_cast<UInt>(numMeshVertices) +
					(dimension + 3)*static_cast<UInt>(numMeshFrontElements) +
					(dimension + 2)*static_cast<UInt>(numMeshElements), //the ids of elements are needed
					//(dimension + 1)*numMeshElements,
					Communicator::ULINT);

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

				//real - vertices->coords
				size += comm->packSize(dimension*static_cast<UInt>(numMeshVertices), Communicator::REAL);

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;
			}

			PMJMesh::Front *front = this->getFront();

			if (front)
			{
				//bool - front sorted
				size += comm->packSize(1, Communicator::BOOL);

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

				//long - numvertices, numfrontelements in boundary,
				// numfrontelements in elements, numfrontelements in rejected,
				// numfrontelements in crossing
				// vertices->id, frontelements->id
				size += comm->packSize(5 + static_cast<UInt>(numFrontVertices) + static_cast<UInt>(numFrontFrontElements), Communicator::ULINT);

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

				if (!mesh)
				{
					//real - vertices->coords
					size += comm->packSize(dimension*static_cast<UInt>(numFrontVertices), Communicator::REAL);

					//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;

					//long - frontelements->vertices->id
					size += comm->packSize(dimension*static_cast<UInt>(numFrontFrontElements), Communicator::ULINT);

					//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;
				}

				//std::cout << "pack size for task " << this->getId() << " = " << size << std::endl;
			}
		}
	}

	/*if (this->builder)
	{
		//unsigned - dimension
		size += comm->packSize(1, Communicator::UINT);
	}

	if (this->partition)
	{
		if (this->builder)
		{
			const Data::Box *box = static_cast<PMJMesh::Partition *>(this->partition)->getBox();

			UInt dimension = box->dimension();

			//box.setBox(NULL, NULL);

			//real - partition->box->min->coords, partition->box->max->coords,
			size += comm->packSize(2*dimension, Communicator::REAL);
		}
		else
		{
			size += this->partition->packSize(comm);
		}
	}

	if (this->builder)
	{
		UInt dimension = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->getDimension();

		//unsigned - status
		size += comm->packSize(1, Communicator::UINT);

		//bool - mesh exists, front exists
		size += comm->packSize(2, Communicator::BOOL);

		MJMesh::Mesh *mesh = this->getMesh();

		if (mesh)
		{
			//unsigned - numvertices, numfrontelements, numelements
			size += comm->packSize(3, Communicator::UINT);

			//long - vertices->id, frontelements->ids, frontelements->vertices->id,
			// frontelements->adjacency->id
			// elements->ids, elements->vertices->id
			size += comm->packSize(numMeshVertices +
				(dimension + 3)*numMeshFrontElements +
				(dimension + 2)*numMeshElements, //the ids of elements are needed
				//(dimension + 1)*numMeshElements,
				Communicator::ULINT);

			//real - vertices->coords
			size += comm->packSize(dimension*numMeshVertices, Communicator::REAL);
		}

		PMJMesh::Front *front = this->getFront();

		if (front)
		{
			//unsigned - numvertices, numfrontelements in boundary,
			// numfrontelements in elements, numfrontelements in rejected,
			// numfrontelements in crossing
			//size += comm->packSize(5, Communicator::UINT);
			size += comm->packSize(4, Communicator::UINT);

			//long - vertices->id, frontelements->id
			size += comm->packSize(numFrontVertices + numFrontFrontElements, Communicator::ULINT);

			if (!mesh)
			{
				//long - frontelements->vertices->id
				size += comm->packSize(dimension*numFrontFrontElements, Communicator::ULINT);

				//real - vertices->coords
				size += comm->packSize(dimension*numFrontVertices, Communicator::REAL);
			}
		}
	}*/

	//std::cout << "final pack size for task " << this->getId() << " = " << size << std::endl;

	return size;
}

UInt PMJMesh::MJMeshTask::packSize(const Parallel::Communicator *comm) const
{
	ULInt numMeshVertices, numMeshFrontElements, numMeshElements,
		numFrontVertices, numFrontFrontElements;

	numMeshVertices = numMeshFrontElements = numMeshElements =
	numFrontVertices = numFrontFrontElements = 0;

	MJMesh::Mesh *mesh = this->getMesh();

	if (mesh)
	{
		numMeshVertices = mesh->verticesSize();
		numMeshFrontElements = mesh->frontElementsSize();
		numMeshElements = mesh->size();
	}

	PMJMesh::Front *front = this->getFront();

	if (front)
	{
		numFrontVertices = static_cast<UInt>(front->verticesSize());
		numFrontFrontElements = front->size(); //numFrontFrontElements = front->size(0) + front->size(1) + front->size(2) + front->size(3);
	}

	return this->packSize(comm,
		numMeshVertices, numMeshFrontElements, numMeshElements,
		numFrontVertices, numFrontFrontElements);
}

void PMJMesh::MJMeshTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	//Data::VertexList &meshVertices, Data::FrontElementList &meshFels, Data::ElementList &meshEls,
	const Data::Mesh *mesh,
	//Data::VertexList &frontVertices, Data::FrontElementList &frontBoundary, Data::FrontElementList &frontElements,
	//Data::FrontElementList &frontRejected, Data::FrontElementList &frontCrossing) const
	const Data::Front *front) const
{
	//std::cout << comm->rank() << " start packing task " << this->getId() << std::endl;
	//std::cout << "starting position = " << position << std::endl;
	//Int inipos =  position;

	comm->pack(static_cast<UInt>(this->type), buffer, size, position);
	//std::cout << "type = " << this->type << std::endl;
	//std::cout << "position = " << position << std::endl;
	comm->pack(static_cast<UInt>(this->taskType), buffer, size, position);
	//std::cout << "taskType = " << this->taskType << std::endl;
	//std::cout << "position = " << position << std::endl;
	//std::cout << "actual pack size = " << position - inipos << std::endl;
	comm->pack(this->id, buffer, size, position);
	//std::cout << "id = " << this->id << std::endl;
	//std::cout << "position = " << position << std::endl;
	//std::cout << "actual pack size = " << position - inipos << std::endl;

	comm->pack((this->partition != NULL), buffer, size, position);
	//std::cout << "(this->partition != NULL) = " << std::boolalpha << (this->partition != NULL) << std::endl;
	//std::cout << "position = " << position << std::endl;
	comm->pack((this->builder != NULL), buffer, size, position);
	//std::cout << "(this->builder != NULL) = " << std::boolalpha << (this->builder != NULL) << std::endl;
	//std::cout << "position = " << position << std::endl;
	//std::cout << "actual pack size = " << position - inipos << std::endl;

	if ((this->partition) || (this->builder))
	{
		if (!this->builder)
		{
			//std::cout << "packing task with no builder" << std::endl;

			comm->pack(this->partition->packSize(comm), buffer, size, position);
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual pack size = " << position - inipos << std::endl;

			Parallel::Package p = this->partition->pack(comm);

			comm->pack(p.second, p.first, buffer, size, position);
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual pack size = " << position - inipos << std::endl;

			Transferable::free(p);
		}
		else
		{
			//std::cout << "packing task with builder" << std::endl;

			UInt status = this->builder->getStatus().getType();

			comm->pack(status, buffer, size, position);
			//std::cout << "status = " << status << std::endl;
			//std::cout << "position = " << position << std::endl;

			UInt dimension = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->getDimension();

			comm->pack(dimension, buffer, size, position);
			//std::cout << "dimension = " << dimension << std::endl;
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual pack size = " << position - inipos << std::endl;

			if (this->partition)
			{
				//std::cout << "packing " << 2*dimension << " reals" << std::endl;
				const Data::Box *box = static_cast<PMJMesh::Partition *>(this->partition)->getBox();

				for (UInt i = 0; i < dimension; i++)
				{
					comm->pack(box->getMin()->getCoord(i), buffer, size, position);
					//std::cout << "box->min[i] = " << box->getMin()->getCoord(i) << std::endl;
					//std::cout << "position = " << position << std::endl;
				}

				for (UInt i = 0; i < dimension; i++)
				{
					comm->pack(box->getMax()->getCoord(i), buffer, size, position);
					//std::cout << "box->max[i] = " << box->getMax()->getCoord(i) << std::endl;
					//std::cout << "position = " << position << std::endl;
				}

				comm->pack(this->partition->getLoad(), buffer, size, position);
				//std::cout << "load = " << this->partition->getLoad() << std::endl;

				//box.setBox(NULL, NULL);
				//std::cout << "actual pack size = " << position - inipos << std::endl;
			}

			//MJMesh::Mesh *mesh = this->getMesh();
			//PMJMesh::Front *front = this->getFront();

			comm->pack((mesh != NULL), buffer, size, position);
			//std::cout << "(mesh != NULL) = " << std::boolalpha << (mesh != NULL) << std::endl;
			//std::cout << "position = " << position << std::endl;
			comm->pack((front != NULL), buffer, size, position);
			//std::cout << "(front != NULL) = " << std::boolalpha << (front != NULL) << std::endl;
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual pack size = " << position - inipos << std::endl;

			if (mesh)
			{
				//comm->pack(static_cast<UInt>(meshVertices.size()), buffer, size, position);
				comm->pack(mesh->verticesSize(), buffer, size, position);
				//std::cout << "mesh->verticesSize() = " << mesh->verticesSize() << std::endl;
				//std::cout << "position = " << position << std::endl;

				//comm->pack(static_cast<UInt>(meshFels.size()), buffer, size, position);
				comm->pack(mesh->frontElementsSize(), buffer, size, position);
				//std::cout << "mesh->frontElementsSize() = " << mesh->frontElementsSize() << std::endl;
				//std::cout << "position = " << position << std::endl;

				//comm->pack(static_cast<UInt>(meshEls.size()), buffer, size, position);
				comm->pack(mesh->size(), buffer, size, position);
				//std::cout << "mesh->size() = " << mesh->size() << std::endl;
				//std::cout << "position = " << position << std::endl;

				//while (!meshVertices.empty())
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					//Data::Vertex *v = meshVertices.front();
					Data::Vertex *v = (*iter);

					//meshVertices.pop_front();

					//std::cout << "packing vertex " << v->text() << std::endl;

					comm->pack(v->getId(), buffer, size, position);
					//std::cout << "v->getId() = " << v->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					for (UInt i = 0; i < dimension; i++)
					{
						comm->pack(v->getPoint()->getCoord(i), buffer, size, position);
						//std::cout << "v->pt->coord[i] = " << v->getPoint()->getCoord(i) << std::endl;
						//std::cout << "position = " << position << std::endl;
					}
				}

				//while (!meshFels.empty())
				for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
					 iter != mesh->feEnd(); iter++)
				{
					//Data::FrontElement *fel = meshFels.front();
					Data::FrontElement *fel = (*iter);

					//std::cout << "packing fel " << fel->text() << std::endl;

					//meshFels.pop_front();

					comm->pack(fel->getId(), buffer, size, position);
					//std::cout << "fel->getId() = " << fel->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					for (UInt i = 0; i < dimension; i++)
					{
						//std::cout << "packing fel vertex id " << fel->getVertex(i)->getId() << std::endl;

						comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
						//std::cout << "fel->v[i]->id = " << fel->getVertex(i)->getId() << std::endl;
						//std::cout << "position = " << position << std::endl;
					}

					//Data::ElementList adj = static_cast<MJMesh::FrontElement *>(fel)->getAdjacency();
					Data::ElementList::iterator adjIt = static_cast<MJMesh::FrontElement *>(fel)->eBegin();

					//std::cout << "packing front element " << fel->getId() << " from task " << this->getId() << " with " << adj.size() << " adjacent elements:";

					for (UInt i = 0; i < 2; i++)
					{
						//LInt id = -1;
						ULInt id = 0;

						//if (!adj.empty())
						if (adjIt != static_cast<MJMesh::FrontElement *>(fel)->eEnd())
						{
							//id = adj.front()->getId();
							id = (*adjIt)->getId();

							//std::cout << " " << id;

							//adj.pop_front();
							adjIt++;
						}

						comm->pack(id, buffer, size, position);
						//std::cout << "packing fel->adj[i]->id = " << id << std::endl;
						//std::cout << "position = " << position << std::endl;
					}

					//std::cout << std::endl;
				}

				//while (!meshEls.empty())
				for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
					 iter != mesh->eEnd(); iter++)
				{
					//Data::Element *el = meshEls.front();
					Data::Element *el = (*iter);

					//meshEls.pop_front();

					comm->pack(el->getId(), buffer, size, position);
					//std::cout << "el->getId() = " << el->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					for (UInt i = 0; i < dimension + 1; i++)
					{
						comm->pack(el->getVertex(i)->getId(), buffer, size, position);
						//std::cout << "el->v[i]->id = " << el->getVertex(i)->getId() << std::endl;
						//std::cout << "position = " << position << std::endl;
					}
				}

				//std::cout << "actual pack size = " << position - inipos << std::endl;
			}

			if (front)
			{
				comm->pack(front->isSorted(), buffer, size, position);

				//comm->pack(static_cast<UInt>(frontVertices.size()), buffer, size, position);
				comm->pack(front->verticesSize(), buffer, size, position);
				//std::cout << "front->verticesSize() = " << front->verticesSize() << std::endl;
				//std::cout << "position = " << position << std::endl;

				//std::cout << "packing " << frontVertices.size() << " ulints";
				//if (!mesh)
				//{
				//	std::cout << " and " << frontVertices.size()*dimension << " reals";
				//}
				//std::cout << std::endl;

				//while (!frontVertices.empty())
				for (Data::Front::ConstVertexIterator iter = front->vBegin();
					 iter != front->vEnd(); iter++)
				{
					//Data::Vertex *v = frontVertices.front();
					Data::Vertex *v = (*iter);

					//frontVertices.pop_front();

					comm->pack(v->getId(), buffer, size, position);
					//std::cout << "v->getId() = " << v->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						for (UInt i = 0; i < dimension; i++)
						{
							comm->pack(v->getPoint()->getCoord(i), buffer, size, position);
							//std::cout << "v->pt->coord[i] = " << v->getPoint()->getCoord(i) << std::endl;
							//std::cout << "position = " << position << std::endl;
						}
					}
				}

				for (UShort state = 0; state < 4; state++)
				{
					//std::cout << "packing state " << static_cast<UInt>(state) << std::endl;
					//std::cout << "packing front->size(state) = " << front->size(state) << std::endl;

					comm->pack(front->size(state), buffer, size, position);

					for (Data::Front::ConstFrontElementIterator iter = front->eBegin(state);
						 iter != front->eEnd(state); iter++)
					{
						Data::FrontElement *fel = (*iter);

						//std::cout << "packing fel = " << fel->getId() << std::endl;
						comm->pack(fel->getId(), buffer, size, position);

						if (!mesh)
						{
							for (UInt i = 0; i < dimension; i++)
							{
								//std::cout << "packing fel->vertex[" << i << "] = " << fel->getVertex(i)->getId() << std::endl;
								comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
							}
						}
					}
				}

				/*//comm->pack(static_cast<UInt>(frontBoundary.size()), buffer, size, position);
				comm->pack(front->size(0), buffer, size, position);
				//std::cout << "front->size(0) = " << front->size(0) << std::endl;
				//std::cout << "position = " << position << std::endl;

				//std::cout << "packing " << frontBoundary.size() << " ulints";
				//if (!mesh)
				//{
				//	std::cout << " and " << frontBoundary.size()*dimension << " ulints";
				//}
				//std::cout << std::endl;

				//while (!frontBoundary.empty())
				for (Data::Front::ConstFrontElementIterator iter = front->eBegin(0);
					 iter != front->eEnd(0); iter++)
				{
					//Data::FrontElement *fel = frontBoundary.front();
					Data::FrontElement *fel = (*iter);

					//frontBoundary.pop_front();

					comm->pack(fel->getId(), buffer, size, position);
					//std::cout << "fel->getId() = " << fel->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						for (UInt i = 0; i < dimension; i++)
						{
							comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
							//std::cout << "fel->v[i]->getId() = " << fel->getVertex(i)->getId() << std::endl;
							//std::cout << "position = " << position << std::endl;
						}
					}
				}

				comm->pack(front->size(1), buffer, size, position);
				//std::cout << "front->size(1) = " << front->size(1) << std::endl;
				//std::cout << "position = " << position << std::endl;

				//std::cout << "packing " << frontElements.size() << " ulints";
				//if (!mesh)
				//{
				//	std::cout << " and " << frontElements.size()*dimension << " ulints";
				//}
				//std::cout << std::endl;

				//while (!frontElements.empty())
				for (Data::Front::ConstFrontElementIterator iter = front->eBegin(1);
					 iter != front->eEnd(1); iter++)
				{
					//Data::FrontElement *fel = frontElements.front();
					Data::FrontElement *fel = (*iter);

					//frontElements.pop_front();

					comm->pack(fel->getId(), buffer, size, position);
					//std::cout << "fel->getId() = " << fel->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						for (UInt i = 0; i < dimension; i++)
						{
							comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
							//std::cout << "fel->v[i]->getId() = " << fel->getVertex(i)->getId() << std::endl;
							//std::cout << "position = " << position << std::endl;
						}
					}
				}

				//std::cout << "packing " << frontRejected.size() << " ulints";
				//if (!mesh)
				//{
				//	std::cout << " and " << frontRejected.size()*dimension << " ulints";
				//}
				//std::cout << std::endl;

				//comm->pack(static_cast<UInt>(frontRejected.size()), buffer, size, position);
				comm->pack(front->size(2), buffer, size, position);
				//std::cout << "front->size(2) = " << front->size(2) << std::endl;
				//std::cout << "position = " << position << std::endl;

				//while (!frontRejected.empty())
				for (Data::Front::ConstFrontElementIterator iter = front->eBegin(2);
					 iter != front->eEnd(2); iter++)
				{
					//Data::FrontElement *fel = frontRejected.front();
					Data::FrontElement *fel = (*iter);

					//frontRejected.pop_front();

					comm->pack(fel->getId(), buffer, size, position);
					//std::cout << "fel->getId() = " << fel->getId() << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						for (UInt i = 0; i < dimension; i++)
						{
							comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
							//std::cout << "fel->v[i]->getId() = " << fel->getVertex(i)->getId() << std::endl;
							//std::cout << "position = " << position << std::endl;
						}
					}
				}

				//comm->pack(static_cast<UInt>(frontCrossing.size()), buffer, size, position);
				comm->pack(front->size(3), buffer, size, position);
				//std::cout << "front->size(3) = " << front->size(3) << std::endl;

				//while (!frontCrossing.empty())
				for (Data::Front::ConstFrontElementIterator iter = front->eBegin(3);
					 iter != front->eEnd(3); iter++)
				{
					//Data::FrontElement *fel = frontCrossing.front();
					Data::FrontElement *fel = (*iter);

					//frontCrossing.pop_front();

					comm->pack(fel->getId(), buffer, size, position);
					//std::cout << "fel->getId() = " << fel->getId() << std::endl;

					if (!mesh)
					{
						for (UInt i = 0; i < dimension; i++)
						{
							comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
							//std::cout << "fel->v[i]->getId() = " << fel->getVertex(i)->getId() << std::endl;
						}
					}
				}

				//std::cout << "actual pack size = " << position - inipos << std::endl;*/
			}
		}
	}

	//std::cout << "final actual pack size for task " << this->getId() << " = " << position - inipos << std::endl;

	//std::cout << "finish packing task " << this->getId() << std::endl;
}

void PMJMesh::MJMeshTask::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	//Data::VertexList /*meshVertices, */frontVertices;
	//Data::FrontElementList /*meshFels, frontBoundary, frontElements, frontRejected, */frontCrossing;
	//Data::ElementList meshEls;

	MJMesh::Mesh *mesh = this->getMesh();
	PMJMesh::Front *front = this->getFront();

	/*if (mesh)
	{
		//meshVertices = mesh->getVertices();
		//meshFels = mesh->getFrontElements();
		//meshEls = mesh->getElements();
	}*/

	/*if (front)
	{
		//frontVertices = front->getVertices();
		//frontBoundary = front->getBoundary();
		//frontElements = front->getElements();
		//frontRejected = front->getRejected();
		//frontCrossing = front->getCrossing();
	}*/

	this->pack(comm, buffer, size, position,
		//meshVertices, meshFels, meshEls,
		mesh,
		//frontVertices, frontBoundary, frontElements, frontRejected, frontCrossing);
		front);
}

Parallel::Package PMJMesh::MJMeshTask::pack(const Parallel::Communicator *comm) const
{
	//Data::VertexList /*meshVertices, */frontVertices;
	//Data::FrontElementList /*meshFels, frontBoundary, frontElements, frontRejected, */frontCrossing;
	//Data::ElementList meshEls;

	MJMesh::Mesh *mesh = this->getMesh();
	PMJMesh::Front *front = this->getFront();

	/*if (mesh)
	{
		//meshVertices = mesh->getVertices();
		//meshFels = mesh->getFrontElements();
		//meshEls = mesh->getElements();
	}*/

	/*if (front)
	{
		//frontVertices = front->getVertices();
		//frontBoundary = front->getBoundary();
		//frontElements = front->getElements();
		//frontRejected = front->getRejected();
		//frontCrossing = front->getCrossing();
	}*/

	Int size = this->packSize(comm,
		//static_cast<UInt>(meshVertices.size()), static_cast<UInt>(meshFels.size()),
		mesh ? mesh->verticesSize() : 0, mesh ? mesh->frontElementsSize() : 0,
		//static_cast<UInt>(meshEls.size()), static_cast<UInt>(frontVertices.size()),
		mesh ? mesh->size() : 0, front ? front->verticesSize() : 0,
		//static_cast<UInt>(frontBoundary.size() + frontElements.size() + frontRejected.size() + frontCrossing.size()));
		front ? front->size(0) + front->size(1) + front->size(2) + front->size(3) : 0);
	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	this->pack(comm, buffer, size, position,
		//meshVertices, meshFels, meshEls,
		mesh,
		//frontVertices, frontBoundary, frontElements, frontRejected, frontCrossing);
		front);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::MJMeshTask::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	//std::cout << "==============================================================" << std::endl;
	//std::cout << std::endl << std::endl;
	//std::cout << "debug unpacking task" << std::endl;
	//std::cout << "starting position = " << position << std::endl;
	//std::cout << comm->rank() << " start unpacking task " << std::endl;
	//Int inipos =  position;

	Int size = p.first;
	char *buffer = p.second;

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
		//std::cout << "type = " << ignore << std::endl;
		//std::cout << "position = " << position << std::endl;
	}

	UInt taskType = this->taskType;

	comm->unpack(taskType, buffer, size, position);
	//std::cout << "position = " << position << std::endl;

	this->taskType = static_cast<PMJMesh::TaskType>(taskType);
	//std::cout << "taskType = " << this->taskType << std::endl;

	//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

	//std::cout << "debug task type = " << this->taskType << std::endl;

	comm->unpack(this->id, buffer, size, position);
	//std::cout << "id = " << this->id << std::endl;
	//std::cout << "position = " << position << std::endl;
	//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

	//std::cout << comm->rank() << " start unpacking task " << this->getId() << std::endl;

	//std::cout << "debug id = " << this->id << std::endl;

	bool partition = false, builder = false;

	comm->unpack(partition, buffer, size, position);
	//std::cout << "(partition != NULL) = " << std::boolalpha << partition << std::endl;
	//std::cout << "position = " << position << std::endl;

	//std::cout << "debug partition = " << partition << std::endl;

	comm->unpack(builder, buffer, size, position);
	//std::cout << "(builder != NULL) = " << std::boolalpha << builder << std::endl;
	//std::cout << "position = " << position << std::endl;
	//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

	//std::cout << "debug builder = " << builder << std::endl;

	this->status = this->starting;

	if (partition || builder)
	{
		this->status = this->ready;

		if (!builder)
		{
			//std::cout << "debug there is no builder" << std::endl;

			//Int packedSize = size - this->packSize(comm);
			Int packedSize = 0;
			comm->unpack(packedSize, buffer, size, position);
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

			char *packed = new char[packedSize];

			//std::cout << "debug unpacking partition" << std::endl;

			comm->unpack(packed, packedSize, buffer, size, position);
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

			Parallel::Package p2 = std::make_pair(packedSize, packed);

			//std::cout << "debug unpacking partition" << std::endl;

			this->partition = static_cast<PMJMesh::Partition *>(comm->unpack(p2, true));

			this->id = this->partition->getId();

			//std::cout << "debug partition unpacked" << std::endl;
		}
		else
		{
			//std::cout << "debug there is builder" << std::endl;

			UInt status = 0;

			comm->unpack(status, buffer, size, position);
			//std::cout << "status = " << status << std::endl;
			//std::cout << "position = " << position << std::endl;

			//std::cout << "debug there is builder" << std::endl;

			UInt dimension = 0;

			comm->unpack(dimension, buffer, size, position);
			//std::cout << "dimension = " << dimension << std::endl;
			//std::cout << "position = " << position << std::endl;
			//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

			//std::cout << "debug dimension = " << dimension << std::endl;

			//std::cout << "debug creating builder" << std::endl;

			//PMJMesh::WorkerMainDrive *drive = new PMJMesh::WorkerMainDrive(status);
			this->makeBuilder();

			PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder);

			//this->builder = drive;

			drive->setDimension(dimension);

			if (partition)
			{
				//std::cout << "debug there is partition" << std::endl;

				Data::Point *min = (dimension == 2) ?
					static_cast<Data::Point *>(new Data::Point2D()) :
					static_cast<Data::Point *>(new Data::Point3D());

				for (UInt i = 0; i < dimension; i++)
				{
					Real coord = 0.0;

					comm->unpack(coord, buffer, size, position);
					//std::cout << "box->min[i] = " << coord << std::endl;
					//std::cout << "position = " << position << std::endl;

					//std::cout << "debug min coord = " << coord << std::endl;

					min->setCoord(i, coord);
				}

				Data::Point *max = (dimension == 2) ?
					static_cast<Data::Point *>(new Data::Point2D()) :
					static_cast<Data::Point *>(new Data::Point3D());

				for (UInt i = 0; i < dimension; i++)
				{
					Real coord = 0.0;

					comm->unpack(coord, buffer, size, position);
					//std::cout << "box->max[i] = " << coord << std::endl;
					//std::cout << "position = " << position << std::endl;

					//std::cout << "debug max coord = " << coord << std::endl;

					max->setCoord(i, coord);
				}

				Data::Box *box = new Data::Box(min, max);

				//std::cout << "debug creating partition" << std::endl;

				Real load = 0.0;

				comm->unpack(load, buffer, size, position);
				//std::cout << "load = " << load << std::endl;

				this->partition = new PMJMesh::Partition(box, load);

				this->partition->setId(this->id);

				//std::cout << "debug seting partition id = " << this->partition->getId() << std::endl;
				//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;
			}

			bool mesh = false, front = false;

			comm->unpack(mesh, buffer, size, position);
			//std::cout << "(mesh != NULL) = " << std::boolalpha << mesh << std::endl;
			//std::cout << "position = " << position << std::endl;

			//std::cout << "debug mesh = " << mesh << std::endl;

			comm->unpack(front, buffer, size, position);
			//std::cout << "(front != NULL) = " << std::boolalpha << front << std::endl;
			//std::cout << "position = " << position << std::endl;

			//std::cout << "debug front = " << front << std::endl;
			//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

#if USE_C__11
			Data::VertexHashMap vMap;
			Data::FrontElementHashMap felMap;
#else
			Data::VertexMap vMap;
			Data::FrontElementMap felMap;
#endif //#if USE_C__11

			if (mesh || front)
			{
				this->status = this->finished;
			}

			if (mesh)
			{
				//std::cout << "debug there is mesh" << std::endl;

				MJMesh::Mesh *mesh = new MJMesh::Mesh();

				drive->setMesh(mesh);

				ULInt numVertices = 0;
				ULInt numFels = 0;
				ULInt numEls = 0;

				comm->unpack(numVertices, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numvertices = " << numVertices << std::endl;

				comm->unpack(numFels, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numFels = " << numFels << std::endl;

				comm->unpack(numEls, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numEls = " << numEls << std::endl;

#if USE_C__11
				vMap.reserve(numVertices);
				felMap.reserve(numFels);

				FEListHashMap mapAdjEls;

				mapAdjEls.reserve(numEls);
#else
				FEListMap mapAdjEls;
#endif //#if USE_C__11

				for (ULInt i = 0; i < numVertices; i++)
				{
					Data::Point *p = (dimension == 2) ?
						static_cast<Data::Point *>(new Data::Point2D()) :
						static_cast<Data::Point *>(new Data::Point3D());
					MJMesh::Vertex *v = new MJMesh::Vertex(p);

					ULInt id = 0;

					comm->unpack(id, buffer, size, position);
					//std::cout << "position = " << position << std::endl;

					//std::cout << "debug unpacking vertex id = " << id << std::endl;

					v->setId(id);

					for (UInt i = 0; i < dimension; i++)
					{
						Real coord = 0.0;

						comm->unpack(coord, buffer, size, position);
						//std::cout << "position = " << position << std::endl;

						//std::cout << "debug unpacking vertex id = " << id << ", coord = " << coord << std::endl;

						p->setCoord(i, coord);
					}

					vMap[id] = v;

					mesh->add(v);
				}

				for (ULInt i = 0; i < numFels; i++)
				{
					ULInt id = 0, idv[dimension], idAdj[2];

					comm->unpack(id, buffer, size, position);
					//std::cout << "position = " << position << std::endl;

					//std::cout << "debug unpacking fel id = " << id << std::endl;

					for (UInt j = 0; j < dimension; j++)
					{
						comm->unpack(idv[j], buffer, size, position);
						//std::cout << "position = " << position << std::endl;

						//std::cout << "debug unpacking fel id = " << id << ", vertex id = " << idv[j] << std::endl;
					}

					MJMesh::FrontElement *fel = (dimension == 2) ?
						static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
							static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
						static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
							static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
							static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

					felMap[id] = fel;

					mesh->add(fel);

					//std::cout << "debug unpacking fel " << id << std::endl;

					comm->unpack(idAdj[0], buffer, size, position);
					//std::cout << "position = " << position << std::endl;
					comm->unpack(idAdj[1], buffer, size, position);
					//std::cout << "position = " << position << std::endl;

					//std::cout << "debug unpacking fel->adj " << idAdj[0] << ", " << idAdj[1] << std::endl;

					//if (idAdj[0] != -1)
					if (idAdj[0] != 0)
					{
						mapAdjEls[idAdj[0]].push_back(fel);

						//if (idAdj[1] != -1)
						if (idAdj[1] != 0)
						{
							mapAdjEls[idAdj[1]].push_back(fel);
						}
					}

					//debug
					/*int num = 0;

					if (idAdj[0] != 0)
					{
						num++;
					}

					if (idAdj[1] != 0)
					{
						num++;
					}

					std::cout << "unpacking front element " << fel->getId() << " from task " << this->getId() << " with " << num << " adjacent elements:";

					for (int i = 0; i < num; i++)
					{
						std::cout << " " << idAdj[i];
					}

					std::cout << "\n";*/
					//enddebug

					//std::cout << "debug unpacked fel" << std::endl;
				}

				for (ULInt i = 0; i < numEls; i++)
				{
					ULInt id, idv[dimension + 1];

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug unpacking el id = " << id << std::endl;

					//std::cout << "position = " << position << std::endl;

					for (UInt j = 0; j < dimension + 1; j++)
					{
						//std::cout << "debug element vertex id" << std::endl;

						comm->unpack(idv[j], buffer, size, position);
						//std::cout << "debug unpacking el id = " << id << ", vertex id = " << idv[j] << std::endl;
						//std::cout << "position = " << position << std::endl;
					}

					MJMesh::Element *el = (dimension == 2) ?
						static_cast<MJMesh::Element *>(new MJMesh::Element2D(
							static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
							static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id)) :
						static_cast<MJMesh::Element *>(new MJMesh::Element3D(
							static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
							static_cast<MJMesh::Vertex *>(vMap[idv[2]]), static_cast<MJMesh::Vertex *>(vMap[idv[3]]), id));

					mesh->add(el);

					//Data::FrontElementList fels = mapAdjEls[id];
					Data::FrontElementList *fels = &mapAdjEls[id];

					//while (!fels.empty())
					for (Data::FrontElementList::iterator iter = fels->begin();
                         iter != fels->end(); iter++)
					{
						//static_cast<MJMesh::FrontElement *>(fels.front())->add(el);
						static_cast<MJMesh::FrontElement *>((*iter))->add(el);

						//fels.pop_front();
					}
				}

				//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;
			}

			if (front)
			{
				bool sorted = false;

				comm->unpack(sorted, buffer, size, position);

				//std::cout << "debug there is front" << std::endl;

				//std::cout << "debug creating front" << std::endl;

				PMJMesh::Front *front = new PMJMesh::Front(sorted);

				front->initialize();

				//drive->setFront(front);

				/*if (!mesh)
				{
					std::cout << "debug creating mesh" << std::endl;

					this->mesh = new MJMesh::Mesh();
				}*/

				ULInt numVertices = 0;

				comm->unpack(numVertices, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numvertices = " << numVertices << std::endl;

#if USE_C__11
				vMap.reserve(numVertices);
#endif //#if USE_C__11

				for (ULInt i = 0; i < numVertices; i++)
				{
					ULInt id = 0;

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug vertex id = " << id << std::endl;
					//std::cout << "position = " << position << std::endl;

					MJMesh::Vertex *v = NULL;

					if (mesh)
					{
						v = static_cast<MJMesh::Vertex *>(vMap[id]);
					}
					else
					{
						Data::Point *p = (dimension == 2) ?
							static_cast<Data::Point *>(new Data::Point2D()) :
							static_cast<Data::Point *>(new Data::Point3D());
						v = new MJMesh::Vertex(p);

						v->setId(id);

						for (UInt i = 0; i < dimension; i++)
						{
							Real coord = 0.0;

							comm->unpack(coord, buffer, size, position);
							//std::cout << "debug vertex coord = " << coord << std::endl;
							//std::cout << "position = " << position << std::endl;

							p->setCoord(i, coord);
						}

						vMap[id] = v;

						//this->mesh->add(v);
					}

					front->add(v);
				}

				for (UShort state = 0; state < 4; state++)
				{
					ULInt numEls = 0;

					//std::cout << "unpacking state " << static_cast<UInt>(state) << std::endl;
					comm->unpack(numEls, buffer, size, position);
					//std::cout << "unpacking front->size(state) = " << numEls << std::endl;

					for (ULInt i = 0; i < numEls; i++)
					{
						ULInt id = 0;

						comm->unpack(id, buffer, size, position);
						//std::cout << "unpacking fel = " << id << std::endl;

						MJMesh::FrontElement *fel = NULL;

						if (mesh)
						{
							fel = static_cast<MJMesh::FrontElement *>(felMap[id]);
						}
						else
						{
							ULInt idv[dimension];

							for (UInt j = 0; j < dimension; j++)
							{
								comm->unpack(idv[j], buffer, size, position);
								//std::cout << "unpacking fel->vertex[" << j << "] = " << idv[j] << std::endl;
							}

							fel = (dimension == 2) ?
								static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
									static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
								static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
									static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
									static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

							//felMap[id] = fel;
						}

						if (state == 0)
						{
							front->addBoundary(fel);
						}
						else if (state == 1)
						{
							front->addElement(fel);
						}
						else if (state == 2)
						{
							front->addRejected(fel);
						}
						else if (state == 3)
						{
							front->addCrossing(fel);
						}
					}
				}

				/*UInt numEls = 0;

				comm->unpack(numEls, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numboundary = " << numEls << std::endl;

				for (UInt i = 0; i < numEls; i++)
				{
					ULInt id = 0;

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug boundary fel id = " << id << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						ULInt idv[dimension];

						for (UInt j = 0; j < dimension; j++)
						{
							comm->unpack(idv[j], buffer, size, position);
							//std::cout << "debug unpacking fel id = " << id << ", vertex id = " << idv[j] << std::endl;
							//std::cout << "position = " << position << std::endl;
						}

						MJMesh::FrontElement *fel = (dimension == 2) ?
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
								static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

						felMap[id] = fel;

						//this->mesh->add(fel);
					}

					front->addBoundary(felMap[id]);
				}

				comm->unpack(numEls, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numelements = " << numEls << std::endl;

				for (UInt i = 0; i < numEls; i++)
				{
					ULInt id = 0;

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug element fel id = " << id << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						ULInt idv[dimension];

						for (UInt j = 0; j < dimension; j++)
						{
							comm->unpack(idv[j], buffer, size, position);
							//std::cout << "debug unpacking fel id = " << id << ", vertex id = " << idv[j] << std::endl;
							//std::cout << "position = " << position << std::endl;
						}

						MJMesh::FrontElement *fel = (dimension == 2) ?
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
								static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

						felMap[id] = fel;

						//this->mesh->add(fel);
					}

					front->addElement(felMap[id]);
				}

				comm->unpack(numEls, buffer, size, position);
				//std::cout << "position = " << position << std::endl;

				//std::cout << "debug numrejected = " << numEls << std::endl;

				for (UInt i = 0; i < numEls; i++)
				{
					ULInt id = 0;

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug rejected fel id = " << id << std::endl;
					//std::cout << "position = " << position << std::endl;

					if (!mesh)
					{
						ULInt idv[dimension];

						for (UInt j = 0; j < dimension; j++)
						{
							comm->unpack(idv[j], buffer, size, position);
							//std::cout << "debug unpacking fel id = " << id << ", vertex id = " << idv[j] << std::endl;
							//std::cout << "position = " << position << std::endl;
						}

						MJMesh::FrontElement *fel = (dimension == 2) ?
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]), id)) :
							static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
								static_cast<MJMesh::Vertex *>(vMap[idv[0]]), static_cast<MJMesh::Vertex *>(vMap[idv[1]]),
								static_cast<MJMesh::Vertex *>(vMap[idv[2]]), id));

						felMap[id] = fel;

						//this->mesh->add(fel);
					}

					front->addRejected(felMap[id]);
				}

				comm->unpack(numEls, buffer, size, position);

				//std::cout << "debug numcrossing = " << numEls << std::endl;

				for (UInt i = 0; i < numEls; i++)
				{
					ULInt id = 0;

					//std::cout << "debug rejected fel id" << std::endl;

					comm->unpack(id, buffer, size, position);
					//std::cout << "debug crossing fel id = " << id << std::endl;

					if (!mesh)
					{
						ULInt idv[dimension];

						for (UInt j = 0; j < dimension; j++)
						{
							comm->unpack(idv[j], buffer, size, position);
							//std::cout << "debug unpacking fel id = " << id << ", vertex id = " << idv[j] << std::endl;
						}

						MJMesh::FrontElement *fel = (dimension == 2) ?
							(MJMesh::FrontElement *)(new MJMesh::FrontElement2D(
								(MJMesh::Vertex *)vMap[idv[0]], (MJMesh::Vertex *)vMap[idv[1]], id)) :
							(MJMesh::FrontElement *)(new MJMesh::FrontElement3D(
								(MJMesh::Vertex *)vMap[idv[0]], (MJMesh::Vertex *)vMap[idv[1]],
								(MJMesh::Vertex *)vMap[idv[2]], id));

						felMap[id] = fel;

						//this->mesh->add(fel);
					}

					front->addCrossing(felMap[id]);
				}*/

				if (partition)
				{
					static_cast<PMJMesh::Partition *>(this->partition)->setSubfront(front);
				}
				else
				{
					drive->setFront(front);
				}

				//std::cout << "actual unpack size = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;
			}

			//static_cast<PMJMesh::GeometryFrontAdvancer *>(this->builder)->setMesh(this->mesh);
			//static_cast<PMJMesh::GeometryFrontAdvancer *>(this->builder)->setFront(this->front);
		}
	}

	//std::cout << "final actual unpack size for task " << this->getId() << " = " << position - inipos + comm->packSize(1, Parallel::Communicator::UINT) << std::endl;

	//std::cout << "finish unpacking task " << this->getId() << std::endl;

	//std::cout << std::endl << std::endl;
}

void PMJMesh::MJMeshTask::execute()
{
	//std::cout << "task " << this->getId() << ", status = " << this->status.getMessage() << std::endl;

	if ((this->status.getType() != this->ready.getType()) &&
		(this->status.getType() != this->running.getType()))
	{
		return;
	}

	this->status = this->running;

	if ((!this->builder) || (!this->partition))
	{
		this->status = finished;

		return;
	}

	dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->setBox(static_cast<PMJMesh::Partition *>(this->partition)->getBox());
	dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->setId(this->getId());
	dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->setFront(static_cast<PMJMesh::Partition *>(this->partition)->getSubfront());
	dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->setComm(this->comm);

	this->builder->execute();

	this->status = this->finished;
}

#if USE_THREAD_COLOR
void PMJMesh::MJMeshTask::colorizeMesh(UInt process) const
{
	if ((this->builder) && (dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->getMesh()))
	{
		dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->colorizeMesh(process);
	}
	else if (this->partition)
	{
		Data::Front *front = static_cast<PMJMesh::Partition *>(this->partition)->getSubfront();

		Real r, g, b;
		WorkerMainDrive::getMeshColor(r, g, b, static_cast<PMJMesh::Communicator *>(this->comm), process);

		if (front)
		{
			for (Data::Front::AllFrontElementIterator iter = front->allEBegin();
				 iter != front->allEEnd(); iter++)
			{
				(*iter)->setColor(r, g, b);
			}

			for (Data::Front::ConstVertexIterator iter = front->vBegin();
				 iter != front->vEnd(); iter++)
			{
				(*iter)->setColor(r, g, b);
			}
		}
	}
}
#endif //#if USE_THREAD_COLOR

#if USE_GUI
void PMJMesh::MJMeshTask::setR(Real r) const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->setR(r);
	}
}

Real PMJMesh::MJMeshTask::getR() const
{
	return (this->partition) ? static_cast<PMJMesh::Partition *>(this->partition)->getR() : 0.0;
}

void PMJMesh::MJMeshTask::setG(Real g) const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->setG(g);
	}
}

Real PMJMesh::MJMeshTask::getG() const
{
	return (this->partition) ? static_cast<PMJMesh::Partition *>(this->partition)->getG() : 0.0;
}

void PMJMesh::MJMeshTask::setB(Real b) const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->setB(b);
	}
}

Real PMJMesh::MJMeshTask::getB() const
{
	return (this->partition) ? static_cast<PMJMesh::Partition *>(this->partition)->getB() : 0.0;
}

void PMJMesh::MJMeshTask::highlight() const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->highlight();
	}
}

void PMJMesh::MJMeshTask::unhighlight() const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->unhighlight();
	}
}

bool PMJMesh::MJMeshTask::isHighlighted() const
{
	return (this->partition) ? static_cast<PMJMesh::Partition *>(this->partition)->isHighlighted() : false;
}

void PMJMesh::MJMeshTask::draw() const
{
	if (this->partition)
	{
		static_cast<PMJMesh::Partition *>(this->partition)->draw();
	}
}

void PMJMesh::MJMeshTask::drawGeneratedState() const
{
	if ((!this->partition) ||
		(this->generated.empty()))
	{
		return;
	}

	UInt dim = 0;

	if ((dim == 0) && (this->builder)) dim = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->builder)->getDimension();
	if ((dim == 0) && (this->partition) && (static_cast<PMJMesh::Partition *>(this->partition)->getBox())) dim = static_cast<PMJMesh::Partition *>(this->partition)->getBox()->dimension();
	if ((dim == 0) && (this->getFront())) dim = this->getFront()->dimension();
	if ((dim == 0) && (this->getMesh())) dim = this->getMesh()->dimension();

	if (dim == 0) return;

	if (dim == 2)
	{
		Data::Box *box = static_cast<PMJMesh::Partition *>(this->partition)->getBox();
		Data::Point2D bMin(box->getMin());
		Data::Point2D bMax(box->getMax());

		Real sizes[2] = {0.0, 0.0};

		sizes[0] = (bMax.getX() - bMin.getX())/3.0;
		sizes[1] = (bMax.getY() - bMin.getY())/3.0;

		bMin.setPosition(bMin.getX() + sizes[0], bMin.getY() + sizes[1]);
		bMax.setPosition(bMax.getX() - sizes[0], bMax.getY() - sizes[1]);

		sizes[0] = (bMax.getX() - bMin.getX())/3.0;
		sizes[1] = (bMax.getY() - bMin.getY())/3.0;

		for (UInt i = 0; i < 3; i++)
		{
			for (UInt j = 0; j < 3; j++)
			{
				Data::Point2D min(bMin), max(bMax);

				min.setX(min.getX() + i*sizes[0]);
				max.setX(max.getX() - (2 - i)*sizes[0]);

				min.setY(min.getY() + j*sizes[1]);
				max.setY(max.getY() - (2 - j)*sizes[1]);

				bool generated = false;

				if ((i == 0) && (j == 0)) generated = this->generated[7];
				if ((i == 0) && (j == 1)) generated = this->generated[3];
				if ((i == 0) && (j == 2)) generated = this->generated[6];
				if ((i == 1) && (j == 0)) generated = this->generated[4];
				if ((i == 1) && (j == 1)) generated = this->generated[0];
				if ((i == 1) && (j == 2)) generated = this->generated[2];
				if ((i == 2) && (j == 0)) generated = this->generated[8];
				if ((i == 2) && (j == 1)) generated = this->generated[1];
				if ((i == 2) && (j == 2)) generated = this->generated[5];

				Data::Box b(min, max);

				b.localScale(((i == 1) && (j == 1)) ? 1.25 : 0.75);

				if (generated)
				{
					b.setColor(1.0, 0.0, 0.0);
				}
				else
				{
					b.setColor(0.0, 1.0, 0.0);
				}

				b.highlight();

				b.draw();

				b.unhighlight();

				b.setColor(1.0, 1.0, 1.0);

				b.draw();
			}
		}
	}
	else if (dim == 3)
	{
		Data::Box *box = ((PMJMesh::Partition *)this->partition)->getBox();
		Data::Point3D bMin = *static_cast<Data::Point3D *>(box->getMin());
		Data::Point3D bMax = *static_cast<Data::Point3D *>(box->getMax());

		Real sizes[3] = {0.0, 0.0, 0.0};

		sizes[0] = (bMax.getX() - bMin.getX())/3.0;
		sizes[1] = (bMax.getY() - bMin.getY())/3.0;
		sizes[2] = (bMax.getZ() - bMin.getZ())/3.0;

		bMin.setPosition(bMin.getX() + sizes[0], bMin.getY() + sizes[1], bMin.getZ() + sizes[2]);
		bMax.setPosition(bMax.getX() - sizes[0], bMax.getY() - sizes[1], bMax.getZ() - sizes[2]);

		sizes[0] = (bMax.getX() - bMin.getX())/3.0;
		sizes[1] = (bMax.getY() - bMin.getY())/3.0;
		sizes[2] = (bMax.getZ() - bMin.getZ())/3.0;

		for (UInt i = 0; i < 3; i++)
		{
			for (UInt j = 0; j < 3; j++)
			{
				for (UInt k = 0; k < 3; k++)
				{
					Data::Point3D min(bMin), max(bMax);

					min.setX(min.getX() + i*sizes[0]);
					max.setX(max.getX() - (2 - i)*sizes[0]);

					min.setY(min.getY() + j*sizes[1]);
					max.setY(max.getY() - (2 - j)*sizes[1]);

					min.setZ(min.getZ() + k*sizes[2]);
					max.setZ(max.getZ() - (2 - k)*sizes[2]);

					bool generated = false;

					if ((i == 0) && (j == 0) && (k == 0)) generated = this->generated[23];
					if ((i == 0) && (j == 0) && (k == 1)) generated = this->generated[13];
					if ((i == 0) && (j == 0) && (k == 2)) generated = this->generated[21];
					if ((i == 0) && (j == 1) && (k == 0)) generated = this->generated[15];
					if ((i == 0) && (j == 1) && (k == 1)) generated = this->generated[4];
					if ((i == 0) && (j == 1) && (k == 2)) generated = this->generated[18];
					if ((i == 0) && (j == 2) && (k == 0)) generated = this->generated[26];
					if ((i == 0) && (j == 2) && (k == 1)) generated = this->generated[10];
					if ((i == 0) && (j == 2) && (k == 2)) generated = this->generated[20];
					if ((i == 1) && (j == 0) && (k == 0)) generated = this->generated[14];
					if ((i == 1) && (j == 0) && (k == 1)) generated = this->generated[5];
					if ((i == 1) && (j == 0) && (k == 2)) generated = this->generated[11];
					if ((i == 1) && (j == 1) && (k == 0)) generated = this->generated[6];
					if ((i == 1) && (j == 1) && (k == 1)) generated = this->generated[0];
					if ((i == 1) && (j == 1) && (k == 2)) generated = this->generated[3];
					if ((i == 1) && (j == 2) && (k == 0)) generated = this->generated[17];
					if ((i == 1) && (j == 2) && (k == 1)) generated = this->generated[2];
					if ((i == 1) && (j == 2) && (k == 2)) generated = this->generated[8];
					if ((i == 2) && (j == 0) && (k == 0)) generated = this->generated[24];
					if ((i == 2) && (j == 0) && (k == 1)) generated = this->generated[16];
					if ((i == 2) && (j == 0) && (k == 2)) generated = this->generated[22];
					if ((i == 2) && (j == 1) && (k == 0)) generated = this->generated[12];
					if ((i == 2) && (j == 1) && (k == 1)) generated = this->generated[1];
					if ((i == 2) && (j == 1) && (k == 2)) generated = this->generated[9];
					if ((i == 2) && (j == 2) && (k == 0)) generated = this->generated[25];
					if ((i == 2) && (j == 2) && (k == 1)) generated = this->generated[7];
					if ((i == 2) && (j == 2) && (k == 2)) generated = this->generated[19];

					Data::Box b(min, max);

					b.localScale(((i == 1) && (j == 1) && (k == 1)) ? 1.25 : 0.75);

					if (generated)
					{
						b.setColor(1.0, 0.0, 0.0);
					}
					else
					{
						b.setColor(0.0, 1.0, 0.0);
					}

					b.highlight();

					b.draw();

					b.unhighlight();

					b.setColor(1.0, 1.0, 1.0);

					b.draw();
				}
			}
		}
	}
}

std::string PMJMesh::MJMeshTask::text() const
{
	return (this->partition)? static_cast<PMJMesh::Partition *>(this->partition)->text() : "";
}
#endif //#if USE_GUI
