#ifndef _DATA_GRAPH_NODE_H_
#define _DATA_GRAPH_NODE_H_

#include "Data/Definitions.h"
#include "Data/Identifiable.h"

namespace Data
{
	class GraphNode :
		public Data::Identifiable
	{
	public:

		GraphNode(Graph *graph, ULInt id = 0);
		virtual ~GraphNode();

		virtual void setGraph(Graph *graph);
		virtual Graph *getGraph() const;

#if USE_EXPERIMENTAL_5
		virtual UInt numAdjacents() const = 0;
#else
		virtual UInt numAdjacents() const;
#endif //#if USE_EXPERIMENTAL_5

#if USE_EXPERIMENTAL_5
		virtual void setAdjacent(UInt i, GraphNode *n) = 0;
		virtual GraphNode *getAdjacent(UInt i) const = 0;
#else
		virtual void setAdjacent(UInt i, GraphNode *n);
		virtual GraphNode *getAdjacent(UInt i) const;
#endif //#if USE_EXPERIMENTAL_5

#if USE_EXPERIMENTAL_5
		virtual GraphNodeVector getAdjacents() const = 0;
#else
		virtual GraphNodeVector getAdjacents() const;
#endif //#if USE_EXPERIMENTAL_5

#if USE_EXPERIMENTAL_5
		virtual void add(GraphNode *n) = 0;
		virtual void remove(GraphNode *n) = 0;
#else
		virtual void add(GraphNode *n);
		virtual void remove(GraphNode *n);
#endif //#if USE_EXPERIMENTAL_5
		virtual bool have(GraphNode *n) const;

#if USE_ARRAY
		virtual GraphNode &operator=(const GraphNode &n);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

#if (!USE_EXPERIMENTAL_5)
#if USE_ARRAY
		GraphNode **adjacents;
		UShort numAdjs;
#else
		GraphNodeVector adjacents;
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_5)

		Graph *graph;
	};
}

#endif //#ifndef _DATA_GRAPH_NODE_H_
