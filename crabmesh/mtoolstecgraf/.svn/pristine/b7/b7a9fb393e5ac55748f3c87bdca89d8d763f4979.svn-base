#ifndef _CRAB_MESH_REDIRECTOR_H_
#define _CRAB_MESH_REDIRECTOR_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

namespace CRAbMesh
{
	class Redirector : public wxEvtHandler
	{
	public:

		Redirector(std::ostream *ostream, wxTextCtrl *tcLog = NULL, bool start = false);
		Redirector(std::FILE *fstream, wxTextCtrl *tcLog = NULL, bool start = false);
		Redirector(wxTextCtrl *tcLog = NULL);
		~Redirector();

		void SetMsecs(int msecs);

		void Start(wxTextCtrl *tcLog, std::ostream *ostream);
		void Start(wxTextCtrl *tcLog, std::FILE *fstream);
		void Start(wxTextCtrl *tcLog);
		void End(bool restart = false);

		void LogText(bool restart = false);

		void Enable(bool enabled = true);
		void Disable();

		static void RemoveFiles();

		void vprint(char *str);
		int print(char *str);
		int printf(const char *str, va_list valist);
		int printf(const char *str, ...);
		int operator()(const char *str, ...);
		void operator()(char *str);

		template <typename T>
		void operator<<(T t);

	protected:

		void MakeFilename();

		wxString GetText(bool end = false, bool restart = false);

		void OnTimer(wxTimerEvent &event);

	protected:

		int msecs;

		wxTextCtrl *tcLog;

		std::streambuf *oldbuffer;
		std::filebuf *newbuffer;

		wxTimer timer;

		std::string filename;

		static const std::string pattern;

		std::FILE *fstream;
		std::ostream *ostream;

		wxString text;

		bool enabled;
	};
}

template <typename T>
void CRAbMesh::Redirector::operator<<(T t)
{
    if (this->tcLog)
    {
        this->text << t;

        this->LogText(false);
    }
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_REDIRECTOR_H_
