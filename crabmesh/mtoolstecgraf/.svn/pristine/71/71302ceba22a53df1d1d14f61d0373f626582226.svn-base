#ifndef _PMJMESH_POSITION_TREE_REFINER_H_
#define _PMJMESH_POSITION_TREE_REFINER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/TreeRefiner.h"

namespace PMJMesh
{
	class PositionTreeRefiner : public Performer::TreeRefiner
	{
	public:

		PositionTreeRefiner(UInt dimension, Data::Tree *tree, /*UInt minLevel,*/
			const Data::PositionMap *position);
		virtual ~PositionTreeRefiner();

		virtual void setDimension(UInt dimension);

		//virtual void setMinLevel(UInt minLevel);

		virtual void setPosition(const Data::PositionMap *position);

		virtual void execute();

		virtual void refine(Data::TreeNode *n);

		virtual void refine(Data::TreeNode *n, UInt numChildren);

	protected:

		virtual void subdivide(Data::TreeNode *n);

		///TODO gambiarra, tirar isso daqui e colocar na classe apropriada
		virtual void setTreeOn(Data::TreeNode *n, bool force = false);

	protected:

		UInt dimension;

		//UInt minLevel;

		const Data::PositionMap *position;
	};
}

#endif //#ifndef _PMJMESH_POSITION_TREE_REFINER_H_
