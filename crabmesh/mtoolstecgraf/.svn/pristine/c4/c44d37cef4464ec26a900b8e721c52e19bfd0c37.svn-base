#include "PMJMesh/FullFrontUpdater.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "Data/GeometricShape.h"
#include "Data/Edge.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/Partition.h"

//debug
//#include <mpi.h>
//endebug

PMJMesh::FullFrontUpdater::FullFrontUpdater(UInt dimension, bool shared,
	Data::Boundary *boundary, PMJMesh::Front *front, MJMesh::Mesh *mesh,
	const PMJMesh::TaskManager *taskManager, Parallel::Communicator *comm) :
	FrontUpdater(dimension, shared, boundary, front, mesh, taskManager, comm)
{

}

PMJMesh::FullFrontUpdater::~FullFrontUpdater()
{

}

//void PMJMesh::FullFrontUpdater::createVMap(Data::VertexList &vertices, Data::VertexMap &vMap) const
void PMJMesh::FullFrontUpdater::createVMap(const Data::Front *front, Data::VertexMap &vMap) const
{
	//while (!vertices.empty())
	for (Data::Front::ConstVertexIterator iter = front->vBegin();
		 iter != front->vEnd(); iter++)
	{
		//std::cout << "adding vertex " << vertices.front()->text() << " to vMap when creating vMap" << std::endl;

		//vMap[vertices.front()->getId()] = vertices.front();
		vMap[(*iter)->getId()] = (*iter);

		//vertices.pop_front();
	}
}

//void PMJMesh::FullFrontUpdater::createFelMap(Data::FrontElementList &fels, Data::FrontElementMap &felMap) const
void PMJMesh::FullFrontUpdater::createFelMap(const Data::Front *front, Data::FrontElementMap &felMap) const
{
	//while (!fels.empty())
	for (Data::Front::ConstAllFrontElementIterator iter = front->allEBegin(), end = front->allEEnd();
		 iter != end; iter++)
	{
		//std::cout << "adding fel " << fels.front()->text() << " to felMap when creating felMap" << std::endl;

		//felMap[fels.front()->getId()] = fels.front();
		felMap[(*iter)->getId()] = (*iter);

		//fels.pop_front();
	}
}

void PMJMesh::FullFrontUpdater::replace(MJMesh::Vertex *v, MJMesh::Vertex *original, Data::ElementList &els) const
{
	while (!els.empty())
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());

		els.pop_front();

		v->remove(e);
		original->add(e);

		e->replace(v, original);
	}
}

void PMJMesh::FullFrontUpdater::replace(MJMesh::Vertex *v, MJMesh::Vertex *original, const Data::FrontElementMap &felMap, Data::FrontElementList &fels, bool forceRemove) const
{
	while (!fels.empty())
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

		fels.pop_front();

		if (forceRemove)
		{
			v->remove(fel);
		}

		//if front element already existed
		if (felMap.find(fel->getId()) != felMap.end())
		{
			fel->replace(v, NULL);

			continue;
		}

		if (!forceRemove)
		{
			v->remove(fel);
		}

		original->add(fel);

		fel->replace(v, original);
	}
}

void PMJMesh::FullFrontUpdater::replace(MJMesh::Vertex *v, MJMesh::Vertex *original, const Data::FrontElementMap &felMap, bool forceRemove) const
{
#if USE_THREAD_COLOR
	original->setColor(v->getR(), v->getG(), v->getB());
#endif //#if USE_THREAD_COLOR

	//std::cout << "debug original = " << original->getId() << std::endl;

	Data::ElementList els = v->getAdjacentElements();

	//std::cout << "debug v->adjacentElements = " << els.size() << std::endl;

	this->replace(v, original, els);

	Data::FrontElementList fels = v->getAdjacentFrontElements();

	//std::cout << "debug v->adjacentFels = " << fels.size() << std::endl;

	this->replace(v, original, felMap, fels, forceRemove);
}

