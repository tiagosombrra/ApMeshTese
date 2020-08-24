#include "PMJMesh/PartialMeshUpdater.h"

#include "Performer/IdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Communicator.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"

const bool PMJMesh::PartialMeshUpdater::useInnerMesh = false;

PMJMesh::PartialMeshUpdater::PartialMeshUpdater(UInt smoothingLayer,
	MJMesh::Boundary *boundary, Performer::IdManager *idManager,
	UInt dimension, MJMesh::Mesh *mesh, Parallel::Communicator *comm,
	bool noBoundary, bool external) :
		PMJMesh::MeshUpdater::MeshUpdater(dimension, mesh, comm, external)
{
	//this->useInnerMesh = false;
	this->innerMesh = NULL;
	//this->idBoundary[0] = this->idBoundary[1] = 0;
	//this->process = 0;

	this->setSmoothingLayer(smoothingLayer);
	this->setBoundary(boundary);
	this->setIdManager(idManager);

	this->noBoundary = noBoundary;
	this->firstTime = true;
}

PMJMesh::PartialMeshUpdater::~PartialMeshUpdater()
{
	if (this->innerMesh)
	{
		delete this->innerMesh;
	}
}

void PMJMesh::PartialMeshUpdater::setSmoothingLayer(UInt smoothingLayer)
{
	this->smoothingLayer = smoothingLayer;
}

void PMJMesh::PartialMeshUpdater::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMJMesh::PartialMeshUpdater::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;

	/*if (this->idManager)
	{
		this->idBoundary[0] = this->idManager->current(0);
		this->idBoundary[1] = this->idManager->current(1);
	}*/
}

Performer::IdManager *PMJMesh::PartialMeshUpdater::getIdManager() const
{
	return this->idManager;
}

/*void PMJMesh::PartialMeshUpdater::setProcess(Int process)
{
	this->process = process;
}*/

/*void PMJMesh::PartialMeshUpdater::setCreatedFrontElements(bool created)
{
	this->createdFels = created;
}*/

const MJMesh::Mesh *PMJMesh::PartialMeshUpdater::getInnerMesh() const
{
	return this->innerMesh;
}

void PMJMesh::PartialMeshUpdater::removeFromBoundary(
	const PMJMesh::Front *front, MJMesh::Mesh *mesh, const PMJMesh::Front *except)
{
#if USE_GUI
	this->boundary->freeze();
	mesh->freeze();
#endif //#if USE_GUI

	for (UShort i = 0; i < 4; i++)
	{
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(i);
			 iter != front->eEnd(i); iter++)
		{
			Data::FrontElement *fel = (*iter);

			/*if (fel->getId() == 1859)
			{
				std::cout << "fel " << fel->getId() << " is being tested in removeFromBoundary" << std::endl;
			}*/

			if ((!this->boundary->have(fel)) ||
				(!mesh->have(fel)) ||
				((except) && (except->have(fel))))
			{
				/*if (fel->getId() == 1859)
				{
					std::cout << "fel " << fel->getId() << " is not in boundary or not in mesh or in except" << std::endl;
				}*/

				continue;
			}

			//Data::ElementList adjs = mesh->adjacency(fel);

			//if ((adjs.empty()) || (!mesh->have(adjs.front())))
			if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0) || (!mesh->have((*static_cast<MJMesh::FrontElement *>(fel)->eBegin()))))
			{
				this->boundary->remove(fel);

				mesh->remove(fel, false);

				//this->boundaryFrontElements.push_back(fel);
				this->boundaryFrontElements[fel->getId()] = fel;

				/*if (fel->getId() == 1859)
				{
					std::cout << "fel " << fel->getId() << " is being added to this->boundaryFrontElements" << std::endl;
				}*/
			}
		}
	}

	for (Data::Front::ConstVertexIterator iter = front->vBegin();
		 iter != front->vEnd(); iter++)
	{
		Data::Vertex *v = (*iter);

		/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
		{
			std::cout << "vertex " << v->getId() << " is being tested in removeFromBoundary" << std::endl;
		}*/

		if ((!this->boundary->have(v)) ||
			(!mesh->have(v)) ||
			((except) && (except->have(v))))
		{
			/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
			{
				std::cout << "vertex " << v->getId() << " is not in boundary or not in mesh or in except" << std::endl;
			}*/

			continue;
		}

		if (this->boundary->adjacency(v).empty())
		{
			//std::cout << "removing vertex " << v->text() << " from boundary" << std::endl;

			this->boundary->remove(v);

			mesh->remove(v);

			//this->boundaryVertices.push_back(v);
			this->boundaryVertices[v->getId()] = v;

			/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
			{
				std::cout << "vertex " << v->getId() << " is being added to this->boundaryVertices" << std::endl;
			}*/
		}
		else if (mesh->haveInner(v))
		{
			mesh->removeInner(v);

			/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
			{
				std::cout << "vertex " << v->getId() << " is being removed from inner" << std::endl;
			}*/
		}
	}

