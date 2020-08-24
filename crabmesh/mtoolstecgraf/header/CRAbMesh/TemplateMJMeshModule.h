#ifndef _CRAB_MESH_TEMPLATE_MJMESH_MODULE_H_
#define _CRAB_MESH_TEMPLATE_MJMESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include "CRAbMesh/MJMeshModule.h"
#include "TemplateMJMesh/Definitions.h"

namespace CRAbMesh
{
	class TemplateMJMeshModule : public CRAbMesh::MJMeshModule
	{
	public:

		TemplateMJMeshModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~TemplateMJMeshModule();

		virtual wxString Name() const;

	protected:

		virtual void MakeMainDrive();

	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_TEMPLATE_MJMESH_MODULE_H_
