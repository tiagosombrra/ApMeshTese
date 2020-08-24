#include "Performer/TemplateMeshGenerator.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Mesh.h"
#include "Performer/IdManager.h"

using namespace Performer;

const Performer::Status Performer::TemplateMeshGenerator::notInit =
	Performer::Status(Performer::TemplateMeshGenerator::NOT_INIT, "not initiated");
const Performer::Status Performer::TemplateMeshGenerator::init =
	Performer::Status(Performer::TemplateMeshGenerator::INIT, "initiated");
const Performer::Status Performer::TemplateMeshGenerator::success =
	Performer::Status(Performer::TemplateMeshGenerator::SUCCESS, "success");
const Performer::Status Performer::TemplateMeshGenerator::error =
	Performer::Status(Performer::TemplateMeshGenerator::ERROR, "error");

Performer::TemplateMeshGenerator::VertexTree::VertexTree(Data::BoxTreeCell *c, VertexTree *parent) :
	Data::TreeNode::TreeNode(parent, c->getId())
{
	this->cell = c;

	for (UInt i = 0; i < 51; i++)
	{
		this->vertices[i] = NULL;
	}

	for (UInt i = 0; i < 9; i++)
	{
		this->adjs[i] = NULL;
	}
}

Performer::TemplateMeshGenerator::VertexTree::~VertexTree()
{

}

UInt Performer::TemplateMeshGenerator::VertexTree::numAdjacents() const
{
	return 9;
}

void Performer::TemplateMeshGenerator::VertexTree::setAdjacent(UInt i, Data::GraphNode *n)
{
	this->adjs[i] = n;
}

Data::GraphNode *Performer::TemplateMeshGenerator::VertexTree::getAdjacent(UInt i) const
{
	return this->adjs[i];
}

void Performer::TemplateMeshGenerator::VertexTree::add(Data::GraphNode *n)
{
	for (UInt i = 0; i < this->numAdjacents(); i++)
	{
		if (!this->adjs[i])
		{
			this->adjs[i] = n;
		}
	}
}

void Performer::TemplateMeshGenerator::VertexTree::remove(Data::GraphNode *n)
{
	for (UInt i = 0; i < this->numAdjacents(); i++)
	{
		if (this->adjs[i] == n)
		{
			this->adjs[i] = NULL;
		}
	}
}

Data::GraphNodeVector Performer::TemplateMeshGenerator::VertexTree::getAdjacents() const
{
	return Data::GraphNodeVector(this->adjs, this->adjs + this->numAdjacents());
}

Performer::TemplateMeshGenerator::TemplateMeshGenerator() :
	Performer::MeshGenerator::MeshGenerator()
{
	this->status = this->notInit;
	this->tree = NULL;
	this->root = NULL;
}

Performer::TemplateMeshGenerator::~TemplateMeshGenerator()
{
	if (this->root)
	{
		delete this->root;
	}
}

void Performer::TemplateMeshGenerator::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

void Performer::TemplateMeshGenerator::execute()
{
	if (!this->tree)
	{
		this->status = this->error;

		return;
	}

	if (!this->mesh)
	{
		this->mesh = this->makeMesh();
	}

	if (!this->root)
	{
		this->makeTree();
	}

	this->execute(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->root);

	this->status = this->success;

	this->deleteExtraVertices();
}

void Performer::TemplateMeshGenerator::execute(const Data::BoxTreeCell *cell, VertexTree *root)
{
	if (cell->isLeaf())
	{
		if (!this->shouldApplyTemplate(cell))
		{
			return;
		}

		Data::BoolVector inMesh;

		for (UInt i = 0; i < 51; i++)
		{
			inMesh.push_back((root->vertices[i]) && (this->getMesh()->have(root->vertices[i])));
		}

		Data::ElementList newElements;
		Data::FrontElementList newFrontElements;
		Data::VertexList newVertices;

		this->applyTemplate(cell, root, newElements, newFrontElements, newVertices);

		for (UInt i = 0; i < 51; i++)
		{
			if ((!root->vertices[i]) || (inMesh[i]))
			{
				continue;
			}

			for (Data::ElementList::iterator iter = newElements.begin();
				 iter != newElements.end(); iter++)
			{
				if ((*iter)->have(root->vertices[i]))
				{
					if (!this->getMesh()->have(root->vertices[i]))
					{
						this->getMesh()->add(root->vertices[i]);
					}

					if (root->vertices[i]->getId() == 0)
					{
						root->vertices[i]->setId(this->idManager->next(0));
					}

					break;
				}
			}
		}

		while (!newElements.empty())
		{
			Data::Element *el = newElements.front();

			newElements.pop_front();

			if (!this->getMesh()->have(el))
			{
				this->getMesh()->add(el);
			}

			if (el->getId() == 0)
			{
				el->setId(this->idManager->next(2));
			}
		}

		while (!newFrontElements.empty())
		{
			Data::FrontElement *fel = newFrontElements.front();

			newFrontElements.pop_front();

			if (!this->getMesh()->have(fel))
			{
				this->getMesh()->add(fel);
			}

			if (fel->getId() == 0)
			{
				fel->setId(this->idManager->next(1));
			}
		}

		while (!newVertices.empty())
		{
			Data::Vertex *v = newVertices.front();

			newVertices.pop_front();

			if (!this->getMesh()->have(v))
			{
				this->getMesh()->add(v);
			}

			if (v->getId() == 0)
			{
				v->setId(this->idManager->next(0));
			}
		}

		return;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->execute(static_cast<Data::BoxTreeCell *>(cell->child(i)), static_cast<VertexTree *>(root->child(i)));
	}
}

