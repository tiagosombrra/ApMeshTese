#include "CRAbMesh/MJMeshModule.h"

#if USE_GUI

#include <wx/gbsizer.h>
#include <wx/propdlg.h>
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/TreeItemData.h"
#include "CRAbMesh/ViewModule.h"
#include "MJMesh/MainDrive.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Front.h"
#include "MJMesh/Boundary.h"

CRAbMesh::MJMeshModule::MJMeshModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule(frame, canvas)
{
	this->treeFactor = 1.0;
	this->tolerance = 0.001;

	this->searchTreesEnabled = true;

	//this->boundaryAdapted = true;
	this->boundarySorted = false;
#if USE_NUMERICAL
	this->hotellingTransformEnabled = false;
#endif //#if USE_NUMERICAL
	this->usingUniformRefinement = true;
	this->localBackTrackingEnabled = true;
	this->meshSorted = true;

	this->numImproves = 5;
	this->maxBackTracks = 10;

	this->stepByStep = true;
	this->progressShown = true;

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

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}
}

CRAbMesh::MJMeshModule::~MJMeshModule()
{
	this->tcTreeFactor->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(MJMeshModule::OnKillFocus), NULL, this);
    this->tcTolerance ->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(MJMeshModule::OnKillFocus), NULL, this);
}

wxString CRAbMesh::MJMeshModule::Name() const
{
	return _("JMesh (Markos version)");
}

wxMenu *CRAbMesh::MJMeshModule::GetMenu(unsigned int i, wxString &title) const
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

wxAuiToolBar *CRAbMesh::MJMeshModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::MJMeshModule::GetPanel(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("Configuration");
		break;
	case 1:
		title = _("Mesh items");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->panels.size()) ? this->panels[i] : NULL;
}

bool CRAbMesh::MJMeshModule::LoadFile(const std::string &filename)
{
	/*this->drive->setInputFileName(filename);

	bool loaded = this->drive->loadFile();

	this->drive->setInputFileName("");*/

	bool loaded = Module::LoadFile(filename);

	if (loaded)
	{
		//Module::LoadFile(filename);

		this->FillTree();

		MJMesh::MainDrive *drive = dynamic_cast<MJMesh::MainDrive *>(this->drive);

		this->SetTreeFactor(drive->getTreeFactor());
		this->SetTolerance(Data::getTolerance());

		if (drive->getDimension() == 2)
		{
			this->frame->GetViewModule()->SetDepthTestEnabled(false);
			this->frame->GetViewModule()->SetCullFaceEnabled(false);

			this->SetMaxBackTracks(10);
		}
		else if (drive->getDimension() == 3)
		{
			this->frame->GetViewModule()->SetDepthTestEnabled(true);
			this->frame->GetViewModule()->SetCullFaceEnabled(false);

			this->SetMaxBackTracks(20);
		}

		this->tcTreeFactor->SetFocus();
	}

	return loaded;
}

bool CRAbMesh::MJMeshModule::SaveFile(const std::string &filename)
{
	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	return saved;
}

void CRAbMesh::MJMeshModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		MJMesh::MainDrive *drive = dynamic_cast<MJMesh::MainDrive *>(this->drive);

		Data::setTolerance(this->GetTolerance());

		drive->setTreeFactor(this->GetTreeFactor());
		drive->setBoundarySorted(this->IsBoundarySorted());
#if USE_NUMERICAL
		drive->setHotellingTransformEnabled(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
		drive->setUsingUniformRefinement(this->IsUsingUniformRefinement());
		drive->setNumImproves(this->GetNumImproves());
		drive->setLocalBackTrackingEnabled(this->IsLocalBackTrackingEnabled());
		drive->setMeshSorted(this->IsMeshSorted());
		drive->setMaxBackTracks(this->GetMaxBackTracks());
	}

	Data::setStepByStep(this->IsStepByStep() || this->IsProgressShown());
}