#if USE_GUI
	this->boundary->unfreeze();
	mesh->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh::PartialMeshUpdater::addToBoundary()
{
#if USE_GUI
	this->boundary->freeze();
#endif //#if USE_GUI

#if USE_C__11
	for (Data::VertexHashMap::iterator iter = this->boundaryVertices.begin();
#else
	for (Data::VertexMap::iterator iter = this->boundaryVertices.begin();
#endif //#if USE_C__11
		 iter != this->boundaryVertices.end(); iter++)
	{
		Data::Vertex *v = (*iter).second;

		this->boundary->add(v);

		this->getMesh()->add(v, false);

		/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
		{
			std::cout << "vertex " << v->getId() << " being added back to boundary" << std::endl;
		}*/
	}

	this->boundaryVertices.clear();

#if USE_C__11
	for (Data::FrontElementHashMap::iterator iter = this->boundaryFrontElements.begin();
#else
	for (Data::FrontElementMap::iterator iter = this->boundaryFrontElements.begin();
#endif //#if USE_C__11
		 iter != this->boundaryFrontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter).second;

		this->boundary->add(fel);

		this->getMesh()->add(fel);

		/*if (fel->getId() == 1859)
		{
			std::cout << "fel " << fel->getId() << " is being added back to boundary" << std::endl;
		}*/
	}

	this->boundaryFrontElements.clear();

	this->readjustSharedVerticesAdjacencies();

#if USE_GUI
	this->boundary->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh::PartialMeshUpdater::adjustSharedVerticesAdjacencies()
{
#if USE_C__11
	for (Data::VertexHashMap::iterator iter = this->sharedBoundaryVertices.begin();
#else
	for (Data::VertexMap::iterator iter = this->sharedBoundaryVertices.begin();
#endif //#if USE_C__11
		 iter != this->sharedBoundaryVertices.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second);

		Data::FrontElementList adjs = v->getAdjacentFrontElements();

		for (Data::FrontElementList::iterator iter2 = adjs.begin();
			 iter2 != adjs.end();)
		{
			if (this->getMesh()->have((*iter2)))
			{
				iter2 = adjs.erase(iter2);

				continue;
			}

			static_cast<MJMesh::Vertex *>(v)->remove((*iter2));

			iter2++;
		}

		if (!adjs.empty())
		{
#if USE_C__11
			std::pair<VertexFEListHashMap::iterator, bool> p = this->sharedBoundaryVerticesAdjacency.insert(std::make_pair(v, adjs));
#else
			std::pair<VertexFEListMap::iterator, bool> p = this->sharedBoundaryVerticesAdjacency.insert(std::make_pair(v, adjs));
#endif //#if USE_C__11

			if (!p.second)
			{
				(*p.first).second.splice((*p.first).second.end(), adjs);
			}
		}
	}
}

void PMJMesh::PartialMeshUpdater::readjustSharedVerticesAdjacencies()
{
#if USE_C__11
	for (VertexFEListHashMap::iterator iter = this->sharedBoundaryVerticesAdjacency.begin();
#else
	for (VertexFEListMap::iterator iter = this->sharedBoundaryVerticesAdjacency.begin();
#endif //#if USE_C__11
		 iter != this->sharedBoundaryVerticesAdjacency.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).first);

		for (Data::FrontElementList::iterator iter2 = (*iter).second.begin();
			 iter2 != (*iter).second.end(); iter2++)
		{
			v->add((*iter2));
		}
	}

	this->sharedBoundaryVertices.clear();
	this->sharedBoundaryVerticesAdjacency.clear();

}

void PMJMesh::PartialMeshUpdater::replace(MJMesh::Mesh *mesh, MJMesh::Mesh *outside,
	const MJMesh::Boundary &sharedBoundary, bool addBoundary, const PMJMesh::Front *front)
{
	//outside keeps the temporary vertices/front elements

#if USE_C__11
	Data::VertexHashMap newVertices;
#else
	Data::VertexMap newVertices;
#endif //#if USE_C__11

	/*if (this->external)
	{
		std::stringstream str;
		str << "process " << this->comm->rank() << ", partial mesh updater split, initial ids: "
			<< this->idManager->getId(0) << ", " << this->idManager->getId(1) << ", "
			<< this->idManager->getId(2) << std::endl;
		std::cout << str.str();
	}*/

	//std::cout << "replacing vertices and front elements in shared boundary" << std::endl;

#if USE_C__11
	Data::VertexHashMap originals;
#else
	Data::VertexMap originals;
#endif //#if USE_C__11

	for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
		 iter != sharedBoundary.vEnd(); iter++)
	{
		/*if ((*iter)->getId() == 2174)
		{
			std::cout << "debug vertex in shared boundary " << (*iter)->text() << std::endl;
		}*/

		if (front)
		{
			bool ohave = outside->have((*iter));
			bool fhave = front->have((*iter));

			if ((ohave) || (fhave))
			{
				if (addBoundary)
				{
					this->boundary->add((*iter));
				}

				if (ohave)
				{
					if (outside->haveInner((*iter)))
					{
						outside->removeInner((*iter));
					}
				}
				else
				{
					outside->add((*iter), false);
				}

				continue;
			}
		}

		/*if ((*iter)->getId() == 384)
		{
			std::cout << "debug" << std::endl;
		}*/

		VMap map;

		map.original = (*iter);
		map.temporary = new MJMesh::Vertex(map.original->getPoint(), map.original->getId());

		//std::cout << "creating vertex at " << map.temporary << std::endl;
		//std::cout << "creating vertex " << map.temporary->text() << std::endl;
		//std::cout << "replacing original at " << map.original << std::endl;
		//std::cout << "replacing original " << map.original->text() << std::endl;

#if USE_GUI
		map.temporary->setColor(map.original->getR(), map.original->getG(), map.original->getB());
#endif //#if USE_GUI

		newVertices[map.temporary->getId()] = map.temporary;

		if (addBoundary)
		{
			this->newBoundaryVertices.push_back(map);

			this->boundary->add(map.temporary);
		}

		if ((this->noBoundary) && (this->firstTime) && (this->boundary->have(map.original)))
		{
			/*this->boundary->remove(map.original);

			this->boundaryVertices[map.original->getId()] = map.original;

			outside->remove(map.original);*/

			originals[map.original->getId()] = map.original;
		}

		outside->add(map.temporary, false);

		//Data::FrontElementList fels = mesh->findAdjacentFrontElements(map.original);

		//if ((this->noBoundary) && (this->firstTime) && (this->boundary->have(map.original)))
		//{
		//	originals[map.original->getId()] = map.original;
		//}

		//while (!fels.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(map.original)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(map.original)->feEnd();)
		{
			//Data::FrontElement *fel = fels.front();
			Data::FrontElement *fel = (*iter);

			//fels.pop_front();

			/*if (fel->getId() == 650)
			{
				std::cout << "debug adj fel " << fel->text() << std::endl;
			}*/

			if (!outside->have(fel))
			{
			    iter++;

				continue;
			}

			if ((this->noBoundary) && (this->firstTime) && (this->boundary->have(fel)))
			{
				//Data::ElementList els = this->getMesh()->adjacency(fel);

				//if ((!els.empty()) && (!outside->have(els.front())))
				if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() != 0) && (!outside->have((*static_cast<MJMesh::FrontElement *>(fel)->eBegin()))))
				{
				    iter++;

					continue;
				}
			}

			fel->replace(map.original, map.temporary);

			//static_cast<MJMesh::Vertex *>(map.original)->remove(fel);
			iter = static_cast<MJMesh::Vertex *>(map.original)->erase(iter);
			static_cast<MJMesh::Vertex *>(map.temporary)->add(fel);
		}

		Data::ElementList els = mesh->adjacency(map.original);

		for (Data::ElementList::iterator iter = els.begin();
			 iter != els.end(); iter++)
		{
			Data::Element *e = (*iter);

			if (!outside->have(e))
			{
				continue;
			}

			e->replace(map.original, map.temporary);

			static_cast<MJMesh::Vertex *>(map.original)->remove(e);
			static_cast<MJMesh::Vertex *>(map.temporary)->add(e);

			if (!this->external)
			{
				continue;
			}

			UInt numVerts = e->numVertices();

			MJMesh::Vertex *vertices[numVerts];

			for (Data::ElementList::iterator iter2 = els.begin();
				 iter2 != els.end(); iter2++)
			{
				Data::Element *other = (*iter2);

				if ((e == other) ||
					(outside->have(other)))
				{
					continue;
				}

				UInt count = 0;

				for (UInt i = 0; i < numVerts; i++)
				{
					if (other->getShape()->have(e->getVertex(i)->getPoint()))
					{
						vertices[count++] = static_cast<MJMesh::Vertex *>(e->getVertex(i));
					}
					else
					{
						vertices[numVerts - 1] = static_cast<MJMesh::Vertex *>(e->getVertex(i));
					}
				}

				if (count != (numVerts - 1))
				{
					continue;
				}

				bool create = true;

				for (UInt i = 0; i < numVerts - 1; i++)
				{
					if (vertices[i] == map.temporary)
					{
						continue;
					}

					if (newVertices.find(vertices[i]->getId()) != newVertices.end())
					{
						create = false;

						break;
					}
				}

				if (!create)
				{
					continue;
				}

				FEMap femap;

				femap.original = NULL;
				femap.temporary = (this->dimension == 2) ?
					static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(vertices[0], vertices[1], this->idManager->next(1))) :
					static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(vertices[0], vertices[1], vertices[2], this->idManager->next(1)));

				if (!femap.temporary->accordingToNormal(vertices[numVerts - 1]->getPoint()))
				{
					femap.temporary->invert();
				}

				//std::cout << "creating temporary fel " << femap.temporary->text() << std::endl;

				if (addBoundary)
				{
					this->newBoundaryFrontElements.push_back(femap);

					//std::cout << "adding temporary fel to this->boundary" << std::endl;

					this->boundary->add(femap.temporary);
				}

				//std::cout << "adding temporary fel to outside" << std::endl;

				outside->add(femap.temporary);

				static_cast<MJMesh::FrontElement *>(femap.temporary)->add(static_cast<MJMesh::Element *>(e));
			}
		}
	}

	/*if (this->external)
	{
		std::stringstream str;
		str << "process " << this->comm->rank() << ", partial mesh updater split, final ids: "
			<< this->idManager->getId(0) << ", " << this->idManager->getId(1) << ", "
			<< this->idManager->getId(2) << std::endl;
		std::cout << str.str();
	}*/

	if (!addBoundary)
	{
#if USE_C__11
		for (Data::VertexHashMap::iterator iter = newVertices.begin();
#else
		for (Data::VertexMap::iterator iter = newVertices.begin();
#endif //#if USE_C__11
			 iter != newVertices.end(); iter++)
		{
			Data::Vertex *v = (*iter).second;

			Data::Point *tpoint = (this->dimension == 2) ?
				static_cast<Data::Point *>(new Data::Point2D(v->getPoint())) :
				static_cast<Data::Point *>(new Data::Point3D(v->getPoint()));

			v->setPoint(tpoint);
		}
	}

	//shared fels should be empty when this->external = true (except for the
	//  FinalMeshUpdater case, where it should contain only front elements
	//  that are also in the boundary)
	for (Data::Boundary::ConstFrontElementIterator iter = sharedBoundary.eBegin();
		 iter != sharedBoundary.eEnd(); iter++)
	{
		//std::cout << "fel in sharedBoundary = " << (*iter)->text() << std::endl;

		/*if ((*iter)->getId() == 650)
		{
			std::cout << "debug fel in shared boundary " << (*iter)->text() << std::endl;
		}*/

		if (front)
		{
			bool have = front->have((*iter));

			if (!have)
			{
				for (UInt i = 0; i < (*iter)->numVertices(); i++)
				{
					if (front->have((*iter)->getVertex(i)))
					{
						have = true;

						break;
					}
				}
			}

			bool ohave = false;

			if (!have)
			{
				ohave = outside->have((*iter));
			}

			if ((have) || (ohave))
			{
				if (addBoundary)
				{
					this->boundary->add((*iter));

					//std::cout << "fel added to this->boundary" << std::endl;
				}

				if (!ohave)
				{
					//std::cout << "fel added to outside" << std::endl;

					outside->add((*iter));
				}

				continue;
			}
		}

		FEMap map;

		map.original = (*iter);

		MJMesh::Vertex *vertices[map.original->numVertices()];

		bool foundAll = true;

		for (UInt i = 0; i < map.original->numVertices(); i++)
		{
#if USE_C__11
			Data::VertexHashMap::iterator it = newVertices.find(map.original->getVertex(i)->getId());
#else
			Data::VertexMap::iterator it = newVertices.find(map.original->getVertex(i)->getId());
#endif //#if USE_C__11

			/*if (map.original->getVertex(i)->getId() == 384)
			{
				std::cout << "debug" << std::endl;
			}*/

			if (it == newVertices.end())
			{
				foundAll = false;

				if ((!this->noBoundary) || (!this->firstTime))
				{
					break;
				}

				vertices[i] = NULL;
			}
			else
			{
				vertices[i] = static_cast<MJMesh::Vertex *>((*it).second);
			}
		}

		if (!foundAll)
		{
			/*if ((this->noBoundary) && (this->firstTime))
			{
				for (UInt i = 0; i < map.original->numVertices(); i++)
				{
					if (!vertices[i])
					{
						continue;
					}

					this->sharedBoundaryVertices[map.original->getVertex(i)->getId()] = map.original->getVertex(i);
					//this->sharedBoundaryVertices[map.original->getVertex(i)->getId()] = originals[map.original->getVertex(i)->getId()];

					//MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>(map.original->getVertex(i));
					//MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>(originals[temp->getId()]);

					//map.original->setVertex(i, orig);

					//temp->remove(map.original);
					//orig->add(map.original);
				}
			}*/

			continue;
		}

		if ((this->noBoundary) && (this->firstTime) && (this->boundary->have(map.original)))
		{
			for (UInt i = 0; i < map.original->numVertices(); i++)
			{
				MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>(map.original->getVertex(i));
				MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>(originals[temp->getId()]);

				map.original->setVertex(i, orig);

				temp->remove(map.original);
				orig->add(map.original);
			}
		}

		//changes the direction of the front element
		map.temporary = (this->dimension == 2) ?
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(vertices[0], vertices[1], map.original->getId())) :
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(vertices[0], vertices[1], vertices[2], map.original->getId()));

