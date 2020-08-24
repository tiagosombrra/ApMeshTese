#include "PMJMesh/ActualFrontUpdater.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/MJMeshTask.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Mesh.h"

PMJMesh::ActualFrontUpdater::ActualFrontUpdater(UInt dimension, Front *front,
	Parallel::Communicator *comm, bool external) :
		PMJMesh::AbstractMeshUpdater::AbstractMeshUpdater(dimension, front, comm, external)
{

}

PMJMesh::ActualFrontUpdater::~ActualFrontUpdater()
{

}

void PMJMesh::ActualFrontUpdater::setFront(Front *front)
{
	this->mesh = front;
}

PMJMesh::Front *PMJMesh::ActualFrontUpdater::getFront()
{
	return static_cast<PMJMesh::Front *>(mesh);
}

void PMJMesh::ActualFrontUpdater::finalizeFront()
{
	this->finalizeMesh();
}

void PMJMesh::ActualFrontUpdater::initializeShared()
{

}

void PMJMesh::ActualFrontUpdater::initializeMaster()
{

}

void PMJMesh::ActualFrontUpdater::initializeWorker()
{

}

void PMJMesh::ActualFrontUpdater::finalizeShared()
{
	//deletedFrontElements and deletedVertices should be empty when this->external = true
	while (!this->deletedFrontElements.empty())
	{
		delete this->deletedFrontElements.front();

		this->deletedFrontElements.pop_front();
	}

	while (!this->deletedVertices.empty())
	{
		delete this->deletedVertices.front();

		this->deletedVertices.pop_front();
	}

	while (!this->tempMap.empty())
	{
		CommonVertexMap::iterator it = this->tempMap.begin();

		if ((*it).second.found)
		{
#if USE_C__11
			for (Data::FrontElementHash::iterator iter = (*it).second.commonFront.begin();
#else
			for (Data::FrontElementSet::iterator iter = (*it).second.commonFront.begin();
#endif //#if USE_C__11
				 iter != (*it).second.commonFront.end(); iter++)
			{
				Data::FrontElement *adj = (*iter);

				if ((*it).second.oldFront.find(adj) == (*it).second.oldFront.end())
				{
					continue;
				}

				adj->replace((*it).second.temp, (*it).second.orig);

				(*it).second.temp->remove(adj);

				(*it).second.orig->add(adj);
			}
		}

		this->tempMap.erase(it);
	}
}

void PMJMesh::ActualFrontUpdater::finalizeMaster()
{

}

void PMJMesh::ActualFrontUpdater::finalizeWorker()
{

}

void PMJMesh::ActualFrontUpdater::executeShared()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->masterTask->getBuilder());

	if (!drive)
	{
		return;
	}

	PMJMesh::Front *oldFront = drive->getOldFront();
	PMJMesh::Front *commonFront = drive->getCommonFront();
	PMJMesh::Front *newFront = drive->getNewFront();

	if ((!oldFront) && (!commonFront) && (!newFront))
	{
		//non-working task

		if (static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront())
		{
			static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront()->clear();
		}

		return;
	}

#if USE_GUI
	drive->setOldFront(NULL);
	drive->setCommonFront(NULL);
	drive->setNewFront(NULL);
#endif //#if USE_GUI

	if (oldFront)
	{
		oldFront->moveBoundaryToElements();
		oldFront->moveCrossingToRejected();
		oldFront->moveRejectedToElements();
	}

	if (newFront)
	{
		newFront->moveBoundaryToElements();
		newFront->moveCrossingToRejected();
		newFront->moveRejectedToElements();
	}

	//if (commonFront)
	//{
		//commonFront->moveBoundaryToElements();
		//commonFront->moveCrossingToRejected();
		//commonFront->moveRejectedToElements();
	//}

	if (this->external)
	{
		this->executeSharedExternal(oldFront, commonFront, newFront);
	}
	else
	{
		this->executeSharedInternal(oldFront, commonFront, newFront, this->masterTask->getMesh());
	}

	if (oldFront)
	{
		drive->setOldFront(NULL);

		delete oldFront;
	}

	if (commonFront)
	{
		if (commonFront == static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront())
		{
			static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->setSubfront(NULL);
		}

		drive->setCommonFront(NULL);

		commonFront->clear();

		delete commonFront;
	}

	if (newFront)
	{
		static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->setSubfront(NULL);

		drive->setNewFront(NULL);

		delete newFront;

		this->updated = true;
	}
}

