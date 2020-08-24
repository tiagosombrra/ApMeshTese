#include "PMJMesh/WorkerMainDrive.h"

#include "Performer/IdManager.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Mesh.h"
#include "Parallel/Communicator.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/WorkerMinTreeRefiner.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/TopologyFrontAdvancer.h"
#include "PMJMesh/WorkerMeshImprover.h"
#include "PMJMesh/WorkerCellClassifier.h"
#include "PMJMesh/WorkerCellClassifier2.h"
#include "PMJMesh/WorkerMinTreeDisrefiner.h"
#include "PMJMesh/Communicator.h"

using namespace PMJMesh;

const Performer::Status PMJMesh::WorkerMainDrive::errorMinTreeRefinement =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_MIN_TREE_REFINEMENT, "error - min tree refinement did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorFrontGeometryAdvancer =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_FRONT_GEOMETRY_ADVANCER, "error - geometry advancing front did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorFrontTopologyAdvancer =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_FRONT_TOPOLOGY_ADVANCER, "error - topology advancing front did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorFrontBackTrackingAdvancer =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_FRONT_BACK_TRACKING_ADVANCER, "error - back tracking advancing front did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorMeshImprover =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_MESH_IMPROVER, "error - mesh improvement did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorTreeClassifier =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_TREE_CLASSIFIER, "error - tree classification did not work in worker task");
const Performer::Status PMJMesh::WorkerMainDrive::errorTreeDisrefiner =
	Performer::Status(PMJMesh::WorkerMainDrive::ERROR_TREE_DISREFINER, "error - tree disrefinement did not work in worker task");