#if USE_GUI
		map.temporary->setColor(map.original->getR(), map.original->getG(), map.original->getB());
#endif //#if USE_GUI

		//std::cout << "creating temporary fel " << map.temporary->text() << " for original fel " << map.original->text() << std::endl;

		if (addBoundary)
		{
			this->newBoundaryFrontElements.push_back(map);

			//std::cout << "adding temporary fel to this->boundary" << std::endl;

			this->boundary->add(map.temporary);
		}

		/*std::cout << "shared fel, original " << map.original->text() << std::endl;
		std::cout << "shared fel, temporary " << map.temporary->text() << std::endl;

		if (outside->have(map.original))
		{
			std::cout << "outside has original" << std::endl;
		}
		else
		{
			std::cout << "outside DOES NOT HAVE original" << std::endl;
		}

		if (mesh->have(map.original))
		{
			std::cout << "mesh has original" << std::endl;
		}
		else
		{
			std::cout << "mesh DOES NOT HAVE original" << std::endl;
		}*/

		outside->add(map.temporary);

		//std::cout << "adding temporary fel to outside" << std::endl;

		//Data::ElementList els = mesh->adjacency(map.original);

		//while (!els.empty())
		for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(map.original)->eBegin();
             iter != static_cast<MJMesh::FrontElement *>(map.original)->eEnd();)
		{
			//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
			MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

			//els.pop_front();

			if (outside->have(e))
			{
				//static_cast<MJMesh::FrontElement *>(map.original)->remove(e);
				iter = static_cast<MJMesh::FrontElement *>(map.original)->erase(iter);
				static_cast<MJMesh::FrontElement *>(map.temporary)->add(e);

				Data::Vertex *other = NULL;

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					if (!map.temporary->have(e->getVertex(i)))
					{
						other = e->getVertex(i);

						break;
					}
				}

				if (!map.temporary->accordingToNormal(other))
				{
					map.original->invert();
					map.temporary->invert();
				}
			}
			else
            {
                iter++;
            }
		}
	}
}

