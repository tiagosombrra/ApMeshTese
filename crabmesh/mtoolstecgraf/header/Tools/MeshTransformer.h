#ifndef _TOOLS_MESH_TRANSFORMER_H_
#define _TOOLS_MESH_TRANSFORMER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/MeshTransformer.h"

namespace Tools
{
	class MeshTransformer :
		public Performer::MeshTransformer,
		public Tools::Tool
	{
	public:

		MeshTransformer();
		virtual ~MeshTransformer();

		virtual void execute();

	protected:

		virtual void applyTranslation(Data::Point *p) const;
		virtual void applyScale(Data::Point *p) const;
		virtual void applyRotation(Data::Point *p) const;

		virtual void applyTransform(Data::Point *p, Real *matrix[4]) const;

	};
}

#endif //#ifndef _TOOLS_MESH_TRANSFORMER_H_