void PMJMesh::FullFrontUpdater::replace(MJMesh::FrontElement *fel, MJMesh::FrontElement *original) const
{
#if USE_THREAD_COLOR
	original->setColor(fel->getR(), fel->getG(), fel->getB());
#endif //#if USE_THREAD_COLOR

	//Data::ElementList els = fel->getAdjacency();

	//while (!els.empty())
	for (Data::ElementList::iterator iter = fel->eBegin();
         iter != fel->eEnd(); iter = fel->erase(iter))
	{
		//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
		MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

		//els.pop_front();

		//fel->remove(e);
		original->add(e);
	}

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

		if (v)
		{
			v->remove(fel);
		}
	}
}

//debug
//#include <mpi.h>
//endebug

void PMJMesh::FullFrontUpdater::clear(Data::VertexList &vertices/*, Data::Front *front*/) const
{
	while (!vertices.empty())
	{
		Data::Vertex *v = vertices.front();

		//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "now deleting vertex " << v->text() << " in FullFrontUpdater" << std::endl;
		/*if (front)
		{
			front->remove(v);
		}*/

		delete v;

		vertices.pop_front();
	}
}

void PMJMesh::FullFrontUpdater::clear(Data::FrontElementList &fels/*, Data::Front *front*/) const
{
	while (!fels.empty())
	{
		Data::FrontElement *fel = fels.front();

		//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "now deleting fel " << fel->text() << " in FullFrontUpdater" << std::endl;
		/*if (front)
		{
			front->remove(fel);
		}*/

		fel->clear();

		delete fel;

		fels.pop_front();
	}
}

void PMJMesh::FullFrontUpdater::add(Data::Mesh *mesh, MJMesh::Vertex *v, bool /*forceInner*/)
{
	mesh->add(v);
}

void PMJMesh::FullFrontUpdater::add(Data::Mesh *mesh, MJMesh::FrontElement *fel)
{
	mesh->add(fel);
}

//bool PMJMesh::FullFrontUpdater::updateMesh(Data::VertexList &vertices,
bool PMJMesh::FullFrontUpdater::updateMeshVertices(const Data::Mesh *mesh,
	Data::VertexList &vertices,
	const Data::VertexMap &vMap, const Data::FrontElementMap &felMap,
	bool forceInner, bool forceRemove)
{
	//std::cout << "debug updating mesh vertices" << std::endl;

	bool updated = false;

	//Data::VertexList deletedVertices;

	//while (!vertices.empty())
	for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter++)
	{
		//MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		//std::cout << "debug v = " << v->getId() << std::endl;

		//vertices.pop_front();

		Data::VertexMap::const_iterator it = vMap.find(v->getId());

		if (it != vMap.end())
		{
			//std::cout << "debug vertex existed" << std::endl;

			MJMesh::Vertex *original = static_cast<MJMesh::Vertex *>((*it).second);

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "vertex " << v->text() << " will be replaced by its original " << original->text() << " in FullFrontUpdater" << std::endl;

			this->replace(v, original, felMap, forceRemove);

			//deletedVertices.push_back(v);
			vertices.push_back(v);
		}
		else
		{
			//std::cout << "debug vertex did not exist" << std::endl;

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "vertex " << v->text() << " will be added to mesh" << " in FullFrontUpdater" << std::endl;

 			this->add(this->mesh, v, forceInner);

			//std::cout << "debug vertex added" << std::endl;

			updated = true;
		}
	}

	//vertices.swap(deletedVertices);

	//std::cout << "debug mesh vertices updated" << std::endl;

	return updated;
}

//bool PMJMesh::FullFrontUpdater::updateMesh(Data::FrontElementList &fels,
bool PMJMesh::FullFrontUpdater::updateMeshFrontElements(const Data::Mesh *mesh, Data::FrontElementList &fels,
	const Data::FrontElementMap &felMap)
{
	bool updated = false;

	//Data::FrontElementList deletedFels;

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		//fels.pop_front();

		Data::FrontElementMap::const_iterator it = felMap.find(fel->getId());

		if (it != felMap.end())
		{
			MJMesh::FrontElement *original = static_cast<MJMesh::FrontElement *>((*it).second);

			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " will be replaced by its original: " << original->text() << " in FullFrontUpdater" << std::endl;

			this->replace(fel, original);

			//deletedFels.push_back(fel);
			fels.push_back(fel);
		}
		else
		{
			//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "fel " << fel->text() << " will be added to mesh" << " in FullFrontUpdater" << std::endl;

			this->add(this->mesh, fel);

			updated = true;
		}
	}

	//fels.swap(deletedFels);

	return updated;
}

