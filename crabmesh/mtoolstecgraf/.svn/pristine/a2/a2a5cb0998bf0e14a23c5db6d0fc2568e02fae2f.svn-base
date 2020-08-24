#include "PMJMesh/EmptyMeshUpdater.h"

#if USE_NEW_FRONT_UPDATER

#include "Performer/IdManager.h"
#include "Parallel/Communicator.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"

#if USE_THREAD_COLOR
#include "PMJMesh/Communicator.h"
#endif //#if USE_THREAD_COLOR

PMJMesh::EmptyMeshUpdater::EmptyMeshUpdater(const PMJMesh::Front *front,
	UInt smoothingLayer, MJMesh::Boundary *boundary, Performer::IdManager *idManager,
	UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool noBoundary, bool external) :
		PMJMesh::PartialMeshUpdater::PartialMeshUpdater(smoothingLayer, boundary,
			idManager, dimension, mesh, comm, noBoundary, external)
{
	this->accumFront = NULL;
	this->originalFront = NULL;
	this->numMeshesFirstTime = 0;
	this->deleteAccumFront = true;

	this->setFront(front);
}

PMJMesh::EmptyMeshUpdater::~EmptyMeshUpdater()
{
	if (this->accumFront)
	{
		delete this->accumFront;
	}

	if (this->originalFront)
	{
		delete this->originalFront;
	}
}

void PMJMesh::EmptyMeshUpdater::setFront(const PMJMesh::Front *front)
{
	this->front = front;
}

void PMJMesh::EmptyMeshUpdater::splitMesh()
{
	if ((!this->comm) || (!this->comm->isParallel()))
	{
		if ((!this->mesh) || (!this->front))
		{
			return;
		}

		this->splitMeshShared();

		return;
	}

	if (this->comm->isMaster())
	{
		if (!this->mesh)
		{
			return;
		}

		this->splitMeshMaster();

		return;
	}

	this->splitMeshWorker();
}

/*//debug
void PMJMesh::EmptyMeshUpdater::print() const
{
	std::cout << Data::trace() << std::endl;
	if ((this->front) && (this->originalFront)) std::cout << "this->front->size() = " << this->front->size() << std::endl;
	if ((this->front) && (this->originalFront)) std::cout << "this->front->vsize() = " << this->front->verticesSize() << std::endl;
	if (this->accumFront) std::cout << "this->accumFront->size() = " << this->accumFront->size() << std::endl;
	if (this->accumFront) std::cout << "this->accumFront->vsize() = " << this->accumFront->verticesSize() << std::endl;
	std::cout << "this->fronts.size() = " << this->fronts.size() << std::endl;
	if (this->originalFront) std::cout << "this->originalFront->size() = " << this->originalFront->size() << std::endl;
	if (this->originalFront) std::cout << "this->originalFront->vsize() = " << this->originalFront->verticesSize() << std::endl;
	std::cout << "this->numMeshesFirstTime = " << this->numMeshesFirstTime << std::endl;
	std::cout << "this->smoothingLayer = " << this->smoothingLayer << std::endl;
	if (this->innerMesh) std::cout << "this->innerMesh->size() = " << this->innerMesh->size() << std::endl;
	if (this->innerMesh) std::cout << "this->innerMesh->fesize() = " << this->innerMesh->frontElementsSize() << std::endl;
	if (this->innerMesh) std::cout << "this->innerMesh->vsize() = " << this->innerMesh->verticesSize() << std::endl;
	if (this->boundary) std::cout << "this->boundary->size() = " << this->boundary->size() << std::endl;
	if (this->boundary) std::cout << "this->boundary->vsize() = " << this->boundary->verticesSize() << std::endl;
	if (this->idManager) std::cout << "this->idManager->id(0) = " << this->idManager->getId(0) << std::endl;
	if (this->idManager) std::cout << "this->idManager->id(1) = " << this->idManager->getId(1) << std::endl;
	if (this->idManager) std::cout << "this->idManager->id(2) = " << this->idManager->getId(2) << std::endl;
	std::cout << "this->useInnerMesh = " << std::boolalpha << this->useInnerMesh << std::endl;
	std::cout << "this->meshes.size() = " << this->meshes.size() << std::endl;
	std::cout << "this->newBoundaryVertices.size() = " << this->newBoundaryVertices.size() << std::endl;
	std::cout << "this->newBoundaryFrontElements.size() = " << this->newBoundaryFrontElements.size() << std::endl;
	std::cout << "this->oldVertices.size() = " << this->oldVertices.size() << std::endl;
	std::cout << "this->oldFrontElements.size() = " << this->oldFrontElements.size() << std::endl;
	std::cout << "this->noBoundary = " << std::boolalpha << this->noBoundary << std::endl;
	std::cout << "this->firstTime = " << std::boolalpha << this->firstTime << std::endl;
	std::cout << "this->boundaryVertices.size() = " << this->boundaryVertices.size() << std::endl;
	std::cout << "this->sharedBoundaryVertices.size() = " << this->sharedBoundaryVertices.size() << std::endl;
	std::cout << "this->boundaryFrontElements.size() = " << this->boundaryFrontElements.size() << std::endl;
}
//endebug*/