void PMJMesh::PartialMeshUpdater::split(const PMJMesh::Front *oldFront,
	const PMJMesh::Front *commonFront, const PMJMesh::Front *newFront,
	MJMesh::Mesh *mesh, MJMesh::Mesh *outside, MJMesh::Mesh *inside,
	bool addCommon, bool testExcept, bool addBoundary)
{
	Data::VertexList vertices;

	if ((!this->noBoundary) || (!this->firstTime))
	{
		Data::VertexList moreVertices = oldFront->getVertices();
		vertices.splice(vertices.end(), moreVertices);
	}

	Data::VertexList moreVertices = newFront->getVertices();
	vertices.splice(vertices.end(), moreVertices);

	if (addCommon)
	{
		Data::VertexList moreVertices = commonFront->getVertices();
		vertices.splice(vertices.end(), moreVertices);
	}

	MJMesh::Boundary sharedBoundary;

	Data::FrontList except;

	if (testExcept)
	{
		except.push_back(const_cast<PMJMesh::Front *>(oldFront));
		except.push_back(const_cast<PMJMesh::Front *>(commonFront));
	}

	/*for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		std::cout << "original mesh fel at " << (*iter) << " from mesh at " << mesh << std::endl;
		std::cout << "original mesh fel " << (*iter)->getId() << " from mesh at " << mesh << std::endl;
	}*/

	/*//debug
	for (Data::Mesh::VertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter++)
	{
		(*iter)->setColor(0.0, 1.0, 0.0);
	}

	for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		(*iter)->setColor(0.0, 1.0, 0.0);
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter++)
	{
		(*iter)->setColor(0.0, 1.0, 0.0);
	}
	//endebug*/

	mesh->split(vertices, this->smoothingLayer, outside, sharedBoundary, /*true, */except);

	//std::cout << "new split: sharedVertices: " << sharedVertices.size() << ", fels: " << sharedFels.size() << std::endl;

	/*for (Data::Mesh::ConstFrontElementIterator iter = outside->feBegin();
		 iter != outside->feEnd(); iter++)
	{
		std::cout << "(before replace) outside mesh fel at " << (*iter) << " from mesh at " << outside << std::endl;
		std::cout << "(before replace) outside mesh fel " << (*iter)->getId() << " from mesh at " << outside << std::endl;
	}*/

	/*//debug
	for (Data::Mesh::VertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter++)
	{
		(*iter)->setR(1.0);

		if (outside->have((*iter)))
		{
			(*iter)->setG(1.0);
		}
		else
		{
			(*iter)->setG(0.0);
		}

		if (sharedBoundary.have((*iter)))
		{
			(*iter)->setB(1.0);
		}
		else
		{
			(*iter)->setB(0.0);
		}
	}

	for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		(*iter)->setR(1.0);

		if (outside->have((*iter)))
		{
			(*iter)->setG(1.0);
		}
		else
		{
			(*iter)->setG(0.0);
		}

		if (sharedBoundary.have((*iter)))
		{
			(*iter)->setB(1.0);
		}
		else
		{
			(*iter)->setB(0.0);
		}
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter++)
	{
		(*iter)->setColor(1.0, 0.0, 0.0);
	}
	//endebug*/

	this->replace(mesh, outside, sharedBoundary, addBoundary);

	/*//debug
	for (Data::Boundary::VertexIterator iter = sharedBoundary.vBegin();
		 iter != sharedBoundary.vEnd(); iter++)
	{
		(*iter)->setB(1.0);

		if (outside->have((*iter)))
		{
			(*iter)->setG(1.0);
		}
		else
		{
			(*iter)->setG(0.0);
		}

		if (mesh->have((*iter)))
		{
			(*iter)->setR(1.0);
		}
		else
		{
			(*iter)->setR(0.0);
		}
	}

	for (Data::Boundary::FrontElementIterator iter = sharedBoundary.eBegin();
		 iter != sharedBoundary.eEnd(); iter++)
	{
		(*iter)->setB(1.0);

		if (outside->have((*iter)))
		{
			(*iter)->setG(1.0);
		}
		else
		{
			(*iter)->setG(0.0);
		}

		if (mesh->have((*iter)))
		{
			(*iter)->setR(1.0);
		}
		else
		{
			(*iter)->setR(0.0);
		}
	}

	//outside->merge(mesh);
	//debug*/

	sharedBoundary.clear();

	/*for (Data::Mesh::ConstFrontElementIterator iter = outside->feBegin();
		 iter != outside->feEnd(); iter++)
	{
		std::cout << "(after replace) outside mesh fel at " << (*iter) << " from mesh at " << outside << std::endl;
		std::cout << "(after replace) outside mesh fel " << (*iter)->getId() << " from mesh at " << outside << std::endl;
	}*/

	if (inside)
	{
		inside->merge(mesh, except);

		mesh->clear();
	}
	else
	{
		for (Data::FrontList::iterator iter = except.begin();
			 iter != except.end(); iter++)
		{
			/*if (iter == except.begin())
			{
				std::cout << "OLD front " << (*iter) << std::endl;
			}
			else
			{
				std::cout << "COMMON front " << (*iter) << std::endl;
			}*/

			PMJMesh::Front *front = static_cast<PMJMesh::Front *>((*iter));

			for (UShort i = 0; i < 4; i++)
			{
				for (Data::Front::ConstFrontElementIterator iter2 = front->eBegin(i);
					 iter2 != front->eEnd(i); iter2++)
				{
					//std::cout << "trying to access fel at " << (*iter2) << std::endl;
					//std::cout << "trying to access fel " << (*iter2)->getId() << std::endl;

					mesh->remove((*iter2), false);
				}
			}

			for (Data::Front::ConstVertexIterator iter2 = front->vBegin();
				 iter2 != front->vEnd(); iter2++)
			{
				mesh->remove((*iter2));
			}
		}

		/*for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
			 iter != mesh->feEnd(); iter++)
		{
			std::cout << "remaining mesh fel at " << (*iter) << " from mesh at " << mesh << std::endl;
			std::cout << "remaining mesh fel " << (*iter)->getId() << " from mesh at " << mesh << std::endl;
		}*/
	}
}

