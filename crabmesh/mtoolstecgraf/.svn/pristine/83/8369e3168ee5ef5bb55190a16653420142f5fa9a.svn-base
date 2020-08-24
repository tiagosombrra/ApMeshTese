#ifndef _CRAB_MESH_THREAD_H_
#define _CRAB_MESH_THREAD_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/thread.h>
//#include <wx/glcanvas.h>

namespace CRAbMesh
{
	class Thread : public wxThread
	{
	public:

		Thread(wxEvtHandler *parent, Module *module);
		//virtual ~Thread();

		virtual void setModule(Module *module);
		virtual Module *getModule() const;

		virtual wxThread::ExitCode Entry();

	public:

		enum
		{
			SUCCESS = 0,
			PENDING,
			ERROR = 100
		};

	protected:

		wxEvtHandler *parent;
		//wxGLContext *glContext;

		Module *module;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_THREAD_H_