void PMJMesh::EmptyMeshUpdater::initializeShared()
{
	if (!this->accumFront)
	{
		this->accumFront = new PMJMesh::Front(this->front->isSorted());

		if (this->noBoundary)
		{
			this->originalFront = new PMJMesh::Front(this->front->isSorted());

			this->originalFront->set(this->front);

			this->originalFront->moveCrossingToRejected();
			this->originalFront->moveRejectedToElements();
			this->originalFront->moveBoundaryToElements();
		}
		else
		{
			this->accumFront->set(this->front);

			this->accumFront->moveCrossingToRejected();
			this->accumFront->moveRejectedToElements();
			this->accumFront->moveBoundaryToElements();
		}
	}
}

void PMJMesh::EmptyMeshUpdater::initializeMaster()
{
	if (!this->accumFront)
	{
		this->accumFront = new PMJMesh::Front(this->front->isSorted());

		if (this->noBoundary)
		{
			this->originalFront = new PMJMesh::Front(this->front->isSorted());

			this->originalFront->set(this->front);

			this->originalFront->moveCrossingToRejected();
			this->originalFront->moveRejectedToElements();
			this->originalFront->moveBoundaryToElements();
		}
		else
		{
			this->accumFront->set(this->front);

			this->accumFront->moveCrossingToRejected();
			this->accumFront->moveRejectedToElements();
			this->accumFront->moveBoundaryToElements();
		}
	}

	//this->print();
}

void PMJMesh::EmptyMeshUpdater::initializeWorker()
{

}

void PMJMesh::EmptyMeshUpdater::finalizeShared()
{
	PMJMesh::PartialMeshUpdater::finalizeShared();
}

void PMJMesh::EmptyMeshUpdater::finalizeMaster()
{
	PMJMesh::PartialMeshUpdater::finalizeMaster();

	//this->print();
}

void PMJMesh::EmptyMeshUpdater::finalizeWorker()
{
	if ((this->noBoundary) && (this->firstTime))
	{
		this->numMeshesFirstTime = static_cast<UInt>(this->meshes.size());
	}

	PMJMesh::PartialMeshUpdater::finalizeWorker();
}

void PMJMesh::EmptyMeshUpdater::executeShared()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->masterTask->getBuilder());

	if (!drive)
	{
		return;
	}

	MJMesh::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->masterTask)->getMesh();

	PMJMesh::Front *oldFront = drive->getOldFront();

	if (!oldFront)
	{
		return;
	}

#if USE_GUI
	mesh->freeze();
