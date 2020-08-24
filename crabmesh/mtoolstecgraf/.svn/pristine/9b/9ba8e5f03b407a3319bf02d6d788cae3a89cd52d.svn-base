#ifndef _PMTOOLS2_WORKER_MAIN_DRIVE_H_
#define _PMTOOLS2_WORKER_MAIN_DRIVE_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "PMTools/WorkerMainDrive.h"

namespace PMTools2
{
    class WorkerMainDrive : public PMTools::WorkerMainDrive
    {
    public:

        WorkerMainDrive(Parallel::Communicator *comm, UInt status = 0);
		virtual ~WorkerMainDrive();

		virtual void setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition);
		virtual void setCreateCrackInInnerFront(bool createCrackInInnerFront);
		virtual void setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront);
		virtual void setFixateFront(bool fixateFront);
		virtual void setShouldAdvanceFront(bool shouldAdvanceFront);

		virtual void setUseUniformSize(bool useUniformSize);

    protected:

        virtual PMTools::MeshGenerator *makeMeshGenerator() const;

        virtual void executeFrontGenerator();
        virtual void executeFrontMerger();

        virtual bool executeMeshGenerator();

    protected:

        virtual PMJMesh2::AuxiliaryFrontGenerator *makeFrontGenerator();

    protected:

        PMJMesh2::AuxiliaryFrontGenerator *frontGenerator;

    protected:

        bool createInnerFrontInDecomposition;
        bool createCrackInInnerFront;
		bool displaceVerticesInInnerFront;
		bool fixateFront;
		bool shouldAdvanceFront;

		bool useUniformSize;

    };
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_WORKER_MAIN_DRIVE_H_
