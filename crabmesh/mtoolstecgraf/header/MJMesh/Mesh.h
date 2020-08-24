#ifndef _MJMESH_MESH_H_
#define _MJMESH_MESH_H_

#include "MJMesh/Definitions.h"
#include "Data/Mesh.h"

namespace MJMesh
{
	class Mesh : public Data::Mesh
	{
	public:

		Mesh(ULInt id = 0);
		virtual ~Mesh();

		virtual void setVertices(const Data::VertexList &vertices);

		virtual void setFrontElements(const Data::FrontElementList &frontElements);

		virtual void setElements(const Data::ElementList &elements);

#if (!USE_EXPERIMENTAL_MESH)
		virtual void setInnerVertices(const Data::VertexList &innerVertices);
#endif //#if (!USE_EXPERIMENTAL_MESH)

		//involves dynamic_cast, avoid whenever possible (or store the return
        //  value for future references
		virtual bool isSurface() const;

		virtual void clear();

		virtual void add(Data::Vertex *v, bool inner = true);
		virtual void addInner(Data::Vertex *v);
		virtual void add(Data::FrontElement *e);
		virtual void add(Data::Element *e);

		virtual bool remove(Data::Vertex *v);
		virtual void removeInner(Data::Vertex *v);

		virtual bool have(const Data::Vertex *v) const;
		virtual bool haveInner(const Data::Vertex *v) const;
		virtual bool have(const Data::FrontElement *e) const;
		virtual bool have(const Data::Element *e) const;

		virtual Data::Mesh::VertexIterator erase(const Data::Mesh::VertexIterator &iter);
#if USE_EXPERIMENTAL_MESH
		virtual Data::Mesh::InnerVertexIterator eraseInner(const Data::Mesh::InnerVertexIterator &iter);
		virtual Data::Mesh::InnerVertexIterator eraseFromInner(const Data::Mesh::InnerVertexIterator &iter);
#else
		virtual Data::Mesh::VertexIterator eraseInner(const Data::Mesh::VertexIterator &iter);
		virtual Data::Mesh::VertexIterator eraseFromInner(const Data::Mesh::VertexIterator &iter);
#endif //#if USE_EXPERIMENTAL_MESH
		virtual Data::Mesh::FrontElementIterator erase(const Data::Mesh::FrontElementIterator &iter);
		virtual Data::Mesh::ElementIterator erase(const Data::Mesh::ElementIterator &iter);

		virtual void swap(Mesh *mesh);

		virtual Data::ElementList adjacency(const Data::Vertex *v) const;
		//virtual Data::ElementList adjacency(const MJMesh::Vertex *v) const;

		virtual Data::ElementList adjacency(const Data::FrontElement *e) const;
		//virtual Data::ElementList adjacency(const MJMesh::FrontElement *e) const;

		virtual Data::FrontElementList findAdjacentFrontElements(const Data::Vertex *v) const;
		virtual Data::FrontElementList findAdjacentFrontElements(const Data::Element *e) const;

#if USE_DELETE_FELS
        virtual UInt createAdjacentFrontElements(Data::Vertex *v,
            Data::FrontElementList &created, bool add, const Data::Boundary *boundary);
#endif //#if USE_DELETE_FELS

		using Data::Mesh::remove;

		virtual bool remove(Data::FrontElement *e);
		virtual bool remove(Data::FrontElement *e, bool removeFromAdjacentVertices);

		virtual bool remove(Data::Element *e);
		virtual bool remove(Data::Element *e, bool removeFromAdjacentVertices);

