#ifndef _PJMESH_WORKER_MAIN_DRIVE_H_
#define _PJMESH_WORKER_MAIN_DRIVE_H_

#include "PJmesh/Definitions.h"

#include "PMJMesh/WorkerMainDrive.h"
#include "Jmesh/MainDrive.h"

namespace PJmesh
{
	class WorkerMainDrive : public PMJMesh::WorkerMainDrive, public Jmesh::MainDrive
	{
	public:

		WorkerMainDrive(Parallel::Communicator *comm);
		virtual ~WorkerMainDrive();

		virtual void setVersion(int version);
		virtual void setFormat(int format);
		virtual void setFactor(double factor);
		virtual void setTol(double tol);
		virtual void setInternal(int internal);

#if USE_GUI
		using Jmesh::MainDrive::setMesh;
#endif //#if USE_GUI

		virtual void setMesh(MJMesh::Mesh *mesh);

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &inputFileName) const;

		virtual bool loadOutputFile(const std::string &outputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual void execute();

	protected:

		virtual bool executeMeshGenerator();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeToJmeshConversion;
		double timeFromJmeshConversion;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	};
}

#endif //#ifndef _PJMESH_WORKER_MAIN_DRIVE_H_
