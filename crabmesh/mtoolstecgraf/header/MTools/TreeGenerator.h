#ifndef _MTOOLS_TREE_GENERATOR_H_
#define _MTOOLS_TREE_GENERATOR_H_

#include "MTools/Definitions.h"

#if USE_MTOOLS

#include "OctTree.hpp"
#include "Performer/TreeGenerator.h"

namespace MTools
{
	class TreeGenerator : virtual public Performer::TreeGenerator
	{
	public:

		TreeGenerator(const FTools::OctTree *ftree = NULL);
		virtual ~TreeGenerator();

		virtual void setFTree(const FTools::OctTree *ftree);

		virtual void execute();

	protected:

		struct TreeGeneratorList
		{
			Performer::TreeGenerator *generator;
			Data::GraphNodeList *cells;
		};

	protected:

		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		//virtual bool shouldSubdivideChildren(const FTools::OctTree *fcell) const;

		static void visitStream(void *ptr, double x, double y, double z, double size, bool root, bool leaf);
		static void visitList(void *ptr, double x, double y, double z, double size, bool root, bool leaf);

		virtual void subdivide(Data::OctTreeCell *cell) const;
		virtual void subdivide(std::stringstream &linearized, Data::OctTreeCell *cell) const;

	protected:

		const FTools::OctTree *ftree;
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _MTOOLS_TREE_GENERATOR_H_
