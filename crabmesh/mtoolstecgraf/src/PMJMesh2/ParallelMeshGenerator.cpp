#include "PMJMesh2/ParallelMeshGenerator.h"

#include "Data/State.h"
#include "Performer/RangedIdManager.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/HotellingTransformer.h"
#include "PMJMesh2/WorkerMainDrive.h"
#include "PMJMesh2/MJMeshTask.h"
#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/FrontPartitioner.h"
#include "PMJMesh2/CrossingReplacer.h"

#include "PMJMesh/MeshUpdater.h"
#include "Data/OStream.h"

using namespace PMJMesh2;

const Performer::Status PMJMesh2::ParallelMeshGenerator::notInit =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::NOT_INIT, "not initialized");
const Performer::Status PMJMesh2::ParallelMeshGenerator::frontPartitioned =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::FRONT_PARTITIONED, "front partitioned");
const Performer::Status PMJMesh2::ParallelMeshGenerator::sharedMemoryMeshGenerated =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::SHARED_MEMORY_MESH_GENERATED, "shared memory mesh generated");
const Performer::Status PMJMesh2::ParallelMeshGenerator::distributedMemoryMeshGenerated =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::DISTRIBUTED_MEMORY_MESH_GENERATED, "distributed memory mesh generated");
const Performer::Status PMJMesh2::ParallelMeshGenerator::success =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::SUCCESS, "success");

const Performer::Status PMJMesh2::ParallelMeshGenerator::error =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::ERROR, "error");
const Performer::Status PMJMesh2::ParallelMeshGenerator::errorMeshNotFullyGenerated =
	Performer::Status(PMJMesh2::ParallelMeshGenerator::ERROR_MESH_NOT_FULLY_GENERATED, "error - mesh not fully generated");

PMJMesh2::ParallelMeshGenerator::ParallelMeshGenerator() :
    Performer::MeshGenerator::MeshGenerator(),
    Parallel::Builder::Builder()
{
    this->dimension = 0;
    this->boundarySorted = false;
    this->numImproves = 5;
    this->smoothingFactor = 1.0;
    this->localBackTrackingEnabled = true;
    this->meshSorted = true;
    this->maxBackTracks = 20;

    this->numTasksPerThread = 1;
    this->createInnerFrontInDecomposition = false;
    this->rangeProportion = 1.0;
    this->improvementInSlavesEnabled = true;
    this->improvementLayers = 2;
    this->updateType = PARTIAL;
    this->finalMeshState = JOINED;

    this->createCrackInInnerFront = false;
    this->displaceVerticesInInnerFront = false;

    this->boundary = NULL;
    this->tree = NULL;
    this->bsp = NULL;

    this->idManager = NULL;
	this->idoffset = 0;
	this->idrange = 1024;

    this->front = NULL;

//#if USE_OPENMP
    this->commonParent = NULL;
    this->replacer = NULL;
//#endif //#if USE_OPENMP

#if USE_MPI
    this->createBoundaryInFullDistributedMerge = false;
#endif //#if USE_MPI

    this->status = this->notInit;
}

PMJMesh2::ParallelMeshGenerator::~ParallelMeshGenerator()
{
//#if USE_OPENMP
    if (this->replacer)
    {
        delete this->replacer;
    }
//#endif //#if USE_OPENMP

    /*for (UInt i = 0; i < this->tasks.size(); i++)
    {
        static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition())->setBox(NULL);

        delete this->tasks[i];
    }*/

    for (UInt i = 0; i < this->idManagers.size(); i++)
	{
		if (this->idManagers[i])
		{
			delete this->idManagers[i];
		}
	}

	this->idManagers.clear();
}

void PMJMesh2::ParallelMeshGenerator::setDimension(UInt dimension)
{
    this->dimension = dimension;
}

void PMJMesh2::ParallelMeshGenerator::setBoundarySorted(bool boundarySorted)
{
    this->boundarySorted = boundarySorted;
}

void PMJMesh2::ParallelMeshGenerator::setNumImproves(UInt numImproves)
{
    this->numImproves = numImproves;
}

void PMJMesh2::ParallelMeshGenerator::setSmoothingFactor(Real smoothingFactor)
{
    this->smoothingFactor = smoothingFactor;
}

void PMJMesh2::ParallelMeshGenerator::setLocakBackTrackingEnabled(bool localBackTrackingEnabled)
{
    this->localBackTrackingEnabled = localBackTrackingEnabled;
}

void PMJMesh2::ParallelMeshGenerator::setMeshSorted(bool meshSorted)
{
    this->meshSorted = meshSorted;
}

void PMJMesh2::ParallelMeshGenerator::setMaxBackTracks(UInt maxBackTracks)
{
    this->maxBackTracks = maxBackTracks;
}

void PMJMesh2::ParallelMeshGenerator::setNumTasksPerThread(UInt numTasksPerThread)
{
    this->numTasksPerThread = (numTasksPerThread == 0) ? 1 : numTasksPerThread;
}

void PMJMesh2::ParallelMeshGenerator::setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition)
{
    this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
}

void PMJMesh2::ParallelMeshGenerator::setRangeProportion(Real rangeProportion)
{
    this->rangeProportion = rangeProportion;
}

void PMJMesh2::ParallelMeshGenerator::setImprovementInSlavesEnabled(bool improvementInSlavesEnabled)
{
    this->improvementInSlavesEnabled = improvementInSlavesEnabled;
}

void PMJMesh2::ParallelMeshGenerator::setImprovementLayers(UInt improvementLayers)
{
    this->improvementLayers = improvementLayers;
}

void PMJMesh2::ParallelMeshGenerator::setUpdateType(enum MeshUpdateType updateType)
{
    this->updateType = updateType;
}

void PMJMesh2::ParallelMeshGenerator::setFinalMeshState(enum FinalMeshState finalMeshState)
{
    this->finalMeshState = finalMeshState;
}

void PMJMesh2::ParallelMeshGenerator::setBoundary(const MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

void PMJMesh2::ParallelMeshGenerator::setTree(const Data::BoxTree *tree)
{
    this->tree = tree;
}

void PMJMesh2::ParallelMeshGenerator::setBsp(const PartitionBSPTree *bsp)
{
    this->bsp = bsp;
}

PMJMesh::Front *PMJMesh2::ParallelMeshGenerator::getFront() const
{
    return this->front;
}

void PMJMesh2::ParallelMeshGenerator::setTasks(const Parallel::TaskVector &tasks)
{
    this->tasks = tasks;
}

void PMJMesh2::ParallelMeshGenerator::setIdManager(Performer::IdManager *idManager)
{
    this->idManager = idManager;
}

void PMJMesh2::ParallelMeshGenerator::execute()
{
    if ((this->dimension != 2) && (this->dimension != 3))
    {
        return;
    }

    if ((!this->comm) || (!this->boundary) || (!this->tree) || (!this->bsp) || (this->tasks.empty()))
    //if ((!this->comm) || (!this->boundary) || (!this->bsp) || (this->tasks.empty()))
    {
        return;
    }

    if ((this->status == this->success) ||
        (this->status >= this->error))
    {
        return;
    }

#if DEBUG_PMJMESH2_PROFILE
	Real start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    switch (this->status.getType())
    {
    case PMJMesh2::ParallelMeshGenerator::NOT_INIT:

        this->executeFrontPartitioning();

#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) Data::cout() << "Time for executeFrontPartitioning() = " << Data::time() - start << Data::endl;
        start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

#if USE_GUI
        if (Data::isStepByStep())
        {
            break;
        }
#endif //#if USE_GUI

    case PMJMesh2::ParallelMeshGenerator::FRONT_PARTITIONED:

        this->executeSharedMeshGeneration();

#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) Data::cout() << "Time for executeSharedMeshGeneration() = " << Data::time() - start << Data::endl;
        start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

#if USE_GUI
        if ((Data::isStepByStep()) && (this->comm->numProcesses() > 1))
        {
            break;
        }
#endif //#if USE_GUI

    case PMJMesh2::ParallelMeshGenerator::SHARED_MEMORY_MESH_GENERATED:

        this->executeDistributedMeshGeneration();

#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) Data::cout() << "Time for executeDistributedMeshGeneration() = " << Data::time() - start << Data::endl;
        start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    case PMJMesh2::ParallelMeshGenerator::DISTRIBUTED_MEMORY_MESH_GENERATED:

        this->executeFrontCheck();

#if DEBUG_PMJMESH2_PROFILE
        if (this->comm->isMaster()) Data::cout() << "Time for executeFrontCheck() = " << Data::time() - start << Data::endl;
        start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE
    }
}

#if USE_NUMERICAL
void PMJMesh2::ParallelMeshGenerator::executeHotellingBackTransform(PMJMesh::HotellingTransformer *hotellingTransformer)
{
    if (this->updateType == FULL)
    {
        return;
    }

    if (this->updateType == PARTIAL)
    {
        Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

        Data::Mesh *originalMesh = hotellingTransformer->getMesh();

        hotellingTransformer->setMesh(this->mesh);

        for (Int i = 0; i < static_cast<Int>(this->tasks.size()); i++)
        {
            MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getMesh());

            if ((mesh) && (mesh != originalMesh))
            {
                hotellingTransformer->addMesh(mesh);
            }

            Data::MeshList meshes = this->cast(this->tasks[i])->getMeshes();

            while (!meshes.empty())
            {
                MJMesh::Mesh *mesh2 = static_cast<MJMesh::Mesh *>(meshes.front());

                if ((mesh2) && (mesh2 != mesh) && (mesh2 != originalMesh))
                {
                    hotellingTransformer->addMesh(mesh2);
                }

                meshes.pop_front();
            }
        }

        hotellingTransformer->executeBackOnMeshes();

        hotellingTransformer->setMesh(originalMesh);
    }
}
#endif //#if USE_NUMERICAL

void PMJMesh2::ParallelMeshGenerator::finalize()
{
#if USE_DELETE_FELS
    this->front->clear();
#endif //#if USE_DELETE_FELS

    if (this->updateType == FULL)
    {
        return;
    }

#if USE_GUI
    if (this->mesh)
    {
        this->mesh->freeze();
    }
#endif //#if USE_GUI

    if (this->updateType == PARTIAL)
    {
        Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

#if ((USE_MPI) && (USE_THREAD_COLOR))
        Real r, g, b;

        if ((comm->isMaster()) && (comm->numProcesses() > 1))
        {
            Data::pallete(0, r, g, b);
        }
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

#if ((USE_MPI) && (USE_THREAD_COLOR))
        this->finalizeJoinSharedMeshes(r, g, b);
#else
        this->finalizeJoinSharedMeshes();
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

#if USE_MPI
        if (comm->numProcesses() > 1)
        {
            if (this->finalMeshState == JOINED)
            {
                this->finalizeJoinDistributedMeshes();
            }
            else if (this->finalMeshState == DISTRIBUTED)
            {
                this->finalizeDistributedMeshes();
            }
        }
#endif //#if USE_MPI

#if USE_DELETE_FELS
        this->finalizeClearMesh();
#endif //#if USE_DELETE_FELS

        //Data::cout() << "A Proc " << this->comm->rank() << ", mesh at " << this->mesh << Data::endl;
    }

#if USE_GUI
    if (this->mesh)
    {
        this->mesh->unfreeze();
    }
#endif //#if USE_GUI
}

#if ((USE_MPI) && (USE_THREAD_COLOR))
void PMJMesh2::ParallelMeshGenerator::colorizeMesh(Parallel::TMCommunicator *comm, Data::Mesh *mesh, Real r, Real g, Real b) const
{
    if ((!comm->isMaster()) || (comm->numProcesses() == 1))
    {
        return;
    }

    for (Data::Mesh::ElementIterator iter = mesh->eBegin();
         iter != mesh->eEnd(); iter++)
    {
        (*iter)->setColor(r, g, b);
    }

    for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
         iter != mesh->feEnd(); iter++)
    {
        (*iter)->setColor(r, g, b);
    }

    for (Data::Mesh::VertexIterator iter = mesh->vBegin();
         iter != mesh->vEnd(); iter++)
    {
        (*iter)->setColor(r, g, b);
    }
}
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

#if ((USE_MPI) && (USE_THREAD_COLOR))
void PMJMesh2::ParallelMeshGenerator::finalizeJoinSharedMeshes(Real r, Real g, Real b)
#else
void PMJMesh2::ParallelMeshGenerator::finalizeJoinSharedMeshes()
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))
{
#if ((USE_MPI) && (USE_THREAD_COLOR))
    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

    for (Int i = 0; i < static_cast<Int>(this->tasks.size()); i++)
    {
        MJMesh::Mesh *mesh = NULL;

        if (i != 0)
        {
            mesh = const_cast<MJMesh::Mesh *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getMesh());

            if (this->mesh)
            {
#if ((USE_MPI) && (USE_THREAD_COLOR))
                this->colorizeMesh(comm, mesh, r, g, b);
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

                //std::cout << "trying to merge with task " << this->tasks[i]->getId() << std::endl;

                this->mergeFromSharedTask(mesh);
            }
            else
            {
                this->mesh = mesh;

#if USE_GUI
                if (!this->mesh->isFrozen())
                {
                    this->mesh->freeze();
                }
#endif //#if USE_GUI

                dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->setMesh(NULL);
            }
        }

        //Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>(this->tasks[i])->getMeshes();
        Data::MeshList meshes = this->cast(this->tasks[i])->getMeshes();

        while (!meshes.empty())
        {
            mesh = static_cast<MJMesh::Mesh *>(meshes.front());

            if (this->mesh)
            {
#if ((USE_MPI) && (USE_THREAD_COLOR))
                this->colorizeMesh(comm, mesh, r, g, b);
#endif //#if ((USE_MPI) && (USE_THREAD_COLOR))

                this->mergeFromSharedTask(mesh);
            }
            else
            {
                this->mesh = mesh;

#if USE_GUI
                if (!this->mesh->isFrozen())
                {
                    this->mesh->freeze();
                }
#endif //#if USE_GUI

                //static_cast<PMJMesh2::MJMeshTask *>(this->tasks[i])->remove(this->mesh);
                this->cast(this->tasks[i])->remove(this->mesh);
            }

            meshes.pop_front();
        }

#if USE_GUI
        this->cast(this->tasks[i])->clearMids();
#endif //#if USE_GUI

        Data::Boundary *boundary = const_cast<Data::Boundary *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getBoundary());

        dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->setBoundary(NULL);

        if ((boundary) && (boundary != this->boundary))
        {
#if USE_DELETE_FELS
            if (this->comm->isMaster())
            {
                if( (this->tasks[i]) && (this->tasks[i]->getBuilder()) && (dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getNewFront()) )
                    dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getNewFront()->clear();

                if( (this->tasks[i]) && (this->tasks[i]->getBuilder()) && (dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getOldFront()) )
                    dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[i]->getBuilder())->getOldFront()->clear();
            }
#endif //#if USE_DELETE_FELS

            boundary->clear();

            delete boundary;
        }
    }
}

#if USE_MPI
void PMJMesh2::ParallelMeshGenerator::finalizeJoinDistributedMeshes()
{
    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    Int numProcs = comm->numProcesses();

    if (comm->isMaster())
    {
        Parallel::Transferable **mts = comm->gather();

        MJMesh::Boundary tempBoundary;

        for (Int i = 0; i < numProcs - 1; i++)
        {
            //Data::cout() << "mt from processor " << i + 1 << Data::endl;

            PMJMesh::MeshTransferable *mt = static_cast<PMJMesh::MeshTransferable *>(mts[i]);

            if (!mt)
            {
                //Data::cout() << "mt is NULL" << Data::endl;

                continue;
            }

            MJMesh::Mesh *mesh = static_cast<MJMesh::Mesh *>(mt->getMesh());

            mt->setMesh(NULL);

            delete mt;

            //Data::cout() << "mesh at " << mesh << Data::endl;
            //Data::cout() << "mesh->vs = " << mesh->verticesSize() << Data::endl;
            //Data::cout() << "mesh->fels = " << mesh->frontElementsSize() << Data::endl;
            //Data::cout() << "mesh->els = " << mesh->size() << Data::endl;

#if USE_THREAD_COLOR
            Real r, g, b;
            Data::pallete(i + 1, r, g, b);
#endif //#if USE_THREAD_COLOR

            for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
                 iter != mesh->feEnd();)
            {
                MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));
                MJMesh::FrontElement *orig = NULL;

                //bool print = (temp->getId() == 84082);

#if USE_THREAD_COLOR
                temp->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

                /*if (print)
                {
                    Data::cout() << "temp fel " << temp->text() << " at " << temp << Data::endl;
                }*/

#if USE_C__11
                Data::FrontElementHashMap::iterator it = this->bFrontElements.find(temp->getId());
#else
                Data::FrontElementMap::iterator it = this->bFrontElements.find(temp->getId());
#endif //#if USE_C__11

                if (it == this->bFrontElements.end())
                {
                    this->mesh->add(temp);

                    /*if (print)
                    {
                        Data::cout() << "adding temp to this->mesh" << Data::endl;
                    }*/
                }
                else
                {
                    orig = static_cast<MJMesh::FrontElement *>((*it).second);

                    /*if (print)
                    {
                        Data::cout() << "orig at " << orig << Data::endl;
                        Data::cout() << "orig fel " << orig->text() << Data::endl;
                    }*/

#if USE_THREAD_COLOR
                    orig->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

                    //Data::ElementList els = temp->getAdjacency();

                    //while (!els.empty())
                    for (Data::ElementList::iterator iter = temp->eBegin();
                         iter != temp->eEnd(); iter = temp->erase(iter))
                    {
                        //MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
                        MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

                        /*if (print)
                        {
                            Data::cout() << "adj e " << e->text() << Data::endl;
                        }*/

                        //els.pop_front();

                        //temp->remove(e);
                        orig->add(e);
                    }

                    for (UInt i = 0; i < temp->numVertices(); i++)
                    {
                        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(temp->getVertex(i));

                        if (v)
                        {
                            /*if (print)
                            {
                                Data::cout() << "v " << v->text() << Data::endl;
                            }*/

                            v->remove(temp);
                        }

                        temp->setVertex(i, NULL);
                    }

                    if (!this->mesh->have(orig))
                    {
                        this->mesh->add(orig);

                        /*if (print)
                        {
                            Data::cout() << "adding orig to this->mesh" << Data::endl;
                        }*/
                    }
                }

                iter = mesh->erase(iter);

                if ((!orig) && (temp->numAdjacents() == 1))
                {
                    this->bFrontElements.insert(std::make_pair(temp->getId(), temp));

                    tempBoundary.add(temp);

                    /*if (print)
                    {
                        Data::cout() << "adding temp to tempBoundary and bFrontElements" << Data::endl;
                    }*/
                }

                /*if (print)
                {
                    Data::cout() << "removing temp from mesh" << Data::endl;
                }*/

                if (orig)
                {
                    /*if (print)
                    {
                        Data::cout() << "deleting temp" << Data::endl;
                    }*/

                    delete temp;

                    temp = orig;
                }

                for (UInt i = 0; i < temp->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(temp->getVertex(i));

                    if (!v->have(temp))
                    {
                        v->add(temp);
                    }
                }
            }

            for (Data::Mesh::VertexIterator iter = mesh->vBegin();
                 iter != mesh->vEnd();)
            {
                MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));
                MJMesh::Vertex *orig = NULL;

#if USE_THREAD_COLOR
                temp->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

                //Data::cout() << "temp v " << temp->getId() << " at " << temp << Data::endl;

#if USE_C__11
                Data::VertexHashMap::iterator it = this->bVertices.find(temp->getId());
#else
                Data::VertexMap::iterator it = this->bVertices.find(temp->getId());
#endif //#if USE_C__11

                if (it == this->bVertices.end())
                {
                    //Data::FrontElementList fels = temp->getAdjacentFrontElements();

                    //while (!fels.empty())
                    for (Data::FrontElementList::iterator iter = temp->feBegin();
                         iter != temp->feEnd(); iter++)
                    {
                        //Data::FrontElement *e = fels.front();
                        Data::FrontElement *e = (*iter);

                        //fels.pop_front();

                        if (tempBoundary.have(e))
                        {
                            this->bVertices.insert(std::make_pair(temp->getId(), temp));

                            break;
                        }
                    }

                    this->mesh->add(temp);

                    //Data::cout() << "adding temp to this->mesh" << Data::endl;
                }
                else
                {
                    orig = static_cast<MJMesh::Vertex *>((*it).second);

                    //Data::cout() << "orig v " << orig->getId() << " at " << orig << Data::endl;

#if USE_THREAD_COLOR
                    orig->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

                    //Data::ElementList els = temp->getAdjacentElements();

                    //while (!els.empty())
                    for (Data::ElementList::iterator iter = temp->eBegin();
                         iter != temp->eEnd(); iter = temp->erase(iter))
                    {
                        //Data::Element *e = els.front();
                        Data::Element *e = (*iter);

                        //els.pop_front();

                        e->replace(temp, orig);

                        //temp->remove(e);

                        orig->add(e);
                    }

                    //Data::FrontElementList fels = temp->getAdjacentFrontElements();

                    //Data::FrontElementList defFEls = orig->getAdjacentFrontElements();

                    //while (!fels.empty())
                    for (Data::FrontElementList::iterator iter = temp->feBegin();
                         iter != temp->feEnd(); iter = temp->erase(iter))
                    {
                        //Data::FrontElement *e = fels.front();
                        Data::FrontElement *e = (*iter);

                        //fels.pop_front();

                        e->replace(temp, orig);

                        //temp->remove(e);

                        orig->add(e);
                    }

                    if (!this->mesh->have(orig))
                    {
                        this->mesh->add(orig);

                        //Data::cout() << "adding orig to this->mesh" << Data::endl;
                    }
                }

                iter = mesh->erase(iter);

                {
                    MJMesh::Vertex *v = orig ? orig : temp;

                    bool bhave = this->boundary->have(v);
                    bool mhave = !this->mesh->haveInner(v);

                    if ((bhave) && (!mhave))
                    {
                        this->mesh->removeInner(v);
                    }
                    else if ((!bhave) && (mhave))
                    {
                        this->mesh->addInner(v);
                    }
                }

                //Data::cout() << "removing temp from mesh" << Data::endl;

                if (orig)
                {
                    //Data::cout() << "deleting temp" << Data::endl;

                    delete temp;
                }
            }

            for (Data::Mesh::ElementIterator iter = mesh->eBegin();
                 iter != mesh->eEnd(); iter = mesh->erase(iter))
            {
                MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

                this->mesh->add(e);

#if USE_THREAD_COLOR
                e->setColor(r, g, b);
#endif //#if USE_THREAD_COLOR

                //Data::cout() << "el " << (*iter)->getId() << " at " << (*iter) << Data::endl;
                //Data::cout() << "adding el to mesh1" << Data::endl;

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                    if (!v->have(e))
                    {
                        v->add(e);
                    }
                }

                Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(e);

                while (!adjs.empty())
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(adjs.front());

                    adjs.pop_front();

                    if (!fel->haveAdjacent(e))
                    {
                        fel->add(e);
                    }
                }
            }

            delete mesh;
        }

        tempBoundary.clear();

        delete [] mts;
    }
    else if (comm->isSlave())
    {
        PMJMesh::MeshTransferable *mt = new PMJMesh::MeshTransferable(this->dimension, this->mesh);

        comm->gather(mt, comm->root());

        mt->setMesh(NULL);

        delete mt;

        if (this->mesh)
        {
            this->removeBoundaryFromMesh();

#if USE_GUI
            this->mesh->unfreeze();
#endif //#if USE_GUI

            delete this->mesh;

            //Data::cout() << "D Proc " << this->comm->rank() << ", mesh at " << this->mesh << Data::endl;

            this->mesh = NULL;

            //Data::cout() << "E Proc " << this->comm->rank() << ", mesh at " << this->mesh << Data::endl;
        }
    }
}

