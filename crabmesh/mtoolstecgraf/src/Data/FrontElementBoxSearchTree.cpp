#include "Data/FrontElementBoxSearchTree.h"

using namespace Data;

#if USE_EXPERIMENTAL_5

Data::FrontElementBoxSearchTree2D::FrontElementBoxSearchTree2D(const BoxTree *tree,
	FrontElementBoxSearchTreeCell2D *root, ULInt id)
	: BoxSearchTree2D(tree, root, id)
{

}

Data::FrontElementBoxSearchTree2D::~FrontElementBoxSearchTree2D()
{

}

BoxSearchTreeCell *Data::FrontElementBoxSearchTree2D::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new FrontElementBoxSearchTreeCell2D(c, static_cast<FrontElementBoxSearchTreeCell2D *>(parent), id);
}

Data::FrontElementBoxSearchTree3D::FrontElementBoxSearchTree3D(const BoxTree *tree,
	FrontElementBoxSearchTreeCell3D *root, ULInt id)
	: BoxSearchTree3D(tree, root, id)
{

}

Data::FrontElementBoxSearchTree3D::~FrontElementBoxSearchTree3D()
{

}

BoxSearchTreeCell *Data::FrontElementBoxSearchTree3D::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new FrontElementBoxSearchTreeCell3D(c, static_cast<FrontElementBoxSearchTreeCell3D *>(parent), id);
}

#else

Data::FrontElementBoxSearchTree::FrontElementBoxSearchTree(const BoxTree *tree,
	FrontElementBoxSearchTreeCell *root, ULInt id)
	: BoxSearchTree(tree, root, id)
{

}

Data::FrontElementBoxSearchTree::~FrontElementBoxSearchTree()
{

}

BoxSearchTreeCell *Data::FrontElementBoxSearchTree::makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const
{
	return new FrontElementBoxSearchTreeCell(c, static_cast<FrontElementBoxSearchTreeCell *>(parent), id);
}

#endif //#if USE_EXPERIMENTAL_5
