#include "CRAbMesh/JmeshModule.h"

#if USE_GUI

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/Boundary.h"

CRAbMesh::JmeshModule::JmeshModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule(frame, canvas)
{
	this->version = _("Advan");
	this->versions[_("Advan")] = Jmesh::MainDrive::ADVAN;
	this->versions[_("Small")] = Jmesh::MainDrive::SMALL;

#if USE_NUMERICAL
	this->hotellingTransformEnabled = false;
#endif //#if USE_NUMERICAL
	this->usingUniformRefinement = true;

	this->SetOption(VERTICES, true);
	this->SetOption(ELEMENTS, true);
	this->SetOption(FRONT_ELEMENTS, true);
	this->SetOption(TREE_CELLS, true);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, true, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, true, false);

	this->SetOption(DRAW_NORMAL, true, true);
	this->SetOption(DRAW_DISCRETIZATION, true, false);

	this->SetOption(DRAW_BOUNDARY, true, true);
	this->SetOption(DRAW_TREE, true, true);
	this->SetOption(DRAW_FRONT, false, false);
	this->SetOption(DRAW_MESH, true, true);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}
}

CRAbMesh::JmeshModule::~JmeshModule()
{

}

wxString CRAbMesh::JmeshModule::Name() const
{
	return _("Jmesh");
}

wxMenu *CRAbMesh::JmeshModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("&Configuration");
		break;
	case 1:
		title = _("&Execute");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::JmeshModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::JmeshModule::GetPanel(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("Configuration");
		break;
	case 1:
		title = _("Mesh items");
		break;
	case 2:
		title = _("Log");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->panels.size()) ? this->panels[i] : NULL;
}

bool CRAbMesh::JmeshModule::LoadFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	/*this->drive->setInputFileName(filename);

	bool loaded = this->drive->loadFile();

	this->drive->setInputFileName("");*/

	bool loaded = Module::LoadFile(filename);

	this->redirector.LogText();

	if (loaded)
	{
		//Module::LoadFile(filename);

		this->FillTree();

		//this->SetTolerance(Data::getTolerance());

		this->frame->GetViewModule()->SetDepthTestEnabled(true);
		this->frame->GetViewModule()->SetCullFaceEnabled(false);
	}

	return loaded;
}

bool CRAbMesh::JmeshModule::SaveFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	this->redirector.LogText();

	return saved;
}

void CRAbMesh::JmeshModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
#if (!USE_ONLY_MTOOLS)
		Jmesh::MainDrive *drive = dynamic_cast<Jmesh::MainDrive *>(this->drive);

		drive->setVer(this->GetVersion());

#if USE_NUMERICAL
		drive->setHotellingTransformEnabled(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
		drive->setUsingUniformRefinement(this->IsUsingUniformRefinement());
#endif //#if (!USE_ONLY_MTOOLS)
	}
}

void CRAbMesh::JmeshModule::Execute()
{
	this->redirector.Start(this->tcLog, stderr);

	this->frame->GetViewModule()->Execute();

	//disable items
	if (this->miConfAdvanVersion) this->miConfAdvanVersion->Enable(false);
	if (this->miConfSmallVersion) this->miConfSmallVersion->Enable(false);
#if USE_NUMERICAL
	if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable(false);

	if (this->rbVersions) this->rbVersions->Enable(false);

#if USE_NUMERICAL
	if (this->cbEnableHotellingTransform) this->cbEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable(false);

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	Data::setStepByStep(false);
}

void CRAbMesh::JmeshModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, pending);

	//this->gWait->SetValue(0);
	//this->gWait->Enable(false);

#if (!USE_ONLY_MTOOLS)
	Jmesh::MainDrive *drive = dynamic_cast<Jmesh::MainDrive *>(this->drive);

	if (clearing)
	{
		drive->clear();
	}

	this->redirector.LogText();

	if (pending)
	{
		this->canvas->Execute();
	}
	else
	{
		this->frame->Ready();
		this->FillTree();

		if (this->bExecute) this->bExecute->Enable();
		if (this->bClear) this->bClear->Enable();

		if (this->miExecuteExecute) this->miExecuteExecute->Enable();
		if (this->miExecuteClear) this->miExecuteClear->Enable();
		if (this->miExecuteResults) this->miExecuteResults->Enable();

		if (this->bExecute) this->bExecute->SetFocus();

		wxString s(drive->getStatus().getMessage().c_str(), wxMBConvLibc());

		this->frame->SetStatusText(s);

		if ((!this->sbsQuality) &&
			(drive->getStatus().getType() == drive->success.getType()))
		{
			this->AddQualityString();
		}
	}
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::JmeshModule::Clear(bool onlyDrive, bool destroying)
{
	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		/*Jmesh::MainDrive *drive = dynamic_cast<Jmesh::MainDrive *>(this->drive);

		drive->clear();*/

		return;
	}

	//clear tree items
	this->ClearTree();

	if (this->tcLog)
	{
        this->tcLog->Clear();
    }

    //enable and reset items
    if (this->miConfAdvanVersion) this->miConfAdvanVersion->Enable();
    if (this->miConfSmallVersion) this->miConfSmallVersion->Enable();