//bool PMJMesh::FullFrontUpdater::updateMesh(Data::ElementList &els)
bool PMJMesh::FullFrontUpdater::updateMeshElements(const Data::Mesh *mesh)
{
	//while (!els.empty())
	for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter++)
	{
		//Data::Element *e = els.front();

		//els.pop_front();
		Data::Element *e = (*iter);

		//if (MPI::COMM_WORLD.Get_rank() == 0) std::cout << "el " << e->text() << " will be added to mesh" << " in FullFrontUpdater" << std::endl;

		this->mesh->add(e);
	}

	//return updated;
	return true;
}

//bool PMJMesh::FullFrontUpdater::updateFront(Data::VertexList &vertices,
bool PMJMesh::FullFrontUpdater::updateFrontVertices(Data::Front *front,
	Data::VertexMap &vMap, Data::VertexList &deleted)
{
	//std::cout << "updating front vertices for task " << this->task->getId() << std::endl;
	//std::cout << "vertices.size = " << vertices.size() << ", vMap.size = " << vMap.size() << ", deleted.size = " << deleted.size() << std::endl;

	bool updated = false;

	//while (!vertices.empty())
	for (Data::Front::ConstVertexIterator iter = front->vBegin();
		 iter != front->vEnd(); iter++)
	{
		//MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		//std::cout << "vertex in front: " << v->text() << std::endl;

		//vertices.pop_front();

		Data::VertexMap::iterator it = vMap.find(v->getId());

		if (it == vMap.end())
		{
			//std::cout << "vertex " << v->text() << " is new and will be added to the front" << std::endl;

			this->front->add(v);

			updated = true;
		}
		else
		{
			//std::cout << "vertex " << v->getId() << " already existed: " << (*it).second->text() << std::endl;

			vMap.erase(it);
		}
	}

	this->clear(deleted/*, front*/);

	while (!vMap.empty())
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*vMap.begin()).second);

		vMap.erase(vMap.begin());

		//std::cout << "removing vertex " << v->text() << " from front" << std::endl;

		this->front->remove(v);
	}

	return updated;
}

//bool PMJMesh::FullFrontUpdater::updateFront(Data::FrontElementList &fels,
bool PMJMesh::FullFrontUpdater::updateFrontElements(Data::Front *front,
	Data::FrontElementMap &felMap, Data::FrontElementList &deleted)
{
	//std::cout << "updating front fels for task " << this->task->getId() << std::endl;
	//std::cout << "fels.size = " << fels.size() << ", felMap.size = " << felMap.size() << ", deleted.size = " << deleted.size() << std::endl;

	bool updated = false;

	//while (!fels.empty())
	for (Data::Front::AllFrontElementIterator iter = front->allEBegin(), end = front->allEEnd();
		 iter != end; iter++)
	{
		//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		//std::cout << "fel in front: " << fel->text() << std::endl;

		//fels.pop_front();

		Data::FrontElementMap::iterator it = felMap.find(fel->getId());

		if (it == felMap.end())
		{
			//std::cout << "fel " << fel->text() << " is new and will be added to the front" << std::endl;

			this->front->add(fel);

			updated = true;
		}
		else
		{
			//std::cout << "fel " << fel->getId() << " already existed: " << (*it).second->text() << std::endl;

			felMap.erase(it);
		}

		//std::cout << "this->front->size = " << this->front->size() << ", fels.size = " << fels.size() << std::endl;
	}

	//std::cout << "debug deleting duplicated fels" << std::endl;

	this->clear(deleted/*, front*/);

	//std::cout << "removing old elements from front" << std::endl;

	while (!felMap.empty())
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*felMap.begin()).second);

		felMap.erase(felMap.begin());

		//std::cout << "removing fel " << fel->text() << " from front" << std::endl;

		this->front->remove(fel);

		//std::cout << "this->front->size = " << this->front->size() << std::endl;
	}

	//std::cout << "debug front elements updated" << std::endl;

	return updated;
}

