#include "PMJMesh/PositionTreeRefiner.h"

#include "Data/Tree.h"
#include "Data/BoxTreeCell.h"

#if USE_GUI
#include "PMJMesh/AngleTreeCellClassifier.h"
#endif //#if USE_GUI

PMJMesh::PositionTreeRefiner::PositionTreeRefiner(UInt dimension, Data::Tree *tree,
	/*UInt minLevel, */const Data::PositionMap *position) :
	TreeRefiner(tree)
{
	this->setDimension(dimension);

	//this->setMinLevel(minLevel);

	this->setPosition(position);
}

PMJMesh::PositionTreeRefiner::~PositionTreeRefiner()
{

}

void PMJMesh::PositionTreeRefiner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

/*void PMJMesh::PositionTreeRefiner::setMinLevel(UInt minLevel)
{
	this->minLevel = minLevel;
}*/

void PMJMesh::PositionTreeRefiner::setPosition(const Data::PositionMap *position)
{
	this->position = position;
}

/*//debug
void printPosition(const Data::PositionMap *position)
{
	for (Data::PositionMap::const_iterator iter = position->begin();
		 iter != position->end(); iter++)
	{
		std::cerr << (*iter).first << std::endl;
	}
}

void printTree(const Data::Tree *t)
{
	Data::GraphNodeList nodes;

	nodes.push_back(t->getRoot());

	while (!nodes.empty())
	{
		Data::TreeNode *n = static_cast<Data::TreeNode *>(nodes.front());

		nodes.pop_front();

		std::cerr << n->getId() << std::endl;

		for (UInt i = 0; i < n->numChildren(); i++)
		{
			nodes.push_back(n->child(i));
		}
	}
}
//endebug*/

void PMJMesh::PositionTreeRefiner::execute()
{
	if (!this->tree)
	{
		return;
	}

	//debug
	//std::cerr << std::endl << "now printing the tree" << std::endl << std::endl;
	//printTree(this->tree);
	//std::cerr << std::endl << "now printing the position" << std::endl << std::endl;
	//printPosition(this->position);
	//endebug

	this->setTreeOn(this->tree->getRoot());

	this->refine(this->tree->getRoot());

	UInt numChildren = (this->dimension == 2) ? 4 : 8;

	this->refine(this->tree->getRoot(), numChildren);
}

void PMJMesh::PositionTreeRefiner::setTreeOn(Data::TreeNode *n, bool force)
{
	if ((!n->getTree()) || (force))
	{
		n->setTree(this->tree);

#if (!USE_EXPERIMENTAL_TREE)
		if (!n->isLeaf())
		{
			this->tree->addChildren(n);
		}
#endif //#if (!USE_EXPERIMENTAL_TREE)

		force = true;
	}

	for (UInt i = 0; i < n->numChildren(); i++)
	{
		this->setTreeOn(n->child(i), force);
	}
}

void PMJMesh::PositionTreeRefiner::refine(Data::TreeNode *n)
{
	/*bool found = false;

	//the first adjacent is the parent
	for (UInt i = 1; i < n->numChildren(); i++)
	{
		if (this->position->find(n->child(i)->getId()) != this->position->end())
		{
			found = true;

			break;
		}
	}*/

	/*bool found = true;

	//the first adjacent is the parent
	for (UInt i = 1; i < n->numChildren(); i++)
	{
		if (this->position->find(n->child(i)->getId()) == this->position->end())
		{
			found = false;

			break;
		}
	}

	if (!found)
	{
		n->deleteChildren();
	}

	if (n->isLeaf())
	{
		if ((this->position->find(n->getId()) == this->position->end()) &&
			(!n->isRoot()))
		{
			n->parent()->remove(n);
		}
	}
	else if (found)
	{
		Data::GraphNodeVector adjs = n->getAdjacents();

		UInt size = static_cast<UInt>(adjs.size());

		//the first adjacent is the parent
		for (UInt i = 1; i < size; i++)
		{
			this->refine(static_cast<Data::TreeNode *>(adjs[i]));
		}
	}*/

	if (!n->isLeaf())
	{
		//bool found = (this->position->count(n->child(0)->getId()) > 0);
		bool found = (this->position->count(n->getId()*n->numChildren() + 1) > 0);

		if (found)
		{
			for (UInt i = 0; i < n->numChildren(); i++)
			{
				this->refine(n->child(i));
			}
		}
		else
		{
			n->deleteChildren();
		}
	}

	/*Data::GraphNodeVector adjs = n->getAdjacents();

	UInt size = static_cast<UInt>(adjs.size());

	//the first adjacent is the parent
	for (UInt i = 1; i < size; i++)
	{
		this->refine(static_cast<Data::TreeNode *>(adjs[i]));
	}

	if ((n->isLeaf()) &&
		(this->position->find(n->getId()) == this->position->end()))
	{
		//std::cerr << "debug PositionTreeRefiner::refine(TreeNode *n), n->id = " << n->getId() << std::endl;

		n->parent()->remove(n);

		delete n;
	}*/
}

void PMJMesh::PositionTreeRefiner::refine(Data::TreeNode *n, UInt numChildren)
{
	if (n->isLeaf())
	{
		ULInt first = n->getId()*numChildren + 1;
		//ULInt last = first + numChildren;

		if (this->position->find(first) != this->position->end())
		{
			this->subdivide(n);
		}
		/*else
		{
			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n), (*this->position->find(n->getId())).second);
		}*/
	}

	for (UInt i = 0; i < n->numChildren(); i++)
	{
		this->refine(n->child(i), numChildren);
	}
}

void PMJMesh::PositionTreeRefiner::subdivide(Data::TreeNode *n)
{
	Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(n);

	c->subdivide();

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		Data::PositionMap::const_iterator it = this->position->find(c->child(i)->getId());

		PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), (*it).second);
	}
}
