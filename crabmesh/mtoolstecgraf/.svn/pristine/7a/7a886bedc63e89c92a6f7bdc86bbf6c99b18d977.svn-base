#ifndef _DATA_GRAPH_H_
#define _DATA_GRAPH_H_

#include "Data/Definitions.h"
#include "Data/GraphNode.h"

namespace Data
{
	class Graph :
		public Data::Identifiable
	{
    public:

        typedef GraphNodeSet::iterator NodeIterator;
		typedef GraphNodeSet::const_iterator ConstNodeIterator;

	public:

		Graph(ULInt id = 0);
		Graph(const GraphNodeList &nodes, ULInt id = 0);
		virtual ~Graph();

		virtual void setNodes(const GraphNodeList &nodes);
		virtual GraphNodeList getNodes() const;
		virtual const GraphNodeSet &getNodesSet() const;

		//virtual void setNode(UInt i, GraphNode *n);
		//virtual GraphNode *getNode(UInt i) const;
		virtual GraphNode *node(ULInt id) const;

		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);
		virtual bool have(GraphNode *n) const;

		virtual NodeIterator nBegin();
		virtual ConstNodeIterator nBegin() const;
		virtual NodeIterator nEnd();
		virtual ConstNodeIterator nEnd() const;

	protected:

		GraphNodeSet nodes;
	};
}

#endif //#ifndef _DATA_GRAPH_H_