void PMJMesh2::ParallelMeshGenerator::finalizeDistributedMeshes()
{
    if ((!this->comm->isSlave()) || (!this->mesh))
    {
        return;
    }

    for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
         iter != this->mesh->feEnd(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

            if (!v->have(fel))
            {
                v->add(fel);
            }
        }
    }

    for (Data::Mesh::ElementIterator iter = this->mesh->eBegin();
         iter != this->mesh->eEnd(); iter++)
    {
        MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

        for (UInt i = 0; i < e->numVertices(); i++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

            if (!v->have(e))
            {
                v->add(e);
            }
        }

        Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(e);

        while (!fels.empty())
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

            fels.pop_front();

            if (!fel->haveAdjacent(e))
            {
                fel->add(e);
            }
        }
    }
}
#endif //#if USE_MPI

#if USE_DELETE_FELS
void PMJMesh2::ParallelMeshGenerator::finalizeClearMesh()
{
    if (!this->mesh)
    {
        return;
    }

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
}
#endif //#if USE_DELETE_FELS

PMJMesh::Front *PMJMesh2::ParallelMeshGenerator::makeFront(const MJMesh::Boundary *boundary) const
{
    PMJMesh::Front *front = new PMJMesh::Front(this->boundarySorted);

    for (Data::Boundary::ConstVertexIterator iter = boundary->vBegin();
		 iter != boundary->vEnd(); iter++)
	{
		front->add((*iter));
	}

	for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
		 iter != boundary->eEnd(); iter++)
	{
		front->addBoundary((*iter));
	}

	return front;
}

Performer::IdManager *PMJMesh2::ParallelMeshGenerator::makeIdManager(const Parallel::TMCommunicator *comm, Int id) const
{
    UInt numProcs = comm->numProcesses();
    UInt rank = comm->rank();

    /*if (numProcs > 1)
    {
        numProcs--;
        rank--;
    }*/

    ULInt procOffset = rank*this->idrange;

    this->idoffset = numProcs*this->idrange;
    ULInt tidrange = this->idrange/comm->getMaxThreads();

    Performer::RangedIdManager *manager = new Performer::RangedIdManager(1, 1, 1, 1, 3);

    ULInt threadOffset = id*tidrange;

    manager->setRange(tidrange);
    manager->setOffset(this->idoffset);

    manager->setMin(0, this->idManager->getId(0) + procOffset + threadOffset);
    manager->setMin(1, this->idManager->getId(1) + procOffset + threadOffset);
    manager->setMin(2, this->idManager->getId(2) + procOffset + threadOffset);

    return manager;
}

const PMJMesh2::Task *PMJMesh2::ParallelMeshGenerator::cast(const Parallel::Task *t) const
{
    return static_cast<const PMJMesh2::MJMeshTask *>(t);
}

PMJMesh2::Task *PMJMesh2::ParallelMeshGenerator::cast(Parallel::Task *t) const
{
    return static_cast<PMJMesh2::MJMeshTask *>(t);
}

bool PMJMesh2::ParallelMeshGenerator::setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
    Data::BoxTree *tree, Performer::IdManager *idManager) const
{
    //this if is to deal with the tasks in the shared mode, because some of
	// them may not have front
	if (static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront())
	{
		MJMesh::Front *front = static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront();

		front->setSorted(this->boundarySorted);
	}

	//std::cout << "num fels in subfront = " << static_cast<PMJMesh::Partition *>(t->getPartition())->getSubfront()->size() << std::endl;

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

	PMJMesh2::WorkerMainDrive *drive = dynamic_cast<PMJMesh2::WorkerMainDrive *>(t->getBuilder());

	if (!drive)
    {
        t->makeBuilder();

        drive = dynamic_cast<PMJMesh2::WorkerMainDrive *>(t->getBuilder());
    }

    drive->setDimension(this->dimension);
    drive->setBoundary(boundary);
    drive->setTree(tree);

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
    drive->setFixateFront((this->updateType == FULL) || (static_cast<Data::TreeNode *>(this->bsp->node(t->getId()))->isLeaf()));

	if (!t->getComm())
	{
		t->setComm(this->comm);
	}

	return true;
}

void PMJMesh2::ParallelMeshGenerator::getParameters(const Parallel::Task *t) const
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	drive->setIdManager(NULL);
}

bool PMJMesh2::ParallelMeshGenerator::isInError(const Parallel::Task *t) const
{
	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

	bool inError = (drive->getStatus().getType() >= drive->error.getType());

	if (inError)
	{
		Data::cout() << "ERROR: Some error occurred in task " << t->getId() << ": " << drive->getStatus().getMessage() << Data::endl;

		this->status.append(drive->getStatus());
	}

	return inError;
}

#if USE_OPENMP
void PMJMesh2::ParallelMeshGenerator::setLock(PartitionBSPTreeCell *cell, bool set) const
{
    if (set)
    {
        cell->getLock().set();
    }
    else
    {
        cell->getLock().unset();
    }

    for (UInt i = 0; i < cell->numChildren(); i++)
    {
        this->setLock(static_cast<PartitionBSPTreeCell *>(cell->child(i)), set);
    }
}
#endif //#if USE_OPENMP

//#if USE_OPENMP
PartitionBSPTreeCell *PMJMesh2::ParallelMeshGenerator::findCommonParent() const
{
    return this->findCommonParent(0, static_cast<UInt>(this->tasks.size()));
}

PartitionBSPTreeCell *PMJMesh2::ParallelMeshGenerator::findCommonParent(UInt min, UInt max) const
{
    if (max == min + 1)
    {
        return this->cast(this->tasks[min])->getCell();
    }

    PartitionBSPTreeCell *commonParent = static_cast<PartitionBSPTreeCell *>(this->bsp->getRoot());

    while (true)
    {
        bool allDescendants[commonParent->numChildren()];

        for (UInt i = 0; i < commonParent->numChildren(); i++)
        {
            allDescendants[i] = true;
        }

        for (UInt j = min; j < max; j++)
        {
            bool shouldBreak = true;

            for (UInt i = 0; i < commonParent->numChildren(); i++)
            {
                if ((allDescendants[i]) &&
                    (!static_cast<PartitionBSPTreeCell *>(commonParent->child(i))->isDescendant(this->tasks[j]->getId())))
                {
                    allDescendants[i] = false;
                }

                if (allDescendants[i])
                {
                    shouldBreak = false;
                }
            }

            if (shouldBreak)
            {
                break;
            }
        }

        bool found = false;

        for (UInt i = 0; i < commonParent->numChildren(); i++)
        {
            if (allDescendants[i])
            {
                found = true;

                commonParent = static_cast<PartitionBSPTreeCell *>(commonParent->child(i));

                break;
            }
        }

        if (!found)
        {
            break;
        }
    }

    return commonParent;
}
//#endif //#if USE_OPENMP

void PMJMesh2::ParallelMeshGenerator::executeFrontPartitioning()
{
    this->front = this->makeFront(this->boundary);

    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    UInt numThreads = comm->getMaxThreads();

#if USE_C__11
    Data::FrontElementHash *rejecteds[numThreads*this->numTasksPerThread];
#else
    Data::FrontElementSet *rejecteds[numThreads*this->numTasksPerThread];
#endif //#if USE_C__11

    bool addInRejected = ((!static_cast<PMJMesh2::Communicator *>(this->comm)->isSharedParallelismEnabled()) && (this->numTasksPerThread == 1));

#if USE_OPENMP
    #pragma omp parallel if (comm->isSharedParallelismEnabled())
#endif //#if USE_OPENMP
    //for (UInt i = 0; i < this->tasks.size(); i++)
    {
        Int id = comm->threadId();
        //Int id = static_cast<Int>(i);

        for (UInt i = 0; i < this->numTasksPerThread; i++)
        {
            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[id*this->numTasksPerThread + i]);

            FrontPartitioner partitioner(this->dimension, addInRejected, task, this->front);

            partitioner.execute();

            rejecteds[id*this->numTasksPerThread + i] = partitioner.getRejected();

            task->setPartition(task->getPartition());
        }
    }

    if (!addInRejected)
    {
        //for (UInt i = 0; i < numThreads; i++)
        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            PMJMesh::Partition *partition = static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition());
            PMJMesh::Front *front = static_cast<PMJMesh::Front *>(partition->getSubfront());

#if USE_C__11
            for (Data::FrontElementHash::iterator iter = rejecteds[i]->begin();
#else
            for (Data::FrontElementSet::iterator iter = rejecteds[i]->begin();
#endif //#if USE_C__11
                 iter != rejecteds[i]->end(); iter++)
            {
                Data::FrontElement *e = (*iter);

                front->addRejected(e);

/*#if USE_GUI
                if (!partition->boundary.have(e))
                {
                    partition->boundary.add(e);
                }
#endif //#if USE_GUI*/

#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
                    this->rejectedFrontElements.insert(e);

                    /*if ((e->getId() == 1411) || (e->getId() == 1367))
                    {
                        std::cout << "debug PMG1 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                        std::cout << Data::trace() << std::endl;
                    }*/

                    //std::cout << "inserting (1) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                }
#endif //#if USE_MPI

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

/*#if USE_GUI
                    if (!partition->boundary.have(v))
                    {
                        partition->boundary.add(v);
                    }
#endif //#if USE_GUI*/

                    if (!front->have(v))
                    {
                        front->add(v);
                    }
                }
            }

            delete rejecteds[i];

            rejecteds[i] = NULL;

            this->tasks[i]->setPartition(partition);
        }

//#if USE_OPENMP
        this->commonParent = this->findCommonParent();

        this->replacer = new PMJMesh2::CrossingReplacer(this->dimension, true, this->bsp,
            this->commonParent, const_cast<MJMesh::Boundary *>(this->boundary), &this->tasks);

        this->replacer->execute();
//#endif //#if USE_OPENMP
    }
    else
    {
        if (rejecteds[0])
        {
#if USE_MPI
            if (this->updateType != PMJMesh2::FULL)
            {
#if USE_C__11
                for (Data::FrontElementHash::iterator iter = rejecteds[0]->begin();
#else
                for (Data::FrontElementSet::iterator iter = rejecteds[0]->begin();
#endif //#if USE_C__11
                     iter != rejecteds[0]->end(); iter++)
                {
                    Data::FrontElement *e = (*iter);

                    this->rejectedFrontElements.insert(e);

                    /*if ((e->getId() == 1411) || (e->getId() == 1367))
                    {
                        std::cout << "debug PMG2 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                        std::cout << Data::trace() << std::endl;
                    }*/

                    //std::cout << "inserting (2) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                }
            }
#endif //#if USE_MPI

            delete rejecteds[0];

            rejecteds[0] = NULL;
        }
    }

#if USE_GUI
    if (Data::isStepByStep())
    {
        Int nProcesses = 1;
        Int nThreads = 1;
        Int rank = 0;
#if USE_MPI
        nProcesses = this->comm->numProcesses();
        rank = this->comm->rank();
#endif //#if USE_MPI
#if USE_OPENMP
        nThreads = static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads();
#endif //#if USE_OPENMP

        Int color = rank;

        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            PMJMesh::Partition *partition = static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition());
            PMJMesh::Front *front = static_cast<PMJMesh::Front *>(partition->getSubfront());

            MJMesh::Mesh *mesh = new MJMesh::Mesh();

            static_cast<PMJMesh::MJMeshTask *>(this->tasks[i])->setMesh(mesh);

            Real r, g, b;
            Data::pallete(color, r, g, b);

            for (UShort state = 0; state < 4; state++)
            {
                for (Data::Front::FrontElementIterator iter = front->eBegin(state);
                     iter != front->eEnd(state); iter++)
                {
                    (*iter)->setColor(r, g, b);
                    mesh->add((*iter));
                }
            }

            for (Data::Front::VertexIterator iter = front->vBegin();
                 iter != front->vEnd(); iter++)
            {
                (*iter)->setColor(r, g, b);
                mesh->add((*iter));
            }

            if (((nProcesses == 1)) &&
                ((nThreads == 1) ||
                 (this->numTasksPerThread == 1) ||
                 (i%this->numTasksPerThread + 1 == this->numTasksPerThread)))
            {
                color++;
            }
        }
    }
#endif //#if USE_GUI

    /*//debug
    for (UInt i = 0; i < this->tasks.size(); i++)
    {
        PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

        if (!task)
        {
            continue;
        }

        PMJMesh::Front *front = static_cast<PMJMesh::Front *>(task->getFront());

        if (!front)
        {
            continue;
        }

        Data::cout() << "end of executeFrontPartitioning on process " << this->comm->rank() << Data::endl;

        ULInt ids[4] = {94655, 94656, 93959, 94654};

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (UShort j = 0; j < 4; j++)
            {
                bool found = false;

                for (Data::Front::ConstFrontElementIterator iter = front->eBegin(j);
                     iter != front->eEnd(j); iter++)
                {
                    if ((*iter)->getId() == ids[k])
                    {
                        Data::cout() << "found fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    break;
                }
            }
        }

        PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

        if (!drive)
        {
            continue;
        }

        MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive->getBoundary()));

        if (!boundary)
        {
            continue;
        }

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
                 iter != boundary->eEnd(); iter++)
            {
                if ((*iter)->getId() == ids[k])
                {
                    Data::cout() << "found fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                    break;
                }
            }
        }
    }
    //endebug*/

    this->status = this->frontPartitioned;
}

bool PMJMesh2::ParallelMeshGenerator::removeFromBoundary(MJMesh::Vertex *v,
    MJMesh::Mesh *newMesh, MJMesh::Boundary *newBoundary, bool fromMap, PMJMesh::Front *front) const
{
    if ((this->boundary->have(v)) ||
        (v->numAdjacentElements() == 0) ||
        (v->numAdjacentFrontElements() == 0))
    {
        /*if ((v->getId() == 40) || (v->getId() == 41))
        {
            Data::cout() << "vertex " << v->text() << " at " << v << " is in this->boundary or has no adj fels or has no adj els" << Data::endl;
        }*/

        return false;
    }

    //bool oneFelRemoved = false;

    /*if ((v->getId() == 40) || (v->getId() == 41))
    {
        Data::cout() << "removeFromBoundary v " << v->text() << Data::endl;
    }*/

    for (Data::FrontElementList::iterator iter2 = v->feBegin();
         iter2 != v->feEnd();)
    {
        MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter2));

        /*if ((v->getId() == 40) || (v->getId() == 41))
        {
            Data::cout() << "test adj fel " << fel->text() << Data::endl;
        }*/

        if (!newBoundary->have(fel))
        {
            /*if ((v->getId() == 40) || (v->getId() == 41))
            {
                Data::cout() << "adj fel " << fel->text() << " is NOT in newBoundary" << Data::endl;
            }*/

            iter2++;

            continue;
        }

        /*if ((v->getId() == 40) || (v->getId() == 41))
        {
            Data::cout() << "adj fel " << fel->text() << " is in newBoundary" << Data::endl;
        }*/

        UInt count = 0;

        for (Data::ElementList::iterator iter3 = v->eBegin();
             (iter3 != v->eEnd()) && (count <= 1); iter3++)
        {
            MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter3));

            /*if ((v->getId() == 40) || (v->getId() == 41))
            {
                Data::cout() << "adj el " << e->text() << Data::endl;
            }*/

            if (!newMesh->have(e))
            {
                /*if ((v->getId() == 40) || (v->getId() == 41))
                {
                    Data::cout() << "adj el is NOT in newMesh" << Data::endl;
                }*/

                continue;
            }

            /*if ((v->getId() == 40) || (v->getId() == 41))
            {
                Data::cout() << "adj el is in newMesh" << Data::endl;
            }*/

            if (fel->haveAdjacent(e))
            {
                /*if ((v->getId() == 40) || (v->getId() == 41))
                {
                    Data::cout() << "adj fel has pointer to adj el" << Data::endl;
                }*/

                count++;

                continue;
            }

            /*if ((v->getId() == 40) || (v->getId() == 41))
            {
                Data::cout() << "adj fel has NO pointer to adj el" << Data::endl;
            }*/

            bool found = true;

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                if (!e->have(fel->getVertex(i)))
                {
                    found = false;

                    /*if ((v->getId() == 40) || (v->getId() == 41))
                    {
                        Data::cout() << "adj el is NOT adjacent to adj fel" << Data::endl;
                    }*/

                    break;
                }
            }

            if (found)
            {
                /*if ((v->getId() == 40) || (v->getId() == 41))
                {
                    Data::cout() << "adj el is adjacent to adj fel" << Data::endl;
                }*/

                count++;

#if (!USE_DELETE_FELS)
                fel->add(e);
#endif //#if (!USE_DELETE_FELS)
            }
        }

        if (count <= 1)
        {
            iter2++;

            /*if ((v->getId() == 40) || (v->getId() == 41))
            {
                Data::cout() << "adj fel " << fel->text() << " has at most one adj el" << Data::endl;
            }*/

            continue;
        }

        //oneFelRemoved = true;

        /*if ((v->getId() == 40) || (v->getId() == 41))
        {
            Data::cout() << "adj fel " << fel->text() << " has at least two adj els" << Data::endl;
        }*/

        newBoundary->remove(fel);

        if ((front) && (front->have(fel)))
        {
            front->remove(fel);
        }

        iter2++;

#if USE_DELETE_FELS
        //Data::cout() << "deleted fel " << fel->text() << Data::endl;

        v->remove(fel);

        newMesh->remove(fel);

        if (fromMap)
        {
            this->bFrontElements.erase(fel->getId());
        }

        delete fel;
#endif //#if USE_DELETE_FELS
    }

    return (v->numAdjacentFrontElements() == 0);

    /*if (!oneFelRemoved)
    {
        return false;
    }

    for (Data::FrontElementList::iterator iter2 = v->feBegin();
         iter2 != v->feEnd(); iter2++)
    {
        MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter2));

        if (newBoundary->have(fel))
        {
            //Data::cout() << "not removed v " << v->text() << Data::endl;

            return false;
        }
    }

    return true;*/
}