bool Performer::TemplateMeshGenerator::shouldApplyTemplateInAnyDescendant(const Data::BoxTreeCell *cell)
{
	if (cell->isLeaf())
	{
		return this->shouldApplyTemplate(cell);
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		if (this->shouldApplyTemplateInAnyDescendant(static_cast<Data::BoxTreeCell *>(cell->child(i))))
		{
			return true;
		}
	}

	return false;
}

void Performer::TemplateMeshGenerator::makeTree()
{
	this->root = new VertexTree(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), NULL);

	//this->makeTree(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->root);

	Data::GraphNodeList cells;

	cells.push_back(this->root);

	while (!cells.empty())
	{
		VertexTree *root = static_cast<VertexTree *>(cells.front());

		cells.pop_front();

		if (!this->shouldApplyTemplateInAnyDescendant(root->cell))
		{
			continue;
		}

		this->makeVertices(root);

		if (root->cell->isLeaf())
		{
			continue;
		}

		for (UInt i = 0; i < root->cell->numChildren(); i++)
		{
			Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(root->cell->child(i));

			VertexTree *croot = new VertexTree(child, root);

			croot->adjs[0] = root;
			root->adjs[i+1] = croot;

			cells.push_back(croot);
		}
	}

	this->status = this->init;
}

/*void Performer::TemplateMeshGenerator::makeTree(const Data::BoxTreeCell *cell, VertexTree *root) const
{
	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(cell->child(i));

		VertexTree *croot = new VertexTree(child, root);

		root->add(croot);

		this->makeTree(child, croot);
	}
}*/

