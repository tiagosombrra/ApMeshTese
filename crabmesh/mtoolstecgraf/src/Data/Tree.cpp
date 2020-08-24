#include "Data/Tree.h"

#include "Data/TreeNode.h"

using namespace Data;

#if USE_EXPERIMENTAL_TREE
Data::Tree::LeafIterator::LeafIterator()
{
    this->node = NULL;
}

Data::Tree::LeafIterator::LeafIterator(TreeNode *node)
{
    this->node = node;

    if (this->node)
    {
        while (!this->node->isLeaf())
        {
            this->node = this->node->child(0);
        }
    }
}

Data::Tree::LeafIterator &Data::Tree::LeafIterator::operator++()
{
    TreeNode *parent = this->node->parent();

    while (parent)
    {
        for (UInt i = 0; i < parent->numChildren() - 1; i++)
        {
            if (parent->child(i) == this->node)
            {
                this->node = parent->child(i + 1);

                while (!this->node->isLeaf())
                {
                    this->node = this->node->child(0);
                }

                return (*this);
            }
        }

        this->node = parent;

        parent = this->node->parent();
    }

    this->node = NULL;

    return (*this);
}

Data::Tree::LeafIterator Data::Tree::LeafIterator::operator++(int)
{
    LeafIterator tmp = (*this);

    ++(*this);

    return tmp;
}

Data::Tree::LeafIterator &Data::Tree::LeafIterator::operator--()
{
    TreeNode *parent = this->node->parent();

    while (parent)
    {
        for (UInt i = parent->numChildren() - 1; i > 0; i--)
        {
            if (parent->child(i) == this->node)
            {
                this->node = parent->child(i - 1);

                while (!this->node->isLeaf())
                {
                    this->node = this->node->child(this->node->numChildren() - 1);
                }

                return (*this);
            }
        }

        this->node = parent;

        parent = this->node->parent();
    }

    this->node = NULL;

    return (*this);
}

Data::Tree::LeafIterator Data::Tree::LeafIterator::operator--(int)
{
    LeafIterator tmp = (*this);

    --(*this);

    return tmp;
}

Data::Tree::LeafIterator &Data::Tree::LeafIterator::operator=(const LeafIterator &iter)
{
    this->node = iter.node;

    return (*this);
}

bool Data::Tree::LeafIterator::operator==(const LeafIterator &iter)
{
    return (this->node == iter.node);
}

bool Data::Tree::LeafIterator::operator!=(const LeafIterator &iter)
{
    return (this->node != iter.node);
}

TreeNode *Data::Tree::LeafIterator::operator*() const
{
    return node;
}

Data::Tree::ConstLeafIterator::ConstLeafIterator()
{
    this->node = NULL;
}

Data::Tree::ConstLeafIterator::ConstLeafIterator(const TreeNode *node)
{
    this->node = node;

    if (this->node)
    {
        while (!this->node->isLeaf())
        {
            this->node = this->node->child(0);
        }
    }
}

Data::Tree::ConstLeafIterator &Data::Tree::ConstLeafIterator::operator++()
{
    const TreeNode *parent = this->node->parent();

    while (parent)
    {
        for (UInt i = 0; i < parent->numChildren() - 1; i++)
        {
            if (parent->child(i) == this->node)
            {
                this->node = parent->child(i + 1);

                while (!this->node->isLeaf())
                {
                    this->node = this->node->child(0);
                }

                return (*this);
            }
        }

        this->node = parent;

        parent = this->node->parent();
    }

    this->node = NULL;

    return (*this);
}

Data::Tree::ConstLeafIterator Data::Tree::ConstLeafIterator::operator++(int)
{
    ConstLeafIterator tmp = (*this);

    ++(*this);

    return tmp;
}

Data::Tree::ConstLeafIterator &Data::Tree::ConstLeafIterator::operator--()
{
    const TreeNode *parent = this->node->parent();

    while (parent)
    {
        for (UInt i = parent->numChildren() - 1; i > 0; i--)
        {
            if (parent->child(i) == this->node)
            {
                this->node = parent->child(i - 1);

                while (!this->node->isLeaf())
                {
                    this->node = this->node->child(this->node->numChildren() - 1);
                }

                return (*this);
            }
        }

        this->node = parent;

        parent = this->node->parent();
    }

    this->node = NULL;

    return (*this);
}

Data::Tree::ConstLeafIterator Data::Tree::ConstLeafIterator::operator--(int)
{
    ConstLeafIterator tmp = (*this);

    --(*this);

    return tmp;
}

Data::Tree::ConstLeafIterator &Data::Tree::ConstLeafIterator::operator=(const ConstLeafIterator &iter)
{
    this->node = iter.node;

    return (*this);
}

bool Data::Tree::ConstLeafIterator::operator==(const ConstLeafIterator &iter)
{
    return (this->node == iter.node);
}

bool Data::Tree::ConstLeafIterator::operator!=(const ConstLeafIterator &iter)
{
    return (this->node != iter.node);
}

const TreeNode *Data::Tree::ConstLeafIterator::operator*() const
{
    return node;
}
#endif //#if USE_EXPERIMENTAL_TREE

Data::Tree::Tree(TreeNode *root, ULInt id) :
	Graph(id)
{
	this->setRoot(root);
}

