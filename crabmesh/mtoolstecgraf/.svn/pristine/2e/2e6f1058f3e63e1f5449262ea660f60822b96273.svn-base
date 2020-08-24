#ifndef _PMJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
#define _PMJMESH_BACK_TRACKING_FRONT_ADVANCER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/BackTrackingFrontAdvancer.h"

namespace PMJMesh
{
	class BackTrackingFrontAdvancer : public MJMesh::BackTrackingFrontAdvancer
	{
	public:

		BackTrackingFrontAdvancer(UInt dimension, MJMesh::Boundary *boundary,
			Data::BoxTree *tree, MJMesh::Mesh *mesh, MJMesh::Front *front,
			//bool adjustFront = true, bool deleteElements = true,
			bool adjustFront, bool deleteElements,
			//UInt maxBackTracks = 0);
			UInt maxBackTracks, enum MJMesh::BackTrackingFrontAdvancer::Mode mode);
		virtual ~BackTrackingFrontAdvancer();

		virtual void deleteOldMesh();
		virtual void restoreOldMesh();

	protected:

		virtual void invert();
	};
}

#endif //#ifndef _PMJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
