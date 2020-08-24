#ifndef _MJMESH_MIN_TREE_REFINER_H_
#define _MJMESH_MIN_TREE_REFINER_H_

#include "MJMesh/Definitions.h"
#include "Performer/MinTreeRefiner.h"

namespace MJMesh
{
	class MinTreeRefiner : public Performer::MinTreeRefiner
	{
	public:

		MinTreeRefiner(Data::BoxTree *tree, UInt minLevel);
		MinTreeRefiner(Data::BoxTree *tree, const MJMesh::Boundary *boundary);
		virtual ~MinTreeRefiner();

		virtual void setBoundary(const MJMesh::Boundary *boundary);

		virtual UInt findMinLevel() const;

		virtual void execute();

	public:

		enum
		{
			NOT_INIT = 0,
			ENDED,
		};

		static const Performer::Status notInit;
		static const Performer::Status ended;

	protected:

		const MJMesh::Boundary *boundary;
	};
}

#endif //#ifndef _MJMESH_MIN_TREE_REFINER_H_
