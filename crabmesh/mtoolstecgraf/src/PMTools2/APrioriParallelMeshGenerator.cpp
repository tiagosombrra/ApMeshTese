#include "PMTools2/APrioriParallelMeshGenerator.h"

#if USE_MTOOLS

#include "Parallel/Task.h"
#include "PMTools2/WorkerMainDrive.h"
#include "PMJMesh2/PartitionBSPTree.h"

PMTools2::APrioriParallelMeshGenerator::APrioriParallelMeshGenerator() :
    PMJMesh2::APrioriParallelMeshGenerator(),
    PMTools2::ParallelMeshGenerator()
{

}

PMTools2::APrioriParallelMeshGenerator::~APrioriParallelMeshGenerator()
{

}

void PMTools2::APrioriParallelMeshGenerator::execute()
{
    PMJMesh2::APrioriParallelMeshGenerator::execute();
}

void PMTools2::APrioriParallelMeshGenerator::finalize()
{
    PMTools2::ParallelMeshGenerator::finalize();
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
std::string PMTools2::APrioriParallelMeshGenerator::timing(const PMJMesh2::Task *task, Real timeTotal, const Data::ULI4Tuple &prev) const
{
    return PMTools2::ParallelMeshGenerator::timing(task, timeTotal, prev);
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

const PMJMesh2::Task *PMTools2::APrioriParallelMeshGenerator::cast(const Parallel::Task *t) const
{
    return PMTools2::ParallelMeshGenerator::cast(t);
}

PMJMesh2::Task *PMTools2::APrioriParallelMeshGenerator::cast(Parallel::Task *t) const
{
    return PMTools2::ParallelMeshGenerator::cast(t);
}

bool PMTools2::APrioriParallelMeshGenerator::setTaskParameters(Parallel::Task *t, MJMesh::Boundary *boundary,
    Data::BoxTree *tree, Performer::IdManager *idManager) const
{
    bool set = PMTools2::ParallelMeshGenerator::setTaskParameters(t, boundary, tree, idManager);

	if (set)
	{
		PMTools2::WorkerMainDrive *drive = dynamic_cast<PMTools2::WorkerMainDrive *>(t->getBuilder());

		drive->setTestingBox(false);
		drive->setCreateInnerFrontInDecomposition(false);

		drive->setShouldAdvanceFront(static_cast<Data::TreeNode *>(this->bsp->node(t->getId()))->isLeaf());
	}

	return set;
}

void PMTools2::APrioriParallelMeshGenerator::getParameters(const Parallel::Task *t) const
{
    PMTools2::ParallelMeshGenerator::getParameters(t);
}

bool PMTools2::APrioriParallelMeshGenerator::isInError(const Parallel::Task *t) const
{
    return PMTools2::ParallelMeshGenerator::isInError(t);
}

void PMTools2::APrioriParallelMeshGenerator::executeFrontCheck()
{
    PMTools2::ParallelMeshGenerator::executeFrontCheck();
}

#endif //#if USE_MTOOLS
