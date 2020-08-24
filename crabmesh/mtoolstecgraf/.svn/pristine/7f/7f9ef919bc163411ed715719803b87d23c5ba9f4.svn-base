#include "PMJMesh/EmptyFrontUpdater.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/TaskManager.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Vertex.h"
#include "Parallel/Communicator.h"
#include "Parallel/StaticLoadBalancer.h"
#include "Performer/IdManager.h"
#include "Data/Boundary.h"
#include "Data/Edge.h"

//debug
//#include <mpi.h>
//endebug

PMJMesh::EmptyFrontUpdater::EmptyFrontUpdater(UInt dimension, UInt smoothingLayer,
	bool parallel, bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
	MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
	Parallel::Communicator *comm) :
	PartialFrontUpdater(dimension, smoothingLayer, parallel, shared, boundary,
		front, mesh, taskManager, comm)
{

}

PMJMesh::EmptyFrontUpdater::~EmptyFrontUpdater()
{

}

void PMJMesh::EmptyFrontUpdater::initialize()
{
	//PartialFrontUpdater::initialize();
	FullFrontUpdater::initialize();

	if ((this->shared) || (this->comm->isMaster()))
	{
		return;
	}

	this->tasks.push_back(static_cast<Parallel::Task *>(NULL));
}

void PMJMesh::EmptyFrontUpdater::finalize()
{
	//PartialFrontUpdater::finalize();
	FullFrontUpdater::finalize();

	if ((this->shared) || (this->comm->isMaster()))
	{
		return;
	}

	Parallel::TaskVector::iterator iter = this->tasks.begin();

	for (; iter != this->tasks.end() && (*iter) != NULL; iter++);

	if (iter == this->tasks.end())
	{
		return;
	}

	this->tasks.erase(iter);
}

//debug
//#include "PMJMesh/MainDrive.h"
//endebug

void PMJMesh::EmptyFrontUpdater::splitMesh()
{
	if (this->shared)
	{
		this->splitMeshShared();
	}
	else if (this->comm->isMaster())
	{
		//debug
		//std::cout << "testing mesh before splitting" << std::endl;
		//this->mesh->test((MJMesh::Boundary *)this->boundary, this->front);
		//endebug

		this->splitMeshMaster();

		//debug
		//std::cout << "testing mesh after splitting" << std::endl;
		//this->mesh->test((MJMesh::Boundary *)this->boundary, this->front);
		//endebug
	}
	else
	{
		this->splitMeshWorker();
	}
}

void PMJMesh::EmptyFrontUpdater::splitMeshShared()
{
	this->buildBoundaryMaps();

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

		MJMesh::Mesh *mesh = t->getMesh();

		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

		//Data::FrontElementList oldFront = drive->getOldFront();
		//Data::FrontElementList newFront = drive->getNewFront();
		//Data::FrontElementList commonFront = drive->getCommonFront();

		//this->splitMesh(oldFront, newFront, commonFront, mesh);
		this->splitMesh(drive->getOldFront(), drive->getNewFront(), drive->getCommonFront(), mesh);

		mesh->clear();

		delete t;

		tasks[i] = NULL;
	}

	this->tasks.clear();

	this->clearBoundaryMaps();
}

void PMJMesh::EmptyFrontUpdater::splitMeshWorker()
{
	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		this->task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

		Data::FrontElementList oldFront = drive->getOldFront();
		Data::FrontElementList newFront = drive->getNewFront();

		oldFront.splice(oldFront.end(), newFront);

		drive->setOldFront(oldFront);
		drive->setNewFront(newFront);

		PartialFrontUpdater::updateMeshWorker();
	}

	Parallel::LoadBalancer *lb = new Parallel::StaticLoadBalancer(this->comm);

	lb->setTasks(this->tasks);

	this->tasks.clear();

	//std::cout << "empty front updater, split mesh worker, sending to master, process = " << this->comm->rank() << std::endl;

	lb->sendToMaster();

	//std::cout << "empty front updater, split mesh worker, sent to master, process = " << this->comm->rank() << std::endl;

	delete lb;
}

void PMJMesh::EmptyFrontUpdater::add(Data::Mesh *mesh, MJMesh::Vertex *v, bool /*forceInner*/)
{
	bool inner = (this->boundaryVs.find(v) == this->boundaryVs.end());

	mesh->add(v, inner);

	if (!inner)
	{
		this->vMap[this->process][v->getId()] = v;

		//std::cout << "adding vertex " << v->text() << " to this->vMap" << std::endl;
	}
}