void CRAbMesh::MJMeshModule::Execute()
{
	this->frame->GetViewModule()->Execute();

	//get parameters for main drive
	if (this->tcTreeFactor)
	{
		double treeFactor = 0.0;

		this->tcTreeFactor->GetValue().ToDouble(&treeFactor);

		this->SetTreeFactor(treeFactor);

		this->tcTreeFactor->Enable(false);
	}

	if (this->tcTolerance)
	{
		double tolerance;

		this->tcTolerance->GetValue().ToDouble(&tolerance);

		this->SetTolerance(tolerance);

		this->tcTolerance->Enable(false);
	}


	//disable items
	if (this->miConfConfiguration) this->miConfConfiguration->Enable(false);
	//if (this->miConfEnableBoundaryAdapt) this->miConfEnableBoundaryAdapt->Enable(false);
	if (this->miConfSortBoundaryElements) this->miConfSortBoundaryElements->Enable(false);
#if USE_NUMERICAL
	if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable(false);
	if (this->miConfEnableLocalBackTracking) this->miConfEnableLocalBackTracking->Enable(false);
	if (this->miConfSortMesh) this->miConfSortMesh->Enable(false);
	//if (this->miConfStepByStep) this->miConfStepByStep->Enable(false);

	if (this->miExecuteExecute) this->miExecuteExecute->Enable(false);
	if (this->miExecuteClear) this->miExecuteClear->Enable(false);
	if (this->miExecuteResults) this->miExecuteResults->Enable(false);

	//if (this->cbEnableBoundaryAdapt) this->cbEnableBoundaryAdapt->Enable(false);
	if (this->cbSortBoundaryElements) this->cbSortBoundaryElements->Enable(false);
#if USE_NUMERICAL
	if (this->cbEnableHotellingTransform) this->cbEnableHotellingTransform->Enable(false);
#endif //#if USE_NUMERICAL
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable(false);
	if (this->cbEnableLocalBackTracking) this->cbEnableLocalBackTracking->Enable(false);
	if (this->cbSortMesh) this->cbSortMesh->Enable(false);
	//if (this->cbStepByStep) this->cbStepByStep->Enable(false);

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);

	//if (this->gWait) this->gWait->Enable();
}

void CRAbMesh::MJMeshModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, (pending && (!this->IsStepByStep()) && (this->IsProgressShown())));

	//this->gWait->SetValue(0);
	//this->gWait->Enable(false);

	MJMesh::MainDrive *drive = dynamic_cast<MJMesh::MainDrive *>(this->drive);

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
	}
}

void CRAbMesh::MJMeshModule::Clear(bool onlyDrive, bool destroying)
{
	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		/*MJMesh::MainDrive *drive = dynamic_cast<MJMesh::MainDrive *>(this->drive);

		drive->clear();*/

		return;
	}

	//clear tree items
	this->ClearTree();

    //enable and reset items
    if (this->miConfConfiguration) this->miConfConfiguration->Enable();
    //if (this->miConfEnableBoundaryAdapt) this->miConfEnableBoundaryAdapt->Enable();
    if (this->miConfSortBoundaryElements) this->miConfSortBoundaryElements->Enable();
#if USE_NUMERICAL
    if (this->miConfEnableHotellingTransform) this->miConfEnableHotellingTransform->Enable();
#endif //#if USE_NUMERICAL
    if (this->miConfUseUniformRefinement) this->miConfUseUniformRefinement->Enable();
    if (this->miConfEnableLocalBackTracking) this->miConfEnableLocalBackTracking->Enable();
    if (this->miConfSortMesh) this->miConfSortMesh->Enable();
    //if (this->miConfStepByStep) this->miConfStepByStep->Enable();

    if (this->tcTreeFactor) this->tcTreeFactor->Enable();
	if (this->tcTolerance) this->tcTolerance->Enable();

	//if (this->cbEnableBoundaryAdapt) this->cbEnableBoundaryAdapt->Enable();
	if (this->cbSortBoundaryElements) this->cbSortBoundaryElements->Enable();
#if USE_NUMERICAL
	if (this->cbEnableHotellingTransform) this->cbEnableHotellingTransform->Enable();
