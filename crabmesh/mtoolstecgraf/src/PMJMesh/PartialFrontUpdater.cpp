#include "PMJMesh/PartialFrontUpdater.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/TaskManager.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "Parallel/Communicator.h"
#include "Parallel/StaticLoadBalancer.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "Performer/IdManager.h"
#include "Data/Boundary.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

PMJMesh::PartialFrontUpdater::PartialFrontUpdater(UInt dimension, UInt smoothingLayer,
	bool parallel, bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
	MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
	Parallel::Communicator *comm) :
	FullFrontUpdater(dimension, shared, boundary, front, mesh, taskManager, comm)
{
	this->setSmoothingLayer(smoothingLayer);

	this->setParallel(parallel);

	this->innerMesh = new MJMesh::Mesh();

	this->idBoundaryVertex = this->idBoundaryFrontElement = 0;
	this->setCreatedFels(false);

	this->process = 0;

	this->builtVMap = this->builtFEMap = false;
}

PMJMesh::PartialFrontUpdater::~PartialFrontUpdater()
{
	if (this->innerMesh)
	{
		delete this->innerMesh;
	}
}

void PMJMesh::PartialFrontUpdater::setParallel(bool parallel)
{
	this->parallel = parallel;
}

void PMJMesh::PartialFrontUpdater::setProcess(Int process)
{
	this->process = process;
}

void PMJMesh::PartialFrontUpdater::setSmoothingLayer(UInt smoothingLayer)
{
	this->smoothingLayer = smoothingLayer;
}

void PMJMesh::PartialFrontUpdater::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;

	this->idBoundaryVertex = this->idManager->getId(0);
	this->idBoundaryFrontElement = this->idManager->getId(1);
}

void PMJMesh::PartialFrontUpdater::setCreatedFels(bool createdFels)
{
	this->createdFels = createdFels;
}

MJMesh::Mesh *PMJMesh::PartialFrontUpdater::getMesh() const
{
	return this->mesh;
}

MJMesh::Mesh *PMJMesh::PartialFrontUpdater::getInnerMesh() const
{
	return this->innerMesh;
}

void PMJMesh::PartialFrontUpdater::initialize()
{
	FullFrontUpdater::initialize();

	if (this->shared)
	{
		this->buildBoundaryMaps();
	}
}

void PMJMesh::PartialFrontUpdater::finalize()
{
	if (this->shared)
	{
		this->clearBoundaryMaps();
	}

	FullFrontUpdater::finalize();
}

void PMJMesh::PartialFrontUpdater::finalizeMesh()
{
	if (this->shared)
	{
		this->finalizeMeshShared();
	}
	else if (this->comm->isMaster())
	{
		this->finalizeMeshMaster();
	}
	else
	{
		this->finalizeMeshWorker();
	}
}

/*void PMJMesh::PartialFrontUpdater::print(MJMesh::Mesh *mesh)
{
	std::cerr << "Mesh is:\n\n";
	std::cerr << "Vertices:\n";

	Data::VertexList vertices = mesh->getVertices();

	while (!vertices.empty())
	{
		MJMesh::Vertex *v = (MJMesh::Vertex *)vertices.front();

		vertices.pop_front();

		std::cerr << "Vertex " << v->getId() << " is located at (" << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ")\n";

		Data::FrontElementList adjFels = v->getAdjacentFrontElements();

		std::cerr << "Vertex " << v->getId() << " has " << adjFels.size() << " adjacent front elements:";

		while (!adjFels.empty())
		{
			Data::FrontElement *fel = adjFels.front();

			std::cerr << " " << fel->getId();

			adjFels.pop_front();
		}

		std::cerr << "\n";

		Data::ElementList adjEls = v->getAdjacentElements();

		std::cerr << "Vertex " << v->getId() << " has " << adjEls.size() << " adjacent elements:";

		while (!adjEls.empty())
		{
			Data::Element *e = adjEls.front();

			std::cerr << " " << e->getId();

			adjEls.pop_front();
		}

		std::cerr << "\n";

		std::cerr << "Vertex " << v->getId() << " has " << v->getAdjacency().size() << " adjacent edges\n";
	}

	std::cerr << "\n";

	Data::FrontElementList fels = mesh->getFrontElements();

	while (!fels.empty())
	{
		MJMesh::FrontElement *fel = (MJMesh::FrontElement *)fels.front();

		fels.pop_front();

		std::cerr << "Front element " << fel->getId() << " has vertices";

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			std::cerr << " " << fel->getVertex(i)->getId();
		}

		std::cerr << "\n";

		Data::ElementList adjEls = fel->getAdjacency();

		std::cerr << "Front element " << fel->getId() << " has " << adjEls.size() << " adjacent elements:";

		while (!adjEls.empty())
		{
			Data::Element *e = adjEls.front();

			std::cerr << " " << e->getId();

			adjEls.pop_front();
		}

		std::cerr << "\n";
	}

	Data::ElementList els = mesh->getElements();

	while (!els.empty())
	{
		MJMesh::Element *e = (MJMesh::Element *)els.front();

		els.pop_front();

		std::cerr << "Element " << e->getId() << " has vertices";

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			std::cerr << " " << e->getVertex(i)->getId();
		}

		std::cerr << "\n";
	}

	std::cerr << std::endl;
}*/