#endif //#if USE_GUI

	//std::cout << "EmptyMeshUpdater, task " << this->masterTask->getId() << std::endl;

	for (UShort i = 0; i < 4; i++)
	{
		for (Data::Front::ConstFrontElementIterator iter = oldFront->eBegin(i);
			 iter != oldFront->eEnd(i); iter++)
		{
			mesh->remove((*iter), false);
		}
	}

	for (Data::Front::ConstVertexIterator iter = oldFront->vBegin();
		 iter != oldFront->vEnd(); iter++)
	{
		mesh->remove((*iter));

		//std::cout << "oldFront, removing vertex " << (*iter) << ", " << (*iter)->text() << " from sub-mesh" << std::endl;
	}

	PMJMesh::Front *commonFront = drive->getCommonFront();

	if (commonFront)
	{
		for (UShort i = 0; i < 4; i++)
		{
			for (Data::Front::ConstFrontElementIterator iter = commonFront->eBegin(i);
				 iter != commonFront->eEnd(i); iter++)
			{
				mesh->remove((*iter), false);
			}
		}

		for (Data::Front::ConstVertexIterator iter = commonFront->vBegin();
			 iter != commonFront->vEnd(); iter++)
		{
			mesh->remove((*iter));

			//std::cout << "commonFront, removing vertex " << (*iter) << ", " << (*iter)->text() << " from sub-mesh" << std::endl;
		}
	}

	PMJMesh::Front *newFront = drive->getNewFront();

	if (newFront)
	{
#if USE_GUI
		this->getMesh()->freeze();
		this->accumFront->freeze();
#endif //#if USE_GUI

		for (UShort i = 0; i < 4; i++)
		{
			for (Data::Front::ConstFrontElementIterator iter = newFront->eBegin(i);
				 iter != newFront->eEnd(i); iter++)
			{
				mesh->remove((*iter), false);

				this->getMesh()->add((*iter));
				this->accumFront->addElement((*iter));
			}
		}

		for (Data::Front::ConstVertexIterator iter = newFront->vBegin();
			 iter != newFront->vEnd(); iter++)
		{
			mesh->remove((*iter));

			if ((!commonFront->have((*iter))) && (!oldFront->have((*iter))))
			{
				this->getMesh()->add((*iter));
				this->accumFront->add((*iter));

				//std::cout << "newFront, adding vertex " << (*iter) << ", " << (*iter)->text() << " to super-mesh and to accumFront" << std::endl;
			}
			else if ((this->noBoundary) && (this->originalFront->have((*iter))) && (!this->accumFront->have((*iter))))
			{
				this->accumFront->add((*iter));
			}
		}

#if USE_GUI
		this->getMesh()->unfreeze();
		this->accumFront->unfreeze();
#endif //#if USE_GUI
	}

#if USE_GUI
	mesh->unfreeze();
#endif //#if USE_GUI

	if ((mesh->size() > 0) ||
		(mesh->frontElementsSize() > 0) ||
		(mesh->verticesSize() > 0))
	{
		this->meshes.push_back(mesh);

		drive->setMesh(NULL);
	}

	this->updated = true;
}

void PMJMesh::EmptyMeshUpdater::executeMaster()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->workerTask->getBuilder());

	if (!drive)
	{
		return;
	}

	PMJMesh::Front *newFront = dynamic_cast<PMJMesh::Partition *>(this->workerTask->getPartition())->getSubfront();

	if (!newFront)
	{
		return;
	}

#if USE_C__11
	Data::VertexHashMap vMap;
	Data::FrontElementHashMap felMap;
#else
	Data::VertexMap vMap;
	Data::FrontElementMap felMap;
#endif //#if USE_C__11

	PMJMesh::Front *oldFront = dynamic_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront();

	if (oldFront)
	{
		this->createMaps(oldFront, vMap, felMap);

		/*for (UShort state = 0; state < 4; state++)
		{
			for (Data::Front::ConstFrontElementIterator iter = oldFront->eBegin(state);
				 iter != oldFront->eEnd(state); iter++)
			{
				felMap[(*iter)->getId()] = (*iter);
			}
		}

		for (Data::Front::ConstVertexIterator iter = oldFront->vBegin();
			 iter != oldFront->vEnd(); iter++)
		{
			vMap[(*iter)->getId()] = (*iter);
		}*/
	}

