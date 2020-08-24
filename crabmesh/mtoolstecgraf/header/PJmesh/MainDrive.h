#ifndef _PJMESH_MAIN_DRIVE_H_
#define _PJMESH_MAIN_DRIVE_H_

#include "PJmesh/Definitions.h"

#include "PMJMesh/MainDrive.h"
#include "Jmesh/MainDrive.h"

#include "PJmesh/JmeshMainDef3D.h"

namespace PJmesh
{
	class MainDrive : virtual public Jmesh::MainDrive, virtual public PMJMesh::MainDrive
	{
	public:

		enum Versions
		{
			ADVAN = 1,
			SMALL,
		};

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
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		//virtual bool executeMeshBuilder();

	protected:

#if (!USE_WORKER_FILE_LOADER)
		virtual PMJMesh::InputData *makeInputData() const;
		virtual PMJMesh::InputData *makeAndFillInputData() const;
		virtual void fillWithInputData(const PMJMesh::InputData *input);
#endif //#if (!USE_WORKER_FILE_LOADER)

		virtual Parallel::Communicator *makeCommunicator() const;
		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
		virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual PMJMesh::TaskManager *makeTaskManager() const;
		virtual MJMesh::BackTrackingFrontAdvancer *makeFrontBackTrackingAdvancer() const;

	};
}

#endif //#ifndef _PJMESH_MAIN_DRIVE_H_