bool PMJMesh::PartialMeshUpdater::split(PMJMesh::WorkerMainDrive *drive,
	MJMesh::Mesh *mesh, MJMesh::Mesh *outside, MJMesh::Mesh *inside,
	bool addCommon, bool testExcept, bool addBoundary)
{
	PMJMesh::Front *oldFront = drive->getOldFront();

	if (!oldFront)
	{
		return false;
	}

#if USE_GUI
	mesh->freeze();
	outside->freeze();

	if (inside)
	{
		inside->freeze();
	}
#endif //#if USE_GUI

	PMJMesh::Front *commonFront = drive->getCommonFront();
	PMJMesh::Front *newFront = drive->getNewFront();

	this->split(oldFront, commonFront, newFront,
		mesh, outside, inside,
		addCommon, testExcept, addBoundary);

	if (!inside)
	{
		if ((mesh->size() > 0) ||
			(mesh->frontElementsSize() > 0) ||
			(mesh->verticesSize() > 0))
		{
			this->meshes.push_back(mesh);

			drive->setMesh(NULL);
		}
	}

#if USE_GUI
	mesh->unfreeze();
	outside->unfreeze();

	if (inside)
	{
		inside->unfreeze();
	}
#endif //#if USE_GUI

	return true;
}

void PMJMesh::PartialMeshUpdater::merge(MJMesh::Mesh *mesh, PMJMesh::Front *update,
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

		/*if (temp->getId() == 1859)
		{
			std::cout << "fel " << temp->text() << " is being tested in merge" << std::endl;
		}*/

#if USE_C__11
		Data::FrontElementHashMap::iterator it = felReplace.find(temp->getId());
#else
		Data::FrontElementMap::iterator it = felReplace.find(temp->getId());
#endif //#if USE_C__11

		MJMesh::FrontElement *orig = NULL;

		if (it == felReplace.end())
		{
			this->getMesh()->add(temp);

			/*if (temp->getId() == 1859)
			{
				std::cout << "fel " << temp->text() << " is not in felReplace" << std::endl;
			}*/

			if (((!update) || (!update->have(temp))) &&
				//((this->external) || (mesh->adjacency(temp).size() == 1)))
				((this->external) || (temp->numAdjacents() == 1)))
			{
				this->boundary->add(temp);

				this->oldFrontElements[temp->getId()] = temp;

				for (UInt i = 0; i < temp->numVertices(); i++)
				{
					Data::Vertex *v = temp->getVertex(i);

					/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
					{
						std::cout << "vertex " << v->text() << " being tested in merge (fels)" << std::endl;
					}*/

#if USE_C__11
					Data::VertexHashMap::iterator it2 = vReplace.find(v->getId());
#else
					Data::VertexMap::iterator it2 = vReplace.find(v->getId());
#endif //#if USE_C__11

					if (it2 != vReplace.end())
					{
						/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
						{
							std::cout << "vertex " << v->text() << " is in vReplace" << std::endl;
						}*/

						if (this->noBoundary)
						{
							this->sharedBoundaryVertices[v->getId()] = (*it2).second;

							/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
							{
								std::cout << "vertex " << v->text() << " being added to this->sharedBoundaryVertices" << std::endl;
							}*/
						}

						continue;
					}

					/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
					{
						std::cout << "vertex " << v->text() << " is being added to this->getMesh()" << std::endl;
					}*/

					//MJMesh::Mesh::add() checks for previously inserted vertices
					this->getMesh()->add(v, false);

					if (!this->boundary->have(v))
					{
						this->boundary->add(v);

						this->oldVertices[v->getId()] = v;

						/*if ((v->getId() == 156) || (v->getId() == 1109) || (v->getId() == 444) || (v->getId() == 1110))
						{
							std::cout << "vertex " << v->text() << " being added to this->boundary" << std::endl;
							std::cout << "vertex " << v->text() << " being added to this->oldVertices" << std::endl;
						}*/
					}
				}
			}
		}
		else
		{
			/*if (temp->getId() == 1859)
			{
				std::cout << "fel " << temp->text() << " is in felReplace as " << (*it).second->text() << std::endl;
				std::cout << "fel " << temp->text() << " is being replaced by orig" << std::endl;
			}*/

			orig = static_cast<MJMesh::FrontElement *>((*it).second);

			this->replace(temp, orig);

			/*if (temp->getId() == 1859)
			{
				std::cout << "original fel is now " << orig->text() << std::endl;
			}*/

			if ((update) && (update->have(temp)))
			{
				update->remove(temp);

				update->add(orig);

				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->text() << " is being added to update" << std::endl;
				}*/
			}
		}

		iter = mesh->erase(iter);

		if (orig)
		{
			/*if (temp->getId() == 1859)
			{
				std::cout << "fel " << temp->text() << " is being deleted because orig exists" << std::endl;
			}*/

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

		/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
		{
			std::cout << "vertex " << temp->text() << " is being tested in merge" << std::endl;
		}*/

		if (it == vReplace.end())
		{
			this->getMesh()->add(temp);

			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->text() << " is not in vReplace" << std::endl;
				std::cout << "vertex " << temp->text() << " is being added to mesh" << std::endl;
			}*/
		}
		else
		{
			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->text() << " is in vReplace as " << (*it).second->text() << std::endl;
				std::cout << "vertex " << temp->text() << " is being replaced by orig" << std::endl;
			}*/

			orig = static_cast<MJMesh::Vertex *>((*it).second);

			this->replace(temp, orig, true);

			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "original vertex is now " << orig->text() << std::endl;
			}*/

			if ((update) && (update->have(temp)))
			{
				update->remove(temp);

				update->add(orig);

				/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
				{
					std::cout << "vertex " << temp->text() << " is being added to update" << std::endl;
				}*/
			}
		}

		iter = mesh->erase(iter);

		if (orig)
		{
			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->text() << " being deleted because orig exists" << std::endl;
			}*/

			delete temp;
		}
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter = mesh->erase(iter))
	{
		this->getMesh()->add((*iter));
	}
}

