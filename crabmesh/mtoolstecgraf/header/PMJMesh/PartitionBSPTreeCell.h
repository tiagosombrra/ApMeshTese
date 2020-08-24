#ifndef _PMJMESH_PARTITION_BSP_TREE_CELL_H_
#define _PMJMESH_PARTITION_BSP_TREE_CELL_H_

#include "PMJMesh/Definitions.h"
#include "Data/BSPTreeCell.h"

namespace PMJMesh
{
	class PartitionBSPTreeCell : public Data::BSPTreeCell
	{
	public:

		PartitionBSPTreeCell(PMJMesh::PartitionBSPTreeCell *parent = NULL, ULInt id = 0);
		virtual ~PartitionBSPTreeCell();

		virtual void subdivide();

		virtual bool hasNeighbors(Data::UIntVector sides) const;
		virtual void setNeighbors(Data::UIntVector sides, const Data::GraphNodeList &neighs);
		virtual void getNeighbors(Data::UIntVector sides, Data::GraphNodeList &neighs) const;
		virtual void clearNeighbors();

#if USE_ARRAY
		using Data::BSPTreeCell::operator=;
		virtual PartitionBSPTreeCell &operator=(const PartitionBSPTreeCell &c);
#endif //#if USE_ARRAY

	protected:

#if USE_C__11
		/*struct NeighborMapEqual
		{
			bool operator()(Data::UIntVector const &v1, Data::UIntVector const &v2) const
			{
				size_t size = v1.size();

				if (size != v2.size())
				{
					return false;
				}

				for (UInt i = 0; i < size; i++)
				{
					if (v1[i] != v2[i])
					{
						return false;
					}
				}

				return true;
			};
		};*/

		struct NeighborMapHash
		{
			size_t operator()(Data::UIntVector const &n) const
			{
				size_t s = n.size();

				return (s == 0) ? static_cast<size_t>(0) :
						((s == 1) ? static_cast<size_t>(n[0]) :
						((s == 2) ? static_cast<size_t>(5*(n[0] + 1) + n[1]) :
						((s == 3) ? static_cast<size_t>(5*(n[0] + 1) + 4*(n[1] + 1) + n[2] + 2) : std::hash<ULInt>()(n[0]))));
			}
		};

		typedef std::unordered_map<Data::UIntVector, Data::GraphNodeList, NeighborMapHash/*, NeighborMapEqual*/> NeighborMap;
#else
		typedef std::map<Data::UIntVector, Data::GraphNodeList> NeighborMap;
#endif //#if USE_C__11

	protected:

		NeighborMap neighs;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_BSP_TREE_CELL_H_