#if USE_DELETE_FELS
void PMJMesh2::ParallelMeshGenerator::createNewFrontElements(MJMesh::Vertex *v,
    MJMesh::Mesh *newMesh, MJMesh::Boundary *newBoundary, MJMesh::Mesh *mesh, UInt threadId) const
{
//    if (v->getId() == 484)
//    {
//        //Data::ElementList adjs = v->getAdjacentElements();
//
//        std::cout << "adj els of v " << v->getId() << " at " << v << " are at:" << std::endl;
//
//        for (Data::ElementList::iterator iter2 = --v->eEnd();
//             iter2 != v->eBegin(); iter2++)
//        {
//            std::cout << " " << (*iter2) << std::endl;
//        }
//
//        std::cout << std::endl;
//    }

    for (Data::ElementList::iterator iter2 = v->eBegin();
         iter2 != v->eEnd(); iter2++)
    {
        MJMesh::Element *e1 = static_cast<MJMesh::Element *>((*iter2));

//        if (v->getId() == 484)
//        {
//            Data::cout() << " debug, v " << v->getId() << " at " << v << ", e1 at " << e1 << Data::endl;
//            Data::cout() << " debug, v " << v->getId() << " at " << v << ", e1 " << e1->text() << Data::endl;
//        }

        bool have = newMesh->have(e1);

        Data::ElementList::iterator iter3 = iter2;

        for (++iter3; iter3 != v->eEnd(); iter3++)
        {
            MJMesh::Element *e2 = static_cast<MJMesh::Element *>((*iter3));

            if (newMesh->have(e2) == have)
            {
                continue;
            }

            UInt i = e1->index(v);

            if (this->dimension == 2)
            {
                MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(e1->getVertex((i + 1)%e1->numVertices()));
                MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(e1->getVertex((i + e1->numVertices() - 1)%e1->numVertices()));

                MJMesh::Vertex *o = (e2->have(v1)) ? v1 :
                    ((e2->have(v2)) ? v2 : NULL);

                if (!o)
                {
                    continue;
                }

                MJMesh::FrontElement *fel = NULL;

                for (Data::FrontElementList::iterator iter4 = v->feBegin();
                     iter4 != v->feEnd(); iter4++)
                {
                    if ((*iter4)->have(o))
                    {
                        fel = static_cast<MJMesh::FrontElement *>((*iter4));

                        break;
                    }
                }

                if (!fel)
                {
                    fel = new MJMesh::FrontElement2D(v, o, this->idManagers[threadId]->next(1));

                    if (((have) && (o == v2)) || ((!have) && (o == v1)))
                    {
                        fel->invert();
                    }

                    fel->add(have ? e1 : e2);

                    mesh->add(fel);

                    if (!newBoundary->have(o))
                    {
                        newBoundary->add(o);
                    }
                }

                if (!newBoundary->have(fel))
                {
                    newBoundary->add(fel);
                }
            }
            else
            {
                MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(e1->getVertex((i + 1)%e1->numVertices()));
                MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(e1->getVertex((i + 2)%e1->numVertices()));
                MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(e1->getVertex((i + e1->numVertices() - 1)%e1->numVertices()));

                MJMesh::Vertex *o1 = NULL, *o2 = NULL;

                if (e2->have(v1))
                {
                    o1 = v1;

                    if (e2->have(v2))
                    {
                        o2 = v2;
                    }
                    else if (e2->have(v3))
                    {
                        o2 = v3;
                    }
                }
                else if (e2->have(v2))
                {
                    o1 = v2;

                    if (e2->have(v3))
                    {
                        o2 = v3;
                    }
                }

                if ((!o1) || (!o2))
                {
                    continue;
                }

                MJMesh::FrontElement *fel = NULL;

                for (Data::FrontElementList::iterator iter4 = v->feBegin();
                     iter4 != v->feEnd(); iter4++)
                {
                    if (((*iter4)->have(o1)) && ((*iter4)->have(o2)))
                    {
                        fel = static_cast<MJMesh::FrontElement *>((*iter4));

                        break;
                    }
                }

                if (!fel)
                {
                    fel = new MJMesh::FrontElement3D(v, o1, o2, this->idManagers[threadId]->next(1));

                    if ((((i%2) == 0) && ( have) && (o1 == v1) && (o2 == v3)) ||
                        (((i%2) == 0) && (!have) && (o1 == v1) && (o2 == v2)) ||
                        (((i%2) == 0) && (!have) && (o1 == v2) && (o2 == v3)) ||
                        (((i%2) == 1) && ( have) && (o1 == v1) && (o2 == v2)) ||
                        (((i%2) == 1) && ( have) && (o1 == v2) && (o2 == v3)) ||
                        (((i%2) == 1) && (!have) && (o1 == v1) && (o2 == v3)))
                    {
                        fel->invert();
                    }

                    fel->add(have ? e1 : e2);

                    mesh->add(fel);

                    if (!newBoundary->have(o1))
                    {
                        newBoundary->add(o1);
                    }

                    if (!newBoundary->have(o2))
                    {
                        newBoundary->add(o2);
                    }

                    /*if (fel->getId() == 84082)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", creating new fels, created fel " << fel->text() << " at " << fel << Data::endl;
                    }*/
                }

                if (!newBoundary->have(fel))
                {
                    newBoundary->add(fel);
                }

                /*if (fel->getId() == 84082)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", creating new fels, fel " << fel->text() << " at " << fel << Data::endl;
                }*/
            }
        }
    }
}
#endif //#if USE_DELETE_FELS

void PMJMesh2::ParallelMeshGenerator::mergeFromSharedTask(MJMesh::Mesh *mesh)
{
#if USE_GUI
    mesh->freeze();
#endif //#if USE_GUI

    for (Data::Mesh::VertexIterator iter = mesh->vBegin();
         iter != mesh->vEnd(); iter = mesh->erase(iter))
    {
        Data::Vertex *v = (*iter);

        /*//if (v->getId() == 110927)
        {
            Data::cout() << "rank " << this->comm->rank() << ", trying to merge, vertex: at " << v << Data::endl;
            Data::cout() << "rank " << this->comm->rank() << ", trying to merge, vertex: " << v->text() << Data::endl;
        }*/

        bool binner = !this->boundary->have(v);

        if (!this->mesh->have(v))
        {
            this->mesh->add(v, binner);
        }
        else
        {
            bool minner = this->mesh->haveInner(v);

            if ((binner) && (!minner))
            {
                this->mesh->addInner(v);
            }
            else if ((!binner) && (minner))
            {
                this->mesh->removeInner(v);
            }
        }
    }

    for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
         iter != mesh->feEnd(); iter = mesh->erase(iter))
    {
        Data::FrontElement *fel = (*iter);

        //std::cout << "trying fel at " << fel << ", mesh at " << mesh << std::endl;

        if (!this->mesh->have(fel))
        {
            this->mesh->add(fel);
        }
    }

    for (Data::Mesh::ElementIterator iter = mesh->eBegin();
         iter != mesh->eEnd(); iter = mesh->erase(iter))
    {
        Data::Element *el = (*iter);

        this->mesh->add(el);
    }

#if USE_GUI
    mesh->unfreeze();

    mesh->clearLists();
#endif //#if USE_GUI
}

void PMJMesh2::ParallelMeshGenerator::mergeBoxesAndId(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    Data::Box *box = static_cast<PMJMesh::Partition *>(task1->getPartition())->getBox();
    Data::Box *pbox = parent->box();

    for (UInt i = 0; i < this->dimension; i++)
    {
        box->getMin()->setCoord(i, pbox->getMin()->getCoord(i));
        box->getMax()->setCoord(i, pbox->getMax()->getCoord(i));
    }

    task1->setId(parent->getId());
    task1->getPartition()->setId(parent->getId());
    dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder())->setId(parent->getId());

    //task1->getPartition()->setLoad(this->partitionTreeGenerator->loadInner(this->bsp->node(task1->getId())));
    //task1->getPartition()->setLoad(0.0);
    task1->getPartition()->setLoad(parent->getLoad());

    PMJMesh2::Task *t1 = this->cast(task1);
    PMJMesh2::Task *t2 = this->cast(task2);

    if (t1)
    {
        if (t1->getCell())
        {
            t1->getCell()->setTask(NULL);
        }

        t1->setCell(parent);
    }

    if (t2)
    {
        if (t2->getCell())
        {
            t2->getCell()->setTask(NULL);
        }

        t2->setCell(NULL);
    }

    parent->setTask(task1);
}

void PMJMesh2::ParallelMeshGenerator::mergeCommonFront(PMJMesh::MJMeshTask *task) const
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    PMJMesh::Front *commonFront = drive->getCommonFront();

    if (!commonFront)
    {
        return;
    }

    PMJMesh::Front *oldFront = drive->getOldFront();
    //PMJMesh::Front *commonFront = drive->getCommonFront();
    PMJMesh::Front *newFront = drive->getNewFront();

    /*std::stringstream str;
    str << "rank " << this->comm->rank() << ", task " << task->getId() << Data::endl;
    str << "oldfront->size " << oldFront->size() << Data::endl;
    str << "commonfront->size " << commonFront->size() << Data::endl;
    str << "newfront->size " << newFront->size() << Data::endl;
    str << Data::trace() << Data::endl;
    Data::cout() << str.str();*/

#if USE_GUI
    oldFront->freeze();
    newFront->freeze();
    commonFront->freeze();
#endif //#if USE_GUI

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = commonFront->eBegin(state);
             iter != commonFront->eEnd(state); iter = commonFront->erase(iter, state))
        {
            oldFront->addBoundary((*iter));
            newFront->addBoundary((*iter));
        }
    }

    for (Data::Front::VertexIterator iter = commonFront->vBegin();
         iter != commonFront->vEnd(); iter = commonFront->erase(iter))
    {
        if (!oldFront->have((*iter)))
        {
            oldFront->add((*iter));
        }

        if (!newFront->have((*iter)))
        {
            newFront->add((*iter));
        }
    }

#if USE_GUI
    oldFront->unfreeze();
    newFront->unfreeze();
    commonFront->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh2::ParallelMeshGenerator::move(PMJMesh::Front *from, PMJMesh::Front *to,
    const PMJMesh::Front *exceptFrom, const PMJMesh::Front *exceptTo) const
{

#if USE_GUI
    from->freeze();
    to->freeze();
#endif //#if USE_GUI

	/*//debug
	{
		for (UShort state = 0; state < 4; state++)
        {
            for (Data::Front::ConstFrontElementIterator iter = exceptFrom->eBegin(state);
                 iter != exceptFrom->eEnd(state); iter++)
            {
                Data::cout() << "fel in exceptFrom " << (*iter)->getId() << Data::endl;
            }
        }

        for (UShort state = 0; state < 4; state++)
        {
            for (Data::Front::ConstFrontElementIterator iter = exceptTo->eBegin(state);
                 iter != exceptTo->eEnd(state); iter++)
            {
                Data::cout() << "fel in exceptTo " << (*iter)->getId() << Data::endl;
            }
        }
	}
	//endebug*/

    if (exceptTo)
    {
        for (UShort state = 0; state < 4; state++)
        {
            for (Data::Front::FrontElementIterator iter = to->eBegin(state);
                 iter != to->eEnd(state);)
            {
                MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                //Data::cout() << "fel in to " << fel->getId() << Data::endl;

                if (((exceptTo->have(fel)) && (!from->have(fel))) ||
					((this->boundary->have(fel)) && (fel->numAdjacents() == 1)))
                {
                    iter = to->erase(iter, state);

                    //Data::cout() << "removing from to" << Data::endl;

                    for (UInt i = 0; i < fel->numVertices(); i++)
                    {
                        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

                        if (!to->haveAny(v->getAdjacentFrontElements()))
                        {
                            to->remove(v);
                        }
                    }

                    continue;
                }

                iter++;
            }
        }
    }

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = from->eBegin(state);
             iter != from->eEnd(state); iter = from->erase(iter, state))
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

            //Data::cout() << "fel in from " << fel->getId() << Data::endl;

            if ((!to->have(fel)) &&
                ((!exceptFrom) || (!exceptFrom->have(fel))) &&
				//((!this->boundary->have(fel)) || (fel->getAdjacency().empty())))
				((!this->boundary->have(fel)) || (fel->numAdjacents() == 0)))
            {
            	//Data::cout() << "adding to to" << Data::endl;

                to->add(fel);
            }
        }
    }

    for (Data::Front::VertexIterator iter = from->vBegin();
         iter != from->vEnd(); iter = from->erase(iter))
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        if ((!to->have(v)) &&
            ((!exceptFrom) || (to->haveAny(v->getAdjacentFrontElements()))))
        {
            to->add(v);
        }
    }

#if USE_GUI
    from->unfreeze();
    to->unfreeze();
#endif //#if USE_GUI
}

Data::Box *PMJMesh2::ParallelMeshGenerator::box(PMJMesh::MJMeshTask *task) const
{
    return static_cast<PMJMesh::Partition *>(task->getPartition())->getBox();
}

void PMJMesh2::ParallelMeshGenerator::sharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    if (this->updateType == FULL)
    {
        this->fullSharedMerge(task1, task2, parent);
    }
    else if (this->updateType == PARTIAL)
    {
        this->partialSharedMerge(task1, task2);
    }
    else if (this->updateType == EMPTY)
    {
        this->emptySharedMerge(task1, task2);
    }

    this->mergeBoxesAndId(task1, task2, parent);
}

void PMJMesh2::ParallelMeshGenerator::fullSharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    this->mergeCommonFront(task1);
    this->mergeCommonFront(task2);

    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());
    PMJMesh::WorkerMainDrive *drive2 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task2->getBuilder());

    PMJMesh::Front *oldFront1 = drive1->getOldFront();
    PMJMesh::Front *oldFront2 = drive2->getOldFront();

    PMJMesh::Front *newFront1 = drive1->getNewFront();
    PMJMesh::Front *newFront2 = drive2->getNewFront();

    MJMesh::Boundary *oldBoundary1 = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive1->getBoundary()));
    MJMesh::Boundary *oldBoundary2 = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive2->getBoundary()));

    /*drive1->setCommonFront(NULL);
    drive2->setCommonFront(NULL);

    delete commonFront1;
    delete commonFront2;*/

    /*drive2->setOldFront(NULL);

    delete oldFront2;*/

    newFront1->moveCrossingToRejected();
    newFront1->moveRejectedToElements();

    newFront2->moveCrossingToRejected();
    newFront2->moveRejectedToElements();

    this->move(newFront2, newFront1, oldFront1, oldFront2);
    this->move(oldFront2, oldFront1);

    /*drive2->setNewFront(NULL);

    delete newFront2;*/

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Mesh *mesh2 = const_cast<MJMesh::Mesh *>(drive2->getMesh());

#if USE_GUI
    mesh1->freeze();
    mesh2->freeze();
#endif //#if USE_GUI

    mesh1->merge(mesh2, this->boundary);

#if USE_GUI
    mesh1->unfreeze();
    mesh2->unfreeze();
#endif //#if USE_GUI

    if (parent->getId() == 0)
    {
        drive1->setBoundary(const_cast<MJMesh::Boundary *>(this->boundary));
        drive2->setBoundary(NULL);
    }
    else
    {
        MJMesh::Boundary *newBoundary = new MJMesh::Boundary();

#if USE_GUI
        newBoundary->freeze();
#endif //#if USE_GUI

        drive1->setBoundary(newBoundary);
        drive2->setBoundary(NULL);

        if ((oldBoundary1 == this->boundary) || (oldBoundary2 == this->boundary))
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
                 iter != this->boundary->vEnd(); iter++)
            {
                Data::Vertex *v = (*iter);

                if (mesh1->have(v))
                {
                    newBoundary->add(v);
                }
            }

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
                 iter != this->boundary->eEnd(); iter++)
            {
                Data::FrontElement *fel = (*iter);

                if (mesh1->have(fel))
                {
                    newBoundary->add(fel);
                }
            }
        }
        else
        {
            for (Data::Boundary::VertexIterator iter = oldBoundary1->vBegin();
                 iter != oldBoundary1->vEnd(); iter++)
            {
                Data::Vertex *v = (*iter);

                if ((mesh1->have(v)) && (!newBoundary->have(v)))
                {
                    newBoundary->add(v);
                }
            }

            for (Data::Boundary::FrontElementIterator iter = oldBoundary1->eBegin();
                 iter != oldBoundary1->eEnd(); iter++)
            {
                Data::FrontElement *fel = (*iter);

                if ((mesh1->have(fel)) && (!newBoundary->have(fel)))
                {
                    newBoundary->add(fel);
                }
            }

            for (Data::Boundary::VertexIterator iter = oldBoundary2->vBegin();
                 iter != oldBoundary2->vEnd(); iter++)
            {
                Data::Vertex *v = (*iter);

                if ((mesh1->have(v)) && (!newBoundary->have(v)))
                {
                    newBoundary->add(v);
                }
            }

            for (Data::Boundary::FrontElementIterator iter = oldBoundary2->eBegin();
                 iter != oldBoundary2->eEnd(); iter++)
            {
                Data::FrontElement *fel = (*iter);

                if ((mesh1->have(fel)) && (!newBoundary->have(fel)))
                {
                    newBoundary->add(fel);
                }
            }
        }

        /*for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
             iter != newBoundary->vEnd(); iter++)
        {
            Data::Vertex *v = (*iter);

            Data::Boundary::VertexIterator iter2 = iter;

            for (iter2++; iter2 != newBoundary->vEnd(); iter2++)
            {
                if (v == (*iter2))
                {
                    Data::cout() << "vertex twice in boundary: " << v << ", " << v->getId() << Data::endl;
                }
                else if (v->getId() == (*iter2)->getId())
                {
                    Data::cout() << "vertices with same id in boundary: " << v << ", " << (*iter2) << ", " << v->getId() << Data::endl;
                }
            }

            if (!mesh1->have(v))
            {
                Data::cout() << "vertex not in mesh: " << v << ", " << v->getId() << Data::endl;
            }
            else if (mesh1->haveInner(v))
            {
                Data::cout() << "vertex as inner in mesh: " << v << ", " << v->getId() << Data::endl;
            }
        }*/

#if USE_GUI
        newBoundary->unfreeze();
#endif //#if USE_GUI
    }

    if (oldBoundary1 != this->boundary)
    {
        oldBoundary1->clear();

        delete oldBoundary1;
    }

    if (oldBoundary2 != this->boundary)
    {
        oldBoundary2->clear();

        delete oldBoundary2;
    }
}

void PMJMesh2::ParallelMeshGenerator::partialSharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const
{
    //Data::cout() << "partialSharedMerge tasks " << task1->getId() << " and " << task2->getId() << Data::endl;

    /*if (task1->getId() == 11)
    {
        Data::cout() << "debug" << Data::endl;
    }*/

    this->mergeCommonFront(task1);
    this->mergeCommonFront(task2);

    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());
    PMJMesh::WorkerMainDrive *drive2 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task2->getBuilder());

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Mesh *mesh2 = const_cast<MJMesh::Mesh *>(drive2->getMesh());

    PMJMesh::Front *newFront1 = drive1->getNewFront();
    PMJMesh::Front *newFront2 = drive2->getNewFront();

    PMJMesh::Front *oldFront1 = drive1->getOldFront();
    PMJMesh::Front *oldFront2 = drive2->getOldFront();

#if USE_GUI
    oldFront1->freeze();
    oldFront2->freeze();

    mesh1->freeze();
    mesh2->freeze();
#endif //#if USE_GUI

    MJMesh::Mesh *newMesh = new MJMesh::Mesh();
    MJMesh::Boundary *oldBoundary1 = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive1->getBoundary()));
    MJMesh::Boundary *oldBoundary2 = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive2->getBoundary()));
    MJMesh::Boundary *newBoundary = new MJMesh::Boundary();

    /*//debug
    std::cout << "oldFront1 at " << oldFront1 << ", oldFront2 at " << oldFront2 << std::endl;
    std::cout << "newFront1 at " << newFront1 << ", newFront2 at " << newFront2 << std::endl;
    std::cout << "mesh1 at " << mesh1 << ", mesh2 at " << mesh2 << ", newMesh at " << newMesh << std::endl;
    //endebug*/

    /*//debug
    std::cout << "before splitting" << std::endl;
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront1->eBegin(state);
             iter != oldFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront1->eBegin(state);
             iter != newFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront2->eBegin(state);
             iter != oldFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront2->eBegin(state);
             iter != newFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
         iter != newMesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newMesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    //endebug*/

    //Data::cout() << "partialSharedMerge this->boundary = " << this->boundary << ", newBoundary = " << newBoundary << Data::endl;

#if USE_GUI
    newBoundary->freeze();
