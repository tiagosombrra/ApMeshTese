#ifndef _TOOLS_TOOL_H_
#define _TOOLS_TOOL_H_

#include "Tools/Definitions.h"
#include "Performer/Builder.h"

namespace Tools
{
	class Tool : public Performer::Builder
	{
	friend
		class Tools::ExternalTool;

	public:

		Tool();
		virtual ~Tool();

		virtual Data::UI2Tuple measure() const;
		virtual UInt measure(UInt i) const;

	protected:

		virtual void startMeasure(UInt total = 1) const;
		virtual void setMeasure(UInt current, UInt total) const;
		virtual void addMeasure(UInt current, UInt total) const;
		virtual void subtractMeasure(UInt current, UInt total) const;
		virtual void endMeasure() const;

		virtual void startTiming(UInt size = 0) const;
		virtual void timing(std::ostream *str, const std::string &message = "") const;
		virtual void timing(const std::string &message = "") const;

	protected:

		mutable UInt current;
		mutable UInt total;

#if USE_TIME_MEASUREMENTS
		mutable Real tstart, tend, ttime, ttotal, tstep;
#endif //#if USE_TIME_MEASUREMENTS
		mutable UInt size, count;
	};
}

#endif //#ifndef _TOOLS_TOOL_H_
