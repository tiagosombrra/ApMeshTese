#ifndef _PMJMESH_MASTER_CELL_CLASSIFIER_H_
#define _PMJMESH_MASTER_CELL_CLASSIFIER_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/AngleTreeCellClassifier.h"

namespace PMJMesh
{
	class MasterCellClassifier : public PMJMesh::AngleTreeCellClassifier
	{
	public:

		MasterCellClassifier(UInt dimension, const Parallel::ThreadManager *tm, const Data::BoxTree *tree);
		virtual ~MasterCellClassifier();

		virtual void setSlavePosition(const Data::PositionMap *position);

		virtual void execute();

		using PMJMesh::AngleTreeCellClassifier::adjustPosition;
		virtual void adjustPosition();

	protected:

		virtual void setParentPosition(ULInt id, enum Data::Position pos);
		virtual void setParentPosition(Data::BoxTreeCell *c, enum Data::Position pos);

		//virtual bool adjustPosition(ULInt id, UInt numChildren, Data::Position pos, ULInt max);

		virtual void insertMissingPositions();

		virtual bool ignore(ULInt id, Data::Position pos) const;

	protected:

		const Data::PositionMap *slavePosition;
	};
}

#endif //#ifndef _PMJMESH_MASTER_CELL_CLASSIFIER_H_