		//finds all layers of vertices, front elements and elements adjacent
		//  to input list of vertices and moves them to the given mesh. shared
		//  vertices/front elements will not be in given mesh but, if desired,
		//  can be retrieved in given input lists. this method does not change
		//  the data structures pointers of the vertices/ front elements/
		//  elements. the input boolean noIntersection means that this mesh and
		//  the input mesh have no intersection, i.e., there is no vertex,
		//  front element or element shared by both meshes
		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh/*,
			bool noIntersection = false*/);
		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
			MJMesh::Boundary &sharedBoundary/*, bool noIntersection = false*/,
			const Data::FrontList &except = Data::FrontList());

		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
			const MJMesh::Boundary *boundary/*, bool noIntersection = false*/);
		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary/*,
			bool noIntersection = false*/, const Data::FrontList &except = Data::FrontList());

		virtual void splitTo(const Data::VertexList &vertices, UInt layers,
            const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
		virtual void splitTo(const Data::VertexList &vertices, UInt layers,
			MJMesh::Boundary &sharedBoundary, const Data::Box *box = NULL, const MJMesh::Front *front = NULL);

#if USE_ID_HASH_IN_MESH_SPLIT
		virtual void splitToIteration(VertexIdHash &current, VertexIdHash &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(VertexIdHash &current, VertexIdHash &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
#elif USE_C__11
		virtual void splitToIteration(Data::VertexHash &current, Data::VertexHash &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(Data::VertexHash &current, Data::VertexHash &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
#else
		virtual void splitToIteration(Data::VertexSet &current, Data::VertexSet &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(Data::VertexSet &current, Data::VertexSet &next, const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		///if (boundary != NULL), then vertices present in sharedBoundary that
		///  are also in this are marked as inner. this must be changed by the
		///  caller of the Mesh::splitTo() method.
		virtual void splitTo(const Data::VertexList &vertices, UInt layers,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
		virtual void splitTo(const Data::VertexList &vertices, UInt layers,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary1, const MJMesh::Boundary *boundary2,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
		//virtual void splitTo(const Data::VertexList &vertices, UInt layers,
		//	MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries, const Data::Box *box = NULL);

		virtual void merge(MJMesh::Mesh *mesh, const Data::Boundary *boundary);
		virtual void merge(MJMesh::Mesh *mesh,
            const Data::FrontList &except = Data::FrontList(),
            const Data::Boundary *boundary = NULL);

		virtual Data::ElementList sort() const;

#if USE_GUI
		using Data::Mesh::draw;
		virtual void draw() const;

		//debug
		//virtual bool test(const MJMesh::Boundary *boundary = NULL, const MJMesh::Front *front = NULL) const;
		//virtual void oldTest(const MJMesh::Boundary *boundary = NULL, const MJMesh::Front *front = NULL) const;
		virtual void print() const;
		//endebug
#endif //#if USE_GUI

	protected:

		virtual bool frontHave(const Data::FrontList &except, const Data::Vertex *v) const;
		virtual bool frontHave(const Data::FrontList &except, const Data::FrontElement *fel) const;

		virtual bool boundaryHave(const Data::BoundaryList &boundaries, const Data::Vertex *v) const;
		virtual bool boundaryHave(const Data::BoundaryList &boundaries, const Data::FrontElement *fel) const;

		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
			bool outputShared, MJMesh::Boundary &sharedBoundary/*,
			bool noIntersection = false*/, const Data::FrontList &except = Data::FrontList());
		virtual void split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
			bool outputShared, MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary/*,
			bool noIntersection = false*/, const Data::FrontList &except = Data::FrontList());

		virtual void splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
        virtual void splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary1, const MJMesh::Boundary *boundary2,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
        virtual void splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
#if USE_ID_HASH_IN_MESH_SPLIT
		virtual void splitToBeginning(VertexIdHash &current, /*VertexIdHash &next,*/
			const Data::VertexList &vertices/*, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary*/);
		virtual void splitToIteration(VertexIdHash &current, VertexIdHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToIteration(VertexIdHash &current, VertexIdHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(VertexIdHash &current, VertexIdHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
		virtual void splitToEnding(VertexIdHash &current, VertexIdHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
#elif USE_C__11
		virtual void splitToBeginning(Data::VertexHash &current, /*Data::VertexHash &next,*/
			const Data::VertexList &vertices/*, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary*/);
		virtual void splitToIteration(Data::VertexHash &current, Data::VertexHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToIteration(Data::VertexHash &current, Data::VertexHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(Data::VertexHash &current, Data::VertexHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL);
		virtual void splitToEnding(Data::VertexHash &current, Data::VertexHash &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front);
#else
		virtual void splitToBeginning(Data::VertexSet &current, /*Data::VertexSet &next,*/
			const Data::VertexList &vertices/*, bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary*/);
		virtual void splitToIteration(Data::VertexSet &current, Data::VertexSet &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToIteration(Data::VertexSet &current, Data::VertexSet &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList *boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front = NULL, bool swap = true);
		virtual void splitToEnding(Data::VertexSet &current, Data::VertexSet &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
			const Data::Box *box = NULL, const MJMesh::Front *front);
		virtual void splitToEnding(Data::VertexSet &current, Data::VertexSet &next,
			/*const Data::VertexList &vertices, */bool outputShared,
			MJMesh::Boundary &sharedBoundary, const Data::BoundaryList *boundaries,
			const Data::Box *box = NULL, const MJMesh::Front *front);
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

	protected:

		//these methods do not check if given input belong to the mesh
		virtual void forceRemove(Data::Vertex *v);
		virtual void forceRemoveInner(Data::Vertex *v);
		virtual void forceRemove(Data::FrontElement *e);
		virtual void forceRemove(Data::Element *e);

		///just clears the data structures, neither not delete or removes the objects
		virtual void clearVertices() const;
		virtual void clearFrontElements() const;
		virtual void clearElements() const;

	};
}

#endif //#ifndef _MJMESH_MESH_H_
