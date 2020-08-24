#include "Data/BoxSearchTree.h"

#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"
#include "Data/BoxSearchTreeCell.h"

using namespace Data;

//Real Data::BoxSearchTree::timeInit = 0.0;
//Real Data::BoxSearchTree::timeExecute = 0.0;
//Real Data::BoxSearchTree::timeEnd = 0.0;

Data::BoxSearchTree::BoxSearchTree(const BoxTree *tree,
	BoxSearchTreeCell *root, ULInt id) :
		GeometricSearchTree(root, id)
{
	this->twin = false;

	this->setTree(tree);
}

Data::BoxSearchTree::~BoxSearchTree()
{
	if ((!this->twin) && (this->tree))
	{
		this->setRoot(NULL);

		delete this->tree;

		this->tree = NULL;
	}
	else if (this->getRoot())
	{
		this->getRoot()->setTree(NULL);
	}
}

void Data::BoxSearchTree::setTree(const BoxTree *tree)
{
	this->tree = tree;

	this->twin = (this->tree) ? true : false;
}

const BoxTree *Data::BoxSearchTree::getTree() const
{
	return this->tree;
}

GraphNodeList Data::BoxSearchTree::neighbors(const BoxSearchTreeCell *cell) const
{
	GraphNodeList neighs = this->tree->neighbors(cell->getCell());

	//for (UInt i = 0; i < neighs.size(); i++)
	for (GraphNodeList::iterator iter = neighs.begin();
		 iter != neighs.end(); iter++)
	{
#if USE_C__11
		GraphNodeHashMap::const_iterator it = this->map.find((*iter)->getId());
#else
		GraphNodeMap::const_iterator it = this->map.find((*iter)->getId());
#endif //#if USE_C__11

		/*while (it == this->map.end())
		{
			neighs[i] = ((TreeNode *)neighs[i])->parent();

			it = this->map.find(neighs[i]->getId());
		}*/

		//neighs[i] = (it != map.end()) ? (*it).second : NULL;
		(*iter) = (it != map.end()) ? (*it).second : NULL;
	}

	return neighs;
}

/*bool Data::BoxSearchTree::shouldAddChildren(const Data::BoxTreeCell **//*cell*//*) const
{
	return true;
}*/

bool Data::BoxSearchTree::shouldRebuild() const
{
	return false;
}

bool Data::BoxSearchTree::rebuildTreeNodes()
{
	return false;
}

BoxTree *Data::BoxSearchTree::makeTree() const
{
	return NULL;
}

/*bool Data::BoxSearchTree::in(const Point &p) const
{
	return (this->tree) ? this->tree->in(p) : false;
}

bool Data::BoxSearchTree::in(const Point *p) const
{
	return (this->tree) ? this->tree->in(p) : false;
}

bool Data::BoxSearchTree::on(const Point &p) const
{
	return (this->tree) ? this->tree->on(p) : false;
}

bool Data::BoxSearchTree::on(const Point *p) const
{
	return (this->tree) ? this->tree->on(p) : false;
}

bool Data::BoxSearchTree::out(const Point &p) const
{
	return (this->tree) ? this->tree->out(p) : false;
}

bool Data::BoxSearchTree::out(const Point *p) const
{
	return (this->tree) ? this->tree->out(p) : false;
}

BoxTreeCell *Data::BoxSearchTree::find(const Point &p) const
{
	return (this->tree) ? this->tree->find(p) : NULL;
}

BoxTreeCell *Data::BoxSearchTree::find(const Point *p) const
{
	return (this->tree) ? this->tree->find(p) : NULL;
}

Real Data::BoxSearchTree::size() const
{
	return (this->tree) ? this->tree->size() : 0.0;
}

GraphNodeVector Data::BoxSearchTree::neighbors(const BoxTreeCell *c) const
{
	return (this->tree) ? this->tree->neighbors(c) : GraphNodeVector();
}*/

