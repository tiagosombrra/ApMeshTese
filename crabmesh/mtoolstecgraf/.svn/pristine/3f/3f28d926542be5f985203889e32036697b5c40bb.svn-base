#ifndef _PMJMESH2_WORKER_MAIN_DRIVE_H_
#define _PMJMESH2_WORKER_MAIN_DRIVE_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh/WorkerMainDrive.h"

namespace PMJMesh2
{
    class WorkerMainDrive : public PMJMesh::WorkerMainDrive
    {
    public:

        WorkerMainDrive(Parallel::Communicator *comm, UInt status = 0);
		virtual ~WorkerMainDrive();

		virtual void setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition);
		virtual void setCreateCrackInInnerFront(bool createCrackInInnerFront);
		virtual void setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront);
		virtual void setFixateFront(bool fixateFront);
		virtual void setShouldAdvanceFront(bool shouldAdvanceFront);

    protected:

        virtual void executeFrontGenerator();
        virtual void executeFrontMerger();

        virtual bool executeFrontGeometryAdvancer();
        virtual bool executeFrontTopologyAdvancer();
        virtual bool executeFrontBackTrackingAdvancer();
        //virtual bool executeMeshImprover();

    protected:

        virtual MJMesh::GeometryFrontAdvancer *makeFrontGeometryAdvancer() const;
        virtual AuxiliaryFrontGenerator *makeFrontGenerator();
        virtual MJMesh::MeshImprover *makeMeshImprover() const;

    protected:

        AuxiliaryFrontGenerator *frontGenerator;

    protected:

        bool createInnerFrontInDecomposition;
        bool createCrackInInnerFront;
		bool displaceVerticesInInnerFront;
		bool fixateFront;
		bool shouldAdvanceFront;

    };
}

#endif //#ifndef _PMJMESH2_WORKER_MAIN_DRIVE_H_
