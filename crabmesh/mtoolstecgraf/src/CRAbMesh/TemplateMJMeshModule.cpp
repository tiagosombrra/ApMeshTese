#include "CRAbMesh/TemplateMJMeshModule.h"

#if USE_GUI

#include "TemplateMJMesh/MainDrive.h"

using namespace TemplateMJMesh;

CRAbMesh::TemplateMJMeshModule::TemplateMJMeshModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MJMeshModule::MJMeshModule(frame, canvas, false)
{
	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}
}

CRAbMesh::TemplateMJMeshModule::~TemplateMJMeshModule()
{

}

wxString CRAbMesh::TemplateMJMeshModule::Name() const
{
	return _("Template-based JMesh (Markos version)");
}

void CRAbMesh::TemplateMJMeshModule::MakeMainDrive()
{
	this->drive = new TemplateMJMesh::MainDrive();
}

#endif //#if USE_GUI
