#ifndef _DATA_GEOMETRIC_SEARCH_TREE_H_
#define _DATA_GEOMETRIC_SEARCH_TREE_H_

#include "Data/Definitions.h"
#include "Data/Tree.h"
#include "Data/GeometricSearchTreeNode.h"

namespace Data
{
	class GeometricSearchTree : public Data::Tree
	{
	public:

		GeometricSearchTree(GeometricSearchTreeNode *root = NULL, ULInt id = 0);
		virtual ~GeometricSearchTree();

		using Tree::add;
		using Tree::remove;

		virtual bool add(const Identifiable *s);
		virtual bool remove(const Identifiable *s);

		virtual IdentifiableList allShapes() const;

		virtual IdentifiableList inRange(const Box &box) const;

		virtual void clear();

		virtual bool build() = 0;
		virtual bool rebuild();

		virtual bool shouldRebuild() const = 0;

#if USE_GUI
		virtual void draw() const = 0;
#endif //#if USE_GUI

	protected:

		virtual bool rebuildTreeNodes() = 0;

	protected:

		ULInt added;
		ULInt removed;
		ULInt addedSinceLastRebuild;
		ULInt removedSinceLastRebuild;
	};
}

#endif //#ifndef _DATA_GEOMETRIC_SEARCH_TREE_H_
