#ifndef _PMJMESH_PARTITION_QUAD_TREE_CELL_H_
#define _PMJMESH_PARTITION_QUAD_TREE_CELL_H_

#include "PMJMesh/Definitions.h"

#include "Data/QuadTreeCell.h"

namespace PMJMesh
{
	class PartitionQuadTreeCell : public Data::QuadTreeCell
	{
	public:

		PartitionQuadTreeCell(Data::BoxTreeCell *original,
			PMJMesh::PartitionQuadTreeCell *parent = NULL, ULInt id = 0);
		virtual ~PartitionQuadTreeCell();

		virtual void setOriginal(Data::BoxTreeCell *original);
		virtual Data::BoxTreeCell *getOriginal() const;

		virtual void subdivide();

		virtual bool hasNeighbors(UInt side) const;
		virtual void setNeighbors(UInt side, const Data::GraphNodeList &neighbors);
		virtual Data::GraphNodeList getNeighbors(UInt side) const;
		virtual void clearNeighbors();

#if USE_ARRAY
		using Data::QuadTreeCell::operator=;
		virtual PartitionQuadTreeCell &operator=(const PartitionQuadTreeCell &c);
#endif //#if USE_ARRAY

	protected:

		Data::GraphNodeList neighbors[4];
		Data::BoolVector neighborsSet;

		Data::BoxTreeCell *original;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_QUAD_TREE_CELL_H_