#endif //#if USE_GUI

    drive1->setBoundary(newBoundary);
    drive2->setBoundary(NULL);

    /*if (newBoundary == this->boundary)
    {
        newBoundary = new MJMesh::Boundary();

        drive1->setBoundary(newBoundary);
    }
    else
    {
        newBoundary->clear();
    }*/

    //this->move(newFront2, newFront1);

    //Data::VertexList vertices = newFront1->getVertices();

    //Data::FrontList fronts;
    //fronts.push_back(oldFront1);
    //fronts.push_back(oldFront2);

    /*Data::cout() << "before splitting" << Data::endl;

    for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
         iter != mesh1->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task1->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh1->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh2->vBegin();
         iter != mesh2->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task2->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh2->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }*/

    //Data::Box *box1 = static_cast<PMJMesh::Partition *>(task1->getPartition())->getBox();
    //Data::Box *box2 = static_cast<PMJMesh::Partition *>(task2->getPartition())->getBox();
    Data::Box *box1 = this->box(task1);
    Data::Box *box2 = this->box(task2);

    /*if (task1->getId() == 11)
    {
        Data::cout() << "debug splitting task 1" << Data::endl;
    }*/

    newMesh->splitTo(newFront1->getVertices(), this->improvementLayers, *newBoundary, oldBoundary1, this->boundary, box1, newFront1);

    /*//debug
    std::cout << "after first splitTo" << std::endl;
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront1->eBegin(state);
             iter != oldFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront1->eBegin(state);
             iter != newFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront2->eBegin(state);
             iter != oldFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront2->eBegin(state);
             iter != newFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
         iter != newMesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newMesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    //endebug*/

    /*if (task1->getId() == 11)
    {
        Data::cout() << "debug splitting task 2" << Data::endl;
    }*/

    /*{
        const Data::Boundary *boundary = newBoundary;

        for (Data::Boundary::ConstVertexIterator iter = boundary->vBegin();
             iter != boundary->vEnd(); iter++)
        {
            Data::Boundary::ConstVertexIterator iter2 = iter;

            for (iter2++; iter2 != boundary->vEnd(); iter2++)
            {
                if ((*iter2) == (*iter))
                {
                    Data::cout() << "v iguais " << (*iter) << " com id " << (*iter)->getId() << Data::endl;
                }
                else if ((*iter2)->getId() == (*iter)->getId())
                {
                    Data::cout() << "v iguais " << (*iter) << " e " << (*iter2) << " com id " << (*iter)->getId() << Data::endl;
                }
            }
        }

        for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
             iter != boundary->eEnd(); iter++)
        {
            Data::Boundary::ConstFrontElementIterator iter2 = iter;

            for (iter2++; iter2 != boundary->eEnd(); iter2++)
            {
                if ((*iter2) == (*iter))
                {
                    Data::cout() << "e iguais " << (*iter) << " com id " << (*iter)->getId() << Data::endl;
                }
                else if ((*iter2)->getId() == (*iter)->getId())
                {
                    Data::cout() << "e iguais " << (*iter) << " e " << (*iter2) << " com id " << (*iter)->getId() << Data::endl;
                }
            }
        }
    }*/

    /*//debug
    Data::cout() << "after first splitto" << Data::endl;

    for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
         iter != newBoundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        Data::cout() << "testing vertex " << v->text() << Data::endl;

        if ((v->numAdjacentElements() == 0) || (v->numAdjacentFrontElements() == 0))
        {
            Data::cout() << "testing numels " << v->numAdjacentElements() << ", numfels = " << v->numAdjacentFrontElements() << Data::endl;

            continue;
        }

        if (this->boundary->have(v))
        {
            Data::cout() << "inboundary" << Data::endl;

            continue;
        }

        bool all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            Data::cout() << "adj fel " << (*iter2)->text() << Data::endl;

            if (!newBoundary->have((*iter2)))
            {
                Data::cout() << "adj not in this->boundary" << Data::endl;

                all = false;

                break;
            }
        }

        if (!all)
        {
            Data::cout() << "not all adjs in this->boundary" << Data::endl;

            continue;
        }

        all = true;

        for (Data::ElementList::iterator iter2 = v->eBegin();
             iter2 != v->eEnd(); iter2++)
        {
            Data::cout() << "adj el " << (*iter2)->text() << Data::endl;

            if (!newMesh->have((*iter2)))
            {
                Data::cout() << "adj not in newMesh" << Data::endl;

                all = false;

                break;
            }
        }

        if (!all)
        {
            Data::cout() << "not all adjs in newMesh" << Data::endl;

            continue;
        }

        all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            Data::FrontElement *fel = (*iter2);

            Data::cout() << "adj fel " << fel->text() << Data::endl;

            Data::Vertex *v1 = NULL, *v2 = NULL;

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) != v)
                {
                    if (!v1)
                    {
                        v1 = fel->getVertex(i);
                    }
                    else
                    {
                        v2 = fel->getVertex(i);
                    }
                }
            }

            Data::cout() << "adj v1 " << v1->text() << Data::endl;
            Data::cout() << "adj v2 " << v2->text() << Data::endl;

            UInt count = 0;

            for (Data::ElementList::iterator iter3 = v->eBegin();
                 iter3 != v->eEnd(); iter3++)
            {
                Data::Element *e = (*iter3);

                if ((e->have(v1)) && (e->have(v2)))
                {
                    Data::cout() << "adj e have all vs " << e->text() << Data::endl;

                    count++;
                }
            }

            if (count != 1)
            {
                all = false;

                break;
            }
        }

        if (all)
        {
            Data::cout() << "adjs of v are consistent" << Data::endl;

            continue;
        }

        v->highlight();

        Data::cout() << "debug 1, task1 " << task1->getId() << ", task2 " << task2->getId() << ", v " << v->text() << Data::endl;

        //while (true);
    }
    //endebug*/

    newMesh->splitTo(newFront2->getVertices(), this->improvementLayers, *newBoundary, oldBoundary2, this->boundary, box2, newFront2);

    /*//debug
    std::cout << "after second splitTo" << std::endl;
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront1->eBegin(state);
             iter != oldFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront1->eBegin(state);
             iter != newFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront2->eBegin(state);
             iter != oldFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront2->eBegin(state);
             iter != newFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
         iter != newMesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newMesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    //endebug*/

    /*{
        const Data::Boundary *boundary = newBoundary;

        for (Data::Boundary::ConstVertexIterator iter = boundary->vBegin();
             iter != boundary->vEnd(); iter++)
        {
            Data::Boundary::ConstVertexIterator iter2 = iter;

            for (iter2++; iter2 != boundary->vEnd(); iter2++)
            {
                if ((*iter2) == (*iter))
                {
                    Data::cout() << "v iguais " << (*iter) << " com id " << (*iter)->getId() << Data::endl;
                }
                else if ((*iter2)->getId() == (*iter)->getId())
                {
                    Data::cout() << "v iguais " << (*iter) << " e " << (*iter2) << " com id " << (*iter)->getId() << Data::endl;
                }
            }
        }

        for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
             iter != boundary->eEnd(); iter++)
        {
            Data::Boundary::ConstFrontElementIterator iter2 = iter;

            for (iter2++; iter2 != boundary->eEnd(); iter2++)
            {
                if ((*iter2) == (*iter))
                {
                    Data::cout() << "e iguais " << (*iter) << " com id " << (*iter)->getId() << Data::endl;
                }
                else if ((*iter2)->getId() == (*iter)->getId())
                {
                    Data::cout() << "e iguais " << (*iter) << " e " << (*iter2) << " com id " << (*iter)->getId() << Data::endl;
                }
            }
        }
    }*/

    /*//debug
    Data::cout() << "after second splitto" << Data::endl;

    for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
         iter != newBoundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        Data::cout() << "testing vertex " << v->text() << Data::endl;

        if ((v->numAdjacentElements() == 0) || (v->numAdjacentFrontElements() == 0))
        {
            Data::cout() << "testing numels " << v->numAdjacentElements() << ", numfels = " << v->numAdjacentFrontElements() << Data::endl;

            continue;
        }

        if (this->boundary->have(v))
        {
            Data::cout() << "inboundary" << Data::endl;

            continue;
        }

        bool all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            Data::cout() << "adj fel " << (*iter2)->text() << Data::endl;

            if (!newBoundary->have((*iter2)))
            {
                Data::cout() << "adj not in this->boundary" << Data::endl;

                all = false;

                break;
            }
        }

        if (!all)
        {
            Data::cout() << "not all adjs in this->boundary" << Data::endl;

            continue;
        }

        all = true;

        for (Data::ElementList::iterator iter2 = v->eBegin();
             iter2 != v->eEnd(); iter2++)
        {
            Data::cout() << "adj el " << (*iter2)->text() << Data::endl;

            if (!newMesh->have((*iter2)))
            {
                Data::cout() << "adj not in newMesh" << Data::endl;

                all = false;

                break;
            }
        }

        if (!all)
        {
            Data::cout() << "not all adjs in newMesh" << Data::endl;

            continue;
        }

        all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            Data::FrontElement *fel = (*iter2);

            Data::cout() << "adj fel " << fel->text() << Data::endl;

            Data::Vertex *v1 = NULL, *v2 = NULL;

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) != v)
                {
                    if (!v1)
                    {
                        v1 = fel->getVertex(i);
                    }
                    else
                    {
                        v2 = fel->getVertex(i);
                    }
                }
            }

            Data::cout() << "adj v1 " << v1->text() << Data::endl;
            Data::cout() << "adj v2 " << v2->text() << Data::endl;

            UInt count = 0;

            for (Data::ElementList::iterator iter3 = v->eBegin();
                 iter3 != v->eEnd(); iter3++)
            {
                Data::Element *e = (*iter3);

                if ((e->have(v1)) && (e->have(v2)))
                {
                    Data::cout() << "adj e have all vs " << e->text() << Data::endl;

                    count++;
                }
            }

            if (count != 1)
            {
                all = false;

                break;
            }
        }

        if (all)
        {
            Data::cout() << "adjs of v are consistent" << Data::endl;

            continue;
        }

        v->highlight();

        Data::cout() << "debug 2, task1 " << task1->getId() << ", task2 " << task2->getId() << ", v " << v->text() << Data::endl;

        //while (true);
    }
    //endebug*/

    if (oldBoundary1 != this->boundary)
    {
        oldBoundary1->clear();

        delete oldBoundary1;
    }

    if (oldBoundary2 != this->boundary)
    {
        oldBoundary2->clear();

        delete oldBoundary2;
    }

    //mesh1->split(newFront1->getVertices(), this->improvementLayers, newMesh, *newBoundary, this->boundary, true);
    //mesh2->split(newFront2->getVertices(), this->improvementLayers, newMesh, *newBoundary, this->boundary, false);

    this->move(newFront2, newFront1, oldFront1, oldFront2);

    /*//debug
    std::cout << "after move" << std::endl;
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront1->eBegin(state);
             iter != oldFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront1->eBegin(state);
             iter != newFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront2->eBegin(state);
             iter != oldFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront2->eBegin(state);
             iter != newFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
         iter != newMesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newMesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    //endebug*/

    /*Data::cout() << "after splitting" << Data::endl;

    for (Data::Mesh::VertexIterator iter = newMesh->vBegin();
         iter != newMesh->vEnd(); iter++)
    {
        Data::cout() << "newMesh, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (newMesh->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
         iter != mesh1->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task1->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh1->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh2->vBegin();
         iter != mesh2->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task2->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh2->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }*/

    oldFront1->clear();
    oldFront2->clear();

    /*//debug
    for (Data::Boundary::FrontElementIterator iter = newBoundary->eBegin();
         iter != newBoundary->eEnd(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            if (!newBoundary->have(v))
            {
                Data::cout() << "(1) not in boundary " << v->getId() << Data::endl;

                for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
                     iter != newBoundary->vEnd(); iter++)
                {
                    if ((*iter)->getId() == v->getId())
                    {
                        Data::cout() << "  but in boundary " << (*iter)->getId() << Data::endl;

                        break;
                    }
                }
            }
        }
    }
    //endebug*/

    Data::VertexList removed;

    for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
         iter != newBoundary->vEnd(); iter++)
    {
        /*Data::cout() << "newBoundary, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (!newBoundary->have((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }*/

        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        //Data::cout() << "v " << v->getId() << Data::endl;

        //v->setColor(0.0, 1.0, 0.0);

        oldFront1->add(v);

        if (!newMesh->have(v))
        {
            newMesh->add(v, false);
        }
        else if (newMesh->haveInner(v))
        {
            newMesh->removeInner(v);
        }

        if (this->removeFromBoundary(v, newMesh, newBoundary))
        {
            removed.push_back(v);

            oldFront1->remove(v);

            newMesh->addInner(v);

            //Data::cout() << "removed v " << v->text() << Data::endl;

            continue;
        }

#if USE_DELETE_FELS
        this->createNewFrontElements(static_cast<MJMesh::Vertex *>(v), newMesh, newBoundary, mesh1, static_cast<Parallel::TMCommunicator *>(this->comm)->threadId());
#endif //#if USE_DELETE_FELS
    }

    while (!removed.empty())
    {
        newBoundary->remove(removed.front());

        removed.pop_front();
    }

    for (Data::Boundary::FrontElementIterator iter = newBoundary->eBegin();
         iter != newBoundary->eEnd(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        //Data::cout() << "fel " << fel->getId() << Data::endl;

        //fel->setColor(0.0, 1.0, 0.0);

        oldFront1->addBoundary(fel);

        if (!newMesh->have(fel))
        {
            newMesh->add(fel);
        }

        /*//debug
        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            if (!newBoundary->have(v))
            {
                Data::cout() << "not in boundary " << v->getId() << Data::endl;

                for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
                     iter != newBoundary->vEnd(); iter++)
                {
                    if ((*iter)->getId() == v->getId())
                    {
                        Data::cout() << "  but in boundary " << (*iter)->getId() << Data::endl;

                        break;
                    }
                }
            }
        }
        //endebug*/
    }

    //mesh2->merge(mesh1, this->boundary);
    //static_cast<PMJMesh2::MJMeshTask *>(task2)->add(mesh1);
    this->cast(task2)->add(mesh1);

#if USE_GUI
    {
        Data::Point2D mid2d;
        Data::Point3D mid;

        if (this->dimension == 2)
        {
            mid2d = static_cast<PMJMesh::Partition *>(task1->getPartition())->getBox()->mid2D();

            mid.setCoord(0, mid2d.getCoord(0));
            mid.setCoord(1, mid2d.getCoord(1));
            mid.setCoord(2, 0.0);
        }
        else
        {
            mid = static_cast<PMJMesh::Partition *>(task1->getPartition())->getBox()->mid3D();
        }

        this->cast(task2)->add(mid);
    }
#endif //#if USE_GUI

    /*Data::cout() << "after merging" << Data::endl;

    for (Data::Mesh::VertexIterator iter = newMesh->vBegin();
         iter != newMesh->vEnd(); iter++)
    {
        Data::cout() << "newMesh, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (newMesh->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
         iter != mesh1->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task1->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh1->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh2->vBegin();
         iter != mesh2->vEnd(); iter++)
    {
        Data::cout() << "mesh " << task2->getId() << ", vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (((*iter)->getId() == 656) || ((*iter)->getId() == 657) || ((*iter)->getId() == 912) || ((*iter)->getId() == 913))
        {
            if (mesh2->haveInner((*iter)))
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is inner" << Data::endl;
            }
            else
            {
                Data::cout() << "vertex " << (*iter)->getId() << " is NOT inner" << Data::endl;
            }
        }
    }*/

    task1->setMesh(newMesh);

    /*//debug
    Data::cout() << "after creating fels" << Data::endl;

    for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
         iter != newBoundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        if ((v->numAdjacentElements() == 0) || (v->numAdjacentFrontElements() == 0))
        {
            continue;
        }

        if (this->boundary->have(v))
        {
            continue;
        }

        bool all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            if (!newBoundary->have((*iter2)))
            {
                all = false;

                break;
            }
        }

        if (!all)
        {
            continue;
        }

        all = true;

        for (Data::ElementList::iterator iter2 = v->eBegin();
             iter2 != v->eEnd(); iter2++)
        {
            if (!newMesh->have((*iter2)))
            {
                all = false;

                break;
            }
        }

        if (!all)
        {
            continue;
        }

        all = true;

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            Data::FrontElement *fel = (*iter2);

            Data::Vertex *v1 = NULL, *v2 = NULL;

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) != v)
                {
                    if (!v1)
                    {
                        v1 = fel->getVertex(i);
                    }
                    else
                    {
                        v2 = fel->getVertex(i);
                    }
                }
            }

            UInt count = 0;

            for (Data::ElementList::iterator iter3 = v->eBegin();
                 iter3 != v->eEnd(); iter3++)
            {
                Data::Element *e = (*iter3);

                if ((e->have(v1)) && (e->have(v2)))
                {
                    count++;
                }
            }

            if (count != 1)
            {
                all = false;

                break;
            }
        }

        if (all)
        {
            continue;
        }

        v->highlight();

        Data::cout() << "debug 3, task1 " << task1->getId() << ", task2 " << task2->getId() << ", v " << v->text() << Data::endl;

        while (true);
    }
    //endebug*/

#if USE_GUI
    mesh1->unfreeze();
    mesh2->unfreeze();

    oldFront1->unfreeze();
    oldFront2->unfreeze();

    newBoundary->unfreeze();
#endif //#if USE_GUI

    //delete mesh1;

    /*//debug
    std::cout << "after everything" << std::endl;
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront1->eBegin(state);
             iter != oldFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront1->eBegin(state);
             iter != newFront1->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = oldFront2->eBegin(state);
             iter != oldFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "oldFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = newFront2->eBegin(state);
             iter != newFront2->eEnd(state); iter++)
        {
            if ((*iter)->getId() == 186413)
            {
                std::cout << "newFront2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
            }
        }
    }
    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh2 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
         iter != newMesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newMesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }
    //endebug*/
}

void PMJMesh2::ParallelMeshGenerator::emptySharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const
{

}

void PMJMesh2::ParallelMeshGenerator::makeMaps() const
{
    if (!this->bVertices.empty())
    {
        return;
    }

#if USE_C__11
    this->bVertices.reserve(this->boundary->verticesSize());
    this->bFrontElements.reserve(this->boundary->size());
#endif //#if USE_C__11

    for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
         iter != this->boundary->vEnd(); iter++)
    {
        this->bVertices[(*iter)->getId()] = (*iter);
    }

    for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
         iter != this->boundary->eEnd(); iter++)
    {
        this->bFrontElements[(*iter)->getId()] = (*iter);
    }
}

void PMJMesh2::ParallelMeshGenerator::checkAndAddInFront(MJMesh::FrontElement *temp, MJMesh::FrontElement *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const
{
    bool rhave = remote->have(temp);
    bool lhave = local->have(orig ? orig : temp);

    //bool debug = (temp->getId() == 40);
    //bool debug = true;

    /*if (debug)
    {
        Data::cout() << "rank " << this->comm->rank() << ", temp " << temp->text() << Data::endl;

        if (orig)
        {
            Data::cout() << "rank " << this->comm->rank() << ", orig " << orig->text() << Data::endl;
        }

        if (this->boundary->have(temp))
        {
            Data::cout() << "rank " << this->comm->rank() << ", boundary have temp" << Data::endl;
        }
        else
        {
            Data::cout() << "rank " << this->comm->rank() << ", boundary does not have temp" << Data::endl;
        }

        if (orig)
        {
            if (this->boundary->have(orig))
            {
                Data::cout() << "rank " << this->comm->rank() << ", boundary have orig" << Data::endl;
            }
            else
            {
                Data::cout() << "rank " << this->comm->rank() << ", boundary does not have orig" << Data::endl;
            }
        }

        if (rhave)
        {
            Data::cout() << "rank " << this->comm->rank() << ", remote have temp" << Data::endl;
        }
        else
        {
            Data::cout() << "rank " << this->comm->rank() << ", remote does not have temp" << Data::endl;
        }

        if (lhave)
        {
            if (orig)
            {
                Data::cout() << "rank " << this->comm->rank() << ", local have orig" << Data::endl;
            }
            else
            {
                Data::cout() << "rank " << this->comm->rank() << ", local have temp" << Data::endl;
            }
        }
        else
        {
            if (orig)
            {
                Data::cout() << "rank " << this->comm->rank() << ", local does not have orig" << Data::endl;
            }
            else
            {
                Data::cout() << "rank " << this->comm->rank() << ", local does not have temp" << Data::endl;
            }
        }
    }*/

    if ((rhave) && (lhave))
    {
        remote->remove(temp);

        /*if (debug)
        {
            Data::cout() << "rank " << this->comm->rank() << ", removing temp from remote" << Data::endl;
        }*/

        return;
    }

    if ((!rhave) && (!lhave))
    {
        /*if (debug)
        {
            Data::cout() << "rank " << this->comm->rank() << ", doing nothing" << Data::endl;
        }*/

        return;
    }

    if ((rhave) && (!lhave))
    {
        if (orig)
        {
            //local->remove(orig);

            //Data::cout() << "should remove orig fel " << orig->getId() <<  " from local" << Data::endl;

            UInt size = orig->numAdjacents();
            bool have = this->boundary->have(orig);

            if (((size == 0) && (have)) ||
                ((size == 1) && (!have)))
            {
                if ((have) || (remote->have(temp, 0)))
                {
                    local->addBoundary(orig);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding orig to local boundary" << Data::endl;
                    }*/
                }
                else if (remote->have(temp, 1))
                {
                    local->addElement(orig);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding orig to local elements" << Data::endl;
                    }*/
                }
                else if (remote->have(temp, 2))
                {
                    local->addRejected(orig);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding orig to local rejected" << Data::endl;
                    }*/
                }
                else if (remote->have(temp, 3))
                {
                    local->addCrossing(orig);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding orig to local crossing" << Data::endl;
                    }*/
                }
            }
            /*else if (debug)
            {
                Data::cout() << "rank " << this->comm->rank() << ", doing nothing with orig" << Data::endl;
            }*/
        }
        else
        {
            if (remote->have(temp, 0))
            {
                local->addBoundary(temp);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", adding temp to local boundary" << Data::endl;
                }*/
            }
            else if (remote->have(temp, 1))
            {
                local->addElement(temp);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", adding temp to local elements" << Data::endl;
                }*/
            }
            else if (remote->have(temp, 2))
            {
                local->addRejected(temp);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", adding temp to local rejected" << Data::endl;
                }*/
            }
            else if (remote->have(temp, 3))
            {
                local->addCrossing(temp);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", adding temp to local crossing" << Data::endl;
                }*/
            }
        }

        remote->remove(temp);

        /*if (debug)
        {
            Data::cout() << "rank " << this->comm->rank() << ", removing temp from remote" << Data::endl;
        }*/

        return;
    }

    if ((!rhave) && (lhave))
    {
        //if (!orig->getAdjacency().empty())
        if (orig->numAdjacents() != 0)
        {
            local->remove(orig);

            /*if (debug)
            {
                Data::cout() << "rank " << this->comm->rank() << ", removing orig from local" << Data::endl;
            }*/
        }
        /*else if (debug)
        {
            Data::cout()<< "rank " << this->comm->rank() << ", doing nothing" << Data::endl;
        }*/
    }

    /*if ((!remote) || (!remote->have(temp)))
    {
        return;
    }

    if (!local)
    {
        if (orig)
        {
            remote->remove(temp);
        }

        return;
    }

    if ((orig) && (local->have(orig)))
    {
        return;
    }

    UShort state = 4;

    for (state = 0; state < 4; state++)
    {
        if (remote->have(temp, state))
        {
            break;
        }
    }

    MJMesh::FrontElement *add = orig ? orig : temp;

    switch (state)
    {
    case 0:
        local->addBoundary(add);
        break;
    case 1:
        local->addElement(add);
        break;
    case 2:
        local->addRejected(add);
        break;
    case 3:
        local->addCrossing(add);
        break;
    }*/
}

void PMJMesh2::ParallelMeshGenerator::checkAndAddInFront(MJMesh::Vertex *temp, MJMesh::Vertex *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const
{
    bool rhave = remote->have(temp);
    bool lhave = local->have(orig ? orig : temp);

    /*if (rhave)
    {
        Data::cout() << "remote have temp" << Data::endl;
    }
    else
    {
        Data::cout() << "remote does not have temp" << Data::endl;
    }

    if (lhave)
    {
        if (orig)
        {
            Data::cout() << "local have orig" << Data::endl;
        }
        else
        {
            Data::cout() << "local have temp" << Data::endl;
        }
    }
    else
    {
        if (orig)
        {
            Data::cout() << "local does not have orig" << Data::endl;
        }
        else
        {
            Data::cout() << "local does not have temp" << Data::endl;
        }
    }*/

    if ((rhave) && (lhave))
    {
        remote->remove(temp);

        //Data::cout() << "removing temp from remote" << Data::endl;

        return;
    }

    if ((!rhave) && (!lhave))
    {
        //Data::cout() << "doing nothing" << Data::endl;

        return;
    }

    if ((rhave) && (!lhave))
    {
        if (orig)
        {
            if (local->haveAny(orig->getAdjacentFrontElements()))
            {
                local->add(orig);
            }

            //if (local->shouldRemove(orig))
            //{
                //local->remove(orig);

                //Data::cout() << "should remove orig v " << orig->getId() <<  " from local" << Data::endl;
            //}
        }
        else
        {
            local->add(temp);

            //Data::cout() << "adding temp to local" << Data::endl;
        }

        remote->remove(temp);

        //Data::cout() << "removing temp from remote" << Data::endl;

        return;
    }

    if ((!rhave) && (lhave))
    {
        if (local->shouldRemove(orig))
        {
            local->remove(orig);

            //Data::cout() << "removing orig from local" << Data::endl;
        }
    }

    /*if ((!remote) || (!remote->have(temp)))
    {
        return;
    }

    if (!local)
    {
        if (orig)
        {
            remote->remove(temp);
        }

        return;
    }

    if ((orig) && (local->have(orig)))
    {
        return;
    }

    local->add(orig ? orig : temp);*/
}

void PMJMesh2::ParallelMeshGenerator::distributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    if (this->updateType == FULL)
    {
        this->fullDistributedMerge(task1, task2, parent);
    }
    else if (this->updateType == PARTIAL)
    {
        this->partialDistributedMerge(task1, task2);
    }
    else if (this->updateType == EMPTY)
    {
        this->emptyDistributedMerge(task1, task2);
    }

    //Data::cout() << "deleting task2 at " << task2 << Data::endl;

    delete task2;

    //Data::cout() << "ended deleting task2" << Data::endl;

    this->mergeBoxesAndId(task1, NULL, parent);
}