#endif //#if USE_NUMERICAL
	if (this->cbUseUniformRefinement) this->cbUseUniformRefinement->Enable();
	if (this->cbEnableLocalBackTracking) this->cbEnableLocalBackTracking->Enable();
	if (this->cbSortMesh) this->cbSortMesh->Enable();
	//if (this->cbStepByStep) this->cbStepByStep->Enable();
	//if (this->tcElapsedTime) this->tcElapsedTime->SetValue(_("0 s"));
	//if (this->tcTotalTime) this->tcTotalTime->SetValue(_("0 s"));

	//remove string, if any
	if (this->sbsQuality)
	{
		wxBoxSizer *pConfSizer = static_cast<wxBoxSizer *>(this->GetPanel(0)->GetSizer());

		this->sbsQuality->Clear(true);

		pConfSizer->Detach(this->sbsQuality);

		delete this->sbsQuality;

		this->sbsQuality = NULL;

		pConfSizer->Layout();
		this->GetPanel(0)->FitInside();
	}
}

void CRAbMesh::MJMeshModule::SetTreeFactor(double treeFactor)
{
	this->treeFactor = treeFactor;

	wxString s;
    s << this->treeFactor;
    this->tcTreeFactor->SetValue(s);

    //this->confDialog->Update();
}

double CRAbMesh::MJMeshModule::GetTreeFactor() const
{
	return this->treeFactor;
}

void CRAbMesh::MJMeshModule::SetTolerance(double tolerance)
{
	this->tolerance = tolerance;

    wxString s;
    s << this->tolerance;
    this->tcTolerance->SetValue(s);

    //this->confDialog->Update();
}

double CRAbMesh::MJMeshModule::GetTolerance() const
{
	return this->tolerance;
}

void CRAbMesh::MJMeshModule::SetSeachTreesEnabled(bool searchTreesEnabled)
{
	this->searchTreesEnabled = searchTreesEnabled;

	//this->frame->UpdateConfiguration();
    //this->confDialog->Update();
}

bool CRAbMesh::MJMeshModule::IsSearchTreesEnabled() const
{
	return this->searchTreesEnabled;
}

/*void CRAbMesh::MJMeshModule::SetBoundaryAdapted(bool boundaryAdapted)
{
	this->boundaryAdapted = boundaryAdapted;

	this->frame->UpdateConfiguration();
}

bool CRAbMesh::MJMeshModule::IsBoundaryAdapted() const
{
	return this->boundaryAdapted;
}*/

void CRAbMesh::MJMeshModule::SetBoundarySorted(bool boundarySorted)
{
	this->boundarySorted = boundarySorted;

    this->miConfSortBoundaryElements->Check(this->boundarySorted);
    this->cbSortBoundaryElements->SetValue(this->boundarySorted);
}

bool CRAbMesh::MJMeshModule::IsBoundarySorted() const
{
	return this->boundarySorted;
}

#if USE_NUMERICAL
void CRAbMesh::MJMeshModule::SetHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;

    this->miConfEnableHotellingTransform->Check(this->hotellingTransformEnabled);
    this->cbEnableHotellingTransform->SetValue(this->hotellingTransformEnabled);
}

bool CRAbMesh::MJMeshModule::IsHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void CRAbMesh::MJMeshModule::SetUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;

    this->miConfUseUniformRefinement->Check(this->usingUniformRefinement);
    this->cbUseUniformRefinement->SetValue(this->usingUniformRefinement);
}

bool CRAbMesh::MJMeshModule::IsUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

void CRAbMesh::MJMeshModule::SetLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;

    this->miConfEnableLocalBackTracking->Check(this->localBackTrackingEnabled);
    this->cbEnableLocalBackTracking->SetValue(this->localBackTrackingEnabled);
}

bool CRAbMesh::MJMeshModule::IsLocalBackTrackingEnabled() const
{
	return this->localBackTrackingEnabled;
}

void CRAbMesh::MJMeshModule::SetMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;

    this->miConfSortMesh->Check(this->meshSorted);
    this->cbSortMesh->SetValue(this->meshSorted);
}

bool CRAbMesh::MJMeshModule::IsMeshSorted() const
{
	return this->meshSorted;
}

void CRAbMesh::MJMeshModule::SetNumImproves(unsigned int numImproves)
{
	this->numImproves = numImproves;

    /*wxString s;
    s << this->numImproves;
    this->tcNumImproves->SetValue(s);*/
    //this->confDialog->Update();
}

