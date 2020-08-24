#ifndef _TOOLS_MESH_SPLITTER_H_
#define _TOOLS_MESH_SPLITTER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/MeshSplitter.h"

namespace Tools
{
	class MeshSplitter :
		public Performer::MeshSplitter,
		public Tools::Tool
	{
	public:

		MeshSplitter();
		virtual ~MeshSplitter();

		virtual void execute();

	protected:

		virtual void addToVertices(Data::Vertex *v, Data::Mesh *mesh, Data::VertexList &vertices);

		virtual void copy();
		virtual void copy(Data::Mesh *mesh);

		virtual Data::Mesh *makeMesh() const;
		virtual Data::Boundary *makeBoundary() const;
		virtual Data::Vertex *makeVertex(const Data::Vertex *vertex,
			const Performer::MeshSplitter::VMap &vMapSoFar) const;
		virtual Data::FrontElement *makeFrontElement(const Data::FrontElement *frontElement,
			const Performer::MeshSplitter::VMap &vMap, const Performer::MeshSplitter::FEMap &felMapSoFar) const;
		virtual Data::Element *makeElement(const Data::Element *element,
			const Performer::MeshSplitter::VMap &vMap, const Performer::MeshSplitter::FEMap &felMap, const Performer::MeshSplitter::EMap &elMapSoFar) const;
	};
}

#endif //#ifndef _TOOLS_MESH_SPLITTER_H_
