#ifndef _DATA_RECURSIVE_DECOMPOSITION_TREE_CELL_H_
#define _DATA_RECURSIVE_DECOMPOSITION_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/TreeNode.h"
#include "Data/Colorable.h"
#include "Data/GeometricShape.h"

namespace Data
{
	class RecursiveDecompositionTreeCell : public Data::TreeNode
#if USE_GUI
		, public Data::Drawable
#endif //#if USE_GUI
	{
	public:

#if USE_EXPERIMENTAL
		RecursiveDecompositionTreeCell(ULInt id = 0);
		RecursiveDecompositionTreeCell(RecursiveDecompositionTree *tree, ULInt id = 0);
		RecursiveDecompositionTreeCell(RecursiveDecompositionTreeCell *parent, ULInt id = 0);
#else
		RecursiveDecompositionTreeCell(RecursiveDecompositionTreeCell *parent = NULL, ULInt id = 0);
#endif //#if USE_EXPERIMENTAL
		virtual ~RecursiveDecompositionTreeCell();

		virtual bool isSubdivided() const;

		virtual GeometricShape *makeShape() const = 0;
		virtual GeometricShape *shape() const;

		virtual RecursiveDecompositionTreeCell *find(const Point &p) const = 0;
		virtual RecursiveDecompositionTreeCell *find(const Point *p) const = 0;

#if USE_ARRAY
		using TreeNode::operator=;
		virtual RecursiveDecompositionTreeCell &operator=(const RecursiveDecompositionTreeCell &c);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual bool isHighlighted() const;
		virtual void unhighlight() const;

		using Drawable::draw;
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		mutable GeometricShape *shp;
	};
}

#endif //#ifndef _DATA_RECURSIVE_DECOMPOSITION_TREE_CELL_H_
