#ifndef _PMTOOLS_WORKER_MAIN_DRIVE_H_
#define _PMTOOLS_WORKER_MAIN_DRIVE_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "PMJMesh/WorkerMainDrive.h"
#include "MTools/MainDrive.h"
#include "OctTree.hpp"

namespace PMTools
{
	class WorkerMainDrive : public PMJMesh::WorkerMainDrive, public MTools::MainDrive
	{
	public:

		WorkerMainDrive(Parallel::Communicator *comm, UInt status = 0);
		virtual ~WorkerMainDrive();

#if USE_GUI
		using MTools::MainDrive::setMesh;
#endif //#if USE_GUI

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual void setEnableBackTracking(bool enableBackTracking);

		virtual void setFtree(const FTools::OctTree *ftree);

		virtual void setUsingUniformRefinement(bool usingUniformRefinement);

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &inputFileName) const;

		virtual bool loadOutputFile(const std::string &outputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual void execute();

	protected:

	    virtual PMTools::MeshGenerator *makeMeshGenerator() const;

		virtual bool executeMeshGenerator();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeOtherStuff;
		double timeToMToolsConversion;
		double timeFromMToolsConversion;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	protected:

		bool enableBackTracking;

		const FTools::OctTree *ftree;
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_WORKER_MAIN_DRIVE_H_
