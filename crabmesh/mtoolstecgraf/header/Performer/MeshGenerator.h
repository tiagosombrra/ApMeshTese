#ifndef _PERFORMER_MESH_GENERATOR_H_
#define _PERFORMER_MESH_GENERATOR_H_

#include "Performer/Builder.h"

namespace Performer
{
	class MeshGenerator : public Performer::Builder
	{
	public:

		MeshGenerator();
		virtual ~MeshGenerator();

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual Data::Mesh *getMesh() const;

		virtual void execute() = 0;

	protected:

		Data::Mesh *mesh;

		Performer::IdManager *idManager;
	};
}

#endif //#ifndef _PERFORMER_MESH_GENERATOR_H_
