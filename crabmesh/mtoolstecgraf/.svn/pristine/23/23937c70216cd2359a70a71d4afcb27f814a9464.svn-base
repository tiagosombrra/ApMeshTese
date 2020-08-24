#ifndef _PMJMESH2_A_PRIORI_SUPPORT_MESH_LAPLACIAN_SMOOTHER_H_
#define _PMJMESH2_A_PRIORI_SUPPORT_MESH_LAPLACIAN_SMOOTHER_H_

#include "PMJMesh2/Definitions.h"
#include "MJMesh/LaplacianSmoother.h"

namespace PMJMesh2
{
    class APrioriSupportMeshLaplacianSmoother : public MJMesh::LaplacianSmoother
    {
    public:

        APrioriSupportMeshLaplacianSmoother(
			Data::Mesh *mesh = NULL,
			Data::Boundary *boundary = NULL,
			bool testAdjacency = true,
			Real factor = 0.5,
			UInt maxCount = 10,
			UInt minMoved = 0,
			bool sortPoints = false);
		virtual ~APrioriSupportMeshLaplacianSmoother();

		virtual void setBoundary(Data::Boundary *boundary);

		virtual void setMaxCount(UInt maxCount);

		virtual void setMinMoved(UInt minMoved);

		virtual void execute();

    protected:

        Data::Boundary *boundary;
        UInt maxCount;
        UInt minMoved;


    };
}


#endif //#ifndef _PMJMESH2_A_PRIORI_SUPPORT_MESH_LAPLACIAN_SMOOTHER_H_
