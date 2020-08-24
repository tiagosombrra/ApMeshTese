#include "PMTools2/MainDrive.h"

#if USE_MTOOLS

#include "PMTools2/Communicator.h"
#include "PMTools2/ParallelMeshGenerator.h"
#include "PMTools2/APrioriParallelMeshGenerator.h"
#include "PMTools2/MToolsTask.h"
#include "PMTools/MshRegion3D.h"
#include "PMTools/TreeGenerator.h"
#include "MTools/TreeGenerator.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/BoundaryBuilder.h"
#include "PMJMesh/MinTreeRefiner.h"
#include "MJMesh/Mesh.h"
#include "Performer/IdManager.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/OStream.h"

using namespace PMTools2;

PMTools2::MainDrive::MainDrive(Parallel::Communicator *comm) :
    MTools::MainDrive::MainDrive(),
    PMJMesh2::MainDrive::MainDrive(comm)
{
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

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
    this->usingMtoolsOctreeForLoadEstimation = true;
    //this->usingMtoolsOctreeForLoadEstimation = false;
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
}

PMTools2::MainDrive::~MainDrive()
{
    this->clear();
}

#if USE_NUMERICAL
void PMTools2::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
    this->PMJMesh2::MainDrive::hotellingTransformEnabled = this->MTools::MainDrive::hotellingTransformEnabled = hotellingTransformEnabled;
}

bool PMTools2::MainDrive::isHotellingTransformEnabled() const
{
    return this->PMJMesh2::MainDrive::hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void PMTools2::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->PMJMesh2::MainDrive::usingUniformRefinement = this->MTools::MainDrive::usingUniformRefinement = usingUniformRefinement;
}

bool PMTools2::MainDrive::isUsingUniformRefinement() const
{
	return this->PMJMesh2::MainDrive::usingUniformRefinement;
}

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
void PMTools2::MainDrive::setUsingMtoolsOctreeForLoadEstimation(bool usingMtoolsOctreeForLoadEstimation)
{
    this->usingMtoolsOctreeForLoadEstimation = usingMtoolsOctreeForLoadEstimation;
}
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

std::string PMTools2::MainDrive::qualityString() const
{
	return PMJMesh2::MainDrive::qualityString();
}

Performer::BoundaryBuilder *PMTools2::MainDrive::makeBoundaryBuilder() const
{
	return PMJMesh2::MainDrive::makeBoundaryBuilder();
	//return MTools::MainDrive::makeBoundaryBuilder();
}

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
Performer::TreeGenerator *PMTools2::MainDrive::makeTreeGenerator() const
{
    if (!this->usingMtoolsOctreeForLoadEstimation)
    {
        return PMJMesh2::MainDrive::makeTreeGenerator();
    }

	return new PMTools::TreeGenerator(this->MTools::MainDrive::boundary,
		this->mshRegion->GetOctTree()
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		, static_cast<PMTools::Communicator *>(this->comm)
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		);
}
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

MTools::MshRegion3D *PMTools2::MainDrive::makeMshRegion() const
{
    return new PMTools::MshRegion3D();
}

Parallel::Communicator *PMTools2::MainDrive::makeCommunicator() const
{
    return new PMTools2::Communicator();
}

PMJMesh2::ParallelMeshGenerator *PMTools2::MainDrive::makeMeshGenerator() const
{
    PMTools2::APrioriParallelMeshGenerator *aprGenerator = NULL;
    PMTools2::ParallelMeshGenerator *generator = NULL;

    switch (this->interfaceMeshGenerationType)
    {
    case PMJMesh2::A_POSTERIORI:
        generator = new PMTools2::ParallelMeshGenerator();
        break;

    case PMJMesh2::A_PRIORI:
        aprGenerator = new PMTools2::APrioriParallelMeshGenerator();
        generator = aprGenerator;
        break;

    case PMJMesh2::A_PRIORI_INTERNAL:
        //aprGenerator = new PMTools2::APrioriParallelMeshGenerator();
        //generator = aprGenerator;
        break;
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
        generator->setImprovementLayers((this->improvementLayers > 0) ? this->improvementLayers - 1 : 0); //generator->setImprovementLayers(this->improvementLayers);
        generator->setUpdateType(this->updateType);
        generator->setFinalMeshState(this->finalMeshState);

        generator->setComm(this->comm);
        generator->setIdManager(this->idManager);

        generator->setBoundary(static_cast<MJMesh::Boundary *>(this->PMJMesh2::MainDrive::boundary));
        generator->setTree(this->PMJMesh2::MainDrive::tree);
        generator->setBsp(this->partitionTree);

        generator->setTasks(this->tasks);

        generator->setFTree(this->mshRegion->GetOctTree());
        generator->setUseUniformRefinement(this->PMJMesh2::MainDrive::usingUniformRefinement);
        generator->setUseUniformSize(this->mshRegion->GetOctTree()->IsUseUniformSize());

        if (this->interfaceMeshGenerationType != PMJMesh2::A_POSTERIORI)
        {
            aprGenerator->setInterfaceElementSizeFactor(this->aPrioriInterfaceElementSizeFactor);
        }
    }

    return generator;
}