void PMJMesh::PartialFrontUpdater::buildBoundaryMaps()
{
	this->builtVMap = this->builtFEMap = false;

	if (this->boundary->isVerticesMapEmpty())
	{
		this->boundary->buildVerticesMap();

		this->builtVMap = true;
	}

	if (this->boundary->isElementsMapEmpty())
	{
		this->boundary->buildElementsMap();

		this->builtFEMap = true;
	}
}

void PMJMesh::PartialFrontUpdater::clearBoundaryMaps()
{
	if (this->builtVMap)
	{
		this->boundary->clearVerticesMap();
	}

	if (this->builtFEMap)
	{
		this->boundary->clearElementsMap();
	}

	this->builtVMap = this->builtFEMap = false;
}

Data::FrontElementList PMJMesh::PartialFrontUpdater::findAdjacentFrontElements(const MJMesh::Vertex *v, const Data::Element *e) const
{
	//Data::FrontElementList vAdjs = v->getAdjacentFrontElements();
	Data::FrontElementList adjs;

	//while (!vAdjs.empty())
	for (Data::FrontElementList::iterator iter = v->feBegin();
         iter != v->feEnd(); iter++)
	{
		//Data::FrontElement *fel = vAdjs.front();
		Data::FrontElement *fel = (*iter);

		//vAdjs.pop_front();

		UInt adjCount = 1;

		for (UInt j = 0; j < fel->numVertices(); j++)
		{
			if (fel->getVertex(j) == v)
			{
				continue;
			}

			if (e->have(fel->getVertex(j)))
			{
				adjCount++;
			}
		}

		if (adjCount == fel->numVertices())
		{
			adjs.push_back(fel);
		}
	}

	return adjs;
}

void PMJMesh::PartialFrontUpdater::restoreBoundary()
{
	/*Data::FrontElementList fels = this->boundary->getElements(), actualFels;*/

/*#if USE_GUI
	//do not render front elements that will possibly be deleted
	this->boundary->setElements(Data::FrontElementList());
#endif //#if USE_GUI*/

	//for (Data::FrontElementList::iterator iter = fels.begin();
	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
		 //iter != fels.end(); iter++)
		 iter != this->boundary->eEnd();)
	{
		Data::FrontElement *e = (*iter);

		//if (e->getId() <= this->idBoundaryFrontElement)
		if (e->getId() < this->idBoundaryFrontElement)
		{
			//actualFels.push_back(e);
			iter++;
		}
		else
		{
			if (this->createdFels)
			{
				this->mesh->remove(e);
			}

			iter = this->boundary->erase(iter);

			if (this->createdFels)
			{
				delete e;
			}
		}
	}

	//fels.clear();

	//this->boundary->setElements(actualFels);

	//actualFels.clear();

	//Data::VertexList vertices = this->boundary->getVertices(), actualVertices;
	//Data::VertexList innerVertices = this->mesh->getInnerVertices();
	Data::VertexList newInnerVertices;

	//for (Data::VertexList::iterator iter = vertices.begin();
	for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
		 //iter != vertices.end(); iter++)
		 iter != this->boundary->vEnd();)
	{
		Data::Vertex *v = (*iter);

		//if (v->getId() <= this->idBoundaryVertex)
		if (v->getId() < this->idBoundaryVertex)
		{
			//actualVertices.push_back(v);
			iter++;
		}
		else
		{
			//innerVertices.push_back(v);
			newInnerVertices.push_back(v);
			iter = this->boundary->erase(iter);
		}
	}

	//vertices.clear();

	//this->boundary->setVertices(actualVertices);

	//actualVertices.clear();

	this->boundary->clearMaps();

	//this->mesh->setInnerVertices(innerVertices);
	while (!newInnerVertices.empty())
	{
		this->mesh->addInner(newInnerVertices.front());

		newInnerVertices.pop_front();
	}

	//innerVertices.clear();
}

void PMJMesh::PartialFrontUpdater::add(Data::Mesh *mesh, MJMesh::Vertex *v, bool forceInner)
{
	//bool inner = (this->vMap[this->task->getId()].count(v->getId()) == 0);
	//bool inner = (this->vMap[this->process].count(v->getId()) == 0);
	bool inner = forceInner ? true : (this->vMap[this->process].count(v->getId()) == 0);

	mesh->add(v, inner);
}

