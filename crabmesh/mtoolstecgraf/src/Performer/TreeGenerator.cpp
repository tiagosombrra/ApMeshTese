#include "Performer/TreeGenerator.h"

#include "Data/Tree.h"

Performer::TreeGenerator::TreeGenerator() :
	Performer::GraphBuilder()
{

}

Performer::TreeGenerator::~TreeGenerator()
{

}

#if USE_GUI
void Performer::TreeGenerator::setTree(Data::Tree *tree)
{
	this->setGraph(tree);
}
#endif //#if USE_GUI

Data::Tree *Performer::TreeGenerator::getTree() const
{
	return static_cast<Data::Tree *>(this->getGraph());
}
