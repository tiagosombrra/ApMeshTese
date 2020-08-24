#include "PMJMesh/GlobalTaskManager.h"

#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Performer/RangedIdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTreeCell.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/OctTree.h"
#include "MJMesh/Mesh.h"
#include "Parallel/MPIMessage.h"
#include "Parallel/LoadBalancer.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MinTreeRefiner.h"
#include "PMJMesh/PositionTreeRefiner.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/FrontPartitioner.h"
#include "PMJMesh/BoxTreeShifter.h"
#include "PMJMesh/BSPTreeShifter.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/Communicator.h"
#include "PMJMesh/AngleTreeCellClassifier.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh/PosterioriLoadCalculator.h"
#include "PMJMesh/AverageLoadTreePartitioner.h"
#include "PMJMesh/BoundaryLoadTreePartitioner.h"
#include "PMJMesh/BoundaryNumberTreePartitioner.h"
#include "PMJMesh/BoundaryMinNumberBSPTreePartitioner.h"
#include "PMJMesh/BoundarySmartMinNumberBSPTreePartitioner.h"
#include "PMJMesh/TreeUpdater.h"
#if USE_NEW_FRONT_UPDATER
#include "PMJMesh/ActualFrontUpdater.h"
#include "PMJMesh/MeshUpdater.h"
#include "PMJMesh/PartialMeshUpdater.h"
#include "PMJMesh/EmptyMeshUpdater.h"
#include "PMJMesh/FinalMeshUpdater.h"
#else
#include "PMJMesh/FullFrontUpdater.h"
#include "PMJMesh/PartialFrontUpdater.h"
#include "PMJMesh/EmptyFrontUpdater.h"
#include "PMJMesh/FinalFrontUpdater.h"
#endif //#if USE_NEW_FRONT_UPDATER
#include "PMJMesh/CrossingReplacer.h"
#include "PMJMesh/QuadTree.h"
#include "PMJMesh/OctTree.h"

using namespace PMJMesh;

const Performer::Status PMJMesh::GlobalTaskManager::notInit =
	Performer::Status(GlobalTaskManager::NOT_INIT, "not init");
const Performer::Status PMJMesh::GlobalTaskManager::init =
	Performer::Status(GlobalTaskManager::INIT, "init");
const Performer::Status PMJMesh::GlobalTaskManager::success =
	Performer::Status(GlobalTaskManager::SUCCESS, "success");
const Performer::Status PMJMesh::GlobalTaskManager::error =
	Performer::Status(GlobalTaskManager::ERROR, "error");

/*const Performer::Status PMJMesh::GlobalTaskManager::sharedLoadBalanced =
	Performer::Status(GlobalTaskManager::SHARED_LOAD_BALANCED, "shared - load balanced");
const Performer::Status PMJMesh::GlobalTaskManager::sharedMeshGenerated =
	Performer::Status(GlobalTaskManager::SHARED_MESH_GENERATED, "shared - mesh generated");
const Performer::Status PMJMesh::GlobalTaskManager::sharedFrontUpdated =
	Performer::Status(GlobalTaskManager::SHARED_FRONT_UPDATED, "shared - front updated");
const Performer::Status PMJMesh::GlobalTaskManager::sharedTreeShifted =
	Performer::Status(GlobalTaskManager::SHARED_TREE_SHIFTED, "shared - tree shifted");
const Performer::Status PMJMesh::GlobalTaskManager::sharedFrontDivided =
	Performer::Status(GlobalTaskManager::SHARED_FRONT_DIVIDED, "shared - front divided");
	const Performer::Status PMJMesh::GlobalTaskManager::sharedFinalized =
	Performer::Status(GlobalTaskManager::SHARED_FINALIZED, "shared - finalized");

const Performer::Status PMJMesh::GlobalTaskManager::distributedLoadBalanced =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_LOAD_BALANCED, "distributed - load balanced");
const Performer::Status PMJMesh::GlobalTaskManager::distributedTasksSent =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_TASKS_SENT, "distributed - tasks sent");
const Performer::Status PMJMesh::GlobalTaskManager::distributedTasksReceived =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_TASKS_RECEIVED, "distributed - tasks received");
const Performer::Status PMJMesh::GlobalTaskManager::distributedFrontUpdated =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_FRONT_UPDATED, "distributed - front updated");
const Performer::Status PMJMesh::GlobalTaskManager::distributedTreeShifted =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_TREE_SHIFTED, "distributed - tree shifted");
const Performer::Status PMJMesh::GlobalTaskManager::distributedFrontDivided =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_FRONT_DIVIDED, "distributed - front divided");
const Performer::Status PMJMesh::GlobalTaskManager::distributedFinalized =
	Performer::Status(GlobalTaskManager::DISTRIBUTED_FINALIZED, "distributed - finalized");*/

const Performer::Status PMJMesh::GlobalTaskManager::loadBalanced =
	Performer::Status(GlobalTaskManager::LOAD_BALANCED, "load balanced");
const Performer::Status PMJMesh::GlobalTaskManager::workersMeshGenerated =
	Performer::Status(GlobalTaskManager::WORKERS_MESH_GENERATED, "workers mesh generated");
const Performer::Status PMJMesh::GlobalTaskManager::frontUpdated =
	Performer::Status(GlobalTaskManager::FRONT_UPDATED, "front updated");
const Performer::Status PMJMesh::GlobalTaskManager::treeShifted =
	Performer::Status(GlobalTaskManager::TREE_SHIFTED, "tree shifted");
const Performer::Status PMJMesh::GlobalTaskManager::frontDivided =
	Performer::Status(GlobalTaskManager::FRONT_DIVIDED, "front divided");
	const Performer::Status PMJMesh::GlobalTaskManager::workersFinalized =
	Performer::Status(GlobalTaskManager::WORKERS_FINALIZED, "workers finalized");

const Performer::Status PMJMesh::GlobalTaskManager::parallelMeshGenerated =
	Performer::Status(GlobalTaskManager::PARALLEL_MESH_GENERATED, "parallel mesh generated");
const Performer::Status PMJMesh::GlobalTaskManager::serialMeshGenerated =
	Performer::Status(GlobalTaskManager::SERIAL_MESH_GENERATED, "serial mesh generated");

PMJMesh::GlobalTaskManager::GlobalTaskManager(bool sharedParallel,
	UInt dimension, MJMesh::Boundary *boundary, Data::BoxTree *tree,
	Real percentageNonWorkingTask, bool heightDependentInLoad, UInt removeLayersInLoad, bool smoothInSlavesEnabled,
	UInt numImproves, Real smoothingFactor,
#if (!USE_DELETE_FELS)
	UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
	UInt smoothingLayer,
	bool localBackTrackingEnabled, bool meshSorted, UInt maxBackTracks,
	enum PartitionerType partitionerType, enum MeshUpdateType updateType,
	enum ShiftType shiftType,
	bool usePartitioningThreshold,
	Real partitioningThreshold, bool sorted, bool usingSearchTrees, Real rangeProportion,
	UInt minRefinementLevel, bool skippingInnerInDecomposition, bool createInnerFrontInDecomposition,
	enum Parallel::AvailableLoadBalancers balancerType,
	Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm) :
	PMJMesh::TaskManager(sharedParallel, Parallel::TaskVector(),
		dimension, boundary, tree, loadBalancer, comm)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
	this->partitioningThreshold = partitioningThreshold;
	this->maxCalculatedLoad = 0.0;

	this->balancerType = balancerType;

	this->setPartitionerType(partitionerType);
	this->setUpdateType(updateType);
	this->setShiftType(shiftType);

	this->front = new PMJMesh::Front(this->boundary);
	this->mesh = new MJMesh::Mesh();

	this->shifter = NULL;

	this->considerOnCellsInLoad = false;
	this->considerOnCellsInLoad = true;

#if USE_NEW_FRONT_UPDATER
	this->slaveIdManager = NULL;

	this->meshUpdater = NULL;
#endif //#if USE_NEW_FRONT_UPDATER
	this->frontUpdater = NULL;

	this->partitionTree = NULL;

	this->replacer = NULL;

	this->taskSent = true;
	this->completelyGeneratedSafetyCheck = this->maxCompletelyGeneratedSafetyCheck = 0;

	this->setUsingSearchTrees(usingSearchTrees);
	this->setRangeProportion(rangeProportion);

	this->setPercentageNonWorkingTask(percentageNonWorkingTask);

	this->setRemoveLayersInLoad(removeLayersInLoad);
	this->setHeightDependentInLoad(heightDependentInLoad);

	this->setSmoothInSlavesEnabled(smoothInSlavesEnabled);
	this->setNumImproves(numImproves);
	this->setSmoothingFactor(smoothingFactor);
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
	this->setSmoothingLayer(smoothingLayer);
	this->setLocalBackTrackingEnabled(localBackTrackingEnabled);
	this->setMeshSorted(meshSorted);
	this->setMaxBackTracks(maxBackTracks);
	this->minRefinementLevel = minRefinementLevel;

	this->sorted = sorted;

	this->skippingInnerInDecomposition = skippingInnerInDecomposition;
	this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
	this->createCrackInInnerFront = false;
	this->displaceVerticesInInnerFront = false;
	this->deleteFelsInInnerFront = false;

	this->numLayersMissingInGeneration = 2.0;
	this->useAbsLoadThreshold = false;
	this->useSumLoads = true;
	this->absLoadThreshold = 0.0;

#if USE_DELETE_FELS
    this->deleteExtraFrontElements = true;
    this->deleteFelsInInnerFront = true;
#endif //#if USE_DELETE_FELS

	this->status = GlobalTaskManager::notInit;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterTreeDecomposition = this->timeMeshGeneratorMasterFrontDecomposition =
		this->timeMeshGeneratorWorkerMeshGeneration = this->timeMeshGeneratorWorkerMeshImprovement =
		this->timeMeshGeneratorWorkerTreeClassification =
		this->timeMeshGeneratorWorkerFromMToolsConversion = this->timeMeshGeneratorWorkerToMToolsConversion =
		this->timeMeshGeneratorWorkerOthers = this->timeMeshGeneratorMasterMeshUpdate =
		this->timeMeshGeneratorMasterSendTasks = this->timeMeshGeneratorMasterReceiveTasks =
		this->timeMeshGeneratorMasterTreeShift = this->timeMeshGeneratorMasterFinalization =
		this->timeMeshGeneratorMasterOthers = 0.0;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
}

PMJMesh::GlobalTaskManager::~GlobalTaskManager()
{
	if (this->shifter)
	{
		delete this->shifter;
	}

	if (this->partitionTree)
	{
		delete this->partitionTree;
	}

	if (this->frontUpdater)
	{
		delete this->frontUpdater;
	}

#if USE_NEW_FRONT_UPDATER
	if (this->meshUpdater)
	{
		delete this->meshUpdater;
	}

	if (this->slaveIdManager)
	{
		delete this->slaveIdManager;
	}
#endif //#if USE_NEW_FRONT_UPDATER

	if (this->replacer)
	{
		delete this->replacer;

		this->replacer = NULL;
	}
}

void PMJMesh::GlobalTaskManager::setPartitionerType(enum PartitionerType partitionerType)
{
	this->partitionerType = partitionerType;
}

void PMJMesh::GlobalTaskManager::setUpdateType(enum MeshUpdateType updateType)
{
	this->updateType = updateType;
}

void PMJMesh::GlobalTaskManager::setShiftType(enum ShiftType shiftType)
{
	this->shiftType = shiftType;
}

void PMJMesh::GlobalTaskManager::setFront(PMJMesh::Front *front)
{
	this->front = front;
}

PMJMesh::Front *PMJMesh::GlobalTaskManager::getFront() const
{
	return this->front;
}

void PMJMesh::GlobalTaskManager::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

MJMesh::Mesh *PMJMesh::GlobalTaskManager::getMesh() const
{
	return this->mesh;
}

#if USE_NEW_FRONT_UPDATER
const Data::Mesh *PMJMesh::GlobalTaskManager::getUnsmoothedMesh() const
#else
Data::Mesh *PMJMesh::GlobalTaskManager::getUnsmoothedMesh() const
#endif //#if USE_NEW_FRONT_UPDATER
{
	//debug
	//return NULL;
	//endebug
#if USE_NEW_FRONT_UPDATER
	return ((this->updateType != PMJMesh::FULL) && (this->meshUpdater)) ? static_cast<PartialMeshUpdater *>(this->meshUpdater)->getInnerMesh() : NULL;
#else
	return (this->updateType != PMJMesh::FULL) ? static_cast<PartialFrontUpdater *>(this->frontUpdater)->getInnerMesh() : NULL;
#endif //#if USE_NEW_FRONT_UPDATER
}

void PMJMesh::GlobalTaskManager::setPercentageNonWorkingTask(Real percentageNonWorkingTask)
{
	this->percentageNonWorkingTask = percentageNonWorkingTask;
}

void PMJMesh::GlobalTaskManager::setUsingSearchTrees(bool usingSearchTrees)
{
	this->usingSearchTrees = usingSearchTrees;
}

void PMJMesh::GlobalTaskManager::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

void PMJMesh::GlobalTaskManager::setRemoveLayersInLoad(UInt removeLayersInLoad)
{
	this->removeLayersInLoad = removeLayersInLoad;
}

void PMJMesh::GlobalTaskManager::setHeightDependentInLoad(bool heightDependentInLoad)
{
	this->heightDependentInLoad = heightDependentInLoad;
}

#if (!USE_DELETE_FELS)
void PMJMesh::GlobalTaskManager::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::GlobalTaskManager::setSmoothingLayer(UInt smoothingLayer)
{
	this->smoothingLayer = smoothingLayer;
}

void PMJMesh::GlobalTaskManager::setSmoothInSlavesEnabled(bool smoothInSlavesEnabled)
{
	this->smoothInSlavesEnabled = smoothInSlavesEnabled;
}

void PMJMesh::GlobalTaskManager::setNumImproves(UInt numImproves)
{
	this->numImproves = numImproves;
}

void PMJMesh::GlobalTaskManager::setSmoothingFactor(Real smoothingFactor)
{
	this->smoothingFactor = smoothingFactor;
}

void PMJMesh::GlobalTaskManager::setLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;
}

void PMJMesh::GlobalTaskManager::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

void PMJMesh::GlobalTaskManager::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

Parallel::Task *PMJMesh::GlobalTaskManager::makeTask() const
{
	return new PMJMesh::MJMeshTask();
}

