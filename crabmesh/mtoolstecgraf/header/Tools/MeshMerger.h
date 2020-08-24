#ifndef _TOOLS_MESH_MERGER_H_
#define _TOOLS_MESH_MERGER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/MeshMerger.h"

namespace Tools
{
	class MeshMerger :
		public Performer::MeshMerger,
		public Tools::Tool
	{
	public:

		MeshMerger();
		virtual ~MeshMerger();

		virtual void execute();

    protected:

#if USE_C__11
        typedef std::unordered_map<ULInt, Data::FrontElementList> FEAdjacency;
#else
        typedef std::map<ULInt, Data::FrontElementList> FEAdjacency;
#endif //#if USE_C__11

	protected:

		virtual void merge(const Data::Mesh *mesh);

		virtual Data::Mesh *makeMesh() const;
		virtual Data::Boundary *makeBoundary() const;
		virtual Data::Vertex *makeVertex(const Data::Vertex *vertex,
			const Performer::MeshMerger::VMap &vMapSoFar) const;
		virtual Data::FrontElement *makeFrontElement(const Data::FrontElement *frontElement,
			const Performer::MeshMerger::VMap &vMap, const Performer::MeshMerger::FEMap &felMapSoFar) const;
		virtual Data::Element *makeElement(const Data::Element *element,
			const Performer::MeshMerger::VMap &vMap, const Performer::MeshMerger::FEMap &felMap, const Performer::MeshMerger::EMap &elMapSoFar) const;

    protected:

        mutable FEAdjacency feAdjacency;
	};
}

#endif //#ifndef _TOOLS_MESH_MERGER_H_
