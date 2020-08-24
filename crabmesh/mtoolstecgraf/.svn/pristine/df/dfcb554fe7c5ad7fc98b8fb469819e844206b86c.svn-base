#ifndef _PMJMESH2_TASK_H_
#define _PMJMESH2_TASK_H_

#include "PMJMesh2/Definitions.h"
#include "Parallel/Task.h"

namespace PMJMesh2
{
    class Task
    {
    public:

        Task();
		virtual ~Task();

		virtual void setCell(PartitionBSPTreeCell *cell);
		virtual PartitionBSPTreeCell *getCell() const;

		virtual void setMeshes(const Data::MeshList &meshes);
		virtual const Data::MeshList &getMeshes() const;
		virtual void clearMeshes();

		virtual void add(Data::Mesh *mesh);
		virtual void remove(Data::Mesh *mesh);
		virtual bool have(const Data::Mesh *mesh);

#if USE_GUI
        virtual void setMids(const Data::Point3DObjList &mids);
        virtual const Data::Point3DObjList &getMids() const;
		virtual void clearMids();

		virtual void add(const Data::Point3D &mid);
#endif //#if USE_GUI

		virtual void setSendAdvanced(bool sendAdvanced);
		virtual void mergeAdvancedFrom(PMJMesh2::Task *task);
		virtual void setAdvancedFrom(PMJMesh2::Task *task);
		virtual void addAdvanced(const Data::FrontElement *fel);
		virtual void removeAdvanced(const Data::FrontElement *fel);
		virtual bool isAdvanced(const Data::FrontElement *fel);
		virtual void clearAdvanced();

		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

    protected:

        virtual UInt packSize(const Parallel::Communicator *comm, ULInt numAdvanced) const;

		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
#if USE_C__11
            const Data::ULIntHash &advancedIds
#else
            const Data::ULIntSet &advancedIds
#endif //#if USE_C__11
        ) const;

    protected:

        PartitionBSPTreeCell *cell;

        Data::MeshList meshes;
#if USE_GUI
        Data::Point3DObjList mids;
#endif //#if USE_GUI

		bool sendAdvanced;
#if USE_C__11
		Data::ULIntHash advancedIds;
#else
		Data::ULIntSet advancedIds;
#endif //#if USE_C__11
    };
}

#endif //#ifndef _PMJMESH2_TASK_H_
