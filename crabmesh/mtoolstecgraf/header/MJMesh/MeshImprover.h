#ifndef _MJMESH_MESH_IMPROVER_H_
#define _MJMESH_MESH_IMPROVER_H_

#include "MJMesh/Definitions.h"
#include "Performer/MeshImprover.h"

namespace MJMesh
{
	class MeshImprover : public Performer::MeshImprover
	{
	public:

		MeshImprover(UInt dimension, MJMesh::Mesh *mesh, MJMesh::Front *front,
			Data::BoxTree *tree, MJMesh::Boundary *boundary, UInt numImproves,
			Real smoothingFactor, bool localBackTrackingEnabled,
			bool meshSorted, UInt maxBackTracks);
		virtual ~MeshImprover();

		virtual void setDimension(UInt dimension);

		virtual void setFront(MJMesh::Front *front);

		virtual void setTree(Data::BoxTree *tree);

		virtual void setBoundary(MJMesh::Boundary *boundary);

		virtual void setSmoothingFactor(Real smoothingFactor);

		virtual void setMeshSorted(bool meshSorted);

		virtual void setLocalBackTrackingEnabled(bool localBackTrackingEnabled);

		virtual void setMaxBackTracks(UInt maxBackTracks);

		virtual void setIdManager(Performer::IdManager *idManager);

		virtual void execute();

	protected:

		UInt dimension;

		MJMesh::Front *front;
		Data::BoxTree *tree;
		MJMesh::Boundary *boundary;

		Real smoothingFactor;
		bool meshSorted;
		bool localBackTrackingEnabled;
		UInt maxBackTracks;

		Performer::IdManager *idManager;
	};
}

#endif //#ifndef _MJMESH_MESH_IMPROVER_H_
