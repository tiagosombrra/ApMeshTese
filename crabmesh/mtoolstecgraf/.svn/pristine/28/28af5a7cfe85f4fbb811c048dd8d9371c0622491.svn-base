#include "Performer/TreeCellClassifier.h"

#include "Data/State.h"
#include "Data/Boundary.h"
#include "Data/Front.h"
#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"

Performer::TreeCellClassifier::TreeCellClassifier(const Data::Boundary *boundary, const Data::BoxTree *tree) :
	Performer::Builder()
{
	this->setBoundary(boundary);
	//this->setFront(NULL);
	this->setTree(tree);
}

/*Performer::TreeCellClassifier::TreeCellClassifier(const Data::Front *front, const Data::BoxTree *tree) :
	Performer::Builder()
{
	this->setBoundary(NULL);
	this->setFront(front);
	this->setTree(tree);
}*/

Performer::TreeCellClassifier::~TreeCellClassifier()
{

}

//Data::FrontElementList Performer::TreeCellClassifier::findElements() const
const Data::FrontElementList *Performer::TreeCellClassifier::findElements() const
{
	if (this->boundary)
	{
		//return this->boundary->getElements();///TODO optimize it (pointer to list)
		return &this->boundary->getElements();///TODO optimize it (pointer to list)
		/*Data::FrontElementVector els = this->boundary->getElements();

		Data::FrontElementList elements(els.begin(), els.end());

		return elements;*/
	}

	/*if (this->front)
	{
		return this->front->getAllElements();
	}*/

	//return Data::FrontElementList();
	return NULL;
}

#if USE_GUI
void Performer::TreeCellClassifier::setColor(const Data::BoxTreeCell *cell, enum Data::Position pos)
{
    Real r, g, b;

	//debug
	switch (pos)
	{
	case Data::OUT:
		Data::getState().getTypeColor(Data::State::ColoredType::TREE_CELL_OUT, r, g, b);
		//cell->unhighlight();
		break;
	case Data::IN:
		Data::getState().getTypeColor(Data::State::ColoredType::TREE_CELL_IN, r, g, b);
		//cell->unhighlight();
		break;
	case Data::ON:
		Data::getState().getTypeColor(Data::State::ColoredType::TREE_CELL_ON, r, g, b);
		//cell->unhighlight();
		break;
	case Data::UNKNOWN:
		Data::getState().getTypeColor(Data::State::ColoredType::TREE_CELL_UNKNOWN, r, g, b);
		//cell->unhighlight();
		break;
	}
	//endebug

	cell->setColor(r, g, b);
}

void Performer::TreeCellClassifier::setPosition(Data::PositionMap &position, const Data::BoxTreeCell *cell, enum Data::Position pos)
{
	position[cell->getId()] = pos;

	TreeCellClassifier::setColor(cell, pos);
}
#endif //#if USE_GUI

void Performer::TreeCellClassifier::setPosition(const Data::BoxTreeCell *cell, enum Data::Position pos)
{
#if USE_GUI
	this->setPosition(this->position, cell, pos);
#else
	this->position[cell->getId()] = pos;
#endif //#if USE_GUI
}

void Performer::TreeCellClassifier::setChildrenPosition(const Data::BoxTreeCell *cell, enum Data::Position pos)
{
	this->setPosition(cell, pos);

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->setChildrenPosition(static_cast<Data::BoxTreeCell *>(cell->child(i)), pos);
	}
}

/*void Performer::TreeCellClassifier::setFront(const Data::Front *front)
{
	this->front = front;
}*/

void Performer::TreeCellClassifier::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::TreeCellClassifier::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

const Data::PositionMap &Performer::TreeCellClassifier::getPosition() const
{
	return this->position;
}

void Performer::TreeCellClassifier::execute()
{
	this->classify(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->findElements());
}