void PMJMesh::PartialMeshUpdater::initializeShared()
{
	if ((this->useInnerMesh) && (!this->innerMesh))
	{
		this->innerMesh = new MJMesh::Mesh();
	}
}

void PMJMesh::PartialMeshUpdater::initializeMaster()
{

}

void PMJMesh::PartialMeshUpdater::initializeWorker()
{
	if ((this->useInnerMesh) && (!this->innerMesh))
	{
		this->innerMesh = new MJMesh::Mesh();
	}
}

void PMJMesh::PartialMeshUpdater::finalizeShared()
{
	if ((this->noBoundary) && (this->firstTime))
	{
		this->adjustSharedVerticesAdjacencies();
	}

	this->firstTime = false;
}

void PMJMesh::PartialMeshUpdater::finalizeMaster()
{
	if ((this->noBoundary) && (this->firstTime))
	{
		this->adjustSharedVerticesAdjacencies();
	}

	this->firstTime = false;
}

void PMJMesh::PartialMeshUpdater::finalizeWorker()
{
	this->firstTime = false;
}

void PMJMesh::PartialMeshUpdater::executeShared()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->masterTask->getBuilder());

	if (!drive)
	{
		return;
	}

	this->split(drive, static_cast<PMJMesh::MJMeshTask *>(this->masterTask)->getMesh(), this->getMesh(), this->innerMesh, false, true, true);

	if (this->noBoundary)
	{
		PMJMesh::Front *oldFront = drive->getOldFront();

		if (oldFront)
		{
			this->removeFromBoundary(oldFront, this->getMesh(), drive->getNewFront());
		}
	}

	this->updated = true;
}

void PMJMesh::PartialMeshUpdater::executeMaster()
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

	if (this->noBoundary)
	{
		this->removeFromBoundary(oldFront, this->getMesh(), newFront);
	}

	this->updated = true;
}

void PMJMesh::PartialMeshUpdater::executeWorker()
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->workerTask->getBuilder());

	if (!drive)
	{
		return;
	}

	MJMesh::Mesh *newMesh = new MJMesh::Mesh();

	MJMesh::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->workerTask)->getMesh();

	this->split(drive, mesh, newMesh, this->innerMesh, true, false, false);

	drive->setMesh(newMesh);

	if (this->useInnerMesh)
	{
		delete mesh;
	}

	this->updated = true;
}