void PMJMesh::ActualFrontUpdater::executeSharedExternal(Front *oldFront, Front *commonFront, Front *newFront)
{
	if (oldFront)
	{
		for (Data::Front::FrontElementIterator iter = oldFront->eBegin(1);
			 iter != oldFront->eEnd(1); iter = oldFront->erase(iter, 1))
		{
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

			/*if (fel->getId() == 51640)
            {
                std::cout << "found fel at " << fel << std::endl;
                std::cout << "found fel " << fel->text() << std::endl;
            }*/

			this->getFront()->remove(fel);
		}

		for (Data::Front::VertexIterator iter = oldFront->vBegin();
			 iter != oldFront->vEnd(); iter = oldFront->erase(iter))
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if ((!commonFront->have(v)) && (!newFront->have(v)))
			{
				this->getFront()->remove(v);
			}
		}
	}

	if (newFront)
	{
		for (Data::Front::VertexIterator iter = newFront->vBegin();
			 iter != newFront->vEnd(); iter = newFront->erase(iter))
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (!this->getFront()->have(v))
			{
				this->getFront()->add(v);
			}
		}

		for (Data::Front::FrontElementIterator iter = newFront->eBegin(1);
			 iter != newFront->eEnd(1); iter = newFront->erase(iter, 1))
		{
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

			this->getFront()->add(fel);
		}
	}
}

