#include "PMJMesh/SecondaryPartitionBuilder.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/BoxTree.h"
#include "Data/Box.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTreeCell.h"

PMJMesh::SecondaryPartitionBuilder::SecondaryPartitionBuilder(UInt dimension,
	Data::BoxTree *partitionTree, bool face, bool edge, bool vertex) :
	Performer::Builder()
{
	this->setDimension(dimension);

	this->setPartitionTree(partitionTree);

	this->setFace(face);
	this->setEdge(edge);
	this->setVertex(vertex);
}

PMJMesh::SecondaryPartitionBuilder::~SecondaryPartitionBuilder()
{
	/*this->partitionTree = NULL;

	this->faceBoxes.clear();
	this->edgeBoxes.clear();
	this->vertexBoxes.clear();*/
}

void PMJMesh::SecondaryPartitionBuilder::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::SecondaryPartitionBuilder::setPartitionTree(Data::BoxTree *partitionTree)
{
	this->partitionTree = partitionTree;
}

void PMJMesh::SecondaryPartitionBuilder::setFace(bool face)
{
	this->face = face;
}

void PMJMesh::SecondaryPartitionBuilder::setEdge(bool edge)
{
	this->edge = edge;
}

void PMJMesh::SecondaryPartitionBuilder::setVertex(bool vertex)
{
	this->vertex = vertex;
}

Data::BoxList PMJMesh::SecondaryPartitionBuilder::getFaceBoxes() const
{
	return this->faceBoxes;
}

Data::BoxList PMJMesh::SecondaryPartitionBuilder::getEdgeBoxes() const
{
	return this->edgeBoxes;
}

Data::BoxList PMJMesh::SecondaryPartitionBuilder::getVertexBoxes() const
{
	return this->vertexBoxes;
}

Data::BoxList PMJMesh::SecondaryPartitionBuilder::getSecondaryPartitions() const
{
	Data::BoxList boxes = this->faceBoxes;

	boxes.insert(boxes.end(), this->edgeBoxes.begin(), this->edgeBoxes.end());
	boxes.insert(boxes.end(), this->vertexBoxes.begin(), this->vertexBoxes.end());

	return boxes;
}

void PMJMesh::SecondaryPartitionBuilder::execute()
{
	if (this->face)
	{
		this->makeFaceBoxes();
	}

	if (this->edge)
	{
		this->makeEdgeBoxes();
	}

	if (this->vertex)
	{
		this->makeVertexBoxes();
	}
}

Data::Box *PMJMesh::SecondaryPartitionBuilder::makeBox(const Data::Point &min, const Data::Point &max) const
{
	Data::Point *bmin = (this->dimension == 2) ? static_cast<Data::Point *>(new Data::Point2D(min)) : static_cast<Data::Point *>(new Data::Point3D(min));
	Data::Point *bmax = (this->dimension == 2) ? static_cast<Data::Point *>(new Data::Point2D(max)) : static_cast<Data::Point *>(new Data::Point3D(max));

	Data::Box *b = new Data::Box(bmin, bmax);

	return b;
}

Data::Box *PMJMesh::SecondaryPartitionBuilder::makeFaceBox(UInt coord, const Data::BoxTreeCell *c, bool greater) const
{
	UInt i = greater ? coord : coord + this->dimension;

	Data::GraphNodeList neighbors = c->neighbors(i);
	/*Data::GraphNodeList neighbors = (this->dimension == 2) ?
		((MJMesh::QuadTreeCell *)c)->getNeighbors(i) :
		((MJMesh::OctTreeCell *)c)->getNeighbors(i);*/

	if (neighbors.empty())
	{
		return NULL;
	}

	//Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors[0]);
	Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors.front());

	if (neighbors.size() == 1)
	{
		if ((!greater) || (c->level() != n->level()))
		{
			return NULL;
		}
	}

	bool noNeighbor = true;

	//for (UInt j = 0; j < neighbors.size(); j++)
	for (Data::GraphNodeList::iterator iter = neighbors.begin();
		 iter != neighbors.end(); iter++)
	{
		//if (neighbors[j]->getId() > 0)
		if ((*iter)->getId() > 0)
		{
			noNeighbor = false;

			break;
		}
	}

	if (noNeighbor)
	{
		return NULL;
	}

#if USE_EXPERIMENTAL_3
	Data::Box *b = (this->dimension == 2) ?
        this->makeBox(c->min2D(), c->max2D()) :
        this->makeBox(c->min3D(), c->max3D());
#else
	Data::Box *b = this->makeBox(c->min(), c->max());