PMJMesh::WorkerMainDrive::WorkerMainDrive(Parallel::Communicator *comm, UInt status) :
	MJMesh::MainDrive(), Data::Identifiable()
{
	this->setComm(comm);

	this->setTestingBox(true);
	this->setRangeProportion(1.0);
	this->setSmoothInSlavesEnabled(false);
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(0);
#endif //#if (!USE_DELETE_FELS)
	this->setMinRefinementLevel(0);
#if (!USE_DELETE_FELS)
	this->setRemoveBoundaryFelsForSmoothing(false);
#endif //#if (!USE_DELETE_FELS)
	this->setClassifyTreeCells(true);
	this->setFindingCommonFront(true);

	this->setBox(NULL);

	this->idManager = NULL;

	this->classifier = NULL;
	this->minTreeDisrefiner = NULL;

	this->inverted = false;

	this->refinedCells = NULL;

#if USE_NEW_FRONT_UPDATER
	this->oldFront = NULL;
	this->commonFront = NULL;
#endif //#if USE_NEW_FRONT_UPDATER

	if (status >= WorkerMainDrive::error.getType())
	{
		switch (status)
		{
		case WorkerMainDrive::ERROR:
			this->status = WorkerMainDrive::error;
			break;
		case WorkerMainDrive::ERROR_DIMENSION:
			this->status = WorkerMainDrive::errorDimension;
			break;
		case WorkerMainDrive::ERROR_POINT_OUT_OF_TREE:
			this->status = WorkerMainDrive::errorPointOutOfTree;
			break;
		case WorkerMainDrive::ERROR_MESH_NOT_GENERATED_TOPOLOGY:
			this->status = WorkerMainDrive::errorMeshNotGeneratedTopology;
			break;
		case WorkerMainDrive::ERROR_MESH_NOT_GENERATED_BACK_TRACKING:
			this->status = WorkerMainDrive::errorMeshNotGeneratedBackTracking;
			break;
		}
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement = 0.0;
	this->timeTrees = 0.0;
	this->timeGeometry = 0.0;
	this->timeTopology = 0.0;
	this->timeBackTracking = 0.0;
	this->timeClassify = 0.0;
	this->timeImprover = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PMJMesh::WorkerMainDrive::~WorkerMainDrive()
{
	if (this->classifier)
	{
		delete this->classifier;
	}

	if (this->minTreeDisrefiner)
	{
		delete this->minTreeDisrefiner;
	}

	//same for all WorkerMainDrivers
	this->boundary = NULL;
	this->tree = NULL;
	//shared with (and deleted by) PMJMesh::Partition (not anymore)
	//this->front = NULL;

#if USE_NEW_FRONT_UPDATER
	if (this->oldFront)
	{
		delete this->oldFront;
	}

	if (this->commonFront)
	{
		delete this->commonFront;
	}
#endif //#if USE_NEW_FRONT_UPDATER
}

void PMJMesh::WorkerMainDrive::setComm(Parallel::Communicator *comm)
{
	this->comm = comm;
}

void PMJMesh::WorkerMainDrive::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::WorkerMainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMJMesh::WorkerMainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

void PMJMesh::WorkerMainDrive::setFront(PMJMesh::Front *front)
{
	this->front = front;
}

void PMJMesh::WorkerMainDrive::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

void PMJMesh::WorkerMainDrive::setBox(Data::Box *box)
{
	this->box = box;
}

void PMJMesh::WorkerMainDrive::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

void PMJMesh::WorkerMainDrive::setTestingBox(bool testingBox)
{
	this->testingBox = testingBox;
}

void PMJMesh::WorkerMainDrive::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

void PMJMesh::WorkerMainDrive::setSmoothInSlavesEnabled(bool smoothInSlavesEnabled)
{
	this->smoothInSlavesEnabled = smoothInSlavesEnabled;
}

#if (!USE_DELETE_FELS)
void PMJMesh::WorkerMainDrive::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::WorkerMainDrive::setMinRefinementLevel(UInt minRefinementLevel)
{
	this->minRefinementLevel = minRefinementLevel;
}

#if (!USE_DELETE_FELS)
void PMJMesh::WorkerMainDrive::setRemoveBoundaryFelsForSmoothing(bool removeBoundaryFelsForSmoothing)
{
	this->removeBoundaryFelsForSmoothing = removeBoundaryFelsForSmoothing;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::WorkerMainDrive::setClassifyTreeCells(bool classifyTreeCells)
{
	this->classifyTreeCells = classifyTreeCells;
}

void PMJMesh::WorkerMainDrive::setFindingCommonFront(bool findingCommonFront)
{
    this->findingCommonFront = findingCommonFront;
}

#if USE_NEW_FRONT_UPDATER
void PMJMesh::WorkerMainDrive::setOldFront(PMJMesh::Front *oldFront)
#else
void PMJMesh::WorkerMainDrive::setOldFront(const Data::FrontElementList &oldFront)
#endif //#if USE_NEW_FRONT_UPDATER
{
	this->oldFront = oldFront;
}

#if USE_NEW_FRONT_UPDATER
PMJMesh::Front *PMJMesh::WorkerMainDrive::getOldFront() const
#else
const Data::FrontElementList &PMJMesh::WorkerMainDrive::getOldFront() const
#endif //#if USE_NEW_FRONT_UPDATER
{
	return this->oldFront;
}

#if USE_NEW_FRONT_UPDATER
void PMJMesh::WorkerMainDrive::setNewFront(PMJMesh::Front *newFront)
#else
void PMJMesh::WorkerMainDrive::setNewFront(const Data::FrontElementList &newFront)
#endif //#if USE_NEW_FRONT_UPDATER
{
#if USE_NEW_FRONT_UPDATER
	this->front = newFront;
#else
	this->newFront = newFront;
#endif //#if USE_NEW_FRONT_UPDATER
}

#if USE_NEW_FRONT_UPDATER
PMJMesh::Front *PMJMesh::WorkerMainDrive::getNewFront() const
#else
const Data::FrontElementList &PMJMesh::WorkerMainDrive::getNewFront() const
#endif //#if USE_NEW_FRONT_UPDATER
{
#if USE_NEW_FRONT_UPDATER
	return static_cast<PMJMesh::Front *>(this->front);
#else
	return this->newFront;
#endif //#if USE_NEW_FRONT_UPDATER
}

#if USE_NEW_FRONT_UPDATER
void PMJMesh::WorkerMainDrive::setCommonFront(PMJMesh::Front *commonFront)
#else
void PMJMesh::WorkerMainDrive::setCommonFront(const Data::FrontElementList &commonFront)
#endif //#if USE_NEW_FRONT_UPDATERs
{
	this->commonFront = commonFront;
}

#if USE_NEW_FRONT_UPDATER
PMJMesh::Front *PMJMesh::WorkerMainDrive::getCommonFront() const
#else
const Data::FrontElementList &PMJMesh::WorkerMainDrive::getCommonFront() const
#endif //#if USE_NEW_FRONT_UPDATER
{
	return this->commonFront;
}

//debug
#include "Data/Box.h"
#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"
//endebug

std::string PMJMesh::WorkerMainDrive::debug(const Data::BoxTreeCell *cell) const
{
	std::stringstream str;

	bool out = false;

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (((cell->box()->getMin()->getCoord(i) <  this->box->getMin()->getCoord(i)) &&
			 (cell->box()->getMax()->getCoord(i) <= this->box->getMin()->getCoord(i))) ||
			((cell->box()->getMax()->getCoord(i) >  this->box->getMax()->getCoord(i)) &&
			 (cell->box()->getMin()->getCoord(i) >= this->box->getMax()->getCoord(i))))
		{
			out = true;

			break;
		}
	}

	if (out)
	{
		str << "cell " << cell->getId() << " is OUT box" << std::endl;

		return str.str();
	}

	//if (cell->box()->on(this->box))
	if (cell->box()->optOn(this->box))
	{
		str << "cell " << cell->getId() << " is ON box" << std::endl;
	}
	else
	{
		str << "cell " << cell->getId() << " is IN box" << std::endl;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		str << this->debug(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}

	return str.str();
}

std::string PMJMesh::WorkerMainDrive::debug() const
{
	std::stringstream str;

	return str.str();

	str << "begining" << std::endl;

	str << this->debug(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));

	str << "ending" << std::endl;

	return str.str();
}

//debug
#include "MJMesh/Boundary.h"
//endebug

void PMJMesh::WorkerMainDrive::execute()
/*{
	static int c = 0;
	++c;

	if (c > 4) {executer(); return;}

	switch (this->getId())
	{
	case 1:
		this->execute1();
		break;
	case 2:
		this->execute2();
		break;
	case 3:
		this->execute3();
		break;
	case 4:
		this->execute4();
		break;
	default:
		this->executer();
		return;
	}
}

void PMJMesh::WorkerMainDrive::execute1()
{
	this->executer();
}

void PMJMesh::WorkerMainDrive::execute2()
{
	this->executer();
}

void PMJMesh::WorkerMainDrive::execute3()
{
	this->executer();
}

void PMJMesh::WorkerMainDrive::execute4()
{
	this->executer();
}

//void PMJMesh::WorkerMainDrive::execute()
void PMJMesh::WorkerMainDrive::executer()*/
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement = 0.0;
	this->timeTrees = 0.0;
	this->timeGeometry = 0.0;
	this->timeTopology = 0.0;
	this->timeBackTracking = 0.0;
	this->timeClassify = 0.0;
	this->timeImprover = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->status = this->init;

	if (!this->executeTreeMinRefiner(true))
	{
		return;
	}

	if (!this->executeFrontGeometryAdvancer())
	{
#if USE_THREAD_COLOR
		this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

		return;
	}

	if (!this->executeFrontTopologyAdvancer())
	{
#if USE_THREAD_COLOR
		this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

		return;
	}

	if (!this->executeFrontBackTrackingAdvancer())
	{
#if USE_THREAD_COLOR
		this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

		return;
	}

	if (!this->executeMeshImprover())
	{
#if USE_THREAD_COLOR
		this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

		return;
	}

	if (!this->executeTreeClassifier())
	{
#if USE_THREAD_COLOR
		this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

		return;
	}

#if USE_THREAD_COLOR
	this->colorizeMesh();
#endif //#if USE_THREAD_COLOR

	if (!this->executeTreeDisrefiner())
	{
		return;
	}

	this->status = this->success;
}

void PMJMesh::WorkerMainDrive::findCommonFront()
{
    if (!this->findingCommonFront)
    {
        return;
    }

#if USE_NEW_FRONT_UPDATER
    /*if (this->getId() == 448)
    {
        std::cout << "worker main drive " << this->getId() << std::endl;
    }*/

	this->commonFront = static_cast<PMJMesh::Front *>(this->front)->setIntersection(this->oldFront);
#else
	Data::FrontElementList oldFront;

	this->oldFront.swap(oldFront);

	Data::FrontElementSet newFront(this->newFront.begin(), this->newFront.end());

	this->newFront.clear();

	this->commonFront.clear();

	while (!oldFront.empty())
	{
		Data::FrontElement *e = oldFront.front();
		oldFront.pop_front();

		//Data::FrontElementList::iterator it = std::find(this->newFront.begin(), this->newFront.end(), e);
		Data::FrontElementSet::iterator it = newFront.find(e);

		//if (it != this->newFront.end())
		if (it != newFront.end())
		{
			this->commonFront.push_back(e);

			//this->newFront.erase(it);
			newFront.erase(it);
		}
		else
		{
			this->oldFront.push_back(e);
		}
	}

	this->newFront.insert(this->newFront.end(), newFront.begin(), newFront.end());

	/*//debug
	if (this->comm->isMaster())
	{
		this->oldFrontElements.clear();

		for (Data::FrontElementList::iterator iter = this->oldFront.begin();
			 iter != this->oldFront.end(); iter++)
		{
			Data::ElementList adj = this->mesh->adjacency((*iter));

			if (!adj.empty())
			{
				this->oldFrontElements[(*iter)] = adj;

				static_cast<MJMesh::FrontElement *>((*iter))->clearAdjacency();
			}
		}
	}
	//endebug*/
#endif //#if USE_NEW_FRONT_UPDATER
}

void PMJMesh::WorkerMainDrive::invertNewFront()
{
	this->inverted = !this->inverted;

#if USE_NEW_FRONT_UPDATER
	for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin(), end = this->front->allEEnd();
		 iter != end; iter++)
#else
	for (Data::FrontElementList::iterator iter = this->newFront.begin();
		 iter != this->newFront.end(); iter++)
#endif //#if USE_NEW_FRONT_UPDATER
	{
		(*iter)->invert();
	}
}

