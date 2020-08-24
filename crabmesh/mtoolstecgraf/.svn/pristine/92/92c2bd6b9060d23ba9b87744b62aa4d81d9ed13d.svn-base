#include "MJMesh/FrontElementSearchTreeGenerator.h"

#include "MJMesh/FrontElementBoxSearchTree.h"
#include "Data/FrontElementADTree.h"
#include "Data/FrontElement.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

MJMesh::FrontElementSearchTreeGenerator::FrontElementSearchTreeGenerator(
	const MJMesh::Front *front, const Data::BoxTree *tree) :
		Performer::TreeGenerator()
{
	this->setTree(tree);

	this->front = front;
	this->boundary = NULL;
}

MJMesh::FrontElementSearchTreeGenerator::FrontElementSearchTreeGenerator(
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree) :
		Performer::TreeGenerator()
{
	this->setTree(tree);

	this->front = NULL;
	this->boundary = boundary;
}

MJMesh::FrontElementSearchTreeGenerator::~FrontElementSearchTreeGenerator()
{
	//this->elements.clear();
}

void MJMesh::FrontElementSearchTreeGenerator::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

Data::GeometricSearchTree *MJMesh::FrontElementSearchTreeGenerator::makeTree() const
{
#if USE_ADT
    return new Data::FrontElementADTree();
#else
#if USE_EXPERIMENTAL_5
	return (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 2) ?
        static_cast<Data::GeometricSearchTree *>(new MJMesh::FrontElementBoxSearchTree2D(this->front, static_cast<const Data::BoxTree *>(this->tree))) :
        (this->front ?
            static_cast<Data::GeometricSearchTree *>(new MJMesh::FrontElementBoxSearchTree3D(this->front, static_cast<const Data::BoxTree *>(this->tree))) :
            static_cast<Data::GeometricSearchTree *>(new MJMesh::FrontElementBoxSearchTree3D(this->boundary, static_cast<const Data::BoxTree *>(this->tree))));
#else
	return this->front ?
        new MJMesh::FrontElementBoxSearchTree(this->front, static_cast<const Data::BoxTree *>(this->tree)) :
        new MJMesh::FrontElementBoxSearchTree(this->boundary, static_cast<const Data::BoxTree *>(this->tree));
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_ADT
}

void MJMesh::FrontElementSearchTreeGenerator::execute()
{
	this->graph = this->makeTree();

#if USE_ADT
    static_cast<Data::FrontElementADTree *>(this->graph)->build(
        this->front ? this->front->getAllElements() : this->boundary->getElements());
#else
	static_cast<Data::GeometricSearchTree *>(this->graph)->build();

	if (this->front)
    {
        for (Data::Front::ConstAllFrontElementIterator iter = this->front->allEBegin(), end = this->front->allEEnd();
             iter != end; iter++)
        {
            static_cast<Data::GeometricSearchTree *>(this->graph)->add((*iter));
        }
    }
    else
    {
        for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
             iter != end; iter++)
        {
            static_cast<Data::GeometricSearchTree *>(this->graph)->add((*iter));
        }
    }
#endif //#if USE_ADT
}
