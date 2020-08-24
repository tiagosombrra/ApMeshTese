#ifndef _TOOLS_VERTEX_MERGER_H_
#define _TOOLS_VERTEX_MERGER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/VertexMerger.h"

namespace Tools
{
	class VertexMerger :
		public Performer::VertexMerger,
		public Tools::Tool
	{
	public:

		VertexMerger();
		virtual ~VertexMerger();

		virtual void execute();

	protected:

		virtual void mergeMesh(Data::Vertex *v, Data::Mesh::VertexIterator iter);
		virtual void mergeBoundary(Data::Vertex *v, Data::Boundary::VertexIterator iter);

		virtual void replace(Data::FrontElement *fel, Data::Vertex *vOld, Data::Vertex *vNew) const;
		virtual void replace(Data::Element *e, Data::Vertex *vOld, Data::Vertex *vNew) const;

	};
}

#endif //#ifndef _TOOLS_VERTEX_MERGER_H_
