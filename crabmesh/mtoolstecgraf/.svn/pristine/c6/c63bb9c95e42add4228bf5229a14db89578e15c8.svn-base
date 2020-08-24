#include "PMJMesh/MainDrive.h"

#include "Data/BoxTree.h"
#include "Data/FrontElement.h"
#include "Data/Vertex.h"
#include "Data/BoxSearchTree.h"
#include "Performer/IdManager.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/InputData.h"
#include "PMJMesh/Communicator.h"
#include "PMJMesh/BoundaryBuilder.h"
#include "PMJMesh/HotellingTransformer.h"
#include "PMJMesh/TreeGenerator.h"
#include "PMJMesh/MinTreeRefiner.h"
#include "PMJMesh/NeighborTreeRefiner.h"
#include "PMJMesh/GlobalTaskManager.h"
#include "PMJMesh/APrioriTaskManager.h"
#include "Parallel/WeightedGreedySerialStaticLoadBalancer.h"
#include "Parallel/SpringSerialStaticLoadBalancer.h"
#include "Parallel/DemandParallelDynamicLoadBalancer.h"

//debug
#include "Data/Mesh.h"
#include "MJMesh/Vertex.h"
//endebug

using namespace PMJMesh;

const Performer::Status PMJMesh::MainDrive::errorPMJMesh =
	Performer::Status(PMJMesh::MainDrive::ERROR_PJMESH, "error - pjmesh");

const Performer::Status PMJMesh::MainDrive::runtimeInfoSent =
	Performer::Status(PMJMesh::MainDrive::RUNTIME_INFO_SENT, "runtime info sent");

PMJMesh::MainDrive::MainDrive(Parallel::Communicator *comm, enum Parallel::AvailableLoadBalancers type) :
	MJMesh::MainDrive(),
	Parallel::MainDrive(comm)
{
	///differences between constructor, cli and gui:
	///                                   constructor  cli    gui
	///setParallel()                      true         false  user
	///setSkippingInnerInDecomposition()  false        true   false
	///setRemoveLayersInLoad()            2            2      0
	///setHeightDependentInLoad()         true         true   false
	///setTreeFactor()                    1.0          file   user

	this->balancer = NULL;
	this->geometryManager = NULL;
#if USE_TIMING
	this->treeGenerationTime = 0.0;
#endif //#if USE_TIMING

	this->setComm(comm ? comm : this->makeCommunicator());

	//if ((this->comm) && (!this->comm->isInitialized()))
	if (this->comm)
	{
		this->comm->init();
	}

	this->setLoadBalancerType(type);

	this->setClassifyTreeOnConstruction(true);

	this->setRangeProportion(1.0);

	this->setUsePartitioningThreshold(false);
	this->setPartitioningThreshold(0.0);
	this->setPercentageNonWorkingTask(0.0);
	this->setRemoveLayersInLoad(2);
	this->setHeightDependentInLoad(true);

	this->setSmoothInSlavesEnabled(true);
	this->setPartitionerType(BOX_LOAD);
	this->setUpdateType(EMPTY_NO_BOUNDARY);
	this->setShiftType(LATERAL);
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(0);
#endif //#if (!USE_DELETE_FELS)
	this->setSmoothingLayer(1);

	this->setAlgorithm(GLOBAL_SHIFT);

	this->setUsingSearchTrees(true);

	this->setParallel(true);

	this->setUsingUniformRefinement(false);
	this->setMinRefinementPostponed(true);
	this->minRefinementLevel = 0;

	this->setSkippingInnerInDecomposition(false);
	this->setCreateInnerFrontInDecomposition(false);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	this->elementsMap = NULL;
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshInit = this->timePMJMeshBoundaryBuilder =
		this->timePMJMeshTreeGeneratorGeneration =
		this->timePMJMeshTreeGeneratorClassification =
		this->timePMJMeshTreeGeneratorOthers =
		this->timePMJMeshMeshGeneratorMasterTreeDecomposition =
		this->timePMJMeshMeshGeneratorMasterFrontDecomposition =
		this->timePMJMeshMeshGeneratorWorkerMeshGeneration =
		this->timePMJMeshMeshGeneratorWorkerMeshImprovement =
		this->timePMJMeshMeshGeneratorWorkerTreeClassification =
		this->timePMJMeshMeshGeneratorWorkerOthers =
		this->timePMJMeshMeshGeneratorMasterSendTasks =
		this->timePMJMeshMeshGeneratorMasterReceiveTasks =
		this->timePMJMeshMeshGeneratorMasterMeshUpdate =
		this->timePMJMeshMeshGeneratorMasterTreeShift =
		this->timePMJMeshMeshGeneratorMasterFinalization =
		this->timePMJMeshMeshGeneratorMasterOthers =
		this->timePMJMeshMeshImprovement =
		this->timePMJMeshOverhead = 0.0;
#endif //#if DEBUG_TIMING_PMJMESH
}

PMJMesh::MainDrive::~MainDrive()
{
	this->clear();

	if (this->comm)
	{
		if ((this->status.getType() == MJMesh::MainDrive::notInit.getType()) &&
#if USE_GUI
			(this->boundaryBuilder) &&
#endif //#if USE_GUI
			(!this->comm->isFinalized()) &&
			(this->comm->isMaster()))
		{
			UInt msg = 0;

			this->comm->broadcast(msg, -1);
		}

		this->comm->finalize();
//#endif //#if USE_GUI
	}
}

void PMJMesh::MainDrive::setAlgorithm(enum AvailableAlgorithms algorithm)
{
	this->algorithm = algorithm;
}

enum AvailableAlgorithms PMJMesh::MainDrive::getAlgorithm() const
{
	return this->algorithm;
}

void PMJMesh::MainDrive::setParallel(bool parallel)
{
	this->parallel = parallel;

	//static_cast<Parallel::MPICommunicator *>(this->comm)->setShParallel(this->parallel);
	static_cast<Parallel::TMCommunicator *>(this->comm)->setSharedParallelismEnabled(this->parallel);
}

void PMJMesh::MainDrive::setMinRefinementPostponed(bool minRefinementPostponed)
{
	this->minRefinementPostponed = minRefinementPostponed;
}

bool PMJMesh::MainDrive::isMinRefinementPostponed() const
{
	return this->minRefinementPostponed;
}

void PMJMesh::MainDrive::setClassifyTreeOnConstruction(bool classifyTreeOnConstruction)
{
	this->classifyTreeOnConstruction = classifyTreeOnConstruction;
}

void PMJMesh::MainDrive::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

void PMJMesh::MainDrive::setUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
}

void PMJMesh::MainDrive::setPartitioningThreshold(Real partitioningThreshold)
{
	this->partitioningThreshold = partitioningThreshold;
}

