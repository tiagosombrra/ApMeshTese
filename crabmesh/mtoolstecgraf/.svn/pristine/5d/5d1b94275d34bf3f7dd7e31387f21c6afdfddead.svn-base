#include "PMJMesh2/APrioriParallelMeshGenerator.h"

#if USE_TECGRAF_MSH_SURF

#include "Data/State.h"
#include "Data/Triangle3D.h"
#include "Performer/RangedIdManager.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Element.h"
#include "MJMesh/ElementSurface.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MeshTransferable.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh2/WorkerMainDrive.h"
#include "PMJMesh2/MJMeshTask.h"
#include "PMJMesh2/PartitionBSPTree.h"
#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/APrioriFrontPartitioner.h"
#include "PMJMesh2/APrioriCrossingReplacer.h"

#include "PMJMesh2/TriangleExecutor.h"
#include "MJMesh/Triangle3D.h"

#include "Tools/BoundaryLaplacianSmoother.h"
#include "Tools/EdgeSwapBoundaryAdapter.h"
#include "PMJMesh/MeshUpdater.h"
#include "Data/OStream.h"

#include <iomanip>

//../../../../bin/Debug
//../../../../bin/Debug/obj

using namespace PMJMesh2;

PMJMesh2::APrioriParallelMeshGenerator::APrioriParallelMeshGenerator() :
    PMJMesh2::ParallelMeshGenerator::ParallelMeshGenerator()
{
    this->createInnerFrontInDecomposition = false;
    this->interfaceElementSizeFactor = 1.0;
}

PMJMesh2::APrioriParallelMeshGenerator::~APrioriParallelMeshGenerator()
{

}

void PMJMesh2::APrioriParallelMeshGenerator::setInterfaceElementSizeFactor(Real interfaceElementSizeFactor)
{
    this->interfaceElementSizeFactor = interfaceElementSizeFactor;
}

Real PMJMesh2::APrioriParallelMeshGenerator::getInterfaceElementSizeFactor() const
{
    return this->interfaceElementSizeFactor;
}

void PMJMesh2::APrioriParallelMeshGenerator::execute()
{
    //std::cout << "---- Executando a priori ----     " << this->status.getType() << "\t É Mestre? " << this->comm->isMaster() << std::endl;
    //std::cout << "id do processo: " << this->comm->rank() << std::endl;
    //std::stringstream str;
//    str << "Processor = " << this->comm->rank() << "\n";
//	str << "Thread = " << static_cast<Parallel::TMCommunicator *>(this->comm)->threadId() << "\n";
//	str << "---- ----  \n";
    //str << "Task = " << task->getId() << "\n";
    //std::cout << str.str() << std::endl;

    this->createInnerFrontInDecomposition = false;

    if ((this->dimension != 2) && (this->dimension != 3))
    {
        return;
    }

    if ((!this->comm) || (!this->boundary) || (!this->tree) || (!this->bsp) || (this->tasks.empty()))
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

        //this->executeFrontPartitioning();
        //this->executeDistributedFrontPartitioning();
        this->executeDistributedFrontPartitioningInParallel();

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

    //std::cout << "---- SAINDO DA EXECUCAO priori ----" << std::endl;
    //std::cout << str.str() << std::endl;
}

bool PMJMesh2::APrioriParallelMeshGenerator::setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
        Data::BoxTree *tree, Performer::IdManager *idManager) const
{
    bool set = ParallelMeshGenerator::setTaskParameters(t, boundary, tree, idManager);

    if (set)
    {
        PMJMesh2::WorkerMainDrive *drive = dynamic_cast<PMJMesh2::WorkerMainDrive *>(t->getBuilder());

        drive->setTestingBox(false);
        drive->setCreateInnerFrontInDecomposition(false);

        drive->setShouldAdvanceFront(static_cast<Data::TreeNode *>(this->bsp->node(t->getId()))->isLeaf());
    }

    return set;
}

Data::Box *PMJMesh2::APrioriParallelMeshGenerator::box(PMJMesh::MJMeshTask */*task*/) const
{
    return NULL;
}

void PMJMesh2::APrioriParallelMeshGenerator::moveFromOldToNewFront(PMJMesh::MJMeshTask *task) const
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    //std::cout << "drive->id = " << drive->getId() << std::endl;

    //if (drive1->getId() == 7) while (true);

    PMJMesh::Front *newFront = drive->getNewFront();

    PMJMesh::Front *oldFront = drive->getOldFront();

#if USE_DELETE_FELS
    MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());
#endif //#if USE_DELETE_FELS

#if USE_GUI
    oldFront->freeze();
    newFront->freeze();
#endif //#if USE_GUI

    for (Data::Front::FrontElementIterator iter = oldFront->eBegin(1);
            iter != oldFront->eEnd(1);)
    {
        Data::FrontElement *fel = (*iter);

        /*if ((fel->getId() == 1411) || (fel->getId() == 1367))
        {
            std::cout << "debug moving from old to new front " << fel->getId() << std::endl;
        }*/

        iter = oldFront->erase(iter, 1);

        bool have = newFront->have(fel);

        if (have)
        {
            if ((newFront->have(fel, 0)) || (newFront->have(fel, 1)) || (newFront->have(fel, 2)))
            {
                newFront->remove(fel);
            }
        }

        if ((!have) || (!newFront->have(fel, 1)))
        {
            newFront->addElement(fel);
        }

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

            if (!newFront->have(v))
            {
                newFront->add(v);
            }

            if (!oldFront->haveAny(v->getAdjacentFrontElements()))
            {
                oldFront->remove(v);
            }
        }
    }

#if USE_GUI
    oldFront->unfreeze();
    newFront->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh2::APrioriParallelMeshGenerator::sharedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());

    PMJMesh::Front *newFront1 = drive1->getNewFront();

#if USE_DELETE_FELS
    PMJMesh::WorkerMainDrive *drive2 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task2->getBuilder());

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Mesh *mesh2 = const_cast<MJMesh::Mesh *>(drive2->getMesh());
#endif //#if USE_DELETE_FELS

    this->moveFromOldToNewFront(task1);
    this->moveFromOldToNewFront(task2);

    //if (drive1->getId() == 7) while (true);

    PMJMesh2::ParallelMeshGenerator::sharedMerge(task1, task2, parent);

    //if (drive1->getId() == 7) while (true);

    ///PMJMesh::ParallelMeshGenerator::sharedMerge() created a different mesh,
    ///  so mesh is different from mesh1, even though they're both from drive1
    MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    Data::Boundary *boundary = const_cast<Data::Boundary *>(drive1->getBoundary());

#if USE_GUI
    newFront1->freeze();
#if USE_DELETE_FELS
    mesh1->freeze();
    mesh2->freeze();
    mesh->freeze();
#endif //#if USE_DELETE_FELS
#endif //#if USE_GUI

    /*std::cout << "drives " << drive1->getId() << " and " << drive2->getId() << std::endl;
    std::cout << "meshes at " << mesh1 << " and " << mesh2 << " mesh at " << mesh << std::endl;

    for (Data::Front::AllFrontElementIterator iter = newFront1->allEBegin();
         iter != newFront1->allEEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "newFront1 fel " << (*iter)->getId() << " at " << (*iter) << std::endl;
        }
    }*/

    for (Data::Front::AllFrontElementIterator iter = newFront1->allEBegin();
            iter != newFront1->allEEnd();)
    {
        MJMesh::FrontElement *fel1 = static_cast<MJMesh::FrontElement *>((*iter));

        Data::Front::AllFrontElementIterator iter2 = iter;

        bool remove = false;

        //shouldRemove = 0 -> remove both
        //shouldRemove = 1 -> remove only fel1 and leave fel2 in front
        //shouldRemove = 2 -> remove only fel2 and leave fel1 in front
        UInt shouldRemove = 0;

        for (++iter2; iter2 != newFront1->allEEnd();)
        {
            MJMesh::FrontElement *fel2 = static_cast<MJMesh::FrontElement *>((*iter2));

            if (fel2->getId() != fel1->getId())
            {
                ++iter2;

                continue;
            }

            /*if ((drive1->getId() == 2) && (fel1->getId() == 186413))
            {
                std::cout << "debug" << std::endl;
            }*/

            if ((fel1->numAdjacents() == 1) && (fel2->numAdjacents() == 0))
            {
                shouldRemove = 1;
            }
            else if ((fel1->numAdjacents() == 0) && (fel2->numAdjacents() == 1))
            {
                shouldRemove = 2;
            }

            if (shouldRemove == 1)
            {
                iter = newFront1->erase(iter);

                MJMesh::FrontElement *tmp = fel1;
                fel1 = fel2;
                fel2 = tmp;
            }
            else
            {
                iter2 = newFront1->erase(iter2);
            }

            this->replacer->backReplace(fel1, fel2, true, true, true, true);

            UInt numVertices = fel2->numVertices();

            Data::Vertex *shouldDelete[numVertices];

            for (UInt j = 0; j < numVertices; j++)
            {
                MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(fel2->getVertex(j));

                shouldDelete[j] = NULL;

                for (UInt i = 0; i < fel1->numVertices(); i++)
                {
                    MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(fel1->getVertex(i));

                    if (v1 == v2)
                    {
                        break;
                    }

                    if (v1->getId() != v2->getId())
                    {
                        continue;
                    }

                    /*if (v1->getId() == 207)
                    {
                        std::cout << "teste" << std::endl;
                    }*/

                    MJMesh::Vertex *v1original = this->replacer->backReplaceToOriginal(task1->getId(), v1, this->boundary);
                    MJMesh::Vertex *v2original = this->replacer->backReplaceToOriginal(task1->getId(), v2, this->boundary);

                    if (v1original)
                    {
                        v1 = v1original;
                    }

                    if (v2original)
                    {
                        v2 = v2original;
                    }

                    this->replacer->backReplace(v1, v2, true, true, true, true);

                    shouldDelete[j] = v2;
                }
            }

            /*if (fel2->getId() == 186413)
            {
                std::cout << "deleting fel2 " << fel2->text() << " at " << fel2 << std::endl;
                std::cout << "mesh1->have(fel2): " << std::boolalpha << mesh1->have(fel2) << std::endl;
                std::cout << "mesh2->have(fel2): " << std::boolalpha << mesh2->have(fel2) << std::endl;
                std::cout << "mesh->have(fel2): " << std::boolalpha << mesh->have(fel2) << std::endl;
            }*/

            /*if ((fel2->getId() == 1411) || (fel2->getId() == 1367))
            {
                std::cout << "debug APPMG1 removing/deleting from rejectedFrontElements fel " << fel2->text() << " at " << fel2 << ", backtrace: " << std::endl;
                std::cout << Data::trace() << std::endl;
            }*/

            this->rejectedFrontElements.erase(fel2);

            delete fel2;

            for (UInt i = 0; i < numVertices; i++)
            {
                if (shouldDelete[i])
                {
                    delete shouldDelete[i];
                }
            }

            if (shouldRemove != 1)
            {
                remove = true;
            }

            break;
        }

        if (!remove)
        {
            if (shouldRemove != 1)
            {
                ++iter;
            }

            continue;
        }

        iter = newFront1->erase(iter);

        for (UInt i = 0; i < fel1->numVertices(); i++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel1->getVertex(i));

            if (!newFront1->haveAny(v->getAdjacentFrontElements()))
            {
                newFront1->remove(v);

                if (this->front->have(v))
                {
                    this->front->remove(v);
                }

                if ((!boundary->have(v)) && (!mesh->haveInner(v)))
                {
                    mesh->addInner(v);
                }
            }
        }

#if USE_DELETE_FELS
        mesh1->remove(fel1);
        mesh2->remove(fel1);
        mesh->remove(fel1);

        if (this->front->have(fel1))
        {
            this->front->remove(fel1);
        }

        /*if ((fel1->getId() == 1411) || (fel1->getId() == 1367))
        {
            std::cout << "debug APPMG2 removing/deleting from rejectedFrontElements fel " << fel1->text() << " at " << fel1 << ", backtrace: " << std::endl;
            std::cout << Data::trace() << std::endl;
        }*/

        /*if (fel1->getId() == 186413)
        {
            std::cout << "deleting fel1 " << fel1->text() << " at " << fel1 << std::endl;
            std::cout << "mesh1->have(fel1): " << std::boolalpha << mesh1->have(fel1) << std::endl;
            std::cout << "mesh2->have(fel1): " << std::boolalpha << mesh2->have(fel1) << std::endl;
            std::cout << "mesh->have(fel1): " << std::boolalpha << mesh->have(fel1) << std::endl;
        }*/

        this->rejectedFrontElements.erase(fel1);

        delete fel1;
#endif //#if USE_DELETE_FELS
    }

#if USE_GUI
    newFront1->unfreeze();
#if USE_DELETE_FELS
    mesh1->unfreeze();
    mesh2->unfreeze();
    mesh->unfreeze();
#endif //#if USE_DELETE_FELS
#endif //#if USE_GUI

    /*for (Data::Mesh::FrontElementIterator iter = mesh1->feBegin();
         iter != mesh1->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh1 fel " << (*iter)->text() << " at " << (*iter) << std::endl;
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
    for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
         iter != mesh->feEnd(); iter++)
    {
        if ((*iter)->getId() == 186413)
        {
            std::cout << "mesh fel " << (*iter)->text() << " at " << (*iter) << std::endl;
        }
    }*/
    /*//debug
    std::cout << "fels in old front of drive " << drive1->getId() << std::endl;
    for (Data::Front::AllFrontElementIterator iter = oldFront1->allEBegin();
         iter != oldFront1->allEEnd(); iter++)
    {
        std::cout << "fel " << (*iter)->text() << std::endl;
    }
    std::cout << "fels in new front of drive " << drive1->getId() << std::endl;
    for (Data::Front::AllFrontElementIterator iter = newFront1->allEBegin();
         iter != newFront1->allEEnd(); iter++)
    {
        std::cout << "fel " << (*iter)->text() << std::endl;
    }
    //endebug*/

    //if (drive1->getId() == 7) while (true);
    //if (drive1->getId() == 7)
    //    std::cout << "PARA NO : " << drive1->getId() << std::endl;
}

/*
void PMJMesh2::APrioriParallelMeshGenerator::executeDistributedFrontPartitioning()
{
    std::cout << "Criando os subdomínios pela BSP" << std::endl;

    this->front = this->makeFront(this->boundary);

    this->commonParent = this->findCommonParent();

    std::cout << "CP: " << this->commonParent->getId() << std::endl;
    std::cout << "Root: " << this->bsp->getId() << std::endl;

    bool both = false;

    Data::GraphNodeList list;
    Data::UIntList listTask;

    list.push_back(this->bsp->getRoot());
    listTask.push_back(0);

    while (!list.empty())
    {
        Data::BSPTreeCell *cell = static_cast<Data::BSPTreeCell *>(list.front());
        UInt tindx = listTask.front();

        list.pop_front();
        listTask.pop_front();

        //std::cout << "ID DA CELULA: " << cell->getId() << std::endl;
        //std::cout << "CELULA Cortada na direção: " << cell->getSplitDirection() << std::endl;

        if (cell->isLeaf())
        {
            std::cout << "ID DA FOLHA: " << cell->getId() << std::endl;
            continue;
        }

        for (UInt i = 0; i < cell->numChildren(); i++)
        {
            list.push_back(cell->child(i));
        }

        if (cell == this->commonParent)
        {
            both = true;
        }

        listTask.push_back(tindx);

        if (both)
        {
            //faz nos dois
            listTask.push_back(tindx + cell->child(0)->numLeaves());

            PMJMesh::MJMeshTask *task1 = static_cast<PMJMesh::MJMeshTask *>(this->tasks[tindx]);
            PMJMesh::MJMeshTask *task2 = static_cast<PMJMesh::MJMeshTask *>(this->tasks[listTask.back()]);

            PartitioningThisFront(this->bsp->getRoot() == cell, task1, task2, static_cast<Data::BSPTreeCell *>(cell->child(0))->box(), static_cast<Data::BSPTreeCell *>(cell->child(1))->box(), cell->getSplitDirection() );
        }
        else
        {
            listTask.push_back(tindx);

            if ((cell->child(0) == this->commonParent) || ( static_cast<Data::BSPTreeCell *>( cell->child(0))->isDescendant(this->commonParent->getId()) ) )
            {
                //esquerda
                std::cout << "ID DO NÓ ESQUERDA: " << cell->getId() << std::endl;
                PartitioningThisFront(this->bsp->getRoot() == cell, static_cast<PMJMesh::MJMeshTask *>(this->tasks[tindx]), NULL, static_cast<Data::BSPTreeCell *>(cell->child(0))->box(), NULL, cell->getSplitDirection());
            }
            else
            {
                //direita
                std::cout << "ID DO NÓ DIREITA: " << cell->getId() << std::endl;
                PartitioningThisFront(this->bsp->getRoot() == cell, static_cast<PMJMesh::MJMeshTask *>(this->tasks[tindx]), NULL, NULL, static_cast<Data::BSPTreeCell *>(cell->child(1))->box(), cell->getSplitDirection());
            }
        }
    }

    std::cout << "LISTA DE NÓS FINAIS E INICIAIS: " << common_vertices.size() << std::endl;
    //std::list<Data::VertexList *>::iterator iter;
    for (Data::VertexSet::iterator iter = common_vertices.begin(); iter != common_vertices.end(); iter++)
    {
        std::cout << "ID DO VÉRTICE: " << (*iter)->getId() << std::endl;
    }

//#if USE_OPENMP
    this->replacer = new PMJMesh2::APrioriCrossingReplacer(this->dimension, true, true, this->bsp,
        this->commonParent, const_cast<MJMesh::Boundary *>(this->boundary), &this->tasks, &this->common_vertices);

    this->replacer->execute();
//#endif //#if USE_OPENMP

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

            if (front)
            {
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

    this->status = this->frontPartitioned;
}
*/

void PMJMesh2::APrioriParallelMeshGenerator::executeDistributedFrontPartitioningInParallel()
{
    //std::cout << "Criando os subdomínios pela BSP" << std::endl;

    this->front = this->makeFront(this->boundary);

    this->commonParent = this->findCommonParent();

    //std::cout << "CP: " << this->commonParent->getId() << std::endl;
    //std::cout << "Root: " << this->bsp->getId() << std::endl;

    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    Data::VertexSet common_vertices_vec[comm->getMaxThreads()];           ///ALOCA UM VETOR PARA GUARDAR OS VERTICES QUE SERÃO REPETIDOS

    ///CRIACAO e inicializacao DO idManager[0]
    UInt numProcs = comm->numProcesses();

    if (this->idManagers.empty())
    {
        this->idManagers.resize(comm->getMaxThreads(), NULL);
    }

    this->idoffset = numProcs*this->idrange;
    this->idManagers[0] = new Performer::RangedIdManager(1, 1, 1, 1, 3);

    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setRange(numProcs*this->idrange);  //eh esse mesmo, igual ao offset
    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setOffset(this->idoffset);

    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin(0, this->idManager->getId(0));
    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin(1, this->idManager->getId(1));
    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin(2, this->idManager->getId(2));

//    std::cout << "(1) this->idrange = " << this->idrange << std::endl;
//    std::cout << "range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getRange() << std::endl;
//    std::cout << "offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getOffset() << std::endl;
//    std::cout << "min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(0) << std::endl;
//    std::cout << "id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(0) << std::endl;
//    std::cout << "min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(1) << std::endl;
//    std::cout << "id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(1) << std::endl;
//    std::cout << "min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(2) << std::endl;
//    std::cout << "id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(2) << std::endl;

    ///Gerando as interfaces para a parte distribuida da arvore BSP
    Data::GraphNodeList list;

    list.push_back(this->bsp->getRoot());

    //adicionado por Markos me 16/09/2015
    /*ULInt maxId = 0;

    for (UShort i = 0; i < 4; i++)
    {
        for (Data::Front::FrontElementIterator iter = this->front->eBegin(i);
             iter != this->front->eEnd(i); iter++)
        {
            if ((*iter)->getId() > maxId)
            {
                maxId = (*iter)->getId();
            }
        }
    }*/

    while (!list.empty())
    {
        Data::BSPTreeCell *cell = static_cast<Data::BSPTreeCell *>(list.front());

        list.pop_front();

        if (cell == this->commonParent)
        {
            break;
        }

//        std::cout << "ID DA CELULA: " << cell->getId() << std::endl;
//        std::cout << "CELULA Cortada na direção: " << cell->getSplitDirection() << std::endl;

        if (cell->isLeaf())
        {
            //std::cout << "ID DA FOLHA: " << cell->getId() << std::endl;
            continue;
        }

        for (UInt i = 0; i < cell->numChildren(); i++)
        {
            list.push_back(cell->child(i));
        }

        if (cell == this->commonParent)
        {
            break;
        }

        //ULInt idChild = 0;

        ///AQUI para os processos (this->idManagers[0]) (min e range)

        if ((cell->child(0) == this->commonParent) || ( static_cast<Data::BSPTreeCell *>( cell->child(0))->isDescendant(this->commonParent->getId()) ) )
        {
            //esquerda
//            std::cout << "ID DO NÓ ESQUERDA: " << cell->getId() << std::endl;
            PartitioningThisFront(this->bsp->getRoot() == cell, static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]), NULL, static_cast<Data::BSPTreeCell *>(cell->child(0))->box(), NULL, cell->getSplitDirection(), common_vertices_vec[0], this->idManagers[0]);

            //if (cell->child(0) != this->commonParent)
            {
                int num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMaxDistLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMinDistLimit();

                static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setRange( num_pro * this->idrange );

//                std::cout << "(2.1) this->idrange = " << this->idrange << std::endl;
//                std::cout << "range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getRange() << std::endl;
//                std::cout << "offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getOffset() << std::endl;
//                std::cout << "min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(0) << std::endl;
//                std::cout << "id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(0) << std::endl;
//                std::cout << "min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(1) << std::endl;
//                std::cout << "id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(1) << std::endl;
//                std::cout << "min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(2) << std::endl;
//                std::cout << "id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(2) << std::endl;
            }

            //idChild = cell->child(0)->getId();
        }
        else
        {
            //direita
//            std::cout << "ID DO NÓ DIREITA: " << cell->getId() << std::endl;
            PartitioningThisFront(this->bsp->getRoot() == cell, static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]), NULL, NULL, static_cast<Data::BSPTreeCell *>(cell->child(1))->box(), cell->getSplitDirection(), common_vertices_vec[0], this->idManagers[0]);

            //if (cell->child(1) != this->commonParent)
            {
                int num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMaxDistLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinDistLimit();
                int shift = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinDistLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell)->getMinDistLimit();

//std::cout << "shift (cell->child(1))->getMinLimit = " << static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinDistLimit() << std::endl;
//std::cout << "shift (cell)->getMinLimit = " << static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell)->getMinDistLimit() << std::endl;

                static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin( 0, static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(0) + shift * this->idrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin( 1, static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(1) + shift * this->idrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setMin( 2, static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(2) + shift * this->idrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setRange( num_pro * this->idrange );

//                std::cout << "(2.2) this->idrange = " << this->idrange << std::endl;
//                std::cout << "shift  = " << shift << std::endl;
//                std::cout << "range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getRange() << std::endl;
//                std::cout << "offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getOffset() << std::endl;
//                std::cout << "min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(0) << std::endl;
//                std::cout << "id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(0) << std::endl;
//                std::cout << "min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(1) << std::endl;
//                std::cout << "id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(1) << std::endl;
//                std::cout << "min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(2) << std::endl;
//                std::cout << "id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(2) << std::endl;
            }

            //idChild = cell->child(1)->getId();
        }

        //adding generated front elements to global list (16/09/2015)
        /*#if USE_C__11
                ULIntFrontElementListMap::iterator it = this->interfaceFrontElements.insert(std::make_pair(idChild, Data::FrontElementHash())).first;
        #else
                ULIntFrontElementListMap::iterator it = this->interfaceFrontElements.insert(std::make_pair(idChild, Data::FrontElementSet())).first;
        #endif //#if USE_C__11*/

        //ULInt newMaxId = 0;

        PMJMesh::Front *front = static_cast<PMJMesh::Partition *>(this->tasks[0]->getPartition())->getSubfront();

        for (UShort i = 0; i < 4; i++)
        {
            for (Data::Front::FrontElementIterator iter = front->eBegin(i);
                    iter != front->eEnd(i); iter++)
            {
                Data::FrontElement *fel = (*iter);

                if (!this->boundary->have(fel))
                {
                    this->interfaceFrontElements[fel->getId()] = fel;
                }

                /*if (fel->getId() > maxId)
                {
                    (*it).second.insert(fel);
                }

                if (fel->getId() > newMaxId)
                {
                    newMaxId = fel->getId();
                }*/
            }
        }

        for (Data::Front::VertexIterator iter = front->vBegin();
                iter != front->vEnd(); iter++)
        {
            Data::Vertex *v = (*iter);

            if (!this->boundary->have(v))
            {
                this->interfaceVertices[v->getId()] = v;
            }
        }

        /*if (newMaxId > maxId)
        {
            maxId = newMaxId;
        }*/

        if (this->bsp->getRoot() == cell)
        {
            for (UShort i = 0; i < 4; i++)
            {
                for (Data::Front::FrontElementIterator iter = this->front->eBegin(i);
                        iter != this->front->eEnd(i);)
                {
                    Data::FrontElement *fel = (*iter);

                    if (this->boundary->have(fel))
                    {
                        iter++;
                    }
                    else
                    {
                        iter = this->front->erase(iter, i);
                    }
                }
            }

            for (Data::Front::VertexIterator iter = this->front->vBegin();
                    iter != this->front->vEnd();)
            {
                Data::Vertex *v = (*iter);

                if (this->boundary->have(v))
                {
                    iter++;
                }
                else
                {
                    iter = this->front->erase(iter);
                }
            }
        }
    }

    ///Gerando as interfaces para a parte compartilhada da arvore BSP

    ///CRIACAO e inicializacao DOs idManager[1..numthreads]

#if USE_C__11
    Int max = static_cast<Int>(std::ceil(std::log2(static_cast<double>(comm->getMaxThreads()))));
#else
    Int max = static_cast<Int>(std::ceil(std::log(static_cast<double>(comm->getMaxThreads()))/std::log(2.0)));
#endif //#if USE_C__11

    static_cast<Performer::RangedIdManager *>(this->idManagers[0])->setRange(this->idrange);

//    std::cout << "(3.1) this->idrange = " << this->idrange << std::endl;
//    std::cout << "[" << 0 << "]" << " range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getRange() << std::endl;
//    std::cout << "[" << 0 << "]" << " offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getOffset() << std::endl;
//    std::cout << "[" << 0 << "]" << " min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(0) << std::endl;
//    std::cout << "[" << 0 << "]" << " id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(0) << std::endl;
//    std::cout << "[" << 0 << "]" << " min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(1) << std::endl;
//    std::cout << "[" << 0 << "]" << " id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(1) << std::endl;
//    std::cout << "[" << 0 << "]" << " min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getMin(2) << std::endl;
//    std::cout << "[" << 0 << "]" << " id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[0])->getId(2) << std::endl;

    for (Int i = 1; i < comm->getMaxThreads(); i++)
    {
        if (!this->idManagers[i])
        {
            this->idManagers[i] = new Performer::RangedIdManager(1, 1, 1, 1, 3);
        }

        static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setRange(this->idrange);
        static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setOffset(this->idoffset);
        static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(0, this->idManagers[0]->getId(0));
        static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(1, this->idManagers[0]->getId(1));
        static_cast<Performer::RangedIdManager *>(this->idManagers[i])->setMin(2, this->idManagers[0]->getId(2));

//        std::cout << "(3.2) this->idrange = " << this->idrange << std::endl;
//        std::cout << "[" << i << "]" << " range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getRange() << std::endl;
//        std::cout << "[" << i << "]" << " offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getOffset() << std::endl;
//        std::cout << "[" << i << "]" << " min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(0) << std::endl;
//        std::cout << "[" << i << "]" << " id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getId(0) << std::endl;
//        std::cout << "[" << i << "]" << " min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(1) << std::endl;
//        std::cout << "[" << i << "]" << " id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getId(1) << std::endl;
//        std::cout << "[" << i << "]" << " min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getMin(2) << std::endl;
//        std::cout << "[" << i << "]" << " id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[i])->getId(2) << std::endl;
    }

    ULInt tidrange = this->idrange / comm->getMaxThreads();

#if USE_OPENMP
    if (this->commonParent)
    {
        this->setLock(this->commonParent, true);
    }

    this->commonParent->getLock().unset();

    #pragma omp parallel if (comm->isSharedParallelismEnabled())
#endif //#if USE_OPENMP
    {
        Int id = comm->threadId();

        //Data::cout() << "debug tid " << id << ", min " << id*this->numTasksPerThread << ", max " << (id + 1)*this->numTasksPerThread << Data::endl;

        /*if (id == 0)
        {
            Data::cout() << "debug" << Data::endl;
        }*/

        ///Pegando a celula da BSP correta
        PartitionBSPTreeCell *threadCommonParent = NULL;

        if (this->numTasksPerThread == 1)
        {
            threadCommonParent = this->cast(this->tasks[id])->getCell();
        }
        else
        {
            threadCommonParent = this->findCommonParent(id*this->numTasksPerThread, (id + 1)*this->numTasksPerThread);
        }

        PartitionBSPTreeCell *cell = threadCommonParent;

#if ((USE_MPI) && (USE_OPENMP))
        Int maxIterations = -1;
#endif //#if ((USE_MPI) && (USE_OPENMP))

        //Data::cout() << "debug bef tid " << id << ", cp " << this->commonParent->getId() << ", tcp " << threadCommonParent->getId() << ", cell " << cell->getId() << Data::endl;

        for (Int j = 0; j <= max; j++)
        {
#if ((USE_MPI) && (USE_OPENMP))
            if ((maxIterations == -1) && (this->commonParent))
            {
                maxIterations = cell->level() - this->commonParent->level();
            }
#endif //#if ((USE_MPI) && (USE_OPENMP))

#if USE_MPI
            if ((comm->numProcesses() > 1) &&
                    ((j == max)
#if USE_OPENMP
                     || ((maxIterations != -1) && (j == maxIterations))
#endif //#if USE_OPENMP
                    ))
            {
                //Data::cout() << "debug brk1 tid " << id << ", cp " << this->commonParent->getId() << ", tcp " << threadCommonParent->getId() << ", cell " << cell->getId() << Data::endl;

                break;
            }
#endif //#if USE_MPI

            PartitionBSPTreeCell *otherCell = cell->parent() ? static_cast<PartitionBSPTreeCell *>(cell->parent()->child(1)) : NULL;

            if ((!otherCell) || (otherCell == cell))
            {
                //Data::cout() << "debug brk2 tid " << id << ", cp " << this->commonParent->getId() << ", tcp " << threadCommonParent->getId() << ", cell " << cell->getId() << Data::endl;

                break;
            }

            cell = static_cast<PartitionBSPTreeCell *>(cell->parent());

            //Data::cout() << "debug upd tid " << id << ", cp " << this->commonParent->getId() << ", tcp " << threadCommonParent->getId() << ", cell " << cell->getId() << Data::endl;
        }

        //Data::cout() << "debug fin tid " << id << ", cp " << this->commonParent->getId() << ", tcp " << threadCommonParent->getId() << ", cell " << cell->getId() << Data::endl;

        ///Gerando as interfaces para a parte compartilhada da arvore BSP

        Data::GraphNodeList list;
        Data::UIntList listTask;

        list.push_back(cell);
        listTask.push_back(id*this->numTasksPerThread);

        bool both = false;

        while (!list.empty())
        {
            PMJMesh2::PartitionBSPTreeCell *cell = static_cast<PMJMesh2::PartitionBSPTreeCell *>(list.front());
            UInt tindx = listTask.front();

            list.pop_front();
            listTask.pop_front();
            /*
                        Data::cout() << "tid " << id << " ID DA CELULA: " << cell->getId() << Data::endl;
                        Data::cout() << "tid " << id << " CELULA Cortada na direção: " << cell->getSplitDirection() << Data::endl;
                        Data::cout() << "tid " << id << " limites: " << static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell)->getMinLimit() << ", " << static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell)->getMaxLimit() << Data::endl;
            */
            if (cell->isLeaf())
            {
                //Data::cout() << "ID DA FOLHA: " << cell->getId() << Data::endl;
                continue;
            }

            ULInt otherTindx = tindx + cell->child(0)->numLeaves();

            if (cell == threadCommonParent)
            {
                both = true;
            }

            listTask.push_back(tindx);

            if (both)
            {
                for (UInt i = 0; i < cell->numChildren(); i++)
                {
                    list.push_back(cell->child(i));
                }

                listTask.push_back(otherTindx);
            }
            else
            {
                list.push_back(cell->child(0));
            }

            PMJMesh::MJMeshTask *task1 = static_cast<PMJMesh::MJMeshTask *>(this->tasks[tindx]);
            PMJMesh::MJMeshTask *task2 = static_cast<PMJMesh::MJMeshTask *>(this->tasks[otherTindx]);

#if USE_OPENMP
            cell->getLock().set();
#endif //#if USE_OPENMP

            PartitioningThisFront(this->bsp->getRoot() == cell, task1, task2, static_cast<Data::BSPTreeCell *>(cell->child(0))->box(), static_cast<Data::BSPTreeCell *>(cell->child(1))->box(), cell->getSplitDirection(), common_vertices_vec[id], this->idManagers[id] );

//            std::cout << "(4) this->idrange = " << this->idrange << std::endl;
//            std::cout << "tidrange = " << tidrange << std::endl;
//            std::cout << "[" << id << "]" << " range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getRange() << std::endl;
//            std::cout << "[" << id << "]" << " offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getOffset() << std::endl;
//            std::cout << "[" << id << "]" << " min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(0) << std::endl;
//            std::cout << "[" << id << "]" << " id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(0) << std::endl;
//            std::cout << "[" << id << "]" << " min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(1) << std::endl;
//            std::cout << "[" << id << "]" << " id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(1) << std::endl;
//            std::cout << "[" << id << "]" << " min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(2) << std::endl;
//            std::cout << "[" << id << "]" << " id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(2) << std::endl;

            if(!both)
            {
                ///AQUI para as threads (this->idManagers[id]) (min e range)

                int otherId = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinSharLimit();
                int num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMaxSharLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinSharLimit();
                int shift = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinSharLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell)->getMinSharLimit();

                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setId( 0, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(0) );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setMin( 0, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(0) + shift * tidrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setId( 1, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(1) );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setMin( 1, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(1) + shift * tidrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setId( 2, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(2) );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setMin( 2, static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(2) + shift * tidrange );
                static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->setRange( num_pro * tidrange );

//                std::cout << "(4.1) this->idrange = " << this->idrange << std::endl;
//                std::cout << "[" << otherId << "]" << " range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getRange() << std::endl;
//                std::cout << "[" << otherId << "]" << " offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getOffset() << std::endl;
//                std::cout << "[" << otherId << "]" << " min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(0) << std::endl;
//                std::cout << "[" << otherId << "]" << " id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getId(0) << std::endl;
//                std::cout << "[" << otherId << "]" << " min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(1) << std::endl;
//                std::cout << "[" << otherId << "]" << " id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getId(1) << std::endl;
//                std::cout << "[" << otherId << "]" << " min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(2) << std::endl;
//                std::cout << "[" << otherId << "]" << " id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getId(2) << std::endl;

//                Data::cout() << "tid " << id << ", num_pro (dir) = " << num_pro << Data::endl;
//                Data::cout() << "tid " << id << ", otherId (dir) = " << otherId << Data::endl;
//                Data::cout() << "tid " << id << ", shift   (dir) = " << shift << Data::endl;
//                Data::cout() << "tid " << id << ", range   (dir) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getRange() << Data::endl;
//                Data::cout() << "tid " << id << ", min[0]  (dir) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(0) << Data::endl;
//                Data::cout() << "tid " << id << ", min[1]  (dir) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(1) << Data::endl;
//                Data::cout() << "tid " << id << ", min[2]  (dir) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[otherId])->getMin(2) << Data::endl;

                num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMaxSharLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMinSharLimit();
                static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setRange( num_pro * tidrange );

//                std::cout << "(4.2) this->idrange = " << this->idrange << std::endl;
//                std::cout << "[" << id << "]" << " range  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getRange() << std::endl;
//                std::cout << "[" << id << "]" << " offset = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getOffset() << std::endl;
//                std::cout << "[" << id << "]" << " min[0] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(0) << std::endl;
//                std::cout << "[" << id << "]" << " id[0]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(0) << std::endl;
//                std::cout << "[" << id << "]" << " min[1] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(1) << std::endl;
//                std::cout << "[" << id << "]" << " id[1]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(1) << std::endl;
//                std::cout << "[" << id << "]" << " min[2] = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(2) << std::endl;
//                std::cout << "[" << id << "]" << " id[2]  = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getId(2) << std::endl;
//
//                Data::cout() << "tid " << id << ", num_pro (esq) = " << num_pro << Data::endl;
//                Data::cout() << "tid " << id << ", range   (esq) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getRange() << Data::endl;
//                Data::cout() << "tid " << id << ", min[0]  (esq) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(0) << Data::endl;
//                Data::cout() << "tid " << id << ", min[1]  (esq) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(1) << Data::endl;
//                Data::cout() << "tid " << id << ", min[2]  (esq) = " << static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin(2) << Data::endl;

                /*if ((cell->child(0) == threadCommonParent) || ( static_cast<Data::BSPTreeCell *>( cell->child(0))->isDescendant(threadCommonParent->getId()) ) )
                {
                    //esquerda
                    std::cout << "ID DO NÓ ESQUERDA: " << cell->getId() << std::endl;

                    if (cell->child(0) != threadCommonParent)
                    {
                        int num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMaxLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getMinLimit();

                        static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setRange( num_pro * tidrange );
                    }
                }
                else
                {
                    //direita
                    std::cout << "ID DO NÓ DIREITA: " << cell->getId() << std::endl;

                    if (cell->child(1) != threadCommonParent)
                    {
                        int num_pro = static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMaxLimit() - static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getMinLimit();

                        static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setMin( static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin() +
                                                                                           num_pro * tidrange );
                        static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setRange( num_pro * tidrange );
                    }
                }*/

                //static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setMin( static_cast<Performer::RangedIdManager *>(this->idManagers[id])->getMin() +
                //cell->numLeaves() + static_cast<Performer::RangedIdManager *>(this->idManager)->getRange() );
                //static_cast<Performer::RangedIdManager *>(this->idManagers[id])->setRange( this->idrange / comm->numThreads() );
            }

#if USE_OPENMP
            cell->getLock().unset();

            static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(0))->getLock().unset();
            static_cast<PMJMesh2::PartitionBSPTreeCell *>(cell->child(1))->getLock().unset();
#endif //#if USE_OPENMP
        }
    }

    if ((this->numTasksPerThread <= 1) && (comm->getMaxThreads() <= 1) && (comm->numProcesses() <= 1))
    {
        ///De markos em 14/09/2015, copiado e colado de PMJMesh2::ParallelMeshGenerator

        PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[0]);

#if USE_C__11
        Data::FrontElementHash *rejecteds = NULL;
#else
        Data::FrontElementSet *rejecteds = NULL;
#endif //#if USE_C__11

        bool addInRejected = ((!static_cast<PMJMesh2::Communicator *>(this->comm)->isSharedParallelismEnabled()) && (this->numTasksPerThread == 1));

        FrontPartitioner partitioner(this->dimension, addInRejected, task, this->front);

        partitioner.execute();

        rejecteds = partitioner.getRejected();

        task->setPartition(task->getPartition());

        if (!addInRejected)
        {
            PMJMesh::Partition *partition = static_cast<PMJMesh::Partition *>(task->getPartition());
            PMJMesh::Front *front = static_cast<PMJMesh::Front *>(partition->getSubfront());

#if USE_C__11
            for (Data::FrontElementHash::iterator iter = rejecteds->begin();
#else
            for (Data::FrontElementSet::iterator iter = rejecteds->begin();
#endif //#if USE_C__11
                    iter != rejecteds->end(); iter++)
            {
                Data::FrontElement *e = (*iter);

                front->addRejected(e);

#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
                    this->rejectedFrontElements.insert(e);

                    /*if ((e->getId() == 1411) || (e->getId() == 1367))
                    {
                        std::cout << "debug APPMG3 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                        std::cout << Data::trace() << std::endl;
                    }*/

                    //std::cout << "inserting (3) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                }
#endif //#if USE_MPI

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                    if (!front->have(v))
                    {
                        front->add(v);
                    }
                }
            }

            delete rejecteds;

            this->tasks[0]->setPartition(partition);
        }
        else
        {
            if (rejecteds)
            {
#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
#if USE_C__11
                    for (Data::FrontElementHash::iterator iter = rejecteds->begin();
#else
                    for (Data::FrontElementSet::iterator iter = rejecteds->begin();
#endif //#if USE_C__11
                            iter != rejecteds->end(); iter++)
                    {
                        Data::FrontElement *e = (*iter);

                        this->rejectedFrontElements.insert(e);

                        /*if ((e->getId() == 1411) || (e->getId() == 1367))
                        {
                            std::cout << "debug APPMG4 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                            std::cout << Data::trace() << std::endl;
                        }*/

                        //std::cout << "inserting (4) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                    }
                }
#endif //#if USE_MPI

                delete rejecteds;
            }
        }
    }

    for(Int i= 1; i<comm->getMaxThreads(); i++)
    {
        common_vertices_vec[0].insert( common_vertices_vec[i].begin(), common_vertices_vec[i].end() );
    }

    ///CRIAR UMA common_vertices AQUI!!!!! !!
    /*
        std::cout << "LISTA DE NÓS FINAIS E INICIAIS: " << common_vertices_vec[0].size() << std::endl;
        for (Data::VertexSet::iterator iter = common_vertices_vec[0].begin(); iter != common_vertices_vec[0].end(); iter++)
        {
            std::cout << "ID DO VÉRTICE: " << (*iter)->getId() << std::endl;
        }
    */
    this->common_vertices_set.swap(common_vertices_vec[0]);

//#if USE_OPENMP
    this->replacer = new PMJMesh2::APrioriCrossingReplacer(this->dimension, true, true, this->bsp,
            this->commonParent, const_cast<MJMesh::Boundary *>(this->boundary), &this->tasks, &this->common_vertices_set);

    this->replacer->execute();
//#endif //#if USE_OPENMP

    /*//debug
    #if USE_GUI
        for (UInt i = 0; i < this->tasks.size(); i++)
        {
            PMJMesh::Partition *partition = static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition());
            PMJMesh::Front *sub_front = partition->getSubfront();

            if (!sub_front)
            {
                continue;
            }

            for (Data::Front::AllFrontElementIterator iter = sub_front->allEBegin();
                 iter != sub_front->allEEnd(); iter++)
            {
                if (!this->boundary->have((*iter)))
                {
                    const_cast<MJMesh::Boundary *>(this->boundary)->add((*iter));
                }
            }

            for (Data::Front::VertexIterator iter = sub_front->vBegin();
                 iter != sub_front->vEnd(); iter++)
            {
                if (!this->boundary->have((*iter)))
                {
                    const_cast<MJMesh::Boundary *>(this->boundary)->add((*iter));
                }
            }
        }
    #endif //#if USE_GUI
    //endebug*/

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

            if (front)
            {
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

    this->status = this->frontPartitioned;

    //std::cout << "Subdomínios da BSP criados" << std::endl;
}

void PMJMesh2::APrioriParallelMeshGenerator::PartitioningThisFront(bool first, PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, Data::Box *box1, Data::Box *box2, UInt direction, Data::VertexSet &common_vertices_list, Performer::IdManager *idManager)
{
    //std::cout << "Criando uma partição" << std::endl;

    PMJMesh::Partition *partition1 = static_cast<PMJMesh::Partition *>(task1->getPartition());
    PMJMesh::Front *sub_front1 = partition1->getSubfront();
    Data::Box *pbox1 = partition1->getBox();
    partition1->setBox(box1 ? box1 : box2);

    PMJMesh::Partition *partition2 = NULL;
    PMJMesh::Front *sub_front2 = NULL;
    Data::Box *pbox2 = NULL;

    if (task2)
    {
        partition2 = static_cast<PMJMesh::Partition *>(task2->getPartition());
        sub_front2 = partition2->getSubfront();
        pbox2 = partition2->getBox();
        partition2->setBox(box2);
    }

    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    UInt numThreads = comm->getMaxThreads();

#if USE_C__11
    Data::FrontElementHash *rejecteds[2] = {NULL, NULL};
#else
    Data::FrontElementSet *rejecteds[2] = {NULL, NULL};
#endif //#if USE_C__11

    bool addInRejected = ((!static_cast<PMJMesh2::Communicator *>(this->comm)->isSharedParallelismEnabled()) && (this->numTasksPerThread == 1));

    PMJMesh::Front *front = this->front;

    if (!first)
    {
        front = sub_front1;
        partition1->setSubfront(NULL);
    }

    Real coord = box1 ? box1->getPoint(1)->getCoord(direction) : box2->getPoint(0)->getCoord(direction);

    //std::cout << "######### APrioriFrontPartitioner for task " << task1->getId() << "  Direction: " << direction << std::endl;


    ///TESTE DO PLANO COM A LISTA rejecteds[0]. SE FOR RUIM O ANGULO, GERAR UM NOVO VALOR PARA COORD E CHAMAR APrioriFrontPartitioner NOVAMENTE
    /*if (this->dimension == 3)
    {
        ///Calcular o delta, e verificar se eh += ou -=;
        Real delta =  2.0 * this->tree->size() / (std::pow(2.0, this->tree->depth()));
        Real op = 1.0;
        int vezes = 0;
        UInt maxBadAngle = 0;
        Real badAngleCos = 0.93;
        Real coord_ori = coord;

        while( true )
        {
            APrioriFrontPartitioner partitioner(this->dimension, addInRejected, task1, front, direction, coord);        ///ESSE É O CASO INICIAL, MODIFICAR PARA OS PROXIMOS

            partitioner.execute();

            rejecteds[0] = partitioner.getRejected();         ///esta lista está na crossing para o Apriori

            sub_front1 = partition1->getSubfront();

            Data::Vector3D v3d;
            v3d.setCoord((direction)%3, 1.0);

            UInt countBadAngle = 0;

            //std::cout << "vez " << vezes << std::endl;

            for (Data::FrontElementHash::iterator iter = rejecteds[0]->begin();
                    iter != rejecteds[0]->end(); iter++)
            {
                MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement3D *>((*iter));

                //std::cout << "direction " << direction << " testing bad angle with fel " << fel->getId() << " (2)" << std::endl;

                Data::Triangle3D *triangle = static_cast<Data::Triangle3D *>(fel->getShape());

                Data::Vector3D normal = triangle->normal();

                Real cos = normal.cosAngle(v3d);

                if ( fabs(cos) > badAngleCos )
                {
                    //std::cout << "bad angle: dir " << direction << "; coord " << coord << "; fel " << fel->getId() << "; cosangle " << fabs(cos) << " (2)" << std::endl;

                    countBadAngle++;
                }

                if (countBadAngle > maxBadAngle)
                {
                    break;
                }
            }

            vezes++;

            std::cout << "Faces ruins: " << countBadAngle << std::endl;

            if (countBadAngle <= maxBadAngle )
            {
                std::cout << "- - - - Numero de tentativas para o plano ideal: " << vezes << " na coord: " << coord << std::endl;
                break;
            }

            if( vezes > 20 )
            {
                std::cout << "Não achou plano ideal" << std::endl;
                coord = coord_ori;
                break;
            }

            op *= -1.0;
            coord += delta * vezes * op;
            //coord += delta;

            if (box1)
            {
                box1->getPoint(1)->setCoord(direction, coord);
                //std::cout << "BOX 1 Coord: " << coord << std::endl;
            }

            if (box2)
            {
                box2->getPoint(0)->setCoord(direction, coord);
                //std::cout << "BOX 2 Coord: " << coord << std::endl;
            }

            sub_front1->clear();

            delete rejecteds[0];

            rejecteds[0] = NULL;
        }

        task1->setPartition(task1->getPartition());
    }
    else*/
    {
        APrioriFrontPartitioner partitioner(this->dimension, addInRejected, task1, front, direction, coord);        ///ESSE É O CASO INICIAL, MODIFICAR PARA OS PROXIMOS

        partitioner.execute();

        rejecteds[0] = partitioner.getRejected();         ///esta lista está na crossing para o Apriori

        task1->setPartition(task1->getPartition());

        sub_front1 = partition1->getSubfront();
    }

    if (task2)
    {
        coord = box2->getPoint(0)->getCoord(direction);

        //std::cout << "APrioriFrontPartitioner for task " << task2->getId() << std::endl;

        APrioriFrontPartitioner partitioner(this->dimension, addInRejected, task2, front, direction, coord);        ///ESSE É O CASO INICIAL, MODIFICAR PARA OS PROXIMOS

        partitioner.execute();

        rejecteds[1] = partitioner.getRejected();         ///esta lista está na crossing para o Apriori

        task2->setPartition(task2->getPartition());

        sub_front2 = partition2->getSubfront();
    }

    if ((!task2) && (!first))
    {
        for (Data::Front::FrontElementIterator iter = front->eBegin(1);
                iter != front->eEnd(1); iter++)
        {
            if (!sub_front1->have((*iter)))
            {
                this->rejectedFrontElements.insert((*iter));

                /*if (((*iter)->getId() == 1411) || ((*iter)->getId() == 1367))
                {
                    std::cout << "debug APPMG5 inserting into rejectedFrontElements fel " << (*iter)->text() << " at " << (*iter) << ", backtrace: " << std::endl;
                    std::cout << Data::trace() << std::endl;
                }*/

                //std::cout << "inserting (5) fel " << (*iter)->text() << " at " << (*iter) << " in rejectedFrontElements" << std::endl;
            }
        }
    }

    partition1->setBox(pbox1);

    if (task2)
    {
        partition2->setBox(pbox2);
    }

    if (!addInRejected)
    {
        //std::cout << "finishing APrioriFrontPartitioner for task " << task1->getId() << std::endl;

#if USE_C__11
        for (Data::FrontElementHash::iterator iter = rejecteds[0]->begin();
#else
        for (Data::FrontElementSet::iterator iter = rejecteds[0]->begin();
#endif //#if USE_C__11
                iter != rejecteds[0]->end(); iter++)
        {
            Data::FrontElement *e = (*iter);

            sub_front1->addCrossing(e);              ///LISTA DAS CROSSING SENDO PREENCHIDA

            //std::cout << " " << e->text() << " in crossing added" << std::endl;

#if USE_MPI
            if (this->updateType != PMJMesh2::FULL)
            {
                this->rejectedFrontElements.insert(e);

                /*if ((e->getId() == 1411) || (e->getId() == 1367))
                {
                    std::cout << "debug APPMG6 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                    std::cout << Data::trace() << std::endl;
                }*/

                //std::cout << "inserting (6) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
            }
#endif //#if USE_MPI

            for (UInt i = 0; i < e->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                if (!sub_front1->have(v))
                {
                    sub_front1->add(v);
                }
            }
        }

        delete rejecteds[0];

        rejecteds[0] = NULL;

        task1->setPartition(partition1);

        if (task2)
        {
            //std::cout << "finishing APrioriFrontPartitioner for task " << task2->getId() << std::endl;

#if USE_C__11
            for (Data::FrontElementHash::iterator iter = rejecteds[1]->begin();
#else
            for (Data::FrontElementSet::iterator iter = rejecteds[1]->begin();
#endif //#if USE_C__11
                    iter != rejecteds[1]->end(); iter++)
            {
                Data::FrontElement *e = (*iter);

                sub_front2->addCrossing(e);              ///LISTA DAS CROSSING SENDO PREENCHIDA

                //std::cout << " " << e->text() << " in crossing added" << std::endl;

#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
                    this->rejectedFrontElements.insert(e);

                    /*if ((e->getId() == 1411) || (e->getId() == 1367))
                    {
                        std::cout << "debug APPMG7 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                        std::cout << Data::trace() << std::endl;
                    }*/

                    //std::cout << "inserting (7) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                }
#endif //#if USE_MPI

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                    if (!sub_front2->have(v))
                    {
                        sub_front2->add(v);
                    }
                }
            }

            delete rejecteds[1];

            rejecteds[1] = NULL;

            task2->setPartition(partition2);
        }

    }
    else        ///MEM DISTRIBUIDA
    {
        for (UInt i = 0; i < 2; i++)
        {
            //std::cout << "finishing APrioriFrontPartitioner -> insert in rejectedFrontElements" << std::endl;

            if (rejecteds[i])
            {
#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
#if USE_C__11
                    for (Data::FrontElementHash::iterator iter = rejecteds[i]->begin();
#else
                    for (Data::FrontElementSet::iterator iter = rejecteds[i]->begin();
#endif //#if USE_C__11
                            iter != rejecteds[i]->end(); iter++)
                    {
                        Data::FrontElement *e = (*iter);

                        //std::cout << " " << e->text() << std::endl;

                        this->rejectedFrontElements.insert(e);

                        /*if ((e->getId() == 1411) || (e->getId() == 1367))
                        {
                            std::cout << "debug APPMG8 inserting into rejectedFrontElements fel " << e->text() << " at " << e << ", backtrace: " << std::endl;
                            std::cout << Data::trace() << std::endl;
                        }*/

                        //std::cout << "inserting (8) fel " << e->text() << " at " << e << " in rejectedFrontElements" << std::endl;
                    }
                }
#endif //#if USE_MPI

                delete rejecteds[i];

                rejecteds[i] = NULL;
            }
        }
    }

    //std::cout << "Task Partição1 : " << task1->getId() << std::endl;
    //if (task2) std::cout << "Task Partição2 : " << task2->getId() << std::endl;

    //if(task1->getId() != 50 && task2->getId() != 50)
    //return;

    //PMJMesh::Front *oldFront = this->front;

    //this->front = front;

    ///SE DER MERDA NOS IDS TIRAR ESSE idManager2
    Performer::RangedIdManager *idManager2 = NULL;
    if (task2)
    {
        idManager2 = new Performer::RangedIdManager(1, 1, 1, 1, 3);

        idManager2->setOffset(static_cast<Performer::RangedIdManager *>(idManager)->getOffset());
        idManager2->setRange(static_cast<Performer::RangedIdManager *>(idManager)->getRange());

        idManager2->setMin(0, static_cast<Performer::RangedIdManager *>(idManager)->getMin(0));
        idManager2->setMin(1, static_cast<Performer::RangedIdManager *>(idManager)->getMin(1));
        idManager2->setMin(2, static_cast<Performer::RangedIdManager *>(idManager)->getMin(2));

        idManager2->setId(0, static_cast<Performer::RangedIdManager *>(idManager)->getId(0));
        idManager2->setId(1, static_cast<Performer::RangedIdManager *>(idManager)->getId(1));
        idManager2->setId(2, static_cast<Performer::RangedIdManager *>(idManager)->getId(2));
    }

    buildSubDomain(box1 ? box1 : box2, sub_front1, front, common_vertices_list, idManager, task1->getId(), box1 ? direction : direction + this->dimension, task2 ? false : true);

    task1->setPartition(task1->getPartition());

    if (task2)
    {
        //buildSubDomain(box2, sub_front2, front, common_vertices_list, idManager, task2->getId(), direction + dimension, true);
        buildSubDomain(box2, sub_front2, front, common_vertices_list, idManager2, task2->getId(), direction + this->dimension, true);

        task2->setPartition(task2->getPartition());

        delete idManager2;
    }

    //this->front = oldFront;

    if (!first)
    {
        front->clear();

        delete front;
    }
}

/*
void PMJMesh2::APrioriParallelMeshGenerator::executeFrontPartitioning()
{
    std::cout << "Criando os subdomínios" << std::endl;

    this->front = this->makeFront(this->boundary);

    Parallel::TMCommunicator *comm = static_cast<Parallel::TMCommunicator *>(this->comm);

    UInt numThreads = comm->getMaxThreads();

#if USE_C__11
    Data::FrontElementHash *rejecteds[numThreads*this->numTasksPerThread];
#else
    Data::FrontElementSet *rejecteds[numThreads*this->numTasksPerThread];
#endif //#if USE_C__11

    bool addInRejected = ((!static_cast<PMJMesh2::Communicator *>(this->comm)->isSharedParallelismEnabled()) && (this->numTasksPerThread == 1));

    std::cout << "Número de Threads: " << numThreads << std::endl;
    std::cout << "Número de tarefas: " << numTasksPerThread << std::endl;

#if USE_OPENMP
        //#pragma omp parallel for if(static_cast<PMJMesh::Communicator *>(this->comm)->isSharedParallelismEnabled())
        #pragma omp parallel if (comm->isSharedParallelismEnabled())
#endif //#if USE_OPENMP
    {
        Int id = comm->threadId();

        for(UInt pos = 0; pos < numTasksPerThread; pos++)
        {
            int position = id*this->numTasksPerThread + pos;

            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>(this->tasks[position]);

            FrontPartitioner partitioner(this->dimension, addInRejected, task, this->front);

            partitioner.execute();

            rejecteds[position] = partitioner.getRejected();         ///esta lista está na crossing para o Apriori

            task->setPartition(task->getPartition());
        }
    }

    if (!addInRejected)
    {
        for (UInt i = 0; i < this->tasks.size(); i++)       ///COLOCA OS ELEMMENTOS NA LISTA CROSSING
        {
            PMJMesh::Partition *partition = static_cast<PMJMesh::Partition *>(this->tasks[i]->getPartition());
            PMJMesh::Front *sub_front = partition->getSubfront();
            Data::Box *box = partition->getBox();

#if USE_C__11
            for (Data::FrontElementHash::iterator iter = rejecteds[i]->begin();
#else
            for (Data::FrontElementSet::iterator iter = rejecteds[i]->begin();
#endif //#if USE_C__11
                 iter != rejecteds[i]->end(); iter++)
            {
                Data::FrontElement *e = (*iter);

                sub_front->addCrossing(e);              ///LISTA DAS CROSSING SENDO PREENCHIDA

#if USE_MPI
                if (this->updateType != PMJMesh2::FULL)
                {
                    this->rejectedFrontElements.insert(e);
                }
#endif //#if USE_MPI

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                    if (!sub_front->have(v))
                    {
                        sub_front->add(v);
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


#if USE_OPENMP
        //#pragma omp parallel for if(static_cast<PMJMesh::Communicator *>(this->comm)->isSharedParallelismEnabled())
        #pragma omp parallel if (comm->isSharedParallelismEnabled())
#endif //#if USE_OPENMP
        {
            Int id = comm->threadId();

            for(UInt pos = 0; pos < numTasksPerThread; pos++)
            {
                int posicao = id*this->numTasksPerThread + pos;

                PMJMesh::Front *sub_front = static_cast<PMJMesh::Partition*>(this->tasks[posicao]->getPartition())->getSubfront();
                Data::Box *box = static_cast<PMJMesh::Partition*>(this->tasks[posicao]->getPartition())->getBox();

                std::cout << "Task Partição: " << this->tasks[posicao]->getId() << std::endl;

                //if(this->tasks[posicao]->getId() != 51 && this->tasks[posicao]->getId() != 11)
                    //continue;


                buildSubDomain(box, sub_front, this->tasks[posicao]->getId(), 0, false);
                buildSubDomain(box, sub_front, this->tasks[posicao]->getId(), 1, false);
                buildSubDomain(box, sub_front, this->tasks[posicao]->getId(), 2, false);
                buildSubDomain(box, sub_front, this->tasks[posicao]->getId(), 3, false);

                this->tasks[posicao]->setPartition(this->tasks[posicao]->getPartition());
            }
        }
    }
    else        ///MEM DISTRIBUIDA
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

    this->status = this->frontPartitioned;
}
*/
void PMJMesh2::APrioriParallelMeshGenerator::buildSubDomain(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, ULInt taskId, UInt side, bool updateBox)

{
    //std::cout << " - - - - - - - - - - - Criando os segmentos de interfaces da Task: " << taskId << std::endl;
    /*
    #pragma omp critical
    {
        std::cout << "----------- ANTES --------------" << std::endl;
        std::cout << "Na subfront em " << sub_front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< sub_front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = sub_front->eBegin(i); it != sub_front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << sub_front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = sub_front->vBegin(); it != sub_front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }

        std::cout << "na front em " << this->front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< this->front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = this->front->eBegin(i); it != this->front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << this->front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = this->front->vBegin(); it != this->front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }
    }
    */

    sub_front->sortVertexs();

    if ( this->dimension == 3 )
    {
        buildSubDomain3D(box, sub_front, local_front, common_vertices_list, id_Manager, taskId, side, updateBox);
        return;
    }



    std::list<Data::TreeNode *> topBoundaryCells;
    std::list<Data::TreeNode *> baseBoundaryCells;
    std::list<Data::TreeNode *> leftBoundaryCells;
    std::list<Data::TreeNode *> rightBoundaryCells;

    //Data::FrontElementList element_front = this->front->getBoundary();
    Data::FrontElementList element_front = local_front->getBoundary();

    Data::TreeNode *root;

    root = tree->getRoot();

    interceptPartitionFront( box, root, topBoundaryCells, baseBoundaryCells, leftBoundaryCells, rightBoundaryCells, side);

    std::list<Data::TreeNode *> onTopBoundaryCells;
    std::list<Data::TreeNode *> onBaseBoundaryCells;
    std::list<Data::TreeNode *> onLeftBoundaryCells;
    std::list<Data::TreeNode *> onRightBoundaryCells;

    //UInt dim = box->dimension();
    std::list<Data::TreeNode *>::iterator iter;
    std::list<Data::TreeNode *>::iterator iter2;

    Data::FrontElementList interfaceEdges;
    Data::FrontElementList swappingEdges;


    switch(side)
    {

    ///DIREITA
    case 0:
    {
        //std::cout << "DIREITA" << std::endl;
        //if( rightBoundaryCells.size() == 0 ) continue;
        if( rightBoundaryCells.size() == 0 ) break;

        //DEIXA APENAS ARESTAS IN
        for (iter = rightBoundaryCells.begin(); iter != rightBoundaryCells.end(); )
        {

            if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
            {
                onRightBoundaryCells.push_back( (*iter) );

                iter = rightBoundaryCells.erase( iter );
            }
            else
            {
                iter++;
            }
        }

        onRightBoundaryCells.sort( compareBoxY );
        rightBoundaryCells.sort( compareBoxY );

        iter = rightBoundaryCells.begin();
        iter2 = onRightBoundaryCells.begin();

        while( (iter2 != onRightBoundaryCells.end()) &&
                ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                  static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
        {
            iter2++;
        }

        MJMesh::Vertex *vMin;
        if(rightBoundaryCells.empty())
        {
            //vMin = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
            if( iter2 != onRightBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) < box->getMax()->getCoord(1) )
            {
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
            }
            else
            {
                Data::Point2D p1 = Data::Point2D(box->getMax()->getCoord(0), box->getMin()->getCoord(1));
                Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMax()->getCoord(1));

                Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                vMin = bestVertex( sub_front, local_front, id_Manager, box_edge, taskId);
            }
        }
        else
        {
            if( iter2 == onRightBoundaryCells.end() )
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
            else
            {
                if ( static_cast<Data::BoxTreeCell *>(*iter )->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) )
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), id_Manager->next(0));
                }
                else
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), id_Manager->next(0));
                }
            }

        }

//std::cout << "Vmin ANTES: " << vMin->getId() << std::endl;
        ///CRIA AS ARESTAS INICIAIS
        if( vMin->getPoint()->getCoord(1) > box->getMin()->getCoord(1) + Data::getTolerance()  )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMin()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

            bool tooClose = false;
            //std::cout << "Vmin: " << vMin->getId() << std::endl;

            //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
            for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
            {
                //if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) < 0.008 )
                if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    //std::cout << "TEMMMMM: " << ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) << std::endl;

                    tooClose = true;

                    sub_front->remove(vMin);

                    //#if USE_OPENMP
                    //#pragma omp critical (front)
                    //#endif //#if USE_OPENMP
                    //{
                    if(local_front->have(vMin))
                        local_front->remove(vMin);
                    //}

                    //vMin = nearestVertex(box, sub_front, element_front, e_ori, vMin);   //ATUALIZA VMIN
                    vMin = nearestCrossVertex(sub_front, vMin);   //ATUALIZA VMIN

                    //CRIA AS ARESTAS QUE ESTÃO PROX DA BORDA
                    Data::FrontElementList adj_list = interceptFront(local_front, e_ori);

                    for( std::list<Data::FrontElement *>::iterator it = adj_list.begin(); it != adj_list.end(); it++ )
                    {
                        if( box->out( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->out( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                        {
                            //std::cout << "AHHHH" << std::endl;
                            sub_front->remove( (*it) );
                            if( vMin->getId() != (*it)->getVertex(0)->getId() )
                                sub_front->remove( (*it)->getVertex(0) );
                            if( vMin->getId() != (*it)->getVertex(1)->getId() )
                                sub_front->remove( (*it)->getVertex(1) );
                        }
                        //else
                        //swappingEdges.push_back( (*it) );
                    }

                    break;
                }
            }

            ///ADICIONADO PARA TRATAR CELULAS ON NO MOVO MODELO
            MJMesh::Vertex *v_teste = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMin()->getCoord(1), 0);
            MJMesh::Vertex *v_base = findElementVertex(sub_front, local_front, v_teste);
            delete v_teste;

            if(!tooClose)
            {
                if(v_base != NULL)
                {
                    //std::cout << "BUSCOU: " << v_base->getId() << " COM " << v_base << std::endl;
                    delete e_ori;
                    e_ori = new MJMesh::FrontElement2D( v_base, vMin, id_Manager->next(1) );
                    //if(this->boundary->have(v_base))
                    common_vertices_list.insert(v_base);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                    includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e_ori, v_base, vMin,taskId);
                }
                else
                    InitialEdge(taskId, box, sub_front, interfaceEdges, local_front, common_vertices_list, id_Manager, vMin, e_ori, false);

                //if(e0 != NULL )
                //    removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"
            }

            if(v_base == NULL)
                delete e_ori;
            delete v;
        }

        //common_vertices_list.insert(vMin);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

        ///CRIA AS ARESTAS INTERNAS
        //while( iter!=rightBoundaryCells.end() && iter2!=onRightBoundaryCells.end() )
        while( iter!=rightBoundaryCells.end() || iter2!=onRightBoundaryCells.end() )
        {
            if( (iter2 != onRightBoundaryCells.end()) &&
                    ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                      static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
            {
                iter2++;
                continue;
            }

            //CÉLULAS DE DENTRO ACABARAM
            if( (iter == rightBoundaryCells.end() && iter2!=onRightBoundaryCells.end() ) || rightBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vMax, taskId);

                vMin = vMax;

                iter2++;

                continue;
            }
            //CÉLULAS DE FORA ACABARAM
            if( (iter2 == onRightBoundaryCells.end() && iter!=rightBoundaryCells.end()) || onRightBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vMax, taskId);

                vMin = vMax;

                iter++;

                continue;
            }

            if( iter == rightBoundaryCells.end() || iter2 == onRightBoundaryCells.end())
            {
                break;
            }

            //CELULAS DOS 2 LADOS
            ///CELULA DO LADO INTERIOR MENOR
            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) + Data::getTolerance() )
            {
                //std::cout << "LADO INTERIOR MENOR" << std::endl;

                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) < static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) - Data::getTolerance() )
                {
                    //std::cout << "sabia int" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);
                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0), vMin->getPoint()->getCoord(1) + L, 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }


                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, 0 );

                            Data::FrontElementSet edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);
                            delete aux_e;

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    //std::cout << "DELETAAAAAAA: "<< std::endl;
                                    Data::FrontElementList adjs0 = static_cast<MJMesh::Vertex *>((*it)->getVertex(0))->getAdjacentFrontElements();
                                    Data::FrontElementList adjs1 = static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->getAdjacentFrontElements();

                                    int size0 = 0;
                                    int size1 = 0;

                                    while (!adjs0.empty())      ///CONTA AS ADJ DO VERTICE 0
                                    {
                                        Data::FrontElement *adj = adjs0.front();

                                        if( sub_front->have(adj) )
                                            size0++;

                                        //std::cout << "adj fel0 " << adj->text() << std::endl;
                                        adjs0.pop_front();
                                    }
                                    while (!adjs1.empty())      ///CONTA AS ADJ DO VERTICE 1
                                    {
                                        Data::FrontElement *adj = adjs1.front();

                                        if( sub_front->have(adj) )
                                            size1++;

                                        //std::cout << "adj fel1 " << adj->text() << std::endl;
                                        adjs1.pop_front();
                                    }
                                    //std::cout << "SIZE 0: " << size0 << "  ID: " << (*it)->getVertex(0)->getId() << std::endl;
                                    //std::cout << "SIZE 1: " << size1 << "  ID: " << (*it)->getVertex(1)->getId() << std::endl;

                                    sub_front->remove( (*it) );
                                    local_front->remove( (*it) );

                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() &&  size0 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(0) ))
                                        {
                                            //std::cout << "Remove vertice 0: " << (*it)->getVertex(0)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(0) );
                                        }
                                        if( local_front->have( (*it)->getVertex(0) ))
                                            local_front->remove( (*it)->getVertex(0) );

                                    }

                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() &&  size1 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(1) ))
                                        {
                                            //std::cout << "Remove vertice 1: " << (*it)->getVertex(1)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(1) );
                                        }
                                        if( local_front->have( (*it)->getVertex(1) ))
                                            local_front->remove( (*it)->getVertex(1) );
                                    }
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }
                            //delete aux_e;


                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);
                            //addAdjCross(sub_front, vMin, v_aux);

                            vMin = vTemp;
                            iter++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        //unusedVertex(sub_front, rejected, vMin);
                        if( !unusedVertex(sub_front, local_front, interfaceEdges, vMin) )
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );

                            MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                            if( v_aux == NULL )
                                v_aux = vTemp;

                            //std::cout << "ID Vauxxx: " << v_aux->getId() << std::endl;

                            //if( includeInModel(box, sub_front, rejected, aux_e, vMin, vTemp) == true )
                            if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vMin, vTemp, taskId) == false )
                            {
                                if( !box->in(vTemp->getPoint()) && !box->in(vMin->getPoint()) )             ///FORA DA BOX - ADD AO SUB_DOMINIO
                                {
                                    aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );
                                    swappingEdges.push_back( aux_e );
                                }
                            }
                            else
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);


                        }
                        else
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                                if(v_aux==NULL)
                                    v_aux = nearestCrossVertex(sub_front, vTemp);

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            e->replace(vMin, NULL);
                            delete vMin;
                        }

                        //std::cout << "ID MINNNNNN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtempppp: " << vTemp->getId() << std::endl;
                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;

                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
                        {
                            iter2++;
                        }

                        iter++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }


                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );


                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vAux, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;

                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
                    iter2++;

                iter++;
                delete vAux;
            }
            else    ///CELULA DO LADO EXTERNO MENOR
            {
                //std::cout << "PEGA A DE FORA" << std::endl;
                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min2D().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(1), 0);
                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) < static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) - Data::getTolerance() )
                {
                    //std::cout << "sabia ext" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);
                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0), vMin->getPoint()->getCoord(1) + L, 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }


                        bool isIn = false;
                        Data::FrontElementSet edgesinternalList;

                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///DOIS PONTOS ESTÃO NA BORDA
                        {
                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);          /// !!!!!!!!!!!!!!!  TIREI POIS NÃO SE UTILIZA !!!!!!!!!!!!!!!!!!!!
                            //addAdjCross(sub_front, vMin, v_aux);



                            ///ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                            if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )
                            {
                                if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                                {
                                    MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                    addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                                }

                                vMin = vTemp;
                                iter2++;

                                delete e;
                                delete vMax;

                                continue;
                            }


                            ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )      /// ? ??
                                {
                                    isIn = true;

                                    if(sub_front->have( (*it) ))
                                        sub_front->remove( (*it) );
                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                        sub_front->remove( (*it)->getVertex(0) );
                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                        sub_front->remove( (*it)->getVertex(1) );
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }

                            delete aux_e;


                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }



                        ///ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                        edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                        {
                            MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                            delete aux;

                            if( e!= NULL )
                                edgesinternalList.insert(e);

                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        }

                        for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                        {
                            if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) || box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                            {
                                isIn = true;

                                if(sub_front->have( (*it) ))
                                    sub_front->remove( (*it) );
                                if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                    sub_front->remove( (*it)->getVertex(0) );
                                if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                    sub_front->remove( (*it)->getVertex(1) );
                            }
                            else
                                swappingEdges.push_back( (*it) );
                        }

                        delete aux_e;


                        if( !isIn && vMin->getId() != vTemp->getId() && edgesinternalList.size() == 0 )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                if(v_aux==NULL)
                                    v_aux = nearestCrossVertex(sub_front, vTemp);

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            if ( !sub_front->have( vTemp ) )
                                vTemp = findOrCreate(taskId, sub_front, local_front, id_Manager, vTemp->getPoint()->getCoord(0), vTemp->getPoint()->getCoord(1), 0 );

                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );
                            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vMin, vTemp, taskId);
                        }

                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;
                        iter2++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }


                delete e;
                delete vMax;


                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );


                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vAux, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                iter2++;
                delete vAux;
            }

        }
//std::cout << "ACABOUUUUUuuuuu: " << vMin->getId() << std::endl;

        ///CRIA AS ARESTAS FINAIS
        if( vMin->getPoint()->getCoord(1) + Data::getTolerance() < box->getMax()->getCoord(1) )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMax()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );


            MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, local_front, e_ori, vMin, taskId);
            if(vFinal == NULL)
                vFinal = findOrCreate(taskId, sub_front, local_front, id_Manager, box->getMax()->getCoord(0), box->getMax()->getCoord(1));

            MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, id_Manager->next(1) );

            //removeDanglingEdges(box, sub_front, e);

            //std::cout << "ARESTA FINAL :::: " << vMin->getId() << " -- " << vFinal->getId() << std::endl;

            //includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin);
            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vFinal, taskId);

            //if(this->boundary->have(vFinal))
            //common_vertices_list.insert(vFinal);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

            delete e_ori;
            delete v;

            ///TESTE PARA FECHAR A FRONTEIRA FORA DA BOX
            if( vFinal->getAdjacentFrontElements().size() == 1 )
            {
                MJMesh::Vertex *vTemp = nearestCrossVertex(sub_front, vFinal);

                if( vTemp != NULL && vTemp->getPoint()->getCoord(1) > vFinal->getPoint()->getCoord(1) && box->out( vTemp->getPoint() ) )
                {
                    vMin = vFinal;
                    vFinal = vTemp;

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, id_Manager->next(1) );

                    //std::cout << "ARESTA FINAL EXTRA :::: " << vMin->getId() << " -- " << vFinal->getId() << std::endl;

                    existEdgeInGlobal(e, local_front);

                    interfaceEdges.push_back( e );

                    if(!sub_front->have(e))
                    {
                        sub_front->add(e);
                    }

                    //if(this->boundary->have(vFinal))
                    //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                }
            }

            common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
        }

    }
    break;

    ///TOP
    case 1:
    {
        //std::cout << "TOP" << std::endl;
        //if( topBoundaryCells.empty() ) continue;
        if( topBoundaryCells.empty() ) break;

        for (iter = topBoundaryCells.begin(); iter != topBoundaryCells.end(); )
        {
            if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
            {
                onTopBoundaryCells.push_back( (*iter) );

                iter = topBoundaryCells.erase( iter );
            }
            else
            {
                iter++;
            }

        }

        onTopBoundaryCells.sort( compareBoxX );
        topBoundaryCells.sort( compareBoxX );

        iter = topBoundaryCells.begin();
        iter2 = onTopBoundaryCells.begin();

        while( (iter2 != onTopBoundaryCells.end()) &&
                ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                  static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
        {
            iter2++;
        }

        MJMesh::Vertex *vMin;
        if ( topBoundaryCells.empty() )
        {

            //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
            if( iter2 != onTopBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) < box->getMax()->getCoord(0) )
            {
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
            }
            else
            {

                Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMax()->getCoord(1));
                Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMax()->getCoord(1));

                Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                vMin = bestVertex( sub_front, local_front, id_Manager, box_edge, taskId);
            }
        }
        else
        {

            if( iter2 == onTopBoundaryCells.end() )
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
            else
            {
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) )
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), id_Manager->next(0));
                }
                else
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), id_Manager->next(0));
                }
            }
        }

        //std::cout << "Vmin inicial " << vMin->getId() << std::endl;

        ///CRIA AS ARESTAS INICIAIS
        if( vMin->getPoint()->getCoord(0) >= box->getMin()->getCoord(0) + Data::getTolerance()  )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMax()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );


            bool tooClose = false;

            //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
            for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
            {

                //if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) < 0.008 )
                if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    //std::cout << "TEMMMMM " << vMin->getId() << " em " << (*it)->getId() << std::endl;
                    tooClose = true;

                    sub_front->remove(vMin);

#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMin))
                            local_front->remove(vMin);
                    }

                    //vMin = nearestVertex(box, sub_front, element_front, e_ori, vMin);   //ATUALIZA VMIN
                    vMin = nearestCrossVertex(sub_front, vMin);   //ATUALIZA VMIN

                    //CRIA AS ARESTAS QUE ESTÃO PROX DA BORDA
                    Data::FrontElementList adj_list = interceptFront(local_front, e_ori);

                    for( std::list<Data::FrontElement *>::iterator it = adj_list.begin(); it != adj_list.end(); it++ )
                    {
                        if( box->out( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->out( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                        {
                            sub_front->remove( (*it) );
                            if( vMin->getId() != (*it)->getVertex(0)->getId() )
                                sub_front->remove( (*it)->getVertex(0) );
                            if( vMin->getId() != (*it)->getVertex(1)->getId() )
                                sub_front->remove( (*it)->getVertex(1) );
                        }
                        //else
                        //swappingEdges.push_back( (*it) );
                    }

                    break;
                }
            }

            ///ADICIONADO PARA TRATAR CELULAS ON NO MOVO MODELO
            MJMesh::Vertex *v_teste = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMax()->getCoord(1), 0);
            MJMesh::Vertex *v_base = findElementVertex(sub_front, local_front, v_teste);
            delete v_teste;

            if(!tooClose)
            {
//std::cout << "Prox Teste falhou" << std::endl;
                if(v_base != NULL)
                {
//std::cout << "vBase " << v_base->getId() << std::endl;
                    delete e_ori;
                    e_ori = new MJMesh::FrontElement2D( vMin, v_base, id_Manager->next(1) );
                    //if(this->boundary->have(v_base))
                    common_vertices_list.insert(v_base);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                    includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e_ori, vMin, v_base, taskId);
                }
                else
                    InitialEdge(taskId, box, sub_front, interfaceEdges, local_front, common_vertices_list, id_Manager, vMin, e_ori, true);

                //if(e0 != NULL )
                //removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"

            }

            if(v_base == NULL)
                delete e_ori;
            delete v;
        }

        //common_vertices_list.insert(vMin);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

        ///CRIA AS ARESTAS INTERNAS
        //while( iter!=topBoundaryCells.end() && iter2!=onTopBoundaryCells.end())
        while( iter!=topBoundaryCells.end() || iter2!=onTopBoundaryCells.end())
        {
            if( ( (iter2 != onTopBoundaryCells.end()) ) &&
                    ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                      static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) )  )
            {
                iter2++;
                continue;
            }

            if( (iter == topBoundaryCells.end() && iter2!=onTopBoundaryCells.end()) || topBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMax, vMin, taskId);

                vMin = vMax;

                iter2++;

                continue;
            }

            if( (iter2 == onTopBoundaryCells.end() && iter!=topBoundaryCells.end()) || onTopBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMax, vMin, taskId);

                vMin = vMax;

                iter++;

                continue;
            }

            if( iter == topBoundaryCells.end() && iter2 == onTopBoundaryCells.end())
                break;

            ///CELULAS IGUAIS  FICA COM A INTERNA
            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) + Data::getTolerance() )
            {
                //std::cout << "Pega IN" << std::endl;

                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) < static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
                {
                    //std::cout << "sabia int" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0) + L, vMin->getPoint()->getCoord(1), 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "vMin: " << vMin->getId() << std::endl;
                        //std::cout << "vMax: " << vMax->getId() << std::endl;
                        //std::cout << "vTemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }



                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );

                            std::set<Data::FrontElement *> edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);
                            delete aux_e;

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                //std::cout << "-------------------------EDGE: " << (*it)->getId() << std::endl;
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    //std::cout << "DELETAAAAAAA: "<< std::endl;
                                    Data::FrontElementList adjs0 = static_cast<MJMesh::Vertex *>((*it)->getVertex(0))->getAdjacentFrontElements();
                                    Data::FrontElementList adjs1 = static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->getAdjacentFrontElements();

                                    int size0 = 0;
                                    int size1 = 0;

                                    while (!adjs0.empty())      ///CONTA AS ADJ DO VERTICE 0
                                    {
                                        Data::FrontElement *adj = adjs0.front();

                                        if( sub_front->have(adj) )
                                            size0++;

                                        //std::cout << "adj fel0 " << adj->text() << std::endl;
                                        adjs0.pop_front();
                                    }
                                    while (!adjs1.empty())      ///CONTA AS ADJ DO VERTICE 1
                                    {
                                        Data::FrontElement *adj = adjs1.front();

                                        if( sub_front->have(adj) )
                                            size1++;

                                        //std::cout << "adj fel1 " << adj->text() << std::endl;
                                        adjs1.pop_front();
                                    }
                                    //std::cout << "SIZE 0: " << size0 << "  ID: " << (*it)->getVertex(0)->getId() << std::endl;
                                    //std::cout << "SIZE 1: " << size1 << "  ID: " << (*it)->getVertex(1)->getId() << std::endl;

                                    if( sub_front->have( (*it) ) )
                                        sub_front->remove( (*it) );
                                    //this->front->remove( (*it) );

                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() &&  size0 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(0) ))
                                        {
                                            //std::cout << "Remove vertice 0: " << (*it)->getVertex(0)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(0) );
                                        }
                                        //if( this->front->have( (*it)->getVertex(0) ))
                                        //this->front->remove( (*it)->getVertex(0) );

                                    }

                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() &&  size1 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(1) ))
                                        {
                                            //std::cout << "Remove vertice 1: " << (*it)->getVertex(1)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(1) );
                                        }
                                        //if( this->front->have( (*it)->getVertex(1) ))
                                        //this->front->remove( (*it)->getVertex(1) );
                                    }
                                }
                                else
                                {
                                    //std::cout << "INSEREEEE: " << std::endl;

                                    swappingEdges.push_back( (*it) );
                                }
                            }
                            //delete aux_e;

                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);
                            //std::cout << "Min: " << vMin->getId() << std::endl;
                            //std::cout << "Aux: " << v_aux->getId() << std::endl;
                            //std::cout << "Temp: " << vTemp->getId() << std::endl;

                            //addAdjCross(sub_front, vMin, vTemp);

                            //if(para)
                            //break;


                            vMin = vTemp;
                            iter++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        //unusedVertex(sub_front, rejected, vMin);
                        if( !unusedVertex(sub_front, local_front, interfaceEdges, vMin) )
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, id_Manager->next(1) );

                            MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                            if(v_aux==NULL)
                            {
                                v_aux = nearestCrossVertex(sub_front, vTemp);
                            }

                            //if( includeInModel(box, sub_front, rejected, aux_e, vTemp, vMin) == true )
                            if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vTemp, vMin, taskId) == false )
                            {
                                if( !box->in(vTemp->getPoint()) && !box->in(vMin->getPoint()) )             ///FORA DA BOX - ADD AO SUB_DOMINIO
                                {
                                    aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                    existEdgeInGlobal(aux_e, local_front);
                                    //std::cout << "ARESTA ID : " << aux_e->getId() << std::endl;
                                    swappingEdges.push_back( aux_e );
                                }
                            }
                            else
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                        }
                        else
                        {
                            //std::cout << "NÃO USADO" << std::endl;
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                                if(v_aux==NULL)
                                    v_aux = vTemp;


                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            e->replace(vMin, NULL);
                            delete vMin;
                        }

                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;

                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
                        {
                            iter2++;
                        }

                        iter++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }

                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);


                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vAux, vMin, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;

                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, vAux->getPoint()->getCoord(0), vAux->getPoint()->getCoord(1));
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
                    iter2++;

                iter++;
                delete vAux;

            }
            else
            {
                //CELULA DO LADO EXTERNO MENOR
                //std::cout << "Pega EXT" << std::endl;

                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, 0 );

                changeVertex(sub_front, vMin, vMax);


                if( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) + Data::getTolerance() < static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0))
                {
                    //std::cout << "sabia ext" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );


                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0) + L, vMin->getPoint()->getCoord(1), 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "vMin: " << vMin->getId() << std::endl;
                        //std::cout << "vMax: " << vMax->getId() << std::endl;
                        //std::cout << "vTemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }


                        bool isIn = false;
                        Data::FrontElementSet edgesinternalList;

                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///DOIS VERTICES NA BORDA
                        {
                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);          /// !!!!!!!!!!!!!!!  TIREI POIS NÃO SE UTILIZA !!!!!!!!!!!!!!!!!!!!
                            //addAdjCross(sub_front, vMin, v_aux);


                            ///ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                            if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )
                            {
                                if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                                {
                                    //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                    MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                    if(v_aux==NULL)
                                        v_aux = nearestCrossVertex(sub_front, vTemp);

                                    addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                                }

                                vMin = vTemp;
                                iter2++;

                                delete e;
                                delete vMax;

                                continue;
                            }


                            ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )      /// ? ??
                                {
                                    isIn = true;

                                    if(sub_front->have( (*it) ))
                                        sub_front->remove( (*it) );
                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                        sub_front->remove( (*it)->getVertex(0) );
                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                        sub_front->remove( (*it)->getVertex(1) );
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }

                            delete aux_e;



                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }



                        ///ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                if(v_aux!=NULL)
                                    addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                        edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                        {
                            MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                            delete aux;

                            if( e!= NULL )
                                edgesinternalList.insert(e);

                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        }

                        for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                        {
                            if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )      /// ? ??
                            {
                                isIn = true;

                                if(sub_front->have( (*it) ))
                                    sub_front->remove( (*it) );
                                if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                    sub_front->remove( (*it)->getVertex(0) );
                                if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                    sub_front->remove( (*it)->getVertex(1) );
                            }
                            else
                                swappingEdges.push_back( (*it) );
                        }

                        delete aux_e;


                        if( !isIn && vMin->getId() != vTemp->getId() && edgesinternalList.size() == 0 )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                if(v_aux!=NULL)
                                    addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            if ( !sub_front->have( vTemp ) )
                                vTemp = findOrCreate(taskId, sub_front, local_front, id_Manager, vTemp->getPoint()->getCoord(0), vTemp->getPoint()->getCoord(1), 0 );

                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, id_Manager->next(1) );
                            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vTemp, vMin, taskId);
                        }

                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;
                        iter2++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }

                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vAux, vMin, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                iter2++;
                delete vAux;
            }
        }
//std::cout << "vMIN é:::: " << vMin->getId() << std::endl;
        ///CRIA AS ARESTAS FINAIS
        if( vMin->getPoint()->getCoord(0) + Data::getTolerance() < box->getMax()->getCoord(0) )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMax()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

            MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, local_front, e_ori, vMin, taskId);
            if(vFinal == NULL)
                vFinal = findOrCreate(taskId, sub_front, local_front, id_Manager, box->getMax()->getCoord(0), box->getMax()->getCoord(1));


            MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal,vMin, id_Manager->next(1) );

            //removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

            //std::cout << "ARESTA FINAL :::: " << vFinal->getId() << " -- " << vMin->getId() << std::endl;

            //includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);
            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vFinal, vMin, taskId);

            //if(this->boundary->have(vFinal))
            //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

            delete e_ori;
            delete v;

            ///TESTE PARA FECHAR A FRONTEIRA FORA DA BOX
            if( vFinal->getAdjacentFrontElements().size() == 1 )
            {
                MJMesh::Vertex *vTemp = nearestCrossVertex(sub_front, vFinal);

                if( vTemp != NULL && vTemp->getPoint()->getCoord(0) > vFinal->getPoint()->getCoord(0) && box->out( vTemp->getPoint() ) )
                {
                    vMin = vFinal;
                    vFinal = vTemp;

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal, vMin, id_Manager->next(1) );

                    //std::cout << "ARESTA FINAL EXTRA :::: " << vFinal->getId() << " -- " << vMin->getId() << std::endl;

                    existEdgeInGlobal(e, local_front);

                    interfaceEdges.push_back( e );

                    if(!sub_front->have(e))
                    {
                        sub_front->add(e);
                    }

                    //if(this->boundary->have(vFinal))
                    //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                }
            }

            common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

        }

    }
    break;

    ///ESQUERDA
    case 2:
    {
        //std::cout << "ESQUERDA" << std::endl;
        //if( leftBoundaryCells.size() == 0 ) continue;
        if( leftBoundaryCells.size() == 0 ) break;

        for (iter = leftBoundaryCells.begin(); iter != leftBoundaryCells.end(); )
        {
            if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
            {
                onLeftBoundaryCells.push_back( (*iter) );

                iter = leftBoundaryCells.erase( iter );
            }
            else
            {
                iter++;
            }
        }

        onLeftBoundaryCells.sort( compareBoxY );
        leftBoundaryCells.sort( compareBoxY );

        iter = leftBoundaryCells.begin();
        iter2 = onLeftBoundaryCells.begin();

        while( (iter2 != onLeftBoundaryCells.end()) &&
                ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                  static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
        {
            iter2++;
        }

        MJMesh::Vertex *vMin;

        if(leftBoundaryCells.empty())
        {
            //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
            if( iter2 != onLeftBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) < box->getMax()->getCoord(1) )
            {
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
            }
            else
            {
                Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMin()->getCoord(1));
                Data::Point2D p2 = Data::Point2D(box->getMin()->getCoord(0), box->getMax()->getCoord(1));

                Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                vMin = bestVertex( sub_front, local_front, id_Manager, box_edge, taskId);
            }
        }
        else
        {
            if( iter2 == onLeftBoundaryCells.end() )
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
            else
            {
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) )
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), id_Manager->next(0));
                }
                else
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), id_Manager->next(0));
                }
            }

        }


        ///CRIA AS ARESTAS INICIAIS
        if( vMin->getPoint()->getCoord(1) > box->getMin()->getCoord(1) + Data::getTolerance()  )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D * e_ori= new MJMesh::FrontElement2D( v, vMin );


            bool tooClose = false;
            //std::cout << "Vmin: " << vMin->getId() << std::endl;

            //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
            for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
            {
                //if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) < 0.008 )
                if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    //std::cout << "TEMMMMM: " << ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) << std::endl;
                    //std::cout << "TEMMMMM: " << Data::getTolerance()*700.0 << std::endl;

                    tooClose = true;

                    sub_front->remove(vMin);

#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMin))
                            local_front->remove(vMin);
                    }

                    //vMin = nearestVertex(box, sub_front, element_front, e_ori, vMin);   //ATUALIZA VMIN
                    vMin = nearestCrossVertex(sub_front, vMin);   //ATUALIZA VMIN

                    //CRIA AS ARESTAS QUE ESTÃO PROX DA BORDA
                    Data::FrontElementList adj_list = interceptFront(local_front, e_ori);

                    for( std::list<Data::FrontElement *>::iterator it = adj_list.begin(); it != adj_list.end(); it++ )
                    {
                        if( box->out( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->out( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                        {
                            //std::cout << "AHHHH" << std::endl;
                            sub_front->remove( (*it) );
                            if( vMin->getId() != (*it)->getVertex(0)->getId() )
                                sub_front->remove( (*it)->getVertex(0) );
                            if( vMin->getId() != (*it)->getVertex(1)->getId() )
                                sub_front->remove( (*it)->getVertex(1) );
                        }
                        //else
                        //swappingEdges.push_back( (*it) );
                    }

                    break;
                }
            }

            ///ADICIONADO PARA TRATAR CELULAS ON NO MOVO MODELO
            MJMesh::Vertex *v_teste = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), 0);
            MJMesh::Vertex *v_base = findElementVertex(sub_front, local_front, v_teste);
            delete v_teste;

            if(!tooClose)
            {
                //std::cout << "VAI BUSCAR" << std::endl;

                if(v_base != NULL)
                {
                    //std::cout << "BUSCOU: " << v_base->getId() << " COM " << v_base << std::endl;
                    delete e_ori;
                    e_ori = new MJMesh::FrontElement2D( vMin, v_base, id_Manager->next(1) );
                    //if(this->boundary->have(v_base))
                    common_vertices_list.insert(v_base);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                    includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e_ori, vMin, v_base,taskId);
                }
                else
                    InitialEdge(taskId, box, sub_front, interfaceEdges, local_front, common_vertices_list, id_Manager, vMin, e_ori, true);

                //if(e0 != NULL )
                //removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"
            }

            if(v_base == NULL)
                delete e_ori;
            delete v;
        }

        //common_vertices_list.insert(vMin);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

        ///CRIA AS ARESTAS INTERNAS
        //while( iter!=leftBoundaryCells.end() && iter2!=onLeftBoundaryCells.end() )
        while( iter!=leftBoundaryCells.end() || iter2!=onLeftBoundaryCells.end() )
        {

            if( (iter2 != onLeftBoundaryCells.end()) &&
                    ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                      static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
            {
                iter2++;
                continue;
            }

            if( (iter == leftBoundaryCells.end() && iter2!=onLeftBoundaryCells.end()) || leftBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vMax, taskId);

                vMin = vMax;

                iter2++;

                continue;
            }

            if( (iter2 == onLeftBoundaryCells.end() && iter!=leftBoundaryCells.end()) || onLeftBoundaryCells.empty()  )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMax, vMin, taskId);

                vMin = vMax;

                iter++;

                continue;
            }

            if( iter == leftBoundaryCells.end() || iter2 == onLeftBoundaryCells.end())
                break;

            //CELULAS IGUAIS
            //CELULA DO LADO INTERIOR MENOR
            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
            {
                //std::cout << "PEGA A DE DENTRO" << std::endl;

                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min2D().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) < static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) - Data::getTolerance() )
                {
                    //std::cout << "sabia int" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0), vMin->getPoint()->getCoord(1) + L, 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }

                        bool isIn = false;
                        Data::FrontElementSet edgesinternalList;

                        //ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )     ///VERTICES NA BORDA
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) || box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    isIn = true;

                                    if(sub_front->have( (*it) ))
                                        sub_front->remove( (*it) );
                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                        sub_front->remove( (*it)->getVertex(0) );
                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                        sub_front->remove( (*it)->getVertex(1) );
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }

                            delete aux_e;




                            vMin = vTemp;

                            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
                            {
                                iter2++;
                            }

                            iter++;

                            delete e;
                            delete vMax;

                            continue;
                        }

                        ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                        edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                        {
                            MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                            MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                            delete aux;

                            if(e != NULL)
                                edgesinternalList.insert(e);

                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        }

                        for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                        {
                            if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )      /// ? ??
                            {
                                isIn = true;

                                if(sub_front->have( (*it) ))
                                    sub_front->remove( (*it) );
                                if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                    sub_front->remove( (*it)->getVertex(0) );
                                if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                    sub_front->remove( (*it)->getVertex(1) );
                            }
                            else
                                swappingEdges.push_back( (*it) );
                        }

                        delete aux_e;

                        if( !isIn && vMin->getId() != vTemp->getId() && edgesinternalList.size() == 0 )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                if(v_aux==NULL)
                                    v_aux = nearestCrossVertex(sub_front, vTemp);

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            if ( !sub_front->have( vTemp ) )
                                vTemp = findOrCreate(taskId, sub_front, local_front, id_Manager, vTemp->getPoint()->getCoord(0), vTemp->getPoint()->getCoord(1), 0 );

                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, id_Manager->next(1) );
                            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vTemp, vMin, taskId);
                        }


                        vMin = vTemp;

                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
                        {
                            iter2++;
                        }

                        iter++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }

                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vAux, vMin, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;


                if(static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
                    iter2++;


                iter++;
                delete vAux;
                continue;
            }

            else    //CELULA DO LADO EXTERNO MENOR
            {
                //std::cout << "PEGA A DE FORA" << std::endl;

                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) < static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) - Data::getTolerance() )
                {
                    //std::cout << "sabia ext" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0), vMin->getPoint()->getCoord(1) + L, 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }


                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, 0 );

                            Data::FrontElementSet edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);
                            delete aux_e;

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vTemp, vMin, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    //std::cout << "DELETAAAAAAA: "<< std::endl;
                                    Data::FrontElementList adjs0 = static_cast<MJMesh::Vertex *>((*it)->getVertex(0))->getAdjacentFrontElements();
                                    Data::FrontElementList adjs1 = static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->getAdjacentFrontElements();

                                    int size0 = 0;
                                    int size1 = 0;

                                    while (!adjs0.empty())      ///CONTA AS ADJ DO VERTICE 0
                                    {
                                        Data::FrontElement *adj = adjs0.front();

                                        if( sub_front->have(adj) )
                                            size0++;

                                        //std::cout << "adj fel0 " << adj->text() << std::endl;
                                        adjs0.pop_front();
                                    }
                                    while (!adjs1.empty())      ///CONTA AS ADJ DO VERTICE 1
                                    {
                                        Data::FrontElement *adj = adjs1.front();

                                        if( sub_front->have(adj) )
                                            size1++;

                                        //std::cout << "adj fel1 " << adj->text() << std::endl;
                                        adjs1.pop_front();
                                    }
                                    //std::cout << "SIZE 0: " << size0 << "  ID: " << (*it)->getVertex(0)->getId() << std::endl;
                                    //std::cout << "SIZE 1: " << size1 << "  ID: " << (*it)->getVertex(1)->getId() << std::endl;

                                    sub_front->remove( (*it) );
                                    local_front->remove( (*it) );

                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() &&  size0 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(0) ))
                                        {
                                            //std::cout << "Remove vertice 0: " << (*it)->getVertex(0)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(0) );
                                        }
                                        if( local_front->have( (*it)->getVertex(0) ))
                                            local_front->remove( (*it)->getVertex(0) );

                                    }

                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() &&  size1 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(1) ))
                                        {
                                            //std::cout << "Remove vertice 1: " << (*it)->getVertex(1)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(1) );
                                        }
                                        if( local_front->have( (*it)->getVertex(1) ))
                                            local_front->remove( (*it)->getVertex(1) );
                                    }
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }
                            //delete aux_e;


                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);
                            //addAdjCross(sub_front, vMin, v_aux);

                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        //unusedVertex(sub_front, rejected, vMin);
                        if( !unusedVertex(sub_front, local_front, interfaceEdges, vMin) )
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vTemp, vMin, id_Manager->next(1) );

                            MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                            if(v_aux==NULL)
                                v_aux = nearestCrossVertex(sub_front, vTemp);


                            //if( includeInModel(box, sub_front, rejected, aux_e, vTemp, vMin) == true )
                            if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vTemp, vMin, taskId) == false )
                            {
                                if( !box->in(vTemp->getPoint()) && !box->in(vMin->getPoint()) )             ///FORA DA BOX - ADD AO SUB_DOMINIO
                                {
                                    aux_e = new MJMesh::FrontElement2D( vTemp, vMin, id_Manager->next(1) );
                                    swappingEdges.push_back( aux_e );
                                }
                            }
                            else
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                        }
                        else
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                                if(v_aux==NULL)
                                    v_aux = vTemp;

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            e->replace(vMin, NULL);
                            delete vMin;
                        }

                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;
                        iter2++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }

                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMax, vMin, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vAux, vMin, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                iter2++;
                delete vAux;
                continue;
            }

        }


        ///CRIA AS ARESTAS FINAIS
        if( vMin->getPoint()->getCoord(1) + Data::getTolerance() < box->getMax()->getCoord(1) )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMax()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

            MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, local_front, e_ori, vMin, taskId);

            if(vFinal == NULL)
                vFinal = findOrCreate(taskId, sub_front, local_front, id_Manager, box->getMin()->getCoord(0), box->getMax()->getCoord(1));

            MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal, vMin, id_Manager->next(1) );

            //removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

            //std::cout << "ARESTA FINAL :::: " << vFinal->getId()  << " -- " << vMin->getId() << std::endl;

            //includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);
            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vFinal, vMin, taskId);

            //if(this->boundary->have(vFinal))
            //common_vertices_list.insert(vFinal);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

            delete e_ori;
            delete v;

            ///TESTE PARA FECHAR A FRONTEIRA FORA DA BOX
            if( vFinal->getAdjacentFrontElements().size() == 1 )
            {
                MJMesh::Vertex *vTemp = nearestCrossVertex(sub_front, vFinal);

                if( vTemp != NULL && vTemp->getPoint()->getCoord(1) > vFinal->getPoint()->getCoord(1) && box->out( vTemp->getPoint() ) )
                {
                    vMin = vFinal;
                    vFinal = vTemp;

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal, vMin, id_Manager->next(1) );

                    //std::cout << "ARESTA FINAL EXTRA :::: " << vFinal->getId() << " -- " << vMin->getId() << std::endl;

                    existEdgeInGlobal(e, local_front);

                    interfaceEdges.push_back( e );

                    if(!sub_front->have(e))
                    {
                        sub_front->add(e);
                    }

                    //      if(this->boundary->have(vFinal))
                    //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                }
            }

            common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
        }

    }
    break;

    ///BASE
    case 3:
    {
        //std::cout << "BASE" << std::endl;
        //if( baseBoundaryCells.empty() ) continue;
        if( baseBoundaryCells.empty() ) break;

        for (iter = baseBoundaryCells.begin(); iter != baseBoundaryCells.end(); )
        {
            if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
            {
                onBaseBoundaryCells.push_back( (*iter) );

                iter = baseBoundaryCells.erase( iter );
            }
            else
            {
                iter++;
            }
        }

        onBaseBoundaryCells.sort( compareBoxX );
        baseBoundaryCells.sort( compareBoxX );

        iter = baseBoundaryCells.begin();
        iter2 = onBaseBoundaryCells.begin();

        while( ( (iter2 != onBaseBoundaryCells.end()) ) &&
                ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                  static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
        {
            iter2++;
        }

        MJMesh::Vertex *vMin = NULL;
        if ( baseBoundaryCells.empty() )
        {
            //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
            if( iter2 != onBaseBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) < box->getMax()->getCoord(0) )
            {
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
            }
            else
            {
                Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMin()->getCoord(1));
                Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMin()->getCoord(1));

                Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                vMin = bestVertex( sub_front, local_front, id_Manager, box_edge, taskId);
            }
        }
        else
        {
            if( iter2 == onBaseBoundaryCells.end() )
                vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
            else
            {
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) )
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), id_Manager->next(0));
                }
                else
                {
                    vMin = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
                    //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max().getCoord(1), id_Manager->next(0));
                }
            }

        }

        ///CRIA AS ARESTAS INICIAIS
        if( vMin->getPoint()->getCoord(0) > box->getMin()->getCoord(0) + Data::getTolerance()  )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

            bool tooClose = false;

            //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
            for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
            {
                //if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) < 0.008 )
                if( ((MJMesh::FrontElement2D*)(*it))->distance( vMin->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    //std::cout << "TEMMMMM" << std::endl;
                    tooClose = true;

                    sub_front->remove(vMin);

#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMin))
                            local_front->remove(vMin);
                    }

                    //vMin = nearestVertex(box, sub_front, element_front, e_ori, vMin);   //ATUALIZA VMIN
                    vMin = nearestCrossVertex(sub_front, vMin);   //ATUALIZA VMIN

                    //CRIA AS ARESTAS QUE ESTÃO PROX DA BORDA
                    Data::FrontElementList adj_list = interceptFront(local_front, e_ori);
                    //Data::FrontElementList els = vMin->getAdjacentFrontElements();
                    for( std::list<Data::FrontElement *>::iterator it = adj_list.begin(); it != adj_list.end(); it++ )
                    {
                        //bool isIn = false;
                        if( box->out( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->out( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                        {
                            //std::cout << "AHHHH" << std::endl;
                            sub_front->remove( (*it) );
                            if( vMin->getId() != (*it)->getVertex(0)->getId() )
                                sub_front->remove( (*it)->getVertex(0) );
                            if( vMin->getId() != (*it)->getVertex(1)->getId() )
                                sub_front->remove( (*it)->getVertex(1) );
                        }
                        //else
                        //swappingEdges.push_back( (*it) );
                        //NÂO CRIA ARESTAS INICIAS E DA PROBLEMA DO CLEANEDGES
                    }
                    break;
                }
            }

            ///ADICIONADO PARA TRATAR CELULAS ON NO MOVO MODELO
            MJMesh::Vertex *v_teste = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), 0);
            MJMesh::Vertex *v_base = findElementVertex(sub_front, local_front, v_teste);
            delete v_teste;

            if(!tooClose)
            {
                if(v_base != NULL)
                {
                    delete e_ori;
                    e_ori = new MJMesh::FrontElement2D( v_base, vMin, id_Manager->next(1) );
                    //if(this->boundary->have(v_base))
                    common_vertices_list.insert(v_base);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                    includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e_ori, v_base, vMin, taskId);

                }
                else
                    InitialEdge(taskId, box, sub_front, interfaceEdges, local_front, common_vertices_list, id_Manager, vMin, e_ori, false);

                //if(e0 != NULL )
                //removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"
            }

            if(v_base == NULL)
                delete e_ori;
            delete v;
        }

        //common_vertices_list.insert(vMin);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

        ///CRIA AS ARESTAS INTERNAS
        //while( iter!=baseBoundaryCells.end() && iter2!=onBaseBoundaryCells.end() )
        while( iter!=baseBoundaryCells.end() || iter2!=onBaseBoundaryCells.end() )
        {
            if( ( (iter2 != onBaseBoundaryCells.end()) ) &&
                    ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                      static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
            {
                iter2++;
                continue;
            }

            if( (iter == baseBoundaryCells.end() && iter2!=onBaseBoundaryCells.end()) || baseBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vMax, taskId);

                vMin = vMax;

                iter2++;

                continue;
            }

            if( (iter2 == onBaseBoundaryCells.end() && iter!=baseBoundaryCells.end()) || onBaseBoundaryCells.empty() )
            {
                MJMesh::Vertex *vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vMax, taskId);

                vMin = vMax;

                iter++;

                continue;
            }

            if( iter == baseBoundaryCells.end()  && iter2 == onBaseBoundaryCells.end() )
                break;

            ///CELULAS IGUAIS FICA COM A INTERNA
            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
            {
                //std::cout << "LADO INTERIOR MENOR" << std::endl;
                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) < static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) - Data::getTolerance() )
                {
                    //std::cout << "sabia int" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0) + L, vMin->getPoint()->getCoord(1), 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }


                        bool isIn = false;
                        Data::FrontElementSet edgesinternalList;

                        //ARESTA COLADA NA BORDA LIGANDO PRA FORA - NÂO REMOVER
                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && box->out( vTemp->getPoint() ) && box->in( vMin->getPoint() ) )     ///VERTICES NA BORDA
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }


                            ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                            edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                            /*if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }*/

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) || box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    //std::cout << "DELETAAAAAAA: "<< std::endl;
                                    isIn = true;

                                    if(sub_front->have( (*it) ))
                                        sub_front->remove( (*it) );
                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                        sub_front->remove( (*it)->getVertex(0) );
                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                        sub_front->remove( (*it)->getVertex(1) );
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }

                            delete aux_e;




                            vMin = vTemp;

                            if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
                            {
                                iter2++;
                            }

                            iter++;

                            delete e;
                            delete vMax;

                            continue;
                        }


                        ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                        edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);

                        if( this->boundary->have(vTemp) && this->boundary->have(vMin) && edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                        {
                            MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                            MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                            delete aux;

                            if( e!= NULL )
                                edgesinternalList.insert(e);

                            if(this->boundary->have(vTemp))
                                common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        }

                        for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                        {
                            if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )  /// ? ??
                            {
                                //std::cout << "DELETAAAAAAA: "<< std::endl;
                                isIn = true;

                                if(sub_front->have( (*it) ))
                                    sub_front->remove( (*it) );
                                if( vTemp->getId() != (*it)->getVertex(0)->getId() && sub_front->have( (*it)->getVertex(0) ))
                                    sub_front->remove( (*it)->getVertex(0) );
                                if( vTemp->getId() != (*it)->getVertex(1)->getId() && sub_front->have( (*it)->getVertex(1) ))
                                    sub_front->remove( (*it)->getVertex(1) );
                            }
                            else
                                swappingEdges.push_back( (*it) );
                        }

                        delete aux_e;


                        if( !isIn && vMin->getId() != vTemp->getId() && edgesinternalList.size() == 0 )
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vTemp);
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp); ///TESTANDO

                                if(v_aux==NULL)
                                    v_aux = nearestCrossVertex(sub_front, vTemp);

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            if ( !sub_front->have( vTemp ) )
                                vTemp = findOrCreate(taskId, sub_front, local_front, id_Manager, vTemp->getPoint()->getCoord(0), vTemp->getPoint()->getCoord(1), 0 );

                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );
                            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vMin, vTemp, taskId);
                        }


                        vMin = vTemp;
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
                        {
                            iter2++;
                        }

                        iter++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }

                delete e;
                delete vMax;


                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1), 0);

                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vAux, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));

                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;

                if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
                    iter2++;


                iter++;
                delete vAux;
            }
            else
            {
                //std::cout << "LADO EXTERIOR MENOR" << std::endl;
                //CELULA DO LADO EXTERNO MENOR
                MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max2D().getCoord(1), 0);

                MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, 0 );

                changeVertex(sub_front, vMin, vMax);

                if( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) < static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) + Data::getTolerance() )
                {
                    //std::cout << "sabia ext" << std::endl;

                    //double L = static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1);
                    double L = static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) - static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1);

                    MJMesh::Vertex *vTemp = nearestVertex(local_front, vMax);

                    double dist = vTemp->getPoint()->distance( vMin->getPoint() );

                    MJMesh::Vertex *vMinMax = new MJMesh::Vertex( vMin->getPoint()->getCoord(0) + L, vMin->getPoint()->getCoord(1), 0);
                    double dist2 = vTemp->getPoint()->distance( vMinMax->getPoint() );
                    delete vMinMax;

                    if( dist < L + Data::getTolerance() || dist2 < L + Data::getTolerance() )
                    {
                        //std::cout << "MENORRRRRRRRRRRRRRRRRrrrr: " << dist << std::endl;
                        //std::cout << "ID MIN: " << vMin->getId() << std::endl;
                        //std::cout << "ID Vtemp: " << vTemp->getId() << std::endl;
                        //std::cout << "ID current: " << id_Manager->current() << std::endl;

                        ///PROBLEMA COM FRATURA
                        if( !sub_front->have(vTemp) )
                        {
                            sub_front->add( vTemp );
                        }

                        if( this->boundary->have(vMin) && this->boundary->have(vTemp) )     ///ADD/REMOVE TODAS ARESTAS DA FORNTEIRA ENTRE A ARESTA DE VERIFICAÇÂO
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, 0 );

                            Data::FrontElementSet edgesinternalList = edgesInternal(aux_e, local_front, common_vertices_list);
                            delete aux_e;

                            if( edgesinternalList.size() == 0 && vMin->getId() != vTemp->getId() )
                            {
                                MJMesh::FrontElement2D *aux = new MJMesh::FrontElement2D( vMin, vTemp, 0 );
                                MJMesh::FrontElement2D *e =  static_cast<MJMesh::FrontElement2D *>(existEdgeInGlobal(aux, local_front));
                                delete aux;

                                if( e!= NULL )
                                    edgesinternalList.insert(e);

                                if(this->boundary->have(vTemp))
                                    common_vertices_list.insert(vTemp);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                            }

                            for( std::set<Data::FrontElement *>::iterator it = edgesinternalList.begin(); it != edgesinternalList.end(); it++ )
                            {
                                if( box->in( ((MJMesh::FrontElement2D*)(*it))->getV1()->getPoint() ) && box->in( ((MJMesh::FrontElement2D*)(*it))->getV2()->getPoint() ) )
                                {
                                    //std::cout << "DELETAAAAAAA: " << (*it)->getId() << std::endl;
                                    Data::FrontElementList adjs0 = static_cast<MJMesh::Vertex *>((*it)->getVertex(0))->getAdjacentFrontElements();
                                    Data::FrontElementList adjs1 = static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->getAdjacentFrontElements();

                                    int size0 = 0;
                                    int size1 = 0;

                                    while (!adjs0.empty())      ///CONTA AS ADJ DO VERTICE 0
                                    {
                                        Data::FrontElement *adj = adjs0.front();

                                        if( sub_front->have(adj) )
                                            size0++;

                                        //std::cout << "adj fel0 " << adj->text() << std::endl;
                                        adjs0.pop_front();
                                    }
                                    while (!adjs1.empty())      ///CONTA AS ADJ DO VERTICE 1
                                    {
                                        Data::FrontElement *adj = adjs1.front();

                                        if( sub_front->have(adj) )
                                            size1++;

                                        //std::cout << "adj fel1 " << adj->text() << std::endl;
                                        adjs1.pop_front();
                                    }
                                    //std::cout << "SIZE 0: " << size0 << "  ID: " << (*it)->getVertex(0)->getId() << std::endl;
                                    //std::cout << "SIZE 1: " << size1 << "  ID: " << (*it)->getVertex(1)->getId() << std::endl;

                                    if( sub_front->have( (*it) ) )
                                        sub_front->remove( (*it) );
                                    //if( this->front->have( (*it) ) )
                                    //this->front->remove( (*it) );

                                    if( vTemp->getId() != (*it)->getVertex(0)->getId() &&  size0 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(0) ))
                                        {
                                            //std::cout << "Remove vertice 0: " << (*it)->getVertex(0)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(0) );
                                        }
                                        if( local_front->have( (*it)->getVertex(0) ))
                                            local_front->remove( (*it)->getVertex(0) );

                                    }

                                    if( vTemp->getId() != (*it)->getVertex(1)->getId() &&  size1 < 2 )
                                    {
                                        if( sub_front->have( (*it)->getVertex(1) ))
                                        {
                                            //std::cout << "Remove vertice 1: " << (*it)->getVertex(1)->getId() << std::endl;
                                            sub_front->remove( (*it)->getVertex(1) );
                                        }
                                        if( local_front->have( (*it)->getVertex(1) ))
                                            local_front->remove( (*it)->getVertex(1) );
                                    }
                                }
                                else
                                    swappingEdges.push_back( (*it) );
                            }
                            //delete aux_e;


                            //MJMesh::Vertex *v_aux = nearestCrossVertex(sub_front, vMin);
                            //addAdjCross(sub_front, vMin, v_aux);

                            vMin = vTemp;
                            iter2++;

                            delete e;
                            delete vMax;

                            continue;
                        }

                        //unusedVertex(sub_front, rejected, vMin);
                        if( !unusedVertex(sub_front, local_front, interfaceEdges, vMin) )
                        {
                            MJMesh::FrontElement2D *aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );

                            MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                            if(v_aux==NULL)
                                v_aux = nearestCrossVertex(sub_front, vTemp);

                            //if( includeInModel(box, sub_front, rejected, aux_e, vMin, vTemp) == true )
                            if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, aux_e, vMin, vTemp, taskId) == false )
                            {
                                if( !box->in(vTemp->getPoint()) && !box->in(vMin->getPoint()) )             ///FORA DA BOX - ADD AO SUB_DOMINIO
                                {
                                    aux_e = new MJMesh::FrontElement2D( vMin, vTemp, id_Manager->next(1) );
                                    swappingEdges.push_back( aux_e );
                                }
                            }
                            else
                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);

                        }
                        else
                        {
                            if (!sub_front->have(vTemp->getAdjacentFrontElements().front(), 2))
                            {
                                MJMesh::Vertex *v_aux = nearestVertex(local_front, vTemp);

                                if(v_aux==NULL)
                                    v_aux = vTemp;

                                addAdjCross(sub_front, swappingEdges, v_aux, vTemp);
                            }

                            e->replace(vMin, NULL);
                            delete vMin;
                        }

                        if(this->boundary->have(vTemp))
                            common_vertices_list.insert(vTemp);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                        vMin = vTemp;
                        iter2++;

                        delete e;
                        delete vMax;

                        continue;
                    }
                    //else
                    //sub_front->remove(vTemp);
                }
                //std::cout << "saiu do sabia ext =D" << std::endl;

                delete e;
                delete vMax;

                vMax = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
                e = new MJMesh::FrontElement2D( vMin, vMax, id_Manager->next(1) );

                // vAux é usado somente em caso de buraco na partição
                MJMesh::Vertex *vAux = NULL;
                if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1), 0);
                else
                    vAux = new MJMesh::Vertex( static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1), 0);


                if( includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vAux, taskId) == false )
                {
#if USE_OPENMP
                    #pragma omp critical (front)
#endif //#if USE_OPENMP
                    {
                        if(local_front->have(vMax))
                            local_front->remove(vMax);
                    }
                    sub_front->remove(vMax);
                    //delete e;
                    delete vMax;


                    if( edgeOk(sub_front, local_front, interfaceEdges, id_Manager, vAux, taskId) == false)
                    {
                        vMin = vAux;
                        continue;
                    }

                    delete vAux;

                    if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
                    else
                        vAux = this->findOrCreate(taskId, sub_front, local_front, id_Manager, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));


                    vMin = vAux;
                    continue;
                }
                else
                    vMin = vMax;


                iter2++;
                delete vAux;
            }

        }

        ///CRIA AS ARESTAS FINAIS
        if( vMin->getPoint()->getCoord(0) + Data::getTolerance() < box->getMax()->getCoord(0) )
        {
            //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
            MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMin()->getCoord(1), id_Manager->next(0));
            MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

            MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, local_front, e_ori, vMin, taskId);
            if(vFinal == NULL)
                vFinal = findOrCreate(taskId, sub_front, local_front, id_Manager, box->getMax()->getCoord(0), box->getMin()->getCoord(1));

            MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, id_Manager->next(1) );

            //removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

            //std::cout << "ARESTA FINAL :::: " << " -- " << vMin->getId() << vFinal->getId() << std::endl;

            //includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin);
            includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, e, vMin, vFinal, taskId);

            //if(this->boundary->have(vFinal))
            //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

            delete e_ori;
            delete v;

            ///TESTE PARA FECHAR A FRONTEIRA FORA DA BOX
            if( vFinal->getAdjacentFrontElements().size() == 1 )
            {
                MJMesh::Vertex *vTemp = nearestCrossVertex(sub_front, vFinal);

                if( vTemp != NULL && vTemp->getPoint()->getCoord(0) > vFinal->getPoint()->getCoord(0) && box->out( vTemp->getPoint() ) )
                {
                    vMin = vFinal;
                    vFinal = vTemp;

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, id_Manager->next(1) );

                    //std::cout << "ARESTA FINAL EXTRA :::: " << vMin->getId() << " -- " << vFinal->getId() << std::endl;

                    existEdgeInGlobal(e, local_front);

                    interfaceEdges.push_back( e );

                    if(!sub_front->have(e))
                    {
                        sub_front->add(e);
                    }

                    //if(this->boundary->have(vFinal))
                    //common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
                }
            }

            common_vertices_list.insert(vFinal);                   ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
        }

    }
    break;

    default:
        std::cout << "Not implemented..." << std::endl;
        break;
    }


//PEGAR AS REJECTED E ELIMINAR AS ARESTAS TOTALMENTE FORA DA PARTIÇÃO
    //sub_front->setCrossing(sub_front->getRejected());

    //correctRejected(sub_front, rejected);     //Retira arestas soltas

    ///sub_front->moveRejectedToCrossing();     //NOVAS VERSAO NAO TEM ISSO MAIS

    //sub_front->setElements(interfaceEdges);
    for (Data::FrontElementList::iterator iter = interfaceEdges.begin(); iter != interfaceEdges.end(); iter++)      ///COPIANDOS AS ARESTAS CRIADAS PARA A SUB FRONT
    {
        if (!sub_front->have((*iter)))
        {
            sub_front->addElement((*iter));
        }
    }

    for (Data::FrontElementList::iterator iter = swappingEdges.begin(); iter != swappingEdges.end(); iter++)      ///COPIANDOS AS ARESTAS DO TESTE DE PROX PARA A SUB FRONT
    {
        if (sub_front->have((*iter)))
        {
            continue;
        }

        //sub_front->addRejected( (*iter) );
        //if(!sub_front->have( (*iter) ) )
        sub_front->addCrossing( (*iter) );
        //sub_front->addBoundary( (*iter) );

        for (UInt i = 0; i < (*iter)->numVertices(); i++)
        {
            Data::Vertex *v = (*iter)->getVertex(i);

            if (!sub_front->have(v))
            {
                sub_front->add(v);
            }
        }
    }

    correctHoles(sub_front, local_front);

    cleanOutEdges(sub_front);

    for (Data::Front::FrontElementIterator iter = sub_front->eBegin(3);
            iter != sub_front->eEnd(3);)
    {
        Data::FrontElement *fel = (*iter);

        iter = sub_front->erase(iter, 3);

        if (this->boundary->have(fel))
        {
            sub_front->addBoundary(fel);
        }
        else
        {
            if( !sub_front->have(fel) )
                sub_front->addElement(fel);
        }
    }

    ///TRANSFORMAR AS CÉLULAS UTILIZADAS EM ON
    ///CÉLULAS IN
    if(updateBox)
    {
        for (iter = topBoundaryCells.begin(); iter != topBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = baseBoundaryCells.begin(); iter != baseBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = leftBoundaryCells.begin(); iter != leftBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = rightBoundaryCells.begin(); iter != rightBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);

        for (iter = onTopBoundaryCells.begin(); iter != onTopBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = onBaseBoundaryCells.begin(); iter != onBaseBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = onLeftBoundaryCells.begin(); iter != onLeftBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
        for (iter = onRightBoundaryCells.begin(); iter != onRightBoundaryCells.end(); iter++)
            PMJMesh::setPosition(box->dimension(), static_cast<Data::BoxTreeCell *>( (*iter) ), Data::ON);
    }


    /*#pragma omp critical
    {
        std::cout << "--- DEPOIS ---" << std::endl;
        std::cout << "****** Na subfront em " << sub_front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< sub_front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = sub_front->eBegin(i); it != sub_front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << sub_front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = sub_front->vBegin(); it != sub_front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }

        std::cout << "!!!!!!!!!!! Na front em " << this->front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< this->front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = this->front->eBegin(i); it != this->front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << this->front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = this->front->vBegin(); it != this->front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }
    }*/

    return;
}

///FUNÇÃO DO UNIQUE DA SURFACE_LIST
bool same_vertex (Data::Vertex * first, Data::Vertex * second)
{
    return ( first->getId() == second->getId() );
}
bool same_point (Data::Point * first, Data::Point * second)
{
    return ( first->match(second) );
}
bool same_point2 (Data::Point3D first, Data::Point3D second)
{
    return ( first.match(second) );
}
///FUNÇÃO DO SORT POINT3D
bool sort_points (Data::Point &first, Data::Point &second)
{
    if ( first.getCoord(0)  != second.getCoord(0) )
    {
        return first.getCoord(0) < second.getCoord(0);
    }
    if (first.getCoord(1) != second.getCoord(1))
    {
        return first.getCoord(1) < second.getCoord(1);
    }
    return first.getCoord(2) < second.getCoord(2);
}
///FUNÇÃO DO SORT POINT3D
bool sort_point (Data::Point * first, Data::Point * second)
{
    if ( first->getCoord(0)  != second->getCoord(0) )
    {
        return first->getCoord(0) < second->getCoord(0);
    }
    if (first->getCoord(1) != second->getCoord(1))
    {
        return first->getCoord(1) < second->getCoord(1);
    }
    return first->getCoord(2) < second->getCoord(2);
}
///FUNÇÃO DO SORT VERTEX
bool sort_vertex (Data::Vertex * first, Data::Vertex * second)
{
    return sort_point( first->getPoint(), second->getPoint() );
}
///FUNÇÃO DO SORT VERTEX PELO NUMERO DE ADJ
bool sort_vertex_adj (Data::Vertex * first, Data::Vertex * second)
{
    return ( static_cast<MJMesh::Vertex *>(first)->numAdjacentFrontElements() < static_cast<MJMesh::Vertex *>(second)->numAdjacentFrontElements() );
}


bool sort_frontElementId (Data::FrontElement *first, Data::FrontElement *second)
{
    return first->getId() < second->getId();
}

void makeSurfaceVertexes( Data::VertexList &surface_vertexs, Data::VertexHashMap &originals )
{
    Data::VertexList final_vertexs;

    originals.reserve(surface_vertexs.size());

    for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
    {
        Data::Vertex *v = NULL;

        for (Data::Front::VertexIterator iter = final_vertexs.begin(); iter != final_vertexs.end(); iter++)
        {
            if( (*iter)->getId() == (*it)->getId() )
            {
                v = (*iter);
                break;
            }
        }

        if (v == NULL)
        {
            v = new MJMesh::Vertex( (*it)->getPoint()->getCoord(0),
                                    (*it)->getPoint()->getCoord(1),
                                    (*it)->getPoint()->getCoord(2), (*it)->getId() );

            originals[(*it)->getId()] = (*it);
        }

        final_vertexs.push_back( v );
    }

    surface_vertexs.swap( final_vertexs );
}

void PMJMesh2::APrioriParallelMeshGenerator::orient(MJMesh::FrontElementSurface *current, Data::FrontElementHash &right, const Data::FrontElementHash &wrong, bool detour) const
{
    MJMesh::Vertex *v2 = current->getV2();

    if ((detour) && (v2->numAdjacentFrontElements() != 2))
    {
        return;
    }

    Data::FrontElementList adjs;

    for (Data::FrontElementList::iterator iter = v2->feBegin();
            iter != v2->feEnd(); iter++)
    {
        MJMesh::FrontElementSurface *next = static_cast<MJMesh::FrontElementSurface *>((*iter));

        if (next == current)
        {
            continue;
        }

        if (!right.insert(next).second)
        {
            continue;
        }

        if (next->getV2() == v2)
        {
            next->invert();
        }

        adjs.push_back(next);
    }

    if (adjs.empty())
    {
        return;
    }

    for (Data::FrontElementList::iterator iter = adjs.begin(), last = --adjs.end();
            iter != adjs.end(); iter++)
    {
        MJMesh::FrontElementSurface *next = static_cast<MJMesh::FrontElementSurface *>((*iter));

        if ((detour) && (wrong.find(next) != wrong.end()))
        {
            continue;
        }

        this->orient(next, right, wrong, (detour || (iter != last)));
    }
}

Data::Mesh* PMJMesh2::APrioriParallelMeshGenerator::generateSuportMesh( Data::VertexList internal_vertexs, Data::ElementList internal_elements,
        MJMesh::Boundary *surface_boundary, Real maxElementSize, ULInt vId, ULInt feId )
{
    Data::VertexHashMap interface_vertexes;
    interface_vertexes.reserve( surface_boundary->getVertices().size() );
    Data::VertexList new_surface_vertexs;
    Data::FrontElementList new_surface_elements;


    for (Data::Front::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); it++)
    {
        MJMesh::Vertex *v = new MJMesh::Vertex( (*it)->getPoint()->getCoord(0), (*it)->getPoint()->getCoord(1), (*it)->getPoint()->getCoord(2), (*it)->getId() );
        interface_vertexes[ (*it)->getId() ] = v;
        new_surface_vertexs.push_back(v);
    }

    for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( interface_vertexes[ (*it)->getVertex(0)->getId() ] );
        MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>( interface_vertexes[ (*it)->getVertex(1)->getId() ] );

        MJMesh::FrontElementSurface *edge = new MJMesh::FrontElementSurface( v1, v2, (*it)->getId() );
        new_surface_elements.push_back( edge );
    }

    MJMesh::Boundary *new_surface_boundary = new MJMesh::Boundary();
    new_surface_boundary->setVertices( new_surface_vertexs );
    new_surface_boundary->setElements( new_surface_elements );


    TecGraf::MshSurf::MainDrive *mesh_surf_driver = new TecGraf::MshSurf::MainDrive();

    mesh_surf_driver->setAlgorithm( TecGraf::MshSurf::SURF_3D_SUPPORT );

    Data::Mesh *supportMesh = new MJMesh::Mesh();
    supportMesh->setVertices( internal_vertexs );
    supportMesh->setElements( internal_elements );

    Performer::IdManager *mesh_surf_idManager = mesh_surf_driver->makeIdManager(NULL);
    mesh_surf_idManager->setId(0, 1 + vId);
    mesh_surf_idManager->setId(1, 1 + feId);

    mesh_surf_driver->setSupportMaxElementSize(maxElementSize*2.0);
    mesh_surf_driver->setSupportComputeCurvature(false);
    mesh_surf_driver->setSupportInvertMesh(false);
    mesh_surf_driver->setIdManager(mesh_surf_idManager);
    mesh_surf_driver->setBoundary( new_surface_boundary );
    mesh_surf_driver->setSupportMesh( supportMesh );
    //mesh_surf_driver->setSupportMaxNumberElements(static_cast<Int>( supportMesh->size() * 5.0 ));


    //std::cout << "GERANDO NOVA SUPERFICIE!  TAM : " << maxElementSize << std::endl;

    mesh_surf_driver->execute();

    if( mesh_surf_driver->getStatus() != mesh_surf_driver->success )
    {
        std::cout << "Erro ao tentar gerar a nova grade do MeshSurf!!!" << std::endl;
    }

    Data::Mesh *generatedSurfaceMesh = const_cast<Data::Mesh *>(mesh_surf_driver->getMesh());

    /*std::ofstream myfile("/home/nfs/daniel/Desktop/new_grade.gen");
    if (myfile.is_open())
    {
        myfile << "2.5" << std::endl ;
        myfile << generatedSurfaceMesh->getVertices().size() << std::endl ;
        for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
        {
            myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
        }
        myfile << "0" << std::endl;
        myfile << generatedSurfaceMesh->getElements().size() << std::endl ;
        for (Data::Mesh::ElementIterator it = generatedSurfaceMesh->eBegin(); it != generatedSurfaceMesh->eEnd(); it++)
        {
            myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
        }
    }
    myfile.close();*/



    return generatedSurfaceMesh;
}

void PMJMesh2::APrioriParallelMeshGenerator::translatePlane( Data::VertexList &internal_vertexs, UInt side, Real new_pos )
{
    for (std::list<Data::Vertex*>::iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
    {
        if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            (*it)->getPoint()->setCoord(0, new_pos);
        if( side == 1 || side == 4 ) ///TOP E BASE
            (*it)->getPoint()->setCoord(1, new_pos);
        if( side == 2 || side == 5 ) ///FRONT E BACK
            (*it)->getPoint()->setCoord(2, new_pos);
    }
}

void PMJMesh2::APrioriParallelMeshGenerator::flatSurfaceVertices( MJMesh::Boundary *surface_boundary, UInt side, Real new_pos, double minElementSize )
{
    ///PLANIFICANDO VERTICES PARA A POSIÇÃO NEW_POS
    for (Data::Boundary::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); ++it)
    {
        if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            (*it)->getPoint()->setCoord(0, new_pos);
        if( side == 1 || side == 4 ) ///TOP E BASE
            (*it)->getPoint()->setCoord(1, new_pos);
        if( side == 2 || side == 5 ) ///FRONT E BACK
            (*it)->getPoint()->setCoord(2, new_pos);
    }

    ///VERIFICANDO SE VERTICES FORAM PLANIFICADOS PARA A MESMA POSIÇÃO NO PLANO
    for (Data::Boundary::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); ++it)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>( (*it) );
        //std::cout << "v " << v->getId() << std::endl;

        Data::FrontElementList list_adj = v->getAdjacentFrontElements();
        for(std::list<Data::FrontElement *>::iterator ite = list_adj.begin();  ite != list_adj.end(); ++ite)
        {
            //std::cout << "FrontElement  " << (*ite)->text() << std::endl;
            MJMesh::FrontElement2D *aresta = ((MJMesh::FrontElement2D*)(*ite));
            MJMesh::Vertex *v_adj = NULL;

            if( ((MJMesh::FrontElement2D*)(*ite))->getV1()->getId() == v->getId() )
                v_adj = ((MJMesh::FrontElement2D*)(*ite))->getV2();
            else
                v_adj = ((MJMesh::FrontElement2D*)(*ite))->getV1();

            //std::cout << "v_adj " << v_adj->getId() << std::endl;


            if( v->getPoint()->match( v_adj->getPoint() ) || v->getPoint()->equal( v_adj->getPoint() )
                    || minElementSize/2.0 > aresta->getV1()->getPoint()->distance(aresta->getV2()->getPoint()) )
            {
                //std::cout << "Vertices colidindo OU QUASE na hora de planificar!!! " << v->getId() << " com " << v_adj->getId() << std::endl;
                //std::cout << "MINSIZE: " << minElementSize << " e aresta: " << aresta->getV1()->getPoint()->distance(aresta->getV2()->getPoint()) << std::endl;


                ///DESLOCANDO V PARA A MEDIA DELE COM O SEU ADJ QUE NÃO TEM COLISÃO
                for(std::list<Data::FrontElement *>::iterator it1 = list_adj.begin();  it1 != list_adj.end(); it1++)
                {
                    //std::cout << "FrontElement adj  " << (*it1)->text() << std::endl;

                    Data::FrontElement *fel = (*it1);

                    if (fel->have(v_adj))
                    {
                        continue;
                    }

                    Data::Vector3D vec;
                    vec.sum(fel->getVertex(0)->getPoint());
                    vec.sum(fel->getVertex(1)->getPoint());
                    vec.multiply(1.0/2.0);

                    v->getPoint()->setCoords( vec.getCoords() );
                }

                ///DESLOCANDO V_ADJ PARA A MEDIA DELE COM O SEU ADJ QUE NÃO TEM COLISÃO
                Data::FrontElementList list_adj2 = v_adj->getAdjacentFrontElements();
                for(std::list<Data::FrontElement *>::iterator it2 = list_adj2.begin();  it2 != list_adj2.end(); it2++)
                {
                    //std::cout << "FrontElement adj  " << (*it2)->text() << std::endl;

                    Data::FrontElement *fel = (*it2);

                    if (fel->have(v))
                    {
                        continue;
                    }

                    Data::Vector3D vec;
                    vec.sum(fel->getVertex(0)->getPoint());
                    vec.sum(fel->getVertex(1)->getPoint());
                    vec.multiply(1.0/2.0);

                    v_adj->getPoint()->setCoords( vec.getCoords() );
                }
            }
        }
    }

    ///VERIFICANDO SE O VERTICES ADJ DE UM VERTICE NÃO FOI PARA TRAS DELE - SE FOR INVERTER AS POSIÇÕES
    for (Data::Boundary::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); ++it)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>( (*it) );
        Data::FrontElementList list_adj = v->getAdjacentFrontElements();

        for(std::list<Data::FrontElement *>::iterator ite = list_adj.begin();  ite != list_adj.end(); ++ite)
        {
            MJMesh::FrontElement2D *aresta = ((MJMesh::FrontElement2D*)(*ite));
            MJMesh::Vertex *v_adj = NULL;
            MJMesh::Vertex *v_adj2 = NULL;

            if( aresta->getV1()->getId() == v->getId() )
                v_adj = aresta->getV2();
            else
                v_adj = aresta->getV1();


            Data::FrontElementList list_adj2 = v_adj->getAdjacentFrontElements();
            for(std::list<Data::FrontElement *>::iterator ite2 = list_adj2.begin();  ite2 != list_adj2.end(); ++ite2)
            {
                MJMesh::FrontElement2D *aresta2 = ((MJMesh::FrontElement2D*)(*ite2));

                if( aresta == aresta2 ) continue;


                if( aresta2->getV1()->getId() == v_adj->getId() )
                    v_adj2 = aresta2->getV2();
                else
                    v_adj2 = aresta2->getV1();

                break;
            }

            if( v->getPoint()->distance( v_adj->getPoint() ) > v->getPoint()->distance( v_adj2->getPoint() ) )
            {
                //Data::Vector3D vetor1 = ( v->getPoint(), v_adj->getPoint() );
                //Data::Vector3D vetor2 = ( v->getPoint(), v_adj2->getPoint() );
                Data::Vector2D vetor1;
                Data::Vector2D vetor2;

                if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                {
                    vetor1.setPosition( v_adj->getPoint()->getCoord(1) - v->getPoint()->getCoord(1), v_adj->getPoint()->getCoord(2) - v->getPoint()->getCoord(2) );
                    vetor2.setPosition( v_adj2->getPoint()->getCoord(1) - v->getPoint()->getCoord(1), v_adj2->getPoint()->getCoord(2) - v->getPoint()->getCoord(2) );
                }
                if( side == 1 || side == 4 ) ///TOP E BASE
                {
                    vetor1.setPosition( v_adj->getPoint()->getCoord(0) - v->getPoint()->getCoord(0), v_adj->getPoint()->getCoord(2) - v->getPoint()->getCoord(2) );
                    vetor2.setPosition( v_adj2->getPoint()->getCoord(0) - v->getPoint()->getCoord(0), v_adj2->getPoint()->getCoord(2) - v->getPoint()->getCoord(2) );
                }
                if( side == 2 || side == 5 ) ///FRONT E BACK
                {
                    vetor1.setPosition( v_adj->getPoint()->getCoord(0) - v->getPoint()->getCoord(0), v_adj->getPoint()->getCoord(1) - v->getPoint()->getCoord(1) );
                    vetor2.setPosition( v_adj2->getPoint()->getCoord(0) - v->getPoint()->getCoord(0), v_adj2->getPoint()->getCoord(1) - v->getPoint()->getCoord(1) );
                }

                Real cos = vetor1.cosAngle(vetor2);

                ///VIZINHOS ESTÂO EM UMA CURVA
                if( cos <= 0.94 ) continue;

                //std::cout << "Cos: " << cos << " com " << v->getId() << std::endl;
                //std::cout << "invertendo: " << v_adj->text() << " com " << v_adj2->text() << std::endl;
                Data::Real temp_coords[3] = {v_adj->getPoint()->getCoord(0), v_adj->getPoint()->getCoord(1), v_adj->getPoint()->getCoord(2) };
                v_adj->getPoint()->setCoords( v_adj2->getPoint()->getCoords() );
                v_adj2->getPoint()->setCoords( temp_coords );
                //std::cout << "invertido: " << v_adj->text() << " com " << v_adj2->text() << std::endl;
            }
        }
    }

}

void PMJMesh2::APrioriParallelMeshGenerator::prepareSurfacesMeshSurf(
    PMJMesh::Front *sub_front, PMJMesh::Front *local_front, const Performer::InnerSurfacesIdentifier::SurfaceTree *root,
    Data::VertexSet &common_vertices_list, Data::VertexHashMap &original_surface_vertexes,
    Data::VertexList internal_vertexs, Data::ElementList internal_elements,
    Performer::IdManager *id_Manager, ULInt taskId, bool invert_Mesh, UInt side, Real &meshSurf_Processing)
{
    //Real start = Data::time();
    //std::cout << "num children: " << root->children.size() << std::endl;

    //Performer::InnerSurfacesIdentifier::SurfaceTreeList surfaces = root->children;

    MJMesh::Boundary *surface_boundary = static_cast<MJMesh::Boundary *>(root->surface->boundary);


    ///ADICIONANDO OS POSSIVEIS BURACOS NA surface->boundary
    for (Performer::InnerSurfacesIdentifier::SurfaceTreeList::const_iterator iter = root->children.begin();
            iter != root->children.end(); iter++)
    {
        Data::Boundary *other_boundary = (*iter)->surface->boundary;

        for (Data::Boundary::VertexIterator iter2 = other_boundary->vBegin();
                iter2 != other_boundary->vEnd(); iter2 = other_boundary->erase(iter2))
        {
            surface_boundary->add((*iter2));
        }

        for (Data::Boundary::FrontElementIterator iter2 = other_boundary->eBegin();
                iter2 != other_boundary->eEnd(); iter2 = other_boundary->erase(iter2))
        {
            surface_boundary->add((*iter2));
        }
    }

    double maxElementSize = 0.0;
    double minElementSize = DBL_MAX;
    ULInt vId = 0;
    ULInt feId = 0;

    ///ENCONTRANDO ID's E TAMANHO MAXIMO DOS ELEMENTOS
    for (Data::Boundary::FrontElementIterator it = surface_boundary->eBegin();
            it != surface_boundary->eEnd(); ++it)
    {

        maxElementSize += (*it)->getShape()->size();

        if( minElementSize > (*it)->getShape()->size() )
        {
            minElementSize = (*it)->getShape()->size();
        }

        if( (*it)->getId() > feId )
        {
            feId = (*it)->getId();
        }
        if( (*it)->getVertex(0)->getId() > vId )
        {
            vId = (*it)->getVertex(0)->getId();
        }
    }

    maxElementSize /= static_cast<Real>(surface_boundary->size());
    maxElementSize *= this->interfaceElementSizeFactor;              ///REDUTOR DE ELEMENTOS
    Real razao = maxElementSize * 0.1;
    Real plane_pos[4];

    Data::Point3D p_min = Data::Point3D(DBL_MAX, DBL_MAX, DBL_MAX);
    Data::Point3D p_max = Data::Point3D(-DBL_MAX, -DBL_MAX, -DBL_MAX);

    ///ENCONTRANDO POSIÇÔES DOS VERTICES DA GRADE
    {
        Real new_pos = 0.0;
        Real pos_max = -DBL_MAX;
        Real pos_min = DBL_MAX;

        for (Data::Boundary::VertexIterator it = surface_boundary->vBegin();
                it != surface_boundary->vEnd(); ++it)
        {
            for(int i=0; i<3; i++)
            {
                if( p_min.getCoord(i) > (*it)->getPoint()->getCoord(i) )
                    p_min.setCoord( i, (*it)->getPoint()->getCoord(i) - Data::getTolerance() );

                if( p_max.getCoord(i) < (*it)->getPoint()->getCoord(i) )
                    p_max.setCoord( i, (*it)->getPoint()->getCoord(i) + Data::getTolerance() );
            }


            if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            {
                new_pos += (*it)->getPoint()->getCoord(0);

                if( pos_max < (*it)->getPoint()->getCoord(0) )
                    pos_max = (*it)->getPoint()->getCoord(0);
                if( pos_min > (*it)->getPoint()->getCoord(0) )
                    pos_min = (*it)->getPoint()->getCoord(0);
            }
            if( side == 1 || side == 4 ) ///TOP E BASE
            {
                new_pos += (*it)->getPoint()->getCoord(1);

                if( pos_max < (*it)->getPoint()->getCoord(1) )
                    pos_max = (*it)->getPoint()->getCoord(1);
                if( pos_min > (*it)->getPoint()->getCoord(1) )
                    pos_min = (*it)->getPoint()->getCoord(1);
            }
            if( side == 2 || side == 5 ) ///FRONT E BACK
            {
                new_pos += (*it)->getPoint()->getCoord(2);

                if( pos_max < (*it)->getPoint()->getCoord(2) )
                    pos_max = (*it)->getPoint()->getCoord(2);
                if( pos_min > (*it)->getPoint()->getCoord(2) )
                    pos_min = (*it)->getPoint()->getCoord(2);
            }
        }

        double med = new_pos / static_cast<Real>(surface_boundary->verticesSize());
        double eixo_med = (pos_max + pos_min) / 2.0;
        double med_med = (pos_max + pos_min + med) / 3.0;

        //std::cout << "POSIÇÂO NOVA NO EIXO - MEDIA: " << med << std::endl;
        //std::cout << "POSIÇÂO NOVA NO EIXO - EIXO MEDIO: " << eixo_med << std::endl;
        //std::cout << "POSIÇÂO NOVA NO EIXO - JUNÇÂO: " << med_med << std::endl;

        plane_pos[0] = med;
        plane_pos[1] = eixo_med;
        plane_pos[2] = med_med;
        if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            plane_pos[3] = internal_vertexs.front()->getPoint()->getCoord(0);
        if( side == 1 || side == 4 ) ///TOP E BASE
            plane_pos[3] = internal_vertexs.front()->getPoint()->getCoord(1);
        if( side == 2 || side == 5 ) ///FRONT E BACK
            plane_pos[3] = internal_vertexs.front()->getPoint()->getCoord(2);
    }


    //std::cout << "VALOR MAX DEPOIS: " << maxElementSize << std::endl;

    ///CRIAÇAO .GEN
    {
        /*static int ert = 1;
        std::string as = "/home/nfs/daniel/Desktop/entrada_MeshSurf";
        as += std::to_string(ert++);
        as += ".gen";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_boundary->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_boundary->size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();*/


        /*static int eert = 1;
        std::string pal = "/home/nfs/daniel/Desktop/grade";
        pal += std::to_string(eert++);
        pal += ".gen";
        std::ofstream myfile2 ( pal );
        if (myfile2.is_open())
        {
            myfile2 << std::setprecision(30) << "2.5" << std::endl ;
            myfile2 << internal_vertexs.size() << std::endl ;
            for (std::list<Data::Vertex*>::const_iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
            {
                myfile2 << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile2 << "0" << std::endl;
            myfile2 << internal_elements.size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=internal_elements.begin(); it!=internal_elements.end(); ++it)
            {
                myfile2 << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile2.close();*/
    }


    TecGraf::MshSurf::MainDrive *mesh_surf_driver;
    Data::Mesh *supportMesh = NULL;
    Data::Mesh *generatedSurfaceMesh = NULL;
    Real time_MeshSurf_Processing = 0.0;


    int tentativa = 0;
    int repeticoes = 1;


    //Real t1 = Data::time();
    //std::cout << "Preparou para Geração da Malha: " << t1 - start << std::endl;

    ///EXECUTANDO MESH_SURF
    while( true )
    {
        mesh_surf_driver = new TecGraf::MshSurf::MainDrive();

        mesh_surf_driver->setAlgorithm( TecGraf::MshSurf::SURF_3D_SUPPORT );

        //surface_boundary = new MJMesh::Boundary();
        //surface_boundary->setVertices( node_surface_vertices );
        //surface_boundary->setElements( node_surface_elements );

        ///planificar anel de arestas
        flatSurfaceVertices( surface_boundary, side, plane_pos[ tentativa ], minElementSize );

        ///move o plano
        translatePlane( internal_vertexs, side, plane_pos[ tentativa++ ] );


        ///GRADE DE ACORDO COM A OCTREE
        supportMesh = new MJMesh::Mesh();
        supportMesh->setVertices( internal_vertexs );
        supportMesh->setElements( internal_elements );


        ///GERAR A NOVA GRADE DE ACORDO COM A BORDA
        //supportMesh = generateSuportMesh(internal_vertexs, internal_elements, surface_boundary, maxElementSize, vId, feId);


        ///MELHORIA DA GRADE DE SUPORTE
        PMJMesh2::APrioriSupportMeshLaplacianSmoother smoother(supportMesh, surface_boundary, true, 0.5, 2, 0, true);
        smoother.setSortPointsDisplacement(true);
        smoother.execute();


        ///IMPRESSÂO
        {
            /*Static int ert = 1;
            std::string as = "/home/nfs/daniel/Desktop/entrada_MeshSurf_MOD";
            as += std::to_string(ert++);
            as += ".gen";
            std::ofstream myfile ( as );

            if (myfile.is_open())
            {
                myfile << std::setprecision(30) << "2.5" << std::endl ;
                myfile << surface_boundary->verticesSize() << std::endl ;
                for (Data::Front::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); it++)
                {
                    myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile << surface_boundary->size() << std::endl ;
                for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
                {
                    myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
                }
                myfile << "0" << std::endl;
            }
            myfile.close();*/

            /*static int eert = 1;
            std::string pal = "/home/nfs/daniel/Desktop/grade_MOD";
            pal += std::to_string(eert++);
            pal += ".gen";
            std::ofstream myfile2 ( pal );
            if (myfile2.is_open())
            {
                myfile2 << std::setprecision(30) << "2.5" << std::endl ;
                myfile2 << internal_vertexs.size() << std::endl ;
                for (std::list<Data::Vertex*>::const_iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
                {
                    myfile2 << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile2 << "0" << std::endl;
                myfile2 << internal_elements.size() << std::endl ;
                for (std::list<Data::Element*>::const_iterator it=internal_elements.begin(); it!=internal_elements.end(); ++it)
                {
                    myfile2 << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
                }
            }
            myfile2.close();*/
        }

        Performer::IdManager *mesh_surf_idManager = mesh_surf_driver->makeIdManager(NULL);
        mesh_surf_idManager->setId(0, vId +1 );
        mesh_surf_idManager->setId(1, feId +1 );

        mesh_surf_driver->setSupportMaxElementSize(maxElementSize);
        mesh_surf_driver->setSupportComputeCurvature(false);
        mesh_surf_driver->setSupportInvertMesh(false);
        mesh_surf_driver->setIdManager(mesh_surf_idManager);
        mesh_surf_driver->setBoundary( surface_boundary );
        mesh_surf_driver->setSupportMesh( supportMesh );
        //mesh_surf_driver->setSupportMaxNumberElements(static_cast<Int>( supportMesh->size() * 10.0 ));


        //std::cout << "TENTANDO GERAR SUPERFICIE!  TAM : " << maxElementSize << " parametro: " << this->interfaceElementSizeFactor << " e plano:" << plane_pos[tentativa-1] << " tentativa: " << tentativa-1 << std::endl;


        mesh_surf_driver->execute();

        //std::cout << "SUPERFICIE TAM : " << const_cast<Data::Mesh *>(mesh_surf_driver->getMesh())->getElements().size() << std::endl;


        if( mesh_surf_driver->getStatus() != mesh_surf_driver->success )
        {
            std::cout << "Erro ao tentar gerar a interface pelo MeshSurf!!!     " << mesh_surf_driver->getStatus().getMessage() << std::endl;
        }

        if( repeticoes == 1 )
            time_MeshSurf_Processing = Data::time();

        //std::cout << "taskId : " << taskId << std::endl;
        //std::cout << "side   : " << side << std::endl;
        //std::cout << "Status   : " << mesh_surf_driver->getStatus().getMessage() << std::endl;
        //std::cout << "GEROU A SUPERFICIE COM TAM : " << maxElementSize << std::endl;
        //std::cout << "GEROU A SUPERFICIE COM TAM : " << mesh_surf_driver->getMesh()->getElements().size() << " COM TAM: " << maxElementSize << std::endl;

        //if( (mesh_surf_driver->getStatus() == mesh_surf_driver->success) && checkSurfaceMesh( sub_front, const_cast<Data::Mesh *>(mesh_surf_driver->getMesh()), surface_boundary, mesh_surf_idManager, surface_boundary->getVertices(), surface_boundary->getElements(), original_surface_vertexes, side, plane_pos[tentativa-1], p_min, p_max ) )
        if( (mesh_surf_driver->getStatus() == mesh_surf_driver->success) && improveSurfaceMesh( sub_front, const_cast<Data::Mesh *>(mesh_surf_driver->getMesh()), surface_boundary, mesh_surf_idManager, surface_boundary->getVertices(), surface_boundary->getElements(), original_surface_vertexes, side, plane_pos[tentativa-1], p_min, p_max ) )
        {
            generatedSurfaceMesh = const_cast<Data::Mesh *>(mesh_surf_driver->getMesh());

            //std::cout << generatedSurfaceMesh->text() << std::endl;

            ///IMPRESSÃO GEN
            {
                /*static int ert = 1;
                std::string as = "/home/nfs/daniel/Desktop/generatedSurfaceMesh";
                as += std::to_string(ert++);
                as += ".gen";
                std::ofstream myfile ( as );


                //std::ofstream myfile("/home/nfs/daniel/Desktop/ELEMENTOS_criados.gen");
                myfile << std::setprecision(30) << "2.5" << std::endl ;
                myfile << generatedSurfaceMesh->getVertices().size() << std::endl ;
                for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
                {
                    myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile << "0" << std::endl;
                myfile << generatedSurfaceMesh->getElements().size() << std::endl ;
                int i = 1;
                for (Data::Mesh::ElementIterator iter = generatedSurfaceMesh->eBegin(); iter != generatedSurfaceMesh->eEnd(); iter++)
                {
                    myfile << i++ << " 3 "  <<  (*iter)->getVertex(0)->getId() << " " << (*iter)->getVertex(1)->getId() << " " << (*iter)->getVertex(2)->getId() << std::endl;
                }
                myfile.close();*/
            }


            mesh_surf_driver->setBoundary(NULL);
            mesh_surf_driver->setMesh(NULL);
            supportMesh->clear();

            delete mesh_surf_driver;

            ///MELHORIA POR SWAP DA MALHA GERADA
            Tools::EdgeSwapBoundaryAdapter *adapter = new Tools::EdgeSwapBoundaryAdapter( generatedSurfaceMesh );
            adapter->setNoCrack(true);
            adapter->setNormalTestEnabled(false);
            adapter->setQualityThreshold(0.40);
            adapter->setSizeProportion(1.5);
            adapter->setChangeSurfaceBoundaryEnabled(true);
            adapter->setMaxEdgesOnBoundary(1);

            do
            {
                adapter->execute();
                //std::cout << "Melhoria - Mudou algo na Malha? " << adapter->isAdapted() << std::endl;
            }
            while (adapter->isAdapted());

            delete adapter;

            ///MELHORIA POR SMOOTH NOS VERTICES
            Tools::BoundaryLaplacianSmoother *smoother = new Tools::BoundaryLaplacianSmoother( this->dimension, NULL, generatedSurfaceMesh, 0.5, true );
            smoother->setNormalTestEnabled(false);
            smoother->setSortPointsDisplacement(true);
            smoother->execute();
            //std::cout << "Smooth feito: " << smoother->isSmoothed() << std::endl;
            delete smoother;

            ///IMPRESSÃO GEN
            {
                /*static int ert = 1;
                std::string as = "/home/nfs/daniel/Desktop/generatedSurfaceMesh_MOD";
                as += std::to_string(ert++);
                as += ".gen";
                std::ofstream myfile ( as );


                //std::ofstream myfile("/home/nfs/daniel/Desktop/ELEMENTOS_criados.gen");
                myfile << std::setprecision(30) << "2.5" << std::endl ;
                myfile << generatedSurfaceMesh->getVertices().size() << std::endl ;
                for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
                {
                    myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile << "0" << std::endl;
                myfile << generatedSurfaceMesh->getElements().size() << std::endl ;
                int i = 1;
                for (Data::Mesh::ElementIterator iter = generatedSurfaceMesh->eBegin(); iter != generatedSurfaceMesh->eEnd(); iter++)
                {
                    myfile << i++ << " 3 "  <<  (*iter)->getVertex(0)->getId() << " " << (*iter)->getVertex(1)->getId() << " " << (*iter)->getVertex(2)->getId() << std::endl;
                }
                myfile.close();*/
            }

            ///ENCONTRANDO OS VERTICES PARA CRIAR UMA BOX PARA A INTERFACE
            /*p_min.setPosition( DBL_MAX, DBL_MAX, DBL_MAX );
            p_max.setPosition( -DBL_MAX, -DBL_MAX, -DBL_MAX );
            for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
            {
                for(int i=0; i<3; i++)
                {
                    if( p_min.getCoord(i) > (*it)->getPoint()->getCoord(i) )
                        p_min.setCoord( i, (*it)->getPoint()->getCoord(i) - 3.0*Data::getTolerance() );

                    if( p_max.getCoord(i) < (*it)->getPoint()->getCoord(i) )
                        p_max.setCoord( i, (*it)->getPoint()->getCoord(i) + 3.0*Data::getTolerance() );
                }
            }*/

            break;
        }
        else
        {
            //std::cout << "GEROU BURACOS!" << std::endl;

            Data::Mesh *mesh_surf_mesh = const_cast<Data::Mesh *>(mesh_surf_driver->getMesh());

            for (Data::Boundary::FrontElementIterator iter = surface_boundary->eBegin();
                    iter != surface_boundary->eEnd(); iter++)
            {
                MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(*iter);

                MJMesh::Vertex *v_aux1 = fel->getV1();
                MJMesh::Vertex *v_aux2 = fel->getV2();

                mesh_surf_mesh->remove(fel);

                fel->clearAdjacency();
                v_aux1->clearAdjacentElements();
                v_aux2->clearAdjacentElements();

                ///DEVOLVENDO A ADJ DOS VERTICES
                v_aux1->add(fel);
                v_aux2->add(fel);
            }

            for (Data::Boundary::VertexIterator iter = surface_boundary->vBegin();
                    iter != surface_boundary->vEnd(); iter++)
            {
                mesh_surf_mesh->remove((*iter));
            }

            mesh_surf_driver->setBoundary(NULL);
            supportMesh->clear();

            delete mesh_surf_driver;

            //clearSurfaceMesh( const_cast<Data::Mesh *>(mesh_surf_driver->getMesh()), surface_boundary );


            if( tentativa >= 4 )
            {
                maxElementSize -= razao;
                tentativa = 0;
            }
            repeticoes++;

            if( repeticoes == 41 )      ///maxElementSize será ZERO
            {
                std::cout << "Impossivel gerar malha de interface - maxElementSize zerado após várias tentativas" << std::endl;
                return;
            }
        }
    }

    //std::cout << "* * * GEROU SUP!  TAM : " << maxElementSize << " par: " << this->interfaceElementSizeFactor << " plano: " << plane_pos[tentativa-1] << " tent: " << repeticoes << " check: " << Data::time() - time_MeshSurf_Processing << std::endl;
    //std::cout << "GEROU A SUPERFICIE COM TAM : " << generatedSurfaceMesh->getElements().size() << std::endl;

    {
        /*Data::VertexList vertices = sub_front->getVertices();
        Data::FrontElementList elements = sub_front->getAllElements();
        elements.insert(elements.end(), sub_front->eBegin(3), sub_front->eEnd(3));

        static int ertt = 1;
        std::string as = "/home/nfs/daniel/Desktop/sub_front_antes";
        as += std::to_string(ertt++);
        as += ".gen";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << vertices.size() << std::endl ;
            for (Data::Front::VertexIterator it = vertices.begin(); it != vertices.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=elements.begin(); it!=elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 3 " << ((MJMesh::FrontElement3D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement3D*)(*it))->getV2()->getId()<< " " << ((MJMesh::FrontElement3D*)(*it))->getV3()->getId() << std::endl ;
            }
        }
        myfile.close();*/
    }

    ///ADICIONANDO A MALHA DO generatedSurfaceMesh NA SUBFRONT
    if( generatedSurfaceMesh )
    {
        Data::VertexHashMap temp_interface_vertexes;
        temp_interface_vertexes.reserve( generatedSurfaceMesh->getVertices().size() );

        ///ESTRATEGIA NÃO FUNCIONA PARA MEMORIA COMPARTILHADA
        //Data::Box b(p_min, p_max);
        //Data::VertexList vertices;
        //vertices = local_front->verticesInRange(b);

        //std::cout << "TAMANHO DA LISTA: " << vertices.size() << std::endl;
        //std::cout << "TOTAL: " << generatedSurfaceMesh->getVertices().size() << std::endl;

        ///VERTICES
        for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
        {
            if( surface_boundary->have( (*it) ) )
            {
                temp_interface_vertexes[ (*it)->getId() ] = original_surface_vertexes[ (*it)->getId() ];
            }
            else
            {
                ///LOCAL / COMPARTILHADO
                //MJMesh::Vertex *v = simpleFindOrCreate( sub_front, local_front, id_Manager, vertices,(*it)->getPoint()->getCoord(0), (*it)->getPoint()->getCoord(1), (*it)->getPoint()->getCoord(2) );

                ///MEM DISTRIBUIDA
                MJMesh::Vertex *v = new MJMesh::Vertex( (*it)->getPoint()->getCoord(0), (*it)->getPoint()->getCoord(1), (*it)->getPoint()->getCoord(2), id_Manager->next(0) );
                local_front->add(v);
                sub_front->add(v);


                ///ADD NA TABELA HASH
                temp_interface_vertexes[(*it)->getId()] = v;
            }
        }

        //vertices.clear();

        ///ELEMENTOS
        for (Data::Mesh::ElementIterator iter = generatedSurfaceMesh->eBegin(); iter != generatedSurfaceMesh->eEnd(); iter++)
        {
            MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(0)->getId() ] );
            MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(1)->getId() ] );
            MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(2)->getId() ] );

            MJMesh::FrontElement *e = NULL;

            if (!e)
            {
                if(invert_Mesh)
                    e = new MJMesh::FrontElement3D( v3, v2, v1, id_Manager->next(1) );
                else
                    e = new MJMesh::FrontElement3D( v1, v2, v3, id_Manager->next(1) );
            }

            //std::cout << "ELE: " << e->text() << std::endl;

            ///TIREI ESTE TESTE, SE DER PROBLEMA SO TIRAR
            ///DEIXEI APENAS O addCrossing
            /*bool jump = false;

            for (Data::FrontElementList::iterator iter2 = v1->feBegin(); iter2 != v1->feEnd(); iter2++)
            {
                UInt i = (*iter2)->index(v1);

                if( (e->getId() == (*iter2)->getId()) || ((*iter2)->getVertex((i + 1)%(*iter2)->numVertices()) != v2) ||
                        ((*iter2)->getVertex((i + 2)%(*iter2)->numVertices()) != v3))
                {
                    continue;
                }
                else
                {
                    jump = true;
                    break;
                }
            }

            if(jump)
            {
                sub_front->addBoundary( e );
                std::cout << "ADD NA Boundary: " << e->text() << std::endl;         ///TALVEZ POSSA TIRAR ESSA VERIFICAÇÃO - ADD SOMENTE NA CROSS
            }
            else
                sub_front->addCrossing( e );*/

            sub_front->addCrossing( e );
        }

        ///IMPRESSÃO GEN
        {
            /*static int ert = 1;
            std::string as = "/home/nfs/daniel/Desktop/ELEMENTOS_criados_";
            as += std::to_string(taskId);
            as += "_";
            as += std::to_string(ert++);
            as += ".gen";
            std::ofstream myfile ( as );


            //std::ofstream myfile("/home/nfs/daniel/Desktop/ELEMENTOS_criados.gen");
            myfile << std::setprecision(30) << "2.5" << std::endl ;
            myfile << generatedSurfaceMesh->getVertices().size() << std::endl ;
            for (Data::Mesh::VertexIterator it = generatedSurfaceMesh->vBegin(); it != generatedSurfaceMesh->vEnd(); it++)
            {
                myfile << temp_interface_vertexes[(*it)->getId()]->getId() << "  "  << temp_interface_vertexes[(*it)->getId()]->getPoint()->getCoord(0) << " " << temp_interface_vertexes[(*it)->getId()]->getPoint()->getCoord(1) << " " << temp_interface_vertexes[(*it)->getId()]->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << generatedSurfaceMesh->getElements().size() << std::endl ;
            int i = 1;
            for (Data::Mesh::ElementIterator iter = generatedSurfaceMesh->eBegin(); iter != generatedSurfaceMesh->eEnd(); iter++)
            {
                MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(0)->getId() ] );
                MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(1)->getId() ] );
                MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>( temp_interface_vertexes[ (*iter)->getVertex(2)->getId() ] );
                //static int i = 1;
                myfile << i++ << " 3 "  <<  v1->getId() << " " << v2->getId() << " " << v3->getId() << std::endl;
            }
            myfile.close();*/
        }

    }
    else
        std::cout << "MESH NULL" << std::endl;

    surface_boundary->clear();

    delete generatedSurfaceMesh;

    {
        /*Data::VertexList vertices = sub_front->getVertices();
        Data::FrontElementList elements = sub_front->getAllElements();
        elements.insert(elements.end(), sub_front->eBegin(3), sub_front->eEnd(3));

        static int ertt = 1;
        std::string as = "/home/nfs/daniel/Desktop/sub_front_depois";
        as += std::to_string(ertt++);
        as += ".gen";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << vertices.size() << std::endl ;
            for (Data::Front::VertexIterator it = vertices.begin(); it != vertices.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=elements.begin(); it!=elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 3 " << ((MJMesh::FrontElement3D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement3D*)(*it))->getV2()->getId()<< " " << ((MJMesh::FrontElement3D*)(*it))->getV3()->getId() << std::endl ;
            }
        }
        myfile.close();*/
    }

    ///TEMPO DE AJUSTE PARA O MESHSURF
    meshSurf_Processing += Data::time() - time_MeshSurf_Processing;


    ///CHAMADA RECURSIVA PARA OS FILHOS
    for (Performer::InnerSurfacesIdentifier::SurfaceTreeList::const_iterator iter = root->children.begin();
            iter != root->children.end(); iter++)
    {
        for (Performer::InnerSurfacesIdentifier::SurfaceTreeList::const_iterator iter2 = (*iter)->children.begin();
                iter2 != (*iter)->children.end(); iter2++)
        {
            this->prepareSurfacesMeshSurf(sub_front, local_front, (*iter2),
                                          common_vertices_list, original_surface_vertexes, internal_vertexs, internal_elements,
                                          id_Manager, taskId, invert_Mesh, side, meshSurf_Processing);
        }
    }
}

void PMJMesh2::APrioriParallelMeshGenerator::improveBoundaryLoop( PMJMesh::Front *sub_front, Data::VertexHashMap &original_surface_vertexes, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements )
{
    ///PROCURANDO 3 VERTICES QUE FORMAM UMA FACE - CONVERTE PARA SOMENTE 2 VERTICES E 1 ARESTA
    bool para = true;
    while( para )
    {
        para = false;
        for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
        {
            MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( (*it) );
            MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>( original_surface_vertexes[ v1->getId() ] );
            MJMesh::Vertex *v2 = NULL;
            MJMesh::Vertex *v3 = NULL;

            if( v1->numAdjacentFrontElements() > 2 || v1->numAdjacentFrontElements() == 1 )
            {
                //std::cout << "ERROR... ANEL DE ARESTAS AINDA POSSUI ERROS!!!" << std::endl;
                continue;
            }

            ///SELECIONANDO OS VERTICES 2 VERTICES ADJACENTES A V
            for(Data::FrontElementList::iterator itt = v1->feBegin(); itt != v1->feEnd(); ++itt)
            {
                if ( static_cast<const MJMesh::FrontElement *>((*itt))->getV1() != v1)
                {
                    if( v2 )
                        v3 = static_cast<const MJMesh::FrontElement *>((*itt))->getV1();
                    else
                        v2 = static_cast<const MJMesh::FrontElement *>((*itt))->getV1();
                }
                else
                {
                    if( v2 )
                        v3 = static_cast<const MJMesh::FrontElement *>((*itt))->getV2();
                    else
                        v2 = static_cast<const MJMesh::FrontElement *>((*itt))->getV2();
                }
            }

            ///ARESTA DE V1 ESTÁ SOLTA - SERÁ ELIMINADA DEPOIS
            if( v3 == NULL )
                continue;

            if( original_surface_vertexes.find( v2->getId() ) == original_surface_vertexes.end()
                    || original_surface_vertexes.find( v3->getId() ) == original_surface_vertexes.end() )
            {
                continue;
            }

            MJMesh::Vertex *v2_ori = static_cast<MJMesh::Vertex *>( original_surface_vertexes[ v2->getId() ] );
            MJMesh::Vertex *v3_ori = static_cast<MJMesh::Vertex *>( original_surface_vertexes[ v3->getId() ] );

            for(Data::FrontElementList::iterator itt = v_ori->feBegin(); itt != v_ori->feEnd(); ++itt)
            {
                MJMesh::FrontElement3D *face = static_cast<MJMesh::FrontElement3D *>( (*itt) );

                if (!sub_front->have(face, 3))
                {
                    continue;
                }

                if( face->have( v2_ori ) && face->have( v3_ori ) )
                {
                    //std::cout << "MELHORANDO PELA FACE!!!" << std::endl;
                    //std::cout << "v1: " << v1->getId() << " v2: " << v2->getId() << " v3: " << v3->getId() << std::endl;
                    //std::cout << "ORI v: " << v_ori->getId()  << " ORI v2: " << v2_ori->getId() << " ORI v3: " << v3_ori->getId() << std::endl;

                    MJMesh::FrontElement2D* aresta_adj1 = ((MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().front()));
                    MJMesh::FrontElement2D* aresta_adj2 = ((MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().back()));

                    surface_elements.remove( aresta_adj1 );
                    surface_elements.remove( aresta_adj2 );
                    delete aresta_adj1;
                    delete aresta_adj2;

                    surface_vertexs.remove( v1 );
                    delete v1;


                    ///VERIFICANDO SE A ARESTA BOA JA EXISTE, SE EXISTIR NÃO CRIA OUTRA
                    bool exist = false;
                    for(Data::FrontElementList::iterator iter = v2->feBegin(); iter != v2->feEnd(); ++iter)
                    {
                        if( (*iter)->have( v3 ) )
                        {
                            //std::cout << "ARESTA BOA JÁ EXISTE " << v2->getId() << " com " << v3->getId() << std::endl;
                            exist = true;
                            break;
                        }
                    }

                    para = true;

                    if(exist) break;

                    MJMesh::FrontElement2D *e = ((MJMesh::FrontElement2D*)(surface_elements.back()));
                    MJMesh::FrontElementSurface *edge = new MJMesh::FrontElementSurface( v2, v3, e->getId() + 1 );
                    surface_elements.push_back( edge );

                    break;
                }
            }

            if(para)
                break;
        }
    }

}

void PMJMesh2::APrioriParallelMeshGenerator::removeOddEdges( Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, Data::FrontElementHash &wrongElements)
{
    UInt oldNumElements = 0;

    bool repeat = true;
    while( repeat )
    {
        repeat = false;

        if (surface_elements.size() == oldNumElements)
        {
            break;
        }

        wrongElements.clear();

        oldNumElements = surface_elements.size();

        for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); )
        {
            MJMesh::FrontElement2D* ele = ((MJMesh::FrontElement2D*)(*it));
            MJMesh::Vertex *v1 = ele->getV1();
            MJMesh::Vertex *v2 = ele->getV2();

            ///TIREI ISSO - CASO DE ARESTAS DUPLICADAS E SOLTAS
            //if( v1->getAdjacentFrontElements().size() == 2 && v2->getAdjacentFrontElements().size() == 2 )
            //{
            //it++;
            //continue;
            //}

            ///ARESTAS SOLTAS
            if( v1->numAdjacentFrontElements() < 2 || v2->numAdjacentFrontElements() < 2 )
            {
                if( v1->numAdjacentFrontElements() < 2 )
                {
                    surface_vertexs.remove( v1 );
                    delete ele;
                    delete v1;

                    ///PONTO PODE FICAR SOLTO
                    if( v2->numAdjacentFrontElements() == 0 )
                    {
                        surface_vertexs.remove( v2 );
                        delete v2;
                    }
                }
                else
                {
                    surface_vertexs.remove( v2 );
                    delete ele;
                    delete v2;

                    ///PONTO PODE FICAR SOLTO
                    if( v1->numAdjacentFrontElements() == 0 )
                    {
                        surface_vertexs.remove( v1 );
                        delete v1;
                    }
                }

                it = surface_elements.erase( it );
                repeat = true;

                continue;
            }

            if( v1->numAdjacentFrontElements() == 2 && v2->numAdjacentFrontElements() > 2 &&
                    ((MJMesh::FrontElement2D*)v1->getAdjacentFrontElements().front())->getV1()->getId() == ((MJMesh::FrontElement2D*)v1->getAdjacentFrontElements().back())->getV2()->getId() &&
                    ((MJMesh::FrontElement2D*)v1->getAdjacentFrontElements().front())->getV2()->getId() == ((MJMesh::FrontElement2D*)v1->getAdjacentFrontElements().back())->getV1()->getId() )
            {
                it = surface_elements.erase( it );
                delete ele;
                repeat = true;
                continue;
            }

            if( v2->numAdjacentFrontElements() == 2 && v1->numAdjacentFrontElements() > 2 &&
                    ((MJMesh::FrontElement2D*)v2->getAdjacentFrontElements().front())->getV1()->getId() == ((MJMesh::FrontElement2D*)v2->getAdjacentFrontElements().back())->getV2()->getId() &&
                    ((MJMesh::FrontElement2D*)v2->getAdjacentFrontElements().front())->getV2()->getId() == ((MJMesh::FrontElement2D*)v2->getAdjacentFrontElements().back())->getV1()->getId() )
            {
                it = surface_elements.erase( it );
                delete ele;
                repeat = true;
                continue;
            }

            /*if( v1->numAdjacentFrontElements() < 2 || v2->numAdjacentFrontElements() < 2 )
            {
                it++;

                continue;
            }*/

            ///ARESTAS DUPLICADAS
            Data::FrontElementList list_adj = v1->getAdjacentFrontElements();
            //for (std::list<Data::FrontElement *>::iterator ite=surface_elements.begin(); ite!=surface_elements.end(); ite++ )
            for (std::list<Data::FrontElement *>::iterator ite=list_adj.begin(); ite!=list_adj.end(); ite++ )
            {
                if( (*ite)->getId() != ele->getId()
                        && ( ((MJMesh::FrontElement2D*)(*ite))->getV1()->getId() == v1->getId() || ((MJMesh::FrontElement2D*)(*ite))->getV1()->getId() == v2->getId() )
                        && ( ((MJMesh::FrontElement2D*)(*ite))->getV2()->getId() == v1->getId() || ((MJMesh::FrontElement2D*)(*ite))->getV2()->getId() == v2->getId() ) )
                {
                    //std::cout << "VAI REMOVER: " << (*it)->getId() << std::endl;
                    it = surface_elements.erase( it );
                    delete ele;
                    ele = NULL;
                    repeat = true;
                    break;
                }
            }

            if( !ele )
            {
                continue;
            }

            it++;
            repeat = true;

            if (surface_elements.size() == oldNumElements)
            {
                wrongElements.insert(ele);
            }
        }
    }
}

void PMJMesh2::APrioriParallelMeshGenerator::fixBoundaryLoops( PMJMesh::Front *sub_front, Data::VertexHashMap &original_surface_vertexes, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements, Data::Triangle3D &box_plane, Data::FrontElementHash &wrongElements )
{

    //static int tert = 1;
    //std::cout << "MELHORIA INICIAL FEITA " << tert++ << std::endl;

    bool para = true;
    MJMesh::Vertex *v = NULL;

    while( para )
    {
        para = false;

        removeOddEdges(surface_vertexs, surface_elements, wrongElements);

        improveBoundaryLoop(sub_front, original_surface_vertexes, surface_vertexs, surface_elements);

        //std::cout << "RECOMEÇOU !!!!" << std::endl;

        ///ARQUIVO GEN
        {
            /*static int ert = 1;
            std::cout << "TENTATIVA: " << ert << std::endl;
            std::string as = "/home/nfs/daniel/Desktop/DELETAR/eliminandoloops";
            as += std::to_string(ert++);
            as += ".gen";
            std::ofstream myfile ( as );


            //std::ofstream myfile ("/home/nfs/daniel/Desktop/eliminandoloops.gen");
            if (myfile.is_open())
            {
                myfile << "2.5" << std::endl ;
                myfile << surface_vertexs.size() << std::endl ;
                for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
                {
                    myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile << surface_elements.size() << std::endl ;
                for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
                {
                    myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                           << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
                }
                myfile << "0" << std::endl;
            }
            myfile.close();*/
        }


        UInt minAdj = UINT_MAX;
        bool vertex_finded = false;
        MJMesh::Vertex *v_temp = NULL;
        for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
        {
            UInt adj = static_cast<MJMesh::Vertex *>((*it))->numAdjacentFrontElements();

            if( adj  > 2 && adj < minAdj && (*it) != v )
            {
                vertex_finded = true;
                v_temp = static_cast<MJMesh::Vertex *>((*it));
                minAdj = adj;

                if( adj == 3  )
                    break;
            }
        }

        if (!v_temp || !vertex_finded)
        {
            break;
        }

        v = v_temp;

        if( v->numAdjacentFrontElements() > 2 )
        {
            //std::cout << "MAIS QUE 2 VIZINHOS " << v->getId() << " SIZE:  " << v->numAdjacentFrontElements() << std::endl;

            std::vector<MJMesh::Vertex *> vertexs;
            Real max_ang = 0.0;
            MJMesh::FrontElementSurface *max_ele = NULL;

            ///SELECIONANDO OS VERTICES VIZINHOS DO VERTICE RUIM
            for (Data::FrontElementList::iterator iter = v->feBegin(); iter != v->feEnd(); iter++)
            {
                if( v->getId() == static_cast<const MJMesh::FrontElement *>((*iter))->getV1()->getId() )
                    vertexs.push_back( static_cast<const MJMesh::FrontElement *>((*iter))->getV2() );
                else
                    vertexs.push_back( static_cast<const MJMesh::FrontElement *>((*iter))->getV1() );
            }

            //std::cout << "V:  " << v->getId() << " E: " << v->getAdjacentElements().size() << " FE: " << v->numAdjacentFrontElements() << std::endl;

            ///SELECINANDO A FACE DE MAIOR NUMERO DE CONTATOS COM O ANEL DE ARESTAS
            MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>( original_surface_vertexes[ v->getId() ] );
            int maxcontatos = 0;
            MJMesh::FrontElement3D *face = NULL;
            MJMesh::Vertex *v2 = NULL;
            MJMesh::Vertex *v3 = NULL;
            for(Data::FrontElementList::iterator itt = v_ori->feBegin(); itt != v_ori->feEnd(); ++itt)
            {
                MJMesh::FrontElement3D *tmpface = static_cast<MJMesh::FrontElement3D *>( (*itt) );
                MJMesh::Vertex *tmpv2 = NULL;
                MJMesh::Vertex *tmpv3 = NULL;

                if( !sub_front->have( tmpface, 3 ) )
                    continue;

                int contatos = 0;


                ///CONTANDO QUANTOS VERTICES SÂO ADJ A CADA FACE - USA V, V2 e V3
                for (UInt i = 0; i < vertexs.size(); i++)
                {
                    if( tmpface->have( original_surface_vertexes[ vertexs[i]->getId() ] ) )
                    {
                        //std::cout << "FACE " << tmpface->getId() << " tem vertice " << vertexs[i]->getId() << std::endl;
                        contatos++;
                        if(tmpv2 == NULL)
                            tmpv2 = vertexs[i];
                        else
                            tmpv3 = vertexs[i];
                    }
                }
                if(contatos > maxcontatos )
                {
                    maxcontatos = contatos;
                    face = tmpface;
                    v2 = tmpv2;
                    v3 = tmpv3;
                    continue;
                }
                if(contatos == maxcontatos && maxcontatos != 0)
                {
                    Real d1 = box_plane.distance( tmpface->getShape()->mid3D() );
                    Real d2 = box_plane.distance( face->getShape()->mid3D() );

                    if( std::fabs(d1) > std::fabs(d2) )
                    {
                        maxcontatos = contatos;
                        face = tmpface;
                        v2 = tmpv2;
                        v3 = tmpv3;
                    }
                }
            }

            ///TROCA 2 OU 1 ARESTAS POR 1 MELHOR
            if (!face)
            {
                continue;
            }

            //std::cout << "FACE " << face->text() << " tem contatos " << maxcontatos << std::endl;

            if( maxcontatos == 1 )
            {
                //std::cout << "           V:  " << v->getId() << " E: " << v->getAdjacentElements().size() << " FE: " << v->numAdjacentFrontElements() << std::endl;

                ///RECUPERANDO O VERTICE V3
                for (UInt i = 0; i < 3; i++)
                {
                    if( face->getVertex( i )->getId() != v->getId() && face->getVertex( i )->getId() != v2->getId() )
                    {
                        if(v3 == NULL)
                        {
                            v3 = ( (MJMesh::Vertex*) face->getVertex( i ) );
                            break;
                        }
                    }
                }

                ///PROCURANDO ARESTA PARA DELETAR
                for (Data::FrontElementList::iterator iter = v->feBegin(); iter != v->feEnd(); iter++)
                {
                    MJMesh::FrontElement2D* aresta = ((MJMesh::FrontElement2D*)(*iter));
                    MJMesh::Vertex *v_temp = static_cast<MJMesh::FrontElement *>((*iter))->getV1();
                    MJMesh::Vertex *v_temp2 = static_cast<MJMesh::FrontElement *>((*iter))->getV2();

                    if( v2->getId() == v_temp->getId() || v2->getId() == v_temp2->getId() )
                    {
                        surface_elements.remove( aresta );

                        delete aresta;
                        break;
                    }
                }

                ///PROCURANDO O VERTICE V3 DA BORDA
                for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
                {
                    v_temp = static_cast<MJMesh::Vertex *>((*it));
                    if( v_temp->getId() == v3->getId() )
                    {
                        v3 = v_temp;
                        break;
                    }
                }


                int x = surface_elements.back()->getId();
                MJMesh::FrontElementSurface *edge = new MJMesh::FrontElementSurface( v, v3, ++x );
                surface_elements.push_back( edge );
                MJMesh::FrontElementSurface *edge2 = new MJMesh::FrontElementSurface( v2, v3, ++x );
                surface_elements.push_back( edge2 );

                para = true;
                continue;
            }

            ///contar quantidade de vertices adjs
            ///se for 1 = verifica se o outro vertice está na lista de vertices ja adicionados, se n for ignorar
            ///se for 2 = temos pelo menos 2 arestas de uma face incluida, deletar 1 para ficar so com 1
            if( maxcontatos >= 1 )
            {
                //std::cout << "FACE text: " << face->text() << std::endl;

                Data::Vector3D vec[3];
                vec[0] = Data::Vector3D( face->getV1()->getPoint(), face->getV2()->getPoint() );
                vec[1] = Data::Vector3D( face->getV2()->getPoint(), ((MJMesh::FrontElement3D *)face)->getV3()->getPoint() );
                vec[2] = Data::Vector3D( ((MJMesh::FrontElement3D *)face)->getV3()->getPoint(), face->getV1()->getPoint() );

                Data::Vector3D normal = box_plane.normal();

                Real cos[3];
                cos[0] = normal.cosAngle(vec[0]);
                cos[1] = normal.cosAngle(vec[1]);
                cos[2] = normal.cosAngle(vec[2]);

                for(int p=0; p<3; p++)
                {
                    //std::cout << "cos " << p << ": " << cos[p] << std::endl;
                    if( std::fabs(cos[p]) <= std::fabs(cos[(p+1)%3]) && std::fabs(cos[p]) <= std::fabs(cos[(p+2)%3]) )
                    {
                        //std::cout << "Melhor " << p << ": " << cos[p] << std::endl;

                        MJMesh::Vertex *v_tests[3];

                        /// 1º VERTEX NA ARESTA BOA
                        v_tests[0] = ( (MJMesh::Vertex*) face->getVertex( p ) );

                        /// 2º VERTEX NA ARESTA BOA
                        v_tests[1] = ( (MJMesh::Vertex*) face->getVertex( (p+1) % 3 ) );

                        ///VERTEX QUE NÃO ESTÀ NA ARESTA BOA
                        v_tests[2] = ( (MJMesh::Vertex*) face->getVertex( (p+2) % 3 ) );


                        if( original_surface_vertexes.find( v_tests[0]->getId() ) == original_surface_vertexes.end()
                                || original_surface_vertexes.find( v_tests[1]->getId() ) == original_surface_vertexes.end() )
                        {
                            continue;
                        }

                        ///ENCONTRANDO OS VERTEX QUE ESTÃO NO ANEL DE ARESTAS E ATUALIZANDO v_tests
                        for (UInt j = 0; j < 3; j++)
                        {
                            ///V NÃO ESTÀ NA SUA PROPRIA LISTA DOS SEUS ADJACENTES
                            if( v_tests[j]->getId() == v->getId() )
                            {
                                //if (v_tests[j] == v) std::cout << "IGUAIS 1" << std::endl;

                                v_tests[j] = v;
                                continue;
                            }

                            for (UInt i = 0; i < vertexs.size(); i++)
                            {
                                if( v_tests[j]->getId() == vertexs[i]->getId() )
                                {
                                    //if (v_tests[j] == vertexs[i]) std::cout << "IGUAIS 2" << std::endl;

                                    v_tests[j] = vertexs[i];
                                    break;
                                }

                                bool found = false;

                                ///VERIFICANDO SE PODE ESTAR NOS ADJ DOS ADJ
                                for (Data::FrontElementList::iterator iter = vertexs[i]->feBegin(); iter != vertexs[i]->feEnd(); iter++)
                                {
                                    if( v_tests[j]->getId() == static_cast<const MJMesh::FrontElement *>((*iter))->getV1()->getId() )
                                    {
                                        //if (v_tests[j] == static_cast<const MJMesh::FrontElement *>((*iter))->getV1()) std::cout << "IGUAIS 3" << std::endl;

                                        v_tests[j] = static_cast<const MJMesh::FrontElement *>((*iter))->getV1();
                                        found = true;
                                        break;
                                    }

                                    if( v_tests[j]->getId() == static_cast<const MJMesh::FrontElement *>((*iter))->getV2()->getId() )
                                    {
                                        //if (v_tests[j] == static_cast<const MJMesh::FrontElement *>((*iter))->getV2()) std::cout << "IGUAIS 4" << std::endl;

                                        v_tests[j] = static_cast<const MJMesh::FrontElement *>((*iter))->getV2();
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

                        if( v_tests[0] == original_surface_vertexes[ v_tests[0]->getId() ]
                                || v_tests[1] == original_surface_vertexes[ v_tests[1]->getId() ] )
                            //|| v_tests[2] == original_surface_vertexes[ v_tests[2]->getId() ] )
                        {
                            //std::cout << "Vértice não existe mais... não pode criar a melhor aresta" << std::endl;
                            continue;
                        }


                        //std::cout << "END V1:  " << v_tests[1] << " END ORI V1: " << original_surface_vertexes[ v_tests[1]->getId() ]  << std::endl;
                        //std::cout << "ORI V1:  " << original_surface_vertexes[ v_tests[1]->getId() ]->text()  << std::endl;

                        //std::cout << "V0:  " << v_tests[0]->getId() << " E: " << v_tests[0]->getAdjacentElements().size() << " FE: " << v_tests[0]->numAdjacentFrontElements() << std::endl;
                        //std::cout << "V1:  " << v_tests[1]->getId() << " E: " << v_tests[1]->getAdjacentElements().size() << " FE: " << v_tests[1]->numAdjacentFrontElements() << std::endl;
                        //std::cout << "V2:  " << v_tests[2]->getId() << " E: " << v_tests[2]->getAdjacentElements().size() << " FE: " << v_tests[2]->numAdjacentFrontElements() << std::endl;


                        Data::FrontElementList list_adj = v_tests[0]->getAdjacentFrontElements();
                        list_adj.insert(list_adj.end(), v_tests[1]->getAdjacentFrontElements().begin(), v_tests[1]->getAdjacentFrontElements().end());

                        //std::cout << "LIST ADJ: " << list_adj.size() << std::endl;

                        //if( v_tests[2] != original_surface_vertexes[ v_tests[2]->getId() ] )
                        if( original_surface_vertexes.find( v_tests[2]->getId() ) != original_surface_vertexes.end() )
                        {
                            if( v_tests[2] != original_surface_vertexes[v_tests[2]->getId()] )
                                list_adj.insert(list_adj.end(), v_tests[2]->getAdjacentFrontElements().begin(), v_tests[2]->getAdjacentFrontElements().end());
                        }

                        //std::cout << "TESTE: " << list_adj.size() << std::endl;


                        ///PROCURA E DELETA 1 OU 2 ARESTAS QUE SÃO RUINS - PODE SER QUE PRECISE ADD A ARESTA BOA
                        bool exist = false;
                        //for (std::list<Data::FrontElement *>::iterator itt2=surface_elements.begin(); itt2!=surface_elements.end(); )
                        for (std::list<Data::FrontElement *>::iterator itt2=list_adj.begin(); itt2!=list_adj.end(); )
                        {
                            MJMesh::FrontElement2D* aresta = ((MJMesh::FrontElement2D*)(*itt2));

                            ///PROCURANDO ARESTA BOA PARA ADICIONAR CASO NÂO EXISTA
                            if( aresta->have(v_tests[0]) && aresta->have(v_tests[1]) )
                            {
                                //std::cout << "Achou MELHOR ARESTA: " << v_tests[0]->getId() << " e " << v_tests[1]->getId() << std::endl;
                                //itt2++;
                                list_adj.remove(aresta);
                                itt2=list_adj.begin();
                                exist = true;
                                continue;
                            }
                            ///PROCURANDO ARESTA 1 RUIM PARA DELETAR CASO EXISTA
                            if( aresta->have(v_tests[0]) && aresta->have(v_tests[2]) )
                            {
                                //std::cout << "Achou 1 - DEL: " << v_tests[0]->getId() << " e " << v_tests[2]->getId() << std::endl;
                                //itt2 = surface_elements.erase( itt2 );
                                surface_elements.remove( aresta );
                                list_adj.remove(aresta);
                                itt2=list_adj.begin();

                                delete aresta;
                                para = true;
                                continue;
                            }
                            ///PROCURANDO ARESTA 2 RUIM PARA DELETAR CASO EXISTA
                            if( aresta->have(v_tests[1]) && aresta->have(v_tests[2]) )
                            {
                                //std::cout << "Achou 2 - DEL: " << v_tests[1]->getId() << " e " << v_tests[2]->getId() << std::endl;
                                //itt2 = surface_elements.erase( itt2 );
                                surface_elements.remove( aresta );
                                list_adj.remove(aresta);
                                itt2=list_adj.begin();

                                delete aresta;
                                para = true;
                                continue;
                            }

                            //itt2++;
                            itt2 = list_adj.erase( itt2 );
                        }

                        if( v_tests[2]->numAdjacentFrontElements() == 0 )
                        {
                            surface_vertexs.remove( v_tests[2] );
                            delete v_tests[2];
                        }

                        if( exist == false )
                        {
                            //std::cout << "CRIANDO ARESTA BOA: " << v_tests[0]->getId() << " e " << v_tests[1]->getId() << std::endl;

                            int x = surface_elements.back()->getId();
                            MJMesh::FrontElementSurface *edge = new MJMesh::FrontElementSurface( v_tests[0], v_tests[1], ++x );
                            surface_elements.push_back( edge );

                            /*bool find1 = false;
                            bool find2 = false;
                            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
                            {
                                if( (*it)->getId() == v_tests[0]->getId() )
                                {
                                    find1 = true;
                                }
                                if( (*it)->getId() == v_tests[1]->getId() )
                                {
                                    find2 = true;
                                }
                            }
                            if( !find1  )
                            {
                                surface_vertexs.push_back( v_tests[0] );
                                std::cout << "CRIOU: " << v_tests[0]->text() << std::endl;
                            }
                            if( !find2  )
                            {
                                surface_vertexs.push_back( v_tests[1] );
                                std::cout << "CRIOU: " << v_tests[1]->text() << std::endl;
                            }*/

                            para = true;
                        }

                        break;
                    }
                }
                if( para )
                    continue;
            }

            ///TODAS AS ARESTAS DE UMA FACE INCLUIDAS
            ///EXISTE UMA FACE NA LISTA. DELETAR AS DUAS PIORES ARESTAS PELO ANGULO COM O PLANO DE CORTE
            if( v2 && v3 && false )
            {
                ///VERIFICANDO SE REALMENTE EXISTE TODAS AS ARESTAS DE UMA FACE INCLUSA
                bool loop = false;
                for (Data::FrontElementList::iterator iter = v2->feBegin(); iter != v2->feEnd(); iter++)
                {
                    if( static_cast<const MJMesh::FrontElement *>((*iter))->have( v3 ) )
                    {
                        loop = true;
                        break;
                    }
                }

                if(loop)
                {
                    std::cout << "TRIANGULO INCLUSO: " << v->getId() << "  " << v2->getId() << "  " << v3->getId() << std::endl;

                    para = true;

                    Data::Vector3D vec1( v->getPoint(), v2->getPoint() );
                    Data::Vector3D vec2( v2->getPoint(), v3->getPoint() );
                    Data::Vector3D vec3( v->getPoint(), v3->getPoint() );

                    Data::Vector3D normal = box_plane.normal();

                    Real cos1 = normal.cosAngle(vec1);
                    Real cos2 = normal.cosAngle(vec2);
                    Real cos3 = normal.cosAngle(vec3);


                    ///UM LOOP NOS 3 VERTICES - SELECIONAR PELO DISTANCIA AO PLANO DE PARTIÇÃO
                    if( v2->numAdjacentFrontElements() >= 3 && v3->numAdjacentFrontElements() >= 3  )
                    {
                        MJMesh::Vertex *v_temp = NULL;

                        Real dists[4];
                        dists[0] = std::fabs(box_plane.signedPlaneDistance( v->getPoint() ) );
                        dists[1] = std::fabs(box_plane.signedPlaneDistance( v2->getPoint() ) );
                        dists[2] = std::fabs(box_plane.signedPlaneDistance( v3->getPoint() ) );
                        dists[3] = 0.0;


                        for( int i = 0; i < 3; i++ )
                        {
                            if( dists[3] <= dists[i] )
                            {
                                dists[3] = dists[i];

                                if( i == 0 )
                                    v_temp  = v;
                                if( i == 1 )
                                    v_temp  = v2;
                                if( i == 2 )
                                    v_temp  = v3;
                            }
                        }
                        std::cout << "Dist V: " << dists[0] << std::endl;
                        std::cout << "Dist V2: " << dists[1] << std::endl;
                        std::cout << "Dist V3: " << dists[2] << std::endl;
                        std::cout << "Max Vert dist: " << v_temp->getId() << std::endl;

                        for (Data::FrontElementList::iterator ite = v_temp->feBegin(); ite != v_temp->feEnd();)
                        {
                            MJMesh::FrontElement2D* ele = ((MJMesh::FrontElement2D*)(*ite));
                            surface_elements.remove( ele );
                            delete (ele);
                            ite = v_temp->feBegin();
                        }
                        if( v_temp->numAdjacentFrontElements() == 0 )
                        {
                            surface_vertexs.remove( v_temp );
                            delete v_temp;
                        }

                        continue;
                    }

                    ///DELETA V E SUAS ARESTAS ADJACENTES
                    if( std::fabs(cos2) <= std::fabs(cos1) && std::fabs(cos2) <= std::fabs(cos3) )
                    {
                        for (Data::FrontElementList::iterator ite = v->feBegin(); ite != v->feEnd();)
                        {
                            MJMesh::FrontElement2D* ele = ((MJMesh::FrontElement2D*)(*ite));
                            ite++;

                            if( (ele->have(v2) && ele->have(v)) || (ele->have(v3) && ele->have(v)) )
                            {
                                if( std::fabs(cos1) <= std::fabs(cos3) )
                                {
                                    if( (ele->have(v2) && ele->have(v)) )
                                        continue;
                                }
                                else
                                {
                                    if( (ele->have(v3) && ele->have(v)) )
                                        continue;
                                }

                                std::cout << "DEL 1: " << ele->getId() << std::endl;

                                surface_elements.remove( ele );
                                delete (ele);
                                ite = v->feBegin();
                            }
                        }
                        if( v->numAdjacentFrontElements() == 0 )
                        {
                            surface_vertexs.remove( v );
                            delete v;
                        }
                        continue;
                    }

                    ///DELETA V2 E SUAS ARESTAS ADJACENTES
                    if( std::fabs(cos3) <= std::fabs(cos1) && std::fabs(cos3) <= std::fabs(cos2) )
                    {
                        for (Data::FrontElementList::iterator ite = v2->feBegin(); ite != v2->feEnd(); )
                        {
                            MJMesh::FrontElement2D* ele = ((MJMesh::FrontElement2D*)(*ite));
                            ite++;

                            if( (ele->have(v2) && ele->have(v)) || (ele->have(v3) && ele->have(v2)) )
                            {
                                if( std::fabs(cos1) <= std::fabs(cos2) )
                                {
                                    if( (ele->have(v2) && ele->have(v)) )
                                        continue;
                                }
                                else
                                {
                                    if( (ele->have(v2) && ele->have(v3)) )
                                        continue;
                                }

                                std::cout << "DEL 2: " << ele->getId() << std::endl;

                                surface_elements.remove( ele );
                                delete (ele);
                                ite = v2->feBegin();
                            }
                        }
                        if( v2->numAdjacentFrontElements() == 0 )
                        {
                            surface_vertexs.remove( v2 );
                            delete v2;
                        }
                        continue;
                    }

                    ///DELETA V3 E SUAS ARESTAS ADJACENTES
                    if( std::fabs(cos1) <= std::fabs(cos2) && std::fabs(cos1) <= std::fabs(cos3) )
                    {
                        for (Data::FrontElementList::iterator ite = v3->feBegin(); ite != v3->feEnd(); )
                        {
                            MJMesh::FrontElement2D* ele = ((MJMesh::FrontElement2D*)(*ite));
                            ite++;

                            if( (ele->have(v2) && ele->have(v3)) || (ele->have(v3) && ele->have(v)) )
                            {
                                if( std::fabs(cos2) <= std::fabs(cos3) )
                                {
                                    if( (ele->have(v2) && ele->have(v3)) )
                                        continue;
                                }
                                else
                                {
                                    if( (ele->have(v) && ele->have(v3)) )
                                        continue;
                                }

                                std::cout << "DEL 3: " << ele->getId() << std::endl;

                                surface_elements.remove( ele );
                                delete (ele);
                                ite = v3->feBegin();
                            }
                        }
                        if( v3->numAdjacentFrontElements() == 0 )
                        {
                            surface_vertexs.remove( v3 );
                            delete v3;
                        }
                        continue;
                    }
                }
            }


            if( v->numAdjacentFrontElements() > 2 )
            {
                std::cout << "CHEGOU NO FINAL E NADA... " << std::endl;
                para = true;
                return;
            }
        }
    }



    ///ARQUIVO GEN
    {
        /*static int ert = 1;
        std::string as = "/home/nfs/daniel/Desktop/validarloops";
        as += std::to_string(ert++);
        as += ".gen";
        std::ofstream myfile ( as );


        //std::ofstream myfile ("/home/nfs/daniel/Desktop/eliminandoloops.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_vertexs.size() << std::endl ;
            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                       << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();*/
    }



    ///VERIFICANDO LOOP - SÓ FUNCIONA PARA MODELOS QUE NÃO TENHO BURACOS  OU SUB-REGIÕES
    if( true )
    {
        //std::cout << "VERIFICANDO LOOP" << std::endl;
        surface_vertexs.sort(sort_vertex);
        MJMesh::Vertex *v_front = (MJMesh::Vertex *)(surface_vertexs.front());
        MJMesh::Vertex *v1 = v_front;
        MJMesh::Vertex *v_prev = v1;

        MJMesh::FrontElement2D* aresta = (MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().front());
        bool continua = true;
        int qtd = 0;
        while( continua )
        {
            MJMesh::Vertex *v2 = NULL;

            do
            {
                if( aresta->getV1() == v1 )
                {
                    if( aresta->getV2() == v_prev )
                    {
                        aresta = (MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().back());
                        continue;
                    }

                    v2 = aresta->getV2();
                }
                else
                {
                    if( aresta->getV1() == v_prev )
                    {
                        aresta = (MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().back());
                        continue;
                    }

                    v2 = aresta->getV1();
                }
            }
            while( v2 == NULL );

            qtd++;

            if( v2 == v_front )
                continua = false;

            v_prev = v1;
            v1 = v2;
            aresta = (MJMesh::FrontElement2D*)(v1->getAdjacentFrontElements().front());
        }

        if( qtd != surface_vertexs.size() )
        {
            //std::cout << "QUANTIDADE ERRADA : " << qtd << " com " << surface_vertexs.size() << std::endl;

            aresta = (MJMesh::FrontElement2D*)(v_front->getAdjacentFrontElements().front());
            surface_elements.remove( aresta );
            aresta->clearAdjacency();
            delete aresta;

            removeOddEdges(surface_vertexs, surface_elements, wrongElements);
        }
    }

    //std::cout << "MELHRIA FINAL FEITA" << std::endl;
}

void PMJMesh2::APrioriParallelMeshGenerator::createSurfaceBoundary(
    PMJMesh::Front *sub_front, Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements,
    Data::VertexHashMap &original_surface_vertexes, Data::Triangle3D &box_plane, UInt side)
{
    struct Test
    {
        static bool in(Real dist, Real tol)
        {
            return (dist > tol);
        };
        static bool on(Real dist, Real tol)
        {
            return (dist >= -tol) && (dist <= tol);
        };
        static bool out(Real dist, Real tol)
        {
            return (dist < -tol);
        };
    };

    if (sub_front->size(3) == 0)
    {
        sub_front->moveRejectedToCrossing();
    }

    //std::cout << "sizes of sub_front before createSurfaceBoundary: " << sub_front->size(0) << ", " << sub_front->size(1) << ", " << sub_front->size(2) << ", " << sub_front->size(3) << std::endl;


    Data::FrontElementList cross_faces = sub_front->getCrossing();
    cross_faces.sort( sort_frontElementId );


    ///CRIANDO ARQUIVO DAS FACES CROSS ANTES
    {
        /*std::ofstream myfile ("/home/nfs/daniel/Desktop/saida_cross_vai_comecar.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile <<  cross_faces.size() * 3 << std::endl;
            int o = 1;
            for(std::list<Data::FrontElement *>::iterator it = cross_faces.begin(); it != cross_faces.end(); ++it)
            {
                myfile << o++ << " "  << (*it)->getVertex(0)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(0)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(0)->getPoint()->getCoord(2) << std::endl;
                myfile << o++ << " "  << (*it)->getVertex(1)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(1)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(1)->getPoint()->getCoord(2) << std::endl;
                myfile << o++ << " "  << (*it)->getVertex(2)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(2)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(2)->getPoint()->getCoord(2) << std::endl;
            }
            myfile <<  0 << std::endl;
            myfile << cross_faces.size() << std::endl;
            o=1;
            for(std::list<Data::FrontElement *>::iterator it = cross_faces.begin(); it != cross_faces.end(); ++it)
            {
                myfile << (*it)->getId() << " 3 " << o++ << " " << o++ << " " << o++ << std::endl;
            }
        }
        myfile.close();*/
    }


    ///SELEÇÃO DOS VÉRTICES/ARESTAS DA SUPERFICIE
    //for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    for(std::list<Data::FrontElement *>::iterator it = cross_faces.begin(); it != cross_faces.end(); ++it)
    {
        MJMesh::FrontElement3D *face = static_cast<MJMesh::FrontElement3D *>( (*it) );
        //std::cout << face->getId() << std::endl ;
        //std::cout << face->getV1()->text() << std::endl ;
        //std::cout << face->getV2()->text() << std::endl ;
        //std::cout << face->getV3()->text() << std::endl ;


        bool f1 = false;
        bool f2 = false;
        bool f3 = false;

        Real dists[3];
        dists[0] = box_plane.signedPlaneDistance( face->getVertex(0)->getPoint() );
        dists[1] = box_plane.signedPlaneDistance( face->getVertex(1)->getPoint() );
        dists[2] = box_plane.signedPlaneDistance( face->getVertex(2)->getPoint() );


        int num = 0;

        //if( newBox.in( (*it)->getVertex(0)->getPoint() ) || newBox.on( (*it)->getVertex(0)->getPoint() ) ) {
        if( Test::in(dists[0], Data::getTolerance()) || Test::on(dists[0], Data::getTolerance()) )
        {
            f1 = true;
            num++;
        }

        //if( newBox.in( (*it)->getVertex(1)->getPoint() ) || newBox.on( (*it)->getVertex(1)->getPoint() ) ) {
        if( Test::in(dists[1], Data::getTolerance()) || Test::on(dists[1], Data::getTolerance()) )
        {
            f2 = true;
            num++;
        }

        //if( newBox.in( (*it)->getVertex(2)->getPoint() ) || newBox.on( (*it)->getVertex(2)->getPoint() ) ) {
        if( Test::in(dists[2], Data::getTolerance()) || Test::on(dists[2], Data::getTolerance()) )
        {
            f3 = true;
            num++;
        }


        //std::cout << num << "   F: " << f1 << " " << f2 << " " << f3 << std::endl;


        ///TRIANGULO SENDO CORTADO AO MEIO
        if( num == 2 || num == 1 )
        {
            /*for( int i=0; i<3; i++ )
            {
                if( std::fabs(dists[i]) < std::fabs(dists[ (i+1)%3 ]) && std::fabs(dists[i]) < std::fabs(dists[ (i+2)%3 ]) )
                {
                    if( std::fabs(dists[ (i+1)%3 ]) < std::fabs(dists[ (i+2)%3 ])  )
                    {
                        surface_vertexs.push_back( (*it)->getVertex( i ) );
                        surface_vertexs.push_back( (*it)->getVertex( (i+1)%3 ) );
                    }else
                    {
                        surface_vertexs.push_back( (*it)->getVertex( i ) );
                        surface_vertexs.push_back( (*it)->getVertex( (i+2)%3 ) );
                    }
                }
            }*/



            if( Test::in(dists[0], Data::getTolerance()) || Test::out(dists[0], Data::getTolerance()) )
            {
                if( (Test::out(dists[1], Data::getTolerance()) && Test::out(dists[2], Data::getTolerance()))
                        || (Test::in(dists[1], Data::getTolerance()) && Test::in(dists[2], Data::getTolerance())) )
                {
                    Data::Vector3D vec1( face->getVertex( 2 )->getPoint(), face->getVertex( 1 )->getPoint() );
                    Data::Vector3D vec2( face->getVertex( 0 )->getPoint(), face->getVertex( 1 )->getPoint() );
                    Data::Vector3D vec3( face->getVertex( 2 )->getPoint(), face->getVertex( 0 )->getPoint() );
                    Data::Vector3D normal = box_plane.normal();

                    Real cos1 = std::fabs(normal.cosAngle(vec1));
                    Real cos2 = std::fabs(normal.cosAngle(vec2));
                    Real cos3 = std::fabs(normal.cosAngle(vec3));

                    ///SELECIONANDO A MELHOR ARESTA PELO ANGULO
                    if( cos1 < cos2 && cos1 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(2) );
                        surface_vertexs.push_back( face->getVertex(1) );
                        //continue;
                    }
                    else if( cos2 < cos1 && cos2 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(0) );
                        surface_vertexs.push_back( face->getVertex(1) );
                        //continue;
                    }
                    else if( cos3 < cos1 && cos3 < cos2 )
                    {
                        surface_vertexs.push_back( face->getVertex(2) );
                        surface_vertexs.push_back( face->getVertex(0) );
                        //continue;
                    }


                    ///SELECIONANDO A MELHOR ARESTA QUE CORTA A PARTIÇÃO
                    if( std::fabs(dists[0]) + Data::getTolerance() > std::fabs(dists[1]) && std::fabs(dists[0]) + Data::getTolerance() > std::fabs(dists[2]) )
                    {
                        surface_vertexs.push_back( face->getVertex(2) );
                        surface_vertexs.push_back( face->getVertex(1) );
                        continue;
                    }
                    else
                    {
                        if( std::fabs(dists[1]) < std::fabs(dists[2]) + Data::getTolerance() )
                        {
                            surface_vertexs.push_back( face->getVertex(0) );
                            surface_vertexs.push_back( face->getVertex(1) );
                        }
                        else
                        {
                            surface_vertexs.push_back( face->getVertex(2) );
                            surface_vertexs.push_back( face->getVertex(0) );
                        }
                        continue;
                    }

                    /*surface_vertexs.push_back( (*it)->getVertex(2) );
                    surface_vertexs.push_back( (*it)->getVertex(0) );
                    //std::cout << ++pos << "  "  << (*it)->getVertex(2)->getPoint()->getCoord(0) << " " << (*it)->getVertex(2)->getPoint()->getCoord(1) << " " << (*it)->getVertex(2)->getPoint()->getCoord(2) << std::endl ;
                    //std::cout << ++pos << "  "  << (*it)->getVertex(0)->getPoint()->getCoord(0) << " " << (*it)->getVertex(0)->getPoint()->getCoord(1) << " " << (*it)->getVertex(0)->getPoint()->getCoord(2) << std::endl ;

                    surface_vertexs.push_back( (*it)->getVertex(0) );
                    surface_vertexs.push_back( (*it)->getVertex(1) );
                    //std::cout << ++pos << "  "  << (*it)->getVertex(0)->getPoint()->getCoord(0) << " " << (*it)->getVertex(0)->getPoint()->getCoord(1) << " " << (*it)->getVertex(0)->getPoint()->getCoord(2) << std::endl ;
                    //std::cout << ++pos << "  "  << (*it)->getVertex(1)->getPoint()->getCoord(0) << " " << (*it)->getVertex(1)->getPoint()->getCoord(1) << " " << (*it)->getVertex(1)->getPoint()->getCoord(2) << std::endl ;
                    continue;
                    }*/
                }
            }

            if( Test::in(dists[1], Data::getTolerance()) || Test::out(dists[1], Data::getTolerance()) )
            {
                if( (Test::out(dists[0], Data::getTolerance()) && Test::out(dists[2], Data::getTolerance()) )
                        || (Test::in(dists[0], Data::getTolerance()) && Test::in(dists[2], Data::getTolerance())) )
                {
                    Data::Vector3D vec1( face->getVertex( 0 )->getPoint(), face->getVertex( 2 )->getPoint() );
                    Data::Vector3D vec2( face->getVertex( 0 )->getPoint(), face->getVertex( 1 )->getPoint() );
                    Data::Vector3D vec3( face->getVertex( 1 )->getPoint(), face->getVertex( 2 )->getPoint() );
                    Data::Vector3D normal = box_plane.normal();

                    Real cos1 = std::fabs(normal.cosAngle(vec1));
                    Real cos2 = std::fabs(normal.cosAngle(vec2));
                    Real cos3 = std::fabs(normal.cosAngle(vec3));

                    ///SELECIONANDO A MELHOR ARESTA PELO ANGULO
                    if( cos1 < cos2 && cos1 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(0) );
                        surface_vertexs.push_back( face->getVertex(2) );
                        //continue;
                    }
                    else if( cos2 < cos1 && cos2 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(0) );
                        surface_vertexs.push_back( face->getVertex(1) );
                        //continue;
                    }
                    else if( cos3 < cos1 && cos3 < cos2 )
                    {
                        surface_vertexs.push_back( face->getVertex(1) );
                        surface_vertexs.push_back( face->getVertex(2) );
                        //continue;
                    }

                    ///SELECIONANDO A MELHOR ARESTA QUE CORTA A PARTIÇÃO
                    if( std::fabs(dists[1]) + Data::getTolerance() > std::fabs(dists[0]) && std::fabs(dists[1]) + Data::getTolerance() > std::fabs(dists[2]) )
                    {
                        surface_vertexs.push_back( face->getVertex(0) );
                        surface_vertexs.push_back( face->getVertex(2) );
                        continue;
                    }
                    else
                    {
                        if( std::fabs(dists[0]) < std::fabs(dists[2]) + Data::getTolerance() )
                        {
                            surface_vertexs.push_back( face->getVertex(0) );
                            surface_vertexs.push_back( face->getVertex(1) );
                        }
                        else
                        {
                            surface_vertexs.push_back( face->getVertex(1) );
                            surface_vertexs.push_back( face->getVertex(2) );
                        }
                        continue;
                    }
                }
            }

            if( Test::in(dists[2], Data::getTolerance()) || Test::out(dists[2], Data::getTolerance()) )
            {
                if( (Test::out(dists[0], Data::getTolerance()) && Test::out(dists[1], Data::getTolerance()))
                        || (Test::in(dists[0], Data::getTolerance()) && Test::in(dists[1], Data::getTolerance())) )
                {
                    Data::Vector3D vec1( face->getVertex( 1 )->getPoint(), face->getVertex( 0 )->getPoint() );
                    Data::Vector3D vec2( face->getVertex( 2 )->getPoint(), face->getVertex( 0 )->getPoint() );
                    Data::Vector3D vec3( face->getVertex( 1 )->getPoint(), face->getVertex( 2 )->getPoint() );
                    Data::Vector3D normal = box_plane.normal();

                    Real cos1 = std::fabs(normal.cosAngle(vec1));
                    Real cos2 = std::fabs(normal.cosAngle(vec2));
                    Real cos3 = std::fabs(normal.cosAngle(vec3));

                    ///SELECIONANDO A MELHOR ARESTA PELO ANGULO
                    if( cos1 < cos2 && cos1 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(1) );
                        surface_vertexs.push_back( face->getVertex(0) );
                        //continue;
                    }
                    else if( cos2 < cos1 && cos2 < cos3 )
                    {
                        surface_vertexs.push_back( face->getVertex(2) );
                        surface_vertexs.push_back( face->getVertex(0) );
                        //continue;
                    }
                    else if( cos3 < cos1 && cos3 < cos2 )
                    {
                        surface_vertexs.push_back( face->getVertex(1) );
                        surface_vertexs.push_back( face->getVertex(2) );
                        //continue;
                    }


                    ///SELECIONANDO A MELHOR ARESTA QUE CORTA A PARTIÇÃO
                    if( std::fabs(dists[2]) + Data::getTolerance() > std::fabs(dists[1]) && std::fabs(dists[2]) + Data::getTolerance() > std::fabs(dists[0]) )
                    {
                        surface_vertexs.push_back( face->getVertex(1) );
                        surface_vertexs.push_back( face->getVertex(0) );
                        continue;
                    }
                    else
                    {
                        if( std::fabs(dists[0]) < std::fabs(dists[1]) + Data::getTolerance() )
                        {
                            surface_vertexs.push_back( face->getVertex(2) );
                            surface_vertexs.push_back( face->getVertex(0) );
                        }
                        else
                        {
                            surface_vertexs.push_back( face->getVertex(1) );
                            surface_vertexs.push_back( face->getVertex(2) );
                        }
                        continue;
                    }
                }
            }
        }

        ///TRIANGULO EM CIMA DA PARTIÇÃO
        ///É IDENTIFICADO O VÉRTICE MAIS PROXIMO DO PLANO DE PARTIÇÃO (PP) E SELECIONA A ARESTA DO TRIANGULO QUE FAZ O MELHOR ANGULO COM O PP
        if( (f1 && f2 && f3) || num<2 )
        {
            for( int i=0; i<3; i++ )
            {
                ///VÉRTICE MAIS PROXIMO DO PP
                if( std::fabs(dists[i]) <= std::fabs(dists[ (i+1)%3 ])  + Data::getTolerance() && std::fabs(dists[i]) <= std::fabs(dists[ (i+2)%3 ])  + Data::getTolerance() )
                {
//std::cout << " ** DIST 1: " << std::fabs(dists[ (i+1)%3 ]) << std::endl;
//std::cout << " ** DIST 2: " << std::fabs(dists[ (i+2)%3 ]  + Data::getTolerance()) << std::endl;

                    Data::Vector3D vec1( face->getVertex( i )->getPoint(), face->getVertex( (i+1)%3 )->getPoint() );
                    Data::Vector3D vec2( face->getVertex( i )->getPoint(), face->getVertex( (i+2)%3 )->getPoint() );
                    Data::Vector3D normal = box_plane.normal();

                    Real cos1 = normal.cosAngle(vec1);
                    Real cos2 = normal.cosAngle(vec2);

                    //std::cout << " cos 1: " << cos1 << std::endl;
                    //std::cout << " cos 2: " << cos2 << std::endl;

                    ///ARESTA DO TRIANGULO QUE FAZ O MELHOR ANGULO COM O PP
                    //if( std::fabs(dists[ (i+1)%3 ]) <= std::fabs(dists[ (i+2)%3 ]  + Data::getTolerance())  )
                    if( std::fabs(cos1) <= std::fabs(cos2)  )
                    {
                        surface_vertexs.push_back( face->getVertex( i ) );
                        surface_vertexs.push_back( face->getVertex( (i+1)%3 ) );
                    }
                    else
                    {
                        surface_vertexs.push_back( face->getVertex( i ) );
                        surface_vertexs.push_back( face->getVertex( (i+2)%3 ) );
                    }
                }
            }
            //continue;
        }

        /*
                if( f1 )
                {
                    surface_vertexs.push_back( (*it)->getVertex(0) );


                    //std::cout << " P1 "  << f1 << std::endl ;
                    //std::cout << (*it)->getVertex(0)->getId() << "  "  << (*it)->getVertex(0)->getPoint()->getCoord(0) << " " << (*it)->getVertex(0)->getPoint()->getCoord(1) << " " << (*it)->getVertex(0)->getPoint()->getCoord(2) << std::endl ;

                }
                if( f2 )
                {
                    surface_vertexs.push_back( (*it)->getVertex(1) );

                    //std::cout << " P2 "  << f2 << std::endl ;
                    //std::cout << (*it)->getVertex(1)->getId() << "  "  << (*it)->getVertex(1)->getPoint()->getCoord(0) << " " << (*it)->getVertex(1)->getPoint()->getCoord(1) << " " << (*it)->getVertex(1)->getPoint()->getCoord(2) << std::endl ;
                }
                if( f3 )
                {
                    surface_vertexs.push_back( (*it)->getVertex(2) );

                    //std::cout << " P3 "  << f3 << std::endl ;
                    //std::cout << (*it)->getVertex(2)->getId() << "  "  << (*it)->getVertex(2)->getPoint()->getCoord(0) << " " << (*it)->getVertex(2)->getPoint()->getCoord(1) << " " << (*it)->getVertex(2)->getPoint()->getCoord(2) << std::endl ;
                }
            */
    }


    //std::cout << "DEBUG 0 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;


    ///FAZ UMA COPIA DOS VERTICES ORIGINAIS
    makeSurfaceVertexes(surface_vertexs, original_surface_vertexes);

    //ULInt vId = 0, feId = 0;
    //Real maxElementSize = 0.0;
    int x = 1;
    ///MONTAGEM DAS ARESTAS
    for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
    {

        MJMesh::Vertex *v_aux = (MJMesh::Vertex*)(*it);

        /*if (v_aux->getId() > vId)
        {
            vId = v_aux->getId();
        }*/

        ++it;
        MJMesh::FrontElementSurface *edge = new MJMesh::FrontElementSurface( v_aux, (MJMesh::Vertex*)(*it), x++ );
        surface_elements.push_back( edge );

        /*if (edge->getId() > feId)
        {
            feId = edge->getId();
        }*/

        //maxElementSize += edge->getShape()->size();
    }


    //maxElementSize /= static_cast<Real>(surface_vertexs.size());

    //std::cout << "DEBUG 1 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;

    surface_vertexs.sort(sort_vertex);
    surface_vertexs.unique(same_vertex);

    //std::cout << "DEBUG 1.555555555 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;

    ///IMPRESSÃO GEN
    {
        /*std::ofstream myfile ("/home/nfs/daniel/Desktop/saida_aresta.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_vertexs.size() << std::endl ;
            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                       << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();*/
    }





    //std::cout << "TERMINOU A ELIMINAÇÃO!!!" << std::endl;

    ///IMPRESSÃO GEN
    {
        std::ofstream myfile ("/home/nfs/daniel/Desktop/saida_aresta_reduzido.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_vertexs.size() << std::endl ;
            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                       << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();
    }


    //std::cout << "DEBUG 1 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;
    //std::cout << "DEBUG 1 TAM SURFACE ELEMENTS: " << surface_elements.size() << std::endl;

    ///REMOÇÃO DE POSSÍVEIS CICLOS
    ///CORREÇÃO DAS ARESTAS SOLTAS OU DUPLICADAS
    Data::FrontElementHash wrongElements;
    fixBoundaryLoops(sub_front, original_surface_vertexes, surface_vertexs, surface_elements, box_plane, wrongElements);

    //std::cout << "DEBUG 2 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;
    //std::cout << "DEBUG 2 TAM SURFACE ELEMENTS: " << surface_elements.size() << std::endl;

    ///IMPRESSÃO GEN
    {
        /*std::ofstream myfile ("/home/nfs/daniel/Desktop/saida_aresta_QUASEprontas.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_vertexs.size() << std::endl ;
            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                       << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();*/
    }


    ///CORREÇÃO DA ORIENTAÇÃO DAS ARESTAS
    MJMesh::Vertex *min = static_cast<MJMesh::Vertex *>(surface_vertexs.front());
    if( surface_vertexs.size() > 0 )
    {
        Data::VertexHashMap bound_vertexes;
        bound_vertexes.reserve( surface_vertexs.size() );

        while( bound_vertexes.size() < surface_elements.size() )
        {
            for (Data::Front::VertexIterator it = ++surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*it));


                if ( bound_vertexes.find( v->getId() ) != bound_vertexes.end() )
                    continue;


                if (v->numAdjacentFrontElements() != 2)
                {
                    continue;
                }

                for (UInt i = 0; i < this->dimension; i++)
                {
                    UInt j = (i + side + this->dimension - 1)%this->dimension;

                    if (v->getPoint()->getCoord(j) < min->getPoint()->getCoord(j))
                    {
                        min = v;

                        break;
                    }

                    if (v->getPoint()->getCoord(j) > min->getPoint()->getCoord(j))
                    {
                        break;
                    }
                }
            }

            MJMesh::FrontElementSurface *first = static_cast<MJMesh::FrontElementSurface *>(*min->feBegin());
            MJMesh::FrontElementSurface *second = static_cast<MJMesh::FrontElementSurface *>(*(--min->feEnd()));

            //MJMesh::FrontElementSurface *first = static_cast<MJMesh::FrontElementSurface *>(surface_elements.front());

            if (first->getV1() == min)
            {
                first->invert();
            }

            if (second->getV2() == min)
            {
                second->invert();
            }

            if (first->getV1()->getPoint()->getCoord((side + 1)%this->dimension) > second->getV2()->getPoint()->getCoord((side + 1)%this->dimension))
            {
                first->invert();
                second->invert();

                std::swap(first, second);
            }

            Data::FrontElementHash rightElements;

            rightElements.insert(first);
            rightElements.insert(second);

            //std::cout << "PRIMEIRO: " << first->text() << std::endl;
            //std::cout << "SEGUNDO: " << second->text() << std::endl;

            this->orient(second, rightElements, wrongElements, false);


            for(Data::FrontElementHash::iterator iter = rightElements.begin(); iter != rightElements.end(); iter++)
            {
                Data::FrontElement *fel = (*iter);
                //std::cout << fel->text() << std::endl;
                bound_vertexes[fel->getVertex(0)->getId()] = fel->getVertex(0);
                bound_vertexes[fel->getVertex(1)->getId()] = fel->getVertex(1);
            }

            for (Data::Front::VertexIterator it = ++surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*it));

                if ( bound_vertexes.find( v->getId() ) == bound_vertexes.end() )
                {
                    min = v;
                    break;
                }
            }



            //MJMesh::FrontElementSurface *next = NULL, *current = first;

            //do
            //{
            //if(surface_elements.size() == 0)
            //break;

            //MJMesh::Vertex *v2 = current->getV2();

            //for (Data::FrontElementList::iterator iter = v2->feBegin();
            //iter != v2->feEnd(); iter++)
            //{
            //if ((*iter) != current)
            //{
            //next = static_cast<MJMesh::FrontElementSurface *>((*iter));

            //break;
            //}
            //}

            //if (next->getV2() == v2)
            //{
            //next->invert();
            //}

            //current = next;
            //next = NULL;
            //}
            //while (current != first);
        }
    }

    //std::cout << "DEBUG 3 TAM SURFACE VERTEXS: " << surface_vertexs.size() << std::endl;


    ///DEBUG colocando os vertices que estao na surface_elements (bugando no caso do ciclo)
    /*surface_vertexs.clear();
    for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
    {
        for (UInt i = 0; i < (*it)->numVertices(); i++)
        {
            Data::Vertex *v = (*it)->getVertex(i);

            if (std::find(surface_vertexs.begin(), surface_vertexs.end(), v) == surface_vertexs.end())
            {
                surface_vertexs.push_back(v);
            }
        }
    }*/
    ///END DEBUG


    ///TESTE DO Tools::InnerSurfaceIdentifier *identifier
    {
        /*
           MJMesh::Boundary *surface_boundary;
           surface_boundary = new MJMesh::Boundary();
           surface_boundary->setVertices( surface_vertexs );
           surface_boundary->setElements( surface_elements );



           Tools::InnerSurfacesIdentifier *identifier = new Tools::InnerSurfacesIdentifier();
           identifier->setBoundary(surface_boundary);
           identifier->setSurfaceDirection(side);
           identifier->setNeedTree(true);
           identifier->execute();

           const Performer::InnerSurfacesIdentifier::SurfaceTree *root = identifier->getTree();

           Data::FrontElementList list_surfaces = identifier->getInnerSurfaces();

           std::cout << "TESTE: " << side << "  " << list_surfaces.size() << std::endl;

           if(root==NULL) std::cout << "TA NULU" << std::endl;

           std::list<Performer::InnerSurfacesIdentifier::SurfaceTree *> surfaces = root->children;

           if(root->surface == NULL) std::cout << "TA NULU surf" << std::endl;
           if(root->surface->position == NULL) std::cout << "TA NULU pos" << std::endl;
           if(root->surface->boundary == NULL) std::cout << "TA NULU surf" << std::endl;

           std::cout << "TAM: " << root->surface->boundary->getElements().size() << std::endl;
           std::cout << "POSIÇÃO: " << root->surface->position << std::endl;


           //delete identifier;*/
    }

    ///IMPRESSÃO GEN
    {
        /*std::cout << "surface boundary FINAIS" << std::endl;
        std::cout << "2.5" << std::endl ;
        std::cout << surface_vertexs.size() << std::endl ;
        for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
        {
        std::cout << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
        }
        std::cout << surface_elements.size() << std::endl ;
        for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
        {
        std::cout << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
        << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
        }
        std::cout << "0" << std::endl;*/
    }
}


void PMJMesh2::APrioriParallelMeshGenerator::createInterface3D(
    PMJMesh::Front *sub_front, PMJMesh::Front *local_front,
    Data::VertexSet &common_vertices_list, Data::VertexHashMap &original_surface_vertexes,
    Data::VertexList &surface_vertexs, Data::FrontElementList &surface_elements,
    const Data::VertexList &internal_vertexs, const Data::ElementList &internal_elements,
    Performer::IdManager *id_Manager, ULInt taskId, bool invert_Mesh, UInt side, Real &meshSurf_Processing)
{
    //std::cout << "GERAR MALHA EM " << taskId << std::endl;

    ///ARQUIVO GEN
    {
        /*static int ert = 1;
        std::string as = "/home/nfs/daniel/Desktop/entrada_SurfacesIdentifier.gen";
        as += std::to_string(ert++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/entrada_SurfacesIdentifier.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surface_vertexs.size() << std::endl ;
            for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << surface_elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                       << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
            }
            myfile << "0" << std::endl;
        }
        myfile.close();*/
    }

    if( surface_vertexs.empty() || surface_elements.empty() )
    {
        std::cout << "ANEL DE ARESTAS COM ERROR!!!" << std::endl;
    }

    surface_vertexs.sort(sort_vertex);
    surface_elements.sort(sort_frontElementId);

    ///LISTA DOS VERTICES QUES ESTÃO NO CORTE DA PARTIÇÃO
    for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
    {
        common_vertices_list.insert( original_surface_vertexes[(*it)->getId()] );
    }

    Real start = Data::time();

    ///PROCURANDO CICLOS E ORDENANDO COM O Tools::InnerSurfaceIdentifier
    ///     o Tools::InnerSurfaceIdentifier que vai dividir as arestas em ciclos
    {
        //std::cout << "TOTAL DE VERTICES: " << surface_vertexs.size() << std::endl ;
        //std::cout << "TOTAL DE ARESTAS: " << surface_elements.size() << std::endl ;


        MJMesh::Boundary *surface_boundary = new MJMesh::Boundary();
        surface_boundary->setVertices( surface_vertexs );
        surface_boundary->setElements( surface_elements );

        MJMesh::Mesh *supportMesh = new MJMesh::Mesh();
        supportMesh->setVertices( internal_vertexs );
        supportMesh->setElements( internal_elements );

        Tools::InnerSurfacesIdentifier *identifier = new Tools::InnerSurfacesIdentifier();
        identifier->setBoundary( static_cast<Data::Boundary *>( surface_boundary ) );
        identifier->setMesh(supportMesh);
        identifier->setSurfaceDirection(side%3);
        identifier->setNeedTree(true);
        identifier->execute();

        const Performer::InnerSurfacesIdentifier::SurfaceTree *root = identifier->getTree();
        //Performer::InnerSurfacesIdentifier::SurfaceTreeList surfaces = root->children;

        ///PERCORRE A ÁRVORE DE SUPERFICIES E CHAMA O MESHSURF
        //prepareSurfacesMeshSurf(sub_front, local_front, root, common_vertices_list, original_surface_vertexes, internal_vertexs, internal_elements, id_Manager, taskId, invert_Mesh, side);

        for (Performer::InnerSurfacesIdentifier::SurfaceTreeList::const_iterator iter = root->children.begin();
                iter != root->children.end(); iter++)
        {
            prepareSurfacesMeshSurf(sub_front, local_front, (*iter), common_vertices_list, original_surface_vertexes, internal_vertexs, internal_elements, id_Manager, taskId, invert_Mesh, side, meshSurf_Processing);
        }


        /*std::cout << "num children: " << root->children.size() << std::endl;

        const Performer::InnerSurfacesIdentifier::SurfaceTree *node = root->children.front();

        Data::FrontElementList node_surface_elements = node->surface->boundary->getElements();
        Data::VertexList node_surface_vertices = node->surface->boundary->getVertices();

        std::cout << "children boundary E: " << node_surface_elements.size() << std::endl;
        std::cout << "children boundary V: " << node_surface_vertices.size() << std::endl;
        std::cout << "children size: " << node->children.size() << std::endl;*/


        //supportMesh->clear();
        delete supportMesh;
        //surface_boundary->clear();
        //delete surface_boundary;

        delete identifier;
    }
}

bool PMJMesh2::APrioriParallelMeshGenerator::improveSurfaceMesh( PMJMesh::Front *sub_front, Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary, Performer::IdManager *mesh_surf_idManager, const Data::VertexList &surface_vertexs, const Data::FrontElementList &surface_elements, const Data::VertexHashMap &original_surface_vertexes, int side, Real pos_plane, Data::Point3D p_min, Data::Point3D p_max )
{
    Real start = Data::time();
    //std::cout << " CHECANDO A SUPERFICIE " << std::endl;

    ///IMPRESSÃO
    {
        /*static int ert = 1;
        std::cout << " CHECANDO A SUPERFICIE " << ert << std::endl;
        std::string as = "/home/nfs/daniel/Desktop/Malha_MeshSurf";
        as += std::to_string(ert++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surfaceMesh->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << surfaceMesh->size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile.close();*/
    }

    ///TESTE PARA SABER SE A MALHA FOI GERADA FORA DO MODELO
    if( isElementInside(surfaceMesh->getElements().back()->getShape()->box(), tree->getRoot()) == false )
    {
        std::cout << " GEROU ELEMENTO FORA DO MODELO! " << surfaceMesh->getElements().back()->getId() << std::endl;
        return false;
    }

    ///TESTE PARA SABER SE A MALHA FOI GERADA FORA DO CONTORNO DE ENTRADA
    Data::Box b(p_min, p_max);
    if( b.out( surfaceMesh->getElements().back()->getShape()->box() ) )
    {
        std::cout << " GEROU ELEMENTO FORA DO CONTORNO!!! " << surfaceMesh->getElements().back()->getId() << std::endl;
        return false;
    }


    ///DEVOLVENDO OS VERTICES DA BORDA À POSIÇÃO ORIGINAL
    for (Data::Mesh::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); it++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>( (*it) );
        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find( v->getId() ) ).second );

        v->getPoint()->setCoords( v_ori->getPoint()->getCoords() );
    }


    ///MELHORIAS NA MALHA
    for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::FrontElement *ele = static_cast<MJMesh::FrontElement *>((*it));

        ///CADA ARESTA É ADJACENTE A SOMENTE UM ELEMENTO
        MJMesh::Element *element = NULL;

        for (Data::Mesh::ElementIterator ite = ele->getV1()->eBegin(); ite != ele->getV1()->eEnd(); ite++)
        {
            if( (*ite)->have( ele->getV1() ) && (*ite)->have( ele->getV2() ) )
            {
                element = static_cast<MJMesh::Element *>( (*ite) );
                break;
            }
        }

        if( element == NULL )
        {
            std::cout << "Aresta com error (sem elemento ligado): " << ele->text() << std::endl;
            return false;
        }

        //std::cout << "Aresta" << ele->text() << std::endl;
        //std::cout << element->getId() << "\t" << element << std::endl;
        //std::cout << "S: "<< ele->getAdjacency().size() << std::endl;

        Data::Triangle3D *t = static_cast<Data::Triangle3D*>( element->getShape() );

        ///SEMPRE PEGO O VERTICE 0 DA ARESTA
        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(0)->getId())).second );
        MJMesh::Vertex *v_test = static_cast<MJMesh::Vertex *>( ele->getVertex(0) );
        MJMesh::Vertex *v_dif = NULL;
        for( int x=0; x<3; x++ )
        {
            if( ele->getVertex(0)->getId()!= element->getVertex(x)->getId() && ele->getVertex(1)->getId()!= element->getVertex(x)->getId() )
                v_dif = static_cast<MJMesh::Vertex *>( element->getVertex(x) );
        }


        MJMesh::FrontElement *ele_pos = NULL;
        bool flag_adj = false;

        ///RETIRADO POR CONTA DE MUITOS ERROS
        /*
                ///REPOSICIONANDO TODOS OS V_ADJ PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
                for (Data::Mesh::ElementIterator ite2 = v_dif->eBegin(); ite2 != v_dif->eEnd(); ite2++)
                {
                    if( (*ite2)->have( ele->getVertex(0) ) || (*ite2)->have( ele->getVertex(1) ) )
                    {
                        for( int x=0; x<3; x++ )
                        {
                            if( (*ite2)->getVertex(x)->getId()!= ele->getVertex(0)->getId() && (*ite2)->getVertex(x)->getId()!= ele->getVertex(1)->getId()
                                    && (*ite2)->getVertex(x)->getId()!= v_dif->getId() && !surface_boundary->have( (*ite2)->getVertex(x) ) )
                            {
                                MJMesh::Vertex *v_adj = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(x) );

                                ///REPOSICIONANDO O V_ADJ PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
                                if( v_adj != NULL )
                                {
                                    Real p_pos[3] = {0,0,0};
                                    Real area = 0.0;
                                    for (Data::Mesh::ElementIterator ite = v_adj->eBegin(); ite != v_adj->eEnd(); ite++)
                                    {
                                        MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite) );

                                        area += e->getShape()->size();

                                        p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                                        p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                                        p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();

                                    }
                                    p_pos[0] /= area;
                                    p_pos[1] /= area;
                                    p_pos[2] /= area;
                                    v_adj->getPoint()->setCoord(0, p_pos[0]);
                                    v_adj->getPoint()->setCoord(1, p_pos[1]);
                                    v_adj->getPoint()->setCoord(2, p_pos[2]);
                                }
                            }

                        }
                    }
                }


                ///ERRO QUANDO UM VERTICE TEM ADJS COM UMA ÁREA MUITO GRANDE - GERA COLISÕES COM OUTROS ELEMENTOS MESMO N SENDO ADJ AO VERTICE - MELHOR TIRAR
                ///REPOSICIONANDO O V_DIF PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
                if( !surface_boundary->have( v_dif ) )
                {
                    //Real area = 0.0;
                    //Data::Vector3D vec( {0.0, 0.0, 0.0 } );
                    //Data::Vector3D vec_ori( {0.0, 0.0, 0.0 } );
                    //vec_ori.setCoords( v_dif->getPoint()->getCoordsArray() );
                    Data::Real vec[3] = {0.0, 0.0, 0.0 };
                    Data::Real vec_ori[3] = {v_dif->getPoint()->getCoord(0), v_dif->getPoint()->getCoord(1), v_dif->getPoint()->getCoord(2) };

                    for (Data::Mesh::ElementIterator ite = v_dif->eBegin(); ite != v_dif->eEnd(); ite++)
                    {
                        MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite) );

                        //area += e->getShape()->size();

                        vec[0] += e->getVertex(0)->getPoint()->getCoord(0);
                        vec[0] += e->getVertex(1)->getPoint()->getCoord(0);
                        vec[0] += e->getVertex(2)->getPoint()->getCoord(0);

                        vec[1] += e->getVertex(0)->getPoint()->getCoord(1);
                        vec[1] += e->getVertex(1)->getPoint()->getCoord(1);
                        vec[1] += e->getVertex(2)->getPoint()->getCoord(1);

                        vec[2] += e->getVertex(0)->getPoint()->getCoord(2);
                        vec[2] += e->getVertex(1)->getPoint()->getCoord(2);
                        vec[2] += e->getVertex(2)->getPoint()->getCoord(2);

                        //Data::Point *pos = e->getVertex(0)->getPoint();
                        //pos->sum( e->getVertex(1)->getPoint()->getCoords() );
                        //pos->sum( e->getVertex(2)->getPoint()->getCoords() );

                        //p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                        //p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                        //p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();

                    }
                    //p_pos[0] /= area;
                    //p_pos[1] /= area;
                    //p_pos[2] /= area;

                    vec[0] /= v_dif->numAdjacentElements() * 3.0;
                    vec[1] /= v_dif->numAdjacentElements() * 3.0;
                    vec[2] /= v_dif->numAdjacentElements() * 3.0;

                    v_dif->getPoint()->setCoords( vec );
                }
        */

        ///AJUSTANDO APENAS A "ALTURA" DOS VERTICES VIZINHOS A BORDA
        if( !surface_boundary->have( v_dif ) )
        {
            Data::Real vec[3] = {0.0, 0.0, 0.0 };

            for (Data::Mesh::ElementIterator ite = v_dif->eBegin(); ite != v_dif->eEnd(); ite++)
            {
                MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite) );

                vec[0] += e->getVertex(0)->getPoint()->getCoord(0);
                vec[1] += e->getVertex(0)->getPoint()->getCoord(1);
                vec[2] += e->getVertex(0)->getPoint()->getCoord(2);

                vec[0] += e->getVertex(1)->getPoint()->getCoord(0);
                vec[1] += e->getVertex(1)->getPoint()->getCoord(1);
                vec[2] += e->getVertex(1)->getPoint()->getCoord(2);

                vec[0] += e->getVertex(2)->getPoint()->getCoord(0);
                vec[1] += e->getVertex(2)->getPoint()->getCoord(1);
                vec[2] += e->getVertex(2)->getPoint()->getCoord(2);


                /*if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                {
                    vec[0] += e->getVertex(0)->getPoint()->getCoord(0);
                    vec[0] += e->getVertex(1)->getPoint()->getCoord(0);
                    vec[0] += e->getVertex(2)->getPoint()->getCoord(0);
                }
                if( side == 1 || side == 4 ) ///TOP E BASE
                {
                    vec[1] += e->getVertex(0)->getPoint()->getCoord(1);
                    vec[1] += e->getVertex(1)->getPoint()->getCoord(1);
                    vec[1] += e->getVertex(2)->getPoint()->getCoord(1);
                }
                if( side == 2 || side == 5 ) ///FRONT E BACK
                {
                    vec[2] += e->getVertex(0)->getPoint()->getCoord(2);
                    vec[2] += e->getVertex(1)->getPoint()->getCoord(2);
                    vec[2] += e->getVertex(2)->getPoint()->getCoord(2);
                }*/
            }

            if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA      -   0
            {
                vec[0] /= v_dif->numAdjacentElements() * 3.0;
                vec[1] /= v_dif->numAdjacentElements() * 3.0;
                vec[2] /= v_dif->numAdjacentElements() * 3.0;
                v_dif->getPoint()->setCoord(0, vec[0]);

                //v_dif->getPoint()->setCoord(1, (v_dif->getPoint()->getCoord(1)*2.0 + vec[1])/3.0 );
                //v_dif->getPoint()->setCoord(2, (v_dif->getPoint()->getCoord(2)*2.0 + vec[2])/3.0 );

            }
            if( side == 1 || side == 4 ) ///TOP E BASE      -   1
            {
                vec[0] /= v_dif->numAdjacentElements() * 3.0;
                vec[1] /= v_dif->numAdjacentElements() * 3.0;
                vec[2] /= v_dif->numAdjacentElements() * 3.0;
                v_dif->getPoint()->setCoord(1, vec[1]);

                //v_dif->getPoint()->setCoord(0, (v_dif->getPoint()->getCoord(0)*2.0 + vec[0])/3.0 );
                //v_dif->getPoint()->setCoord(2, (v_dif->getPoint()->getCoord(2)*2.0 + vec[2])/3.0 );
            }
            if( side == 2 || side == 5 ) ///FRONT E BACK    -   2
            {
                vec[0] /= v_dif->numAdjacentElements() * 3.0;
                vec[1] /= v_dif->numAdjacentElements() * 3.0;
                vec[2] /= v_dif->numAdjacentElements() * 3.0;
                v_dif->getPoint()->setCoord(2, vec[2]);

                //v_dif->getPoint()->setCoord(0, (v_dif->getPoint()->getCoord(0)*2.0 + vec[0])/3.0 );
                //v_dif->getPoint()->setCoord(1, (v_dif->getPoint()->getCoord(1)*2.0 + vec[1])/3.0 );
            }
        }

        ///CHECA SE O VIZINHO DA ARESTA COMPARTILHA UM ELEMENTO COM A ARESTA PRINCIPAL
        for (std::list<Data::FrontElement *>::iterator itt = v_test->feBegin(); itt != v_test->feEnd(); itt++)
        {
            if( (*itt)->getId() == ele->getId() ) continue;

            ele_pos = static_cast<MJMesh::FrontElement *>((*itt));

            //std::cout << " TESTANDO: " << ele->getId() << " com : "  << ele_pos->getId() << std::endl;
            //std::cout << " S: " << ele_pos->getAdjacency().size() << std::endl;
            //std::cout << " ele: " << ele->text() << std::endl;
            //std::cout << " ele_pos: " << ele_pos->text() << std::endl;
            //std::cout << " elemente id: " << element->text() << std::endl;

            MJMesh::ElementSurface *face_in_edge = NULL;

            for (Data::Mesh::ElementIterator ite2 = v_test->eBegin(); ite2 != v_test->eEnd(); ite2++)
            {
                if( (*ite2)->have( ele->getVertex(1) ) && (*ite2)->have( ele_pos->getVertex(0) ) && (*ite2)->have( ele_pos->getVertex(1) ) )
                {
                    face_in_edge = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                    break;
                }
            }

//std::cout << " ele_pos element id: " << ele_pos->getAdjacency().front()->getId() << std::endl;

            if( face_in_edge == NULL ) continue;

            //std::cout << "    COMPARTILHA FACE: " << element->getId() << " com o elemento_pos: "  << ele_pos->getId() << " e elemente id: " << ele->getId() << std::endl;
            //ele_pos = static_cast<MJMesh::FrontElement *>((*it));
            flag_adj = true;


            ///Verifica se a face adj tem um vertice(v_aux3) na borda
            ///Se tiver na borda tem que pegar o seu vizinho, depois deletar as 3 faces
            ///Gerar um vertice que é a media dos baricentros e ligar tudo (5 faces novas)

            ///FACES QUE ESTÃO EM QUINAS SERÃO DIVIDIDAS EM 2 USANDO A SUA FACE ADJ
            if( flag_adj )
            {
                //MJMesh::ElementSurface *face_in_edge = static_cast<MJMesh::ElementSurface *>( ele_pos->getAdjacency().front() );
                MJMesh::ElementSurface *face_in_edge_adj = NULL;       ///primeiro vizinho
                MJMesh::ElementSurface *face_in_edge_adj2 = NULL;      ///Vizinho do vizinho mas compartilha vertice v_aux1 ou v_aux2
                MJMesh::Vertex *v_aux1 = static_cast<MJMesh::Vertex *>( ele->getVertex(1) );
                MJMesh::Vertex *v_aux2 = NULL;
                MJMesh::Vertex *v_aux3 = NULL;
                MJMesh::Vertex *v_aux4 = NULL;

                ///Encontrando v_aux2
                for(int x=0; x<3; x++)
                {
                    if( face_in_edge->getVertex(x)->getId() != v_test->getId() && face_in_edge->getVertex(x)->getId() != v_aux1->getId() )
                    {
                        v_aux2 = static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(x) );
                        continue;
                    }
                }

                //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                //std::cout << "V_TEST: "  << v_test->text() << std::endl;
                //std::cout << "ELE: "  << ele->text() << std::endl;
                //std::cout << "ELEMENT: "  << element->text() << std::endl;
                //std::cout << "ELE_POS: "  << ele_pos->text() << std::endl;
                //std::cout << "FACE DE TESTE face_in_edge: "  << face_in_edge->text() << std::endl;

                ///Encontrando v_aux3 e face_in_edge_adj
                for (Data::Mesh::ElementIterator ite2 = v_aux1->eBegin(); ite2 != v_aux1->eEnd(); ite2++)
                {
                    //std::cout << "TESTANDO: "  << (*ite2)->text() << std::endl;

                    if( (*ite2)->have( v_aux1 ) && (*ite2)->have( v_aux2 ) && !(*ite2)->have( v_test ) )
                    {
                        for(int x=0; x<3; x++)
                        {
                            if( (*ite2)->getVertex(x)->getId() != v_aux1->getId() && (*ite2)->getVertex(x)->getId() != v_aux2->getId() )
                                v_aux3 = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(x) );
                        }

                        //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                        //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                        //std::cout << "V_AUX3: "  << v_aux3->text() << std::endl;
                        //std::cout << "ACHEI FACE face_in_edge_adj: "  << (*ite2)->text() << std::endl;
                        //std::cout << "FACE DE TESTE face_in_edge: "  << face_in_edge->text() << std::endl;

                        face_in_edge_adj = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                    }
                }

                ///MALHA DO MESH SURF CORROMPIDA
                if( v_aux3 == NULL )
                {
                    //std::cout << "MALHA DO MESH SURF CORROMPIDA - v_aux3 NULL V_TEST " << v_test->getId()  << std::endl;
                    return false;
                }

                ///ENCONTRANDO A FACE ADJ ( face_in_edge_adj2 ) DA face_in_edge_adj E O SEU VERTICE ( V_AUX4 )
                for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                {
                    if( ( (*ite2)->have( v_aux1 ) || (*ite2)->have( v_aux2 ) ) && (*ite2)->getId() != face_in_edge_adj->getId() )
                    {
                        for( int p = 0; p < 3; p++ )
                        {
                            if( (*ite2)->getVertex(p)->getId() != v_aux1->getId() &&
                                    (*ite2)->getVertex(p)->getId() != v_aux2->getId() &&
                                    (*ite2)->getVertex(p)->getId() != v_aux3->getId() &&
                                    !surface_boundary->have( (*ite2)->getVertex(p) ) )
                            {
                                face_in_edge_adj2 = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                                v_aux4 = static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(p) );

                                //std::cout << "FACE irmã: "  << face_in_edge_adj2->text() << std::endl;
                                //std::cout << "FACE irmã vertex: "  << v_aux4->text() << std::endl;
                            }
                        }
                    }
                }


                ///MALHA DO MESH SURF CORROMPIDA
                if( v_aux4 == NULL )
                {
                    ///V_AUX4 PODE ESTAR NA BORDA
                    Real d = DBL_MAX;
                    for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                    {
                        if( ( (*ite2)->have( v_aux1 ) || (*ite2)->have( v_aux2 ) ) && (*ite2)->getId() != face_in_edge_adj->getId() )
                        {
                            for( int p = 0; p < 3; p++ )
                            {
                                if( (*ite2)->getVertex(p)->getId() != v_aux1->getId() &&
                                        (*ite2)->getVertex(p)->getId() != v_aux2->getId() &&
                                        (*ite2)->getVertex(p)->getId() != v_aux3->getId() )
                                {
                                    if( d > v_test->getPoint()->distance( (*ite2)->getVertex(p)->getPoint() ) )
                                    {
                                        d = v_test->getPoint()->distance( (*ite2)->getVertex(p)->getPoint() );
                                        v_aux4 = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(p) );
                                        face_in_edge_adj2 = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                                    }
                                }
                            }
                        }
                    }

                    if( v_aux4 == NULL ) return false;
                }

                ///ELEMENTO ADJ ESTÀ TOCANDO NA BORDA
                if( surface_boundary->have( v_aux3 ) )      ///FACES TOCAM A BORDA!!!!
                {
                    //std::cout << "IF: "  << v_test->text() << std::endl;
                    //std::cout << "+++++++++++++++++++++++++++++++++++++++++"  << std::endl;
                    //std::cout << "V_TEST: "  << v_test->text() << std::endl;
                    //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                    //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                    //std::cout << "V_AUX3: "  << v_aux3->text() << std::endl;
                    //std::cout << "V_AUX4: "  << v_aux4->text() << std::endl;
                    //std::cout << "face_in_edge: "  << face_in_edge->text() << std::endl;
                    //std::cout << "face_in_edge_adj: "  << face_in_edge_adj->text() << std::endl;
                    //std::cout << "face_in_edge_adj2: "  << face_in_edge_adj2->text() << std::endl;

                    Data::VertexVector vertexs;

                    vertexs.push_back( v_test );
                    vertexs.push_back( v_aux1 );
                    vertexs.push_back( v_aux2 );
                    vertexs.push_back( v_test );

                    if( face_in_edge_adj2->have( v_aux1 ) )
                    {
                        vertexs.push_back( v_aux3 );
                        vertexs.push_back( v_aux2 );

                        vertexs.push_back( v_aux1 );
                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux3 );
                    }
                    else
                    {
                        vertexs.push_back( v_aux1 );
                        vertexs.push_back( v_aux3 );

                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux2 );
                        vertexs.push_back( v_aux3 );
                        vertexs.push_back( v_aux4 );
                    }


                    ///CRIAÇÃO DO NOVO VERTICE PELA MEDIA DAS 3 FACES
                    Real p[3] = {0,0,0};
                    p[0] = ( ( face_in_edge->getShape()->mid3D().getCoord(0) + face_in_edge_adj->getShape()->mid3D().getCoord(0) + face_in_edge_adj2->getShape()->mid3D().getCoord(0) ) / 3.0 );
                    p[1] = ( ( face_in_edge->getShape()->mid3D().getCoord(1) + face_in_edge_adj->getShape()->mid3D().getCoord(1) + face_in_edge_adj2->getShape()->mid3D().getCoord(1) ) / 3.0 );
                    p[2] = ( ( face_in_edge->getShape()->mid3D().getCoord(2) + face_in_edge_adj->getShape()->mid3D().getCoord(2) + face_in_edge_adj2->getShape()->mid3D().getCoord(2) ) / 3.0 );


                    /*if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                    {
                        Real v = pos_plane - p->getX();
                        p->setX( p->getX() + v );
                    }
                    if( side == 1 || side == 4 ) ///TOP E BASE
                    {
                        Real v = pos_plane - p->getY();
                        p->setY( p->getY() + v );
                    }
                    if( side == 2 || side == 5 ) ///FRONT E BACK
                    {
                        Real v = pos_plane - p->getZ();
                        p->setZ( p->getZ() + v );
                    }*/


                    MJMesh::Vertex *new_vertex = new MJMesh::Vertex( p[0], p[1], p[2], mesh_surf_idManager->next(0) );
                    surfaceMesh->add(new_vertex);


                    //std::cout << "ele: " <<  ele->text() << std::endl;
                    //std::cout << "ele_pos: " <<  ele_pos->text() << std::endl;
                    //std::cout << "v_test: " <<  v_test->text() << std::endl;
                    //std::cout << "v_aux1: " <<  v_aux1->text() << std::endl;
                    //std::cout << "v_aux2: " <<  v_aux2->text() << std::endl;
                    //std::cout << "v_aux3: " <<  v_aux3->text() << std::endl;
                    //std::cout << "v_aux4: " <<  v_aux4->text() << std::endl;
                    //std::cout << "new_vertex: " <<  new_vertex->text() << std::endl;

                    /*bool conect_v1 = false;
                    for (Data::FrontElementList::iterator ite = v_aux3->feBegin(); ite != v_aux3->feEnd(); ite++)
                    {
                        if( (*ite)->have( v_aux1 )  )
                            conect_v1 = true;

                        if( (*ite)->have( v_aux2 )  )
                            conect_v1 = false;

                        //std::cout << "ANTES FE id: " <<  (*ite)->getId() << std::endl;
                        //std::cout << "FE adj: " << (*ite)->text() << std::endl;
                    }*/

                    ///REMOVENDO DA surface_boundary E DELETANDO
                    surfaceMesh->remove( face_in_edge );
                    surfaceMesh->remove( face_in_edge_adj );
                    surfaceMesh->remove( face_in_edge_adj2 );

                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(0) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(1) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(2) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(0) )->remove( face_in_edge_adj );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(1) )->remove( face_in_edge_adj );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(2) )->remove( face_in_edge_adj );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(0) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(1) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(2) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(0) )->remove( face_in_edge_adj2 );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(1) )->remove( face_in_edge_adj2 );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(2) )->remove( face_in_edge_adj2 );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(0) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(1) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(2) )->remove( face_in_edge );

                    ///REMOVENDO AS FACES ANTIGAS DAS ARESTAS
                    MJMesh::Vertex *vlist[5] = {v_test, v_aux1, v_aux2, v_aux3, v_aux4};
                    for (int i = 0; i < 5; i++)
                    {
                        MJMesh::Vertex *v = vlist[i];

                        for (Data::Mesh::FrontElementIterator edgeit = v->feBegin(); edgeit != v->feEnd(); edgeit++)
                        {
                            MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>(*edgeit);

                            if (fe->haveAdjacent(face_in_edge))
                            {
                                fe->remove(face_in_edge);
                            }

                            if (fe->haveAdjacent(face_in_edge_adj))
                            {
                                fe->remove(face_in_edge_adj);
                            }

                            if (fe->haveAdjacent(face_in_edge_adj2))
                            {
                                fe->remove(face_in_edge_adj2);
                            }
                        }
                    }

                    delete face_in_edge;
                    delete face_in_edge_adj;
                    delete face_in_edge_adj2;


                    ///NOVAS FACES - PRINCIPAL
                    MJMesh::ElementSurface *e1 = NULL;
                    MJMesh::ElementSurface *e2 = NULL;
                    MJMesh::ElementSurface *e3 = NULL;
                    MJMesh::ElementSurface *e4 = NULL;
                    MJMesh::ElementSurface *e5 = NULL;


                    ///NOVAS FACES - ADS
                    e1 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[0] ), static_cast<MJMesh::Vertex *>( vertexs[1] ), new_vertex, mesh_surf_idManager->next(2) );
                    e2 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[2] ), static_cast<MJMesh::Vertex *>( vertexs[3] ), new_vertex, mesh_surf_idManager->next(2) );

                    e3 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[4] ), static_cast<MJMesh::Vertex *>( vertexs[5] ), new_vertex, mesh_surf_idManager->next(2) );

                    e4 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[6] ), static_cast<MJMesh::Vertex *>( vertexs[7] ), new_vertex, mesh_surf_idManager->next(2) );
                    e5 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[8] ), static_cast<MJMesh::Vertex *>( vertexs[9] ), new_vertex, mesh_surf_idManager->next(2) );


                    ///ADICIONANDO AS FACES NOVAS NAS ARESTAS
                    MJMesh::ElementSurface *flist[5] = {e1, e2, e3, e4, e5};
                    for( int i = 0; i<10; i+=2 )
                    {
                        MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( vertexs[i] );
                        MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>( vertexs[i+1] );

                        for (Data::Mesh::FrontElementIterator edgeit = v1->feBegin(); edgeit != v1->feEnd(); edgeit++)
                        {
                            MJMesh::FrontElement *fe1 = static_cast<MJMesh::FrontElement *>(*edgeit);

                            for (Data::Mesh::FrontElementIterator edgeit2 = v2->feBegin(); edgeit2 != v2->feEnd(); edgeit2++)
                            {
                                MJMesh::FrontElement *fe2 = static_cast<MJMesh::FrontElement *>(*edgeit2);

                                if( fe1 == fe2 )
                                {
                                    fe1->add( flist[ i/2 ] );
                                    break;
                                }
                            }
                        }
                    }

                    //static_cast<MJMesh::Vertex *>( vertexs[0] )->add(e1);
                    //static_cast<MJMesh::Vertex *>( vertexs[1] )->add(e1);

                    //static_cast<MJMesh::Vertex *>( vertexs[2] )->add(e2);
                    //static_cast<MJMesh::Vertex *>( vertexs[3] )->add(e2);

                    //static_cast<MJMesh::Vertex *>( vertexs[4] )->add(e3);
                    //static_cast<MJMesh::Vertex *>( vertexs[5] )->add(e3);

                    //static_cast<MJMesh::Vertex *>( vertexs[6] )->add(e4);
                    //static_cast<MJMesh::Vertex *>( vertexs[7] )->add(e4);

                    //static_cast<MJMesh::Vertex *>( vertexs[8] )->add(e5);
                    //static_cast<MJMesh::Vertex *>( vertexs[9] )->add(e5);

                    /*if( conect_v1 == true )
                    {
                        std::cout << "IF"  << std::endl;
                        e1 = new MJMesh::ElementSurface( v_test, v_aux1, new_vertex, mesh_surf_idManager->next(1) );
                        e2 = new MJMesh::ElementSurface( v_aux2, v_test, new_vertex, mesh_surf_idManager->next(1) );

                        e3 = new MJMesh::ElementSurface( v_aux1, v_aux3, new_vertex, mesh_surf_idManager->next(1) );

                        e4 = new MJMesh::ElementSurface( v_aux3, v_aux4, new_vertex, mesh_surf_idManager->next(1) );
                        e5 = new MJMesh::ElementSurface( v_aux4, v_aux2, new_vertex, mesh_surf_idManager->next(1) );
                    }
                    else
                    {
                        std::cout << "ELSE"  << std::endl;
                        e1 = new MJMesh::ElementSurface( v_test, v_aux1, new_vertex, mesh_surf_idManager->next(1) );
                        e2 = new MJMesh::ElementSurface( v_aux2, v_test, new_vertex, mesh_surf_idManager->next(1) );

                        e3 = new MJMesh::ElementSurface( v_aux3, v_aux2, new_vertex, mesh_surf_idManager->next(1) );

                        e4 = new MJMesh::ElementSurface( v_aux1, v_aux4, new_vertex, mesh_surf_idManager->next(1) );
                        e5 = new MJMesh::ElementSurface( v_aux4, v_aux3, new_vertex, mesh_surf_idManager->next(1) );
                    }


                    v_test->add(e1);
                    v_aux1->add(e1);

                    v_aux2->add(e2);
                    v_test->add(e2);

                    v_aux2->add(e3);
                    v_aux3->add(e3);

                    v_aux1->add(e4);

                    v_aux3->add(e5);*/

                    surfaceMesh->add( e1 );
                    surfaceMesh->add( e2 );
                    surfaceMesh->add( e3 );
                    surfaceMesh->add( e4 );
                    surfaceMesh->add( e5 );

                    //std::cout << "ele: " <<  ele->text() << std::endl;
                    //std::cout << "ele_pos: " <<  ele_pos->text() << std::endl;
                    //std::cout << "Novo vertice: " <<  new_vertex->text() << std::endl;

                    //std::cout << "Adicionou 1: "  << e1->text() << std::endl;
                    //std::cout << "Adicionou 2: "  << e2->text() << std::endl;
                    //std::cout << "Adicionou 3: "  << e3->text() << std::endl;
                    //std::cout << "Adicionou 4: "  << e4->text() << std::endl;
                    //std::cout << "Adicionou 5: "  << e5->text() << std::endl;
                }
                else                                        ///FACES NÃO TOCAM A BORDA
                {
                    //std::cout << "ELSE: "  << v_test->text() << std::endl;

                    ///REPOSICIONANDO O VERTICE DO CENTRO
                    Real p_pos[3] = {0,0,0};
                    Real area = 0.0;
                    for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                    {
                        MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite2) );

                        area += e->getShape()->size();

                        p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                        p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                        p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();
                    }

                    p_pos[0] /= area;
                    p_pos[1] /= area;
                    p_pos[2] /= area;
                    v_aux3->getPoint()->setCoord(0, p_pos[0]);
                    v_aux3->getPoint()->setCoord(1, p_pos[1]);
                    v_aux3->getPoint()->setCoord(2, p_pos[2]);

                    ///TROCANDO O v_aux2 PELO VERTEX DA QUINA ( v_test ) DO ADJ
                    for(int x=0; x<3; x++)
                    {
                        if( face_in_edge_adj->getVertex(x)->getId() != v_aux1->getId() && face_in_edge_adj->getVertex(x)->getId() != v_aux3->getId() )
                        {
                            v_aux2->remove( face_in_edge_adj );
                            v_test->add(face_in_edge_adj);
                            face_in_edge_adj->replace( face_in_edge_adj->getVertex(x), v_test);
                            //face_in_edge_adj->setVertex(x, v_test);
                        }
                    }

                    ///TROCANDO O v_aux1 PELO VERTEX DA QUINA ( v_aux3 ) DA FACE DE TESTE
                    for(int x=0; x<3; x++)
                    {
                        if( face_in_edge->getVertex(x)->getId() != v_aux2->getId() && face_in_edge->getVertex(x)->getId() != v_test->getId() )
                        {
                            v_aux1->remove( face_in_edge );
                            v_aux3->add(face_in_edge);
                            face_in_edge->replace( face_in_edge->getVertex(x), v_aux3);
                            //face_in_edge->setVertex(x, v_aux3);
                        }
                    }

                }
            }
        }
    }



    /*for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::FrontElement *ele = static_cast<MJMesh::FrontElement *>((*it));
        //MJMesh::Element *element = static_cast<MJMesh::Element *>( ele->getAdjacency().front() );
        MJMesh::Element *element = NULL;

        //std::cout << "= = = = = = = = = = =" << std::endl;
        //std::cout << "ele: " << ele->text() << std::endl;


        ///PROCURANDO A FACE QUE CORRESPONDE A ARESTA ATUAL
        for (Data::Mesh::ElementIterator ite = ele->getV1()->eBegin(); ite != ele->getV1()->eEnd(); ite++)
        {
            if( (*ite)->have( ele->getV1() ) && (*ite)->have( ele->getV2() ) )
            {
                element = static_cast<MJMesh::Element *>( (*ite) );
                break;
            }
        }

        //std::cout << "element: " << element->text() << std::endl;

        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(0)->getId())).second );
        MJMesh::Vertex *v_ori2 = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(1)->getId())).second );
        MJMesh::Vertex *v_dif = NULL;
        for( int x=0; x<3; x++ )
        {
            if( ele->getVertex(0)->getId()!= element->getVertex(x)->getId() && ele->getVertex(1)->getId()!= element->getVertex(x)->getId() )
                v_dif = static_cast<MJMesh::Vertex *>( element->getVertex(x) );
        }



    }*/


    ///IMPRESSÃO
    {
        /*static int ertt = 1;
        std::string as = "/home/nfs/daniel/Desktop/Malha_MeshSurf_Mod";
        as += std::to_string(ertt++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surfaceMesh->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << surfaceMesh->size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile.close();*/
    }

    //std::cout << "Verificação da malha: " << Data::time() - start << std::endl;
    return true;
}

bool PMJMesh2::APrioriParallelMeshGenerator::checkSurfaceMesh( PMJMesh::Front *sub_front, Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary, Performer::IdManager *mesh_surf_idManager, const Data::VertexList &surface_vertexs, const Data::FrontElementList &surface_elements, const Data::VertexHashMap &original_surface_vertexes, int side, Real pos_plane, Data::Point3D p_min, Data::Point3D p_max )
{
    Real start = Data::time();
    //std::cout << " CHECANDO A SUPERFICIE " << std::endl;

    ///IMPRESSÃO
    {
        /*static int ert = 1;
        std::string as = "/home/nfs/daniel/Desktop/Malha_MeshSurf";
        as += std::to_string(ert++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
        if (myfile.is_open())
        {
            myfile << std::setprecision(30) << "2.5" << std::endl ;
            myfile << surfaceMesh->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << surfaceMesh->size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile.close();*/
    }

    //std::unordered_map<ULInt, Data::Vertex*> vertexHash;

    /*for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
    {
        MJMesh::FrontElement2D *ele = ((MJMesh::FrontElement2D *)(*it));

        //std::cout << (*it)->getId() << "  ADJ: " << ele->numAdjacents() << std::endl ;

        if( ele->numAdjacents() == 0 )
        {
            std::cout << ele->getId() << " está solta!"<< std::endl;
            return false;
        }
    }*/

    ///TESTE PARA SABER SE A MALHA FOI GERADA FORA DO MODELO
    if( isElementInside(surfaceMesh->getElements().back()->getShape()->box(), tree->getRoot()) == false )
    {
        std::cout << " GEROU ELEMENTO FORA DO MODELO! " << surfaceMesh->getElements().back()->getId() << std::endl;
        return false;
    }

    ///TESTE PARA SABER SE A MALHA FOI GERADA FORA DO CONTORNO DE ENTRADA
    Data::Box b(p_min, p_max);
    if( b.out( surfaceMesh->getElements().back()->getShape()->box() ) )
    {
        std::cout << " GEROU ELEMENTO FORA DO CONTORNO!!! " << surfaceMesh->getElements().back()->getId() << std::endl;
        return false;
    }

    ///DEVOLVENDO OS VERTICES DA BORDA À POSIÇÃO ORIGINAL
    for (Data::Mesh::VertexIterator it = surface_boundary->vBegin(); it != surface_boundary->vEnd(); it++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>( (*it) );
        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find( v->getId() ) ).second );

        v->getPoint()->setCoords( v_ori->getPoint()->getCoords() );
    }


    for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::FrontElement *ele = static_cast<MJMesh::FrontElement *>((*it));

        ///SE UMA ARESTA DA BORDA TIVER MAIS QUE 1 ADJACENTE - PROCURA REPLICAS E DELETA
        while( ele->getAdjacency().size() != 1 )
        {
            /*MJMesh::Vertex *v_aux1 = ele->getV1();
            MJMesh::Vertex *v_aux2 = ele->getV2();
            bool cont = true;
            for (Data::Mesh::ElementIterator ite2 = v_aux1->eBegin(); ite2 != v_aux1->eEnd(); ite2++)
            {
                for (Data::Mesh::ElementIterator ite3 = v_aux2->eBegin(); ite3 != v_aux2->eEnd(); ite3++)
                {
                    if( (*ite2)->getId() == (*ite3)->getId() )
                    {
                        std::cout << "Mesmo elemento compartilhado: " << ele->text() << std::endl;
                        cont = false;
                    }
                }
            }

            if( cont == false && ele->getAdjacency().size() <= 1 ) break;*/


            //std::cout << "ADJACENCIA DE BORDA ERRADA: " << ele->text() << std::endl;

            int size_adj = ele->getAdjacency().size();

            if( size_adj == 0 || size_adj > 1 )
            {
                //std::cout << " GEROU COM ADJ ERRADA OU ZERADA!" << std::endl;
                return false;
            }

            Data::ElementList list_aux3 = ele->getAdjacency();

            //std::cout << "Size V1: " << ele->getV1()->getAdjacentElements().size() << std::endl;
            //std::cout << "Size V2: " << ele->getV2()->getAdjacentElements().size() << std::endl;

            /*for (Data::ElementList::iterator it = l3.begin(); it != l3.end(); it++)
            {
                std::cout << "VIZINHOS: " << (*it)->getId() << " vertices: " << (*it)->text() << std::endl;
                std::cout << "VIZINHOS END: " << (*it) << std::endl;
            }*/

            for (Data::ElementList::iterator it1 = list_aux3.begin(); it1 != list_aux3.end();)
            {
                MJMesh::ElementSurface *e = static_cast<MJMesh::ElementSurface *>( (*it1) );
                for (Data::ElementList::iterator it2 = ++it1; it2 != list_aux3.end(); it2++)
                {
                    if( e->getId() == (*it2)->getId() )
                    {
                        std::cout << "SÂO DUPLICADOS: " << e->getId() << " e " << (*it2)->getId() << std::endl;
                        ele->remove( static_cast<MJMesh::Element *>( e ) );
                    }
                }
            }

            MJMesh::FrontElement *ele_pos = NULL;
            ++it;
            if( it != surface_boundary->eEnd() )
            {
                ele_pos = static_cast<MJMesh::FrontElement *>((*it));
                if( ele->getAdjacency().size() != 1 && ele_pos->getAdjacency().size() != 1 )
                {
                    bool flag = false;
                    Data::ElementList l1 = ele->getAdjacency();
                    Data::ElementList l2 = ele_pos->getAdjacency();
                    for (Data::ElementList::iterator it = l1.begin(); it != l1.end(); it++)
                    {
                        for (Data::ElementList::iterator it2 = l2.begin(); it2 != l2.end(); it2++)
                        {
                            if ( (*it)->have( (*it2)->getVertex(0) )  && (*it)->have( (*it2)->getVertex(1) ) && (*it)->have( (*it2)->getVertex(2) ) )
                            {
                                if( (*it)->getId() == (*it2)->getId() )
                                {
                                    std::cout << "SÂO IGUAIS: " << (*it)->getId() << " e " << (*it2)->getId() << std::endl;
                                    std::cout << (*it)->text() << std::endl;

                                    ele->remove( static_cast<MJMesh::Element *>((*it)) );
                                    ele_pos->remove( static_cast<MJMesh::Element *>((*it2)) );
                                    surfaceMesh->remove( (*it) );
                                    delete (*it);
                                    flag = true;
                                }
                            }
                            if(flag) break;
                        }
                        if(flag) break;
                    }
                }
            }
            it--;

            if( size_adj == ele->getAdjacency().size() )
            {
                //std::cout << "SEM JEITO: " << ele->text() << std::endl;
                return false;
            }
        }


        ///CADA ARESTA É ADJACENTE A SOMENTE UM ELEMENTO
        MJMesh::Element *element = NULL;

        for (Data::Mesh::ElementIterator ite = ele->getV1()->eBegin(); ite != ele->getV1()->eEnd(); ite++)
        {
            if( (*ite)->have( ele->getV1() ) && (*ite)->have( ele->getV2() ) )
            {
                element = static_cast<MJMesh::Element *>( (*ite) );
                break;
            }
        }


        //std::cout << "Aresta" << ele->text() << std::endl;
        //std::cout << element->getId() << "\t" << element << std::endl;
        //std::cout << "S: "<< ele->getAdjacency().size() << std::endl;

        Data::Triangle3D *t = static_cast<Data::Triangle3D*>( element->getShape() );

        ///SEMPRE PEGO O VERTICE 0 DA ARESTA
        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(0)->getId())).second );
        MJMesh::Vertex *v_test = static_cast<MJMesh::Vertex *>( ele->getVertex(0) );
        MJMesh::Vertex *v_dif = NULL;
        for( int x=0; x<3; x++ )
        {
            if( ele->getVertex(0)->getId()!= element->getVertex(x)->getId() && ele->getVertex(1)->getId()!= element->getVertex(x)->getId() )
                v_dif = static_cast<MJMesh::Vertex *>( element->getVertex(x) );
        }


        MJMesh::FrontElement *ele_pos = NULL;
        bool flag_adj = false;


        ///CHECA SE O VIZINHO DA ARESTA COMPARTILHA UM ELEMENTO COM A ARESTA PRINCIPAL
        for (std::list<Data::FrontElement *>::iterator itt = v_test->feBegin(); itt != v_test->feEnd(); itt++)
        {
            if( (*itt)->getId() == ele->getId() ) continue;

            ele_pos = static_cast<MJMesh::FrontElement *>((*itt));

            //std::cout << " TESTANDO: " << ele->getId() << " com : "  << ele_pos->getId() << std::endl;
            //std::cout << " S: " << ele_pos->getAdjacency().size() << std::endl;
            //std::cout << " ele: " << ele->text() << std::endl;
            //std::cout << " ele_pos: " << ele_pos->text() << std::endl;
            //std::cout << " elemente id: " << element->text() << std::endl;

            MJMesh::ElementSurface *face_in_edge = NULL;

            for (Data::Mesh::ElementIterator ite2 = v_test->eBegin(); ite2 != v_test->eEnd(); ite2++)
            {
                if( (*ite2)->have( ele->getVertex(1) ) && (*ite2)->have( ele_pos->getVertex(0) ) && (*ite2)->have( ele_pos->getVertex(1) ) )
                {
                    face_in_edge = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                    break;
                }
            }

//std::cout << " ele_pos element id: " << ele_pos->getAdjacency().front()->getId() << std::endl;

            if( face_in_edge == NULL ) continue;

            //std::cout << "    COMPARTILHA FACE: " << element->getId() << " com o elemento_pos: "  << ele_pos->getId() << " e elemente id: " << ele->getId() << std::endl;
            //ele_pos = static_cast<MJMesh::FrontElement *>((*it));
            flag_adj = true;


            ///Verifica se a face adj tem um vertice(v_aux3) na borda
            ///Se tiver na borda tem que pegar o seu vizinho, depois deletar as 3 faces
            ///Gerar um vertice que é a media dos baricentros e ligar tudo (5 faces novas)

            ///FACES QUE ESTÃO EM QUINAS SERÃO DIVIDIDAS EM 2 USANDO A SUA FACE ADJ
            if( flag_adj )
            {
                //MJMesh::ElementSurface *face_in_edge = static_cast<MJMesh::ElementSurface *>( ele_pos->getAdjacency().front() );
                MJMesh::ElementSurface *face_in_edge_adj = NULL;       ///primeiro vizinho
                MJMesh::ElementSurface *face_in_edge_adj2 = NULL;      ///Vizinho do vizinho mas compartilha vertice v_aux1 ou v_aux2
                MJMesh::Vertex *v_aux1 = static_cast<MJMesh::Vertex *>( ele->getVertex(1) );
                MJMesh::Vertex *v_aux2 = NULL;
                MJMesh::Vertex *v_aux3 = NULL;
                MJMesh::Vertex *v_aux4 = NULL;

                ///Encontrando v_aux2
                for(int x=0; x<3; x++)
                {
                    if( face_in_edge->getVertex(x)->getId() != v_test->getId() && face_in_edge->getVertex(x)->getId() != v_aux1->getId() )
                    {
                        v_aux2 = static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(x) );
                        continue;
                    }
                }

                //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                //std::cout << "V_TEST: "  << v_test->text() << std::endl;
                //std::cout << "ELE: "  << ele->text() << std::endl;
                //std::cout << "ELEMENT: "  << element->text() << std::endl;
                //std::cout << "ELE_POS: "  << ele_pos->text() << std::endl;
                //std::cout << "FACE DE TESTE face_in_edge: "  << face_in_edge->text() << std::endl;

                ///Encontrando v_aux3 e face_in_edge_adj
                for (Data::Mesh::ElementIterator ite2 = v_aux1->eBegin(); ite2 != v_aux1->eEnd(); ite2++)
                {
                    //std::cout << "TESTANDO: "  << (*ite2)->text() << std::endl;

                    if( (*ite2)->have( v_aux1 ) && (*ite2)->have( v_aux2 ) && !(*ite2)->have( v_test ) )
                    {
                        for(int x=0; x<3; x++)
                        {
                            if( (*ite2)->getVertex(x)->getId() != v_aux1->getId() && (*ite2)->getVertex(x)->getId() != v_aux2->getId() )
                                v_aux3 = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(x) );
                        }

                        //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                        //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                        //std::cout << "V_AUX3: "  << v_aux3->text() << std::endl;
                        //std::cout << "ACHEI FACE face_in_edge_adj: "  << (*ite2)->text() << std::endl;
                        //std::cout << "FACE DE TESTE face_in_edge: "  << face_in_edge->text() << std::endl;

                        face_in_edge_adj = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                    }
                }

                ///MALHA DO MESH SURF CORROMPIDA
                if( v_aux3 == NULL )
                {
                    //std::cout << "MALHA DO MESH SURF CORROMPIDA - v_aux3 NULL V_TEST " << v_test->getId()  << std::endl;
                    return false;
                }

                ///ENCONTRANDO A FACE ADJ ( face_in_edge_adj2 ) DA face_in_edge_adj E O SEU VERTICE ( V_AUX4 )
                for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                {
                    if( ( (*ite2)->have( v_aux1 ) || (*ite2)->have( v_aux2 ) ) && (*ite2)->getId() != face_in_edge_adj->getId() )
                    {
                        for( int p = 0; p < 3; p++ )
                        {
                            if( (*ite2)->getVertex(p)->getId() != v_aux1->getId() &&
                                    (*ite2)->getVertex(p)->getId() != v_aux2->getId() &&
                                    (*ite2)->getVertex(p)->getId() != v_aux3->getId() &&
                                    !surface_boundary->have( (*ite2)->getVertex(p) ) )
                            {
                                face_in_edge_adj2 = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                                v_aux4 = static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(p) );

                                //std::cout << "FACE irmã: "  << face_in_edge_adj2->text() << std::endl;
                                //std::cout << "FACE irmã vertex: "  << v_aux4->text() << std::endl;
                            }
                        }
                    }
                }


                ///MALHA DO MESH SURF CORROMPIDA
                if( v_aux4 == NULL )
                {
                    ///V_AUX4 PODE ESTAR NA BORDA
                    Real d = DBL_MAX;
                    for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                    {
                        if( ( (*ite2)->have( v_aux1 ) || (*ite2)->have( v_aux2 ) ) && (*ite2)->getId() != face_in_edge_adj->getId() )
                        {
                            for( int p = 0; p < 3; p++ )
                            {
                                if( (*ite2)->getVertex(p)->getId() != v_aux1->getId() &&
                                        (*ite2)->getVertex(p)->getId() != v_aux2->getId() &&
                                        (*ite2)->getVertex(p)->getId() != v_aux3->getId() )
                                {
                                    if( d > v_test->getPoint()->distance( (*ite2)->getVertex(p)->getPoint() ) )
                                    {
                                        d = v_test->getPoint()->distance( (*ite2)->getVertex(p)->getPoint() );
                                        v_aux4 = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(p) );
                                        face_in_edge_adj2 = static_cast<MJMesh::ElementSurface *>( (*ite2) );
                                    }
                                }
                            }
                        }
                    }

                    if( v_aux4 == NULL ) return false;
                }

                ///ELEMENTO ADJ ESTÀ TOCANDO NA BORDA
                if( surface_boundary->have( v_aux3 ) )      ///FACES TOCAM A BORDA!!!!
                {
                    //std::cout << "IF: "  << v_test->text() << std::endl;
                    //std::cout << "+++++++++++++++++++++++++++++++++++++++++"  << std::endl;
                    //std::cout << "V_TEST: "  << v_test->text() << std::endl;
                    //std::cout << "V_AUX1: "  << v_aux1->text() << std::endl;
                    //std::cout << "V_AUX2: "  << v_aux2->text() << std::endl;
                    //std::cout << "V_AUX3: "  << v_aux3->text() << std::endl;
                    //std::cout << "V_AUX4: "  << v_aux4->text() << std::endl;
                    //std::cout << "face_in_edge: "  << face_in_edge->text() << std::endl;
                    //std::cout << "face_in_edge_adj: "  << face_in_edge_adj->text() << std::endl;
                    //std::cout << "face_in_edge_adj2: "  << face_in_edge_adj2->text() << std::endl;

                    Data::VertexVector vertexs;

                    vertexs.push_back( v_test );
                    vertexs.push_back( v_aux1 );
                    vertexs.push_back( v_aux2 );
                    vertexs.push_back( v_test );

                    if( face_in_edge_adj2->have( v_aux1 ) )
                    {
                        vertexs.push_back( v_aux3 );
                        vertexs.push_back( v_aux2 );

                        vertexs.push_back( v_aux1 );
                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux3 );
                    }
                    else
                    {
                        vertexs.push_back( v_aux1 );
                        vertexs.push_back( v_aux3 );

                        vertexs.push_back( v_aux4 );
                        vertexs.push_back( v_aux2 );
                        vertexs.push_back( v_aux3 );
                        vertexs.push_back( v_aux4 );
                    }


                    ///CRIAÇÃO DO NOVO VERTICE PELA MEDIA DAS 3 FACES
                    Real p[3] = {0,0,0};
                    p[0] = ( ( face_in_edge->getShape()->mid3D().getCoord(0) + face_in_edge_adj->getShape()->mid3D().getCoord(0) + face_in_edge_adj2->getShape()->mid3D().getCoord(0) ) / 3.0 );
                    p[1] = ( ( face_in_edge->getShape()->mid3D().getCoord(1) + face_in_edge_adj->getShape()->mid3D().getCoord(1) + face_in_edge_adj2->getShape()->mid3D().getCoord(1) ) / 3.0 );
                    p[2] = ( ( face_in_edge->getShape()->mid3D().getCoord(2) + face_in_edge_adj->getShape()->mid3D().getCoord(2) + face_in_edge_adj2->getShape()->mid3D().getCoord(2) ) / 3.0 );


                    /*if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                    {
                        Real v = pos_plane - p->getX();
                        p->setX( p->getX() + v );
                    }
                    if( side == 1 || side == 4 ) ///TOP E BASE
                    {
                        Real v = pos_plane - p->getY();
                        p->setY( p->getY() + v );
                    }
                    if( side == 2 || side == 5 ) ///FRONT E BACK
                    {
                        Real v = pos_plane - p->getZ();
                        p->setZ( p->getZ() + v );
                    }*/


                    MJMesh::Vertex *new_vertex = new MJMesh::Vertex( p[0], p[1], p[2], mesh_surf_idManager->next(0) );
                    surfaceMesh->add(new_vertex);


                    //std::cout << "ele: " <<  ele->text() << std::endl;
                    //std::cout << "ele_pos: " <<  ele_pos->text() << std::endl;
                    //std::cout << "v_test: " <<  v_test->text() << std::endl;
                    //std::cout << "v_aux1: " <<  v_aux1->text() << std::endl;
                    //std::cout << "v_aux2: " <<  v_aux2->text() << std::endl;
                    //std::cout << "v_aux3: " <<  v_aux3->text() << std::endl;
                    //std::cout << "v_aux4: " <<  v_aux4->text() << std::endl;
                    //std::cout << "new_vertex: " <<  new_vertex->text() << std::endl;

                    /*bool conect_v1 = false;
                    for (Data::FrontElementList::iterator ite = v_aux3->feBegin(); ite != v_aux3->feEnd(); ite++)
                    {
                        if( (*ite)->have( v_aux1 )  )
                            conect_v1 = true;

                        if( (*ite)->have( v_aux2 )  )
                            conect_v1 = false;

                        //std::cout << "ANTES FE id: " <<  (*ite)->getId() << std::endl;
                        //std::cout << "FE adj: " << (*ite)->text() << std::endl;
                    }*/

                    ///REMOVENDO DA surface_boundary E DELETANDO
                    surfaceMesh->remove( face_in_edge );
                    surfaceMesh->remove( face_in_edge_adj );
                    surfaceMesh->remove( face_in_edge_adj2 );

                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(0) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(1) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge->getVertex(2) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(0) )->remove( face_in_edge_adj );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(1) )->remove( face_in_edge_adj );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(2) )->remove( face_in_edge_adj );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(0) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(1) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj->getVertex(2) )->remove( face_in_edge );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(0) )->remove( face_in_edge_adj2 );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(1) )->remove( face_in_edge_adj2 );
                    static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(2) )->remove( face_in_edge_adj2 );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(0) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(1) )->remove( face_in_edge );
                    //static_cast<MJMesh::Vertex *>( face_in_edge_adj2->getVertex(2) )->remove( face_in_edge );

                    ///REMOVENDO AS FACES ANTIGAS DAS ARESTAS
                    MJMesh::Vertex *vlist[5] = {v_test, v_aux1, v_aux2, v_aux3, v_aux4};
                    for (int i = 0; i < 5; i++)
                    {
                        MJMesh::Vertex *v = vlist[i];

                        for (Data::Mesh::FrontElementIterator edgeit = v->feBegin(); edgeit != v->feEnd(); edgeit++)
                        {
                            MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>(*edgeit);

                            if (fe->haveAdjacent(face_in_edge))
                            {
                                fe->remove(face_in_edge);
                            }

                            if (fe->haveAdjacent(face_in_edge_adj))
                            {
                                fe->remove(face_in_edge_adj);
                            }

                            if (fe->haveAdjacent(face_in_edge_adj2))
                            {
                                fe->remove(face_in_edge_adj2);
                            }
                        }
                    }

                    delete face_in_edge;
                    delete face_in_edge_adj;
                    delete face_in_edge_adj2;


                    ///NOVAS FACES - PRINCIPAL
                    MJMesh::ElementSurface *e1 = NULL;
                    MJMesh::ElementSurface *e2 = NULL;
                    MJMesh::ElementSurface *e3 = NULL;
                    MJMesh::ElementSurface *e4 = NULL;
                    MJMesh::ElementSurface *e5 = NULL;


                    ///NOVAS FACES - ADS
                    e1 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[0] ), static_cast<MJMesh::Vertex *>( vertexs[1] ), new_vertex, mesh_surf_idManager->next(2) );
                    e2 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[2] ), static_cast<MJMesh::Vertex *>( vertexs[3] ), new_vertex, mesh_surf_idManager->next(2) );

                    e3 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[4] ), static_cast<MJMesh::Vertex *>( vertexs[5] ), new_vertex, mesh_surf_idManager->next(2) );

                    e4 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[6] ), static_cast<MJMesh::Vertex *>( vertexs[7] ), new_vertex, mesh_surf_idManager->next(2) );
                    e5 = new MJMesh::ElementSurface( static_cast<MJMesh::Vertex *>( vertexs[8] ), static_cast<MJMesh::Vertex *>( vertexs[9] ), new_vertex, mesh_surf_idManager->next(2) );


                    ///ADICIONANDO AS FACES NOVAS NAS ARESTAS
                    MJMesh::ElementSurface *flist[5] = {e1, e2, e3, e4, e5};
                    for( int i = 0; i<10; i+=2 )
                    {
                        MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>( vertexs[i] );
                        MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>( vertexs[i+1] );

                        for (Data::Mesh::FrontElementIterator edgeit = v1->feBegin(); edgeit != v1->feEnd(); edgeit++)
                        {
                            MJMesh::FrontElement *fe1 = static_cast<MJMesh::FrontElement *>(*edgeit);

                            for (Data::Mesh::FrontElementIterator edgeit2 = v2->feBegin(); edgeit2 != v2->feEnd(); edgeit2++)
                            {
                                MJMesh::FrontElement *fe2 = static_cast<MJMesh::FrontElement *>(*edgeit2);

                                if( fe1 == fe2 )
                                {
                                    fe1->add( flist[ i/2 ] );
                                    break;
                                }
                            }
                        }
                    }

                    //static_cast<MJMesh::Vertex *>( vertexs[0] )->add(e1);
                    //static_cast<MJMesh::Vertex *>( vertexs[1] )->add(e1);

                    //static_cast<MJMesh::Vertex *>( vertexs[2] )->add(e2);
                    //static_cast<MJMesh::Vertex *>( vertexs[3] )->add(e2);

                    //static_cast<MJMesh::Vertex *>( vertexs[4] )->add(e3);
                    //static_cast<MJMesh::Vertex *>( vertexs[5] )->add(e3);

                    //static_cast<MJMesh::Vertex *>( vertexs[6] )->add(e4);
                    //static_cast<MJMesh::Vertex *>( vertexs[7] )->add(e4);

                    //static_cast<MJMesh::Vertex *>( vertexs[8] )->add(e5);
                    //static_cast<MJMesh::Vertex *>( vertexs[9] )->add(e5);

                    /*if( conect_v1 == true )
                    {
                        std::cout << "IF"  << std::endl;
                        e1 = new MJMesh::ElementSurface( v_test, v_aux1, new_vertex, mesh_surf_idManager->next(1) );
                        e2 = new MJMesh::ElementSurface( v_aux2, v_test, new_vertex, mesh_surf_idManager->next(1) );

                        e3 = new MJMesh::ElementSurface( v_aux1, v_aux3, new_vertex, mesh_surf_idManager->next(1) );

                        e4 = new MJMesh::ElementSurface( v_aux3, v_aux4, new_vertex, mesh_surf_idManager->next(1) );
                        e5 = new MJMesh::ElementSurface( v_aux4, v_aux2, new_vertex, mesh_surf_idManager->next(1) );
                    }
                    else
                    {
                        std::cout << "ELSE"  << std::endl;
                        e1 = new MJMesh::ElementSurface( v_test, v_aux1, new_vertex, mesh_surf_idManager->next(1) );
                        e2 = new MJMesh::ElementSurface( v_aux2, v_test, new_vertex, mesh_surf_idManager->next(1) );

                        e3 = new MJMesh::ElementSurface( v_aux3, v_aux2, new_vertex, mesh_surf_idManager->next(1) );

                        e4 = new MJMesh::ElementSurface( v_aux1, v_aux4, new_vertex, mesh_surf_idManager->next(1) );
                        e5 = new MJMesh::ElementSurface( v_aux4, v_aux3, new_vertex, mesh_surf_idManager->next(1) );
                    }


                    v_test->add(e1);
                    v_aux1->add(e1);

                    v_aux2->add(e2);
                    v_test->add(e2);

                    v_aux2->add(e3);
                    v_aux3->add(e3);

                    v_aux1->add(e4);

                    v_aux3->add(e5);*/

                    surfaceMesh->add( e1 );
                    surfaceMesh->add( e2 );
                    surfaceMesh->add( e3 );
                    surfaceMesh->add( e4 );
                    surfaceMesh->add( e5 );

                    //std::cout << "ele: " <<  ele->text() << std::endl;
                    //std::cout << "ele_pos: " <<  ele_pos->text() << std::endl;
                    //std::cout << "Novo vertice: " <<  new_vertex->text() << std::endl;

                    //std::cout << "Adicionou 1: "  << e1->text() << std::endl;
                    //std::cout << "Adicionou 2: "  << e2->text() << std::endl;
                    //std::cout << "Adicionou 3: "  << e3->text() << std::endl;
                    //std::cout << "Adicionou 4: "  << e4->text() << std::endl;
                    //std::cout << "Adicionou 5: "  << e5->text() << std::endl;
                }
                else                                        ///FACES NÃO TOCAM A BORDA
                {
                    //std::cout << "ELSE: "  << v_test->text() << std::endl;

                    ///REPOSICIONANDO O VERTICE DO CENTRO
                    Real p_pos[3] = {0,0,0};
                    Real area = 0.0;
                    for (Data::Mesh::ElementIterator ite2 = v_aux3->eBegin(); ite2 != v_aux3->eEnd(); ite2++)
                    {
                        MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite2) );

                        area += e->getShape()->size();

                        p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                        p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                        p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();
                    }

                    p_pos[0] /= area;
                    p_pos[1] /= area;
                    p_pos[2] /= area;
                    v_aux3->getPoint()->setCoord(0, p_pos[0]);
                    v_aux3->getPoint()->setCoord(1, p_pos[1]);
                    v_aux3->getPoint()->setCoord(2, p_pos[2]);

                    ///TROCANDO O v_aux2 PELO VERTEX DA QUINA ( v_test ) DO ADJ
                    for(int x=0; x<3; x++)
                    {
                        if( face_in_edge_adj->getVertex(x)->getId() != v_aux1->getId() && face_in_edge_adj->getVertex(x)->getId() != v_aux3->getId() )
                        {
                            v_aux2->remove( face_in_edge_adj );
                            v_test->add(face_in_edge_adj);
                            face_in_edge_adj->replace( face_in_edge_adj->getVertex(x), v_test);
                            //face_in_edge_adj->setVertex(x, v_test);
                        }
                    }

                    ///TROCANDO O v_aux1 PELO VERTEX DA QUINA ( v_aux3 ) DA FACE DE TESTE
                    for(int x=0; x<3; x++)
                    {
                        if( face_in_edge->getVertex(x)->getId() != v_aux2->getId() && face_in_edge->getVertex(x)->getId() != v_test->getId() )
                        {
                            v_aux1->remove( face_in_edge );
                            v_aux3->add(face_in_edge);
                            face_in_edge->replace( face_in_edge->getVertex(x), v_aux3);
                            //face_in_edge->setVertex(x, v_aux3);
                        }
                    }

                }
            }
        }
    }


    ///IMPRESSÃO
    {
        /*static int ertt = 1;
        std::string as = "/home/nfs/daniel/Desktop/Malha_MeshSurf_PRE_Mod";
        as += std::to_string(ertt++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
        if (myfile.is_open())
        {
            myfile << std::setprecision(30) << "2.5" << std::endl ;
            myfile << surfaceMesh->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << surfaceMesh->size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile.close();*/
    }


    for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::FrontElement *ele = static_cast<MJMesh::FrontElement *>((*it));
        //MJMesh::Element *element = static_cast<MJMesh::Element *>( ele->getAdjacency().front() );
        MJMesh::Element *element = NULL;

        //std::cout << "= = = = = = = = = = =" << std::endl;
        //std::cout << "ele: " << ele->text() << std::endl;


        ///PROCURANDO A FACE QUE CORRESPONDE A ARESTA ATUAL
        for (Data::Mesh::ElementIterator ite = ele->getV1()->eBegin(); ite != ele->getV1()->eEnd(); ite++)
        {
            if( (*ite)->have( ele->getV1() ) && (*ite)->have( ele->getV2() ) )
            {
                element = static_cast<MJMesh::Element *>( (*ite) );
                break;
            }
        }

        //std::cout << "element: " << element->text() << std::endl;

        MJMesh::Vertex *v_ori = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(0)->getId())).second );
        MJMesh::Vertex *v_ori2 = static_cast<MJMesh::Vertex *>((*original_surface_vertexes.find(ele->getVertex(1)->getId())).second );
        MJMesh::Vertex *v_dif = NULL;
        for( int x=0; x<3; x++ )
        {
            if( ele->getVertex(0)->getId()!= element->getVertex(x)->getId() && ele->getVertex(1)->getId()!= element->getVertex(x)->getId() )
                v_dif = static_cast<MJMesh::Vertex *>( element->getVertex(x) );
        }


        MJMesh::Vertex *v_adj = NULL;
        MJMesh::Element *element_adj = NULL;
        ///REPOSICIONANDO TODOS OS V_ADJ PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
        for (Data::Mesh::ElementIterator ite2 = v_dif->eBegin(); ite2 != v_dif->eEnd(); ite2++)
        {
            if( (*ite2)->have( ele->getVertex(0) ) || (*ite2)->have( ele->getVertex(1) ) )
            {
                for( int x=0; x<3; x++ )
                {
                    if( (*ite2)->getVertex(x)->getId()!= ele->getVertex(0)->getId() && (*ite2)->getVertex(x)->getId()!= ele->getVertex(1)->getId()
                            && (*ite2)->getVertex(x)->getId()!= v_dif->getId() && !surface_boundary->have( (*ite2)->getVertex(x) ) )
                    {
                        v_adj = static_cast<MJMesh::Vertex *>( (*ite2)->getVertex(x) );
                        element_adj = static_cast<MJMesh::Element *>( (*ite2) );


                        ///REPOSICIONANDO O V_ADJ PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
                        if( v_adj != NULL )
                        {
                            Real p_pos[3] = {0,0,0};
                            Real area = 0.0;
                            for (Data::Mesh::ElementIterator ite = v_adj->eBegin(); ite != v_adj->eEnd(); ite++)
                            {
                                MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite) );

                                /*Data::Vector3D norm;
                                if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                                {
                                    norm.setPosition(1,0,0);
                                }
                                if( side == 1 || side == 4 ) ///TOP E BASE
                                {
                                    norm.setPosition(0,1,0);
                                }
                                if( side == 2 || side == 5 ) ///FRONT E BACK
                                {
                                    norm.setPosition(0,0,1);
                                }
                                Data::Triangle3D *t = static_cast<Data::Triangle3D *>( e->getShape() );*/

                                //area += e->getShape()->size() * fabs(norm.dot( t->normal() ));
                                area += e->getShape()->size();

                                p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                                p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                                p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();

                            }
                            p_pos[0] /= area;
                            p_pos[1] /= area;
                            p_pos[2] /= area;
                            v_adj->getPoint()->setCoord(0, p_pos[0]);
                            v_adj->getPoint()->setCoord(1, p_pos[1]);
                            v_adj->getPoint()->setCoord(2, p_pos[2]);
                        }
                        /*if( v_adj != NULL )
                        {
                            Real p_pos[3] = {0,0,0};
                            for (Data::Mesh::ElementIterator ite = v_adj->eBegin(); ite != v_adj->eEnd(); ite++)
                            {
                                for( int x=0; x<3; x++ )
                                {
                                    if  ( (*ite)->getVertex(x)->getId() == v_adj->getId() )
                                        continue;

                                    p_pos[0] += (*ite)->getVertex(x)->getPoint()->getCoord(0);
                                    p_pos[1] += (*ite)->getVertex(x)->getPoint()->getCoord(1);
                                    p_pos[2] += (*ite)->getVertex(x)->getPoint()->getCoord(2);
                                }
                            }
                            p_pos[0] /= v_adj->getAdjacentElements().size()*2;
                            p_pos[1] /= v_adj->getAdjacentElements().size()*2;
                            p_pos[2] /= v_adj->getAdjacentElements().size()*2;
                            v_adj->getPoint()->setCoord(0, p_pos[0]);
                            v_adj->getPoint()->setCoord(1, p_pos[1]);
                            v_adj->getPoint()->setCoord(2, p_pos[2]);
                        }*/

                    }

                }
            }
        }


        /*if( element->getId() == 72 )
        {
            std::cout << "aresta: " << ele->text() << std::endl;
            std::cout << "Face: " << element->text() << std::endl;
            std::cout << "v_adj: " << v_adj->text() << std::endl;
            std::cout << "v_ori: " << v_ori->text() << std::endl;
            std::cout << "v_ori2: " << v_ori2->text() << std::endl;
            std::cout << "v_dif: " << v_dif->text() << std::endl;
        }*/


        ///REPOSICIONANDO O V_DIF PARA A MEDIA DAS FACES ADJACENTES, CONSIDERANDO A AREA
        if( !surface_boundary->have( v_dif ) )
        {
            Real p_pos[3] = {0,0,0};
            Real area = 0.0;
            for (Data::Mesh::ElementIterator ite = v_dif->eBegin(); ite != v_dif->eEnd(); ite++)
            {
                MJMesh::Element *e = static_cast<MJMesh::Element *>( (*ite) );

                area += e->getShape()->size();

                p_pos[0] += e->getShape()->mid3D().getCoord(0) * e->getShape()->size();
                p_pos[1] += e->getShape()->mid3D().getCoord(1) * e->getShape()->size();
                p_pos[2] += e->getShape()->mid3D().getCoord(2) * e->getShape()->size();

                /*for( int x=0; x<3; x++ )
                {
                    if  ( (*ite)->getVertex(x)->getId() == v_dif->getId() )
                        continue;

                    p_pos[0] += (*ite)->getVertex(x)->getPoint()->getCoord(0);
                    p_pos[1] += (*ite)->getVertex(x)->getPoint()->getCoord(1);
                    p_pos[2] += (*ite)->getVertex(x)->getPoint()->getCoord(2);
                }*/
            }
            p_pos[0] /= area;
            p_pos[1] /= area;
            p_pos[2] /= area;
            v_dif->getPoint()->setCoord(0, p_pos[0]);
            v_dif->getPoint()->setCoord(1, p_pos[1]);
            v_dif->getPoint()->setCoord(2, p_pos[2]);
        }

        //std::cout << "v_ori: " << v_ori->text() << std::endl;
        //std::cout << "v_dif: " << v_dif->text() << std::endl;

        /*if( v_adj != NULL && v_adj->getId() == 49563 )
        {
            std::cout << "aresta: " << ele->text() << std::endl;
            std::cout << "Face: " << element->text() << std::endl;
            std::cout << "v_adj: " << v_adj->text() << std::endl;
            std::cout << "v_ori: " << v_ori->text() << std::endl;
            std::cout << "v_ori2: " << v_ori2->text() << std::endl;
            std::cout << "v_dif: " << v_dif->text() << std::endl;
        }*/


        /*if( element->getId() == 72 )
        {
            static int ertt = 1;
            std::string as = "/home/nfs/daniel/Desktop/teste";
            as += std::to_string(ertt++);
            as += ".gen";
            std::ofstream myfile ( as );

            //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
            if (myfile.is_open())
            {
                myfile << "2.5" << std::endl ;
                myfile << surfaceMesh->verticesSize() << std::endl ;
                for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
                {
                    myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
                }
                myfile << "0" << std::endl;
                myfile << surfaceMesh->size() << std::endl ;
                for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
                {
                    myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
                }
            }
            myfile.close();
        }*/

        //continue;
        ///TESTANDO ANGULO DA FACE DO MESHSURF COM A DA SUPERFÌCIE - SE FOR RUIM DESLOCAR NO SENTIDO DA NORMAL DO V_DIF
        //Real min_dist = DBL_MAX;
        //Data::Point3D p = Data::Point3D( 0.0, 0.0, 0.0 );
        for (Data::FrontElementList::iterator ite = v_ori->feBegin(); ite != v_ori->feEnd(); ite++)
        {
            //if( (*ite)->have( v_ori2 ) && !surface_boundary->have( v_dif ) && sub_front->have( (*ite) ) )
            if( (*ite)->have( v_ori2 ) && !surface_boundary->have( v_dif ) && this->boundary->have( (*ite) ) )
            {
                Data::Triangle3D *triangle = static_cast<Data::Triangle3D *>(element->getShape());
                Data::Triangle3D *triangle2 = static_cast<Data::Triangle3D *>((*ite)->getShape());

                Data::Vector3D normal = triangle->normal();
                Data::Vector3D normal3 = triangle2->normal();
                Data::Vector3D v_dif_normal = Data::Vector3D();

                for (Data::Mesh::ElementIterator ite2 = v_dif->eBegin(); ite2 != v_dif->eEnd(); ite2++)
                {
                    v_dif_normal.sum( static_cast<Data::Triangle3D *>((*ite)->getShape())->normal() );
                }
                v_dif_normal.multiply( 1.0 / v_dif->numAdjacentElements() );

                Real dist = triangle->incenter3D().distance( triangle2->incenter3D() );
                v_dif_normal.multiply( dist*0.1 );  ///Valor corresponde a 10% de dist
                //v_dif_normal.multiply( 1.0 / 10.0 );  ///Valor corresponde a 10% da normal


                //if ( fabs(cos) >= 0.94 )
                while ( fabs( normal.cosAngle(normal3) ) >= 0.93 )
                {
                    v_dif->getPoint()->sum( v_dif_normal );
                    normal = triangle->normal();


                    //if( indo == 18 || indo == 20 )
                    //{
                    //std::cout << "TESTANDO COM FACE: " << (*ite)->text() << std::endl;
                    //std::cout << "Usando: " << v_ori->getId() << " e " << v_ori2->getId() << std::endl;
                    //std::cout << "ANG: " << fabs( normal.cosAngle(normal3) ) << " " << element->text() << " " << (*ite)->text() << std::endl;
                    //std::cout << "v_dif: " << v_dif->text() << std::endl;
                    //std::cout << "normal: " << v_dif_normal.text() << std::endl;
                    //std::cout << "dist: " << dist << std::endl;
                    //std::cout << "p: " << p.text() << std::endl;
                    //}


                    /*Real dist = triangle->incenter3D().distance( triangle2->incenter3D() );
                    if( min_dist > dist )
                    {
                        min_dist = dist;

                        p.setPosition( triangle->incenter3D().getCoord(0) - triangle2->incenter3D().getCoord(0),
                                       triangle->incenter3D().getCoord(1) - triangle2->incenter3D().getCoord(1),
                                       triangle->incenter3D().getCoord(2) - triangle2->incenter3D().getCoord(2) );
                    }*/
                }

            }
        }


        ///CODIGOS ABAIXO ESTÃO GERANDO ERROS NA MALHA - DESCONSIDERAR ELES!!!
        continue;

        ///SE TIVER UMA FACE RUIM COM A SUPERFICIE - AVALIADO ACIMA
        //if( v_adj != NULL && min_dist != DBL_MAX )
        {
            /*if( element->getId() == 72 )
            {
            std::cout << "+++++++++++++++ " << min_dist << std::endl;
            std::cout << "aresta: " << ele->text() << std::endl;
            std::cout << "Face: " << element->text() << std::endl;
            std::cout << "v_ori: " << v_ori->text() << std::endl;
            std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
            //std::cout << "ANG: " << fabs(cos) << " " << element->text() << " " << (*ite)->text() << std::endl;
            std::cout << "Vertice 0: " << ele->getVertex(0)->text() << std::endl;
            std::cout << "Vertice 1: " << ele->getVertex(1)->text() << std::endl;
            std::cout << "v_dif: " << v_dif->text() << std::endl;
            std::cout << "v_adj: " << v_adj->text() << std::endl;
            }*/


            ///element - TROCANDO O v_dif PELO VERTEX ( v_adj ) DO ADJ
            for(int x=0; x<3; x++)
            {
                if( element->getVertex(x)->getId() == v_dif->getId() )
                {
                    v_dif->remove( element );
                    v_adj->add( element );
                    element->replace( element->getVertex(x), v_adj);
                }
            }

            ///element_adj - TROCANDO O ele->getVertex(0) PELO VERTEX ( ele->getVertex(1) ) DA FACE DE TESTE
            for(int x=0; x<3; x++)
            {
                if( element_adj->getVertex(x)->getId() != v_dif->getId() && element_adj->getVertex(x)->getId() != v_adj->getId() )
                {
                    static_cast<MJMesh::Vertex *>( element_adj->getVertex(x) )->remove( element_adj );

                    if( element_adj->getVertex(x)->getId() == ele->getVertex(0)->getId() )
                    {
                        static_cast<MJMesh::Vertex *>( ele->getVertex(1) )->add(element_adj);
                        element_adj->replace( ele->getVertex(0), ele->getVertex(1));
                    }
                    else
                    {
                        static_cast<MJMesh::Vertex *>( ele->getVertex(0) )->add(element_adj);
                        element_adj->replace( ele->getVertex(1), ele->getVertex(0));
                    }
                }
            }

            //break;
        }

        continue;
        ///PROCURANDO COLISÃO COM AS FACES DA SUPERFICIE
        for (Data::FrontElementList::iterator ite = v_ori->feBegin(); ite != v_ori->feEnd(); ite++)
        {
            if( !sub_front->have( (*ite) ) ) continue;

            Data::Triangle3D *t2 = static_cast<Data::Triangle3D*>( (*ite)->getShape() );


            ///VERIFICA SE ALGUM ELEMENTO GERADO INTERCEPTA ALGUM ELEMENTO JA EXISTENTE NA MALHA
            ///SE OCORRER O VERTICE GERADO (V_DIF) SERA TRANSLADADO PARA O BARICENTRO DE SEUS ADJACENTES
            ///(V_DIF) PODE SER DA BORDA E DAR PROBLEMA (NÃO FUNCIONA)
            {
                bool interception = false;
                Data::Point3D p = Data::Point3D( 0.0, 0.0, 0.0 );
                Data::Point3D p2 = Data::Point3D( 0.0, 0.0, 0.0 );


                for (Data::ElementList::iterator itt = v_dif->eBegin(); itt != v_dif->eEnd(); itt++)
                {

                    Data::Triangle3D *t3 = static_cast<Data::Triangle3D*>( (*itt)->getShape() );


                    //std::cout << "test element ori: " << (*ite)->text() << std::endl;
                    //std::cout << "test element v_dif: " << (*itt)->text() << std::endl;



                    double Bx = ((*itt)->getVertex(0)->getPoint()->getCoord(0) + (*itt)->getVertex(1)->getPoint()->getCoord(0) + (*itt)->getVertex(2)->getPoint()->getCoord(0)) / 3.0;
                    double By = ((*itt)->getVertex(0)->getPoint()->getCoord(1) + (*itt)->getVertex(1)->getPoint()->getCoord(1) + (*itt)->getVertex(2)->getPoint()->getCoord(1)) / 3.0;
                    double Bz = ((*itt)->getVertex(0)->getPoint()->getCoord(2) + (*itt)->getVertex(1)->getPoint()->getCoord(2) + (*itt)->getVertex(2)->getPoint()->getCoord(2)) / 3.0;

                    p.setX( p.getX() + Bx );
                    p.setY( p.getY() + By );
                    p.setZ( p.getZ() + Bz );

                    for( int i = 0; i < 3; i++ )
                    {
                        if( !element->have( (*itt)->getVertex(i) ) && (*itt)->getId() != element->getId() )
                        {
                            p2.setX( (*itt)->getVertex(i)->getPoint()->getCoord(0) );
                            p2.setY( (*itt)->getVertex(i)->getPoint()->getCoord(1) );
                            p2.setZ( (*itt)->getVertex(i)->getPoint()->getCoord(2) );
                        }
                    }

                    if( t2->eintercept( t3 ) )
                    {
                        interception = true;
                        std::cout << "44444444444444444444444444444444444444444444444444444444 VDIF: " << v_dif->getId() << std::endl;
                        std::cout << "ele: " << ele->text() << std::endl;
                        std::cout << "element: " << element->text() << std::endl;
                        std::cout << "VERIFICA: " << (*itt)->getId() << " com o elemento: "  << (*ite)->getId() << std::endl;
                        std::cout << "1: " << (*itt)->text() << std::endl;
                        std::cout << "2: " << (*ite)->text() << std::endl;
                        std::cout << "TOCA? " << t2->eintercept( t3 ) << std::endl;
                        std::cout << "44444444444444444444444444444444444444444444444444444444"  << std::endl;
                    }
                }

                if( interception )
                {
                    p.setX( p.getX() / double( v_dif->getAdjacentElements().size() ) );
                    p.setY( p.getY() / double( v_dif->getAdjacentElements().size() ) );
                    p.setZ( p.getZ() / double( v_dif->getAdjacentElements().size() ) );

                    p.setX( ( p.getX() + p2.getX() ) / 2.0 );
                    p.setY( ( p.getY() + p2.getY() ) / 2.0 );
                    p.setZ( ( p.getZ() + p2.getZ() ) / 2.0 );

                    v_dif->getPoint()->setCoord(0, p.getX());
                    v_dif->getPoint()->setCoord(1, p.getY());
                    v_dif->getPoint()->setCoord(2, p.getZ());
                    //std::cout << "NEW POS: " << v_dif->text( ) << std::endl;
                    break;
                }
            }


            ///VERIFICA SE O ELEMENTO GERADO TOCA EM UM ELEMENTO JA EXISTENTE
            /*if ( t->eintercept( t2 ) )
            {
                std::cout << "Elemento: " << element->getId() << " TOCOU o elemento: "  << (*ite)->getId() << std::endl;
                Data::FrontElement *f = (*ite);

                //std::cout << v_test->getId() << " ADJs: "  << v_test->getAdjacentElements().size() << std::endl;

                //std::cout << "==="  << std::endl;
                //std::cout << "Ele1: "  << element->text() << std::endl;

                for (Data::Mesh::ElementIterator i = v_test->eBegin(); i != v_test->eEnd(); i++)
                {
                    MJMesh::Element *e = static_cast<MJMesh::Element *>((*i));
            //std::cout << "Ele2: "  << e->text() << std::endl;
                    if( element->isAdjacent(e) && e->getId() != element->getId() )
                    {
                        //std::cout << "Elemento: " << element->getId() << " TOCOU o elemento: "  << (*ite)->getId() << std::endl;
                        //std::cout << "Ele2: "  << e->text() << std::endl;

                        ///VERTICE QUE FAZ PAR COM v_test
                        MJMesh::Vertex *v_1 = static_cast<MJMesh::Vertex *>( ele->getVertex( 1 ) );
                        MJMesh::Vertex *v_2 = NULL;
                        MJMesh::Vertex *v_3 = NULL;

                        toca = true;

                        for(int x=0; x<3; x++)
                        {
                            if( !element->have( e->getVertex(x) ) )
                            {
                                v_2 = static_cast<MJMesh::Vertex *>(e->getVertex(x));
                                break;
                            }
                        }

                        for(int x=0; x<3; x++)
                        {
                            if( element->getVertex(x)->getId() != v_1->getId() && element->getVertex(x)->getId() != v_test->getId() )
                            {
                                v_3 = static_cast<MJMesh::Vertex *>(element->getVertex(x));

                                element->replace( element->getVertex(x), v_2);

                                //element->setVertex( x, v_2 );

                                break;
                            }
                        }

                        for(int x=0; x<3; x++)
                        {
                            if( e->getVertex(x)->getId() != v_2->getId() && e->getVertex(x)->getId() != v_3->getId() )
                            {
                                e->replace( e->getVertex(x), v_1);

                                //e->setVertex( x, v_1 );

                                //ele_pos->remove(element);
                                //ele_pos->add(e);

                                break;
                            }
                        }

            //std::cout << " v1: "  << v_1->getId() << std::endl;
            //std::cout << " v2: "  << v_2->getId() << std::endl;
            //std::cout << " v3: "  << v_3->getId() << std::endl;
            //std::cout << "Ele1: "  << element->text() << std::endl;
            //std::cout << "Ele2: "  << e->text() << std::endl;

                        //break;
                    }
                }


                //#pragma omp critical
                //toca = true;
            }*/
        }
    }


    ///TESTE DE COLISÃO - VALIDA AS FACES QUE PERTENCEM A BORDA
    /*for (std::list<Data::FrontElement *>::iterator it=surface_boundary->eBegin(); it!=surface_boundary->eEnd(); ++it)
    {
        MJMesh::FrontElement *aresta = static_cast<MJMesh::FrontElement *>((*it));

        Data::ElementList list_ele = aresta->getAdjacency();

        //std::cout << "ARESTA: " << aresta->getId() << "  " << list_ele.size() << std::endl;
        //std::cout << "ARESTA: " << aresta->text() << std::endl;

        if( list_ele.size() > 1 )
        {
            //std::cout << "Aresta com mais de 1 adjacente! " << aresta->getId()  << std::endl;
            return false;
        }

        for (Data::ElementList::iterator itt1=list_ele.begin(); itt1!=list_ele.end(); itt1++)
        {
            //std::cout << "E1: " << (*itt1)->text() << std::endl;
            MJMesh::ElementSurface *e1 = static_cast<MJMesh::ElementSurface *>( (*itt1) );
            Data::Triangle3D *t1 = static_cast<Data::Triangle3D*>( e1->getShape() );

            //std::cout << "E: " << e1->text() << std::endl;

            Data::ElementList list_adj = static_cast<MJMesh::Vertex *>( e1->getVertex(0) )->getAdjacentElements();
            list_adj.insert(list_adj.end(), static_cast<MJMesh::Vertex *>( e1->getVertex(1) )->getAdjacentElements().begin(), static_cast<MJMesh::Vertex *>( e1->getVertex(1) )->getAdjacentElements().end());
            list_adj.insert(list_adj.end(), static_cast<MJMesh::Vertex *>( e1->getVertex(2) )->getAdjacentElements().begin(), static_cast<MJMesh::Vertex *>( e1->getVertex(2) )->getAdjacentElements().end());

            //std::cout << "ELE1: " << list_ele.size() << std::endl;
            //std::cout << "ELE2: " << list_adj.size() << std::endl;

            //if( e1->getVertex(0)->getId() != 18722 && e1->getVertex(1)->getId() != 18722 && e1->getVertex(2)->getId() != 18722  )
            //continue;

            //for (std::list<Data::Element*>::const_iterator itt2=surfaceMesh->eBegin(); itt2!=surfaceMesh->eEnd(); itt2++)
            for (Data::ElementList::iterator itt2 = list_adj.begin(); itt2!=list_adj.end(); itt2++)
            {
                MJMesh::ElementSurface *e2 = static_cast<MJMesh::ElementSurface *>( (*itt2) );
                Data::Triangle3D *t2 = static_cast<Data::Triangle3D*>( e2->getShape() );

                if(e1 == e2)
                    continue;

                //std::cout << "E1: " << e1->text() << std::endl;
                //std::cout << "E2: " << e2->text() << std::endl;

                if( t1->eintercept(t2) || t1-> intercept(t2) )//|| e1->intercept(e2) )
                {
                    //std::cout << "Elementos Auto Colidindo! " << e1->getId() << " com " << e2->getId() << std::endl;
                    return false;
                }
            }

        }
    }*/


    ///IMPRESSÃO
    {
        /*static int ertt = 1;
        std::string as = "/home/nfs/daniel/Desktop/Malha_MeshSurf_Mod";
        as += std::to_string(ertt++);
        as += ".gen";
        std::ofstream myfile ( as );

        //std::ofstream myfile ("/home/nfs/daniel/Desktop/Malha_MeshSurf.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << surfaceMesh->verticesSize() << std::endl ;
            for (Data::Front::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << surfaceMesh->size() << std::endl ;
            for (std::list<Data::Element*>::const_iterator it=surfaceMesh->eBegin(); it!=surfaceMesh->eEnd(); ++it)
            {
                myfile << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
            }
        }
        myfile.close();*/
    }

    //std::cout << "Verificação da malha: " << Data::time() - start << std::endl;
    return true;
}

void PMJMesh2::APrioriParallelMeshGenerator::clearSurfaceMesh( Data::Mesh *surfaceMesh, MJMesh::Boundary *surface_boundary )
{
    bool flag = true;

    while(flag)
    {
        flag = false;
        for (Data::Mesh::VertexIterator it = surfaceMesh->vBegin(); it != surfaceMesh->vEnd(); it++ )
        {
            if( !surface_boundary->have( (*it) ) )
            {
                //std::cout << "removeu " << (*it)->getId() << " em " << (*it) << std::endl;
                surfaceMesh->remove( (*it) );
                delete (*it);
                flag = true;
                break;
            }
        }
    }
    surfaceMesh->clear();
}

void PMJMesh2::APrioriParallelMeshGenerator::cleanOutFaces(PMJMesh::Front *sub_front, const Data::VertexHashMap &surface_vertexes_map)
{
    Data::FrontElementSet fels;

    fels.insert(sub_front->eBegin(2), sub_front->eEnd(2));
    fels.insert(sub_front->eBegin(3), sub_front->eEnd(3));

    while (!fels.empty())
    {
        Data::FrontElement *fel = *fels.begin();

        fels.erase(fels.begin());

        bool must_erase = false;

        //Data::cout() << "cleanOutFaces testing fel " << fel->text() << " in state " << static_cast<UInt>(sub_front->getState(fel)) << Data::endl;

        UInt numVertices = fel->numVertices();

        for (UInt i = 0; i < numVertices; i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v)->getAdjacentFrontElements();

            Data::Vertex *v2 = fel->getVertex((i + 1)%numVertices);

            Data::FrontElement *adj = NULL;

            for (Data::FrontElementList::iterator iter2 = adjs.begin(); iter2 != adjs.end(); iter2++)
            {
                Data::FrontElement *other = (*iter2);

                if ((other == fel) || (!other->have(v2)) || (!sub_front->have(other)))
                {
                    continue;
                }

                if (adj) break;

                adj = other;

                //std::cout << " " << fel->getId() << " at " << fel << " found adj " << adj->getId() << " at vertex " << v->getId() << std::endl ;
            }

            if (!adj)
            {
                must_erase = true;

                break;
            }
        }

        if(!must_erase)
        {
            //Data::cout() << "nothing to do for fel " << fel->text() << Data::endl;

            continue;
        }

        //Data::cout() << "erasing fel " << fel->text() << Data::endl;

        sub_front->remove(fel);

        for (UInt i = 0; i < numVertices; i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v)->getAdjacentFrontElements();

            bool haveAny = false;

            while (!adjs.empty())
            {
                Data::FrontElement *adj = adjs.front();

                adjs.pop_front();

                if (adj == fel)
                {
                    continue;
                }

                if (!sub_front->have(adj))
                {
                    continue;
                }

                haveAny = true;

                fels.insert(adj);
            }

            if (!haveAny)
            {
                sub_front->remove(v);
            }
        }
    }

    return;

    bool erasedAny = true;

    while(erasedAny)
    {

        #pragma omp parallel shared(erasedAny, sub_front) num_threads(2)
        {
            erasedAny = false;

            #pragma omp for
            for( UShort t = 0; t <= 3; t++ )
            {
                for( Data::Front::FrontElementIterator it = sub_front->eBegin(t); it != sub_front->eEnd(t); )
                {
                    /*if( t == 0 && !sub_front->have( (*it), 3 ) )        ///PROCURA ELEMENTO DA BORDA NA CROSS
                    {
                        it++;
                        continue;
                    }*/

                    bool must_erase = false;

                    Data::FrontElement *fel = (*it);

                    //std::cout << " REMOÇÃO 1 " << fel->getId() << " at " << fel << std::endl ;
                    //Data::cout() << "cleanOutFaces testing fel " << fel->text() << " in state " << static_cast<UInt>(t) << Data::endl;

                    UInt numVertices = fel->numVertices();

                    /*for (Data::Front::FrontElementIterator iter = sub_front->eBegin(0); iter != sub_front->eEnd(0); )
                    {
                        if( (*iter)->have( fel->getVertex(0)->getPoint() ) && (*iter)->have( fel->getVertex(1)->getPoint() ) && (*iter)->have( fel->getVertex(2)->getPoint() ) )
                        {
                            std::cout << " REMOÇÃO " << fel->getId() << " at " << fel << std::endl ;
                            std::cout << " Ori " << (*iter)->getId() << std::endl ;
                            erasedAny = true;
                            iter = sub_front->erase(iter, 0);

                            for (UInt i = 0; i < numVertices; i++)
                            {
                                Data::Vertex *v = (*iter)->getVertex(i);

                                Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v)->getAdjacentFrontElements();

                                if (!sub_front->haveAny(adjs))
                                {
                                    sub_front->remove(v);
                                }
                            }
                            break;
                        }
                        else
                            iter++;
                    }*/


                    for (UInt i = 0; i < numVertices; i++)
                    {
                        Data::Vertex *v = fel->getVertex(i);

                        Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v)->getAdjacentFrontElements();

                        Data::Vertex *v2 = fel->getVertex((i + 1)%numVertices);

                        Data::FrontElement *adj = NULL;

                        for (Data::FrontElementList::iterator iter2 = adjs.begin(); iter2 != adjs.end(); iter2++)
                        {
                            Data::FrontElement *other = (*iter2);

                            if ((other == fel) || (!other->have(v2)) || (!sub_front->have(other)))
                            {
                                continue;
                            }

                            if (adj) break;

                            adj = other;

                            //std::cout << " " << fel->getId() << " at " << fel << " found adj " << adj->getId() << " at vertex " << v->getId() << std::endl ;
                        }

                        if (!adj)
                        {
                            must_erase = true;

                            erasedAny = true;

                            break;
                        }
                    }

                    if(!must_erase)
                    {
                        //Data::cout() << "nothing to do for fel " << fel->text() << Data::endl;

                        it++;

                        continue;
                    }

                    //it = sub_front->erase(it, 3);
//int tid = omp_get_thread_num();
//Data::cout() << "Hello World from thread = " << tid << " T " << t << Data::endl;
//std::cout << " REMOÇÃO 2 " << fel->getId() << " at " << fel << " T " << t << std::endl ;

                    #pragma omp critical
                    {
                        //Data::cout() << "erasing fel " << fel->text() << Data::endl;

                        it = sub_front->erase(it, t);
                    }
                    for (UInt i = 0; i < numVertices; i++)
                    {
                        Data::Vertex *v = fel->getVertex(i);

                        Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v)->getAdjacentFrontElements();

                        if (!sub_front->haveAny(adjs))
                        {
                            #pragma omp critical
                            {
                                sub_front->remove(v);
                            }
                        }
                    }

                }
            }
        }///OMP
    }


    /*for (std::unordered_map<ULInt, Data::Vertex*>::iterator it = surface_vertexes_map.begin(); it != surface_vertexes_map.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << std::endl;
        MJMesh::Vertex *v = ((MJMesh::Vertex*)it->second);

        std::cout << "numAdjacentFrontElements: " << v->numAdjacentFrontElements() << std::endl;

        Data::FrontElementList list_adj = v->getAdjacentFrontElements();

        for(std::FrontElementList::iterator ite = list_adj.begin();  ite != list_adj.end(); ++ite)
        {
            MJMesh::FrontElement * ele = ((MJMesh::FrontElement*)(*ite));

            std::cout << ele->getId() << " ==== numAdjacents: " << ele->getAdjacency().size() << std::endl;
            Data::FrontElement * e;
            e->
        }
    }*/


}

///************************ 3D ************************
void PMJMesh2::APrioriParallelMeshGenerator::buildSubDomain3D(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, ULInt taskId, UInt side, bool updateBox)
{
    //Data::cout() << "------------------------------ Build SubDomain 3D da Partição " << taskId << " ------------------------------" << std::endl;
    Real start = Data::time();

    /*
    #pragma omp critical
    {
        std::cout << "----------- ANTES --------------" << std::endl;
        std::cout << "Na subfront em " << sub_front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< sub_front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = sub_front->eBegin(i); it != sub_front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << sub_front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = sub_front->vBegin(); it != sub_front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }

        std::cout << "na front em " << this->front << std::endl;

        for (UShort i = 0; i < 4; i++)
        {
            std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< this->front->size(i) << std::endl;

            for( Data::Front::FrontElementIterator it = this->front->eBegin(i); it != this->front->eEnd(i); it++ )
            {
                std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl ;
            }
        }

        std::cout << "verts: " << this->front->verticesSize() << std::endl;

        for (Data::Front::VertexIterator it = this->front->vBegin(); it != this->front->vEnd(); it++)
        {
            std::cout << " " << (*it)->getId() << " at " << (*it) << std::endl;
        }
    }*/

    Data::Box plane;
    Data::Triangle3D box_plane;

    bool invert_SupportMesh = false;
    bool invert_Mesh = false;

    switch(side)
    {
    ///DIREITA
    case 0:
    {
        //std::cout << "DIREITA!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMax()->getCoord(0), box->getMin()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMax()->getCoord(0), box->getMax()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMax()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);

        //invert_SupportMesh = true;
        invert_Mesh = true;
    }
    break;

    ///TOP
    case 1:
    {
        //std::cout << "TOP!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMin()->getCoord(0), box->getMax()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMax()->getCoord(0), box->getMax()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMin()->getCoord(0), box->getMax()->getCoord(1), box->getMax()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);

        invert_SupportMesh = true;
        invert_Mesh = true;
    }
    break;

    ///FRONT
    case 2:
    {
        //std::cout << "FRONT!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMax()->getCoord(0), box->getMax()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMax()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);
        //box_plane.setPoints(pmax, pmed, pmin);

        //invert_SupportMesh = true;
        invert_Mesh = true;
    }
    break;

    ///ESQUERDA
    case 3:
    {
        //std::cout << "ESQUERDA!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMin()->getCoord(0), box->getMax()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);

        //invert_SupportMesh = true;
        //invert_Mesh = true;
    }
    break;

    ///BASE
    case 4:
    {
        //std::cout << "BASE!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMax()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMax()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);

        invert_SupportMesh = true;
        //invert_Mesh = true;
    }
    break;

    ///BACK
    case 5:
    {
        //std::cout << "BACK!" << std::endl;
        Data::Point *pmin = new Data::Point3D( box->getMin()->getCoord(0), box->getMin()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMin( pmin );
        Data::Point *pmax = new Data::Point3D( box->getMax()->getCoord(0), box->getMax()->getCoord(1), box->getMin()->getCoord(2) );
        plane.setMax( pmax );

        Data::Point *pmed = new Data::Point3D( box->getMax()->getCoord(0), box->getMin()->getCoord(1), box->getMin()->getCoord(2) );

        box_plane.setPoints(pmin, pmed, pmax);

        //invert_SupportMesh = true;
        //invert_Mesh = true;
    }
    break;
    }

    //std::cout << "BOX: max: " << box->getMax()->text() << " , min: " << box->getMin()->text() << std::endl;
    //std::cout << "PLANE min: " << plane.getMin()->getCoord(0) << "  " << plane.getMin()->getCoord(1) << "  " << plane.getMin()->getCoord(2) << std::endl;
    //std::cout << "PLANE max: " << plane.getMax()->getCoord(0) << "  " << plane.getMax()->getCoord(1) << "  " << plane.getMax()->getCoord(2) << std::endl;

    Data::VertexList surface_vertexs;
    Data::FrontElementList surface_elements;
    Data::VertexHashMap original_surface_vertexes;


    ///CRIANDO ARQUIVO DAS FACES CROSS ANTES
    {
        /*std::ofstream myfile ("/home/nfs/daniel/Desktop/saida_cross.gen");
        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile <<  sub_front->size(3) * 3 << std::endl;
            int o = 1;
            for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
            {
                myfile << o++ << " "  << (*it)->getVertex(0)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(0)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(0)->getPoint()->getCoord(2) << std::endl;
                myfile << o++ << " "  << (*it)->getVertex(1)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(1)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(1)->getPoint()->getCoord(2) << std::endl;
                myfile << o++ << " "  << (*it)->getVertex(2)->getPoint()->getCoord(0) << " "  << (*it)->getVertex(2)->getPoint()->getCoord(1) << " "  << (*it)->getVertex(2)->getPoint()->getCoord(2) << std::endl;
            }
            myfile <<  0 << std::endl;
            myfile << sub_front->size(3) << std::endl;
            o=1;
            for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
            {
                myfile << (*it)->getId() << " 3 " << o++ << " " << o++ << " " << o++ << std::endl;
            }
        }
        myfile.close();*/
    }

    ///CRIANDO ARQUIVO DOS VERTICES
    {
        /*std::ofstream myfile ("/home/nfs/daniel/Desktop/entrada_Vertexs.gen");
        if (myfile.is_open())
        {
            Data::VertexList lista_vertexs = sub_front->getVertices();
            for (std::list<Data::Vertex*>::iterator it = lista_vertexs.begin(); it != lista_vertexs.end(); it++)

            {
                myfile << (*it)->text() << std::endl;
            }
        }
        myfile.close();*/
    }

    ///ACHA, MONTA e CORRIGE AS ARESTAS E VERTICES DA BORDA
    //std::cout << "Build SubDomain 3D da Partição " << taskId << " ------------------------------" << std::endl;
    createSurfaceBoundary(sub_front, surface_vertexs, surface_elements, original_surface_vertexes, box_plane, side);
    Real time_createSurfaceBoundary = Data::time();

    //std::cout << "sizes of sub_front after createSurfaceBoundary: " << sub_front->size(0) << ", " << sub_front->size(1) << ", " << sub_front->size(2) << ", " << sub_front->size(3) << std::endl;

    Data::ElementList internal_elements;
    Data::VertexList internal_vertexs;

    std::list<Data::Point3D> int_points;


    ///ACHA OS VERTICIES INTERNOS
    interceptPartitionFront( box, plane, side%this->dimension, tree->getRoot(), int_points);


    ///REMOVENDO OS PONTOS REPETIDOS DO int_points
    int_points.sort( sort_points );
    //std::cout << "ANTES 1: " << int_points.size() << std::endl;
    int_points.unique( same_point2 );
    std::list<Data::Point3D>::iterator it = int_points.begin();
    int idV = 0;
    while( it != int_points.end() )
    {
        Data::Point3D p1 = (*it);
        it++;

        MJMesh::Vertex *v = new MJMesh::Vertex( p1.getCoord(0), p1.getCoord(1), p1.getCoord(2), idV++ );
        internal_vertexs.push_back( v );
    }
    int_points.clear();

    //std::cout<< "antes 2: " << internal_vertexs.size() << std::endl;

    ///INSERINDO VERTICES DO ANEL DE ARESTAS
    for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
    {
        MJMesh::Vertex *v = NULL;

        if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            v = new MJMesh::Vertex( internal_vertexs.front()->getPoint()->getCoord(0), (*it)->getPoint()->getCoord(1), (*it)->getPoint()->getCoord(2), idV++ );
        if( side == 1 || side == 4 ) ///TOP E BASE
            v = new MJMesh::Vertex( (*it)->getPoint()->getCoord(0), internal_vertexs.front()->getPoint()->getCoord(1), (*it)->getPoint()->getCoord(2), idV++ );
        if( side == 2 || side == 5 ) ///FRONT E BACK
            v = new MJMesh::Vertex( (*it)->getPoint()->getCoord(0), (*it)->getPoint()->getCoord(1), internal_vertexs.front()->getPoint()->getCoord(2), idV++ );

        internal_vertexs.push_back( v );
    }

    //std::cout<< "antes 3: " << internal_vertexs.size() << std::endl;

    ///CRIAÇAO .GEN
    {
        /*std::string as = "/home/nfs/daniel/Desktop/entrada_Triangle";
        as += ".gen";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << internal_vertexs.size() << std::endl ;
            for (std::list<Data::Vertex*>::const_iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << "0" << std::endl ;
        }
        myfile.close();*/
    }

    ///GERAÇÂO DA GRADE POR Delaunay - Shewchuk
    PMJMesh2::TriangleExecutor *triangleExecutor = new PMJMesh2::TriangleExecutor( internal_vertexs, invert_SupportMesh );
    triangleExecutor->execute();

    internal_vertexs = triangleExecutor->getvertexs();
    internal_elements = triangleExecutor->getElements();

    //std::cout<< "depois: " << internal_vertexs.size() << std::endl;
    //std::cout << "internal elements size: " << internal_elements.size() << std::endl;


    Real time_interceptPartitionFront = Data::time();

    //std::cout << "internal size: " << internal_vertexs.size() << std::endl;

    //std::cout << "\t TEMPO interceptPartitionFront: " << time_interceptPartitionFront - time_createSurfaceBoundary << std::endl;

    /*std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "TAMANHO E " << surface_elements.size() << std::endl;
    std::cout << "TAMANHO V " << surface_vertexs.size() << std::endl;
    std::cout << "TAMANHO INT E " << internal_elements.size() << std::endl;
    std::cout << "TAMANHO INT V " << internal_vertexs.size() << std::endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;*/


    ///IMPRESSÂO .GEN
    {
        /*std::cout << "surface mesh" << std::endl;
        std::cout << "2.5" << std::endl ;
        std::cout << internal_vertexs.size() << std::endl ;
        for (std::list<Data::Vertex*>::iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
        {
            std::cout << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
        }
        std::cout << "0" << std::endl;
        std::cout << internal_elements.size() << std::endl ;
        for (std::list<Data::Element*>::iterator it=internal_elements.begin(); it!=internal_elements.end(); ++it)
        {
            std::cout << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
        }

        std::cout << "surface boundary" << std::endl;
        std::cout << "2.5" << std::endl ;
        std::cout << surface_vertexs.size() << std::endl ;
        for (Data::Front::VertexIterator it = surface_vertexs.begin(); it != surface_vertexs.end(); it++)
        {
            std::cout << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
        }
        std::cout << surface_elements.size() << std::endl ;
        for (std::list<Data::FrontElement *>::iterator it=surface_elements.begin(); it!=surface_elements.end(); ++it)
        {
            std::cout << (*it)->getId() << " 2 " << ((MJMesh::FrontElement2D*)(*it))->getV1()->getId() << " "
                                      << ((MJMesh::FrontElement2D*)(*it))->getV2()->getId() << std::endl ;
        }
        std::cout << "0" << std::endl;*/
    }

    ///PARTE DO MESH SURF 3D
    Real meshSurf_Processing = 0.0;
    createInterface3D(sub_front, local_front, common_vertices_list, original_surface_vertexes, surface_vertexs, surface_elements, internal_vertexs, internal_elements, id_Manager, taskId, invert_Mesh, side, meshSurf_Processing);
    Real time_createInterface3D = Data::time();

    //std::cout << "sizes of sub_front after createInterface3D: " << sub_front->size(0) << ", " << sub_front->size(1) << ", " << sub_front->size(2) << ", " << sub_front->size(3) << std::endl;


    ///IMPRESSÃO DO NOVO SUBDOMÍNIO PROCESSADO
    {
        /*Data::VertexList vertices = sub_front->getVertices();
        Data::FrontElementList elements = sub_front->getAllElements();
        elements.insert(elements.end(), sub_front->eBegin(3), sub_front->eEnd(3));

        std::string as = "/home/nfs/daniel/Desktop/particao_crua";
        as += std::to_string(taskId);
        as += ".gen";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << "2.5" << std::endl ;
            myfile << vertices.size() << std::endl ;
            for (Data::Front::VertexIterator it = vertices.begin(); it != vertices.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << "0" << std::endl;
            myfile << elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=elements.begin(); it!=elements.end(); ++it)
            {
                myfile << (*it)->getId() << " 3 " << ((MJMesh::FrontElement3D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement3D*)(*it))->getV2()->getId()<< " " << ((MJMesh::FrontElement3D*)(*it))->getV3()->getId() << std::endl ;
            }
        }
        myfile.close();*/
    }


    ///DELETAR FACES CROSS NÃO UTILIADAS
    cleanOutFaces(sub_front, original_surface_vertexes);
    Real time_cleanOutFaces = Data::time();



    ///VERIFICA SE A PARTIÇÃO FOI COMPLETAMENTE LIMPA
    //if( sub_front->size(0) == 0 || sub_front->size(3) == 0 )
    if( sub_front->size(3) == 0 )
    {
        Data::cout() << "ERROR ON PARTITION " << taskId << "!!! Faces deletadas!" << std::endl;

        return;
    }

    //std::cout << "sizes of sub_front after cleanOutFaces: " << sub_front->size(0) << ", " << sub_front->size(1) << ", " << sub_front->size(2) << ", " << sub_front->size(3) << std::endl;

    delete box_plane.getP2();

    for (Data::Front::FrontElementIterator iter = sub_front->eBegin(3);
            iter != sub_front->eEnd(3);)
    {
        Data::FrontElement *fel = (*iter);

        iter = sub_front->erase(iter, 3);

        if (this->boundary->have(fel))
        {
            sub_front->addBoundary(fel);
        }
        else
        {
            if( !sub_front->have(fel) )
                sub_front->addElement(fel);
        }
    }


    /*std::cout << "Na subfront em " << sub_front << std::endl;
    for (UShort i = 0; i < 4; i++)
    {
        std::cout << "fels: " << static_cast<UInt>(i) << ":    "<< sub_front->size(i) << std::endl;

        for( Data::Front::FrontElementIterator it = sub_front->eBegin(i); it != sub_front->eEnd(i); it++ )
        {
            std::cout << " " << (*it)->text() << "   AT:  " << (*it) << std::endl;
        }
        std::cout << " " << "  ========================================== " << std::endl;
        std::cout << " " << "  ========================================== " << std::endl;
        for( Data::Front::FrontElementIterator it = sub_front->eBegin(i); it != sub_front->eEnd(i); it++ )
        {
            for (int i = 0; i < 3; i++ )
            {
                MJMesh::Vertex *v = (MJMesh::Vertex*)( (*it)->getVertex(i) );
                if( v->getId() == 17 )
                {
                    std::cout << " " << v->getId() << "   AT:  " << v << std::endl;
                    std::cout << "getAdjacentElements " << v->getAdjacentElements().size() << std::endl;
                    std::cout << "getAdjacentFrontElements " << v->getAdjacentFrontElements().size() << std::endl;
                }
            }
        }
    }*/

    //std::cout << "sizes of sub_front after everything: " << sub_front->size(0) << ", " << sub_front->size(1) << ", " << sub_front->size(2) << ", " << sub_front->size(3) << std::endl;



    ///IMPRESSÃO DO NOVO SUBDOMÍNIO PROCESSADO
    {
        /*Data::VertexList vertices = sub_front->getVertices();
        Data::FrontElementList elements = sub_front->getAllElements();

        std::string as = "/home/nfs/daniel/Desktop/particao";
        as += std::to_string(taskId);
        as += ".3djmsh";
        std::ofstream myfile ( as );

        if (myfile.is_open())
        {
            myfile << std::setprecision(30) << "0.4" << std::endl ;
            myfile << "0.000000001" << std::endl ;
            myfile << vertices.size() << std::endl ;
            for (Data::Front::VertexIterator it = vertices.begin(); it != vertices.end(); it++)
            {
                myfile << (*it)->getId() << "  "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
            }
            myfile << elements.size() << std::endl ;
            for (std::list<Data::FrontElement *>::iterator it=elements.begin(); it!=elements.end(); ++it)
            {
                myfile << (*it)->getId() << " " << ((MJMesh::FrontElement3D*)(*it))->getV1()->getId() << " " << ((MJMesh::FrontElement3D*)(*it))->getV2()->getId()<< " " << ((MJMesh::FrontElement3D*)(*it))->getV3()->getId() << std::endl ;
            }
        }
        myfile.close();*/
    }


    /*Data::cout() << "***************" << std::endl;
    Data::cout() << "TEMPO createSurfaceBoundary: " << time_createSurfaceBoundary - start << std::endl;
    Data::cout() << "TEMPO interceptPartitionFront: " << time_interceptPartitionFront - time_createSurfaceBoundary << std::endl;
    Data::cout() << "TEMPO createInterface3D: " << time_createInterface3D - time_interceptPartitionFront << std::endl;
    Data::cout() << "TEMPO cleanOutFaces: " << time_cleanOutFaces - time_createInterface3D << std::endl;
    Data::cout() << "TEMPO checkSurfaceMesh: " << meshSurf_Processing << std::endl;
    //Data::cout() << "TEMPO TOTAL PARA GERAÇÃO DA MALHA DE INTERFACE: " << Data::time() - start << std::endl << std::endl;
    Data::cout() << "TEMPO PARA GERAÇÃO DA MALHA DE INTERFACE: " << Data::time() - start - meshSurf_Processing << std::endl;
    Data::cout() << "***************" << std::endl;*/
    return;
}

bool PMJMesh2::APrioriParallelMeshGenerator::isElementInside(Data::Box plane, Data::TreeNode *root)
{
    UInt num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!plane.intercept(b))     ///REMOVENDO AS CELULAS QUE NAO TOCAM NA BOX
        {
            continue;
        }

        enum Data::Position decision = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n));

        if( !(n->isLeaf()) )
        {
            if( decision == Data::ON  )
            {
                return isElementInside(plane, n);
            }

            if( decision == Data::IN  )
            {
                return true;
            }
            if( decision == Data::OUT  )
            {
                return false;
            }
        }
        else
        {
            if( decision == Data::ON || decision == Data::IN )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

void PMJMesh2::APrioriParallelMeshGenerator::interceptPartitionFront(Data::Box * box, Data::Box plane, int dir, Data::TreeNode *root, std::list<Data::Point3D> &internal_points)
{
    UInt dim = box->dimension();

    UInt num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        //if (!box->intercept(b))     ///REMOVENDO AS CELULAS QUE NAO TOCAM NA BOX
        if (!plane.intercept(b))     ///REMOVENDO AS CELULAS QUE NAO TOCAM NA BOX
        {
            continue;
        }

        enum Data::Position decision = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n));

        if( !(n->isLeaf()) )
        {
            /*bool call = true;

            if (box->in(b))
            {
                call = false;

                for (UInt i = 0; i < dim; i++)
                {
                    if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                            (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                    {
                        call = true;

                        break;
                    }
                }
            }
            if(call)*/
            if( decision == Data::ON || decision == Data::IN )
            {
                interceptPartitionFront(box, plane, dir, n, internal_points);
                continue;
            }
        }


        /*bool on = true;

        if (box->in(b))
        {
            on = false;

            for (UInt i = 0; i < dim; i++)
            {
                if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                        (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                {
                    on = true;

                    break;
                }
            }
        }

        if(on)*/
        else
        {


#if USE_GUI
            //if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::IN )
            if( decision == Data::IN )
            {
                //PINTA CELULAS DA BORDA DA PARTIÇÃO
                static_cast<Data::BoxTreeCell *>(n)->setColor(0.0, 0.0, 1.0);
                static_cast<Data::BoxTreeCell *>(n)->highlight();
            }
#endif //#if USE_GUI


            if( decision == Data::ON || decision == Data::IN )
                //if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::ON ||
                //PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::IN )
            {
//std::cout << "TEM ALGO" << std::endl;

                //Data::Point3D p1 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                //Data::Point3D p2 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                //Data::Point3D p3 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                //Data::Point3D p4 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                //Data::Point3D p5 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );
                //Data::Point3D p6 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );
                //Data::Point3D p7 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );
                //Data::Point3D p8 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );

                //internal_points.push_back( p1 );
                //internal_points.push_back( p2 );
                //internal_points.push_back( p3 );
                //internal_points.push_back( p4 );
                //internal_points.push_back( p5 );
                //internal_points.push_back( p6 );
                //internal_points.push_back( p7 );
                //internal_points.push_back( p8 );

                //if( plane.on( p1 ) ) internal_points.push_back( p1 );
                //if( plane.on( p2 ) ) internal_points.push_back( p2 );
                //if( plane.on( p3 ) ) internal_points.push_back( p3 );
                //if( plane.on( p4 ) ) internal_points.push_back( p4 );
                //if( plane.on( p5 ) ) internal_points.push_back( p5 );
                //if( plane.on( p6 ) ) internal_points.push_back( p6 );
                //if( plane.on( p7 ) ) internal_points.push_back( p7 );
                //if( plane.on( p8 ) ) internal_points.push_back( p8 );

                switch(dir)
                {
                ///EIXO X
                case 0:
                {
                    Data::Point3D p1 = Data::Point3D( plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                    Data::Point3D p2 = Data::Point3D( plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                    Data::Point3D p3 = Data::Point3D( plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );
                    Data::Point3D p4 = Data::Point3D( plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );

                    internal_points.push_back( p1 );
                    internal_points.push_back( p2 );
                    internal_points.push_back( p3 );
                    internal_points.push_back( p4 );
                }
                break;
                ///EIXO Y
                case 1:
                {
                    Data::Point3D p1 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                    Data::Point3D p2 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(2) );
                    Data::Point3D p3 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );
                    Data::Point3D p4 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), plane.getMin()->getCoord(dir), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(2) );

                    internal_points.push_back( p1 );
                    internal_points.push_back( p2 );
                    internal_points.push_back( p3 );
                    internal_points.push_back( p4 );

                }
                break;
                ///EIXO Z
                case 2:
                {
                    Data::Point3D p1 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), plane.getMin()->getCoord(dir) );
                    Data::Point3D p2 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(1), plane.getMin()->getCoord(dir) );
                    Data::Point3D p3 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->min3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), plane.getMin()->getCoord(dir) );
                    Data::Point3D p4 = Data::Point3D( static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(0), static_cast<Data::BoxTreeCell *>(n)->max3D().getCoord(1), plane.getMin()->getCoord(dir) );

                    internal_points.push_back( p1 );
                    internal_points.push_back( p2 );
                    internal_points.push_back( p3 );
                    internal_points.push_back( p4 );
                }
                break;
                }

                /*
                std::list<MJMesh::Vertex *> vertexs_faces;
                static int id = 1;

                if( plane.on( p1 ) ) vertexs_faces.push_back( findInternal(p1, internal_vertexs) );
                if( plane.on( p2 ) ) vertexs_faces.push_back( findInternal(p2, internal_vertexs) );
                if( plane.on( p3 ) ) vertexs_faces.push_back( findInternal(p3, internal_vertexs) );
                if( plane.on( p4 ) ) vertexs_faces.push_back( findInternal(p4, internal_vertexs) );
                if( plane.on( p5 ) ) vertexs_faces.push_back( findInternal(p5, internal_vertexs) );
                if( plane.on( p6 ) ) vertexs_faces.push_back( findInternal(p6, internal_vertexs) );
                if( plane.on( p7 ) ) vertexs_faces.push_back( findInternal(p7, internal_vertexs) );
                if( plane.on( p8 ) ) vertexs_faces.push_back( findInternal(p8, internal_vertexs) );


                if( vertexs_faces.size() == 4 )
                {
                    vertexs_faces.sort( sort_vertex );
                    std::vector<MJMesh::Vertex *> vertexs_faces_aux;
                    for (std::list<MJMesh::Vertex *>::iterator it = vertexs_faces.begin(); it != vertexs_faces.end(); it++)
                    {
                //std::cout << (*it)->text() << std::endl;
                        vertexs_faces_aux.push_back( (*it) );
                    }


                    MJMesh::ElementSurface *e1;
                    MJMesh::ElementSurface *e2;

                    if( invert_SupportMesh )
                    {
                        e1 = new MJMesh::ElementSurface( vertexs_faces_aux[1], vertexs_faces_aux[2], vertexs_faces_aux[0]);
                        e2 = new MJMesh::ElementSurface( vertexs_faces_aux[1], vertexs_faces_aux[3], vertexs_faces_aux[2]);
                    }
                    else
                    {
                        e1 = new MJMesh::ElementSurface( vertexs_faces_aux[0], vertexs_faces_aux[2], vertexs_faces_aux[1]);
                        e2 = new MJMesh::ElementSurface( vertexs_faces_aux[2], vertexs_faces_aux[3], vertexs_faces_aux[1]);
                    }

                    bool flag1 = true;
                    bool flag2 = true;

                    for (std::list<Data::Element*>::iterator it = internal_elements.begin(); it != internal_elements.end(); it++)
                    {
                        if( flag1 == true && e1->getVertex(0)->getId() == (*it)->getVertex(0)->getId() && e1->getVertex(1)->getId() == (*it)->getVertex(1)->getId() && e1->getVertex(2)->getId() == (*it)->getVertex(2)->getId() )
                        {
                            flag1 = false;
                            delete e1;
                            //std::cout << "NOA CRIA!!! 1" << std::endl;
                        }
                        if( flag2 == true && e2->getVertex(0)->getId() == (*it)->getVertex(0)->getId() && e2->getVertex(1)->getId() == (*it)->getVertex(1)->getId() && e2->getVertex(2)->getId() == (*it)->getVertex(2)->getId() )
                        {
                            flag2 = false;
                            delete e2;
                            //std::cout << "NOA CRIA!!! 2" << std::endl;
                        }

                        if(!flag1 && !flag2)
                            break;
                    }

                    if(flag1)
                    {
                        e1->setId(id++);
                        internal_elements.push_back( e1 );
                    }

                    if(flag2)
                    {
                        e2->setId(id++);
                        internal_elements.push_back( e2 );
                    }
                }*/
            }
        }
    }
}



void PMJMesh2::APrioriParallelMeshGenerator::interceptPartitionFront(Data::Box * box, Data::TreeNode *root,
        std::list<Data::TreeNode *> &topBoundaryCells, std::list<Data::TreeNode *> &baseBoundaryCells,
        std::list<Data::TreeNode *> &leftBoundaryCells, std::list<Data::TreeNode *> &rightBoundaryCells, UInt side)
{
    UInt dim = box->dimension();

    UInt num_children;

    num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!box->intercept(b))
            continue;


        if( !(n->isLeaf()) )
        {
            bool call = true;

            if (box->in(b))
            {
                call = false;

                for (UInt i = 0; i < dim; i++)
                {
                    if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                            (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                    {
                        call = true;

                        break;
                    }
                }
            }

            if(call)
            {
                interceptPartitionFront(box, n,
                                        topBoundaryCells, baseBoundaryCells, leftBoundaryCells, rightBoundaryCells, side);
                continue;
            }
        }


        bool on = true;

        if (box->in(b))
        {
            on = false;

            for (UInt i = 0; i < dim; i++)
            {
                if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                        (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                {
                    on = true;

                    break;
                }
            }
        }

        if(on)
        {
            if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::IN )
            {
#if USE_GUI
                //PINTA CELULAS DA BORDA DA PARTIÇÃO
                static_cast<Data::BoxTreeCell *>(n)->setColor(0.0, 0.0, 1.0);
                static_cast<Data::BoxTreeCell *>(n)->highlight();
#endif //#if USE_GUI

                if ( side==1 && ( b->getMax()->getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ||
                                  b->getMin()->getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
                {
                    topBoundaryCells.push_back( n );
                    //std::cout << "top" << std::endl;
                }

                if ( side==3 && ( b->getMin()->getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                                  b->getMax()->getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ) )
                {
                    baseBoundaryCells.push_back( n );
                    //std::cout << "base" << std::endl;
                }
                if ( side==2 && ( b->getMin()->getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                                  b->getMax()->getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ) )
                {
                    leftBoundaryCells.push_back( n );
                    //std::cout << "left" << std::endl;
                }


                if ( side==0 && ( b->getMax()->getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ||
                                  b->getMin()->getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
                {
                    rightBoundaryCells.push_back( n );
                    //std::cout << "right" << std::endl;
                }
            }
        }
    }
}


MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::findInternal(Data::Point3D p, Data::VertexList &internal_vertexs)
{
    static int id = 1;

    /*for (std::list<Data::Vertex*>::iterator it = internal_vertexs.begin(); it != internal_vertexs.end(); it++)
    {
        if( p.match( (*it)->getPoint() ) )
            return static_cast<MJMesh::Vertex *>(*it);
    }*/

    MJMesh::Vertex *v = new MJMesh::Vertex( p.getCoord(0), p.getCoord(1), p.getCoord(2), id++ );
    internal_vertexs.push_back( v );
    //return ( v );
    return NULL;
}

///ORDENA PELO ALTURA EIXO Y
bool PMJMesh2::APrioriParallelMeshGenerator::compareBoxY(Data::TreeNode *first, Data::TreeNode *second)
{
    if( static_cast<Data::BoxTreeCell *>(first)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(second)->max2D().getCoord(1) + Data::getTolerance() )
        return true;
    else
        return false;
}

///ORDENA PELO ALTURA EIXO X
bool PMJMesh2::APrioriParallelMeshGenerator::compareBoxX(Data::TreeNode *first, Data::TreeNode *second)
{
    if( static_cast<Data::BoxTreeCell *>(first)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(second)->min2D().getCoord(0) + Data::getTolerance() )
        return true;
    else
        return false;
}

MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::simpleFindOrCreate(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Data::VertexList &vertices, Real x, Real y, Real z)
{
    /*Data::Point3D p = Data::Point3D(x, y, z);

    Data::Box b(p, p);

    Data::VertexList vertices;
    MJMesh::Vertex *v = new MJMesh::Vertex(x,y,z, 0);


    vertices = local_front->verticesInRange(b);

    if (vertices.empty())
    {

        v->setId( id_Manager->next(0) );
        local_front->add(v);

    }
    else
    {
        v->setId(vertices.front()->getId());
    }

    sub_front->add(v);


    return v;*/

    MJMesh::Vertex *v = new MJMesh::Vertex(x,y,z, 0);
    Data::Front::VertexIterator it_aux = vertices.end();

    //for (Data::Front::VertexIterator it = local_front->vBegin(); it != local_front->vEnd(); it++)
    for( Data::Front::VertexIterator it = vertices.begin(); it != vertices.end(); it++ )
    {
        MJMesh::Vertex *v_temp = static_cast<MJMesh::Vertex *>( (*it) );

        if ( v_temp->getPoint()->match( v->getPoint() ) || v_temp->getPoint()->equal( v->getPoint() ) )
        {
            v->setId( v_temp->getId() );
            it_aux = it;
            break;
        }
    }

    if( it_aux != vertices.end() )
        vertices.erase(it_aux);

    if (v->getId() == 0)
    {
        int new_id = id_Manager->next(0);
        v->setId( new_id );
        local_front->add(v);
    }

    sub_front->add(v);

    return v;
}

MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::findOrCreate(ULInt taskId, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Real x, Real y, Real z)
{
    Data::Point3D p( (this->dimension) );

    p.setCoord(0, x);
    p.setCoord(1, y);

    if (this->dimension > 2)
    {
        p.setCoord(2, z);
    }

    Data::Box b(p, p);

    Data::VertexList vertices;
    MJMesh::Vertex *v;

    vertices = sub_front->verticesInRange(b);


    if (!vertices.empty())
    {
        return static_cast<MJMesh::Vertex *>(vertices.front());
    }

    v = new MJMesh::Vertex(
        (this->dimension == 2) ?
        static_cast<Data::Point *>(new Data::Point2D(p)) :
        static_cast<Data::Point *>(new Data::Point3D(p)),
        0);


    bool added = false;
    MJMesh::Vertex *localv = NULL;

#if USE_OPENMP
    #pragma omp critical (front)
#endif //#if USE_OPENMP
    {
        vertices = local_front->verticesInRange(b);

        if (vertices.empty())
        {
            v->setId( id_Manager->next(0) );
            local_front->add(v);
            added = true;
        }
        else
        {
            v->setId(vertices.front()->getId());
        }
    }

    #pragma omp critical
    {
        localv = this->replacer ? static_cast<MJMesh::Vertex *>(this->replacer->getTemporaryVertex(taskId, v->getId())) : NULL;
    }

    #pragma omp critical (sub_front)
    {
        sub_front->add(v);
    }

    if (localv)
    {
        if (!added)
        {
            delete v;
        }
        return localv;
    }

    return v;
}


MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::bestVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Performer::IdManager *id_Manager, Data::Edge2D *e, ULInt taskId)
{
    for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    {
        if( e->intercept( (*it)->getVertex(0)->getPoint(), (*it)->getVertex(1)->getPoint() ) ||
                e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() ) )
        {
            Data::Edge2D aux_edge = Data::Edge2D( static_cast<Data::Point2D*>((*it)->getVertex(0)->getPoint()), static_cast<Data::Point2D*>((*it)->getVertex(1)->getPoint()) );
            if( (aux_edge).right( e->getP2() ) )
            {
                return  findOrCreate(taskId, sub_front, local_front, id_Manager, e->getP1()->getCoord(0),
                                     e->getP1()->getCoord(1));
            }
            else
            {
                return  findOrCreate(taskId, sub_front, local_front, id_Manager, e->getP2()->getCoord(0),
                                     e->getP2()->getCoord(1));
            }
        }
    }

    return  findOrCreate(taskId, sub_front, local_front, id_Manager, e->getP1()->getCoord(0),
                         e->getP1()->getCoord(1));
}


MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::nearestVertex(PMJMesh::Front *local_front, MJMesh::Vertex* vertex)
{
    double dist = HUGE_VAL;
    MJMesh::Vertex* v = NULL;
    Data::FrontElement *e_aux = NULL;

    //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
    for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
    {
        if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
        {
            double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );
            //double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );

            /*if(d <= dist)
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
            }*/

            ///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
            }


        }
        else
        {
            double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );
            //double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );

            /*if(d <= dist)
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
            }*/

            ///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
            }
        }
    }

//std::cout << "PROCUROU 1: " << v->getId() << std::endl;
//std::cout << "PROCUROU 2: " << vertex->getId() << std::endl;

    if( v == NULL || v->getPoint()->equal(vertex->getPoint()) )
        return NULL;






//std::cout << "pasouuuuu" << std::endl;
    //bool exist = false;
    //Data::FrontElementList element = sub_front->getRejected();

    //for( std::list<Data::FrontElement *>::iterator it = element.begin(); it != element.end(); it++ )
    /*for( Data::Front::FrontElementIterator it = sub_front->eBegin(2); it != sub_front->eEnd(2); it++ )
    {
    //std::cout << "PROCURANDO" << std::endl;
        if( ( e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(1)->getPoint()) )
         || ( e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(1)->getPoint()) ) )
        {
            //exist = true;
            break;
        }
    }*/
    /*
            if(exist == false)
            {
                if( box->out(v->getPoint()) )
                {
                    //sub_front->addRejected(e_aux);
                    sub_front->add(v);
                }
            }
    */
//std::cout << "JA ERA" << std::endl;
    return v;
}


MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::nearestCrossVertex(PMJMesh::Front *sub_front, MJMesh::Vertex* vertex)
{
    double dist = HUGE_VAL;
    MJMesh::Vertex* v = NULL;
    Data::FrontElement *e_aux = NULL;

    for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    {
//std::cout << "ID CROSSS: " << (*it)->getId() << std::endl;
        /*if( (*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()) || (*it)->getVertex(1)->getPoint()->equal(vertex->getPoint()) )
            continue;*/


        if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
        {
            if((*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()))
            {
                double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
                if(d <= dist && (*it)->accordingToNormal( vertex ))
                {
                    dist = d;
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                    e_aux = (*it);
                }
                continue;
            }


            //double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );
            double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
                continue;
            }

            if (d <= dist && ((!v) || (!v->getPoint()->match((*it)->getVertex(0)->getPoint()))))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
            }

        }
        else
        {
            if((*it)->getVertex(1)->getPoint()->equal(vertex->getPoint()))
            {
                double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
                if(d <= dist && (*it)->accordingToNormal( vertex ))
                {
                    dist = d;
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                    e_aux = (*it);
                }
                continue;
            }

            //double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );
            double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
                continue;
            }

            if(d <= dist && ((!v) || (!v->getPoint()->match((*it)->getVertex(1)->getPoint()))))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
            }

        }
    }
    /*
    std::cout << "ID CROSSS EScolhido: " << e_aux->getId() << std::endl;
    std::cout << "ID CROSSS EScolhido: " << v->getId() << std::endl;

    std::cout << "PROCUROU 1: " << v->getId() << std::endl;
    std::cout << "PROCUROU 2: " << vertex->getId() << std::endl;
    */
    if( v == NULL || v->getPoint()->equal(vertex->getPoint()) )
        return NULL;
//std::cout << "pasouuuuu" << std::endl;


    /*if(!sub_front->have(e_aux, 2))   //tah na rejected
    {
        if( box->out(v->getPoint()) )
        {
            //sub_front->addRejected(e_aux);
            sub_front->add(v);
        }
    }*/

    return v;
}

MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::nearestCrossVertexNotAdj(PMJMesh::Front *sub_front, MJMesh::Vertex* vertex)
{
    double dist = HUGE_VAL;
    MJMesh::Vertex* v = NULL;
    Data::FrontElement *e_aux = NULL;

    Data::FrontElementList vertex_adj = vertex->getAdjacentFrontElements();

    for (Data::FrontElementList::iterator iter = vertex_adj.begin();
            iter != vertex_adj.end(); )
    {
        if (!sub_front->have(*iter))
        {
            iter = vertex_adj.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    {
//std::cout << "ID CROSSS: " << (*it)->getId() << std::endl;
        /*if( (*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()) || (*it)->getVertex(1)->getPoint()->equal(vertex->getPoint()) )
            continue;*/
        ///SE FOR ADJACENTE AO VERTICE DE ENTRADA -> PULA
        bool pula = false;
        for( std::list<Data::FrontElement *>::iterator iter = vertex_adj.begin(); iter != vertex_adj.end(); iter++ )
        {
            if( (*iter)->getVertex(0)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || (*iter)->getVertex(1)->getPoint()->equal((*it)->getVertex(0)->getPoint())
                    || (*iter)->getVertex(0)->getPoint()->equal((*it)->getVertex(1)->getPoint()) || (*iter)->getVertex(1)->getPoint()->equal((*it)->getVertex(1)->getPoint()) )
            {
                pula = true;
                break;
            }
        }
        if(pula)
            continue;

        if( (*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()) || (*it)->getVertex(1)->getPoint()->equal(vertex->getPoint())
                || (*it)->getVertex(0)->getId() == vertex->getId() || (*it)->getVertex(1)->getId() == vertex->getId())
        {
            continue;
        }

        ///PROCURA QUAL O MAIS PERTO
        if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
        {
            //double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );
            double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
                continue;
            }

            if (d <= dist && ((!v) || (!v->getPoint()->match((*it)->getVertex(0)->getPoint()))))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
            }

        }
        else
        {
            //double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );
            double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );

///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
            if(d <= dist && (*it)->accordingToNormal( vertex ))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
                continue;
            }

            if(d <= dist && ((!v) || (!v->getPoint()->match((*it)->getVertex(1)->getPoint()))))
            {
                dist = d;
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
            }

        }
    }

    if( v == NULL || v->getPoint()->equal(vertex->getPoint()) )
        return NULL;

    return v;
}

MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::findElementVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, MJMesh::Vertex* vertex)
{
    //double dist = HUGE_VAL;
    MJMesh::Vertex* v = NULL;
    Data::FrontElement *e_aux = NULL;

    for( Data::Front::FrontElementIterator it = local_front->eBegin(1); it != local_front->eEnd(1); it++ )
    {
//std::cout << "ID CROSSS: " << (*it)->getId() << std::endl;

        if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
        {
            if((*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()))
            {
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
                return v;
            }
        }
        else
        {
            if((*it)->getVertex(1)->getPoint()->equal(vertex->getPoint()))
            {
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
                return v;
            }
        }
    }

    return v;
}

MJMesh::FrontElement2D * PMJMesh2::APrioriParallelMeshGenerator::InitialEdge(ULInt taskId, Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList &interfaceEdges, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, MJMesh::Vertex *vMin, MJMesh::FrontElement2D *e, bool invert)
{
//std::cout << "Fazendo chamada: " << element_front.size() << std::endl;
    //double dist = HUGE_VAL;
    MJMesh::Vertex *v_aux = NULL;

    //v_aux = nearestCrossVertex(sub_front, vMin);
    v_aux = nearestVertexSubFront(box,sub_front, local_front, e, vMin,taskId);
    if(v_aux == NULL)
        v_aux = nearestCrossVertex(sub_front, vMin);

//if(v_aux == NULL)
    //std::cout << "nulo!" << std::endl;
//else
    //std::cout << "V INICIAL: " << v_aux->getId() << std::endl;

    if(v_aux == NULL)
    {
        Data::FrontElement *e_aux;
        //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
        for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
        {
            if( ( e->intercept( (*it) ) || e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() )
                    || (*it)->in( e->getV1()->getPoint() ) || (*it)->in( e->getV2()->getPoint() ) ) )
            {
//std::cout << "Vazio: " << (*it)->getId() << "  " << sub_front->getRejected().size() << std::endl;
                if( e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                        e->straightDistance( (*it)->getVertex(1)->getPoint() ) )
                {
                    if( v_aux == NULL || e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                            e->straightDistance( v_aux->getPoint() ) )
                    {
                        if(vMin->getPoint()->equal((*it)->getVertex(0)->getPoint()))
                            continue;

                        v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                        e_aux = (*it);
                    }
                }
                else
                {
                    if( v_aux == NULL || e->straightDistance( (*it)->getVertex(1)->getPoint() ) <=
                            e->straightDistance( v_aux->getPoint() ) )
                    {
                        if(vMin->getPoint()->equal((*it)->getVertex(1)->getPoint()))
                            continue;

                        v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                        e_aux = (*it);
                    }
                }
            }
        }
        if( v_aux != NULL && box->out(v_aux->getPoint()) )         //1 VERTICE FORA
        {
            //std::cout << "adding " << e_aux->getId() << " at " << e_aux << " in subfront (1)" << std::endl;
            //sub_front->addRejected(e_aux);
            if( !sub_front->have(e_aux) )
                sub_front->addElement(e_aux);
            if( !sub_front->have(v_aux) )
                sub_front->add(v_aux);
        }

    }

    if(v_aux == NULL)
        return NULL;

    //if(this->boundary->have(v_aux))
    common_vertices_list.insert(this->findOrCreate(taskId, sub_front, local_front, id_Manager, v_aux->getPoint()->getCoord(0), v_aux->getPoint()->getCoord(1)));                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

    if(invert == false)
    {
        MJMesh::FrontElement2D *edge = new MJMesh::FrontElement2D( v_aux, vMin, id_Manager->next(1) );

//std::cout << "DEUBG 11: " << edge->getV1()->getPoint()->getCoord(0) << std::endl;
//std::cout << "DEUBG 22: " << edge->getV2()->getPoint()->getCoord(0) << std::endl;
        if(includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, edge, v_aux, vMin, taskId))
            return edge;
        else
            return NULL;
    }
    else
    {
        MJMesh::FrontElement2D *edge = new MJMesh::FrontElement2D( vMin, v_aux, id_Manager->next(1) );

//std::cout << "DEUBG 1: " << edge->getV1()->getPoint()->getCoord(0) << std::endl;
//std::cout << "DEUBG 2: " << edge->getV2()->getPoint()->getCoord(0) << std::endl;
//std::cout << "vMin: " << vMin->getId() << std::endl;
//std::cout << "v_aux: " << v_aux->getId() << std::endl;
//std::cout << "edge: " << edge->getId() << std::endl;


        if(includeInModel(sub_front, local_front, interfaceEdges, common_vertices_list, id_Manager, edge, vMin, v_aux, taskId))
            return edge;
        else
            return NULL;
    }
}

bool PMJMesh2::APrioriParallelMeshGenerator::includeInModel(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, Data::VertexSet &common_vertices_list, Performer::IdManager *id_Manager, MJMesh::FrontElement2D *e, MJMesh::Vertex *v1, MJMesh::Vertex *v2, ULInt taskId)
{
    Data::Point2D p1(this->dimension), p2(this->dimension);

    for (UInt i = 0; i < this->dimension; i++)
    {
        p1.setCoord(i, std::min(v1->getPoint()->getCoord(i), v2->getPoint()->getCoord(i)) - Data::getTolerance());
        p2.setCoord(i, std::max(v1->getPoint()->getCoord(i), v2->getPoint()->getCoord(i)) + Data::getTolerance());
    }

    Data::Box box(p1, p2);

    Data::VertexList vertices = local_front->verticesInRange(box);

    //delete box.getMin();
    //delete box.getMax();

    Data::Vertex *v1aux = NULL;
    Data::Vertex *v2aux = NULL;

    for (Data::VertexList::iterator iter = vertices.begin();
            iter != vertices.end(); iter++)
    {
        Data::Vertex *v = (*iter);

        if (v->getId() == v1->getId())
        {
            v1aux = v;
        }

        if (v->getId() == v2->getId())
        {
            v2aux = v;
        }

        if ((v1aux) && (v2aux))
        {
            break;
        }
    }

    if ((v1aux) && (v2aux))
    {
        bool found1 = false;
        bool found2 = false;

        Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(v1aux)->getAdjacentFrontElements();

        while (!adjs.empty())
        {
            if ((local_front->have(adjs.front())) && (local_front->have(adjs.front(), 0)))
            {
                found1 = true;

                break;
            }

            adjs.pop_front();
        }

        adjs = static_cast<MJMesh::Vertex *>(v2aux)->getAdjacentFrontElements();

        while (!adjs.empty())
        {
            if ((local_front->have(adjs.front())) && (local_front->have(adjs.front(), 0)))
            {
                found2 = true;

                break;
            }

            adjs.pop_front();
        }

        if ((found1) && (found2))
        {
            //std::cout << "entrou no código do markos para " << v1->getId() << " e " << v2->getId() << std::endl;
            //std::cout << "Deletando   " << e->getId() << std::endl;

            delete e;
            return false;
        }
    }

    Data::Box box1( *e->getV1()->getPoint(), *e->getV2()->getPoint() );
    Data::Box box2( *e->getV2()->getPoint(), *e->getV1()->getPoint() );

    bool flag;
    flag = boxEdgeTest( &box1, tree->getRoot() ) == false || boxEdgeTest( &box2, tree->getRoot() ) == false;


    ///CASO O MODELO TENHA FRATURAS
    bool flag2 = false;
    if( boxOnEdgeTest( &box1, tree->getRoot() ) == false || boxOnEdgeTest( &box2, tree->getRoot() ) == false )
    {
        Data::FrontElementList vertices1;
        Data::FrontElementList vertices2;
        vertices1 = sub_front->frontElementsInRange(box1);
        vertices2 = sub_front->frontElementsInRange(box2);

        bool cross = false;

        for( std::list<Data::FrontElement *>::iterator it = vertices1.begin(); it != vertices1.end(); it++ )
        {
            if( e->eintercept((*it)) )
            {
                cross = true;
                break;
            }

        }

        for( std::list<Data::FrontElement *>::iterator it = vertices2.begin(); it != vertices2.end(); it++ )
        {
            if( e->eintercept((*it)) )
            {
                cross = true;
                break;
            }
        }

        if(cross)
            flag2 = true;

    }


    if( flag || flag2)
    {
        //PRIMEIRO VERTEX

        //MJMesh::Vertex* v_axu1 = nearestVertexSubFront(box, sub_front, element_front, e, v1);
        MJMesh::Vertex* v_axu1 = NULL;

        if( !this->boundary->have(v1) )
        {
            //v_axu1 = nearestCrossVertex(sub_front, v1);
            v_axu1 = nearestCrossVertexNotAdj(sub_front, v1);               ///MODIFICADO ISTO
        }
        else
            common_vertices_list.insert(this->findOrCreate(taskId, sub_front, local_front, id_Manager, v1->getPoint()->getCoord(0), v1->getPoint()->getCoord(1)));                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS



        MJMesh::FrontElement2D *edge1 = NULL;


//std::cout << "V1: " << v1->getId() << std::endl;
//if(v_axu1 != NULL)
//std::cout << v_axu1->getId() << std::endl;

        if(v_axu1 != NULL)
        {
            /*bool isOk = true;

            for( Data::Front::FrontElementIterator it = sub_front->eBegin(2); it != sub_front->eEnd(2); it++ )
            {
                if( ((MJMesh::FrontElement2D*)(*it))->distance( v1->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    isOk = false;
                    break;
                }
            }

            if(isOk)*/
            {
                v1 = this->findOrCreate(taskId, sub_front, local_front, id_Manager, v1->getPoint()->getCoord(0), v1->getPoint()->getCoord(1));
                edge1 = new MJMesh::FrontElement2D( v1, v_axu1, id_Manager->next(1) );


                if(this->boundary->have(v_axu1))
                    common_vertices_list.insert(v_axu1);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

                Data::FrontElement* aux = existEdgeInGlobal(edge1, local_front);

//std::cout << "ID ARESTA: " << edge1->getId() << std::endl;
//std::cout << "ID V0: " << v1->getId() << std::endl;
//std::cout << "ID V1: " << v_axu1->getId() << std::endl;

                if ((aux) && (this->boundary->have(aux)))
                {
//std::cout << "GOLLL 1" << std::endl;
                    delete edge1;
                    edge1 = NULL;
                    if(!sub_front->have(aux))
                    {
                        std::cout << "adding " << aux->getId() << " at " << aux << " in subfront (2)" << std::endl;
                        //sub_front->addRejected(aux);
                        sub_front->addElement(aux);
                    }
                }
                else
                {
//std::cout << "GOLLL 1111111111" << std::endl;
                    if(edge1)
                        interfaceEdges.push_back( edge1 );
                }
            }

            //removeDanglingEdges(box, sub_front, edge1);
        }
        //SEGUNDO VERTEX

        //MJMesh::Vertex* v_axu2 = nearestVertexSubFront(box, sub_front, element_front, e, v2);
        MJMesh::Vertex* v_axu2 = NULL;

        if( !this->boundary->have(v2) )
        {
            //v_axu2 = nearestCrossVertex(sub_front, v2);
            v_axu2 = nearestCrossVertexNotAdj(sub_front, v2);               ///MODIFICADO ISTO
        }
        else
            common_vertices_list.insert(this->findOrCreate(taskId, sub_front, local_front, id_Manager, v2->getPoint()->getCoord(0), v2->getPoint()->getCoord(1)));                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS


        MJMesh::FrontElement2D *edge2 = NULL;


//std::cout << "V2: " << v2->getId() << std::endl;
//if(v_axu2 != NULL)
//std::cout << v_axu2->getId() << std::endl;

        if(v_axu2 != NULL)
        {
            /*bool isOk = true;

            for( Data::Front::FrontElementIterator it = sub_front->eBegin(2); it != sub_front->eEnd(2); it++ )
            {
                if( ((MJMesh::FrontElement2D*)(*it))->distance( v2->getPoint() ) <= Data::getTolerance()*700.0 )
                {
                    isOk = false;
                    break;
                }
            }

            if(isOk)*/
            {
                v2 = this->findOrCreate(taskId, sub_front, local_front, id_Manager, v2->getPoint()->getCoord(0), v2->getPoint()->getCoord(1));
                edge2 = new MJMesh::FrontElement2D( v_axu2, v2, id_Manager->next(1) );

                if(this->boundary->have(v_axu2))
                    common_vertices_list.insert(v_axu2);                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

                Data::FrontElement* aux = existEdgeInGlobal(edge2, local_front);

                if ((aux) && (this->boundary->have(aux)))
                {
//std::cout << "GOLLL 2" << std::endl;
                    delete edge2;
                    edge2 = NULL;
                    if(!sub_front->have(aux))
                    {
                        std::cout << "adding " << aux->getId() << " at " << aux << " in subfront (3)" << std::endl;
                        //sub_front->addRejected(aux);
                        sub_front->addElement(aux);
                    }
                }
                else
                {
//std::cout << "GOLLL 222222222" << std::endl;
                    if(edge2)
                        interfaceEdges.push_back( edge2 );
                }
            }



            //removeDanglingEdges(box, sub_front, edge2);
        }

        /*
        #if USE_OPENMP
                #pragma omp critical (mesh)
        #endif //#if USE_OPENMP
                {
                    if( (edge1) && (v_axu1 != NULL && v1->getId() != v_axu1->getId()) )
                    {
                        this->mesh->add(edge1);
                        this->mesh->add(v1);
                        this->mesh->add(v_axu1);
                    }
                    if( (edge2) && (v_axu2 != NULL && v2->getId() != v_axu2->getId()) )
                    {
                        this->mesh->add(edge2);
                        this->mesh->add(v_axu2);
                        this->mesh->add(v2);
                    }
                }
        */
        delete e;

        return false;
    }
    else
    {
        existEdgeInGlobal(e, local_front);
        interfaceEdges.push_back( e );

        if(!sub_front->have(e))
        {
            sub_front->add(e);
        }
        /*
        #if USE_OPENMP
                #pragma omp critical (mesh)
        #endif //#if USE_OPENMP
                {
                    if( !this->mesh->have(e) )
                        this->mesh->add(e);
                    if( !this->mesh->have(e->getV1()) )
                        this->mesh->add(e->getV1());
                    if( !this->mesh->have(e->getV2()) )
                        this->mesh->add(e->getV2());
                }
        */
        return true;
    }
}

std::list<Data::FrontElement *> PMJMesh2::APrioriParallelMeshGenerator::interceptFront( PMJMesh::Front *local_front, MJMesh::FrontElement2D *e)
{
    Data::FrontElementList list;

    //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
    for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
    {
        if( e->in( (*it)->getVertex(0) ) || e->in( (*it)->getVertex(1) ) )
        {
            if( (e->angle( (*it)->getVertex(0)->getPoint() ) < 0.2 || e->angle( (*it)->getVertex(0)->getPoint() ) > 3.0 || std::isnan(e->angle( (*it)->getVertex(0)->getPoint() )) )
                    && (e->angle( (*it)->getVertex(1)->getPoint() ) < 0.2 || e->angle( (*it)->getVertex(1)->getPoint() ) > 3.0 || std::isnan(e->angle( (*it)->getVertex(1)->getPoint() )) ) )
            {
                list.push_back((*it));
//std::cout << "LOOK: " << (*it)->getId() << std::endl;
            }
        }
    }

    return list;
}

bool PMJMesh2::APrioriParallelMeshGenerator::changeVertex(PMJMesh::Front *sub_front, MJMesh::Vertex *&v, MJMesh::Vertex *v_ref)
{

    if(!boundary->have(v))
        return false;

    MJMesh::Vertex *vzim = NULL;

    Data::FrontElementList list_adj = v->getAdjacentFrontElements();
    for( std::list<Data::FrontElement *>::iterator it = list_adj.begin(); it != list_adj.end(); it++ )
    {
        if( !(*it)->accordingToNormal( v_ref ) )
        {
            ///CASO TENHA DOIS VERTICES NA MESMA POSIÇÃO, PEGA O OUTRO VERTICE QUE NÃO TENHA ARESTA CRIADA AINDA NELE
            Data::Point2D p1(this->dimension);
            p1.setCoord(0, v->getPoint()->getCoord(0));
            p1.setCoord(1, v->getPoint()->getCoord(1));
            Data::Box box11(p1, p1);
            Data::FrontElementList vertices_look;
            vertices_look = sub_front->frontElementsInRange(box11);

            delete box11.getMin();
            delete box11.getMax();

            if(vertices_look.size() > 1)
            {
                for( std::list<Data::FrontElement *>::iterator iter = vertices_look.begin(); iter != vertices_look.end(); iter++ )
                {
                    if( (*iter)->getVertex(0)->getId() != v->getId() && (*iter)->getVertex(0)->getPoint()->equal(v->getPoint()) )
                    {
                        vzim = (MJMesh::Vertex*)(*iter)->getVertex(0);
                    }
                    if( (*iter)->getVertex(1)->getId() != v->getId() && (*iter)->getVertex(1)->getPoint()->equal(v->getPoint()) )
                    {
                        vzim = (MJMesh::Vertex*)(*iter)->getVertex(1);
                    }
                }
            }
        }
    }

    if(vzim==NULL)
        return false;

    v = vzim;

    if( !sub_front->have(v) )
        sub_front->add(v);


    return true;
}

bool PMJMesh2::APrioriParallelMeshGenerator::unusedVertex(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, MJMesh::Vertex* vertex)
{
    //TESTA SE O PONTO VMIN NAO SERA MAIS UTILIZADO E O REMOVE
    Data::FrontElementList aux_list = vertex->getAdjacentFrontElements();
    if(aux_list.size() > 0)
    {
        Data::FrontElement *aux_ele = aux_list.front();

        bool inboundary = false;
        for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
        {
            if( (vertex->getId() == (*it)->getVertex(0)->getId() || vertex->getId() == (*it)->getVertex(1)->getId()))
                return false;

            if( (*it)->getVertex(0)->getId() == aux_ele->getVertex(0)->getId() || (*it)->getVertex(0)->getId() == aux_ele->getVertex(1)->getId() )
            {
                inboundary = true;
                break;
            }
        }
        if(!inboundary)
            return false;

        if( local_front->have( aux_ele ) )
        {
            local_front->remove(aux_ele);
            local_front->remove(vertex);
        }

        if (sub_front->have(aux_ele))
        {
            sub_front->remove(aux_ele);
        }

        interfaceEdges.remove(aux_ele);
        sub_front->remove(vertex);

        ///MARKOS
        ///ACHO QUE aux_ele E vertex DEVERIAM SER APAGADOS AQUI!
        delete aux_ele;
        /*
                    #if USE_OPENMP
                            #pragma omp critical (mesh)
                    #endif //#if USE_OPENMP
                    {

                        if( this->mesh->have( aux_ele ) )
                        {
                            this->mesh->remove(aux_ele);
                            this->mesh->remove(vertex);
                        }
                    }
        */
        return true;
    }
    return false;
}

Data::FrontElementSet PMJMesh2::APrioriParallelMeshGenerator::edgesInternal(MJMesh::FrontElement2D *e, PMJMesh::Front *local_front, Data::VertexSet &common_vertices_list)
{
    Data::FrontElementSet list;

    //const Data::FrontElementList element_front = this->front->getBoundary();
    //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )


    Data::Point2D p1(this->dimension), p2(this->dimension);

    for (UInt i = 0; i < this->dimension; i++)
    {
        p1.setCoord(i, std::min(e->getVertex(0)->getPoint()->getCoord(i), e->getVertex(1)->getPoint()->getCoord(i)) - Data::getTolerance());
        p2.setCoord(i, std::max(e->getVertex(0)->getPoint()->getCoord(i), e->getVertex(1)->getPoint()->getCoord(i)) + Data::getTolerance());
    }

    Data::Box b(p1, p2);
    Data::VertexList vertices;
    vertices = local_front->verticesInRange(b);

    //delete b.getMin();
    //delete b.getMax();

    //std::cout << "::::::::::::::::::::TESTE: " << vertices.size() << std::endl;
    for (Data::VertexList::iterator iter = vertices.begin(); iter != vertices.end(); iter++)
    {
        //std::cout << "Quem? : " << (*iter)->getId() << std::endl;

        Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>((*iter))->getAdjacentFrontElements();
        //std::cout << "TAM? : " << adjs.size() << std::endl;
        while (!adjs.empty())      ///CONTA AS ADJ DO VERTICE
        {
            Data::FrontElement *adj = adjs.front();

            adjs.pop_front();

            if ( (e->getVertex(0)->getId() == adj->getVertex(0)->getId() || e->getVertex(0)->getId() == adj->getVertex(1)->getId())
                    && (e->getVertex(1)->getId() == adj->getVertex(0)->getId() || e->getVertex(1)->getId() == adj->getVertex(1)->getId()) )
            {
                continue;
            }

            if( !b.out(adj->getVertex(0)->getPoint()) && !b.out(adj->getVertex(1)->getPoint()) && adj->getId() != 0 )
            {
                list.insert( (adj) );
                //std::cout << "Entrou: " << adj->text() << std::endl;

                if(this->boundary->have(adj->getVertex(0)))
                    common_vertices_list.insert(adj->getVertex(0));                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS

                if(this->boundary->have(adj->getVertex(1)))
                    common_vertices_list.insert(adj->getVertex(1));                       ///ADICIONA NUMA LISTA DE VERTICES QUE SERÃO REPLICADOS
            }

        }

    }
    return list;

    ///CODIGO ANTIGO, NÃO FUNCIONA PARA TODOS OS CASOS
    /*
    for( Data::Front::FrontElementIterator it = this->front->eBegin(0); it != this->front->eEnd(0); it++ )
    {
        if( (*it)->getId() == 191 )
            std::cout << "::::::::::::::::::: ACJEI :::::::::::::::::::: " << std::endl;

        if( e->eintercept( (*it) ) )
        {
            std::cout << "add na lista:::::::::::::::::::: " << std::endl;
            list.push_back( (*it) );
            continue;
        }


        if( ( (*it)->in( e->getVertex(0) ) || (*it)->in( e->getVertex(1) ) ) ||
            ( (*it)->on( e->getVertex(0) ) || (*it)->on( e->getVertex(1) ) ) )
            {
                if( (e->distance( (*it)->getVertex(0)->getPoint() ) <= Data::getTolerance()*10.0 || isnan( e->distance( (*it)->getVertex(0)->getPoint() ) ))
                    &&  (e->distance( (*it)->getVertex(1)->getPoint() ) <= Data::getTolerance()*10.0 ||  isnan( e->distance( (*it)->getVertex(1)->getPoint() ) ) ) )
                {
                if( ( e->getVertex(0)->getPoint()->squaredDistance( e->getVertex(1)->getPoint() ) >
                       (*it)->getVertex(0)->getPoint()->squaredDistance( e->getVertex(1)->getPoint() ) + (*it)->getVertex(1)->getPoint()->squaredDistance( e->getVertex(0)->getPoint() ) )
                       || ( e->getVertex(0)->getPoint()->squaredDistance( e->getVertex(1)->getPoint() ) >
                       (*it)->getVertex(0)->getPoint()->squaredDistance( e->getVertex(0)->getPoint() ) + (*it)->getVertex(1)->getPoint()->squaredDistance( e->getVertex(1)->getPoint() ) ) )
                    list.push_back( (*it) );
                }
            }

    }
    return list;
    */
}

bool PMJMesh2::APrioriParallelMeshGenerator::addAdjCross(PMJMesh::Front *sub_front, Data::FrontElementList &swappingEdges, MJMesh::Vertex* v_cross, MJMesh::Vertex* vertex)
{
    /*bool isIn = false;
    Data::FrontElementList els = vertex->getAdjacentFrontElements();
    for( std::list<Data::FrontElement *>::iterator it = els.begin(); it != els.end(); it++ )
    {
        if( !sub_front->have( (*it) ) && this->boundary->have( (*it) ) )
        {
            isIn = true;
            //sub_front->addRejected( (*it) );
            sub_front->addCrossing( (*it) );

            if (!sub_front->have( (*it)->getVertex(0) ))
            {
                sub_front->add( (*it)->getVertex(0) );
            }

            if (!sub_front->have( (*it)->getVertex(1) ))
            {
                sub_front->add( (*it)->getVertex(1) );
            }
        }
    }

    if(isIn && cont)
    {
        for( std::list<Data::FrontElement *>::iterator it = els.begin(); it != els.end(); it++ )
        {
            if( (*it)->getVertex(0)->getId() != vertex->getId() )
                addAdjCross(sub_front, (MJMesh::Vertex*)(*it)->getVertex(0), false);

            if( (*it)->getVertex(1)->getId() != vertex->getId() )
                addAdjCross(sub_front, (MJMesh::Vertex*)(*it)->getVertex(1), false);
        }
    }

    return isIn;*/

    bool isIn = false;
    Data::FrontElementList els;

    if(v_cross != NULL)
        els = v_cross->getAdjacentFrontElements();
    else
        return false;

    /*
        ///TEM QUE TESTAR ESSE TRECHO
        if( !this->boundary->have(v_cross) || !this->boundary->have(vertex) )
            return false;

        bool getout = true;
        for( std::list<Data::FrontElement *>::iterator it = els.begin(); it != els.end(); it++ )
        {
            if (sub_front->have((*it), 3))
            {
                Data::FrontElementList eles;
                eles = vertex->getAdjacentFrontElements();
                for( std::list<Data::FrontElement *>::iterator iter = eles.begin(); iter != eles.end(); iter++ )
                {
                    if(!sub_front->have((*iter), 3))
                    {
                        getout = false;
                    }
                }
                if(getout)
                    return false; //tah na crossing
            }
        }
    */

    for( std::list<Data::FrontElement *>::iterator it = els.begin(); it != els.end(); it++ )
    {
        if( !sub_front->have( (*it) ) && this->boundary->have( (*it) ) )
        {
//std::cout << "COLOCA EDGE: " << (*it)->getId() << std::endl;
            isIn = true;
            //sub_front->addRejected( (*it) );
            //sub_front->addCrossing( (*it) );
            swappingEdges.push_back( (*it) );


            //if( !this->front->have( (*it) ) )
            //this->front->add( (*it) );

            double dist0 = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );
            double dist1 = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );

            if (!sub_front->have( (*it)->getVertex(0) ) && dist0 < dist1 )
            {
//std::cout << "... 0" << std::endl;
                sub_front->add( (*it)->getVertex(0) );

                //if( !this->front->have( (*it)->getVertex(0) ) )
                //this->front->add( (*it)->getVertex(0) );

                if( !(*it)->getVertex(0)->getPoint()->equal(vertex->getPoint()) )
                    addAdjCross( sub_front, swappingEdges, (MJMesh::Vertex*)(*it)->getVertex(0), vertex );
            }

            if (!sub_front->have( (*it)->getVertex(1) ) && dist1 < dist0 )
            {
//std::cout << "... 1" << std::endl;
                sub_front->add( (*it)->getVertex(1) );

                //if( !this->front->have( (*it)->getVertex(1) ) )
                //this->front->add( (*it)->getVertex(1) );

                if( !(*it)->getVertex(1)->getPoint()->equal(vertex->getPoint()) )
                    addAdjCross( sub_front, swappingEdges, (MJMesh::Vertex*)(*it)->getVertex(1), vertex );
            }
        }
    }
//std::cout << "SAINDOooooooooo" << std::endl;
    return isIn;



}

bool PMJMesh2::APrioriParallelMeshGenerator::edgeOk(PMJMesh::Front *sub_front, PMJMesh::Front *local_front, Data::FrontElementList &interfaceEdges, Performer::IdManager *id_Manager, MJMesh::Vertex *&v, ULInt taskId)
{
    MJMesh::Vertex* v_aux = nearestCrossVertex(sub_front, v);

    for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    {
        if( ((MJMesh::FrontElement2D*)(*it))->distance( v->getPoint() ) <= Data::getTolerance()*700.0 )
        {
            v = findOrCreate(taskId, sub_front, local_front, id_Manager, v->getPoint()->getCoord(0), v->getPoint()->getCoord(1));
            Data::FrontElementList adjs = v->getAdjacentFrontElements();
//std::cout << "ENTROU AKIIIIIIIIIIIIIIIIIIII: " << adjs.size() << std::endl;
            if(adjs.size() > 0)
            {
                Data::FrontElement *e = adjs.front();

#if USE_OPENMP
                #pragma omp critical        ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
#endif //#if USE_OPENMP
                {
                    std::cout << "OK Ed 1: " << e->getId() << std::endl;
//std::cout << "OK 1: " << v->getId() << std::endl;
                    if(sub_front->have(e))
                        sub_front->remove(e);

                    if(local_front->have(e))
                        local_front->remove(e);
                    /*
                                        if(this->mesh->have(e))
                                            this->mesh->remove(e);

                                        this->mesh->remove(v);
                    */
                    if( local_front->have(v) )
                        local_front->remove(v);

                    sub_front->remove(v);

                    interfaceEdges.remove(e);

                    delete e;
                }
            }
            delete v;
            v = NULL;


            ///CASO TENHA DOIS VERTICES NA MESMA POSIÇÃO, PEGA O OUTRO VERTICE QUE NÃO TENHA ARESTA CRIADA AINDA NELE
            Data::Point2D p1(this->dimension);
            p1.setCoord(0, v_aux->getPoint()->getCoord(0));
            p1.setCoord(1, v_aux->getPoint()->getCoord(1));
            Data::Box box11(p1, p1);
            Data::FrontElementList vertices_look;
            vertices_look = sub_front->frontElementsInRange(box11);

            delete box11.getMin();
            delete box11.getMax();

            if(vertices_look.size() > 1)
            {
                for( std::list<Data::FrontElement *>::iterator it = vertices_look.begin(); it != vertices_look.end(); it++ )
                {
                    if( (*it)->getVertex(0)->getId() != v_aux->getId() && (*it)->getVertex(0)->getPoint()->equal(v_aux->getPoint()) )
                    {
                        v_aux = (MJMesh::Vertex*)(*it)->getVertex(0);
                    }
                    if( (*it)->getVertex(1)->getId() != v_aux->getId() && (*it)->getVertex(1)->getPoint()->equal(v_aux->getPoint()) )
                    {
                        v_aux = (MJMesh::Vertex*)(*it)->getVertex(1);
                    }
                }
            }


            if( v!=NULL )
            {
                if( !sub_front->have(v) )
                {
                    sub_front->add(v);
                }
            }

            v = v_aux;
            return false;
        }
    }
    return true;
}

MJMesh::Vertex* PMJMesh2::APrioriParallelMeshGenerator::nearestVertexSubFront(Data::Box *box, PMJMesh::Front *sub_front, PMJMesh::Front *local_front, MJMesh::FrontElement2D *e, MJMesh::Vertex* vertex, ULInt taskId)
{
    double dist = HUGE_VAL;
    MJMesh::Vertex* v = NULL;
    Data::FrontElement *e_aux = NULL;


    Data::Point2D p1(this->dimension), p2(this->dimension);

    for (UInt i = 0; i < this->dimension; i++)
    {
        p1.setCoord(i, std::min(e->getVertex(0)->getPoint()->getCoord(i), e->getVertex(1)->getPoint()->getCoord(i)) - Data::getTolerance());
        p2.setCoord(i, std::max(e->getVertex(0)->getPoint()->getCoord(i), e->getVertex(1)->getPoint()->getCoord(i)) + Data::getTolerance());
    }

    Data::Box b(p1, p2);

    //delete b.getMin();
    //delete b.getMax();

    //for( std::list<Data::FrontElement *>::const_iterator it = element_front.begin(); it != element_front.end(); it++ )
    for( Data::Front::FrontElementIterator it = local_front->eBegin(0); it != local_front->eEnd(0); it++ )
    {
        if( e->intercept( (*it) ) || b.have((*it)->getVertex(0)->getPoint()) || b.have((*it)->getVertex(1)->getPoint())
                || e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() )
                || (*it)->in( e->getV1()->getPoint() ) || (*it)->in( e->getV2()->getPoint() ) )
        {

            /*if( e->in( (*it)->getVertex(1)->getPoint() ) )
            {
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                e_aux = (*it);
                break;
            }
            if( e->in( (*it)->getVertex(0)->getPoint() ) )
            {
                v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                e_aux = (*it);
                break;
            }*/


            /*
                        if( e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                            e->straightDistance( (*it)->getVertex(1)->getPoint() ) )
            */

            if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                    vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
            {
                double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );
                //double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );
///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
                if(d <= dist && (*it)->accordingToNormal( vertex ))
                {
                    dist = d;
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                    e_aux = (*it);
                }
            }
            else
            {
                double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );
                //double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );
///OLHANDO SE O VERTICE ESTÀ PARA FORA DO SUBDOMINIO
                if(d <= dist && (*it)->accordingToNormal( vertex ))
                {
                    dist = d;
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                    e_aux = (*it);
                }
            }
        }
    }

//std::cout << "PROCUROU 1: " << v->getId() << std::endl;
//std::cout << "PROCUROU 2: " << vertex->getId() << std::endl;

    if( v == NULL || v->getPoint()->equal(vertex->getPoint()) )
        return NULL;
//std::cout << "pasouuuuu" << std::endl;
    bool exist = false;
    //Data::FrontElementList element = sub_front->getRejected();

    //for( std::list<Data::FrontElement *>::iterator it = element.begin(); it != element.end(); it++ )
    for( Data::Front::FrontElementIterator it = sub_front->eBegin(3); it != sub_front->eEnd(3); it++ )
    {
//std::cout << "PROCURANDO" << std::endl;
        if( ( e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(1)->getPoint()) )
                || ( e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(1)->getPoint()) ) )
        {
            exist = true;
            break;
        }
    }

    if(exist == false)
    {
        if( box->out(v->getPoint()) )
        {
            if (!sub_front->have(e_aux))
            {
                std::cout << "adding " << e_aux->getId() << " at " << e_aux << " in subfront (4)" << std::endl;
                //sub_front->addRejected(e_aux);
                sub_front->addElement(e_aux);
            }

            if (!sub_front->have(v))
            {
                sub_front->add(v);
            }
        }
    }
//std::cout << "JA ERA" << std::endl;

    MJMesh::Vertex *localv = this->replacer ? static_cast<MJMesh::Vertex *>(this->replacer->getTemporaryVertex(taskId, v->getId())) : NULL;

    return localv ? localv : v;
}

Data::FrontElement* PMJMesh2::APrioriParallelMeshGenerator::existEdgeInGlobal(MJMesh::FrontElement *e, PMJMesh::Front *local_front)
{
    if(e == NULL) return NULL;

    Data::FrontElementList list;
    bool value = false;

    Data::FrontElement* aux = NULL;

#if USE_OPENMP
    #pragma omp critical (front)    ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
#endif //#if USE_OPENMP
    {
        list = local_front->frontElementsInRange( e->getShape()->box() );

        if( list.size() == 0 )
        {
            if(!local_front->have(e))
            {
                local_front->add(e);
            }
        }

        if( list.size() != 0 )
        {
            //return false;       //ADICIONAR AQUI !!!
            Data::VertexVector vertices = e->getVertices();

            std::list<Data::FrontElement *>::iterator iter;
            for (iter = list.begin(); iter != list.end(); iter++)
            {
                bool equal = false;
                Data::VertexVector verticesVec = (*iter)->getVertices();


                //for(uint i = 0; i < vertices.size(); i++)
                for(UInt i = 0; i < vertices.size(); i++)
                {
                    equal = false;

                    //for(uint j = 0; j < verticesVec.size(); j++)
                    for(UInt j = 0; j < verticesVec.size(); j++)
                    {
                        if( vertices[i]->getPoint()->equal( verticesVec[j]->getPoint() ) )
                        {
                            equal = true;   //IGUAL
                            j = static_cast<UInt>(verticesVec.size());
                            //break;
                        }
                    }

                    if( equal == false )
                        i = static_cast<UInt>(vertices.size());
                    //break;
                }

                if( equal == true )     //ACHOU
                {
                    e->setId( (*iter)->getId() );

                    for(uint i = 0; i < vertices.size(); i++)
                    {
                        for(uint j = 0; j < verticesVec.size(); j++)
                        {
                            if( vertices[i]->getPoint()->equal( verticesVec[j]->getPoint() ) )
                                vertices[i]->setId( verticesVec[j]->getId() );
                        }
                    }

                    value = true;
                    aux = (*iter);
                    //return true;
                }
            }

            if(value == false)
            {
                //#pragma omp critical (front)
                if(!local_front->have(e))
                {
                    local_front->add(e);
                }
                aux = NULL;
            }
        }
    }

    return aux;
}

bool PMJMesh2::APrioriParallelMeshGenerator::boxEdgeTest(Data::Box *box, Data::TreeNode *root)
{
    //int num_children;
    UInt num_children;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        //#pragma omp critical (root)   //SOMENTE LEITURA
        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!box->intercept(b))
            continue;

        if( !(n->isLeaf()) )
        {
            if( boxEdgeTest(box, n) == false )
                return false;
        }
        else
        {
            if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::OUT )
                return false;
        }
    }
    return true;
}

bool PMJMesh2::APrioriParallelMeshGenerator::boxOnEdgeTest(Data::Box *box, Data::TreeNode *root)
{
    //int num_children;
    UInt num_children;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        //#pragma omp critical (root)   //SOMENTE LEITURA
        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!box->intercept(b))
            continue;

        if( !(n->isLeaf()) )
        {
            if( boxOnEdgeTest(box, n) == false )
                return false;
        }
        else
        {
            if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::ON )
                return false;
        }
    }
    return true;
}

void PMJMesh2::APrioriParallelMeshGenerator::correctHoles(PMJMesh::Front *sub_front, PMJMesh::Front *local_front)
{
#if USE_OPENMP
    #pragma omp critical (front)        ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
#endif //#if USE_OPENMP
    {
        Data::FrontElementList crossing = sub_front->getCrossing();

        for (Data::FrontElementList::iterator iter = crossing.begin(); iter != crossing.end(); iter++)
        {
            Data::FrontElementList adjs1 = ((MJMesh::Vertex *)(*iter)->getVertex(0))->getAdjacentFrontElements();
            Data::FrontElementList adjs2 = ((MJMesh::Vertex *)(*iter)->getVertex(1))->getAdjacentFrontElements();

            bool flag = false;
            bool del = false;
            Data::FrontElement *adj1 = NULL;
            Data::FrontElement *adj2 = NULL;
            Data::Vertex *v_aux = NULL;

            for (Data::FrontElementList::iterator it1 = adjs1.begin(); it1 != adjs1.end(); it1++)
            {
                if( (*it1)->getId() == (*iter)->getId() )
                    continue;

                if (!sub_front->have((*it1)))
                {
                    continue;
                }

                //Aresta está solta
                if( ( (*it1)->getVertex(0)->getId() == (*iter)->getVertex(1)->getId() || (*it1)->getVertex(0)->getId() == (*iter)->getVertex(0)->getId() )
                        && ((MJMesh::Vertex *)(*it1)->getVertex(1))->getAdjacentFrontElements().size() == 0 )
                {
                    del = true;
                    adj1 = (*it1);
                    v_aux = (*it1)->getVertex(0);
                    break;
                }
                if( ( (*it1)->getVertex(1)->getId() == (*iter)->getVertex(1)->getId() || (*it1)->getVertex(1)->getId() == (*iter)->getVertex(0)->getId() )
                        && ((MJMesh::Vertex *)(*it1)->getVertex(0))->getAdjacentFrontElements().size() == 0 )
                {
                    del = true;
                    adj1 = (*it1);
                    v_aux = (*it1)->getVertex(1);
                    break;
                }


                for (Data::FrontElementList::iterator it2 = adjs2.begin(); it2 != adjs2.end(); it2++)
                {
                    if( (*it2)->getId() == (*iter)->getId() )
                        continue;

                    if (!sub_front->have((*it2)))
                    {
                        continue;
                    }

                    //Aresta está solta
                    if( ( (*it2)->getVertex(0)->getId() == (*iter)->getVertex(1)->getId() || (*it2)->getVertex(0)->getId() == (*iter)->getVertex(0)->getId() )
                            && ((MJMesh::Vertex *)(*it2)->getVertex(1))->getAdjacentFrontElements().size() == 0 )
                    {
                        del = true;
                        adj1 = (*it2);
                        v_aux = (*it2)->getVertex(0);
                        break;
                    }
                    if( ( (*it2)->getVertex(1)->getId() == (*iter)->getVertex(1)->getId() || (*it2)->getVertex(1)->getId() == (*iter)->getVertex(0)->getId() )
                            && ((MJMesh::Vertex *)(*it2)->getVertex(0))->getAdjacentFrontElements().size() == 0 )
                    {
                        del = true;
                        adj1 = (*it2);
                        v_aux = (*it2)->getVertex(1);
                        break;
                    }


                    if( (*it1)->getVertex(0)->getId() == (*it2)->getVertex(1)->getId() || (*it1)->getVertex(1)->getId() == (*it2)->getVertex(0)->getId() )
                    {
//std::cout << "INNNNNNNNNNNNNNNNNNNNNNNNNN" << std::endl;
                        flag = true;

                        adj1 = (*it1);
                        adj2 = (*it2);

                        break;
                    }
                }


                if (flag || del)
                {
                    break;
                }
            }

            if(flag)
            {

                //#if USE_OPENMP
                //#pragma omp critical        ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
                //#endif //#if USE_OPENMP
                {
                    /*for (UShort state = 0; state < 4; state++)
                    {
                        for (Data::Front::FrontElementIterator iter = sub_front->eBegin(state);
                             iter != sub_front->eEnd(state); iter++)
                        {
                            if ((*iter)->getId() == adj1->getId())
                            {
                                std::cout << "adj1->id is in sub_front " << (*iter) << " " << adj1 << std::endl;
                                std::cout << "adj1 id" <<  adj1->getId() << std::endl;
                            }

                            if ((*iter) == adj1)
                            {
                                std::cout << "adj1 is in sub_front " << (*iter) << " " << adj1 << std::endl;
                                std::cout << "adj1 id" <<  adj1->getId() << std::endl;
                            }

                            if ((*iter)->getId() == adj2->getId())
                            {
                                std::cout << "adj2->id is in sub_front " << (*iter) << " " << adj2 << std::endl;
                                std::cout << "adj2 id" <<  adj2->getId() << std::endl;
                            }

                            if ((*iter) == adj2)
                            {
                                std::cout << "adj2 is in sub_front " << (*iter) << " " << adj2 << std::endl;
                                std::cout << "adj2 id" <<  adj2->getId() << std::endl;
                            }
                        }
                    }*/

                    if(sub_front->have(adj1))
                    {
                        sub_front->remove(adj1);
                    }
                    if(sub_front->have(adj2))
                    {
                        sub_front->remove(adj2);
                    }


                    if( adj1->getVertex(0)->getId() == adj2->getVertex(1)->getId()
                            || adj1->getVertex(0)->getId() == adj2->getVertex(0)->getId() )
                    {
                        //this->mesh->remove(adj1->getVertex(0));
                        local_front->remove(adj1->getVertex(0));
                        sub_front->remove(adj1->getVertex(0));
                    }
                    else
                    {
                        //this->mesh->remove(adj1->getVertex(1));
                        local_front->remove(adj1->getVertex(1));
                        sub_front->remove(adj1->getVertex(1));
                    }
                    /*
                                    if(this->mesh->have(adj1))
                                    {
                                        this->mesh->remove(adj1);
                                    }
                                    if(this->mesh->have(adj2))
                                    {
                                        this->mesh->remove(adj2);
                                    }
                    */
                    if(local_front->have(adj1))
                    {
                        local_front->remove(adj1);
                        delete adj1;
                    }
                    if(local_front->have(adj2))
                    {
                        local_front->remove(adj2);
                        delete adj2;
                    }

                    //delete adj1;
                    //delete adj2;
                }
            }

            if(del)
            {
                //#if USE_OPENMP
                //#pragma omp critical        ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
                //#endif //#if USE_OPENMP
                {
                    if(sub_front->have(adj1))
                    {
                        sub_front->remove(adj1);
                    }

                    //this->mesh->remove(v_aux);
                    local_front->remove(v_aux);
                    sub_front->remove(v_aux);

                    if(local_front->have(adj1))
                        local_front->remove(adj1);
                    /*
                                    if(this->mesh->have(adj1))
                                    {
                                        this->mesh->remove(adj1);
                                        delete adj1;
                                    }
                    */
                    delete adj1;

                    delete v_aux;
                }
            }
        }
    }
}

void PMJMesh2::APrioriParallelMeshGenerator::cleanOutEdges(PMJMesh::Front *sub_front)
{

    Data::FrontElementList crossing = sub_front->getCrossing();

    //REMOVE ELEMENTOS DUPLICADOS
    //crossing.sort( sort_Element );
    //crossing.unique( same_Element );

    Data::VertexSet crossingVertices;

    Data::FrontElementList removed, disconsider;

    bool changed = false;

    Data::VertexList dontRemove;


    /*for( Data::FrontElementList::iterator it = crossing.begin(); it != crossing.end(); it++)
    {
    std::cout << "LIMPA ID: " << (*it)->getId() << std::endl;
    }*/

#if USE_OPENMP
    #pragma omp critical        ///Front pode ser alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
#endif //#if USE_OPENMP
    {

        do
        {
            changed = false;

            for (Data::FrontElementList::iterator iter = crossing.begin(); iter != crossing.end();)
            {
                //std::cout << "crossing.size = " << crossing.size() << " ID: " << (*iter)->getId() << std::endl;

                Data::FrontElement *fel = (*iter);

                //std::cout << "for fel " << fel->text() << std::endl;

                UInt count[fel->numVertices()];

                for (UInt i = 0; i < fel->numVertices(); i++)
                {
                    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

                    //std::cout << "for v " << v->getId() << " END: " << v << std::endl;

                    Data::FrontElementList adjs = v->getAdjacentFrontElements();

                    count[i] = 1;

                    while (!adjs.empty())
                    {
                        //std::cout << "TAMANHA: " << adjs.size() << std::endl;
                        Data::FrontElement *adj = adjs.front();

                        //std::cout << "adj fel " << adj->text() << std::endl;

                        adjs.pop_front();

                        if (adj == fel)
                        {
                            //std::cout << "ARESTA" << std::endl;
                            continue;
                        }

                        //SE TIVER 1 VERTEX TEM TENHA ADJACENTE QUE NÃO ESTEJA NA CROSS =  MANTEM
                        //CC = REMOVE VERTEX E SUA ARESTA

                        int nearFront = 0;

                        for(Data::FrontElementList::iterator it = crossing.begin(); it != crossing.end(); it++)
                        {
                            if(  adj->getId() == (*it)->getId() )
                            {
                                //count[i]++;
                                count[i] = 20;
                                //std::cout << "CROSS" << (*it)->getId() << std::endl;
                                nearFront++;
                                break;
                            }

                        }

                        if (nearFront > 0)
                        {
                            continue;
                        }

                        //if (front->haveAny(Data::FrontElementList(1, adj)))
                        if (sub_front->have(adj))
                        {
                            //count[i]++;
                            count[i] = 40;

                            //std::cout << "adjacent " << adj->getId() << " is on the front" << std::endl;
                        }
                    }

                    //std::cout << "fel->id = " << fel->getId() << ", v->id = " << v->getId() << ", count = " << count[i] << std::endl;
                }

                //there are only two vertices
                bool remove = false;

                if ( ((count[0] == 20) && (count[1] == 40)) || ((count[1] == 20) && (count[0] == 40)) )
                {
                    //std::cout << "NÂO TIRAAAAAAAAAAAAAAAAAAAAAAAA: " << fel->getId() << std::endl;
                    dontRemove.push_back( fel->getVertex(0) );
                    dontRemove.push_back( fel->getVertex(1) );

                    iter = crossing.erase(iter);

                    continue;
                }

                if ( (count[0] == 20) && (count[1] == 20) )
                {
                    //remove = true;
                    iter++;

                    continue;
                }

                if ((count[0] == 1) || (count[1] == 1))
                {


                    if (count[0] == 1)
                    {
                        bool isIn = false;

                        for (Data::VertexList::iterator it = dontRemove.begin(); it != dontRemove.end(); it++)
                        {
                            if( fel->getVertex(0)->getId() == (*it)->getId() )
                            {
                                //std::cout << "NÂO DELETA VERTEX: " << (*it)->getId() << std::endl;
                                isIn = true;
                                break;
                            }
                        }

                        if(!isIn)
                            if (crossingVertices.insert(fel->getVertex(0)).second)
                            {
                                //std::cout << "DELETOUUUUUUUUUUUUUUUUUUUUUUUUU VERTEX: " << fel->getVertex(0)->getId() << std::endl;
                                //this->front->remove(fel->getVertex(0));
                                if( sub_front->have(fel->getVertex(0)) )
                                    sub_front->remove(fel->getVertex(0));
                            }
                    }



                    if (count[1] == 1)
                    {
                        bool isIn = false;

                        for (Data::VertexList::iterator it = dontRemove.begin(); it != dontRemove.end(); it++)
                        {
                            if( fel->getVertex(1)->getId() == (*it)->getId() )
                            {
                                //std::cout << "NÂO DELETA VERTEX: " << (*it)->getId() << std::endl;
                                isIn = true;
                                break;
                            }
                        }

                        if(!isIn)
                            if (crossingVertices.insert(fel->getVertex(1)).second)
                            {
                                //std::cout << "DELETOUUUUUUUUUUUUUUUUUUUUUUUUU VERTEX: " << fel->getVertex(1)->getId() << std::endl;
                                //this->front->remove(fel->getVertex(1));
                                if( sub_front->have(fel->getVertex(1)) )
                                    sub_front->remove(fel->getVertex(1));
                            }
                    }

                    remove = true;
                }
                else
                {
                    /*
                    if (count[0] != 1)
                        dontRemove.push_back( fel->getVertex(0) );
                    if (count[1] != 1)
                        dontRemove.push_back( fel->getVertex(1) );
                    */
                    iter++;

                    continue;
                }

                if (remove)
                {
                    //std::cout << " ----------  fel " << fel->getId() << " is non-manifold!!!!" << std::endl;

                    //this->front->remove(fel);
                    if( sub_front->have(fel) )
                        sub_front->remove(fel);

                    removed.push_back(fel);

                    iter = crossing.erase(iter);

                    changed = true;
                }
            }
        }
        while (changed);

        //adicionado por Markos em 16/12/2011
        std::multimap<UInt, Data::Vertex *> countVertices;

        //VertexList vertices = this->front->getVertices();
        Data::VertexList vertices = sub_front->getVertices();

        while (!vertices.empty())
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());

            vertices.pop_front();

            //modificado por Markos em 16/09/2015, BSP nao tem mais mestre, todos os processos sao iguais
            //if (this->comm->isMaster())
            {
                UInt count = 0;

                Data::FrontElementList fels = v->getAdjacentFrontElements();

                while (!fels.empty())
                {
                    Data::FrontElement *fel = fels.front();

                    fels.pop_front();

                    //if (this->front->haveAny(Data::FrontElementList(1, fel)))
                    if (sub_front->haveAny(Data::FrontElementList(1, fel)))
                    {
                        count++;
                    }
                    else
                    {
                        disconsider.push_back(fel);
                    }
                }

                countVertices.insert(std::make_pair(count, v));
            }
            //modificado por Markos em 16/09/2015, BSP nao tem mais mestre, todos os processos sao iguais
            /*else
            {
                countVertices.insert(std::make_pair(static_cast<UInt>(v->getAdjacentFrontElements().size()), v));
            }*/
        }

        while (true)
        {
            //VertexMultiMap::iterator iter = countVertices.lower_bound(1);
            PMJMesh::VertexMultiMap::iterator iter = countVertices.begin();

            if (iter == countVertices.end())
            {
                break;
            }

            //std::cout << "trying to remove vertex " << (*iter).second->getId() << " of count " << (*iter).first << std::endl;

            if ((*iter).first != 1)
            {
                break;
            }

            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second);

            countVertices.erase(iter);

            if (crossingVertices.insert(v).second)
            {
                //this->front->remove(v);
                sub_front->remove(v);

                Data::FrontElementList fels = v->getAdjacentFrontElements();

                while (!fels.empty())
                {
                    Data::FrontElement *fel = fels.front();

                    fels.pop_front();

                    //modificado por Markos em 16/09/2015, BSP nao tem mais mestre, todos os processos sao iguais
                    if (/*(this->comm->isMaster()) && */(std::find(disconsider.begin(), disconsider.end(), fel) != disconsider.end()))
                    {
                        continue;
                    }

                    if (std::find(removed.begin(), removed.end(), fel) == removed.end())
                    {
                        //std::cout << " ----------  fel " << fel->getId() << " is non-manifold!!!! (2)" << std::endl;

                        removed.push_back(fel);

                        //this->front->remove(fel);
                        sub_front->remove(fel);

                        Data::Vertex *other = (fel->getVertex(0) == v) ? fel->getVertex(1) : fel->getVertex(0);

                        for (iter = countVertices.begin();
                                iter != countVertices.end(); iter++)
                        {
                            if ((*iter).second == other)
                            {
                                UInt count = (*iter).first;

                                countVertices.erase(iter);

                                if (count == 1)
                                {
                                    if (crossingVertices.insert(other).second)
                                    {
                                        //this->front->remove(other);
                                        sub_front->remove(other);
                                    }
                                }
                                else
                                {
                                    countVertices.insert(std::make_pair(count - 1, other));
                                }

                                break;
                            }
                        }
                    }
                }
            }
        }

    }
//std::cout << "crossing.size = " << crossing.size() << std::endl;
    /*
        for (Data::FrontElementList::iterator iter = crossing.begin();
             iter != crossing.end(); iter++)
         {
    std::cout << " ID: " << (*iter)->getId() << std::endl;
         }


        crossing.swap(removed);

    std::cout << " IDDDDDDDDDDDDDDDD:" << crossing.size() << std::endl;
        for (Data::FrontElementList::iterator iter = crossing.begin();
             iter != crossing.end(); iter++)
         {
    std::cout << " ID: " << (*iter)->getId() << std::endl;
         }
    */
}

void PMJMesh2::APrioriParallelMeshGenerator::splitMesh(PMJMesh::MJMeshTask *task)
{
    PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    PMJMesh::Front *newFront = drive->getNewFront();

    if (newFront->isEmpty())
    {
        /*ULIntFrontElementListMap::iterator it = this->interfaceFrontElements.find(task->getId());

        if (it != this->interfaceFrontElements.end())
        {
            MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());

            PMJMesh::Front *oldFront = drive->getOldFront();

        #if USE_C__11
            for (Data::FrontElementHash::iterator iter = (*it).second.begin();
        #else
            for (Data::FrontElementSet::iterator iter = (*it).second.begin();
        #endif //#if USE_C__11
                 iter != (*it).second.end(); iter++)
            {
                Data::FrontElement *fel = (*iter);

                newFront->addElement(fel);

                if (oldFront->have(fel))
                {
                    oldFront->remove(fel);
                }

                for (UInt i = 0; i < fel->numVertices(); i++)
                {
                    Data::Vertex *v = fel->getVertex(i);

                    if (!newFront->have(v))
                    {
                        newFront->add(v);
                    }

                    if (!oldFront->haveAny(mesh->findAdjacentFrontElements(v)))
                    {
                        oldFront->remove(v);
                    }
                }
            }
        }*/

        this->moveFromOldToNewFront(task);
    }

    PMJMesh2::ParallelMeshGenerator::splitMesh(task);

    //colocando o outro vertice das arestas que soh tem um vertice na task->mesh
    drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());

    MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());

    for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
            iter != mesh->feEnd(); iter++)
    {
        MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

            if (mesh->have(v))
            {
                continue;
            }

            Data::MeshList meshes;

            for (MJMesh::VertexSpecificData::MeshDataMap::iterator iter = v->getData().meshes.begin();
                    iter != v->getData().meshes.end(); iter++)
            {
                meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
            }

            while (!meshes.empty())
            {
                static_cast<MJMesh::Mesh *>(meshes.front())->remove(v);

                meshes.pop_front();
            }

            //Data::cout() << "adding final vertex " << v->text() << " to mesh at " << mesh << Data::endl;

            mesh->add(v, true);
        }
    }
}

void PMJMesh2::APrioriParallelMeshGenerator::makeMaps() const
{
    if (!this->bVertices.empty())
    {
        return;
    }

    PMJMesh2::ParallelMeshGenerator::makeMaps();

    this->bFrontElements.insert(this->interfaceFrontElements.begin(), this->interfaceFrontElements.end());
    this->bVertices.insert(this->interfaceVertices.begin(), this->interfaceVertices.end());

    /*if (this->interfaceFrontElements.empty())
    {
        return;
    }

    ULIntFrontElementListMap::iterator it = --this->interfaceFrontElements.end();

    #if USE_C__11
    for (Data::FrontElementHash::iterator iter = (*it).second.begin();
    #else
    for (Data::FrontElementSet::iterator iter = (*it).second.begin();
    #endif //#if USE_C__11
         iter != (*it).second.end(); iter++)
    {
        Data::FrontElement *fel = (*iter);

        if (this->boundary->have(fel))
        {
            continue;
        }

        fel->invert();

        this->bFrontElements[fel->getId()] = fel;

        for (UInt i = 0; i < fel->numVertices(); i++)
        {
            Data::Vertex *v = fel->getVertex(i);

            if (!this->boundary->have(v))
            {
                this->bVertices[v->getId()] = v;
            }
        }
    }

    this->interfaceFrontElements.erase(it);*/
}

void PMJMesh2::APrioriParallelMeshGenerator::checkAndAddInFront(MJMesh::FrontElement *temp, MJMesh::FrontElement *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const
{
    /*if ((temp) && (temp->getId() == 40))
    {
        std::cout << "APrioriParallelMeshGenerator::checkAndAddInFront temp " << temp << " " << temp->text() << std::endl;
    }

    if ((orig) && (orig->getId() == 40))
    {
        std::cout << "APrioriParallelMeshGenerator::checkAndAddInFront orig " << orig << " " << orig->text() << std::endl;
    }

    if ((temp) && (temp->getId() == 40) && (orig) && (orig->getId() == 40))
    {
        std::cout << "local " << local << ", remote " << remote << std::endl;
        if (local && orig) std::cout << "local->have(orig) " << std::boolalpha << local->have(orig) << std::endl;
        if (remote && temp) std::cout << "remote->have(temp) " << std::boolalpha << remote->have(temp) << std::endl;
    }*/

    if ((temp) && (orig) && (local) && (remote) && (local->have(orig)))
    {
        if (remote->have(temp))
        {
            if (orig->numAdjacents() == 2)
            {
                /*if ((temp) && (temp->getId() == 40) && (orig) && (orig->getId() == 40))
                {
                    std::cout << "adding to frontElementsToDelete" << std::endl;
                }*/

                remote->remove(temp);
                local->remove(orig);

#if USE_DELETE_FELS
                this->frontElementsToDelete.push_back(orig);
#endif //#if USE_DELETE_FELS

                return;
            }
        }
        else if (orig->numAdjacents() == 1)
        {
            //std::cout << "NOT adding to frontElementsToDelete and doing noting" << std::endl;

            return;
        }
    }

    /*if ((temp) && (temp->getId() == 40) && (orig) && (orig->getId() == 40))
    {
        std::cout << "NOT adding to frontElementsToDelete" << std::endl;
    }*/

    PMJMesh2::ParallelMeshGenerator::checkAndAddInFront(temp, orig, remote, local);
}

void PMJMesh2::APrioriParallelMeshGenerator::checkAndAddInFront(MJMesh::Vertex *temp, MJMesh::Vertex *orig, PMJMesh::Front *remote, PMJMesh::Front *local) const
{
    /*if ((temp) && (temp->getId() == 41))
    {
        std::cout << "APrioriParallelMeshGenerator::checkAndAddInFront temp " << temp << " " << temp->text() << std::endl;
    }

    if ((orig) && (orig->getId() == 41))
    {
        std::cout << "APrioriParallelMeshGenerator::checkAndAddInFront orig " << orig << " " << orig->text() << std::endl;
    }

    if ((temp) && (temp->getId() == 41) && (orig) && (orig->getId() == 41))
    {
        std::cout << "local " << local << ", remote " << remote << std::endl;
        if (local && orig) std::cout << "local->have(orig) " << std::boolalpha << local->have(orig) << std::endl;
        if (remote && temp) std::cout << "remote->have(temp) " << std::boolalpha << remote->have(temp) << std::endl;
        std::cout << "local->haveAny(orig->adjFel) " << std::boolalpha << local->haveAny(orig->getAdjacentFrontElements()) << std::endl;
    }*/

    if ((temp) && (orig) && (local) && (remote) && (local->have(orig)) && (remote->have(temp)))
    {
        bool remove = false;

#if USE_DELETE_FELS
        remove = (orig->numAdjacentFrontElements() == 0) || (!local->haveAny(orig->getAdjacentFrontElements()));
#else
        remove = !local->haveAny(orig->getAdjacentFrontElements());
#endif //#if USE_DELETE_FELS

        if (remove)
        {
            remote->remove(temp);
            local->remove(orig);
        }

        return;
    }

    PMJMesh2::ParallelMeshGenerator::checkAndAddInFront(temp, orig, remote, local);
}

#if USE_DELETE_FELS
void PMJMesh2::APrioriParallelMeshGenerator::distributedMerge(PMJMesh::MJMeshTask *task1, PMJMesh::MJMeshTask *task2, PartitionBSPTreeCell *parent) const
{
    PMJMesh2::ParallelMeshGenerator::distributedMerge(task1, task2, parent);

    PMJMesh::WorkerMainDrive *drive1 = dynamic_cast<PMJMesh::WorkerMainDrive *>(task1->getBuilder());

    MJMesh::Mesh *mesh1 = const_cast<MJMesh::Mesh *>(drive1->getMesh());
    MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(drive1->getBoundary()));

    while (!this->frontElementsToDelete.empty())
    {
        Data::FrontElement *fel = this->frontElementsToDelete.front();

        this->frontElementsToDelete.pop_front();

        mesh1->remove(fel);

        if (boundary->have(fel))
        {
            boundary->remove(fel);
        }

        /*if (this->front->have(fel))
        {
            this->front->remove(fel);
        }*/

        /*if ((fel->getId() == 1411) || (fel->getId() == 1367))
        {
            std::cout << "debug APPMG9 removing/deleting from rejectedFrontElements fel " << fel->text() << " at " << fel << ", backtrace: " << std::endl;
            std::cout << Data::trace() << std::endl;
        }*/

        this->rejectedFrontElements.erase(fel);

        //Data::cout() << "deleting temp (3) at " << fel << Data::endl;

        delete fel;
    }
}
#endif //#if USE_DELETE_FELS

#endif //#if USE_TECGRAF_MSH_SURF
