#ifndef _MJMESH_FRONT_ELEMENT_H_
#define _MJMESH_FRONT_ELEMENT_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/Front.h"
#include "Data/FrontElement.h"
#include "MJMesh/SpecificData.h"

namespace MJMesh
{
	class FrontElement : public Data::FrontElement
	{
	public:

		FrontElement(Data::GeometricShape *shape, ULInt id = 0);
		virtual ~FrontElement();

		virtual MJMesh::Vertex *getV1() const;
		virtual MJMesh::Vertex *getV2() const;

		virtual const Data::ElementList &getAdjacency() const;

		virtual void add(MJMesh::Element *e) const;
		virtual void remove(MJMesh::Element *e) const;
		virtual bool haveAdjacent(const MJMesh::Element *e) const;
		virtual void clearAdjacency() const;
		virtual UInt numAdjacents() const;

		virtual Data::ElementList::iterator eBegin();
		virtual Data::ElementList::iterator eEnd();
		virtual Data::ElementList::const_iterator eBegin() const;
		virtual Data::ElementList::const_iterator eEnd() const;

		virtual Data::ElementList::iterator erase(Data::ElementList::iterator &iter);

		virtual Real angle(const Data::Point &p) const;
		virtual Real angle(const Data::Point *p) const = 0;

		virtual Real distance(const Data::Point *p) const = 0;
		virtual Real straightDistance(const Data::Point *p) const = 0;

		virtual Real rank(const Data::Point *p, const Data::PointList &ideal) const;
		virtual Real rank(const Data::Point *p) const;

		virtual void invert();

		//methods related to specific data;
		virtual FrontElementSpecificData &getData() const;

		virtual void addBoundaryData(const MJMesh::Boundary *boundary, const Data::Boundary::FrontElementIterator &boundaryIt) const;
		virtual void removeBoundaryData(const MJMesh::Boundary *boundary) const;
		virtual bool isInBoundary(const MJMesh::Boundary *boundary) const;

		virtual const Data::Boundary::FrontElementIterator &getBoundaryData(const MJMesh::Boundary *boundary) const;
		virtual Data::Boundary::FrontElementIterator getAndRemoveBoundaryData(const MJMesh::Boundary *boundary) const;

		virtual void swapBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const;
		virtual void replaceBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const;

		virtual void addFrontData(const Front *front) const;
		virtual void addFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt) const;
		virtual void addFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;
		/*virtual void addFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt,
			const Data::GeometricSearchTreeNode *stnode, const GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;*/
		virtual void removeFrontData(const Front *front) const;
		virtual FrontElementSpecificData::FrontSpecificData getAndRemoveFrontData(const Front *front) const;

		virtual void setFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt) const;
		virtual void setFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;
		/*virtual void setFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt,
			const Data::GeometricSearchTreeNode *stnode, const GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const;*/

		virtual bool isInFront(const Front *front) const;
		virtual bool isInAnyFront() const;
		virtual bool isInAnyFrontOtherThan(const Front *front) const;

		virtual FrontElementSpecificData::FrontSpecificData &getFrontData(const Front *front) const;
		virtual UShort getFrontState(const Front *front) const;
		virtual const Data::Front::FrontElementIterator &getFrontIterator(const Front *front) const;
		virtual FrontElementSpecificData::SearchTreeDataList &getFrontSTNodes(const Front *front) const;
		/*virtual const Data::GeometricSearchTreeNode *getFrontSTNode(const Front *front) const;
		virtual const GeometricSearchTreeNode::ShapeSet::iterator &getSTNodeIterator(const Front *front) const;*/

		virtual void addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::FrontElementIterator &meshIt) const;
		virtual void removeMeshData(const MJMesh::Mesh *mesh) const;
		virtual bool isInMesh(const MJMesh::Mesh *mesh) const;

		virtual void setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::FrontElementIterator &meshIt) const;
		virtual const Data::Mesh::FrontElementIterator &getMeshIterator(const MJMesh::Mesh *mesh) const;

		virtual Data::Mesh::FrontElementIterator getAndRemoveMeshIterator(const MJMesh::Mesh *mesh) const;

		virtual void swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;
		virtual void replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const;

#if USE_ARRAY
		using Data::FrontElement::operator=;
		virtual MJMesh::FrontElement &operator=(const MJMesh::FrontElement &e);
#endif //#if USE_ARRAY

		using Data::FrontElement::text;
		virtual std::string text(const Data::AbstractMesh *mesh) const;

	protected:

		mutable FrontElementSpecificData data;

		mutable Data::ElementList adjacency;
	};
}

#endif //#ifndef _MJMESH_FRONT_ELEMENT_H_
