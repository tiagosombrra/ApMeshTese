#include "PMTools2/ParallelMeshGenerator.h"

#if USE_MTOOLS

#include "Data/OStream.h"
#include "Data/TreeNode.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/GeometryFrontAdvancer.h"
#include "MJMesh/TopologyFrontAdvancer.h"
#include "MJMesh/BackTrackingFrontAdvancer.h"
#include "PMTools2/Communicator.h"
#include "PMTools2/MToolsTask.h"
#include "PMTools2/WorkerMainDrive.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "PMJMesh2/PartitionBSPTree.h"
#include "PJmesh/BackTrackingFrontAdvancer.h"

PMTools2::ParallelMeshGenerator::ParallelMeshGenerator() :
    PMJMesh2::ParallelMeshGenerator::ParallelMeshGenerator()
{
    this->ftree = NULL;

    this->createCrackInInnerFront = true;

    this->useUniformRefinement = false;
    this->useUniformSize = false;

#if USE_MPI
    this->createBoundaryInFullDistributedMerge = true;
#endif //#if USE_MPI
}

PMTools2::ParallelMeshGenerator::~ParallelMeshGenerator()
{

}

void PMTools2::ParallelMeshGenerator::setFTree(const FTools::OctTree *ftree)
{
    this->ftree = ftree;
}

void PMTools2::ParallelMeshGenerator::setUseUniformRefinement(bool useUniformRefinement)
{
    this->useUniformRefinement = useUniformRefinement;
}

void PMTools2::ParallelMeshGenerator::setUseUniformSize(bool useUniformSize)
{
    this->useUniformSize = useUniformSize;
}

const PMJMesh2::Task *PMTools2::ParallelMeshGenerator::cast(const Parallel::Task *t) const
{
    return static_cast<const PMTools2::MToolsTask *>(t);
}

PMJMesh2::Task *PMTools2::ParallelMeshGenerator::cast(Parallel::Task *t) const
{
    return static_cast<PMTools2::MToolsTask *>(t);
}

void PMTools2::ParallelMeshGenerator::finalize()
{
    PMJMesh2::ParallelMeshGenerator::finalize();

    if (this->updateType == PMJMesh2::FULL)
    {
        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            if ((this->tasks[i]) && (this->tasks[i]->getBuilder()) &&
                (dynamic_cast<PMTools2::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getOldFront()))
            {
                dynamic_cast<PMTools2::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getOldFront()->clear();
            }
        }

        if (this->mesh)
        {
#if USE_GUI
            this->mesh->freeze();
#endif //#if USE_GUI

            for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
                 iter != this->mesh->feEnd();)
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

#if USE_GUI
            this->mesh->unfreeze();
#endif //#if USE_GUI
        }

        return;
    }
}

