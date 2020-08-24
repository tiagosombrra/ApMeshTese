#ifndef _PERFORMER_MESH_MERGER_H_
#define _PERFORMER_MESH_MERGER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class MeshMerger : public Performer::Builder
	{
	public:

		MeshMerger();
		virtual ~MeshMerger();

		virtual void setShouldMakeBoundary(bool shouldMakeBoundary);
		virtual void setKeepOldIds(bool keepOldIds);

		virtual void setMeshes(const Data::MeshList &meshes);

		virtual void setIdManager(IdManager *idManager);

		virtual Data::Boundary *getBoundary() const;

		virtual Data::Mesh *getMesh() const;

		virtual void execute();

	protected:

#if USE_C__11
		typedef Data::VertexHashMap VMap;
		typedef Data::FrontElementHashMap FEMap;
		typedef Data::ElementHashMap EMap;

		typedef std::unordered_map<Data::Vertex *, ULInt> VertexIdMap;
		typedef std::unordered_map<Data::FrontElement *, ULInt> FrontElementIdMap;
		typedef std::unordered_map<Data::Element *, ULInt> ElementIdMap;
#else
		typedef Data::VertexMap VMap;
		typedef Data::FrontElementMap FEMap;
		typedef Data::ElementMap EMap;

		typedef std::map<Data::Vertex *, ULInt> VertexIdMap;
		typedef std::map<Data::FrontElement *, ULInt> FrontElementIdMap;
		typedef std::map<Data::Element *, ULInt> ElementIdMap;
#endif //#if USE_C__11

	protected:

		virtual void merge(const Data::Mesh *mesh);

		virtual Data::Mesh *makeMesh() const = 0;
		virtual Data::Boundary *makeBoundary() const = 0;
		virtual Data::Vertex *makeVertex(const Data::Vertex *vertex,
			const VMap &vMapSoFar) const = 0;
		virtual Data::FrontElement *makeFrontElement(const Data::FrontElement *frontElement,
			const VMap &vMap, const FEMap &felMapSoFar) const = 0;
		virtual Data::Element *makeElement(const Data::Element *element,
			const VMap &vMap, const FEMap &felMap, const EMap &elMapSoFar) const = 0;

	protected:

		bool shouldMakeBoundary;
		bool keepOldIds;

		Data::MeshList meshes;

		Data::Boundary *boundary;

		Data::Mesh *mesh;

		IdManager *idManager;

#if USE_GUI
		Real r, g, b;
#endif //#if USE_GUI

	};
}

#endif //#ifndef _PERFORMER_MESH_MERGER_H_
