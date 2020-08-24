#ifndef _DATA_TREE_NODE_H_
#define _DATA_TREE_NODE_H_

#include "Data/Definitions.h"
#include "Data/GraphNode.h"

namespace Data
{
	class TreeNode : public Data::GraphNode
	{
	public:

#if USE_EXPERIMENTAL
		TreeNode(ULInt id = 0);
		TreeNode(Tree *tree, ULInt id = 0);
		TreeNode(TreeNode *parent, ULInt id = 0);
#else
		TreeNode(TreeNode *parent = NULL, ULInt id = 0);
#endif //#if USE_EXPERIMENTAL
		virtual ~TreeNode();

		virtual void setTree(Tree *tree);
		virtual Tree *getTree() const;

		virtual GraphNodeList getLeaves() const;
		virtual void getLeaves(GraphNodeList &leaves) const;

		virtual GraphNodeList getInternalNodes() const;
		virtual void getInternalNodes(GraphNodeList &nodes) const;

		virtual void getInternalNodesAndLeaves(GraphNodeList &nodes, GraphNodeList &leaves) const;

		virtual TreeNode *node(ULInt id) const;

		virtual UInt level() const;
		virtual UInt depth() const;

		virtual ULInt numNodes() const;
		virtual ULInt numLeaves() const;
		virtual UInt numChildren() const;

		virtual bool isRoot() const;
		virtual bool isLeaf() const;

		virtual TreeNode *parent() const;
		virtual TreeNode *child(UInt i) const;

		virtual void deleteChildren();

#if USE_ARRAY
		using GraphNode::operator=;
		virtual TreeNode &operator=(const TreeNode &n);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual std::string text() const;
#endif //#if USE_GUI

		//mutable UInt debug[8];

	protected:

		UInt l;
	};
}

#endif //#ifndef _DATA_TREE_NODE_H_
