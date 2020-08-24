#ifndef _PMJMESH_BOUNDARY_TRANSFERABLE_H_
#define _PMJMESH_BOUNDARY_TRANSFERABLE_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Transferable.h"

namespace PMJMesh
{
	class BoundaryTransferable : public Parallel::Transferable
	{
	public:

		BoundaryTransferable();
		BoundaryTransferable(UInt dimension, Data::Boundary *boundary = NULL);
		virtual ~BoundaryTransferable();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual Data::Boundary *getBoundary() const;

		virtual void setIdManager(Performer::IdManager *idManager);
		virtual Performer::IdManager *getIdManager() const;

		virtual UInt packSize(const Parallel::Communicator *comm, ULInt numVertices, ULInt numElements) const;
		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			//const Data::VertexVector &vertices, const Data::FrontElementVector &elements) const;
			//const Data::VertexList &vertices, const Data::FrontElementList &elements) const;
			const Data::Boundary *boundary) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

	protected:

		Data::Boundary *boundary;

		UInt dimension;

		Performer::IdManager *idManager;
	};
}

#endif //#ifndef _PMJMESH_BOUNDARY_TRANSFERABLE_H_
