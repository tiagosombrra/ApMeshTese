#include "PMTools/GlobalTaskManager.h"

#if (USE_MTOOLS)

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
//#include "PMTools/MeshGenerator.h"
#include "PMTools/MToolsTask.h"
#include "PMTools/WorkerMainDrive.h"

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#include "Data/TreeNode.h"
#include "PMJMesh/LoadCalculator.h"
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

PMTools::GlobalTaskManager::GlobalTaskManager(const FTools::OctTree *ftree,
	bool sharedParallel, UInt dimension,
	MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
	bool heightDependentInLoad, UInt removeLayersInLoad,  bool smoothInSlavesEnabled, UInt numImproves,
	Real smoothingFactor,
#if (!USE_DELETE_FELS)
	UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
	UInt smoothingLayer,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks,
	enum PMJMesh::PartitionerType partitionerType,
	enum PMJMesh::MeshUpdateType updateType, enum PMJMesh::ShiftType shiftType,
	bool usePartitioningThreshold, Real maxAllowedLoad,
	bool sorted, bool usingSearchTrees, Real rangeProportion, UInt minRefinementLevel,
	bool skippingInnerInDecomposition, bool createInnerFrontInDecomposition,
	enum Parallel::AvailableLoadBalancers balancerType,
	Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm) :
	PMJMesh::GlobalTaskManager(sharedParallel, dimension, boundary, tree,
		percentageNonWorkingTask, heightDependentInLoad, removeLayersInLoad, smoothInSlavesEnabled,
		numImproves, smoothingFactor,
#if (!USE_DELETE_FELS)
		unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
		smoothingLayer,
		localBackTrackingEnabled, meshSorted, maxBackTracks, partitionerType,
		updateType, shiftType,
		usePartitioningThreshold, maxAllowedLoad, sorted,
		usingSearchTrees, rangeProportion, minRefinementLevel,
		skippingInnerInDecomposition, createInnerFrontInDecomposition,
		balancerType, loadBalancer, comm)
{
	this->numLayersMissingInGeneration = 3.0;

	this->disableBackTrackingInInterTasks = false;
	this->createCrackInInnerFront = true;
	this->deleteFelsInInnerFront = true;

#if USE_DELETE_FELS
    this->deleteExtraFrontElements = false;
#endif //#if USE_DELETE_FELS

	this->ftree = ftree;
}

PMTools::GlobalTaskManager::~GlobalTaskManager()
{

}

Parallel::Task *PMTools::GlobalTaskManager::makeTask() const
{
	return new PMTools::MToolsTask();
}

void PMTools::GlobalTaskManager::finalizeMesh()
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

bool PMTools::GlobalTaskManager::checkInnerFront(const Data::VertexList &vertices, const Data::FrontElementList &fels,
	const PMJMesh::Partition *partition, Real size) const
{
	return PMJMesh::GlobalTaskManager::checkInnerFront(vertices, fels, partition, size*1.5);
}

#if USE_NEW_FRONT_UPDATER
PMJMesh::ActualFrontUpdater *PMTools::GlobalTaskManager::makeFrontUpdater() const
{
	return new PMJMesh::ActualFrontUpdater(this->dimension, this->front, this->comm, true);
}

PMJMesh::MeshUpdater *PMTools::GlobalTaskManager::makeMeshUpdater() const
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

bool PMTools::GlobalTaskManager::setTaskParameters(Parallel::Task *t,
	MJMesh::Boundary *boundary, Data::BoxTree *tree,
	Performer::IdManager *idManager) const
{
	bool ret = PMJMesh::GlobalTaskManager::setTaskParameters(t, boundary, tree, idManager);

	if (ret)
	{
		PMTools::WorkerMainDrive *drive = dynamic_cast<PMTools::WorkerMainDrive *>(t->getBuilder());

		drive->setFtree(this->ftree);
	}

	return ret;
}

void PMTools::GlobalTaskManager::balanceLoad(bool shared)
{
	if (this->disableBackTrackingInInterTasks)
	{
		for (UInt i = 0; i < this->tasks.size(); i++)
		{
			PMTools::MToolsTask *t = static_cast<PMTools::MToolsTask *>(this->tasks[i]);

			bool enableBackTracking = !this->isInterTask(t);

			t->setEnableBackTracking(enableBackTracking);

/*#if USE_GUI
			if (enableBackTracking)
			{
				t->unhighlight();
			}
			else
			{
				t->highlight();
			}
#endif //#if USE_GUI*/
		}
	}

	PMJMesh::GlobalTaskManager::balanceLoad(shared);
}

