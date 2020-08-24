#include "PMJMesh/VertexSearchTreeGenerator.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/VertexADTree.h"

PMJMesh::VertexSearchTreeGenerator::VertexSearchTreeGenerator(
	const MJMesh::Front *front, const Data::BoxTree *tree, const Data::Box *box) :
		MJMesh::VertexSearchTreeGenerator(front, tree)
{
	this->setBox(box);
}

PMJMesh::VertexSearchTreeGenerator::~VertexSearchTreeGenerator()
{
	//this->box = NULL;
}

void PMJMesh::VertexSearchTreeGenerator::setBox(const Data::Box *box)
{
	this->box = box;
}

Data::GeometricSearchTree *PMJMesh::VertexSearchTreeGenerator::makeTree() const
{
#if USE_ADT
    return new Data::VertexADTree();
#else
#if USE_EXPERIMENTAL_5
	return (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 2) ?
        static_cast<Data::GeometricSearchTree *>(new PMJMesh::VertexBoxSearchTree2D(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box)) :
        static_cast<Data::GeometricSearchTree *>(new PMJMesh::VertexBoxSearchTree3D(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box));
#else
	return new PMJMesh::VertexBoxSearchTree(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box);
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_ADT
}
