#include "CRAbMesh/App.h"

#if USE_GUI

//#include <signal.h>
#include "CRAbMesh/Frame.h"
//#include "CRAbMesh/Redirector.h"

IMPLEMENT_APP(CRAbMesh::App)

/*Data::IntList signalized;

void action(int signum)
{
	if (std::find(signalized.begin(), signalized.end(), signum) != signalized.end())
	{
		return;
	}

	signalized.push_back(signum);

	//std::cout << "signum = " << signum << std::endl;

	CRAbMesh::Redirector::RemoveFiles();
}

void testAndSetAction(int sig, struct sigaction *handler, struct sigaction *old)
{
	std::cout << "sig = " << sig << std::endl;

	sigaction(sig, NULL, old);

	if (old->sa_handler != SIG_IGN)
	{
		sigaction(sig, handler, NULL);
	}
}

void registerHandler()
{
	struct sigaction handler, old;

	handler.sa_handler = action;
	sigemptyset(&handler.sa_mask);
	handler.sa_flags = 0;

	//testAndSetAction(SIGINT,  &handler, &old);
	//testAndSetAction(SIGHUP,  &handler, &old);
	//testAndSetAction(SIGTERM, &handler, &old);
	//testAndSetAction(SIGABRT, &handler, &old);
	//testAndSetAction(SIGILL,  &handler, &old);
	//testAndSetAction(SIGKILL, &handler, &old);
	//testAndSetAction(SIGPIPE, &handler, &old);
	//testAndSetAction(SIGQUIT, &handler, &old);
	testAndSetAction(SIGSEGV, &handler, &old);
	//testAndSetAction(SIGTSTP, &handler, &old);
	//testAndSetAction(SIGSYS,  &handler, &old);
}*/

CRAbMesh::App::App() :
	wxApp()
{
    this->frame = NULL;
}

CRAbMesh::App::~App()
{

}

bool CRAbMesh::App::OnInit()
{
	//registerHandler();

	int argc = this->argc;

	char **argv = new char*[argc];

	for (int i = 0; i < this->argc; i++)
	{
		wxString wxstr;

		wxstr << this->argv[i];

		std::string stdstr(wxstr.mb_str(wxMBConvLibc()));

		argv[i] = new char[stdstr.size()];

		for (unsigned int j = 0; j < stdstr.size(); j++)
		{
			argv[i][j] = stdstr[j];
		}
	}

	CRAbMesh::init(&argc, argv);

	for (int i = 0; i < this->argc; i++)
	{
		delete [] argv[i];
	}

	delete [] argv;

	wxInitAllImageHandlers();

	Frame *frame = new Frame();

	frame->LoadConfiguration();

	frame->Show();

	//gambiarra
	bool open = false;
	//end gambiarra

	for (int i = 1; i < this->argc; i++)
	{
		wxString path;

		path << this->argv[i];

		if (frame->OpenFile(path))
		{
			//gambiarra
			open = true;
			//end gambiarra

			break;
		}
	}

#if (!USE_ONLY_MTOOLS)
	//gambiarra
	if (!open)
	{
		frame->SetCurrentModule(moduleNames[1]);

		wxFileDialog *open = new wxFileDialog(frame, _("Open file"));

		open->Show();
		open->Hide();

		delete open;

		frame->SetCurrentModule(moduleNames[0]);
	}
	//end gambiarra
#endif //#if (!USE_ONLY_MTOOLS)

#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	frame->SetCurrentModule(moduleNames[1]);
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))

    return true;
}

int CRAbMesh::App::OnExit()
{
    CRAbMesh::finalize();

    return 0;
}

#endif //#if USE_GUI