void PMJMesh::GlobalTaskManager::execute()
{
	///DONE
	/// apply cell classification when building the box tree, to remove the
	///  cell classification step and tho diminish the number of cells in the
	///  whole tree. DONE
	/// on the two tree refinements, refine only cells that are on/inside the
	///  region, not the outside cells. DONE
	/// use a heap in front advancing methods and in the Front class. DONE
	///  (but this is not very good. a multiset was also implemented, but the
	///  original list showed to be the best option)
	/// crate a class TreeShifter for shifting the tree. DONE
	/// finalize sooner when distributed version is not required. DONE
	/// ajeitar o balanceamento, nos casos de greedy e spring, pq as particoes
	///  de um mesmo processo com varias threads estao seguidas, seria melhor
	///  se alternasse com os outros processos. DONE
	/// do not send partitions that will not create mesh. DONE
	/// apagar a arvore qd nao precisa mais dela, depois da geracao por
	///  geometria. DONE
	/// separate class Shape in class (interface) DrawableObject with drawing
	///  information. DONE
	/// fazer o LoadBalancer tratar somente tarefas, e nao mais particoes. DONE
	/// try to shift to other directions even when the front is not updated. DONE
	/// ajeitar a atualizacao da classificacao da arvore. DONE
	/// apply mesh improvement in slaves, except where it cannot be applied. DONE
	/// enviar as tarefas por comunicacao coletiva. DONE
	/// tirar a heranca do box tree cell com a box, para diminuir a qtd de memoria.
	///  isso implica que os pontos maximo e minimo deve ser achados em mais
	///  tempo, pois devem olhar os do pai e calcular o seu. alem disso, a
	///  particao nao tem mais uma box, entao o shift nao vai ser mais tao
	///  trivial, pois a box vai estar na particao e a celula nao tem referencia
	///  a ela (aos pontos). este item se traduz nas seguintes modificacoes:
	///  1 criar uma classe que herda de Tree para representar uma decomposição
	///    espacial recursiva, que tem um ponteiro para uma GeometricShape e
	///    de DrawableObject.
	///  2 fazer uma classe para representar uma celula da arvore descrita
	///    no item 1, herdando somente de TreeNode. Esta classe deve saber
	///    encontrar o GeometricShape a partir da celula pai, mas nao guardar
	///    um objeto do tipo GeometricShape.
	///  3 fazer a boxtree herdar dessa classe descrita no item 1, somente.
	///  4 fazer a boxtreecell herdar somente da classe descrita no item 2.
	///  5 ajeitar o shift. do jeito que está, uma celula da arvore sofre o
	///    shift e, automaticamente, o retangulo da particao sofre o shift. isso
	///    acontece por que a celula herda de uma Box, e a particao tem um
	///    ponteiro para essa box. como a celula nao vai herdar mais da box,
	///    a classe TreeShifter tem que ter um ponteiro para a Box
	///    correspondente a uma BoxTreeCell, e esta Box deve sofrer o shift. DONE
	/// nao fazer o segundo refinamento, pq tah comendo muita memoria. isso
	///  implica que os processos devem calcular o tamanho maximo, mas nao
	///  refinar, que implica que o mestre deve usar esse tamanho para calcular
	///  as cargas, e os escravos devem refinar a arvore qd forem fazer a
	///  geracao e depois destruir as celulas geradas. DONE
	/// add 'using' when only some virtual overloaded methods are implemented
	///  in a derived class, and other methods are implemented in a base class. DONE
	/// use the MJMesh::MainDrive instead of PMJMesh::GeometryFrontAdvancer,
	///  (P)MJMesh::TopologyFrontAdvancer and (P)MJMesh::Improver. DONE
	/// fazer a Mesh cuidar dos ids dos vertices/elementos/elementos de
	///  fronteira, removendo assim essa resposabilidade dos Builderes. DONE
	///   Pensando bem, eh melhor deixar mesmo nos builderes, ainda mais por
	///   conta do paralelismos. DONE
	///   Pensando bem, melhor criar uma classe soh para cuidar dos ids dos
	///   vertices/elementos/elementos de fronteira. DONE
	/// olhar o exemplo do teste_quadrado.new.2djmsh, que tah demorando muito
	///  na melhoria. DONE
	/// incorporate the serial mesh generation, when the user wants it. for now,
	///  this algorithm only executes the parallel version, if the user wants the
	///  serial version, he has to execute the MJMesh program. DONE
	/// create a superclass for Boundary, Front and Mesh. DONE
	/// add virtual inheritance where needed. DONE
	/// consider empty partitions when balancing the load (or not?). DONE (not)
	/// change sort methods to use std::sort algorithms, where needed. DONE
	/// criar as classes PMJMesh::QuadTreeCell e PMJMesh::OctTreeCell e adicionar
	///  a posicao (IN, ON ou OUT) como atributo. DONE
	/// ajeitar a atualizacao da fronteira. tah funcionando, mas talvez pudesse
	///  ser feito de um jeito melhor. DONE


	///TODO
	/// em EmptyFrontUpdater e PartialFrontUpdater, verificar se os atributos
	///  parallel e shared sao redundantes (to com pressa para defender)
	/// use specific errors for TaskManager (do not use MJMesh::MainDrive errors).
	/// do not advance other partitions if an error in geometry advancer
	///  occurred. (DONE in shared)
	/// perform tests to check if the absence of the ON cells significantly
	///  change the load balancing.
	/// substituir, em MJMesh::BackTrackingFrontAdvancer.cpp, os std::find por
	///  um std::set<> e um std::set<>::find()

	if ((this->status.getType() == GlobalTaskManager::success.getType()) ||
		(this->status.getType() >= GlobalTaskManager::error.getType()))
	{
		return;
	}

	//std::cout << "debug p " << this->comm->rank() << " executing" << std::endl;

	//std::cout << "debug p " << this->comm->rank() << " comm->numworkers = " << this->comm->numWorkers() << std::endl;
	//std::cout << "debug p " << this->comm->rank() << " comm->numprocesses = " << this->comm->numProcesses() << std::endl;
	//std::cout << "debug p " << this->comm->rank() << " this->numprocesses = " << this->numProcesses << std::endl;

	//debug
	//Real debugstart = Data::time();
	//endebug
#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	Real debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	switch (this->status.getType())
	{
	case GlobalTaskManager::NOT_INIT:

		if (this->comm->isMaster())
		{
			//std::cout << "debug master or single will initialize" << std::endl;

			this->initializeMaster();

			//std::cout << "Master time for initializing = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
		}
		else
		{
			//std::cout << "debug slave will initialize" << std::endl;

			this->initializeSlave();
		}

		this->status = GlobalTaskManager::init;

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case GlobalTaskManager::INIT:
	/*case GlobalTaskManager::SHARED_LOAD_BALANCED:
	case GlobalTaskManager::SHARED_MESH_GENERATED:
	case GlobalTaskManager::SHARED_FRONT_UPDATED:
	case GlobalTaskManager::SHARED_TREE_SHIFTED:
	case GlobalTaskManager::SHARED_FRONT_DIVIDED:
	case GlobalTaskManager::DISTRIBUTED_LOAD_BALANCED:
	case GlobalTaskManager::DISTRIBUTED_TASKS_SENT:
	case GlobalTaskManager::DISTRIBUTED_TASKS_RECEIVED:
	case GlobalTaskManager::DISTRIBUTED_FRONT_UPDATED:
	case GlobalTaskManager::DISTRIBUTED_TREE_SHIFTED:
	case GlobalTaskManager::DISTRIBUTED_FRONT_DIVIDED:*/
	case GlobalTaskManager::LOAD_BALANCED:
	case GlobalTaskManager::WORKERS_MESH_GENERATED:
	case GlobalTaskManager::FRONT_UPDATED:
	case GlobalTaskManager::TREE_SHIFTED:
	case GlobalTaskManager::FRONT_DIVIDED:

		if ((this->partitions.size() > 1) ||
			(this->comm->isSlave()))
		{
			/*if (!this->comm->isParallel())
			{
				this->executeShared();
			}
			else
			{
				this->executeDistributed();
			}*/

			this->executeSharedOrDistributed();

			//if (this->comm->isMaster()) std::cout << "Master time for executing = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();

			if (this->status.getType() >= this->error.getType())
			{
				this->comm->finalize();

				break;
			}

#if USE_GUI
			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}
		else
		{
			this->done.swap(this->tasks);
		}

	/*case GlobalTaskManager::SHARED_FINALIZED:
	case GlobalTaskManager::DISTRIBUTED_FINALIZED:*/
	case GlobalTaskManager::WORKERS_FINALIZED:

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
		debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

		if (this->updateType == PMJMesh::FULL)
		{
			this->comm->finalize();
		}

		//if (this->comm->isMaster()) std::cout << "Master time for finalizing communicator = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		this->clearDone();

		//if (this->comm->isMaster()) std::cout << "Master time for clearing done tasks = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		this->status = GlobalTaskManager::parallelMeshGenerated;

//#if USE_GUI
		//if (Data::isStepByStep()) break;
//#endif //#if USE_GUI

	case GlobalTaskManager::PARALLEL_MESH_GENERATED:

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
		this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

		if (this->comm->isMaster())
		{
			//std::cout << "debug master or single will generate final mesh" << std::endl;

			this->executeGeometryGeneration();

			//std::cout << "Master time for executing geometry generation = " << Data::time() - debugstart << std::endl;
			//debugstart = Data::time();
		}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
		this->timeMeshGeneratorMasterFinalization += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

		if (this->status.getType() >= this->error.getType()) break;

		this->status = serialMeshGenerated;
	}

	if (this->status.getType() >= this->error.getType()) return;

	if (this->status.getType() == this->serialMeshGenerated.getType())
	{
		this->status = GlobalTaskManager::success;
	}

	//if (this->comm->isMaster()) std::cout << "Master time for finalizing = " << Data::time() - debugstart << std::endl;
}

void PMJMesh::GlobalTaskManager::executeSharedOrDistributed()
{
	bool distributed = this->comm->isParallel();

#if USE_GUI
	//bool updated = false;
#endif //#if USE_GUI

	while (true)
	{
		//std::cout << this->tree->text() << std::endl;

		if ((!distributed) || (this->comm->isMaster()))
		{
			//debug
			//Real debugstart = Data::time();
			//endebug
#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
			Real debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

			switch (this->status.getType())
			{
			case GlobalTaskManager::INIT:
			case GlobalTaskManager::FRONT_DIVIDED:

				//std::cout << "debug balancing load" << std::endl;

				this->balanceLoad(!distributed);

				//std::cout << "Master, executing, time for balancing load = " << Data::time() - debugstart << std::endl;
				//debugstart = Data::time();

				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::loadBalanced;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

//#if USE_GUI
				//if (Data::isStepByStep()) break;
//#endif //#if USE_GUI

			case GlobalTaskManager::LOAD_BALANCED:

				//std::cout << "debug generating mesh" << std::endl;

				if (distributed)
				{
#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					this->sendTasksForSlaves();

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterSendTasks += Data::time() - debugstart;

					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					//std::cout << "Master, executing, time for sending tasks to slaves = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();

					this->receiveTasksFromSlaves();

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterReceiveTasks += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					//std::cout << "Master, executing, time for receiving tasks from slaves = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();
				}
				else
				{
					this->generateMeshInCells();

					//std::cout << "Master, executing, time for generating mesh in cells = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();
				}

				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::workersMeshGenerated;

//#if USE_GUI
				//if (Data::isStepByStep()) break;
//#endif //#if USE_GUI

			case GlobalTaskManager::WORKERS_MESH_GENERATED:

				//std::cout << "debug updating tasks" << std::endl;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

#if USE_GUI
				//updated =
#endif //#if USE_GUI
					this->updateFront(!distributed);

					//std::cout << "Master, executing, time for updating front = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();

				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::frontUpdated;

#if USE_GUI
				if (/*(updated) && */(Data::isStepByStep())) break;
#endif //#if USE_GUI

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				this->timeMeshGeneratorMasterMeshUpdate += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

			case GlobalTaskManager::FRONT_UPDATED:

				if (this->completelyGenerated())
				{
					//std::cout << "Master, executing, time for testing if generated = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();

					//std::cout << "debug finalizing parallel procedure" << std::endl;

					if (distributed)
					{
						this->sendTerminationMessage();

						//std::cout << "Master, executing, time for sending termination message = " << Data::time() - debugstart << std::endl;
						//debugstart = Data::time();
					}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					///TODO acho que nao precisa desse this->shiftTree(true), mas em todo caso...
					this->shiftTree(true);

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterTreeShift += Data::time() - debugstart;
					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					//std::cout << "Master, executing, time for shifting tree = " << Data::time() - debugstart << std::endl;
					//debugstart = Data::time();

					if (!distributed)
					{
						this->clearBoundaries();

						//std::cout << "Master, executing, time for clearing boundaries = " << Data::time() - debugstart << std::endl;
						//debugstart = Data::time();
					}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

#if USE_NEW_FRONT_UPDATER
					if (this->frontUpdater)
					{
						this->frontUpdater->finalizeFront();

						//std::cout << "Master, executing, time for finalizing front = " << Data::time() - debugstart << std::endl;
						//debugstart = Data::time();
					}
#endif //#if USE_NEW_FRONT_UPDATER

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterMeshUpdate += Data::time() - debugstart;
					debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					if (this->status.getType() >= this->error.getType()) break;

					//this->debugHighlightInterTasks();

					this->status = GlobalTaskManager::workersFinalized;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
					this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

					break;
				}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

				//std::cout << "Master, executing, time for testing if generated = " << Data::time() - debugstart << std::endl;
				//debugstart = Data::time();

				//std::cout << "debug shifting tree" << std::endl;

				this->shiftTree(false);

				//std::cout << "Master, executing, time for shifting tree = " << Data::time() - debugstart << std::endl;
				//debugstart = Data::time();

				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::treeShifted;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				this->timeMeshGeneratorMasterTreeShift += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

#if USE_GUI
				//if (Data::isStepByStep()) break;
#endif //#if USE_GUI

			case GlobalTaskManager::TREE_SHIFTED:

				//std::cout << "debug partitioning front" << std::endl;

				this->divideFront();

				//this->debugHighlightInterTasks();

				//std::cout << "Master, executing, time for dividing front = " << Data::time() - debugstart << std::endl;
				//debugstart = Data::time();

				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::frontDivided;

#if USE_GUI
				if (Data::isStepByStep()) break;
#endif //#if USE_GUI

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
				this->timeMeshGeneratorMasterFrontDecomposition += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
			}

			//this->mesh->test(this->boundary, this->front);

			if (this->status.getType() >= this->error.getType()) break;

			if (this->status.getType() == GlobalTaskManager::workersFinalized.getType())
			{
				break;
			}

#if USE_GUI
			if ((Data::isStepByStep()) &&
				(this->status.getType() == GlobalTaskManager::frontUpdated.getType() ||
				 this->status.getType() == GlobalTaskManager::treeShifted.getType()  ||
				 this->status.getType() == GlobalTaskManager::frontDivided.getType()))
			{
				break;
			}
#endif //#if USE_GUI
		}
		else
		{
			if (!this->receiveTasksFromMaster())
			{
				if (this->status.getType() >= this->error.getType()) break;

				this->status = GlobalTaskManager::workersFinalized;

				break;
			}

			this->generateMeshInCells();

			this->sendTasksForMaster();
		}
	}
}

