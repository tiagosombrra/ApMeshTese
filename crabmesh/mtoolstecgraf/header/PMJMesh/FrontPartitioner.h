#ifndef _PMJMESH_FRONT_PARTITIONER_H_
#define _PMJMESH_FRONT_PARTITIONER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/Clipper.h"
#include "Parallel/Partitioner.h"

namespace PMJMesh
{
	class FrontPartitioner : public Parallel::Partitioner
	{
	public:

		FrontPartitioner(UInt dimension, const MJMesh::Front *front,
			const Parallel::PartitionVector &partitions);
		FrontPartitioner(UInt dimension, const MJMesh::Front *front,
			const Parallel::TaskVector &tasks, UInt shift, bool replaceCrossing = false);
		virtual ~FrontPartitioner();

		virtual void setDimension(UInt dimension);

		virtual void setFront(const MJMesh::Front *front);

		virtual void setPartitions(const Parallel::PartitionVector &partitions);
		virtual void setTasks(const Parallel::TaskVector &tasks);

		virtual void setShift(UInt shift);

		virtual void setReplaceCrossing(bool replaceCrossing);

		virtual void setReplacer(CrossingReplacer *replacer);
		virtual CrossingReplacer *getReplacer() const;

		virtual void ensureExistenceOfSubfront(PMJMesh::Partition *p) const;

		virtual void execute();

	protected:

		//virtual bool out(const Data::BoxTreeCell *cell) const;
		//this method tests if the given front element is completely outside
		// the given box
		virtual bool out(const Data::Box *box, const MJMesh::FrontElement *e) const;
		//test if all the points are inside the cell
		virtual bool completelyIn(const Data::Box *box, const MJMesh::FrontElement *e) const;

#if USE_EXPERIMENTAL_3
		virtual Data::Point2DObjList clip2D(const Data::Box *box, const Data::GeometricShape *s) const;
		virtual Data::Point3DObjList clip3D(const Data::Box *box, const Data::GeometricShape *s) const;
#else
		virtual Data::PointObjList clip(const Data::Box *box, const Data::GeometricShape *s) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual bool belongTo(const MJMesh::FrontElement *e, PMJMesh::Partition *p, Parallel::PartitionList &partitions, bool &exclusive) const;

		//virtual Parallel::PartitionList findPartitions(const MJMesh::FrontElement *e) const;
		virtual bool findPartitions(const MJMesh::FrontElement *e, Parallel::PartitionList &partitions) const;
		//virtual void findPartitions(Data::FrontElementList &fels, bool boundary);
		virtual void findPartitions(UShort state, bool boundary);

	protected:

		Parallel::TaskVector tasks;

		UInt shift;

		UInt dimension;

		const MJMesh::Front *front;

		bool replaceCrossing;

		CrossingReplacer *replacer;
	};
}

#endif //#ifndef _PMJMESH_FRONT_PARTITIONER_H_