void PMJMesh::ActualFrontUpdater::executeSharedInternal(Front *oldFront, Front *commonFront, Front *newFront, MJMesh::Mesh *mesh)
{
	//std::cout << "ActualFrontUpdater, task " << this->masterTask->getId() << std::endl;

	if (oldFront)
	{
		for (Data::Front::VertexIterator iter = oldFront->vBegin();
			 iter != oldFront->vEnd();)
		{
			MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));

			/*{
				std::cout << "debug front updater, oldFront, vertex " << temp->text() << ", " << temp << " will be tested" << std::endl;
			}*/

			bool cHave = commonFront->have(temp);
			bool nHave = newFront->have(temp);
			//bool have = (commonFront->have(temp) || newFront->have(temp));
			bool have = ((cHave) || (nHave));

			if ((nHave) && (!cHave))
			{
				commonFront->add(temp);
			}

			if (nHave)
			{
				std::pair<CommonVertexMap::iterator, bool> p = this->tempMap.insert(std::make_pair(temp->getId(), CommonVertex()));

				if (p.second)
				{
					(*p.first).second.orig = temp;
					(*p.first).second.temp = NULL;
					(*p.first).second.found = false;
				}

				//Data::FrontElementList adjs = temp->getAdjacentFrontElements();

				//while (!adjs.empty())
				for (Data::FrontElementList::iterator iter = temp->feBegin();
                     iter != temp->feEnd(); iter++)
				{
					//Data::FrontElement *fel = adjs.front();
					Data::FrontElement *fel = (*iter);

					//adjs.pop_front();

					if (oldFront->have(fel))
					{
						(*p.first).second.oldFront.insert(fel);
					}

					if (commonFront->have(fel))
					{
						(*p.first).second.commonFront.insert(fel);
					}

					/*if (newFront->have(fel))
					{
						(*p.first).second.newFront.insert(fel);
					}*/
				}
			}

#if USE_C__11
			Data::VertexHashMap::iterator it;
#else
			Data::VertexMap::iterator it;
#endif //#if USE_C__11

			MJMesh::Vertex *orig = NULL;

			/*{
				std::cout << "debug front updater, oldFront, vertex " << temp->text() << ", " << temp << " will be tested" << std::endl;

				if (have)
				{
					std::cout << "debug front updater vertex " << temp->text() << ", " << temp << " also in commonFront or in newFront" << std::endl;
				}
				else
				{
					std::cout << "debug front updater vertex " << temp->text() << ", " << temp << " neither in commonFront nor in newFront" << std::endl;
				}
			}*/

			//std::cout << "oldFront, testing temp vertex " << temp << ", " << temp->text() << std::endl;

			if (!have)
			{
				it = this->vMap.find(temp->getId());

				if (it != this->vMap.end())
				{
					orig = static_cast<MJMesh::Vertex *>((*it).second);

					/*{
						std::cout << "debug front updater original vertex " << orig->text() << ", " << orig << std::endl;
					}*/

					this->replace(temp, orig, false);

					//std::cout << "oldFront, replacing temp vertex " << temp << ", " << temp->text() << " for orig " << orig << ", " << orig->text() << std::endl;
				}

				/*{
					std::cout << "debug front updater removing vertex " << temp->text() << ", " << temp << " from this->front" << std::endl;
				}*/

				//std::cout << "oldFront, removing temp vertex " << temp << ", " << temp->text() << " from front" << std::endl;

				this->getFront()->remove(temp);
			}

			iter = oldFront->erase(iter);

			if (!have)
			{
				if (it != this->vMap.end())
				{
					/*{
						std::cout << "debug front updater, erasing and deleting vertex " << temp->text() << ", " << temp << std::endl;
					}*/

					this->vMap.erase(it);

					temp->setPoint(NULL);

					if ((mesh) && (mesh->remove(temp)))
					{
						mesh->add(orig);
					}

					/*if (temp->getData().fronts.size() > 0)
					{
						std::cout << "temp vertex " << temp << " still in " << temp->getData().fronts.size() << " fronts:";

						for (MJMesh::VertexSpecificData::FrontDataMap::iterator iter = temp->getData().fronts.begin();
							 iter != temp->getData().fronts.end(); iter++)
						{
							std::cout << " " << (*iter).first;
						}

						std::cout << std::endl
							<< "oldFront = " << oldFront << std::endl
							<< "newFront = " << newFront << std::endl
							<< "commonFront = " << commonFront << std::endl
							<< "this->front = " << this->getFront() << std::endl;
					}*/

					//std::cout << "oldFront, deleting temp vertex " << temp << ", " << temp->text() << std::endl;

					if (temp->isInAnyFront())
					{
						/*{
							std::cout << "debug" << std::endl;
						}*/

						this->deletedVertices.push_back(temp);
					}
					else
					{
						delete temp;
					}
				}
			}
		}

		for (Data::Front::FrontElementIterator iter = oldFront->eBegin(1);
			 iter != oldFront->eEnd(1);)
		{
			MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));

			/*{
				std::cout << "debug front updater, oldFront, fel " << temp->text() << ", " << temp << " will be tested" << std::endl;
			}*/

#if USE_C__11
			Data::FrontElementHashMap::iterator it = this->felMap.find(temp->getId());
#else
			Data::FrontElementMap::iterator it = this->felMap.find(temp->getId());