void PMJMesh::GlobalTaskManager::initializeMaster()
{
	//Real debugstart = Data::time();
#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	Real debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	this->taskSent = true;

	//Data::VertexVector vertices = this->boundary->getVertices();
	//Data::VertexList vertices = this->boundary->getVertices();

	//while (!vertices.empty())
	for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
		 iter != this->boundary->vEnd(); iter++)
	{
		//this->mesh->add(vertices.back(), false);
		this->mesh->add((*iter), false);

		//vertices.pop_back();
	}

	//Data::FrontElementVector frontElements = this->boundary->getElements();
	//Data::FrontElementList frontElements = this->boundary->getElements();

	//while (!frontElements.empty())
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		//this->mesh->add(frontElements.back());
		this->mesh->add((*iter));

		//frontElements.pop_back();
	}

	//std::cout << "Master, initializing, time for adding boundary to mesh = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())) == Data::UNKNOWN)
	{
		PMJMesh::AngleTreeCellClassifier *atcc = new PMJMesh::AngleTreeCellClassifier(
			//this->dimension, static_cast<PMJMesh::Communicator *>(this->comm), this->front, this->tree);
			this->dimension, static_cast<PMJMesh::Communicator *>(this->comm), this->boundary, this->tree);

		atcc->execute();

		delete atcc;

		//std::cout << "Master, initializing, time for classifying cells = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();
	}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	LoadCalculator *lc = this->makeLoadCalculator(true);

	lc->execute();

#if USE_C__11
	Data::RealHashMap load = lc->getTreeLoad();
#else
	Data::RealMap load = lc->getTreeLoad();
#endif //#if USE_C__11

	//std::cout << "debug totalload = " << load[(Data::BoxTreeCell *)this->tree->getRoot()] << std::endl;

	//delete lc;

	//std::cout << "Master, initializing, time for calculating load = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	TreePartitioner *tc = this->makeTreePartitioner(&load, lc);

	tc->execute();

	delete lc;

	this->maxCalculatedLoad = tc->getMaxCalculatedLoad();

	this->partitionTree = tc->getPartitionTree();

	this->partitions = tc->getPartitions();

	Data::ULIntVector treeNewCells = tc->getTreeNewCells();

	delete tc;

	//std::cout << "Master, initializing, time for creating partition tree = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	while (!treeNewCells.empty())
	{
		ULInt id = treeNewCells.back();

		treeNewCells.pop_back();

		PMJMesh::setPosition(this->dimension, this->tree, id, Data::IN);
	}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterTreeDecomposition += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	//std::cout << "Master, initializing, time for classifying new cells = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (!this->shifter)
	{
		this->shifter = this->makePartitionTreeShifter();

		//this->shifter->setLateral(true);
		//this->shifter->setDiagonal(true);
		//this->shifter->setDiagonalVertex(true);
	}

	//std::cout << "Master, initializing, time for creating box shifter = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	this->maxCompletelyGeneratedSafetyCheck = this->shifter->maxStates();

	if (!this->shifter->isLateral())
	{
		this->maxCompletelyGeneratedSafetyCheck -= this->shifter->maxState(0) - this->shifter->minState(0);
	}

	if (!this->shifter->isDiagonal())
	{
		this->maxCompletelyGeneratedSafetyCheck -= this->shifter->maxState(1) - this->shifter->minState(1);
	}

	if (!this->shifter->isDiagonalVertex())
	{
		this->maxCompletelyGeneratedSafetyCheck -= this->shifter->maxState(2) - this->shifter->minState(2);
	}

	for (UInt i = 0; i < this->partitions.size(); i++)
	{
		Parallel::Partition *partition = this->partitions[i];

		//PMJMesh::MJMeshTask *t = new PMJMesh::MJMeshTask();
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->makeTask());
		t->setComm(this->comm);

		t->setPartition(partition);

		//std::cout << "making task " << t->getId() << std::endl;

		this->tasks.push_back(t);

		//t->resizeGenerated(2*this->dimension + 1);
		//t->resizeGenerated(this->shifter->numStates());
		t->resizeGenerated(this->shifter->maxStates());

		if (!this->shifter->isLateral())
		{
			for (UInt j = this->shifter->minState(0); j < this->shifter->maxState(0); j++)
			{
				t->setGenerated(j, true);
			}
		}

		if (!this->shifter->isDiagonal())
		{
			for (UInt j = this->shifter->minState(1); j < this->shifter->maxState(1); j++)
			{
				t->setGenerated(j, true);
			}
		}

		if (!this->shifter->isDiagonalVertex())
		{
			for (UInt j = this->shifter->minState(2); j < this->shifter->maxState(2); j++)
			{
				t->setGenerated(j, true);
			}
		}
	}

	//std::cout << "Master, initializing, time for setting generated in tasks = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (this->partitions.size() == 1)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]);

		//for (UInt j = 0; j < this->shifter->numStates(); j++)
		for (UInt j = 0; j < this->shifter->maxStates(); j++)
		{
			t->setGenerated(j, true);
		}

		if (this->comm->isParallel())
		{
			this->taskSent = false;

			this->sendTerminationMessage();
		}

		this->comm->finalize();

		//std::cout << "Master, initializing, time for finalizing the unique task = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		return;
	}

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	FrontPartitioner *fc = this->makeFrontPartitioner();

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	fc->execute();

	this->replacer = fc->getReplacer();

	fc->setReplacer(NULL);

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	if ((!this->skippingInnerInDecomposition) && (this->createInnerFrontInDecomposition))
	{
		this->createInnerFront(fc);
	}

	delete fc;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterFrontDecomposition += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	//std::cout << "Master, initializing, time for partitioning the front = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	/*for (UInt i = 0; i < this->tasks.size(); i++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

		for (UInt j = 0; j < this->shifter->numStates(); j++)
		{
			t->setGenerated(j, false);
		}
	}*/

	this->initializeGeneratedTasks();

	//std::cout << "Master, initializing, time for initializing generated tasks = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	if (!this->comm->isParallel())
	{
		this->comm->finalize();
	}

	//std::cout << "Master, initializing, time for creating master cell classifier = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if USE_NEW_FRONT_UPDATER
	this->frontUpdater = this->makeFrontUpdater();

	this->meshUpdater = this->makeMeshUpdater();
#else
	switch (this->updateType)
	{
	case PMJMesh::FULL:
		this->frontUpdater = new FullFrontUpdater(this->dimension,
			!this->comm->isParallel(), this->boundary, this->front, this->mesh);
		break;

	case PMJMesh::PARTIAL:
		this->frontUpdater = new PartialFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, !this->comm->isParallel(),
			this->boundary, this->front, this->mesh, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::PARTIAL_NO_BOUNDARY:
		this->frontUpdater = new PartialFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, !this->comm->isParallel(),
			this->boundary, this->front, this->mesh, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::EMPTY:
		this->frontUpdater = new EmptyFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, !this->comm->isParallel(),
			this->boundary, this->front, this->mesh, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::EMPTY_NO_BOUNDARY:
		this->frontUpdater = new EmptyFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, !this->comm->isParallel(),
			this->boundary, this->front, this->mesh, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::FINAL:
		this->frontUpdater = new FinalFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, !this->comm->isParallel(),
			this->boundary, this->front, this->mesh, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;
	}
#endif //#if USE_NEW_FRONT_UPDATER

	//this->debugHighlightInterTasks();

	if (this->useAbsLoadThreshold)
	{
		this->absLoadThreshold = this->calculateAbsLoadThreshold();
	}

	//std::cout << "Master, initializing, time for creating front and mesh updaters = " << Data::time() - debugstart << std::endl;

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
}

void PMJMesh::GlobalTaskManager::initializeSlave()
{
	this->taskSent = false;

#if USE_NEW_FRONT_UPDATER
	this->meshUpdater = this->makeMeshUpdater();
#else
	switch (this->updateType)
	{
	case PMJMesh::FULL:
		break;

	case PMJMesh::PARTIAL:
		this->frontUpdater = new PartialFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, false,
			this->boundary, this->front, NULL, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::PARTIAL_NO_BOUNDARY:
		this->frontUpdater = new PartialFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, false,
			this->boundary, this->front, NULL, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::EMPTY:
		this->frontUpdater = new EmptyFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, false,
			this->boundary, this->front, NULL, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::EMPTY_NO_BOUNDARY:
		this->frontUpdater = new EmptyFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, false,
			this->boundary, this->front, NULL, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;

	case PMJMesh::FINAL:
		this->frontUpdater = new FinalFrontUpdater(this->dimension,
			this->smoothingLayer, this->parallel, false,
			this->boundary, this->front, NULL, this, this->comm);

		static_cast<PartialFrontUpdater *>(this->frontUpdater)->setIdManager(this->idManager);
		break;
	}
#endif //#if USE_NEW_FRONT_UPDATER
}

Real PMJMesh::GlobalTaskManager::calculateAbsLoadThreshold() const
{
	Parallel::Task *heaviest = NULL;

	for (Parallel::TaskVector::const_iterator iter = this->tasks.begin();
		 iter != this->tasks.end(); iter++)
	{
		Parallel::Task *t = (*iter);

		if ((!heaviest) ||
			(t->getPartition()->getLoad() > heaviest->getPartition()->getLoad()))
		{
			heaviest = t;
		}
	}

	if (!heaviest)
	{
		return 0.0;
	}

	/*Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(heaviest->getId()));

	if (!cell)
	{
		return 0.0;
	}

	UInt level = cell->level();

	UInt maxLevel = this->tree->depth();

	Real diff = static_cast<Real>(maxLevel - level);

	Real load = std::pow(2.0, diff);

	load = std::pow(load, this->dimension - i);*/

	Real load = (this->dimension == 2) ? std::sqrt(heaviest->load()) :
		std::pow(heaviest->load(), 2.0/3.0);

	Real threshold = 2.0*this->numLayersMissingInGeneration*load;

	return threshold;
}

bool PMJMesh::GlobalTaskManager::underAbsThreshold(Real load) const
{
	return load < this->absLoadThreshold;
}

bool PMJMesh::GlobalTaskManager::underThreshold(Parallel::Task *t) const
{
	Real load = t->load();

	return this->useAbsLoadThreshold ? this->underAbsThreshold(load) :
		(load < this->maxCalculatedLoad*this->percentageNonWorkingTask);
}

void PMJMesh::GlobalTaskManager::splitTasks(Parallel::TaskVector &working, Parallel::TaskVector &nonWorking) const
{
	UInt shift = this->shiftState();

	Parallel::TaskVector underThreshold;

	Real sumUnderThrsLoads = 0.0;

	//std::cout << "abs load threshold = " << this->absLoadThreshold << std::endl;

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		Parallel::Task *t = this->tasks[i];

		/*//debug
		if (static_cast<PMJMesh::MJMeshTask *>(t)->isGenerated(shift))
		{
			std::cout << "task " << t->getId() << " has generated in shift " << shift << std::endl;
		}
		else if (t->load() < Data::getTolerance())
		{
			std::cout << "task " << t->getId() << " has load to small: " << t->load() << std::endl;
		}
		else
		{
			std::cout << "task " << t->getId() << " is going to generate mesh" << std::endl;
		}
		//endebug*/

		Real load = t->load();

		//std::cout << "load for task " << t->getId() << " = " << load << std::endl;

		if ((static_cast<PMJMesh::MJMeshTask *>(t)->isGenerated(shift)) ||
			(load < Data::getTolerance()))
		{
			nonWorking.push_back(t);
		}
		else if (this->underThreshold(t))
		{
			underThreshold.push_back(t);

			sumUnderThrsLoads += load;
		}
		else
		{
			working.push_back(t);

			if (this->comm->isParallel())
			{
				if (!t->getBuilder())
				{
					t->makeBuilder();

					//PMJMesh::WorkerMainDrive *drive = new PMJMesh::WorkerMainDrive();
					PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

					drive->setDimension(this->dimension);

					//t->setBuilder(drive);
				}
			}
		}
	}

	if ((this->useAbsLoadThreshold) && (this->useSumLoads))
	{
		if ((working.empty()) && (underThreshold.size() > 1))
		{
			if (!this->underAbsThreshold(sumUnderThrsLoads))
			{
				working.swap(underThreshold);
			}
		}
		else if ((working.size() == 1) && (underThreshold.empty()))
		{
			nonWorking.push_back(working.front());

			working.clear();
		}
		else if ((working.size() == 1) && (underThreshold.size() == 1))
		{
			working.push_back(underThreshold.front());

			underThreshold.clear();
		}
		else if (working.size() == 1)
		{
			if (this->underAbsThreshold(sumUnderThrsLoads))
			{
				nonWorking.push_back(working.front());

				working.clear();
			}
			else
			{
				for (UInt i = 0; i < underThreshold.size(); i++)
				{
					working.push_back(underThreshold[i]);
				}

				underThreshold.clear();
			}
		}
		else if (underThreshold.size() > 1)
		{
			if (!this->underAbsThreshold(sumUnderThrsLoads))
			{
				for (UInt i = 0; i < underThreshold.size(); i++)
				{
					working.push_back(underThreshold[i]);
				}

				underThreshold.clear();
			}
		}
	}
	else
	{
		if (working.size() == 1)
		{
			nonWorking.push_back(working.front());

			working.clear();
		}
	}

	for (Parallel::TaskVector::iterator iter = underThreshold.begin();
		 iter != underThreshold.end(); iter++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

		t->setGenerated(shift, true);

		nonWorking.push_back(t);
	}
}

