#include "PMJMesh/VertexBoxSearchTree.h"

#include "Data/BoxTreeCell.h"
#include "Data/Box.h"

//new
#include "Data/QuadTree.h"
#include "Data/OctTree.h"
#include "Data/BoxSearchTreeCell.h"
//end new

#if USE_EXPERIMENTAL_5

PMJMesh::VertexBoxSearchTree2D::VertexBoxSearchTree2D(
	const MJMesh::Front *front, const Data::BoxTree *tree,
	const Data::Box *box) :
		MJMesh::VertexBoxSearchTree2D(front, tree)
{
	this->cellInBox = false;

	this->setBox(box);
}

PMJMesh::VertexBoxSearchTree2D::~VertexBoxSearchTree2D()
{
	if (this->box)
	{
		delete this->box;
	}
}

void PMJMesh::VertexBoxSearchTree2D::setBox(const Data::Box *box)
{
	this->box = box;
}

/*bool PMJMesh::VertexBoxSearchTree2D::shouldAddChildren(const Data::BoxTreeCell *cell) const
{
	return ((PMJMesh::getPosition(2, cell) != Data::OUT) && (!this->box->optOut(cell->box())));
}*/

/*bool PMJMesh::VertexBoxSearchTree2D::build()
{
	if (!this->box)
	{
		return false;
	}

	bool ret = Data::VertexBoxSearchTree2D::build();

	this->box = NULL;

	return ret;
}*/

#if USE_UNIQUE_SEARCH_TREE
bool PMJMesh::VertexBoxSearchTree2D::build(BoxTreeCell *cell, BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bcell->makeCell();

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(2, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	this->cellInBox = cinb;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	return true;
}
#else
bool PMJMesh::VertexBoxSearchTree2D::build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(2, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	this->cellInBox = cinb;

	return true;
}
#endif //#if USE_UNIQUE_SEARCH_TREE

bool PMJMesh::VertexBoxSearchTree2D::build()
{
	if (!this->box)
	{
		return false;
	}

	//Real debugstart = Data::time();

	if (!this->tree)
	{
		this->tree = this->makeTree();
	}

	if (!this->tree)
	{
		this->box = NULL;

		return false;
	}

	//Data::GraphNodeVector nodes;
	//Data::GraphNodeList nodes;

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	do
	{
		UInt pos = cell->numChildren();

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			//if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->in(this->box))
			if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->optIn(this->box))
			{
				pos = i;

				break;
			}
		}

		if (pos == cell->numChildren())
		{
			break;
		}

		cell = static_cast<Data::BoxTreeCell *>(cell->child(pos));
	}
	while (true);

#if USE_EXPERIMENTAL_TREE
	this->setRoot(this->makeTreeCell(cell, NULL, cell->getId()));
#else
	this->root = this->makeTreeCell(cell, NULL, cell->getId());
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_C__11
#if USE_EXPERIMENTAL_TREE
	this->map.reserve(this->tree->getRoot()->numNodes());
#else
	this->map.reserve(this->root->numNodes());
#endif //#if USE_EXPERIMENTAL_TREE
#endif //#if USE_C__11

#if USE_UNIQUE_SEARCH_TREE
#if USE_EXPERIMENTAL_TREE
	static_cast<Data::BoxSearchTreeCell *>(this->getRoot())->makeCell();
#else
	static_cast<Data::BoxSearchTreeCell *>(this->root)->makeCell();
#endif //#if USE_EXPERIMENTAL_TREE
#endif //#if USE_UNIQUE_SEARCH_TREE

#if USE_EXPERIMENTAL_TREE
	this->getRoot()->setTree(this);
#else
	this->root->setTree(this);
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_UNIQUE_SEARCH_TREE
	Data::BoxTree *tree = new Data::QuadTree(*cell->box()->getMin(), *cell->box()->getMax());

	delete tree->getRoot();

	tree->setRoot(static_cast<Data::BoxSearchTreeCell *>(this->root)->getCell());

	tree->getRoot()->setTree(tree);
