#ifndef _CRAB_MESH_APP_H_
#define _CRAB_MESH_APP_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

namespace CRAbMesh
{
	class App : public wxApp
	{
	public:

		App();
		~App();

		bool OnInit();
		int OnExit();

	protected:

		Frame *frame;
	};
}

#endif //#if USE_GUI

#endif //#ifndef #ifndef _CRAB_MESH_APP_H_

