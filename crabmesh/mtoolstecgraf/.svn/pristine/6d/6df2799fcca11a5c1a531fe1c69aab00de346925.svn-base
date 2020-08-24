#include "Data/FrontElementADTree.h"

#include "Data/FrontElementADTreeCell.h"

Data::FrontElementADTree::FrontElementADTree() :
	Data::AlternatingDigitalTree::AlternatingDigitalTree()
{

}

Data::FrontElementADTree::~FrontElementADTree()
{

}

bool Data::FrontElementADTree::build(const FrontElementList &fels)
{
	if (!this->getRoot())
	{
		this->makeRoot();
	}

	static_cast<Data::FrontElementADTreeCell *>(this->getRoot())->build(fels);

	return true;
}

void Data::FrontElementADTree::makeRoot()
{
	this->setRoot(new FrontElementADTreeCell());
}
