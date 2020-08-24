#ifndef _PERFORMER_MESH_OPTIMIZER_H_
#define _PERFORMER_MESH_OPTIMIZER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class MeshOptimizer : public Performer::Builder
	{
	public:

		MeshOptimizer(Data::Mesh *mesh = NULL);
		virtual ~MeshOptimizer();

		virtual void setMesh(Data::Mesh *mesh);
		virtual Data::Mesh *getMesh() const;

		virtual void execute() = 0;

		enum
		{
			NOT_INIT = 0,
			INIT,
			ENDED
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status ended;

	protected:

		Data::Mesh *mesh;
	};
}

#endif //#ifndef _PERFORMER_MESH_OPTIMIZER_H_
