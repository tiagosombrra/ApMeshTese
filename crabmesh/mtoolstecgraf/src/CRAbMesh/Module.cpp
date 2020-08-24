#include "CRAbMesh/Module.h"

#if USE_GUI

#include "CRAbMesh/Camera.h"
#include "Performer/MainDrive.h"
#include "Data/Drawable.h"

using namespace CRAbMesh;

CRAbMesh::Module::Module(Frame *frame, Canvas *canvas)
{
	this->loadedFilename = "";
	this->handler = false;

	this->drive = NULL;
	this->camera = NULL;

	this->SetFrame(frame);
	this->SetCanvas(canvas);

	//this->Connect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(Module::OnAuiPaneClose));
}

CRAbMesh::Module::~Module()
{
	if (this->drive)
	{
		delete this->drive;
	}

	if (this->camera)
	{
		delete camera;
	}

	for (MenuVector::iterator iter = this->menus.begin();
		 iter != this->menus.end(); iter++)
	{
		delete (*iter);
	}

	this->menus.clear();

	for (ToolBarVector::iterator iter = this->toolbars.begin();
		 iter != this->toolbars.end(); iter++)
	{
		delete (*iter);
	}

	this->toolbars.clear();

	for (PanelVector::iterator iter = this->panels.begin();
		 iter != this->panels.end(); iter++)
	{
		(*iter)->Destroy();
	}

	this->panels.clear();
}

void CRAbMesh::Module::SetFrame(Frame *frame)
{
	this->frame = frame;
}

void CRAbMesh::Module::SetCanvas(Canvas *canvas)
{
	this->canvas = canvas;
}

void CRAbMesh::Module::SetCamera(Camera *camera)
{
	if (this->camera)
	{
		delete this->camera;
	}

	this->camera = camera;
}

Camera *CRAbMesh::Module::GetCamera() const
{
	return this->camera;
}

void CRAbMesh::Module::SetHandler(bool handler)
{
	this->handler = handler;
}

bool CRAbMesh::Module::IsHandler() const
{
	return this->handler;
}

Performer::MainDrive *CRAbMesh::Module::GetDrive() const
{
	return this->drive;
}

unsigned int CRAbMesh::Module::GetMenuCount() const
{
	return static_cast<unsigned int>(this->menus.size());
}

wxMenu *CRAbMesh::Module::GetMenu(unsigned int i)
{
	wxString title;

	return this->GetMenu(i, title);
}

unsigned int CRAbMesh::Module::GetToolbarCount() const
{
	return static_cast<unsigned int>(this->toolbars.size());
}

wxAuiToolBar *CRAbMesh::Module::GetToolbar(unsigned int i)
{
	wxString title;

	return this->GetToolbar(i, title);
}

unsigned int CRAbMesh::Module::GetPanelCount() const
{
	return static_cast<unsigned int>(this->panels.size());
}

wxPanel *CRAbMesh::Module::GetPanel(unsigned int i)
{
	wxString title;

	return this->GetPanel(i, title);
}

bool CRAbMesh::Module::LoadFile(const std::string &filename)
{
	if (this->GetDrive())
	{
		this->GetDrive()->setInputFileName(filename);

		bool loaded = this->GetDrive()->loadFile();

		this->GetDrive()->setInputFileName("");

		if (loaded)
		{
			this->loadedFilename = filename;
		}

		return loaded;
	}

	return true;
}

bool CRAbMesh::Module::SaveCamera(const std::string &filename) const
{
	if (!this->camera)
	{
		return false;
	}

	std::ofstream file(filename.c_str());

	if (!file.is_open())
	{
		return false;
	}

	file << *this->camera << std::endl;

	file.flush();
	file.close();

	return true;
}

bool CRAbMesh::Module::LoadCamera(const std::string &filename)
{
	if (!this->camera)
	{
		return false;
	}

	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		return false;
	}

	file >> *this->camera;

	return true;
}

wxString CRAbMesh::Module::GetLoadedFilename() const
{
	return wxString(this->loadedFilename.c_str(), wxMBConvLibc());
}

const Data::DrawableList &CRAbMesh::Module::GetPicked() const
{
	return this->picked;
}

void CRAbMesh::Module::Clear(bool onlyDrive, bool destroying)
{
	this->picked.clear();

	if (onlyDrive)
	{
		delete this->drive;

		this->drive = NULL;

		if (!destroying)
		{
			this->MakeMainDrive();
		}
	}
	else
	{
		this->loadedFilename = "";
	}
}