void PMJMesh::EmptyFrontUpdater::add(Data::Mesh *mesh, MJMesh::FrontElement *fel)
{
	bool inner = (this->boundaryFels.find(fel) == this->boundaryFels.end());

	if  ((!this->shared) &&
		 (this->comm->rank() == 0) &&
		 (fel->getId() >= this->idManager->getId(1)))
	{
		//debug
		//std::cout << "fel " << fel->text() << " has id greater than " << this->idManager->getId(1) << std::endl;
		//endebug

		this->idManager->setId(fel->getId() + 1);
	}

	mesh->add(fel);

	if (!inner)
	{
		this->felMap[this->process][fel->getId()] = fel;

		//std::cout << "adding fel " << fel->text() << " to this->felMap" << std::endl;
	}
}

void PMJMesh::EmptyFrontUpdater::findNewBoundary(MJMesh::Mesh *mesh,
	const Data::FrontElementMap &felMap,
	Data::FrontElementSet &boundaryFels, Data::VertexSet &boundaryVs)
{
	//Data::FrontElementList fels = mesh->getFrontElements();

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		//Data::FrontElement *fel = fels.front();
		Data::FrontElement *fel = (*iter);

		//fels.pop_front();

		//if (mesh->adjacency(fel).size() == 2)
		if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 2)
		{
			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " in inside" << std::endl;

			continue;
		}

		if (felMap.find(fel->getId()) != felMap.end())
		{
			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " already exists: " << (*felMap.find(fel->getId())).second->text() << std::endl;

			continue;
		}

		//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " is new in boundary" << std::endl;

		//std::cout << "adding fel " << fel->text() << " to the boundary set" << std::endl;

		boundaryFels.insert(fel);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			//if (fel->getVertex(i)->getId() > this->idBoundaryVertex)
			if (fel->getVertex(i)->getId() >= this->idBoundaryVertex)
			{
				//std::cout << "adding vertex " << fel->getVertex(i)->text() << " to the boundary set through fel " << fel->text() << std::endl;

				boundaryVs.insert(fel->getVertex(i));
			}
		}
	}
}

void PMJMesh::EmptyFrontUpdater::splitMeshMaster(const Data::VertexMap &vMap, const Data::FrontElementMap &felMap)
{
	MJMesh::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->task)->getMesh();

	this->boundaryVs.clear();
	this->boundaryFels.clear();

	this->findNewBoundary(mesh, felMap, this->boundaryFels, this->boundaryVs);

	//Data::VertexList vertices = mesh->getVertices();
	Data::VertexList vertices;

	//this->updateMesh(vertices, vMap, felMap, true, true);
	this->updateMeshVertices(mesh, vertices, vMap, felMap, true, true);

	//Data::FrontElementList fels = mesh->getFrontElements();
	Data::FrontElementList fels;

	//this->updateMesh(fels, felMap);
	this->updateMeshFrontElements(mesh, fels, felMap);

	//Data::ElementList els = mesh->getElements();

	//this->updateMesh(els);
	this->updateMeshElements(mesh);

	for (Data::VertexSet::iterator iter = boundaryVs.begin();
		 iter != boundaryVs.end(); iter++)
	{
		//std::cout << "adding vertex " << (*iter)->text() << " to the boundary" << std::endl;

		this->boundary->add((*iter));
	}

	for (Data::FrontElementSet::iterator iter = boundaryFels.begin();
		 iter != boundaryFels.end(); iter++)
	{
		//std::cout << "adding fel " << (*iter)->text() << " to the boundary" << std::endl;

		this->boundary->add((*iter));
	}

	mesh->clear();

	this->clear(vertices/*, NULL*/);

	this->clear(fels/*, NULL*/);
}

