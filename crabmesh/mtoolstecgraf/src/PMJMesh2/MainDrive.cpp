#include "PMJMesh2/MainDrive.h"

#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/PartitionBSPTreeGenerator.h"
#include "PMJMesh2/ParallelMeshGenerator.h"
#include "PMJMesh2/APrioriParallelMeshGenerator.h"
#include "PMJMesh2/InputData.h"
#include "PMJMesh2/GeometryBSPTreeGenerator.h"
#include "PMJMesh2/LoadBSPTreeGenerator.h"
#include "PMJMesh2/FileBSPTreeGenerator.h"
#include "PMJMesh2/MJMeshTask.h"
#include "PMJMesh/HotellingTransformer.h"
#include "PMJMesh/TreeGenerator.h"
#include "PMJMesh/MinTreeRefiner.h"
#include "PMJMesh/NeighborTreeRefiner.h"
#include "PMJMesh/BoundaryBuilder.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh/AngleTreeCellClassifier.h"
#include "Parallel/GreedySerialStaticLoadBalancer.h"
#include "Performer/IdManager.h"

#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Vertex.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

#include "Data/OStream.h"

#include "PMJMesh2/CellCounter.h"

using namespace PMJMesh2;

const Performer::Status PMJMesh2::MainDrive::errorPMJMesh2 =
	Performer::Status(PMJMesh2::MainDrive::ERROR_PJMESH2, "error - pjmesh2");

const Performer::Status PMJMesh2::MainDrive::loadCalculated =
	Performer::Status(PMJMesh2::MainDrive::LOAD_CALCULATED, "load calculated");
const Performer::Status PMJMesh2::MainDrive::partitionTreeGenerated =
	Performer::Status(PMJMesh2::MainDrive::PARTITION_TREE_GENERATED, "partition tree generated");
const Performer::Status PMJMesh2::MainDrive::meshGenerated =
	Performer::Status(PMJMesh2::MainDrive::MESH_GENERATED, "mesh generated");

PMJMesh2::MainDrive::MainDrive(Parallel::Communicator *comm) :
    MJMesh::MainDrive::MainDrive(),
    Parallel::MainDrive::MainDrive(comm)
{
    this->setNumThreads(0);
    this->setNumTasksPerThread(1);
    this->setHeightDependentInLoad(false);
    this->setRemoveLayersInLoad(1);
    this->setLoadThreshold(0.5);
    this->setPartitionerType(LOAD_BASED_BSP);
#if USE_FILE_BASED_BSP
    this->setPartitionBSPFileName("");
#endif //#if USE_FILE_BASED_BSP
    this->setInterfaceMeshGenerationType(A_POSTERIORI);
    this->setCreatingInnerFrontInDecomposition(true);
    this->setRangeProportion(1.0);
    this->setImprovementInSlavesEnabled(true);
    this->setImprovementLayers(1);
    this->setUpdateType(PARTIAL);
    this->setFinalMeshState(DISTRIBUTED);
    this->setAPrioriInterfaceElementSizeFactor(1.0);
    this->setAPrioriInterfaceBadAngleCos(0.95);
    this->setAPrioriInterfaceShiftDelta(2.0);

    if (!this->comm)
    {
        this->setComm(this->makeCommunicator());
    }

    //if (!this->comm->isInitialized())
    if (this->comm)
    {
        this->comm->init();
    }

    this->partitionTree = NULL;

    this->loadCalculator = NULL;
    this->partitionTreeGenerator = NULL;
    this->meshGenerator = NULL;

    this->elementsMap = NULL;

#if DEBUG_CELL_COUNTER
    this->onlyCount = false;
/*#if USE_GUI
    this->onlyCount = true;
#endif //#if USE_GUI*/
#endif //#if DEBUG_CELL_COUNTER
}

PMJMesh2::MainDrive::~MainDrive()
{
    this->clear();

	if (this->comm)
	{
#if (!USE_WORKER_FILE_LOADER)
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
#endif //#if (!USE_WORKER_FILE_LOADER)

		this->comm->finalize();
//#endif //#if USE_GUI
	}
}

void PMJMesh2::MainDrive::setNumThreads(UInt numThreads)
{
    this->numThreads = numThreads;
}

UInt PMJMesh2::MainDrive::getNumThreads() const
{
    if ((this->comm) && (this->numThreads == 0))
    {
        this->numThreads = static_cast<PMJMesh2::Communicator *>(this->comm)->getMaxThreads();
    }

    return this->numThreads;
}

void PMJMesh2::MainDrive::setNumTasksPerThread(UInt numTasksPerThread)
{
    this->numTasksPerThread = (numTasksPerThread == 0) ? 1 : numTasksPerThread;
}

UInt PMJMesh2::MainDrive::getNumTasksPerThread() const
{
    return this->numTasksPerThread;
}

void PMJMesh2::MainDrive::setMinNumTasks(UInt minNumTasks)
{
    UInt numWorkers = this->getNumThreads()*static_cast<PMJMesh2::Communicator *>(this->comm)->numProcesses();

    this->setNumTasksPerThread(static_cast<UInt>(std::ceil(static_cast<double>(minNumTasks)/static_cast<double>(numWorkers))));
}

