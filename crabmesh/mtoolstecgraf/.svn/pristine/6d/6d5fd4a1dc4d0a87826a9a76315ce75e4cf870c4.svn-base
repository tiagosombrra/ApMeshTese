#ifndef _JMESH_TREE_GENERATOR_H_
#define _JMESH_TREE_GENERATOR_H_

#include "Jmesh/Definitions.h"

#include "Jmesh/JmeshMainDef3D.h"
#include "Performer/TreeGenerator.h"

namespace Jmesh
{
	class TreeGenerator : virtual public Performer::TreeGenerator
	{
	public:

		TreeGenerator(const Msh3DOctTree root = NULL);
		virtual ~TreeGenerator();

		virtual void setRoot(const Msh3DOctTree root);

		virtual void execute();

	protected:

		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		virtual bool shouldSubdivideChildren(const Msh3DOctTree root) const;

		virtual void subdivide(Data::OctTreeCell *cell) const;
		virtual void subdivide(const Msh3DOctTree root, Data::OctTreeCell *cell) const;

	protected:

		Msh3DOctTree root;
	};
}

#endif //#ifndef _JMESH_TREE_GENERATOR_H_
