#ifndef _PERFORMER_BOUNDARY_ADAPTER_H_
#define _PERFORMER_BOUNDARY_ADAPTER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryAdapter : public Performer::Builder
	{
	public:

		BoundaryAdapter(Data::Boundary *boundary = NULL);
		BoundaryAdapter(Data::Mesh *surface);
		virtual ~BoundaryAdapter();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual Data::Boundary *getBoundary() const;

		virtual void setSurface(Data::Mesh *surface);
		virtual Data::Mesh *getSurface() const;

		virtual Data::AbstractMesh *getMesh() const;

		virtual void setIdManager(IdManager *idManager);
		virtual IdManager *getIdManager() const;

		virtual bool isBoundary() const;
		virtual bool isSurface() const;

		virtual void add(Data::Vertex *v) const;
		virtual void add(Data::Element *e) const;
		virtual void remove(Data::Vertex *v) const;
		virtual void remove(Data::Element *e) const;

		virtual Data::ElementList adjacency(const Data::Vertex *v) const;

		virtual bool adapt(Data::/*Front*/Element *element,
            Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const = 0;

		virtual void execute() = 0;

	private:

		mutable Data::AbstractMesh *mesh;
		bool boundary;

    protected:

		IdManager *idManager;
	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_ADAPTER_H_