void Performer::TemplateMeshGenerator::makeVertices(VertexTree *root)
{
	if (root->cell->dimension() == 2)
	{
		Data::Point2D min(root->cell->box()->getMin());
		Data::Point2D max(root->cell->box()->getMax());

		//map of indices from child to parent, depending on the position of the child:
		//  index 0 of child 0 corresponds to index 0 of its parent
		//  index 1 of child 0 corresponds to index 4 of its parent
		//  index 2 of child 0 corresponds to index 8 of its parent
		//  index 3 of child 0 corresponds to index 7 of its parent
		//  index 0 of child 1 corresponds to index 4 of its parent
		//  and so on...
		static const UInt mapParent[4][4] =
		{
			{0,4,8,7},
			{4,1,5,8},
			{8,5,2,6},
			{7,8,6,3}
		};

		//map of indices from parent to parent neighbor, depending on the side:
		//  index 5 of parent corresponds to index 7 of its right  (0) neighbor
		//  index 6 of parent corresponds to index 4 of its top    (1) neighbor
		//  index 7 of parent corresponds to index 5 of its left   (2) neighbor
		//  index 4 of parent corresponds to index 6 of its bottom (3) neighbor
		static UInt mapParentNeighbor[4][2] =
		{
			{5,7},
			{6,4},
			{7,5},
			{4,6}
		};

		/*mapParentNeighbor[0][0] = 5; mapParentNeighbor[0][1] = 7;
		mapParentNeighbor[1][0] = 6; mapParentNeighbor[1][1] = 4;
		mapParentNeighbor[2][0] = 7; mapParentNeighbor[2][1] = 5;
		mapParentNeighbor[3][0] = 4; mapParentNeighbor[3][1] = 6;*/

		UInt pos = 5;

		if (root->parent())
		{
			pos = root->cell->position();

			/*if (pos == 0)
			{
				mapParent[0] = 0;
				mapParent[1] = 4;
				mapParent[2] = 8;
				mapParent[3] = 7;
			}
			else if (pos == 1)
			{
				mapParent[0] = 4;
				mapParent[1] = 1;
				mapParent[2] = 5;
				mapParent[3] = 8;
			}
			else if (pos == 2)
			{
				mapParent[0] = 8;
				mapParent[1] = 5;
				mapParent[2] = 2;
				mapParent[3] = 6;
			}
			else if (pos == 3)
			{
				mapParent[0] = 7;
				mapParent[1] = 8;
				mapParent[2] = 6;
				mapParent[3] = 3;
			}*/

			for (UInt i = 0; i < 4; i++)
			{
				root->vertices[i] = static_cast<VertexTree *>(root->parent())->vertices[mapParent[i][pos]];
			}
		}

		if (!root->vertices[0])
		{
			root->vertices[0] = this->makeVertex(min, 0);

			this->vertices.push_back(root->vertices[0]);
		}

		if (!root->vertices[1])
		{
			root->vertices[1] = this->makeVertex(Data::Point2D(max.getX(), min.getY()), 0);

			this->vertices.push_back(root->vertices[1]);
		}

		if (!root->vertices[2])
		{
			root->vertices[2] = this->makeVertex(max, 0);

			this->vertices.push_back(root->vertices[2]);
		}

		if (!root->vertices[3])
		{
			root->vertices[3] = this->makeVertex(Data::Point2D(min.getX(), max.getY()), 0);

			this->vertices.push_back(root->vertices[3]);
		}

		if (root->parent())
		{
			for (UInt i = 0; i < 4; i++)
			{
				UInt parentPos = mapParent[i][pos];

				if (static_cast<VertexTree *>(root->parent())->vertices[parentPos])
				{
					continue;
				}

				static_cast<VertexTree *>(root->parent())->vertices[parentPos] = root->vertices[i];

				if ((parentPos < 4) || (parentPos == 8))
				{
					continue;
				}

				for (UInt side = 0; side < 4; side++)
				{
					if (mapParentNeighbor[side][0] != parentPos)
					{
						continue;
					}

					const Data::BoxTreeCell *parent = static_cast<const VertexTree *>(root->parent())->cell;

					Data::BoxTreeCell *neigh = this->tree->neighbor(parent, side);

					if ((!neigh) || (neigh->level() != parent->level()))
					{
						continue;
					}

					VertexTree *neighRoot = this->mate(neigh);

					neighRoot->vertices[mapParentNeighbor[side][1]] = root->vertices[i];
				}
			}
		}
	}
	else
	{
		Data::Point3D min(root->cell->box()->getMin());
		Data::Point3D max(root->cell->box()->getMax());

		static const UInt mapParent[8][8] =
		{
			{ 0, 8,25,11,16,24,50,23},
			{ 8, 1, 9,25,24,17,20,50},
			{25, 9, 2,10,50,20,18,21},
			{11,25,10, 3,23,50,21,19},
			{16,24,50,23, 4,12,22,15},
			{24,17,20,50,12, 5,13,22},
			{50,20,18,21,22,13, 6,14},
			{23,50,21,19,15,22,14, 7}
		};

		static UInt mapParentNeighbor[6][10] =
		{
			{20,23, 9,11,13,15,17,16,18,19},
			{21,24,10, 8,14,12,18,17,19,16},
			{22,25,12, 8,13, 9,14,10,15,11},
			{23,20,11, 9,15,13,16,17,19,18},
			{24,21, 8,10,12,14,16,19,17,18},
			{25,22, 8,12, 9,13,10,14,11,15}
		};

		//on sides 0,1, the index 18 (on the parent) corresponds to the index 16
		//  on the edge neighbor (of the parent), if they are on the same level;
		//  otherwise, if the position of the parent is 2, the index 18
		//  corresponds to the index 42, else (the position is 6), the index 18
		//  corresponds to the index 43

		static UInt mapParentEdgeNeighbor[12][7] =
		{
			{0,1,18,16,2,42,43},
			{0,2,13,11,5,33,32},
			{0,4,17,19,1,48,49},
			{0,5, 9,15,1,41,40},

			{1,2,14, 8,6,27,26},
			{1,3,19,17,3,44,45},
			{1,5,10,12,2,35,34},

			{2,3,15, 9,4,28,29},
			{2,4,12,10,4,31,30},

			{3,4,16,18,0,46,47},
			{3,5,11,13,0,36,37},

			{4,5, 8,14,0,39,38}
		};

		UInt pos = 9;

		if (root->parent())
		{
			pos = root->cell->position();

			for (UInt i = 0; i < 8; i++)
			{
				root->vertices[i] = static_cast<VertexTree *>(root->parent())->vertices[mapParent[i][pos]];
			}
		}

		if (!root->vertices[0])
		{
			root->vertices[0] = this->makeVertex(min, 0);

			this->vertices.push_back(root->vertices[0]);
		}

		if (!root->vertices[1])
		{
			root->vertices[1] = this->makeVertex(Data::Point3D(max.getX(), min.getY(), min.getZ()), 0);

			this->vertices.push_back(root->vertices[1]);
		}

		if (!root->vertices[2])
		{
			root->vertices[2] = this->makeVertex(Data::Point3D(max.getX(), max.getY(), min.getZ()), 0);

			this->vertices.push_back(root->vertices[2]);
		}

		if (!root->vertices[3])
		{
			root->vertices[3] = this->makeVertex(Data::Point3D(min.getX(), max.getY(), min.getZ()), 0);

			this->vertices.push_back(root->vertices[3]);
		}

		if (!root->vertices[4])
		{
			root->vertices[4] = this->makeVertex(Data::Point3D(min.getX(), min.getY(), max.getZ()), 0);

			this->vertices.push_back(root->vertices[4]);
		}

		if (!root->vertices[5])
		{
			root->vertices[5] = this->makeVertex(Data::Point3D(max.getX(), min.getY(), max.getZ()), 0);

			this->vertices.push_back(root->vertices[5]);
		}

		if (!root->vertices[6])
		{
			root->vertices[6] = this->makeVertex(max, 0);

			this->vertices.push_back(root->vertices[6]);
		}

		if (!root->vertices[7])
		{
			root->vertices[7] = this->makeVertex(Data::Point3D(min.getX(), max.getY(), max.getZ()), 0);

			this->vertices.push_back(root->vertices[7]);
		}

		if (root->parent())
		{
			for (UInt i = 0; i < 8; i++)
			{
				UInt parentPos = mapParent[i][pos];

				if (static_cast<VertexTree *>(root->parent())->vertices[parentPos])
				{
					continue;
				}

				static_cast<VertexTree *>(root->parent())->vertices[parentPos] = root->vertices[i];

				if ((parentPos < 8) || (parentPos == 50))
				{
					continue;
				}

				for (UInt side = 0; side < 6; side++)
				{
					for (UInt j = 0; j < 5; j++)
					{
						if (mapParentNeighbor[side][2*j+0] != parentPos)
						{
							continue;
						}

						const Data::BoxTreeCell *parent = static_cast<const VertexTree *>(root->parent())->cell;

						Data::BoxTreeCell *neigh = this->tree->neighbor(parent, side);

						if ((!neigh) || (neigh->level() != parent->level()))
						{
							continue;
						}

						VertexTree *neighRoot = this->mate(neigh);

						neighRoot->vertices[mapParentNeighbor[side][2*j+1]] = root->vertices[i];
					}
				}

				for (UInt j = 0; j < 12; j++)
				{
					if (parentPos != mapParentEdgeNeighbor[j][2])
					{
						continue;
					}

					Data::UIntVector sides;

					sides.push_back(mapParentEdgeNeighbor[j][0]);
					sides.push_back(mapParentEdgeNeighbor[j][1]);

					const Data::BoxTreeCell *parent = static_cast<const VertexTree *>(root->parent())->cell;

					Data::BoxTreeCell *neigh = this->tree->neighbor3D2(parent, sides);

					if ((!neigh) || (neigh->level() < parent->level() - 1))
					{
						continue;
					}

					VertexTree *neighRoot = this->mate(neigh);

					if (neigh->level() == parent->level())
					{
						neighRoot->vertices[mapParentEdgeNeighbor[j][3]] = root->vertices[i];
					}
					else if (parent->position() == mapParentEdgeNeighbor[j][4])
					{
						neighRoot->vertices[mapParentEdgeNeighbor[j][5]] = root->vertices[i];
					}
					else
					{
						neighRoot->vertices[mapParentEdgeNeighbor[j][6]] = root->vertices[i];
					}
				}
			}
		}
	}
}