unsigned int CRAbMesh::MJMeshModule::GetNumImproves() const
{
	return this->numImproves;
}

void CRAbMesh::MJMeshModule::SetMaxBackTracks(unsigned int maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;

    /*wxString s;
    s << this->maxBackTracks;
    this->tcMaxBackTracks->SetValue(s);*/
    //this->confDialog->Update();
}

unsigned int CRAbMesh::MJMeshModule::GetMaxBackTracks() const
{
	return this->maxBackTracks;
}

void CRAbMesh::MJMeshModule::SetStepByStep(bool stepByStep)
{
    if ((!Data::isStepByStep()) && (!this->IsStepByStep()) && (!this->IsProgressShown()) && (stepByStep))
	{
		Data::setStepByStep(true);
	}

    this->stepByStep = stepByStep;

    this->miConfStepByStep->Check(this->stepByStep);
    this->cbStepByStep->SetValue(this->stepByStep);

    if (this->IsStepByStep())
	{
		this->miConfShowProgress->Enable(false);
		this->cbShowProgress->Enable(false);
	}
	else
	{
		this->miConfShowProgress->Enable(true);
		this->cbShowProgress->Enable(true);
	}
}

bool CRAbMesh::MJMeshModule::IsStepByStep() const
{
    return this->stepByStep;
}

void CRAbMesh::MJMeshModule::SetProgressShown(bool progressShown)
{
	if ((!Data::isStepByStep()) && (!this->IsStepByStep()) && (!this->IsProgressShown()) && (progressShown))
	{
		Data::setStepByStep(true);
	}

	this->progressShown = progressShown;

	this->miConfShowProgress->Check(this->progressShown);
    this->cbShowProgress->SetValue(this->progressShown);
}

bool CRAbMesh::MJMeshModule::IsProgressShown() const
{
	return this->progressShown;
}

void CRAbMesh::MJMeshModule::SetGUIState()
{
	Data::setTolerance(this->tolerance);
	Data::setStepByStep(this->stepByStep);

	wxString s = _("");
	s << Data::getTolerance();

	this->tcTolerance->SetValue(s);
	this->cbStepByStep->SetValue(Data::isStepByStep());
}

void CRAbMesh::MJMeshModule::MakeMainDrive()
{
	this->drive = new MJMesh::MainDrive();
}

void CRAbMesh::MJMeshModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	this->menus.push_back(this->MakeExecuteMenu());
}

void CRAbMesh::MJMeshModule::MakeToolbars()
{
	//Module::MakeToolbars();
}

void CRAbMesh::MJMeshModule::MakePanels()
{
	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());

	//Module::MakePanels();
}

wxMenu *CRAbMesh::MJMeshModule::MakeConfigurationMenu()
{
	//wxMenu *mConfiguration = new wxMenu();
	wxMenu *mConfiguration = MeshModule::MakeConfigurationMenu();

	unsigned int row = 0;

	this->miConfConfiguration = mConfiguration->Insert(row++, wxID_PROPERTIES, _("&Configuration..."));
    mConfiguration->InsertSeparator(row++);
    //this->miConfEnableBoundaryAdapt     = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Enable boundary adapt"));
    this->miConfSortBoundaryElements    = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Advance based on element size"));
#if USE_NUMERICAL
    this->miConfEnableHotellingTransform = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Enable Hotelling transform"));
#endif //#if USE_NUMERICAL
    this->miConfUseUniformRefinement    = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Generate a uniform mesh"));
    this->miConfEnableLocalBackTracking = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Enable local back-tracking"));
    this->miConfSortMesh                = mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Sort mesh"));
    mConfiguration->InsertSeparator(row++);
    this->miConfStepByStep   			= mConfiguration->InsertCheckItem(row++, wxID_ANY, _("&Step by step"));
    this->miConfShowProgress 			= mConfiguration->InsertCheckItem(row++, wxID_ANY, _("Show &progress"));

    if (mConfiguration->GetMenuItemCount() != row)
	{
		mConfiguration->InsertSeparator(row++);
	}

	//this->miConfEnableBoundaryAdapt->Check(this->IsBoundaryAdapted());
    this->miConfSortBoundaryElements->Check(this->IsBoundarySorted());
#if USE_NUMERICAL
    this->miConfEnableHotellingTransform->Check(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
    this->miConfUseUniformRefinement->Check(this->IsUsingUniformRefinement());
    this->miConfEnableLocalBackTracking->Check(this->IsLocalBackTrackingEnabled());
    this->miConfSortMesh->Check(this->IsMeshSorted());

    this->miConfStepByStep->Check(this->IsStepByStep());
    this->miConfShowProgress->Check(this->IsProgressShown());

    this->Connect(this->miConfConfiguration->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfConfiguration));

    //this->Connect(this->miConfEnableBoundaryAdapt->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfEnableBoundaryAdapt));
    this->Connect(this->miConfSortBoundaryElements->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfSortBoundaryElements));
