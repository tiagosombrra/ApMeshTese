#include "CRAbMesh/TecGrafMshSurfModule.h"

#if USE_GUI
#if USE_TECGRAF_MSH_SURF

#include <wx/gbsizer.h>
#include <wx/choicebk.h>
#include "Data/OStream.h"
#include "Data/Box.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/ViewModule.h"
#include "CRAbMesh/TreeItemData.h"
#include "TecGraf/Definitions.h"
#include "TecGraf/MshSurf/MainDrive.h"

CRAbMesh::Redirector *CRAbMesh::TecGrafMshSurfModule::mshSurfRedirector = NULL;

void CRAbMesh::mshSurfPrint(char *msg)
{
    if (CRAbMesh::TecGrafMshSurfModule::mshSurfRedirector)
    {
        CRAbMesh::TecGrafMshSurfModule::mshSurfRedirector->vprint(msg);
    }
}

CRAbMesh::TecGrafMshSurfModule::TecGrafMshSurfModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule::MeshModule(frame, canvas)
{
	SUPPORT_VERTICES = "Support mesh vertices";
	SUPPORT_FRONT_ELEMENTS = "Support mesh front elements";
	SUPPORT_ELEMENTS = "Support mesh elements";

	DRAW_SUPPORT_MESH = "Draw support mesh";

	this->InsertOption(SUPPORT_VERTICES,       TREE_CELLS,          TYPE_TREE_ITEM);
	this->InsertOption(SUPPORT_FRONT_ELEMENTS, SUPPORT_VERTICES,       TYPE_TREE_ITEM);
	this->InsertOption(SUPPORT_ELEMENTS,       SUPPORT_FRONT_ELEMENTS, TYPE_TREE_ITEM);
	this->InsertOption(DRAW_SUPPORT_MESH,      DRAW_MESH, TYPE_MENU_AND_CHECK_ITEM);

	this->SetConfiguration(DRAW_SUPPORT_MESH,     true, false);

	this->SetOption(VERTICES, true);
	this->SetOption(ELEMENTS, true);
	this->SetOption(FRONT_ELEMENTS, true);
	this->SetOption(TREE_CELLS, false);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, true, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, true, false);

	this->SetOption(DRAW_NORMAL, true, true);
	this->SetOption(DRAW_DISCRETIZATION, true, false);

	this->SetOption(DRAW_BOUNDARY, true, true);
	this->SetOption(DRAW_TREE, false, false);
	this->SetOption(DRAW_FRONT, true, true);
	this->SetOption(DRAW_MESH, true, true);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	this->SetOption(SUPPORT_VERTICES,       true);
	this->SetOption(SUPPORT_FRONT_ELEMENTS, true);
	this->SetOption(SUPPORT_ELEMENTS,       true);
	this->SetOption(DRAW_SUPPORT_MESH,      true, true);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}

	this->useRedirector = true;
}

CRAbMesh::TecGrafMshSurfModule::~TecGrafMshSurfModule()
{

}

wxString CRAbMesh::TecGrafMshSurfModule::Name() const
{
	return _("TecGraf MshSurf");
}

wxMenu *CRAbMesh::TecGrafMshSurfModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("&Configuration");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::TecGrafMshSurfModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::TecGrafMshSurfModule::GetPanel(unsigned int i, wxString &title) const
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

bool CRAbMesh::TecGrafMshSurfModule::LoadFile(const std::string &filename)
{
	bool loaded = CRAbMesh::MeshModule::LoadFile(filename);

	if (loaded)
	{
		//Module::LoadFile(filename);

		this->FillTree();

		//TecGraf::MshSurf::MainDrive *drive = dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive);

		this->frame->GetViewModule()->SetDepthTestEnabled(true);
		this->frame->GetViewModule()->SetCullFaceEnabled(false);

		if ((this->bcbAlgorithm->GetSelection() == 2) ||
            (this->bcbAlgorithm->GetSelection() == 3))
        {
            Real maxElementSize = 0.0;

            for (Data::Boundary::ConstFrontElementIterator iter = this->GetBoundary()->eBegin();
                 iter != this->GetBoundary()->eEnd(); iter++)
            {
                maxElementSize += (*iter)->getShape()->size();
            }

            maxElementSize /= static_cast<Real>(this->GetBoundary()->size());

            wxString s = wxString::Format("%.6f", maxElementSize);

            if (this->bcbAlgorithm->GetSelection() == 2)
            {
                this->tcAnalyticalMaxElementSize->SetValue(s);
            }
            else if (this->bcbAlgorithm->GetSelection() == 3)
            {
                this->tcSupportMaxElementSize->SetValue(s);
            }
        }
	}

	return loaded;
}

