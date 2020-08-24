#include "MJMesh/QuadTree.h"

#include "MJMesh/Edge2D.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/QuadTreeCell.h"

MJMesh::QuadTree::QuadTree(const Data::Point2D &min, const Data::Point2D &max, ULInt id) :
	//Data::QuadTree(min, max, id)
	Data::QuadTree(new Data::Box(min, max), new MJMesh::QuadTreeCell(NULL, 0), id)
{
	/*this->nodes.clear();

	delete this->getRoot();

	this->setRoot(new MJMesh::QuadTreeCell(NULL, 0));*/
}

MJMesh::QuadTree::QuadTree(Data::Box *box, MJMesh::QuadTreeCell *root, ULInt id) :
	Data::QuadTree(box, root, id)
{

}

MJMesh::QuadTree::~QuadTree()
{

}

MJMesh::QuadTreeCell *MJMesh::QuadTree::find(const MJMesh::FrontElement2D *e) const
{
#if USE_EXPERIMENTAL_3
	return static_cast<MJMesh::QuadTreeCell *>(this->find(static_cast<MJMesh::Edge2D *>(e->getShape())->mid2D()));
#else
	return static_cast<MJMesh::QuadTreeCell *>(this->find(static_cast<MJMesh::Edge2D *>(e->getShape())->mid()));
#endif //#if USE_EXPERIMENTAL_3
}

Data::BoxTreeCell *MJMesh::QuadTree::find(const MJMesh::FrontElement *e) const
{
	return static_cast<Data::BoxTreeCell *>(this->find(static_cast<const MJMesh::FrontElement2D *>(e)));
}

/*Data::GraphNodeVector MJMesh::QuadTree::neighbors(const Data::BoxTreeCell *c, UInt side) const
{
	//debug
	return Data::BoxTree::neighbors(c, side);
	//endebug

	return ((MJMesh::QuadTreeCell *)c)->getNeighbors(side);
}

Data::GraphNodeVector MJMesh::QuadTree::neighbors(const Data::BoxTreeCell *c) const
{
	//debug
	return Data::BoxTree::neighbors(c);
	//endebug

	return ((MJMesh::QuadTreeCell *)c)->getNeighbors();
}*/
