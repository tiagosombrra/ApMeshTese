#ifndef _PERFORMER_TREE_GENERATOR_H_
#define _PERFORMER_TREE_GENERATOR_H_

#include "Performer/Definitions.h"

#include "Performer/GraphBuilder.h"

namespace Performer
{
	class TreeGenerator : public Performer::GraphBuilder
	{
	public:

		TreeGenerator();
		virtual ~TreeGenerator();

#if USE_GUI
		virtual void setTree(Data::Tree *tree);
#endif //#if USE_GUI

		virtual Data::Tree *getTree() const;

		virtual void execute() = 0;
	};
}

#endif //#ifndef _PERFORMER_TREE_GENERATOR_H_