bool CRAbMesh::TecGrafMshSurfModule::SaveFile(const std::string &filename)
{
	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	return saved;
}

void CRAbMesh::TecGrafMshSurfModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		TecGraf::MshSurf::MainDrive *drive = dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive);

		if (this->bcbAlgorithm->GetSelection() == 2)
		{
			double maxElementSize = 0.0;
			bool computeCurvature = false;

			this->tcAnalyticalMaxElementSize->GetValue().ToDouble(&maxElementSize);
			computeCurvature = this->cbAnalyticalComputeCurvature->GetValue();

            drive->setAlgorithm(TecGraf::MshSurf::SURF_3D_ANALYTICAL);

            drive->setAnalyticalMaxElementSize(maxElementSize);
            drive->setAnalyticalComputeCurvature(computeCurvature);
		}
		else if (this->bcbAlgorithm->GetSelection() == 3)
		{
			double maxElementSize = 0.0;
			bool computeCurvature = false;
			bool invertMesh = false;

			this->tcSupportMaxElementSize->GetValue().ToDouble(&maxElementSize);
			computeCurvature = this->cbSupportComputeCurvature->GetValue();
			invertMesh = this->cbSupportInvertMesh->GetValue();

            drive->setAlgorithm(TecGraf::MshSurf::SURF_3D_SUPPORT);

            drive->setSupportMaxElementSize(maxElementSize);
            drive->setSupportComputeCurvature(computeCurvature);
            drive->setSupportInvertMesh(invertMesh);
		}

		if (this->useRedirector)
        {
            CRAbMesh::TecGrafMshSurfModule::mshSurfRedirector = &this->redirector;

            drive->setMessageFunction(CRAbMesh::mshSurfPrint);
        }
        else
        {
            CRAbMesh::TecGrafMshSurfModule::mshSurfRedirector = NULL;

            drive->setMessageFunction(NULL);
        }
	}
}

void CRAbMesh::TecGrafMshSurfModule::Execute()
{
	if (this->useRedirector)
    {
        this->redirector.Start(this->tcLog);
    }

	this->frame->GetViewModule()->Execute();

	//disable items
	if (this->bcbAlgorithm) this->bcbAlgorithm->Enable(false);

	if (this->tcAnalyticalMaxElementSize) this->tcAnalyticalMaxElementSize->Enable(false);
	if (this->cbAnalyticalComputeCurvature) this->cbAnalyticalComputeCurvature->Enable(false);

	if (this->bSupportLoadSupportMesh) this->bSupportLoadSupportMesh->Enable(false);
	if (this->tcSupportMaxElementSize) this->tcSupportMaxElementSize->Enable(false);
	if (this->cbSupportComputeCurvature) this->cbSupportComputeCurvature->Enable(false);
	if (this->cbSupportInvertMesh) this->cbSupportInvertMesh->Enable(false);

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);
}

void CRAbMesh::TecGrafMshSurfModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, pending);

	TecGraf::MshSurf::MainDrive *drive = dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive);

	if (clearing)
	{
		drive->clear();
	}

	if (this->useRedirector)
    {
        this->redirector.LogText(pending);
    }

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

