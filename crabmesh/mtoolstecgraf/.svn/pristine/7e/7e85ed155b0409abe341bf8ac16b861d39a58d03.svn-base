#include "Data/GraphNode.h"

#include "Data/Graph.h"

using namespace Data;

Data::GraphNode::GraphNode(Graph *graph, ULInt id) :
	Identifiable(id)
{
#if (!USE_EXPERIMENTAL_5)
#if USE_ARRAY
	this->numAdjs = 0;
	this->adjacents = NULL;
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_5)

	this->setGraph(graph);
}

Data::GraphNode::~GraphNode()
{
	if (this->graph)
	{
		this->graph->remove(this);

		this->graph = NULL;
	}

#if (!USE_EXPERIMENTAL_5)
	for (UInt i = 0; i < this->numAdjacents(); i++)
	{
		this->adjacents[i]->remove(this);
	}
#endif //#if (!USE_EXPERIMENTAL_5)

	/*while (!this->adjacents.empty())
	{
		GraphNode *n = this->adjacents.back();
		this->adjacents.pop_back();

		n->remove(this);
	}*/

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_5)
	if (this->adjacents)
	{
		delete [] this->adjacents;
	}
#endif //#if (!USE_EXPERIMENTAL_5)
#endif //#if USE_ARRAY
}

void Data::GraphNode::setGraph(Graph *graph)
{
	this->graph = graph;
}

Graph *Data::GraphNode::getGraph() const
{
	return this->graph;
}

#if (!USE_EXPERIMENTAL_5)
UInt Data::GraphNode::numAdjacents() const
{
#if USE_ARRAY
	return this->numAdjs;
#else
	return this->adjacents.size();
#endif //#if USE_ARRAY
}

void Data::GraphNode::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::GraphNode::getAdjacent(UInt i) const
{
	//return (i < this->numAdjacents()) ? this->adjacents[i] : NULL;
	return this->adjacents[i];
}

GraphNodeVector Data::GraphNode::getAdjacents() const
{
#if USE_ARRAY
	return (this->adjacents) ? GraphNodeVector(this->adjacents, this->adjacents + this->numAdjacents()) : GraphNodeVector();
#else
	return this->adjacents;
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_5)

bool Data::GraphNode::have(GraphNode *n) const
{
#if USE_ARRAY
	for (UInt i = 0; i < this->numAdjacents(); i++)
	{
#if USE_EXPERIMENTAL_5
		if (this->getAdjacent(i) == n)
#else
		if (this->adjacents[i] == n)
#endif //#if USE_EXPERIMENTAL_5
		{
			return true;
		}
	}

	return false;
#else
	return this->adjacents.empty() ? false : std::find(this->adjacents.begin(), this->adjacents.end(), n) != this->adjacents.end();
#endif //#if USE_ARRAY
}

#if (!USE_EXPERIMENTAL_5)
void Data::GraphNode::add(GraphNode *n)
{
#if USE_ARRAY
	if (!this->adjacents)
	{
		this->adjacents = new GraphNode*[1];

		this->adjacents[0] = n;

		this->numAdjs = 1;

		return;
	}

	GraphNode **adjacents = this->adjacents;

	this->adjacents = new GraphNode*[this->numAdjs + 1];

	for (UInt i = 0; i < this->numAdjs; i++)
	{
		this->adjacents[i] = adjacents[i];
	}

	delete [] adjacents;

	this->adjacents[this->numAdjs++] = n;
#else
	this->adjacents.push_back(n);
#endif //#if USE_ARRAY
}

void Data::GraphNode::remove(GraphNode *n)
{
#if USE_ARRAY
	if (!this->adjacents)
	{
		return;
	}

	UInt found = this->numAdjs;

	for (UInt i = 0; i < this->numAdjs; i++)
	{
		if (this->adjacents[i] == n)
		{
			found = i;

			break;
		}
	}

	if (found < this->numAdjs)
	{
		GraphNode **adjacents = this->adjacents;

		this->adjacents = new GraphNode*[this->numAdjs - 1];

		for (UInt i = 0; i < found; i++)
		{
			this->adjacents[i] = adjacents[i];
		}

		for (UInt i = found; i < static_cast<UShort>(this->numAdjs - 1); i++)
		{
			this->adjacents[i] = adjacents[i + 1];
		}

		delete [] adjacents;

		this->numAdjs--;
	}
#else
	if (this->adjacents.empty())
	{
		return;
	}

	GraphNodeVector::iterator it = std::find(this->adjacents.begin(), this->adjacents.end(), n);

	if (it != this->adjacents.end())
	{
		this->adjacents.erase(it);
	}
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_5)

#if USE_ARRAY
GraphNode &Data::GraphNode::operator=(const GraphNode &n)
{
	if (this != &n)
	{
		this->id = n.id;

#if (!USE_EXPERIMENTAL_5)
		if (this->numAdjs != n.numAdjs)
		{
			if (this->adjacents)
			{
				delete [] this->adjacents;
			}

			this->numAdjs = n.numAdjs;

			this->adjacents = new GraphNode*[this->numAdjs];
		}
#endif //#if (!USE_EXPERIMENTAL_5)

		/*if (this->adjacents)
		{
			delete [] this->adjacents;
		}

		this->numAdjs = n.numAdjs;

		this->adjacents = new GraphNode*[this->numAdjs];*/

#if (!USE_EXPERIMENTAL_5)
		for (UInt i = 0; i < this->numAdjs; i++)
		{
			this->adjacents[i] = n.adjacents[i];
		}
#endif //#if (!USE_EXPERIMENTAL_5)
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
void Data::GraphNode::draw() const
{
	//draw the node;

	//draw the adjacency
}

std::string Data::GraphNode::text() const
{
	std::stringstream str;

	str << this->getId();

	for (UInt i = 0; i < this->numAdjacents(); i++)
	{
#if USE_EXPERIMENTAL_5
		str << " " << this->getAdjacent(i)->getId() << std::endl;
#else
		str << " " << this->adjacents[i]->getId() << std::endl;
#endif //#if USE_EXPERIMENTAL_5
	}

	return str.str();
}
#endif //#if USE_GUI
