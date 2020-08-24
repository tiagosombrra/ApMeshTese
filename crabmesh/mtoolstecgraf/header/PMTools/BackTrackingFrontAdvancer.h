#ifndef _PMTOOLS_BACK_TRACKING_FRONT_ADVANCER_H_
#define _PMTOOLS_BACK_TRACKING_FRONT_ADVANCER_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "MJMesh/BackTrackingFrontAdvancer.h"
#include "OctTree.hpp"

namespace PMTools
{
	class BackTrackingFrontAdvancer : public MJMesh::BackTrackingFrontAdvancer
	{
	public:

		BackTrackingFrontAdvancer(const FTools::OctTree *ftree,
			const MJMesh::Boundary *boundary, MJMesh::Front *front, MJMesh::Mesh *mesh);
		virtual ~BackTrackingFrontAdvancer();

		virtual void execute();

	protected:

		virtual void deleteNonBoundaryFels();

	protected:

		const FTools::OctTree *ftree;

#if DEBUG_TIMING_PMTOOLS
	public:

		mutable Real timeBackTracking, timeToMToolsConversion, timeFromMToolsConversion;
#endif //#if DEBUG_TIMING_PMTOOLS

	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_BACK_TRACKING_FRONT_ADVANCER_H_

