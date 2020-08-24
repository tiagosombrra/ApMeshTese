#include "CRAbMesh/MToolsModule.h"

#if ((USE_MTOOLS) && (USE_GUI))

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/Boundary.h"

CRAbMesh::MToolsModule::MToolsModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule(frame, canvas)
{
	//this->version = "Advan";
	//this->versions["Advan"] = MTools::MainDrive::ADVAN;
	//this->versions["Small"] = MTools::MainDrive::SMALL;

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
	this->SetOption(DRAW_FRONT, true, true);
	this->SetOption(DRAW_MESH, true, true);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	this->redirector.SetMsecs(5000);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}
}

CRAbMesh::MToolsModule::~MToolsModule()
{

}

wxString CRAbMesh::MToolsModule::Name() const
{
	return _("MTools");
}

wxMenu *CRAbMesh::MToolsModule::GetMenu(unsigned int i, wxString &title) const
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

wxAuiToolBar *CRAbMesh::MToolsModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::MToolsModule::GetPanel(unsigned int i, wxString &title) const
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

bool CRAbMesh::MToolsModule::LoadFile(const std::string &filename)
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

bool CRAbMesh::MToolsModule::SaveFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	this->redirector.LogText();

	return saved;
}

void CRAbMesh::MToolsModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		MTools::MainDrive *drive = dynamic_cast<MTools::MainDrive *>(this->drive);

		//drive->setVer(this->GetVersion());

#if USE_NUMERICAL
		drive->setHotellingTransformEnabled(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
		drive->setUsingUniformRefinement(this->IsUsingUniformRefinement());
	}
}

void CRAbMesh::MToolsModule::Execute()
{
	this->redirector.Start(this->tcLog, stderr);

	this->frame->GetViewModule()->Execute();

	//disable items
	//if (this->miConfAdvanVersion) this->miConfAdvanVersion->Enable(false);
	//if (this->miConfSmallVersion) this->miConfSmallVersion->Enable(false);
#if USE_NUMERICAL
	if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable(false);

	//if (this->rbVersions) this->rbVersions->Enable(false);

#if USE_NUMERICAL
	if (this->cbEnableHotellingTransform) this->cbEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable(false);

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	Data::setStepByStep(false);
}

void CRAbMesh::MToolsModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, pending);

	//this->gWait->SetValue(0);
	//this->gWait->Enable(false);

	MTools::MainDrive *drive = dynamic_cast<MTools::MainDrive *>(this->drive);

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
}

void CRAbMesh::MToolsModule::Clear(bool onlyDrive, bool destroying)
{
	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		/*MTools::MainDrive *drive = dynamic_cast<MTools::MainDrive *>(this->drive);

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
    //if (this->miConfAdvanVersion) this->miConfAdvanVersion->Enable();
    //if (this->miConfSmallVersion) this->miConfSmallVersion->Enable();
#if USE_NUMERICAL
    if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable();
#endif //#if USE_NUMERICAL
    if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable();

	//if (this->rbVersions) this->rbVersions->Enable();
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

/*void CRAbMesh::MToolsModule::SetVersion(std::string version)
{
	this->version = version;

	wxString value(this->version.c_str(), wxMBConvLibc());
	this->rbVersions->SetStringSelection(value);

	if (this->version == "Advan")
	{
		this->miConfAdvanVersion->Check(true);
		//this->miConfSmallVersion->Check(false);
	}
	else if (this->version == "Small")
	{
		//this->miConfAdvanVersion->Check(false);
		this->miConfSmallVersion->Check(true);
	}
}

enum MTools::MainDrive::Versions CRAbMesh::MToolsModule::GetVersion() const
{
	VersionMap::const_iterator it = this->versions.find(this->version);

	return (it == this->versions.end()) ? MTools::MainDrive::ADVAN : (*it).second;
}

std::string CRAbMesh::MToolsModule::GetVersionString() const
{
	return this->version;
}

std::string CRAbMesh::MToolsModule::GetVersions() const
{
	std::stringstream s;

	for (VersionMap::const_iterator iter = this->versions.begin();
		 iter != this->versions.end(); iter++)
	{
		s << (*iter).first << "\n";
	}

	return s.str();
}*/

#if USE_NUMERICAL
void CRAbMesh::MToolsModule::SetHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;

    this->miConfEnableHotellingTransform->Check(this->hotellingTransformEnabled);
    this->cbEnableHotellingTransform->SetValue(this->hotellingTransformEnabled);
}

bool CRAbMesh::MToolsModule::IsHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void CRAbMesh::MToolsModule::SetUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;

    this->miConfUseUniformRefinement->Check(this->usingUniformRefinement);
    this->cbUseUniformRefinement->SetValue(this->usingUniformRefinement);
}

bool CRAbMesh::MToolsModule::IsUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

void CRAbMesh::MToolsModule::SetGUIState()
{

}

void CRAbMesh::MToolsModule::MakeMainDrive()
{
	this->drive = new MTools::MainDrive();
}

void CRAbMesh::MToolsModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::MToolsModule::MakeToolbars()
{
	//Module::MakeToolbars();
}