PMJMesh::MJMeshTask *PMTools2::MainDrive::makeTask(PMJMesh::Partition *p, PMJMesh2::PartitionBSPTreeCell *cell) const
{
    PMTools2::MToolsTask *task = new PMTools2::MToolsTask();

    task->setPartition(p);
    task->setComm(this->comm);
    task->makeBuilder();

    task->setCell(cell);

    return task;
}

bool PMTools2::MainDrive::loadFile()
{
    return PMJMesh2::MainDrive::loadFile();
}

bool PMTools2::MainDrive::saveFile() const
{
    return PMJMesh2::MainDrive::saveFile();
}

bool PMTools2::MainDrive::loadInputFile(const std::string &inputFileName)
{
    return PMJMesh2::MainDrive::loadInputFile(inputFileName);
}

bool PMTools2::MainDrive::saveInputFile(const std::string &outputFileName) const
{
    return PMJMesh2::MainDrive::saveInputFile(outputFileName);
}

bool PMTools2::MainDrive::loadOutputFile(const std::string &inputFileName)
{
    return PMJMesh2::MainDrive::loadOutputFile(inputFileName);
}

bool PMTools2::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
    return PMJMesh2::MainDrive::saveOutputFile(outputFileName);
}

void PMTools2::MainDrive::execute()
{
    PMJMesh2::MainDrive::execute();
}

void PMTools2::MainDrive::clear()
{
    this->MTools::MainDrive::boundary = NULL;
	this->MTools::MainDrive::mesh = NULL;
	this->MTools::MainDrive::tree = NULL;

	MTools::MainDrive::clear();
	PMJMesh2::MainDrive::clear();
}

bool PMTools2::MainDrive::executeInit()
{
	this->mshRegion = this->makeMshRegion();

	if (this->PMJMesh2::MainDrive::boundaryBuilder)
	{
		this->MTools::MainDrive::boundaryBuilder = this->PMJMesh2::MainDrive::boundaryBuilder;
	}

	return PMJMesh2::MainDrive::executeInit();
}

bool PMTools2::MainDrive::executeBoundaryBuilder(bool del)
{
	if (this->MTools::MainDrive::boundaryBuilder)
	{
		this->PMJMesh2::MainDrive::boundaryBuilder = this->MTools::MainDrive::boundaryBuilder;
	}
	else
	{
		this->MTools::MainDrive::boundaryBuilder = this->PMJMesh2::MainDrive::boundaryBuilder;
	}

#if (!USE_WORKER_FILE_LOADER)
	static_cast<PMJMesh::BoundaryBuilder *>(this->MTools::MainDrive::boundaryBuilder)->setDimension(this->dimension);
#endif //#if (!USE_WORKER_FILE_LOADER)

	bool ret = MTools::MainDrive::executeBoundaryBuilder(false);

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->PMJMesh2::MainDrive::boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->MTools::MainDrive::boundaryBuilder;

		this->MTools::MainDrive::boundaryBuilder = NULL;
		this->PMJMesh2::MainDrive::boundaryBuilder = NULL;
	}

	this->PMJMesh2::MainDrive::boundary = this->MTools::MainDrive::boundary;

	if (ret)
	{
		this->Performer::MainDrive::status = this->boundaryBuilt;
	}

	return ret;
}

#if USE_NUMERICAL
bool PMTools2::MainDrive::executeHotellingTransform()
{
	bool ret = MTools::MainDrive::executeHotellingTransform();

	if (ret)
    {
        this->Performer::MainDrive::status = this->hotellingTransformApplied;
    }

	return ret;
}
#endif //#if USE_NUMERICAL

