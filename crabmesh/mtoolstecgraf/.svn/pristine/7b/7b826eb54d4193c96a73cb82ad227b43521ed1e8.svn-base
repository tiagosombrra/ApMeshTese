#ifndef _MJMESH_NEIGHBOR_TREE_REFINER_H_
#define _MJMESH_NEIGHBOR_TREE_REFINER_H_

#include "MJMesh/Definitions.h"
#include "Performer/NeighborTreeRefiner.h"

namespace MJMesh
{
	class NeighborTreeRefiner : public Performer::NeighborTreeRefiner
	{
	public:

		NeighborTreeRefiner(Data::BoxTree *tree);
		virtual ~NeighborTreeRefiner();

		virtual void execute();

	protected:

		using Performer::NeighborTreeRefiner::fillSet;
		virtual void fillSet();

	public:

		enum
		{
			NOT_INIT = 0,
			ENDED,
		};

		static const Performer::Status notInit;
		static const Performer::Status ended;
	};
}

#endif //#ifndef _MJMESH_NEIGHBOR_TREE_REFINER_H_