#if USE_NUMERICAL
    this->Connect(this->miConfEnableHotellingTransform->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->miConfUseUniformRefinement->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfUseUniformRefinement));
    this->Connect(this->miConfEnableLocalBackTracking->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfEnableLocalBackTracking));
    this->Connect(this->miConfSortMesh->GetId(),                wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfSortMesh));

    this->Connect(this->miConfStepByStep->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfStepByStep));
    this->Connect(this->miConfShowProgress->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnConfShowProgress));

    return mConfiguration;
}

wxMenu *CRAbMesh::MJMeshModule::MakeExecuteMenu()
{
	wxMenu *mExecute = new wxMenu();

	//this->miExecuteExecute = mExecute->Append(wxID_ANY, _("&Execute\tEnter"));
    //this->miExecuteExecute = mExecute->Append(wxID_EXECUTE, _("&Execute\tEnter"));
    this->miExecuteExecute = mExecute->Append(wxID_OK, _("&Execute\tEnter"));
    //this->miExecuteClear   = mExecute->Append(wxID_ANY, _("&Clear"));
    this->miExecuteClear   = mExecute->Append(wxID_CLEAR, _("&Clear"));
    mExecute->AppendSeparator();
    //this->miExecuteResults = mExecute->Append(wxID_ANY, _("&Results"));
    this->miExecuteResults = mExecute->Append(wxID_INDEX, _("&Results"));

    this->Connect(this->miExecuteExecute->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnExecute));
    this->Connect(this->miExecuteClear->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnClear));

    this->Connect(this->miExecuteResults->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MJMeshModule::OnResults));

    return mExecute;
}