void PMJMesh::EmptyFrontUpdater::splitMeshMaster()
{
	Parallel::LoadBalancer *lb = new Parallel::StaticLoadBalancer(this->comm);

	//std::cout << "empty front updater, split mesh master, receiving from slaves" << std::endl;

	lb->receiveFromSlaves();

	//std::cout << "empty front updater, split mesh master, received from slaves" << std::endl;

	this->tasks = lb->getTasks();

	this->initialize();

	this->partition = NULL;

	ProcessVertexMap vMapBkp;
	ProcessFrontElementMap felMapBkp;

	vMapBkp.swap(this->vMap);
	felMapBkp.swap(this->felMap);

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		this->task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

#if USE_THREAD_COLOR
		dynamic_cast<WorkerMainDrive *>(this->task->getBuilder())->colorizeMesh(lb->getProcess(this->task));
#endif //#if USE_THREAD_COLOR

		this->process = lb->getProcess(this->task);

		//std::cout << "EmptyFrontUpdater::splitMeshMaster para task " << this->task->getId() << " do processo " << this->process << std::endl;

		this->splitMeshMaster(vMapBkp[this->process], felMapBkp[this->process]);

		delete this->task;
	}

	this->tasks.clear();

	this->task = NULL;

	Data::VertexSet vertices;
	Data::FrontElementSet fels;

	for (ProcessVertexMap::iterator iter = vMapBkp.begin();
		 iter != vMapBkp.end(); iter++)
	{
		for (Data::VertexMap::iterator iter2 = (*iter).second.begin();
			 iter2 != (*iter).second.end(); iter2++)
		{
			vertices.insert((*iter2).second);
		}

		(*iter).second.clear();
	}

	for (ProcessFrontElementMap::iterator iter = felMapBkp.begin();
		 iter != felMapBkp.end(); iter++)
	{
		for (Data::FrontElementMap::iterator iter2 = (*iter).second.begin();
			 iter2 != (*iter).second.end(); iter2++)
		{
			fels.insert((*iter2).second);
		}

		(*iter).second.clear();
	}

	vMapBkp.clear();
	felMapBkp.clear();

	for (Data::VertexSet::iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		if ((*iter)->getId() < this->idBoundaryVertex)
		{
			continue;
		}

		this->mesh->add((*iter));
	}

	for (Data::FrontElementSet::iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		if ((*iter)->getId() < this->idBoundaryFrontElement)
		{
			continue;
		}

		this->mesh->add((*iter));
	}

	this->finalize();

	delete lb;
}

void PMJMesh::EmptyFrontUpdater::updateMeshShared()
{
	MJMesh::Mesh *mesh = this->task->getMesh();

	//std::cout << "debug mesh got " << std::endl;

	if (mesh)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->taskManager->makeTask());

		t->setId(this->task->getId());

		t->setComm(this->comm);

		t->makeBuilder();

		PMJMesh::WorkerMainDrive *d = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

		d->setDimension(this->dimension);

		d->setMesh(this->task->getMesh());

		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

		d->setOldFront(drive->getOldFront());

		d->setNewFront(drive->getNewFront());

		d->setCommonFront(drive->getCommonFront());

		this->tasks.push_back(t);

		drive->setMesh(NULL);
	}
}

bool PMJMesh::EmptyFrontUpdater::updateFrontShared()
{
	return FullFrontUpdater::updateFrontShared();
}

bool PMJMesh::EmptyFrontUpdater::updateMeshWorker()
{
	//std::cout << "executando EmptyFrontUpdater::updateMeshWorker para task " << this->task->getId() << std::endl;

	MJMesh::Mesh *mesh = this->task->getMesh();

	PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->taskManager->makeTask());
	t->setComm(this->comm);
	t->makeBuilder();

	PMJMesh::WorkerMainDrive *d = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());
	d->setDimension(this->dimension);

	t->setId(this->task->getId());

	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

	d->setMesh(mesh);

	d->setOldFront(drive->getOldFront());
	d->setNewFront(drive->getNewFront());
	d->setCommonFront(drive->getCommonFront());

	this->tasks.push_back(t);

	drive->setMesh(NULL);

	return true;
}

bool PMJMesh::EmptyFrontUpdater::updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool /*forceInner*/)
{
	//std::cout << "from partition " << this->partition->getId() << ", from process " << this->process << std::endl;

	bool updated = false;

	PMJMesh::Front *front = this->task->getFront();

	//std::cout << "debug front got" << std::endl;

	if (front)
	{
		//std::cout << "debug there is front" << std::endl;

		//Data::VertexList fVertices = front->getVertices();

		//std::cout << "debug front->vertices->size = " << vertices.size() << std::endl;

		//this->updateFront(fVertices, vMap, felMap);
		this->updateFrontVertices(front, vMap, felMap);

		//Data::FrontElementList fFels = front->getAllElements();

		//std::cout << "debug front->fels->size = " << fels.size() << std::endl;

		//if (this->updateFront(fFels, felMap))
		if (this->updateFrontElements(front, felMap))
		{
			updated = true;
		}
	}

	//std::cout << "debug moving task " << this->task->getId() << std::endl;

	return updated;
}

