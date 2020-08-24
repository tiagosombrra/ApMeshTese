#include "CRAbMesh/Thread.h"

#if USE_GUI

#include "CRAbMesh/Module.h"
#include "CRAbMesh/Canvas.h"
#include "Performer/MainDrive.h"

using namespace CRAbMesh;

DECLARE_EVENT_TYPE(EVT_THREAD, -1)

DEFINE_EVENT_TYPE(EVT_THREAD)

CRAbMesh::Thread::Thread(wxEvtHandler *parent, Module *module) :
	wxThread()
{
	this->parent = parent;
	//this->glContext = NULL;

	this->setModule(module);
}

/*CRAbMesh::Thread::~Thread()
{
	//delete this->glContext;
}*/

void CRAbMesh::Thread::setModule(Module *module)
{
	this->module = module;
}

Module *CRAbMesh::Thread::getModule() const
{
	return this->module;
}

wxThread::ExitCode CRAbMesh::Thread::Entry()
{
	/*if (!this->glContext)
	{
		this->glContext = new wxGLContext(static_cast<CRAbMesh::Canvas *>(this->parent), static_cast<CRAbMesh::Canvas *>(this->parent)->glContext);
	}

	this->glContext->SetCurrent(*static_cast<CRAbMesh::Canvas *>(this->parent));*/

	int status = SUCCESS;

	while (!TestDestroy())
	{
		if ((!this->module) || (!this->module->GetDrive()))
		{
			status = ERROR;

			break;
		}

		static_cast<CRAbMesh::Canvas *>(this->parent)->RenderExclusively(true, true, true);

		this->module->GetDrive()->execute();

		static_cast<CRAbMesh::Canvas *>(this->parent)->RenderExclusively(true, false, true);

		if ((this->module->GetDrive()->getStatus().getType() == this->module->GetDrive()->success.getType()) ||
			(this->module->GetDrive()->getStatus().getType() == this->module->GetDrive()->notInit.getType()))
		{
			status = SUCCESS;
		}
		else if (this->module->GetDrive()->getStatus().getType() >= this->module->GetDrive()->error.getType())
		{
			status = ERROR;
		}
		else
		{
			status = PENDING;
		}

		break;
	}

	this->module = NULL;

	wxCommandEvent evt(EVT_THREAD, 0);

	evt.SetInt(status);

	parent->AddPendingEvent(evt);

	return NULL;
}

#endif //#if USE_GUI