void PMJMesh2::MainDrive::setHeightDependentInLoad(bool heightDependentInLoad)
{
    this->heightDependentInLoad = heightDependentInLoad;
}

bool PMJMesh2::MainDrive::isHeightDependentInLoad() const
{
    return this->heightDependentInLoad;
}

void PMJMesh2::MainDrive::setRemoveLayersInLoad(UInt removeLayersInLoad)
{
    this->removeLayersInLoad = removeLayersInLoad;

#if USE_REMOVED_LAYERS_PROPORTIONS
    this->removedLayersProportions.resize(this->removeLayersInLoad, 1.0);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
}

UInt PMJMesh2::MainDrive::getRemoveLayersInLoad() const
{
    return this->removeLayersInLoad;
}

#if USE_REMOVED_LAYERS_PROPORTIONS
void PMJMesh2::MainDrive::setRemovedLayersProportions(UInt i, Real proportion)
{
    this->removedLayersProportions.resize(std::max(i, this->removeLayersInLoad), 1.0);

    this->removedLayersProportions[i] = proportion;
}

Real PMJMesh2::MainDrive::getRemovedLayersProportions(UInt i) const
{
    return this->removedLayersProportions[i];
}

void PMJMesh2::MainDrive::setRemovedLayersProportions(const Data::RealVector &removedLayersProportions)
{
    this->removedLayersProportions = removedLayersProportions;
}

const Data::RealVector &PMJMesh2::MainDrive::getRemovedLayersProportions() const
{
    return this->removedLayersProportions;
}
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

void PMJMesh2::MainDrive::setLoadThreshold(Real loadThreshold)
{
    this->loadThreshold = loadThreshold;
}

Real PMJMesh2::MainDrive::getLoadThreshold() const
{
    return this->loadThreshold;
}

void PMJMesh2::MainDrive::setPartitionerType(enum PMJMesh2::PartitionerType partitionerType)
{
    this->partitionerType = partitionerType;
}

enum PMJMesh2::PartitionerType PMJMesh2::MainDrive::getPartitionerType()
{
    return this->partitionerType;
}

#if USE_FILE_BASED_BSP
void PMJMesh2::MainDrive::setPartitionBSPFileName(const std::string &partitionBSPFileName)
{
    this->partitionBSPFileName = partitionBSPFileName;
}

std::string PMJMesh2::MainDrive::getPartitionBSPFileName() const
{
    return this->partitionBSPFileName;
}
#endif //#if USE_FILE_BASED_BSP

void PMJMesh2::MainDrive::setInterfaceMeshGenerationType(enum PMJMesh2::InterfaceMeshGenerationType interfaceMeshGenerationType)
{
    this->interfaceMeshGenerationType = interfaceMeshGenerationType;
}

enum PMJMesh2::InterfaceMeshGenerationType PMJMesh2::MainDrive::getInterfaceMeshGenerationType()
{
    return this->interfaceMeshGenerationType;
}

void PMJMesh2::MainDrive::setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition)
{
    this->creatingInnerFrontInDecomposition = creatingInnerFrontInDecomposition;
}

bool PMJMesh2::MainDrive::isCreatingInnerFrontInDecomposition() const
{
    return this->creatingInnerFrontInDecomposition;
}

void PMJMesh2::MainDrive::setRangeProportion(Real rangeProportion)
{
    this->rangeProportion = rangeProportion;
}

Real PMJMesh2::MainDrive::getRangeProportion() const
{
    return this->rangeProportion;
}

void PMJMesh2::MainDrive::setImprovementInSlavesEnabled(bool improvementInSlavesEnabled)
{
    this->improvementInSlavesEnabled = improvementInSlavesEnabled;
}

bool PMJMesh2::MainDrive::isImprovementInSlavesEnabled() const
{
    return this->improvementInSlavesEnabled;
}

void PMJMesh2::MainDrive::setImprovementLayers(UInt improvementLayers)
{
    this->improvementLayers = improvementLayers;
}

UInt PMJMesh2::MainDrive::getImprovementLayers() const
{
    return this->improvementLayers;
}

void PMJMesh2::MainDrive::setUpdateType(enum PMJMesh2::MeshUpdateType updateType)
{
    this->updateType = updateType;
}

enum PMJMesh2::MeshUpdateType PMJMesh2::MainDrive::getUpdateType()
{
    return this->updateType;
}

void PMJMesh2::MainDrive::setFinalMeshState(enum FinalMeshState finalMeshState)
{
    this->finalMeshState = finalMeshState;
}

enum FinalMeshState PMJMesh2::MainDrive::getFinalMeshState() const
{
    return this->finalMeshState;
}

void PMJMesh2::MainDrive::setAPrioriInterfaceElementSizeFactor(Real aPrioriInterfaceElementSizeFactor)
{
    this->aPrioriInterfaceElementSizeFactor = aPrioriInterfaceElementSizeFactor;
}

Real PMJMesh2::MainDrive::getAPrioriInterfaceElementSizeFactor() const
{
    return this->aPrioriInterfaceElementSizeFactor;
}

