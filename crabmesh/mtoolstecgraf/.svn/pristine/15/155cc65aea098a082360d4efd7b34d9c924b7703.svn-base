#include "Performer/MinTreeRefiner.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

using namespace Performer;

Performer::MinTreeRefiner::MinTreeRefiner(Data::BoxTree *tree, UInt minLevel) :
	TreeRefiner(tree)
{
	this->setMinLevel(minLevel);
}

Performer::MinTreeRefiner::~MinTreeRefiner()
{

}

void Performer::MinTreeRefiner::subdivide(Data::TreeNode *n)
{
	this->subdivide(static_cast<Data::BoxTreeCell *>(n));
}

void Performer::MinTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
	c->subdivide();
}

void Performer::MinTreeRefiner::setMinLevel(UInt minLevel)
{
	this->minLevel = minLevel;
}

UInt Performer::MinTreeRefiner::getMinLevel() const
{
	return this->minLevel;
}

void Performer::MinTreeRefiner::refine(Data::TreeNode *n)
{
	this->refine(static_cast<Data::BoxTreeCell *>(n));
}

void Performer::MinTreeRefiner::refine(Data::BoxTreeCell *c)
{
	if (c->level() >= this->minLevel)
	{
		return;
	}

	if (!c->isSubdivided())
	{
		this->subdivide(c);
	}

	this->refineChildren(c);
}

void Performer::MinTreeRefiner::refineChildren(Data::BoxTreeCell *c)
{
	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}

void Performer::MinTreeRefiner::execute()
{
	if (!this->tree)
	{
		return;
	}

	this->minLevel = this->findMinLevel();

	this->refine(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}
