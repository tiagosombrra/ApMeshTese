#ifndef _TOOLS_HOTELLING_TRANSFORMER_H_
#define _TOOLS_HOTELLING_TRANSFORMER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "MJMesh/HotellingTransformer.h"

#if USE_NUMERICAL

namespace Tools
{
	class HotellingTransformer :
		public MJMesh::HotellingTransformer,
		public Tools::ExternalTool
	{
	public:

		HotellingTransformer(const Tool *tool = NULL);
		virtual ~HotellingTransformer();

		virtual void execute();

	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _TOOLS_HOTELLING_TRANSFORMER_H_
