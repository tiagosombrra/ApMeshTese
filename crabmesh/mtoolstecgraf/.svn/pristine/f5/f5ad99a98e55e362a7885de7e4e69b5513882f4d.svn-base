#ifndef _PERFORMER_VERTEX_MERGER_H_
#define _PERFORMER_VERTEX_MERGER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

namespace Performer
{
	class VertexMerger : public Performer::Builder
	{
	public:

		VertexMerger();
		virtual ~VertexMerger();

		virtual void setMesh(Data::Mesh *mesh);
		virtual void setBoundary(Data::Boundary *boundary);
		virtual void setUseTolerance(bool useTolerance);
		virtual void setTolerance(Real tolerance);

		virtual bool isMerged() const;

		virtual void execute();

	protected:

#if USE_EXPERIMENTAL_3
		typedef std::list<Data::Vector *> VectorList;
#else
		typedef std::list<Data::Vector> VectorList;
#endif //#if USE_EXPERIMENTAL_3

	protected:

		virtual void mergeMesh(Data::Vertex *v, Data::Mesh::VertexIterator iter);
		virtual void mergeBoundary(Data::Vertex *v, Data::Boundary::VertexIterator iter);

		virtual void add(bool match, bool equal, VectorList &vectors, UInt &numVectors, Data::Vertex *v, Data::Vertex *o) const;
		virtual void sum(Data::Vertex *v, VectorList &vectors, UInt numVectors);

		virtual void matchOrEqual(const Data::Vertex *v1, const Data::Vertex *v2, bool &match, bool &equal) const;

		virtual bool equal(const Data::Vertex *v1, const Data::Vertex *v2) const;
		virtual bool match(const Data::Vertex *v1, const Data::Vertex *v2) const;

		virtual void replace(Data::FrontElement *fel, Data::Vertex *vOld, Data::Vertex *vNew) const;
		virtual void replace(Data::Element *e, Data::Vertex *vOld, Data::Vertex *vNew) const;

	protected:

		UInt dimension;

		Data::Mesh *mesh;
		Data::Boundary *boundary;

		bool useTolerance;
		Real tolerance;

		bool merged;

	};
}

#endif //#ifndef _PERFORMER_VERTEX_MERGER_H_