#if USE_GUI
	this->getMesh()->freeze();
	this->accumFront->freeze();
	newFront->freeze();
#endif //#if USE_GUI

	for (UShort state = 0; state < 4; state++)
	{
		Data::FrontElementList replacementFels;

		for (Data::Front::ConstFrontElementIterator iter = newFront->eBegin(state);
			 iter != newFront->eEnd(state);)
		{
			MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));

#if USE_C__11
			Data::FrontElementHashMap::iterator it = felMap.find(temp->getId());
#else
			Data::FrontElementMap::iterator it = felMap.find(temp->getId());
#endif //#if USE_C__11

			/*if (temp->getId() == 1859)
			{
				std::cout << "fel " << temp->text() << " is being tested in Empty::executeMaster" << std::endl;
			}*/

			if (it == felMap.end())
			{
				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->text() << " is not in felMap" << std::endl;
					std::cout << "fel added to this->accumFront" << std::endl;
					std::cout << "fel added to this->mesh" << std::endl;
				}*/

				this->accumFront->addElement(temp);
				this->getMesh()->add(temp);

				iter++;
			}
			else
			{
				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->text() << " is in felMap as " << (*it).second->text() << std::endl;
					std::cout << "fel in this->accumFront = " << std::boolalpha << this->accumFront->have((*it).second) << std::endl;
					std::cout << "fel in this->originalFront = " << std::boolalpha << this->originalFront->have((*it).second) << std::endl;
				}*/

				MJMesh::FrontElement *orig = static_cast<MJMesh::FrontElement *>((*it).second);

				this->replace(temp, orig);

				/*if (temp->getId() == 1859)
				{
					std::cout << "original fel is now " << orig->text() << std::endl;
				}*/

				//std::cout << "mesh from task " << this->workerTask->getId() << " has fel " << temp->text() << " that was replaced by " << orig->text() << std::endl;

				iter = newFront->erase(iter, state);

				replacementFels.push_back(orig);

				delete temp;
			}
		}

		while (!replacementFels.empty())
		{
			if (state == 0)
			{
				newFront->addBoundary(replacementFels.front());
			}
			else if (state == 1)
			{
				newFront->addElement(replacementFels.front());
			}
			else if (state == 2)
			{
				newFront->addRejected(replacementFels.front());
			}
			else if (state == 3)
			{
				newFront->addCrossing(replacementFels.front());
			}

			replacementFels.pop_front();
		}
	}

	Data::VertexList replacementVertices;

	for (Data::Front::VertexIterator iter = newFront->vBegin();
		 iter != newFront->vEnd();)
	{
		MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));

		/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
		{
			std::cout << "vertex " << temp->text() << " is being tested in Empty::executeMaster" << std::endl;
		}*/

#if USE_C__11
		Data::VertexHashMap::iterator it = vMap.find(temp->getId());
#else
		Data::VertexMap::iterator it = vMap.find(temp->getId());
#endif //#if USE_C__11

		if (it == vMap.end())
		{
			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->text() << " is in not vMap" << std::endl;
				std::cout << "vertex added to this->accumFront" << std::endl;
				std::cout << "vertex added to this->mesh" << std::endl;
			}*/

			this->accumFront->add(temp);
			this->getMesh()->add(temp);

			iter++;
		}
		else
		{
			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->text() << " is in vMap as " << (*it).second->text() << std::endl;
				std::cout << "vertex in this->accumFront = " << std::boolalpha << this->accumFront->have((*it).second) << std::endl;
				std::cout << "vertex in this->originalFront = " << std::boolalpha << this->originalFront->have((*it).second) << std::endl;
			}*/

			MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>((*it).second);

			this->replace(temp, orig, true);

			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "original vertex is now " << orig->text() << std::endl;
			}*/

			//std::cout << "mesh from task " << this->workerTask->getId() << " has fel " << temp->text() << " that was replaced by " << orig->text() << std::endl;

			iter = newFront->erase(iter);

			replacementVertices.push_back(orig);

			delete temp;
		}
	}

	while (!replacementVertices.empty())
	{
		newFront->add(replacementVertices.front());

		replacementVertices.pop_front();
	}

