#include "Data/TreeNode.h"

#include "Data/Tree.h"

using namespace Data;

#if USE_EXPERIMENTAL
Data::TreeNode::TreeNode(ULInt id) :
	GraphNode(NULL, id)
{
	this->l = 0;
}

Data::TreeNode::TreeNode(Tree *tree, ULInt id) :
	GraphNode(tree, id)
{
	this->l = 0;
}
#endif //#if USE_EXPERIMENTAL

Data::TreeNode::TreeNode(TreeNode *parent, ULInt id) :
	GraphNode((parent ? static_cast<Graph *>(parent->getTree()) : NULL), id)
{
#if (!USE_EXPERIMENTAL_5)
	//this->adjacents.push_back(parent);
	this->add(parent);

	if (parent)
	{
		//parent->adjacents.push_back(this);
		parent->add(this);
	}
#endif //#if (!USE_EXPERIMENTAL_5)

	//for (int i = 0; i < 8; i++) this->debug[i] = 0;
	this->l = parent ? (parent->level() + 1) : 0;
}

Data::TreeNode::~TreeNode()
{
#if (!USE_EXPERIMENTAL_5)
	if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

#if USE_ARRAY
	if (this->adjacents)
	{
		for (UInt i = 1; i < this->numAdjs; i++)
		{
			delete this->adjacents[i];
		}

		delete [] this->adjacents;

		this->adjacents = NULL;
		this->numAdjs = 0;
	}
#else
	while (this->adjacents.size() > 1)
	{
		delete this->adjacents.back();

		this->adjacents.pop_back();
	}

	this->adjacents.clear();
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_5)
}

void Data::TreeNode::setTree(Tree *tree)
{
	this->setGraph(static_cast<Graph *>(tree));
}

Tree *Data::TreeNode::getTree() const
{
	return static_cast<Tree *>(this->getGraph());
}

UInt Data::TreeNode::level() const
{
	//return (this->isRoot()) ? 0 : this->parent()->level() + 1;
	return this->l;
}

UInt Data::TreeNode::depth() const
{
	if (this->isLeaf())
	{
		return 0;
	}

	UInt d = 0;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		d = std::max(d, this->child(i)->depth());
	}

	return d + 1;
}

ULInt Data::TreeNode::numNodes() const
{
	if (this->isLeaf())
	{
		return 1;
	}

	ULInt d = 1;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		d += this->child(i)->numNodes();
	}

	return d;
}

ULInt Data::TreeNode::numLeaves() const
{
	if (this->isLeaf())
	{
		return 1;
	}

	ULInt d = 0;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		d += this->child(i)->numLeaves();
	}

	return d;
}

UInt Data::TreeNode::numChildren() const
{
	//return this->adjacents.size() - 1;
	return this->numAdjacents() - 1;
}

bool Data::TreeNode::isRoot() const
{
	return (this->parent() == NULL);
}

bool Data::TreeNode::isLeaf() const
{
	return (this->numChildren() == 0);
}

TreeNode *Data::TreeNode::parent() const
{
	return static_cast<TreeNode *>(this->getAdjacent(0));
}

TreeNode *Data::TreeNode::child(UInt i) const
{
	return static_cast<TreeNode *>(this->getAdjacent(i + 1));
}

void Data::TreeNode::deleteChildren()
{
#if USE_EXPERIMENTAL_5
    UInt numAdjs = this->numAdjacents();

    for (UInt i = 0; i < numAdjs; i++)
    {
        delete this->getAdjacent(i);

        this->setAdjacent(i, NULL);
    }

    if (this->getTree())
    {
        this->getTree()->add(this);
    }
#else
#if USE_ARRAY
	if (this->adjacents)
	{
#if USE_GUI
		UInt numAdjs = this->numAdjs;

		this->numAdjs = 0;
#endif //#if USE_GUI
		GraphNode **adjacents = this->adjacents;

		this->adjacents = new GraphNode*[1];

		this->adjacents[0] = adjacents[0];

		//numAdjs will be this->numAdjs if gui is not enabled or will be the
		//  local numAdjs if gui is enabled
		for (UInt i = 1; i < numAdjs; i++)
		{
			delete adjacents[i];
		}

		delete [] adjacents;

		this->numAdjs = 1;

		if (this->getTree())
		{
			this->getTree()->add(this);
		}
	}
#else
	while (this->adjacents.size() > 1)
	{
		delete this->adjacents.back();

		this->adjacents.pop_back();
	}
#endif //#if USE_ARRAY
#endif //#if USE_EXPERIMENTAL_5
}

GraphNodeList Data::TreeNode::getLeaves() const
{
	GraphNodeList leaves;

	this->getLeaves(leaves);

	return leaves;
}

void Data::TreeNode::getLeaves(GraphNodeList &leaves) const
{
	if (this->isLeaf())
	{
		leaves.push_back(const_cast<TreeNode *>(this));

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		this->child(i)->getLeaves(leaves);
	}
}

GraphNodeList Data::TreeNode::getInternalNodes() const
{
	GraphNodeList nodes;

	this->getInternalNodes(nodes);

	return nodes;
}

void Data::TreeNode::getInternalNodes(GraphNodeList &nodes) const
{
	if (this->isLeaf())
	{
		return;
	}

	nodes.push_back(const_cast<TreeNode *>(this));

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		this->child(i)->getInternalNodes(nodes);
	}
}

void Data::TreeNode::getInternalNodesAndLeaves(GraphNodeList &nodes, GraphNodeList &leaves) const
{
	if (this->isLeaf())
	{
		leaves.push_back(const_cast<TreeNode *>(this));

		return;
	}

	nodes.push_back(const_cast<TreeNode *>(this));

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		this->child(i)->getInternalNodesAndLeaves(nodes, leaves);
	}
}

TreeNode *Data::TreeNode::node(ULInt id) const
{
	if (this->getId() == id)
	{
		return const_cast<TreeNode *>(this);
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		TreeNode *n = this->child(i)->node(id);

		if (n)
		{
			return n;
		}
	}

	return NULL;
}

#if USE_ARRAY
TreeNode &Data::TreeNode::operator=(const TreeNode &n)
{
	if (this != &n)
	{
		Data::GraphNode::operator=(n);

		this->l = n.l;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
std::string Data::TreeNode::text() const
{
	std::stringstream str;

	str << this->getId() << std::endl;

	return str.str();
}
#endif
