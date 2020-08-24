#ifndef _PMTOOLS2_MESH_GENERATOR_H_
#define _PMTOOLS2_MESH_GENERATOR_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "PMTools/MeshGenerator.h"

namespace PMTools2
{
	class MeshGenerator : public PMTools::MeshGenerator
	{
	public:

		MeshGenerator();
		virtual ~MeshGenerator();

		virtual void setUseUniformRefinement(bool useUniformRefinement);
		virtual void setUseUniformSize(bool useUniformSize);

	protected:

		virtual PMTools::MshRegion3D *makeMshRegion() const;

	protected:

		bool useUniformRefinement;
		bool useUniformSize;

	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_MESH_GENERATOR_H_
