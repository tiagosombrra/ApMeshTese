#include "PMJMesh/OctTree.h"

#include "Data/Box.h"
#include "PMJMesh/OctTreeCell.h"

PMJMesh::OctTree::OctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id) :
	MJMesh::OctTree(new Data::Box(min, max), new PMJMesh::OctTreeCell(NULL, 0), id)
{

}

PMJMesh::OctTree::OctTree(Data::Box *box, PMJMesh::OctTreeCell *root, ULInt id) :
	MJMesh::OctTree::OctTree(box, root, id)
{

}

PMJMesh::OctTree::~OctTree()
{

}

Data::PositionMap PMJMesh::OctTree::positions(bool onlyLeaves) const
{
	Data::PositionMap positions;

	this->positions(positions, onlyLeaves);

	return positions;
}

void PMJMesh::OctTree::positions(Data::PositionMap &positions, bool onlyLeaves) const
{
	static_cast<PMJMesh::OctTreeCell *>(this->getRoot())->positions(positions, onlyLeaves);
}

void PMJMesh::OctTree::adjustPosition()
{
	static_cast<PMJMesh::OctTreeCell *>(this->getRoot())->adjustPositionFromChildren(true);
}