void PMTools::GlobalTaskManager::executeGeometryGeneration()
{
	if (this->updateType != PMJMesh::FINAL)
	{
		return;
	}

#if DEBUG_DISABLE_IMPROVEMENT
	PMTools::setWorkerImprovementEnabled(PMTools::isMasterImprovementEnabled());
#endif //#if DEBUG_DISABLE_IMPROVEMENT

	//debug
	//Real debugstart = Data::time();
	//endebug

	/*if ((this->frontUpdater) && (this->updateType != PMJMesh::FULL))
	{
		if ((this->updateType == PMJMesh::EMPTY) ||
			(this->updateType == PMJMesh::FINAL))
		{
			static_cast<PMJMesh::EmptyFrontUpdater *>(this->frontUpdater)->splitMesh();
		}

		this->boundary->clearMaps();
	}

	std::cout << "Master time for spliting mesh = " << Data::time() - debugstart << std::endl;
	debugstart = Data::time();*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

	PMTools::MToolsTask *t = static_cast<PMTools::MToolsTask *>(this->makeTask());

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

	//PMJMesh::Partition *p = new PMJMesh::Partition(static_cast<Data::Box *>(this->tree->getShape()), load);
	PMJMesh::Partition *p = new PMJMesh::Partition(NULL, load);

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

	dynamic_cast<PMTools::WorkerMainDrive *>(t->getBuilder())->setClassifyTreeCells(false);

	//std::cout << "before local task front size = " << this->front->size() << std::endl;
	//std::cout << "before local task local front size = " << front->size() << std::endl;

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	//std::cout << "Master time for initializing final task = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	t->execute();

	//std::cout << "Master time for executing final task = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	//std::cout << "before local task front size = " << this->front->size() << std::endl;
	//std::cout << "before local task local front size = " << front->size() << std::endl;

	//ULInt ignore = 0;

	this->getParameters(t);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
	std::cout << this->timing(t, Data::time() - start) << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

	this->isInError(t);

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

	//std::cout << "Master time for finalizing final task = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

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

	//std::cout << "Master time for spliting mesh = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PMTools::GlobalTaskManager::timing(const Parallel::Task *task, Real timeTotal) const
{
	PMTools::WorkerMainDrive *drive = dynamic_cast<PMTools::WorkerMainDrive *>(task->getBuilder());

	Real timeTrees = drive->timeTrees;
	Real timeGeometry = drive->timeGeometry;
	Real timeTopology = drive->timeTopology;
	Real timeBackTracking = drive->timeBackTracking;
	Real timeClassify = drive->timeClassify;
	Real timeRefinement = drive->timeRefinement;
	Real timeImprover = drive->timeImprover;
	Real timeToMToolsConversion = drive->timeToMToolsConversion;
	Real timeFromMToolsConversion = drive->timeFromMToolsConversion;
	Real timeOverhead = timeTotal - timeTrees - timeGeometry - timeTopology - timeBackTracking - timeClassify - timeRefinement - timeImprover - timeToMToolsConversion - timeFromMToolsConversion;
	//UInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->innerVerticesSize() : 0;
	//UInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->verticesSize() - (drive->MJMesh::MainDrive::getFront() ? drive->MJMesh::MainDrive::getFront()->verticesSize() : 0) : 0;
	ULInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->verticesSize() - (drive->getOldFront() ? drive->getOldFront()->verticesSize() : 0) : 0;
	ULInt numElements = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->size() : 0;

	std::stringstream str;
	str << TaskManager::timing(task, timeTotal);
	str << "Tree Refiner and Disrefiner = " << timeRefinement << " (" << 100.0*timeRefinement/timeTotal << "%)\n";
	str << "Search Tree Builder and Destroyer = " << timeTrees << " (" << 100.0*timeTrees/timeTotal << "%)\n";
	str << "Geometry Front Advancer = " << timeGeometry << " (" << 100.0*timeGeometry/timeTotal << "%)\n";
	str << "Topology Front Advancer = " << timeTopology << " (" << 100.0*timeTopology/timeTotal << "%)\n";
	str << "Back-Tracking Front Advancer (+ find common front) = " << timeBackTracking << " (" << 100.0*timeBackTracking/timeTotal << "%)\n";
	str << "Mesh Improver (+ inverting new front) = " << timeImprover << " (" << 100.0*timeImprover/timeTotal << "%)\n";
	str << "Tree Classifier = " << timeClassify << " (" << 100.0*timeClassify/timeTotal << "%)\n";
	str << "Converting Data Structures to MTools = " << timeToMToolsConversion << " (" << 100.0*timeToMToolsConversion/timeTotal << "%)\n";
	str << "Building Mesh Data Structures = " << timeFromMToolsConversion << " (" << 100.0*timeFromMToolsConversion/timeTotal << "%)\n";
	str << "Overhead = " << timeOverhead << " (" << 100.0*timeOverhead/timeTotal << "%)\n";
	str << "Number of Vertices = " << numVertices << "\n";
	str << "Number of Elements = " << numElements << "\n";

#if DEBUG_TIMING_PMTOOLS
	this->timeMeshGeneratorWorkerMeshGeneration += timeGeometry;
	this->timeMeshGeneratorWorkerTreeClassification += timeClassify;
	this->timeMeshGeneratorWorkerToMToolsConversion += timeToMToolsConversion;
	this->timeMeshGeneratorWorkerFromMToolsConversion += timeFromMToolsConversion;
	this->timeMeshGeneratorWorkerOthers += timeTotal - timeGeometry - timeClassify - timeToMToolsConversion - timeFromMToolsConversion;
#endif //#if DEBUG_TIMING_PMTOOLS

	return str.str();
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#endif //#if (USE_MTOOLS)