void CRAbMesh::TecGrafMshSurfModule::Clear(bool onlyDrive, bool destroying)
{
	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		return;
	}

	//clear tree items
	this->ClearTree();

	if (this->tcLog)
	{
        this->tcLog->Clear();
    }

    //enable and reset items
    if (this->bcbAlgorithm) this->bcbAlgorithm->Enable();

    if (this->tcAnalyticalMaxElementSize) this->tcAnalyticalMaxElementSize->Enable();
	if (this->cbAnalyticalComputeCurvature) this->cbAnalyticalComputeCurvature->Enable();

	if (this->bSupportLoadSupportMesh) this->bSupportLoadSupportMesh->Enable();
	if (this->tcSupportMaxElementSize) this->tcSupportMaxElementSize->Enable();
	if (this->cbSupportComputeCurvature) this->cbSupportComputeCurvature->Enable();
	if (this->cbSupportInvertMesh) this->cbSupportInvertMesh->Enable();

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

void CRAbMesh::TecGrafMshSurfModule::SetGUIState()
{

}

void CRAbMesh::TecGrafMshSurfModule::SetSupportMeshDrawable(bool supportMeshDrawable)
{
	this->SetConfiguration(DRAW_SUPPORT_MESH, supportMeshDrawable);
}

bool CRAbMesh::TecGrafMshSurfModule::IsSupportMeshDrawable() const
{
	return this->IsConfiguration(DRAW_SUPPORT_MESH);
}

bool CRAbMesh::TecGrafMshSurfModule::IsDrawable() const
{
	return (MeshModule::IsDrawable() ||
			this->IsSupportMeshDrawable());
}

void CRAbMesh::TecGrafMshSurfModule::MakeMainDrive()
{
	this->drive = new TecGraf::MshSurf::MainDrive();
}

void CRAbMesh::TecGrafMshSurfModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
}

void CRAbMesh::TecGrafMshSurfModule::MakeToolbars()
{

}

void CRAbMesh::TecGrafMshSurfModule::MakePanels()
{
	this->tcLog = NULL;

	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
	//this->panels.push_back(this->MakeLogPanel());
}