void PMJMesh::MainDrive::setPartitioningThreshold(UInt partitioningThreshold)
{
	this->partitioningThreshold = static_cast<Real>(partitioningThreshold);
}

void PMJMesh::MainDrive::setPercentageNonWorkingTask(Real percentageNonWorkingTask)
{
	this->percentageNonWorkingTask = percentageNonWorkingTask;
}

void PMJMesh::MainDrive::setRemoveLayersInLoad(UInt removeLayersInLoad)
{
	this->removeLayersInLoad = removeLayersInLoad;
}

void PMJMesh::MainDrive::setHeightDependentInLoad(bool heightDependentInLoad)
{
	this->heightDependentInLoad = heightDependentInLoad;
}

void PMJMesh::MainDrive::setSmoothInSlavesEnabled(bool smoothInSlavesEnabled)
{
	this->smoothInSlavesEnabled = smoothInSlavesEnabled;
}

#if (!USE_DELETE_FELS)
void PMJMesh::MainDrive::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::MainDrive::setSmoothingLayer(UInt smoothingLayer)
{
	this->smoothingLayer = smoothingLayer;
}

void PMJMesh::MainDrive::setPartitionerType(enum PartitionerType partitionerType)
{
	this->partitionerType = partitionerType;
}

enum PartitionerType PMJMesh::MainDrive::getPartitionerType() const
{
	return this->partitionerType;
}

void PMJMesh::MainDrive::setUpdateType(enum MeshUpdateType updateType)
{
	this->updateType = updateType;
}

enum MeshUpdateType PMJMesh::MainDrive::getUpdateType() const
{
	return this->updateType;
}

void PMJMesh::MainDrive::setShiftType(enum ShiftType shiftType)
{
	this->shiftType = shiftType;
}

enum ShiftType PMJMesh::MainDrive::getShiftType() const
{
	return this->shiftType;
}

void PMJMesh::MainDrive::setLoadBalancerType(enum Parallel::AvailableLoadBalancers type)
{
	this->loadBalancerType = type;

	if (this->loadBalancer)
	{
		delete this->loadBalancer;

		this->loadBalancer = NULL;
	}
}

void PMJMesh::MainDrive::setSkippingInnerInDecomposition(bool skippingInnerInDecomposition)
{
	this->skippingInnerInDecomposition = skippingInnerInDecomposition;
}

void PMJMesh::MainDrive::setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition)
{
	this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
}

#if USE_GUI
void PMJMesh::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	MJMesh::MainDrive::setBoundary(boundary);

	if (this->geometryManager)
	{
		GlobalTaskManager *manager = dynamic_cast<GlobalTaskManager *>(this->geometryManager);

		if (manager)
		{
			manager->setBoundary(boundary);
		}
	}
}

void PMJMesh::MainDrive::setTree(Data::BoxTree *tree)
{
	MJMesh::MainDrive::setTree(tree);

	if (this->geometryManager)
	{
		this->geometryManager->setTree(tree);
	}
}

void PMJMesh::MainDrive::setFront(MJMesh::Front *front)
{
	MJMesh::MainDrive::setFront(front);

	if (this->geometryManager)
	{
		GlobalTaskManager *manager = dynamic_cast<GlobalTaskManager *>(this->geometryManager);

		if (manager)
		{
			manager->setFront(dynamic_cast<PMJMesh::Front *>(front));
		}
	}
}

void PMJMesh::MainDrive::setMesh(MJMesh::Mesh *mesh)
{
	MJMesh::MainDrive::setMesh(mesh);

	if (this->geometryManager)
	{
		GlobalTaskManager *manager = dynamic_cast<GlobalTaskManager *>(this->geometryManager);

		if (manager)
		{
			manager->setMesh(mesh);
		}
	}
}

#if USE_NEW_FRONT_UPDATER
const Data::Mesh *PMJMesh::MainDrive::getUnsmoothedMesh() const
#else
Data::Mesh *PMJMesh::MainDrive::getUnsmoothedMesh() const
#endif //#if USE_NEW_FRONT_UPDATER
{
	return (this->geometryManager) ? static_cast<GlobalTaskManager *>(this->geometryManager)->getUnsmoothedMesh() : NULL;
}
#endif //#if USE_GUI

Parallel::TaskVector PMJMesh::MainDrive::getTasks() const
{
	return (this->geometryManager) ? this->geometryManager->getTasks() : Parallel::TaskVector();
}

bool PMJMesh::MainDrive::loadFile()
{
#if (!USE_WORKER_FILE_LOADER)
	if (this->comm->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
		return MJMesh::MainDrive::loadFile();
	}

	return false;
}

bool PMJMesh::MainDrive::saveFile() const
{
	if (this->comm->isMaster())
	{
		return MJMesh::MainDrive::saveFile();
	}

	return false;
}

void PMJMesh::MainDrive::execute()
{
	if ((this->status.getType() == MJMesh::MainDrive::success.getType()) ||
		(this->status.getType() >= MJMesh::MainDrive::error.getType()))
	{
		return;
	}

	//std::cout << "debug" << std::endl;

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real startParallel = 0.0;
	Real endParallel = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if DEBUG_TIMING_PMJMESH
	Real debugruntime = 0.0;
#endif //#if DEBUG_TIMING_PMJMESH

	//Real debugstart = Data::time();

	switch (this->status.getType())
	{
	case MJMesh::MainDrive::NOT_INIT:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeInit())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeInit() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeInit() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

	case MJMesh::MainDrive::INIT:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		//begin adjusting boundary

		if (!this->executeBoundaryBuilder(true))
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeBoundaryBuilder() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeBoundaryBuilder() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

		//end adjusting boundary
	case MJMesh::MainDrive::BOUNDARY_BUILT:
#if USE_NUMERICAL
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

        //begin hotelling transform
        if (!this->executeHotellingTransform())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeHotellingTransform() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeHotellingTransform() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

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

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeTreeGenerator() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeTreeGenerator() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_GENERATED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeTreeMinRefiner(true))
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeTreeMinRefiner() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeTreeMinRefiner() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_MIN_REFINED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeTreeNeighborRefiner(true))
		{
			break;
		}

		//end adjusting tree

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeTreeNeighborRefiner() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeTreeNeighborRefiner() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

