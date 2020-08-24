#include "PMTools/MainDrive.h"

#if (USE_MTOOLS)

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMTools/Communicator.h"
#include "PMTools/TreeGenerator.h"
#include "PMTools/GlobalTaskManager.h"
#include "PMTools/BackTrackingFrontAdvancer.h"
#include "PMJMesh/BoundaryBuilder.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/WorkerCellClassifier.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"

//debug
#include "AdvFront3D.hpp"
//endebug

PMTools::MainDrive::MainDrive(Parallel::Communicator *comm) :
	MTools::MainDrive(),
	PMJMesh::MainDrive(comm)
{
	///differences between constructor, cli and gui:
	///                                   constructor  cli    gui
	///setParallel()                      true         false  user
	///setSkippingInnerInDecomposition()  true         true   false
	///setRemoveLayersInLoad()            3            3      0
	///setHeightDependentInLoad()         true         true   false

	this->setAlgorithm(PMJMesh::GLOBAL_SHIFT);
	this->setMinRefinementPostponed(true);
	this->setPercentageNonWorkingTask(0.0);

#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(false);
	this->setParallel(true);
	this->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);
	this->setPartitionerType(PMJMesh::BOX_LOAD);
	this->setUsePartitioningThreshold(false);
	this->setRemoveLayersInLoad(3);
	this->setHeightDependentInLoad(true);
	this->setSkippingInnerInDecomposition(true);
	this->setRangeProportion(1.0);
	this->setSmoothingLayer(1);
	this->setUpdateType(PMJMesh::EMPTY_NO_BOUNDARY);
	this->setShiftType(PMJMesh::LATERAL);

	if (comm)
	{
		this->setComm(comm);
	}
	else
	{
		if (this->comm)
		{
			delete this->comm;

			this->comm = NULL;
		}

		this->setComm(this->makeCommunicator());
	}

	//if ((this->comm) && (!this->comm->isInitialized()))
	if (this->comm)
	{
		this->comm->init();
	}

	this->dimension = 3;
	//this->setBoundarySorted(false);

#if DEBUG_TIMING_PMTOOLS
	this->timeInit = this->timeBoundaryBuilder = this->timeHotellingTransformer = this->timeBuildAdvFront =
		this->timeTreeGeneratorGeneration = this->timeTreeGeneratorConstruction =
		this->timeTreeGeneratorClassification = this->timeTreeGeneratorOthers =
		this->timeMeshGeneratorMasterTreeDecomposition =
		this->timeMeshGeneratorMasterFrontDecomposition = this->timeMeshGeneratorWorkerMeshGeneration =
		this->timeMeshGeneratorWorkerTreeClassification = this->timeMeshGeneratorWorkerOthers =
		this->timeMeshGeneratorWorkerToMToolsConversion = this->timeMeshGeneratorWorkerFromMToolsConversion =
		this->timeMeshGeneratorMasterMeshUpdate = this->timeMeshGeneratorMasterTreeShift =
		this->timeMeshGeneratorMasterFinalization = this->timeMeshGeneratorMasterOthers =
		this->timeMeshBackTrackerBackTracking = this->timeMeshBackTrackerToMToolsConversion =
		this->timeMeshBackTrackerFromMToolsConversion = this->timeMeshBackTrackerOthers =
		this->timeHotellingBackTransformer = this->timeOverhead = 0.0;
#endif //#if DEBUG_TIMING_PMTOOLS
}

PMTools::MainDrive::~MainDrive()
{
	this->clear();
}

#if USE_NUMERICAL
void PMTools::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
    this->PMJMesh::MainDrive::hotellingTransformEnabled = this->MTools::MainDrive::hotellingTransformEnabled = hotellingTransformEnabled;
}

bool PMTools::MainDrive::isHotellingTransformEnabled() const
{
    return this->PMJMesh::MainDrive::hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void PMTools::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->PMJMesh::MainDrive::usingUniformRefinement = this->MTools::MainDrive::usingUniformRefinement = usingUniformRefinement;
}

bool PMTools::MainDrive::isUsingUniformRefinement() const
{
	return this->PMJMesh::MainDrive::usingUniformRefinement;
}

const Data::Boundary *PMTools::MainDrive::getBoundary() const
{
	return MTools::MainDrive::getBoundary();
}

