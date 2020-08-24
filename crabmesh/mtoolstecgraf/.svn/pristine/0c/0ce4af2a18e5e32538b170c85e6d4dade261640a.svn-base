#include "PMJMesh/MeshUpdater.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/MJMeshTask.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Mesh.h"

PMJMesh::MeshUpdater::MeshUpdater(UInt dimension, MJMesh::Mesh *mesh,
	Parallel::Communicator *comm, bool external) :
		PMJMesh::AbstractMeshUpdater::AbstractMeshUpdater(dimension, mesh, comm, external)
{

}

PMJMesh::MeshUpdater::~MeshUpdater()
{

}

MJMesh::Mesh *PMJMesh::MeshUpdater::getMesh()
{
	return static_cast<MJMesh::Mesh *>(this->mesh);
}

void PMJMesh::MeshUpdater::createMaps(const PMJMesh::Front *front,
#if USE_C__11
	Data::VertexHashMap &vMap, Data::FrontElementHashMap &felMap) const
#else
	Data::VertexMap &vMap, Data::FrontElementMap &felMap) const
#endif //#if USE_C__11
{
#if USE_C__11
	vMap.reserve(vMap.size() + front->verticesSize());
	felMap.reserve(felMap.size() + front->size());
#endif //#if USE_C__11

	for (UShort state = 0; state < 4; state++)
	{
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(state);
			 iter != front->eEnd(state); iter++)
		{
			felMap[(*iter)->getId()] = (*iter);
		}
	}

	for (Data::Front::ConstVertexIterator iter = front->vBegin();
		 iter != front->vEnd(); iter++)
	{
		vMap[(*iter)->getId()] = (*iter);
	}
}

void PMJMesh::MeshUpdater::merge(MJMesh::Mesh *mesh, PMJMesh::Front *update,
#if USE_C__11
	Data::VertexHashMap &vReplace, Data::FrontElementHashMap &felReplace)
#else
	Data::VertexMap &vReplace, Data::FrontElementMap &felReplace)
#endif //#if USE_C__11
{
	for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd();)
	{
		MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));

#if USE_C__11
		Data::FrontElementHashMap::iterator it = felReplace.find(temp->getId());
#else
		Data::FrontElementMap::iterator it = felReplace.find(temp->getId());
#endif //#if USE_C__11

		MJMesh::FrontElement *orig = NULL;

		if (it == felReplace.end())
		{
			this->getMesh()->add(temp);
		}
		else
		{
			orig = static_cast<MJMesh::FrontElement *>((*it).second);

			this->replace(temp, orig);

			if ((update) && (update->have(temp)))
			{
				update->remove(temp);

				update->add(orig);
			}
		}

		iter = mesh->erase(iter);

		if (orig)
		{
			delete temp;
		}
	}

	for (Data::Mesh::VertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd();)
	{
		MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));

#if USE_C__11
		Data::VertexHashMap::iterator it = vReplace.find(temp->getId());
#else
		Data::VertexMap::iterator it = vReplace.find(temp->getId());
#endif //#if USE_C__11

		MJMesh::Vertex *orig = NULL;

		if (it == vReplace.end())
		{
			this->getMesh()->add(temp);
		}
		else
		{
			orig = static_cast<MJMesh::Vertex *>((*it).second);

			this->replace(temp, orig, true);

			if ((update) && (update->have(temp)))
			{
				update->remove(temp);

				update->add(orig);
			}
		}

		iter = mesh->erase(iter);

		if (orig)
		{
			delete temp;
		}
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter = mesh->erase(iter))
	{
		this->getMesh()->add((*iter));
	}
}

void PMJMesh::MeshUpdater::initializeShared()
{

}

void PMJMesh::MeshUpdater::initializeMaster()
{

}

void PMJMesh::MeshUpdater::initializeWorker()
{

}

void PMJMesh::MeshUpdater::finalizeShared()
{

}

void PMJMesh::MeshUpdater::finalizeMaster()
{

}

void PMJMesh::MeshUpdater::finalizeWorker()
{

}

void PMJMesh::MeshUpdater::executeShared()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->masterTask->getBuilder());

	if (!drive)
	{
		return;
	}

	MJMesh::Mesh *mesh = this->masterTask->getMesh();

#if USE_GUI
	drive->setMesh(NULL);
#endif //#if USE_GUI

	PMJMesh::Front *oldFront = drive->getOldFront();
	PMJMesh::Front *commonFront = drive->getCommonFront();

	Data::FrontList except;
	except.push_back(oldFront);
	except.push_back(commonFront);

	this->getMesh()->merge(mesh, except);

#if USE_GUI
	drive->setMesh(mesh);
#endif //#if USE_GUI

	//drive->setMesh(NULL);

	//delete mesh;

	this->updated = true;
}

void PMJMesh::MeshUpdater::executeMaster()
{
	PMJMesh::WorkerMainDrive *workerDrive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->workerTask->getBuilder());

	if (!workerDrive)
	{
		return;
	}

	PMJMesh::Front *oldFront = dynamic_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront();
	PMJMesh::Front *newFront = dynamic_cast<PMJMesh::Partition *>(this->workerTask->getPartition())->getSubfront();

	MJMesh::Mesh *mesh = this->workerTask->getMesh();

#if USE_GUI
	this->getMesh()->freeze();
	mesh->freeze();
#endif //#if USE_GUI

#if USE_C__11
	Data::VertexHashMap vMap;
	Data::FrontElementHashMap felMap;
#else
	Data::VertexMap vMap;
	Data::FrontElementMap felMap;
#endif //#if USE_C__11

	if (oldFront)
	{
		this->createMaps(oldFront, vMap, felMap);
	}

	this->merge(mesh, newFront, vMap, felMap);

#if USE_GUI
	this->getMesh()->unfreeze();
	mesh->unfreeze();
#endif //#if USE_GUI

	this->updated = true;
}

void PMJMesh::MeshUpdater::executeWorker()
{

}

void PMJMesh::MeshUpdater::finalizeMeshShared()
{

}

void PMJMesh::MeshUpdater::finalizeMeshMaster()
{

}

void PMJMesh::MeshUpdater::finalizeMeshWorker()
{

}

#endif //#if USE_NEW_FRONT_UPDATER