/*#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI*/

	case MJMesh::MainDrive::TREE_NEIGHBOR_REFINED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeSendRuntimeInfo())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeSendRuntimeInfo() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeSendRuntimeInfo() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

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

	case PMJMesh::MainDrive::RUNTIME_INFO_SENT:
		//begin mesh generation

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		if (this->comm->isMaster())
		{
			startParallel = Data::time();
		}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		if (!this->executeFrontGeometryAdvancer())
		{
			break;
		}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		if (this->comm->isMaster())
		{
			endParallel = Data::time();
		}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeFrontGeometryAdvancer() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeFrontGeometryAdvancer() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeFrontTopologyAdvancer())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeFrontTopologyAdvancer() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeFrontTopologyAdvancer() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_TOPOLOGY_ADVANCED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		if (!this->executeFrontBackTrackingAdvancer())
		{
			break;
		}

		//end mesh generation

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeFrontBackTrackingAdvancer() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeFrontBackTrackingAdvancer() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		if (/*(this->dimension == 3) && */(Data::isStepByStep())) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:
		/*if (this->comm->isMaster())
		{
			std::cout << "debug status = " << this->status.getMessage()
				<< ", time = " << Data::time() - start << std::endl;
		}*/

		//begin improvement

		if (!this->executeMeshImprover())
		{
			break;
		}

		//end improvement

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeMeshImprover() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeMeshImprover() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

#if USE_GUI
		//if ((this->status.getType() != MJMesh::MainDrive::meshImproved.getType()) &&
		//	(Data::isStepByStep()))
		if (Data::isStepByStep())
		{
			break;
		}
#endif //#if USE_GUI

	case MJMesh::MainDrive::MESH_IMPROVED:
#if USE_NUMERICAL
        //begin hotelling back transform
        if (!this->executeHotellingBackTransform())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeHotellingBackTransform() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeHotellingBackTransform() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
		//debugstart = Data::time();

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
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
			std::cout << "Runtime parallel = " << endParallel - startParallel << std::endl;
			std::cout << "Runtime serial   = " << (end - start) - (endParallel - startParallel) << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#if DEBUG_TIMING_PMJMESH
			debugruntime = end - start;
#endif //#if DEBUG_TIMING_PMJMESH
		}
#endif //#if USE_TIME_MEASUREMENTS

		if (!this->executeFinalize())
		{
			break;
		}

		//if (this->comm->isMaster()) std::cout << "Master, MainDrive, time for executeFinalize() = " << Data::time() - debugstart << std::endl;
		//else {std::stringstream str; str << "Slave " << this->comm->rank() << ", MainDrive, time for executeFinalize() = " << Data::time() - debugstart << std::endl; std::cout << str.str();}
	}

	//std::cout << "debug status = " << this->status.getMessage() << std::endl;

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS

	/*//debug
	if (this->mesh)
	{
		for (Data::Mesh::VertexIterator iter2 = this->mesh->vBegin();
			 iter2 != this->mesh->vEnd(); iter2++)
		{
			std::cout << "vertex " << (*iter2)->getId() << " in fronts";

			for (MJMesh::VertexSpecificData::FrontDataMap::iterator iter3 = static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.begin();
				 iter3 != static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.end(); iter3++)
			{
				std::cout << " " << (*iter3).first;
			}

			std::cout << std::endl;
		}
	}
	//endebug*/

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshOverhead = debugruntime -
		this->timePMJMeshInit - this->timePMJMeshBoundaryBuilder -
		this->timePMJMeshHotellingTransformer -
		this->timePMJMeshTreeGeneratorGeneration - this->timePMJMeshTreeGeneratorClassification -
		this->timePMJMeshTreeGeneratorOthers -
		this->timePMJMeshMeshGeneratorMasterTreeDecomposition -
		this->timePMJMeshMeshGeneratorMasterFrontDecomposition -
		this->timePMJMeshMeshGeneratorWorkerMeshGeneration -
		this->timePMJMeshMeshGeneratorWorkerMeshImprovement -
		this->timePMJMeshMeshGeneratorWorkerTreeClassification -
		this->timePMJMeshMeshGeneratorWorkerOthers -
		this->timePMJMeshMeshGeneratorMasterMeshUpdate -
		this->timePMJMeshMeshGeneratorMasterSendTasks -
		this->timePMJMeshMeshGeneratorMasterReceiveTasks -
		this->timePMJMeshMeshGeneratorMasterTreeShift -
		this->timePMJMeshMeshGeneratorMasterFinalization -
		this->timePMJMeshMeshGeneratorMasterOthers -
		this->timePMJMeshMeshImprovement -
		this->timePMJMeshHotellingBackTransformer;

	std::stringstream str;

	str << "Timings:" << std::endl;

	str << "initializing: " << this->timePMJMeshInit << " (" << 100.0*this->timePMJMeshInit/debugruntime << " %)" << std::endl;

	str << "building boundary: " << this->timePMJMeshBoundaryBuilder << " (" << 100.0*this->timePMJMeshBoundaryBuilder/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	str << "hotelling transform: " << this->timePMJMeshHotellingTransformer << " (" << 100.0*this->timePMJMeshHotellingTransformer/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	str << "generating tree: " << this->timePMJMeshTreeGeneratorGeneration << " (" << 100.0*this->timePMJMeshTreeGeneratorGeneration/debugruntime << " %)" << std::endl;
	str << "classifying tree: " << this->timePMJMeshTreeGeneratorClassification << " (" << 100.0*this->timePMJMeshTreeGeneratorClassification/debugruntime << " %)" << std::endl;
	str << "tree overhead: " << this->timePMJMeshTreeGeneratorOthers << " (" << 100.0*this->timePMJMeshTreeGeneratorOthers/debugruntime << " %)" << std::endl;

	str << "decomposing tree - calculating load and partitioning: " << this->timePMJMeshMeshGeneratorMasterTreeDecomposition << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterTreeDecomposition/debugruntime << " %)" << std::endl;
	str << "decomposing front - initial and during shifts: " << this->timePMJMeshMeshGeneratorMasterFrontDecomposition << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterFrontDecomposition/debugruntime << " %)" << std::endl;
	str << "generating mesh in workers: " << this->timePMJMeshMeshGeneratorWorkerMeshGeneration << " (" << 100.0*this->timePMJMeshMeshGeneratorWorkerMeshGeneration/debugruntime << " %)" << std::endl;
	str << "improving mesh in workers: " << this->timePMJMeshMeshGeneratorWorkerMeshImprovement << " (" << 100.0*this->timePMJMeshMeshGeneratorWorkerMeshImprovement/debugruntime << " %)" << std::endl;
	str << "classifying tree in workers: " << this->timePMJMeshMeshGeneratorWorkerTreeClassification << " (" << 100.0*this->timePMJMeshMeshGeneratorWorkerTreeClassification/debugruntime << " %)" << std::endl;
	str << "doing other stuff in workers: " << this->timePMJMeshMeshGeneratorWorkerOthers << " (" << 100.0*this->timePMJMeshMeshGeneratorWorkerOthers/debugruntime << " %)" << std::endl;
	str << "sending front: " << this->timePMJMeshMeshGeneratorMasterSendTasks << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterSendTasks/debugruntime << " %)" << std::endl;
	str << "receiving updated front: " << this->timePMJMeshMeshGeneratorMasterReceiveTasks << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterReceiveTasks/debugruntime << " %)" << std::endl;
	str << "updating front and mesh: " << this->timePMJMeshMeshGeneratorMasterMeshUpdate << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterMeshUpdate/debugruntime << " %)" << std::endl;
	str << "shifting tree cells: " << this->timePMJMeshMeshGeneratorMasterTreeShift << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterTreeShift/debugruntime << " %)" << std::endl;
	str << "finalizing mesh by master: " << this->timePMJMeshMeshGeneratorMasterFinalization << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterFinalization/debugruntime << " %)" << std::endl;
	str << "doing other stuff in master: " << this->timePMJMeshMeshGeneratorMasterOthers << " (" << 100.0*this->timePMJMeshMeshGeneratorMasterOthers/debugruntime << " %)" << std::endl;

	str << "improvement: " << this->timePMJMeshMeshImprovement << " (" << 100.0*this->timePMJMeshMeshImprovement/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	str << "hotelling back transform: " << this->timePMJMeshHotellingBackTransformer << " (" << 100.0*this->timePMJMeshHotellingBackTransformer/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	str << "overhead: " << this->timePMJMeshOverhead << " (" << 100.0*this->timePMJMeshOverhead/debugruntime << " %)" << std::endl;

	/*str << std::endl << "FrontAdvancer timings:" << std::endl;
	str << "initializing: " << Performer::FrontAdvancer::timeInit << std::endl;
	str << "initializing (search trees): " << Performer::FrontAdvancer::timeInitMakingTrees << std::endl;
	str << "executing: " << Performer::FrontAdvancer::timeExecute << std::endl;
	str << "executing (is empty): " << Performer::FrontAdvancer::timeIsEmpty << std::endl;
	str << "executing (next): " << Performer::FrontAdvancer::timeNext << std::endl;
	str << "executing (next geometry): " << Performer::FrontAdvancer::timeNextGeometry << std::endl;
	str << "executing (next geometry reject): " << Performer::FrontAdvancer::timeNextGeometryReject << std::endl;
	str << "executing (next geometry out): " << Performer::FrontAdvancer::timeNextGeometryOut << std::endl;
	str << "executing (next topology): " << Performer::FrontAdvancer::timeNextTopology << std::endl;
	str << "executing (next topology reject): " << Performer::FrontAdvancer::timeNextTopologyReject << std::endl;
	str << "executing (best): " << Performer::FrontAdvancer::timeBest << std::endl;
	str << "executing (best geometry ideal): " << Performer::FrontAdvancer::timeIdeal << std::endl;
	str << "executing (best geometry radius): " << Performer::FrontAdvancer::timeRadius << std::endl;
	str << "executing (best geometry restricted): " << Performer::FrontAdvancer::timeRestricted << std::endl;
	str << "executing (best geometry candidates): " << Performer::FrontAdvancer::timeCandidates << std::endl;
	str << "executing (best geometry fels in range): " << Performer::FrontAdvancer::timeFrontElementsInRange << std::endl;
	str << "executing (best geometry candidates best valid): " << Performer::FrontAdvancer::timeCandidatesBestValid << std::endl;
	str << "executing (best geometry ideals best valid): " << Performer::FrontAdvancer::timeIdealBestValid << std::endl;
	str << "executing (best topology candidates): " << Performer::FrontAdvancer::timeTopologyCandidates << std::endl;
	str << "executing (best topology candidates best valid): " << Performer::FrontAdvancer::timeTopologyBestValid << std::endl;
	str << "executing (reject): " << Performer::FrontAdvancer::timeReject << std::endl;
	str << "executing (status check): " << Performer::FrontAdvancer::timeStatusCheck << std::endl;
	str << "executing (error check): " << Performer::FrontAdvancer::timeErrorCheck << std::endl;
	str << "executing (advance): " << Performer::FrontAdvancer::timeAdvance << std::endl;
	str << "ending: " << Performer::FrontAdvancer::timeEnd << std::endl;
	str << "num advs geometry: " << Performer::FrontAdvancer::numBestWithInsertion << std::endl;
	str << "num advs topology: " << Performer::FrontAdvancer::numBestWithoutInsertion << std::endl;

	str << std::endl << "BoxSearchTree timings:" << std::endl;
	str << "initializing: " << Data::BoxSearchTree::timeInit << std::endl;
	str << "executing: " << Data::BoxSearchTree::timeExecute << std::endl;
	str << "ending: " << Data::BoxSearchTree::timeEnd << std::endl;*/

	std::cout << str.str() << std::endl;
