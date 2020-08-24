#ifndef _MJMESH_BOUNDARY_BUILDER_H_
#define _MJMESH_BOUNDARY_BUILDER_H_

#include "MJMesh/Definitions.h"
#include "Performer/BoundaryBuilder.h"

namespace MJMesh
{
	class BoundaryBuilder : public Performer::BoundaryBuilder
	{
	public:

		BoundaryBuilder(bool sorted);
		virtual ~BoundaryBuilder();

		virtual void setSorted(bool sorted);

		virtual Performer::IdManager *getIdManager() const;

		//methods for 2D version
		virtual MJMesh::Vertex *add(ULInt id, Real x, Real y);
		virtual MJMesh::FrontElement2D *add(ULInt id, ULInt id1, ULInt id2);

		//methods for 3D version
		virtual MJMesh::Vertex *add(ULInt id, Real x, Real y, Real z);
		virtual MJMesh::FrontElementSurface *addSurface(ULInt id, ULInt id1, ULInt id2);
		virtual MJMesh::FrontElement3D *add(ULInt id, ULInt id1, ULInt id2, ULInt id3);
#if USE_QUADS
		virtual MJMesh::FrontElement3D *add(ULInt id, ULInt id1, ULInt id2, ULInt id3, ULInt id4);
#endif //#if USE_QUADS

		virtual void incrementIds(bool vertices, bool elements, ULInt inc = 1);
		virtual void clear();
		virtual void clearElements();

		virtual void execute();

	protected:

		Performer::IdManager *idManager;

		UInt dimension;
		Real minElementSize;

		Data::VertexMap verticesMap;

		bool sorted;

	};
}

#endif //#ifndef _MJMESH_BOUNDARY_BUILDER_H_
