#ifndef _MJMESH_VERTEX_H_
#define _MJMESH_VERTEX_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/Front.h"
#include "Data/Vertex.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

#include "MJMesh/SpecificData.h"

#if USE_POOL
#include "Data/Poolable.h"
#endif //#if USE_POOL

namespace MJMesh
{
	class Vertex : public Data::Vertex
#if USE_POOL
		, public Data::Poolable<MJMesh::Vertex>
#endif //#if USE_POOL
	{
#if USE_POOL
		USING_POOLABLE(MJMesh::Vertex)
#endif //#if USE_POOL

	public:

		Vertex(Data::Point *p, ULInt id = 0);
		Vertex(Real x, Real y, ULInt id);
		Vertex(Real x, Real y, Real z, ULInt id);
		virtual ~Vertex();

		virtual void setAdjacentElements(const Data::ElementList &adjacentElements);
		virtual const Data::ElementList &getAdjacentElements() const;
		virtual void setAdjacentFrontElements(const Data::FrontElementList &adjacentFrontElements);
		virtual const Data::FrontElementList &getAdjacentFrontElements() const;

		/*virtual void add(Data::Edge *e);
		virtual void remove(Data::Edge *e);*/

		//using Data::Vertex::add;
		//using Data::Vertex::remove;

		virtual void add(Data::Element *e);
		virtual void remove(Data::Element *e);
		virtual bool have(Data::Element *e);
		virtual void clearAdjacentElements();
		virtual UInt numAdjacentElements() const;

		virtual Data::ElementList::iterator eBegin();
		virtual Data::ElementList::iterator eEnd();
		virtual Data::ElementList::const_iterator eBegin() const;
		virtual Data::ElementList::const_iterator eEnd() const;

		virtual Data::ElementList::iterator erase(Data::ElementList::iterator &iter);

		virtual void add(Data::FrontElement *e);
		virtual void remove(Data::FrontElement *e);
		virtual bool have(Data::FrontElement *e);
		virtual void clearAdjacentFrontElements();
		virtual UInt numAdjacentFrontElements() const;

		virtual Data::FrontElementList::iterator feBegin();
		virtual Data::FrontElementList::iterator feEnd();
		virtual Data::FrontElementList::const_iterator feBegin() const;
		virtual Data::FrontElementList::const_iterator feEnd() const;

		virtual Data::FrontElementList::iterator erase(Data::FrontElementList::iterator &iter);

		//methods related to specific data;
		virtual VertexSpecificData &getData() const;

		virtual void addBoundaryData(const MJMesh::Boundary *boundary, const Data::Mesh::VertexIterator &boundaryIt) const;
		virtual void removeBoundaryData(const MJMesh::Boundary *boundary) const;
		virtual bool isInBoundary(const MJMesh::Boundary *boundary) const;

		virtual const Data::Boundary::VertexIterator &getBoundaryData(const MJMesh::Boundary *boundary) const;
		virtual Data::Boundary::VertexIterator getAndRemoveBoundaryData(const MJMesh::Boundary *boundary) const;

		virtual void swapBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const;
		virtual void replaceBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const;

		virtual void addFrontData(const Front *front) const;
		virtual void addFrontData(const Front *front, const Data::Front::VertexIterator &frontIt) const;
		virtual void addFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;
		/*virtual void addFrontData(const Front *front, const Data::Front::VertexIterator &frontIt,
			const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;*/
		virtual void removeFrontData(const Front *front) const;
		virtual VertexSpecificData::FrontSpecificData getAndRemoveFrontData(const Front *front) const;

		virtual void setFrontData(const Front *front, const Data::Front::VertexIterator &frontIt) const;
		virtual void setFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;
		/*virtual void setFrontData(const Front *front, const Data::Front::VertexIterator &frontIt,
			const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;*/

		virtual bool isInFront(const Front *front) const;
		virtual bool isInAnyFront() const;
		virtual bool isInAnyFrontOtherThan(const Front *front) const;

		virtual VertexSpecificData::FrontSpecificData &getFrontData(const Front *front) const;
		virtual const Data::Front::VertexIterator &getFrontIterator(const Front *front) const;
		virtual VertexSpecificData::SearchTreeDataList &getFrontSTNodes(const Front *front) const;
		/*virtual const Data::GeometricSearchTreeNode *getFrontSTNode(const Front *front) const;
		virtual const Data::GeometricSearchTreeNode::ShapeSet::iterator &getSTNodeIterator(const Front *front) const;*/

		virtual void addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt) const;
#if USE_EXPERIMENTAL_MESH
		virtual void addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner) const;
#else
		virtual void addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const;
#endif //#if USE_EXPERIMENTAL_MESH
		virtual void addMeshData(const MJMesh::Mesh *mesh, const VertexSpecificData::MeshSpecificData &data) const;
		virtual void removeMeshData(const MJMesh::Mesh *mesh) const;
		virtual bool isInMesh(const MJMesh::Mesh *mesh) const;

#if USE_EXPERIMENTAL_MESH
		virtual void setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner) const;
#else
		virtual void setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const;
#endif //#if USE_EXPERIMENTAL_MESH
		virtual void setMeshIterator(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt) const;
#if USE_EXPERIMENTAL_MESH
		virtual void setMeshInner(const MJMesh::Mesh *mesh, bool inner) const;
#else
		virtual void setMeshInner(const MJMesh::Mesh *mesh, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const;
#endif //#if USE_EXPERIMENTAL_MESH

		virtual const Data::Mesh::VertexIterator &getMeshIterator(const MJMesh::Mesh *mesh) const;
		virtual bool isMeshInner(const MJMesh::Mesh *mesh) const;
#if (!USE_EXPERIMENTAL_MESH)
		virtual const Data::Mesh::VertexIterator &getMeshInnerIterator(const MJMesh::Mesh *mesh) const;
#endif //#if (!USE_EXPERIMENTAL_MESH)

		virtual VertexSpecificData::MeshSpecificData getAndRemoveMeshData(const MJMesh::Mesh *mesh) const;

		virtual void swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;
		virtual void replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;

#if USE_GUI
		virtual void highlight() const;
		virtual void unhighlight() const;
#endif //#if USE_GUI

		using Data::Vertex::text;
		virtual std::string text(const Data::AbstractMesh *mesh) const;

	protected:

		mutable VertexSpecificData data;

		Data::ElementList adjacentElements;
		Data::FrontElementList adjacentFrontElements;
	};
}

#endif //#ifndef _MJMESH_VERTEX_H_