#endif //#if DEBUG_TIMING_PMJMESH
}

void PMJMesh::MainDrive::clear()
{
	if (this->balancer)
	{
		delete this->balancer;

		this->balancer = NULL;

		if (this->geometryManager)
		{
			this->geometryManager->setLoadBalancer(NULL);
		}
	}

	if (this->geometryManager)
	{
		delete this->geometryManager;

		this->geometryManager = NULL;
	}

	//std::cout << "debug Z" << std::endl;

	if (this->boundary)
	{
		if (this->comm->isSlave())
		{
			if (this->front)
			{
				this->front->clear();
			}

			//Data::FrontElementVector elements = this->boundary->getElements();
			Data::FrontElementList elements = this->boundary->getElements();
			this->boundary->setElements(Data::FrontElementList());

			while (!elements.empty())
			{
				Data::FrontElement *e = elements.back();
				elements.pop_back();

				if (e)
				{
					delete e;
				}
			}

			//Data::VertexVector vertices = this->boundary->getVertices();
			//Data::VertexList vertices = this->boundary->getVertices();
			Data::VertexList vertices = this->boundary->getVertices();
			this->boundary->setVertices(Data::VertexList());

			while (!vertices.empty())
			{
				Data::Vertex *v = vertices.back();
				vertices.pop_back();

				if (v)
				{
					delete v;
				}
			}

			//std::cout << "debug Z slave" << std::endl;
		}

		//std::cout << "debug X " << this->comm->rank() << std::endl;

		if ((this->comm->isSlave()) || (this->front) || (this->mesh))
		{
			this->boundary->clear();
		}

		//std::cout << "debug X2 " << std::endl;
	}

	MJMesh::MainDrive::clear();
}

bool PMJMesh::MainDrive::executeInit()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	if ((this->partitionerType == SMART_BSP_NUMBER) &&
		(this->minRefinementPostponed))
	{
		this->minRefinementPostponed = false;
	}

#if USE_POOL
	PMJMesh::initThreadPools(static_cast<PMJMesh::Communicator *>(this->comm));
#endif //#if USE_POOL

