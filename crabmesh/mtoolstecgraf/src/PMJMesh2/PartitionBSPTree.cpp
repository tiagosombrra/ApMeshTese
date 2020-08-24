#include "PMJMesh2/PartitionBSPTree.h"

#include "PMJMesh2/PartitionBSPTreeCell.h"
#include "Data/Box.h"

using namespace PMJMesh2;

PMJMesh2::PartitionBSPTree::PartitionBSPTree(const Data::Point &min, const Data::Point &max, ULInt id) :
	Data::BSPTree::BSPTree(new Data::Box(min, max), new PartitionBSPTreeCell(NULL, 0), id)
{

}

PMJMesh2::PartitionBSPTree::PartitionBSPTree(Data::Box *box, PartitionBSPTreeCell *root, ULInt id) :
	Data::BSPTree::BSPTree(box, root, id)
{

}

PMJMesh2::PartitionBSPTree::~PartitionBSPTree()
{

}