void PMJMesh::GlobalTaskManager::balanceLoad(bool shared)
{
	Parallel::TaskVector tasks;

	//debug
	//std::cout << "number of tasks = " << this->tasks.size() << std::endl;
	//endebug

	this->splitTasks(tasks, this->nonWorkingTasks);

	if (tasks.empty())
	{
		this->completelyGeneratedSafetyCheck++;
	}
	else
	{
		this->completelyGeneratedSafetyCheck = 0;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	std::cout << "number of working tasks = " << tasks.size() << std::endl;
	std::cout << "number of working procs = " << this->comm->numWorkers() << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->loadBalancer->setTasks(tasks);

	if (shared)
	{
		this->loadBalancer->sort(false);

		this->tasks.clear();
	}
	else
	{
		this->loadBalancer->execute();
	}
}

void PMJMesh::GlobalTaskManager::sendTasksForSlaves()
{
	if (!this->loadBalancer->getTasks().empty())
	{
		ULInt buf[5] = {1, 0, this->idManager->getId(0), this->idManager->getId(1), 0};

		/*std::stringstream str;
		str << "broadcasting ids vertex: " << buf[2] << ", fel: " << buf[3] << ", el: " << buf[4] << std::endl;
		std::cout << str.str();*/

		this->comm->broadcast(buf, 5, -1);

		this->loadBalancer->sendToSlaves();
	}
}

void PMJMesh::GlobalTaskManager::receiveTasksFromSlaves()
{
	this->done.swap(this->tasks);

	if (!this->loadBalancer->getTasks().empty())
	{
		ULInt data[4] = {0, 0, 0, 0};

		ULInt *buffer = this->comm->reduce(data, 4, Communicator::MAXIMUM, this->comm->root());

		if (data[3] >= this->error.getType())
		{
			this->status = this->error;

			this->sendTerminationMessage();

#if (!USE_GUI)
			this->comm->finalize();

			exit(EXIT_FAILURE);
#endif //#if (!USE_GUI)
		}

		/*std::stringstream str;
		str << "reduced ids vertex: " << buffer[0] << ", fel: " << buffer[1] << " and el: " << buffer[2] << std::endl;
		std::cout << str.str();*/

		this->idManager->setId(0, buffer[0]);
		this->idManager->setId(1, buffer[1]);
		this->idManager->setId(2, buffer[2]);

		delete [] buffer;

		this->loadBalancer->receiveFromSlaves();

#if USE_THREAD_COLOR
		Parallel::TaskVector tasks = this->loadBalancer->getTasks();

		for (Parallel::TaskVector::iterator iter = tasks.begin();
			 iter != tasks.end(); iter++)
		{
			dynamic_cast<MJMeshTask *>((*iter))->colorizeMesh(this->loadBalancer->getProcess((*iter)));
		}
#endif //#if USE_THREAD_COLOR
	}

	this->tasks = this->loadBalancer->getTasks();
}

bool PMJMesh::GlobalTaskManager::updateFront(bool shared, bool updateTree)
{
	//std::cout << "debug updating front" << std::endl;

	//debug
	//Real debugstart = Data::time();
	//endebug

	if (this->replacer)
	{
		this->replacer->execute();

		delete this->replacer;

		this->replacer = NULL;
	}

#if USE_NEW_FRONT_UPDATER
	this->meshUpdater->initialize();
#endif //#if USE_NEW_FRONT_UPDATER
	this->frontUpdater->initialize();

	//std::cout << "Master, updating front, time for initializing front updater = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	UInt shift = this->shiftState();

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	//std::cout << "Master, updating front, time for determining shift state = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	std::sort(this->tasks.begin(), this->tasks.end(), Data::Identifiable::greater);

	for (Parallel::TaskVector::iterator iter = this->tasks.begin();
		 iter != this->tasks.end(); iter++)
	{
		this->task = (*iter);

		if (shared)
		{
			static_cast<PMJMesh::MJMeshTask *>(this->task)->setGenerated(shift, true);

#if USE_NEW_FRONT_UPDATER
			this->meshUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
			this->frontUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
#endif //#if USE_NEW_FRONT_UPDATER
		}
		else
		{
			ULInt id = this->task->getId();

			//std::cout << "debug id of partition = " << id << std::endl;

			Parallel::Task *originalTask = NULL;

			for (UInt i = 0; i < this->done.size(); i++)
			{
				if (this->done[i]->getId() == id)
				{
					originalTask = this->done[i];

					break;
				}
			}

			static_cast<PMJMesh::MJMeshTask *>(originalTask)->setGenerated(shift, true);

#if USE_NEW_FRONT_UPDATER
			this->meshUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(originalTask));
			this->frontUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(originalTask));

			this->meshUpdater->setWorkerTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
			this->frontUpdater->setWorkerTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
#else
			this->frontUpdater->setPartition(static_cast<PMJMesh::Partition *>(originalTask->getPartition()));
#endif //#if USE_NEW_FRONT_UPDATER

			if (this->updateType != PMJMesh::FULL)
			{
#if USE_NEW_FRONT_UPDATER
				//static_cast<PartialMeshUpdater *>(this->meshUpdater)->setProcess(this->loadBalancer->getProcess(this->task));
#else
				static_cast<PartialFrontUpdater *>(this->frontUpdater)->setProcess(this->loadBalancer->getProcess(this->task));
#endif //#if USE_NEW_FRONT_UPDATER
			}
		}

		//std::cout << "Master, updating front, time for setting generated = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		//std::cout << "Master, updating front, time for updating cell position = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if (!USE_NEW_FRONT_UPDATER)
		this->frontUpdater->setTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
#endif //#if (!USE_NEW_FRONT_UPDATER)

		//std::cout << "testing mesh before update: " << std::endl;
		//this->mesh->test(this->boundary, this->front);

#if USE_NEW_FRONT_UPDATER
		this->meshUpdater->execute();
#endif //#if USE_NEW_FRONT_UPDATER
		this->frontUpdater->execute();

		//std::cout << "Master, updating front, time for executing front updater = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		//std::cout << "testing mesh after update: " << std::endl;
		//this->mesh->test(this->boundary, this->front);

		if (shared)
		{
			this->done.push_back(this->task);
		}

		//debug only one task uncomment this
		//break;
		//endebug
	}

	std::sort(this->nonWorkingTasks.begin(), this->nonWorkingTasks.end(), Data::Identifiable::greater);

	while (!this->nonWorkingTasks.empty())
	{
		this->task = this->nonWorkingTasks.back();

		this->nonWorkingTasks.pop_back();

		if (shared)
		{
#if USE_NEW_FRONT_UPDATER
			this->meshUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
			this->frontUpdater->setMasterTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
#else
			this->frontUpdater->setTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
#endif //#if USE_NEW_FRONT_UPDATER

#if USE_NEW_FRONT_UPDATER
			this->meshUpdater->execute();
#endif //#if USE_NEW_FRONT_UPDATER
			this->frontUpdater->execute();

			this->done.push_back(this->task);
		}
		else
		{
			PMJMesh::Front *front = static_cast<PMJMesh::Partition *>(this->task->getPartition())->getSubfront();

			if (front)
			{
				front->clear();
			}
		}
	}

	//std::cout << "Master, updating front, time for dealing with nonworkingtasks = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if USE_NEW_FRONT_UPDATER
	this->meshUpdater->finalize();
#endif //#if USE_NEW_FRONT_UPDATER
	this->frontUpdater->finalize();

	//std::cout << "Master, updating front, time for finalizing front updater = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	this->task = NULL;

	bool value = this->frontUpdater->isUpdated();

	//this->backReplaceInnerFront();

	if ((updateTree) &&
		((shared) || (!this->tasks.empty())))
	{
		PMJMesh::TreeUpdater *treeUpdater = new PMJMesh::TreeUpdater(this->dimension, this->tree, this->comm);

		treeUpdater->execute();

		delete treeUpdater;
	}

	//std::cout << "Master, updating front, time for refining tree according to position = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	this->updateGeneratedTasks(shared);

	//std::cout << "Master, updating front, time for updating generated tasks = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	return value;
}

void PMJMesh::GlobalTaskManager::sendTerminationMessage()
{
	//std::cout << "debug master will now send termination message" << std::endl;

	//ULInt buf[2] = {0, ((this->taskSent) ? 1 : 0)};
	ULInt buf[5] = {(this->status.getType() >= this->error.getType()) ? static_cast<ULInt>(2) : static_cast<ULInt>(0),
		((this->taskSent) ? static_cast<ULInt>(1) : static_cast<ULInt>(0)),
		this->idManager->getId(0), this->idManager->getId(1), 0};

	this->comm->broadcast(buf, 5, -1);
}

void PMJMesh::GlobalTaskManager::shiftTree(bool middle)
{
	this->shifter->setRestore(middle);

	this->shifter->execute();
}

void PMJMesh::GlobalTaskManager::divideFront()
{
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	//std::cout << "dividing front" << std::endl;

	this->tasks.swap(this->done);

	LoadCalculator *lc = this->makeLoadCalculator(false);

	//std::cout << "executing load calculator" << std::endl;

	lc->execute();

	//std::cout << "end executing load calculator" << std::endl;
	//std::cout << "getting load" << std::endl;

	//Data::RealMap load = lc->getTreeLoad();

	//std::cout << "end getting load" << std::endl;

	//std::cout << "debug totalload = " << load[(Data::BoxTreeCell *)this->tree->getRoot()] << std::endl;

	//std::cout << "debug tasks.size = " << this->tasks.size() << std::endl;

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		//std::cout << "debug tasks[" << i << "] = " << this->tasks[i]->getId() << std::endl;

		const Data::Box *b = static_cast<PMJMesh::Partition *>(static_cast<PMJMesh::MJMeshTask *>(this->tasks[i])->getPartition())->getBox();

		lc->setBox(b);

		//std::cout << "executing load calculator for task " << this->tasks[i]->getId() << std::endl;

		lc->execute();

		//std::cout << "end executing load calculator for task " << this->tasks[i]->getId() << std::endl;

		//std::cout << "debug partition = " << this->partitions[i]->getId() << std::endl;
		//std::cout << "debug previous load = " << this->partitions[i]->getLoad() << std::endl;

		this->tasks[i]->getPartition()->setLoad(lc->getLoad());

		//std::cout << "debug current load = " << this->partitions[i]->getLoad() << std::endl;
	}

	delete lc;

	UInt shift = this->shiftState();

	FrontPartitioner *fc = this->makeFrontPartitioner();

	//std::cout << "executing front partitioner" << std::endl;

	fc->execute();

	this->replacer = fc->getReplacer();

	fc->setReplacer(NULL);

	//std::cout << "end executing front partitioner" << std::endl;

	delete fc;

	//std::cout << "updating shift state generation" << std::endl;

	///se já passou fronteira pela tarefa (gerou elemento no estado central, 0),
	///  mas nao tem mais fronteira mais, entao a tarefa nao vai nunca gerar
	///  elemento nesse estado. logo, diga que esse estado já gerou elementos.
	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

		if ((t->isGenerated(0)) &&
			((!static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront()) ||
			 (static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront()->isEmpty())))
		{
			//std::cout << "partition " << t->getPartition()->getId() << " is empty" << std::endl;

			t->setGenerated(shift, true);
		}
	}

	//std::cout << "end updating shift state generation" << std::endl;

	//std::cout << "end dividing front" << std::endl;

	//std::cout << "ending divideFront()" << std::endl;

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI
}

void PMJMesh::GlobalTaskManager::sortFront()
{
#if (!USE_SET)
	if (this->comm->numWorkers() <= 0)
	{
		return;
	}

	if (!this->sorted)
	{
		return;
	}

	Data::FrontElementList sorted;

	static_cast<PMJMesh::Front *>(this->front)->moveRejectedToElements();

	Data::FrontElementList fels = this->front->getBoundary();

	while (!fels.empty())
	{
		Data::FrontElementList::iterator minIter = fels.begin();
		Real minSize = (*minIter)->getShape()->size();

		for (Data::FrontElementList::iterator iter = ++fels.begin();
			 iter != fels.end(); iter++)
		{
			Real size = (*iter)->getShape()->size();

			if (size < minSize)
			{
				minSize = size;
				minIter = iter;
			}
		}

		Data::FrontElement *min = (*minIter);

		fels.erase(minIter);

		sorted.push_back(min);
	}

	this->front->setBoundary(sorted);

	sorted.clear();

	fels = this->front->getElements();

	while (!fels.empty())
	{
		Data::FrontElementList::iterator minIter = fels.begin();
		Real minSize = (*minIter)->getShape()->size();

		for (Data::FrontElementList::iterator iter = ++fels.begin();
			 iter != fels.end(); iter++)
		{
			Real size = (*iter)->getShape()->size();

			if (size < minSize)
			{
				minSize = size;
				minIter = iter;
			}
		}

		Data::FrontElement *min = (*minIter);

		fels.erase(minIter);

		sorted.push_back(min);
	}

	this->front->setElements(sorted);
#endif //#if (!USE_SET)
}

void PMJMesh::GlobalTaskManager::executeGeometryGeneration()
{
	//std::cout << "debug boundary size = " << this->boundary->verticesSize() << std::endl;
	//std::cout << "debug front size = " << this->front->verticesSize() << std::endl;

	//std::cout << "before geometry generation" << std::endl;
	//this->mesh->test(this->boundary, this->front);

	this->sortFront();

	if (this->minRefinementLevel > 0)
	{
		PMJMesh::MinTreeRefiner *minRefiner = new PMJMesh::MinTreeRefiner(
			this->dimension, this->tree, NULL, this->minRefinementLevel, false,
			//true);
			//static_cast<PMJMesh::Communicator *>(this->comm)->isShParallel());
			//static_cast<PMJMesh::Communicator *>(this->comm));
			NULL);

		//minRefiner->setMinLevel(this->minRefinementLevel);

		minRefiner->execute();

		delete minRefiner;
	}

#if USE_DELETE_FELS
	MJMesh::Boundary boundary;

	for (Data::Front::VertexIterator iter = this->front->vBegin();
         iter != this->front->vEnd(); iter++)
    {
        boundary.add((*iter));
    }

    for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin();
         iter != this->front->allEEnd(); iter++)
    {
        boundary.add((*iter));
    }

	MJMesh::GeometryFrontAdvancer *geometryAdvancer =
		new MJMesh::GeometryFrontAdvancer(this->dimension, &boundary, this->tree, this->usingSearchTrees);
#else
	MJMesh::GeometryFrontAdvancer *geometryAdvancer =
		new MJMesh::GeometryFrontAdvancer(this->dimension, this->boundary, this->tree, this->usingSearchTrees);
