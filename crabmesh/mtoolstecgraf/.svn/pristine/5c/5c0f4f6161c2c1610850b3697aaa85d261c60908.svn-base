#ifndef _DATA_BOX_SEARCH_TREE_CELL_H_
#define _DATA_BOX_SEARCH_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/BoxSearchTree.h"
#include "Data/GeometricSearchTreeNode.h"

namespace Data
{
	class BoxSearchTreeCell : public Data::GeometricSearchTreeNode
	{
	public:

		BoxSearchTreeCell(BoxTreeCell *cell = NULL, BoxSearchTreeCell *parent = NULL, ULInt id = 0);
		virtual ~BoxSearchTreeCell();

		virtual void setCell(BoxTreeCell *cell);
		virtual BoxTreeCell *getCell() const;

		virtual void makeCell();

		using GeometricSearchTreeNode::add;
		using GeometricSearchTreeNode::remove;

		virtual bool add(const Identifiable *s);
		virtual bool remove(const Identifiable *s);

		virtual GeometricSearchTreeNode::ShapeSet inRange(const Box *box) const;

#if USE_ARRAY
		using GeometricSearchTreeNode::operator=;
		virtual BoxSearchTreeCell &operator=(const BoxSearchTreeCell &c);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		virtual void inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const;

		/*virtual GraphNodeSet cellsInRange(const Box *box) const;
		virtual void cellsInRange(const Box *box, GraphNodeSet &cells) const;*/

		virtual bool inRange(const Identifiable *s) const;
		virtual bool inRange(const Box *box, const Identifiable *s) const = 0;

	protected:

		BoxTreeCell *cell;
	};
}

#if USE_EXPERIMENTAL_5
namespace Data
{
    class BoxSearchTreeCell2D : public Data::BoxSearchTreeCell
    {
    public:

        BoxSearchTreeCell2D(BoxTreeCell *cell = NULL, BoxSearchTreeCell2D *parent = NULL, ULInt id = 0);
		virtual ~BoxSearchTreeCell2D();

        virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;

		using Data::BoxSearchTreeCell::add;
		using Data::BoxSearchTreeCell::remove;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);

#if USE_ARRAY
		using Data::BoxSearchTreeCell::operator=;
		virtual BoxSearchTreeCell2D &operator=(const BoxSearchTreeCell2D &c);
#endif //#if USE_ARRAY

    protected:

        GraphNode *adjacents[5];
    };

    class BoxSearchTreeCell3D : public Data::BoxSearchTreeCell
    {
    public:

        BoxSearchTreeCell3D(BoxTreeCell *cell = NULL, BoxSearchTreeCell3D *parent = NULL, ULInt id = 0);
		virtual ~BoxSearchTreeCell3D();

        virtual UInt numAdjacents() const;

		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;

		virtual GraphNodeVector getAdjacents() const;

		using Data::BoxSearchTreeCell::add;
		using Data::BoxSearchTreeCell::remove;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);

#if USE_ARRAY
		using Data::BoxSearchTreeCell::operator=;
		virtual BoxSearchTreeCell3D &operator=(const BoxSearchTreeCell3D &c);
#endif //#if USE_ARRAY

    protected:

        GraphNode *adjacents[9];
    };
}
#endif //#if USE_EXPERIMENTAL_5

#endif //#ifndef _DATA_BOX_SEARCH_TREE_CELL_H_
