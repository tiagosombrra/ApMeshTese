#include "CRAbMesh/PMToolsModule.h"

#if ((USE_MTOOLS) && (USE_GUI))

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/Front.h"
#include "Parallel/Communicator.h"

CRAbMesh::PMToolsModule::PMToolsModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::PMJMeshModule(frame, canvas, false)
{
	//this->SetOption(FRONT, false, false);

	//this->SetMeshUpdateType(PMJMesh::MeshUpdateType::FULL);

	this->redirector.SetMsecs(5000);

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

#if (USE_ONLY_MTOOLS != 2)
		if (dynamic_cast<PMTools::MainDrive *>(this->drive)->getComm()->isSlave())
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
#endif //#if (USE_ONLY_MTOOLS != 2)
	}

	this->removedControls = false;

	this->DisableUnusedControls();

	//this->redirector.Disable();
}

CRAbMesh::PMToolsModule::~PMToolsModule()
{

}

wxString CRAbMesh::PMToolsModule::Name() const
{
	wxString name = _("PMTools");

#if (USE_ONLY_MTOOLS != 2)
	PMTools::MainDrive *drive = dynamic_cast<PMTools::MainDrive *>(this->drive);

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
#endif //#if (USE_ONLY_MTOOLS != 2)

	return name;
}

/*wxMenu *CRAbMesh::PMToolsModule::GetMenu(unsigned int i, wxString &title) const
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

wxAuiToolBar *CRAbMesh::PMToolsModule::GetToolbar(unsigned int *//*i*//*, wxString &title) const
{
	title = _("");

	return NULL;
}*/

wxPanel *CRAbMesh::PMToolsModule::GetPanel(unsigned int i, wxString &title) const
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

bool CRAbMesh::PMToolsModule::LoadFile(const std::string &filename)
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

bool CRAbMesh::PMToolsModule::SaveFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	this->redirector.LogText();

	return saved;
}

/*void CRAbMesh::PMToolsModule::SetParameters(bool firstTime)
{
	*//*if (firstTime)
	{
		PMTools::MainDrive *drive = dynamic_cast<PMTools::MainDrive *>(this->drive);

		drive->setRemoveLayersInLoad(3);
	}*//*

	PMJMeshModule::SetParameters(firstTime);

	if (firstTime)
	{
		PMTools::MainDrive *drive = dynamic_cast<PMTools::MainDrive *>(this->drive);

		drive->setRemoveLayersInLoad(3);
	}
}*/

void CRAbMesh::PMToolsModule::Execute()
{
	this->redirector.Start(this->tcLog, stderr);

	this->frame->GetViewModule()->Execute();

	//disable items
	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	//Data::setStepByStep(false);

	PMJMeshModule::Execute();
}

