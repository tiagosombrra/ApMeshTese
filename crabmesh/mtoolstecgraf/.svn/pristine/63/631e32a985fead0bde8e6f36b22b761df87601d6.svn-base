#ifndef _PMTOOLS2_MAIN_DRIVE_H_
#define _PMTOOLS2_MAIN_DRIVE_H_

#include "PMTools2/Definitions.h"

#if USE_MTOOLS

#include "MTools/MainDrive.h"
#include "PMJMesh2/MainDrive.h"

namespace PMTools2
{
    class MainDrive :
        virtual public MTools::MainDrive,
        virtual public PMJMesh2::MainDrive
    {
    public:

        MainDrive(Parallel::Communicator *comm = NULL);
        virtual ~MainDrive();

#if USE_NUMERICAL
        virtual void setHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool isHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

        virtual void setUsingUniformRefinement(bool usingUniformRefinement);
		virtual bool isUsingUniformRefinement() const;

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
        virtual void setUsingMtoolsOctreeForLoadEstimation(bool usingMtoolsOctreeForLoadEstimation);
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

        virtual bool loadFile();
		virtual bool saveFile() const;

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &outputFileName) const;

		virtual bool loadOutputFile(const std::string &inputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual std::string qualityString() const;

        virtual void execute();

    protected:

        virtual void clear();

        virtual bool executeInit();
		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
		virtual bool executeHotellingTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		//virtual bool executePartitionTreeGenerator();
		virtual bool executeParallelMeshGenerator();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

    protected:

        virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
        virtual Performer::TreeGenerator *makeTreeGenerator() const;
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

        virtual MTools::MshRegion3D *makeMshRegion() const;

        virtual Parallel::Communicator *makeCommunicator() const;

        virtual PMJMesh2::ParallelMeshGenerator *makeMeshGenerator() const;

        virtual PMJMesh::MJMeshTask *makeTask(PMJMesh::Partition *p, PMJMesh2::PartitionBSPTreeCell *cell) const;

//#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION
    protected:

        bool usingMtoolsOctreeForLoadEstimation;
//#endif //#if USE_MTOOLS_OCT_TREE_FOR_LOAD_ESTIMATION

    };
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS2_MAIN_DRIVE_H_
