#ifndef _TOOLS_MESH_TOPOLOGY_CHECKER_H_
#define _TOOLS_MESH_TOPOLOGY_CHECKER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/MeshTopologyChecker.h"
#if USE_TECGRAF
#include "TecGraf/Definitions.h"
#endif //#if USE_TECGRAF

namespace Tools
{
	class MeshTopologyChecker :
		public Performer::MeshTopologyChecker,
		public Tools::Tool
	{
	public:

		MeshTopologyChecker();
		virtual ~MeshTopologyChecker();

#if USE_TECGRAF
		virtual void setModel(const TecGraf::Model *model);
#endif //#if USE_TECGRAF

		virtual void execute();

	protected:

	    using Performer::MeshTopologyChecker::isTestingAngles;
	    virtual bool isTestingAngles() const;

		//virtual bool check(const Data::Mesh::ConstVertexIterator &iter, const Data::Mesh::ConstVertexIterator &end) const;
		virtual bool check(const Data::Vertex *test) const;
		//virtual bool check(const Data::Mesh::ConstFrontElementIterator &iter, const Data::Mesh::ConstFrontElementIterator &end) const;
		virtual bool check(const Data::FrontElement *test) const;
		//virtual bool check(const Data::Mesh::ConstElementIterator &iter, const Data::Mesh::ConstElementIterator &end) const;
		virtual bool check(const Data::Element *test) const;

		virtual bool check(const Data::FrontElement *test, UInt i) const;
		virtual bool check(const Data::Element *test, UInt i) const;

	protected:

		const MJMesh::Mesh *mjmesh;
		bool surface;

#if USE_TECGRAF
		const TecGraf::Model *model;
#endif //#if USE_TECGRAF

	};
}

#endif //#ifndef _TOOLS_MESH_TOPOLOGY_CHECKER_H_