#endif //#if USE_C__11

			/*{
				std::cout << "debug front updater, this->external is " << std::boolalpha << this->external << std::endl;
			}*/

			MJMesh::FrontElement *orig = NULL;

			if (it != this->felMap.end())
			{
				orig = static_cast<MJMesh::FrontElement *>((*it).second);

				/*{
					std::cout << "debug front updater, original fel " << orig->text() << ", " << orig << std::endl;
				}*/

				this->replace(temp, orig);
			}
			/*else
			{
				std::cout << "debug front updater, original fel not found" << std::endl;
			}*/

			/*{
				std::cout << "debug front updater, removing fel from this->front" << std::endl;
			}*/

			//std::cout << "removing temp fel at " << temp << " from this->getFront() at " << this->getFront() << std::endl;
			//std::cout << "removing temp fel " << temp->getId() << " from this->getFront() at " << this->getFront() << std::endl;

			this->getFront()->remove(temp);

			iter = oldFront->erase(iter, 1);

			if (it != this->felMap.end())
			{
				/*{
					std::cout << "debug front updater, erasing and deleting fel " << temp->text() << ", " << temp << std::endl;
				}*/

				this->felMap.erase(it);

				if ((mesh) && (mesh->remove(temp)))
				{
					mesh->add(orig);
				}

				//std::cout << "actual front updater, deleting fel at " << temp << std::endl;
				//std::cout << "actual front updater, deleting fel " << temp->getId() << std::endl;

				if (temp->isInAnyFront())
				{
					/*std::cout << "temp is still in other front(s):";
					for (MJMesh::FrontElementSpecificData::FrontDataMap::iterator iter = temp->getData().fronts.begin();
						 iter != temp->getData().fronts.end(); iter++)
					{
						std::cout << " " << (*iter).first;
					}
					std::cout << std::endl;*/

					this->deletedFrontElements.push_back(temp);
				}
				else
				{
					delete temp;
				}
			}
		}
	}

	if (newFront)
	{
#if USE_C__11
		Data::VertexHashMap tempMap;
#else
		Data::VertexMap tempMap;
#endif //#if USE_C__11

		for (Data::Front::VertexIterator iter = newFront->vBegin();
			 iter != newFront->vEnd(); iter = newFront->erase(iter))
		{
			MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>((*iter));

			/*{
				std::cout << "debug front updater, newFront, vertex " << orig->text() << ", " << orig << " will be tested" << std::endl;
			}*/

			MJMesh::Vertex *temp = NULL;

#if USE_C__11
			Data::VertexHashMap::iterator it = this->vMap.find(orig->getId());
#else
			Data::VertexMap::iterator it = this->vMap.find(orig->getId());
#endif //#if USE_C__11

			bool found = (it != this->vMap.end());

			//std::cout << "newFront, testing temp vertex " << orig << ", " << orig->text() << std::endl;

			if (found)
			{
				temp = orig;

				orig = static_cast<MJMesh::Vertex *>((*it).second);

				/*{
					std::cout << "debug front updater, newFront, found original vertex " << orig->text() << ", " << orig << std::endl;
				}*/

				//std::cout << "newFront, found orig vertex " << orig << ", " << orig->text() << std::endl;

				if (temp == orig)
				{
					temp = this->tempMap[orig->getId()].temp;
				}
			}
			else
			{
				temp = new MJMesh::Vertex(orig->getPoint(), orig->getId());

#if USE_THREAD_COLOR
				temp->setColor(orig->getR(), orig->getG(), orig->getB());
#endif //#if USE_THREAD_COLOR

				this->vMap[temp->getId()] = orig;

				/*{
					std::cout << "debug front updater, newFront, making temp vertex " << temp->text() << ", " << temp << std::endl;
				}*/

				//std::cout << "newFront, creating new temp vertex " << temp << ", " << temp->text() << std::endl;
			}

			tempMap[temp->getId()] = temp;

			if (found)
			{
				/*{
					std::cout << "debug front updater, newFront, replacing temp with original vertex" << std::endl;
				}*/

				this->replace(temp, orig, true, (this->dimension == 3) ? commonFront : NULL);

				//std::cout << "newFront, replacing temp vertex " << temp << ", " << temp->text() << " for orig " << orig << ", " << orig->text() << std::endl;

				continue;
			}

			if (commonFront->have(orig))
			{
				{
					CommonVertexMap::iterator it = this->tempMap.find(orig->getId());

					if (it != this->tempMap.end())
					{
						(*it).second.temp = temp;
						(*it).second.found = true;
					}
				}

				/*{
					std::cout << "debug front updater, newFront, commonFront has original vertex " << orig->text() << ", " << orig << std::endl;
				}*/

				//std::cout << "newFront, commonFront has orig vertex " << orig << ", " << orig->text() << std::endl;

				//Data::FrontElementList fels = orig->getAdjacentFrontElements();

				//while (!fels.empty())]
				for (Data::FrontElementList::iterator iter = orig->feBegin();
                     iter != orig->feEnd();)
				{
					//Data::FrontElement *adj = fels.front();
					Data::FrontElement *adj = (*iter);

					//fels.pop_front();

					if (commonFront->have(adj))
					{
						/*{
							std::cout << "debug front updater, newFront, commonFront has adj front element " << adj->text() << ", " << adj << std::endl;
						}*/

						//std::cout << "newFront, commonFront has adj fel " << adj << ", " << adj->text() << std::endl;
						//std::cout << "newFront, in fel, replacing orig " << orig << ", " << orig->text() << " for temp " << temp << " " << temp->text() << std::endl;

						adj->replace(orig, temp);

						//orig->remove(adj);

						temp->add(adj);

						iter = orig->erase(iter);
					}
					else
                    {
                        iter++;
                    }
				}

				//std::cout << "newFront, removing orig vertex " << orig << ", " << orig->text() << " from front" << std::endl;

				this->getFront()->remove(orig);

				/*{
					std::cout << "debug front updater, newFront, removing original vertex from front " << orig->text() << ", " << orig << std::endl;
				}*/
			}

			/*{
				std::cout << "debug front updater, newFront, adding temp vertex to this->front " << temp->text() << ", " << temp << std::endl;
			}*/

			//std::cout << "newFront, adding temp vertex " << temp << ", " << temp->text() << " to front" << std::endl;

			this->getFront()->add(temp);
		}

		for (Data::Front::FrontElementIterator iter = newFront->eBegin(1);
			 iter != newFront->eEnd(1); iter = newFront->erase(iter, 1))
		{
			MJMesh::FrontElement *orig = static_cast<MJMesh::FrontElement *>((*iter));

			/*{
				std::cout << "debug front updater, newFront, fel " << orig->text() << ", " << orig << " will be tested" << std::endl;
			}*/

			MJMesh::FrontElement *temp = NULL;

			MJMesh::Vertex *temps[orig->numVertices()];

			for (UInt i = 0; i < orig->numVertices(); i++)
			{
				temps[i] = static_cast<MJMesh::Vertex *>(tempMap[orig->getVertex(i)->getId()]);
			}

			temp = (this->dimension == 2) ?
				static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(temps[0], temps[1], orig->getId())) :
				static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(temps[0], temps[1], temps[2], orig->getId()));