void Performer::TemplateMeshGenerator::deleteExtraVertices()
{
	while (!this->vertices.empty())
	{
		Data::Vertex *v = this->vertices.front();

		this->vertices.pop_front();

		if (!this->getMesh()->have(v))
		{
			delete v;
		}
	}
}

Data::UIntList Performer::TemplateMeshGenerator::path(const Data::BoxTreeCell *cell) const
{
	Data::UIntList path;

	const Data::BoxTreeCell *p = cell;

	while (p->parent())
	{
		for (UInt i = 0; i < p->parent()->numChildren(); i++)
		{
			if (p->parent()->child(i) == p)
			{
				path.push_front(i);

				break;
			}
		}

		p = static_cast<Data::BoxTreeCell *>(p->parent());
	}

	return path;
}

Performer::TemplateMeshGenerator::VertexTree *Performer::TemplateMeshGenerator::mate(Data::UIntList path) const
{
	VertexTree *t = this->root;

	for (Data::UIntList::iterator iter = path.begin();
		 iter != path.end(); iter++)
	{
		t = static_cast<VertexTree *>(t->child((*iter)));
	}

	return t;
}

Performer::TemplateMeshGenerator::VertexTree *Performer::TemplateMeshGenerator::mate(const Data::BoxTreeCell *cell) const
{
	return this->mate(this->path(cell));
}

