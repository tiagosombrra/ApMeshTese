#ifndef _PMJMESH2_PARTITION_BSP_TREE_CELL_H_
#define _PMJMESH2_PARTITION_BSP_TREE_CELL_H_

#include "PMJMesh2/Definitions.h"
#include "Data/BSPTreeCell.h"

#if USE_OPENMP
#include "Parallel/OMPLock.h"
#endif //#if USE_OPENMP

namespace PMJMesh2
{
    class PartitionBSPTreeCell : public Data::BSPTreeCell
    {
    public:

		PartitionBSPTreeCell(PartitionBSPTreeCell *parent = NULL, ULInt id = 0);
		virtual ~PartitionBSPTreeCell();

		virtual void subdivide();

		virtual void setPeer(Int peer);
		virtual UInt getPeer() const;

		virtual void setLoad(Real load);
		virtual Real getLoad() const;

		virtual void setTask(Parallel::Task *task);
		virtual Parallel::Task *getTask() const;

		virtual void setDistLimits(Int min, Int max);
		virtual Int getMinDistLimit() const;
		virtual Int getMaxDistLimit() const;

		virtual void setSharLimits(Int min, Int max);
		virtual Int getMinSharLimit() const;
		virtual Int getMaxSharLimit() const;

#if USE_OPENMP
		virtual Parallel::OMPLock &getLock();
#endif //#if USE_OPENMP

#if USE_ARRAY
		using Data::BSPTreeCell::operator=;
		virtual PartitionBSPTreeCell &operator=(const PartitionBSPTreeCell &c);
#endif //#if USE_ARRAY

/*//debug
#if USE_GUI
    public:

        Data::GraphNodeList cells;
#endif //#if USE_GUI
//endebug*/

    protected:

        Int peer;
        Real load;

        Int distLimits[2];
        Int sharLimits[2];

#if USE_OPENMP
        Parallel::OMPLock lock;
#endif //#if USE_OPENMP

        Parallel::Task *task;

    };
}

#endif //#ifndef _PMJMESH2_PARTITION_BSP_TREE_CELL_H_
