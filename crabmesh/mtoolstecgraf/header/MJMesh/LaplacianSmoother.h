#ifndef _MJMESH_LAPLACIAN_SMOOTHER_H_
#define _MJMESH_LAPLACIAN_SMOOTHER_H_

#include "MJMesh/Definitions.h"
#include "Performer/LaplacianSmoother.h"

namespace MJMesh
{
	class LaplacianSmoother : public Performer::LaplacianSmoother
	{
	public:

		LaplacianSmoother(
			UInt dimension,
			Data::Mesh *mesh = NULL,
			bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0,
			bool sortPointsDisplacement = false);
		virtual ~LaplacianSmoother();

		virtual void execute();

#if (!USE_DELETE_FELS)
#if USE_ATTRIBS
	protected:

		virtual void clearAttribs() const;
#endif //#if USE_ATTRIBS
#endif //#if (!USE_DELETE_FELS)
	};
}

#endif //#ifndef _MJMESH_LAPLACIAN_SMOOTHER_H_