void PMJMesh::PartialFrontUpdater::findNewBoundary(const MJMesh::Mesh *newMesh,
	const MJMesh::Front *newFront, const Data::FrontElementMap &felMap,
	Data::VertexList &newBoundaryVertices, Data::FrontElementList &newBoundaryElements)
{
	if (!newFront)
	{
		return;
	}

	//Data::FrontElementList fels = newFront->getAllElements();

	//Data::FrontElementSet front(fels.begin(), fels.end());

	//fels = newMesh->getFrontElements();

	Data::VertexSet vertices;

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = newMesh->feBegin();
		 iter != newMesh->feEnd(); iter++)
	{
		//Data::FrontElement *fel = fels.front();
		Data::FrontElement *fel = (*iter);

		//fels.pop_front();

		//Data::ElementList els = newMesh->adjacency(fel);

		//std::cout << "front element " << fel->getId() << " in task " << this->task->getId() << " has " << els.size() << " adjacent elements\n";

		//if (els.size() == 2)
		if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 2)
		{
			continue;
		}

		//Data::FrontElementSet::iterator it = front.find(fel);

		//if (it != front.end())
		if (newFront->have(fel))
		{
			//front.erase(it);

			continue;
		}

		if (felMap.find(fel->getId()) != felMap.end())
		{
			continue;
		}

		//std::cout << "adding front element " << fel->getId() << " (with " << els.size() << " adjacent elements) to boundary (from task "  << this->task->getId() << ")\n";

		this->felMap[this->process][fel->getId()] = fel;

		newBoundaryElements.push_back(fel);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			vertices.insert(fel->getVertex(i));
		}
	}

	Data::ElementSet elements;

	while (!vertices.empty())
	{
		Data::Vertex *v = *vertices.begin();

		vertices.erase(vertices.begin());

		//std::cout << "adding vertex " << v->getId() << " located at " << v->getPoint()->text() << " to boundary (from process "  << this->process << ")\n";

		this->vMap[this->process][v->getId()] = v;

		newBoundaryVertices.push_back(v);

		//Data::ElementList adjs = newMesh->adjacency(v);

		//elements.insert(adjs.begin(), adjs.end());
		elements.insert(static_cast<MJMesh::Vertex *>(v)->eBegin(), static_cast<MJMesh::Vertex *>(v)->eEnd());
	}

	while (!elements.empty())
	{
		Data::Element *e = *elements.begin();

		elements.erase(elements.begin());

		this->elMap[this->process][e->getId()] = e;
	}

	//std::cout << "ended task " << this->task->getId() << std::endl;
}

void PMJMesh::PartialFrontUpdater::splitMesh(const Data::FrontElementList &oldFront,
	const Data::FrontElementList &newFront, const Data::FrontElementList &commonFront,
	const MJMesh::Mesh *newMesh)
{
	Data::VertexSet boundaryVertices;
	Data::FrontElementSet boundaryFels;

	this->splitMesh(oldFront, newFront, commonFront, newMesh,
		boundaryVertices, boundaryFels,
		this->innerBoundaryFels, this->innerBoundaryEls);

	//debug
	//Real debugstart = Data::time();
	//endebug

	for (Data::VertexSet::iterator iter = boundaryVertices.begin();
		 iter != boundaryVertices.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		if (!this->boundary->have(v))
		{
			this->boundary->add(v);
		}
	}

	//std::cout << "PartialFrontUpdater, time for adding vertices in boundary = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	for (Data::FrontElementSet::iterator iter = boundaryFels.begin();
		 iter != boundaryFels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		if (!this->boundary->have(fel))
		{
			this->boundary->add(fel);
		}
	}

	//std::cout << "PartialFrontUpdater, time for adding elements in boundary = " << Data::time() - debugstart << std::endl;
}

void PMJMesh::PartialFrontUpdater::adjustElements(Data::ElementList &elements, const Data::ElementSet &in)
{
	Data::ElementList els;

	while (!elements.empty())
	{
		Data::Element *e = elements.front();

		elements.pop_front();

		if (in.find(e) != in.end())
		{
			els.push_back(e);
		}
	}

	elements.swap(els);
}

void PMJMesh::PartialFrontUpdater::adjustElements(Data::ElementList &elements, const Data::Mesh *mesh)
{
	for (Data::ElementList::iterator iter = elements.begin();
		 iter != elements.end();)
	{
		if (mesh->have((*iter)))
		{
			iter++;
		}
		else
		{
			iter = elements.erase(iter);
		}
	}

	/*Data::ElementList els;

	while (!elements.empty())
	{
		Data::Element *e = elements.front();

		elements.pop_front();

		if (mesh->have(e))
		{
			els.push_back(e);
		}
	}

	elements.swap(els);*/
}

bool PMJMesh::PartialFrontUpdater::insertInInnerBoundary(Data::Element *e, const Data::VertexSet &boundaryVertices) const
{
	bool insertInInnerBoundary = false;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

		if (boundaryVertices.find(v) != boundaryVertices.end())
		{
			v->remove(e);

			insertInInnerBoundary = true;
		}
	}

	return insertInInnerBoundary;
}

bool PMJMesh::PartialFrontUpdater::insertInInnerBoundary(Data::FrontElement *fel, const Data::VertexSet &boundaryVertices) const
{
	bool insertInInnerBoundary = false;

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

		if (boundaryVertices.find(v) != boundaryVertices.end())
		{
			v->remove(fel);

			insertInInnerBoundary = true;
		}
	}

	return insertInInnerBoundary;
}

