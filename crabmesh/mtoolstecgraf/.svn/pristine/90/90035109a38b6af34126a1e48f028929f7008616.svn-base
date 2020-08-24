#include "PMJMesh/FrontElementSearchTreeGenerator.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/FrontElementADTree.h"

PMJMesh::FrontElementSearchTreeGenerator::FrontElementSearchTreeGenerator(
	const MJMesh::Front *front, const Data::BoxTree *tree, const Data::Box *box) :
		MJMesh::FrontElementSearchTreeGenerator(front, tree)
{
	this->setBox(box);
}

PMJMesh::FrontElementSearchTreeGenerator::~FrontElementSearchTreeGenerator()
{
	//this->box = NULL;
}

void PMJMesh::FrontElementSearchTreeGenerator::setBox(const Data::Box *box)
{
	this->box = box;
}

Data::GeometricSearchTree *PMJMesh::FrontElementSearchTreeGenerator::makeTree() const
{
#if USE_ADT
    return new Data::FrontElementADTree();
#else
#if USE_EXPERIMENTAL_5
	return (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 2) ?
        static_cast<Data::GeometricSearchTree *>(new PMJMesh::FrontElementBoxSearchTree2D(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box)) :
        static_cast<Data::GeometricSearchTree *>(new PMJMesh::FrontElementBoxSearchTree3D(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box));
#else
	return new PMJMesh::FrontElementBoxSearchTree(this->front, static_cast<const Data::BoxTree *>(this->tree), this->box);
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_ADT
}

#if USE_ADT
void PMJMesh::FrontElementSearchTreeGenerator::execute()
{
    this->graph = this->makeTree();

    Data::FrontElementList fels = this->front->getAllElements();
    fels.insert(fels.end(), this->front->eBegin(3), this->front->eEnd(3));

    static_cast<Data::FrontElementADTree *>(this->graph)->build(fels);
}
#endif //#if USE_ADT
