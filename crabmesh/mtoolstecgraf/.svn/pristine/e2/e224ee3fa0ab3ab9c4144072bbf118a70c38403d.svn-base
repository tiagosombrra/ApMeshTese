#ifndef _PERFORMER_MESH_SPLITTER_H_
#define _PERFORMER_MESH_SPLITTER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Mesh.h"

namespace Performer
{
	class MeshSplitter
	{
	public:

		MeshSplitter();
		virtual ~MeshSplitter();

		virtual void setShouldMakeBoundaries(bool shouldMakeBoundaries);

		virtual void setDuplicate(bool duplicate);
		virtual bool isDuplicate() const;

		virtual void setMesh(const Data::Mesh *mesh);

		virtual const Data::MeshList &getMeshes() const;
		virtual const Data::BoundaryList &getBoundaries() const;

		virtual void execute();

	protected:

		typedef Data::VertexSet VSet;
		typedef Data::FrontElementSet FESet;

#if USE_C__11
		//typedef Data::VertexHash VSet;
		//typedef Data::FrontElementHash FESet;

		typedef Data::VertexHashMap VMap;
		typedef Data::FrontElementHashMap FEMap;
		typedef Data::ElementHashMap EMap;
#else
		//typedef Data::VertexSet VSet;
		//typedef Data::FrontElementSet FESet;

		typedef Data::VertexMap VMap;
		typedef Data::FrontElementMap FEMap;
		typedef Data::ElementMap EMap;
#endif //#if USE_C__11

	protected:

		virtual Data::Mesh *makeMesh() const = 0;
		virtual Data::Boundary *makeBoundary() const = 0;
		virtual Data::Vertex *makeVertex(const Data::Vertex *vertex,
			const VMap &vMapSoFar) const = 0;
		virtual Data::FrontElement *makeFrontElement(const Data::FrontElement *frontElement,
			const VMap &vMap, const FEMap &felMapSoFar) const = 0;
		virtual Data::Element *makeElement(const Data::Element *element,
			const VMap &vMap, const FEMap &felMap, const EMap &elMapSoFar) const = 0;

		virtual ULInt numVerticesInMeshes() const;

		virtual bool inAnyMesh(const Data::Vertex *v) const;
		virtual bool inAnyMesh(const Data::FrontElement *fe) const;
		virtual bool inAnyMesh(const Data::Element *e) const;

		virtual Data::Vertex *select() const;
		virtual void addToVertices(Data::Vertex *v, Data::Mesh *mesh, Data::VertexList &vertices);
		virtual void split(Data::Vertex *v, Data::Mesh *mesh);

		virtual void copy();
		virtual void copy(Data::Mesh *mesh);

	protected:

		bool shouldMakeBoundaries;
		bool duplicate;

		const Data::Mesh *mesh;

		Data::MeshList meshes;

		Data::BoundaryList boundaries;

#if USE_GUI
		Real r, g, b;
#endif //#if USE_GUI

		mutable Data::Mesh::ConstVertexIterator selected;
	};
}

#endif //#ifndef _PERFORMER_MESH_SPLITTER_H_