void PMJMesh::PartialFrontUpdater::splitMesh(const Data::FrontElementList &oldFront,
	const Data::FrontElementList &newFront, const Data::FrontElementList &commonFront,
	const MJMesh::Mesh *newMesh,
	Data::VertexSet &boundaryVertices, Data::FrontElementSet &boundaryFels,
	Data::FrontElementSet &innerBoundaryFels, Data::ElementSet &innerBoundaryEls,
	bool worker)
{
	//debug
	//Real debugstart = Data::time();

	Data::VertexSet vs;

	for (Data::FrontElementList::const_iterator iter = oldFront.begin();
		 iter != oldFront.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::Vertex *v = fel->getVertex(i);

			vs.insert(v);
		}
	}

	//std::cout << "PartialFrontUpdater, time for oldFront = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	Data::VertexSet oldVertices(vs);

	for (Data::FrontElementList::const_iterator iter = newFront.begin();
		 iter != newFront.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			vs.insert(fel->getVertex(i));
		}
	}

	//std::cout << "PartialFrontUpdater, time for newFront = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	for (Data::FrontElementList::const_iterator iter = commonFront.begin();
		 iter != commonFront.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			oldVertices.insert(fel->getVertex(i));
		}
	}

	//std::cout << "PartialFrontUpdater, time for commonFront = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (worker)
	{
		for (Data::VertexSet::iterator iter = oldVertices.begin();
			 iter != oldVertices.end(); iter++)
		{
			this->mesh->add((*iter));
		}

		for (Data::FrontElementList::const_iterator iter = oldFront.begin();
			 iter != oldFront.end(); iter++)
		{
			this->mesh->add((*iter));
		}

		for (Data::FrontElementList::const_iterator iter = commonFront.begin();
			 iter != commonFront.end(); iter++)
		{
			this->mesh->add((*iter));
		}
	}

	//std::cout << "PartialFrontUpdater, time for adding to mesh = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	Data::VertexList vertices(vs.begin(), vs.end());

	vs.clear();

	Data::ElementSet els;

	//std::cout << "PartialFrontUpdater, time for lists = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	newMesh->findLayers(vertices, this->smoothingLayer, vs, els);

	//std::cout << "PartialFrontUpdater, time for finding layers = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//vertices = newMesh->getVertices();
	//Data::FrontElementList frontElements = newMesh->getFrontElements();
	//Data::ElementList elements = newMesh->getElements();

	//Data::ElementSet newMeshElements(elements.begin(), elements.end());

	//bool felsExist = ((frontElements.size()) != (oldFront.size() + newFront.size() + commonFront.size()));
	bool felsExist = ((newMesh->frontElementsSize()) != (oldFront.size() + newFront.size() + commonFront.size()));

	//std::cout << "PartialFrontUpdater, time for getting fels and els = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//while (!vertices.empty())
	for (Data::Mesh::ConstVertexIterator iter = newMesh->vBegin();
		 iter != newMesh->vEnd(); iter++)
	{
		//Data::Vertex *v = vertices.front();
		Data::Vertex *v = (*iter);

		//vertices.pop_front();

		if (oldVertices.find(v) != oldVertices.end())
		{
			continue;
		}

		Data::ElementList adjs = newMesh->adjacency(v);

		if (!worker)
		{
			//remove from the list of adjacent elements the ones that do not
			//  belong to the newMesh (this could happen because the vertex
			//  has pointers to the adjacent elements, even if one of them
			//  belongs to another task, but only in the shared memory version).

			//this->adjustElements(adjs, newMeshElements);
			this->adjustElements(adjs, newMesh);
		}

		Data::ElementList inLayer;

		for (Data::ElementList::iterator iter = adjs.begin();
			 iter != adjs.end(); iter++)
		{
			if (els.find((*iter)) != els.end())
			{
				inLayer.push_back((*iter));
			}
		}

		if (inLayer.empty())
		{
			this->innerMesh->add(v);

			continue;
		}

		bool inner = true;

		if (inLayer.size() != adjs.size())
		{
			boundaryVertices.insert(v);

			inner = false;

			///for pjmesh, because there is no front element adjacent to
			///  this vertex. creates the front elements that will belong to
			///  the temporary inner boundary
			///  TODO make it generic, it works only for tetrahedral elements
			if (!felsExist)
			{
				Data::FrontElementList adjFels = newMesh->findAdjacentFrontElements(v);

				Data::GeometricShape *s = (this->dimension == 2) ?
					static_cast<Data::GeometricShape *>(new Data::Edge2D()) :
					static_cast<Data::GeometricShape *>(new Data::Triangle3D());

				for (Data::ElementList::iterator iter = inLayer.begin();
					 iter != inLayer.end(); iter++)
				{
					Data::Element *e = (*iter);

					for (UInt i = 0; i < e->numVertices(); i++)
					{
						bool hasV = false;

						Data::Vertex *vl[this->dimension];

						for (UInt j = 0; j < this->dimension; j++)
						{
							vl[j] = e->getVertex((i + j)%e->numVertices());

							s->setPoint(j, vl[j]->getPoint());

							if (vl[j] == v)
							{
								hasV = true;
							}
						}

						if (!hasV)
						{
							continue;
						}

						if ((this->dimension == 3) && ((i%2) == 1))
						{
							Data::Point *p = s->getPoint(0);
							Data::Vertex *vtmp = vl[0];

							s->setPoint(0, s->getPoint(1));
							vl[0] = vl[1];

							s->setPoint(1, p);
							vl[1] = vtmp;
						}

						//sequence of vertices in test is a fel adjacent to v,
						//  and may or may not belong to the inner boundary

						bool isInner = false;

						for (Data::ElementList::iterator iter2 = inLayer.begin();
							 iter2 != inLayer.end(); iter2++)
						{
							Data::Element *e2 = (*iter2);

							if (e2 == e)
							{
								continue;
							}

							bool e2HasS = false;

							if (this->dimension == 2)
							{
								e2HasS = static_cast<MJMesh::Element2D *>(e2)->have(static_cast<Data::Edge2D *>(s));
							}
							else
							{
								e2HasS = static_cast<MJMesh::Element3D *>(e2)->have(static_cast<Data::Triangle3D *>(s));
							}

							if (e2HasS)
							{
								isInner = true;

								break;
							}
						}

						if (isInner)
						{
							continue;
						}

						bool exists = false;

						for (Data::FrontElementList::iterator iter2 = adjFels.begin();
							 iter2 != adjFels.end(); iter2++)
						{
							if ((*iter2)->getShape()->equal(s))
							{
								exists = true;

								break;
							}
						}

						if (exists)
						{
							continue;
						}

						Data::FrontElement *fel = NULL;

						if (this->dimension == 2)
						{
							fel = new MJMesh::FrontElement2D(
								static_cast<MJMesh::Vertex *>(vl[0]),
								static_cast<MJMesh::Vertex *>(vl[1]));
						}
						else
						{
							fel = new MJMesh::FrontElement3D(
								static_cast<MJMesh::Vertex *>(vl[0]),
								static_cast<MJMesh::Vertex *>(vl[1]),
								static_cast<MJMesh::Vertex *>(vl[2]));
						}

						fel->setId(this->idManager->next(1));

						//debug
						/*std::stringstream str;
						str << "process " << MPI::COMM_WORLD.Get_rank() << ", created fel " << fel->text() << std::endl;
						std::cout << str.str();*/
						//endebug

						this->createdFels = true;

						this->mesh->add(fel);

						boundaryFels.insert(fel);

						static_cast<MJMesh::FrontElement *>(fel)->add(static_cast<MJMesh::Element *>(e));
					}
				}

				delete s;
			}
		}

		this->mesh->add(v, inner);
	}

	//std::cout << "PartialFrontUpdater, time for dealing with vertices = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//while (!elements.empty())
	for (Data::Mesh::ConstElementIterator iter = newMesh->eBegin();
		 iter != newMesh->eEnd(); iter++)
	{
		//Data::Element *e = elements.front();
		Data::Element *e = (*iter);

		//elements.pop_front();

		//if the current element belongs to the layers that will be improved
		if (els.find(e) != els.end())
		{
			this->mesh->add(e);

			continue;
		}

		//if the current element belongs to the part of the mesh that was
		//  improved by some task

		this->innerMesh->add(e);

		if (this->insertInInnerBoundary(e, boundaryVertices))
		{
			innerBoundaryEls.insert(e);

			//std::cout << "element " << e->getId() << " " << e << " added to innerBoundaryEls" << std::endl;
		}
	}

	//std::cout << "PartialFrontUpdater, time for dealing with elements = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	Data::FrontElementSet oldAndCommonFrontSet(commonFront.begin(), commonFront.end());
	oldAndCommonFrontSet.insert(oldFront.begin(), oldFront.end());

	//while (!frontElements.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = newMesh->feBegin();
		 iter != newMesh->feEnd(); iter++)
	{
		//Data::FrontElement *fel = frontElements.front();
		Data::FrontElement *fel = (*iter);

		//frontElements.pop_front();

		//if the current front element already existed, then it was already
		//  added to the correct mesh

		if (oldAndCommonFrontSet.erase(fel) > 0)
		{
			continue;
		}

		Data::ElementList adjs = newMesh->adjacency(fel);

		/*{
			std::cout << "debug" << std::endl;
		}*/

		if (!worker)
		{
			//remove from the list of adjacent elements the ones that do not
			//  belong to the newMesh (this could happen because the front
			//  element has pointers to the adjacent elements, even if one of
			//  them belongs to another task, but only in the shared memory
			//  version).

			//this->adjustElements(adjs, newMeshElements);
			this->adjustElements(adjs, newMesh);
		}

		Data::ElementList inLayer;

		for (Data::ElementList::iterator iter = adjs.begin();
			 iter != adjs.end(); iter++)
		{
			if (els.find((*iter)) != els.end())
			{
				inLayer.push_back((*iter));
			}
		}

		if (inLayer.empty())
		{
			//none of the adjacent elements belong to the layers, meaning
			//  that this front element also does not belong to the layers
			//  that will be improved

			this->innerMesh->add(fel);

			if (this->insertInInnerBoundary(fel, boundaryVertices))
			{
				innerBoundaryFels.insert(fel);

				//std::cout << "fel " << fel->getId() << " " << fel << " added to innerBoundaryFels" << std::endl;
			}

			continue;
		}
		//at least one adjacent element belongs to the layers, meaning
		//  that this front element may belong to the layers

		this->mesh->add(fel);

		if (inLayer.size() != adjs.size())
		{
			//only one of the adjacent elements belongs to the layers, meaning
			//  that this front element belong to the newly formed boundary

			boundaryFels.insert(fel);

			if (!worker)
			{
				//adjs = static_cast<MJMesh::FrontElement *>(fel)->getAdjacency();

				//for (Data::ElementList::iterator iter = adjs.begin();
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
					 //iter != adjs.end(); iter++)
					 iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd();)
				{
					MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

					if (std::find(inLayer.begin(), inLayer.end(), e) == inLayer.end())
					{
						//static_cast<MJMesh::FrontElement *>(fel)->remove(e);
						iter = static_cast<MJMesh::FrontElement *>(fel)->erase(e);
					}
					else
                    {
                        iter++;
                    }
				}
			}
		}
	}

	//std::cout << "PartialFrontUpdater, time for dealing with front elements = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();
}