#if USE_GUI
	this->getMesh()->unfreeze();
	this->accumFront->unfreeze();
	newFront->unfreeze();
#endif //#if USE_GUI

	this->updated = true;

	//this->print();
}

void PMJMesh::EmptyMeshUpdater::executeWorker()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->workerTask->getBuilder());

	if ((!drive) || (!drive->getNewFront()))
	{
		return;
	}

	PMJMesh::Front *oldFront = drive->getOldFront();
	drive->setOldFront(NULL);

	PMJMesh::Front *commonFront = drive->getCommonFront();
	drive->setCommonFront(NULL);

	PMJMesh::Front *newFront = new PMJMesh::Front(this->front->isSorted());
	newFront->set(drive->getNewFront());

	MJMesh::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->workerTask)->getMesh();
	drive->setMesh(NULL);

	this->meshes.push_back(mesh);

	this->fronts.push_back(oldFront);
	this->fronts.push_back(commonFront);
	this->fronts.push_back(newFront);

	this->updated = true;
}

void PMJMesh::EmptyMeshUpdater::finalizeMeshShared()
{
	PMJMesh::PartialMeshUpdater::finalizeMeshShared();

	while (!this->meshes.empty())
	{
		Data::Mesh *mesh = this->meshes.front();

		this->meshes.pop_front();

#if USE_GUI
		this->getMesh()->freeze();
#endif //#if USE_GUI

		this->getMesh()->merge(static_cast<MJMesh::Mesh *>(mesh));

#if USE_GUI
		this->getMesh()->unfreeze();
#endif //#if USE_GUI

		delete mesh;
	}

	/*if (this->noBoundary)
	{
		this->addToBoundary();
	}*/
}

void PMJMesh::EmptyMeshUpdater::finalizeMeshMaster()
{
	PartialMeshUpdater::finalizeMeshMaster();

	//this->print();
}

void PMJMesh::EmptyMeshUpdater::finalizeMeshWorker()
{
	PartialMeshUpdater::finalizeMeshWorker();
}

void PMJMesh::EmptyMeshUpdater::splitMeshShared()
{
	Data::VertexList vertices = this->accumFront->getVertices();

	/*//debug
	for (Data::VertexList::iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		std::cout << "vertex in accum front at " << (*iter) << std::endl;
		std::cout << "vertex in accum front " << (*iter)->text() << std::endl;
	}
	//endebug*/

	if (this->deleteAccumFront)
	{
		delete this->accumFront;

		this->accumFront = NULL;
	}

	MJMesh::Boundary sharedBoundary;

#if USE_GUI
	this->getMesh()->freeze();
	this->boundary->freeze();
#endif //#if USE_GUI

	this->getMesh()->splitTo(vertices, this->smoothingLayer, sharedBoundary);

	if (this->noBoundary)
	{
		this->firstTime = true;
	}

	this->replace(this->getMesh(), this->getMesh(), sharedBoundary, true);

	if (this->noBoundary)
	{
		this->firstTime = false;
	}

	sharedBoundary.clear();

#if USE_GUI
	this->getMesh()->unfreeze();
	this->boundary->unfreeze();
#endif //#if USE_GUI

	if (this->noBoundary)
	{
		this->removeFromBoundary(this->originalFront, this->getMesh(), this->front);

		delete this->originalFront;

		this->originalFront = NULL;

		this->adjustSharedVerticesAdjacencies();
	}
}

