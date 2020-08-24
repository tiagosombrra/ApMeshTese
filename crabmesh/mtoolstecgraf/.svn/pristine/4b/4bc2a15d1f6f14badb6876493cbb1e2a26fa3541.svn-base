#ifndef _DATA_TREE_H_
#define _DATA_TREE_H_

#include "Data/Definitions.h"
#include "Data/Graph.h"

namespace Data
{
	class Tree : public Data::Graph
	{
    public:

#if USE_EXPERIMENTAL_TREE
        class LeafIterator
        {
        public:

            LeafIterator();
            LeafIterator(TreeNode *node);

            LeafIterator &operator++();
			LeafIterator operator++(int);

			LeafIterator &operator--();
			LeafIterator operator--(int);

			LeafIterator &operator=(const LeafIterator &iter);

			bool operator==(const LeafIterator &iter);
			bool operator!=(const LeafIterator &iter);

			TreeNode *operator*() const;

        protected:

            TreeNode *node;
        };

        class ConstLeafIterator
        {
        public:

            ConstLeafIterator();
            ConstLeafIterator(const TreeNode *node);

            ConstLeafIterator &operator++();
			ConstLeafIterator operator++(int);

			ConstLeafIterator &operator--();
			ConstLeafIterator operator--(int);

			ConstLeafIterator &operator=(const ConstLeafIterator &iter);

			bool operator==(const ConstLeafIterator &iter);
			bool operator!=(const ConstLeafIterator &iter);

			const TreeNode *operator*() const;

        protected:

            const TreeNode *node;
        };
#else
        typedef Data::Graph::NodeIterator LeafIterator;
		typedef Data::Graph::ConstNodeIterator ConstLeafIterator;
#endif //#if USE_EXPERIMENTAL_TREE

	public:

		Tree(TreeNode *root = NULL, ULInt id = 0);
		virtual ~Tree();

		virtual void setRoot(TreeNode *root);
		virtual TreeNode *getRoot() const;

		virtual GraphNode *node(ULInt id) const;

#if (!USE_EXPERIMENTAL_TREE)
		virtual void addChildren(TreeNode *n);
#endif //#if (!USE_EXPERIMENTAL_TREE)

#if (!USE_EXPERIMENTAL_TREE)
		virtual void setLeaves(const GraphNodeList &leaves);
#endif //#if (!USE_EXPERIMENTAL_TREE)
		virtual GraphNodeList getLeaves() const;
#if USE_EXPERIMENTAL_TREE
        virtual void getLeaves(GraphNodeList &leaves) const;
#endif //#if USE_EXPERIMENTAL_TREE
#if (!USE_EXPERIMENTAL_TREE)
		virtual const GraphNodeSet &getLeavesSet() const;
#endif //#if (!USE_EXPERIMENTAL_TREE)

		virtual LeafIterator lBegin();
		virtual ConstLeafIterator lBegin() const;
		virtual LeafIterator lEnd();
		virtual ConstLeafIterator lEnd() const;

		virtual ULInt numNodes() const;
		virtual ULInt numLeaves() const;
		virtual UInt depth() const;

#if (!USE_EXPERIMENTAL_TREE)
	protected:

		TreeNode *root;
#endif //#if (!USE_EXPERIMENTAL_TREE)
	};
}

#endif //#ifndef _DATA_TREE_H_