void PMJMesh::PartialFrontUpdater::updateMeshShared()
{
	MJMesh::Mesh *mesh = this->task->getMesh();

	//std::cout << "debug mesh got " << std::endl;

	if (mesh)
	{
		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

		//Data::FrontElementList oldFront = drive->getOldFront();
		//Data::FrontElementList newFront = drive->getNewFront();
		//Data::FrontElementList commonFront = drive->getCommonFront();

		//this->splitMesh(oldFront, newFront, commonFront, mesh);
		this->splitMesh(drive->getOldFront(), drive->getNewFront(), drive->getCommonFront(), mesh);

		mesh->clear();

		drive->setMesh(NULL);

		delete mesh;
	}
}

bool PMJMesh::PartialFrontUpdater::updateFrontShared()
{
	if (!this->innerMesh)
	{
		return false;
	}

	return FullFrontUpdater::updateFrontShared();
}

Data::Vertex *PMJMesh::PartialFrontUpdater::copy(Data::Vertex *v) const
{
	return (this->dimension == 2) ?
		new MJMesh::Vertex(new Data::Point2D(v->getPoint()), v->getId()) :
		new MJMesh::Vertex(new Data::Point3D(v->getPoint()), v->getId());
}

Data::FrontElement *PMJMesh::PartialFrontUpdater::copy(Data::FrontElement *fel, const Data::VertexMap &mapVertices) const
{
	Data::VertexVector vertices = fel->getVertices();

	for (UInt i = 0; i < vertices.size(); i++)
	{
		vertices[i] = (*mapVertices.find(vertices[i]->getId())).second;
	}

	return (this->dimension == 2) ?
		static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(static_cast<MJMesh::Vertex *>(vertices[0]), static_cast<MJMesh::Vertex *>(vertices[1]), fel->getId())) :
		static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(static_cast<MJMesh::Vertex *>(vertices[0]), static_cast<MJMesh::Vertex *>(vertices[1]), static_cast<MJMesh::Vertex *>(vertices[2]), fel->getId()));
}