bool PMJMesh::WorkerMainDrive::executeTreeMinRefiner(bool /*del*/)
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->minTreeRefiner = this->makeTreeMinRefiner();

	this->minTreeRefiner->execute();

	this->refinedCells = static_cast<PMJMesh::WorkerMinTreeRefiner *>(this->minTreeRefiner)->getRefinedCells();

	//bool ok = (this->minTreeRefiner->getStatus() < this->minTreeRefiner->error);
	bool ok = true;

	if (!ok)
	{
		this->status = this->errorMinTreeRefinement;

		this->status.append(this->minTreeRefiner->getStatus().getMessage());
	}

	delete this->minTreeRefiner;

	this->minTreeRefiner = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeFrontGeometryAdvancer()
{
	if (!this->geometryAdvancer)
	{
		this->geometryAdvancer = this->makeFrontGeometryAdvancer();
	}

	if (this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::notInit.getType())
	{
		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setBox(this->box);

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setFront(this->front);

		if (this->front)
		{
#if USE_NEW_FRONT_UPDATER
			if (!this->oldFront)
            {
                this->oldFront = new PMJMesh::Front(this->front->isSorted());

                this->oldFront->set(this->front);

#if USE_DELETE_FELS
                static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setOldFront(this->oldFront);
#endif //#if USE_DELETE_FELS
            }
#else
			this->oldFront = static_cast<PMJMesh::Front *>(this->front)->getAllElements();
#endif //#if USE_NEW_FRONT_UPDATER

			//Data::FrontElementList crossing = static_cast<PMJMesh::Front *>(this->front)->getCrossing();

			//this->oldFront.insert(this->oldFront.end(), crossing.begin(), crossing.end());
		}
	}

	this->geometryAdvancer->setIdManager(this->idManager);

	//this->crossing.clear();

	this->geometryAdvancer->execute();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTrees += static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->timeTrees;
	this->timeGeometry += static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->timeGeometry;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if ((this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::ended.getType()) ||
		(this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::rejected.getType()) ||
		(this->geometryAdvancer->getStatus().getType() >= MJMesh::GeometryFrontAdvancer::error.getType()))
	{
		//delete ((PMJMesh::GeometryFrontAdvancer *)this->builder)->getBox();

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setBox(NULL);

		//this->crossing = static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getCrossing();

		this->mesh = static_cast<MJMesh::Mesh *>(static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getMesh());
		this->front = static_cast<PMJMesh::Front *>(static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getFront());

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setMesh(NULL);
		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setFront(NULL);
	}

	bool ok = (this->geometryAdvancer->getStatus().getType() < MJMesh::GeometryFrontAdvancer::error.getType());

	if (!ok)
	{
		this->status = this->errorFrontGeometryAdvancer;

		this->status.append(this->geometryAdvancer->getStatus());
	}

	delete this->geometryAdvancer;

	this->geometryAdvancer = NULL;

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeFrontTopologyAdvancer()
{
/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTopology = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

	if (!this->front)
	{
		return true;
	}

	if (this->front->isEmpty())
	{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		//this method deletes the search trees
		this->front->prepareForTopologyPhase(false);

		this->front->clearSearchTrees();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeTrees += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return true;
	}

	/*if (this->getId() == 18)
	{
		std::cout << "debug" << std::endl;
	}*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (!this->topologyAdvancer)
	{
		this->topologyAdvancer = this->makeFrontTopologyAdvancer();

#if USE_DELETE_FELS
        static_cast<PMJMesh::TopologyFrontAdvancer *>(this->topologyAdvancer)->setOldFront(this->oldFront);
#endif //#if USE_DELETE_FELS
	}

	this->topologyAdvancer->setIdManager(this->idManager);

	this->topologyAdvancer->execute();

	bool rejected = (this->topologyAdvancer->getStatus().getType() == this->topologyAdvancer->rejected.getType());
	bool error = (this->topologyAdvancer->getStatus().getType() >= this->topologyAdvancer->error.getType());

	bool ok = ((!error) && (((!rejected) || (this->dimension != 2))));

	if (!ok)
	{
		this->status = this->errorFrontTopologyAdvancer;

		this->status.append(this->topologyAdvancer->getStatus());
	}

	delete this->topologyAdvancer;

	this->topologyAdvancer = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTopology += Data::time() - start;

	start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->front->clearSearchTrees();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTrees += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeFrontBackTrackingAdvancer()
{
	if (!this->front)
	{
/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeBackTracking = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

		return true;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	///TODO back tracking in slaves

	static_cast<PMJMesh::Front *>(this->front)->moveCrossingToRejected();

#if (!USE_NEW_FRONT_UPDATER)
	this->newFront = this->front->getAllElements();
#endif //#if (!USE_NEW_FRONT_UPDATER)

	//this->newFront.insert(this->newFront.end(), this->crossing.begin(), this->crossing.end());
	//this->crossing.clear();

	this->findCommonFront();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeBackTracking += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	bool ok = true;

	if (!ok)
	{
		this->status = this->errorFrontBackTrackingAdvancer;
	}

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeMeshImprover()
{
	if ((!this->front) || (!this->smoothInSlavesEnabled))
	{
/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeImprover = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

		return true;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->invertNewFront();

	this->improver = this->makeMeshImprover();

	this->improver->setIdManager(this->idManager);

	this->improver->execute();

#if USE_DELETE_FELS
    while (!this->oldInnerVertices.empty())
    {
        this->mesh->addInner(this->oldInnerVertices.front());

        this->oldInnerVertices.pop_front();
    }
#endif //#if USE_DELETE_FELS

	//bool ok = (this->improver->getStatus() < this->improver->error);
	bool ok = true;

	if (!ok)
	{
		this->status = this->errorMeshImprover;

		this->status.append(this->improver->getStatus());
	}

	delete this->improver;

	this->improver = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeImprover += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeTreeClassifier()
{
	if (!this->classifyTreeCells)
	{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		//this->timeClassify = 0.0;

		Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		if (this->inverted)
		{
			this->invertNewFront();
		}

#if USE_NEW_FRONT_UPDATER
		if (this->comm->isSlave())
		{
			static_cast<PMJMesh::Front *>(this->front)->setMerge(this->commonFront);
		}
#endif

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		//this is counted as if happened in improvement phase
		this->timeImprover += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return true;
	}

	if (!this->front)
	{
/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeClassify = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		if (this->inverted)
		{
			this->invertNewFront();
		}

#if USE_NEW_FRONT_UPDATER
		if (this->comm->isSlave())
		{
			static_cast<PMJMesh::Front *>(this->front)->setMerge(this->commonFront);
		}
#endif

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		//this is counted as if happened in improvement phase
		this->timeImprover += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return true;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	//Real debugstart = Data::time();

#if (!USE_WORKER_CELL_CLASSIFIER_2)
	if (!this->inverted)
	{
		this->invertNewFront();
	}

	//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to invert front = " << Data::time() - debugstart << std::endl;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	//this is counted as if happened in improvement phase
	this->timeImprover += Data::time() - start;

	start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#endif //#if (!USE_WORKER_CELL_CLASSIFIER_2)

	//debugstart = Data::time();

	this->classifier = this->makeTreeCellClassifier();

	//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to make cell classifier = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	this->classifier->execute();

	//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to execute cell classifier = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//bool ok = (this->classifier->getStatus() < this->classifier->error);
	bool ok = true;

	if (!ok)
	{
		this->status = this->errorTreeClassifier;

		this->status.append(this->classifier->getStatus());
	}

	delete this->classifier;

	this->classifier = NULL;

	//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to delete cell classifier = " << Data::time() - debugstart << std::endl;

	/*if (this->comm->isMaster())
	{
		for (ElementListMap::iterator iter = this->oldFrontElements.begin();
			 iter != this->oldFrontElements.end(); iter++)
		{
			Data::FrontElement *fel = (*iter).first;

			Data::ElementList adj = (*iter).second;

			while (!adj.empty())
			{
				static_cast<MJMesh::FrontElement *>(fel)->add(static_cast<MJMesh::Element *>(adj.front()));

				adj.pop_front();
			}
		}

		this->oldFrontElements.clear();
	}*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeClassify += Data::time() - start;

	start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	//debugstart = Data::time();

#if USE_WORKER_CELL_CLASSIFIER_2
	if (this->inverted)
#endif //#if USE_WORKER_CELL_CLASSIFIER_2
	{
		this->invertNewFront();
	}

	//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to invert new front = " << Data::time() - debugstart << std::endl;

#if USE_NEW_FRONT_UPDATER
	if (this->comm->isSlave())
	{
		//debugstart = Data::time();

		static_cast<PMJMesh::Front *>(this->front)->setMerge(this->commonFront);

		//std::cout << "PMJMesh::WorkerMainDrive::executeTreeClassifier, time to merge new and common fronts = " << Data::time() - debugstart << std::endl;
	}
#endif

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeImprover += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

bool PMJMesh::WorkerMainDrive::executeTreeDisrefiner()
{
	if (!this->front)
	{
		return true;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->minTreeDisrefiner = this->makeTreeMinDisrefiner();

	this->minTreeDisrefiner->execute();

	this->refinedCells = NULL;

	//bool ok = (this->minTreeDisrefiner->getStatus() < this->minTreeDisrefiner->error);
	bool ok = true;

	if (!ok)
	{
		this->status = this->errorTreeDisrefiner;

		this->status.append(this->minTreeDisrefiner->getStatus());
	}

	delete this->minTreeDisrefiner;

	this->minTreeDisrefiner = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

MJMesh::MinTreeRefiner *PMJMesh::WorkerMainDrive::makeTreeMinRefiner() const
{
	return new PMJMesh::WorkerMinTreeRefiner(this->dimension, this->tree,
		this->minRefinementLevel, static_cast<PMJMesh::Communicator *>(this->comm), this->box);
}

MJMesh::GeometryFrontAdvancer *PMJMesh::WorkerMainDrive::makeFrontGeometryAdvancer() const
{
	return new PMJMesh::GeometryFrontAdvancer(this->dimension, this->boundary,
		this->tree, this->comm, this->rangeProportion, this->usingSearchTrees, this->testingBox);
}

MJMesh::TopologyFrontAdvancer *PMJMesh::WorkerMainDrive::makeFrontTopologyAdvancer() const
{
	return new PMJMesh::TopologyFrontAdvancer(this->dimension, this->boundary,
		this->tree, static_cast<PMJMesh::Front *>(this->front), this->mesh, this->box,
		this->comm, this->rangeProportion, this->usingSearchTrees, this->testingBox);
		//false, this->testingBox);
}

MJMesh::BackTrackingFrontAdvancer *PMJMesh::WorkerMainDrive::makeFrontBackTrackingAdvancer() const
{
	///TODO frontbacktrackingadvancer nao eh necessario no 2D
	return NULL;
}

MJMesh::MeshImprover *PMJMesh::WorkerMainDrive::makeMeshImprover() const
{
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	MJMesh::Boundary *boundary = new MJMesh::Boundary();

	for (Data::Front::AllFrontElementIterator iter = this->oldFront->allEBegin();
		 iter != this->oldFront->allEEnd(); iter++)
	{
		boundary->add((*iter));
	}

	for (Data::Front::VertexIterator iter = this->oldFront->vBegin();
		 iter != this->oldFront->vEnd(); iter++)
	{
		boundary->add((*iter));

#if USE_DELETE_FELS
        if (this->mesh->haveInner((*iter)))
        {
            this->mesh->removeInner((*iter));

            this->oldInnerVertices.push_back((*iter));
        }
#endif //#if USE_DELETE_FELS
	}

	for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin();
		 iter != this->front->allEEnd(); iter++)
	{
		boundary->add((*iter));
	}

	for (Data::Front::VertexIterator iter = this->front->vBegin();
		 iter != this->front->vEnd(); iter++)
	{
		if (!boundary->have((*iter)))
		{
			boundary->add((*iter));

#if USE_DELETE_FELS
            if (this->mesh->haveInner((*iter)))
            {
                this->mesh->removeInner((*iter));

                this->oldInnerVertices.push_back((*iter));
            }
#endif //#if USE_DELETE_FELS
		}
	}

	if (this->commonFront)
    {
        for (Data::Front::AllFrontElementIterator iter = this->commonFront->allEBegin();
             iter != this->commonFront->allEEnd(); iter++)
        {
            boundary->add((*iter));
        }

        for (Data::Front::VertexIterator iter = this->commonFront->vBegin();
             iter != this->commonFront->vEnd(); iter++)
        {
            if (!boundary->have((*iter)))
            {
                boundary->add((*iter));

#if USE_DELETE_FELS
                if (this->mesh->haveInner((*iter)))
                {
                    this->mesh->removeInner((*iter));

                    this->oldInnerVertices.push_back((*iter));
                }
#endif //#if USE_DELETE_FELS
            }
        }
    }

#if USE_DELETE_FELS
	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		boundary, this->tree, true,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#else
	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, boundary, this->tree, this->removeBoundaryFelsForSmoothing, true,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#endif //#if USE_DELETE_FELS
#elif USE_NEW_FRONT_UPDATER
	Data::FrontElementList fels;
	fels.insert(fels.end(), this->oldFront->allEBegin(), this->oldFront->allEEnd());
	fels.insert(fels.end(), this->front->allEBegin(), this->front->allEEnd());
	fels.insert(fels.end(), this->commonFront->allEBegin(), this->commonFront->allEEnd());

	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, fels, this->tree, this->removeBoundaryFelsForSmoothing,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#else
	Data::FrontElementList fels = this->oldFront;
	fels.insert(fels.end(), this->newFront.begin(), this->newFront.end());
	fels.insert(fels.end(), this->commonFront.begin(), this->commonFront.end());

	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, fels, this->tree, this->removeBoundaryFelsForSmoothing,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
}

Performer::TreeCellClassifier *PMJMesh::WorkerMainDrive::makeTreeCellClassifier() const
{
#if USE_WORKER_CELL_CLASSIFIER_2
	return new PMJMesh::WorkerCellClassifier2(this->dimension, this->tree, this->box,
		this->oldFront, static_cast<PMJMesh::Front *>(this->front), this->commonFront, this->inverted);
#else
#if USE_NEW_FRONT_UPDATER
	return new PMJMesh::WorkerCellClassifier(this->dimension, this->tree, this->box,
		this->oldFront->getAllElements(), this->front->getAllElements(), this->commonFront->getAllElements());
#else
	return new PMJMesh::WorkerCellClassifier(this->dimension, this->tree, this->box,
		this->oldFront, this->newFront, this->commonFront);
#endif //#if USE_NEW_FRONT_UPDATER
#endif //#endif // USE_WORKER_CELL_CLASSIFIER_2
}

Performer::TreeRefiner *PMJMesh::WorkerMainDrive::makeTreeMinDisrefiner() const
{
	return new PMJMesh::WorkerMinTreeDisrefiner(this->dimension, this->tree, this->refinedCells);
}

#if USE_THREAD_COLOR
void PMJMesh::WorkerMainDrive::getMeshColor(Real &r, Real &g, Real &b, Parallel::ThreadManager *tm, Int process)
{
	//r = g = b = 0.0;

#if USE_OPENMP
	process = (process < 0) ? (tm ? tm->threadId() : 0) : (process - 1);
#else
	process = (process < 0) ? 0 : process - 1;
#endif //#if USE_OPENMP

	Data::pallete(process, r, g, b);
}

void PMJMesh::WorkerMainDrive::colorizeMesh(Int process) const
{
	/*if ((!this->mesh) || (this->mesh->size() == 0))
	{
		if ((!front) ||
			(!this->front) ||
			(this->front->size() == 0))
		{
			return;
		}
	}*/

	Real r, g, b;
	this->getMeshColor(r, g, b, static_cast<PMJMesh::Communicator *>(this->comm), process);

	if (this->mesh)
	{
		//Data::FrontElementList fels = this->mesh->getFrontElements();

		//while (!fels.empty())
		for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
			 iter != this->mesh->feEnd(); iter++)
		{
			//fels.front()->setColor(r, g, b);
			(*iter)->setColor(r, g, b);

			//fels.pop_front();
		}

		//Data::VertexList vertices = this->mesh->getVertices();

		//while (!vertices.empty())
		for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin();
			 iter != this->mesh->vEnd(); iter++)
		{
			//vertices.front()->setColor(r, g, b);
			(*iter)->setColor(r, g, b);

			//vertices.pop_front();
		}

		return;
	}
}
#endif //#if USE_THREAD_COLOR
