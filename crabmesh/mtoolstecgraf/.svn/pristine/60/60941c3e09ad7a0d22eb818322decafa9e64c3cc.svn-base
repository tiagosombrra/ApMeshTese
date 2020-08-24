#ifndef _MJMESH_FRONT_ADVANCER_H_
#define _MJMESH_FRONT_ADVANCER_H_

#include "MJMesh/Definitions.h"
#include "Performer/FrontAdvancer.h"

namespace MJMesh
{
	class FrontAdvancer : public Performer::FrontAdvancer
	{
	public:

		FrontAdvancer(UInt dimension, const Data::Boundary *boundary,
			const Data::BoxTree *tree, bool usingSearchTrees);
		virtual ~FrontAdvancer();

		virtual void setFront(MJMesh::Front *front);
		virtual Data::Front *getFront() const;

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void setTree(const Data::BoxTree *tree);

		virtual void setUsingSearchTrees(bool usingSearchTrees);

		virtual void setVSTree(Data::GeometricSearchTree *vstree);
		virtual void setFESTree(Data::GeometricSearchTree *festree);

		virtual Data::GeometricSearchTree *makeVertexSearchTree(const MJMesh::Front *front = NULL) const;

		virtual Data::GeometricSearchTree *makeFrontElementSearchTree(const MJMesh::Front *front = NULL) const;

	protected:

		virtual Data::Mesh *makeMesh() const;
		virtual Data::Front *makeFront() const;
		virtual Data::Vertex *makeVertex(Data::Point *p) const;

		virtual MJMesh::VertexSearchTreeGenerator *makeVertexSearchTreeGenerator(const MJMesh::Front *front = NULL) const;

		virtual MJMesh::FrontElementSearchTreeGenerator *makeFrontElementSearchTreeGenerator(const MJMesh::Front *front = NULL) const;

#if USE_ADT
        using Performer::FrontAdvancer::advance;
        virtual void advance(Data::FrontElement *e, Data::Vertex *v);
#endif //#if USE_ADT

		virtual Data::ElementTuple makeElement(const Data::FrontElement *e, const Data::Point *p) const;

		/*virtual Data::FrontElementList makeFrontElements(
			const Data::FrontElement *element, Data::Point *p,
			Data::Vertex *&v) const;

		virtual Data::Element *makeElement(const Data::FrontElement *element,
			const Data::FrontElementList &elements, const Data::Vertex *v) const = 0;*/

		virtual bool better(Real rank1, Real rank2) const;

		virtual bool equal(Real rank1, Real rank2) const;

		virtual bool accordingToNormal(const Data::FrontElement *element, const Data::FrontElement *adj) const;
		virtual bool better(const Data::FrontElement *element, Data::Vertex *candidate, Data::Vertex *old) const;

		using Performer::FrontAdvancer::isValid;

		virtual bool isValid(const Data::FrontElement *element, Data::Point *point, Real radius) const;

		/*virtual bool isValid(const Data::FrontElement *element, Data::Point *point) const;*/

		virtual bool isValid(const Data::Point *point, const Data::FrontElement *e) const;
		virtual bool isValid(const Data::Point *point, const Data::FrontElement *e, const Data::FrontElementList &list) const;

		virtual const Data::BoxTreeCell *cell(const Data::Point &p) const;

		virtual Data::PointList ideal(const Data::FrontElement *element, Real factor = 1.0) const;

		virtual Real radius(const Data::FrontElement *element) const;

	protected:

		const Data::BoxTree *tree;

		UInt dimension;

		bool usingSearchTrees;

		mutable Data::GeometricSearchTree *vstree;
		mutable Data::GeometricSearchTree *festree;
	};
}

#endif //#ifndef _MJMESH_FRONT_ADVANCER_H_