void CRAbMesh::PMToolsModule::Ready(bool clearing, bool pending)
{
	if (!((pending) &&
		  (!this->IsStepByStep()) &&
		  (this->IsProgressShown())))
	{
		this->redirector.LogText();
	}

	this->CRAbMesh::PMJMeshModule::Ready(clearing, pending);

	/*PMTools::MainDrive *drive = dynamic_cast<PMTools::MainDrive *>(this->drive);

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

void CRAbMesh::PMToolsModule::Clear(bool onlyDrive, bool destroying)
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
		/*PMTools::MainDrive *drive = dynamic_cast<PMTools::MainDrive *>(this->drive);

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
    if (this->miConfConfiguration) this->miConfConfiguration->Enable();
	//if (this->miConfStepByStep) this->miConfStepByStep->Enable();
    if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable();
	if (this->miConfEnableParallelism) this->miConfEnableParallelism->Enable();
	if (this->miConfUsePartitioningThreshold) this->miConfUsePartitioningThreshold->Enable();
	if (this->miConfEnableSmoothInSlaves) this->miConfEnableSmoothInSlaves->Enable();

	if (this->tcTreeFactor) this->tcTreeFactor->Enable();
	if (this->tcTolerance) this->tcTolerance->Enable();
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable();

	if (this->cbEnableParallelism) this->cbEnableParallelism->Enable();
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

void CRAbMesh::PMToolsModule::SetGUIState()
{

}

void CRAbMesh::PMToolsModule::MakeMainDrive()
{
#if (USE_ONLY_MTOOLS == 2)
	this->drive = NULL;
#else
	this->drive = new PMTools::MainDrive();
#endif //#if (USE_ONLY_MTOOLS == 2)
}

/*void CRAbMesh::PMToolsModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::PMToolsModule::MakeToolbars()
{
	//Module::MakeToolbars();
}*/

void CRAbMesh::PMToolsModule::MakePanels()
{
	this->tcLog = NULL;

	PMJMeshModule::MakePanels();

	//this->panels.push_back(this->MakeLogPanel());
}

/*wxMenu *CRAbMesh::PMToolsModule::MakeConfigurationMenu()
{
	wxMenu *mConfiguration = PMJMeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	return mConfiguration;
}*/

/*wxMenu *CRAbMesh::PMToolsModule::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMToolsModule::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMToolsModule::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMToolsModule::OnResults));

    return mExecute;
}*/

/*wxPanel *CRAbMesh::PMToolsModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = PMJMeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel

	//buttons
    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

    this->bExecute->SetDefault();

    //Sizeres
    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    this->sbsQuality = NULL;

    //wxStaticBoxSizer *sbsWait = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Please wait"));
    //sbsWait->Add(this->gWait, 1, wxEXPAND | wxALL, 0);

    //wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
    //bsConf->Add(sConf,      0, wxEXPAND | wxALL, 1);
    //bsConf->Add(sbsExecute, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
    //pConfiguration->SetSizer(bsConf, false);

    sConf->Layout();

    //this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PMToolsModule::OnExecute));
    //this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PMToolsModule::OnClear));

	return pConfiguration;
}*/

wxPanel *CRAbMesh::PMToolsModule::MakeLogPanel()
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

wxDialog *CRAbMesh::PMToolsModule::MakeConfigurationDialog()
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

void CRAbMesh::PMToolsModule::DisableUnusedControls()
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

	//this->SetParallelismEnabled(false);
	//this->cbEnableParallelism->Enable(false);
	//this->miConfEnableParallelism->Enable(false);

	this->SetMinRefinementPostponed(false);
	this->cbPostponeMinRefinement->Enable(false);
	this->miConfPostponeMinRefinement->Enable(false);

	if (!this->removedControls)
	{
		wxPanel *pConf = this->GetPanel(0);

		wxSizer *sConf = pConf->GetSizer();

		wxStaticBoxSizer *sbsConfSerial = static_cast<wxStaticBoxSizer *>(sConf->GetItem(static_cast<size_t>(0))->GetSizer());
		wxGridBagSizer *gbsConfSerial = static_cast<wxGridBagSizer *>(sbsConfSerial->GetItem(static_cast<size_t>(0))->GetSizer());

		wxStaticBoxSizer *sbsConfParallel = static_cast<wxStaticBoxSizer *>(sConf->GetItem(1)->GetSizer());
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

		//this->cbEnableParallelism->Hide();
		//gbsConfParallel->Detach(this->cbEnableParallelism);

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

		//mConf->Remove(this->miConfEnableParallelism);
		//delete this->miConfEnableParallelism;
		//this->miConfEnableParallelism = NULL;

		mConf->Remove(this->miConfPostponeMinRefinement);
		delete this->miConfPostponeMinRefinement;
		this->miConfPostponeMinRefinement = NULL;

		mConf->Remove(this->miConfEnableSmoothInSlaves);
		delete this->miConfEnableSmoothInSlaves;
		this->miConfEnableSmoothInSlaves = NULL;

		//Separator
		delete mConf->Remove(mConf->FindItemByPosition(1));

		this->removedControls = true;
	}
}

/*void CRAbMesh::PMToolsModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::PMToolsModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::PMToolsModule::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}*/

/*void CRAbMesh::PMToolsModule::OnTimer()
{
	this->redirector.LogText(true);
}*/

unsigned int CRAbMesh::PMToolsModule::GetDimension() const
{
	return 3;
}

/*
void CRAbMesh::PMToolsModule::SetTree(Data::BoxTree *tree)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::PMToolsModule::GetTree() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getTree();
}

void CRAbMesh::PMToolsModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setBoundary(boundary);
}

const Data::Boundary *CRAbMesh::PMToolsModule::GetBoundary() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getBoundary();
}*/

void CRAbMesh::PMToolsModule::SetFront(Data::Front *front)
{
#if (USE_ONLY_MTOOLS != 2)
	dynamic_cast<PMTools::MainDrive *>(this->drive)->setFront(static_cast<MJMesh::Front *>(front));
#endif //#if (USE_ONLY_MTOOLS != 2)
}

const Data::Front *CRAbMesh::PMToolsModule::GetFront() const
{
#if (USE_ONLY_MTOOLS == 2)
	return NULL;
#else
	return dynamic_cast<PMTools::MainDrive *>(this->drive)->PMJMesh::MainDrive::getFront();
#endif //#if (USE_ONLY_MTOOLS == 2)
}

/*void CRAbMesh::PMToolsModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::PMToolsModule::GetMesh() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getMesh();
}

wxString CRAbMesh::PMToolsModule::QualityString() const
{
	wxString s(dynamic_cast<Jmesh::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}*/

#endif //#if ((USE_MTOOLS) && (USE_GUI))
