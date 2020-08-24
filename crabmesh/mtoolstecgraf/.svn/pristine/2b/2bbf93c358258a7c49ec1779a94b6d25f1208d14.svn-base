#include "CRAbMesh/Redirector.h"

#if USE_GUI

#include <wx/dir.h>

const std::string CRAbMesh::Redirector::pattern = "CRAbMeshTmpXXXXXX";

CRAbMesh::Redirector::Redirector(std::ostream *ostream, wxTextCtrl *tcLog, bool start)
{
	//this->pattern = "CRAbMeshTmpXXXXXX";
	this->oldbuffer = NULL;
	this->newbuffer = NULL;
	this->ostream = ostream;
	this->tcLog = tcLog;
	this->enabled = true;

	this->fstream = NULL;

	this->msecs = 1000;

	this->timer.SetOwner(this, wxID_ANY);

	this->Connect(this->timer.GetId(), wxEVT_TIMER, wxTimerEventHandler(Redirector::OnTimer));

	if (start)
	{
		this->Start(this->tcLog, this->ostream);
	}
}

CRAbMesh::Redirector::Redirector(std::FILE *fstream, wxTextCtrl *tcLog, bool start)
{
	//this->pattern = "CRAbMeshTmpXXXXXX";
	this->fstream = fstream;
	this->tcLog = tcLog;
	this->enabled = true;

	this->oldbuffer = NULL;
	this->newbuffer = NULL;
	this->ostream = NULL;

	this->msecs = 1000;

	this->timer.SetOwner(this, wxID_ANY);

	this->Connect(this->timer.GetId(), wxEVT_TIMER, wxTimerEventHandler(Redirector::OnTimer));

	if (start)
	{
		this->Start(this->tcLog, this->fstream);
	}
}

CRAbMesh::Redirector::Redirector(wxTextCtrl *tcLog)
{
    this->fstream = NULL;
	this->tcLog = tcLog;
	this->enabled = true;

	this->oldbuffer = NULL;
	this->newbuffer = NULL;
	this->ostream = NULL;

	this->msecs = 1000;

	this->timer.SetOwner(this, wxID_ANY);

	this->Connect(this->timer.GetId(), wxEVT_TIMER, wxTimerEventHandler(Redirector::OnTimer));
}

CRAbMesh::Redirector::~Redirector()
{
	if (this->fstream)
	{
		std::fclose(this->fstream);
	}

	if (this->newbuffer)
	{
		this->ostream->rdbuf(this->oldbuffer);

		this->newbuffer->close();

		delete this->newbuffer;
	}

	if (!this->filename.empty())
	{
		std::remove(this->filename.c_str());
	}
}

void CRAbMesh::Redirector::SetMsecs(int msecs)
{
	this->msecs = msecs;
}

wxString CRAbMesh::Redirector::GetText(bool end, bool restart)
{
	if (!this->enabled)
	{
		return wxString();
	}

	if (end)
	{
		this->End(restart);
	}

	wxString text;

	while (!this->text.IsEmpty())
	{
		wxString line = this->text.BeforeFirst('\n');

		this->text = this->text.AfterFirst('\n');

		text << line.Trim(false).Trim(true) << _("\n");
	}

	return text;
}

void CRAbMesh::Redirector::MakeFilename()
{
	if (this->filename.empty())
	{
		char s[this->pattern.length() + 1];
		this->pattern.copy(s, this->pattern.length());
		s[this->pattern.length()] = '\0';

		int code = -1;
		while (code == -1)
		{
			code = mkstemp(s);
		}

		this->filename = s;
	}
}

void CRAbMesh::Redirector::Start(wxTextCtrl *tcLog, std::ostream *ostream)
{
	if (!this->enabled)
	{
		return;
	}

	if (ostream)
	{
		this->ostream = ostream;
	}

	this->fstream = NULL;

	if (tcLog)
	{
		this->tcLog = tcLog;
	}

	this->MakeFilename();

	this->oldbuffer = this->ostream->rdbuf();

	this->newbuffer = new std::filebuf();

	this->newbuffer->open(this->filename.c_str(), std::ios_base::out);

	this->ostream->rdbuf(this->newbuffer);

	if (!this->timer.IsRunning())
	{
		this->timer.Start(this->msecs);
	}
}