#if (!USE_WORKER_FILE_LOADER)
	if (this->comm->isParallel())
	{
		InputData *input = NULL;

		if (this->comm->isMaster())
		{
			UInt msg = 1;

			this->comm->broadcast(msg, -1);

			input = this->makeAndFillInputData();

			//this->comm->syncSend(input);
			this->comm->broadcast(input);
		}
		else
		{
			UInt msg = 33;

			this->comm->broadcast(msg, this->comm->root());

			if (msg == 0)
			{
				this->status = MJMesh::MainDrive::success;

				return false;
			}

			//input = (InputData *)this->comm->syncReceive();
			input = static_cast<InputData *>(this->comm->broadcast(this->comm->root()));

			this->fillWithInputData(input);

			this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());
		}

		delete input;
	}
#endif //#if (!USE_WORKER_FILE_LOADER)

#if DEBUG_TIMING_PMJMESH
	bool ret = MJMesh::MainDrive::executeInit();

	this->timePMJMeshInit = Data::time() - debugstart;

	return ret;
#else
	return MJMesh::MainDrive::executeInit();
#endif //#if DEBUG_TIMING_PMJMESH
}

bool PMJMesh::MainDrive::executeBoundaryBuilder(bool del)
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if (!USE_WORKER_FILE_LOADER)
	static_cast<PMJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setDimension(this->dimension);
#endif //#if (!USE_WORKER_FILE_LOADER)

	if (this->idManager)
	{
		delete this->idManager;

		this->idManager = NULL;
	}

#if DEBUG_TIMING_PMJMESH
	bool ret = MJMesh::MainDrive::executeBoundaryBuilder(del);

	this->timePMJMeshBoundaryBuilder = Data::time() - debugstart;

	return ret;
#else
#if USE_POOL
	bool ret = MJMesh::MainDrive::executeBoundaryBuilder(del);

#if USE_OPENMP
	//PMJMesh::mergeThreadPools();
#endif //#if USE_OPENMP

	return ret;
#else
	return MJMesh::MainDrive::executeBoundaryBuilder(del);
#endif //#endif //#if USE_POOL
#endif //#if DEBUG_TIMING_PMJMESH
}

#if USE_NUMERICAL
#if DEBUG_TIMING_PMJMESH
bool PMJMesh::MainDrive::executeHotellingTransform()
{
	Real debugstart = Data::time();

	bool ret = MJMesh::MainDrive::executeHotellingTransform();

	this->timePMJMeshHotellingTransformer = Data::time() - debugstart;

	return ret;
}
#endif //#if DEBUG_TIMING_PMJMESH
#endif //#if USE_NUMERICAL

bool PMJMesh::MainDrive::executeTreeGenerator(bool del)
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_TIMING
	this->treeGenerationTime = 0.0;
#endif //#if USE_TIMING

	//debug
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI
	//endebug

#if USE_TIMING
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	bool result = MJMesh::MainDrive::executeTreeGenerator(!this->classifyTreeOnConstruction);

#if DEBUG_TIMING_PMJMESH
	Real timegen = dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->timePMJMeshGeneration;
	Real timecla = dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->timePMJMeshClassification;
#endif //#if DEBUG_TIMING_PMJMESH

	if ((this->classifyTreeOnConstruction) &&
		(this->treeGenerator->getStatus().getType() == dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->ended.getType()))
	{
		if (result)
		{
#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
			this->elementsMap = dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->getElementsMap();
			dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->setElementsMap(NULL);
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		}

		if (del)
		{
			delete this->treeGenerator;

			this->treeGenerator = NULL;
		}
	}

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

	//debug
#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI
	//endebug

#if ((USE_POOL) && (USE_OPENMP))
	//PMJMesh::mergeThreadPools();
#endif //#if ((USE_POOL) && (USE_OPENMP))

#if DEBUG_TIMING_PMJMESH
	Real debugruntime = Data::time() - debugstart;

	this->timePMJMeshTreeGeneratorGeneration += timegen;
	this->timePMJMeshTreeGeneratorClassification += timecla;
	this->timePMJMeshTreeGeneratorOthers += debugruntime - timegen - timecla;
#endif //#if DEBUG_TIMING_PMJMESH

	return result;
}

bool PMJMesh::MainDrive::executeTreeMinRefiner(bool del)
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_TIMING
	Real start = Data::time();
#endif //#if USE_TIMING

	bool result = MJMesh::MainDrive::executeTreeMinRefiner(!this->classifyTreeOnConstruction);

#if DEBUG_TIMING_PMJMESH
	if (this->minTreeRefiner)
	{
		Real timegen = dynamic_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->timePMJMeshGeneration;
		Real timecla = dynamic_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->timePMJMeshClassification;
	}
#endif //#if DEBUG_TIMING_PMJMESH

	if ((this->classifyTreeOnConstruction) &&
		(this->minTreeRefiner) &&
		(this->minTreeRefiner->getStatus().getType() == this->minTreeRefiner->ended.getType()))
	{
		if (result)
		{
			this->minRefinementLevel = (this->minRefinementPostponed) ?
				static_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->getMinLevel() : 0;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
			static_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->setElementsMap(NULL);
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		}

		if (del)
		{
			delete this->minTreeRefiner;

			this->minTreeRefiner = NULL;
		}
	}

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

#if ((USE_POOL) && (USE_OPENMP))
	//PMJMesh::mergeThreadPools();
#endif //#if ((USE_POOL) && (USE_OPENMP))

#if DEBUG_TIMING_PMJMESH
	Real debugruntime = Data::time() - debugstart;

	this->timePMJMeshTreeGeneratorGeneration += timegen;
	this->timePMJMeshTreeGeneratorClassification += timecla;
	this->timePMJMeshTreeGeneratorOthers += debugruntime - timegen - timecla;
#endif //#if DEBUG_TIMING_PMJMESH

	return result;
}

bool PMJMesh::MainDrive::executeTreeNeighborRefiner(bool del)
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_TIMING
	Real start = Data::time();
#endif //#if USE_TIMING

	bool result = MJMesh::MainDrive::executeTreeNeighborRefiner(!this->classifyTreeOnConstruction);

#if DEBUG_TIMING_PMJMESH
	Real timegen = dynamic_cast<PMJMesh::NeighborTreeRefiner *>(this->neighborRefiner)->timePMJMeshGeneration;
	Real timecla = dynamic_cast<PMJMesh::NeighborTreeRefiner *>(this->neighborRefiner)->timePMJMeshClassification;
#endif //#if DEBUG_TIMING_PMJMESH

	if ((this->classifyTreeOnConstruction) &&
		(this->neighborRefiner->getStatus().getType() == this->neighborRefiner->ended.getType()))
	{
		if (del)
		{
			delete this->neighborRefiner;

			this->neighborRefiner = NULL;
		}
	}

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

#if ((USE_POOL) && (USE_OPENMP))
	//PMJMesh::mergeThreadPools();
#endif //#if ((USE_POOL) && (USE_OPENMP))