bool PMJMesh::PartialFrontUpdater::updateFrontDistributed(Data::VertexMap &vMap, Data::FrontElementMap &felMap, bool /*forceInner*/)
{
	MJMesh::Mesh *mesh = this->task->getMesh();

	if (mesh)
	{
		Data::VertexList newBoundaryVertices;
		Data::FrontElementList newBoundaryFels;

		this->findNewBoundary(mesh, this->task->getFront(), felMap,
			newBoundaryVertices, newBoundaryFels);

		while (!newBoundaryVertices.empty())
		{
			//std::cout << "now adding vertex " << newBoundaryVertices.front()->text() << " to the boundary" << std::endl;

			this->boundary->add(newBoundaryVertices.front());

			newBoundaryVertices.pop_front();
		}

		while (!newBoundaryFels.empty())
		{
			this->boundary->add(newBoundaryFels.front());

			newBoundaryFels.pop_front();
		}
	}

	return FullFrontUpdater::updateFrontDistributed(vMap, felMap, false);
}

bool PMJMesh::PartialFrontUpdater::updateMeshMaster()
{
	return FullFrontUpdater::updateFrontDistributed();
}

bool PMJMesh::PartialFrontUpdater::updateMeshWorker()
{
	//std::cout << "executando PartialFrontUpdater::updateMeshWorker para task " << this->task->getId() << std::endl;

	this->mesh = new MJMesh::Mesh();

	//std::cout << "malha criada" << std::endl;

	MJMesh::Mesh *mesh = this->task->getMesh();

	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->task->getBuilder());

	//std::cerr << "generated mesh:\n";
	//mesh->print();

	//Data::FrontElementList oldFront = drive->getOldFront();
	//Data::FrontElementList newFront = drive->getNewFront();
	//Data::FrontElementList commonFront = drive->getCommonFront();

	Data::VertexSet boundaryVertices;
	Data::FrontElementSet boundaryFels;

	//this->splitMesh(oldFront, newFront, commonFront, mesh,
	this->splitMesh(drive->getOldFront(), drive->getNewFront(), drive->getCommonFront(),
		mesh, boundaryVertices, boundaryFels,
		this->innerBoundaryFels, this->innerBoundaryEls, true);

	mesh->clear();

	//std::cout << "malha dividida" << std::endl;

	Data::VertexMap mapVertices;
	Data::FrontElementMap mapFels;

	while (!boundaryVertices.empty())
	{
		Data::Vertex *oldV = *boundaryVertices.begin();

		boundaryVertices.erase(boundaryVertices.begin());

		Data::Vertex *v = this->copy(oldV);

		this->innerMesh->add(v);

		mapVertices[oldV->getId()] = v;

#if USE_GUI
		v->setColor(oldV->getR(), oldV->getG(), oldV->getB());
#endif //#if USE_GUI
	}

	//std::cout << "vertices copiados " << std::endl;

	while (!boundaryFels.empty())
	{
		MJMesh::FrontElement *oldFel = static_cast<MJMesh::FrontElement *>(*boundaryFels.begin());

		boundaryFels.erase(boundaryFels.begin());

		Data::VertexVector vertices = oldFel->getVertices();

		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(this->copy(oldFel, mapVertices));

		//std::cout << "elemento de fronteira " << oldFel->text() << " copiado: " << fel->text() << std::endl;

		this->innerMesh->add(fel);

		mapFels[oldFel->getId()] = fel;

		//Data::ElementList adjs = oldFel->getAdjacency();

		//while (!adjs.empty())
		for (Data::ElementList::iterator iter = oldFel->eBegin();
             iter != oldFel->eEnd();)
		{
			//MJMesh::Element *e = static_cast<MJMesh::Element *>(adjs.front());
			MJMesh::Element *e = static_cast<MJMesh::Element *>(adjs.front());

			adjs.pop_front();

			if (this->innerBoundaryEls.find(e) != this->innerBoundaryEls.end())
			{
				//oldFel->remove(e);

				fel->add(e);

				iter = oldFel->erase(iter);
			}
			else
            {
                iter++;
            }
		}

#if USE_GUI
		fel->setColor(oldFel->getR(), oldFel->getG(), oldFel->getB());
#endif //#if USE_GUI
	}

	//std::cout << "elementos de fronteira copiados " << std::endl;

	while (!this->innerBoundaryFels.empty())
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(*this->innerBoundaryFels.begin());

		this->innerBoundaryFels.erase(this->innerBoundaryFels.begin());

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			MJMesh::Vertex *oldV = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

			Data::VertexMap::iterator it = mapVertices.find(oldV->getId());

			if (it == mapVertices.end())
			{
				continue;
			}

			oldV->remove(fel);

			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*it).second);

			fel->setVertex(i, v);
			fel->getShape()->setPoint(i, v->getPoint());

			v->add(fel);
		}
	}

	//std::cout << "camadas de elementos de fronteira ajustadas " << std::endl;

	while (!this->innerBoundaryEls.empty())
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>(*this->innerBoundaryEls.begin());

		this->innerBoundaryEls.erase(this->innerBoundaryEls.begin());

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			MJMesh::Vertex *oldV = static_cast<MJMesh::Vertex *>(e->getVertex(i));

			Data::VertexMap::iterator it = mapVertices.find(oldV->getId());

			if (it == mapVertices.end())
			{
				continue;
			}

			oldV->remove(e);

			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*it).second);

			e->setVertex(i, v);
			e->getShape()->setPoint(i, v->getPoint());

			v->add(e);
		}
	}

	//std::cout << "camadas de elementos ajustadas " << std::endl;

	//std::cout << "size of mesh = " << mesh->size() << std::endl;
	//std::cout << "size of this->mesh (will be sent) = " << this->mesh->size() << std::endl;
	//std::cout << "size of this->innerMesh = " << this->innerMesh->size() << std::endl;

	//this->task->setMesh(this->mesh);
	drive->setMesh(this->mesh);

	//std::cerr << "processed mesh:\n";
	//mesh->print();

	//std::cerr << "new mesh in task:\n";
	//this->mesh->print();

	//std::cerr << "innerMesh:\n";
	//this->innerMesh->print();

	//mesh->clear();

	delete mesh;

	this->mesh = NULL;

	return true;
}