void CRAbMesh::Redirector::Start(wxTextCtrl *tcLog, std::FILE *fstream)
{
	if (!this->enabled)
	{
		return;
	}

	if (fstream)
	{
		this->fstream = fstream;
	}

	this->ostream = NULL;

	if (tcLog)
	{
		this->tcLog = tcLog;
	}

	this->MakeFilename();

	std::freopen(this->filename.c_str(), "w", this->fstream);

	if (!this->timer.IsRunning())
	{
		this->timer.Start(this->msecs);
	}
}

void CRAbMesh::Redirector::Start(wxTextCtrl *tcLog)
{
    if (!this->enabled)
    {
        return;
    }

    this->ostream = NULL;
    this->fstream = NULL;

    if (tcLog)
	{
		this->tcLog = tcLog;
	}

	/*if (!this->timer.IsRunning())
	{
		this->timer.Start(this->msecs);
	}*/
}

void CRAbMesh::Redirector::End(bool restart)
{
	if (!this->enabled)
	{
		return;
	}

	if (this->ostream)
	{
		this->ostream->rdbuf(this->oldbuffer);

		this->oldbuffer = NULL;

		this->newbuffer->close();

		delete this->newbuffer;

		this->newbuffer = NULL;
	}
	else if (this->fstream)
	{
		std::fflush(this->fstream);
		std::fclose(this->fstream);
	}

	if (!this->filename.empty())
    {
        std::ifstream file(this->filename.c_str());

        if ((file.is_open()) && (file.good()))
        {
            this->text.Clear();

            while (!file.eof())
            {
                std::string str;
                std::getline(file, str);

                wxString stmp(str.c_str(), wxMBConvLibc());

                if (file.eof())
                {
                    break;
                }

                this->text << stmp << _("\n");

                //std::cout << this->text << std::endl;
            }

            file.close();

            std::remove(this->filename.c_str());

            //this->filename.clear();
        }
    }

	if (restart)
	{
		if (this->ostream)
		{
			this->Start(this->tcLog, this->ostream);
		}
		else if (this->fstream)
		{
			this->Start(this->tcLog, this->fstream);
		}
		else
        {
            this->Start(this->tcLog);
        }
	}
	else if (this->timer.IsRunning())
	{
		this->timer.Stop();
	}
}

void CRAbMesh::Redirector::LogText(bool restart)
{
	if ((!this->enabled) | (!this->tcLog))
	{
		return;
	}

	wxString str = this->GetText(true, restart);

	if (!str.IsEmpty())
	{
		(*this->tcLog) << str;
	}
}

void CRAbMesh::Redirector::Enable(bool enabled)
{
	this->enabled = enabled;
}

void CRAbMesh::Redirector::Disable()
{
	this->Enable(false);
}

void CRAbMesh::Redirector::RemoveFiles()
{
	//std::cout << pattern << std::endl;

	std::string pattern = CRAbMesh::Redirector::pattern.substr(0, CRAbMesh::Redirector::pattern.size() - 6).append("*");

	wxString filespec(pattern.c_str(), wxMBConvLibc());
	wxArrayString files;

	UInt numFiles = static_cast<UInt>(wxDir::GetAllFiles(wxGetCwd(), &files, filespec, wxDIR_FILES));

	for (UInt i = 0; i < numFiles; i++)
	{
		std::string file(files[i].mb_str(wxMBConvLibc()));

		std::remove(file.c_str());
	}
}

void CRAbMesh::Redirector::OnTimer(wxTimerEvent &WXUNUSED(event))
{
	this->LogText((this->ostream) || (this->fstream));
}

void CRAbMesh::Redirector::vprint(char *str)
{
    this->print(str);
}

int CRAbMesh::Redirector::print(char *str)
{
    return this->printf(str, NULL);
}

int CRAbMesh::Redirector::printf(const char *str, va_list valist)
{
    if (!this->tcLog)
    {
        return 0;
    }

    wxString string(str, wxMBConvLibc());

    int ret = valist ? this->text.PrintfV(string, valist) : this->text.Printf(string);

    this->QueueEvent(new wxTimerEvent(this->timer));

    return ret;
}

int CRAbMesh::Redirector::printf(const char *str, ...)
{
    va_list valist;

    va_start(valist, str);

    int ret = this->printf(str, valist);

    va_end(valist);

    return ret;
}

int CRAbMesh::Redirector::operator()(const char *str, ...)
{
    va_list valist;

    va_start(valist, str);

    int ret = this->printf(str, valist);

    va_end(valist);

    return ret;
}

void CRAbMesh::Redirector::operator()(char *str)
{
    this->vprint(str);
}

#endif //#if USE_GUI
