#include "Tools/CracksIdentifier.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/TreeGenerator.h"
#include "MJMesh/FrontElementSearchTreeGenerator.h"

Tools::CracksIdentifier::CracksIdentifier(const Tool *tool) :
	Performer::CracksIdentifier::CracksIdentifier(),
	Tools::ExternalTool::ExternalTool(tool)
{
	this->front = NULL;
}

Tools::CracksIdentifier::~CracksIdentifier()
{
	if (this->front)
	{
		delete this->front;
	}

	if (this->tree)
	{
		delete this->tree;
	}
}

void Tools::CracksIdentifier::execute()
{
	if (!this->boundary)
	{
		return;
	}

	this->createFront();

	this->addMeasure(0, static_cast<UInt>(this->boundary->size()));

	Performer::CracksIdentifier::execute();

	if (this->front)
	{
		delete this->front;

		this->front = NULL;
	}

	if (this->tree)
	{
		delete this->tree;

		this->tree = NULL;
	}

	this->endMeasure();
}

void Tools::CracksIdentifier::createFront()
{
	//return;

	if (!dynamic_cast<MJMesh::Boundary *>(this->boundary))
	{
		return;
	}

	this->small.clear();

	this->front = new MJMesh::Front(false);

	Real tolerance = Data::getTolerance();

	Data::setTolerance(tolerance*0.00001);

	this->setMeasure(0, static_cast<UInt>(this->boundary->size()) + 1);

	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		this->addMeasure(1, 0);

		if ((*iter)->getShape()->size() <= Data::getTolerance())
		{
			this->small.push_back((*iter));

			std::cout << "too small front element: " << (*iter)->text() << std::endl;

			continue;
		}

		this->front->add((*iter));
	}

	Data::setTolerance(tolerance);

	UInt dimension = this->boundary->dimension();

	MJMesh::TreeGenerator tg(dimension, static_cast<MJMesh::Boundary *>(this->boundary), 0.01);

	tg.execute();

	this->tree = static_cast<Data::BoxTree *>(tg.getTree());

	MJMesh::FrontElementSearchTreeGenerator festg(this->front, this->tree);

	festg.execute();

	Data::BoxSearchTree *stree = static_cast<Data::BoxSearchTree *>(festg.getTree());

	this->front->setFESTree(stree);
}

Data::FrontElement *Tools::CracksIdentifier::findCrackFel(
	Data::Boundary::FrontElementIterator iter, const Data::FrontElement *fel) const
{
	if (!this->front)
	{
		return Performer::CracksIdentifier::findCrackFel(iter, fel);
	}

	Data::Box box = fel->getShape()->box();

	Data::FrontElementList fels = this->front->frontElementsInRange(box);

	for (Data::FrontElementList::iterator iter2 = fels.begin();
		 iter2 != fels.end(); iter2++)
	{
		Data::FrontElement *other = (*iter2);

		if (other == fel)
		{
			continue;
		}

		if (this->equal(other, fel))
		{
			return other;
		}
	}

	return NULL;
}

void Tools::CracksIdentifier::checkForCrack(Data::Boundary::FrontElementIterator iter, Data::FrontElement *fel)
{
	this->addMeasure(1, 0);

	Performer::CracksIdentifier::checkForCrack(iter, fel);
}
