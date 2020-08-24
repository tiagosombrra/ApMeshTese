#ifndef _PERFORMER_MESH_BACK_TRACKER_H_
#define _PERFORMER_MESH_BACK_TRACKER_H_

#include "Performer/Definitions.h"

#include "Performer/MeshOptimizer.h"

namespace Performer
{
	class MeshBackTracker : public Performer::MeshOptimizer
	{
	public:

		MeshBackTracker(Data::Mesh *mesh = NULL, Data::Boundary *boundary = NULL,
			Data::Front *front = NULL, bool meshSorted = true);
		virtual ~MeshBackTracker();

		virtual void setBoundary(Data::Boundary *boundary);

		virtual void setFront(Data::Front *front);

		virtual void setMeshSorted(bool meshSorted);

		virtual void setIdManager(Performer::IdManager *idManager);

	protected:

		static void invert(Data::FrontElement *fel, Data::Element *e);

		//static void markAsDeleted(Data::VertexSet &deleted, Data::Vertex *v, Data::Mesh *mesh);
		//static void markAsDeleted(Data::FrontElementSet &deleted, Data::FrontElement *e, Data::Mesh *mesh);
		//static void markAsDeleted(Data::ElementSet &deleted, Data::Element *e, Data::Mesh *mesh);

		virtual Data::Front *makeFront(Data::Element *e, Data::ElementSet &deletedElements,
			Data::FrontElementSet &deletedFrontElements, Data::VertexSet &deletedVertices) const = 0;

		virtual void insert(Data::ElementList &elements, Data::Element *e) const = 0;

		virtual Data::ElementList sort() const;

	protected:

		Data::Boundary *boundary;

		Data::Front *front;

		bool meshSorted;

		Performer::IdManager *idManager;
	};
}

#endif //#ifndef _PERFORMER_MESH_BACK_TRACKER_H_