#if USE_THREAD_COLOR
			temp->setColor(orig->getR(), orig->getG(), orig->getB());
#endif //#if USE_THREAD_COLOR

			this->felMap[temp->getId()] = orig;

			//std::cout << "created temp at " << temp << " for orig at " << orig << std::endl;
			//std::cout << "created temp " << temp->getId() << " for orig " << orig->getId() << std::endl;

			/*{
				std::cout << "debug front updater, newFront, making temp fel " << temp->text() << ", " << temp << std::endl;
			}

			//{
				std::cout << "debug front updater, newFront, adding fel " << temp->text() << ", " << temp << " to this->front" << std::endl;
			}*/

			this->getFront()->add(temp);
		}
	}
}

void PMJMesh::ActualFrontUpdater::executeMaster()
{
	PMJMesh::WorkerMainDrive *workerDrive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->workerTask->getBuilder());

	if (!workerDrive)
	{
		return;
	}

	PMJMesh::Front *newFront = dynamic_cast<PMJMesh::Partition *>(this->workerTask->getPartition())->getSubfront();
	PMJMesh::Front *oldFront = dynamic_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->getSubfront();

	if (!newFront)
	{
		if (!oldFront)
		{
			return;
		}

		oldFront->clear();

		static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->setSubfront(NULL);

		delete oldFront;

		return;
	}

#if USE_GUI
	static_cast<PMJMesh::Partition *>(this->workerTask->getPartition())->setSubfront(NULL);
	workerDrive->setNewFront(NULL);
