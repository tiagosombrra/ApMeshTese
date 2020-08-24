#include "Data/AlternatingDigitalTree.h"

#include "Data/AlternatingDigitalTreeCell.h"

Data::AlternatingDigitalTree::AlternatingDigitalTree() :
	Data::GeometricSearchTree::GeometricSearchTree()
{

}

Data::AlternatingDigitalTree::~AlternatingDigitalTree()
{

}

bool Data::AlternatingDigitalTree::build()
{
	if (!this->getRoot())
	{
		this->makeRoot();
	}

	return true;
}

bool Data::AlternatingDigitalTree::build(const IdentifiableList &shapes)
{
	if (!this->getRoot())
	{
		this->makeRoot();
	}

	static_cast<Data::AlternatingDigitalTreeCell *>(this->getRoot())->build(shapes);

	return true;
}

bool Data::AlternatingDigitalTree::rebuild()
{
	IdentifiableList shapes;

	static_cast<Data::AlternatingDigitalTreeCell *>(this->getRoot())->allShapes(shapes);

	this->getRoot()->deleteChildren();

	this->build(shapes);

	this->addedSinceLastRebuild = 0;
	this->removedSinceLastRebuild = 0;

	return true;
}

bool Data::AlternatingDigitalTree::shouldRebuild() const
{
	return false;
}

#if USE_GUI
void Data::AlternatingDigitalTree::draw() const
{
	static_cast<Data::AlternatingDigitalTreeCell *>(this->getRoot())->draw();
}
#endif //#if USE_GUI

bool Data::AlternatingDigitalTree::rebuildTreeNodes()
{
	return false;
}