#if DEBUG_TIMING_PMJMESH
	Real debugruntime = Data::time() - debugstart;

	this->timePMJMeshTreeGeneratorGeneration += timegen;
	this->timePMJMeshTreeGeneratorClassification += timecla;
	this->timePMJMeshTreeGeneratorOthers += debugruntime - timegen - timecla;
#endif //#if DEBUG_TIMING_PMJMESH

	return result;
}

bool PMJMesh::MainDrive::executeSendRuntimeInfo()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	static_cast<PMJMesh::Communicator *>(this->comm)->initThreadCount();

	if ((this->comm->isParallel()) &&
		(this->loadBalancerType == Parallel::WEIGHTED_GREEDY_SERIAL_STATIC))
	{
		/*std::cout << "debug tree generation time of process "
				  << this->comm->rank() << " is "
				  << this->treeGenerationTime << std::endl;*/

#if USE_TIMING
		static_cast<PMJMesh::Communicator *>(this->comm)->calcRuntimeInfo(this->treeGenerationTime);
#else
		static_cast<PMJMesh::Communicator *>(this->comm)->calcRuntimeInfo(1.0);
#endif //#if USE_TIMING
	}

	this->status = runtimeInfoSent;

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshTreeGeneratorOthers += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return true;
}

bool PMJMesh::MainDrive::executeFrontGeometryAdvancer()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	if (!this->balancer)
	{
		this->balancer = this->makeLoadBalancer();
	}

	if (!this->geometryManager)
	{
		this->geometryManager = this->makeTaskManager();

		this->geometryManager->setIdManager(this->idManager);
	}

	if ((this->algorithm == GLOBAL_SHIFT) || (this->algorithm == A_PRIORI_INTERFACE_MESH))
	{
		this->front = static_cast<GlobalTaskManager *>(this->geometryManager)->getFront();
		this->mesh = static_cast<GlobalTaskManager *>(this->geometryManager)->getMesh();
	}

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	this->geometryManager->execute();

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterTreeDecomposition = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterTreeDecomposition;
	this->timePMJMeshMeshGeneratorMasterFrontDecomposition = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterFrontDecomposition;
	this->timePMJMeshMeshGeneratorWorkerMeshGeneration = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerMeshGeneration;
	this->timePMJMeshMeshGeneratorWorkerMeshImprovement = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerMeshImprovement;
	this->timePMJMeshMeshGeneratorWorkerTreeClassification = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerTreeClassification;
	this->timePMJMeshMeshGeneratorWorkerOthers = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorWorkerOthers;
	this->timePMJMeshMeshGeneratorMasterSendTasks = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterSendTasks;
	this->timePMJMeshMeshGeneratorMasterReceiveTasks = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterReceiveTasks;
	this->timePMJMeshMeshGeneratorMasterMeshUpdate = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterMeshUpdate;
	this->timePMJMeshMeshGeneratorMasterTreeShift = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterTreeShift;
	this->timePMJMeshMeshGeneratorMasterFinalization = static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterFinalization;
	this->timePMJMeshMeshGeneratorMasterOthers += static_cast<PMJMesh::GlobalTaskManager *>(this->geometryManager)->timeMeshGeneratorMasterOthers;

	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	bool returnCode = false;

	if (this->geometryManager->getStatus().getType() != GlobalTaskManager::success.getType())
	{
		if (this->geometryManager->getStatus().getType() >= GlobalTaskManager::error.getType())
		{
			this->status = this->error;
		}

		//this->status = this->geometryManager->getStatus();
		std::stringstream str;

		str << "parallel front advancer - " << this->geometryManager->getStatus().getMessage();

		this->status.setMessage(str.str());

		returnCode = false;
	}
	else
	{
		returnCode = true;

		/*//debug this should be in executeFrontBacKTrackingAdvancer()
		if (this->updateType != PMJMesh::FULL)
		{
			static_cast<GlobalTaskManager *>(this->geometryManager)->splitMesh();
		}
		//endebug*/

		this->status = MJMesh::MainDrive::frontGeometryAdvanced;
	}

	/*if (this->geometryManager)
	{
		delete this->geometryManager;

		this->geometryManager = NULL;
	}*/

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return returnCode;
}

bool PMJMesh::MainDrive::executeFrontTopologyAdvancer()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	/*if (this->tree)
	{
#if USE_GUI
		//to prevent interface errors
		Data::BoxTree *tree = this->tree;

		this->tree = NULL;

		delete tree;
#else
		delete this->tree;

		this->tree = NULL;
#endif //#if USE_GUI

		this->usingSearchTrees = false;
	}*/

	if (this->comm->isSlave())
	{
		/*if (this->tree)
		{
#if USE_GUI
			//to prevent interface errors
			Data::BoxTree *tree = this->tree;

			this->tree = NULL;

			delete tree;
#else
			delete this->tree;

			this->tree = NULL;
#endif //#if USE_GUI

			this->usingSearchTrees = false;
		}*/

		this->status = MJMesh::MainDrive::frontTopologyAdvanced;

		return true;
	}

#if USE_UNIQUE_SEARCH_TREE
	if ((this->front) && (static_cast<PMJMesh::Front *>(this->front)->getVSTree()))
	{
		static_cast<Data::BoxSearchTree *>(static_cast<PMJMesh::Front *>(this->front)->getVSTree())->setTree(NULL);
		static_cast<Data::BoxSearchTree *>(static_cast<PMJMesh::Front *>(this->front)->getFESTree())->setTree(NULL);
	}
#endif //#if USE_UNIQUE_SEARCH_TREE

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	bool val = MJMesh::MainDrive::executeFrontTopologyAdvancer();

	this->front->deleteSearchTrees();

	/*if (this->tree)
	{
#if USE_GUI
		//to prevent interface errors
		Data::BoxTree *tree = this->tree;

		this->tree = NULL;

		delete tree;
#else
		delete this->tree;

		this->tree = NULL;
#endif //#if USE_GUI

		this->usingSearchTrees = false;
	}*/

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterFinalization += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return val;
}

bool PMJMesh::MainDrive::executeFrontBackTrackingAdvancer()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	/*if (this->tree)
	{
		delete this->tree;

		this->tree = NULL;
	}*/

	if (this->comm->isSlave())
	{
		if (!this->front)
		{
			static_cast<GlobalTaskManager *>(this->geometryManager)->setFront(static_cast<PMJMesh::Front *>(this->front));
		}

#if DEBUG_TIMING_PMJMESH
		this->timePMJMeshMeshGeneratorMasterFinalization += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

		this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;

		return true;
	}

	//if (this->updateType != PMJMesh::FULL)
	{
		static_cast<GlobalTaskManager *>(this->geometryManager)->splitMesh();

		//debug
		//this->status = this->MJMesh::MainDrive::frontBackTrackingAdvanced;
		//this->status = this->MJMesh::MainDrive::error;
		//return false;
		//endebug
	}

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	bool ret = MJMesh::MainDrive::executeFrontBackTrackingAdvancer();

	if (!this->front)
	{
		static_cast<GlobalTaskManager *>(this->geometryManager)->setFront(static_cast<PMJMesh::Front *>(this->front));
	}

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterFinalization += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return ret;

