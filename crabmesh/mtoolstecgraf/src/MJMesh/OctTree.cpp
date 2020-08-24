#include "MJMesh/OctTree.h"

#include "Data/Box.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/OctTreeCell.h"

MJMesh::OctTree::OctTree(const Data::Point3D &min, const Data::Point3D &max, ULInt id) :
	//Data::OctTree(min, max, id)
	Data::OctTree(new Data::Box(min, max), new MJMesh::OctTreeCell(NULL, 0), id)
{
	/*this->nodes.clear();

	delete this->getRoot();

	this->setRoot(new MJMesh::OctTreeCell(NULL, 0));*/
}

MJMesh::OctTree::OctTree(Data::Box *box, MJMesh::OctTreeCell *root, ULInt id) :
	Data::OctTree(box, root, id)
{

}

MJMesh::OctTree::~OctTree()
{

}

MJMesh::OctTreeCell *MJMesh::OctTree::find(const MJMesh::FrontElement3D *e) const
{
#if USE_EXPERIMENTAL_3
	return static_cast<MJMesh::OctTreeCell *>(this->find(static_cast<MJMesh::Triangle3D *>(e->getShape())->mid3D()));
#else
	return static_cast<MJMesh::OctTreeCell *>(this->find(static_cast<MJMesh::Triangle3D *>(e->getShape())->mid()));
#endif //#if USE_EXPERIMENTAL_3
}

Data::BoxTreeCell *MJMesh::OctTree::find(const MJMesh::FrontElement *e) const
{
	return static_cast<Data::BoxTreeCell *>(this->find(static_cast<const MJMesh::FrontElement3D *>(e)));
}

/*Data::GraphNodeVector MJMesh::OctTree::neighbors(const Data::BoxTreeCell *c, UInt side) const
{
	return ((MJMesh::OctTreeCell *)c)->getNeighbors(side);
}

Data::GraphNodeVector MJMesh::OctTree::neighbors(const Data::BoxTreeCell *c) const
{
	return ((MJMesh::OctTreeCell *)c)->getNeighbors();
}*/
