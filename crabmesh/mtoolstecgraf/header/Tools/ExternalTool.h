#ifndef _TOOLS_EXTERNAL_TOOL_H_
#define _TOOLS_EXTERNAL_TOOL_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"

namespace Tools
{
	class ExternalTool : public Tools::Tool
	{
	public:

		ExternalTool(const Tool *tool = NULL);
		virtual ~ExternalTool();

		virtual void setTool(const Tool *tool) const;

	protected:

		virtual void setMeasure(UInt current, UInt total) const;

	protected:

		mutable const Tool *tool;
	};
}

#endif //#ifndef _TOOLS_EXTERNAL_TOOL_H_