void PMJMesh::PartialMeshUpdater::finalizeMeshShared()
{
#if USE_GUI
	this->boundary->freeze();
	this->getMesh()->freeze();
#endif //#if USE_GUI

	if (this->noBoundary)
	{
		this->readjustSharedVerticesAdjacencies();
	}

	while (!this->newBoundaryFrontElements.empty())
	{
		FEMap map = this->newBoundaryFrontElements.front();

		this->newBoundaryFrontElements.pop_front();

		//std::cout << "temporary fel " << map.temporary->text() << std::endl;

		if (map.original)
		{
			//std::cout << "original fel " << map.original->text() << std::endl;

			this->replace(static_cast<MJMesh::FrontElement *>(map.temporary), static_cast<MJMesh::FrontElement *>(map.original));
		}

		//std::cout << "removing temporary fel from this->boundary" << std::endl;
		//std::cout << "removing temporary fel from this->getMesh()" << std::endl;

		this->boundary->remove(map.temporary);
		//either was removed from adjacent vertices in this->replace() or will be removed in delete
		this->getMesh()->remove(map.temporary, false);

		//std::cout << "partial mesh updater, deleting temporary fel at " << map.temporary << std::endl;
		//std::cout << "partial mesh updater, deleting temporary fel " << map.temporary->getId() << std::endl;

		delete map.temporary;
	}

	while (!this->newBoundaryVertices.empty())
	{
		VMap map = this->newBoundaryVertices.front();

		this->newBoundaryVertices.pop_front();

		//std::cout << "updating boundary, original = " << map.original << ", temporary = " << map.temporary << std::endl;
		//std::cout << "updating boundary, original = " << map.original->text() << std::endl;
		//std::cout << "updating boundary, temporary = " << map.temporary->text() << std::endl;

		this->replace(static_cast<MJMesh::Vertex *>(map.temporary), static_cast<MJMesh::Vertex *>(map.original), false);

		this->boundary->remove(map.temporary);
		this->getMesh()->remove(map.temporary);

		map.temporary->setPoint(NULL);

		delete map.temporary;
	}

	if (this->innerMesh)
	{
		this->getMesh()->merge(this->innerMesh);
	}
	else
	{
		while (!this->meshes.empty())
		{
			MJMesh::Mesh *mesh = static_cast<MJMesh::Mesh *>(this->meshes.front());

			this->meshes.pop_front();

			/*for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
				 iter != mesh->feEnd(); iter++)
			{
				std::cout << "will merge mesh fel at " << (*iter) << " from mesh at " << mesh << std::endl;
				std::cout << "will merge mesh fel " << (*iter)->getId() << " from mesh at " << mesh << std::endl;
			}*/

			this->getMesh()->merge(mesh);

			mesh->clear();

			delete mesh;
		}
	}

#if USE_GUI
	this->boundary->unfreeze();
	this->getMesh()->unfreeze();
#endif //#if USE_GUI

	if (this->noBoundary)
	{
		this->addToBoundary();
	}
}

void PMJMesh::PartialMeshUpdater::finalizeMeshMaster()
{
	Parallel::Transferable **workersMeshes = NULL;
	Int numProcesses = this->comm->numProcesses();
	Int sizes[numProcesses - 1];
	Int numMeshes = 0;

	if (this->useInnerMesh)
	{
		workersMeshes = this->comm->gather();
		numMeshes = numProcesses - 1;
	}
	else
	{
		workersMeshes = this->comm->gatherv(sizes);

		for (Int i = 0; i < numProcesses - 1; i++)
		{
			numMeshes += sizes[i];
		}
	}

	Int processor = 0;
	Int meshOfProcessor = 0;

	for (Int j = 0; j < numMeshes; j++)
	{
#if USE_GUI
		this->getMesh()->freeze();
		this->boundary->freeze();
#endif //#if USE_GUI

		if (this->useInnerMesh)
		{
			processor++;
		}
		else
		{
			if (j == 0)
			{
				processor = 1;
			}

			while (meshOfProcessor == sizes[processor - 1])
			{
				processor++;

				meshOfProcessor = 0;
			}

			meshOfProcessor++;
		}

		Data::Mesh *mesh = static_cast<MeshTransferable *>(workersMeshes[j])->getMesh();

		if (!mesh)
		{
			//std::cout << "mesh from processor " << processor << " does not exist" << std::endl;

			continue;
		}

#if USE_THREAD_COLOR
		Real r, g, b;
		PMJMesh::WorkerMainDrive::getMeshColor(r, g, b, static_cast<PMJMesh::Communicator *>(this->comm), processor);
#endif //#if USE_THREAD_COLOR

		/*std::cout << "mesh from processor " << processor << " has " << mesh->verticesSize() << " vertices, "
				  << mesh->frontElementsSize() << " front elements and " << mesh->size() << " elements" << std::endl;*/

		///mesh->frontElements should be empty when this->external = true (or not, if this->noBoundary = true)
		for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
			 iter != mesh->feEnd();)
		{
			MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));
			MJMesh::FrontElement *original = NULL;

			//std::cout << "mesh from processor " << processor << " has fel " << temp->text() << std::endl;

			/*if (temp->getId() == 1859)
			{
				std::cout << "fel " << temp->getId() << " is being tested in finalizeMeshMaster" << std::endl;
			}*/

#if USE_THREAD_COLOR
			temp->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

#if USE_C__11
			Data::FrontElementHashMap::iterator it = this->oldFrontElements.find(temp->getId());
#else
			Data::FrontElementMap::iterator it = this->oldFrontElements.find(temp->getId());