void PMJMesh2::ParallelMeshGenerator::fullDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    this->makeMaps();

    //Data::cout() << "task1 = " << task1 << ", task2 = " << task2 << Data::endl;

    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());
    PMJMesh::WorkerMainDrive *drive2 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task2->getBuilder());

    //Data::cout() << "drive1 = " << drive1 << ", drive2 = " << drive2 << Data::endl;
    //Data::cout() << "front1 = " << task1->getFront() << ", front2 = " << task2->getFront() << Data::endl;

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Mesh *mesh2 = const_cast<MJMesh::Mesh *>(drive2->getMesh());

    //Data::cout() << "mesh1 = " << mesh1 << ", mesh2 = " << mesh2 << Data::endl;

    PMJMesh::Front *front1 = task1->getFront();
    PMJMesh::Front *front2 = task2->getFront();

    if (!drive2->getFront())
    {
        drive2->setFront(front2);
    }

    //Data::cout() << "newFront1 = " << newFront1 << ", newFront2 = " << newFront2 << Data::endl;

    //PMJMesh::Front *commonFront1 = drive1->getCommonFront();
    //PMJMesh::Front *commonFront2 = drive2->getCommonFront();

    //Data::cout() << "commonFront1 = " << commonFront1 << ", commonFront2 = " << commonFront2 << Data::endl;

    //PMJMesh::Front *oldFront1 = drive1->getOldFront();
    //PMJMesh::Front *oldFront2 = drive2->getOldFront();

    //Data::cout() << "oldFront1 = " << oldFront1 << ", oldFront2 = " << oldFront2 << Data::endl;

    MJMesh::Boundary *newBoundary = NULL;

    if (this->createBoundaryInFullDistributedMerge)
    {
        MJMesh::Boundary *oldBoundary1 = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive1->getBoundary()));

        if (parent->getId() == 0)
        {
            drive1->setBoundary(const_cast<MJMesh::Boundary *>(this->boundary));

            if (oldBoundary1 != this->boundary)
            {
                oldBoundary1->clear();

                delete oldBoundary1;
            }
        }
        else if (oldBoundary1 != this->boundary)
        {
            newBoundary = oldBoundary1;
        }
        else
        {
            newBoundary = new MJMesh::Boundary();

            drive1->setBoundary(newBoundary);

            if (oldBoundary1->size() < mesh1->size())
            {
                for (Data::Boundary::FrontElementIterator iter = oldBoundary1->eBegin();
                     iter != oldBoundary1->eEnd(); iter++)
                {
                    Data::FrontElement *fel = (*iter);

                    if (mesh1->have(fel))
                    {
                        newBoundary->add(fel);
                    }
                }
            }
            else
            {
                for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
                     iter != mesh1->feEnd(); iter++)
                {
                    Data::FrontElement *fel = (*iter);

                    if (oldBoundary1->have(fel))
                    {
                        newBoundary->add(fel);
                    }
                }
            }

            if (oldBoundary1->verticesSize() < mesh1->verticesSize())
            {
                for (Data::Boundary::VertexIterator iter = oldBoundary1->vBegin();
                     iter != oldBoundary1->vEnd(); iter++)
                {
                    Data::Vertex *v = (*iter);

                    if (mesh1->have(v))
                    {
                        newBoundary->add(v);
                    }
                }
            }
            else
            {
                for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
                     iter != mesh1->vEnd(); iter++)
                {
                    Data::Vertex *v = (*iter);

                    if (oldBoundary1->have(v))
                    {
                        newBoundary->add(v);
                    }
                }
            }
        }
    }

    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd();)
    {
        MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));
        MJMesh::FrontElement *orig = NULL;

        //Data::cout() << "temp fel " << temp->getId() << " at " << temp << Data::endl;

#if USE_C__11
        Data::FrontElementHashMap::iterator it = this->bFrontElements.find(temp->getId());
#else
        Data::FrontElementMap::iterator it = this->bFrontElements.find(temp->getId());
#endif //#if USE_C__11

        if (it == this->bFrontElements.end())
        {
            mesh1->add(temp);

            //Data::cout() << "adding temp to mesh1" << Data::endl;
        }
        else
        {
            orig = static_cast<MJMesh::FrontElement *>((*it).second);

            //Data::cout() << "orig fel " << orig->getId() << " at " << orig << Data::endl;

            //Data::ElementList els = temp->getAdjacency();

            //while (!els.empty())
            for (Data::ElementList::iterator iter = temp->eBegin();
                 iter != temp->eEnd(); iter = temp->erase(iter))
            {
                //MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
                MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

                //els.pop_front();

                //temp->remove(e);
                orig->add(e);
            }

            for (UInt i = 0; i < temp->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(temp->getVertex(i));

                if (v)
                {
                    v->remove(temp);
                }

                temp->setVertex(i, NULL);
            }

            if (!mesh1->have(orig))
            {
                mesh1->add(orig);

                //Data::cout() << "adding orig to mesh1" << Data::endl;
            }

            if ((newBoundary) && (!newBoundary->have(orig)))
            {
                newBoundary->add(orig);
            }
        }

        //this->checkAndAddInFront(temp, orig, oldFront2, oldFront1);
        //this->checkAndAddInFront(temp, orig, commonFront2, commonFront1);
        //this->checkAndAddInFront(temp, orig, newFront2, newFront1);
        this->checkAndAddInFront(temp, orig, front2, front1);

        iter = mesh2->erase(iter);

        //Data::cout() << "removing temp from mesh2" << Data::endl;

        if (orig)
        {
            //Data::cout() << "deleting temp" << Data::endl;

            delete temp;
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh2->vBegin();
         iter != mesh2->vEnd();)
    {
        MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));
        MJMesh::Vertex *orig = NULL;

        //Data::cout() << "temp v " << temp->getId() << " at " << temp << Data::endl;

#if USE_C__11
        Data::VertexHashMap::iterator it = this->bVertices.find(temp->getId());
#else
        Data::VertexMap::iterator it = this->bVertices.find(temp->getId());
#endif //#if USE_C__11

        if (it == this->bVertices.end())
        {
            mesh1->add(temp);

            //Data::cout() << "adding temp to mesh1" << Data::endl;
        }
        else
        {
            orig = static_cast<MJMesh::Vertex *>((*it).second);

            //Data::cout() << "orig v " << orig->getId() << " at " << orig << Data::endl;

            //Data::ElementList els = temp->getAdjacentElements();

            //while (!els.empty())
            for (Data::ElementList::iterator iter = temp->eBegin();
                 iter != temp->eEnd(); iter = temp->erase(iter))
            {
                //Data::Element *e = els.front();
                Data::Element *e = (*iter);

                //els.pop_front();

                e->replace(temp, orig);

                //temp->remove(e);

                orig->add(e);
            }

            //Data::FrontElementList fels = temp->getAdjacentFrontElements();

            //Data::FrontElementList defFEls = orig->getAdjacentFrontElements();

            //while (!fels.empty())
            for (Data::FrontElementList::iterator iter = temp->feBegin();
                 iter != temp->feEnd(); iter = temp->erase(iter))
            {
                //Data::FrontElement *e = fels.front();
                Data::FrontElement *e = (*iter);

                //fels.pop_front();

                /*bool found = false;

                for (Data::FrontElementList::iterator iter = defFEls.begin();
                     iter != defFEls.end(); iter++)
                {
                    if ((*iter)->getId() == e->getId())
                    {
                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    continue;
                }*/

                e->replace(temp, orig);

                //temp->remove(e);

                orig->add(e);
            }

            if (!mesh1->have(orig))
            {
                mesh1->add(orig, false);

                //Data::cout() << "adding orig to mesh1" << Data::endl;
            }

            if ((newBoundary) && (!newBoundary->have(orig)))
            {
                newBoundary->add(orig);
            }
        }

        //this->checkAndAddInFront(temp, orig, oldFront2, oldFront1);
        //this->checkAndAddInFront(temp, orig, commonFront2, commonFront1);
        //this->checkAndAddInFront(temp, orig, newFront2, newFront1);
        this->checkAndAddInFront(temp, orig, front2, front1);

        iter = mesh2->erase(iter);

        //Data::cout() << "removing temp from mesh2" << Data::endl;

        if (orig)
        {
            //Data::cout() << "deleting temp" << Data::endl;

            delete temp;
        }
    }

    for (Data::Mesh::ElementIterator iter = mesh2->eBegin();
		 iter != mesh2->eEnd(); iter = mesh2->erase(iter))
	{
		mesh1->add((*iter));

		//Data::cout() << "el " << (*iter)->getId() << " at " << (*iter) << Data::endl;
		//Data::cout() << "adding el to mesh1" << Data::endl;
	}

    /*Data::cout() << "after merging" << Data::endl;

    for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
         iter != mesh1->vEnd(); iter++)
    {
        Data::cout() << "mesh1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (front1->have((*iter)))
        {
            Data::cout() << "front1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        Data::cout() << "mesh1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (front1->have((*iter)))
        {
            Data::cout() << "front1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (Data::Mesh::ElementIterator iter = mesh1->eBegin();
         iter != mesh1->eEnd(); iter++)
    {
        Data::cout() << "mesh1, element " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
    }

    for (Data::Front::VertexIterator iter = front1->vBegin();
         iter != front1->vEnd(); iter++)
    {
        Data::cout() << "front1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (!mesh1->have((*iter)))
        {
            Data::cout() << "NOT mesh1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = front1->eBegin(state);
             iter != front1->eEnd(state); iter++)
        {
            Data::cout() << "front1, fel " << (*iter)->getId() << ", at " << (*iter) << ", in state " << static_cast<UInt>(state) << Data::endl;

            if (!mesh1->have((*iter)))
            {
                Data::cout() << "NOT mesh1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
            }
        }
    }*/

	front2->clear();

	task2->setMesh(NULL);

	delete mesh2;
}

void PMJMesh2::ParallelMeshGenerator::partialDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const
{
    this->makeMaps();

    //Data::cout() << "merging task1 " << task1->getId() << " with task2 " << task2->getId() << Data::endl;
    //Data::cout() << "task1 = " << task1 << ", task2 = " << task2 << Data::endl;

    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());
    PMJMesh::WorkerMainDrive *drive2 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task2->getBuilder());

    //Data::cout() << "drive1 = " << drive1 << ", drive2 = " << drive2 << Data::endl;
    //Data::cout() << "front1 = " << task1->getFront() << ", front2 = " << task2->getFront() << Data::endl;

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Mesh *mesh2 = const_cast<MJMesh::Mesh *>(drive2->getMesh());

    //Data::cout() << "mesh1 = " << mesh1 << ", mesh2 = " << mesh2 << Data::endl;

    PMJMesh::Front *front1 = task1->getFront();
    PMJMesh::Front *front2 = task2->getFront();

    if (!drive2->getFront())
    {
        drive2->setFront(front2);
    }

    //Data::cout() << "newFront1 = " << newFront1 << ", newFront2 = " << newFront2 << Data::endl;

    //PMJMesh::Front *commonFront1 = drive1->getCommonFront();
    //PMJMesh::Front *commonFront2 = drive2->getCommonFront();

    //Data::cout() << "commonFront1 = " << commonFront1 << ", commonFront2 = " << commonFront2 << Data::endl;

    PMJMesh::Front *oldFront1 = drive1->getOldFront();
    //PMJMesh::Front *oldFront2 = drive2->getOldFront();

    //Data::cout() << "oldFront1 = " << oldFront1 << ", oldFront2 = " << oldFront2 << Data::endl;
    //Data::cout() << "oldFront1 = " << oldFront1 << Data::endl;

    MJMesh::Boundary *newBoundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive1->getBoundary()));

    oldFront1->clear();

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = front1->eBegin(state);
        	 iter != front1->eEnd(state);)
