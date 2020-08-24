#ifndef _PARALLEL_BUILDER_H_
#define _PARALLEL_BUILDER_H_

#include "Parallel/Definitions.h"
#include "Performer/Builder.h"

namespace Parallel
{
	class Builder : virtual public Performer::Builder
	{
	public:

		Builder(Communicator *comm = NULL);
		virtual ~Builder();

		virtual void setComm(Communicator *comm);
		virtual Communicator *getComm() const;

	protected:

		Communicator *comm;
	};
}

#endif //#ifndef _PARALLEL_BUILDER_H_
