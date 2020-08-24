#ifndef _PMJMESH_TOPOLOGY_FRONT_ADVANCER_H_
#define _PMJMESH_TOPOLOGY_FRONT_ADVANCER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/TopologyFrontAdvancer.h"

namespace PMJMesh
{
	class TopologyFrontAdvancer : public MJMesh::TopologyFrontAdvancer
	{
	public:

		TopologyFrontAdvancer(UInt dimension, const MJMesh::Boundary *boundary,
			const Data::BoxTree *tree, PMJMesh::Front *front, MJMesh::Mesh *mesh,
			const Data::Box *box, Parallel::Communicator *comm, Real rangeProportion,
			bool usingSearchTrees, bool testingBox = true); ///TODO testingBox must be false for APriori version
		virtual ~TopologyFrontAdvancer();

		virtual void setTestingBox(bool testingBox);
		virtual bool isTestingBox() const;

		virtual void setRangeProportion(Real rangeProportion);
		virtual Real getRangeProportion() const;

#if USE_DELETE_FELS
        virtual void setOldFront(const PMJMesh::Front *oldFront);
#endif //#if USE_DELETE_FELS

		virtual Data::FrontElement *next() const;

	protected:

#if USE_DELETE_FELS
        virtual bool shouldDelete(const Data::FrontElement *e) const;
#endif //#if USE_DELETE_FELS

		using MJMesh::TopologyFrontAdvancer::better;
		virtual bool better(const Data::FrontElement *element, Data::Vertex *candidate, Data::Vertex *old) const;

		using MJMesh::TopologyFrontAdvancer::bestValid;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::FrontElementList &frontElements, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius = -1.0) const;

		virtual bool reject(Data::FrontElement *e, UInt code) const;

	protected:

		PMJMesh::GeometryFrontAdvancer *gfa;
	};
}

#endif //#ifndef _PMJMESH_TOPOLOGY_FRONT_ADVANCER_H_