//#if USE_C__11
//        for (Data::FrontElementHash::const_iterator iter = this->rejectedFrontElements.begin();
//#else
//        for (Data::FrontElementSet::const_iterator iter = this->rejectedFrontElements.begin();
//#endif //#if USE_C__11
//             iter != this->rejectedFrontElements.end(); iter++)
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

            /*if (fel->getId() == 40)
            {
                std::cout << "debug partialDistributedMerge front1 " << fel->getId() << " at " << fel << std::endl;
            }*/

            /*{
                std::stringstream str;
                str << "rank " << this->comm->rank() << " fel from rejectedFrontElements at " << fel << Data::endl;
                str << "rank " << this->comm->rank() << " fel from rejectedFrontElements " << fel->text() << Data::endl;
                Data::cout() << str.str();
            }

            if (this->replacer)
            {
                MJMesh::FrontElement *fel2 = static_cast<MJMesh::FrontElement *>(this->replacer->getTemporaryFrontElement(task1->getId(), fel->getId()));

                {
                    std::stringstream str;
                    str << "rank " << this->comm->rank() << " fel from replacer at " << fel2 << Data::endl;
                    Data::cout() << str.str();
                }

                if (fel2)
                {
                    fel = fel2;

                    {
                        std::stringstream str;
                        str << "rank " << this->comm->rank() << " fel from replacer " << fel->text() << Data::endl;
                        Data::cout() << str.str();
                    }
                }
            }

            if (!front1->have(fel))
            {
                continue;
            }*/

            /*{
                std::stringstream str;
                str << "rank " << this->comm->rank() << " fel from front1 " << fel->text() << Data::endl;
                Data::cout() << str.str();
            }*/

            //if ((front2->have(fel)) || (!static_cast<PMJMesh2::MJMeshTask *>(task2)->isAdvanced(fel)))
            if ((front2->have(fel)) || (!this->cast(task2)->isAdvanced(fel)))
            {
                iter++;

                /*if (fel->getId() == 40)
                {
                    std::cout << "debug partialDistributedMerge next" << std::endl;
                }*/

                /*{
                    std::stringstream str;
                    str << "rank " << this->comm->rank() << " fel " << fel->text() << " is in front2 " << std::boolalpha << front2->have(fel) << Data::endl;
                    //str << "rank " << this->comm->rank() << " fel " << fel->text() << " not advanced in task2 " << std::boolalpha << (!static_cast<PMJMesh2::MJMeshTask *>(task2)->isAdvanced(fel)) << Data::endl;
                    str << "rank " << this->comm->rank() << " fel " << fel->text() << " not advanced in task2 " << std::boolalpha << (!this->cast(task2)->isAdvanced(fel)) << Data::endl;
                    Data::cout() << str.str();
                }*/

                continue;
            }

            /*if (fel->getId() == 40)
            {
                std::cout << "debug partialDistributedMerge removing from front1" << std::endl;
            }*/

            /*{
                std::stringstream str;
                str << "rank " << this->comm->rank() << " removing fel " << fel->text() << " from front1" << Data::endl;
                Data::cout() << str.str();
            }*/

            iter = front1->erase(iter, state);
            //front1->remove(fel);

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

                /*{
                    std::stringstream str;
                    str << "rank " << this->comm->rank() << " testing v " << v->text() << " from fel " << std::endl;
                    std::cout << str.str();
                }*/

                if (!front1->haveAny(v->getAdjacentFrontElements()))
                {
                    /*{
                        std::stringstream str;
                        str << "rank " << this->comm->rank() << " removing v " << v->text() << " from front1" << Data::endl;
                        Data::cout() << str.str();
                    }*/

                    front1->remove(v);
                }
            }
        }
    }

	//static_cast<PMJMesh2::MJMeshTask *>(task2)->clearAdvanced();
	//this->cast(task2)->clearAdvanced();

    for (Data::Mesh::FrontElementIterator iter = mesh2->feBegin();
         iter != mesh2->feEnd();)
    {
        MJMesh::FrontElement *temp = static_cast<MJMesh::FrontElement *>((*iter));
        MJMesh::FrontElement *orig = NULL;

        /*if (temp->getId() == 40)
        {
            std::cout << "debug partialDistributedMerge mesh2 " << temp->getId() << " at " << temp << std::endl;
        }*/

        //bool debug = ((temp->getId() == 255) || (temp->getId() == 256) || (temp->getId() == 257) || (temp->getId() == 258));
        //bool debug = (temp->getId() == 84082);

        /*if (debug)
        {
            Data::cout() << "rank " << this->comm->rank() << ", temp fel " << temp->text() << " at " << temp << " from mesh2" << Data::endl;
            Data::cout() << "rank " << this->comm->rank() << ", task1 " << task1->getId() << ", task2 " << task2->getId() << Data::endl;
            //Data::cout() << "rank " << this->comm->rank() << ", task1->advanced(temp) " << std::boolalpha << static_cast<PMJMesh2::MJMeshTask *>(task1)->isAdvanced(temp) << Data::endl;
            Data::cout() << "rank " << this->comm->rank() << ", task1->advanced(temp) " << std::boolalpha << this->cast(task1)->isAdvanced(temp) << Data::endl;
            //Data::cout() << "rank " << this->comm->rank() << ", task2->advanced(temp) " << std::boolalpha << static_cast<PMJMesh2::MJMeshTask *>(task2)->isAdvanced(temp) << Data::endl;
            Data::cout() << "rank " << this->comm->rank() << ", task2->advanced(temp) " << std::boolalpha << this->cast(task2)->isAdvanced(temp) << Data::endl;
        }*/

#if USE_C__11
        Data::FrontElementHashMap::iterator it = this->bFrontElements.find(temp->getId());
#else
        Data::FrontElementMap::iterator it = this->bFrontElements.find(temp->getId());
#endif //#if USE_C__11

        bool advanced = false;

        if (it == this->bFrontElements.end())
        {
            mesh1->add(temp);

            /*if (debug)
            {
                Data::cout() << "rank " << this->comm->rank() << ", adding temp to mesh1" << Data::endl;
            }*/

            /*if (temp->getId() == 40)
            {
                std::cout << "debug partialDistributedMerge adding temp to mesh1" << std::endl;
            }*/
        }
        else
        {
            orig = static_cast<MJMesh::FrontElement *>((*it).second);

            /*if (temp->getId() == 40)
            {
                std::cout << "debug partialDistributedMerge orig " << orig->getId() << " at " << orig << std::endl;
            }*/

            /*if (debug)
            {
                Data::cout() << "rank " << this->comm->rank() << ", orig fel " << orig->text() << " at " << orig << Data::endl;
            }*/

            //Data::ElementList els = temp->getAdjacency();

            //while (!els.empty())
            for (Data::ElementList::iterator iter = temp->eBegin();
                 iter != temp->eEnd(); iter = temp->erase(iter))
            {
                //MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
                MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

                //els.pop_front();

                //temp->remove(e);
                orig->add(e);
            }

            for (UInt i = 0; i < temp->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(temp->getVertex(i));

                if (v)
                {
                    v->remove(temp);
                }

                temp->setVertex(i, NULL);
            }

            //advanced = static_cast<PMJMesh2::MJMeshTask *>(task1)->isAdvanced(temp);
            advanced = this->cast(task1)->isAdvanced(temp);

            if ((!advanced) && (!mesh1->have(orig)))
            {
                mesh1->add(orig);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", adding orig to mesh1" << Data::endl;
                }*/

                /*if (temp->getId() == 40)
                {
                    std::cout << "debug partialDistributedMerge adding orig to mesh1" << std::endl;
                }*/
            }
        }

        if (!advanced)
        {
            //this->checkAndAddInFront(temp, orig, oldFront2, oldFront1);
            //this->checkAndAddInFront(temp, orig, commonFront2, commonFront1);
            //this->checkAndAddInFront(temp, orig, newFront2, newFront1);
            this->checkAndAddInFront(temp, orig, front2, front1);

            MJMesh::FrontElement *fel = orig ? orig : temp;

            if (((orig) && (this->boundary->have(orig)) && (fel->numAdjacents() == 0)) ||
                ((!front1->have(fel)) && (fel->numAdjacents() == 1)))
            {
                if (!orig)
                {
                    this->bFrontElements.insert(std::make_pair(fel->getId(), fel));

                    /*if (debug)
                    {
                        Data::cout() << "adding " << fel->getId() << " at " << fel << " to bFrontElements" << Data::endl;
                    }*/
                }

                if (!newBoundary->have(fel))
                {
                    newBoundary->add(fel);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding fel " << fel->getId() << " to newBoundary " << newBoundary << Data::endl;
                    }*/
                }
                /*else if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", fel " << fel->getId() << " not added to newBoundary " << newBoundary << Data::endl;
                }*/

                if (!oldFront1->have(fel))
                {
                    oldFront1->addBoundary(fel);

                    /*if (debug)
                    {
                        Data::cout() << "rank " << this->comm->rank() << ", adding fel " << fel->getId() << " to oldFront1 " << oldFront1 << Data::endl;
                    }*/
                }
                /*else if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", fel " << fel->getId() << " not added to oldFront1 " << oldFront1 << Data::endl;
                }*/
            }

            /*if ((!orig) && (front1->have(temp)))
            {
                this->bFrontElements.insert(std::make_pair(temp->getId(), temp));
            }*/
        }

        iter = mesh2->erase(iter);

        /*if (debug)
        {
            Data::cout() << "rank " << this->comm->rank() << ", removing temp from mesh2" << Data::endl;
        }*/

        if (orig)
        {
            if ((advanced) && (front2->have(temp)))
            {
                front2->remove(temp);

                /*if (debug)
                {
                    Data::cout() << "rank " << this->comm->rank() << ", removing temp from front2" << Data::endl;
                }*/
            }

            /*if (debug)
            {
                Data::cout() << "rank " << this->comm->rank() << ", deleting temp" << Data::endl;
            }*/

            //Data::cout() << "deleting temp (1) at " << temp << Data::endl;

            delete temp;
        }
    }

    for (Data::Mesh::VertexIterator iter = mesh2->vBegin();
         iter != mesh2->vEnd();)
    {
        MJMesh::Vertex *temp = static_cast<MJMesh::Vertex *>((*iter));
        MJMesh::Vertex *orig = NULL;

        /*if (temp->getId() == 41)
        {
            std::cout << "debug partialDistributedMerge mesh2 " << temp->getId() << " at " << temp << std::endl;
        }*/

        //Data::cout() << "temp v " << temp->getId() << " at " << temp << Data::endl;

#if USE_C__11
        Data::VertexHashMap::iterator it = this->bVertices.find(temp->getId());
#else
        Data::VertexMap::iterator it = this->bVertices.find(temp->getId());
#endif //#if USE_C__11

        bool inboundary = false;
        /*if (temp->getId() == 41)
        {
            std::cout << "debug partialDistributedMerge (1) inboundary = false" << std::endl;
        }*/
        bool anyInMesh = true;

        if (it == this->bVertices.end())
        {
            //Data::FrontElementList fels = temp->getAdjacentFrontElements();

            //while (!fels.empty())
            for (Data::FrontElementList::iterator iter = temp->feBegin();
                 iter != temp->feEnd(); iter++)
            {
                //Data::FrontElement *e = fels.front();
                Data::FrontElement *e = (*iter);

                //fels.pop_front();

                if (newBoundary->have(e))
                {
                    inboundary = true;

                    /*if (temp->getId() == 41)
                    {
                        std::cout << "debug partialDistributedMerge (2) inboundary = true" << std::endl;
                    }*/

                    break;
                }
            }

            mesh1->add(temp, !inboundary);

            //Data::cout() << "adding temp to mesh1" << Data::endl;

            /*if (temp->getId() == 41)
            {
                std::cout << "debug partialDistributedMerge adding temp to mesh1" << std::endl;
            }*/
        }
        else
        {
            orig = static_cast<MJMesh::Vertex *>((*it).second);

            /*if (temp->getId() == 41)
            {
                std::cout << "debug partialDistributedMerge orig " << orig->getId() << " at " << orig << std::endl;
            }*/

            //Data::cout() << "orig v " << orig->getId() << " at " << orig << Data::endl;

            anyInMesh = false;

            //Data::ElementList els = temp->getAdjacentElements();

            //while (!els.empty())
            for (Data::ElementList::iterator iter = temp->eBegin();
                 iter != temp->eEnd(); iter = temp->erase(iter))
            {
                //Data::Element *e = els.front();
                Data::Element *e = (*iter);

                //els.pop_front();

                e->replace(temp, orig);

                //temp->remove(e);

                orig->add(e);

                anyInMesh = true;
            }

            //Data::FrontElementList fels = temp->getAdjacentFrontElements();

            //Data::FrontElementList defFEls = orig->getAdjacentFrontElements();

            //while (!fels.empty())
            for (Data::FrontElementList::iterator iter = temp->feBegin();
                 iter != temp->feEnd(); iter = temp->erase(iter))
            {
                //Data::FrontElement *e = fels.front();
                Data::FrontElement *e = (*iter);

                //fels.pop_front();

                /*bool found = false;

                for (Data::FrontElementList::iterator iter = defFEls.begin();
                     iter != defFEls.end(); iter++)
                {
                    if ((*iter)->getId() == e->getId())
                    {
                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    continue;
                }*/

                e->replace(temp, orig);

                //temp->remove(e);

                orig->add(e);

                if ((!anyInMesh) && (mesh1->have(e)))
                {
                    anyInMesh = true;
                }
            }

            if (!anyInMesh)
            {
                //els = orig->getAdjacentElements();

                //while (!els.empty())
                for (Data::ElementList::iterator iter = orig->eBegin();
                     iter != orig->eEnd(); iter++)
                {
                    //Data::Element *e = els.front();
                    Data::Element *e = (*iter);

                    //els.pop_front();

                    if (mesh1->have(e))
                    {
                        anyInMesh = true;

                        break;
                    }
                }

                if (!anyInMesh)
                {
                    //fels = orig->getAdjacentFrontElements();

                    //while (!fels.empty())
                    for (Data::FrontElementList::iterator iter = orig->feBegin();
                         iter != orig->feEnd(); iter++)
                    {
                        //Data::FrontElement *fel = fels.front();
                        Data::FrontElement *fel = (*iter);

                        //fels.pop_front();

                        if (mesh1->have(fel))
                        {
                            anyInMesh = true;

                            break;
                        }
                    }
                }
            }

            //inboundary = true;
            inboundary = newBoundary->haveAny(orig->getAdjacentFrontElements(), false); ///TESTAR SE A VERSAO DO MARKOS CONTINUA FUNCIONANDO

            /*if (temp->getId() == 41)
            {
                std::cout << "debug partialDistributedMerge (3) inboundary = true" << std::endl;

                std::cout << "debug newBoundary->haveAny(orig->adjFels) " << std::boolalpha << newBoundary->haveAny(orig->getAdjacentFrontElements(), false) << std::endl;
            }*/

            if ((anyInMesh) && (!mesh1->have(orig)))
            {
                //mesh1->add(orig, false);
                mesh1->add(orig, !inboundary); ///TESTAR SE A VERSAO DO MARKOS CONTINUA FUNCIONANDO 2 - A MISSAO

                //Data::cout() << "adding orig to mesh1" << Data::endl;

                /*if (temp->getId() == 41)
                {
                    std::cout << "debug partialDistributedMerge adding orig to mesh1" << std::endl;
                }*/
            }
        }

        if (anyInMesh)
        {
            //this->checkAndAddInFront(temp, orig, oldFront2, oldFront1);
            //this->checkAndAddInFront(temp, orig, commonFront2, commonFront1);
            //this->checkAndAddInFront(temp, orig, newFront2, newFront1);
            this->checkAndAddInFront(temp, orig, front2, front1);

            if (inboundary)
            {
                Data::Vertex *v = orig ? orig : temp;

                if (!orig)
                {
                    this->bVertices.insert(std::make_pair(v->getId(), v));
                }

                if (!newBoundary->have(v))
                {
                    newBoundary->add(v);
                }

                if (!oldFront1->have(v))
                {
                    oldFront1->add(v);
                }
            }

            /*if ((!orig) && (front1->have(temp)))
            {
                this->bVertices.insert(std::make_pair(temp->getId(), temp));
            }*/
        }

        iter = mesh2->erase(iter);

        //Data::cout() << "removing temp from mesh2" << Data::endl;

        if (orig)
        {
            //Data::cout() << "front2->have(temp) " << std::boolalpha << front2->have(temp) << Data::endl;
            //Data::cout() << "anyInMesh " << std::boolalpha << anyInMesh << Data::endl;

            if (/*(!anyInMesh) && */(front2->have(temp)))
            {
                front2->remove(temp);
            }

            //Data::cout() << "front2->have(temp) " << std::boolalpha << front2->have(temp) << Data::endl;
            //Data::cout() << "anyInMesh " << std::boolalpha << anyInMesh << Data::endl;

            //Data::cout() << "deleting temp" << Data::endl;

            //Data::cout() << "deleting temp (2) at " << temp << Data::endl;

            delete temp;
        }
    }

    for (Data::Mesh::ElementIterator iter = mesh2->eBegin();
		 iter != mesh2->eEnd(); iter = mesh2->erase(iter))
	{
		mesh1->add((*iter));

		//Data::cout() << "el " << (*iter)->getId() << " at " << (*iter) << Data::endl;
		//Data::cout() << "adding el to mesh1" << Data::endl;
	}

	/*for (Data::Boundary::FrontElementIterator iter = newBoundary->eBegin();
         iter != newBoundary->eEnd(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        oldFront1->addBoundary(fel);

        this->bFrontElements.insert(std::make_pair(fel->getId(), fel));

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            if (!newBoundary->have(v))
            {
                newBoundary->add(v);
            }

            if (!mesh1->have(v))
            {
                mesh1->add(v, false);
            }
            else if (mesh1->haveInner(v))
            {
                mesh1->removeInner(v);
            }

            if (!oldFront1->have(v))
            {
                oldFront1->add(v);

                this->bVertices.insert(std::make_pair(v->getId(), v));
            }
        }
    }*/

    /*Data::cout() << "after merging" << Data::endl;

    for (Data::Mesh::VertexIterator iter = mesh1->vBegin();
         iter != mesh1->vEnd(); iter++)
    {
        Data::cout() << "mesh1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (front1->have((*iter)))
        {
            Data::cout() << "front1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        Data::cout() << "mesh1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (front1->have((*iter)))
        {
            Data::cout() << "front1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (Data::Mesh::ElementIterator iter = mesh1->eBegin();
         iter != mesh1->eEnd(); iter++)
    {
        Data::cout() << "mesh1, element " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
    }

    for (Data::Front::VertexIterator iter = front1->vBegin();
         iter != front1->vEnd(); iter++)
    {
        Data::cout() << "front1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;

        if (!mesh1->have((*iter)))
        {
            Data::cout() << "NOT mesh1, vertex " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
        }
    }

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::FrontElementIterator iter = front1->eBegin(state);
             iter != front1->eEnd(state); iter++)
        {
            Data::cout() << "front1, fel " << (*iter)->getId() << ", at " << (*iter) << ", in state " << static_cast<UInt>(state) << Data::endl;

            if (!mesh1->have((*iter)))
            {
                Data::cout() << "NOT mesh1, fel " << (*iter)->getId() << ", at " << (*iter) << Data::endl;
            }
        }
    }*/

    //static_cast<PMJMesh2::MJMeshTask *>(task1)->mergeAdvancedFrom(static_cast<PMJMesh2::MJMeshTask *>(task2));
    this->cast(task1)->mergeAdvancedFrom(this->cast(task2));

	front2->clear();

	task2->setMesh(NULL);

	//Data::cout() << "deleting mesh2 at " << mesh2 << Data::endl;

	delete mesh2;
}

void PMJMesh2::ParallelMeshGenerator::emptyDistributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2) const
{

}

bool PMJMesh2::ParallelMeshGenerator::executeShared(PMJMesh::MJMeshTask *task, UInt id, bool shouldMerge, bool &error
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    , Real &generation, Real &wait, Real &merge
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
)
{
    error = false;

    /*//debug
    if (task->getId() == 4)
    {
        return false;
    }
    //endebug*/

    /*if (task->getId() == 2)
    {
        std::cout << "debug" << std::endl;
    }*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Real timingStart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    PartitionBSPTreeCell *cell = this->cast(task)->getCell();

    //Data::cout() << " debug executeShared, task->boundary at " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() << ", this->boundary at " << this->boundary << Data::endl;

    if (!this->setTaskParameters(task,
        dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() ?
            const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary())) :
            const_cast<MJMesh::Boundary *>(this->boundary),
        const_cast<Data::BoxTree *>(this->tree), this->idManagers[id]))
    {
        error = true;
    }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Data::ULI4Tuple prev = this->makeTuple(task);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    if (!error)
    {
#if DEBUG_PMJMESH2_PROFILE
        Real genStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

        //Data::cout() << "debug PMJMesh2::ParallelMeshGenerator::executeShared, task id = " << task->getId() << Data::endl;

        task->execute();

#if DEBUG_PMJMESH2_PROFILE
        if (id == 0) generation += Data::time() - genStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

        //Data::cout() << "task " << task->getId() << " generated the following mesh" << Data::endl;
        //Data::cout() << task->getMesh()->text() << Data::endl;

        if ((task->getStatus().getType() == task->finished.getType()) && (this->isInError(task)))
        {
            error = true;
        }
    }

    this->getParameters(task);

    PartitionBSPTreeCell *otherCell = cell->parent() ? static_cast<PartitionBSPTreeCell *>(cell->parent()->child(1)) : NULL;

#if USE_OPENMP
    cell->getLock().unset();
#endif //#if USE_OPENMP

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Data::cout() << this->timing(this->cast(task), Data::time() - timingStart, prev) << Data::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    if (error)
    {
        return false;
    }

    if (!shouldMerge)
    {
        return false;
    }

    if ((!otherCell) || (otherCell == cell))
    {
        return false;
    }

#if DEBUG_PMJMESH2_PROFILE
    Real waitStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

#if USE_OPENMP
    otherCell->getLock().set();
#endif //#if USE_OPENMP

#if DEBUG_PMJMESH2_PROFILE
    if (id == 0) wait += Data::time() - waitStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

    PMJMesh::MJMeshTask *otherTask = static_cast<PMJMesh::MJMeshTask *>(otherCell->getTask());

    if (!otherTask)
    {
        error = true;

        return false;
    }

#if USE_OPENMP
    cell->getLock().set();
#endif //#if USE_OPENMP

    this->replacer->backReplace(task->getId(), otherTask->getId(), this->boundary);

#if DEBUG_PMJMESH2_PROFILE
    Real mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    //if (task->getId() == 1) while (true);

    this->sharedMerge(task, otherTask, static_cast<PartitionBSPTreeCell *>(cell->parent()));

    //if (task->getId() == 0) while (true);

#if DEBUG_PMJMESH2_PROFILE
    if (id == 0) merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

    return true;
}

void PMJMesh2::ParallelMeshGenerator::executeSharedMeshGeneration()
{
#if USE_GUI
    if (Data::isStepByStep())
    {
        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            Data::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i])->getMesh();

            static_cast<PMJMesh::MJMeshTask *>(this->tasks[i])->setMesh(NULL);

            mesh->clear();

            delete mesh;
        }
    }
#endif //#if USE_GUI

#if DEBUG_PMJMESH2_PROFILE
    Real initialization, generation, merge, wait, administration, finalization;
    initialization = generation = merge = wait = administration = finalization = 0.0;
    Real start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    if (this->idManagers.empty())
	{
		this->idManagers.resize(comm->getMaxThreads(), NULL);
	}

	bool error = false;

#if USE_GUI
    bool step = Data::isStepByStep();

    Data::setStepByStep(false);
#endif //#if USE_GUI

#if USE_C__11
    Int max = static_cast<Int>(std::ceil(std::log2(static_cast<double>(comm->getMaxThreads()))));
#else
    Int max = static_cast<Int>(std::ceil(std::log(static_cast<double>(comm->getMaxThreads()))/std::log(2.0)));
#endif //#if USE_C__11

#if USE_OPENMP
    //PartitionBSPTreeCell *commonParent = this->findCommonParent();

    //this->replacer = new PMJMesh2::CrossingReplacer(this->dimension, true, this->bsp, commonParent, &this->tasks);

    if (this->commonParent)
    {
        this->setLock(this->commonParent, true);
    }

#if DEBUG_PMJMESH2_PROFILE
    initialization = Data::time() - start;
    start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    #pragma omp parallel if (comm->isSharedParallelismEnabled()) shared(error)
#endif //#if USE_OPENMP
    //for (UInt i = 0; i < this->tasks.size(); i++)
    {
        Int id = comm->threadId();

        if (!this->idManagers[id])
        {
            this->idManagers[id] = this->makeIdManager(comm, id);
        }

        if (this->numTasksPerThread > 1)
        {
            Data::GraphNodeList cells, search;

            PartitionBSPTreeCell *threadCommonParent = this->findCommonParent(id*this->numTasksPerThread, (id + 1)*this->numTasksPerThread);

            search.push_back(threadCommonParent);

            while (!search.empty())
            {
                Data::TreeNode *cell = static_cast<Data::TreeNode *>(search.front());

                search.pop_front();

                UInt numChildren = cell->numChildren();

                for (UInt i = 0; i < numChildren; i++)
                {
                    Data::GraphNode *child = cell->child(i);

                    cells.push_front(child);
                    search.push_back(child);
                }
            }

            while (!cells.empty())
            {
                PartitionBSPTreeCell *cell = static_cast<PartitionBSPTreeCell *>(cells.front());

                cells.pop_front();

                PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(cell->getTask());

                bool err = false;

                /*bool shouldContinue = */this->executeShared(task, id, true, err
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                    , generation, wait, merge
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                    );

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::cout << "Gerou malha pelo executeShared : " << generation << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

                if (err)
                {
#if USE_OPENMP
                    #pragma omp critical (settingError)
#endif //#if USE_OPENMP
                    {
                        error = true;

#if USE_OPENMP
                        #pragma omp flush(error)
#endif //#if USE_OPENMP
                    }

                    break;
                }
            }
        }

        if (!error)
        {
            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[id*this->numTasksPerThread]);

#if ((USE_MPI) && (USE_OPENMP))
            Int maxIterations = -1;
#endif //#if ((USE_MPI) && (USE_OPENMP))

            for (Int j = 0; j <= max; j++)
            {
                /*if (true)*/
                {
#if ((USE_MPI) && (USE_OPENMP))
                    PartitionBSPTreeCell *cell = this->cast(task)->getCell();

                    if ((maxIterations == -1) && (this->commonParent))
                    {
                        maxIterations = cell->level() - this->commonParent->level();
                    }
#endif //#if ((USE_MPI) && (USE_OPENMP))

                    bool shouldMerge = true;
                    bool err = false;

#if USE_MPI
                    if ((comm->numProcesses() > 1) &&
                        ((j == max)
#if USE_OPENMP
                        || ((maxIterations != -1) && (j == maxIterations))
#endif //#if USE_OPENMP
                        ))
                    {
                        shouldMerge = false;
                    }
#endif //#if USE_MPI

                    bool shouldContinue = this->executeShared(task, id, shouldMerge, err
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                        , generation, wait, merge
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                        );

                    if (err)
                    {
#if USE_OPENMP
                        #pragma omp critical (settingError)
#endif //#if USE_OPENMP
                        {
                            error = true;

#if USE_OPENMP
                            #pragma omp flush(error)
#endif //#if USE_OPENMP
                        }
                    }

                    if (!shouldContinue)
                    {
                        break;
                    }
                }
                /*else
                {
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                    Real timingStart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if USE_OPENMP
                    PartitionBSPTreeCell *cell = this->cast(task)->getCell();
#endif //#if USE_OPENMP

#if ((USE_MPI) && (USE_OPENMP))
                    if ((maxIterations == -1) && (this->commonParent))
                    {
                        maxIterations = cell->level() - this->commonParent->level();
                    }
#endif //#if ((USE_MPI) && (USE_OPENMP))

                    if (!this->setTaskParameters(task,
                        dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() ?
                            const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary())) :
                            const_cast<MJMesh::Boundary *>(this->boundary),
                        const_cast<Data::BoxTree *>(this->tree), this->idManagers[id]))
                    {
#if USE_OPENMP
                        #pragma omp critical (settingError)
#endif //#if USE_OPENMP
                        {
                            error = true;

                            //Data::cout() << "rank " << comm->rank() << ", thread " << comm->threadId() << ", task " << task->getId() << ", error 1 " << Data::endl;

#if USE_OPENMP
                            #pragma omp flush(error)
#endif //#if USE_OPENMP
                        }
                    }

                    //if (id != 0) while (true);

                    //if (task->getId() == 0)
                    //{
                    //    error = true;
                    //}

                    //if ((task->getId() != 2392) && (task->getId() != 14357) && (task->getId() != 14358))
                    //{
                    //    while (true);
                    //}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                    Data::ULI4Tuple prev = this->makeTuple(task);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

                    if (!error)
                    {
                        //if (task->getId() == 0)
                        //{
                        //    Data::cout() << "debug" << Data::endl;
                        //    while (true);
                        //}

#if DEBUG_PMJMESH2_PROFILE
                        Real genStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

                        task->execute();

#if DEBUG_PMJMESH2_PROFILE
                        if (id == 0) generation += Data::time() - genStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

                        //Data::cout() << "thread " << id << " finished task " << cell->getId() << Data::endl;
                    }

                    if ((task->getStatus().getType() == task->finished.getType()) && (this->isInError(task)))
                    {
#if USE_OPENMP
                        #pragma omp critical (settingError)
#endif //#if USE_OPENMP
                        {
                            error = true;

                            //Data::cout() << "rank " << comm->rank() << ", thread " << comm->threadId() << ", task " << task->getId() << ", error 2 " << Data::endl;

#if USE_OPENMP
                            #pragma omp flush(error)
#endif //#if USE_OPENMP
                        }
                    }

                    this->getParameters(task);

#if USE_OPENMP

                    PartitionBSPTreeCell *otherCell = cell->parent() ? static_cast<PartitionBSPTreeCell *>(cell->parent()->child(1)) : NULL;

                    cell->getLock().unset();
#endif //#if USE_OPENMP

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                    Data::cout() << this->timing(this->cast(task), Data::time() - timingStart, prev) << Data::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

                    if (error)
                    {
                        break;
                    }

#if USE_MPI
                    if ((comm->numProcesses() > 1) &&
                        ((j == max)
#if USE_OPENMP
                        || ((maxIterations != -1) && (j == maxIterations))
#endif //#if USE_OPENMP
                        ))
                    {
                        break;
                    }
#endif //#if USE_MPI

#if USE_OPENMP
                    if ((!otherCell) || (otherCell == cell))
                    {
                        break;
                    }

                    //Data::cout() << "thread " << id << " is waiting for other task " << otherCell->getId() << Data::endl;

#if DEBUG_PMJMESH2_PROFILE
                    Real waitStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

                    otherCell->getLock().set();

#if DEBUG_PMJMESH2_PROFILE
                    if (id == 0) wait += Data::time() - waitStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

                    //Data::cout() << "thread " << id << " acquired lock for other cell " << otherCell->getId() << Data::endl;

                    PMJMesh::MJMeshTask *otherTask = static_cast<PMJMesh::MJMeshTask *>(otherCell->getTask());

                    if (!otherTask)
                    {
                        #pragma omp critical (settingError)
                        {
                            error = true;

                            //Data::cout() << "rank " << comm->rank() << ", thread " << comm->threadId() << ", task " << task->getId() << ", error 3\n";
                            //Data::cout() << "cell " << cell->getId() << ", otherCell " << otherCell->getId() << ", j " << j << ", max " << max << Data::endl;

                            #pragma omp flush(error)
                        }

                        break;
                    }

                    cell->getLock().set();

                    //Data::cout() << "thread " << id << " acquired lock for cell " << task->getId() << Data::endl;

                    //if (cell->parent()->getId() == 0)
                    //{
                    //    Data::cout() << "debug" << Data::endl;
                    //    //while (true);
                    //}

                    //if (j == 0) while (true);

                    //#pragma omp critical
                    {
                        //#pragma omp critical (backreplace)
                        {
                            this->replacer->backReplace(task->getId(), otherTask->getId(), this->boundary);
                        }

                        //if (j == 0) while (true);

                        //#pragma omp critical (merge)
                        {
#if DEBUG_PMJMESH2_PROFILE
                            Real mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

                            this->sharedMerge(task, otherTask, static_cast<PartitionBSPTreeCell *>(cell->parent()));

#if DEBUG_PMJMESH2_PROFILE
                            if (id == 0) merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE
                        }

                        //if (j == 0) while (true);

                        //if (cell->parent()->getId() == 0)
                        //{
                        //    Data::cout() << "debug" << Data::endl;
                        //    while (true);
                        //}
                    }

                    //if (j >= 1) while (true);
#endif //#if USE_OPENMP
                }*/
            }
        }
    }

