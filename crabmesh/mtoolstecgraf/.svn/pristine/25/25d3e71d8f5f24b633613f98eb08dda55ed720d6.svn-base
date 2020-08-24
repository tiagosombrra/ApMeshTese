#include "Performer/GraphBuilder.h"

#include "Data/Graph.h"

Performer::GraphBuilder::GraphBuilder() :
	Builder()
{
	this->graph = NULL;
}

Performer::GraphBuilder::~GraphBuilder()
{
	//this->graph = NULL;
}

#if USE_GUI
void Performer::GraphBuilder::setGraph(Data::Graph *graph)
{
	this->graph = graph;
}
#endif //#if USE_GUI

Data::Graph *Performer::GraphBuilder::getGraph() const
{
	return this->graph;
}