/*#if DEBUG_TIMING_PMJMESH
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	bool ret = MJMesh::MainDrive::executeFrontBackTrackingAdvancer();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshGeneratorMasterFinalization += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return ret;
#else
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);

	bool ret = MJMesh::MainDrive::executeFrontBackTrackingAdvancer();

	Data::setStepByStep(step);

	return ret;
#else
	return MJMesh::MainDrive::executeFrontBackTrackingAdvancer();
#endif //#endif //#if USE_GUI
#endif //#if DEBUG_TIMING_PMJMESH*/
}

bool PMJMesh::MainDrive::executeMeshImprover()
{
	if (this->comm->isSlave())
	{
		this->status = MJMesh::MainDrive::meshImproved;

		return true;
	}

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	bool val = MJMesh::MainDrive::executeMeshImprover();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshMeshImprovement = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	return val;
}

#if USE_NUMERICAL
#if DEBUG_TIMING_PMJMESH
bool PMJMesh::MainDrive::executeHotellingBackTransform()
{
	Real debugstart = Data::time();

	bool ret = MJMesh::MainDrive::executeHotellingBackTransform();

	this->timePMJMeshHotellingBackTransformer = Data::time() - debugstart;

	return ret;
}
#endif //#if DEBUG_TIMING_PMJMESH
#endif //#if USE_NUMERICAL

bool PMJMesh::MainDrive::executeFinalize()
{
	//if (this->updateType != PMJMesh::FULL)
	{
		static_cast<GlobalTaskManager *>(this->geometryManager)->finalizeMesh();
	}

	if (this->comm->isSlave())
	{
		this->status = MJMesh::MainDrive::success;

		return true;
	}

	/*//debug
	bool val = MJMesh::MainDrive::executeFinalize();
	std::cout << "final improved mesh" << std::endl;
	this->test(this->mesh, this->boundary, this->front);
	return val;
	//endebug*/

#if USE_POOL
	bool ret = MJMesh::MainDrive::executeFinalize();

#if USE_OPENMP
	PMJMesh::mergeThreadPools();
#endif //#if USE_OPENMP
	PMJMesh::finalizeThreadPools(static_cast<PMJMesh::Communicator *>(this->comm));

	return ret;
#else
	return MJMesh::MainDrive::executeFinalize();
#endif //#if USE_POOL
}

#if (!USE_WORKER_FILE_LOADER)
InputData *PMJMesh::MainDrive::makeInputData() const
{
	return new InputData();
}