#if DEBUG_PMJMESH2_PROFILE
    administration = Data::time() - start - generation - wait - merge;
    start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

#if USE_OPENMP
    if (this->commonParent)
    {
        this->setLock(this->commonParent, false);
    }
#endif //#if USE_OPENMP

//#if USE_OPENMP
    if (this->replacer)
    {
/*#if USE_MPI
        if ((!this->commonParent->isLeaf()) && (comm->numProcesses() > 1))
        {
            this->replacer->backReplace(this->commonParent->child(0)->getId(), this->commonParent->child(1)->getId(), this->boundary);
        }
#endif //#if USE_MPI*/

        this->replacer->execute();

        delete this->replacer;

        this->replacer = NULL;
    }

    this->commonParent = NULL;
//#endif //#if USE_OPENMP

#if USE_GUI
    Data::setStepByStep(step);
#endif //#if USE_GUI

#if USE_THREAD_COLOR
    if (Data::isStepByStep())
    {
        Int nProcesses = 1;
        Int nThreads = 1;
        Int rank = 0;
#if USE_MPI
        nProcesses = this->comm->numProcesses();
        rank = this->comm->rank();
#endif //#if USE_MPI
#if USE_OPENMP
        nThreads = static_cast<Parallel::TMCommunicator *>(this->comm)->getMaxThreads();
#endif //#if USE_OPENMP

        Int color = rank;

        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            Data::Mesh *mesh = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i])->getMesh();

            Real r, g, b;
            Data::pallete(color, r, g, b);

            if (mesh)
            {
                for (Data::Mesh::ElementIterator iter = mesh->eBegin();
                     iter != mesh->eEnd(); iter++)
                {
                    (*iter)->setColor(r, g, b);
                }
            }

            Data::MeshList meshes = this->cast(this->tasks[i])->getMeshes();

            while (!meshes.empty())
            {
                mesh = meshes.front();

                meshes.pop_front();

                if (mesh)
                {
                    r = g = b = 1.0;

                    for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if (((*iter)->getR() < 1.0) ||
                            ((*iter)->getG() < 1.0) ||
                            ((*iter)->getB() < 1.0))
                        {
                            r = (*iter)->getR();
                            g = (*iter)->getG();
                            b = (*iter)->getB();

                            break;
                        }
                    }

                    if ((r >= 1.0) && (g >= 1.0) && (b >= 1.0))
                    {
                        Data::pallete(color, r, g, b);
                    }

                    for (Data::Mesh::ElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if (((*iter)->getR() >= 1.0) &&
                            ((*iter)->getG() >= 1.0) &&
                            ((*iter)->getB() >= 1.0))
                        {
                            (*iter)->setColor(r, g, b);
                        }
                    }
                }
            }

            if (((nProcesses == 1)) &&
                ((nThreads == 1) ||
                 (this->numTasksPerThread == 1) ||
                 (i%this->numTasksPerThread + 1 == this->numTasksPerThread)))
            {
                color++;
            }
        }
    }
#endif //#if USE_THREAD_COLOR

    this->idManager->setId(0, this->idManagers[0]->getId(0));
    this->idManager->setId(1, this->idManagers[0]->getId(1));
    this->idManager->setId(2, this->idManagers[0]->getId(2));

    for (Int i = 1; i < comm->getMaxThreads(); i++)
    {
        this->idManager->setId(0, std::max(this->idManager->getId(0), this->idManagers[i]->getId(0)));
        this->idManager->setId(1, std::max(this->idManager->getId(1), this->idManagers[i]->getId(1)));
        this->idManager->setId(2, std::max(this->idManager->getId(2), this->idManagers[i]->getId(2)));
    }

    this->mesh = const_cast<MJMesh::Mesh *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->getMesh());

#if USE_MPI
    if (comm->numProcesses() == 1)
#endif //#if USE_MPI
    {
        dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->setMesh(NULL);
    }

    if (error)
    {
        this->status = this->error;
    }
    else
    {
        this->status = this->sharedMemoryMeshGenerated;
    }

    /*//debug
    for (UInt i = 0; i < this->tasks.size(); i++)
    {
        PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

        if (!task)
        {
            continue;
        }

        PMJMesh::Front *front = static_cast<PMJMesh::Front *>(task->getFront());

        if (!front)
        {
            continue;
        }

        Data::cout() << "end of executeSharedMeshGeneration on process " << this->comm->rank() << Data::endl;

        ULInt ids[4] = {94655, 94656, 93959, 94654};

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (UShort j = 0; j < 4; j++)
            {
                bool found = false;

                for (Data::Front::ConstFrontElementIterator iter = front->eBegin(j);
                     iter != front->eEnd(j); iter++)
                {
                    if ((*iter)->getId() == ids[k])
                    {
                        Data::cout() << "found fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    break;
                }
            }
        }

        PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

        if (!drive)
        {
            continue;
        }

        MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive->getBoundary()));

        if (!boundary)
        {
            continue;
        }

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
                 iter != boundary->eEnd(); iter++)
            {
                if ((*iter)->getId() == ids[k])
                {
                    Data::cout() << "found fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                    break;
                }
            }
        }
    }
    //endebug*/

#if DEBUG_PMJMESH2_PROFILE
    finalization = Data::time() - start;

    if (this->comm->rank() == 0)
    {
        Data::cout() << "Time for initializing   executeSharedMeshGeneration() = " << initialization << Data::endl;
        Data::cout() << "Time for generation     executeSharedMeshGeneration() = " << generation << Data::endl;
        Data::cout() << "Time for waiting        executeSharedMeshGeneration() = " << wait << Data::endl;
        Data::cout() << "Time for merging        executeSharedMeshGeneration() = " << merge << Data::endl;
        Data::cout() << "Time for administration executeSharedMeshGeneration() = " << administration << Data::endl;
        Data::cout() << "Time for finalization   executeSharedMeshGeneration() = " << finalization << Data::endl;
    }
#endif //#if DEBUG_PMJMESH2_PROFILE
}

void PMJMesh2::ParallelMeshGenerator::splitMesh(PMJMesh::MJMeshTask *task)
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());

    PMJMesh::Front *newFront = drive->getNewFront();

    PMJMesh::Front *oldFront = drive->getOldFront();

#if USE_GUI
    //oldFront->freeze();

    mesh->freeze();
#endif //#if USE_GUI

    MJMesh::Mesh *newMesh = new MJMesh::Mesh();
    MJMesh::Boundary *oldBoundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive->getBoundary()));
    MJMesh::Boundary *newBoundary = new MJMesh::Boundary();

    //Data::cout() << "splitMesh, this->boundary " << this->boundary << ", oldBoundary " << oldBoundary << ", newBoundary " << newBoundary << Data::endl;

    /*//if (this->comm->rank() == 0)
    {
        Data::cout() << "rank " << this->comm->rank() << ", created newBoundary at " << newBoundary << " replacing oldBoundary at " << oldBoundary << Data::endl;
    }*/

    drive->setBoundary(newBoundary);

    /*if (newBoundary == this->boundary)
    {
        newBoundary = new MJMesh::Boundary();

        drive->setBoundary(newBoundary);
    }
    else
    {
        newBoundary->clear();
    }*/

    //Data::Box *box = static_cast<PMJMesh::Partition *>(task->getPartition())->getBox();
    Data::Box *box = this->box(task);

    newMesh->splitTo(newFront->getVertices(), this->improvementLayers, *newBoundary, oldBoundary, this->boundary, box, newFront);

    /*//debug
    if (this->comm->rank() == 7)
    {
        for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
             iter != newMesh->feEnd(); iter++)
        {
            if ((*iter)->getId() == 362599)
            {
                Data::cout() << "after splitTo" << Data::endl;
                Data::cout() << "found fel " << (*iter)->text() << " at " << (*iter) << " in newMesh" << Data::endl;
                Data::cout() << "fel in this->boundary = " << std::boolalpha << this->boundary->have((*iter)) << Data::endl;
                Data::cout() << "fel in oldBoundary = " << std::boolalpha << oldBoundary->have((*iter)) << Data::endl;
                Data::cout() << "fel in newBoundary = " << std::boolalpha << newBoundary->have((*iter)) << Data::endl;

                break;
            }
        }
    }
    //endebug*/

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after splitTo, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    if (oldBoundary != this->boundary)
    {
        oldBoundary->clear();

        delete oldBoundary;
    }

    //oldFront->clear();

    Data::VertexList removed;

    for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
         iter != newBoundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        //if (j == 1) v->highlight();

        //oldFront->add(v);

        if (!newMesh->have(v))
        {
            newMesh->add(v, false);
        }
        else if (newMesh->haveInner(v))
        {
            newMesh->removeInner(v);
        }

        /*if (v->getId() == 40526)
        {
            Data::cout() << "test for removing vertex " << v->text() << " at " << v << " from boundary" << Data::endl;
        }*/

        if (this->removeFromBoundary(v, newMesh, newBoundary, true, oldFront))
        {
            removed.push_back(v);

            if ((oldFront) && (oldFront->have(v)))
            {
                oldFront->remove(v);
            }

            newMesh->addInner(v);

            /*if (v->getId() == 40526)
            {
                Data::cout() << "removing vertex " << v->text() << " at " << v << " from boundary" << Data::endl;

                for (Data::FrontElementList::iterator iter2 = v->feBegin();
                     iter2 != v->feEnd(); iter2++)
                {
                    if (!newMesh->have((*iter2)))
                    {
                        Data::cout() << "adj fel " << (*iter2)->text() << " at " << (*iter2) << " NOT in mesh at " << newMesh << Data::endl;
                    }
                }

                for (Data::ElementList::iterator iter2 = v->eBegin();
                     iter2 != v->eEnd(); iter2++)
                {
                    if (!newMesh->have((*iter2)))
                    {
                        Data::cout() << "adj el " << (*iter2)->text() << " at " << (*iter2) << " NOT in mesh at " << newMesh << Data::endl;
                    }
                }

                for (Data::FrontElementList::iterator iter2 = v->feBegin();
                     iter2 != v->feEnd(); iter2++)
                {
                    if (mesh->have((*iter2)))
                    {
                        Data::cout() << "adj fel " << (*iter2)->text() << " at " << (*iter2) << " in mesh at " << mesh << Data::endl;
                    }
                }

                for (Data::ElementList::iterator iter2 = v->eBegin();
                     iter2 != v->eEnd(); iter2++)
                {
                    if (mesh->have((*iter2)))
                    {
                        Data::cout() << "adj el " << (*iter2)->text() << " at " << (*iter2) << " in mesh at " << mesh << Data::endl;
                    }
                }

                for (Data::MeshList::const_iterator iter = this->cast(task)->getMeshes().begin(), end = this->cast(task)->getMeshes().end();
                     iter != end; iter++)
                {
                    for (Data::FrontElementList::iterator iter2 = v->feBegin();
                         iter2 != v->feEnd(); iter2++)
                    {
                        if ((*iter)->have((*iter2)))
                        {
                            Data::cout() << "adj fel " << (*iter2)->text() << " at " << (*iter2) << " in mesh at " << (*iter) << Data::endl;
                        }
                    }

                    for (Data::ElementList::iterator iter2 = v->eBegin();
                         iter2 != v->eEnd(); iter2++)
                    {
                        if ((*iter)->have((*iter2)))
                        {
                            Data::cout() << "adj el " << (*iter2)->text() << " at " << (*iter2) << " in mesh at " << (*iter) << Data::endl;
                        }
                    }
                }
            }*/

            //Data::cout() << "removed v " << v->text() << Data::endl;

            continue;
        }

#if USE_DELETE_FELS
        this->createNewFrontElements(static_cast<MJMesh::Vertex *>(v), newMesh, newBoundary, mesh, 0);
#endif //#if USE_DELETE_FELS
    }

    while (!removed.empty())
    {
        /*if (removed.front()->getId() == 110927)
        {
            Data::cout() << "vertex " << removed.front()->text() << " at " << removed.front() << " removed from newBoundary" << Data::endl;
        }*/

        newBoundary->remove(removed.front());

        removed.pop_front();
    }

    for (Data::Boundary::FrontElementIterator iter = newBoundary->eBegin();
         iter != newBoundary->eEnd(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        /*if (this->comm->isMaster())
        {
            Data::cout() << "newBoundary " << newBoundary << " has fel " << (*iter)->getId() << Data::endl;
        }*/

        //oldFront->addBoundary(fel);

        if (!newMesh->have(fel))
        {
            newMesh->add(fel);
        }
    }

    //static_cast<PMJMesh2::MJMeshTask *>(task)->add(mesh);
    this->cast(task)->add(mesh);

#if USE_GUI
    {
        Data::Point2D mid2d;
        Data::Point3D mid;

        if (this->dimension == 2)
        {
            mid2d = static_cast<PMJMesh::Partition *>(task->getPartition())->getBox()->mid2D();

            mid.setCoord(0, mid2d.getCoord(0));
            mid.setCoord(1, mid2d.getCoord(1));
            mid.setCoord(2, 0.0);
        }
        else
        {
            mid = static_cast<PMJMesh::Partition *>(task->getPartition())->getBox()->mid3D();
        }

        this->cast(task)->add(mid);
    }
#endif //#if USE_GUI

    task->setMesh(newMesh);

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after setMesh, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    /*//if (this->comm->rank() == 0)
    {
        for (Data::Mesh::VertexIterator iter = mesh->vBegin();
             iter != mesh->vEnd(); iter++)
        {
            if ((*iter)->getId() == 110927)
            {
                Data::cout() << "rank " << this->comm->rank() << ", vertex at " << (*iter) << " in mesh at " << mesh << Data::endl;
                Data::cout() << "rank " << this->comm->rank() << ", vertex " << (*iter)->text() << Data::endl;
                Data::cout() << "rank " << this->comm->rank() << ", vertex in newBoundary at " << newBoundary
                    << " = " << std::boolalpha << newBoundary->have((*iter)) << Data::endl;
            }
        }

        for (Data::Mesh::FrontElementIterator iter = newMesh->feBegin();
             iter != newMesh->feEnd(); iter++)
        {
            if ((*iter)->getId() == 84082)
            {
                Data::cout() << "rank " << this->comm->rank() << " found fel " << (*iter)->text() << " at " << (*iter) << " in mesh at "
                    << newMesh << ", boundary at " << newBoundary
                    << ", inboundary = " << std::boolalpha << newBoundary->have((*iter)) << Data::endl;

                break;
            }
        }

        //debug
        Data::cout() << "rank " << this->comm->rank() << " after splitto" << Data::endl;

        for (Data::Boundary::VertexIterator iter = newBoundary->vBegin();
             iter != newBoundary->vEnd(); iter++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

            bool print = ((v->getId() == 43621) || (v->getId() == 76714) || (v->getId() == 27299));

            if (!print)
            {
                continue;
            }

            Data::cout() << "rank " << this->comm->rank() << " testing vertex " << v->text() << Data::endl;

            if ((v->numAdjacentElements() == 0) || (v->numAdjacentFrontElements() == 0))
            {
                Data::cout() << "rank " << this->comm->rank() << " testing numels " << v->numAdjacentElements() << ", numfels = " << v->numAdjacentFrontElements() << Data::endl;

                continue;
            }

            if (this->boundary->have(v))
            {
                Data::cout() << "rank " << this->comm->rank() << " inboundary" << Data::endl;

                continue;
            }

            bool all = true;

            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                Data::cout() << "rank " << this->comm->rank() << " adj fel " << (*iter2)->text() << Data::endl;

                if (!newBoundary->have((*iter2)))
                {
                    Data::cout() << "rank " << this->comm->rank() << " adj not in this->boundary" << Data::endl;

                    all = false;

                    break;
                }
            }

            if (!all)
            {
                Data::cout() << "rank " << this->comm->rank() << " not all adjs in this->boundary" << Data::endl;

                continue;
            }

            all = true;

            for (Data::ElementList::iterator iter2 = v->eBegin();
                 iter2 != v->eEnd(); iter2++)
            {
                Data::cout() << "rank " << this->comm->rank() << " adj el " << (*iter2)->text() << Data::endl;

                if (!newMesh->have((*iter2)))
                {
                    Data::cout() << "rank " << this->comm->rank() << " adj not in newMesh" << Data::endl;

                    all = false;

                    break;
                }
            }

            if (!all)
            {
                Data::cout() << "rank " << this->comm->rank() << " not all adjs in newMesh" << Data::endl;

                continue;
            }

            all = true;

            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                Data::FrontElement *fel = (*iter2);

                Data::cout() << "rank " << this->comm->rank() << " adj fel " << fel->text() << Data::endl;

                Data::Vertex *v1 = NULL, *v2 = NULL;

                for (UInt i = 0; i < fel->numVertices(); i++)
                {
                    if (fel->getVertex(i) != v)
                    {
                        if (!v1)
                        {
                            v1 = fel->getVertex(i);
                        }
                        else
                        {
                            v2 = fel->getVertex(i);
                        }
                    }
                }

                Data::cout() << "rank " << this->comm->rank() << " adj v1 " << v1->text() << Data::endl;
                Data::cout() << "rank " << this->comm->rank() << " adj v2 " << v2->text() << Data::endl;

                UInt count = 0;

                for (Data::ElementList::iterator iter3 = v->eBegin();
                     iter3 != v->eEnd(); iter3++)
                {
                    Data::Element *e = (*iter3);

                    if ((e->have(v1)) && (e->have(v2)))
                    {
                        Data::cout() << "rank " << this->comm->rank() << " adj e have all vs " << e->text() << Data::endl;

                        count++;
                    }
                }

                if (count != 1)
                {
                    all = false;

                    break;
                }
            }

            if (all)
            {
                Data::cout() << "rank " << this->comm->rank() << " adjs of v are consistent" << Data::endl;

                continue;
            }

#if USE_GUI
            v->highlight();
#endif //#if USE_GUI

            Data::cout() << "rank " << this->comm->rank() << " debug 2, task " << task->getId() << ", v " << v->text() << Data::endl;

            //while (true);
        }
        //endebug
    }*/

#if USE_GUI
    mesh->clearLists();

    mesh->unfreeze();

    //oldFront->freeze();
#endif //#if USE_GUI

    this->mesh = newMesh;

    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", tasks[0]->mesh = " << static_cast<PMJMesh::MJMeshTask *>(this->tasks[0])->getMesh() << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", tasks[0]->mesh->size = " << static_cast<PMJMesh::MJMeshTask *>(this->tasks[0])->getMesh()->size() << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", mesh = " << mesh << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", mesh->size = " << mesh->size() << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", this->mesh = " << this->mesh << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", this->mesh->size = " << this->mesh->size() << Data::endl;
    //Data::cout() << "after splitting mesh, rank " << this->comm->rank() << ", this->mesh->frontElementsSize = " << this->mesh->frontElementsSize() << Data::endl;
}

void PMJMesh2::ParallelMeshGenerator::addRejectedFrontElementsThatAdvanced(PMJMesh::MJMeshTask *task)
{
    this->cast(task)->setSendAdvanced(true);
    Data::Mesh *mesh = task->getMesh();
    //std::stringstream str;

//std::cout << "TAM rej:  " << this->rejectedFrontElements.size() << std::endl;

#if USE_C__11
    for (Data::FrontElementHash::iterator iter = this->rejectedFrontElements.begin();
#else
    for (Data::FrontElementSet::iterator iter = this->rejectedFrontElements.begin();
#endif //#if USE_C__11
         iter != this->rejectedFrontElements.end(); iter++)
    {
        MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

        /*std::cout << "debug fel at " << fel << std::endl;
        if ((fel->getId() == 1411) || (fel->getId() == 1367))
        {
            std::cout << "fel " << fel->text() << ", mesh->have(fel) = " << std::boolalpha << mesh->have(fel) << std::endl;
        }*/



        if ((fel->numAdjacents() == 0) || (mesh->have(fel)))
        {
            continue;
        }

//std::cout << this->comm->rank() << ", task " << task->getId() << ", advanced fel " << fel->getId() << std::endl;

        this->cast(task)->addAdvanced(fel);

        //str << "rank " << this->comm->rank() << ", task " << task->getId() << ", advanced fel " << fel->getId() << Data::endl;
    }

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after adding advanced fels, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    //Data::cout() << str.str();

    //this->rejectedFrontElements.clear();
}

void PMJMesh2::ParallelMeshGenerator::removeBoundaryFromMesh()
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
}