void PMTools::MainDrive::clear()
{
	this->MTools::MainDrive::boundary = NULL;
	this->MTools::MainDrive::mesh = NULL;
	this->MTools::MainDrive::tree = NULL;

	MTools::MainDrive::clear();
	PMJMesh::MainDrive::clear();
}

bool PMTools::MainDrive::loadFile()
{
#if (!USE_WORKER_FILE_LOADER)
	if (this->comm->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
		return MTools::MainDrive::loadFile();
	}

	return false;
}

bool PMTools::MainDrive::saveFile() const
{
	if (this->comm->isMaster())
	{
		//return MTools::MainDrive::saveFile();
		return PMJMesh::MainDrive::saveFile();
	}

	return false;
}

void PMTools::MainDrive::execute()
{
	if ((this->Performer::MainDrive::status.getType() == PMJMesh::MainDrive::success.getType()) ||
		(this->Performer::MainDrive::status.getType() >= PMJMesh::MainDrive::error.getType()))
	{
		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS
#if DEBUG_TIMING_PMTOOLS
	Real debugruntime = 0.0;
#endif //#if DEBUG_TIMING_PMTOOLS

	switch (this->Performer::MainDrive::status.getType())
	{
	case PMJMesh::MainDrive::NOT_INIT:

		if (!this->executeInit())
		{
			break;
		}

#if USE_GUI
		//if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::INIT:

		//begin adjusting boundary

		if (!this->executeBoundaryBuilder(true))
		{
			break;
		}

		//end adjusting boundary

#if USE_GUI
		//if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::BOUNDARY_BUILT:

	#if USE_NUMERICAL
		//begin hotelling transform

        if (!this->executeHotellingTransform())
		{
			break;
		}

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling transform

    case MJMesh::MainDrive::HOTELLING_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL

		//begin adjusting tree

		if (!this->executeTreeGenerator(true))
		{
			break;
		}

		//end adjusting tree

/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		if (this->comm->isMaster())
		{
			Real end = Data::time();
			std::cout << "Time for generating tree = " << end - start << " seconds" << std::endl;
		}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::TREE_GENERATED:

		//begin mesh generation

		if (!this->executeFrontGeometryAdvancer())
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:

		if (!this->executeFrontBackTrackingAdvancer())
		{
			break;
		}

		//end mesh generation

#if USE_GUI
		if ((this->dimension == 3) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:

#if USE_NUMERICAL
        //begin hotelling back transform

        if (!this->executeHotellingBackTransform())
		{
			break;
		}

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling back transform

	case MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL

#if USE_TIME_MEASUREMENTS
		if (this->comm->isMaster())
		{
			/*std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;*/

			Real end = Data::time();
			std::cout << "Runtime = " << end - start << " seconds" << std::endl;
#if DEBUG_TIMING_PMTOOLS
			debugruntime = end - start;
#endif //#if DEBUG_TIMING_PMTOOLS
		}
#endif //#if USE_TIME_MEASUREMENTS

		if (!this->executeFinalize())
		{
			break;
		}
	}

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->PMJMesh::MainDrive::runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS

#if DEBUG_TIMING_PMTOOLS
	this->timeOverhead = debugruntime -
		this->timeInit - this->timeBoundaryBuilder - this->timeHotellingTransformer - this->timeBuildAdvFront -
		this->timeTreeGeneratorGeneration - this->timeTreeGeneratorConstruction -
		this->timeTreeGeneratorClassification - this->timeTreeGeneratorOthers -
		this->timeMeshGeneratorMasterTreeDecomposition - this->timeMeshGeneratorMasterFrontDecomposition -
		this->timeMeshGeneratorWorkerMeshGeneration - this->timeMeshGeneratorWorkerTreeClassification -
		this->timeMeshGeneratorWorkerToMToolsConversion - this->timeMeshGeneratorWorkerFromMToolsConversion -
		this->timeMeshGeneratorWorkerOthers - this->timeMeshGeneratorMasterMeshUpdate -
		this->timeMeshGeneratorMasterTreeShift - this->timeMeshGeneratorMasterFinalization -
		this->timeMeshGeneratorMasterOthers - this->timeMeshBackTrackerBackTracking -
		this->timeMeshBackTrackerToMToolsConversion - this->timeMeshBackTrackerFromMToolsConversion -
		this->timeMeshBackTrackerOthers - this->timeHotellingBackTransformer;

	std::cout << "Timings:" << std::endl;

	std::cout << "initializing: " << this->timeInit << " (" << 100.0*this->timeInit/debugruntime << " %)" << std::endl;

	std::cout << "building boundary: " << this->timeBoundaryBuilder << " (" << 100.0*this->timeBoundaryBuilder/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling transform: " << this->timeHotellingTransformer << " (" << 100.0*this->timeHotellingTransformer/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "building adv front: " << this->timeBuildAdvFront << " (" << 100.0*this->timeBuildAdvFront/debugruntime << " %)" << std::endl;

	std::cout << "generating octree - mtools: " << this->timeTreeGeneratorGeneration << " (" << 100.0*this->timeTreeGeneratorGeneration/debugruntime << " %)" << std::endl;
	std::cout << "generating octree - pmjmesh: " << this->timeTreeGeneratorConstruction << " (" << 100.0*this->timeTreeGeneratorConstruction/debugruntime << " %)" << std::endl;
	std::cout << "classifying octree: " << this->timeTreeGeneratorClassification << " (" << 100.0*this->timeTreeGeneratorClassification/debugruntime << " %)" << std::endl;
	std::cout << "octree overhead: " << this->timeTreeGeneratorOthers << " (" << 100.0*this->timeTreeGeneratorOthers/debugruntime << " %)" << std::endl;

	std::cout << "decomposing octree - calculating load and partitioning: " << this->timeMeshGeneratorMasterTreeDecomposition << " (" << 100.0*this->timeMeshGeneratorMasterTreeDecomposition/debugruntime << " %)" << std::endl;
	std::cout << "decomposing front - initial and during shifts: " << this->timeMeshGeneratorMasterFrontDecomposition << " (" << 100.0*this->timeMeshGeneratorMasterFrontDecomposition/debugruntime << " %)" << std::endl;
	std::cout << "generating mesh in workers: " << this->timeMeshGeneratorWorkerMeshGeneration << " (" << 100.0*this->timeMeshGeneratorWorkerMeshGeneration/debugruntime << " %)" << std::endl;
	std::cout << "classifying octree in workers: " << this->timeMeshGeneratorWorkerTreeClassification << " (" << 100.0*this->timeMeshGeneratorWorkerTreeClassification/debugruntime << " %)" << std::endl;
	std::cout << "converting data strs to mtools in workers: " << this->timeMeshGeneratorWorkerToMToolsConversion << " (" << 100.0*this->timeMeshGeneratorWorkerToMToolsConversion/debugruntime << " %)" << std::endl;
	std::cout << "converting data strs from mtools in workers: " << this->timeMeshGeneratorWorkerFromMToolsConversion << " (" << 100.0*this->timeMeshGeneratorWorkerFromMToolsConversion/debugruntime << " %)" << std::endl;
	std::cout << "doing other stuff in workers: " << this->timeMeshGeneratorWorkerOthers << " (" << 100.0*this->timeMeshGeneratorWorkerOthers/debugruntime << " %)" << std::endl;
	std::cout << "updating front and mesh: " << this->timeMeshGeneratorMasterMeshUpdate << " (" << 100.0*this->timeMeshGeneratorMasterMeshUpdate/debugruntime << " %)" << std::endl;
	std::cout << "shifting octree cells: " << this->timeMeshGeneratorMasterTreeShift << " (" << 100.0*this->timeMeshGeneratorMasterTreeShift/debugruntime << " %)" << std::endl;
	std::cout << "finalizing mesh by master: " << this->timeMeshGeneratorMasterFinalization << " (" << 100.0*this->timeMeshGeneratorMasterFinalization/debugruntime << " %)" << std::endl;
	std::cout << "doing other stuff in master: " << this->timeMeshGeneratorMasterOthers << " (" << 100.0*this->timeMeshGeneratorMasterOthers/debugruntime << " %)" << std::endl;

	std::cout << "back-tracking/improvement: " << this->timeMeshBackTrackerBackTracking << " (" << 100.0*this->timeMeshBackTrackerBackTracking/debugruntime << " %)" << std::endl;
	std::cout << "converting data strs to mtools in back-tracking: " << this->timeMeshBackTrackerToMToolsConversion << " (" << 100.0*this->timeMeshBackTrackerToMToolsConversion/debugruntime << " %)" << std::endl;
	std::cout << "converting data strs from mtools in back-tracking: " << this->timeMeshBackTrackerFromMToolsConversion << " (" << 100.0*this->timeMeshBackTrackerFromMToolsConversion/debugruntime << " %)" << std::endl;
	std::cout << "back-tracking overhead: " << this->timeMeshBackTrackerOthers << " (" << 100.0*this->timeMeshBackTrackerOthers/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling back transform: " << this->timeHotellingBackTransformer << " (" << 100.0*this->timeHotellingBackTransformer/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "overhead: " << this->timeOverhead << " (" << 100.0*this->timeOverhead/debugruntime << " %)" << std::endl;

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	std::cout << std::endl << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for initializing = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsInit << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for geometric in tests in = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsInTest << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for geometric out tests = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOutOfBoxTest << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for checking if old position is ON = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOldPosOn << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for checking if leaf does not crosses common and does not crosses new = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsNotOnCommonAndNotNewTest << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for working when previous check is true = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsNotOnCommonAndNotNew << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for narrowing lists (not leaf) = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsCommonNewOld << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for checking if cell crosses common or crosses new = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOnCommonOrNewTest << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for working when previous check is true = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOnCommonOrNew << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for checking if cell crosses old = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOnOldTest << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for working when previous check is true = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsOnOld << std::endl;
	std::cout << "WorkerCellClassifier::findOnCells, time for working when neither of the two previous checks is true = " << PMJMesh::WorkerCellClassifier::timeFindOnCellsElse << std::endl;
	std::cout << "WorkerCellClassifier::findInCells, time for classifying cells = " << PMJMesh::WorkerCellClassifier::timeFindInCellsCells << std::endl;
	std::cout << "WorkerCellClassifier::findInCells, time for classifying neighboring cells = " << PMJMesh::WorkerCellClassifier::timeFindInCellsNeighbors << std::endl;
	std::cout << "WorkerCellClassifier::findInCells, time for classifying parents = " << PMJMesh::WorkerCellClassifier::timeFindInCellsParents << std::endl;
	std::cout << "WorkerCellClassifier::findOutCells, time = " << PMJMesh::WorkerCellClassifier::timeFindOutCells << std::endl;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
#endif //#if DEBUG_TIMING_PMTOOLS
}

std::string PMTools::MainDrive::qualityString() const
{
	return PMJMesh::MainDrive::qualityString();
}

bool PMTools::MainDrive::loadInputFile(const std::string &inputFileName)
{
	bool ret = MTools::MainDrive::loadInputFile(inputFileName);

	if (ret)
	{
		this->dimension = 3;
	}

	return ret;
}

bool PMTools::MainDrive::saveInputFile(const std::string &outputFileName) const
{
	//return MTools::MainDrive::saveInputFile(outputFileName);
	return PMJMesh::MainDrive::saveInputFile(outputFileName);
}

bool PMTools::MainDrive::loadOutputFile(const std::string &inputFileName)
{
	return MTools::MainDrive::loadOutputFile(inputFileName);
}

bool PMTools::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	//return MTools::MainDrive::saveOutputFile(outputFileName);
	return PMJMesh::MainDrive::saveOutputFile(outputFileName);
}

bool PMTools::MainDrive::executeInit()
{
#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

#if USE_TIME_MEASUREMENTS
	this->MTools::MainDrive::runtime = 0.0;
	this->PMJMesh::MainDrive::runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	this->mshRegion = this->makeMshRegion();

	if (this->MTools::MainDrive::boundaryBuilder)
	{
		this->PMJMesh::MainDrive::boundaryBuilder = this->MTools::MainDrive::boundaryBuilder;
	}

#if DEBUG_TIMING_PMTOOLS
	bool ret = PMJMesh::MainDrive::executeInit();

	this->timeInit = Data::time() - debugstart;

	return ret;
#else
	return PMJMesh::MainDrive::executeInit();
#endif //#if DEBUG_TIMING_PMTOOLS
}

bool PMTools::MainDrive::executeBoundaryBuilder(bool del)
{
#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	if (this->MTools::MainDrive::boundaryBuilder)
	{
		this->PMJMesh::MainDrive::boundaryBuilder = this->MTools::MainDrive::boundaryBuilder;
	}
	else
	{
		this->MTools::MainDrive::boundaryBuilder = this->PMJMesh::MainDrive::boundaryBuilder;
	}

#if (!USE_WORKER_FILE_LOADER)
	static_cast<PMJMesh::BoundaryBuilder *>(this->MTools::MainDrive::boundaryBuilder)->setDimension(this->dimension);
#endif //#if (!USE_WORKER_FILE_LOADER)

	bool ret = MTools::MainDrive::executeBoundaryBuilder(false);

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->PMJMesh::MainDrive::boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->MTools::MainDrive::boundaryBuilder;

		this->MTools::MainDrive::boundaryBuilder = NULL;
		this->PMJMesh::MainDrive::boundaryBuilder = NULL;
	}

	this->PMJMesh::MainDrive::boundary = this->MTools::MainDrive::boundary;

	if (ret)
	{
		this->Performer::MainDrive::status = this->boundaryBuilt;
	}

#if DEBUG_TIMING_PMTOOLS
	this->timeBoundaryBuilder = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	return ret;
}

#if USE_NUMERICAL
bool PMTools::MainDrive::executeHotellingTransform()
{
#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	bool ret = MTools::MainDrive::executeHotellingTransform();

	if (ret)
    {
        this->status = this->hotellingTransformApplied;
    }

#if DEBUG_TIMING_PMTOOLS
	this->timeHotellingTransformer = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	return ret;
}
#endif //#if USE_NUMERICAL

bool PMTools::MainDrive::executeTreeGenerator(bool del)
{
	//Msh3D::AdvFront does not need to search for cracks
	this->mshRegion->SetNoCracks(true);

	this->mshRegion->SetUseUniformRefinement(this->PMJMesh::MainDrive::usingUniformRefinement);

	//this->mshRegion->SetUseUniformSize(false);
	if ((this->partitionerType == PMJMesh::BSP_NUMBER) || (this->partitionerType == PMJMesh::SMART_BSP_NUMBER))
	{
		this->mshRegion->SetUseUniformSize(false);
	}
	else
	{
		this->mshRegion->SetUseUniformSize(true);
	}

#if USE_TIMING
	this->treeGenerationTime = 0.0;

	Real start = Data::time();
#endif //#if USE_TIMING

	this->mshRegion->GenerateOctTree();

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

	this->mshRegion->SetNoCracks(false);

#if DEBUG_TIMING_PMTOOLS
	this->timeTreeGeneratorGeneration = Msh3D::AdvFront3D::timeTree;
	this->timeBuildAdvFront = MTools::MshRegion3D::timeBuildAdvFront;
#endif //#if DEBUG_TIMING_PMTOOLS

#if USE_TIMING
	start = Data::time();
#endif //#if USE_TIMING

	MTools::MainDrive::executeTreeGenerator(false);

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

	this->minRefinementLevel = !this->mshRegion->GetOctTree()->IsUseUniformSize() ? 0 :
		static_cast<UInt>(std::log(static_cast<Data::BoxTreeCell *>(this->MTools::MainDrive::tree->getRoot())->size()/this->mshRegion->GetOctTree()->GetUniformSize())/std::log(2.0));

#if DEBUG_TIMING_PMTOOLS
	this->timeTreeGeneratorConstruction = dynamic_cast<PMTools::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->timeGeneration;
	this->timeTreeGeneratorClassification = dynamic_cast<PMTools::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->timeClassification;

	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	this->PMJMesh::MainDrive::tree = this->MTools::MainDrive::tree;

	if (del)
	{
		delete this->MTools::MainDrive::treeGenerator;

		this->MTools::MainDrive::treeGenerator = NULL;
	}

	this->executeSendRuntimeInfo();

	this->Performer::MainDrive::status = this->treeGenerated;

#if DEBUG_TIMING_PMTOOLS
	this->timeTreeGeneratorOthers = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	return true;
}

bool PMTools::MainDrive::executeFrontGeometryAdvancer()
{
#if DEBUG_TIMING_PMTOOLS
	//code from PMJMesh::MainDrive::executeFrontGeometryAdvancer()
	Real debugstart = Data::time();

	if (!this->balancer)
	{
		this->balancer = this->makeLoadBalancer();
	}

	if (!this->geometryManager)
	{
		this->geometryManager = this->makeTaskManager();

		this->geometryManager->setIdManager(this->idManager);
	}

	if ((this->algorithm == PMJMesh::GLOBAL_SHIFT) || (this->algorithm == PMJMesh::A_PRIORI_INTERFACE_MESH))
	{
		this->PMJMesh::MainDrive::front = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->getFront();
		this->PMJMesh::MainDrive::mesh = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->getMesh();
	}

	this->timeMeshGeneratorMasterOthers = Data::time() - debugstart;

	this->geometryManager->execute();

	this->timeMeshGeneratorMasterTreeDecomposition = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterTreeDecomposition;
	this->timeMeshGeneratorMasterFrontDecomposition = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterFrontDecomposition;
	this->timeMeshGeneratorWorkerMeshGeneration = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerMeshGeneration;
	this->timeMeshGeneratorWorkerTreeClassification = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerTreeClassification;
	this->timeMeshGeneratorWorkerToMToolsConversion = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerToMToolsConversion;
	this->timeMeshGeneratorWorkerFromMToolsConversion = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerFromMToolsConversion;
	this->timeMeshGeneratorWorkerOthers = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerOthers;
	this->timeMeshGeneratorMasterMeshUpdate = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterMeshUpdate;
	this->timeMeshGeneratorMasterTreeShift = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterTreeShift;
	this->timeMeshGeneratorMasterFinalization = static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterFinalization;
	this->timeMeshGeneratorMasterOthers += static_cast<PMTools::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterOthers;

	debugstart = Data::time();

	bool returnCode = false;

	if (this->geometryManager->getStatus().getType() != PMJMesh::GlobalTaskManager::success.getType())
	{
		if (this->geometryManager->getStatus().getType() >= PMJMesh::GlobalTaskManager::error.getType())
		{
			this->status = this->error;
		}

		std::stringstream str;

		str << "parallel front advancer - " << this->geometryManager->getStatus().getMessage();

		this->status.setMessage(str.str());

		returnCode = false;
	}
	else
	{
		returnCode = true;

		this->status = MJMesh::MainDrive::frontGeometryAdvanced;
	}

	this->MTools::MainDrive::mesh = this->PMJMesh::MainDrive::mesh;

	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;

	return returnCode;
#else
	bool ret = PMJMesh::MainDrive::executeFrontGeometryAdvancer();

	//if (ret)
	{
		this->MTools::MainDrive::mesh = this->PMJMesh::MainDrive::mesh;
	}

	return ret;
#endif //#if DEBUG_TIMING_PMTOOLS
}

bool PMTools::MainDrive::executeFrontBackTrackingAdvancer()
{
#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

#if DEBUG_TIMING_PMTOOLS
	if (this->comm->isSlave())
	{
		if (!this->front)
		{
			static_cast<GlobalTaskManager *>(this->geometryManager)->setFront(static_cast<PMJMesh::Front *>(this->front));
		}

		this->timeMeshBackTrackerBackTracking += Data::time() - debugstart;

		this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;

		return true;
	}

	//if (this->updateType != PMJMesh::FULL)
	{
		static_cast<GlobalTaskManager *>(this->geometryManager)->splitMesh();
	}

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);

	if (this->dimension == 3)
	{
		if (!this->backTracking)
		{
			this->backTracking = this->makeFrontBackTrackingAdvancer();

			this->backTracking->setIdManager(this->idManager);

			this->PMJMesh::MainDrive::front = NULL;
		}

		this->backTracking->execute();

		this->timeMeshBackTrackerBackTracking = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeBackTracking;
		this->timeMeshBackTrackerToMToolsConversion = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeToMToolsConversion;
		this->timeMeshBackTrackerFromMToolsConversion = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeFromMToolsConversion;

		this->PMJMesh::MainDrive::front = static_cast<MJMesh::Front *>(this->backTracking->getFront());

		if (this->backTracking->getStatus().getType() == this->backTracking->ended.getType())
		{
			this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;

			delete this->backTracking;

			this->backTracking = NULL;
		}
		else if (this->backTracking->getStatus().getType() == this->backTracking->rejected.getType())
		{
			this->PMJMesh::MainDrive::front = static_cast<MJMesh::Front *>(this->backTracking->getFront());

			this->status = MJMesh::MainDrive::errorMeshNotGeneratedBackTracking;

			delete this->backTracking;

			this->backTracking = NULL;
		}
	}
	else
	{
		this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;
	}

	Data::setStepByStep(step);
#else
	if (this->dimension == 3)
	{
		if (!this->backTracking)
		{
			this->backTracking = this->makeFrontBackTrackingAdvancer();

			this->backTracking->setIdManager(this->idManager);

			this->front = NULL;
		}

		this->backTracking->execute();

		this->timeMeshBackTrackerBackTracking = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeBackTracking;
		this->timeMeshBackTrackerToMToolsConversion = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeToMToolsConversion;
		this->timeMeshBackTrackerFromMToolsConversion = static_cast<PMTools::BackTrackingFrontAdvancer *>(this->backTracking)->timeFromMToolsConversion;

		this->front = static_cast<MJMesh::Front *>(this->backTracking->getFront());

		if (this->backTracking->getStatus().getType() == this->backTracking->ended.getType())
		{
			this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;

			delete this->backTracking;

			this->backTracking = NULL;
		}
		else if (this->backTracking->getStatus().getType() == this->backTracking->rejected.getType())
		{
			this->status = MJMesh::MainDrive::errorMeshNotGeneratedBackTracking;

			delete this->backTracking;

			this->backTracking = NULL;
		}
	}
	else
	{
		this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;
	}
#endif //#endif //#if USE_GUI

	if (!this->front)
	{
		static_cast<GlobalTaskManager *>(this->geometryManager)->setFront(static_cast<PMJMesh::Front *>(this->front));
	}
#else
	bool ret = PMJMesh::MainDrive::executeFrontBackTrackingAdvancer();
#endif //#if DEBUG_TIMING_PMTOOLS

	if ((this->comm->isMaster()) && (this->PMJMesh::MainDrive::tree))
	{
#if USE_GUI
		//to prevent interface errors
		Data::BoxTree *tree = this->PMJMesh::MainDrive::tree;

		this->PMJMesh::MainDrive::tree = NULL;

		delete tree;
#else
		delete this->PMJMesh::MainDrive::tree;

		this->PMJMesh::MainDrive::tree = NULL;
#endif //#if USE_GUI

		this->MTools::MainDrive::tree = this->PMJMesh::MainDrive::tree;

		this->usingSearchTrees = false;
	}

#if DEBUG_TIMING_PMTOOLS
	this->timeMeshBackTrackerOthers = Data::time() - debugstart -
		this->timeMeshBackTrackerBackTracking -
		this->timeMeshBackTrackerToMToolsConversion - this->timeMeshBackTrackerFromMToolsConversion;
#endif //#if DEBUG_TIMING_PMTOOLS

#if DEBUG_TIMING_PMTOOLS
#if (!USE_GUI)
	if (this->status != MJMesh::MainDrive::frontBackTrackingAdvanced)
	{
		//if (this->updateType != PMJMesh::FULL)
		{
			static_cast<GlobalTaskManager *>(this->geometryManager)->finalizeMesh();
		}

		//exit(EXIT_FAILURE);
	}
#endif //#if (!USE_GUI)

	return (this->status == MJMesh::MainDrive::frontBackTrackingAdvanced);
#else
#if (!USE_GUI)
	if (!ret)
	{
		//if (this->updateType != PMJMesh::FULL)
		{
			static_cast<GlobalTaskManager *>(this->geometryManager)->finalizeMesh();
		}

		//exit(EXIT_FAILURE);
	}
#endif //#if (!USE_GUI)

	return ret;
#endif //#if DEBUG_TIMING_PMTOOLS
}

#if USE_NUMERICAL
bool PMTools::MainDrive::executeHotellingBackTransform()
{
#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	bool ret = MTools::MainDrive::executeHotellingBackTransform();

	if (ret)
    {
        this->status = this->hotellingBackTransformApplied;
    }

#if DEBUG_TIMING_PMTOOLS
	this->timeHotellingBackTransformer = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	return ret;
}
#endif //#if USE_NUMERICAL

bool PMTools::MainDrive::executeFinalize()
{
	bool ret = this->PMJMesh::MainDrive::executeFinalize();

	if (ret)
	{
		this->removeHangingVertices();
	}

	return ret;
}

/*PMJMesh::InputData *PMTools::MainDrive::makeInputData() const
{
	return new PMTools::InputData();
}

PMJMesh::InputData *PMTools::MainDrive::makeAndFillInputData() const
{
	PMTools::InputData *input = static_cast<PMTools::InputData *>(PMJMesh::MainDrive::makeAndFillInputData());

	input->setVer(this->ver);

	input->setVersion(version);
	input->setFormat(format);
	input->setFactor(factor);
	input->setTol(tol);
	input->setInternal(internal);

	input->setNumNode(num_node);
	input->setNodes(nodes);
	input->setTnodes(tnodes);
	input->setNumFace(num_face);
	input->setFaces(faces);

	return input;
}

void PMTools::MainDrive::fillWithInputData(const PMJMesh::InputData *input)
{
	PMJMesh::MainDrive::fillWithInputData(input);

	const PMTools::InputData *inp = static_cast<const PMTools::InputData *>(input);

	this->setVer(inp->getVer());

	version = inp->getVersion();
	format = inp->getFormat();
	factor = inp->getFactor();
	tol = inp->getTol();
	internal = inp->getInternal();

	num_node = inp->getNumNode();
	nodes = inp->getNodes();
	tnodes = inp->getTnodes();
	num_face = inp->getNumFace();
	faces = inp->getFaces();
}*/

Parallel::Communicator *PMTools::MainDrive::makeCommunicator() const
{
	return new PMTools::Communicator();
}

Performer::BoundaryBuilder *PMTools::MainDrive::makeBoundaryBuilder() const
{
	return PMJMesh::MainDrive::makeBoundaryBuilder();
	//return MTools::MainDrive::makeBoundaryBuilder();
}

Performer::TreeGenerator *PMTools::MainDrive::makeTreeGenerator() const
{
	return new PMTools::TreeGenerator(this->MTools::MainDrive::boundary,
		this->mshRegion->GetOctTree()
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		, static_cast<PMTools::Communicator *>(this->comm)
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		);
}

MTools::MshRegion3D *PMTools::MainDrive::makeMshRegion() const
{
	return new PMTools::MshRegion3D();
}

PMJMesh::TaskManager *PMTools::MainDrive::makeTaskManager() const
{
	///TODO replace all these parameters with just the InputData

	switch (this->algorithm)
	{
	case PMJMesh::GLOBAL_SHIFT:
		return new PMTools::GlobalTaskManager(this->mshRegion->GetOctTree(),
			this->parallel, this->dimension, this->PMJMesh::MainDrive::boundary,
			this->PMJMesh::MainDrive::tree, this->percentageNonWorkingTask,
			this->heightDependentInLoad, this->removeLayersInLoad, this->smoothInSlavesEnabled,
			this->numImproves, this->smoothingFactor,
#if (!USE_DELETE_FELS)
			this->unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			this->smoothingLayer, this->localBackTrackingEnabled,
			this->meshSorted, this->maxBackTracks, this->partitionerType,
			this->updateType, this->shiftType,
			this->usePartitioningThreshold, this->partitioningThreshold,
			this->boundarySorted, this->usingSearchTrees, this->rangeProportion,
			this->minRefinementLevel, this->skippingInnerInDecomposition,
			this->createInnerFrontInDecomposition,
			this->loadBalancerType, this->balancer,
			this->comm);

	case PMJMesh::GLOBAL_SHIFT_WITH_ADJACENCY:
		return NULL;
		//return new AdjacentGlobalTaskManager();

	case PMJMesh::LOCAL_SHIFT:
		return NULL;
		//return new LocalTaskManager();

	case PMJMesh::LOCAL_SHIFT_WITH_COVARIANCE:
		return NULL;
		//return new CovarianceLocalTaskManager();

	case PMJMesh::A_PRIORI_INTERFACE_MESH:
		return NULL;
		//return new APrioriTaskManager(this->parallel, this->dimension,
		//	this->boundary, this->tree, this->percentageNonWorkingTask,
		//	this->smoothInSlavesEnabled, this->numImproves, this->smoothingFactor,
		//	this->unsmoothingLayer, this->smoothingLayer, this->localBackTrackingEnabled,
		//	this->meshSorted, this->maxBackTracks, this->updateType, this->position,
		//	this->usePartitioningThreshold, this->partitioningThreshold, this->boundarySorted,
		//	this->usingSearchTrees, this->minRefinementLevel, this->loadBalancerType,
		//	this->balancer, this->comm);
	}

	return NULL;
}

MJMesh::BackTrackingFrontAdvancer *PMTools::MainDrive::makeFrontBackTrackingAdvancer() const
{
	return new PMTools::BackTrackingFrontAdvancer(this->mshRegion->GetOctTree(),
		this->PMJMesh::MainDrive::boundary, this->PMJMesh::MainDrive::front, this->PMJMesh::MainDrive::mesh);
}

#endif //#if (USE_MTOOLS)
