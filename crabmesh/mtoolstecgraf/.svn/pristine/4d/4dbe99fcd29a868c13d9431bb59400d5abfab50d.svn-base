#ifndef _PERFORMER_GRAPH_BUILDER_H_
#define _PERFORMER_GRAPH_BUILDER_H_

#include "Performer/Definitions.h"

#include "Performer/Builder.h"

namespace Performer
{
	class GraphBuilder : public Performer::Builder
	{
	public:

		GraphBuilder();
		virtual ~GraphBuilder();

#if USE_GUI
		virtual void setGraph(Data::Graph *graph);
#endif //#if USE_GUI

		virtual Data::Graph *getGraph() const;

		virtual void execute() = 0;

	protected:

		Data::Graph *graph;
	};
}

#endif //#ifndef _PERFORMER_GRAPH_BUILDER_H_
