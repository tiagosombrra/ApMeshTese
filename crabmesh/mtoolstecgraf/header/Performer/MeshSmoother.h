#ifndef _PERFORMER_MESH_SMOOTHER_H_
#define _PERFORMER_MESH_SMOOTHER_H_

#include "Performer/Definitions.h"

#include "Performer/MeshOptimizer.h"
#include "Data/Mesh.h"

namespace Performer
{
	class MeshSmoother : public Performer::MeshOptimizer
	{
	public:

		MeshSmoother(Data::Mesh *mesh);
		virtual ~MeshSmoother();

		virtual bool smooth(Data::Vertex *v) const = 0;

		virtual void execute();

	private:

		//Data::VertexList vertices;
		//Data::VertexList::iterator currVertex;
#if USE_EXPERIMENTAL_MESH
		Data::Mesh::InnerVertexIterator currVertex;
#else
		Data::Mesh::VertexIterator currVertex;
#endif //#if USE_EXPERIMENTAL_MESH
	};
}

#endif //#ifndef _PERFORMER_MESH_SMOOTHER_H_