//bool PMJMesh::EmptyFrontUpdater::updateFront(Data::VertexList &vertices, Data::VertexMap &vMap, const Data::FrontElementMap &felMap)
bool PMJMesh::EmptyFrontUpdater::updateFrontVertices(Data::Front *front, Data::VertexMap &vMap, const Data::FrontElementMap &felMap)
{
	bool updated = false;

	Data::VertexList deletedVertices;

	//while (!vertices.empty())
	for (Data::Front::ConstVertexIterator iter = front->vBegin();
		 iter != front->vEnd(); iter++)
	{
		//MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		//std::cout << "updating vertex v = " << v->getId() << std::endl;

		//vertices.pop_front();

		Data::VertexMap::iterator it = vMap.find(v->getId());

		//debug
		//ULInt id = v->getId();
		//endebug

		if (it != vMap.end())
		{
			//std::cout << "debug vertex existed" << std::endl;

			MJMesh::Vertex *original = static_cast<MJMesh::Vertex *>((*it).second);

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "vertex " << v->text() << " will be replaced by its original " << original->text() << " in EmptyFrontUpdater" << std::endl;

			vMap.erase(it);

			this->vMap[this->process][v->getId()] = original;

			//debug, o original eh false
			//this->replace(v, original, felMap, false);
			this->replace(v, original, felMap, true);
			//endebug

			deletedVertices.push_back(v);
		}
		else
		{
			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "vertex " << v->text() << " will be added to front" << " in EmptyFrontUpdater" << std::endl;

			this->vMap[this->process][v->getId()] = v;

			//std::cout << "debug vertex did not exist" << std::endl;

 			this->front->add(v);

			//std::cout << "debug vertex added" << std::endl;

			updated = true;
		}

		//std::cout << "vert, in process " << this->process << ", task " << this->task->getId() << ", mapping vertex " << id << " to " << this->vMap[this->process][id]->text() << std::endl;
	}

	this->clear(deletedVertices/*, front*/);

	while (!vMap.empty())
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*vMap.begin()).second);

		vMap.erase(vMap.begin());

		this->vMap[this->process][v->getId()] = v;

		//std::cout << "vMap, in process " << this->process << ", task " << this->task->getId() << ", mapping vertex " << v->getId() << " to " << v->text() << std::endl;

		this->front->remove(v);
	}

	return updated;
}

//bool PMJMesh::EmptyFrontUpdater::updateFront(Data::FrontElementList &fels, Data::FrontElementMap &felMap)
bool PMJMesh::EmptyFrontUpdater::updateFrontElements(Data::Front *front, Data::FrontElementMap &felMap)
{
	bool updated = false;

	Data::FrontElementList deletedFels;

	//while (!fels.empty())
	for (Data::Front::AllFrontElementIterator iter = front->allEBegin(), end = front->allEEnd();
		 iter != end; iter++)
	{
		//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		//fels.pop_front();

		//debug
		//ULInt id = fel->getId();
		//endebug

		Data::FrontElementMap::iterator it = felMap.find(fel->getId());

		if (it != felMap.end())
		{
			deletedFels.push_back(fel);

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " will (not) be replaced by its original: " << (*it).second->text() << " in EmptyFrontUpdater" << std::endl;

			this->felMap[this->process][fel->getId()] = (*it).second;

			felMap.erase(it);
		}
		else
		{
			this->felMap[this->process][fel->getId()] = fel;

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " will be added to front" << " in EmptyFrontUpdater" << std::endl;

			this->front->add(fel);

			updated = true;
		}

		//std::cout << "fels  , in process " << this->process << ", task " << this->task->getId() << ", mapping fel " << id << " to " << this->felMap[this->process][id]->text() << std::endl;
	}

	this->clear(deletedFels/*, front*/);

	//std::cout << "debug removing old elements from front" << std::endl;

	while (!felMap.empty())
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*felMap.begin()).second);

		felMap.erase(felMap.begin());

		this->felMap[this->process][fel->getId()] = fel;

		//std::cout << "felMap, in process " << this->process << ", task " << this->task->getId() << ", mapping fel " << fel->getId() << " to " << fel->text() << std::endl;

		this->front->remove(fel);
	}

	//std::cout << "debug front elements updated" << std::endl;

	return updated;
}
#endif //#if (!USE_NEW_FRONT_UPDATER)
