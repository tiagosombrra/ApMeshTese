#ifndef _PJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
#define _PJMESH_BACK_TRACKING_FRONT_ADVANCER_H_

#include "PJmesh/Definitions.h"

#include "MJMesh/BackTrackingFrontAdvancer.h"

namespace PJmesh
{
	class BackTrackingFrontAdvancer : public MJMesh::BackTrackingFrontAdvancer
	{
	public:

		BackTrackingFrontAdvancer(const MJMesh::Boundary *boundary,
			MJMesh::Front *front, MJMesh::Mesh *mesh);
		virtual ~BackTrackingFrontAdvancer();

		virtual void execute();

	protected:

		virtual void deleteNonBoundaryFels();

	protected:

	};
}

#endif //#ifndef _PJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