wxPanel *CRAbMesh::MJMeshModule::MakeConfigurationPanel()
{
	//wxPanel *pConfiguration = new wxPanel(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 440), wxTAB_TRAVERSAL);
	//wxPanel *pConfiguration = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 440), wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
	//static_cast<wxScrolledWindow *>(pConfiguration)->SetScrollRate(5, 5);

	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel
    //text boxes
    wxStaticText *stTreeFactor = new wxStaticText(pConfiguration, wxID_ANY, _("Tree factor:"));
    this->tcTreeFactor         = new wxTextCtrl  (pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    wxStaticText *stTolerance  = new wxStaticText(pConfiguration, wxID_ANY, _("Tolerance:"));
    this->tcTolerance          = new wxTextCtrl  (pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	//check boxes
    //this->cbEnableBoundaryAdapt     = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable boundary adapt"));
    this->cbSortBoundaryElements    = new wxCheckBox(pConfiguration, wxID_ANY, _("Advance based on element size"));
#if USE_NUMERICAL
    this->cbEnableHotellingTransform = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable Hotelling transform"));
#endif //#if USE_NUMERICAL
    this->cbUseUniformRefinement    = new wxCheckBox(pConfiguration, wxID_ANY, _("Generate a uniform mesh"));
    this->cbEnableLocalBackTracking = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable local back-tracking"));
    this->cbSortMesh                = new wxCheckBox(pConfiguration, wxID_ANY, _("Sort mesh"));

    this->cbStepByStep   = new wxCheckBox(pConfiguration, wxID_ANY, _("Step by step"));
    this->cbShowProgress = new wxCheckBox(pConfiguration, wxID_ANY, _("Show progress"));

    //buttons
    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

    //Gauge
    //this->gWait = new wxGauge(pConfiguration, wxID_ANY, 100);*/

	//default values
    wxString s;

    s = _("");
    s << this->GetTreeFactor();
    this->tcTreeFactor->SetValue(s);
    s = _("");
    s << this->GetTolerance();
    this->tcTolerance->SetValue(s);

    //this->cbEnableBoundaryAdapt->SetValue(this->IsBoundaryAdapted());
    this->cbSortBoundaryElements->SetValue(this->IsBoundarySorted());
#if USE_NUMERICAL
    this->cbEnableHotellingTransform->SetValue(this->IsHotellingTransformEnabled());
#endif //#if USE_NUMERICAL
    this->cbUseUniformRefinement->SetValue(this->IsUsingUniformRefinement());
    this->cbEnableLocalBackTracking->SetValue(this->IsLocalBackTrackingEnabled());
    this->cbSortMesh->SetValue(this->IsMeshSorted());
	this->cbStepByStep->SetValue(this->IsStepByStep());
	this->cbShowProgress->SetValue(this->IsProgressShown());

    if (this->IsStepByStep())
	{
		this->miConfShowProgress->Enable(false);
		this->cbShowProgress->Enable(false);
	}
	else
	{
		this->miConfShowProgress->Enable(true);
		this->cbShowProgress->Enable(true);
	}

    this->tcTreeFactor->SetFocus();
    this->bExecute->SetDefault();

    //Sizeres
    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    //this->sbsQuality = NULL;

    //wxStaticBoxSizer *sbsWait = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Please wait"));
    //sbsWait->Add(this->gWait, 1, wxEXPAND | wxALL, 0);*/

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
	gbsConf->AddGrowableCol(0);
    gbsConf->Add(stTreeFactor,                    wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gbsConf->Add(this->tcTreeFactor,              wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    gbsConf->Add(stTolerance,                     wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gbsConf->Add(this->tcTolerance,               wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    //gbsConf->Add(0, 4,                            wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    //gbsConf->Add(this->cbEnableBoundaryAdapt,     wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gbsConf->Add(this->cbSortBoundaryElements,    wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#if USE_NUMERICAL
    gbsConf->Add(this->cbEnableHotellingTransform, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#endif //#if USE_NUMERICAL
    gbsConf->Add(this->cbUseUniformRefinement,    wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gbsConf->Add(this->cbEnableLocalBackTracking, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gbsConf->Add(this->cbSortMesh,                wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    //gbsConf->Add(0, 4,                            wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
	gbsConf->Add(this->cbStepByStep,              wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
	gbsConf->Add(this->cbShowProgress,            wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

    //wxBoxSizer *sConf = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
    bsConf->Add(gbsConf, 0, wxEXPAND | wxALL, 1);
    bsConf->Add(sConf, 0, wxEXPAND | wxALL, 1);
    bsConf->Add(sbsExecute, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
    pConfiguration->SetSizer(bsConf, false);

    this->tcTreeFactor->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(MJMeshModule::OnKillFocus), NULL, this);
    this->tcTolerance ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(MJMeshModule::OnKillFocus), NULL, this);

    //this->Connect(this->cbEnableBoundaryAdapt->GetId(),      wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnEnableBoundaryAdapt));
    this->Connect(this->cbSortBoundaryElements->GetId(),     wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfSortBoundaryElements));
#if USE_NUMERICAL
    this->Connect(this->cbEnableHotellingTransform->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfEnableHotellingTransform));
#endif //#if USE_NUMERICAL
    this->Connect(this->cbUseUniformRefinement->GetId(),     wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfUseUniformRefinement));
    this->Connect(this->cbEnableLocalBackTracking->GetId(),  wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfEnableLocalBackTracking));
    this->Connect(this->cbSortMesh->GetId(),                 wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfSortMesh));

    this->Connect(this->cbStepByStep->GetId(),   wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfStepByStep));
    this->Connect(this->cbShowProgress->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MJMeshModule::OnConfShowProgress));

    this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MJMeshModule::OnExecute));
    this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MJMeshModule::OnClear));

	return pConfiguration;
}