bool PMTools2::MainDrive::executeTreeGenerator(bool del)
{
#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))
	Real start = Data::time();
#endif //#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))

    //Msh3D::AdvFront does not need to search for cracks
	this->mshRegion->SetNoCracks(true);

	this->mshRegion->SetUseUniformRefinement(this->PMJMesh2::MainDrive::usingUniformRefinement);

	//this->mshRegion->SetUseUniformSize(false);
	this->mshRegion->SetUseUniformSize(true);
	/*if ((this->partitionerType == PMJMesh2::GEOMETRY_BASED_BSP) || (this->partitionerType == PMJMesh2::LOAD_BASED_BSP))
	{
		this->mshRegion->SetUseUniformSize(false);
	}
	else
	{
		this->mshRegion->SetUseUniformSize(true);
	}*/

	this->mshRegion->GenerateOctTree();

	this->mshRegion->SetNoCracks(false);

#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))
    if (this->comm->isMaster()) std::cout << "Time to generate the tree = " << Data::time() - start << std::endl;
    start = Data::time();
#endif //#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
	if (this->usingMtoolsOctreeForLoadEstimation)
    {
        bool result = MTools::MainDrive::executeTreeGenerator(false);

#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))
        if (this->comm->isMaster()) std::cout << "Time to classify the tree = " << Data::time() - start << std::endl;
        start = Data::time();
#endif //#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))

        if ((result) && (this->PMJMesh2::MainDrive::usingUniformRefinement))
        {
            this->elementsMap = dynamic_cast<PMJMesh::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->getElementsMap();
            dynamic_cast<PMJMesh::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->setElementsMap(NULL);
        }
#if DEBUG_CELL_COUNTER
        else if (this->onlyCount)
        {
            this->elementsMap = dynamic_cast<PMJMesh::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->getElementsMap();
            dynamic_cast<PMJMesh::TreeGenerator *>(this->MTools::MainDrive::treeGenerator)->setElementsMap(NULL);
        }
#endif //#if DEBUG_CELL_COUNTER

        //this->minRefinementLevel = !this->mshRegion->GetOctTree()->IsUseUniformSize() ? 0 :
        //	static_cast<UInt>(std::log(static_cast<Data::BoxTreeCell *>(this->MTools::MainDrive::tree->getRoot())->size()/this->mshRegion->GetOctTree()->GetUniformSize())/std::log(2.0));

        this->PMJMesh2::MainDrive::tree = this->MTools::MainDrive::tree;

        if (del)
        {
            delete this->MTools::MainDrive::treeGenerator;

            this->MTools::MainDrive::treeGenerator = NULL;
        }

        if (this->PMJMesh2::MainDrive::usingUniformRefinement)
        {
            if (!this->minTreeRefiner)
            {
                this->minTreeRefiner = this->makeTreeMinRefiner();
            }

            UInt minRefinementLevel = !this->mshRegion->GetOctTree()->IsUseUniformSize() ? 0 :
                static_cast<UInt>(std::log(static_cast<Data::BoxTreeCell *>(this->MTools::MainDrive::tree->getRoot())->size()/this->mshRegion->GetOctTree()->GetUniformSize())/std::log(2.0));

            this->minTreeRefiner->setMinLevel(minRefinementLevel);

#if USE_GUI
            bool step = Data::isStepByStep();

            Data::setStepByStep(false);
#endif //#if USE_GUI

            this->minTreeRefiner->execute();

#if USE_GUI
            Data::setStepByStep(step);
#endif //#if USE_GUI

#if DEBUG_CELL_COUNTER
            if (this->onlyCount)
            {
                static_cast<PMJMesh::MinTreeRefiner *>(this->minTreeRefiner)->setElementsMap(NULL);
            }
#endif //#if DEBUG_CELL_COUNTER

#if DEBUG_CELL_COUNTER
            if (!this->onlyCount)
            {
#endif //#if DEBUG_CELL_COUNTER
                this->elementsMap = NULL;
#if DEBUG_CELL_COUNTER
            }
#endif //#if DEBUG_CELL_COUNTER

            if (del)
            {
                delete this->minTreeRefiner;

                this->minTreeRefiner = NULL;
            }
        }

#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))
        if (this->comm->isMaster()) std::cout << "Time to refine uniformly the tree = " << Data::time() - start << std::endl;
#endif //#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))

        this->Performer::MainDrive::status = this->treeGenerated;

        return true;
    }
//#else
    Msh3D::MshRegion3D::MeshParams params;

	this->mshRegion->GetMeshParams(params);

    Real factor = params.OctreeRefineFactor*params.OptimalSizeFactor;
    factor = 1.0/(factor*factor);

    this->MJMesh::MainDrive::treeFactor = this->MTools::MainDrive::treeFactor = factor;

    bool ret = this->PMJMesh2::MainDrive::executeTreeGenerator(true);

    this->MTools::MainDrive::tree = this->PMJMesh2::MainDrive::tree;

#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))
    if (this->comm->isMaster()) std::cout << "Time to classify the tree = " << Data::time() - start << std::endl;
#endif //#if ((USE_TIME_MEASUREMENTS) && (DEBUG_PMTOOLS2_PROFILE_OCTREE))

    return ret;
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
}

/*bool PMTools2::MainDrive::executePartitionTreeGenerator()
{
    bool ret = PMJMesh2::MainDrive::executePartitionTreeGenerator();

    delete this->PMJMesh2::MainDrive::tree;

    this->PMJMesh2::MainDrive::tree = this->MTools::MainDrive::tree = NULL;

    return ret;
}*/

bool PMTools2::MainDrive::executeParallelMeshGenerator()
{
    bool ret = PMJMesh2::MainDrive::executeParallelMeshGenerator();

	//if (ret)
	{
		this->MTools::MainDrive::mesh = this->PMJMesh2::MainDrive::mesh;
	}

	return ret;
}

#if USE_NUMERICAL
bool PMTools2::MainDrive::executeHotellingBackTransform()
{
	bool ret = MTools::MainDrive::executeHotellingBackTransform();

	if (ret)
    {
        this->Performer::MainDrive::status = this->hotellingBackTransformApplied;
    }

	return ret;
}
#endif //#if USE_NUMERICAL

bool PMTools2::MainDrive::executeFinalize()
{
	bool ret = this->PMJMesh2::MainDrive::executeFinalize();

	if ((ret) && (this->PMJMesh2::MainDrive::mesh))
	{
	    this->MTools::MainDrive::mesh = this->PMJMesh2::MainDrive::mesh;

		this->removeHangingVertices();
	}

	return ret;
}

#endif //#if USE_MTOOLS
