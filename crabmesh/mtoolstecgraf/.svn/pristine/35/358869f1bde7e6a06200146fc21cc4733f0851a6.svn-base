#include "PJmesh/GlobalTaskManager.h"

#include "Data/BoxTree.h"
#include "Parallel/Communicator.h"
#include "PMJMesh/Partition.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#if USE_NEW_FRONT_UPDATER
#include "PMJMesh/ActualFrontUpdater.h"
#include "PMJMesh/MeshUpdater.h"
#include "PMJMesh/PartialMeshUpdater.h"
#include "PMJMesh/EmptyMeshUpdater.h"
//#include "PMJMesh/NoBoundaryEmptyMeshUpdater.h"
#include "PMJMesh/FinalMeshUpdater.h"
#else
#include "PMJMesh/EmptyFrontUpdater.h"
#endif //#if USE_NEW_FRONT_UPDATER
//#include "PJmesh/MeshGenerator.h"
#include "PJmesh/JmeshTask.h"
#include "PJmesh/WorkerMainDrive.h"

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#include "Data/TreeNode.h"
#include "PMJMesh/LoadCalculator.h"
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

PJmesh::GlobalTaskManager::GlobalTaskManager(int version, int format,
	double factor, double tol, int internal,
	bool sharedParallel, UInt dimension,
	MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
	bool heightDependentInLoad, UInt removeLayersInLoad, bool smoothInSlavesEnabled,
	UInt numImproves, Real smoothingFactor,
#if (!USE_DELETE_FELS)
	UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
	UInt smoothingLayer,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks,
	enum PMJMesh::PartitionerType partitionerType,
	enum PMJMesh::MeshUpdateType updateType, enum PMJMesh::ShiftType shiftType,
	bool usePartitioningThreshold, Real maxAllowedLoad,
	bool sorted, bool usingSearchTrees, Real rangeProportion,
	UInt minRefinementLevel, bool skippingInnerInDecomposition,
	bool createInnerFrontInDecomposition,
	enum Parallel::AvailableLoadBalancers balancerType,
	Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm) :
	PMJMesh::GlobalTaskManager(sharedParallel, dimension, boundary, tree,
		percentageNonWorkingTask, heightDependentInLoad, removeLayersInLoad,
		smoothInSlavesEnabled, numImproves, smoothingFactor,
#if (!USE_DELETE_FELS)
		unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
		smoothingLayer, localBackTrackingEnabled, meshSorted, maxBackTracks,
		partitionerType, updateType, shiftType,
		usePartitioningThreshold, maxAllowedLoad, sorted,
		usingSearchTrees, rangeProportion, minRefinementLevel,
		skippingInnerInDecomposition, createInnerFrontInDecomposition, balancerType,
		loadBalancer, comm)
{
	this->numLayersMissingInGeneration = 3.0;

	this->createCrackInInnerFront = true;
	this->deleteFelsInInnerFront = true;

#if USE_DELETE_FELS
    this->deleteExtraFrontElements = false;
#endif //#if USE_DELETE_FELS

	this->version = version;
	this->format = format;
	this->factor = factor;
	this->tol = tol;
	this->internal = internal;
}

PJmesh::GlobalTaskManager::~GlobalTaskManager()
{

}

Parallel::Task *PJmesh::GlobalTaskManager::makeTask() const
{
	return new PJmesh::JmeshTask();
}

void PJmesh::GlobalTaskManager::finalizeMesh()
{
#if USE_NEW_FRONT_UPDATER
	if ((this->updateType != PMJMesh::FULL) && (this->meshUpdater))
#else
	if ((this->updateType != PMJMesh::FULL) && (this->frontUpdater))
#endif //#if USE_NEW_FRONT_UPDATER
	{
		if ((this->comm->isParallel()) &&
			((this->comm->isMaster()) || (this->taskSent)))
		{
#if USE_NEW_FRONT_UPDATER
			//static_cast<PMJMesh::PartialMeshUpdater *>(this->meshUpdater)->setCreatedFels(true);
#else
			static_cast<PMJMesh::PartialFrontUpdater *>(this->frontUpdater)->setCreatedFels(true);
#endif //#if USE_NEW_FRONT_UPDATER
		}
	}

	PMJMesh::GlobalTaskManager::finalizeMesh();
}