#endif //#if USE_DELETE_FELS

	geometryAdvancer->setIdManager(this->idManager);

	geometryAdvancer->setFront(this->front);
	geometryAdvancer->setMesh(this->mesh);

	if (this->usingSearchTrees)
	{
		if (!this->front->getVSTree())
		{
			this->front->setVSTree(geometryAdvancer->makeVertexSearchTree());
		}

		if (!this->front->getFESTree())
		{
			this->front->setFESTree(geometryAdvancer->makeFrontElementSearchTree());
		}
	}

	//std::cout << "debug boundary size = " << this->boundary->verticesSize() << std::endl;
	//std::cout << "debug front size = " << this->front->verticesSize() << std::endl;
	//std::cout << "debug front fels size = " << this->front->size() << std::endl;

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	geometryAdvancer->execute();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if USE_DELETE_FELS
    boundary.clear();
#endif //#if USE_DELETE_FELS

	if ((geometryAdvancer->getStatus().getType() >= geometryAdvancer->error.getType()))
	{
		{
			std::stringstream str;
			str << "ERROR: Some error occurred in final geometry advancer: " << geometryAdvancer->getStatus().getMessage() << std::endl;
			std::cout << str.str();
		}

		this->status = this->error;

		this->status.append(geometryAdvancer->getStatus());
	}

	delete geometryAdvancer;

	//std::cout << "after geometry generation" << std::endl;
	//this->mesh->test(this->boundary, this->front);
}

bool PMJMesh::GlobalTaskManager::receiveTasksFromMaster()
{
	ULInt buf[5] = {0, 0, 0, 0, 0};

	this->comm->broadcast(buf, 5, this->comm->root());

	/*std::stringstream str;
	str << "process " << this->comm->rank() << ", received broadcasted ids, vertex: " << buf[2] << ", fel: " << buf[3] << ", el: " << buf[4] << std::endl;
	std::cout << str.str();*/

	if (buf[2] != 0)
	{
		this->idManager->setId(0, buf[2]);
	}

	if (buf[3] != 0)
	{
		this->idManager->setId(1, buf[3]);
	}

	if (buf[4] != 0)
	{
		this->idManager->setId(2, buf[4]);
	}

	if (buf[0] == 0)
	{
		this->clearBoundaries();

		this->taskSent = (buf[1] > 0);

		if ((this->taskSent) &&
			((this->updateType == PMJMesh::EMPTY) ||
			 (this->updateType == PMJMesh::EMPTY_NO_BOUNDARY) ||
			 (this->updateType == PMJMesh::FINAL)))
		{
#if USE_NEW_FRONT_UPDATER
			static_cast<EmptyMeshUpdater *>(this->meshUpdater)->splitMesh();
#else
			static_cast<EmptyFrontUpdater *>(this->frontUpdater)->splitMesh();
#endif //#if USE_NEW_FRONT_UPDATER
		}

		return false;
	}
	else if (buf[0] != 1)
	{
		this->status = this->error;

#if (!USE_GUI)
		this->comm->finalize();

		exit(EXIT_FAILURE);
#endif //#if (!USE_GUI)

		return false;
	}

	//std::cout << "received idVertex = " << this->idManager->getId(0) << ", idFrontElement = " << this->idManager->getId(1) << std::endl;

	//std::cout << "debug C processor " << this->comm->rank() << std::endl;

	//std::cout << "debug D processor " << this->comm->rank() << std::endl;

	this->loadBalancer->receiveFromMaster();

	return true;
}

void PMJMesh::GlobalTaskManager::generateMeshInCells()
{
#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	Real debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	this->numTasks = this->loadBalancer->numTasks();

#if USE_NEW_FRONT_UPDATER
	if ((this->comm->isSlave()) &&
		(this->updateType != PMJMesh::FULL) &&
		(!this->slaveIdManager))
	{
		this->slaveIdManager = new Performer::RangedIdManager(1, 1, 1, 1, 3);

		this->slaveIdManager->setMin(0, this->idManager->getId(0));
		this->slaveIdManager->setMin(1, this->idManager->getId(1));
		this->slaveIdManager->setMin(2, this->idManager->getId(2));
	}
#endif //#if USE_NEW_FRONT_UPDATER

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;

	Real timeMeshGeneratorWorkerMeshGeneration,
		timeMeshGeneratorWorkerMeshImprovement,
		timeMeshGeneratorWorkerTreeClassification,
		timeMeshGeneratorWorkerFromMToolsConversion,
		timeMeshGeneratorWorkerToMToolsConversion,
		timeMeshGeneratorWorkerOthers;

	timeMeshGeneratorWorkerMeshGeneration = this->timeMeshGeneratorWorkerMeshGeneration;
	timeMeshGeneratorWorkerMeshImprovement = this->timeMeshGeneratorWorkerMeshImprovement;
	timeMeshGeneratorWorkerTreeClassification = this->timeMeshGeneratorWorkerTreeClassification;
	timeMeshGeneratorWorkerFromMToolsConversion = this->timeMeshGeneratorWorkerFromMToolsConversion;
	timeMeshGeneratorWorkerToMToolsConversion = this->timeMeshGeneratorWorkerToMToolsConversion;
	timeMeshGeneratorWorkerOthers = this->timeMeshGeneratorWorkerOthers;

	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	TaskManager::execute();

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	Real timeExecuting = Data::time() - debugstart;

	timeExecuting -= (this->timeMeshGeneratorWorkerMeshGeneration - timeMeshGeneratorWorkerMeshGeneration);
	timeExecuting -= (this->timeMeshGeneratorWorkerMeshImprovement - timeMeshGeneratorWorkerMeshImprovement);
	timeExecuting -= (this->timeMeshGeneratorWorkerTreeClassification - timeMeshGeneratorWorkerTreeClassification);
	timeExecuting -= (this->timeMeshGeneratorWorkerFromMToolsConversion - timeMeshGeneratorWorkerFromMToolsConversion);
	timeExecuting -= (this->timeMeshGeneratorWorkerToMToolsConversion - timeMeshGeneratorWorkerToMToolsConversion);
	timeExecuting -= (this->timeMeshGeneratorWorkerOthers - timeMeshGeneratorWorkerOthers);

	this->timeMeshGeneratorMasterOthers += timeExecuting;

	debugstart = Data::time();
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))

	//std::cout << "debug processor " << this->comm->rank() << " AAAA, slave = " << this->comm->isSlave() << ", smoothEnabled = " << this->smoothInSlavesEnabled << std::endl;

	if ((this->comm->isSlave()) && (this->updateType != PMJMesh::FULL))
	{
#if USE_NEW_FRONT_UPDATER
		if (static_cast<PartialMeshUpdater *>(this->meshUpdater)->getIdManager() != this->slaveIdManager)
		{
			this->slaveIdManager->setRange(this->idrange);
			this->slaveIdManager->setOffset(this->idoffset);

			ULInt procOffset = (this->comm->rank() - 1)*this->idrange;

			this->slaveIdManager->setMin(0, this->slaveIdManager->getMin(0) + procOffset);
			this->slaveIdManager->setMin(1, this->slaveIdManager->getMin(1) + procOffset);
			this->slaveIdManager->setMin(2, this->slaveIdManager->getMin(2) + procOffset);

			static_cast<PartialMeshUpdater *>(this->meshUpdater)->setIdManager(this->slaveIdManager);
		}

		this->slaveIdManager->setId(0, this->idManager->getId(0));
		this->slaveIdManager->setId(1, this->idManager->getId(1));
		this->slaveIdManager->setId(2, this->idManager->getId(2));

		this->meshUpdater->initialize();
#else
		this->frontUpdater->initialize();
#endif //#if USE_NEW_FRONT_UPDATER

		for (UInt i = 0; i < this->tasks.size(); i++)
		{
			this->task = this->tasks[i];

			//std::cout << "task = " << this->task->getId() << std::endl;

#if USE_NEW_FRONT_UPDATER
			/*Performer::RangedIdManager *idManager = new Performer::RangedIdManager(1, 1, this->idoffset, this->idrange, 3);

			ULInt procOffset = (this->comm->rank() - 1)*this->idrange;

			idManager->setMin(0, this->idManager->getId(0) + procOffset);
			idManager->setMin(1, this->idManager->getId(1) + procOffset);
			idManager->setMin(2, this->idManager->getId(2) + procOffset);

			idManager->setId(0, idManager->getMin(0));
			idManager->setId(1, idManager->getMin(1));
			idManager->setId(2, idManager->getMin(2));

			static_cast<PartialMeshUpdater *>(this->meshUpdater)->setIdManager(idManager);*/

			this->meshUpdater->setWorkerTask(static_cast<PMJMesh::MJMeshTask *>(this->task));

			this->meshUpdater->execute();

			/*this->idManager->setId(0, std::max(this->idManager->getId(0), idManager->getId(0)));
			this->idManager->setId(1, std::max(this->idManager->getId(1), idManager->getId(1)));
			this->idManager->setId(2, std::max(this->idManager->getId(2), idManager->getId(2)));

			static_cast<PartialMeshUpdater *>(this->meshUpdater)->setIdManager(this->idManager);

			delete idManager;*/
#else
			this->frontUpdater->setTask(static_cast<PMJMesh::MJMeshTask *>(this->task));
			this->frontUpdater->setPartition(static_cast<PMJMesh::Partition *>(this->task->getPartition()));

			this->frontUpdater->execute();
#endif //#if USE_NEW_FRONT_UPDATER
		}

#if USE_NEW_FRONT_UPDATER
		this->meshUpdater->finalize();

		if (this->slaveIdManager)
		{
			this->idManager->setId(0, std::max(this->idManager->getId(0), this->slaveIdManager->getId(0)));
			this->idManager->setId(1, std::max(this->idManager->getId(1), this->slaveIdManager->getId(1)));
			this->idManager->setId(2, std::max(this->idManager->getId(2), this->slaveIdManager->getId(2)));
		}

		/*this->idManager->setId(0, std::max(this->idManager->getId(0), idManager->getId(0)));
		this->idManager->setId(1, std::max(this->idManager->getId(1), idManager->getId(1)));
		this->idManager->setId(2, std::max(this->idManager->getId(2), idManager->getId(2)));

		static_cast<PartialMeshUpdater *>(this->meshUpdater)->setIdManager(this->idManager);

		delete idManager;*/
#else
		this->frontUpdater->finalize();
#endif //#if USE_NEW_FRONT_UPDATER

		this->task = NULL;

		//debug
		//UInt debug = 0;
		//this->comm->receive(debug, 1);
		//endebug
	}

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
	this->timeMeshGeneratorMasterOthers += Data::time() - debugstart;
#endif //#if ((DEBUG_TIMING_PMTOOLS) || (DEBUG_TIMING_PMJMESH))
}

void PMJMesh::GlobalTaskManager::sendTasksForMaster(bool updateTree)
{
	ULInt data[4] = {this->idManager->getId(0), this->idManager->getId(1), this->idManager->getId(2), this->status.getType()};

	//std::cout << "slaves, before reduce" << std::endl;
	/*std::stringstream str;
	str << "process " << this->comm->rank() << ", sending ids to master, vertex: " << data[0] << ", fel: " << data[1] << " and el: " << data[2] << std::endl;
	std::cout << str.str();*/

	this->comm->reduce(data, 3, Communicator::MAXIMUM, this->comm->root());

	//std::cout << "slaves, after reduce" << std::endl;

	this->loadBalancer->setTasks(this->tasks);

	this->tasks.clear();

	//std::cout << "global geometry task manager, sending tasks for master, process " << this->comm->rank() << std::endl;

	this->loadBalancer->sendToMaster();

	//std::cout << "global geometry task manager, sent tasks for master, process " << this->comm->rank() << std::endl;

	//std::cout << "tarefas enviadas" << std::endl;

	if (updateTree)
	{
		PMJMesh::TreeUpdater *treeUpdater = new PMJMesh::TreeUpdater (this->dimension, this->tree, this->comm);

		treeUpdater->execute();

		delete treeUpdater;
	}
}

void PMJMesh::GlobalTaskManager::clearDone()
{
	while (!this->done.empty())
	{
		this->task = this->done.back();

		this->done.pop_back();

		static_cast<PMJMesh::Partition *>(this->task->getPartition())->setBox(NULL);

		delete this->task;
	}

	this->task = NULL;

	{
		//master

		if (this->partitionTree)
		{
			delete this->partitionTree;

			this->partitionTree = NULL;
		}

		this->partitions.clear();
	}
}

UInt PMJMesh::GlobalTaskManager::shiftState() const
{
	return (this->shifter) ? this->shifter->state() : 0;
}

void PMJMesh::GlobalTaskManager::initializeGeneratedTasks()
{
	for (Parallel::TaskVector::iterator iter = this->tasks.begin();
		 iter != this->tasks.end(); iter++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));
		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(t->getId()));

		//std::cout << "task id = " << t->getId() << std::endl;

		//for (UInt i = 0; i < this->shifter->numShiftedStates(); i++)
		for (UInt i = 1; i < this->shifter->maxStates(); i++)
		{
			if (((i >= this->shifter->minState(0)) && (i < this->shifter->maxState(0)) && (!this->shifter->isLateral())) ||
				((i >= this->shifter->minState(1)) && (i < this->shifter->maxState(1)) && (!this->shifter->isDiagonal())) ||
				((i >= this->shifter->minState(2)) && (i < this->shifter->maxState(2)) && (!this->shifter->isDiagonalVertex())))
			{
				continue;
			}

			//Data::GraphNodeList neighs = this->partitionTree->neighbors(cell, i);
			Data::UIntVector sides = this->shifter->sides(i);
			Data::GraphNodeList neighs = this->partitionTree->neighbors(cell, sides);

			if ((!this->shifter->isLateral()) && (sides.size() > 1))
			{
				for (UInt i = 0; i < sides.size(); i++)
				{
					Data::GraphNodeList moreNeighs = this->partitionTree->neighbors(cell, sides[i]);

					neighs.splice(neighs.end(), moreNeighs);
				}
			}

			bool found = false;

			//for (UInt j = 0; j < neighs.size(); j++)
			for (Data::GraphNodeList::iterator iter2 = neighs.begin();
				 iter2 != neighs.end(); iter2++)
			{
				for (UInt k = 0; k < this->tasks.size(); k++)
				{
					//if (this->tasks[k]->getId() == neighs[j]->getId())
					if (this->tasks[k]->getId() == (*iter2)->getId())
					{
						found = true;

						break;
					}
				}

				if (found)
				{
					break;
				}
			}

			if (!found)
			{
				//t->setGenerated(i + 1, true);
				t->setGenerated(i, true);
			}
		}
	}
}

