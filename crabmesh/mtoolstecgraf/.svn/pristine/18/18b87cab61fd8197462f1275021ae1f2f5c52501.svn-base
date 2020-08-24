#ifndef _TOOLS_TECGRAF_BOUNDARY_BUILDER_H_
#define _TOOLS_TECGRAF_BOUNDARY_BUILDER_H_

#include "Tools/Definitions.h"
#include "TecGraf/BoundaryBuilder.h"

#if USE_TECGRAF

#include "Tools/Tool.h"

namespace Tools
{
	class TecGrafBoundaryBuilder :
		public TecGraf::BoundaryBuilder,
		public Tools::Tool
	{
	public:

		TecGrafBoundaryBuilder(bool sorted);
		virtual ~TecGrafBoundaryBuilder();

		virtual void setModel(TecGraf::Model *model);
		virtual TecGraf::Model *getModel() const;

		virtual void setMakeMeshes(bool makeMeshes);

		virtual const Data::BoundaryList &getBoundaries() const;
		virtual const Data::MeshList &getMeshes() const;
		virtual const Data::Point3DObjList &getTranslations() const;

		virtual void execute();

	protected:

		virtual Data::Mesh *makeMesh() const;

	protected:

		TecGraf::Model *model;

		bool makeMeshes;

		Data::BoundaryList boundaries;
		Data::MeshList meshes;
		Data::Point3DObjList translations;
	};
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TOOLS_TECGRAF_BOUNDARY_BUILDER_H_