wxPanel *CRAbMesh::TecGrafMshSurfModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel
    this->bcbAlgorithm = new wxChoicebook(pConfiguration, wxID_ANY);
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Edge"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Create 3D bil"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Analytical"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Support"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Create 2D"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Create 2D edge"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Edge 2D"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Template"));

    wxStaticText *stAnalyticalMaxElementSize = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Max element size"));
    this->tcAnalyticalMaxElementSize         = new wxTextCtrl(this->bcbAlgorithm->GetPage(2),   wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbAnalyticalComputeCurvature       = new wxCheckBox(this->bcbAlgorithm->GetPage(2),   wxID_ANY, _("Compute curvature"));

    wxStaticText *stSupportMaxElementSize = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Max element size"));
    this->tcSupportMaxElementSize         = new wxTextCtrl(this->bcbAlgorithm->GetPage(3),   wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbSupportComputeCurvature       = new wxCheckBox(this->bcbAlgorithm->GetPage(3),   wxID_ANY, _("Compute curvature"));
    this->cbSupportInvertMesh             = new wxCheckBox(this->bcbAlgorithm->GetPage(3),   wxID_ANY, _("Invert support mesh"));
    this->bSupportLoadSupportMesh         = new wxButton(this->bcbAlgorithm->GetPage(3),     wxID_ANY, _("Load support mesh"));

    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

    //default values
    this->cbAnalyticalComputeCurvature->SetValue(false);
    this->cbSupportComputeCurvature->SetValue(false);
    this->cbSupportInvertMesh->SetValue(false);

    //sizers
    wxFlexGridSizer *gsAnalytical = new wxFlexGridSizer(2);
    gsAnalytical->SetFlexibleDirection(wxVERTICAL);
    gsAnalytical->Add(stAnalyticalMaxElementSize,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsAnalytical->Add(this->tcAnalyticalMaxElementSize,   0, wxEXPAND | wxALL, 0);
    gsAnalytical->Add(this->cbAnalyticalComputeCurvature, 0, wxEXPAND | wxALL, 0);
    gsAnalytical->Add(1, 1,                               0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(2)->SetSizer(gsAnalytical);

    wxFlexGridSizer *gsSupport = new wxFlexGridSizer(2);
    gsSupport->SetFlexibleDirection(wxVERTICAL);
    gsSupport->Add(stSupportMaxElementSize,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsSupport->Add(this->tcSupportMaxElementSize,   0, wxEXPAND | wxALL, 0);
    gsSupport->Add(this->cbSupportComputeCurvature, 0, wxEXPAND | wxALL, 0);
    gsSupport->Add(1, 1,                            0, wxEXPAND | wxALL, 0);
    gsSupport->Add(this->cbSupportInvertMesh,       0, wxEXPAND | wxALL, 0);
    gsSupport->Add(1, 1,                            0, wxEXPAND | wxALL, 0);
    gsSupport->Add(this->bSupportLoadSupportMesh,   0, wxEXPAND | wxALL, 0);
    gsSupport->Add(1, 1,                            0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(3)->SetSizer(gsSupport);

    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
	bsConf->Add(this->bcbAlgorithm, 0, wxEXPAND | wxALL, 0);
	bsConf->Add(sbsExecute,         0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
	bsConf->Add(sConf,              0, wxEXPAND | wxALL, 0);
    pConfiguration->SetSizer(bsConf, false);

    this->Connect(this->bSupportLoadSupportMesh->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMshSurfModule::OnLoadSupportMesh));

    this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMshSurfModule::OnExecute));
    this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMshSurfModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::TecGrafMshSurfModule::MakeLogPanel()
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

void CRAbMesh::TecGrafMshSurfModule::OnLoadSupportMesh(wxCommandEvent &WXUNUSED(event))
{
	wxFileDialog *open = new wxFileDialog(this->frame, _("Open file"), this->frame->GetOpenPath(), _(""),
        this->frame->GetOpenWildcard(),
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
#else
        wxOPEN | wxFILE_MUST_EXIST);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (open->ShowModal() == wxID_OK)
    {
        wxString path = open->GetPath();

        std::string inputFile(path.mb_str(wxMBConvLibc()));
        std::string oldLoadedFilename = this->loadedFilename;

		bool opened = false;

		dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setSuportLoadingSupportMesh(true);

		opened = this->LoadFile(inputFile);

		dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setSuportLoadingSupportMesh(false);

        if (opened)
		{
            this->loadedFilename = oldLoadedFilename;

			this->canvas->FitCamera();

			this->canvas->Refresh(false);
		}
    }

    delete open;
}

void CRAbMesh::TecGrafMshSurfModule::OnTreeMiddleClick(wxTreeEvent &event)
{
    bool found = false;

	wxTreeItemId item = event.GetItem();

	const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

	if ((this->IsEnabled(SUPPORT_VERTICES, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[SUPPORT_VERTICES]))
	{
		Data::Point *p = static_cast<const Data::Vertex *>(d)->getPoint();

		if (p)
        {
            this->canvas->Center(p);

            this->canvas->Refresh(false);
            this->canvas->Update();

            found = true;
        }
	}
	else if (((this->IsEnabled(SUPPORT_FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[SUPPORT_FRONT_ELEMENTS])) ||
			 ((this->IsEnabled(SUPPORT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[SUPPORT_ELEMENTS])))
	{
		Data::GeometricShape *s = static_cast<const Data::Element *>(d)->getShape();

		if (s)
		{
			this->canvas->Center(s);

			this->canvas->Refresh(false);
			this->canvas->Update();

			found = true;
		}
	}

	if (!found)
    {
        CRAbMesh::MeshModule::OnTreeMiddleClick(event);
    }
}

void CRAbMesh::TecGrafMshSurfModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::TecGrafMshSurfModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

bool CRAbMesh::TecGrafMshSurfModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
	bool bounding = CRAbMesh::MeshModule::BoundingBox(minx, miny, minz, maxx, maxy, maxz);

	if (this->GetSupportMesh())
	{
		for (Data::Mesh::ConstVertexIterator iter = this->GetSupportMesh()->vBegin();
			 iter != this->GetSupportMesh()->vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

			if (v->getPoint()->getCoord(0) < minx)
			{
				minx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) < miny)
			{
				miny = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->getCoord(0) > maxx)
			{
				maxx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) > maxy)
			{
				maxy = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->dimension() == 2)
            {
                if (0.0 < minz)
                {
                    minz = 0.0;
                }

                if (0.0 > maxz)
                {
                    maxz = 0.0;
                }
            }
            else
            {
                if (v->getPoint()->getCoord(2) < minz)
                {
                    minz = v->getPoint()->getCoord(2);
                }

                if (v->getPoint()->getCoord(2) > maxz)
                {
                    maxz = v->getPoint()->getCoord(2);
                }
            }
		}

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	return bounding;
}

void CRAbMesh::TecGrafMshSurfModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::TecGrafMshSurfModule::GetDimension() const
{
	unsigned int dimension = 0;

	if ((dimension == 0) && (this->GetMesh()))
	{
		dimension = this->GetMesh()->dimension();
	}

	/*if ((dimension == 0) && (this->GetFront()))
	{
		dimension = this->GetFront()->dimension();
	}*/

	if ((dimension == 0) && (this->GetBoundary()))
	{
		dimension = this->GetBoundary()->dimension();
	}

	return dimension;
}

void CRAbMesh::TecGrafMshSurfModule::SetTree(Data::BoxTree */*tree*/)
{
	//dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::TecGrafMshSurfModule::GetTree() const
{
	//return dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->getTree();
	return NULL;
}

void CRAbMesh::TecGrafMshSurfModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setBoundary(static_cast<MJMesh::Boundary *>(boundary));
}

const Data::Boundary *CRAbMesh::TecGrafMshSurfModule::GetBoundary() const
{
	return dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->getBoundary();
}

void CRAbMesh::TecGrafMshSurfModule::SetFront(Data::Front */*front*/)
{
	//dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setFront(front);
}

const Data::Front *CRAbMesh::TecGrafMshSurfModule::GetFront() const
{
	//return dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->getFront();
	return NULL;
}

void CRAbMesh::TecGrafMshSurfModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::TecGrafMshSurfModule::GetMesh() const
{
	return dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->getMesh();
}

void CRAbMesh::TecGrafMshSurfModule::SetSupportMesh(Data::Mesh *mesh)
{
	dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->setSupportMesh(mesh);
}

const Data::Mesh *CRAbMesh::TecGrafMshSurfModule::GetSupportMesh() const
{
	return dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->getSupportMesh();
}

wxString CRAbMesh::TecGrafMshSurfModule::QualityString() const
{
	wxString s(dynamic_cast<TecGraf::MshSurf::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}

void CRAbMesh::TecGrafMshSurfModule::ClearDisplayLists(bool force) const
{
	MeshModule::ClearDisplayLists(force);

	if (this->GetSupportMesh())
	{
		if ((force) || (this->GetSupportMesh()->isOutdated()))
		{
			this->GetSupportMesh()->clearLists();
		}
	}
}

void CRAbMesh::TecGrafMshSurfModule::Render() const
{
	if (this->IsDrawable())
	{
#if USE_SCREEENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREEENSHOT

		if ((this->IsSupportMeshDrawable()) && (this->GetSupportMesh()))
		{
			this->GetSupportMesh()->draw();
		}
	}

	CRAbMesh::MeshModule::Render();

	if (this->IsDrawable())
	{
		wxTreeItemId item = this->tree->GetSelection();

		if (item.IsOk())
		{
			if (((this->IsEnabled(SUPPORT_VERTICES, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(SUPPORT_VERTICES)).second)) ||
				((this->IsEnabled(SUPPORT_FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == (*this->tiItems.find(SUPPORT_FRONT_ELEMENTS)).second)) ||
				((this->IsEnabled(SUPPORT_ELEMENTS, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(SUPPORT_ELEMENTS)).second)))
			{
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(SUPPORT_VERTICES)).second)
				{
					if ((this->IsAdjacentFrontElementHighlighted()) || (this->IsAdjacentElementHighlighted()))
					{
						Data::Vertex *v = static_cast<Data::Vertex *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						if (this->IsAdjacentElementHighlighted())
						{
							Data::ElementList adjs = (this->GetSupportMesh()) ?
								this->GetSupportMesh()->adjacency(v) : Data::ElementList();

							for (Data::ElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}

						if (this->IsAdjacentFrontElementHighlighted())
						{
							Data::FrontElementList adjs = (this->GetSupportMesh()) ?
                                this->GetSupportMesh()->findAdjacentFrontElements(v) : Data::FrontElementList();

							for (Data::FrontElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}
					}
				}
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(SUPPORT_FRONT_ELEMENTS)).second)
				{
					if (this->IsAdjacentElementHighlighted())
					{
						Data::FrontElement *fel = static_cast<Data::FrontElement *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						Data::ElementList adjs = (this->GetSupportMesh()) ?
							this->GetSupportMesh()->adjacency(fel) : Data::ElementList();

						for (Data::ElementList::const_iterator iter = adjs.begin();
							 iter != adjs.end(); iter++)
						{
							(*iter)->highlight();
							(*iter)->draw();
							(*iter)->unhighlight();
						}
					}
				}

				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->highlight();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->draw();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->unhighlight();
			}
		}
	}
}

void CRAbMesh::TecGrafMshSurfModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	if (this->IsEnabled(SUPPORT_VERTICES, TREE_ITEM))
	{
		if (this->GetSupportMesh())
		{
			Data::VertexList vertices = this->GetSupportMesh()->getVertices();
			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}
	}

	if (this->IsEnabled(SUPPORT_FRONT_ELEMENTS, TREE_ITEM))
	{
		if (this->GetSupportMesh())
		{
			Data::FrontElementList frontElements = this->GetSupportMesh()->getFrontElements();
			frontElements.sort(Data::Identifiable::less);

			while (!frontElements.empty())
			{
				this->AddTreeItem(frontElements.front());

				frontElements.pop_front();
			}
		}
	}

	if (this->IsEnabled(SUPPORT_ELEMENTS, TREE_ITEM))
	{
		if (this->GetSupportMesh())
		{
			Data::ElementList elements = this->GetSupportMesh()->getElements();

			elements.sort(Data::Identifiable::less);

			while (!elements.empty())
			{
				this->AddTreeItem(elements.front());

				elements.pop_front();
			}
		}
	}

	MeshModule::ForceFillTree(false);
}

void CRAbMesh::TecGrafMshSurfModule::AddTreeItem(Data::Vertex *v)
{
	bool added = false;

	if (this->IsEnabled(SUPPORT_VERTICES, TREE_ITEM))
	{
		if ((this->GetSupportMesh()) && (this->GetSupportMesh()->have(v)))
		{
			TreeItemData *item = new TreeItemData(v);
			this->tree->AppendItem(this->tiItems[SUPPORT_VERTICES], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(v);
	}
}

void CRAbMesh::TecGrafMshSurfModule::AddTreeItem(Data::FrontElement *e)
{
	bool added = false;

	if (this->IsEnabled(SUPPORT_FRONT_ELEMENTS, TREE_ITEM))
	{
		if ((this->GetSupportMesh()) && (this->GetSupportMesh()->have(e)))
		{
			TreeItemData *item = new TreeItemData(e);
			this->tree->AppendItem(this->tiItems[SUPPORT_FRONT_ELEMENTS], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(e);
	}
}

void CRAbMesh::TecGrafMshSurfModule::AddTreeItem(Data::Element *e)
{
	bool added = false;

	if (this->IsEnabled(SUPPORT_ELEMENTS, TREE_ITEM))
	{
		if ((this->GetSupportMesh()) && (this->GetSupportMesh()->have(e)))
		{
			TreeItemData *item = new TreeItemData(e);
			this->tree->AppendItem(this->tiItems[SUPPORT_ELEMENTS], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(e);
	}
}

#endif //#if USE_TECGRAF_MSH_SURF
#endif //#if USE_GUI