void PMJMesh::GlobalTaskManager::updateGeneratedTasks(bool shared)
{
	if ((this->partitionerType == PMJMesh::BOX_LOAD) || (this->partitionerType == PMJMesh::BOX_NUMBER))
	{
		UInt state = this->shiftState();

		//std::cout << "updating generated tasks " << std::endl;

		if (state > 0)
		{
			Parallel::TaskVector generated;

			for (Parallel::TaskVector::iterator iter = this->tasks.begin();
				 iter != this->tasks.end(); iter++)
			{
				this->task = (*iter);

				//std::cout << "now testing task " << this->task->getId() << std::endl;

				PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->task);

				if (!shared)
				{
					for (UInt i = 0; i < this->done.size(); i++)
					{
						if (this->done[i]->getId() == this->task->getId())
						{
							t = static_cast<PMJMesh::MJMeshTask *>(this->done[i]);

							break;
						}
					}
				}

				if (!t->isGenerated(state))
				{
					//std::cout << "task " << this->task->getId() << " has not generated mesh" << std::endl;

					continue;
				}

				generated.push_back(t);
			}

			//state--;

			//UInt opposite = (state + this->dimension)%(this->shifter->numShiftedStates()) + 1;
			UInt opposite = this->shifter->oppositeState();

			for (Parallel::TaskVector::iterator iter = generated.begin();
				 iter != generated.end(); iter++)
			{
				PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

				//debug
				//std::cout << "t->id = " << t->getId() << std::endl;
				//std::cout << "side  = " << state << std::endl;
				//std::cout << "oppos = " << opposite << std::endl;
				//endebug

				Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(t->getId()));

				//Data::GraphNodeList neighs = this->partitionTree->neighbors(cell, state);
				Data::GraphNodeList neighs = this->partitionTree->neighbors(cell, this->shifter->sides(state));

				//debug
				//std::cout << "cell->id = " << cell->getId() << std::endl;
				//std::cout << "neighs.size() = " << neighs.size() << std::endl;
				//endebug

				if (neighs.empty())
				{
					//std::cout << "task " << t->getId() << " has no neighbor in side " << state << std::endl;

					continue;
				}

				if ((neighs.size() == 1) &&
					//(static_cast<Data::TreeNode *>(neighs[0])->level() < cell->level()))
					(static_cast<Data::TreeNode *>(neighs.front())->level() < cell->level()))
				{
					//debug
					//std::cout << "smaller level" << std::endl;
					//endebug

					continue;
				}

				//for (UInt i = 0; i < neighs.size(); i++)
				for (Data::GraphNodeList::iterator neighIter = neighs.begin();
					 neighIter != neighs.end(); neighIter++)
				{
					for (Parallel::TaskVector::iterator iter = this->done.begin();
						 iter != this->done.end(); iter++)
					{
						//if ((*iter)->getId() == neighs[i]->getId())
						if ((*iter)->getId() == (*neighIter)->getId())
						{
							//debug
							//std::cout << "neighbor task of " << neighs[i]->getId() << " = " << (*iter)->getId() << std::endl;
							//endebug

							static_cast<PMJMesh::MJMeshTask *>((*iter))->setGenerated(opposite, true);

							break;
						}
					}
				}
			}
		}
	}

	if (!shared)
	{
		for (Parallel::TaskVector::iterator iter = this->tasks.begin();
			 iter != this->tasks.end(); iter++)
		{
			delete (*iter);
		}
	}

	this->tasks.clear();

	this->task = NULL;
}

bool PMJMesh::GlobalTaskManager::completelyGenerated() const
{
	if (this->completelyGeneratedSafetyCheck == this->maxCompletelyGeneratedSafetyCheck)
	{
		return true;
	}

	for (Parallel::TaskVector::const_iterator iter = this->done.begin();
		 iter != this->done.end(); iter++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

		/*//for (UInt i = 0; i < this->shifter->numStates(); i++)
		for (UInt i = 0; i < this->shifter->maxStates(); i++)
		{
			if (!t->isGenerated(i))
			{
				return false;
			}
		}*/

		if (!t->isCompletelyGenerated())
		{
			return false;
		}
	}

	return true;
}

/*//debug
void PMJMesh::GlobalTaskManager::debugHighlightInterTasks()
{
	//std::cout << "this->tasks.size() = " << this->tasks.size() << std::endl;

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		MJMeshTask *t = static_cast<MJMeshTask *>(this->tasks[i]);

		if (this->isInterTask(t))
		{
			t->highlight();
		}
		else
		{
			t->unhighlight();
		}
	}
}
//endebug*/

void PMJMesh::GlobalTaskManager::relatedTasks(const Data::GraphNodeList &neighCells, Parallel::TaskList &neighTasks) const
{
	for (Data::GraphNodeList::const_iterator iter = neighCells.begin();
		 iter != neighCells.end(); iter++)
	{
		Data::BoxTreeCell *neighCell = static_cast<Data::BoxTreeCell *>((*iter));

		for (Parallel::TaskVector::const_iterator iter2 = this->tasks.begin();
			 iter2 != this->tasks.end(); iter2++)
		{
			MJMeshTask *neighTask = static_cast<MJMeshTask *>((*iter2));

			if (neighTask->getId() == neighCell->getId())
			{
				neighTasks.push_back(neighTask);

				break;
			}
		}
	}
}

bool PMJMesh::GlobalTaskManager::foundNonGenerated(const Parallel::TaskList &tasks, Parallel::TaskList *nonGenerated) const
{
	for (Parallel::TaskList::const_iterator iter = tasks.begin();
		 iter != tasks.end(); iter++)
	{
		MJMeshTask *t = static_cast<MJMeshTask *>((*iter));

		if (!t->isGenerated(0))
		{
			if (nonGenerated)
			{
				nonGenerated->push_back(t);

				continue;
			}

			return true;
		}
	}

	return nonGenerated ? !nonGenerated->empty() : false;
}

bool PMJMesh::GlobalTaskManager::foundNonGenerated(const Data::GraphNodeList &neighs, Parallel::TaskList *nonGenerated) const
{
	for (Data::GraphNodeList::const_iterator iter = neighs.begin();
		 iter != neighs.end(); iter++)
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter));

		for (Parallel::TaskVector::const_iterator iter2 = this->tasks.begin();
			 iter2 != this->tasks.end(); iter2++)
		{
			MJMeshTask *neighTask = static_cast<MJMeshTask *>((*iter2));

			if ((neighTask->getId() == neigh->getId()) &&
				(!neighTask->isGenerated(0)))
			{
				if (nonGenerated)
				{
					nonGenerated->push_back(neighTask);

					continue;
				}

				return true;
			}
		}
	}

	return nonGenerated ? !nonGenerated->empty() : false;
}

bool PMJMesh::GlobalTaskManager::foundNonGenerated(const Data::GraphNodeList &neighs) const
{
	return this->foundNonGenerated(neighs, NULL);
}

bool PMJMesh::GlobalTaskManager::isGeneratedExcept(const MJMeshTask *task, UInt except) const
{
	for (UInt state = 1; state < this->shifter->maxStates(); state++)
	{
		if (state == except)
		{
			continue;
		}

		if (!task->isGenerated(state))
		{
			return false;
		}
	}

	return true;
}

bool PMJMesh::GlobalTaskManager::isInterTask(const MJMeshTask *task) const
{
	UInt state = this->shifter->state();

	if (task->isGenerated(state))
	{
		return false;
	}

	if ((!static_cast<PMJMesh::Partition *>(task->getPartition())->getSubfront()) ||
		(static_cast<PMJMesh::Partition *>(task->getPartition())->getSubfront()->isEmpty()))
	{
		return false;
	}

	if (state == 0)
	{
		UInt numNonGens = 0;

		for (UInt state = 1; state < this->shifter->maxStates(); state++)
		{
			if (!task->isGenerated(state))
			{
				numNonGens++;
			}
		}

		if (numNonGens <= 1)
		{
			return true;
		}

		return false;
	}
	else
	{
		Data::UIntVector sides = this->shifter->sides(state);

		UInt opposite = this->shifter->oppositeState();

		if (!task->isGenerated(0))
		{
			if (this->isGeneratedExcept(task, state))
			{
				Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(task->getId()));

				Data::GraphNodeList neighs;

				this->partitionTree->neighbors(cell, sides, neighs);

				Parallel::TaskList neighTasks;

				this->relatedTasks(neighs, neighTasks);

				{
					bool found = false;

					for (Parallel::TaskList::iterator iter = neighTasks.begin();
						 iter != neighTasks.end(); iter++)
					{
						MJMeshTask *t = static_cast<MJMeshTask *>((*iter));

						if (!this->isGeneratedExcept(t, opposite))
						{
							found = true;

							break;
						}
					}

					if (!found)
					{
						return true;
					}
				}
			}

			return false;
		}

		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->partitionTree->node(task->getId()));

		Data::GraphNodeList neighs;

		this->partitionTree->neighbors(cell, sides, neighs);

		Parallel::TaskList neighTasks;

		this->relatedTasks(neighs, neighTasks);

		{
			bool found = false;

			for (Parallel::TaskList::iterator iter = neighTasks.begin();
				 iter != neighTasks.end(); iter++)
			{
				MJMeshTask *t = static_cast<MJMeshTask *>((*iter));

				if (!t->isGenerated(opposite))
				{
					found = true;

					break;
				}
			}

			if (!found)
			{
				return true;
			}
		}

		//if (this->foundNonGenerated(neighs))
		if (this->foundNonGenerated(neighTasks))
		{
			return false;
		}

		if (sides.size() != 1)
		{
			for (UInt i = 0; i < sides.size(); i++)
			{
				Data::GraphNodeList neighs;

				this->partitionTree->neighbors(cell, sides[i], neighs);

				if (this->foundNonGenerated(neighs))
				{
					return false;
				}
			}

			if (sides.size() != 2)
			{
				Data::UIntVector sds(2);

				for (UInt i = 0; i < 2; i++)
				{
					for (UInt j = i + 1; j < 3; j++)
					{
						sds[0] = sides[i];
						sds[1] = sides[j];

						Data::GraphNodeList neighs;

						this->partitionTree->neighbors(cell, sds, neighs);

						if (this->foundNonGenerated(neighs))
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool PMJMesh::GlobalTaskManager::checkInnerFront(const Data::VertexList &vertices,
	const Data::FrontElementList &fels, const PMJMesh::Partition *partition, Real size) const
{
    for (Data::VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		if (partition->getBox()->optOut(p))
		{
			return false;
		}
	}

	Real dist = size*this->rangeProportion;

	for (Data::FrontElementList::const_iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

#if USE_EXPERIMENTAL_3
        struct TestBox
        {
            static bool test(const Data::Point &mid, Data::Vector &n, const PMJMesh::Partition *partition, const Real &dist, const Real &size)
            {
                n.multiply(size);

                n.sum(mid);

                UInt dimension = n.dimension();

                Data::Point2D min2d, max2d;
                Data::Point3D min3d, max3d;

                Data::Point &min = (dimension == 2) ? static_cast<Data::Point &>(min2d) : static_cast<Data::Point &>(min3d);
                Data::Point &max = (dimension == 2) ? static_cast<Data::Point &>(max2d) : static_cast<Data::Point &>(max3d);

                for (UInt i = 0; i < dimension; i++)
                {
                    min.setCoord(i, n.getCoord(i) - dist);
                    max.setCoord(i, n.getCoord(i) + dist);
                }

                /*if ((partition->getBox()->optOut(min)) || (partition->getBox()->optOut(max)))
                {
                    return false;
                }

                return true;*/

                return ((!partition->getBox()->optOut(min)) && (!partition->getBox()->optOut(max)));
            }
        };

        if (this->dimension == 2)
        {
            Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

            if (!TestBox::test(fel->getShape()->mid2D(), n, partition, dist, size))
            {
                return false;
            }
        }
        else
        {
            Data::Vector3D n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();

            if (!TestBox::test(fel->getShape()->mid3D(), n, partition, dist, size))
            {
                return false;
            }
        }
#else
		Data::Point mid = fel->getShape()->mid();

		Data::Vector v(mid);
		Data::Vector n(this->dimension);
		if (this->dimension == 2)
		{
			n = static_cast<Data::Edge2D *>(fel->getShape())->normal();
		}
		else
		{
			n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();
		}

		n.multiply(size);

		n.sum(mid);

		Data::Point min(n), max(n);

		for (UInt i = 0; i < this->dimension; i++)
		{
			min.setCoord(i, min.getCoord(i) - dist);
			max.setCoord(i, max.getCoord(i) + dist);
		}

		if ((partition->getBox()->optOut(min)) || (partition->getBox()->optOut(max)))
		{
			return false;
		}
#endif //#if USE_EXPERIMENTAL_3
	}

    return true;
}

void PMJMesh::GlobalTaskManager::createInnerFront(PMJMesh::Partition *p)
{
#if USE_EXPERIMENTAL_3
	Data::Point2D mid2d;
	Data::Point3D mid3d;

	Data::Point &mid = (this->dimension == 2) ?
        (mid2d = p->getBox()->mid2D(), static_cast<Data::Point &>(mid2d)) :
        (mid3d = p->getBox()->mid3D(), static_cast<Data::Point &>(mid3d));
#else
	Data::Point mid = p->getBox()->mid();
#endif //#if USE_EXPERIMENTAL_3

	Data::BoxTreeCell *cell = this->tree->find(mid);

	UInt level = cell->level();

	Real size = cell->size();

	if ((this->minRefinementLevel > 0) && (level < this->minRefinementLevel))
	{
		size /= std::pow(2.0, this->minRefinementLevel - level);
	}

	Data::VertexList newVertices;
	Data::FrontElementList newFels;

	bool ok = true;

	if (this->dimension == 2)
	{
		if (this->createCrackInInnerFront)
		{
			Data::Point2D *p1 = new Data::Point2D(mid.getCoord(0) - size, mid.getCoord(1));
			Data::Point2D *p2 = new Data::Point2D(mid.getCoord(0), mid.getCoord(1));
			Data::Point2D *p3 = new Data::Point2D(mid.getCoord(0) + size, mid.getCoord(1));
			Data::Point2D *p4 = new Data::Point2D(mid.getCoord(0), mid.getCoord(1));

			if (this->displaceVerticesInInnerFront)
			{
				Real displace = 0.125*size;

				p2->setCoord(1, p2->getCoord(1) + displace);
				p4->setCoord(1, p4->getCoord(1) - displace);
			}

			MJMesh::Vertex *v1 = new MJMesh::Vertex(p1, this->idManager->next(0));
			MJMesh::Vertex *v2 = new MJMesh::Vertex(p2, this->idManager->next(0));
			MJMesh::Vertex *v3 = new MJMesh::Vertex(p3, this->idManager->next(0));
			MJMesh::Vertex *v4 = new MJMesh::Vertex(p4, this->idManager->next(0));

			MJMesh::FrontElement *f1 = new MJMesh::FrontElement2D(v1, v2, this->idManager->next(1));
			MJMesh::FrontElement *f2 = new MJMesh::FrontElement2D(v2, v3, this->idManager->next(1));
			MJMesh::FrontElement *f3 = new MJMesh::FrontElement2D(v3, v4, this->idManager->next(1));
			MJMesh::FrontElement *f4 = new MJMesh::FrontElement2D(v4, v1, this->idManager->next(1));

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);
			newVertices.push_back(v4);

			newFels.push_back(f1);
			newFels.push_back(f2);
			newFels.push_back(f3);
			newFels.push_back(f4);

			ok = this->checkInnerFront(newVertices, newFels, p, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = new Data::VertexList();
				this->createdInnerVertices[v2->getId()].temporaries->push_back(v4);

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f4);

				this->createdInnerFels[f2->getId()].original = f2;
				this->createdInnerFels[f2->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f2->getId()].temporaries->push_back(f3);
			}
		}
		else
		{
			Real disp = size*0.5;

			Data::Point2D *p1 = new Data::Point2D(mid.getCoord(0) - disp, mid.getCoord(1));
			Data::Point2D *p2 = new Data::Point2D(mid.getCoord(0) + disp, mid.getCoord(1));

			MJMesh::Vertex *v1 = new MJMesh::Vertex(p1, this->idManager->next(0));
			MJMesh::Vertex *v2 = new MJMesh::Vertex(p2, this->idManager->next(0));

			MJMesh::FrontElement *f1 = new MJMesh::FrontElement2D(v1, v2, this->idManager->next(1));
			MJMesh::FrontElement *f2 = new MJMesh::FrontElement2D(v2, v1, this->idManager->next(1));

			newVertices.push_back(v1);
			newVertices.push_back(v2);

			newFels.push_back(f1);
			newFels.push_back(f2);

			ok = this->checkInnerFront(newVertices, newFels, p, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f2);
			}
		}
	}
	else if (this->dimension == 3)
	{
		if (this->createCrackInInnerFront)
		{
			Data::Point3D *p1 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p2 = new Data::Point3D(mid.getCoord(0) - size, mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p3 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2) + size);
			Data::Point3D *p4 = new Data::Point3D(mid.getCoord(0) + size, mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p5 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2) - size);
			Data::Point3D *p6 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2));

			if (this->displaceVerticesInInnerFront)
			{
				Real displace = 0.125*size;

				p1->setCoord(1, p1->getCoord(1) + displace);
				p6->setCoord(1, p6->getCoord(1) - displace);
			}

			MJMesh::Vertex *v1 = new MJMesh::Vertex(p1, this->idManager->next(0));
			MJMesh::Vertex *v2 = new MJMesh::Vertex(p2, this->idManager->next(0));
			MJMesh::Vertex *v3 = new MJMesh::Vertex(p3, this->idManager->next(0));
			MJMesh::Vertex *v4 = new MJMesh::Vertex(p4, this->idManager->next(0));
			MJMesh::Vertex *v5 = new MJMesh::Vertex(p5, this->idManager->next(0));
			MJMesh::Vertex *v6 = new MJMesh::Vertex(p6, this->idManager->next(0));

			MJMesh::FrontElement *f1 = new MJMesh::FrontElement3D(v1, v2, v3, this->idManager->next(1));
			MJMesh::FrontElement *f2 = new MJMesh::FrontElement3D(v1, v3, v4, this->idManager->next(1));
			MJMesh::FrontElement *f3 = new MJMesh::FrontElement3D(v1, v4, v5, this->idManager->next(1));
			MJMesh::FrontElement *f4 = new MJMesh::FrontElement3D(v1, v5, v2, this->idManager->next(1));
			MJMesh::FrontElement *f5 = new MJMesh::FrontElement3D(v6, v3, v2, this->idManager->next(1));
			MJMesh::FrontElement *f6 = new MJMesh::FrontElement3D(v6, v4, v3, this->idManager->next(1));
			MJMesh::FrontElement *f7 = new MJMesh::FrontElement3D(v6, v5, v4, this->idManager->next(1));
			MJMesh::FrontElement *f8 = new MJMesh::FrontElement3D(v6, v2, v5, this->idManager->next(1));

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);
			newVertices.push_back(v4);
			newVertices.push_back(v5);
			newVertices.push_back(v6);

			newFels.push_back(f1);
			newFels.push_back(f2);
			newFels.push_back(f3);
			newFels.push_back(f4);
			newFels.push_back(f5);
			newFels.push_back(f6);
			newFels.push_back(f7);
			newFels.push_back(f8);

			ok = this->checkInnerFront(newVertices, newFels, p, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = new Data::VertexList();
				this->createdInnerVertices[v1->getId()].temporaries->push_back(v6);

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerVertices[v4->getId()].original = v4;
				this->createdInnerVertices[v4->getId()].temporaries = NULL;

				this->createdInnerVertices[v5->getId()].original = v5;
				this->createdInnerVertices[v5->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f5);

				this->createdInnerFels[f2->getId()].original = f2;
				this->createdInnerFels[f2->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f2->getId()].temporaries->push_back(f6);

				this->createdInnerFels[f3->getId()].original = f3;
				this->createdInnerFels[f3->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f3->getId()].temporaries->push_back(f7);

				this->createdInnerFels[f4->getId()].original = f4;
				this->createdInnerFels[f4->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f4->getId()].temporaries->push_back(f8);
			}
		}
		else
		{
			Real disp = size*0.5;

			Data::Point3D *p1 = new Data::Point3D(mid.getCoord(0) - disp*0.5, mid.getCoord(1), mid.getCoord(2) + disp*0.5);
			Data::Point3D *p2 = new Data::Point3D(mid.getCoord(0) + disp,     mid.getCoord(1), mid.getCoord(2) + disp*0.5);
			Data::Point3D *p3 = new Data::Point3D(mid.getCoord(0) - disp*0.5, mid.getCoord(1), mid.getCoord(2) - disp);

			MJMesh::Vertex *v1 = new MJMesh::Vertex(p1, this->idManager->next(0));
			MJMesh::Vertex *v2 = new MJMesh::Vertex(p2, this->idManager->next(0));
			MJMesh::Vertex *v3 = new MJMesh::Vertex(p3, this->idManager->next(0));

			MJMesh::FrontElement *f1 = new MJMesh::FrontElement3D(v1, v2, v3, this->idManager->next(1));
			MJMesh::FrontElement *f2 = new MJMesh::FrontElement3D(v1, v3, v2, this->idManager->next(1));

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);

			newFels.push_back(f1);
			newFels.push_back(f2);

			ok = this->checkInnerFront(newVertices, newFels, p, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f2);
			}
		}
	}

	while (!newFels.empty())
	{
		Data::FrontElement *fel = newFels.front();

		newFels.pop_front();

		if (ok)
		{
			this->boundary->add(fel);

			this->front->add(fel);

			this->mesh->add(fel);

			p->getSubfront()->add(fel);
		}
		else
		{
			delete fel;
		}
	}

	while (!newVertices.empty())
	{
		Data::Vertex *v = newVertices.front();

		newVertices.pop_front();

		if (ok)
		{
			this->boundary->add(v);

			this->front->add(v);

			this->mesh->add(v, false);

			p->getSubfront()->add(v);
		}
		else
		{
			delete v;
		}
	}
}

