#include "CRAbMesh/PJmeshModule.h"

#if (USE_GUI)

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/Front.h"
#include "Parallel/Communicator.h"

CRAbMesh::PJmeshModule::PJmeshModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::PMJMeshModule(frame, canvas, false)
{
	//this->SetOption(FRONT, false, false);

	this->rbVersions = NULL;
	this->miConfAdvanVersion = this->miConfSmallVersion = NULL;
	this->versions[_("Advan")] = Jmesh::MainDrive::ADVAN;
	this->versions[_("Small")] = Jmesh::MainDrive::SMALL;
	this->SetVersion(Jmesh::MainDrive::ADVAN);

	//this->SetMeshUpdateType(PMJMesh::MeshUpdateType::FULL);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();

		this->EnableLoadCtrl();
		this->EnableTreeRefinementCtrl();
		this->EnableRangeProportionCtrl();
		this->EnableMeshUpdateTypesCtrl();
		this->EnableShiftTypesCtrl();

#if (!USE_ONLY_MTOOLS)
		if (dynamic_cast<PJmesh::MainDrive *>(this->drive)->getComm()->isSlave())
		{
			this->SetStepByStep(false);
			this->SetProgressShown(false);

			//this->redirector.Disable();

			this->SetBoundaryDrawable(false);
			this->SetTreeDrawable(false);
			this->SetFrontDrawable(false);
			this->SetMeshDrawable(false);
			this->SetPartitionsDrawable(false);
		}
#endif //#if (!USE_ONLY_MTOOLS)
	}

	this->removedControls = false;

	this->DisableUnusedControls();

	//this->redirector.Disable();
}

CRAbMesh::PJmeshModule::~PJmeshModule()
{

}

wxString CRAbMesh::PJmeshModule::Name() const
{
	wxString name = _("PJmesh");

#if (!USE_ONLY_MTOOLS)
	PJmesh::MainDrive *drive = dynamic_cast<PJmesh::MainDrive *>(this->drive);

	if ((drive) && (drive->getComm()) && (drive->getComm()->isParallel()))
	{
		if (drive->getComm()->isMaster())
		{
			name << _(" Master");
		}
		else
		{
			name << _(" Slave ") << drive->getComm()->rank();
		}
	}
#endif //#if (!USE_ONLY_MTOOLS)

	return name;
}

/*wxMenu *CRAbMesh::PJmeshModule::GetMenu(unsigned int i, wxString &title) const
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

wxAuiToolBar *CRAbMesh::PJmeshModule::GetToolbar(unsigned int *//*i*//*, wxString &title) const
{
	title = _("");

	return NULL;
}*/

wxPanel *CRAbMesh::PJmeshModule::GetPanel(unsigned int i, wxString &title) const
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

bool CRAbMesh::PJmeshModule::LoadFile(const std::string &filename)
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

		this->SetTolerance(Data::getTolerance());

		this->frame->GetViewModule()->SetDepthTestEnabled(true);
		this->frame->GetViewModule()->SetCullFaceEnabled(false);
	}

	return loaded;
}

bool CRAbMesh::PJmeshModule::SaveFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	this->redirector.LogText();

	return saved;
}

void CRAbMesh::PJmeshModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
#if (!USE_ONLY_MTOOLS)
		PJmesh::MainDrive *drive = dynamic_cast<PJmesh::MainDrive *>(this->drive);

		drive->setVer(this->GetVersion());
#endif //#if (!USE_ONLY_MTOOLS)
	}

	PMJMeshModule::SetParameters(firstTime);
}

void CRAbMesh::PJmeshModule::Execute()
{
	this->redirector.Start(this->tcLog, stderr);

	this->frame->GetViewModule()->Execute();

	//disable items
	if (this->miConfAdvanVersion) this->miConfAdvanVersion->Enable(false);
	if (this->miConfSmallVersion) this->miConfSmallVersion->Enable(false);
	//if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable(false);

	if (this->rbVersions) this->rbVersions->Enable(false);
	//if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable(false);

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	//Data::setStepByStep(false);

	PMJMeshModule::Execute();
}

