#ifndef _PMJMESH_TREE_TRANSFERABLE_H_
#define _PMJMESH_TREE_TRANSFERABLE_H_

#include "PMJMesh/Definitions.h"
#include "Parallel/Transferable.h"

namespace PMJMesh
{
	class TreeTransferable : public Parallel::Transferable
	{
	public:

		TreeTransferable();
		TreeTransferable(UInt dimension, Data::BoxTree *tree);
		virtual ~TreeTransferable();

		virtual void setDimension(UInt dimension);
		virtual UInt getDimension() const;

		virtual void setTree(Data::BoxTree *tree);
		virtual Data::BoxTree *getTree() const;

		virtual UInt packSize(const Parallel::Communicator *comm, ULInt numCells) const;
		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			const Data::BoxTree *tree) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

	protected:

		UInt dimension;

		Data::BoxTree *tree;

	};
}

#endif //#ifndef _PMJMESH_TREE_TRANSFERABLE_H_