#endif //#if USE_UNIQUE_SEARCH_TREE

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
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<Data::BoxSearchTreeCell *>(this->getRoot()));
#else
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<Data::BoxSearchTreeCell *>(this->root));
#endif //#if USE_EXPERIMENTAL_TREE
	}

	/*while (!nodes.empty())
	{
		//cell = (Data::BoxTreeCell *)nodes.back();
		cell = static_cast<Data::BoxTreeCell *>(nodes.front());

		//nodes.pop_back();
		nodes.pop_front();

#if USE_UNIQUE_SEARCH_TREE
		Data::BoxSearchTreeCell *newCell = this->makeTreeCell(cell,
			static_cast<BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());

		newCell->makeCell();

		this->map[cell->getId()] = newCell;
#else
		this->map[cell->getId()] = this->makeTreeCell(cell,
			static_cast<Data::BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());
#endif //#if USE_UNIQUE_SEARCH_TREE

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
	//std::cout << "time for vertex (new): " << Data::time() - debugstart << std::endl;
	//std::cout << "number for vertex (new): " << this->numNodes() << std::endl;
	//debugstart = Data::time();

	this->box = NULL;

#if USE_UNIQUE_SEARCH_TREE
	this->tree = tree;
#endif //#if USE_UNIQUE_SEARCH_TREE

	//this->timeEnd += Data::time() - debugstart;

	return true;
}

PMJMesh::VertexBoxSearchTree3D::VertexBoxSearchTree3D(
	const MJMesh::Front *front, const Data::BoxTree *tree,
	const Data::Box *box) :
		MJMesh::VertexBoxSearchTree3D(front, tree)
{
	this->cellInBox = false;

	this->setBox(box);
}

PMJMesh::VertexBoxSearchTree3D::~VertexBoxSearchTree3D()
{
	if (this->box)
	{
		delete this->box;
	}
}

void PMJMesh::VertexBoxSearchTree3D::setBox(const Data::Box *box)
{
	this->box = box;
}

/*bool PMJMesh::VertexBoxSearchTree3D::shouldAddChildren(const Data::BoxTreeCell *cell) const
{
	return ((PMJMesh::getPosition(3, cell) != Data::OUT) && (!this->box->optOut(cell->box())));
}*/

/*bool PMJMesh::VertexBoxSearchTree3D::build()
{
	if (!this->box)
	{
		return false;
	}

	bool ret = Data::VertexBoxSearchTree3D::build();

	this->box = NULL;

	return ret;
}*/

#if USE_UNIQUE_SEARCH_TREE
bool PMJMesh::VertexBoxSearchTree3D::build(BoxTreeCell *cell, BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bcell->makeCell();

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(3, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	this->cellInBox = cinb;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	return true;
}
#else
bool PMJMesh::VertexBoxSearchTree3D::build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(3, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	this->cellInBox = cinb;

	return true;
}
#endif //#if USE_UNIQUE_SEARCH_TREE

bool PMJMesh::VertexBoxSearchTree3D::build()
{
	if (!this->box)
	{
		return false;
	}

	//Real debugstart = Data::time();

	if (!this->tree)
	{
		this->tree = this->makeTree();
	}

	if (!this->tree)
	{
		this->box = NULL;

		return false;
	}

	//Data::GraphNodeVector nodes;
	//Data::GraphNodeList nodes;

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	do
	{
		UInt pos = cell->numChildren();

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			//if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->in(this->box))
			if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->optIn(this->box))
			{
				pos = i;

				break;
			}
		}

		if (pos == cell->numChildren())
		{
			break;
		}

		cell = static_cast<Data::BoxTreeCell *>(cell->child(pos));
	}
	while (true);

#if USE_EXPERIMENTAL_TREE
	this->setRoot(this->makeTreeCell(cell, NULL, cell->getId()));
#else
	this->root = this->makeTreeCell(cell, NULL, cell->getId());
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_C__11
#if USE_EXPERIMENTAL_TREE
	this->map.reserve(this->tree->getRoot()->numNodes());