Data::Vertex *Performer::TemplateMeshGenerator::getVertex(const Data::BoxTreeCell *cell, UInt index)
{
	return this->getVertex(this->mate(cell), index);
}

Data::Vertex *Performer::TemplateMeshGenerator::getVertex(const VertexTree *root, UInt index)
{
	return root->vertices[index];
}

/*Data::Vertex *Performer::TemplateMeshGenerator::makeVertex(const Data::BoxTreeCell *cell, UInt index)
{
	return this->makeVertex(this->mate(cell), index);
}

Data::Vertex *Performer::TemplateMeshGenerator::makeVertex(VertexTree *root, UInt index)
{
	const Data::BoxTreeCell *cell = root->cell;

	Data::Vertex *v = NULL;

	if (cell->dimension() == 2)
	{
		Data::Point2D pt = this->point2D(cell, index);

		v = this->makeVertex(pt, this->idManager->next(0));
	}
	else
	{
		Data::Point3D pt = this->point3D(cell, index);

		v = this->makeVertex(pt, this->idManager->next(0));
	}

	this->setVertex(root, index, v);

	return v;
}

Data::Vertex *Performer::TemplateMeshGenerator::getOrMakeVertex(const Data::BoxTreeCell *cell, UInt index)
{
	return this->getOrMakeVertex(this->mate(cell), index);
}

Data::Vertex *Performer::TemplateMeshGenerator::getOrMakeVertex(VertexTree *root, UInt index)
{
	Data::Vertex *v = this->getVertex(root, index);

	return v ? v : this->makeVertex(root, index);
}

void Performer::TemplateMeshGenerator::setVertex(VertexTree *root, UInt index, Data::Vertex *v)
{
	const Data::BoxTreeCell *cell = root->cell;

	root->vertices[index] = v;

	if (cell->dimension() == 2)
	{
		switch (index)
		{
		case 0:
			{
				this->setAdjVertex2D(cell, 2, 1, 3, 5, 1, v);

				this->setAdjVertex2D(cell, 3, 3, 1, 6, 3, v);

				this->setAdjVertex2D(cell, 2, 3, 2, v);

				break;
			}
		case 1:
			{
				this->setAdjVertex2D(cell, 3, 2, 0, 6, 2, v);

				this->setAdjVertex2D(cell, 0, 0, 2, 7, 0, v);

				this->setAdjVertex2D(cell, 3, 0, 3, v);

				break;
			}
		case 2:
			{
				this->setAdjVertex2D(cell, 0, 3, 1, 7, 3, v);

				this->setAdjVertex2D(cell, 1, 1, 3, 4, 1, v);

				this->setAdjVertex2D(cell, 0, 1, 0, v);

				break;
			}
		case 3:
			{
				this->setAdjVertex2D(cell, 1, 0, 2, 4, 0, v);

				this->setAdjVertex2D(cell, 2, 2, 0, 5, 2, v);

				this->setAdjVertex2D(cell, 1, 2, 1, v);

				break;
			}
		case 4:
			{
				this->setAdjsVertex2D(cell, 3, 2, 3, 3, 2, v);

				break;
			}
		case 5:
			{
				this->setAdjsVertex2D(cell, 0, 3, 0, 0, 3, v);

				break;
			}
		case 6:
			{
				this->setAdjsVertex2D(cell, 1, 0, 1, 1, 0, v);

				break;
			}
		case 7:
			{
				this->setAdjsVertex2D(cell, 2, 1, 2, 2, 1, v);

				break;
			}
		}
	}
	else
	{
		switch (index)
		{
		case 0:
			{
				this->setAdjVertex3D(cell, 3, 1, 3, 9, 4, 17, 7, 20, 1, v);

				this->setAdjVertex3D(cell, 4, 3, 1, 10, 4, 19, 5, 21, 3, v);

				this->setAdjVertex3D(cell, 5, 4, 1, 8, 2, 22, 3, 11, 4, v);

				this->setAdjVertex3D(cell, 3, 4, 2, 4, 18, 47, 46, 2, v);

				this->setAdjVertex3D(cell, 4, 5, 7, 1, 14, 39, 38, 7, v);

				this->setAdjVertex3D(cell, 5, 3, 5, 3, 11, 32, 33, 5, v);

				this->setAdjVertex3D(cell, 3, 4, 5, 6, v);

				break;
			}
		}
	}
}

void Performer::TemplateMeshGenerator::setAdjVertex2D(const Data::BoxTreeCell *cell,
	UInt side, UInt index1, UInt pos, UInt index2, UInt setPos, Data::Vertex *v) const
{
	Data::GraphNodeList neighs = cell->neighbors(side);

	if (neighs.size() == 1)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		VertexTree *t = this->mate(neigh);

		if (neigh->level() == cell->level())
		{
			t->vertices[index1] = v;
		}
		else if (cell->position() == pos)
		{
			t->vertices[index2] = v;
		}
		else
		{
			t->vertices[index1] = v;
		}
	}
	else if (neighs.size() == 2)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		if (neigh->position() != setPos)
		{
			neigh = static_cast<Data::BoxTreeCell *>(neighs.back());
		}

		VertexTree *t = this->mate(neigh);

		t->vertices[index1] = v;
	}
}

void Performer::TemplateMeshGenerator::setAdjVertex2D(const Data::BoxTreeCell *cell,
	UInt side1, UInt side2, UInt index, Data::Vertex *v) const
{
	Data::UIntVector sides;

	sides.push_back(side1);
	sides.push_back(side2);

	Data::GraphNodeList neighs = this->tree->neighbors(cell, sides);

	if (neighs.size() == 1)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		VertexTree *t = this->mate(neigh);

		t->vertices[index] = v;
	}
}

void Performer::TemplateMeshGenerator::setAdjsVertex2D(const Data::BoxTreeCell *cell, UInt side, UInt pos1, UInt index1, UInt pos2, UInt index2, Data::Vertex *v) const
{
	Data::GraphNodeList neighs = cell->neighbors(side);

	const Data::BoxTreeCell *neigh1 = static_cast<Data::BoxTreeCell *>(neighs.front());
	const Data::BoxTreeCell *neigh2 = static_cast<Data::BoxTreeCell *>(neighs.back());

	if (neigh1->position() == pos2)
	{
		std::swap(neigh1, neigh2);
	}

	VertexTree *t1 = this->mate(neigh1);
	VertexTree *t2 = this->mate(neigh2);

	t1->vertices[index1] = v;
	t2->vertices[index2] = v;
}

void Performer::TemplateMeshGenerator::setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side, UInt index1,
	UInt pos2, UInt index2, UInt pos3, UInt index3, UInt pos4, UInt index4, UInt setPos, Data::Vertex *v) const
{
	Data::GraphNodeList neighs = cell->neighbors(side);

	if (neighs.size() == 1)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		VertexTree *t = this->mate(neigh);

		if (neigh->level() == cell->level())
		{
			t->vertices[index1] = v;
		}
		else if (cell->position() == pos2)
		{
			t->vertices[index2] = v;
		}
		else if (cell->position() == pos3)
		{
			t->vertices[index3] = v;
		}
		else if (cell->position() == pos4)
		{
			t->vertices[index4] = v;
		}
		else
		{
			t->vertices[index1] = v;
		}
	}
	else if (neighs.size() == 4)
	{
		const Data::BoxTreeCell *neigh = NULL;

		for (Data::GraphNodeList::iterator iter = neighs.begin();
			 iter != neighs.end(); iter++)
		{
			if (static_cast<Data::BoxTreeCell *>((*iter))->position() == setPos)
			{
				neigh = static_cast<Data::BoxTreeCell *>((*iter));

				break;
			}
		}

		VertexTree *t = this->mate(neigh);

		t->vertices[index1] = v;
	}
}

void Performer::TemplateMeshGenerator::setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side1, UInt side2, UInt index1,
	UInt pos2, UInt index2, UInt index3, UInt index4, UInt setPos, Data::Vertex *v) const
{
	Data::UIntVector sides;

	sides.push_back(side1);
	sides.push_back(side2);

	Data::GraphNodeList neighs = this->tree->neighbors(cell, sides);

	if (neighs.size() == 1)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		VertexTree *t = this->mate(neigh);

		if (neigh->level() == cell->level())
		{
			t->vertices[index1] = v;
		}
		else if (neigh->level() + 1 == cell->level())
		{
			if (cell->position() == pos2)
			{
				t->vertices[index2] = v;
			}
			else
			{
				t->vertices[index1] = v;
			}
		}
		else
		{
			if (static_cast<Data::BoxTreeCell *>(cell->parent())->position() == pos2)
			{
				if (cell->position() == pos2)
				{
					t->vertices[index3] = v;
				}
				else
				{
					t->vertices[index2] = v;
				}
			}
			else
			{
				if (cell->position() == pos2)
				{
					t->vertices[index4] = v;
				}
				else
				{
					t->vertices[index1] = v;
				}
			}
		}
	}
	else if (neighs.size() == 2)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		if (neigh->position() != setPos)
		{
			neigh = static_cast<Data::BoxTreeCell *>(neighs.back());
		}

		VertexTree *t = this->mate(neigh);

		t->vertices[index1] = v;
	}
	else if (neighs.size() == 3)
	{
		const Data::BoxTreeCell *neigh = NULL;

		for (Data::GraphNodeList::iterator iter = neighs.begin();
			 iter != neighs.end(); iter++)
		{
			Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

			if (((n->level() + 1 == cell->level()) || (static_cast<Data::BoxTreeCell *>(n->parent())->position() == setPos)) &&
				(n->position() == setPos))
			{
				neigh = static_cast<Data::BoxTreeCell *>((*iter));

				break;
			}
		}

		VertexTree *t = this->mate(neigh);

		t->vertices[index1] = v;
	}
	else if (neighs.size() == 4)
	{
		const Data::BoxTreeCell *neigh = NULL;

		for (Data::GraphNodeList::iterator iter = neighs.begin();
			 iter != neighs.end(); iter++)
		{
			Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

			if ((static_cast<Data::BoxTreeCell *>(n->parent())->position() == setPos) &&
				(n->position() == setPos))
			{
				neigh = static_cast<Data::BoxTreeCell *>((*iter));

				break;
			}
		}

		VertexTree *t = this->mate(neigh);

		t->vertices[index1] = v;
	}
}

void Performer::TemplateMeshGenerator::setAdjVertex3D(const Data::BoxTreeCell *cell, UInt side1, UInt side2, UInt side3, UInt index, Data::Vertex *v) const
{
	Data::UIntVector sides;

	sides.push_back(side1);
	sides.push_back(side2);
	sides.push_back(side3);

	Data::GraphNodeList neighs = this->tree->neighbors(cell, sides);

	if (neighs.size() == 1)
	{
		const Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

		VertexTree *t = this->mate(neigh);

		t->vertices[index] = v;
	}
}

Data::Point2D Performer::TemplateMeshGenerator::point2D(const Data::BoxTreeCell *cell, UInt index) const
{
	Data::Point2D min(cell->box()->getMin());
	Data::Point2D max(cell->box()->getMax());

	switch (index)
	{
	case 0:
		return min;

	case 1:
		return Data::Point2D(max.getX(), min.getY());

	case 2:
		return max;

	case 3:
		return Data::Point2D(min.getX(), max.getY());

	case 4:
		return Data::Point2D((min.getX() + max.getX())*0.5, min.getY());

	case 5:
		return Data::Point2D(max.getX(), (min.getY() + max.getY())*0.5);

	case 6:
		return Data::Point2D((min.getX() + max.getX())*0.5, max.getY());

	case 7:
		return Data::Point2D(min.getX(), (min.getY() + max.getY())*0.5);
	}

	return Data::Point2D((min.getX() + max.getX())*0.5, (min.getY() + max.getY())*0.5);
}

Data::Point3D Performer::TemplateMeshGenerator::point3D(const Data::BoxTreeCell *cell, UInt index) const
{
	Data::Point3D min(cell->box()->getMin());
	Data::Point3D max(cell->box()->getMax());

	switch (index)
	{
	case 0:
		return min;

	case 1:
		return Data::Point3D(max.getX(), min.getY(), min.getZ());

	case 2:
		return Data::Point3D(max.getX(), max.getY(), min.getZ());

	case 3:
		return Data::Point3D(min.getX(), max.getY(), min.getZ());

	case 4:
		return Data::Point3D(min.getX(), min.getY(), max.getZ());

	case 5:
		return Data::Point3D(max.getX(), min.getY(), max.getZ());

	case 6:
		return max;

	case 7:
		return Data::Point3D(min.getX(), max.getY(), max.getZ());

	case 8:
		return Data::Point3D((min.getX() + max.getX())*0.5, min.getY(), min.getZ());

	case 9:
		return Data::Point3D(max.getX(), (min.getY() + max.getY())*0.5, min.getZ());

	case 10:
		return Data::Point3D((min.getX() + max.getX())*0.5, max.getY(), min.getZ());

	case 11:
		return Data::Point3D(min.getX(), (min.getY() + max.getY())*0.5, min.getZ());

	case 12:
		return Data::Point3D((min.getX() + max.getX())*0.5, min.getY(), max.getZ());

	case 13:
		return Data::Point3D(max.getX(), (min.getY() + max.getY())*0.5, max.getZ());

	case 14:
		return Data::Point3D((min.getX() + max.getX())*0.5, max.getY(), max.getZ());

	case 15:
		return Data::Point3D(min.getX(), (min.getY() + max.getY())*0.5, max.getZ());

	case 16:
		return Data::Point3D(min.getX(), min.getY(), (min.getZ() + max.getZ())*0.5);

	case 17:
		return Data::Point3D(max.getX(), min.getY(), (min.getZ() + max.getZ())*0.5);

	case 18:
		return Data::Point3D(max.getX(), max.getY(), (min.getZ() + max.getZ())*0.5);

	case 19:
		return Data::Point3D(min.getX(), max.getY(), (min.getZ() + max.getZ())*0.5);

	case 20:
		return Data::Point3D(max.getX(), (min.getY() + max.getY())*0.5, (min.getZ() + max.getZ())*0.5);

	case 21:
		return Data::Point3D((min.getX() + max.getX())*0.5, max.getY(), (min.getZ() + max.getZ())*0.5);

	case 22:
		return Data::Point3D((min.getX() + max.getX())*0.5, (min.getY() + max.getY())*0.5, max.getZ());

	case 23:
		return Data::Point3D(min.getX(), (min.getY() + max.getY())*0.5, (min.getZ() + max.getZ())*0.5);

	case 24:
		return Data::Point3D((min.getX() + max.getX())*0.5, min.getY(), (min.getZ() + max.getZ())*0.5);

	case 25:
		return Data::Point3D((min.getX() + max.getX())*0.5, (min.getY() + max.getY())*0.5, min.getZ());

	case 26:
		return Data::Point3D(min.getX()*0.75 + max.getX()*0.25, min.getY(), min.getZ());

	case 27:
		return Data::Point3D(min.getX()*0.25 + max.getX()*0.75, min.getY(), min.getZ());

	case 28:
		return Data::Point3D(max.getX(), min.getY()*0.75 + max.getY()*0.25, min.getZ());

	case 29:
		return Data::Point3D(max.getX(), min.getY()*0.25 + max.getY()*0.75, min.getZ());

	case 30:
		return Data::Point3D(min.getX()*0.25 + max.getX()*0.75, max.getY(), min.getZ());

	case 31:
		return Data::Point3D(min.getX()*0.75 + max.getX()*0.25, max.getY(), min.getZ());

	case 32:
		return Data::Point3D(min.getX(), min.getY()*0.25 + max.getY()*0.75, min.getZ());

	case 33:
		return Data::Point3D(min.getX(), min.getY()*0.75 + max.getY()*0.25, min.getZ());

	case 34:
		return Data::Point3D(min.getX()*0.75 + max.getX()*0.25, min.getY(), max.getZ());

	case 35:
		return Data::Point3D(min.getX()*0.25 + max.getX()*0.75, min.getY(), max.getZ());

	case 36:
		return Data::Point3D(max.getX(), min.getY()*0.75 + max.getY()*0.25, max.getZ());

	case 37:
		return Data::Point3D(max.getX(), min.getY()*0.25 + max.getY()*0.75, max.getZ());

	case 38:
		return Data::Point3D(min.getX()*0.25 + max.getX()*0.75, max.getY(), max.getZ());

	case 39:
		return Data::Point3D(min.getX()*0.75 + max.getX()*0.25, max.getY(), max.getZ());

	case 40:
		return Data::Point3D(min.getX(), min.getY()*0.25 + max.getY()*0.75, max.getZ());

	case 41:
		return Data::Point3D(min.getX(), min.getY()*0.75 + max.getY()*0.25, max.getZ());

	case 42:
		return Data::Point3D(min.getX(), min.getY(), min.getZ()*0.75 + max.getZ()*0.25);

	case 43:
		return Data::Point3D(min.getX(), min.getY(), min.getZ()*0.25 + max.getZ()*0.75);

	case 44:
		return Data::Point3D(max.getX(), min.getY(), min.getZ()*0.75 + max.getZ()*0.25);

	case 45:
		return Data::Point3D(max.getX(), min.getY(), min.getZ()*0.25 + max.getZ()*0.75);

	case 46:
		return Data::Point3D(max.getX(), max.getY(), min.getZ()*0.75 + max.getZ()*0.25);

	case 47:
		return Data::Point3D(max.getX(), max.getY(), min.getZ()*0.25 + max.getZ()*0.75);

	case 48:
		return Data::Point3D(min.getX(), max.getY(), min.getZ()*0.75 + max.getZ()*0.25);

	case 49:
		return Data::Point3D(min.getX(), max.getY(), min.getZ()*0.25 + max.getZ()*0.75);
	}

	return Data::Point3D((min.getX() + max.getX())*0.5, (min.getY() + max.getY())*0.5, (min.getZ() + max.getZ())*0.5);
}*/

#if (!USE_EXPERIMENTAL_MESH)
Data::Mesh *Performer::TemplateMeshGenerator::makeMesh() const
{
	return new Data::Mesh();
}
#endif //#if (!USE_EXPERIMENTAL_MESH)
