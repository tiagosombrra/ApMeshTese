#ifndef _PMJMESH_MESH_TRANSFERABLE_H_
#define _PMJMESH_MESH_TRANSFERABLE_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Transferable.h"

namespace PMJMesh
{
	class MeshTransferable : public Parallel::Transferable
	{
	public:

		MeshTransferable();
		MeshTransferable(UInt dimension, Data::Mesh *mesh = NULL);
		virtual ~MeshTransferable();

		virtual void setMesh(Data::Mesh *mesh);
		virtual Data::Mesh *getMesh() const;

		//virtual void setIdManager(Performer::IdManager *idManager);
		//virtual Performer::IdManager *getIdManager() const;

		virtual UInt packSize(const Parallel::Communicator *comm, ULInt numVertices, ULInt numFrontElements, ULInt numElements) const;
		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			const Data::Mesh *mesh) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

	protected:

		Data::Mesh *mesh;

		UInt dimension;

		//Performer::IdManager *idManager;

	};
}

#endif //#ifndef _PMJMESH_MESH_TRANSFERABLE_H_
