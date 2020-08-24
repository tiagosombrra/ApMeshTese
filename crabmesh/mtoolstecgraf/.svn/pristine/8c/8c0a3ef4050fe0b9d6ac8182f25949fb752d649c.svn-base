#include "PMJMesh2/PartitionBSPTreeCell.h"

#include "PMJMesh2/PartitionBSPTree.h"

using namespace PMJMesh2;

PMJMesh2::PartitionBSPTreeCell::PartitionBSPTreeCell(PartitionBSPTreeCell *parent, ULInt id) :
    Data::BSPTreeCell::BSPTreeCell(parent, id)
{
    this->setPeer(-1);
    this->setLoad(0.0);
    this->setTask(NULL);
    this->setDistLimits(0, 0);
    this->setSharLimits(0, 0);
}

PMJMesh2::PartitionBSPTreeCell::~PartitionBSPTreeCell()
{

}

void PMJMesh2::PartitionBSPTreeCell::subdivide()
{
	ULInt id = 2*this->dimension()*this->getId() + 2*this->getSplitDirection();

	new PartitionBSPTreeCell(this, ++id);
	new PartitionBSPTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
    {
        this->getTree()->addChildren(this);
    }
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

void PMJMesh2::PartitionBSPTreeCell::setPeer(Int peer)
{
    this->peer = peer;
}

UInt PMJMesh2::PartitionBSPTreeCell::getPeer() const
{
    return this->peer;
}

void PMJMesh2::PartitionBSPTreeCell::setLoad(Real load)
{
    this->load = load;
}

Real PMJMesh2::PartitionBSPTreeCell::getLoad() const
{
    return this->load;
}

void PMJMesh2::PartitionBSPTreeCell::setTask(Parallel::Task *task)
{
    this->task = task;
}

Parallel::Task *PMJMesh2::PartitionBSPTreeCell::getTask() const
{
    return this->task;
}

void PMJMesh2::PartitionBSPTreeCell::setDistLimits(Int min, Int max)
{
    this->distLimits[0] = min;
    this->distLimits[1] = max;
}

Int PMJMesh2::PartitionBSPTreeCell::getMinDistLimit() const
{
    return this->distLimits[0];
}

Int PMJMesh2::PartitionBSPTreeCell::getMaxDistLimit() const
{
    return this->distLimits[1];
}

void PMJMesh2::PartitionBSPTreeCell::setSharLimits(Int min, Int max)
{
    this->sharLimits[0] = min;
    this->sharLimits[1] = max;
}

Int PMJMesh2::PartitionBSPTreeCell::getMinSharLimit() const
{
    return this->sharLimits[0];
}

Int PMJMesh2::PartitionBSPTreeCell::getMaxSharLimit() const
{
    return this->sharLimits[1];
}

#if USE_OPENMP
Parallel::OMPLock &PMJMesh2::PartitionBSPTreeCell::getLock()
{
    return this->lock;
}
#endif //#if USE_OPENMP

#if USE_ARRAY
PMJMesh2::PartitionBSPTreeCell &PMJMesh2::PartitionBSPTreeCell::operator=(const PartitionBSPTreeCell &c)
{
	if (this != &c)
	{
		Data::BSPTreeCell::operator=(c);

		this->peer = c.peer;
#if USE_OPENMP
		this->lock = c.lock;
#endif //#if USE_OPENMP
	}

	return *this;
}
#endif //#if USE_ARRAY