void PMJMesh2::MainDrive::setAPrioriInterfaceBadAngleCos(Real aPrioriInterfaceBadAngleCos)
{
    this->aPrioriInterfaceBadAngleCos = aPrioriInterfaceBadAngleCos;
}

Real PMJMesh2::MainDrive::getAPrioriInterfaceBadAngleCos() const
{
    return this->aPrioriInterfaceBadAngleCos;
}

void PMJMesh2::MainDrive::setAPrioriInterfaceShiftDelta(Real aPrioriInterfaceShiftDelta)
{
    this->aPrioriInterfaceShiftDelta = aPrioriInterfaceShiftDelta;
}

Real PMJMesh2::MainDrive::getAPrioriInterfaceShiftDelta() const
{
    return this->aPrioriInterfaceShiftDelta;
}

const PartitionBSPTree *PMJMesh2::MainDrive::getPartitionBSPTree() const
{
    return this->partitionTree;
}

const Parallel::TaskVector &PMJMesh2::MainDrive::getTasks() const
{
    return this->tasks;
}

#if DEBUG_CELL_COUNTER
void PMJMesh2::MainDrive::setOnlyCount(bool onlyCount)
{
    this->onlyCount = onlyCount;
}
#endif //#if DEBUG_CELL_COUNTER

bool PMJMesh2::MainDrive::loadFile()
{
#if (!USE_WORKER_FILE_LOADER)
	if (this->comm->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
		return MJMesh::MainDrive::loadFile();
	}

	return false;
}

bool PMJMesh2::MainDrive::saveFile() const
{
    if (this->comm->isMaster())
	{
		return MJMesh::MainDrive::saveFile();
	}

	return false;
}

void PMJMesh2::MainDrive::clear()
{
    if (this->loadCalculator)
	{
		delete this->loadCalculator;

		this->loadCalculator = NULL;
	}

	if (this->partitionTreeGenerator)
	{
		delete this->partitionTreeGenerator;

		this->partitionTreeGenerator = NULL;
	}

	if (this->meshGenerator)
	{
		delete this->meshGenerator;

		this->meshGenerator = NULL;
	}

	if (this->partitionTree)
	{
		delete this->partitionTree;

		this->partitionTree = NULL;
	}

	for (UInt i = 0; i < this->tasks.size(); i++)
    {
        static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition())->setBox(NULL);

        delete this->tasks[i];
    }

    this->tasks.clear();

	if (this->boundary)
	{
		if (this->comm->isSlave())
		{
			if (this->front)
			{
				this->front->clear();
			}

			if (this->mesh)
            {
                for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
                     iter != this->boundary->vEnd(); iter++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

                    v->clearAdjacentFrontElements();
                    v->clearAdjacentElements();

                    this->mesh->remove(v);
                }

                for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
                     iter != this->boundary->eEnd(); iter++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    fel->clearAdjacency();

                    this->mesh->remove(fel);
                }

                delete this->mesh;

                this->mesh = NULL;
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
		}

		if ((this->comm->isMaster()) && (this->comm->numProcesses() > 1) && (this->finalMeshState != JOINED) && (this->mesh))
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
                 iter != this->boundary->vEnd(); iter++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

                if (!this->mesh->have(v))
                {
                    this->mesh->add(v);
                }
            }

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
                 iter != this->boundary->eEnd(); iter++)
            {
                MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                if (!this->mesh->have(fel))
                {
                    this->mesh->add(fel);
                }
            }
        }

		if ((this->comm->isSlave()) || (this->front) || (this->mesh))
		{
			this->boundary->clear();
		}
	}

	MJMesh::MainDrive::clear();
}

#if (!USE_WORKER_FILE_LOADER)
InputData *PMJMesh2::MainDrive::makeInputData() const
{
	return new InputData();
}

InputData *PMJMesh2::MainDrive::makeAndFillInputData() const
{
	InputData *input = this->makeInputData();

	input->setInputFileName(this->inputFileName);

	input->setDimension(this->dimension);

	input->setIdVertex(this->idManager ? this->idManager->getId(0) : 1);
	input->setIdFrontElement(this->idManager ? this->idManager->getId(1) : 1);
	input->setIdElement(this->idManager ? this->idManager->getId(2) : 1);

	input->setBoundarySorted(this->boundarySorted);
#if USE_NUMERICAL
	input->setHotellingTransformEnabled(this->hotellingTransformEnabled);
#endif //#if USE_NUMERICAL
	input->setTreeFactor(this->treeFactor);
	input->setUsingSearchTrees(this->usingSearchTrees);
	input->setNumImproves(this->numImproves);
	input->setSmoothingFactor(this->smoothingFactor);
	input->setMeshSorted(this->meshSorted);
	input->setLocalBackTrackingEnabled(this->localBackTrackingEnabled);
	input->setMaxBackTracks(this->maxBackTracks);

    input->setNumThreads(this->numThreads);
    input->setNumTasksPerThread(this->numTasksPerThread);
    input->setHeightDependentInLoad(this->heightDependentInLoad);
    input->setRemoveLayersInLoad(this->removeLayersInLoad);
#if USE_REMOVED_LAYERS_PROPORTIONS
    input->setRemovedLayersProportions(this->removedLayersProportions);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    input->setLoadThreshold(this->loadThreshold);
    input->setInterfaceBadAngleCos(this->aPrioriInterfaceBadAngleCos);
    input->setInterfaceShiftDelta(this->aPrioriInterfaceShiftDelta);
    input->setPartitionerType(this->partitionerType);
#if USE_FILE_BASED_BSP
    input->setPartitionBSPFileName(this->partitionBSPFileName);
#endif //#if USE_FILE_BASED_BSP
    input->setInterfaceMeshGenerationType(this->interfaceMeshGenerationType);
    input->setInterfaceElementSizeFactor(this->aPrioriInterfaceElementSizeFactor);
    input->setSharedParallelismEnabled(static_cast<PMJMesh2::Communicator *>(this->comm)->isSharedParallelismEnabled());
    input->setCreatingInnerFrontInDecomposition(this->creatingInnerFrontInDecomposition);
	input->setRangeProportion(this->rangeProportion);
	input->setImprovementInSlavesEnabled(this->improvementInSlavesEnabled);
	input->setImprovementLayers(this->improvementLayers);
	input->setUpdateType(this->updateType);
	input->setFinalMeshState(this->finalMeshState);

	input->setTolerance(Data::getTolerance());

#if USE_GUI
	input->setStepByStep(Data::isStepByStep());
#endif //#if USE_GUI

	return input;
}