void PMJMesh::FullFrontUpdater::updateMeshShared()
{
	MJMesh::Mesh *mesh = this->task->getMesh();

	//std::cout << "debug mesh got " << std::endl;

	if (mesh)
	{
		//std::cout << "debug there is mesh = " << std::endl;

		//Data::VertexList vertices = mesh->getVertices();

		//std::cout << "debug mesh->vertices->size = " << vertices.size() << std::endl;

		//while (!vertices.empty())
		for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
			 iter != mesh->vEnd(); iter++)
		{
			//Data::Vertex *v = vertices.front();
			Data::Vertex *v = (*iter);

			//vertices.pop_front();

			if (this->frontVertices.find(v) == this->frontVertices.end())
			{
				this->mesh->add(v);
			}
		}

		//Data::FrontElementList fels = mesh->getFrontElements();

		//while (!fels.empty())
		for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
			 iter != mesh->feEnd(); iter++)
		{
			//Data::FrontElement *fel = fels.front();
			Data::FrontElement *fel = (*iter);

			//fels.pop_front();

			if (this->frontFels.find(fel) == this->frontFels.end())
			{
				this->mesh->add(fel);
			}
		}

		//Data::ElementList els = mesh->getElements();

		//std::cout << "debug mesh->els->size = " << els.size() << std::endl;

		//this->updateMesh(els);
		this->updateMeshElements(mesh);

		mesh->clear();

		dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder())->setMesh(NULL);

		delete mesh;
	}
}

bool PMJMesh::FullFrontUpdater::updateFrontShared()
{
	//debug
	//Real debugstart = Data::time();
	//endebug

	this->partition = static_cast<PMJMesh::Partition *>(this->task->getPartition());

	PMJMesh::Front *front = this->partition->getSubfront();

	//std::cout << "FullFrontUpdater, time for getting subfront = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//this is to deal with the tasks in the shared mode, because some of
	// them may not have front. UPDATE I think this test is not needed anymore.
	if (front)
	{
		this->updateMeshShared();

		//std::cout << "FullFrontUpdater, time for updating mesh = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		//std::cout << "debug getting front from task " << this->task->getId() << std::endl;

		front = this->task->getFront();

		//std::cout << "debug front got" << std::endl;

		if (front)
		{
			//std::cout << "debug there is front" << std::endl;

			PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

			Data::FrontElementList oldFront = drive->getOldFront();
			Data::FrontElementList newFront = drive->getNewFront();
			Data::FrontElementList commonFront = drive->getCommonFront();

			//std::cout << "FullFrontUpdater, time for getting old, new and common fronts = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			drive->setOldFront(Data::FrontElementList());
			drive->setNewFront(Data::FrontElementList());
			drive->setCommonFront(Data::FrontElementList());

			//std::cout << "FullFrontUpdater, time for setting old, new and common fronts = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			if ((!oldFront.empty()) || (!newFront.empty()))
			{
				this->updated = true;
			}

			Data::VertexSet commonFrontVertices, newVertices, oldVertices;

			while (!commonFront.empty())
			{
				Data::FrontElement *fel = commonFront.front();

				commonFront.pop_front();

				for (UInt i = 0; i < fel->numVertices(); i++)
				{
					commonFrontVertices.insert(fel->getVertex(i));
				}
			}

			//std::cout << "FullFrontUpdater, time for finding out commonFrontVertices = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			while (!oldFront.empty())
			{
				Data::FrontElement *fel = oldFront.front();

				oldFront.pop_front();

				this->front->remove(fel);

				for (UInt i = 0; i < fel->numVertices(); i++)
				{
					if (commonFrontVertices.find(fel->getVertex(i)) == commonFrontVertices.end())
					{
						oldVertices.insert(fel->getVertex(i));
					}
				}
			}

			//std::cout << "FullFrontUpdater, time for finding out oldVertices = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			while (!newFront.empty())
			{
				Data::FrontElement *fel = newFront.front();

				newFront.pop_front();

				this->front->add(fel);

				for (UInt i = 0; i < fel->numVertices(); i++)
				{
					if (commonFrontVertices.find(fel->getVertex(i)) == commonFrontVertices.end())
					{
						newVertices.insert(fel->getVertex(i));
					}
				}
			}

			//std::cout << "FullFrontUpdater, time for finding out newVertices = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			while (!oldVertices.empty())
			{
				Data::Vertex *v = *oldVertices.begin();

				oldVertices.erase(oldVertices.begin());

				this->front->remove(v);
			}

			//std::cout << "FullFrontUpdater, time for removing oldVertices = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			while (!newVertices.empty())
			{
				Data::Vertex *v = *newVertices.begin();

				newVertices.erase(newVertices.begin());

				this->front->add(v);
			}

			//std::cout << "FullFrontUpdater, time for adding newVertices = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			front->clear();

			//std::cout << "FullFrontUpdater, time for clearing front = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
		}
	}

	this->partition = NULL;

	return true;
}

