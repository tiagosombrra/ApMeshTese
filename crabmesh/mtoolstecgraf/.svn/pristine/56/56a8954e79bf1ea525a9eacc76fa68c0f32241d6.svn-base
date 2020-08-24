#ifndef _PMTOOLS_MAIN_DRIVE_H_
#define _PMTOOLS_MAIN_DRIVE_H_

#include "PMTools/Definitions.h"

#if (USE_MTOOLS)

#include "PMJMesh/MainDrive.h"
#include "MTools/MainDrive.h"

#include "PMTools/MshRegion3D.h"

namespace PMTools
{
	class MainDrive : virtual public MTools::MainDrive, virtual public PMJMesh::MainDrive
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

		virtual const Data::Boundary *getBoundary() const;

		virtual void clear();

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual std::string qualityString() const;

		virtual void execute();

	protected:

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &outputFileName) const;

		virtual bool loadOutputFile(const std::string &inputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual bool executeInit();
		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
		virtual bool executeHotellingTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		virtual bool executeFrontGeometryAdvancer();
		virtual bool executeFrontBackTrackingAdvancer();
		//virtual bool executeMeshBuilder();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

	protected:

		//virtual PMJMesh::InputData *makeInputData() const;
		//virtual PMJMesh::InputData *makeAndFillInputData() const;
		//virtual void fillWithInputData(const PMJMesh::InputData *input);

		virtual Parallel::Communicator *makeCommunicator() const;
		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
		virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual MTools::MshRegion3D *makeMshRegion() const;
		virtual PMJMesh::TaskManager *makeTaskManager() const;
		virtual MJMesh::BackTrackingFrontAdvancer *makeFrontBackTrackingAdvancer() const;

#if DEBUG_TIMING_PMTOOLS
	protected:

		Real timeInit,
			timeBoundaryBuilder,
#if USE_NUMERICAL
			timeHotellingTransformer,
#endif //#if USE_NUMERICAL
			timeBuildAdvFront,
			timeTreeGeneratorGeneration,
			timeTreeGeneratorConstruction,
			timeTreeGeneratorClassification,
			timeTreeGeneratorOthers,
			timeMeshGeneratorMasterTreeDecomposition,
			timeMeshGeneratorMasterFrontDecomposition,
			timeMeshGeneratorWorkerMeshGeneration,
			timeMeshGeneratorWorkerTreeClassification,
			timeMeshGeneratorWorkerToMToolsConversion,
			timeMeshGeneratorWorkerFromMToolsConversion,
			timeMeshGeneratorWorkerOthers,
			timeMeshGeneratorMasterMeshUpdate,
			timeMeshGeneratorMasterTreeShift,
			timeMeshGeneratorMasterFinalization,
			timeMeshGeneratorMasterOthers,
			timeMeshBackTrackerBackTracking,
			timeMeshBackTrackerToMToolsConversion,
			timeMeshBackTrackerFromMToolsConversion,
			timeMeshBackTrackerOthers,
#if USE_NUMERICAL
			timeHotellingBackTransformer,
#endif //#if USE_NUMERICAL
			timeOverhead;
#endif //#if DEBUG_TIMING_PMTOOLS

	};
}

#endif //#if (USE_MTOOLS)

#endif //#ifndef _PMTOOLS_MAIN_DRIVE_H_