#endif //#if USE_EXPERIMENTAL_3

	const Data::Box *b1 = (greater) ? c->box() : n->box();
	const Data::Box *b2 = (greater) ? n->box() : c->box();

	b->getMin()->setCoord(coord, 0.5*(b1->getMin()->getCoord(coord) + b1->getMax()->getCoord(coord)));
	b->getMax()->setCoord(coord, 0.5*(b2->getMin()->getCoord(coord) + b2->getMax()->getCoord(coord)));

#if USE_GUI
	if (coord == 0)
	{
		b->setColor(0.0, 1.0, 1.0);
	}
	else if (coord == 1)
	{
		b->setColor(1.0, 0.0, 1.0);
	}
	else if (coord == 2)
	{
		b->setColor(1.0, 1.0, 0.0);
	}

	b->highlight();
#endif //#if USE_GUI

	return b;
}

Data::Box *PMJMesh::SecondaryPartitionBuilder::makeEdgeBox(UInt coord, const Data::BoxTreeCell *c) const
{
	if (coord != 2) return NULL;

	if (this->dimension != 2) return NULL;

	//in two dimensions, for example, it has to search for:
	//
	//	the left neighbors of a cell
	//	the bottom neighbors of a cell
	//	the bottom neighbors of the left neighbors of a cell

	UInt search[2];

	switch (this->dimension)
	{
	case 2:
		switch (coord)
		{
		case 2:
			search[0] = 2;
			search[1] = 3;
			break;
		default:
			return NULL;
		}
		break;

	case 3:
		switch (coord)
		{
		case 0:
			search[0] = 4;
			search[1] = 5;
			break;
		case 1:
			search[0] = 3;
			search[1] = 5;
			break;
		case 2:
			search[0] = 3;
			search[1] = 4;
			break;
		default:
			return NULL;
		}
		break;

	default:
		return NULL;
	}

#if USE_EXPERIMENTAL_3
	Data::Point2D cmin2d;
	Data::Point3D cmin3d;

	Data::Point &cmin = (this->dimension == 2) ?
        (cmin2d = c->min2D(), static_cast<Data::Point &>(cmin2d)) :
        (cmin3d = c->min3D(), static_cast<Data::Point &>(cmin3d));
#else
	Data::Point cmin = c->min();
#endif //#if USE_EXPERIMENTAL_3

	Data::GraphNodeVector cells;

	cells.push_back(const_cast<Data::BoxTreeCell *>(c));

	//find the neighbor cells that share the min point
	for (UInt i = 0; i < 2; i++)
	{
		/*Data::GraphNodeList neighbors = (this->dimension == 2) ?
			((MJMesh::QuadTreeCell *)c)->getNeighbors(search[i]) :
			((MJMesh::OctTreeCell *)c)->getNeighbors(search[i]);*/
		Data::GraphNodeList neighbors = c->neighbors(search[i]);

		while (!neighbors.empty())
		{
			//Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors[0]);
			Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors.front());

			//neighbors.erase(neighbors.begin());
			neighbors.pop_front();

			//if (n->on(cmin))
			if (n->optOn(cmin))
			{
				cells.push_back(n);
			}
		}
	}

	//if there is no neighbor or there is only one neighbor, return, because
	// there is no secondary partition to be built there TODO: generalize for
	// more than 2 dimensions
	if (cells.size() != 3)
	{
		return NULL;
	}

	//if one of the neighbors is a neighbor of the other, then it is the case
	// where the point is shared by only three cells, meaning that the secondary
	// partition may not be built because this case is handled by the face boxes

	Data::GraphNodeList neighbors = static_cast<Data::BoxTreeCell *>(cells[1])->neighbors(search[1]);
	/*Data::GraphNodeList neighbors = (this->dimension == 2) ?
		((MJMesh::QuadTreeCell *)cells[1])->getNeighbors(search[1]) :
		((MJMesh::OctTreeCell *)cells[1])->getNeighbors(search[1]);*/

	while (!neighbors.empty())
	{
		//Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors[0]);
		Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors.front());

		//neighbors.erase(neighbors.begin());
		neighbors.pop_front();

		if (n == cells[2])
		{
			return NULL;
		}

		//if (n->on(cmin))
		if (n->optOn(cmin))
		{
			cells.push_back(n);
		}
	}

	neighbors = static_cast<Data::BoxTreeCell *>(cells[2])->neighbors(search[0]);
	/*neighbors = (this->dimension == 2) ?
		((MJMesh::QuadTreeCell *)cells[2])->getNeighbors(search[0]) :
		((MJMesh::OctTreeCell *)cells[2])->getNeighbors(search[0]);*/

	while (!neighbors.empty())
	{
		//Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors[0]);
		Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(neighbors.front());

		//neighbors.erase(neighbors.begin());
		neighbors.pop_front();

		if (n == cells[1])
		{
			return NULL;
		}
	}

	if (cells.size() != 4)
	{
		std::cout << "ERROR: debug something is wrong... secondarypartitionbuilder.cpp" << std::endl;

		return NULL;
	}

	Data::GraphNodeVector auxCells;

	for (UInt i = 0; i < cells.size(); i++)
	{
		if (cells[i]->getId() > 0)
		{
			auxCells.push_back(cells[i]);
		}
	}

	cells = auxCells;

	//only build secondary partition if there are 4 or more partitions
	if (cells.size() < 4)
	{
		return NULL;
	}

	auxCells.clear();

	for (UInt i = 1; i < cells.size(); i++)
	{
		Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>(cells[i]);

		if (n->level() > c->level())
		{
			c = n;
		}
	}

	//c is now the cell with maximum level, that is, the smallest cell involved
	// in this procedure

