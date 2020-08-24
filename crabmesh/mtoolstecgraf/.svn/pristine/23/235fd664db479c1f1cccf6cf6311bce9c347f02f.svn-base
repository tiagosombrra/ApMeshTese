#ifndef _MJMESH_ELEMENT_H_
#define _MJMESH_ELEMENT_H_

#include "MJMesh/Definitions.h"
#include "Data/Element.h"
#include "MJMesh/SpecificData.h"

namespace MJMesh
{
	class Element : public Data::Element
	{
	public:

		Element(Data::GeometricShape *shape, ULInt id = 0);
		virtual ~Element();

		virtual void remove(MJMesh::Vertex *v);

		virtual Real gamma() const = 0;

		static bool gammaGreater(Data::Element *e1, Data::Element *e2);

		virtual ElementSpecificData &getData() const;

		virtual void addMeshData(const MJMesh::Mesh *mesh, Data::Mesh::ElementIterator &meshIt) const;
		virtual void removeMeshData(const MJMesh::Mesh *mesh) const;
		virtual bool isInMesh(const MJMesh::Mesh *mesh) const;

		virtual void setMeshData(const MJMesh::Mesh *mesh, Data::Mesh::ElementIterator &meshIt) const;
		virtual const Data::Mesh::ElementIterator &getMeshIterator(const MJMesh::Mesh *mesh) const;

		virtual Data::Mesh::ElementIterator getAndRemoveMeshIterator(const MJMesh::Mesh *mesh) const;

		virtual void swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;
		virtual void replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;

#if USE_ARRAY
		using Data::Element::operator=;
#endif //#if USE_ARRAY

//debug
		using Data::Element::text;
		virtual std::string text(const Data::AbstractMesh *mesh) const;
//endebug

	protected:

		mutable ElementSpecificData data;
	};
}

#endif //#ifndef _MJMESH_ELEMENT_H_