void PMJMesh2::MainDrive::fillWithInputData(const InputData *input)
{
	this->inputFileName = input->getInputFileName();

	MJMesh::MainDrive::loadFile();

	this->dimension = input->getDimension();

	if (this->idManager)
	{
		delete this->idManager;
	}

	this->idManager = this->makeIdManager(NULL);

	this->idManager->setId(0, input->getIdVertex());
	this->idManager->setId(1, input->getIdFrontElement());
	this->idManager->setId(2, input->getIdElement());

	this->setBoundarySorted(input->isBoundarySorted());
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(input->isHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
	this->setTreeFactor(input->getTreeFactor());
	this->setUsingSearchTrees(input->isUsingSearchTrees());
	this->setNumImproves(input->getNumImproves());
	this->setSmoothingFactor(input->getSmoothingFactor());
	this->setMeshSorted(input->isMeshSorted());
	this->setLocalBackTrackingEnabled(input->isLocalBackTrackingEnabled());
	this->setMaxBackTracks(input->getMaxBackTracks());

    this->setNumThreads(input->getNumThreads());
    this->setNumTasksPerThread(input->getNumTasksPerThread());
    this->setHeightDependentInLoad(input->isHeightDependentInLoad());
    this->setRemoveLayersInLoad(input->getRemoveLayersInLoad());
#if USE_REMOVED_LAYERS_PROPORTIONS
    this->setRemovedLayersProportions(input->getRemovedLayersProportions());
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    this->setLoadThreshold(input->getLoadThreshold());
    this->setAPrioriInterfaceBadAngleCos(input->getInterfaceBadAngleCos());
    this->setAPrioriInterfaceShiftDelta(input->getInterfaceShiftDelta());
    this->setPartitionerType(input->getPartitionerType());
#if USE_FILE_BASED_BSP
    this->setPartitionBSPFileName(input->getPartitionBSPFileName());
#endif //#if USE_FILE_BASED_BSP
    this->setInterfaceMeshGenerationType(input->getInterfaceMeshGenerationType());
    this->setAPrioriInterfaceElementSizeFactor(input->getInterfaceElementSizeFactor());
    static_cast<PMJMesh2::Communicator *>(this->comm)->setSharedParallelismEnabled(input->isSharedParallelismEnabled());
    this->setCreatingInnerFrontInDecomposition(input->isCreatingInnerFrontInDecomposition());
	this->setRangeProportion(input->getRangeProportion());
	this->setImprovementInSlavesEnabled(input->isImprovementInSlavesEnabled());
	this->setImprovementLayers(input->getImprovementLayers());
	this->setUpdateType(input->getUpdateType());
	this->setFinalMeshState(input->getFinalMeshState());

	Data::setTolerance(input->getTolerance());

#if USE_GUI
	//debug
	//Data::setStepByStep(input->isStepByStep());
	Data::setStepByStep(false);
	//endebug
#endif //#if USE_GUI
}

Performer::BoundaryBuilder *PMJMesh2::MainDrive::makeBoundaryBuilder() const
{
	return new PMJMesh::BoundaryBuilder(this->comm);
}
#endif //#if (!USE_WORKER_FILE_LOADER)

Parallel::Communicator *PMJMesh2::MainDrive::makeCommunicator() const
{
    return new PMJMesh2::Communicator();
}

Parallel::LoadBalancer *PMJMesh2::MainDrive::makeLoadBalancer() const
{
    return new Parallel::GreedySerialStaticLoadBalancer(this->comm);
}

Performer::TreeGenerator *PMJMesh2::MainDrive::makeTreeGenerator() const
{
    return new PMJMesh::TreeGenerator(this->dimension, this->boundary, this->treeFactor);
}

#if USE_NUMERICAL
MJMesh::HotellingTransformer *PMJMesh2::MainDrive::makeHotellingTransformer() const
{
    PMJMesh::HotellingTransformer *hotellingTransformer = new PMJMesh::HotellingTransformer();

    hotellingTransformer->setBoundary(this->boundary);
    hotellingTransformer->setMode(Performer::CovarianceMatrixFinder::Mode::NORMAL);
    hotellingTransformer->setFrontElementCentroids(false);
    hotellingTransformer->setWeighted(false);

    return hotellingTransformer;
}
#endif //#if USE_NUMERICAL

MJMesh::MinTreeRefiner *PMJMesh2::MainDrive::makeTreeMinRefiner() const
{
    PMJMesh::MinTreeRefiner *refiner = new PMJMesh::MinTreeRefiner(this->dimension, this->tree, this->boundary, this->minTreeLevel, false, NULL);

    refiner->setElementsMap(this->elementsMap);

    return refiner;
}

MJMesh::NeighborTreeRefiner *PMJMesh2::MainDrive::makeTreeNeighborRefiner() const
{
    PMJMesh::NeighborTreeRefiner *refiner = new PMJMesh::NeighborTreeRefiner(this->dimension, this->tree, this->boundary);

    refiner->setElementsMap(this->elementsMap);

    return refiner;
}

PMJMesh::LoadCalculator *PMJMesh2::MainDrive::makeLoadCalculator() const
{
    return new PMJMesh::LoadCalculator(this->tree, true, this->heightDependentInLoad, this->dimension, 0);
}

PartitionBSPTreeGenerator *PMJMesh2::MainDrive::makePartitionTreeGenerator() const
{
    PartitionBSPTreeGenerator *partitioner = NULL;

    switch (this->partitionerType)
    {
    case GEOMETRY_BASED_BSP:
        partitioner = new GeometryBSPTreeGenerator();
        break;

    case LOAD_BASED_BSP:
        partitioner = new LoadBSPTreeGenerator();
        break;

#if USE_FILE_BASED_BSP
    case FILE_BASED_BSP:
        partitioner = new FileBSPTreeGenerator();
        break;
#endif //#if USE_FILE_BASED_BSP
    }

    if (partitioner)
    {
        partitioner->setDimension(this->dimension);
        partitioner->setComm(this->comm);
        partitioner->setNumTasksPerThread(this->numTasksPerThread);
        partitioner->setBoxTree(this->tree);
        partitioner->setRemoveLayersInLoad(this->removeLayersInLoad);
#if USE_REMOVED_LAYERS_PROPORTIONS
        partitioner->setRemovedLayersProportions(this->removedLayersProportions);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
        partitioner->setLoadCalculator(this->loadCalculator);

        if (this->partitionerType == LOAD_BASED_BSP)
        {
            static_cast<LoadBSPTreeGenerator *>(partitioner)->setCreatingInnerFrontInDecomposition(this->creatingInnerFrontInDecomposition);
            static_cast<LoadBSPTreeGenerator *>(partitioner)->setProportional(true);
            static_cast<LoadBSPTreeGenerator *>(partitioner)->setLoadThreshold(this->loadThreshold);

#if USE_TECGRAF_MSH_SURF
            if (this->interfaceMeshGenerationType == A_PRIORI)
            {
                static_cast<LoadBSPTreeGenerator *>(partitioner)->setUsingFrontTest(true);
                static_cast<LoadBSPTreeGenerator *>(partitioner)->setBoundary(this->boundary);

                static_cast<LoadBSPTreeGenerator *>(partitioner)->setInterfaceBadAngleCos(this->aPrioriInterfaceBadAngleCos);
                static_cast<LoadBSPTreeGenerator *>(partitioner)->setInterfaceShiftDelta(this->aPrioriInterfaceShiftDelta);
            }
#endif //#if USE_TECGRAF_MSH_SURF
        }
#if USE_FILE_BASED_BSP
        else if (this->partitionerType == FILE_BASED_BSP)
        {
            static_cast<FileBSPTreeGenerator *>(partitioner)->setBSPFileName(this->partitionBSPFileName);
        }
#endif //#if USE_FILE_BASED_BSP
    }

    return partitioner;
}

ParallelMeshGenerator *PMJMesh2::MainDrive::makeMeshGenerator() const
{
    ParallelMeshGenerator *generator = NULL;
#if USE_TECGRAF_MSH_SURF
    APrioriParallelMeshGenerator *aprGenerator = NULL;
#endif //#if USE_TECGRAF_MSH_SURF

    switch (this->interfaceMeshGenerationType)
    {
    case A_POSTERIORI:
        generator = new ParallelMeshGenerator();
        break;

#if USE_TECGRAF_MSH_SURF
    case A_PRIORI:
        aprGenerator = new APrioriParallelMeshGenerator();
        generator = aprGenerator;
        break;

    case A_PRIORI_INTERNAL:
        //aprGenerator = new APrioriParallelMeshGenerator();
        //generator = aprGenerator;
        break;
#endif //#if USE_TECGRAF_MSH_SURF
    }

    if (generator)
    {
        generator->setDimension(this->dimension);
        generator->setBoundarySorted(this->boundarySorted);
        generator->setNumImproves(this->numImproves);
        generator->setSmoothingFactor(this->smoothingFactor);
        generator->setLocakBackTrackingEnabled(this->localBackTrackingEnabled);
        generator->setMeshSorted(this->meshSorted);
        generator->setMaxBackTracks(this->maxBackTracks);

        generator->setNumTasksPerThread(this->numTasksPerThread);
        generator->setCreateInnerFrontInDecomposition(this->creatingInnerFrontInDecomposition);
        generator->setRangeProportion(this->rangeProportion);
        generator->setImprovementInSlavesEnabled(this->improvementInSlavesEnabled);
        generator->setImprovementLayers((this->improvementLayers > 0) ? this->improvementLayers - 1 : 0);
        generator->setUpdateType(this->updateType);
        generator->setFinalMeshState(this->finalMeshState);

        generator->setComm(this->comm);
        generator->setIdManager(this->idManager);

        generator->setBoundary(static_cast<MJMesh::Boundary *>(this->boundary));
        generator->setTree(this->tree);
        generator->setBsp(this->partitionTree);

        generator->setTasks(this->tasks);

        if (this->interfaceMeshGenerationType != A_POSTERIORI)
        {
#if USE_TECGRAF_MSH_SURF
            aprGenerator->setInterfaceElementSizeFactor(this->aPrioriInterfaceElementSizeFactor);
#endif //#if USE_TECGRAF_MSH_SURF
        }
    }

    return generator;
}

PMJMesh::MJMeshTask *PMJMesh2::MainDrive::makeTask(PMJMesh::Partition *p, PartitionBSPTreeCell *cell) const
{
    PMJMesh2::MJMeshTask *task = new PMJMesh2::MJMeshTask();

    task->setPartition(p);
    task->setComm(this->comm);
    task->makeBuilder();

    task->setCell(cell);

    return task;
}

void PMJMesh2::MainDrive::execute()
{
    if ((this->status == this->success) ||
		(this->status >= this->error))
	{
		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#if DEBUG_PMJMESH2_PROFILE
	Real prevruntime = 0.0;
#endif //#if DEBUG_PMJMESH2_PROFILE
#endif //#if USE_TIME_MEASUREMENTS

	switch (this->status.getType())
	{
	case MJMesh::MainDrive::NOT_INIT:
		if (!this->executeInit())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime = Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

/*#if USE_WORKER_FILE_LOADER
		if (this->comm->rank() != 0)
		{
            while (true);
        }
#endif //#if USE_WORKER_FILE_LOADER*/

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime = Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeInit() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	case MJMesh::MainDrive::INIT:

		if (!this->executeBoundaryBuilder(true))
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

/*#if (!USE_WORKER_FILE_LOADER)
		if ((this->comm->rank() != 6) && (this->comm->rank() != 7))
		{
            while (true);
        }
#endif //#if (!USE_WORKER_FILE_LOADER)*/

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeBoundaryBuilder() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	case MJMesh::MainDrive::BOUNDARY_BUILT:
#if USE_NUMERICAL
		//begin hotelling transform
        if (!this->executeHotellingTransform())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeHotellingTransform() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling transform
    case MJMesh::MainDrive::HOTELLING_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL

		if (!this->executeTreeGenerator(true))
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeTreeGenerator() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_GENERATED:
	case MJMesh::MainDrive::TREE_MIN_REFINED:
    case MJMesh::MainDrive::TREE_NEIGHBOR_REFINED:

#if USE_GUI
		static_cast<PMJMesh2::Communicator *>(this->comm)->setMaxThreads(this->numThreads);
#endif //#if USE_GUI

		if (!this->executeLoadCalculator())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeLoadCalculator() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

/*#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI*/

	case PMJMesh2::MainDrive::LOAD_CALCULATED:

		if (!this->executePartitionTreeGenerator())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executePartitionTreeGenerator() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

/*#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI*/

	case PMJMesh2::MainDrive::PARTITION_TREE_GENERATED:

#if USE_GUI
		static_cast<PMJMesh2::Communicator *>(this->comm)->setMaxThreads(this->numThreads);
#endif //#if USE_GUI

		if (!this->executeParallelMeshGenerator())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeParallelMeshGenerator() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh2::MainDrive::MESH_GENERATED:
#if USE_NUMERICAL
        //begin hotelling back transform
        if (!this->executeHotellingBackTransform())
		{
			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeHotellingBackTransform() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE

        if (this->comm->isMaster())
        {
            std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
        }

        start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling back transform
	case MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL

#if USE_GUI
		static_cast<PMJMesh2::Communicator *>(this->comm)->setMaxThreads(this->numThreads);
#endif //#if USE_GUI

		if (!this->executeFinalize())
		{
#if USE_TIME_MEASUREMENTS
            this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

			break;
		}

#if USE_TIME_MEASUREMENTS
#if DEBUG_PMJMESH2_PROFILE
        prevruntime = this->runtime;
#endif //#if DEBUG_PMJMESH2_PROFILE
        this->runtime += Data::time() - start;
#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) std::cout << "Time for executeFinalize() = " << this->runtime - prevruntime << std::endl;
#endif //#if DEBUG_PMJMESH2_PROFILE
#endif //#if USE_TIME_MEASUREMENTS
	}
}

bool PMJMesh2::MainDrive::executeInit()
{
    if (!this->comm)
    {
        this->comm = this->makeCommunicator();
    }

    if (!this->improvementInSlavesEnabled)
    {
        this->updateType = EMPTY;
        this->improvementLayers = 0;
    }

    if (this->numThreads == 0)
    {
        this->numThreads = static_cast<PMJMesh2::Communicator *>(this->comm)->getMaxThreads();
    }
    else
    {
        static_cast<PMJMesh2::Communicator *>(this->comm)->setMaxThreads(this->numThreads);
    }

#if USE_POOL
	PMJMesh::initThreadPools(static_cast<PMJMesh2::Communicator *>(this->comm));
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

    return MJMesh::MainDrive::executeInit();
}

bool PMJMesh2::MainDrive::executeBoundaryBuilder(bool del)
{
#if (!USE_WORKER_FILE_LOADER)
	static_cast<PMJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setDimension(this->dimension);
#endif //#if (!USE_WORKER_FILE_LOADER)

	if (this->idManager)
	{
		delete this->idManager;

		this->idManager = NULL;
	}

	return MJMesh::MainDrive::executeBoundaryBuilder(del);
}

bool PMJMesh2::MainDrive::executeTreeGenerator(bool del)
{
	//debug
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI
	//endebug

	bool result = MJMesh::MainDrive::executeTreeGenerator(false);

	if (this->treeGenerator->getStatus().getType() == dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->ended.getType())
	{
		if (result)
		{
			this->elementsMap = dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->getElementsMap();
			dynamic_cast<PMJMesh::TreeGenerator *>(this->treeGenerator)->setElementsMap(NULL);
		}

		if (del)
		{
			delete this->treeGenerator;

			this->treeGenerator = NULL;
		}
	}

	if (!result)
    {
	//debug
#if USE_GUI
        Data::setStepByStep(step);
#endif //#if USE_GUI
	//endebug

        return result;
    }

	result = MJMesh::MainDrive::executeTreeMinRefiner(false);

	if ((this->minTreeRefiner) &&
		(this->minTreeRefiner->getStatus().getType() == this->minTreeRefiner->ended.getType()))
	{
		if (result)
		{
			static_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->setElementsMap(NULL);
		}

		if (del)
		{
			delete this->minTreeRefiner;

			this->minTreeRefiner = NULL;
		}
	}

	if (!result)
    {
	//debug
#if USE_GUI
        Data::setStepByStep(step);
#endif //#if USE_GUI
	//endebug

        return result;
    }

	result = MJMesh::MainDrive::executeTreeNeighborRefiner(false);

	if (this->neighborRefiner->getStatus().getType() == this->neighborRefiner->ended.getType())
	{
#if DEBUG_CELL_COUNTER
        if (this->onlyCount)
        {
            static_cast<PMJMesh::NeighborTreeRefiner *>(this->neighborRefiner)->setElementsMap(NULL);
        }
#endif //#if DEBUG_CELL_COUNTER

		if (del)
		{
			delete this->neighborRefiner;

			this->neighborRefiner = NULL;
		}
	}

#if DEBUG_CELL_COUNTER
    if (!this->onlyCount)
    {
#endif //#if DEBUG_CELL_COUNTER
        this->elementsMap = NULL;
#if DEBUG_CELL_COUNTER
    }
#endif //#if DEBUG_CELL_COUNTER

//debug
#if USE_GUI
    Data::setStepByStep(step);
#endif //#if USE_GUI
	//endebug

	return result;
}

bool PMJMesh2::MainDrive::executeLoadCalculator()
{
    if (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())) == Data::UNKNOWN)
	{
		PMJMesh::AngleTreeCellClassifier *atcc = new PMJMesh::AngleTreeCellClassifier(
			this->dimension, static_cast<PMJMesh2::Communicator *>(this->comm), this->boundary, this->tree);

		atcc->execute();

		delete atcc;
	}

    this->loadCalculator = this->makeLoadCalculator();

	this->loadCalculator->execute();

	this->status = this->loadCalculated;

	return true;
}

bool PMJMesh2::MainDrive::executePartitionTreeGenerator()
{
    this->partitionTreeGenerator = this->makePartitionTreeGenerator();

    if (!this->partitionTreeGenerator)
    {
        this->status = this->error;

        return false;
    }

    this->partitionTreeGenerator->execute();

    this->partitionTree = this->partitionTreeGenerator->getBsp();

    Parallel::PartitionVector partitions = this->partitionTreeGenerator->getPartitions();

    this->tasks.assign(partitions.size(), NULL);

    for (UInt i = 0; i < static_cast<UInt>(partitions.size()); i++)
    {
        PartitionBSPTreeCell *cell = static_cast<PartitionBSPTreeCell *>(this->partitionTree->node(partitions[i]->getId()));

        this->tasks[i] = this->makeTask(static_cast<PMJMesh::Partition *>(partitions[i]), cell);

        cell->setTask(this->tasks[i]);
    }

    delete this->partitionTreeGenerator;

    this->partitionTreeGenerator = NULL;

    delete this->loadCalculator;

    this->loadCalculator = NULL;

    this->status = this->partitionTreeGenerated;

    return true;
}

bool PMJMesh2::MainDrive::executeParallelMeshGenerator()
{
#if DEBUG_CELL_COUNTER
    if (this->onlyCount)
    {
        CellCounter counter;

        counter.setDimension(this->dimension);
        counter.setBSP(this->partitionTree);
        counter.setTree(this->tree);
        counter.setBoundary(static_cast<MJMesh::Boundary *>(this->boundary));
        counter.setNumLayers(this->removeLayersInLoad);
        //counter.setNumLayers(3);
        counter.setTasks(this->tasks);
        counter.setElementsMap(this->elementsMap);

        this->elementsMap = NULL;

        counter.execute();

        this->status = this->meshGenerated;

        return true;
    }
#endif //#if DEBUG_CELL_COUNTER

    if (!this->meshGenerator)
    {
        this->meshGenerator = this->makeMeshGenerator();
    }

    this->meshGenerator->execute();

    //if (!this->front)
    {
        this->front = static_cast<MJMesh::Front *>(this->meshGenerator->getFront());
    }

    //if (!this->mesh)
    {
        this->mesh = static_cast<MJMesh::Mesh *>(this->meshGenerator->getMesh());
    }

    if (this->meshGenerator->getStatus() == this->meshGenerator->success)
    {
        //delete this->meshGenerator;

        //this->meshGenerator = NULL;

        this->status = this->meshGenerated;
    }
    else if (this->meshGenerator->getStatus() >= this->meshGenerator->error)
    {
        //delete this->meshGenerator;

        //this->meshGenerator = NULL;

        this->status = this->meshGenerator->getStatus();
    }

    return true;
}

#if USE_NUMERICAL
bool PMJMesh2::MainDrive::executeHotellingBackTransform()
{
    if (!this->hotellingTransformEnabled)
    {
        this->status = MJMesh::MainDrive::hotellingBackTransformApplied;

        return true;
    }

    this->meshGenerator->executeHotellingBackTransform(static_cast<PMJMesh::HotellingTransformer *>(this->hotellingTransformer));

    return MJMesh::MainDrive::executeHotellingBackTransform();
}
#endif //#if USE_NUMERICAL

bool PMJMesh2::MainDrive::executeFinalize()
{
#if DEBUG_CELL_COUNTER
    if (this->onlyCount)
    {
        this->status = this->success;

        return true;
    }
#endif //#if DEBUG_CELL_COUNTER

    if ((this->comm->isMaster()) &&
        (this->status == PMJMesh2::ParallelMeshGenerator::errorMeshNotFullyGenerated))
    {
        Data::cout() << "ERROR: " << this->getStatus().getMessage() << Data::endl;
        //Data::cout() << this->front->size() << " front elements still in front" << Data::endl;

        this->front->clear();
    }

    this->meshGenerator->finalize();

    if ((this->finalMeshState != JOINED) && (this->comm->numProcesses() > 1) && (this->comm->isSlave()))
    {
        this->mesh = static_cast<MJMesh::Mesh *>(this->meshGenerator->getMesh());
    }

	delete this->meshGenerator;

	this->meshGenerator = NULL;

    if ((this->finalMeshState != JOINED) && (this->comm->numProcesses() > 1))
    {
        bool ret = MJMesh::MainDrive::executeFinalize();

#if USE_POOL
#if USE_OPENMP
        PMJMesh::mergeThreadPools();
#endif //#if USE_OPENMP
        PMJMesh::finalizeThreadPools(static_cast<PMJMesh2::Communicator *>(this->comm));
#endif //#if USE_POOL

        //Data::cout() << "Process " << this->comm->rank() << " mesh at " << this->mesh << Data::endl;
        //Data::cout() << "Process " << this->comm->rank() << " " << this->qualityString() << Data::endl;

        UInt numSlots = this->quality.getNumSlots();
        UInt *histogram = this->comm->reduce(this->quality.getHistogram(), numSlots, Parallel::Communicator::SUM, this->comm->root());

        //Data::cout() << "Process " << this->comm->rank() << " reduce histogram" << Data::endl;

        Real min = this->comm->reduce(this->quality.getMin(), Parallel::Communicator::MINIMUM, this->comm->root());
        //Data::cout() << "Process " << this->comm->rank() << " reduce min" << Data::endl;
        Real max = this->comm->reduce(this->quality.getMax(), Parallel::Communicator::MAXIMUM, this->comm->root());
        //Data::cout() << "Process " << this->comm->rank() << " reduce max" << Data::endl;

        if (this->comm->isMaster())
        {
            this->quality.clear();

            this->quality.setQuality(numSlots, histogram, min, max);
        }

        return ret;
    }

	if (this->comm->isSlave())
	{
		this->status = MJMesh::MainDrive::success;

		return true;
	}

	Performer::Status status = this->status;

	bool ret = MJMesh::MainDrive::executeFinalize();

#if USE_POOL
#if USE_OPENMP
	PMJMesh::mergeThreadPools();
#endif //#if USE_OPENMP
	PMJMesh::finalizeThreadPools(static_cast<PMJMesh2::Communicator *>(this->comm));
#endif //#if USE_POOL

    if (status > this->error)
    {
        this->status = status;
    }

    return ret;
}