Data::Tree::~Tree()
{
#if USE_EXPERIMENTAL_TREE
    if (this->getRoot())
    {
        delete this->getRoot();
    }

    this->nodes.clear();
#else
	this->nodes.clear();

	if (this->root)
	{
		delete this->root;
	}
#endif //#if USE_EXPERIMENTAL_TREE
}

#if (!USE_EXPERIMENTAL_TREE)
void Data::Tree::addChildren(TreeNode *n)
{
	this->remove(n);

	for (UInt i = 0; i < n->numChildren(); i++)
	{
		this->add(n->child(i));
	}
}
#endif //#if (!USE_EXPERIMENTAL_TREE)

#if (!USE_EXPERIMENTAL_TREE)
void Data::Tree::setLeaves(const GraphNodeList &leaves)
{
	this->setNodes(leaves);
}
#endif //#if (!USE_EXPERIMENTAL_TREE)

GraphNodeList Data::Tree::getLeaves() const
{
#if USE_EXPERIMENTAL_TREE
    GraphNodeList leaves;

    this->getLeaves(leaves);

    return leaves;
#else
	return this->getNodes();
#endif //#if USE_EXPERIMENTAL_TREE
}

#if USE_EXPERIMENTAL_TREE
void Data::Tree::getLeaves(GraphNodeList &leaves) const
{
    this->getRoot()->getLeaves(leaves);
}
#endif //#if USE_EXPERIMENTAL_TREE

#if (!USE_EXPERIMENTAL_TREE)
const GraphNodeSet &Data::Tree::getLeavesSet() const
{
	return this->getNodesSet();
}
#endif //#if USE_EXPERIMENTAL_TREE

Data::Tree::LeafIterator Data::Tree::lBegin()
{
#if USE_EXPERIMENTAL_TREE
    return Data::Tree::LeafIterator(this->getRoot());
#else
    return this->nBegin();
#endif //#if USE_EXPERIMENTAL_TREE
}

Data::Tree::ConstLeafIterator Data::Tree::lBegin() const
{
#if USE_EXPERIMENTAL_TREE
    return Data::Tree::ConstLeafIterator(this->getRoot());
#else
    return this->nBegin();
#endif //#if USE_EXPERIMENTAL_TREE
}

Data::Tree::LeafIterator Data::Tree::lEnd()
{
#if USE_EXPERIMENTAL_TREE
    return Data::Tree::LeafIterator(NULL);
#else
    return this->nEnd();
#endif //#if USE_EXPERIMENTAL_TREE
}

Data::Tree::ConstLeafIterator Data::Tree::lEnd() const
{
#if USE_EXPERIMENTAL_TREE
    return Data::Tree::ConstLeafIterator(NULL);
#else
    return this->nEnd();
#endif //#if USE_EXPERIMENTAL_TREE
}

ULInt Data::Tree::numNodes() const
{
#if USE_EXPERIMENTAL_TREE
	return (this->getRoot()) ? this->getRoot()->numNodes() : 0;
#else
	return (this->root) ? this->root->numNodes() : 0;
#endif //#if USE_EXPERIMENTAL_TREE
}

ULInt Data::Tree::numLeaves() const
{
#if USE_EXPERIMENTAL_TREE
	return (this->getRoot()) ? this->getRoot()->numLeaves() : 0;
#else
	return (this->root) ? this->root->numLeaves() : 0;
#endif //#if USE_EXPERIMENTAL_TREE
}

UInt Data::Tree::depth() const
{
#if USE_EXPERIMENTAL_TREE
	return (this->getRoot()) ? this->getRoot()->depth() : 0;
#else
	return (this->root) ? this->root->depth() : 0;
#endif //#if USE_EXPERIMENTAL_TREE
}

void Data::Tree::setRoot(TreeNode *root)
{
#if USE_EXPERIMENTAL_TREE
    this->nodes.clear();

    this->nodes.insert(root);

    if (this->getRoot())
    {
        this->getRoot()->setTree(this);
    }
#else
	this->root = root;

	if (this->root)
	{
		this->add(this->root);

		this->root->setTree(this);
	}
#endif //#if USE_EXPERIMENTAL_TREE
}

TreeNode *Data::Tree::getRoot() const
{
#if USE_EXPERIMENTAL_TREE
	return this->nodes.empty() ? NULL : static_cast<Data::TreeNode *>((*this->nodes.begin()));
#else
	return this->root;
#endif //#if USE_EXPERIMENTAL_TREE
}

GraphNode *Data::Tree::node(ULInt id) const
{
	//depth first search
	//return (this->root) ? this->root->node(id) : NULL;

	//breadth first search
#if USE_EXPERIMENTAL_TREE
	if (!this->getRoot())
#else
	if (!this->root)
#endif //#if USE_EXPERIMENTAL_TREE
	{
		return NULL;
	}

	GraphNodeList nodes;

#if USE_EXPERIMENTAL_TREE
	nodes.push_back(this->getRoot());
#else
	nodes.push_back(this->root);
#endif //#if USE_EXPERIMENTAL_TREE

	while (!nodes.empty())
	{
		TreeNode *node = static_cast<TreeNode *>(nodes.front());

		nodes.pop_front();

		if (node->getId() == id)
		{
			return node;
		}

		for (UInt i = 0; i < node->numChildren(); i++)
		{
			nodes.push_back(node->child(i));
		}
	}

	return NULL;
}