void PMJMesh2::ParallelMeshGenerator::addBoundaryToMesh(PMJMesh::MJMeshTask *task)
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    MJMesh::Boundary *newBoundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive->getBoundary()));

    if ((newBoundary) && (newBoundary != this->boundary))
    {
        Data::Mesh *mesh = this->cast(task)->getMeshes().back();

        for (Data::Boundary::ConstVertexIterator iter = newBoundary->vBegin();
             iter != newBoundary->vEnd(); iter++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

            v->clearAdjacentFrontElements();
            v->clearAdjacentElements();

            this->mesh->remove(v);

            if (!mesh->have(v))
            {
                mesh->add(v, !this->boundary->have(v));
            }
        }

        for (Data::Boundary::ConstFrontElementIterator iter = newBoundary->eBegin();
             iter != newBoundary->eEnd(); iter++)
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

            fel->clearAdjacency();

            this->mesh->remove(fel);

            if (!mesh->have(fel))
            {
                mesh->add(fel);
            }
        }
    }
}

void PMJMesh2::ParallelMeshGenerator::clearDrive(PMJMesh::MJMeshTask *task)
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    drive->getNewFront()->clear();
    drive->getOldFront()->clear();
    if (drive->getCommonFront())
    {
        drive->getCommonFront()->clear();
    }

    MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());

    drive->setMesh(NULL);

    delete mesh;
}

void PMJMesh2::ParallelMeshGenerator::send(ULInt ids[3], PMJMesh::MJMeshTask *task, Int peer)
{
    //Data::cout() << "rank " << this->comm->rank() << " will send to peer " << peer << Data::endl;
    //Data::cout() << "task->mesh->size = " << task->getMesh()->size() << Data::endl;
    //Data::cout() << "task->getFront() = " << task->getFront() << Data::endl;
    //Data::cout() << "task->partition->getFront() = " << static_cast<PMJMesh::Partition *>(task->getPartition())->getSubfront() << Data::endl;
    //Data::cout() << "task->builder->getFront() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getFront() << Data::endl;
    //Data::cout() << "task->builder->getNewFront() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront() << Data::endl;
    //Data::cout() << "task->builder->getOldFront() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getOldFront() << Data::endl;
    //Data::cout() << "task->builder->getCommonFront() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getCommonFront() << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->size(0) = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->size(0) << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->size(1) = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->size(1) << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->size(2) = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->size(2) << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->size(3) = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->size(3) << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->size() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->size() << Data::endl;
    //Data::cout() << "task->builder->getNewFront()->vsize() = " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getNewFront()->verticesSize() << Data::endl;

    this->comm->send(ids, 3, peer, 1);

    this->comm->send(task, peer, 1);

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after sending task, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    if ((this->updateType == FULL) || (this->updateType == PARTIAL))
    {
#if USE_GUI
        this->mesh->freeze();
#endif //#if USE_GUI

        this->removeBoundaryFromMesh();

        /*//debug
        Data::cout() << "proc " << this->comm->rank() << " boundary->debug after clearing adjacencies, " << this->boundary << Data::endl;
        this->boundary->debug();
        //endebug*/

        if (this->updateType == PARTIAL)
        {
            this->addBoundaryToMesh(task);

            /*//debug
            Data::cout() << "proc " << this->comm->rank() << " boundary->debug after clearing adjacencies of newBoundary, " << this->boundary << Data::endl;
            this->boundary->debug();
            //endebug*/
        }

#if USE_GUI
        this->mesh->unfreeze();
#endif //#if USE_GUI
    }

    //Data::cout() << "B Proc " << this->comm->rank() << ", mesh at " << this->mesh << Data::endl;

    this->mesh = NULL;

    //Data::cout() << "C Proc " << this->comm->rank() << ", mesh at " << this->mesh << Data::endl;

    this->clearDrive(task);

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after deleting mesh, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/
}

PMJMesh::MJMeshTask *PMJMesh2::ParallelMeshGenerator::receive(ULInt ids[3], Int peer)
{
    this->comm->receive(ids, 3, peer, 1);

    //std::cout << "Receber task!!!! " << std::endl;

    PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->comm->receive(peer, 1));

    //debug
    //Data::cout() << "proc " << this->comm->rank() << " boundary->debug after receiving otherTask, " << this->boundary << Data::endl;
    //this->boundary->debug();
    //endebug

    //Data::cout() << "othertask->mesh->size = " << otherTask->getMesh()->size() << Data::endl;
    //Data::cout() << "othertask->mesh->size = " << task->getMesh()->size() << Data::endl;

    this->idManagers[0]->setId(0, std::max(this->idManagers[0]->getId(0), ids[0]));
    this->idManagers[0]->setId(1, std::max(this->idManagers[0]->getId(1), ids[1]));
    this->idManagers[0]->setId(2, std::max(this->idManagers[0]->getId(2), ids[2]));

    return task;
}

bool PMJMesh2::ParallelMeshGenerator::executeDistributed(PMJMesh::MJMeshTask *task, bool &error
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    , Real &communication, Real &merge, Real &generation
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    )
{
    error = false;

    PartitionBSPTreeCell *cell = this->cast(task)->getCell();

    PartitionBSPTreeCell *otherCell = cell->parent() ? static_cast<PartitionBSPTreeCell *>(cell->parent()->child(1)) : NULL;

    if (!otherCell)
    {
        return false;
    }

    //if (j == 0) while (true);

#if DEBUG_PMJMESH2_PROFILE
    Real mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    this->mergeCommonFront(task);

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after mergeCommonFront, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    if (this->updateType == PARTIAL)
    {

        this->splitMesh(task);

    }

#if DEBUG_PMJMESH2_PROFILE
    merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

    if (this->updateType != FULL)
    {
        //std::cout << "----  33333333333333 " << std::endl;
        this->addRejectedFrontElementsThatAdvanced(task);
        //std::cout << "----  3 " << std::endl;
    }
    //std::cout << "TAMANHO DA MALHA ATUAL" << this->mesh->size() << std::endl;
    //std::cout << "TAMANHO DOS FRONT ELEMENTS ATUAL" << this->mesh->getFrontElements().size() << std::endl;



    /*for( Data::ElementList::const_iterator it = this->mesh->getElements().begin(); it != this->mesh->getElements().end(); it++ )
    {
        std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
    }*/



    ULInt ids[3] = {this->idManagers[0]->getId(0), this->idManagers[0]->getId(1), this->idManagers[0]->getId(2)};

    Int peer = static_cast<PartitionBSPTreeCell *>(cell->parent())->getPeer();

    //if (j == 0) while (true);

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after finding peer, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

    if (otherCell == cell)
    {
        //otherCell = static_cast<PartitionBSPTreeCell *>(cell->parent()->child(0));
        //Int peer = otherCell->getPeer();

        this->send(ids, task, peer);

        return false;
    }

    //Data::cout() << "rank " << this->comm->rank() << " will receive from peer " << peer << Data::endl;

    //Int peer = otherCell->getPeer();

#if DEBUG_PMJMESH2_PROFILE
    Real commStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    PMJMesh::MJMeshTask *otherTask = this->receive(ids, peer);

#if DEBUG_PMJMESH2_PROFILE
    communication += Data::time() - commStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

#if DEBUG_PMJMESH2_PROFILE
    mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    this->distributedMerge(task, otherTask, static_cast<PartitionBSPTreeCell *>(cell->parent()));

    /*//debug
    if (this->comm->rank() == 7)
    {
        for (Data::Mesh::FrontElementIterator iter = task->getMesh()->feBegin();
             iter != task->getMesh()->feEnd(); iter++)
        {
            if ((*iter)->getId() == 362599)
            {
                Data::cout() << "after distributedMerge" << Data::endl;
                Data::cout() << "found fel " << (*iter)->text() << " at " << (*iter) << " in newMesh" << Data::endl;
                Data::cout() << "fel in this->boundary = " << std::boolalpha << this->boundary->have((*iter)) << Data::endl;
                //Data::cout() << "fel in oldBoundary = " << std::boolalpha << oldBoundary->have((*iter)) << Data::endl;
                Data::cout() << "fel in newBoundary = " << std::boolalpha << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary()->have((*iter)) << Data::endl;

                break;
            }
        }
    }
    //endebug*/

    /*//debug
    Data::cout() << "proc " << this->comm->rank() << " boundary->debug after merging, " << this->boundary << Data::endl;
    this->boundary->debug();
    //endebug*/

#if DEBUG_PMJMESH2_PROFILE
    merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE
//std::cout << "----  6 " << std::endl;
    //if (j == 0) while (true);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Real timingStart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    //Data::cout() << " debug executeShared, task->boundary at " << dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() << ", this->boundary at " << this->boundary << Data::endl;

    if (!this->setTaskParameters(task,
        dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() ?
            const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary())) :
            const_cast<MJMesh::Boundary *>(this->boundary),
        const_cast<Data::BoxTree *>(this->tree), this->idManagers[0]))
    {
        error = true;
    }

    //Data::getState().setDebug(task->getId() == 7);
    /*if (task->getId() == 0)
    {
        while (true);
    }*/

    //if (j == 1) while (true);

    /*//debug
    for (UInt i = 0; i < this->tasks.size(); i++)
    {
        PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[i]);

        if (!task)
        {
            continue;
        }

        PMJMesh::Front *front = static_cast<PMJMesh::Front *>(task->getFront());

        if (!front)
        {
            continue;
        }

        Data::cout() << "before task->execute on executeDistributedMeshGeneration on process " << this->comm->rank() << Data::endl;

        ULInt ids[4] = {94655, 94656, 93959, 94654};

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (UShort j = 0; j < 4; j++)
            {
                bool found = false;

                for (Data::Front::ConstFrontElementIterator iter = front->eBegin(j);
                     iter != front->eEnd(j); iter++)
                {
                    if ((*iter)->getId() == ids[k])
                    {
                        Data::cout() << "found fel " << ids[k] << " in front of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    break;
                }
            }
        }

        PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

        if (!drive)
        {
            continue;
        }

        MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive->getBoundary()));

        if (!boundary)
        {
            continue;
        }

        for (UInt k = 0; k < 4; k++)
        {
            Data::cout() << "searching for fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

            for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
                 iter != boundary->eEnd(); iter++)
            {
                if ((*iter)->getId() == ids[k])
                {
                    Data::cout() << "found fel " << ids[k] << " in boundary of task[" << i << "]->id " << task->getId() << " on process " << this->comm->rank() << Data::endl;

                    break;
                }
            }
        }
    }
    //endebug*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Data::ULI4Tuple prev = this->makeTuple(task);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    if (!error)
    {
#if DEBUG_PMJMESH2_PROFILE
        Real genStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

        /*//debug
        Data::cout() << "proc " << this->comm->rank() << " boundary->debug before executing, " << this->boundary << Data::endl;
        this->boundary->debug();
        //endebug*/

        task->execute();

        /*//debug
        Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executing, " << this->boundary << Data::endl;
        this->boundary->debug();
        //endebug*/

#if DEBUG_PMJMESH2_PROFILE
        generation += Data::time() - genStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

        /*std::stringstream str;
        str << "thread " << id << " finished task " << cell->getId() << Data::endl;
        Data::cout() << str.str();*/
    }

    if ((task->getStatus().getType() == task->finished.getType()) && (this->isInError(task)))
    {
        error = true;
    }

    this->getParameters(task);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Data::cout() << this->timing(this->cast(task), Data::time() - timingStart, prev) << Data::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    if (error)
    {
        return false;
    }

    return true;
}

void PMJMesh2::ParallelMeshGenerator::executeDistributedMeshGeneration()
{
    if (this->status >= this->error)
    {
        return;
    }

    if (this->comm->numProcesses() == 1)
    {
        this->status = this->distributedMemoryMeshGenerated;

        return;
    }

#if DEBUG_PMJMESH2_PROFILE
    Real initialization, communication, generation, merge, administration, finalization;
    initialization = communication = generation = merge = administration = finalization = 0.0;
    Real start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    //Data::cout() << "before splitting mesh, rank " << this->comm->rank() << ", tasks[0]->mesh = " << static_cast<PMJMesh::MJMeshTask *>(this->tasks[0])->getMesh() << Data::endl;
    //Data::cout() << "before splitting mesh, rank " << this->comm->rank() << ", tasks[0]->mesh->size = " << static_cast<PMJMesh::MJMeshTask *>(this->tasks[0])->getMesh()->size() << Data::endl;
    //Data::cout() << "before splitting mesh, rank " << this->comm->rank() << ", this->mesh = " << this->mesh << Data::endl;
    //Data::cout() << "before splitting mesh, rank " << this->comm->rank() << ", this->mesh->size = " << this->mesh->size() << Data::endl;

#if USE_GUI
    bool step = Data::isStepByStep();

    Data::setStepByStep(false);
#endif //#if USE_GUI

    bool error = false;

    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setRange(this->idrange);

    this->idManagers[0]->setId(0, this->idManager->getId(0));
    this->idManagers[0]->setId(1, this->idManager->getId(1));
    this->idManagers[0]->setId(2, this->idManager->getId(2));

#if USE_C__11
    Int max = static_cast<Int>(std::ceil(std::log2(static_cast<double>(this->comm->numProcesses()))));
#else
    Int max = static_cast<Int>(std::ceil(std::log(static_cast<double>(this->comm->numProcesses()))/std::log(2.0)));
#endif //#if USE_C__11

    //Int maxIterations = cell->level() - this->commonParent->level();

#if DEBUG_PMJMESH2_PROFILE
    initialization = Data::time() - start;
    start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    //for (Int j = 1; j <= this->comm->numProcesses(); j *= 2)
    for (Int j = 0; j <= max; j++)
    {
		/*if (true)*/
        {
            //Data::cout() << "proc " << this->comm->rank() << " boundary->debug begining of for, " << this->boundary << Data::endl;
            //this->boundary->debug();

            //Data::cout() << "j = " << j << Data::endl;

            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]);
//std::cout << "Vai executar  executeDistributed " << j << std::endl;
            bool shouldContinue = this->executeDistributed(task, error
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                , communication, merge, generation
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
                );

            if (!shouldContinue)
            {
                break;
            }

            //while (true);
        }
        /*else
        {
            //Data::cout() << "proc " << this->comm->rank() << " boundary->debug begining of for, " << this->boundary << Data::endl;
            //this->boundary->debug();

            //Data::cout() << "j = " << j << Data::endl;

            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]);

            PartitionBSPTreeCell *cell = this->cast(task)->getCell();

            PartitionBSPTreeCell *otherCell = cell->parent() ? static_cast<PartitionBSPTreeCell *>(cell->parent()->child(1)) : NULL;

            if (!otherCell)
            {
                break;
            }

            //if (j == 0) while (true);

#if DEBUG_PMJMESH2_PROFILE
            Real mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

            this->mergeCommonFront(task);

            //Data::cout() << "proc " << this->comm->rank() << " boundary->debug after mergeCommonFront, " << this->boundary << Data::endl;
            //this->boundary->debug();

            if (this->updateType == PARTIAL)
            {
                this->splitMesh(task);
            }

#if DEBUG_PMJMESH2_PROFILE
            merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

            if (this->updateType != FULL)
            {
                this->addRejectedFrontElementsThatAdvanced(task);
            }

            ULInt ids[3] = {this->idManagers[0]->getId(0), this->idManagers[0]->getId(1), this->idManagers[0]->getId(2)};

            Int peer = static_cast<PartitionBSPTreeCell *>(cell->parent())->getPeer();

            //if (j == 0) while (true);

            //Data::cout() << "proc " << this->comm->rank() << " boundary->debug after finding peer, " << this->boundary << Data::endl;
            //this->boundary->debug();

            if (otherCell == cell)
            {
                //otherCell = static_cast<PartitionBSPTreeCell *>(cell->parent()->child(0));
                //Int peer = otherCell->getPeer();

                this->send(ids, task, peer);

                break;
            }

            //Data::cout() << "rank " << this->comm->rank() << " will receive from peer " << peer << Data::endl;

            //Int peer = otherCell->getPeer();

#if DEBUG_PMJMESH2_PROFILE
            Real commStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

            PMJMesh::MJMeshTask *otherTask = this->receive(ids, peer);

#if DEBUG_PMJMESH2_PROFILE
            communication += Data::time() - commStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

#if DEBUG_PMJMESH2_PROFILE
            mergeStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

            this->distributedMerge(task, otherTask, static_cast<PartitionBSPTreeCell *>(cell->parent()));

            //Data::cout() << "proc " << this->comm->rank() << " boundary->debug after merging, " << this->boundary << Data::endl;
            //this->boundary->debug();

#if DEBUG_PMJMESH2_PROFILE
            merge += Data::time() - mergeStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

            //if (j == 0) while (true);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            Real timingStart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

            if (!this->setTaskParameters(task,
                dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary() ?
                    const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getBoundary())) :
                    const_cast<MJMesh::Boundary *>(this->boundary),
                const_cast<Data::BoxTree *>(this->tree), this->idManagers[0]))
            {
                error = true;
            }

            //Data::getState().setDebug(task->getId() == 7);
            //if (task->getId() == 0) while (true);

            //if (j == 1) while (true);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            Data::ULI4Tuple prev = this->makeTuple(task);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

            if (!error)
            {
#if DEBUG_PMJMESH2_PROFILE
                Real genStart = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

                //Data::cout() << "proc " << this->comm->rank() << " boundary->debug before executing, " << this->boundary << Data::endl;
                //this->boundary->debug();

                task->execute();

                //Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executing, " << this->boundary << Data::endl;
                //this->boundary->debug();

#if DEBUG_PMJMESH2_PROFILE
                generation += Data::time() - genStart;
#endif //#if DEBUG_PMJMESH2_PROFILE

                //Data::cout() << "thread " << id << " finished task " << cell->getId() << Data::endl;
            }

            if ((task->getStatus().getType() == task->finished.getType()) && (this->isInError(task)))
            {
                error = true;
            }

            this->getParameters(task);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
            Data::cout() << this->timing(this->cast(task), Data::time() - timingStart, prev) << Data::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

            if (error)
            {
                break;
            }

            //while (true);
        }*/
    }

#if DEBUG_PMJMESH2_PROFILE
    administration = Data::time() - start - communication - merge - generation;
    start = Data::time();
#endif //#if DEBUG_PMJMESH2_PROFILE

    this->idManager->setId(0, this->idManagers[0]->getId(0));
    this->idManager->setId(1, this->idManagers[0]->getId(1));
    this->idManager->setId(2, this->idManagers[0]->getId(2));

    dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->setMesh(NULL);

#if USE_GUI
    Data::setStepByStep(step);
#endif //#if USE_GUI

    if (error)
    {
        this->status = this->error;
    }
    else
    {
        this->status = this->distributedMemoryMeshGenerated;
    }

#if DEBUG_PMJMESH2_PROFILE
    finalization = Data::time() - start;

    if (this->comm->rank() == 0)
    {
        Data::cout() << "Time for initializing   executeDistributedMeshGeneration() = " << initialization << Data::endl;
        Data::cout() << "Time for communication  executeDistributedMeshGeneration() = " << communication << Data::endl;
        Data::cout() << "Time for merging        executeDistributedMeshGeneration() = " << merge << Data::endl;
        Data::cout() << "Time for generation     executeDistributedMeshGeneration() = " << generation << Data::endl;
        Data::cout() << "Time for administration executeDistributedMeshGeneration() = " << administration << Data::endl;
        Data::cout() << "Time for finalization   executeDistributedMeshGeneration() = " << finalization << Data::endl;
    }
#endif //#if DEBUG_PMJMESH2_PROFILE
}

void PMJMesh2::ParallelMeshGenerator::executeFrontCheck()
{
    if (this->status >= this->error)
    {
        return;
    }

    const Data::Front *front = dynamic_cast<PMJMesh::WorkerMainDrive *>(this->tasks[0]->getBuilder())->getFront();

    if (front->isEmpty())
    {
        this->status = this->success;
    }
    else
    {
        this->status = this->errorMeshNotFullyGenerated;

        std::stringstream str;

        str << front->size() << " front elements still in front";

        this->status.append(str.str());
    }
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
Data::ULI4Tuple PMJMesh2::ParallelMeshGenerator::makeTuple(const PMJMesh::MJMeshTask *task) const
{
    Data::ULI4Tuple tuple;

    Data::Front *front = (!task->getBuilder()) ? NULL :
        const_cast<PMJMesh::Front *>(static_cast<PMJMesh::Partition *>(task->getPartition())->getSubfront());
    Data::Mesh *mesh = (!task->getBuilder()) ? NULL :
        const_cast<MJMesh::Mesh *>(dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder())->getMesh());

    ULInt numVertices = mesh ? mesh->innerVerticesSize() : 0;
    ULInt numElements = mesh ? mesh->size() : 0;

    ULInt numFrontVertices = front ? front->verticesSize() : 0;
    ULInt numFrontElements = front ? front->size() : 0;

#if USE_C__11
    tuple = std::make_tuple(numVertices, numElements, numFrontVertices, numFrontElements);
#else
    tuple.values[0] = numVertices;
    tuple.values[1] = numElements;
    tuple.values[2] = numFrontVertices;
    tuple.values[3] = numFrontElements;
#endif //#if USE_C__11

    return tuple;
}

std::string PMJMesh2::ParallelMeshGenerator::timing(const PMJMesh2::Task *task, Real timeTotal, const Data::ULI4Tuple &prev) const
{
	const PMJMesh2::MJMeshTask *t = static_cast<const PMJMesh2::MJMeshTask *>(task);

	PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

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
	str << "Tree Refiner and Disrefiner = " << timeRefinement << " (" << 100.0*timeRefinement/timeTotal << "%)\n";
	str << "Search Tree Builder and Destroyer = " << timeTrees << " (" << 100.0*timeTrees/timeTotal << "%)\n";
	str << "Geometry Front Advancer = " << timeGeometry << " (" << 100.0*timeGeometry/timeTotal << "%)\n";
	str << "Topology Front Advancer = " << timeTopology << " (" << 100.0*timeTopology/timeTotal << "%)\n";
	str << "Back-Tracking Front Advancer (+ finding common front) = " << timeBackTracking << " (" << 100.0*timeBackTracking/timeTotal << "%)\n";
	str << "Mesh Improver (+ inverting new front) = " << timeImprover << " (" << 100.0*timeImprover/timeTotal << "%)\n";
	str << "Tree Classifier = " << timeClassify << " (" << 100.0*timeClassify/timeTotal << "%)\n";
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
