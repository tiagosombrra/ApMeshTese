#include "Data/Graph.h"

#include "Data/GraphNode.h"

using namespace Data;

Data::Graph::Graph(ULInt id) :
	Identifiable(id)
{

}

Data::Graph::Graph(const GraphNodeList &nodes, ULInt id) :
	Identifiable(id)
{
	this->setNodes(nodes);
}

Data::Graph::~Graph()
{
	/*while (!this->nodes.empty())
	{
		delete this->nodes.back();

		this->nodes.pop_back();
	}*/

	for (Data::GraphNodeSet::iterator iter = this->nodes.begin();
		 iter != this->nodes.end(); iter++)
	{
		delete (*iter);
	}
}

void Data::Graph::setNodes(const GraphNodeList &nodes)
{
	//this->nodes = nodes;
	this->nodes.clear();

	this->nodes.insert(nodes.begin(), nodes.end());
}

GraphNodeList Data::Graph::getNodes() const
{
	//return this->nodes;
	return GraphNodeList(this->nodes.begin(), this->nodes.end());
}

const GraphNodeSet &Data::Graph::getNodesSet() const
{
	return this->nodes;
}

/*void Data::Graph::setNode(UInt i, GraphNode *n)
{
	if (i < this->nodes.size())
	{
		this->nodes[i] = n;
	}
}

GraphNode *Data::Graph::getNode(UInt i) const
{
	return (i < this->nodes.size()) ? this->nodes[i] : NULL;
}*/

GraphNode *Data::Graph::node(ULInt id) const
{
	//for (GraphNodeList::const_iterator iter = this->nodes.begin();
	for (GraphNodeSet::const_iterator iter = this->nodes.begin();
		 iter != this->nodes.end(); iter++)
	{
		if ((*iter)->getId() == id)
		{
			return (*iter);
		}
	}

	return NULL;
}

void Data::Graph::add(GraphNode *n)
{
	//this->nodes.push_back(n);
	this->nodes.insert(n);

	n->setGraph(this);
}

void Data::Graph::remove(GraphNode *n)
{
	if (this->nodes.empty())
	{
		return;
	}

	this->nodes.erase(n);

	/*GraphNodeList::iterator it = std::find(this->nodes.begin(), this->nodes.end(), n);

	if (it != this->nodes.end())
	{
		this->nodes.erase(it);
	}*/
}

bool Data::Graph::have(GraphNode *n) const
{
	//return this->nodes.empty() ? false : std::find(this->nodes.begin(), this->nodes.end(), n) != this->nodes.end();
	return this->nodes.empty() ? false : this->nodes.find(n) != this->nodes.end();
}

Data::Graph::NodeIterator Data::Graph::nBegin()
{
    return this->nodes.begin();
}

Data::Graph::ConstNodeIterator Data::Graph::nBegin() const
{
    return this->nodes.begin();
}

Data::Graph::NodeIterator Data::Graph::nEnd()
{
    return this->nodes.end();
}

Data::Graph::ConstNodeIterator Data::Graph::nEnd() const
{
    return this->nodes.end();
}
