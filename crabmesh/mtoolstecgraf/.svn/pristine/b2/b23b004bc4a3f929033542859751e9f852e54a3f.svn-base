#include "Data/VertexADTree.h"

#include "Data/VertexADTreeCell.h"

Data::VertexADTree::VertexADTree() :
	Data::AlternatingDigitalTree::AlternatingDigitalTree()
{

}

Data::VertexADTree::~VertexADTree()
{

}

bool Data::VertexADTree::build(const VertexList &vertices)
{
	if (!this->getRoot())
	{
		this->makeRoot();
	}

	static_cast<Data::VertexADTreeCell *>(this->getRoot())->build(vertices);

	return true;
}

void Data::VertexADTree::makeRoot()
{
	this->setRoot(new VertexADTreeCell());
}