#if USE_EXPERIMENTAL_3
    Data::Point2D min2d, max2d, mid2d;
    Data::Point3D min3d, max3d, mid3d;

    Data::Point &min = (this->dimension == 2) ?
        (min2d = c->min2D(), static_cast<Data::Point &>(min2d)) :
        (min3d = c->min3D(), static_cast<Data::Point &>(min3d));
    Data::Point &max = (this->dimension == 2) ?
        (max2d = c->max2D(), static_cast<Data::Point &>(max2d)) :
        (max3d = c->max3D(), static_cast<Data::Point &>(max3d));
    Data::Point &mid = (this->dimension == 2) ?
        (mid2d = c->mid2D(), static_cast<Data::Point &>(mid2d)) :
        (mid3d = c->mid3D(), static_cast<Data::Point &>(mid3d));

    Real size = max.getCoord(0) - min.getCoord(0);

	Data::Box *b = this->makeBox(min, max);
#else
	Real size = c->max().getCoord(0) - c->min().getCoord(0);
	Data::Point mid = c->mid();

	Data::Box *b = this->makeBox(c->min(), c->max());
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < this->dimension; i++)
	{
		Real min, max;
		min = max = 0.0;

		if (mid.getCoord(i) < cmin.getCoord(i))
		{
			min = mid.getCoord(i);
			max = mid.getCoord(i) + size;
		}
		else
		{
			min = mid.getCoord(i) - size;
			max = mid.getCoord(i);
		}

		b->getMin()->setCoord(i, min);
		b->getMax()->setCoord(i, max);
	}

#if USE_GUI
	if (coord == 0)
	{
		b->setColor(0.0, 0.5, 0.5);
	}
	else if (coord == 1)
	{
		b->setColor(0.5, 0.0, 0.5);
	}
	else if (coord == 2)
	{
		b->setColor(0.5, 0.5, 0.0);
	}

	b->highlight();
#endif //#if USE_GUI

	return b;
}

void PMJMesh::SecondaryPartitionBuilder::makeFaceBoxes()
{
	//Data::GraphNodeList leaves = this->partitionTree->getLeaves();

	//while (!leaves.empty())
	for (Data::Tree::LeafIterator iter = this->partitionTree->lBegin(), end = this->partitionTree->lEnd();
         iter != end; iter++)
	{
		//Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(leaves.front());
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		//leaves.pop_front();

		if (c->getId() <= 0)
		{
			continue;
		}

		for (UInt i = 0; i < this->dimension; i++)
		//for (UInt i = 1; i < 2; i++)
		{
			Data::Box *b = this->makeFaceBox(i, c, true);

			if (b)
			{
				this->faceBoxes.push_back(b);
			}

			b = this->makeFaceBox(i, c, false);

			if (b)
			{
				this->faceBoxes.push_back(b);
			}
		}
	}
}

void PMJMesh::SecondaryPartitionBuilder::makeEdgeBoxes()
{
	//Data::GraphNodeList leaves = this->partitionTree->getLeaves();

	//while (!leaves.empty())
	for (Data::Tree::LeafIterator iter = this->partitionTree->lBegin(), end = this->partitionTree->lEnd();
         iter != end; iter++)
	{
		//Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(leaves.front());
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		//leaves.pop_front();

		if (this->dimension == 2)
		{
			Data::Box *b = this->makeEdgeBox(2, c);

			if (b)
			{
				this->edgeBoxes.push_back(b);
			}

			continue;
		}

		for (UInt i = 0; i < 3; i++)
		{
			Data::Box *b = this->makeEdgeBox(i, c);

			if (b)
			{
				this->edgeBoxes.push_back(b);
			}
		}
	}
}

void PMJMesh::SecondaryPartitionBuilder::makeVertexBoxes()
{

}