#endif //#if USE_GUI

	oldFront->moveBoundaryToElements();
	oldFront->moveCrossingToRejected();
	oldFront->moveRejectedToElements();

	newFront->moveBoundaryToElements();
	newFront->moveCrossingToRejected();
	newFront->moveRejectedToElements();

	for (Data::Front::FrontElementIterator iter = newFront->eBegin(1);
		 iter != newFront->eEnd(1);)
	{
		MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));

		iter = newFront->erase(iter, 1);

		if (oldFront->have(temp))
		{
			oldFront->remove(temp);
		}
		else
		{
			this->getFront()->add(temp);
		}
	}

	for (Data::Front::VertexIterator iter = newFront->vBegin();
		 iter != newFront->vEnd();)
	{
		MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));

		iter = newFront->erase(iter);

		if (oldFront->have(temp))
		{
			oldFront->remove(temp);
		}
		else
		{
			this->getFront()->add(temp);
		}
	}

	for (Data::Front::VertexIterator iter = oldFront->vBegin();
		 iter != oldFront->vEnd();)
	{
		if (this->getFront()->have((*iter)))
		{
			this->getFront()->remove((*iter));
		}

		iter = oldFront->erase(iter);
	}

	for (Data::Front::FrontElementIterator iter = oldFront->eBegin(1);
		 iter != oldFront->eEnd(1);)
	{
		if (this->getFront()->have((*iter)))
		{
			this->getFront()->remove((*iter));
		}

		iter = oldFront->erase(iter, 1);
	}

	static_cast<PMJMesh::Partition *>(this->workerTask->getPartition())->setSubfront(NULL);

	delete newFront;

	static_cast<PMJMesh::Partition *>(this->masterTask->getPartition())->setSubfront(NULL);

	delete oldFront;

	this->updated = true;
}

void PMJMesh::ActualFrontUpdater::executeWorker()
{

}

void PMJMesh::ActualFrontUpdater::finalizeMeshShared()
{
	if (this->external)
	{
		return;
	}

	//std::cout << "finalizing" << std::endl;

	for (UShort state = 0; state < 4; state++)
	{
		for (Data::Front::FrontElementIterator iter = this->getFront()->eBegin(state);
			 iter != this->getFront()->eEnd(state);)
		{
			MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));

			//std::cout << "temp front element " << temp << std::endl;
			//std::cout << "temp front element " << temp->getId() << std::endl;
			//std::cout << "temp front element " << temp->text() << std::endl;

#if USE_C__11
			Data::FrontElementHashMap::iterator it = this->felMap.find(temp->getId());
#else
			Data::FrontElementMap::iterator it = this->felMap.find(temp->getId());
#endif //#if USE_C__11

			if (it == this->felMap.end())
			{
				iter++;

				continue;
			}

			MJMesh::FrontElement *orig = static_cast<MJMesh::FrontElement *>((*it).second);

			//std::cout << "orig front element " << orig << std::endl;
			//std::cout << "orig front element " << orig->getId() << std::endl;
			//std::cout << "orig front element " << orig->text() << std::endl;

			this->replace(temp, orig);

			iter = this->getFront()->replace(iter, state, orig);

			delete temp;

			this->felMap.erase(it);
		}
	}

	for (Data::Front::VertexIterator iter = this->getFront()->vBegin();
		 iter != this->getFront()->vEnd();)
	{
		MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));

		//std::cout << "temp vertex " << temp << std::endl;
		//std::cout << "temp vertex " << temp->text() << std::endl;

#if USE_C__11
		Data::VertexHashMap::iterator it = this->vMap.find(temp->getId());
#else
		Data::VertexMap::iterator it = this->vMap.find(temp->getId());
#endif //#if USE_C__11

		if (it == this->vMap.end())
		{
			iter++;

			continue;
		}

		MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>((*it).second);

		//std::cout << "orig vertex " << orig << std::endl;
		//std::cout << "orig vertex " << orig->text() << std::endl;

		this->replace(temp, orig, true);

		iter = this->getFront()->replace(iter, orig);

		temp->setPoint(NULL);

		delete temp;

		this->vMap.erase(it);
	}

	this->vMap.clear();
	this->felMap.clear();
}

void PMJMesh::ActualFrontUpdater::finalizeMeshMaster()
{

}

void PMJMesh::ActualFrontUpdater::finalizeMeshWorker()
{

}

#endif //#if USE_NEW_FRONT_UPDATER
