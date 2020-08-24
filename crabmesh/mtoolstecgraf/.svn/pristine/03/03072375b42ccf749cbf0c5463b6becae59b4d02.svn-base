#include "Data/VertexBoxSearchTree.h"

#include "Data/VertexBoxSearchTreeCell.h"

using namespace Data;

#if USE_EXPERIMENTAL_5

Data::VertexBoxSearchTree2D::VertexBoxSearchTree2D(const BoxTree *tree,
	VertexBoxSearchTreeCell2D *root, ULInt id) :
		BoxSearchTree2D(tree, root, id)
{

}

Data::VertexBoxSearchTree2D::~VertexBoxSearchTree2D()
{

}

BoxSearchTreeCell *Data::VertexBoxSearchTree2D::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new VertexBoxSearchTreeCell2D(c, static_cast<VertexBoxSearchTreeCell2D *>(parent), id);
}

Data::VertexBoxSearchTree3D::VertexBoxSearchTree3D(const BoxTree *tree,
	VertexBoxSearchTreeCell3D *root, ULInt id) :
		BoxSearchTree3D(tree, root, id)
{

}

Data::VertexBoxSearchTree3D::~VertexBoxSearchTree3D()
{

}

BoxSearchTreeCell *Data::VertexBoxSearchTree3D::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new VertexBoxSearchTreeCell3D(c, static_cast<VertexBoxSearchTreeCell3D *>(parent), id);
}

#else

Data::VertexBoxSearchTree::VertexBoxSearchTree(const BoxTree *tree,
	VertexBoxSearchTreeCell *root, ULInt id) :
		BoxSearchTree(tree, root, id)
{

}

Data::VertexBoxSearchTree::~VertexBoxSearchTree()
{

}

BoxSearchTreeCell *Data::VertexBoxSearchTree::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new VertexBoxSearchTreeCell(c, static_cast<VertexBoxSearchTreeCell *>(parent), id);
}

#endif //#if USE_EXPERIMENTAL_5
