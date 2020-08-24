#ifndef _DATA_BOX_SEARCH_TREE_H_
#define _DATA_BOX_SEARCH_TREE_H_

#include "Data/Definitions.h"
#include "Data/GeometricSearchTree.h"

namespace Data
{
	class BoxSearchTree : public Data::GeometricSearchTree
	{
	public:

		BoxSearchTree(const BoxTree *tree = NULL,
			BoxSearchTreeCell *root = NULL, ULInt id = 0);
		virtual ~BoxSearchTree();

		virtual void setTree(const BoxTree *tree);
		virtual const BoxTree *getTree() const;

		/*virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual BoxTreeCell *find(const Point &p) const;
		virtual BoxTreeCell *find(const Point *p) const;

		virtual Real size() const;

		virtual GraphNodeVector neighbors(const BoxTreeCell *c) const;*/

		virtual void clear();

		virtual bool build();

		virtual GraphNodeList neighbors(const BoxSearchTreeCell *cell) const;

#if USE_GUI
		virtual void draw() const;
#endif //#if USE_GUI

	protected:

		//virtual bool shouldAddChildren(const Data::BoxTreeCell *cell) const;

		virtual bool build(BoxTreeCell *cell, BoxSearchTreeCell *scell);

		virtual bool shouldRebuild() const;
		virtual bool rebuildTreeNodes();

		virtual BoxTree *makeTree() const;
		virtual BoxSearchTreeCell *makeTreeCell(BoxTreeCell *c, BoxSearchTreeCell *parent, ULInt id) const = 0;

	protected:

		const BoxTree *tree;

#if USE_C__11
		GraphNodeHashMap map;
#else
		GraphNodeMap map;
#endif //#if USE_C__11

		//this boolean means that the box tree existed before and, therefore,
		//  should not be deleted
		bool twin;

	/*public:

		static Real timeInit, timeExecute, timeEnd;*/
	};

#if USE_EXPERIMENTAL_5
    class BoxSearchTree2D : public Data::BoxSearchTree
    {
    public:

        BoxSearchTree2D(const BoxTree *tree = NULL,
			BoxSearchTreeCell2D *root = NULL, ULInt id = 0);
		virtual ~BoxSearchTree2D();
    };

    class BoxSearchTree3D : public Data::BoxSearchTree
    {
    public:

        BoxSearchTree3D(const BoxTree *tree = NULL,
			BoxSearchTreeCell3D *root = NULL, ULInt id = 0);
		virtual ~BoxSearchTree3D();
    };
#endif //#if USE_EXPERIMENTAL_5
}

#endif //#ifndef _DATA_BOX_SEARCH_TREE_H_
