#ifndef _MJMESH_TREE_GENERATOR_H_
#define _MJMESH_TREE_GENERATOR_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/Boundary.h"
#include "Performer/TreeGenerator.h"

namespace MJMesh
{
	class TreeGenerator : virtual public Performer::TreeGenerator
	{
	public:

		TreeGenerator(UInt dimension, const MJMesh::Boundary *boundary, Real factor);
		virtual ~TreeGenerator();

		virtual void setDimension(UInt dimension);

		virtual void setFactor(Real factor);

		virtual void setBoundary(const MJMesh::Boundary *boundary);

		virtual UInt getMinLevel() const;
		virtual UInt getMaxLevel() const;

		virtual bool shouldSubdivide(Data::BoxTreeCell *c, Real elSize) const;
		virtual bool shouldSubdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e) const;

		//virtual void subdivide(Data::BoxTreeCell *c, Data::Point p, Real elSize);
		virtual bool subdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e);

		virtual void execute();

	public:

		enum
		{
			NOT_INIT = 0,
			INIT,
			ENDED,

			ERROR = 100
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status ended;
		static const Performer::Status error;

	protected:

		virtual Data::Tree *makeTree(const Data::Point2D &min, const Data::Point2D &max) const;
		virtual Data::Tree *makeTree(const Data::Point3D &min, const Data::Point3D &max) const;

		virtual void subdivide(Data::BoxTreeCell *c) const;

	protected:

		UInt dimension;

		Real factor;

		const MJMesh::Boundary *boundary;

		//Data::FrontElementVector elements;
		//Data::FrontElementList elements;
		Data::BoxTreeCell *root;

		UInt minLevel;
		UInt maxLevel;

    private:

        MJMesh::Boundary::ConstFrontElementIterator iter;

	};
}

#endif //#ifndef _MJMESH_TREE_GENERATOR_H_
