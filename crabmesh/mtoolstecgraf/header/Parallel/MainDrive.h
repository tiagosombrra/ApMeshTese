#ifndef _PARALLEL_MAIN_DRIVE_H_
#define _PARALLEL_MAIN_DRIVE_H_

#include "Parallel/Definitions.h"
#include "Performer/MainDrive.h"

namespace Parallel
{
	class MainDrive : virtual public Performer::MainDrive
	{
	public:

		MainDrive(Communicator *comm = NULL, LoadBalancer *loadBalancer = NULL);
		virtual ~MainDrive();

		virtual void setComm(Communicator *comm);
		virtual Communicator *getComm() const;

		virtual void setLoadBalancer(LoadBalancer *loadBalancer);

	protected:

		virtual Communicator *makeCommunicator() const = 0;
		virtual LoadBalancer *makeLoadBalancer() const = 0;

	protected:

		Communicator *comm;
		LoadBalancer *loadBalancer;
	};
}

#endif //#ifndef _PARALLEL_MAIN_DRIVE_H_