#else
	this->map.reserve(this->root->numNodes());
#endif //#if USE_EXPERIMENTAL_TREE
#endif //#if USE_C__11

#if USE_UNIQUE_SEARCH_TREE
#if USE_EXPERIMENTAL_TREE
	static_cast<Data::BoxSearchTreeCell *>(this->getRoot())->makeCell();
#else
	static_cast<Data::BoxSearchTreeCell *>(this->root)->makeCell();
#endif //#if USE_EXPERIMENTAL_TREE
#endif //#if USE_UNIQUE_SEARCH_TREE

#if USE_EXPERIMENTAL_TREE
	this->getRoot()->setTree(this);
#else
	this->root->setTree(this);
#endif //#if USE_EXPERIMENTAL_TREE

#if USE_UNIQUE_SEARCH_TREE
	Data::BoxTree *tree = new Data::OctTree(*cell->box()->getMin(), *cell->box()->getMax());

	delete tree->getRoot();

	tree->setRoot(static_cast<Data::BoxSearchTreeCell *>(this->root)->getCell());

	tree->getRoot()->setTree(tree);
#endif //#if USE_UNIQUE_SEARCH_TREE

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
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<Data::BoxSearchTreeCell *>(this->getRoot()));
#else
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<Data::BoxSearchTreeCell *>(this->root));
#endif //#if USE_EXPERIMENTAL_TREE
	}

	/*while (!nodes.empty())
	{
		//cell = (Data::BoxTreeCell *)nodes.back();
		cell = static_cast<Data::BoxTreeCell *>(nodes.front());

		//nodes.pop_back();
		nodes.pop_front();

#if USE_UNIQUE_SEARCH_TREE
		Data::BoxSearchTreeCell *newCell = this->makeTreeCell(cell,
			static_cast<BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());

		newCell->makeCell();

		this->map[cell->getId()] = newCell;
#else
		this->map[cell->getId()] = this->makeTreeCell(cell,
			static_cast<Data::BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());
#endif //#if USE_UNIQUE_SEARCH_TREE

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
	//std::cout << "time for vertex (new): " << Data::time() - debugstart << std::endl;
	//std::cout << "number for vertex (new): " << this->numNodes() << std::endl;
	//debugstart = Data::time();

	this->box = NULL;

#if USE_UNIQUE_SEARCH_TREE
	this->tree = tree;
#endif //#if USE_UNIQUE_SEARCH_TREE

	//this->timeEnd += Data::time() - debugstart;

	return true;
}

#else

PMJMesh::VertexBoxSearchTree::VertexBoxSearchTree(
	const MJMesh::Front *front, const Data::BoxTree *tree,
	const Data::Box *box) :
		MJMesh::VertexBoxSearchTree(front, tree)
{
	this->cellInBox = false;

	this->dimension = 0;

	this->setBox(box);
}

PMJMesh::VertexBoxSearchTree::~VertexBoxSearchTree()
{
	if (this->box)
	{
		delete this->box;
	}
}

void PMJMesh::VertexBoxSearchTree::setBox(const Data::Box *box)
{
	this->box = box;
}

/*bool PMJMesh::VertexBoxSearchTree::shouldAddChildren(const Data::BoxTreeCell *cell) const
{
	return ((PMJMesh::getPosition(this->dimension, cell) != Data::OUT) && (!this->box->optOut(cell->box())));
}*/

/*bool PMJMesh::VertexBoxSearchTree::build()
{
	if (!this->box)
	{
		return false;
	}

	bool ret = Data::VertexBoxSearchTree::build();

	this->box = NULL;

	return ret;
}*/