void Data::BoxSearchTree::clear()
{
	this->map.clear();

	if ((!this->twin) && (this->tree))
	{
		delete this->tree;

		this->tree = NULL;
	}

	if (this->getRoot())
	{
#if USE_EXPERIMENTAL_TREE
		static_cast<BoxSearchTreeCell *>(this->getRoot())->setCell(NULL);

		this->getRoot()->setTree(NULL);

		delete this->getRoot();

		this->setRoot(NULL);

        this->nodes.clear();
#else
		this->nodes.clear();

		static_cast<BoxSearchTreeCell *>(this->getRoot())->setCell(NULL);

		this->getRoot()->setTree(NULL);

		delete this->getRoot();

		this->setRoot(NULL);
#endif //#if USE_EXPERIMENTAL_TREE
	}
}

bool Data::BoxSearchTree::build(BoxTreeCell *cell, BoxSearchTreeCell *parent)
{
	BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	/*if (!this->shouldAddChildren(cell))
	{
		return false;
	}*/

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<BoxTreeCell *>(cell->child(i)), bcell);
	}

	return true;
}

bool Data::BoxSearchTree::build()
{
	//Real debugstart = Data::time();

	if (!this->tree)
	{
		this->tree = this->makeTree();
	}

	if (!this->tree)
	{
		return false;
	}

	//GraphNodeList nodes;

#if USE_C__11
	this->map.reserve(this->tree->numNodes());
#endif //#if USE_C__11

	BoxTreeCell *cell = static_cast<BoxTreeCell *>(this->tree->getRoot());

#if USE_EXPERIMENTAL_TREE
	this->setRoot(this->makeTreeCell(cell, NULL, cell->getId()));
#else
	this->root = this->makeTreeCell(cell, NULL, cell->getId());

	this->root->setTree(this);
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_EXPERIMENTAL_TREE
	this->map[cell->getId()] = this->getRoot();
#else
	this->map[cell->getId()] = this->root;
#endif //#if USE_EXPERIMENTAL_TREE

	/*for (UInt i = 0; i < cell->numChildren(); i++)
	{
		nodes.push_back(cell->child(i));
	}*/

	//this->timeInit += Data::time() - debugstart;
	//debugstart = Data::time();

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
#if USE_EXPERIMENTAL_TREE
		this->build(static_cast<BoxTreeCell *>(cell->child(i)), static_cast<BoxSearchTreeCell *>(this->getRoot()));
#else
		this->build(static_cast<BoxTreeCell *>(cell->child(i)), static_cast<BoxSearchTreeCell *>(this->root));
#endif //#if USE_EXPERIMENTAL_TREE
	}

	/*while (!nodes.empty())
	{
		cell = static_cast<BoxTreeCell *>(nodes.back());

		nodes.pop_back();

		this->map[cell->getId()] = this->makeTreeCell(cell,
			static_cast<BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());

		if (!this->shouldAddChildren(cell))
		{
			continue;
		}

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			nodes.push_back(cell->child(i));
		}
	}*/

	//this->timeExecute += Data::time() - debugstart;

	return true;
}

#if USE_GUI
void Data::BoxSearchTree::draw() const
{
	if (this->getRoot())
	{
		this->getRoot()->draw();
	}
}
#endif //#if USE_GUI

#if USE_EXPERIMENTAL_5
Data::BoxSearchTree2D::BoxSearchTree2D(const BoxTree *tree, BoxSearchTreeCell2D *root, ULInt id) :
    Data::BoxSearchTree::BoxSearchTree(tree, root, id)
{

}

Data::BoxSearchTree2D::~BoxSearchTree2D()
{

}

Data::BoxSearchTree3D::BoxSearchTree3D(const BoxTree *tree, BoxSearchTreeCell3D *root, ULInt id) :
    Data::BoxSearchTree::BoxSearchTree(tree, root, id)
{

}

Data::BoxSearchTree3D::~BoxSearchTree3D()
{

}
#endif //#if USE_EXPERIMENTAL_5
