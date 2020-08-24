#ifndef _TEMPLATE_MJMESH_TEMPLATE_MESH_GENERATOR_H_
#define _TEMPLATE_MJMESH_TEMPLATE_MESH_GENERATOR_H_

#include "TemplateMJMesh/Definitions.h"
#include "Performer/TemplateMeshGenerator.h"

namespace TemplateMJMesh
{
	class TemplateMeshGenerator : public Performer::TemplateMeshGenerator
	{
	public:

		TemplateMeshGenerator();
		virtual ~TemplateMeshGenerator();

		virtual void setBoundary(MJMesh::Boundary *boundary);

		virtual void setFront(MJMesh::Front *front);
		virtual MJMesh::Front *getFront() const;

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void execute();

	protected:

		using Performer::TemplateMeshGenerator::execute;

		virtual bool shouldApplyTemplate(const Data::BoxTreeCell *cell);
		virtual void applyTemplate(const Data::BoxTreeCell *cell, VertexTree *root,
			Data::ElementList &newElements, Data::FrontElementList &newFrontElements, Data::VertexList &newVertices);

		virtual Data::Vertex *makeVertex(const Data::Point2D &p, ULInt id) const;
		virtual Data::Vertex *makeVertex(const Data::Point3D &p, ULInt id) const;

		virtual Data::Mesh *makeMesh() const;
		virtual MJMesh::Front *makeFront() const;

	protected:

		MJMesh::Front *front;
		MJMesh::Boundary *boundary;

	};
}

#endif //#ifndef _TEMPLATE_MJMESH_TEMPLATE_MESH_GENERATOR_H_