void CRAbMesh::MToolsModule::MakePanels()
{
	this->tcLog = NULL;

	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
	//this->panels.push_back(this->MakeLogPanel());

	//Module::MakePanels();
}

wxMenu *CRAbMesh::MToolsModule::MakeConfigurationMenu()
{
	//wxMenu *mConfiguration = new wxMenu();
	wxMenu *mConfiguration = MeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	//this->miConfAdvanVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Advan version"));
    //this->miConfSmallVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Small version"));

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

    //this->Connect(this->miConfAdvanVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnConfAdvanVersion));
    //this->Connect(this->miConfSmallVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnConfSmallVersion));
#if USE_NUMERICAL
    this->Connect(this->miConfEnableHotellingTransform->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->miConfUseUniformRefinement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnConfUseUniformRefinement));

    return mConfiguration;
}

wxMenu *CRAbMesh::MToolsModule::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MToolsModule::OnResults));

    return mExecute;
}

wxPanel *CRAbMesh::MToolsModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel

	//radio boxes
	/*wxString vers(this->GetVersions().c_str(), wxMBConvLibc());

    wxStringTokenizer tkz(vers, _("\n"));

    wxArrayString versions;

	while (tkz.HasMoreTokens())
	{
		versions.Add(tkz.GetNextToken());
	}

	wxString value(this->GetVersionString().c_str(), wxMBConvLibc());

    this->rbVersions    = new wxRadioBox(pConfiguration, wxID_ANY, _("Version"), wxDefaultPosition, wxDefaultSize, versions, wxRA_SPECIFY_ROWS);*/
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
    //gbsConf->Add(this->rbVersions,     wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#if USE_NUMERICAL
    gbsConf->Add(this->cbEnableHotellingTransform, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#endif //#if USE_NUMERICAL
    gbsConf->Add(this->cbUseUniformRefinement, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
    bsConf->Add(gbsConf,    0, wxEXPAND | wxALL, 1);
    bsConf->Add(sConf,      0, wxEXPAND | wxALL, 1);
    bsConf->Add(sbsExecute, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
    pConfiguration->SetSizer(bsConf, false);

    //this->Connect(this->rbVersions->GetId(),     wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(MToolsModule::OnConfVersions));
#if USE_NUMERICAL
    this->Connect(this->cbEnableHotellingTransform->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MToolsModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->cbUseUniformRefinement->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MToolsModule::OnConfUseUniformRefinement));

    this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MToolsModule::OnExecute));
    this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MToolsModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::MToolsModule::MakeLogPanel()
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

/*void CRAbMesh::MToolsModule::OnConfAdvanVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion("Advan");
}

void CRAbMesh::MToolsModule::OnConfSmallVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion("Small");
}

void CRAbMesh::MToolsModule::OnConfVersions(wxCommandEvent &WXUNUSED(event))
{
	std::string version(this->rbVersions->GetStringSelection().mb_str(wxMBConvLibc()));

	this->SetVersion(version);
}*/

#if USE_NUMERICAL
void CRAbMesh::MToolsModule::OnConfEnableHotellingTransform(wxCommandEvent &WXUNUSED(event))
{
	this->SetHotellingTransformEnabled(!this->IsHotellingTransformEnabled());
}
#endif //#if USE_NUMERICAL

void CRAbMesh::MToolsModule::OnConfUseUniformRefinement(wxCommandEvent &WXUNUSED(event))
{
	this->SetUsingUniformRefinement(!this->IsUsingUniformRefinement());
}

void CRAbMesh::MToolsModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::MToolsModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::MToolsModule::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}

/*void CRAbMesh::MToolsModule::OnTimer()
{
	this->redirector.LogText(true);
}*/

void CRAbMesh::MToolsModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::MToolsModule::GetDimension() const
{
	return 3;
}

void CRAbMesh::MToolsModule::SetTree(Data::BoxTree *tree)
{
	dynamic_cast<MTools::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::MToolsModule::GetTree() const
{
	return dynamic_cast<MTools::MainDrive *>(this->drive)->getTree();
}

void CRAbMesh::MToolsModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<MTools::MainDrive *>(this->drive)->setBoundary(dynamic_cast<MJMesh::Boundary *>(boundary));
}

const Data::Boundary *CRAbMesh::MToolsModule::GetBoundary() const
{
	return dynamic_cast<MTools::MainDrive *>(this->drive)->getBoundary();
}

void CRAbMesh::MToolsModule::SetFront(Data::Front */*front*/)
{

}

const Data::Front *CRAbMesh::MToolsModule::GetFront() const
{
	return dynamic_cast<MTools::MainDrive *>(this->drive)->getFront();
}

void CRAbMesh::MToolsModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<MTools::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::MToolsModule::GetMesh() const
{
	return dynamic_cast<MTools::MainDrive *>(this->drive)->getMesh();
}

wxString CRAbMesh::MToolsModule::QualityString() const
{
	wxString s(dynamic_cast<MTools::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}

#endif //#if ((USE_MTOOLS) && (USE_GUI))