wxDialog *CRAbMesh::MJMeshModule::MakeConfigurationDialog()
{
	wxPropertySheetDialog *dConf = new wxPropertySheetDialog();

	dConf->Create(this->frame, wxID_ANY, _("Configuration"));
	dConf->CreateButtons(wxOK | wxCANCEL);

	wxBookCtrlBase *nConf = dConf->GetBookCtrl();

	wxPanel *pConf = new wxPanel(nConf);

	wxStaticText *stTreeFactor    = new wxStaticText(pConf, wxID_ANY, _("Tree factor:"));
    wxTextCtrl *tcTreeFactor      = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    wxStaticText *stTolerance     = new wxStaticText(pConf, wxID_ANY, _("Tolerance:"));
    wxTextCtrl *tcTolerance       = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    wxStaticText *stNumImproves   = new wxStaticText(pConf, wxID_ANY, _("Number of improvements:"));
    wxTextCtrl *tcNumImproves     = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    wxStaticText *stMaxBackTracks = new wxStaticText(pConf, wxID_ANY, _("Maximum number of back-tracks:"));
    wxTextCtrl *tcMaxBackTracks   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxString s;
    s = _("");
    s << this->GetTreeFactor();
    tcTreeFactor->SetValue(s);
    s = _("");
    s << this->GetTolerance();
    tcTolerance->SetValue(s);
    s = _("");
    s << this->GetNumImproves();
    tcNumImproves->SetValue(s);
    s = _("");
    s << this->GetMaxBackTracks();
    tcMaxBackTracks->SetValue(s);

    tcTreeFactor->SetFocus();

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
	gbsConf->Add(stTreeFactor,    wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcTreeFactor,    wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stTolerance,     wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcTolerance,     wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    //gbsConf->Add(0, 4,            wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gbsConf->Add(stNumImproves,   wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcNumImproves,   wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stMaxBackTracks, wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcMaxBackTracks, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->AddGrowableCol(0);
	gbsConf->AddGrowableCol(1);

	wxBoxSizer *sCond = new wxBoxSizer(wxVERTICAL);
	sCond->Add(gbsConf, 1, wxEXPAND | wxALL, 0);
	pConf->SetSizer(sCond);

	nConf->AddPage(pConf, _("Configuration"), true);

	dConf->LayoutDialog();

	return dConf;
}

void CRAbMesh::MJMeshModule::OnConfConfiguration(wxCommandEvent &WXUNUSED(event))
{
	wxDialog *dConf = this->MakeConfigurationDialog();

	if (dConf->ShowModal() == wxID_OK)
	{
		this->OnConfigurationDialogOk(dConf);
	}

	dConf->Destroy();
}

void CRAbMesh::MJMeshModule::OnConfigurationDialogOk(const wxDialog *dConf)
{
	const wxPropertySheetDialog *psdConf = static_cast<const wxPropertySheetDialog *>(dConf);

	double treeFactor, tolerance;
	unsigned long int numImproves, maxBackTracks;

	wxWindow *pConf = psdConf->GetBookCtrl()->GetPage(0);

	static_cast<wxTextCtrl *>(pConf->GetChildren()[1])->GetValue().ToDouble(&treeFactor);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[3])->GetValue().ToDouble(&tolerance);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[5])->GetValue().ToULong(&numImproves);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[7])->GetValue().ToULong(&maxBackTracks);

	this->SetTreeFactor(treeFactor);
	this->SetTolerance(tolerance);
	this->SetNumImproves(static_cast<unsigned int>(numImproves));
	this->SetMaxBackTracks(static_cast<unsigned int>(maxBackTracks));
}

/*void CRAbMesh::MJMeshModule::OnConfEnableBoundaryAdapt(wxCommandEvent &WXUNUSED(event))
{
	this->SetBoundaryAdapted(!this->IsBoundaryAdapted());
}*/

void CRAbMesh::MJMeshModule::OnConfSortBoundaryElements(wxCommandEvent &WXUNUSED(event))
{
	this->SetBoundarySorted(!this->IsBoundarySorted());
}

