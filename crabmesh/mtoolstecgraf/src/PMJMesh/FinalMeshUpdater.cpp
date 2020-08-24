#include "PMJMesh/FinalMeshUpdater.h"

#if USE_NEW_FRONT_UPDATER

#include "Performer/IdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Communicator.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"

PMJMesh::FinalMeshUpdater::FinalMeshUpdater(const PMJMesh::Front *front,
	UInt smoothingLayer, MJMesh::Boundary *boundary, Performer::IdManager *idManager,
	UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm, bool external) :
		PMJMesh::EmptyMeshUpdater::EmptyMeshUpdater(front, smoothingLayer, boundary,
			idManager, dimension, mesh, comm, false, external)
{
	this->empty = false;
	this->backup = NULL;
	this->backupBoundary = NULL;

	this->maxBoundaryFelId = this->idManager->getId(1);
}

PMJMesh::FinalMeshUpdater::~FinalMeshUpdater()
{
	if (this->backup)
	{
		if (this->boundary)
		{
			for (Data::Mesh::VertexIterator iter = this->backup->vBegin(), end = this->backup->vEnd();
				 iter != end; ++iter)
			{
				if (this->backup->have((*iter)))
				{
					(*iter)->setPoint(NULL);
				}
			}
		}

		delete this->backup;
	}

	if (this->backupBoundary)
	{
		delete this->backupBoundary;
	}
}

void PMJMesh::FinalMeshUpdater::initializeShared()
{
	if ((this->external) && (!this->accumFront))
	{
		this->accumFront = new PMJMesh::Front(this->front->isSorted());
	}
}

void PMJMesh::FinalMeshUpdater::initializeMaster()
{
	if (!this->accumFront)
	{
		this->accumFront = new PMJMesh::Front(this->front->isSorted());

		this->accumFront->set(this->front);

		this->accumFront->moveCrossingToRejected();
		this->accumFront->moveRejectedToElements();
		//this->accumFront->moveBoundaryToElements();
	}
}

void PMJMesh::FinalMeshUpdater::initializeWorker()
{

}

void PMJMesh::FinalMeshUpdater::finalizeShared()
{

}

void PMJMesh::FinalMeshUpdater::finalizeMaster()
{

}

void PMJMesh::FinalMeshUpdater::finalizeWorker()
{

}

