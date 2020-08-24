#ifndef _PMJMESH_BOUNDARY_BUILDER_H_
#define _PMJMESH_BOUNDARY_BUILDER_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Builder.h"
#include "MJMesh/BoundaryBuilder.h"

namespace PMJMesh
{
	class BoundaryBuilder : public MJMesh::BoundaryBuilder, public Parallel::Builder
	{
	public:

		BoundaryBuilder(Parallel::Communicator *comm);
		virtual ~BoundaryBuilder();

		virtual void setDimension(UInt dimension);

		virtual void execute();

	protected:

		UInt dimension;
	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_BUILDER_H_