void CRAbMesh::PJmeshModule::Ready(bool clearing, bool pending)
{
	if (!((pending) &&
		  (!this->IsStepByStep()) &&
		  (this->IsProgressShown())))
	{
		this->redirector.LogText();
	}

	this->CRAbMesh::PMJMeshModule::Ready(clearing, pending);

	/*PJmesh::MainDrive *drive = dynamic_cast<PJmesh::MainDrive *>(this->drive);

	if (clearing)
	{
		drive->clear();
	}

	if ((pending) &&
		(!this->IsStepByStep()) &&
		(this->IsProgressShown()))
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

		this->redirector.LogText();
	}*/
}

void CRAbMesh::PJmeshModule::Clear(bool onlyDrive, bool destroying)
{
	/*if ((this->drive) && (dynamic_cast<PMJMesh::MainDrive *>(this->drive)->getComm()->isSlave()))
	{
		return;
	}*/

	//dynamic_cast<PMJMesh::MainDrive *>(this->drive)->setDestroying(destroying);

	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		/*PJmesh::MainDrive *drive = dynamic_cast<PJmesh::MainDrive *>(this->drive);

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
    if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable();
    if (this->miConfConfiguration) this->miConfConfiguration->Enable();
	//if (this->miConfStepByStep) this->miConfStepByStep->Enable();
	if (this->miConfUsePartitioningThreshold) this->miConfUsePartitioningThreshold->Enable();
	if (this->miConfEnableSmoothInSlaves) this->miConfEnableSmoothInSlaves->Enable();

	if (this->rbVersions) this->rbVersions->Enable();
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable();

	if (this->tcTreeFactor) this->tcTreeFactor->Enable();
	if (this->tcTolerance) this->tcTolerance->Enable();

	if (this->scPercentageNonWorkingTask) this->scPercentageNonWorkingTask->Enable();
	if (this->cbUsePartitioningThreshold) this->cbUsePartitioningThreshold->Enable();
	if (this->cbEnableSmoothInSlaves) this->cbEnableSmoothInSlaves->Enable();

	if (this->cbLoadBalancer) this->cbLoadBalancer->Enable();
	if (this->cbPartitionerType) this->cbPartitionerType->Enable();
	if (this->cbMeshUpdateType) this->cbMeshUpdateType->Enable();

	this->EnableLoadCtrl();
	this->EnableTreeRefinementCtrl();
	this->EnableRangeProportionCtrl();
	this->EnableMeshUpdateTypesCtrl();
	this->EnableShiftTypesCtrl();

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

void CRAbMesh::PJmeshModule::SetVersion(wxString version)
{
	this->version = version;

	if (this->rbVersions)
	{
		this->rbVersions->SetStringSelection(this->version);
	}

	if ((this->version == _("Advan")) && (this->miConfAdvanVersion))
	{
		this->miConfAdvanVersion->Check(true);
		//this->miConfSmallVersion->Check(false);
	}
	else if ((this->version == _("Small")) && (this->miConfSmallVersion))
	{
		//this->miConfAdvanVersion->Check(false);
		this->miConfSmallVersion->Check(true);
	}
}

void CRAbMesh::PJmeshModule::SetVersion(enum Jmesh::MainDrive::Versions version)
{
	for (VersionMap::iterator iter = this->versions.begin();
		 iter != this->versions.end(); iter++)
	{
		if ((*iter).second == version)
		{
			this->SetVersion((*iter).first);

			break;
		}
	}
}

enum Jmesh::MainDrive::Versions CRAbMesh::PJmeshModule::GetVersion() const
{
	VersionMap::const_iterator it = this->versions.find(this->version);

	return (it == this->versions.end()) ? Jmesh::MainDrive::ADVAN : (*it).second;
}

wxString CRAbMesh::PJmeshModule::GetVersionString() const
{
	return this->version;
}

wxArrayString CRAbMesh::PJmeshModule::GetVersions() const
{
	wxArrayString s;

	for (VersionMap::const_iterator iter = this->versions.begin();
		 iter != this->versions.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PJmeshModule::SetGUIState()
{

}

void CRAbMesh::PJmeshModule::MakeMainDrive()
{
#if USE_ONLY_MTOOLS
	this->drive = NULL;
#else
	this->drive = new PJmesh::MainDrive();
#endif //#if USE_ONLY_MTOOLS
}

/*void CRAbMesh::PJmeshModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::PJmeshModule::MakeToolbars()
{
	//Module::MakeToolbars();
}*/

void CRAbMesh::PJmeshModule::MakePanels()
{
	this->tcLog = NULL;

	PMJMeshModule::MakePanels();

	//this->panels.push_back(this->MakeLogPanel());
}

wxMenu *CRAbMesh::PJmeshModule::MakeConfigurationMenu()
{
	wxMenu *mConfiguration = PMJMeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	this->miConfAdvanVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Advan version"));
    this->miConfSmallVersion = mConfiguration->InsertRadioItem(row++, wxID_ANY, _("&Small version"));

    if (mConfiguration->GetMenuItemCount() != row)
	{
		mConfiguration->InsertSeparator(row++);
	}

    this->Connect(this->miConfAdvanVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PJmeshModule::OnConfAdvanVersion));
    this->Connect(this->miConfSmallVersion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PJmeshModule::OnConfSmallVersion));

    return mConfiguration;
}

