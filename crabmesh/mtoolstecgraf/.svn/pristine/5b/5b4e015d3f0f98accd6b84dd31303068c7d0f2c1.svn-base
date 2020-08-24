#ifndef _DATA_RECURSIVE_DECOMPOSITION_TREE_H_
#define _DATA_RECURSIVE_DECOMPOSITION_TREE_H_

#include "Data/Definitions.h"
#include "Data/Tree.h"
#include "Data/ListDisplayable.h"

namespace Data
{
	class RecursiveDecompositionTree : public Data::Tree
#if USE_GUI
		, public Data::ListDisplayable
#endif //#if USE_GUI
	{
	public:

		RecursiveDecompositionTree(GeometricShape *shape,
			RecursiveDecompositionTreeCell *root = NULL, ULInt id = 0);
		virtual ~RecursiveDecompositionTree();

		virtual void setShape(GeometricShape *shape);
		virtual GeometricShape *getShape() const;

#if USE_GUI
		virtual void setNodes(const GraphNodeList &nodes);
		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);

		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual void unhighlight() const;
		virtual bool isHighlighted() const;

		using ListDisplayable::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		GeometricShape *shape;
	};
}

#endif //#ifndef _DATA_RECURSIVE_DECOMPOSITION_TREE_H_
