#include "CRAbMesh/PMTools2Module.h"

#if ((USE_MTOOLS) && (USE_GUI))

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/Front.h"
#include "Parallel/Communicator.h"

CRAbMesh::PMTools2Module::PMTools2Module(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::PMJMesh2Module(frame, canvas, false)
{
	//this->SetOption(FRONT, false, false);

	//this->SetMeshUpdateType(PMJMesh::MeshUpdateType::FULL);

	this->redirector.SetMsecs(5000);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();

		if (dynamic_cast<PMTools2::MainDrive *>(this->drive)->getComm()->isSlave())
		{
			this->SetStepByStep(false);
			this->SetProgressShown(false);

			//this->redirector.Disable();

			this->SetBoundaryDrawable(false);
			this->SetTreeDrawable(false);
			this->SetFrontDrawable(false);
			this->SetMeshDrawable(false);
			this->SetInnerMeshDrawable(false);
			this->SetPartitionsDrawable(false);
		}

		//this->SetRemoveLayersInLoad(1);
		this->SetRangeProportion(0.75);
	}

	this->removedControls = false;

	this->DisableUnusedControls();

	//this->redirector.Disable();
}

CRAbMesh::PMTools2Module::~PMTools2Module()
{

}

wxString CRAbMesh::PMTools2Module::Name() const
{
	wxString name = _("PMTools2");

	PMTools2::MainDrive *drive = dynamic_cast<PMTools2::MainDrive *>(this->drive);

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

	return name;
}

/*wxMenu *CRAbMesh::PMTools2Module::GetMenu(unsigned int i, wxString &title) const
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

wxAuiToolBar *CRAbMesh::PMTools2Module::GetToolbar(unsigned int *//*i*//*, wxString &title) const
{
	title = _("");

	return NULL;
}*/

wxPanel *CRAbMesh::PMTools2Module::GetPanel(unsigned int i, wxString &title) const
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

bool CRAbMesh::PMTools2Module::LoadFile(const std::string &filename)
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

bool CRAbMesh::PMTools2Module::SaveFile(const std::string &filename)
{
	this->redirector.Start(this->tcLog, stderr);

	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	this->redirector.LogText();

	return saved;
}

/*void CRAbMesh::PMTools2Module::SetParameters(bool firstTime)
{
	*//*if (firstTime)
	{
		PMTools2::MainDrive *drive = dynamic_cast<PMTools2::MainDrive *>(this->drive);

		drive->setRemoveLayersInLoad(3);
	}*//*

	PMJMeshModule::SetParameters(firstTime);

	if (firstTime)
	{
		PMTools2::MainDrive *drive = dynamic_cast<PMTools2::MainDrive *>(this->drive);

		drive->setRemoveLayersInLoad(3);
	}
}*/

void CRAbMesh::PMTools2Module::Execute()
{
	this->redirector.Start(this->tcLog, stderr);

	this->frame->GetViewModule()->Execute();

	//disable items
	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	//Data::setStepByStep(false);

	PMJMesh2Module::Execute();
}

