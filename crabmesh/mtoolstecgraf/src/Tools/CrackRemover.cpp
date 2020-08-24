#include "Tools/CrackRemover.h"

#include "Tools/CracksIdentifier.h"
#include "MJMesh/Vertex.h"

Tools::CrackRemover::CrackRemover() :
	Performer::CrackRemover::CrackRemover(),
	Tools::Tool::Tool()
{

}

Tools::CrackRemover::~CrackRemover()
{

}

void Tools::CrackRemover::execute()
{
	this->startMeasure();

	Performer::CrackRemover::execute();

	this->endMeasure();
}

Performer::CracksIdentifier *Tools::CrackRemover::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->boundary);
	identifier->setUseTolerance(this->useTolerance);

	return identifier;
}

void Tools::CrackRemover::removeCrack(Performer::CracksIdentifier::CrackMap::iterator &iter)
{
	this->addMeasure(1, 0);

	Performer::CrackRemover::removeCrack(iter);
}

void Tools::CrackRemover::removeCracks()
{
	this->addMeasure(0, static_cast<UInt>(this->cracks->size()));

	Performer::CrackRemover::removeCracks();
}

void Tools::CrackRemover::replace(Data::FrontElement *fel, Data::Vertex *oldV, Data::Vertex *newV) const
{
	Performer::CrackRemover::replace(fel, oldV, newV);

	static_cast<MJMesh::Vertex *>(oldV)->remove(fel);

	if (newV)
	{
		static_cast<MJMesh::Vertex *>(newV)->add(fel);
	}
}

void Tools::CrackRemover::replace(Data::Element *e, Data::Vertex *oldV, Data::Vertex *newV) const
{
	Performer::CrackRemover::replace(e, oldV, newV);

	static_cast<MJMesh::Vertex *>(oldV)->remove(e);

	if (newV)
	{
		static_cast<MJMesh::Vertex *>(newV)->add(e);
	}
}