void CRAbMesh::Module::OnTimer()
{

}

bool CRAbMesh::Module::OnLeftDown(int /*x*/, int /*y*/)
{
	return false;
}

bool CRAbMesh::Module::IsMouseMotionAllowed() const
{
	return true;
}

void CRAbMesh::Module::ChangeFrom(const Module */*module*/)
{

}

void CRAbMesh::Module::ChangeTo(const Module */*module*/)
{

}

Data::DrawableList CRAbMesh::Module::Pick(int x, int y, bool holding)
{
	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		(*iter)->unhighlight();
	}

	if (!holding)
	{
		if (this->picked.size() < 2)
		{
			this->PickReal(x, y);
		}
		else
		{
			this->PickIntersection(x, y);

			if (this->picked.empty())
			{
				this->PickReal(x, y);
			}
		}
	}
	else
	{
		if (this->picked.empty())
		{
			this->PickReal(x, y);

			if (!this->picked.empty())
			{
				this->picked.erase(++this->picked.begin(), this->picked.end());
			}
		}
		else
		{
			this->PickUnionFirst(x, y);

			if (this->picked.empty())
			{
				this->PickReal(x, y);
			}
		}
	}

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		(*iter)->highlight();
	}

	return this->picked;
}

void CRAbMesh::Module::PickReal(int x, int y)
{
	if (!this->camera)
	{
		this->picked.clear();

		return;
	}

	unsigned int size = this->PickBufferSize();

	if (size == 0)
	{
		this->picked.clear();

		return;
	}

	GLuint *buffer = new GLuint[size];

	glSelectBuffer(size, buffer);

	this->camera->StartPick(x, y);

	this->RenderPick();

	int hits = this->camera->EndPick();

	hits = (hits < 0) ? size/(Data::Drawable::numElems + 3) : hits;

	this->picked = this->Picked((hits < 0) ? 0 : hits, buffer);

	delete [] buffer;
}

void CRAbMesh::Module::PickIntersection(int x, int y)
{
	if (!this->camera)
	{
		this->picked.clear();

		return;
	}

	unsigned int size = this->PickBufferSize();

	if (size == 0)
	{
		this->picked.clear();

		return;
	}

	GLuint *buffer = new GLuint[size];

	glSelectBuffer(size, buffer);

	this->camera->StartPick(x, y);

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		(*iter)->drawPick();
	}

	int hits = this->camera->EndPick();

	hits = (hits < 0) ? size/(Data::Drawable::numElems + 3) : hits;

	this->picked = this->Picked(hits, buffer);

	delete [] buffer;
}

void CRAbMesh::Module::PickUnionFirst(int x, int y)
{
	if (!this->camera)
	{
		this->picked.clear();

		return;
	}

	unsigned int size = this->PickBufferSize();

	if (size == 0)
	{
		this->picked.clear();

		return;
	}

	GLuint *buffer = new GLuint[size];

	glSelectBuffer(size, buffer);

	this->camera->StartPick(x, y);

	this->RenderPick();

	int hits = this->camera->EndPick();

	hits = (hits < 0) ? size/(Data::Drawable::numElems + 3) : hits;

	Data::DrawableList picked = this->Picked(hits, buffer);

	delete [] buffer;

	if (picked.empty())
	{
		return;
	}

	Data::Drawable *first = picked.front();

	if (std::find(this->picked.begin(), this->picked.end(), first) == this->picked.end())
	{
		this->picked.push_back(first);
	}
}

/*void CRAbMesh::Module::MakeToolbars()
{
	this->toolbarsEnabled.resize(this->GetToolbarCount(), true);
}

void CRAbMesh::Module::MakePanels()
{
	this->toolbarsEnabled.resize(this->GetPanelCount(), true);
}

void CRAbMesh::Module::OnAuiPaneClose(wxAuiManagerEvent &event)
{
	for (unsigned int i = 0; i < this->GetToolbarCount(); i++)
	{
		if (event.pane->window == this->GetToolbar(i))
		{
			this->toolbarsEnabled[i] = false;

			return;
		}
	}

	for (unsigned int i = 0; i < this->GetPanelCount(); i++)
	{
		if (event.pane->window == this->GetPanel(i))
		{
			this->panelsEnabled[i] = false;

			return;
		}
	}
}*/

#endif //#if USE_GUI
