#ifndef _MJMESH_GEOMETRY_FRONT_ADVANCER_H_
#define _MJMESH_GEOMETRY_FRONT_ADVANCER_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/FrontAdvancer.h"

namespace MJMesh
{
	class GeometryFrontAdvancer : public MJMesh::FrontAdvancer
	{
	public:

		GeometryFrontAdvancer(UInt dimension, const MJMesh::Boundary *boundary,
			const Data::BoxTree *tree, bool usingSearchTrees = false);
		virtual ~GeometryFrontAdvancer();

	protected:

		virtual bool isThereError(const Data::FrontElement *fel, const Data::Vertex *best) const;

		using MJMesh::FrontAdvancer::isValid;
		virtual bool isValid(const Data::FrontElement *element, Data::Point *point, Real radius) const;

		// moved to MJMesh::FrontAdvancer
		//virtual const Data::BoxTreeCell *cell(const Data::Point &p) const;

		// moved to MJMesh::FrontAdvancer
		//virtual Data::PointVector ideal(const Data::FrontElement *element) const;

		// moved to MJMesh::FrontAdvancer
		//virtual Real radius(const Data::FrontElement *element) const;

		using MJMesh::FrontAdvancer::best;
		virtual Data::Vertex *best(const Data::FrontElement *element) const;
	};
}

#endif //#ifndef _MJMESH_GEOMETRY_FRONT_ADVANCER_H_