void PMJMesh::GlobalTaskManager::createInnerFront(PMJMesh::FrontPartitioner *fp)
{
	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

		/*UInt size = t->getFront() ? t->getFront()->size() : 0;

		if (size > 0)
		{
			continue;
		}*/

		Data::Position pos = PMJMesh::getPosition(this->dimension, this->tree, t->getId());

		if (pos != Data::IN)
		{
			continue;
		}

		PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(t->getPartition());

		fp->ensureExistenceOfSubfront(p);

		this->createInnerFront(p);
	}
}

void PMJMesh::GlobalTaskManager::backReplaceInnerFront()
{
#if USE_C__11
	for (PMJMesh::CrossingReplacer::FrontElementReplacementHashMap::iterator iter = this->createdInnerFels.begin();
#else
	for (PMJMesh::CrossingReplacer::FrontElementReplacementMap::iterator iter = this->createdInnerFels.begin();
#endif //#if USE_C__11
		 iter != this->createdInnerFels.end(); iter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter).second.original);

		//std::cout << "will replace back fel at " << fel << std::endl;
		//std::cout << "will replace back fel " << fel->getId() << std::endl;

		while (((*iter).second.temporaries) && (!(*iter).second.temporaries->empty()))
		{
			MJMesh::FrontElement *tmp = static_cast<MJMesh::FrontElement *>((*iter).second.temporaries->front());

			(*iter).second.temporaries->pop_front();

			//std::cout << "found tmp fel at " << tmp << std::endl;
			//std::cout << "found tmp fel " << tmp->getId() << std::endl;

			//Data::ElementList adjs = tmp->getAdjacency();

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = tmp->eBegin();
                 iter != tmp->eEnd(); iter = tmp->erase(iter))
			{
				//MJMesh::Element *adj = static_cast<MJMesh::Element *>(adjs.front());
				MJMesh::Element *adj = static_cast<MJMesh::Element *>((*iter));

				//adjs.pop_front();

				//std::cout << "removing adj el at " << adj << " from tmp and adding to fel" << std::endl;
				//std::cout << "removing adj el " << adj->getId() << " from tmp and adding to fel" << std::endl;

				//tmp->remove(adj);

				fel->add(adj);
			}

			while (!tmp->getData().boundaries.empty())
			{
				MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first));

				//std::cout << "removing tmp from boundary at " << front << " and adding fel to it" << std::endl;

				boundary->remove(tmp);
			}

			while (!tmp->getData().fronts.empty())
			{
				PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

				//std::cout << "removing tmp from front at " << front << " and adding fel to it" << std::endl;

				front->remove(tmp);
			}

			while (!tmp->getData().meshes.empty())
			{
				MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

				//std::cout << "removing tmp from mesh at " << mesh << " and adding fel to it" << std::endl;

				mesh->remove(tmp, false);
			}

			delete tmp;
		}

		if ((*iter).second.temporaries)
		{
			delete (*iter).second.temporaries;
		}

		this->boundary->remove(fel);

		if ((this->front) && (this->front->have(fel)))
		{
			this->front->remove(fel);
		}

		if (this->deleteFelsInInnerFront)
		{
			if ((this->mesh) && (this->mesh->have(fel)))
			{
				this->mesh->remove(fel);
			}

			delete fel;
		}
	}

	this->createdInnerFels.clear();

#if USE_C__11
	for (PMJMesh::CrossingReplacer::VertexReplacementHashMap::iterator iter = this->createdInnerVertices.begin();
#else
	for (PMJMesh::CrossingReplacer::VertexReplacementMap::iterator iter = this->createdInnerVertices.begin();
#endif //#if USE_C__11
		 iter != this->createdInnerVertices.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second.original);

		Real coord = 0.0;
		UInt displace = 0;

		//std::cout << "will replace back v at " << v << std::endl;
		//std::cout << "will replace back v " << v->getId() << std::endl;

		while (((*iter).second.temporaries) && (!(*iter).second.temporaries->empty()))
		{
			MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter).second.temporaries->front());

			(*iter).second.temporaries->pop_front();

			//std::cout << "found tmp v at " << tmp << std::endl;
			//std::cout << "found tmp v " << tmp->getId() << std::endl;

			//Data::FrontElementList fadjs = tmp->getAdjacentFrontElements();

			//while (!fadjs.empty())
			for (Data::FrontElementList::iterator iter = tmp->feBegin();
                 iter != tmp->feEnd(); iter = tmp->erase(iter))
			{
				//Data::FrontElement *adj = fadjs.front();
				Data::FrontElement *adj = (*iter);

				//fadjs.pop_front();

				//std::cout << "replacing tmp for v for adj fel at " << adj << std::endl;
				//std::cout << "replacing tmp for v for adj fel " << adj->getId() << std::endl;

				adj->replace(tmp, v);

				//tmp->remove(adj);

				v->add(adj);
			}

			//Data::ElementList adjs = tmp->getAdjacentElements();

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = tmp->eBegin();
                 iter != tmp->eEnd(); iter = tmp->erase(iter))
			{
				//Data::Element *adj = adjs.front();
				Data::Element *adj = (*iter);

				//adjs.pop_front();

				//std::cout << "replacing tmp for v for adj el at " << adj << std::endl;
				//std::cout << "replacing tmp for v for adj el " << adj->getId() << std::endl;

				adj->replace(tmp, v);

				//tmp->remove(adj);

				v->add(adj);
			}

			while (!tmp->getData().boundaries.empty())
			{
				MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first));

				//std::cout << "removing tmp from boundary at " << front << " and adding v to it" << std::endl;

				boundary->remove(tmp);
			}

			while (!tmp->getData().fronts.empty())
			{
				PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

				//std::cout << "removing tmp from front at " << front << " and adding v to it" << std::endl;

				front->remove(tmp);
			}

			while (!tmp->getData().meshes.empty())
			{
				MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

				//std::cout << "removing tmp from mesh at " << mesh << " and adding v to it" << std::endl;

				mesh->remove(tmp);
			}

			if (this->displaceVerticesInInnerFront)
			{
				coord += tmp->getPoint()->getCoord(1);

				displace++;
			}

			delete tmp;
		}

		if ((*iter).second.temporaries)
		{
			delete (*iter).second.temporaries;
		}

		this->boundary->remove(v);

		if ((this->front) && (this->front->have(v)))
		{
			this->front->remove(v);
		}

		this->mesh->addInner(v);

		if ((this->displaceVerticesInInnerFront) && (displace > 0))
		{
			v->getPoint()->setCoord(1, (v->getPoint()->getCoord(1) + coord)/static_cast<Real>(displace + 1));
		}
	}

	this->createdInnerVertices.clear();
}

PMJMesh::LoadCalculator *PMJMesh::GlobalTaskManager::makeLoadCalculator(bool initializing) const
{
	//*
	bool adjustInTree = ((this->partitionerType == PMJMesh::BOX_LOAD) || (this->partitionerType == PMJMesh::BOX_NUMBER)) ?
		initializing : false;
	bool adjustInBox = ((this->partitionerType == PMJMesh::BOX_LOAD) || (this->partitionerType == PMJMesh::BOX_NUMBER)) ?
		(!initializing) : true;

	return new PosterioriLoadCalculator(this->tree,
		this->considerOnCellsInLoad, this->heightDependentInLoad, this->dimension, this->minRefinementLevel,
		this->removeLayersInLoad, adjustInTree, adjustInBox);
	/*/
	return new LoadCalculator(this->tree, &this->position,
		this->considerOnCellsInLoad, this->heightDependentInLoad, this->dimension, this->minRefinementLevel);
	//*/
}

