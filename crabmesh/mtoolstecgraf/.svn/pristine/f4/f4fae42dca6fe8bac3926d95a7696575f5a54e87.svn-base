#ifndef _PERFORMER_BUILDER_H_
#define _PERFORMER_BUILDER_H_

#include "Performer/Definitions.h"
#include "Performer/Status.h"

namespace Performer
{
	class Statused
	{
	public:

		virtual void setStatus(const Status &status);
		virtual const Status &getStatus() const;

	protected:

		mutable Status status;

	};

	class Builder : virtual public Statused
	{
	public:

		Builder();
		virtual ~Builder();

		virtual void execute() = 0;
	};
}

#endif //#ifndef _PERFORMER_BUILDER_H_