bool PJmesh::GlobalTaskManager::checkInnerFront(const Data::VertexList &vertices, const Data::FrontElementList &fels,
	const PMJMesh::Partition *partition, Real size) const
{
	return PMJMesh::GlobalTaskManager::checkInnerFront(vertices, fels, partition, size*1.5);
}

#if USE_NEW_FRONT_UPDATER
PMJMesh::ActualFrontUpdater *PJmesh::GlobalTaskManager::makeFrontUpdater() const
{
	return new PMJMesh::ActualFrontUpdater(this->dimension, this->front, this->comm, true);
}

PMJMesh::MeshUpdater *PJmesh::GlobalTaskManager::makeMeshUpdater() const
{
	if (this->comm->isMaster())
	{
		switch (this->updateType)
		{
		case PMJMesh::FULL:
			return new PMJMesh::MeshUpdater(this->dimension, this->mesh, this->comm, true);

		case PMJMesh::PARTIAL:
			return new PMJMesh::PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
			return new PMJMesh::PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
			break;

		case PMJMesh::EMPTY:
			return new PMJMesh::EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
			return new PMJMesh::EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
			break;

		case PMJMesh::FINAL:
			return new PMJMesh::FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true);
			break;
		}
	}
	else
	{
		switch (this->updateType)
		{
		case PMJMesh::FULL:
			return NULL;

		case PMJMesh::PARTIAL:
			return new PMJMesh::PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
			return new PMJMesh::PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
			break;

		case PMJMesh::EMPTY:
			return new PMJMesh::EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
			return new PMJMesh::EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
			break;

		case PMJMesh::FINAL:
			return new PMJMesh::FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true);
			break;
		}
	}

	return NULL;
}
#endif //#if USE_NEW_FRONT_UPDATER

bool PJmesh::GlobalTaskManager::setTaskParameters(Parallel::Task *t,
	MJMesh::Boundary *boundary, Data::BoxTree *tree,
	Performer::IdManager *idManager) const
{
	bool ret = PMJMesh::GlobalTaskManager::setTaskParameters(t, boundary, tree, idManager);

	if (ret)
	{
		PJmesh::WorkerMainDrive *drive = dynamic_cast<PJmesh::WorkerMainDrive *>(t->getBuilder());

		drive->setVersion(version);
		drive->setFormat(format);
		drive->setFactor(factor);
		drive->setTol(tol);
		drive->setInternal(internal);
	}

	return ret;
}

void PJmesh::GlobalTaskManager::executeGeometryGeneration()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	//Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	PJmesh::JmeshTask *t = static_cast<PJmesh::JmeshTask *>(this->makeTask());

	Real load = 1.0;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	PMJMesh::LoadCalculator *lc = this->makeLoadCalculator(false);

	lc->execute();

#if USE_C__11
	Data::RealHashMap loadMap = lc->getTreeLoad();
#else
	Data::RealMap loadMap = lc->getTreeLoad();
#endif //#if USE_C__11

	delete lc;

	load = std::max(loadMap[this->tree->getRoot()->getId()], 1.0);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	PMJMesh::Partition *p = new PMJMesh::Partition(static_cast<Data::Box *>(this->tree->getShape()), load);

	/*PMJMesh::Front *front = new PMJMesh::Front(this->front);

	front->setVSTree(NULL);
	front->setFESTree(NULL);*/

	PMJMesh::Front *front = new PMJMesh::Front(this->front->isSorted());

	/*front->setVertices(this->front->getVertices());
	//front->setElements(this->front->getAllElements());
	front->setBoundary(this->front->getBoundary());
	front->setElements(this->front->getElements());
	front->setRejected(this->front->getRejected());*/

	front->set(this->front);

	p->setSubfront(front);

	t->setPartition(p);

	this->setTaskParameters(t, this->boundary, this->tree, this->idManager);

	dynamic_cast<PJmesh::WorkerMainDrive *>(t->getBuilder())->setClassifyTreeCells(false);

	//std::cout << "before local task front size = " << this->front->size() << std::endl;
	//std::cout << "before local task local front size = " << front->size() << std::endl;

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	t->execute();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	//std::cout << "before local task front size = " << this->front->size() << std::endl;
	//std::cout << "before local task local front size = " << front->size() << std::endl;

	//ULInt ignore = 0;

	this->getParameters(t);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	//std::cout << this->timing(t, Data::time() - start) << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if USE_NEW_FRONT_UPDATER
	PMJMesh::MeshUpdater meshUpdater(this->dimension, this->mesh, NULL);

	meshUpdater.setMasterTask(t);

	meshUpdater.initialize();
	meshUpdater.execute();
	meshUpdater.finalize();
	meshUpdater.finalizeMesh();

	PMJMesh::ActualFrontUpdater frontUpdater(this->dimension, this->front, NULL);

	frontUpdater.setMasterTask(t);

	frontUpdater.initialize();
	frontUpdater.execute();
	frontUpdater.finalize();
	frontUpdater.finalizeMesh();
