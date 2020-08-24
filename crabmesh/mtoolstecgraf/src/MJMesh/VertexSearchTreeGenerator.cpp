#include "MJMesh/VertexSearchTreeGenerator.h"

#include "MJMesh/VertexBoxSearchTree.h"
#include "Data/VertexADTree.h"
#include "Data/Vertex.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

MJMesh::VertexSearchTreeGenerator::VertexSearchTreeGenerator(
	const MJMesh::Front *front, const Data::BoxTree *tree) :
		Performer::TreeGenerator()
{
	this->setTree(tree);

	this->front = front;
}

MJMesh::VertexSearchTreeGenerator::~VertexSearchTreeGenerator()
{
	//this->vertices.clear();
}

void MJMesh::VertexSearchTreeGenerator::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

Data::GeometricSearchTree *MJMesh::VertexSearchTreeGenerator::makeTree() const
{
#if USE_ADT
    return new Data::VertexADTree();
#else
#if USE_EXPERIMENTAL_5
	return (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 2) ?
        static_cast<Data::GeometricSearchTree *>(new MJMesh::VertexBoxSearchTree2D(this->front, static_cast<const Data::BoxTree *>(this->tree))) :
        static_cast<Data::GeometricSearchTree *>(new MJMesh::VertexBoxSearchTree3D(this->front, static_cast<const Data::BoxTree *>(this->tree)));
#else
	return new MJMesh::VertexBoxSearchTree(this->front, static_cast<const Data::BoxTree *>(this->tree));
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_ADT
}

void MJMesh::VertexSearchTreeGenerator::execute()
{
	this->graph = this->makeTree();

#if USE_ADT
    static_cast<Data::VertexADTree *>(this->graph)->build(this->front->getVertices());
#else
	static_cast<Data::GeometricSearchTree *>(this->graph)->build();

	for (Data::Front::ConstVertexIterator iter = this->front->vBegin();
		 iter != this->front->vEnd(); iter++)
	{
		static_cast<Data::GeometricSearchTree *>(this->graph)->add((*iter));
	}
#endif //#if USE_ADT
}
