#ifndef _MJMESH_TOPOLOGY_FRONT_ADVANCER_H_
#define _MJMESH_TOPOLOGY_FRONT_ADVANCER_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/FrontAdvancer.h"

namespace MJMesh
{
	class TopologyFrontAdvancer : public MJMesh::FrontAdvancer
	{
	public:

		TopologyFrontAdvancer(UInt dimension, const MJMesh::Boundary *boundary,
			const Data::BoxTree *tree, MJMesh::Front *front, MJMesh::Mesh *mesh,
			bool usingSearchTrees);
		virtual ~TopologyFrontAdvancer();

		virtual void setFront(MJMesh::Front *front);

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void execute();

	protected:

		using MJMesh::FrontAdvancer::isValid;
		virtual bool isValid(const Data::FrontElement *element, Data::Point *point, Real radius) const;

		virtual Data::PointList ideal(const Data::FrontElement *element, Real factor = 1.0) const;

		virtual Real radius(const Data::FrontElement *element) const;

		using MJMesh::FrontAdvancer::candidates;
		virtual Data::VertexList candidates(const Data::FrontElement *element) const;

		using MJMesh::FrontAdvancer::best;
		virtual Data::Vertex *best(const Data::FrontElement *element) const;
	};
}

#endif //#ifndef _MJMESH_TOPOLOGY_FRONT_ADVANCER_H_