#else
	PMJMesh::FullFrontUpdater frontUpdater(this->dimension, true,
			this->boundary, this->front, this->mesh);

	frontUpdater.setTask(t);

	frontUpdater.initialize();
	frontUpdater.execute();
	frontUpdater.finalize();

	/*this->frontUpdater->setShared(true);

	this->frontUpdater->setTask(t);

	this->frontUpdater->initialize();

	this->frontUpdater->execute();

	this->frontUpdater->finalize();*/
#endif //#if USE_NEW_FRONT_UPDATER

	//std::cout << "after front updater front size = " << this->front->size() << std::endl;

	p->setBox(NULL);

	delete t;

/*#if USE_NEW_FRONT_UPDATER
	if ((this->meshUpdater) && (this->updateType != PMJMesh::FULL))
#else
	if ((this->frontUpdater) && (this->updateType != PMJMesh::FULL))
#endif //#if USE_NEW_FRONT_UPDATER
	{
		if ((this->updateType == PMJMesh::EMPTY) ||
			(this->updateType == PMJMesh::FINAL))
		{
#if USE_NEW_FRONT_UPDATER
			static_cast<PMJMesh::EmptyMeshUpdater *>(this->meshUpdater)->splitMesh();
#else
			static_cast<PMJMesh::EmptyFrontUpdater *>(this->frontUpdater)->splitMesh();
#endif //#if USE_NEW_FRONT_UPDATER
		}

		this->boundary->clearMaps();
	}*/
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PJmesh::GlobalTaskManager::timing(const Parallel::Task *task, Real timeTotal) const
{
	PJmesh::WorkerMainDrive *drive = dynamic_cast<PJmesh::WorkerMainDrive *>(task->getBuilder());

	Real timeTrees = drive->timeTrees;
	Real timeGeometry = drive->timeGeometry;
	Real timeTopology = drive->timeTopology;
	Real timeBackTracking = drive->timeBackTracking;
	Real timeClassify = drive->timeClassify;
	Real timeRefinement = drive->timeRefinement;
	Real timeImprover = drive->timeImprover;
	Real timeToJmeshConversion = drive->timeToJmeshConversion;
	Real timeFromJmeshConversion = drive->timeFromJmeshConversion;
	Real timeOverhead = timeTotal - timeTrees - timeGeometry - timeTopology - timeBackTracking - timeClassify - timeRefinement - timeImprover - timeToJmeshConversion - timeFromJmeshConversion;
	//UInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->innerVerticesSize() : 0;
	//UInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->verticesSize() - drive->MJMesh::MainDrive::getFront()->verticesSize(): 0;
	ULInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->verticesSize() - (drive->getOldFront() ? drive->getOldFront()->verticesSize() : 0) : 0;
	ULInt numElements = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->size() : 0;

	std::stringstream str;
	str << TaskManager::timing(task, timeTotal);
	str << "Tree Refiner and Disrefiner = " << timeRefinement << "\n";
	str << "Search Tree Builder and Destroyer = " << timeTrees << "\n";
	str << "Geometry Front Advancer = " << timeGeometry << "\n";
	str << "Topology Front Advancer = " << timeTopology << "\n";
	str << "Back-Tracking Front Advancer (+ find common front) = " << timeBackTracking << "\n";
	str << "Mesh Improver (+ inverting new front) = " << timeImprover << "\n";
	str << "Tree Classifier = " << timeClassify << "\n";
	str << "Converting Data Structures to Jmesh = " << timeToJmeshConversion << "\n";
	str << "Building Mesh Data Structures = " << timeFromJmeshConversion << "\n";
	str << "Overhead = " << timeOverhead << "\n";
	str << "Number of Vertices = " << numVertices << "\n";
	str << "Number of Elements = " << numElements << "\n";

	return str.str();
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