bool PMJMesh::PartialFrontUpdater::updateFrontDistributed()
{
	//std::cout << "executando PartialFrontUpdater::updateDistrubutedFront para task " << this->task->getId() << std::endl;

	bool updated = false;

	if (this->comm->isMaster())
	{
		updated = this->updateMeshMaster();
	}
	else
	{
		updated = this->updateMeshWorker();
	}

	return updated;
}

void PMJMesh::PartialFrontUpdater::finalizeMeshShared()
{
	while (!this->innerBoundaryFels.empty())
	{
		Data::FrontElement *e = *this->innerBoundaryFels.begin();

		//debug
		//std::cout << "finalizing fel " << e << std::endl;
		//std::cout << "finalizing fel " << e->getId() << std::endl;
		//endebug

		this->innerBoundaryFels.erase(this->innerBoundaryFels.begin());

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

			if (!v->have(e))
			{
				v->add(e);
			}
		}
	}

	while (!this->innerBoundaryEls.empty())
	{
		Data::Element *e = *this->innerBoundaryEls.begin();

		//debug
		//std::cout << "finalizing element " << e << std::endl;
		//std::cout << "finalizing element " << e->getId() << std::endl;
		//endebug

		this->innerBoundaryEls.erase(this->innerBoundaryEls.begin());

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

			if (!v->have(e))
			{
				v->add(e);
			}
		}
	}

	//Data::FrontElementList fels = this->boundary->getElements();

	//for (Data::FrontElementList::iterator iter = fels.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 //iter != fels.end(); iter++)
		 iter != this->boundary->eEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		//if (fel->getId() > this->idBoundaryFrontElement)
		if (fel->getId() >= this->idBoundaryFrontElement)
		{
			Data::ElementList els;

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				Data::ElementList adjs = this->mesh->adjacency(fel->getVertex(i));

				if (i == 0)
				{
					els.swap(adjs);
				}
				else
				{
					for (Data::ElementList::iterator iter = els.begin();
						 iter != els.end();)
					{
						Data::Element *e = (*iter);

						if (std::find(adjs.begin(), adjs.end(), e) != adjs.end())
						{
							iter++;
						}
						else
						{
							iter = els.erase(iter);
						}
					}
				}
			}

			for (Data::ElementList::iterator iter = els.begin();
				 iter != els.end(); iter++)
			{
				MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

				if (!static_cast<MJMesh::FrontElement *>(fel)->haveAdjacent(e))
				{
					static_cast<MJMesh::FrontElement *>(fel)->add(e);
				}
			}
		}
	}

	this->restoreBoundary();

	//Data::ElementList els = this->innerMesh->getElements();

	//while (!els.empty())
	for (Data::Mesh::ConstElementIterator iter = this->innerMesh->eBegin();
		 iter != this->innerMesh->eEnd(); iter++)
	{
		//this->mesh->add(els.front());
		this->mesh->add((*iter));

		//els.pop_front();
	}

	//Data::FrontElementList mfels = this->innerMesh->getFrontElements();

	//while (!mfels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = this->innerMesh->feBegin();
		 iter != this->innerMesh->feEnd(); iter++)
	{
		//this->mesh->add(mfels.front());
		this->mesh->add((*iter));

		//mfels.pop_front();
	}

	//Data::VertexList vertices = this->innerMesh->getVertices();

	//while (!vertices.empty())
	for (Data::Mesh::ConstVertexIterator iter = this->innerMesh->vBegin();
		 iter != this->innerMesh->vEnd(); iter++)
	{
		//this->mesh->add(vertices.front());
		this->mesh->add((*iter));

		//vertices.pop_front();
	}

	this->innerMesh->clear();
}