bool PMTools2::ParallelMeshGenerator::setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
    Data::BoxTree *tree, Performer::IdManager *idManager) const
{
    //this if is to deal with the tasks in the shared mode, because some of
	// them may not have front
	if (static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront())
	{
		MJMesh::Front *front = static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront();

		front->setSorted(this->boundarySorted);
	}

	//if (this->comm->numProcesses() <= 1)
	{
		t->setStatus(Parallel::Task::ready);

		if (t->getBuilder())
		{
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

	PMTools2::WorkerMainDrive *drive = dynamic_cast<PMTools2::WorkerMainDrive *>(t->getBuilder());

	if (!drive)
    {
        t->makeBuilder();

        drive = dynamic_cast<PMTools2::WorkerMainDrive *>(t->getBuilder());
    }

    drive->setDimension(this->dimension);
    //drive->setBoundary(boundary);
    drive->PMJMesh::WorkerMainDrive::setBoundary(const_cast<MJMesh::Boundary *>(boundary));
    drive->PMJMesh::WorkerMainDrive::setTree(tree);

	//drive->setUsingSearchTrees(this->usingSearchTrees);
	drive->setUsingSearchTrees(true);
	drive->setRangeProportion(this->rangeProportion);

	drive->setIdManager(idManager);

	drive->setSmoothInSlavesEnabled(this->improvementInSlavesEnabled);
	//drive->setImprovementInSlavesEnabled(this->improvementInSlavesEnabled);
#if (!USE_DELETE_FELS)
	drive->setUnsmoothingLayer(0);
#endif //#if (!USE_DELETE_FELS)
	drive->setNumImproves(this->numImproves);
	drive->setSmoothingFactor(this->smoothingFactor);
	drive->setLocalBackTrackingEnabled(this->improvementInSlavesEnabled && this->localBackTrackingEnabled);
	drive->setMeshSorted(this->meshSorted);
	drive->setMaxBackTracks(this->maxBackTracks);
	drive->setMinRefinementLevel(0);
#if (!USE_DELETE_FELS)
	drive->setRemoveBoundaryFelsForSmoothing(!this->comm->isParallel());
#endif //#if (!USE_DELETE_FELS)

	//drive->setTestingBox(true);
	drive->setTestingBox(t->getId() != 0);
	drive->setClassifyTreeCells(false);
	drive->setFindingCommonFront(false);

	drive->setCreateInnerFrontInDecomposition(this->createInnerFrontInDecomposition);
    drive->setCreateCrackInInnerFront(this->createCrackInInnerFront);
    drive->setDisplaceVerticesInInnerFront(this->displaceVerticesInInnerFront);
    drive->setFixateFront((this->updateType == PMJMesh2::FULL) || (static_cast<Data::TreeNode *>(this->bsp->node(t->getId()))->isLeaf()));

    drive->setFtree(this->ftree);
    drive->setUsingUniformRefinement(this->useUniformRefinement);
    drive->setUseUniformSize(this->useUniformSize);

	if (!t->getComm())
	{
		t->setComm(this->comm);
	}

	return true;
}

void PMTools2::ParallelMeshGenerator::getParameters(const Parallel::Task *t) const
{
    PMTools2::WorkerMainDrive *drive = dynamic_cast<PMTools2::WorkerMainDrive *>(t->getBuilder());

	drive->setIdManager(NULL);
}

bool PMTools2::ParallelMeshGenerator::isInError(const Parallel::Task *t) const
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	bool inError = (drive->getStatus().getType() >= drive->error.getType());

	if (inError)
	{
		std::stringstream str;
		str << "ERROR: Some error occurred in task " << t->getId() << ": " << drive->getStatus().getMessage() << std::endl;
		std::cout << str.str();

		this->status.append(drive->getStatus());
	}

	return inError;
}

void PMTools2::ParallelMeshGenerator::executeFrontCheck()
{
    if (this->status >= this->error)
    {
        return;
    }

    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder());
    MJMesh::Front *front = const_cast<MJMesh::Front *>(drive->getFront());

    if (front->isEmpty())
    {
        this->status = this->success;

        return;
    }

	if (this->dimension != 3)
	{
		this->status = this->error;

        return;
	}

    /*//debug
    if (true)
    {
        this->status = this->errorMeshNotFullyGenerated;

        for (UShort state = 0; state < 4; state++)
        {
            for (Data::Front::FrontElementIterator iter = front->eBegin(state);
                 iter != front->eEnd(state); iter++)
            {
                std::cout << "state " << static_cast<UInt>(state) << " front fel " << (*iter)->getId() << " at " << (*iter) << std::endl;
            }
        }

        std::stringstream str;

        str << front->size() << " front elements still in front";

        this->status.append(str.str());

        return;
    }
    //endebug*/

	Data::cout() << "(0) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

	bool generated = false;

	bool usingSearchTrees = (front->isUsingSearchTrees()) || (front->size() > 200);

	const MJMesh::Boundary *boundary = drive->getBoundary() ? static_cast<const MJMesh::Boundary *>(drive->getBoundary()) : this->boundary;

	static const bool useJmesh = true;

	if ((!generated) && (useJmesh))
    {
        Data::cout() << "Going to Jmesh" << Data::endl;

        MJMesh::Mesh *newMesh = new MJMesh::Mesh();
        MJMesh::Boundary sharedBoundary;

        //Data::VertexList vertices = front->getVertices();

        //newMesh->splitTo(vertices, this->improvementLayers, sharedBoundary, boundary);

        PJmesh::BackTrackingFrontAdvancer *backTracking = new PJmesh::BackTrackingFrontAdvancer(boundary, front, static_cast<MJMesh::Mesh *>(this->mesh));
        //PJmesh::BackTrackingFrontAdvancer *backTracking = new PJmesh::BackTrackingFrontAdvancer(&sharedBoundary, front, newMesh);

        backTracking->setIdManager(this->idManager);

        backTracking->execute();

        //static_cast<MJMesh::Mesh *>(this->mesh)->merge(newMesh, boundary);

        //newMesh->clear();
        //delete newMesh;

        //sharedBoundary.clear();

        Data::cout() << "Finished Jmesh" << Data::endl;

        MJMesh::Front *btfront = static_cast<MJMesh::Front *>(backTracking->getFront());

        if (btfront != front)
        {
            front->clear();

            delete front;

            front = btfront;
        }

		if (backTracking->getStatus() == backTracking->ended)
        {
            generated = true;
        }

		delete backTracking;
    }

    Data::cout() << "(1) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    ULInt size = this->mesh->size();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (!generated)
	{
	    Data::cout() << "Going to MJMesh::GeometryFrontAdvancer" << Data::endl;

	    MJMesh::GeometryFrontAdvancer geometryAdvancer(this->dimension, boundary, this->tree, usingSearchTrees);

        geometryAdvancer.setIdManager(this->idManager);

        geometryAdvancer.setFront(front);
        geometryAdvancer.setMesh(static_cast<MJMesh::Mesh *>(this->mesh));

        if (usingSearchTrees)
        {
            Data::cout() << "Creating search trees" << Data::endl;

            if (!static_cast<PMJMesh::Front *>(front)->getVSTree())
            {
                front->setVSTree(geometryAdvancer.makeVertexSearchTree());
            }

            if (!static_cast<PMJMesh::Front *>(front)->getFESTree())
            {
                front->setFESTree(geometryAdvancer.makeFrontElementSearchTree());
            }
        }

#if USE_GUI
        bool step = Data::isStepByStep();

        Data::setStepByStep(false);
#endif //#if USE_GUI

        Data::cout() << "Executing MJMesh::GeometryFrontAdvancer" << Data::endl;

        geometryAdvancer.execute();

        Data::cout() << "Finished MJMesh::GeometryFrontAdvancer" << Data::endl;

#if USE_GUI
        Data::setStepByStep(step);
#endif //#if USE_GUI

        if ((geometryAdvancer.getStatus() >= geometryAdvancer.error))
        {
            this->status = this->error;

            this->status.append(geometryAdvancer.getStatus());

            Data::cout() << "Error in MJMesh::GeometryFrontAdvancer" << Data::endl;

            Data::cout() << "(2) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

            return;
        }

        if (geometryAdvancer.getStatus() == geometryAdvancer.ended)
        {
            /*this->status = this->success;

            //Data::cout() << "(3) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

            return;*/

            Data::cout() << "Generated with MJMesh::GeometryFrontAdvancer" << Data::endl;

            generated = true;
        }
	}

	Data::cout() << "(4) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

	if (!generated)
	{
	    Data::cout() << "Going to MJMesh::TopologyFrontAdvancer" << Data::endl;

	    MJMesh::TopologyFrontAdvancer topologyAdvancer(this->dimension, boundary, this->tree, front, static_cast<MJMesh::Mesh *>(this->mesh), usingSearchTrees);

        topologyAdvancer.setIdManager(this->idManager);

#if USE_GUI
        bool step = Data::isStepByStep();

        Data::setStepByStep(false);
#endif //#if USE_GUI

        Data::cout() << "Executing MJMesh::TopologyFrontAdvancer" << Data::endl;

        topologyAdvancer.execute();

        Data::cout() << "Finished MJMesh::TopologyFrontAdvancer" << Data::endl;

#if USE_GUI
        Data::setStepByStep(step);
#endif //#if USE_GUI

        if (topologyAdvancer.getStatus() >= topologyAdvancer.error)
        {
            this->status = this->error;

            this->status.append(topologyAdvancer.getStatus());

            Data::cout() << "Error in MJMesh::TopologyFrontAdvancer" << Data::endl;

            Data::cout() << "(5) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

            return;
        }

        if (topologyAdvancer.getStatus() == topologyAdvancer.ended)
        {
            /*this->status = this->success;

            //Data::cout() << "(6) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

            return;*/

            Data::cout() << "Generated with MJMesh::TopologyFrontAdvancer" << Data::endl;

            generated = true;
        }
	}

	Data::cout() << "(7) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;

    if (!generated)
    {
        Data::cout() << "Going to MJMesh::BackTrackingFrontAdvancer" << Data::endl;

        MJMesh::BackTrackingFrontAdvancer backTracking(this->dimension,
            boundary, this->tree, static_cast<MJMesh::Mesh *>(this->mesh), static_cast<MJMesh::Front *>(front),
            //true, true, 100*this->dimension, MJMesh::BackTrackingFrontAdvancer::FRONT_ADVANCING);
            true, true, 10*this->dimension, MJMesh::BackTrackingFrontAdvancer::FRONT_ADVANCING);

        backTracking.setIdManager(this->idManager);

#if USE_GUI
        bool step = Data::isStepByStep();

        Data::setStepByStep(false);
#endif //#if USE_GUI

        Data::cout() << "Executing MJMesh::BackTrackingFrontAdvancer" << Data::endl;

        backTracking.execute();

        Data::cout() << "Finished MJMesh::BackTrackingFrontAdvancer" << Data::endl;

#if USE_GUI
        Data::setStepByStep(step);
#endif //#if USE_GUI

        //front = static_cast<MJMesh::Front *>(backTracking.getFront());

        if (backTracking.getStatus() != backTracking.ended)
        {
            front = static_cast<MJMesh::Front *>(backTracking.getFront());

#if USE_GUI
            dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->setFront(front);
#else
            dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->setFront(NULL);
#endif //#if USE_GUI

            if (backTracking.getStatus() == backTracking.rejected)
            {
                this->status = this->errorMeshNotFullyGenerated;

                std::stringstream str;

                str << front->size() << " front elements still in front";

                this->status.append(str.str());

                Data::cout() << "Error in MJMesh::BackTrackingFrontAdvancer" << Data::endl;

                Data::cout() << "(8) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;
            }
            else
            {
                this->status = this->error;

                this->status.append(backTracking.getStatus());

                Data::cout() << "Error in MJMesh::BackTrackingFrontAdvancer" << Data::endl;

                Data::cout() << "(9) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;
            }

            return;
        }
        else
        {
            front = NULL;
        }

        /*if (front)
        {
            delete front;

            front = NULL;
        }*/

        /*this->status = this->success;

        return;*/

        generated = true;
    }

    if (!generated)
    {
        this->status = this->error;

        Data::cout() << "Error in PMTools2::ParallelMeshGenerator::executeFrontCheck" << Data::endl;

        return;
    }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Data::cout() << "Final mesh generation, number of generated elements = " << this->mesh->size() - size << Data::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    this->status = this->success;

    if (front)
    {
        Data::cout() << "(10) front->size() = " << front->size() << ", mesh->size() = " << this->mesh->size() << Data::endl;
    }
    else
    {
        Data::cout() << "(10) no front, mesh->size() = " << this->mesh->size() << Data::endl;
    }
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PMTools2::ParallelMeshGenerator::timing(const PMJMesh2::Task *task, Real timeTotal, const Data::ULI4Tuple &prev) const
{
    const PMTools::MToolsTask *t = static_cast<const PMTools2::MToolsTask *>(task);

	PMTools::WorkerMainDrive *drive = dynamic_cast<PMTools::WorkerMainDrive *>(t->getBuilder());

	//Real timeTrees = drive->timeTrees;
	Real timeGeometry = drive->timeGeometry;
	//Real timeTopology = drive->timeTopology;
	//Real timeBackTracking = drive->timeBackTracking;
	//Real timeClassify = drive->timeClassify;
	//Real timeRefinement = drive->timeRefinement;
	Real timeImprover = drive->timeImprover;
	Real timeOtherStuff = drive->timeOtherStuff;
	Real timeToMToolsConversion = drive->timeToMToolsConversion;
	Real timeFromMToolsConversion = drive->timeFromMToolsConversion;
	//Real timeOverhead = timeTotal - (timeTrees + timeGeometry + timeTopology + timeBackTracking + timeClassify + timeRefinement + timeImprover + timeOtherStuff + timeToMToolsConversion + timeFromMToolsConversion);
	Real timeOverhead = timeTotal - (timeGeometry + timeImprover + timeOtherStuff + timeToMToolsConversion + timeFromMToolsConversion);
	ULInt numVertices = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->innerVerticesSize() : 0;
	ULInt numElements = drive->MJMesh::MainDrive::getMesh() ? drive->MJMesh::MainDrive::getMesh()->size() : 0;

	Data::Box *box = static_cast<PMJMesh::Partition *>(t->getPartition())->getBox();

#if USE_C__11
    ULInt prevNumVertices = std::get<0>(prev);
    ULInt prevNumElements = std::get<1>(prev);
#else
    ULInt prevNumVertices = prev.values[0];
    ULInt prevNumElements = prev.values[1];
#endif //#if USE_C__11

#if USE_C__11
    ULInt prevNumFrontVertices = std::get<2>(prev);
    ULInt prevNumFrontElements = std::get<3>(prev);
#else
    ULInt prevNumFrontVertices = prev.values[2];
    ULInt prevNumFrontElements = prev.values[3];
#endif //#if USE_C__11

	std::stringstream str;
	str.precision(6);
	str.setf(std::ios::fixed, std::ios::floatfield);
	str << "Processor = " << this->comm->rank() << "\n";
	str << "Thread = " << static_cast<Parallel::TMCommunicator *>(this->comm)->threadId() << "\n";
	str << "Task = " << t->getId() << "\n";
	str << "Load = " << t->getPartition()->getLoad() << "\n";
	str << "Total Time = " << timeTotal << "\n";
	//str << "Tree Refiner and Disrefiner = " << timeRefinement << " (" << 100.0*timeRefinement/timeTotal << "%)\n";
	//str << "Search Tree Builder and Destroyer = " << timeTrees << " (" << 100.0*timeTrees/timeTotal << "%)\n";
	str << "Geometry Front Advancer = " << timeGeometry << " (" << 100.0*timeGeometry/timeTotal << "%)\n";
	//str << "Topology Front Advancer = " << timeTopology << " (" << 100.0*timeTopology/timeTotal << "%)\n";
	//str << "Back-Tracking Front Advancer (+ finding common front) = " << timeBackTracking << " (" << 100.0*timeBackTracking/timeTotal << "%)\n";
	str << "Mesh Improver (+ inverting new front) = " << timeImprover << " (" << 100.0*timeImprover/timeTotal << "%)\n";
	str << "Other Stuff in Mesh Generation = " << timeOtherStuff << " (" << 100.0*timeOtherStuff/timeTotal << "%)\n";
	//str << "Tree Classifier = " << timeClassify << " (" << 100.0*timeClassify/timeTotal << "%)\n";
	str << "Converting Data Structures to MTools = " << timeToMToolsConversion << " (" << 100.0*timeToMToolsConversion/timeTotal << "%)\n";
	str << "Building Mesh Data Structures = " << timeFromMToolsConversion << " (" << 100.0*timeFromMToolsConversion/timeTotal << "%)\n";
	str << "Overhead = " << timeOverhead << " (" << 100.0*timeOverhead/timeTotal << "%)\n";
	str << "Number of Vertices (" << numVertices << " - " << prevNumVertices << ") = " << static_cast<LInt>(numVertices) - static_cast<LInt>(prevNumVertices) << "\n";
	str << "Number of Elements (" << numElements << " - " << prevNumElements << ") = " << static_cast<LInt>(numElements) - static_cast<LInt>(prevNumElements) << "\n";
	str << "Number of Front Vertices = " << prevNumFrontVertices << "\n";
	str << "Number of Front Segments/Faces = " << prevNumFrontElements << "\n";
	str << "BoundingBox = ( " << box->getMin()->getCoord(0) << ", " << box->getMin()->getCoord(1) << ", " << ((this->dimension == 2) ? 0.0 : box->getMin()->getCoord(2)) << " ) x "
        << "( " << box->getMax()->getCoord(0) << ", " << box->getMax()->getCoord(1) << ", " << ((this->dimension == 2) ? 0.0 : box->getMax()->getCoord(2)) << " )\n";

	return str.str();
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#endif //#if USE_MTOOLS