#if USE_NUMERICAL
    if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable();
#endif //#if USE_NUMERICAL
    if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable();

	if (this->rbVersions) this->rbVersions->Enable();
#if USE_NUMERICAL
	if (this->cbEnableHotellingTransform) this->cbEnableHotellingTransform->Enable();
#endif //#if USE_NUMERICAL
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable();

	//remove string, if any
	if (this->sbsQuality)
	{
		wxBoxSizer *pConfSizer = static_cast<wxBoxSizer *>(this->GetPanel(0)->GetSizer());

		this->sbsQuality->Clear(true);

		pConfSizer->Remove(this->sbsQuality);

		this->sbsQuality = NULL;

		pConfSizer->Layout();
		this->GetPanel(0)->FitInside();
	}
}

void CRAbMesh::JmeshModule::SetVersion(wxString version)
{
	this->version = version;

	this->rbVersions->SetStringSelection(this->version);

	if (this->version == _("Advan"))
	{
		this->miConfAdvanVersion->Check(true);
		//this->miConfSmallVersion->Check(false);
	}
	else if (this->version == _("Small"))
	{
		//this->miConfAdvanVersion->Check(false);
		this->miConfSmallVersion->Check(true);
	}
}

enum Jmesh::MainDrive::Versions CRAbMesh::JmeshModule::GetVersion() const
{
	VersionMap::const_iterator it = this->versions.find(this->version);

	return (it == this->versions.end()) ? Jmesh::MainDrive::ADVAN : (*it).second;
}

wxString CRAbMesh::JmeshModule::GetVersionString() const
{
	return this->version;
}

wxArrayString CRAbMesh::JmeshModule::GetVersions() const
{
	wxArrayString s;

	for (VersionMap::const_iterator iter = this->versions.begin();
		 iter != this->versions.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

#if USE_NUMERICAL
void CRAbMesh::JmeshModule::SetHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;

    this->miConfEnableHotellingTransform->Check(this->hotellingTransformEnabled);
    this->cbEnableHotellingTransform->SetValue(this->hotellingTransformEnabled);
}

bool CRAbMesh::JmeshModule::IsHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void CRAbMesh::JmeshModule::SetUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;

    this->miConfUseUniformRefinement->Check(this->usingUniformRefinement);
    this->cbUseUniformRefinement->SetValue(this->usingUniformRefinement);
}

bool CRAbMesh::JmeshModule::IsUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

void CRAbMesh::JmeshModule::SetGUIState()
{

}

void CRAbMesh::JmeshModule::MakeMainDrive()
{
#if USE_ONLY_MTOOLS
	this->drive = NULL;
#else
	this->drive = new Jmesh::MainDrive();
#endif //#if USE_ONLY_MTOOLS
}

void CRAbMesh::JmeshModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::JmeshModule::MakeToolbars()
{
	//Module::MakeToolbars();
}

void CRAbMesh::JmeshModule::MakePanels()
{
	this->tcLog = NULL;

	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
	this->panels.push_back(this->MakeLogPanel());

	//Module::MakePanels();
}

wxMenu *CRAbMesh::JmeshModule::MakeConfigurationMenu()
{
	//wxMenu *mConfiguration = new wxMenu();
	wxMenu *mConfiguration = MeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	this->miConfAdvanVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Advan version"));
    this->miConfSmallVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Small version"));

#if USE_NUMERICAL
    this->miConfEnableHotellingTransform = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Enable Hotelling transform"));
#endif //#if USE_NUMERICAL
    this->miConfUseUniformRefinement = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Generate a uniform mesh"));

    if (mConfiguration->GetMenuItemCount() != row)
	{
		mConfiguration->InsertSeparator(row++);
	}

#if USE_NUMERICAL
    this->miConfEnableHotellingTransform->Check(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
    this->miConfUseUniformRefinement->Check(this->IsUsingUniformRefinement());

    this->Connect(this->miConfAdvanVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnConfAdvanVersion));
    this->Connect(this->miConfSmallVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnConfSmallVersion));
#if USE_NUMERICAL
    this->Connect(this->miConfEnableHotellingTransform->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->miConfUseUniformRefinement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnConfUseUniformRefinement));

    return mConfiguration;
}

wxMenu *CRAbMesh::JmeshModule::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(JmeshModule::OnResults));

    return mExecute;
}

wxPanel *CRAbMesh::JmeshModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel

	//radio boxes
	//wxString value(this->GetVersionString().c_str(), wxMBConvLibc());

    this->rbVersions = new wxRadioBox(pConfiguration, wxID_ANY, _("Version"), wxDefaultPosition, wxDefaultSize, this->GetVersions(), wxRA_SPECIFY_ROWS);
#if USE_NUMERICAL
    this->cbEnableHotellingTransform = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable Hotelling transform"));
#endif //#if USE_NUMERICAL
    this->cbUseUniformRefinement = new wxCheckBox(pConfiguration, wxID_ANY, _("Generate a uniform mesh"));

    //buttons
    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