void CRAbMesh::PMTools2Module::Ready(bool clearing, bool pending)
{
	if (!((pending) &&
		  (!this->IsStepByStep()) &&
		  (this->IsProgressShown())))
	{
		this->redirector.LogText();
	}

	this->CRAbMesh::PMJMesh2Module::Ready(clearing, pending);

	/*PMTools2::MainDrive *drive = dynamic_cast<PMTools2::MainDrive *>(this->drive);

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

void CRAbMesh::PMTools2Module::Clear(bool onlyDrive, bool destroying)
{
	PMJMesh2Module::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		return;
	}

	if (this->tcLog)
	{
        this->tcLog->Clear();
    }
}

void CRAbMesh::PMTools2Module::SetGUIState()
{

}

void CRAbMesh::PMTools2Module::MakeMainDrive()
{
	this->drive = new PMTools2::MainDrive();
}

/*void CRAbMesh::PMTools2Module::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::PMTools2Module::MakeToolbars()
{
	//Module::MakeToolbars();
}*/

void CRAbMesh::PMTools2Module::MakePanels()
{
	this->tcLog = NULL;

	PMJMesh2Module::MakePanels();

	//this->panels.push_back(this->MakeLogPanel());
}

/*wxMenu *CRAbMesh::PMTools2Module::MakeConfigurationMenu()
{
	wxMenu *mConfiguration = PMJMeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	return mConfiguration;
}*/

/*wxMenu *CRAbMesh::PMTools2Module::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMTools2Module::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMTools2Module::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMTools2Module::OnResults));

    return mExecute;
}*/

/*wxPanel *CRAbMesh::PMTools2Module::MakeConfigurationPanel()
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

    //this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PMTools2Module::OnExecute));
    //this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PMTools2Module::OnClear));

	return pConfiguration;
}*/

wxPanel *CRAbMesh::PMTools2Module::MakeLogPanel()
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

wxDialog *CRAbMesh::PMTools2Module::MakeConfigurationDialog()
{
	wxDialog *dConf = PMJMesh2Module::MakeConfigurationDialog();

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

void CRAbMesh::PMTools2Module::DisableUnusedControls()
{
	this->tcTreeFactor->Enable(false);

	this->tcTolerance->Enable(false);

	this->SetBoundarySorted(true);
	this->cbSortBoundaryElements->Enable(false);
	this->miConfSortBoundaryElements->Enable(false);

	this->SetLocalBackTrackingEnabled(true);
	this->cbEnableLocalBackTracking->Enable(false);
	this->miConfEnableLocalBackTracking->Enable(false);

	this->SetMeshSorted(true);
	this->cbSortMesh->Enable(false);
	this->miConfSortMesh->Enable(false);

	//this->SetAPrioriEnabled(false);
	//this->cbEnableAPriori->Enable(false);
	//this->miConfEnableAPriori->Enable(false);

	//this->SetParallelismEnabled(true);
	//this->cbEnableParallelism->Enable(false);
	//this->miConfEnableParallelism->Enable(false);

	//this->SetMinRefinementPostponed(false);
	//this->cbPostponeMinRefinement->Enable(false);
	//this->miConfPostponeMinRefinement->Enable(false);

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

		//this->cbEnableAPriori->Hide();
		//gbsConfParallel->Detach(this->cbEnableAPriori);

		//this->cbEnableParallelism->Hide();
		//gbsConfParallel->Detach(this->cbEnableParallelism);

		//this->cbPostponeMinRefinement->Hide();
		//gbsConfParallel->Detach(this->cbPostponeMinRefinement);

		//this->cbEnableSmoothInSlaves->Hide();
		//gbsConfParallel->Detach(this->cbEnableSmoothInSlaves);

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

		//mConf->Remove(this->miConfEnableAPriori);
		//delete this->miConfEnableAPriori;
		//this->miConfEnableAPriori = NULL;

		//mConf->Remove(this->miConfEnableParallelism);
		//delete this->miConfEnableParallelism;
		//this->miConfEnableParallelism = NULL;

		//mConf->Remove(this->miConfPostponeMinRefinement);
		//delete this->miConfPostponeMinRefinement;
		//this->miConfPostponeMinRefinement = NULL;

		//mConf->Remove(this->miConfEnableSmoothInSlaves);
		//delete this->miConfEnableSmoothInSlaves;
		//this->miConfEnableSmoothInSlaves = NULL;

		//Separator
		//delete mConf->Remove(mConf->FindItemByPosition(1));

		this->removedControls = true;
	}
}

/*void CRAbMesh::PMTools2Module::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::PMTools2Module::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::PMTools2Module::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}*/

/*void CRAbMesh::PMTools2Module::OnTimer()
{
	this->redirector.LogText(true);
}*/

unsigned int CRAbMesh::PMTools2Module::GetDimension() const
{
	return 3;
}

/*
void CRAbMesh::PMTools2Module::SetTree(Data::BoxTree *tree)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::PMTools2Module::GetTree() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getTree();
}

void CRAbMesh::PMTools2Module::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setBoundary(boundary);
}

const Data::Boundary *CRAbMesh::PMTools2Module::GetBoundary() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getBoundary();
}*/

void CRAbMesh::PMTools2Module::SetFront(Data::Front *front)
{
	dynamic_cast<PMTools2::MainDrive *>(this->drive)->setFront(static_cast<MJMesh::Front *>(front));
}

const Data::Front *CRAbMesh::PMTools2Module::GetFront() const
{
	return dynamic_cast<PMTools2::MainDrive *>(this->drive)->PMJMesh2::MainDrive::getFront();
}

/*void CRAbMesh::PMTools2Module::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<Jmesh::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::PMTools2Module::GetMesh() const
{
	return dynamic_cast<Jmesh::MainDrive *>(this->drive)->getMesh();
}

wxString CRAbMesh::PMTools2Module::QualityString() const
{
	wxString s(dynamic_cast<Jmesh::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}*/

#endif //#if ((USE_MTOOLS) && (USE_GUI))
