#ifndef _PMJMESH_GEOMETRY_FRONT_ADVANCER_H_
#define _PMJMESH_GEOMETRY_FRONT_ADVANCER_H_

#include "PMJMesh/Definitions.h"

#include "MJMesh/GeometryFrontAdvancer.h"

namespace PMJMesh
{
	class GeometryFrontAdvancer : public MJMesh::GeometryFrontAdvancer
	{
	friend
		class TopologyFrontAdvancer;

	public:

		GeometryFrontAdvancer(UInt dimension, const MJMesh::Boundary *boundary,
			const Data::BoxTree *tree, Parallel::Communicator *comm,
			Real rangeProportion,
			bool usingSearchTrees = false,
			bool testingBox = true); ///TODO testingBox must be false for APriori version
		virtual ~GeometryFrontAdvancer();

		virtual UInt getDimension() const;

		virtual void setTestingBox(bool testingBox);
		virtual bool isTestingBox() const;

		virtual void setRangeProportion(Real rangeProportion);
		virtual Real getRangeProportion() const;

#if USE_DELETE_FELS
        virtual void setOldFront(const PMJMesh::Front *oldFront);
#endif //#if USE_DELETE_FELS

		virtual const MJMesh::Boundary *getBoundary() const;

		virtual const Data::BoxTree *getTree() const;

		virtual void setCommunicator(Parallel::Communicator *comm);

		virtual void setBox(const Data::Box *box);
		virtual const Data::Box *getBox() const;

		//virtual const Data::FrontElementList &getCrossing() const;

		virtual void execute();

	protected:

#if USE_DELETE_FELS
        virtual bool shouldDelete(const Data::FrontElement *e) const;
#endif //#if USE_DELETE_FELS

		//using MJMesh::GeometryFrontAdvancer::better;
		//virtual bool better(const Data::FrontElement *element, Data::Vertex *candidate, Data::Vertex *old) const;

		virtual MJMesh::VertexSearchTreeGenerator *makeVertexSearchTreeGenerator(const MJMesh::Front *front = NULL) const;

		virtual MJMesh::FrontElementSearchTreeGenerator *makeFrontElementSearchTreeGenerator(const MJMesh::Front *front = NULL) const;

		//virtual const Data::BoxTreeCell *cell(const Data::Point &p) const;

		virtual bool out(const Data::Point &p) const;
		virtual bool out(const Data::Point *p) const;
		virtual bool outRange(const Data::FrontElement *e, const Data::Point *p, Real radius) const;

		virtual bool isRestricted(const Data::FrontElement *e, const Data::Point *point, Real radius) const;

		virtual void restricted(const Data::FrontElement *e, Data::PointList &ideal, Real radius) const;

		virtual void restricted(Data::VertexList &candidates) const;

		using MJMesh::GeometryFrontAdvancer::bestValid;
		//virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::PointList &candidates) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::FrontElementList &frontElements, Real radius = -1.0) const;
		virtual Data::Vertex *bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius = -1.0) const;

		using MJMesh::GeometryFrontAdvancer::valid;
		virtual Data::VertexList valid(const Data::FrontElement *element, Data::VertexList &candidates, Real radius = -1.0) const;

		virtual Data::FrontElement *next() const;

	protected:

		const bool onlyInBox;
		const bool rangeInBox;
		const bool notInTest;

		bool testingBox;

		Real rangeProportion;

		const Data::Box *box;
		Data::Box *treeRootBox;

#if USE_DELETE_FELS
        const PMJMesh::Front *oldFront;
#endif //#if USE_DELETE_FELS

		//Data::FrontElementList crossing;

		Parallel::Communicator *comm;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeTrees;
		double timeGeometry;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	};
}

#endif //#ifndef _PMJMESH_GEOMETRY_FRONT_ADVANCER_H_
