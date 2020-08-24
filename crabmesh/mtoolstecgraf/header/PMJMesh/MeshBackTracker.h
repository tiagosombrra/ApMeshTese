#ifndef _PMJMESH_MESH_BACK_TRACKER_H_
#define _PMJMESH_MESH_BACK_TRACKER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/MeshBackTracker.h"

namespace PMJMesh
{
	class MeshBackTracker : public MJMesh::MeshBackTracker
	{
	public:

		MeshBackTracker(UInt dimension, Data::Mesh *mesh, Data::Boundary *boundary,
			Data::BoxTree *tree, Data::Front *front, bool meshSorted,
			UInt maxBackTracks);
		virtual ~MeshBackTracker();

	protected:

		virtual MJMesh::BackTrackingFrontAdvancer *makeBackTrackingFrontAdvancer() const;

		virtual void clearBtfa();
	};
}

#endif //#ifndef _PMJMESH_MESH_BACK_TRACKER_H_