#if USE_NUMERICAL
    this->cbEnableHotellingTransform->SetValue(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
    this->cbUseUniformRefinement->SetValue(this->IsUsingUniformRefinement());

    this->bExecute->SetDefault();

    //Sizeres
    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    this->sbsQuality = NULL;

    //wxStaticBoxSizer *sbsWait = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Please wait"));
    //sbsWait->Add(this->gWait, 1, wxEXPAND | wxALL, 0);*/

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
	gbsConf->AddGrowableCol(0);
    gbsConf->Add(this->rbVersions,             wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#if USE_NUMERICAL
    gbsConf->Add(this->cbEnableHotellingTransform, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#endif //#if USE_NUMERICAL
    gbsConf->Add(this->cbUseUniformRefinement, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
    bsConf->Add(gbsConf,    0, wxEXPAND | wxALL, 1);
    bsConf->Add(sConf,      0, wxEXPAND | wxALL, 1);
    bsConf->Add(sbsExecute, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
    pConfiguration->SetSizer(bsConf, false);

    this->Connect(this->rbVersions->GetId(),             wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(JmeshModule::OnConfVersions));
#if USE_NUMERICAL
    this->Connect(this->cbEnableHotellingTransform->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(JmeshModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->cbUseUniformRefinement->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,  wxCommandEventHandler(JmeshModule::OnConfUseUniformRefinement));

    this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(JmeshModule::OnExecute));
    this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(JmeshModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::JmeshModule::MakeLogPanel()
{
	wxPanel *pLog = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 440), wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
	static_cast<wxScrolledWindow *>(pLog)->SetScrollRate(5, 5);

	this->tcLog = new wxTextCtrl(pLog, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_RICH);

	this->tcLog->SetFont(wxFont(this->tcLog->GetFont().GetPointSize(),
		wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	wxBoxSizer *bsLog = new wxBoxSizer(wxVERTICAL);
	bsLog->Add(this->tcLog, 1, wxEXPAND | wxALL, 1);
	pLog->SetSizer(bsLog);

	return pLog;
}

void CRAbMesh::JmeshModule::OnConfAdvanVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(_("Advan"));
}

void CRAbMesh::JmeshModule::OnConfSmallVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(_("Small"));
}

void CRAbMesh::JmeshModule::OnConfVersions(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(this->rbVersions->GetStringSelection());
}

#if USE_NUMERICAL
void CRAbMesh::JmeshModule::OnConfEnableHotellingTransform(wxCommandEvent &WXUNUSED(event))
{
	this->SetHotellingTransformEnabled(!this->IsHotellingTransformEnabled());
}
#endif //#if USE_NUMERICAL

void CRAbMesh::JmeshModule::OnConfUseUniformRefinement(wxCommandEvent &WXUNUSED(event))
{
	this->SetUsingUniformRefinement(!this->IsUsingUniformRefinement());
}

void CRAbMesh::JmeshModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::JmeshModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::JmeshModule::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}

/*void CRAbMesh::JmeshModule::OnTimer()
{
	this->redirector.LogText(true);
}*/

void CRAbMesh::JmeshModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::JmeshModule::GetDimension() const
{
	return 3;
}

void CRAbMesh::JmeshModule::SetTree(Data::BoxTree *tree)
{
#if (!USE_ONLY_MTOOLS)
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setTree(tree);
#endif //#if USE_ONLY_MTOOLS
}

const Data::BoxTree *CRAbMesh::JmeshModule::GetTree() const
{
#if USE_ONLY_MTOOLS
	return NULL;
#else
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getTree();
#endif //#if USE_ONLY_MTOOLS
}

void CRAbMesh::JmeshModule::SetBoundary(Data::Boundary *boundary)
{
#if (!USE_ONLY_MTOOLS)
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setBoundary(dynamic_cast<MJMesh::Boundary *>(boundary));
#endif //#if (!USE_ONLY_MTOOLS)
}

const Data::Boundary *CRAbMesh::JmeshModule::GetBoundary() const
{
#if USE_ONLY_MTOOLS
	return NULL;
#else
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getBoundary();
#endif //#if USE_ONLY_MTOOLS
}

void CRAbMesh::JmeshModule::SetFront(Data::Front */*front*/)
{

}

const Data::Front *CRAbMesh::JmeshModule::GetFront() const
{
	return NULL;
	//return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getFront();
}

void CRAbMesh::JmeshModule::SetMesh(Data::Mesh *mesh)
{
#if (!USE_ONLY_MTOOLS)
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setMesh(mesh);
#endif //#if (!USE_ONLY_MTOOLS)
}

const Data::Mesh *CRAbMesh::JmeshModule::GetMesh() const
{
#if USE_ONLY_MTOOLS
	return NULL;
#else
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getMesh();
#endif //#if USE_ONLY_MTOOLS
}

wxString CRAbMesh::JmeshModule::QualityString() const
{
#if USE_ONLY_MTOOLS
	return wxString();
#else
	wxString s(dynamic_cast<Jmesh::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
#endif //#if USE_ONLY_MTOOLS
}

#endif //#if USE_GUI