InputData *PMJMesh::MainDrive::makeAndFillInputData() const
{
	InputData *input = this->makeInputData();

	input->setIdVertex(this->idManager ? this->idManager->getId(0) : 1);
	input->setIdFrontElement(this->idManager ? this->idManager->getId(1) : 1);
	input->setIdElement(this->idManager ? this->idManager->getId(2) : 1);

	input->setDimension(this->dimension);

	input->setAlgorithm(this->algorithm);

#if USE_NUMERICAL
	input->setHotellingTransformEnabled(this->hotellingTransformEnabled);
#endif //#if USE_NUMERICAL
	input->setBoundarySorted(this->boundarySorted);
	input->setUsingSearchTrees(this->usingSearchTrees);
	input->setRangeProportion(this->rangeProportion);
	input->setTreeFactor(this->treeFactor);

	input->setSmoothInSlavesEnabled(this->smoothInSlavesEnabled);
	input->setNumImproves(this->numImproves);
	input->setSmoothingFactor(this->smoothingFactor);
#if (!USE_DELETE_FELS)
	input->setUnsmoothingLayer(this->unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
	input->setSmoothingLayer(this->smoothingLayer);
	input->setMeshSorted(this->meshSorted);
	input->setLocalBackTrackingEnabled(this->localBackTrackingEnabled);
	input->setMaxBackTracks(this->maxBackTracks);

	input->setUpdateType(this->updateType);

	input->setParallel(this->parallel);
	input->setMinRefinementPostponed(this->minRefinementPostponed);
	input->setUsePartitioningThreshold(this->usePartitioningThreshold);
	input->setPartitioningThreshold(this->partitioningThreshold);
	input->setLoadBalancer(this->loadBalancerType);

	input->setSkippingInnerInDecomposition(this->skippingInnerInDecomposition);
	input->setCreateInnerFrontInDecomposition(this->createInnerFrontInDecomposition);

	input->setTolerance(Data::getTolerance());

#if USE_GUI
	input->setStepByStep(Data::isStepByStep());
#endif //#if USE_GUI

	return input;
}

void PMJMesh::MainDrive::fillWithInputData(const InputData *input)
{
	if (this->idManager)
	{
		delete this->idManager;
	}

	this->idManager = this->makeIdManager(NULL);

	this->idManager->setId(0, input->getIdVertex());
	this->idManager->setId(1, input->getIdFrontElement());
	this->idManager->setId(2, input->getIdElement());

	this->dimension = input->getDimension();

	this->algorithm = input->getAlgorithm();

	this->setBoundarySorted(input->isBoundarySorted());
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(input->isHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
	this->setTreeFactor(input->getTreeFactor());
	this->setUsingSearchTrees(input->isUsingSearchTrees());
	this->setRangeProportion(input->getRangeProportion());

	this->setSmoothInSlavesEnabled(input->isSmoothInSlavesEnabled());
	this->setNumImproves(input->getNumImproves());
	this->setSmoothingFactor(input->getSmoothingFactor());
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(input->getUnsmoothingLayer());
#endif //#if (!USE_DELETE_FELS)
	this->setSmoothingLayer(input->getSmoothingLayer());
	this->setMeshSorted(input->isMeshSorted());
	this->setLocalBackTrackingEnabled(input->isLocalBackTrackingEnabled());
	this->setMaxBackTracks(input->getMaxBackTracks());

	this->setUpdateType(input->getUpdateType());

	this->setParallel(input->isParallel());
	this->setMinRefinementPostponed(input->isMinRefinementPostponed());
	this->setUsePartitioningThreshold(input->isUsePartitioningThreshold());
	this->setPartitioningThreshold(input->getPartitioningThreshold());
	this->setLoadBalancerType(input->getLoadBalancer());

	this->setSkippingInnerInDecomposition(input->isSkippingInnerInDecomposition());
	this->setCreateInnerFrontInDecomposition(input->isCreateInnerFrontInDecomposition());

	Data::setTolerance(input->getTolerance());

#if USE_GUI
	//debug
	//Data::setStepByStep(input->isStepByStep());
	Data::setStepByStep(false);
	//endebug
#endif //#if USE_GUI
}

Performer::BoundaryBuilder *PMJMesh::MainDrive::makeBoundaryBuilder() const
{
	return new PMJMesh::BoundaryBuilder(this->comm);
}
#endif //#if (!USE_WORKER_FILE_LOADER)

#if USE_NUMERICAL
MJMesh::HotellingTransformer *PMJMesh::MainDrive::makeHotellingTransformer() const
{
    PMJMesh::HotellingTransformer *hotellingTransformer = new PMJMesh::HotellingTransformer();

    hotellingTransformer->setBoundary(this->boundary);
    hotellingTransformer->setMode(Performer::CovarianceMatrixFinder::Mode::NORMAL);
    hotellingTransformer->setFrontElementCentroids(false);
    hotellingTransformer->setWeighted(false);

    return hotellingTransformer;
}
#endif //#if USE_NUMERICAL

Performer::TreeGenerator *PMJMesh::MainDrive::makeTreeGenerator() const
{
	return (this->classifyTreeOnConstruction) ?
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		new PMJMesh::TreeGenerator(this->dimension, this->boundary, this->treeFactor, static_cast<PMJMesh::Communicator *>(this->comm)) :
#else
		new PMJMesh::TreeGenerator(this->dimension, this->boundary, this->treeFactor) :
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		MJMesh::MainDrive::makeTreeGenerator();
}

MJMesh::MinTreeRefiner *PMJMesh::MainDrive::makeTreeMinRefiner() const
{
#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->classifyTreeOnConstruction)
	{
		//PMJMesh::MinTreeRefiner *refiner = new PMJMesh::MinTreeRefiner(this->dimension, this->tree, this->boundary, this->minRefinementPostponed, static_cast<PMJMesh::Communicator *>(this->comm));
		PMJMesh::MinTreeRefiner *refiner = new PMJMesh::MinTreeRefiner(this->dimension, this->tree, this->boundary, this->minTreeLevel, this->minRefinementPostponed, NULL);

		refiner->setElementsMap(this->elementsMap);

		return refiner;
	}

	return MJMesh::MainDrive::makeTreeMinRefiner();
#else
	return (this->classifyTreeOnConstruction) ?
		//new PMJMesh::MinTreeRefiner(this->dimension, this->tree, this->boundary, this->minRefinementPostponed, static_cast<PMJMesh::Communicator *>(this->comm)) :
		new PMJMesh::MinTreeRefiner(this->dimension, this->tree, this->boundary, this->minTreeLevel, this->minRefinementPostponed, NULL) :
		MJMesh::MainDrive::makeTreeMinRefiner();
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
}

MJMesh::NeighborTreeRefiner *PMJMesh::MainDrive::makeTreeNeighborRefiner() const
{
#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->classifyTreeOnConstruction)
	{
		PMJMesh::NeighborTreeRefiner *refiner = new PMJMesh::NeighborTreeRefiner(this->dimension, this->tree, this->boundary);

		refiner->setElementsMap(this->elementsMap);

		return refiner;
	}

	return MJMesh::MainDrive::makeTreeNeighborRefiner();
#else
	return (this->classifyTreeOnConstruction) ?
		//new PMJMesh::NeighborTreeRefiner(this->dimension, this->tree, this->boundary, this->parallel) :
		new PMJMesh::NeighborTreeRefiner(this->dimension, this->tree, this->boundary/*, false*/) :
		MJMesh::MainDrive::makeTreeNeighborRefiner();
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
}

Parallel::Communicator *PMJMesh::MainDrive::makeCommunicator() const
{
	return new PMJMesh::Communicator();
}

Parallel::LoadBalancer *PMJMesh::MainDrive::makeLoadBalancer() const
{
	Parallel::LoadBalancer *balancer = NULL;

	Parallel::TaskVector tasks;

	switch (this->loadBalancerType)
	{
	case Parallel::WEIGHTED_GREEDY_SERIAL_STATIC:
		balancer = new Parallel::WeightedGreedySerialStaticLoadBalancer(
			static_cast<PMJMesh::Communicator *>(this->comm)->runtimes(), tasks, NULL);
		break;

	case Parallel::GREEDY_SERIAL_STATIC:
		balancer = new Parallel::GreedySerialStaticLoadBalancer(tasks, NULL);
		break;

	case Parallel::SPRING_SERIAL_STATIC:
		balancer =  new Parallel::SpringSerialStaticLoadBalancer(tasks, NULL);
		break;

	case Parallel::DEMAND_PARALLEL_DYNAMIC:
		balancer = new Parallel::DemandParallelDynamicLoadBalancer(tasks, NULL);
		break;

	default:
		balancer = NULL;
		break;
	}

	if (balancer)
	{
		balancer->setComm(this->comm);
	}

	return balancer;
}

TaskManager *PMJMesh::MainDrive::makeTaskManager() const
{
	switch (this->algorithm)
	{
	case GLOBAL_SHIFT:
		return new GlobalTaskManager(this->parallel, this->dimension,
			this->boundary, this->tree, this->percentageNonWorkingTask,
			this->heightDependentInLoad, this->removeLayersInLoad, this->smoothInSlavesEnabled, this->numImproves,
			this->smoothingFactor,
#if (!USE_DELETE_FELS)
			this->unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			this->smoothingLayer,
			this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks,
			this->partitionerType, this->updateType, this->shiftType,
			this->usePartitioningThreshold, this->partitioningThreshold,
			this->boundarySorted, this->usingSearchTrees, this->rangeProportion,
			this->minRefinementLevel, this->skippingInnerInDecomposition,
			this->createInnerFrontInDecomposition, this->loadBalancerType, this->balancer,
			this->comm);

	case GLOBAL_SHIFT_WITH_ADJACENCY:
		return NULL;
		//return new AdjacentGlobalTaskManager();

	case LOCAL_SHIFT:
		return NULL;
		//return new LocalTaskManager();

	case LOCAL_SHIFT_WITH_COVARIANCE:
		return NULL;
		//return new CovarianceLocalTaskManager();

	case A_PRIORI_INTERFACE_MESH:
		return new APrioriTaskManager(this->parallel, this->dimension,
			this->boundary, this->tree, this->percentageNonWorkingTask,
			this->heightDependentInLoad, this->smoothInSlavesEnabled, this->numImproves, this->smoothingFactor,
#if (!USE_DELETE_FELS)
			this->unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			this->smoothingLayer, this->localBackTrackingEnabled,
			this->meshSorted, this->maxBackTracks, this->partitionerType,
			this->updateType,
			this->usePartitioningThreshold, this->partitioningThreshold,
			this->boundarySorted, this->usingSearchTrees, this->minRefinementLevel,
			this->skippingInnerInDecomposition, this->createInnerFrontInDecomposition,
			this->loadBalancerType,
			this->balancer, this->comm);
	}

	return NULL;
}
