#ifndef _PMJMESH_PARTITION_OCT_TREE_CELL_H_
#define _PMJMESH_PARTITION_OCT_TREE_CELL_H_

#include "PMJMesh/Definitions.h"

#include "Data/OctTreeCell.h"

namespace PMJMesh
{
	class PartitionOctTreeCell : public Data::OctTreeCell
	{
	public:

		PartitionOctTreeCell(Data::BoxTreeCell *original,
			PMJMesh::PartitionOctTreeCell *parent = NULL, ULInt id = 0);
		virtual ~PartitionOctTreeCell();

		virtual void setOriginal(Data::BoxTreeCell *original);
		virtual Data::BoxTreeCell *getOriginal() const;

		virtual void subdivide();

		virtual bool hasNeighbors(UInt side) const;
		virtual void setNeighbors(UInt side, const Data::GraphNodeList &neighbors);
		virtual Data::GraphNodeList getNeighbors(UInt side) const;
		virtual void clearNeighbors();

#if USE_ARRAY
		using Data::OctTreeCell::operator=;
		virtual PartitionOctTreeCell &operator=(const PartitionOctTreeCell &c);
#endif //#if USE_ARRAY

	protected:

		Data::GraphNodeList neighbors[6];
		Data::BoolVector neighborsSet;

		Data::BoxTreeCell *original;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_OCT_TREE_CELL_H_s