bool PMJMesh::FullFrontUpdater::updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool forceInner)
{
	bool updated = false;

	MJMesh::Mesh *mesh = this->task->getMesh();

	//std::cout << "debug mesh got " << std::endl;

	Data::VertexList vertices;
	Data::FrontElementList fels;

	if (mesh)
	{
		//std::cout << "debug there is mesh = " << std::endl;

		//vertices = mesh->getVertices();

		//std::cout << "debug mesh->vertices->size = " << vertices.size() << std::endl;

		//this->updateMesh(vertices, vMap, felMap, forceInner);
		this->updateMeshVertices(mesh, vertices, vMap, felMap, forceInner);

		//fels = mesh->getFrontElements();

		//std::cout << "debug mesh->fels->size = " << fels.size() << std::endl;

		//this->updateMesh(fels, felMap);
		this->updateMeshFrontElements(mesh, fels, felMap);

		//Data::ElementList els = mesh->getElements();

		//std::cout << "debug mesh->els->size = " << els.size() << std::endl;

		//this->updateMesh(els);
		this->updateMeshElements(mesh);

		mesh->clear();
	}

	//std::cout << "debug getting front from task " << this->task->getId() << std::endl;

	PMJMesh::Front *front = this->task->getFront();

	//std::cout << "debug front got" << std::endl;

	if (front)
	{
		//std::cout << "debug there is front" << std::endl;

		//Data::VertexList fVertices = front->getVertices();

		//std::cout << "debug front->vertices->size = " << vertices.size() << std::endl;

		//this->updateFront(fVertices, vMap, vertices);
		this->updateFrontVertices(front, vMap, vertices);

		//Data::FrontElementList fFels = front->getAllElements();

		//std::cout << "debug front->fels->size = " << fels.size() << std::endl;

		//if (this->updateFront(fFels, felMap, fels))
		if (this->updateFrontElements(front, felMap, fels))
		{
			updated = true;
		}
	}

	//std::cout << "debug moving task " << this->task->getId() << std::endl;

	return updated;
}

bool PMJMesh::FullFrontUpdater::updateFrontDistributed()
{
	PMJMesh::Front *front = this->partition->getSubfront();

	//Data::VertexList vertices = front->getVertices();

	//std::cout << "debug numvertices = " << vertices.size() << std::endl;

	Data::VertexMap vMap;

	//this->createVMap(vertices, vMap);
	this->createVMap(front, vMap);

	//Data::FrontElementList fels = front->getAllElements();

	//std::cout << "debug numfels = " << fels.size() << std::endl;

	Data::FrontElementMap felMap;

	//this->createFelMap(fels, felMap);
	this->createFelMap(front, felMap);

	//std::cout << "debug clearing front " << std::endl;

	front->clear();

	return this->updateFrontDistributed(vMap, felMap);
}

#endif //#if (!USE_NEW_FRONT_UPDATER)