/*wxMenu *CRAbMesh::PJmeshModule::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PJmeshModule::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PJmeshModule::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PJmeshModule::OnResults));

    return mExecute;
}*/

wxPanel *CRAbMesh::PJmeshModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = PMJMeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel

	//radio boxes
	//wxString value = this->GetVersionString();

    this->rbVersions    = new wxRadioBox(pConfiguration, wxID_ANY, _("Jmesh Options"), wxDefaultPosition, wxDefaultSize, this->GetVersions(), wxRA_SPECIFY_ROWS);

    /*//buttons
    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

    this->bExecute->SetDefault();

    //Sizeres
    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    this->sbsQuality = NULL;*/

    //wxStaticBoxSizer *sbsWait = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Please wait"));
    //sbsWait->Add(this->gWait, 1, wxEXPAND | wxALL, 0);

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
	gbsConf->AddGrowableCol(0);
    gbsConf->Add(this->rbVersions,     wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

    //wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
    //bsConf->Add(gbsConf,    0, wxEXPAND | wxALL, 1);
    //bsConf->Add(sConf,      0, wxEXPAND | wxALL, 1);
    //bsConf->Add(sbsExecute, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
    //pConfiguration->SetSizer(bsConf, false);

    sConf->Insert(0, gbsConf, 0, wxEXPAND | wxALL, 1);
    sConf->Layout();

    this->Connect(this->rbVersions->GetId(),     wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(PJmeshModule::OnConfVersions));

    //this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PJmeshModule::OnExecute));
    //this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PJmeshModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::PJmeshModule::MakeLogPanel()
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

wxDialog *CRAbMesh::PJmeshModule::MakeConfigurationDialog()
{
	wxDialog *dConf = PMJMeshModule::MakeConfigurationDialog();

	wxPropertySheetDialog *psdConf = static_cast<wxPropertySheetDialog *>(dConf);

	wxWindow *pConf = psdConf->GetBookCtrl()->GetPage(0);

	static_cast<wxTextCtrl *>(pConf->GetChildren()[1])->Enable(false);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[3])->Enable(false);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[5])->Enable(false);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[7])->Enable(false);

	pConf = psdConf->GetBookCtrl()->GetPage(1);

	static_cast<wxTextCtrl *>(pConf->GetChildren()[9])->Enable(false);

	return psdConf;
}