#endif //#if USE_C__11

			if (it != this->oldFrontElements.end())
			{
				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->getId() << " is in this->oldFrontElements" << std::endl;
				}*/

				original = static_cast<MJMesh::FrontElement *>((*it).second);

				this->replace(temp, original);

				this->boundary->remove(original);

				//std::cout << "mesh from processor " << processor << " has fel " << temp->text() << " that was replaced by " << original->text() << " and removed from boundary" << std::endl;

				this->oldFrontElements.erase(it);
			}
			else if (this->noBoundary)
			{
				it = this->boundaryFrontElements.find(temp->getId());

				if (it != this->boundaryFrontElements.end())
				{
					/*if (temp->getId() == 1859)
					{
						std::cout << "fel " << temp->getId() << " is in this->boundaryFrontElements" << std::endl;
					}*/

					original = static_cast<MJMesh::FrontElement *>((*it).second);

					this->replace(temp, original);
				}
			}

			if (!original)
			{
				//std::cout << "mesh from processor " << processor << " has fel " << temp->text() << " that was added to mesh" << std::endl;

				this->getMesh()->add(temp);

				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->getId() << " is being added to mesh" << std::endl;
				}*/
			}

			iter = mesh->erase(iter);

			if (original)
			{
				//std::cout << "deleting temporary fel " << temp->text() << " at " << temp << std::endl;

				/*if (temp->getId() == 1859)
				{
					std::cout << "fel " << temp->getId() << " is being deleted because orig exists" << std::endl;
				}*/

				delete temp;
			}
		}

		for (Data::Mesh::VertexIterator iter = mesh->vBegin();
			 iter != mesh->vEnd();)
		{
			MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));
			MJMesh::Vertex *original = NULL;

#if USE_THREAD_COLOR
			temp->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

			/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
			{
				std::cout << "vertex " << temp->getId() << " is being tested in finalizeMeshMaster" << std::endl;
			}*/

#if USE_C__11
			Data::VertexHashMap::iterator it = this->oldVertices.find(temp->getId());
#else
			Data::VertexMap::iterator it = this->oldVertices.find(temp->getId());
#endif //#if USE_C__11

			if (it != this->oldVertices.end())
			{
				/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
				{
					std::cout << "vertex " << temp->getId() << " is in this->oldVertices" << std::endl;
				}*/

				original = static_cast<MJMesh::Vertex *>((*it).second);

				this->replace(temp, original, false);

				this->boundary->remove(original);

				this->oldVertices.erase(it);

				if (!this->getMesh()->haveInner(original))
				{
					/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
					{
						std::cout << "vertex " << temp->getId() << " is being added as inner" << std::endl;
					}*/

					this->getMesh()->addInner(original);
				}
			}
			else if (this->noBoundary)
			{
				it = this->boundaryVertices.find(temp->getId());

				if (it != this->boundaryVertices.end())
				{
					/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
					{
						std::cout << "vertex " << temp->getId() << " is in this->boundaryVertices" << std::endl;
					}*/

					original = static_cast<MJMesh::Vertex *>((*it).second);

					this->replace(temp, original, false);
				}
				else
				{
					it = this->sharedBoundaryVertices.find(temp->getId());

					if (it != this->sharedBoundaryVertices.end())
					{
						/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
						{
							std::cout << "vertex " << temp->getId() << " is in this->sharedBoundaryVertices" << std::endl;
						}*/

						original = static_cast<MJMesh::Vertex *>((*it).second);

						this->replace(temp, original, false);
					}
				}
			}

			if (!original)
			{
				this->getMesh()->add(temp);

				/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
				{
					std::cout << "vertex " << temp->getId() << " is being added to mesh" << std::endl;
				}*/
			}

			iter = mesh->erase(iter);

			if (original)
			{
				/*if ((temp->getId() == 156) || (temp->getId() == 1109) || (temp->getId() == 444) || (temp->getId() == 1110))
				{
					std::cout << "vertex " << temp->getId() << " is being deleted because orig exists" << std::endl;
				}*/

				delete temp;
			}
		}

		for (Data::Mesh::ElementIterator iter = mesh->eBegin();
			 iter != mesh->eEnd(); iter = mesh->erase(iter))
		{
			this->getMesh()->add((*iter));
		}

#if USE_GUI
		this->getMesh()->unfreeze();
		this->boundary->unfreeze();
#endif //#if USE_GUI

		delete mesh;

		delete workersMeshes[j];
	}

#if USE_GUI
	this->getMesh()->freeze();
	this->boundary->freeze();
#endif //#if USE_GUI

	///by now, this->oldFrontElements should be empty when this->external = false
#if USE_C__11
	for (Data::FrontElementHashMap::iterator iter = this->oldFrontElements.begin();
#else
	for (Data::FrontElementMap::iterator iter = this->oldFrontElements.begin();
#endif //#if USE_C__11
		 iter != this->oldFrontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter).second;

		/*if (fel->getId() == 1859)
		{
			std::cout << "fel " << fel->getId() << " is being tested in this->oldFrontElements" << std::endl;
		}*/

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			static_cast<MJMesh::Vertex *>(fel->getVertex(i))->remove(fel);
		}

		this->getMesh()->remove(fel);
		this->boundary->remove(fel);

		delete fel;
	}

	this->oldFrontElements.clear();

#if USE_GUI
	this->getMesh()->unfreeze();
	this->boundary->unfreeze();
#endif //#if USE_GUI

	delete [] workersMeshes;

	if (this->noBoundary)
	{
		this->addToBoundary();
	}
}

void PMJMesh::PartialMeshUpdater::finalizeMeshWorker()
{
	if (this->innerMesh)
	{
		MeshTransferable mt(this->dimension, this->innerMesh);

		this->comm->gather(&mt, this->comm->root());

#if USE_GUI
		Data::Mesh *mesh = this->innerMesh;

		this->innerMesh = NULL;

		delete mesh;
#else
		delete this->innerMesh;

		this->innerMesh = NULL;
#endif //#if USE_GUI
	}
	else
	{
		Parallel::Transferable **mt = new Parallel::Transferable*[this->meshes.size()];

		Int size = 0;

		for (Data::MeshList::iterator iter = this->meshes.begin();
			 iter != this->meshes.end(); iter++)
		{
			mt[size++] = new MeshTransferable(this->dimension, (*iter));
		}

		this->comm->gatherv(mt, size, this->comm->root());

		for (Int i = 0; i < size; i++)
		{
			delete mt[i];
		}

		delete [] mt;

		while (!this->meshes.empty())
		{
			delete this->meshes.front();

			this->meshes.pop_front();
		}
	}
}