void PMJMesh::EmptyMeshUpdater::splitMeshMaster()
{
#if USE_C__11
	Data::VertexHashMap vMap;
	Data::FrontElementHashMap felMap;
#else
	Data::VertexMap vMap;
	Data::FrontElementMap felMap;
#endif //#if USE_C__11

	if (this->noBoundary)
	{
		this->createMaps(this->originalFront, vMap, felMap);
	}

	this->createMaps(this->accumFront, vMap, felMap);

	if (this->deleteAccumFront)
	{
		delete this->accumFront;

		this->accumFront = NULL;
	}

	Parallel::Transferable **workersMeshes = NULL;
	Int numProcesses = this->comm->numProcesses();
	Int sizes[numProcesses - 1];
	Int numMeshes = 0;

	workersMeshes = this->comm->gatherv(sizes);

	for (Int i = 0; i < numProcesses - 1; i++)
	{
		numMeshes += sizes[i];
	}

	Int processor = 1;
	Int meshOfProcessor = 0;

	for (Int j = 0; j < numMeshes; j++)
	{
#if USE_GUI
		this->getMesh()->freeze();
		this->boundary->freeze();
#endif //#if USE_GUI

		while (meshOfProcessor == sizes[processor - 1])
		{
			processor++;

			meshOfProcessor = 0;
		}

		meshOfProcessor++;

		Data::Mesh *mesh = static_cast<MeshTransferable *>(workersMeshes[j])->getMesh();

		if (!mesh)
		{
			//std::cout << "mesh from processor " << processor << " does not exist" << std::endl;

			continue;
		}

#if USE_THREAD_COLOR
		Real r, g, b;
		PMJMesh::WorkerMainDrive::getMeshColor(r, g, b, static_cast<PMJMesh::Communicator *>(this->comm), processor);

		for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
			 iter != mesh->feEnd(); iter++)
		{
			(*iter)->setColor(r, g, b);
		}

		for (Data::Mesh::VertexIterator iter = mesh->vBegin();
			 iter != mesh->vEnd(); iter++)
		{
			(*iter)->setColor(r, g, b);
		}
#endif //#if USE_THREAD_COLOR

		this->merge(static_cast<MJMesh::Mesh *>(mesh), NULL, vMap, felMap);

#if USE_GUI
		this->getMesh()->unfreeze();
		this->boundary->unfreeze();
#endif //#if USE_GUI

		delete mesh;

		delete workersMeshes[j];
	}

	delete [] workersMeshes;

	if (this->noBoundary)
	{
		this->removeFromBoundary(this->originalFront, this->getMesh(), this->front);

		delete this->originalFront;

		this->originalFront = NULL;

		this->adjustSharedVerticesAdjacencies();
	}

	//this->print();
}

void PMJMesh::EmptyMeshUpdater::splitMeshWorker()
{
	Parallel::Transferable **mt = new Parallel::Transferable*[this->meshes.size()];

	Int size = 0;

	if (this->noBoundary)
	{
		this->firstTime = true;
	}

	for (Data::MeshList::iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
		if ((this->noBoundary) && (static_cast<UInt>(size) == this->numMeshesFirstTime))
		{
			this->firstTime = false;
		}

		MJMesh::Mesh *mesh = static_cast<MJMesh::Mesh *>((*iter));

		PMJMesh::Front *oldFront = static_cast<PMJMesh::Front *>(this->fronts.front());
		this->fronts.pop_front();

		PMJMesh::Front *commonFront = static_cast<PMJMesh::Front *>(this->fronts.front());
		this->fronts.pop_front();

		PMJMesh::Front *newFront = static_cast<PMJMesh::Front *>(this->fronts.front());
		this->fronts.pop_front();

		MJMesh::Mesh *outside = new MJMesh::Mesh();

		this->split(oldFront, commonFront, newFront, mesh, outside, NULL, true, false, false);

		mt[size++] = new MeshTransferable(this->dimension, outside);

		if (oldFront)
		{
			delete oldFront;
		}

		if (commonFront)
		{
			delete commonFront;
		}

		if (newFront)
		{
			delete newFront;
		}
	}

	this->comm->gatherv(mt, size, this->comm->root());

	for (Int i = 0; i < size; i++)
	{
		delete static_cast<MeshTransferable *>(mt[i])->getMesh();

		delete mt[i];
	}

	delete [] mt;
}
#endif //#if USE_NEW_FRONT_UPDATER