void PMJMesh::PartialFrontUpdater::finalizeMeshMaster()
{
	Parallel::LoadBalancer *lb = new Parallel::StaticLoadBalancer(this->comm);

	//std::cout << "finalize mesh master, receiving from slaves" << std::endl;

	lb->receiveFromSlaves();

	//std::cout << "finalize mesh master, received from slaves" << std::endl;

	Parallel::TaskVector tasks = lb->getTasks();

	while (!tasks.empty())
	{
		this->task = static_cast<PMJMesh::MJMeshTask *>(tasks.back());

#if USE_THREAD_COLOR
		dynamic_cast<WorkerMainDrive *>(this->task->getBuilder())->colorizeMesh(lb->getProcess(this->task));
#endif //#if USE_THREAD_COLOR

		tasks.pop_back();

		MJMesh::Mesh *mesh = this->task->getMesh();

		if (mesh)
		{
			this->process = lb->getProcess(this->task);

			//Data::VertexList vertices = mesh->getVertices();
			Data::VertexList vertices;

			//std::cout << "debug mesh->vertices->size = " << vertices.size() << std::endl;

			//this->updateMesh(vertices, this->vMap[this->process], this->felMap[this->process], true);
			this->updateMeshVertices(mesh, vertices, this->vMap[this->process], this->felMap[this->process], true);

			//this->clear(vertices/*, NULL*/);

			//Data::FrontElementList fels = mesh->getFrontElements();
			Data::FrontElementList fels;

			//std::cout << "debug mesh->fels->size = " << fels.size() << std::endl;

			//this->updateMesh(fels, this->felMap[this->process]);
			this->updateMeshFrontElements(mesh, fels, this->felMap[this->process]);

			//this->clear(fels/*, NULL*/);

			//Data::ElementList els = mesh->getElements();

			//std::cout << "debug mesh->els->size = " << els.size() << std::endl;

			//this->updateMesh(els);
			this->updateMeshElements(mesh);

			mesh->clear();

			this->clear(vertices/*, NULL*/);

			this->clear(fels/*, NULL*/);
		}

		delete this->task;
	}

	this->restoreBoundary();

	this->vMap.clear();
	this->felMap.clear();
	this->elMap.clear();

	delete lb;
}

void PMJMesh::PartialFrontUpdater::finalizeMeshWorker()
{
	PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->taskManager->makeTask());
	t->setComm(this->comm);

	t->makeBuilder();
	PMJMesh::WorkerMainDrive *d = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());
	d->setDimension(this->dimension);

	d->setMesh(this->innerMesh);

	t->setId(0);

	this->innerMesh = NULL;

	Parallel::LoadBalancer *lb = new Parallel::StaticLoadBalancer(this->comm);

	Parallel::TaskVector tasks(1, t);

	lb->setTasks(tasks);

	//std::cout << "finalize mesh worker, sending to master, process " << this->comm->rank() << std::endl;

	lb->sendToMaster();

	//std::cout << "finalize mesh worker, sent to master, process " << this->comm->rank() << std::endl;

	delete lb;
}

#endif //#if (!USE_NEW_FRONT_UPDATER)
