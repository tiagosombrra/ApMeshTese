#include "PMJMesh/QuadTree.h"

#include "Data/Box.h"
#include "PMJMesh/QuadTreeCell.h"

PMJMesh::QuadTree::QuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id) :
	MJMesh::QuadTree(new Data::Box(min, max), new PMJMesh::QuadTreeCell(NULL, 0), id)
{

}

PMJMesh::QuadTree::QuadTree(Data::Box *box, PMJMesh::QuadTreeCell *root, ULInt id) :
	MJMesh::QuadTree::QuadTree(box, root, id)
{

}

PMJMesh::QuadTree::~QuadTree()
{

}

Data::PositionMap PMJMesh::QuadTree::positions(bool onlyLeaves) const
{
	Data::PositionMap positions;

	this->positions(positions, onlyLeaves);

	return positions;
}

void PMJMesh::QuadTree::positions(Data::PositionMap &positions, bool onlyLeaves) const
{
	static_cast<PMJMesh::QuadTreeCell *>(this->getRoot())->positions(positions, onlyLeaves);
}

void PMJMesh::QuadTree::adjustPosition()
{
	static_cast<PMJMesh::QuadTreeCell *>(this->getRoot())->adjustPositionFromChildren(true);
}

#if USE_GUI
std::string PMJMesh::QuadTree::text() const
{
	std::stringstream str;

	str << "quadtree " << this->getId() << std::endl;

	Data::GraphNodeList cells;

	cells.push_back(this->getRoot());

	while (!cells.empty())
	{
		PMJMesh::QuadTreeCell *cell = static_cast<PMJMesh::QuadTreeCell *>(cells.front());

		cells.pop_front();

		str << "cell " << cell->getId() << ", position = " << cell->getPosition() << std::endl;

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			cells.push_back(cell->child(i));
		}
	}

	return str.str();
}
#endif //#if USE_GUI