#if USE_C__11
TreePartitioner *PMJMesh::GlobalTaskManager::makeTreePartitioner(const Data::RealHashMap *load, PMJMesh::LoadCalculator *loadCalculator) const
#else
TreePartitioner *PMJMesh::GlobalTaskManager::makeTreePartitioner(const Data::RealMap *load, PMJMesh::LoadCalculator *loadCalculator) const
#endif //#if USE_C__11
{
	switch (this->partitionerType)
	{
	case PMJMesh::BOX_LOAD:
		return new BoundaryLoadTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(),
			loadCalculator, this->usePartitioningThreshold, this->partitioningThreshold);

	case PMJMesh::BOX_NUMBER:
		return new BoundaryNumberTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(),
			loadCalculator, this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold));

	case PMJMesh::BSP_NUMBER:
		return new BoundaryMinNumberBSPTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(),
			loadCalculator, this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold));

	case PMJMesh::SMART_BSP_NUMBER:
		return new BoundarySmartMinNumberBSPTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(),
			loadCalculator, this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold),
			this->removeLayersInLoad);
	}

	return NULL;
}

#if USE_NEW_FRONT_UPDATER
ActualFrontUpdater *PMJMesh::GlobalTaskManager::makeFrontUpdater() const
{
	return new ActualFrontUpdater(this->dimension, this->front, this->comm, false);
}

MeshUpdater *PMJMesh::GlobalTaskManager::makeMeshUpdater() const
{
	if (this->comm->isMaster())
	{
		switch (this->updateType)
		{
		case PMJMesh::FULL:
#if USE_DELETE_FELS
			return new MeshUpdater(this->dimension, this->mesh, this->comm, true);
#else
			return new MeshUpdater(this->dimension, this->mesh, this->comm, false);
#endif //#if USE_DELETE_FELS

		case PMJMesh::PARTIAL:
#if USE_DELETE_FELS
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
#else
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
#if USE_DELETE_FELS
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
#else
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::EMPTY:
#if USE_DELETE_FELS
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
#else
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
#if USE_DELETE_FELS
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
#else
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::FINAL:
#if USE_DELETE_FELS
			return new FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true);
#else
			return new FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false);
#endif //#if USE_DELETE_FELS
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
#if USE_DELETE_FELS
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
#else
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
#if USE_DELETE_FELS
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
#else
			return new PartialMeshUpdater(this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::EMPTY:
#if USE_DELETE_FELS
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, true);
#else
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
#if USE_DELETE_FELS
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, true);
#else
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true, false);
#endif //#if USE_DELETE_FELS
			break;

		case PMJMesh::FINAL:
#if USE_DELETE_FELS
			return new FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, true);
#else
			return new FinalMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false);
#endif //#if USE_DELETE_FELS
			break;
		}
	}

	return NULL;
}
#endif //#if USE_NEW_FRONT_UPDATER

FrontPartitioner *PMJMesh::GlobalTaskManager::makeFrontPartitioner() const
{
	return new PMJMesh::FrontPartitioner(this->dimension,
		this->front, this->tasks, this->shiftState(),
		(!this->comm->isParallel()) && (this->comm->numWorkers() > 1));
}

BoxTreeShifter *PMJMesh::GlobalTaskManager::makePartitionTreeShifter() const
{
	switch (this->partitionerType)
	{
	case PMJMesh::BOX_LOAD:
	case PMJMesh::BOX_NUMBER:
		return new PMJMesh::BoxTreeShifter(this->dimension, this->partitionTree, this->shiftType);
		//return new PMJMesh::BoxTreeShifter(this->dimension, this->partitionTree, this->shiftType, 0.9);
		//return new PMJMesh::BoxTreeShifter(this->dimension, this->partitionTree, this->shiftType, 0.5, 3);

		/*//shift to the side with smallest span
		{
			Data::Box *box = this->boundary->box();

			UInt dim = 0;
			Real size = REAL_MAX;

			for (UInt i = 0; i < box->dimension(); i++)
			{
				Real s = box->getMax()->getCoord(i) - box->getMin()->getCoord(i);

				if (s < size)
				{
					size = s;

					dim = i;
				}
			}

			delete box;

			this->shifter = new PMJMesh::BoxTreeShifter(this->dimension, this->partitionTree, 0.5, dim);
		}*/

	case PMJMesh::BSP_NUMBER:
	case PMJMesh::SMART_BSP_NUMBER:
		return new PMJMesh::BSPTreeShifter(this->dimension, this->partitionTree, this->shiftType);
	}

	return NULL;
}

void PMJMesh::GlobalTaskManager::splitMesh()
{
	//Real debugstart = Data::time();

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
			static_cast<EmptyMeshUpdater *>(this->meshUpdater)->splitMesh();
#else
			static_cast<EmptyFrontUpdater *>(this->frontUpdater)->splitMesh();
#endif //#if USE_NEW_FRONT_UPDATER
		}
	}*/

#if USE_NEW_FRONT_UPDATER
	if ((this->meshUpdater) && (this->updateType != PMJMesh::FULL))
#else
	if ((this->frontUpdater) && (this->updateType != PMJMesh::FULL))
#endif //#if USE_NEW_FRONT_UPDATER
	{
		if ((this->updateType == PMJMesh::EMPTY) ||
			(this->updateType == PMJMesh::EMPTY_NO_BOUNDARY) ||
			(this->updateType == PMJMesh::FINAL))
		{
#if USE_NEW_FRONT_UPDATER
			static_cast<PMJMesh::EmptyMeshUpdater *>(this->meshUpdater)->splitMesh();
#else
			static_cast<PMJMesh::EmptyFrontUpdater *>(this->frontUpdater)->splitMesh();
#endif //#if USE_NEW_FRONT_UPDATER
		}
	}

#if USE_DELETE_FELS
    if (this->deleteExtraFrontElements)
    {
        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin(), end = this->mesh->feEnd();
             iter != end;)
        {
            Data::FrontElement *fel = (*iter);

            if (this->boundary->have(fel))
            {
                ++iter;

                continue;
            }

            iter = this->mesh->erase(iter);

            delete fel;
        }
    }
#endif //#if USE_DELETE_FELS

	//std::cout << "Master, time for splitting mesh = " << Data::time() - debugstart << std::endl;
}

void PMJMesh::GlobalTaskManager::finalizeMesh()
{
#if USE_NEW_FRONT_UPDATER
	if ((this->updateType != PMJMesh::FULL) && (this->meshUpdater))
#else
	if ((this->updateType != PMJMesh::FULL) && (this->frontUpdater))
#endif //#if USE_NEW_FRONT_UPDATER
	{
		if ((this->comm->isMaster()) || (this->taskSent))
		{
#if USE_NEW_FRONT_UPDATER
			static_cast<PartialMeshUpdater *>(this->meshUpdater)->finalizeMesh();
#else
			static_cast<PartialFrontUpdater *>(this->frontUpdater)->finalizeMesh();
#endif //#if USE_NEW_FRONT_UPDATER
		}

		/*//debug
		if (this->comm->isMaster())
		{
			std::cout << "finalizing mesh" << std::endl;
			//this->mesh->test(this->boundary, this->front);
		}
		//endebug*/

		this->comm->finalize();
	}

	this->backReplaceInnerFront();
}

bool PMJMesh::GlobalTaskManager::setTaskParameters(Parallel::Task *t,
	MJMesh::Boundary *boundary, Data::BoxTree *tree,
	Performer::IdManager *idManager) const
{
	//this if is to deal with the tasks in the shared mode, because some of
	// them may not have front
	if (static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront())
	{
		MJMesh::Front *front = static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront();

		front->setSorted(this->sorted);
	}

	//if (this->comm->numProcesses() <= 1)
	{
		t->setStatus(Parallel::Task::ready);

		if (t->getBuilder())
		{
			//t->getBuilder()->setStatus(PMJMesh::GeometryFrontAdvancer::notInit);
			t->getBuilder()->setStatus(PMJMesh::WorkerMainDrive::notInit);
		}
	}

	/* //this is here just because it could happen in a future, and so I did not want to
	// remove these lines
	if ((this->t->getStatus().getType() != this->t->ready.getType()) &&
		(this->t->getStatus().getType() != this->t->running.getType()))
	{
		return false;
	}*/

	//PMJMesh::GeometryFrontAdvancer *geometryAdvancer = static_cast<PMJMesh::GeometryFrontAdvancer *>(t->getBuilder());
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	if (!drive)
	{
		//geometryAdvancer = new PMJMesh::GeometryFrontAdvancer(this->dimension, this->boundary, this->tree);
		//geometryAdvancer = new PMJMesh::GeometryFrontAdvancer(this->dimension, this->boundaries[id], this->tree);
		//geometryAdvancer = new PMJMesh::GeometryFrontAdvancer(this->dimension, this->boundaries[id], tree);
		//geometryAdvancer = new PMJMesh::GeometryFrontAdvancer(this->dimension, boundary, tree);
		//drive = new PMJMesh::WorkerMainDrive();

		t->makeBuilder();

		//geometryAdvancer = static_cast<PMJMesh::GeometryFrontAdvancer *>(t->getBuilder());
		drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

		drive->setDimension(this->dimension);
		drive->setBoundary(boundary);
		drive->setTree(tree);

		//t->setBuilder(geometryAdvancer);
		//t->setBuilder(drive);
	}
	else
	{
		//static_cast<PMJMesh::GeometryFrontAdvancer *>(t->getBuilder())->setBoundary(boundary);
		dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder())->setBoundary(boundary);

		if (this->comm->isParallel())
		{
			dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder())->setTree(tree);
		}
	}

	//geometryAdvancer->setUsingSearchTrees(this->usingSearchTrees);
	drive->setUsingSearchTrees(this->usingSearchTrees);
	drive->setRangeProportion(this->rangeProportion);

	drive->setIdManager(idManager);

	drive->setSmoothInSlavesEnabled(this->smoothInSlavesEnabled);
#if (!USE_DELETE_FELS)
	drive->setUnsmoothingLayer(this->unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
	drive->setNumImproves(this->numImproves);
	drive->setSmoothingFactor(this->smoothingFactor);
	drive->setLocalBackTrackingEnabled(this->localBackTrackingEnabled);
	drive->setMeshSorted(this->meshSorted);
	drive->setMaxBackTracks(this->maxBackTracks);
	drive->setMinRefinementLevel(this->minRefinementLevel);
#if (!USE_DELETE_FELS)
	drive->setRemoveBoundaryFelsForSmoothing(!this->comm->isParallel());
#endif //#if (!USE_DELETE_FELS)

	drive->setTestingBox(true);
	drive->setClassifyTreeCells(true);

	if (!t->getComm())
	{
		t->setComm(this->comm);
	}

	return true;
}

bool PMJMesh::GlobalTaskManager::isInError(const Parallel::Task *t) const
{
	//PMJMesh::GeometryFrontAdvancer *geometryAdvancer = static_cast<const PMJMesh::GeometryFrontAdvancer *>(t->getBuilder());
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	bool inError = (drive->getStatus().getType() >= drive->error.getType());

	if (inError)
	{
		std::stringstream str;
		str << "ERROR: Some error occurred in task " << t->getId() << ": " << drive->getStatus().getMessage() << std::endl;
		std::cout << str.str();

		//this->status = drive->getStatus();
		this->status = this->error;

		this->status.append(drive->getStatus());
	}

	return inError;
}

void PMJMesh::GlobalTaskManager::getParameters(const Parallel::Task *t) const
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	drive->setIdManager(NULL);
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PMJMesh::GlobalTaskManager::timing(const Parallel::Task *task, Real timeTotal) const
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

	Real timeTrees = drive->timeTrees;
	Real timeGeometry = drive->timeGeometry;
	Real timeTopology = drive->timeTopology;
	Real timeBackTracking = drive->timeBackTracking;
	Real timeClassify = drive->timeClassify;
	Real timeRefinement = drive->timeRefinement;
	Real timeImprover = drive->timeImprover;
	Real timeOverhead = timeTotal - (timeTrees + timeGeometry + timeTopology + timeBackTracking + timeClassify + timeRefinement + timeImprover);
	ULInt numVertices = drive->getMesh() ? drive->getMesh()->innerVerticesSize() : 0;
	ULInt numElements = drive->getMesh() ? drive->getMesh()->size() : 0;

	std::stringstream str;
	str << TaskManager::timing(task, timeTotal);
	str << "Tree Refiner and Disrefiner = " << timeRefinement << " (" << 100.0*timeRefinement/timeTotal << "%)\n";
	str << "Search Tree Builder and Destroyer = " << timeTrees << " (" << 100.0*timeTrees/timeTotal << "%)\n";
	str << "Geometry Front Advancer = " << timeGeometry << " (" << 100.0*timeGeometry/timeTotal << "%)\n";
	str << "Topology Front Advancer = " << timeTopology << " (" << 100.0*timeTopology/timeTotal << "%)\n";
	str << "Back-Tracking Front Advancer (+ finding common front) = " << timeBackTracking << " (" << 100.0*timeBackTracking/timeTotal << "%)\n";
	str << "Mesh Improver (+ inverting new front) = " << timeImprover << " (" << 100.0*timeImprover/timeTotal << "%)\n";
	str << "Tree Classifier = " << timeClassify << " (" << 100.0*timeClassify/timeTotal << "%)\n";
	str << "Overhead = " << timeOverhead << " (" << 100.0*timeOverhead/timeTotal << "%)\n";
	str << "Number of Vertices = " << numVertices << "\n";
	str << "Number of Elements = " << numElements << "\n";

#if DEBUG_TIMING_PMJMESH
	this->timeMeshGeneratorWorkerMeshGeneration += timeGeometry + timeTopology;
	if (this->dimension == 3) this->timeMeshGeneratorWorkerMeshGeneration += timeBackTracking;
	this->timeMeshGeneratorWorkerMeshImprovement += timeImprover;
	this->timeMeshGeneratorWorkerTreeClassification += timeClassify;
	this->timeMeshGeneratorWorkerOthers += timeTotal - timeGeometry - timeTopology - timeImprover - timeClassify;
	if (this->dimension == 3) this->timeMeshGeneratorWorkerOthers -= timeBackTracking;
#endif //#if DEBUG_TIMING_PMJMESH

	return str.str();
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
