#ifndef _DATA_ALTERNATING_DIGITAL_TREE_H_
#define _DATA_ALTERNATING_DIGITAL_TREE_H_

#include "Data/Definitions.h"
#include "Data/GeometricSearchTree.h"

namespace Data
{
	class AlternatingDigitalTree : public Data::GeometricSearchTree
	{
	public:

		AlternatingDigitalTree();
		virtual ~AlternatingDigitalTree();

		virtual bool build();
		virtual bool build(const IdentifiableList &shapes);
		virtual bool rebuild();

		virtual bool shouldRebuild() const;

#if USE_GUI
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		virtual void makeRoot() = 0;

		virtual bool rebuildTreeNodes();

	};
}

#endif //#ifndef _DATA_ALTERNATING_DIGITAL_TREE_H_