#if USE_NUMERICAL
void CRAbMesh::MJMeshModule::OnConfEnableHotellingTransform(wxCommandEvent &WXUNUSED(event))
{
	this->SetHotellingTransformEnabled(!this->IsHotellingTransformEnabled());
}
#endif //#if USE_NUMERICAL

void CRAbMesh::MJMeshModule::OnConfUseUniformRefinement(wxCommandEvent &WXUNUSED(event))
{
	this->SetUsingUniformRefinement(!this->IsUsingUniformRefinement());
}

void CRAbMesh::MJMeshModule::OnConfEnableLocalBackTracking(wxCommandEvent &WXUNUSED(event))
{
	this->SetLocalBackTrackingEnabled(!this->IsLocalBackTrackingEnabled());
}

void CRAbMesh::MJMeshModule::OnConfSortMesh(wxCommandEvent &WXUNUSED(event))
{
	this->SetMeshSorted(!this->IsMeshSorted());
}

void CRAbMesh::MJMeshModule::OnConfStepByStep(wxCommandEvent &WXUNUSED(event))
{
	this->SetStepByStep(!this->IsStepByStep());
}

void CRAbMesh::MJMeshModule::OnConfShowProgress(wxCommandEvent &WXUNUSED(event))
{
	this->SetProgressShown(!this->IsProgressShown());
}

void CRAbMesh::MJMeshModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::MJMeshModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

void CRAbMesh::MJMeshModule::OnResults(wxCommandEvent &WXUNUSED(event))
{
	wxString s = this->QualityString();

	if (s.IsSameAs(_("")))
	{
		s = _("No results yet!");
	}

	wxMessageBox(s, _("Results"));
}

void CRAbMesh::MJMeshModule::OnKillFocus(wxFocusEvent &event)
{
	if (event.GetId() == this->tcTreeFactor->GetId())
	{
		double treeFactor;

		this->tcTreeFactor->GetValue().ToDouble(&treeFactor);

		this->SetTreeFactor(treeFactor);
	}
	else if (event.GetId() == this->tcTolerance->GetId())
	{
		double tolerance;

		this->tcTolerance->GetValue().ToDouble(&tolerance);

		this->SetTolerance(tolerance);
	}

	event.Skip(true);
}

void CRAbMesh::MJMeshModule::SetDimension(unsigned int dimension)
{
	dynamic_cast<MJMesh::MainDrive *>(this->drive)->setDimension(static_cast<unsigned int>(dimension));
}

unsigned int CRAbMesh::MJMeshModule::GetDimension() const
{
	return static_cast<unsigned int>(dynamic_cast<MJMesh::MainDrive *>(this->drive)->getDimension());
}

void CRAbMesh::MJMeshModule::SetTree(Data::BoxTree *tree)
{
	dynamic_cast<MJMesh::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::MJMeshModule::GetTree() const
{
	return dynamic_cast<MJMesh::MainDrive *>(this->drive)->getTree();
}

void CRAbMesh::MJMeshModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<MJMesh::MainDrive *>(this->drive)->setBoundary(dynamic_cast<MJMesh::Boundary *>(boundary));
}

const Data::Boundary *CRAbMesh::MJMeshModule::GetBoundary() const
{
	return dynamic_cast<MJMesh::MainDrive *>(this->drive)->getBoundary();
}

void CRAbMesh::MJMeshModule::SetFront(Data::Front *front)
{
	dynamic_cast<MJMesh::MainDrive *>(this->drive)->setFront(static_cast<MJMesh::Front *>(front));
}

const Data::Front *CRAbMesh::MJMeshModule::GetFront() const
{
	return dynamic_cast<MJMesh::MainDrive *>(this->drive)->getFront();
}

void CRAbMesh::MJMeshModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<MJMesh::MainDrive *>(this->drive)->setMesh(static_cast<MJMesh::Mesh *>(mesh));
}

const Data::Mesh *CRAbMesh::MJMeshModule::GetMesh() const
{
	return dynamic_cast<MJMesh::MainDrive *>(this->drive)->getMesh();
}

wxString CRAbMesh::MJMeshModule::QualityString() const
{
	wxString s(dynamic_cast<MJMesh::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}

#endif //#if USE_GUI
