#ifndef _DATA_GEOMETRIC_SEARCH_TREE_NODE_H_
#define _DATA_GEOMETRIC_SEARCH_TREE_NODE_H_

#include "Data/Definitions.h"
#include "Data/TreeNode.h"

namespace Data
{
	class GeometricSearchTreeNode : public Data::TreeNode
	{
	public:

#if USE_C__11
		typedef Data::IdentifiableHash ShapeSet;
#else
		typedef Data::IdentifiableSet ShapeSet;
#endif //#if USE_C__11

	public:

		GeometricSearchTreeNode(GeometricSearchTreeNode *parent = NULL, ULInt id = 0);
		virtual ~GeometricSearchTreeNode();

		using TreeNode::add;
		using TreeNode::remove;

		virtual bool add(const Identifiable *s) = 0;
		virtual bool remove(const Identifiable *s) = 0;

		virtual ShapeSet allShapes() const;

		virtual ShapeSet inRange(const Box *box) const = 0;

		virtual void clear();

#if USE_ARRAY
		using TreeNode::operator=;
		virtual GeometricSearchTreeNode &operator=(const GeometricSearchTreeNode &n);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void draw() const = 0;
#endif //#if USE_GUI

	protected:

		ShapeSet shapes;
	};
}

#endif //#ifndef _DATA_GEOMETRIC_SEARCH_TREE_NODE_H_