void CRAbMesh::PJmeshModule::DisableUnusedControls()
{
	this->tcTreeFactor->Enable(false);

	this->tcTolerance->Enable(false);

	this->SetBoundarySorted(true);
	this->cbSortBoundaryElements->Enable(false);
	this->miConfSortBoundaryElements->Enable(false);

	this->SetLocalBackTrackingEnabled(false);
	this->cbEnableLocalBackTracking->Enable(false);
	this->miConfEnableLocalBackTracking->Enable(false);

	this->SetMeshSorted(false);
	this->cbSortMesh->Enable(false);
	this->miConfSortMesh->Enable(false);

	this->SetAPrioriEnabled(false);
	this->cbEnableAPriori->Enable(false);
	this->miConfEnableAPriori->Enable(false);

	this->SetParallelismEnabled(false);
	this->cbEnableParallelism->Enable(false);
	this->miConfEnableParallelism->Enable(false);

	this->SetMinRefinementPostponed(false);
	this->cbPostponeMinRefinement->Enable(false);
	this->miConfPostponeMinRefinement->Enable(false);

	if (!this->removedControls)
	{
		wxPanel *pConf = this->GetPanel(0);

		wxSizer *sConf = pConf->GetSizer();

		wxStaticBoxSizer *sbsConfSerial = static_cast<wxStaticBoxSizer *>(sConf->GetItem(1)->GetSizer());
		wxGridBagSizer *gbsConfSerial = static_cast<wxGridBagSizer *>(sbsConfSerial->GetItem(static_cast<size_t>(0))->GetSizer());

		wxStaticBoxSizer *sbsConfParallel = static_cast<wxStaticBoxSizer *>(sConf->GetItem(2)->GetSizer());
		wxGridBagSizer *gbsConfParallel = static_cast<wxGridBagSizer *>(sbsConfParallel->GetItem(static_cast<size_t>(0))->GetSizer());

		wxGBSizerItem *si = NULL;

		//stTreeFactor
		si = static_cast<wxGBSizerItem *>(gbsConfSerial->GetItem(static_cast<size_t>(0)));
		si->GetWindow()->Hide();
		gbsConfSerial->Detach(si->GetWindow());

		this->tcTreeFactor->Hide();
		gbsConfSerial->Detach(this->tcTreeFactor);

		//stTolerance
		si = static_cast<wxGBSizerItem *>(gbsConfSerial->GetItem(static_cast<size_t>(0)));
		si->GetWindow()->Hide();
		gbsConfSerial->Detach(si->GetWindow());

		this->tcTolerance->Hide();
		gbsConfSerial->Detach(this->tcTolerance);

		this->cbSortBoundaryElements->Hide();
		gbsConfSerial->Detach(this->cbSortBoundaryElements);

		this->cbEnableLocalBackTracking->Hide();
		gbsConfSerial->Detach(this->cbEnableLocalBackTracking);

		this->cbSortMesh->Hide();
		gbsConfSerial->Detach(this->cbSortMesh);

		si = NULL;

		for (int row = -1, count = 0; count < static_cast<int>(gbsConfSerial->GetChildren().GetCount()); count++)
		{
			si = static_cast<wxGBSizerItem *>(gbsConfSerial->GetItem(count));

			wxGBPosition pos = si->GetPos();

			if (pos.GetCol() == 0)
			{
				row++;
			}

			pos.SetRow(row);

			si->SetPos(pos);
		}

		gbsConfSerial->Layout();
		sbsConfSerial->Layout();

		this->cbEnableAPriori->Hide();
		gbsConfParallel->Detach(this->cbEnableAPriori);

		this->cbEnableParallelism->Hide();
		gbsConfParallel->Detach(this->cbEnableParallelism);

		this->cbPostponeMinRefinement->Hide();
		gbsConfParallel->Detach(this->cbPostponeMinRefinement);

		this->cbEnableSmoothInSlaves->Hide();
		gbsConfParallel->Detach(this->cbEnableSmoothInSlaves);

		si = NULL;

		for (int row = -1, count = 0; count < static_cast<int>(gbsConfParallel->GetChildren().GetCount()); count++)
		{
			si = static_cast<wxGBSizerItem *>(gbsConfParallel->GetItem(count));

			wxGBPosition pos = si->GetPos();

			if (pos.GetCol() == 0)
			{
				row++;
			}

			pos.SetRow(row);

			si->SetPos(pos);
		}

		gbsConfParallel->Layout();
		sbsConfParallel->Layout();

		sConf->Layout();

		pConf->FitInside();

		wxMenu *mConf = this->GetMenu(0);

		mConf->Remove(this->miConfSortBoundaryElements);
		delete this->miConfSortBoundaryElements;
		this->miConfSortBoundaryElements = NULL;

		mConf->Remove(this->miConfEnableLocalBackTracking);
		delete this->miConfEnableLocalBackTracking;
		this->miConfEnableLocalBackTracking = NULL;

		mConf->Remove(this->miConfSortMesh);
		delete this->miConfSortMesh;
		this->miConfSortMesh = NULL;

		mConf->Remove(this->miConfEnableAPriori);
		delete this->miConfEnableAPriori;
		this->miConfEnableAPriori = NULL;

		mConf->Remove(this->miConfEnableParallelism);
		delete this->miConfEnableParallelism;
		this->miConfEnableParallelism = NULL;

		mConf->Remove(this->miConfPostponeMinRefinement);
		delete this->miConfPostponeMinRefinement;
		this->miConfPostponeMinRefinement = NULL;

		mConf->Remove(this->miConfEnableSmoothInSlaves);
		delete this->miConfEnableSmoothInSlaves;
		this->miConfEnableSmoothInSlaves = NULL;

		//Separator
		delete mConf->Remove(mConf->FindItemByPosition(4));

		this->removedControls = true;
	}
}

