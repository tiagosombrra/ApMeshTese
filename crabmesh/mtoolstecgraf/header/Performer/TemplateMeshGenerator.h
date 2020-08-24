#ifndef _PERFORMER_TEMPLATE_MESH_GENERATOR_H_
#define _PERFORMER_TEMPLATE_MESH_GENERATOR_H_

#include "Performer/Definitions.h"
#include "Performer/MeshGenerator.h"
#include "Data/TreeNode.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

namespace Performer
{
	class TemplateMeshGenerator : public Performer::MeshGenerator
	{
	public:

		TemplateMeshGenerator();
		virtual ~TemplateMeshGenerator();

		virtual void setTree(const Data::BoxTree *tree);

		virtual void execute();

	public:

		enum
		{
			NOT_INIT = 0,
			INIT,
			SUCCESS,
			ERROR = 100
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status success;
		static const Performer::Status error;

	protected:

		struct VertexTree : public Data::TreeNode
		{
			VertexTree(Data::BoxTreeCell *c, VertexTree *parent);
			virtual ~VertexTree();

			virtual UInt numAdjacents() const;
			virtual void setAdjacent(UInt i, Data::GraphNode *n);
			virtual Data::GraphNode *getAdjacent(UInt i) const;
			virtual void add(Data::GraphNode *n);
			virtual void remove(Data::GraphNode *n);
			virtual Data::GraphNodeVector getAdjacents() const;

			using Data::TreeNode::operator=;

			const Data::BoxTreeCell *cell;
			Data::Vertex *vertices[51];
			Data::GraphNode *adjs[9];
		};

	protected:

		virtual void execute(const Data::BoxTreeCell *cell, VertexTree *root);

		virtual bool shouldApplyTemplateInAnyDescendant(const Data::BoxTreeCell *cell);
		virtual bool shouldApplyTemplate(const Data::BoxTreeCell *cell) = 0;
		virtual void applyTemplate(const Data::BoxTreeCell *cell, VertexTree *root,
			Data::ElementList &newElements, Data::FrontElementList &newFrontElements, Data::VertexList &newVertices) = 0;

	protected:

		/*
			BoxTree must have been 2:1 refined.

			index in 2D:

			3---6---2
			|       |
			7   8   5
			|       |
			0---4---1

			4 - in edge 0 1
			5 - in edge 1 2
			6 - in edge 2 3
			7 - in edge 3 0
			8 - center
			otherwise - center


			index in 3D:

			      3--10---2
			     /|      /|
			    /11     / 9
			  19  |   18  |
			  /   0---8---1
			 /   /   /   /
			7--14---6   /
			| 16    | 17
		   15 /    13 /
			|/      |/
			4--12---5

			 8 - in edge 0 1
			 9 - in edge 1 2
			10 - in edge 2 3
			11 - in edge 3 0
			12 - in edge 4 5
			13 - in edge 5 6
			14 - in edge 6 7
			15 - in edge 7 8
			16 - in edge 0 4
			17 - in edge 1 5
			18 - in edge 2 6
			19 - in edge 3 7
			20 - in face 1 2 5 6 (+X)
			21 - in face 2 3 7 6 (+Y)
			22 - in face 4 5 6 7 (+Z)
			23 - in face 0 4 7 3 (-X)
			24 - in face 0 1 5 4 (-Y)
			25 - in face 0 3 2 1 (-Z)

			26 - in edge  0  8
			27 - in edge  8  1
			28 - in edge  1  9
			29 - in edge  9  2
			30 - in edge  2 10
			31 - in edge 10  3
			32 - in edge  3 11
			33 - in edge 11  0
			34 - in edge  4 12
			35 - in edge 12  5
			36 - in edge  5 13
			37 - in edge 13  6
			38 - in edge  6 14
			39 - in edge 14  7
			40 - in edge  7 15
			41 - in edge 15  4
			42 - in edge  0 16
			43 - in edge 16  4
			44 - in edge  1 17
			45 - in edge 17  5
			46 - in edge  2 18
			47 - in edge 18  6
			48 - in edge  3 19
			49 - in edge 19  7
			50 - center
			otherwise - center

		*/

		virtual void makeTree();
		//virtual void makeTree(const Data::BoxTreeCell *cell, VertexTree *root) const;

		virtual void makeVertices(VertexTree *root);

		virtual void deleteExtraVertices();

		virtual Data::UIntList path(const Data::BoxTreeCell *cell) const;

		virtual VertexTree *mate(Data::UIntList path) const;
		virtual VertexTree *mate(const Data::BoxTreeCell *cell) const;

		virtual Data::Vertex *getVertex(const Data::BoxTreeCell *cell, UInt index);
		virtual Data::Vertex *getVertex(const VertexTree *root, UInt index);

		/*virtual Data::Vertex *makeVertex(const Data::BoxTreeCell *cell, UInt index);
		virtual Data::Vertex *makeVertex(VertexTree *root, UInt index);

		virtual Data::Vertex *getOrMakeVertex(const Data::BoxTreeCell *cell, UInt index);
		virtual Data::Vertex *getOrMakeVertex(VertexTree *root, UInt index);

		virtual void setVertex(VertexTree *root, UInt index, Data::Vertex *v);

		virtual void setAdjVertex2D(const Data::BoxTreeCell *cell, UInt side, UInt index1, UInt pos, UInt index2, UInt setPos, Data::Vertex *v) const;
		virtual void setAdjVertex2D(const Data::BoxTreeCell *cell, UInt side1, UInt side2, UInt index, Data::Vertex *v) const;
		virtual void setAdjsVertex2D(const Data::BoxTreeCell *cell, UInt side, UInt pos1, UInt index1, UInt pos2, UInt index2, Data::Vertex *v) const;

		virtual void setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side, UInt index1,
				UInt pos2, UInt index2, UInt pos3, UInt index3, UInt pos4, UInt index4, UInt setPos, Data::Vertex *v) const;
		virtual void setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side1, UInt side2, UInt index1,
				UInt pos2, UInt index2, UInt index3, UInt index4, UInt setPos, Data::Vertex *v) const;
		virtual void setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side1, UInt side2, UInt side3, UInt index, Data::Vertex *v) const;

		virtual Data::Point2D point2D(const Data::BoxTreeCell *cell, UInt index) const;
		virtual Data::Point3D point3D(const Data::BoxTreeCell *cell, UInt index) const;*/

		virtual Data::Vertex *makeVertex(const Data::Point2D &p, ULInt id) const = 0;
		virtual Data::Vertex *makeVertex(const Data::Point3D &p, ULInt id) const = 0;

#if USE_EXPERIMENTAL_MESH
		virtual Data::Mesh *makeMesh() const = 0;
#else
		virtual Data::Mesh *makeMesh() const;
#endif //#if USE_EXPERIMENTAL_MESH

	protected:

		const Data::BoxTree *tree;

		VertexTree *root;
		Data::VertexList vertices;
	};
}

#endif //#ifndef _PERFORMER_TEMPLATE_MESH_GENERATOR_H_
