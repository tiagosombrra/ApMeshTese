#ifndef _PERFORMER_MESH_REFINER_H_
#define _PERFORMER_MESH_REFINER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class MeshRefiner : Performer::Builder
	{
	public:

		MeshRefiner();
		virtual ~MeshRefiner();

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual void setMesh(Data::Mesh *mesh);
		virtual Data::Mesh *getMesh() const;

		virtual void execute() = 0;

	protected:

		Data::Mesh *mesh;

		Performer::IdManager *idManager;
	};
}

#endif //#ifndef _PERFORMER_MESH_REFINER_H_