void CRAbMesh::PJmeshModule::OnConfAdvanVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(_("Advan"));
}

void CRAbMesh::PJmeshModule::OnConfSmallVersion(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(_("Small"));
}

void CRAbMesh::PJmeshModule::OnConfVersions(wxCommandEvent &WXUNUSED(event))
{
	this->SetVersion(this->rbVersions->GetStringSelection());
}

/*void CRAbMesh::PJmeshModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::PJmeshModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::PJmeshModule::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}*/

/*void CRAbMesh::PJmeshModule::OnTimer()
{
	this->redirector.LogText(true);
}*/

unsigned int CRAbMesh::PJmeshModule::GetDimension() const
{
	return 3;
}

/*
void CRAbMesh::PJmeshModule::SetTree(Data::BoxTree *tree)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::PJmeshModule::GetTree() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getTree();
}

void CRAbMesh::PJmeshModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setBoundary(boundary);
}

const Data::Boundary *CRAbMesh::PJmeshModule::GetBoundary() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getBoundary();
}*/

void CRAbMesh::PJmeshModule::SetFront(Data::Front *front)
{
#if (!USE_ONLY_MTOOLS)
	dynamic_cast<PJmesh::MainDrive *>(this->drive)->setFront(static_cast<MJMesh::Front *>(front));
#endif //#if (!USE_ONLY_MTOOLS)
}

const Data::Front *CRAbMesh::PJmeshModule::GetFront() const
{
#if USE_ONLY_MTOOLS
	return NULL;
#else
	return dynamic_cast<PJmesh::MainDrive *>(this->drive)->getFront();
#endif //#if USE_ONLY_MTOOLS
}

/*void CRAbMesh::PJmeshModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::PJmeshModule::GetMesh() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getMesh();
}

wxString CRAbMesh::PJmeshModule::QualityString() const
{
	wxString s(dynamic_cast<Jmesh::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}*/

#endif //#if (USE_GUI)