#if USE_UNIQUE_SEARCH_TREE
bool PMJMesh::VertexBoxSearchTree::build(BoxTreeCell *cell, BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bcell->makeCell();

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(this->dimension, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	this->cellInBox = cinb;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	return true;
}
#else
bool PMJMesh::VertexBoxSearchTree::build(Data::BoxTreeCell *cell, Data::BoxSearchTreeCell *parent)
{
	Data::BoxSearchTreeCell *bcell = NULL;

	this->map[cell->getId()] = bcell = this->makeTreeCell(cell, parent, cell->getId());

	bool cinb = this->cellInBox;

	if (((!this->cellInBox)) &&
		((PMJMesh::getPosition(this->dimension, cell) == Data::OUT) ||
		(this->cellInBox = this->box->optIn(cell->box()), ((!this->cellInBox) && (cell->outOf(this->box))))))
	{
		return false;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), bcell);
	}

	this->cellInBox = cinb;

	return true;
}
#endif //#if USE_UNIQUE_SEARCH_TREE

bool PMJMesh::VertexBoxSearchTree::build()
{
	if (!this->box)
	{
		return false;
	}

	//Real debugstart = Data::time();

	this->dimension = this->box->dimension();

	if (!this->tree)
	{
		this->tree = this->makeTree();
	}

	if (!this->tree)
	{
		this->box = NULL;

		return false;
	}

	//Data::GraphNodeVector nodes;
	//Data::GraphNodeList nodes;

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	do
	{
		UInt pos = cell->numChildren();

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			//if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->in(this->box))
			if (static_cast<Data::BoxTreeCell *>(cell->child(i))->box()->optIn(this->box))
			{
				pos = i;

				break;
			}
		}

		if (pos == cell->numChildren())
		{
			break;
		}

		cell = static_cast<Data::BoxTreeCell *>(cell->child(pos));
	}
	while (true);

	this->root = this->makeTreeCell(cell, NULL, cell->getId());

#if USE_C__11
	this->map.reserve(this->root->numNodes());
#endif //#if USE_C__11

#if USE_UNIQUE_SEARCH_TREE
	static_cast<Data::BoxSearchTreeCell *>(this->root)->makeCell();
#endif //#if USE_UNIQUE_SEARCH_TREE

	this->root->setTree(this);

#if USE_UNIQUE_SEARCH_TREE
	Data::BoxTree *tree = NULL;

	if (this->dimension == 2)
	{
		tree = new Data::QuadTree(*cell->box()->getMin(), *cell->box()->getMax());
	}
	else
	{
		tree = new Data::OctTree(*cell->box()->getMin(), *cell->box()->getMax());
	}

	delete tree->getRoot();

	tree->setRoot(static_cast<Data::BoxSearchTreeCell *>(this->root)->getCell());

	tree->getRoot()->setTree(tree);
#endif //#if USE_UNIQUE_SEARCH_TREE

	this->map[cell->getId()] = this->root;

	/*for (UInt i = 0; i < cell->numChildren(); i++)
	{
		nodes.push_back(cell->child(i));
	}*/

	//this->timeInit += Data::time() - debugstart;
	//debugstart = Data::time();

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->build(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<Data::BoxSearchTreeCell *>(this->root));
	}

	/*while (!nodes.empty())
	{
		//cell = (Data::BoxTreeCell *)nodes.back();
		cell = static_cast<Data::BoxTreeCell *>(nodes.front());

		//nodes.pop_back();
		nodes.pop_front();

#if USE_UNIQUE_SEARCH_TREE
		Data::BoxSearchTreeCell *newCell = this->makeTreeCell(cell,
			static_cast<BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());

		newCell->makeCell();

		this->map[cell->getId()] = newCell;
#else
		this->map[cell->getId()] = this->makeTreeCell(cell,
			static_cast<Data::BoxSearchTreeCell *>(this->map[cell->parent()->getId()]), cell->getId());
#endif //#if USE_UNIQUE_SEARCH_TREE

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
	//std::cout << "time for vertex (new): " << Data::time() - debugstart << std::endl;
	//std::cout << "number for vertex (new): " << this->numNodes() << std::endl;
	//debugstart = Data::time();

	this->box = NULL;

#if USE_UNIQUE_SEARCH_TREE
	this->tree = tree;
#endif //#if USE_UNIQUE_SEARCH_TREE

	//this->timeEnd += Data::time() - debugstart;

	return true;
}

#endif //#if USE_EXPERIMENTAL_5
