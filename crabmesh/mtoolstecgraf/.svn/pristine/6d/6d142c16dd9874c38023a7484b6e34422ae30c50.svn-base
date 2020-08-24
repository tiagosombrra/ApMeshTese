#ifndef _PMJMESH2_A_PRIORI_FRONT_PARTITIONER_H_
#define _PMJMESH2_A_PRIORI_FRONT_PARTITIONER_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/FrontPartitioner.h"

namespace PMJMesh2
{
    class APrioriFrontPartitioner : public PMJMesh2::FrontPartitioner
    {
    public:

        APrioriFrontPartitioner(UInt dimension, bool addInRejected, PMJMesh::MJMeshTask *task,
            const MJMesh::Front *front, UInt direction, Real coord);
        virtual ~APrioriFrontPartitioner();

        virtual void execute();

    protected:

        virtual bool optIn(const Data::Box *box, const Data::Point *point) const;
        virtual bool optOut(const Data::Box *box, const Data::Point *point) const;

        virtual bool completelyIn(const Data::Box *box, const MJMesh::FrontElement *e) const;
        virtual bool out(const Data::Box *box, const MJMesh::FrontElement *e) const;

    protected:

        UInt direction;
        Real coord;
        bool less;
    };
}

#endif //#ifndef _PMJMESH2_A_PRIORI_FRONT_PARTITIONER_H_