void PMJMesh::FinalMeshUpdater::executeShared()
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

		//std::cout << "oldFront, removing vertex " << (*iter) << ", " << (*iter)->text() << std::endl;
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

			//std::cout << "commonFront, removing vertex " << (*iter) << ", " << (*iter)->text() << std::endl;
		}
	}

	if (this->external)
	{
		PMJMesh::Front *newFront = drive->getNewFront();

		if (newFront)
		{
#if USE_GUI
			this->accumFront->freeze();
#endif //#if USE_GUI

			for (UShort i = 0; i < 4; i++)
			{
				for (Data::Front::ConstFrontElementIterator iter = newFront->eBegin(i);
					 iter != newFront->eEnd(i); iter++)
				{
					mesh->remove((*iter), false);

					this->accumFront->addElement((*iter));
				}
			}

#if USE_GUI
			this->accumFront->unfreeze();
#endif //#if USE_GUI
		}
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

void PMJMesh::FinalMeshUpdater::executeMaster()
{
	//same as PMJMesh::EmptyMeshUpdater::executeMaster(), but without adding
	//  the front elements and vertices to the mesh
	///TODO put void PMJMesh::FinalMeshUpdater::executeMaster() and
	///  PMJMesh::EmptyMeshUpdater::executeMaster() in a same
	///  function

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
	}

#if USE_GUI
	//this->getMesh()->freeze();
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

			if (it == felMap.end())
			{
				this->accumFront->addElement(temp);
				//this->getMesh()->add(temp);

				iter++;
			}
			else
			{
				MJMesh::FrontElement *orig = static_cast<MJMesh::FrontElement *>((*it).second);

				this->replace(temp, orig);

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

#if USE_C__11
		Data::VertexHashMap::iterator it = vMap.find(temp->getId());
#else
		Data::VertexMap::iterator it = vMap.find(temp->getId());
#endif //#if USE_C__11

		if (it == vMap.end())
		{
			this->accumFront->add(temp);
			//this->getMesh()->add(temp);

			iter++;
		}
		else
		{
			MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>((*it).second);

			this->replace(temp, orig, true);

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
	//this->getMesh()->unfreeze();
	this->accumFront->unfreeze();
	newFront->unfreeze();
#endif //#if USE_GUI

	this->updated = true;
}

void PMJMesh::FinalMeshUpdater::executeWorker()
{
	this->PMJMesh::EmptyMeshUpdater::executeWorker();
}

void PMJMesh::FinalMeshUpdater::finalizeMeshShared()
{
#if USE_GUI
	this->getMesh()->freeze();
	this->boundary->freeze();
#endif //#if USE_GUI

	if (this->external)
	{
		for (Data::Front::FrontElementIterator iter = this->accumFront->eBegin(1);
			 iter != this->accumFront->eEnd(1);)
		{
			Data::FrontElement *fel = (*iter);

			//std::cout << "fel in this->accumFront = " << fel->text() << std::endl;

			iter = this->accumFront->erase(iter, 1);

			//std::cout << "fel removed from this->accumFront" << std::endl;

			if (!this->empty)
			{
				this->backup->remove(fel);

				//std::cout << "fel removed from this->backup" << std::endl;
			}

			//std::cout << "fel deleted" << std::endl;

			delete fel;
		}
	}

	if (this->empty)
	{
		//if this->external, this->backup will be empty
		for (Data::Mesh::VertexIterator iter = this->backup->vBegin();
			 iter != this->backup->vEnd(); iter = this->backup->erase(iter))
		{
			Data::Vertex *v = (*iter);

			this->getMesh()->addInner(v);

			this->boundary->remove(v);
		}

		for (Data::Mesh::FrontElementIterator iter = this->backup->feBegin();
			 iter != this->backup->feEnd(); iter = this->backup->erase(iter))
		{
			Data::FrontElement *fel = (*iter);

			this->boundary->remove(fel);
		}
	}
	else
	{
		for (Data::Mesh::VertexIterator iter = this->backup->vBegin();
			 iter != this->backup->vEnd(); iter = this->backup->erase(iter))
		{
			Data::Vertex *v = (*iter);

			bool inner = this->backup->haveInner(v);

			this->getMesh()->add(v, inner);

			if ((!inner) && (!this->boundary->have(v)))
			{
				this->boundary->add(v);
			}
		}

		for (Data::Mesh::FrontElementIterator iter = this->backup->feBegin();
			 iter != this->backup->feEnd(); iter = this->backup->erase(iter))
		{
			Data::FrontElement *fel = (*iter);

			//std::cout << "fel in this->backup = " << fel->text() << std::endl;

			this->getMesh()->add(fel);

			if (((this->external) || (this->backupBoundary->have(fel))) &&
				(!this->boundary->have(fel)))
			{
				//std::cout << "fel added to this->boundary" << std::endl;

				this->boundary->add(fel);
			}

			//std::cout << "fel removed from this->backup" << std::endl;
		}

		if (this->backupBoundary)
		{
			this->backupBoundary->clear();

			delete this->backupBoundary;

			this->backupBoundary = NULL;
		}

		this->getMesh()->merge(this->backup);
	}

	delete this->backup;

	this->backup = NULL;

#if USE_GUI
	this->getMesh()->unfreeze();
	this->boundary->unfreeze();
#endif //#if USE_GUI

	PMJMesh::EmptyMeshUpdater::finalizeMeshShared();
}

void PMJMesh::FinalMeshUpdater::finalizeMeshMaster()
{
	///Temporarily -> same thing when this->empty = true and when this->empty = false
#if USE_GUI
	this->getMesh()->freeze();
#endif //#if USE_GUI

	for (UShort state = 1; state < 4; state++)
	{
		for (Data::Front::ConstFrontElementIterator iter = this->accumFront->eBegin(state), end = this->accumFront->eEnd(state);
			 iter != end; iter++)
		{
			this->getMesh()->add((*iter));
		}
	}

	for (Data::Front::ConstVertexIterator iter = this->accumFront->vBegin(), end = this->accumFront->vEnd();
		 iter != end; iter++)
	{
		this->getMesh()->add((*iter), true);
	}

#if USE_GUI
	this->getMesh()->unfreeze();
#endif //#if USE_GUI
	///Temporarily -> same thing when this->empty = true and when this->empty = false

	if (this->empty)
	{
		this->finalizeMeshShared();

		this->deleteAccumFront = true;

		this->PMJMesh::EmptyMeshUpdater::splitMeshMaster();

/*#if USE_GUI
		this->getMesh()->freeze();
#endif //#if USE_GUI

		for (UShort state = 1; state < 4; state++)
		{
			for (Data::Front::ConstFrontElementIterator iter = this->accumFront->eBegin(state), end = this->accumFront->eEnd(state);
				 iter != end; iter++)
			{
				this->getMesh()->add((*iter));
			}
		}

		for (Data::Front::ConstVertexIterator iter = this->accumFront->vBegin(), end = this->accumFront->vEnd();
			 iter != end; iter++)
		{
			this->getMesh()->add((*iter), true);
		}

#if USE_GUI
		this->getMesh()->unfreeze();
#endif //#if USE_GUI*/

		return;
	}

	///Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
	this->finalizeMeshShared();

	this->PMJMesh::EmptyMeshUpdater::finalizeMeshMaster();

	if (this->accumFront)
	{
		delete this->accumFront;

		this->accumFront = NULL;
	}

/*#if USE_GUI
	this->getMesh()->freeze();
#endif //#if USE_GUI

	for (UShort state = 1; state < 4; state++)
	{
		for (Data::Front::ConstFrontElementIterator iter = this->accumFront->eBegin(state), end = this->accumFront->eEnd(state);
			 iter != end; iter++)
		{
			//std::cout << "trying to add accumFront fel at " << (*iter) << std::endl;

			this->getMesh()->add((*iter));
		}
	}

	for (Data::Front::ConstVertexIterator iter = this->accumFront->vBegin(), end = this->accumFront->vEnd();
		 iter != end; iter++)
	{
		this->getMesh()->add((*iter), true);
	}

#if USE_GUI
	this->getMesh()->unfreeze();
#endif //#if USE_GUI*/
	///end Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
}

void PMJMesh::FinalMeshUpdater::finalizeMeshWorker()
{
	if (this->external)
	{
		for (Data::MeshList::iterator iter = this->meshes.begin();
			 iter != this->meshes.end(); iter++)
		{
			Data::Mesh *mesh = (*iter);

			for (Data::Mesh::FrontElementIterator iter2 = mesh->feBegin();
				 iter2 != mesh->feEnd();)
			{
				Data::FrontElement *fel = (*iter2);

				if (fel->getId() < this->maxBoundaryFelId)
				{
					++iter2;

					continue;
				}

				iter2 = mesh->erase(iter2);

				delete fel;
			}
		}
	}

	if (this->empty)
	{
		this->PMJMesh::EmptyMeshUpdater::finalizeMeshWorker();

		return;
	}

	///Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
	this->PMJMesh::EmptyMeshUpdater::finalizeMeshWorker();
	///end Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
}

void PMJMesh::FinalMeshUpdater::splitMeshShared()
{
#if USE_GUI
	this->getMesh()->freeze();
	this->boundary->freeze();
#endif //#if USE_GUI

	this->empty = this->front->isEmpty();

	this->backup = new MJMesh::Mesh();

	if (this->empty)
	{
		if (!this->external)
		{
			for (Data::Mesh::ConstFrontElementIterator iter = this->getMesh()->feBegin(), end = this->getMesh()->feEnd();
				 iter != end; iter++)
			{
				Data::FrontElement *fel = (*iter);

				if (this->boundary->have(fel))
				{
					continue;
				}

				Data::ElementList adjs = this->getMesh()->adjacency(fel);

				Data::BoolVector inMesh(adjs.size(), false);

				UInt i = 0;

				while (!adjs.empty())
				{
					Data::Element *e = adjs.front();

					adjs.pop_front();

					inMesh[i++] = this->getMesh()->have(e);
				}

				if (((inMesh.size() == 1) && (inMesh[0])) ||
					((inMesh[0]) && (!inMesh[1])) ||
					((inMesh[1]) && (!inMesh[0])))
				{
					this->boundary->add(fel);

					this->backup->add(fel);

					for (UInt i = 0; i < fel->numVertices(); i++)
					{
						Data::Vertex *v = fel->getVertex(i);

						if (this->boundary->have(v))
						{
							continue;
						}

						this->boundary->add(v);

						this->getMesh()->removeInner(v);

						this->backup->add(v);;
					}
				}
			}
		}
	}
	else
	{
		Data::VertexList vertices = this->front->getVertices();

		if (!this->external)
		{
			this->backupBoundary = new MJMesh::Boundary();
		}

		MJMesh::Boundary sharedBoundary;

		this->backup->swap(this->getMesh());

		//this->boundary->clear();

		this->getMesh()->splitTo(vertices, this->smoothingLayer, sharedBoundary, this->boundary);

		if (this->accumFront)
		{
#if USE_GUI
			this->accumFront->freeze();
#endif //#if USE_GUI

			for (UShort i = 0; i < 4; i++)
			{
				for (Data::Front::FrontElementIterator iter = this->front->eBegin(i);
					 iter != this->front->eEnd(i); iter++)
				{
					Data::FrontElement *fel = (*iter);

					//std::cout << "fel in this->front = " << fel->text() << std::endl;

					if (this->accumFront->have(fel))
					{
						this->accumFront->remove(fel);

						//std::cout << "fel removed from this->accumFront" << std::endl;
					}
				}
			}

			for (Data::Front::ConstVertexIterator iter = this->front->vBegin();
				 iter != this->front->vEnd(); iter++)
			{
				Data::Vertex *v = (*iter);

				if (this->accumFront->have(v))
				{
					this->accumFront->remove(v);
				}
			}
		}

		if (this->external)
		{
			for (Data::Boundary::FrontElementIterator iter = sharedBoundary.eBegin();
				 iter != sharedBoundary.eEnd();)
			{
				Data::FrontElement *fel = (*iter);

				//std::cout << "fel in sharedBoundary = " << fel->text() << std::endl;

				if (this->boundary->have(fel))
				{
					//std::cout << "fel in this->boundary" << std::endl;

					iter++;
				}
				else
				{
					iter = sharedBoundary.erase(iter);

					//std::cout << "fel removed from sharedBoundary" << std::endl;

					this->accumFront->remove(fel);

					//std::cout << "fel removed from this->accumFront" << std::endl;

					//std::cout << "fel deleted" << std::endl;

					delete fel;
				}
			}

			for (Data::Mesh::FrontElementIterator iter = this->getMesh()->feBegin();
				 iter != this->getMesh()->feEnd();)
			{
				Data::FrontElement *fel = (*iter);

				//std::cout << "fel in this->getMesh() = " << fel->text() << std::endl;

				if ((this->boundary->have(fel)) || (this->front->have(fel)))
				{
					/*if (this->boundary->have(fel))
					{
						std::cout << "fel in this->boundary" << std::endl;
					}

					if (this->front->have(fel))
					{
						std::cout << "fel in this->front" << std::endl;
					}*/

					iter++;
				}
				else
				{
					iter = this->getMesh()->erase(iter);

					//std::cout << "fel removed from this->getMesh()" << std::endl;

					this->accumFront->remove(fel);

					//std::cout << "fel removed from this->accumFront" << std::endl;

					//std::cout << "fel deleted" << std::endl;

					delete fel;
				}
			}
		}
		else
		{
			for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
				 iter != this->boundary->eEnd(); iter = this->boundary->erase(iter))
			{
				this->backupBoundary->add((*iter));
			}
		}

#if USE_GUI
		if (this->accumFront)
		{
			this->accumFront->unfreeze();
		}
#endif //#if USE_GUI

		this->boundary->clear();

		this->replace(this->getMesh(), this->getMesh(), sharedBoundary, true, this->front);

		sharedBoundary.clear();
	}

#if USE_GUI
	this->getMesh()->unfreeze();
	this->boundary->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh::FinalMeshUpdater::splitMeshMaster()
{
	Int size = this->front->isEmpty() ? 0 : static_cast<Int>(this->front->verticesSize());

	this->comm->broadcast(size, this->comm->root());

	if (size == 0)
	{
		this->splitMeshShared();

		return;
	}

	///Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
	/*ULInt *ids = new ULInt[size];

	UInt i = 0;

	for (Data::Front::ConstVertexIterator iter = this->front->vBegin();
		 iter != this->front->vEnd(); iter++)
	{
		ids[i++] = (*iter)->getId();
	}

	this->meshes.push_back(this->getMesh());
	this->fronts.push_back(this->accumFront);

	MJMesh::Mesh **meshes = this->splitMeshDistributed(ids, size);

	this->meshes.pop_back();
	this->fronts.pop_back();

	meshes[0]->clear();

	delete meshes[0];

	delete [] meshes;

	this->PMJMesh::EmptyMeshUpdater::splitMeshMaster();

	this->splitMeshShared();*/

	this->deleteAccumFront = false;

	this->EmptyMeshUpdater::splitMeshMaster();

	this->splitMeshShared();
	///end Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
}

void PMJMesh::FinalMeshUpdater::splitMeshWorker()
{
	Int size = 0;

	this->comm->broadcast(size, this->comm->root());

	this->empty = (size == 0);

	if (this->empty)
	{
		return;
	}

	///Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
	/*ULInt *ids = new ULInt[size];

	MJMesh::Mesh **meshes = this->splitMeshDistributed(ids, size);

	delete [] ids;

	Parallel::Transferable **mt = new Parallel::Transferable*[this->meshes.size()];

	for (Int i = 0; i < static_cast<Int>(this->meshes.size()); i++)
	{
		mt[i] = new MeshTransferable(this->dimension, meshes[i]);
	}

	this->comm->gatherv(mt, static_cast<Int>(this->meshes.size()), this->comm->root());

	delete [] meshes;

	while (!this->fronts.empty())
	{
		if (this->fronts.front())
		{
			delete this->fronts.front();
		}

		this->fronts.pop_front();
	}

	for (Int i = 0; i < static_cast<Int>(this->meshes.size()); i++)
	{
		delete static_cast<MeshTransferable *>(mt[i])->getMesh();

		delete mt[i];
	}

	delete [] mt;*/

	this->EmptyMeshUpdater::splitMeshWorker();
	///end Temporarily -> gather mesh as EmptyMeshUpdater (distributed) and
	///  then split it as FinalMeshUpdater (shared)
}

MJMesh::Mesh **PMJMesh::FinalMeshUpdater::splitMeshDistributed(ULInt *ids, Int size)
{
	this->comm->broadcast(ids, size, this->comm->root());

	UInt numMeshes = static_cast<UInt>(this->meshes.size());

#if USE_C__11
	Data::VertexHashMap vMaps[numMeshes];
#else
	Data::VertexMap vMaps[numMeshes];
#endif //#if USE_C__11

	UInt c = 0;

	for (Data::FrontList::iterator iter = this->fronts.begin();
		 iter != this->fronts.end(); iter++, c++)
	{
		Data::Front *front = (*iter);

		if (!front)
		{
			continue;
		}

		UInt m = c/3;

		for (Data::Front::ConstVertexIterator iter2 = front->vBegin(), end = front->vEnd();
			 iter2 != end; iter2++)
		{
			vMaps[m][(*iter2)->getId()] = (*iter2);
		}
	}

#if USE_ID_HASH_IN_MESH_SPLIT
	MJMesh::VertexIdHash currents[numMeshes], nexts[numMeshes];
#elif USE_C__11
	Data::VertexHash currents[numMeshes], nexts[numMeshes];
#else
	Data::VertexSet currents[numMeshes], nexts[numMeshes];
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

	for (UInt m = 0; m < numMeshes; m++)
	{
		if (vMaps[m].empty())
		{
			continue;
		}

		for (Int i = 0; i < size; i++)
		{
#if USE_C__11
			Data::VertexHashMap::iterator it = vMaps[m].find(ids[i]);
#else
			Data::VertexMap::iterator it = vMaps[m].find(ids[i]);
#endif //#if USE_C__11

			if (it == vMaps[m].end())
			{
				continue;
			}

			currents[m].insert((*it).second);
		}
	}

	MJMesh::Mesh **splits = new MJMesh::Mesh*[numMeshes];

	for (UInt m = 0; m < numMeshes; m++)
	{
		splits[m] = NULL;
	}

	UInt numProcesses = this->comm->numProcesses();
	UInt rank = this->comm->rank();

	Int sizes[numProcesses], displacements[numProcesses];

	for (UInt layer = 0; layer < this->smoothingLayer; layer++)
	{
#if USE_C__11
		Data::ULIntHash layerVertices;
#else
		Data::ULIntSet layerVertices;
#endif //#if USE_C__11

		for (UInt m = 0; m < numMeshes; m++)
		{
			if (currents[m].empty())
			{
				continue;
			}

			if (!splits[m])
			{
				splits[m] = new MJMesh::Mesh();
			}

			splits[m]->splitToIteration(currents[m], nexts[m], NULL, NULL, false);

#if USE_ID_HASH_IN_MESH_SPLIT
			for (MJMesh::VertexIdHash::iterator iter = currents[m].begin();
#elif USE_C__11
			for (Data::VertexHash::iterator iter = currents[m].begin();
#else
			for (Data::VertexSet::iterator iter = currents[m].begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
				 iter != currents[m].end(); iter++)
			{
				vMaps[m].erase((*iter)->getId());
			}

			currents[m].clear();

#if USE_ID_HASH_IN_MESH_SPLIT
			for (MJMesh::VertexIdHash::iterator iter = nexts[m].begin();
#elif USE_C__11
			for (Data::VertexHash::iterator iter = nexts[m].begin();
#else
			for (Data::VertexSet::iterator iter = nexts[m].begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
				 iter != nexts[m].end(); iter++)
			{
				vMaps[m][(*iter)->getId()] = (*iter);

				layerVertices.insert((*iter)->getId());
			}

			currents[m].swap(nexts[m]);
		}

		size = static_cast<Int>(layerVertices.size());

		ULInt *cids = new ULInt[size];

		Int i = 0;

#if USE_C__11
		for (Data::ULIntHash::iterator iter = layerVertices.begin();
#else
		for (Data::ULIntSet::iterator iter = layerVertices.begin();
#endif //#if USE_C__11
			 iter != layerVertices.end(); iter++)
		{
			cids[i++] = (*iter);
		}

		this->comm->allGather(&size, sizes, 1, Parallel::Communicator::INT);

		Int sumSizes = 0;

		for (UInt i = 0; i < numProcesses; i++)
		{
			displacements[i] = sumSizes;
			sumSizes += sizes[i];
		}

		delete [] ids;

		ids = new ULInt[sumSizes];

		this->comm->allGatherv(cids, size, ids, sizes, displacements, Parallel::Communicator::ULINT);

		delete [] cids;

		Int min = displacements[rank];
		Int max = (rank == numProcesses - 1) ? sumSizes : displacements[rank + 1];

		for (UInt m = 0; m < numMeshes; m++)
		{
			for (Int i = 0; i < min; i++)
			{
#if USE_C__11
				Data::VertexHashMap::iterator it = vMaps[m].find(ids[i]);
#else
				Data::VertexMap::iterator it = vMaps[m].find(ids[i]);
#endif //#if USE_C__11

				if (it == vMaps[m].end())
				{
					continue;
				}

				currents[m].insert((*it).second);
			}

			for (Int i = max; i < sumSizes; i++)
			{
#if USE_C__11
				Data::VertexHashMap::iterator it = vMaps[m].find(ids[i]);
#else
				Data::VertexMap::iterator it = vMaps[m].find(ids[i]);
#endif //#if USE_C__11

				if (it == vMaps[m].end())
				{
					continue;
				}

				currents[m].insert((*it).second);
			}
		}
	}

	for (UInt m = 0; m < numMeshes; m++)
	{
		splits[m]->splitToEnding(currents[m], nexts[m]);
	}

	return splits;
}

#endif //#if USE_NEW_FRONT_UPDATER
