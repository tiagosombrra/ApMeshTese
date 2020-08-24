#include "CRAbMesh/NumericalModule.h"

#if ((USE_GUI) && (USE_NUMERICAL) && (USE_EXPRTK))

#include <wx/gbsizer.h>
#include <wx/choicebk.h>
#include <wx/collpane.h>
#include <wx/filename.h>
#include <wx/thread.h>
#include "Data/OStream.h"
#include "Data/Point3D.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/ViewModule.h"
#include "Numerical/InterpretedFunction.h"
#include "Numerical/BisectionEquationRootFinder.h"
#include "Numerical/FalsePositionEquationRootFinder.h"
#include "Numerical/FixedPointEquationRootFinder.h"
#include "Numerical/NewtonRaphsonEquationRootFinder.h"
#include "Numerical/SteffensenEquationRootFinder.h"
#include "Numerical/SecantEquationRootFinder.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Mesh.h"
/*//debug
#include "Data/BSPTree.h"
#include "Data/BSPTreeCell.h"
#include "Data/Point2D.h"

void randomsubdivide(Data::BSPTreeCell *c)
{
	if (c->level() == 7)
	{
		return;
	}

	if ((rand()%4) == 0)
	{
		return;
	}

	Data::UInt dir = rand()%3;
	Data::Real prop = static_cast<Data::Real>(rand())/static_cast<Data::Real>(RAND_MAX);

	while ((prop <= 0.0) || (prop >= 1.0))
	{
		prop = static_cast<Data::Real>(rand())/static_cast<Data::Real>(RAND_MAX);
	}

	//prop = 0.5;

	Data::Real coord = c->box()->getMin()->getCoord(dir) + prop*(c->box()->getMax()->getCoord(dir) - c->box()->getMin()->getCoord(dir));

	c->setSplit(dir, coord);

	c->subdivide();

	for (Data::UInt i = 0; i < c->numChildren(); i++)
	{
		randomsubdivide(static_cast<Data::BSPTreeCell *>(c->child(i)));
	}
}
//endebug*/

CRAbMesh::NumericalModule::NumericalModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule(frame, canvas)
{
	//this->datum = this->data.end();

	DRAW_X_RANGE = "Draw X range";
    DRAW_Y_RANGE = "Draw Y range";
    DRAW_Z_RANGE = "Draw Z range";
    DRAW_XY_RANGE = "Draw XY range";
    DRAW_XZ_RANGE = "Draw XZ range";
    DRAW_YZ_RANGE = "Draw YZ range";
    DRAW_ER_FUNCTION = "Draw Eq Root function";

	this->InsertOption(DRAW_X_RANGE,  DRAW_MESH,     TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_Y_RANGE,  DRAW_X_RANGE,  TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_Z_RANGE,  DRAW_Y_RANGE,  TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_XY_RANGE, DRAW_Z_RANGE,  TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_XZ_RANGE, DRAW_XY_RANGE, TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_YZ_RANGE, DRAW_XZ_RANGE, TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_ER_FUNCTION, DRAW_YZ_RANGE, TYPE_MENU_AND_CHECK_ITEM);

	this->SetConfiguration(DRAW_X_RANGE,  true, false);
	this->SetConfiguration(DRAW_Y_RANGE,  true, false);
	this->SetConfiguration(DRAW_Z_RANGE,  true, false);
	this->SetConfiguration(DRAW_XY_RANGE, false, false);
	this->SetConfiguration(DRAW_XZ_RANGE, true, false);
	this->SetConfiguration(DRAW_YZ_RANGE, false, false);
	this->SetConfiguration(DRAW_ER_FUNCTION, false, false);

	this->SetOption(VERTICES, true);
	this->SetOption(ELEMENTS, false);
	this->SetOption(FRONT_ELEMENTS, true);
	this->SetOption(TREE_CELLS, false);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, false, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, false, false);

	this->SetOption(DRAW_NORMAL, false, false);
	this->SetOption(DRAW_DISCRETIZATION, false, false);

	this->SetOption(DRAW_BOUNDARY, false, false);
	this->SetOption(DRAW_TREE, false, false);
	this->SetOption(DRAW_FRONT, false, false);
	this->SetOption(DRAW_MESH, true, true);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	this->SetOption(DRAW_X_RANGE,  true, true);
	this->SetOption(DRAW_Y_RANGE,  true, true);
	this->SetOption(DRAW_Z_RANGE,  true, true);
	this->SetOption(DRAW_XY_RANGE, true, true);
	this->SetOption(DRAW_XZ_RANGE, true, true);
	this->SetOption(DRAW_YZ_RANGE, true, true);
	this->SetOption(DRAW_ER_FUNCTION, true, true);

/*
	//this->sharedMesh = false;
	//this->createdSharedMesh = false;
	this->continuingConsistencyCheck = false;

	this->pdGlobal = NULL;
	this->pdLocal = NULL;*/

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}

	//this->waiting = false;
	//this->progress = NULL;

	//this->useRedirector = true;

	this->mesh = NULL;

	/*//debug
	srand(0);
	this->debug = new Data::BSPTree(new Data::Point3D(-10.0, -10.0, -10.0), new Data::Point3D(10.0, 10.0, 10.0), new Data::BSPTreeCell());
	this->debug->setColor(1.0, 1.0, 1.0);
	randomsubdivide(static_cast<Data::BSPTreeCell *>(this->debug->getRoot()));
	this->FillTree(true);
	//this->SetOption(TREE_CELLS, true);
	//this->SetOption(DRAW_TREE, true, true);
	//endebug*/
}

CRAbMesh::NumericalModule::~NumericalModule()
{

}

bool CRAbMesh::NumericalModule::SaveFile(const std::string &/*filename*/)
{
	return false;
}

wxString CRAbMesh::NumericalModule::Name() const
{
	return _("Numerical methods");
}

wxMenu *CRAbMesh::NumericalModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("&Configuration");
		break;
	/*case 1:
		title = _("&Picked");
		break;*/
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::NumericalModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::NumericalModule::GetPanel(unsigned int i, wxString &title) const
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

/*wxMenu *CRAbMesh::NumericalModule::GetPickedMenu(wxString &title) const
{
	return this->GetMenu(1, title);
}*/

bool CRAbMesh::NumericalModule::LoadFile(const std::string &/*filename*/)
{
    return false;

	/*bool opened = false;

	if ((!opened) && (this->drive) && (this->MeshModule::LoadFile(filename)))
	{
		MeshData datum;
		datum.boundary = dynamic_cast<Tools::MainDrive *>(this->drive)->getBoundary();
		datum.front = NULL;
		datum.mesh = dynamic_cast<Tools::MainDrive *>(this->drive)->getMesh();
#if USE_TECGRAF
		datum.model = dynamic_cast<Tools::MainDrive *>(this->drive)->getModel();
#endif //#if USE_TECGRAF
		datum.filename = filename;
		this->data.push_back(datum);

		dynamic_cast<Tools::MainDrive *>(this->drive)->setBoundary(NULL);
		dynamic_cast<Tools::MainDrive *>(this->drive)->setMesh(NULL);
#if USE_TECGRAF
		dynamic_cast<Tools::MainDrive *>(this->drive)->setModel(NULL);
#endif //#if USE_TECGRAF

		this->EnsureExistenceOfMesh();

		opened = true;
	}

	Performer::MainDrive *drive = this->drive;

	for (unsigned int i = 0; ((!opened) && (i < MODULES_COUNT)); i++)
	{
		MeshModule *module = dynamic_cast<MeshModule *>(this->frame->modules[moduleNames[i]]);

		if ((module) && (module != this->frame->currentModule))
		{
			this->drive = module->drive;
			this->loadedFilename = module->loadedFilename;

			module->MakeMainDrive();

			if ((module->Module::LoadFile(filename)) &&
				((module->GetBoundary()) || (module->GetFront()) || (module->GetMesh())))
			{
				MeshData datum;
				datum.boundary = const_cast<Data::Boundary *>(module->GetBoundary());
				datum.front = const_cast<Data::Front *>(module->GetFront());
				datum.mesh = const_cast<Data::Mesh *>(module->GetMesh());
				datum.filename = filename;
				this->data.push_back(datum);

				module->SetBoundary(NULL);
				module->SetFront(NULL);
				module->SetMesh(NULL);

				this->EnsureExistenceOfMesh();

				opened = true;
			}

			delete module->drive;

			module->drive = this->drive;
			module->loadedFilename = this->loadedFilename;

			this->drive = NULL;
			this->loadedFilename = "";
		}
	}

	this->drive = drive;

	return opened;*/
}

void CRAbMesh::NumericalModule::SetParameters(bool /*firstTime*/)
{

}

void CRAbMesh::NumericalModule::Execute()
{
	/*if (this->useRedirector)
    {
        this->redirector.Start(this->tcLog, &std::cout);
    }*/

	this->frame->GetViewModule()->Execute();

	Data::setStepByStep(false);
}

void CRAbMesh::NumericalModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, pending);

	//Tools::MainDrive *drive = dynamic_cast<Tools::MainDrive *>(this->drive);

	if (clearing)
	{
		//drive->clear();
	}

	/*if (this->useRedirector)
    {
        this->redirector.LogText((pending) || (!this->tools.empty()));
    }*/

	if (pending)
	{
		this->canvas->Execute();
	}
	else
	{
		this->frame->Ready();

		/*this->ShowMessage(drive->getTool());

		delete drive->getTool();

		drive->setTool(NULL);

		if (this->pdGlobal)
		{
			this->pdGlobal->Update(this->pdGlobal->GetValue() + 1);
		}

		this->ExecuteTools();

		if (this->tools.empty())
		{
			this->frame->SetStatusText(_(""));
		}*/
	}
}

void CRAbMesh::NumericalModule::Clear(bool onlyDrive, bool destroying)
{
	this->ClearTree();

	if (this->tcLog)
	{
        this->tcLog->Clear();
    }

    if (this->mesh)
    {
        delete this->mesh;

        this->mesh = NULL;
    }

	/*if (this->sharedMesh)
	{
		if ((this->createdSharedMesh) && (!this->data.empty()))
		{
			this->data.back().mesh->clear();

			delete this->data.back().mesh;

			this->createdSharedMesh = false;
		}

		this->data.clear();

		this->datum = this->data.end();

		this->sharedMesh = false;
	}
	else*/
	/*{
		this->datum = this->data.end();

		while (!this->data.empty())
		{
		    for (MeshDataList::iterator iter = ++this->data.begin();
                 iter != this->data.end(); iter++)
            {
                if (this->data.front().boundary == (*iter).boundary)
                {
                    (*iter).boundary = NULL;
                }

                if (this->data.front().front == (*iter).front)
                {
                    (*iter).front = NULL;
                }

                if (this->data.front().mesh == (*iter).mesh)
                {
                    (*iter).mesh = NULL;
                }

#if USE_TECGRAF
                if (this->data.front().model == (*iter).model)
                {
                    (*iter).model = NULL;
                }
#endif //#if USE_TECGRAF
            }

		    if (this->data.front().originalModule)
            {
                if (this->data.front().createdMesh)
                {
                    this->data.front().mesh->clear();

                    delete this->data.front().mesh;

                    this->data.front().mesh = NULL;
                }
            }
            else
            {
                if (this->data.front().mesh)
                {
                    if (this->data.front().boundary)
                    {
                        this->data.front().boundary->setVertices(Data::VertexList());
                        this->data.front().boundary->setElements(Data::FrontElementList());
                    }

                    delete this->data.front().mesh;
                }

                if (this->data.front().boundary)
                {
                    delete this->data.front().boundary;
                }

                if (this->data.front().front)
                {
                    delete this->data.front().front;
                }

#if USE_TECGRAF
                if (this->data.front().model)
                {
                    delete this->data.front().model;
                }
#endif //#if USE_TECGRAF
            }

			this->data.pop_front();
		}

		this->datum = this->data.end();
	}*/

	MeshModule::Clear(onlyDrive, destroying);

	this->frame->Retitle();
}

void CRAbMesh::NumericalModule::SetGUIState()
{

}

void CRAbMesh::NumericalModule::ChangeFrom(const Module *module)
{
	if (!module)
	{
		return;
	}

	CRAbMesh::MeshModule::ChangeFrom(module);

	/*if (!this->data.empty())
	{
		return;
	}

#if (!USE_ONLY_MTOOLS)
	const MeshModule *mmodule = dynamic_cast<const MeshModule *>(module);

	if (!mmodule)
	{
		return;
	}

	if ((!mmodule->GetBoundary()) &&
		(!mmodule->GetFront()) &&
		(!mmodule->GetMesh()))
	{
		return;
	}

	MeshData datum;
	datum.boundary = const_cast<Data::Boundary *>(mmodule->GetBoundary());
	datum.front = const_cast<Data::Front *>(mmodule->GetFront());
	datum.mesh = const_cast<Data::Mesh *>(mmodule->GetMesh());
	datum.filename = std::string(mmodule->GetLoadedFilename().mb_str(wxMBConvLibc()));
	datum.originalModule = mmodule;

	this->data.push_back(datum);

#if USE_TECGRAF_MSH_2D
	const TecGrafMsh2DModule *tmsh2dmodule = dynamic_cast<const TecGrafMsh2DModule *>(mmodule);

	if (tmsh2dmodule)
    {
        if ((tmsh2dmodule->GetWarpBoundary()) || (tmsh2dmodule->GetWarpMesh()))
        {
            MeshData datum;
            datum.boundary = const_cast<Data::Boundary *>(tmsh2dmodule->GetWarpBoundary());
            datum.mesh = const_cast<Data::Mesh *>(tmsh2dmodule->GetWarpMesh());
            datum.filename = std::string(mmodule->GetLoadedFilename().mb_str(wxMBConvLibc()));
            datum.originalModule = mmodule;

            this->data.push_back(datum);

            this->EnsureExistenceOfMesh();
        }

        if (tmsh2dmodule->GetDeformedMesh())
        {
            MeshData datum;
            datum.mesh = const_cast<Data::Mesh *>(tmsh2dmodule->GetDeformedMesh());
            datum.originalModule = mmodule;

            this->data.push_back(datum);

            this->EnsureExistenceOfMesh();
        }
    }
#endif //#if USE_TECGRAF_MSH_2D

	this->ForceShowMesh();
#endif //#if (!USE_ONLY_MTOOLS)*/
}

void CRAbMesh::NumericalModule::ChangeTo(const Module *module)
{
	/*for (MeshDataList::iterator iter = this->data.begin();
         iter != this->data.end();)
    {
        if ((*iter).originalModule == module)
        {
            if (iter == this->datum)
            {
                this->datum = this->data.end();
            }

            if ((*iter).createdMesh)
            {
                (*iter).mesh->clear();

                delete (*iter).mesh;
            }

            iter = this->data.erase(iter);
        }
        else
        {
            iter++;
        }
    }*/

    CRAbMesh::MeshModule::ChangeTo(module);
}

void CRAbMesh::NumericalModule::MakeMainDrive()
{
#if USE_ONLY_MTOOLS
	this->drive = NULL;
#else
	this->drive = NULL;//new Numerical::MainDrive();
#endif //#if USE_ONLY_MTOOLS
}

void CRAbMesh::NumericalModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	//this->menus.push_back(this->MakePickedMenu());
}

void CRAbMesh::NumericalModule::MakeToolbars()
{

}

void CRAbMesh::NumericalModule::MakePanels()
{
	this->tcLog = NULL;

	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
	this->panels.push_back(this->MakeLogPanel());
}

/*wxMenu *CRAbMesh::NumericalModule::MakePickedMenu()
{
	wxMenu *mPicked = new wxMenu();

	this->miPickedShow = mPicked->Append(wxID_ANY, _("&Show"));
	mPicked->AppendSeparator();
	this->miPickedDeleteFirstVertex = mPicked->Append(wxID_ANY, _("Delete &vertex"));
	this->miPickedDeleteFirstFrontElement = mPicked->Append(wxID_ANY, _("Delete &front element"));
	this->miPickedDeleteFirstElement = mPicked->Append(wxID_ANY, _("Delete &element"));
	mPicked->AppendSeparator();
	this->miPickedDeleteVertices = mPicked->Append(wxID_ANY, _("Delete vertices"));
	this->miPickedDeleteFrontElements = mPicked->Append(wxID_ANY, _("Delete front elements"));
	this->miPickedDeleteElements = mPicked->Append(wxID_ANY, _("Delete elements"));
	this->miPickedDeleteSelected = mPicked->Append(wxID_ANY, _("Delete selected"));
	mPicked->AppendSeparator();
	this->miPickedMergeVertices = mPicked->Append(wxID_ANY, _("&Merge vertices"));
	mPicked->AppendSeparator();
	this->miPickedClearPickedList = mPicked->Append(wxID_ANY, _("&Clear picked list"));

    this->Connect(this->miPickedShow->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedShow));
    this->Connect(this->miPickedDeleteFirstVertex->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteFirstVertex));
    this->Connect(this->miPickedDeleteFirstFrontElement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteFirstFrontElement));
    this->Connect(this->miPickedDeleteFirstElement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteFirstElement));
    this->Connect(this->miPickedDeleteVertices->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteVertices));
    this->Connect(this->miPickedDeleteFrontElements->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteFrontElements));
    this->Connect(this->miPickedDeleteElements->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteElements));
    this->Connect(this->miPickedDeleteSelected->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedDeleteSelected));
    this->Connect(this->miPickedMergeVertices->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedMergeVertices));
    this->Connect(this->miPickedClearPickedList->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NumericalModule::OnPickedClearPickedList));

    return mPicked;
}*/

wxPanel *CRAbMesh::NumericalModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel
    //button
    //this->bOpenFiles = new wxButton(pConfiguration, wxID_ANY,_("Open files..."));
    //this->bSaveFiles = new wxButton(pConfiguration, wxID_ANY,_("Save files..."));

	//slider
	//this->sMeshes = new wxSlider(pConfiguration, wxID_ANY, 0, 0, 1);

    //operations
    //wxArrayString choices;
    //choices.Add(_("Current mesh"));
    //choices.Add(_("All meshes"));

    //this->rbMeshRange      = new wxRadioBox(  pConfiguration, wxID_ANY, _("Mesh range"), wxDefaultPosition, wxDefaultSize, choices, wxRA_SPECIFY_ROWS);
    wxStaticText *stCamera = new wxStaticText(pConfiguration, wxID_ANY, _("Camera"));
    this->cbAutoFitCamera  = new wxCheckBox(  pConfiguration, wxID_ANY, _("Auto fit"));
    this->bFitCamera       = new wxButton(    pConfiguration, wxID_ANY, _("Fit"));

    this->bcbTools = new wxChoicebook(pConfiguration, wxID_ANY);
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Equation roots"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Linear systems"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Differentiation"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Integration"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Interpolation"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Approximation"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("ODE"));
    this->bcbTools->AddPage(new wxScrolledWindow(this->bcbTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Eigenvalues"));

    wxWindow *pER = this->bcbTools->GetPage(0);

    wxStaticText *stERFunction  = new wxStaticText(pER, wxID_ANY, _("Function"));
    this->tcERFuncion           = new wxTextCtrl  (pER, wxID_ANY, _("-1"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT);
    this->cbERTestFunctionValue = new wxCheckBox  (pER, wxID_ANY, _("Test function value"));
    this->cbERTestFunctionValue->SetValue(true);
    this->cbERTestStepSize      = new wxCheckBox  (pER, wxID_ANY, _("Test step size"));
    this->cbERTestStepSize->SetValue(true);
    wxStaticText *stERMaxSteps  = new wxStaticText(pER, wxID_ANY, _("Max steps"));
    this->tcERMaxSteps          = new wxTextCtrl  (pER, wxID_ANY, _("100"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stERTolerance = new wxStaticText(pER, wxID_ANY, _("Tolerance"));
    this->tcERTolerance         = new wxTextCtrl  (pER, wxID_ANY, _("0.000001"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    wxBookCtrlBase *bcbER = new wxChoicebook(pER, wxID_ANY);
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Bisection"));
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("False position"));
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Fixed point"));
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Newton-Raphson"));
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Steffensen"));
    bcbER->AddPage(new wxScrolledWindow(bcbER, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Secant"));

    wxStaticText *stERBisIntervalMin = new wxStaticText(bcbER->GetPage(0), wxID_ANY, _("Interval min"));
    this->tcERBisIntervalMin         = new wxTextCtrl  (bcbER->GetPage(0), wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stERBisIntervalMax = new wxStaticText(bcbER->GetPage(0), wxID_ANY, _("Interval max"));
    this->tcERBisIntervalMax         = new wxTextCtrl  (bcbER->GetPage(0), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERBisExecute              = new wxButton(    bcbER->GetPage(0), wxID_ANY, _("Execute"));

    wxStaticText *stERFalsPosIntervalMin = new wxStaticText(bcbER->GetPage(1), wxID_ANY, _("Interval min"));
    this->tcERFalsPosIntervalMin         = new wxTextCtrl  (bcbER->GetPage(1), wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stERFalsPosIntervalMax = new wxStaticText(bcbER->GetPage(1), wxID_ANY, _("Interval max"));
    this->tcERFalsPosIntervalMax         = new wxTextCtrl  (bcbER->GetPage(1), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);;
    wxStaticText *stERFalsPosWeightDecreaseCount = new wxStaticText(bcbER->GetPage(1), wxID_ANY, _("Weight decrease count"));
    this->tcERFalsPosWeightDecreaseCount         = new wxTextCtrl  (bcbER->GetPage(1), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERFalsPosExecute              = new wxButton(    bcbER->GetPage(1), wxID_ANY, _("Execute"));

    wxStaticText *stERFixPtIterFunction = new wxStaticText(bcbER->GetPage(2), wxID_ANY, _("Iteration function"));
    this->tcERFixPtIterFunction         = new wxTextCtrl  (bcbER->GetPage(2), wxID_ANY, _("-1"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT);
    wxStaticText *stERFixPtInitialGuess = new wxStaticText(bcbER->GetPage(2), wxID_ANY, _("Initial guess"));
    this->tcERFixPtInitialGuess         = new wxTextCtrl  (bcbER->GetPage(2), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERFixPtExecute               = new wxButton(    bcbER->GetPage(2), wxID_ANY, _("Execute"));

    wxStaticText *stERNewRapDerivtive = new wxStaticText(bcbER->GetPage(3), wxID_ANY, _("Derivative function"));
    this->tcERNewRapDerivtive         = new wxTextCtrl  (bcbER->GetPage(3), wxID_ANY, _("-1"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT);
    wxStaticText *stERNewRapInitialGuess = new wxStaticText(bcbER->GetPage(3), wxID_ANY, _("Initial guess"));
    this->tcERNewRapInitialGuess         = new wxTextCtrl  (bcbER->GetPage(3), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERNewRapExecute            = new wxButton(    bcbER->GetPage(3), wxID_ANY, _("Execute"));

    wxStaticText *stERSteffInitialGuess = new wxStaticText(bcbER->GetPage(4), wxID_ANY, _("Initial guess"));
    this->tcERSteffInitialGuess         = new wxTextCtrl  (bcbER->GetPage(4), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERSteffExecute            = new wxButton(    bcbER->GetPage(4), wxID_ANY, _("Execute"));;

    wxStaticText *stERSecGuess1 = new wxStaticText(bcbER->GetPage(5), wxID_ANY, _("First guess"));
    this->tcERSecGuess1         = new wxTextCtrl  (bcbER->GetPage(5), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stERSecGuess2 = new wxStaticText(bcbER->GetPage(5), wxID_ANY, _("Second guess"));
    this->tcERSecGuess2         = new wxTextCtrl  (bcbER->GetPage(5), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bERSecExecute         = new wxButton(    bcbER->GetPage(5), wxID_ANY, _("Execute"));

    /*this->cbGStopOnFirst       = new wxCheckBox(this->bcbTools->GetPage(0), wxID_ANY, _("Stop on first intersection"));
    this->bCheckGeometry       = new wxButton(  this->bcbTools->GetPage(0), wxID_ANY, _("Geometry"));
    this->cbTStopOnFirst       = new wxCheckBox(this->bcbTools->GetPage(0), wxID_ANY, _("Stop on first error"));
    this->bCheckTopology       = new wxButton(  this->bcbTools->GetPage(0), wxID_ANY, _("Topology"));
    this->cbAllowMultiple      = new wxCheckBox(this->bcbTools->GetPage(0), wxID_ANY, _("Multiple faces on same edge"));
    this->cbWStopOnFirst       = new wxCheckBox(this->bcbTools->GetPage(0), wxID_ANY, _("Stop on first error"));
    this->bCheckWaterTightness = new wxButton(  this->bcbTools->GetPage(0), wxID_ANY, _("Water tightness"));

    wxArrayString covMatModes;
    covMatModes.Add(_("Normal"));
    covMatModes.Add(_("Accum"));
#if USE_EXACT
    covMatModes.Add(_("Ps-exact"));
#endif //#if USE_EXACT

    this->bRemoveCracks       = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Remove cracks"));
    this->bCopyConvert        = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Copy/Convert"));
    this->bFixNormals         = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Fix normals"));
    this->bInvertNormals      = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Invert normals"));
    wxStaticText *stInnerSurfaceSurfDirection   = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Surf Dir (-1,0,1,2)"));
    this->tcInnerSurfaceSurfDirection           = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bIdentifyInnerSurfaces      = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Identify inner surfaces"));
    wxStaticText *stSilhouetteAngle   = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Angle threshold"));
    this->tcSilhouetteAngle           = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("10.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bIdentifySilhouette         = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Identify silhouette"));
    wxStaticText *stSurfacesAngle     = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Angle threshold"));
    this->tcSurfacesAngle             = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("10.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bIdentifySurfaces           = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Identify surfaces"));
    wxStaticText *stVisibilityVertex  = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Vertex for visibility"));
    this->tcVisibilityVertex          = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bIdentifyVisibility         = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Identify visibility"));
    wxStaticText *stVisibilityEdge    = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Edge for visibility"));
    this->tcVisibilityEdge            = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bIdentifyEdgeVisibility     = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Identify edge visibility"));
    wxStaticText *stVisibilityBasedVertex = new wxStaticText(this->bcbTools->GetPage(1), wxID_ANY, _("Edge for visibility (2)"));
    this->tcVisibilityBasedVertex         = new wxTextCtrl(this->bcbTools->GetPage(1), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bFindVisibilityBasedVertex      = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Find visibility-based vertex"));
#if USE_NUMERICAL
    this->cbCovMatrFrontElements      = new wxCheckBox(this->bcbTools->GetPage(1), wxID_ANY, _("Front elements"));
    this->cbCovMatrWeighted           = new wxCheckBox(this->bcbTools->GetPage(1), wxID_ANY, _("Weighted"));
    this->rbCovMatrMode               = new wxRadioBox(this->bcbTools->GetPage(1), wxID_ANY, _("Mode"), wxDefaultPosition, wxDefaultSize, covMatModes, wxRA_SPECIFY_ROWS);
    this->bFindCovarianceMatrix       = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Find covariance matrix"));
    this->bAnalyzePrincipalComponents = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Principal components"));
    this->bHotellingTransorm          = new wxButton(  this->bcbTools->GetPage(1), wxID_ANY, _("Hotelling Transform"));
#endif //#if USE_NUMERICAL

    wxStaticText *stSmoothFactor = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Smoothing factor"));
    this->tcSmoothFactor       = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("1.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbSmoothNormalTest   = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("Normal test"));
    this->bSmooth              = new wxButton(  this->bcbTools->GetPage(2), wxID_ANY, _("Smooth"));
    wxStaticText *stSwSizeProportion = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Size proportion"));
    this->tcSwSizeProportion   = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stSwQualityThreshold = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Quality threshold"));
    this->tcSwQualityThreshold = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("0.2"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbSwNoCrack          = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("No cracks"));
    this->cbSwNormalTest       = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("Normal test"));
    this->cbSwChangeSurface    = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("Change Surface Boundary"));
    wxStaticText *stSwMaxEdgesOnBoundary = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Max edges on boundary"));
    this->tcSwMaxEdgesOnBoundary = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bRefineSwap          = new wxButton(  this->bcbTools->GetPage(2), wxID_ANY, _("Swap edges"));
    wxStaticText *stCSizeProportion = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Size proportion"));
    this->tcCSizeProportion    = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCQualityThreshold = new wxStaticText(this->bcbTools->GetPage(2), wxID_ANY, _("Quality threshold"));
    this->tcCQualityThreshold  = new wxTextCtrl(this->bcbTools->GetPage(2), wxID_ANY, _("0.2"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbInPlane            = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("Only in plane"));
    this->cbCNoCrack           = new wxCheckBox(this->bcbTools->GetPage(2), wxID_ANY, _("No cracks"));
    this->bRefineCollapse      = new wxButton(  this->bcbTools->GetPage(2), wxID_ANY, _("Collapse edges"));

    this->cbUNoCrack           = new wxCheckBox(this->bcbTools->GetPage(3), wxID_ANY, _("No cracks"));
    this->bRefineUniformly     = new wxButton(  this->bcbTools->GetPage(3), wxID_ANY, _("Refine (uniform)"));
    wxStaticText *stSSizeProportion = new wxStaticText(this->bcbTools->GetPage(3), wxID_ANY, _("Size proportion"));
    this->tcSSizeProportion    = new wxTextCtrl(this->bcbTools->GetPage(3), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
	this->cbSNoCrack           = new wxCheckBox(this->bcbTools->GetPage(3), wxID_ANY, _("No cracks"));
    this->bRefineSize          = new wxButton(  this->bcbTools->GetPage(3), wxID_ANY, _("Refine (size)"));
    wxStaticText *stESizeProportion = new wxStaticText(this->bcbTools->GetPage(3), wxID_ANY, _("Size proportion"));
    this->tcESizeProportion    = new wxTextCtrl(this->bcbTools->GetPage(3), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stEQualityThreshold = new wxStaticText(this->bcbTools->GetPage(3), wxID_ANY, _("Quality threshold"));
    this->tcEQualityThreshold  = new wxTextCtrl(this->bcbTools->GetPage(3), wxID_ANY, _("0.2"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbENoCrack           = new wxCheckBox(this->bcbTools->GetPage(3), wxID_ANY, _("No cracks"));
    this->bRefineEdge          = new wxButton(  this->bcbTools->GetPage(3), wxID_ANY, _("Refine (edge)"));

    this->cbMMKeepOldIds    = new wxCheckBox(this->bcbTools->GetPage(4), wxID_ANY, _("Keep old ids"));
    this->bMergeMeshes      = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Merge"));
    this->bSplitMeshes      = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Split"));
    wxStaticText *stTolerance = new wxStaticText(this->bcbTools->GetPage(4), wxID_ANY, _("Tolerance"));
    this->tcTolerance       = new wxTextCtrl(this->bcbTools->GetPage(4), wxID_ANY, _("0.0001"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bMergeVertices    = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Merge vertices"));
    this->bMakeBoundary     = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Make boundary"));
    this->bColorizeMeshes   = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Colorize"));
    this->cColorScheme      = new wxChoice(  this->bcbTools->GetPage(4), wxID_ANY);
    this->bColorizeElements = new wxButton(  this->bcbTools->GetPage(4), wxID_ANY, _("Colorize elements"));

    this->tcTranslate[0] = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcTranslate[1] = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcTranslate[2] = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bTranslate     = new wxButton(  this->bcbTools->GetPage(5), wxID_ANY, _("Translate"));
    this->tcScale[0]     = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("1"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcScale[1]     = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("1"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcScale[2]     = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("1"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bScale         = new wxButton(  this->bcbTools->GetPage(5), wxID_ANY, _("Scale"));
	this->tcRotate[0]    = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcRotate[1]    = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcRotate[2]    = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("0"),         wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcRotate[3]    = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("90"),        wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bRotate        = new wxButton(  this->bcbTools->GetPage(5), wxID_ANY, _("Rotate"));
    this->tcCentralize   = new wxTextCtrl(this->bcbTools->GetPage(5), wxID_ANY, _("10"),        wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->bCentralize    = new wxButton(  this->bcbTools->GetPage(5), wxID_ANY, _("Centralize"));

#if USE_TECGRAF
    this->bBuildBoundaries = new wxButton(  this->bcbTools->GetPage(6), wxID_ANY, _("Build boundaries"));
#endif //#if USE_TECGRAF

    //wxStaticText *stOther = new wxStaticText(pConfiguration, wxID_ANY, _("Other"));
    //this->bSaveFiles    = new wxButton(    pConfiguration, wxID_ANY, _("Save files..."));*/
    this->bClear          = new wxButton(    pConfiguration, wxID_ANY, _("Clear"));

    //default values
    //this->rbMeshRange->SetSelection(0);
    this->cbAutoFitCamera->SetValue(true);

    /*this->cColorScheme->Append(_("Unique"));
    this->cColorScheme->Append(_("Quality"));
    this->cColorScheme->Append(_("Order"));
    this->cColorScheme->Append(_("Type"));
    this->cColorScheme->SetSelection(0);

    this->cbSmoothNormalTest->SetValue(true);
    this->cbSwNormalTest->SetValue(true);
    this->cbSwChangeSurface->SetValue(false);

#if USE_NUMERICAL
    this->rbCovMatrMode->SetSelection(0);
#endif //#if USE_NUMERICAL*/

    //sizers
    //wxStaticBoxSizer *sbsMesh = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Mesh"));
    //sbsMesh->Add(this->sMeshes, 0, wxEXPAND | wxALL, 0);

    wxGridSizer *gsCamera = new wxGridSizer(2);
    gsCamera->Add(this->cbAutoFitCamera, 0, wxEXPAND | wxALL, 0);
    gsCamera->Add(this->bFitCamera,      0, wxEXPAND | wxALL, 0);

    wxGridBagSizer *gsER = new wxGridBagSizer();
    gsER->SetCols(2);
    gsER->SetFlexibleDirection(wxVERTICAL);
    gsER->Add(stERFunction,                wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsER->Add(this->tcERFuncion,           wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsER->Add(this->cbERTestFunctionValue, wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsER->Add(this->cbERTestStepSize,      wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsER->Add(stERMaxSteps,                wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsER->Add(this->tcERMaxSteps,          wxGBPosition(3, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsER->Add(stERTolerance,               wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsER->Add(this->tcERTolerance,         wxGBPosition(4, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsER->Add(bcbER,                       wxGBPosition(5, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    pER->SetSizer(gsER);

    wxGridBagSizer *gsERBis = new wxGridBagSizer();
    gsERBis->SetCols(2);
    //gsERBis->SetFlexibleDirection(wxVERTICAL);
    gsERBis->Add(stERBisIntervalMin,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERBis->Add(this->tcERBisIntervalMin, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERBis->Add(stERBisIntervalMax,       wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERBis->Add(this->tcERBisIntervalMax, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERBis->Add(this->bERBisExecute,      wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(0)->SetSizer(gsERBis);

    wxGridBagSizer *gsERFalsPos = new wxGridBagSizer();
    gsERFalsPos->SetCols(2);
    //gsERFalsPos->SetFlexibleDirection(wxVERTICAL);
    gsERFalsPos->Add(stERFalsPosIntervalMin,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERFalsPos->Add(this->tcERFalsPosIntervalMin, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERFalsPos->Add(stERFalsPosIntervalMax,       wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERFalsPos->Add(this->tcERFalsPosIntervalMax, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERFalsPos->Add(stERFalsPosWeightDecreaseCount,       wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERFalsPos->Add(this->tcERFalsPosWeightDecreaseCount, wxGBPosition(2, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERFalsPos->Add(this->bERFalsPosExecute,      wxGBPosition(3, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(1)->SetSizer(gsERFalsPos);

    wxGridBagSizer *gsERFixPt = new wxGridBagSizer();
    gsERFixPt->SetCols(2);
    //gsERFixPt->SetFlexibleDirection(wxVERTICAL);
    gsERFixPt->Add(stERFixPtIterFunction,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERFixPt->Add(this->tcERFixPtIterFunction, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERFixPt->Add(stERFixPtInitialGuess,       wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERFixPt->Add(this->tcERFixPtInitialGuess, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERFixPt->Add(this->bERFixPtExecute,       wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(2)->SetSizer(gsERFixPt);

    wxGridBagSizer *gsERNewRap = new wxGridBagSizer();
    gsERNewRap->SetCols(2);
    //gsERNewRap->SetFlexibleDirection(wxVERTICAL);
    gsERNewRap->Add(stERNewRapDerivtive,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERNewRap->Add(this->tcERNewRapDerivtive, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERNewRap->Add(stERNewRapInitialGuess,       wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERNewRap->Add(this->tcERNewRapInitialGuess, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERNewRap->Add(this->bERNewRapExecute,    wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(3)->SetSizer(gsERNewRap);

    wxGridBagSizer *gsERSteff = new wxGridBagSizer();
    gsERSteff->SetCols(2);
    //gsERSteff->SetFlexibleDirection(wxVERTICAL);
    gsERSteff->Add(stERSteffInitialGuess,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERSteff->Add(this->tcERSteffInitialGuess, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERSteff->Add(this->bERSteffExecute,       wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(4)->SetSizer(gsERSteff);

    wxGridBagSizer *gsERSec = new wxGridBagSizer();
    gsERSec->SetCols(2);
    //gsERSec->SetFlexibleDirection(wxVERTICAL);
    gsERSec->Add(stERSecGuess1,       wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERSec->Add(this->tcERSecGuess1, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERSec->Add(stERSecGuess2,       wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsERSec->Add(this->tcERSecGuess2, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsERSec->Add(this->bERSecExecute, wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    bcbER->GetPage(5)->SetSizer(gsERSec);

    /*wxGridBagSizer *gsBoundary = new wxGridBagSizer();
    gsBoundary->SetCols(2);
    gsBoundary->SetFlexibleDirection(wxVERTICAL);
    gsBoundary->Add(this->bRemoveCracks,           wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bCopyConvert,            wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bFixNormals,             wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bInvertNormals,          wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stInnerSurfaceSurfDirection,       wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcInnerSurfaceSurfDirection, wxGBPosition(2, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bIdentifyInnerSurfaces,  wxGBPosition(3, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stSilhouetteAngle,             wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcSilhouetteAngle,       wxGBPosition(4, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bIdentifySilhouette,     wxGBPosition(5, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stSurfacesAngle,               wxGBPosition(6, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcSurfacesAngle,         wxGBPosition(6, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bIdentifySurfaces,       wxGBPosition(7, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stVisibilityVertex,            wxGBPosition(8, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcVisibilityVertex,      wxGBPosition(8, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bIdentifyVisibility,     wxGBPosition(9, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stVisibilityEdge,              wxGBPosition(10, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcVisibilityEdge,        wxGBPosition(10, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bIdentifyEdgeVisibility, wxGBPosition(11, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(stVisibilityBasedVertex,       wxGBPosition(12, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->tcVisibilityBasedVertex, wxGBPosition(12, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bFindVisibilityBasedVertex,  wxGBPosition(13, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#if USE_NUMERICAL
    gsBoundary->Add(this->cbCovMatrFrontElements,  wxGBPosition(14, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBoundary->Add(this->cbCovMatrWeighted,       wxGBPosition(14, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->rbCovMatrMode,           wxGBPosition(15, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bFindCovarianceMatrix,   wxGBPosition(16, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bAnalyzePrincipalComponents, wxGBPosition(16, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    gsBoundary->Add(this->bHotellingTransorm,      wxGBPosition(17, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#endif //#if USE_NUMERICAL
    this->bcbTools->GetPage(1)->SetSizer(gsBoundary);

    wxFlexGridSizer *gsRefinements1 = new wxFlexGridSizer(2);
    gsRefinements1->SetFlexibleDirection(wxVERTICAL);
    gsRefinements1->Add(stSmoothFactor,             0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcSmoothFactor,       0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbSmoothNormalTest,   0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->bSmooth,              0, wxEXPAND | wxALL, 0);
    //gsRefinements1->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(stSwSizeProportion,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcSwSizeProportion,   0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(stSwQualityThreshold,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcSwQualityThreshold, 0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbSwNoCrack,          0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbSwNormalTest,       0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbSwChangeSurface,    0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(stSwMaxEdgesOnBoundary,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcSwMaxEdgesOnBoundary, 0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->bRefineSwap,          0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(stCSizeProportion,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcCSizeProportion,    0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(stCQualityThreshold,        0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements1->Add(this->tcCQualityThreshold,  0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbCNoCrack,           0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->cbInPlane,            0, wxEXPAND | wxALL, 0);
    gsRefinements1->Add(this->bRefineCollapse,      0, wxEXPAND | wxALL, 0);
    this->bcbTools->GetPage(2)->SetSizer(gsRefinements1);

    wxFlexGridSizer *gsRefinements2 = new wxFlexGridSizer(2);
    gsRefinements2->SetFlexibleDirection(wxVERTICAL);
    gsRefinements2->Add(this->cbUNoCrack,           0, wxEXPAND | wxALL, 0);
    //gsRefinements2->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(this->bRefineUniformly,     0, wxEXPAND | wxALL, 0);
    //gsRefinements2->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(stSSizeProportion,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements2->Add(this->tcSSizeProportion,    0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(this->cbSNoCrack,           0, wxEXPAND | wxALL, 0);
    //gsRefinements2->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(this->bRefineSize,          0, wxEXPAND | wxALL, 0);
    //gsRefinements2->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(stESizeProportion,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements2->Add(this->tcESizeProportion,    0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(stEQualityThreshold,        0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsRefinements2->Add(this->tcEQualityThreshold,  0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(this->cbENoCrack,           0, wxEXPAND | wxALL, 0);
    //gsRefinements2->Add(1, 1,                       0, wxEXPAND | wxALL, 0);
    gsRefinements2->Add(this->bRefineEdge,          0, wxEXPAND | wxALL, 0);
    this->bcbTools->GetPage(3)->SetSizer(gsRefinements2);

    wxFlexGridSizer *gsMesh = new wxFlexGridSizer(2);
    gsMesh->SetFlexibleDirection(wxVERTICAL);
    gsMesh->Add(this->cbMMKeepOldIds,    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bMergeMeshes,      0, wxEXPAND | wxALL, 0);
    //gsMesh->Add(1, 1,                    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bSplitMeshes,      0, wxEXPAND | wxALL, 0);
    gsMesh->Add(1, 1,                    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(stTolerance,             0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsMesh->Add(this->tcTolerance,       0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bMergeVertices,    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(1, 1,                    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bMakeBoundary,     0, wxEXPAND | wxALL, 0);
	//gsMesh->Add(1, 1,                    0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bColorizeMeshes,   0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->cColorScheme,      0, wxEXPAND | wxALL, 0);
    gsMesh->Add(this->bColorizeElements, 0, wxEXPAND | wxALL, 0);
    this->bcbTools->GetPage(4)->SetSizer(gsMesh);

    wxGridSizer *gsTCTranslate = new wxGridSizer(4);
	gsTCTranslate->Add(this->tcTranslate[0], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCTranslate->Add(this->tcTranslate[1], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCTranslate->Add(this->tcTranslate[2], 0, wxALIGN_CENTER | wxALL, 0);

	wxGridSizer *gsTCScale = new wxGridSizer(4);
	gsTCScale->Add(this->tcScale[0], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCScale->Add(this->tcScale[1], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCScale->Add(this->tcScale[2], 0, wxALIGN_CENTER | wxALL, 0);

    wxGridSizer *gsTCRotate = new wxGridSizer(4);
	gsTCRotate->Add(this->tcRotate[0], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCRotate->Add(this->tcRotate[1], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCRotate->Add(this->tcRotate[2], 0, wxALIGN_CENTER | wxALL, 0);
	gsTCRotate->Add(this->tcRotate[3], 0, wxALIGN_CENTER | wxALL, 0);

	wxGridSizer *gsTCCentralize = new wxGridSizer(4);
	gsTCCentralize->Add(this->tcCentralize, 0, wxALIGN_CENTER | wxALL, 0);

	wxFlexGridSizer *gsTransformations = new wxFlexGridSizer(1);
    gsTransformations->SetFlexibleDirection(wxVERTICAL);
    gsTransformations->Add(gsTCTranslate,     0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(this->bTranslate,  0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(gsTCScale,         0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(this->bScale,      0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(gsTCRotate,        0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(this->bRotate,     0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(gsTCCentralize,    0, wxALIGN_LEFT | wxALL, 0);
    gsTransformations->Add(this->bCentralize, 0, wxALIGN_LEFT | wxALL, 0);
    this->bcbTools->GetPage(5)->SetSizer(gsTransformations);

#if USE_TECGRAF
    wxFlexGridSizer *gsTecGraf = new wxFlexGridSizer(2);
    gsTecGraf->SetFlexibleDirection(wxVERTICAL);
    gsTecGraf->Add(this->bBuildBoundaries, 0, wxEXPAND | wxALL, 0);
    this->bcbTools->GetPage(6)->SetSizer(gsTecGraf);
#endif //#if USE_TECGRAF*/

    wxStaticText *stDrawCoordsX = new wxStaticText(pConfiguration, wxID_ANY, _("Range X"));
    this->tcDrawCoords[0][0]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcDrawCoords[0][1]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stDrawCoordsY = new wxStaticText(pConfiguration, wxID_ANY, _("Range Y"));
    this->tcDrawCoords[1][0]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcDrawCoords[1][1]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stDrawCoordsZ = new wxStaticText(pConfiguration, wxID_ANY, _("Range Z"));
    this->tcDrawCoords[2][0]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("-1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->tcDrawCoords[2][1]    = new wxTextCtrl  (pConfiguration, wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stDrawRefinement = new wxStaticText(pConfiguration, wxID_ANY, _("Refinement"));
    this->tcDrawRefinement         = new wxTextCtrl  (pConfiguration, wxID_ANY, _("100"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    wxGridBagSizer *gsDrawingOptions = new wxGridBagSizer();
    gsDrawingOptions->SetCols(3);
    gsDrawingOptions->Add(stDrawCoordsX,            wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[0][0], wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[0][1], wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(stDrawCoordsY,            wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[1][0], wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[1][1], wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(stDrawCoordsZ,            wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[2][0], wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawCoords[2][1], wxGBPosition(2, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(stDrawRefinement,         wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsDrawingOptions->Add(this->tcDrawRefinement,   wxGBPosition(3, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);

    wxStaticBoxSizer *sbsDrawingOptions = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Drawing options"));
    sbsDrawingOptions->Add(gsDrawingOptions, 0, wxEXPAND | wxALL, 0);

	wxStaticBoxSizer *sbsOperations = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Operations"));
	//sbsOperations->Add(this->rbMeshRange, 0, wxEXPAND | wxALL, 0);
	sbsOperations->Add(stCamera,          0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 2);
	sbsOperations->Add(gsCamera,          0, wxEXPAND | wxALL, 0);
    sbsOperations->Add(this->bcbTools,    0, wxEXPAND | wxALL, 0);
	//sbsOperations->Add(this->bSaveFiles,  0, wxEXPAND | wxALL, 0);
    sbsOperations->Add(this->bClear,      0, wxEXPAND | wxALL, 0);

    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
	//bsConf->Add(sbsMesh,        0, wxEXPAND | wxALL, 0);
	bsConf->Add(sbsOperations,  0, wxEXPAND | wxALL, 0);
	bsConf->Add(sbsDrawingOptions,  0, wxEXPAND | wxALL, 0);
	bsConf->Add(sConf,          0, wxEXPAND | wxALL, 0);
    pConfiguration->SetSizer(bsConf, false);

    //this->Connect(this->bOpenFiles->GetId(),        wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnOpenFiles));
    //this->Connect(this->bSaveFiles->GetId(),        wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnSaveFiles));

    /*this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_TOP,             wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_BOTTOM,          wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_LINEUP,          wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_LINEDOWN,        wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_PAGEUP,          wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_PAGEDOWN,        wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_THUMBTRACK,      wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_THUMBRELEASE,    wxScrollEventHandler(NumericalModule::OnMeshes));
	this->Connect(this->sMeshes->GetId(),           wxEVT_SCROLL_CHANGED,         wxScrollEventHandler(NumericalModule::OnMeshes));*/

	this->Connect(this->bFitCamera->GetId(),    wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnFitCamera));

	this->Connect(this->bERBisExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootBisection));
	this->Connect(this->bERFalsPosExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootFalsePosition));
	this->Connect(this->bERFixPtExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootFixedPoint));
	this->Connect(this->bERNewRapExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootNewtonRaphson));
	this->Connect(this->bERSteffExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootSteffensen));
	this->Connect(this->bERSecExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnExecuteEquationRootSecant));

	/*this->Connect(this->bCheckGeometry->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnCheckGeometry));
	this->Connect(this->bCheckTopology->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnCheckTopology));
	this->Connect(this->bCheckWaterTightness->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnCheckWaterTightness));

	this->Connect(this->bRemoveCracks->GetId(),              wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRemoveCracks));
	this->Connect(this->bCopyConvert->GetId(),               wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnCopyConvert));
	this->Connect(this->bFixNormals->GetId(),                wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnFixNormals));
	this->Connect(this->bInvertNormals->GetId(),             wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnInvertNormals));
	this->Connect(this->bIdentifyInnerSurfaces->GetId(),     wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnIdentifyInnerSurfaces));
	this->Connect(this->bIdentifySilhouette->GetId(),        wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnIdentifySilhouette));
	this->Connect(this->bIdentifySurfaces->GetId(),          wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnIdentifySurfaces));
	this->Connect(this->bIdentifyVisibility->GetId(),        wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnIdentifyVisibility));
	this->Connect(this->bIdentifyEdgeVisibility->GetId(),    wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnIdentifyEdgeVisibility));
	this->Connect(this->bFindVisibilityBasedVertex->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnFindVisibilityBasedVertex));
#if USE_NUMERICAL
	this->Connect(this->bFindCovarianceMatrix->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnFindCovarianceMatrix));
	this->Connect(this->bAnalyzePrincipalComponents->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnAnalyzePrincipalComponents));
	this->Connect(this->bHotellingTransorm->GetId(),    wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnHotellingTransform));
#endif //#if USE_NUMERICAL

	this->Connect(this->bSmooth->GetId(),           wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnSmooth));
	this->Connect(this->bRefineSwap->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRefineSwap));
	this->Connect(this->bRefineCollapse->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRefineCollapse));

	this->Connect(this->bRefineUniformly->GetId(),  wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRefineUniformly));
	this->Connect(this->bRefineSize->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRefineSize));
	this->Connect(this->bRefineEdge->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRefineEdge));

	this->Connect(this->bMergeMeshes->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnMergeMeshes));
	this->Connect(this->bSplitMeshes->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnSplitMeshes));
	this->Connect(this->bMergeVertices->GetId(),    wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnMergeVertices));
	this->Connect(this->bMakeBoundary->GetId(),     wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnMakeBoundary));
	this->Connect(this->bColorizeMeshes->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnColorizeMeshes));
	this->Connect(this->bColorizeElements->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnColorizeElements));

	this->Connect(this->bTranslate->GetId(),        wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnTranslate));
	this->Connect(this->bScale->GetId(),            wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnScale));
	this->Connect(this->bRotate->GetId(),           wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnRotate));
	this->Connect(this->bCentralize->GetId(),       wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnCentralize));

#if USE_TECGRAF
	this->Connect(this->bBuildBoundaries->GetId(),  wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnBuildBoundaries));
#endif //#if USE_TECGRAF*/

	this->Connect(this->bClear->GetId(),            wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NumericalModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::NumericalModule::MakeLogPanel()
{
	wxPanel *pLog = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 210), wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
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

/*void CRAbMesh::NumericalModule::UpdatePDLocal()
{
#if (!USE_ONLY_MTOOLS)
	Tools::MainDrive *drive = dynamic_cast<Tools::MainDrive *>(this->drive);

	if ((this->pdLocal) && (drive) && (drive->getTool()))
	{
		double curr = static_cast<double>(drive->getTool()->measure(0));
		double total = static_cast<double>(drive->getTool()->measure(1));

		int prop = static_cast<int>(static_cast<double>(this->pdLocal->GetRange())*(curr/total));

		///Another test to try to avoid the bug stated in OnTimer()
		if (this->pdLocal)
        {
            this->pdLocal->Update(prop);
        }
	}
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnTimer()
{
#if USE_C__11
    this->CallAfter(&CRAbMesh::NumericalModule::UpdatePDLocal);
#else
    ///Can crash due to the deletion of this->pdLocal on ExecuteTools()
    ///  I tried to do a critical section using wxMutex, wxCriticalSection and
    ///    wxSemaphore, and none of them worked
    this->UpdatePDLocal();
#endif //#if USE_C__11
}

void CRAbMesh::NumericalModule::OnOpenFiles(wxCommandEvent &WXUNUSED(event))
{
	this->OpenFiles();
}

void CRAbMesh::NumericalModule::OpenFiles()
{
	wxFileDialog *open = new wxFileDialog(this->frame, _("Open files"), this->frame->openPath, _(""),
		this->frame->openWildcard,
#if wxCHECK_VERSION(2, 9, 0)
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
#else
		wxOPEN | wxFILE_MUST_EXIST | wxFD_MULTIPLE);
#endif //#if wxCHECK_VERSION(2, 9, 0)

	if (open->ShowModal() == wxID_OK)
    {
    	wxArrayString paths;

    	open->GetPaths(paths);

    	bool openedAny = false;

    	this->Clear();

    	wxProgressDialog pd(_("Opening files"),
			_("Opening files"),
			static_cast<int>(paths.GetCount()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT);

    	for (unsigned int i = 0; i < paths.GetCount(); i++)
    	{
    		if (!pd.Update(i))
			{
				break;
			}

			pd.Refresh(false);

    		std::string inputFile(paths[i].mb_str(wxMBConvLibc()));

			if (this->LoadFile(inputFile))
			{
				openedAny = true;
			}
    	}

    	if (openedAny)
		{
			this->ForceShowMesh();

			this->frame->openPath = open->GetPath().BeforeLast(wxFileName::GetPathSeparator());
		}
    }

	delete open;
}

void CRAbMesh::NumericalModule::OnSaveFiles(wxCommandEvent &WXUNUSED(event))
{
	this->SaveFiles();
}

void CRAbMesh::NumericalModule::SaveFiles()
{
	wxFileDialog *save = new wxFileDialog(this->frame, _("Save files"), this->frame->savePath, _(""),
		this->frame->saveWildcard,
#if wxCHECK_VERSION(2, 9, 0)
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
		wxSAVE | wxOVERWRITE_PROMPT);
#endif //#if wxCHECK_VERSION(2, 9, 0)

	if (save->ShowModal() == wxID_OK)
    {
        std::string ext = this->frame->DetermineGenSaveExtension(save);

    	bool all = (this->rbMeshRange->GetSelection() == 1);

    	this->frame->savePath = save->GetPath().BeforeLast(wxFileName::GetPathSeparator());

		if (!all)
		{
			std::string outputFile(save->GetPath().mb_str(wxMBConvLibc()));

			if (!ext.empty())
            {
                std::stringstream str;

                str << outputFile << ext;

                outputFile = str.str();
            }

			this->SaveFile((*this->datum).boundary, (*this->datum).mesh, outputFile);
		}
		else
		{
			wxProgressDialog pd(_("Saving files"), _("Saving files"), static_cast<int>(this->data.size()), this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT);

			unsigned int i = 0;

			for (MeshDataList::iterator iter = this->data.begin();
				 iter != this->data.end(); iter++)
			{
				wxString filename = save->GetPath();
				wxString extension = filename.AfterLast('.');
				filename = filename.BeforeLast('.');
				filename << i + 1;
				filename << _(".") << extension;

				std::string outputFile(filename.mb_str(wxMBConvLibc()));

				if (!ext.empty())
                {
                    std::stringstream str;

                    str << outputFile << ext;

                    outputFile = str.str();
                }

				this->SaveFile((*iter).boundary, (*iter).mesh, outputFile);

				if (!pd.Update(i++))
				{
					break;
				}

				pd.Refresh(false);
			}
		}
    }
}*/

#if USE_TECGRAF
/*void CRAbMesh::NumericalModule::SetRegionsDrawable(bool regionsDrawable)
{
	this->SetConfiguration(DRAW_REGIONS, regionsDrawable);
}

bool CRAbMesh::NumericalModule::IsRegionsDrawable() const
{
	return this->IsConfiguration(DRAW_REGIONS);
}

void CRAbMesh::NumericalModule::SetSurfacesDrawable(bool surfacesDrawable)
{
	this->SetConfiguration(DRAW_SURFACES, surfacesDrawable);
}

bool CRAbMesh::NumericalModule::IsSurfacesDrawable() const
{
	return this->IsConfiguration(DRAW_SURFACES);
}*/

/*void CRAbMesh::NumericalModule::SetModelDrawable(bool modelDrawable)
{
	this->SetConfiguration(DRAW_MODEL, modelDrawable);
}

bool CRAbMesh::NumericalModule::IsModelDrawable() const
{
	return this->IsConfiguration(DRAW_MODEL);
}

bool CRAbMesh::NumericalModule::IsDrawable() const
{
	return (MeshModule::IsDrawable() ||
			this->IsModelDrawable());
			//this->IsRegionsDrawable() ||
			//this->IsSurfacesDrawable());
}*/
#endif //#if USE_TECGRAF

/*void CRAbMesh::NumericalModule::OnMeshes(wxScrollEvent &WXUNUSED(event))
{
	int val = this->sMeshes->GetValue();

	if ((val >= 0) && (static_cast<unsigned int>(val) < this->data.size()))
	{
		this->datum = this->data.begin();

		std::advance(this->datum, val);

		if (this->cbAutoFitCamera->GetValue())
		{
			this->ForceFitCamera();
		}

		this->picked.clear();

		this->FillTree();

		this->frame->Retitle();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}*/

void CRAbMesh::NumericalModule::OnFitCamera(wxCommandEvent &WXUNUSED(event))
{
	this->ForceFitCamera();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootBisection(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));

    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    Real a = 0.0;
    this->tcERBisIntervalMin->GetValue().ToDouble(&a);
    Real b = 0.0;
    this->tcERBisIntervalMax->GetValue().ToDouble(&b);

    Numerical::BisectionEquationRootFinder bisection;

    bisection.setFunction(&function);
    bisection.setTestingFunction(testFunction);
    bisection.setTestingStepSize(testStepSize);
    bisection.setMaxSteps(static_cast<UInt>(maxSteps));
    bisection.setTolerance(tolerance);
    bisection.setMin(a);
    bisection.setMax(b);
    bisection.setStoringPoints(true);

    bisection.execute();

    Data::Point3DObjVector points = bisection.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << bisection.getNumSteps() << _(" steps, the bisection method reached the root value ") << bisection.getRoot() << _(".");

    if (bisection.isOk())
    {
        wxString msg = _("Bisection method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("Bisection method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootFalsePosition(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));

    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    Real a = 0.0;
    this->tcERFalsPosIntervalMin->GetValue().ToDouble(&a);
    Real b = 0.0;
    this->tcERFalsPosIntervalMax->GetValue().ToDouble(&b);

    unsigned long count = 0;
    this->tcERFalsPosWeightDecreaseCount->GetValue().ToULong(&count);

    Numerical::FalsePositionEquationRootFinder falsePosition;

    falsePosition.setFunction(&function);
    falsePosition.setTestingFunction(testFunction);
    falsePosition.setTestingStepSize(testStepSize);
    falsePosition.setMaxSteps(static_cast<UInt>(maxSteps));
    falsePosition.setTolerance(tolerance);
    falsePosition.setMin(a);
    falsePosition.setMax(b);
    falsePosition.setWeightDecreaseCount(static_cast<UInt>(count));
    falsePosition.setStoringPoints(true);

    falsePosition.execute();

    Data::Point3DObjVector points = falsePosition.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << falsePosition.getNumSteps() << _(" steps, the false position method reached the root value ") << falsePosition.getRoot() << _(".");

    if (falsePosition.isOk())
    {
        wxString msg = _("False position method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("False position method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootFixedPoint(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    std::string str2 = std::string(this->tcERFixPtIterFunction->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction iterFunction;
    iterFunction.setFunction(str2);

    Real x0 = 0.0;
    this->tcERFixPtInitialGuess->GetValue().ToDouble(&x0);

    Numerical::FixedPointEquationRootFinder fixedPoint;

    fixedPoint.setFunction(&function);
    fixedPoint.setTestingFunction(testFunction);
    fixedPoint.setTestingStepSize(testStepSize);
    fixedPoint.setMaxSteps(static_cast<UInt>(maxSteps));
    fixedPoint.setTolerance(tolerance);
    fixedPoint.setStep(&iterFunction);
    fixedPoint.setInitial(x0);
    fixedPoint.setStoringPoints(true);

    fixedPoint.execute();

    Data::Point3DObjVector points = fixedPoint.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << fixedPoint.getNumSteps() << _(" steps, the fixed point method reached the root value ") << fixedPoint.getRoot() << _(".");

    if (fixedPoint.isOk())
    {
        wxString msg = _("Fixed point method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("Fixed point method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootNewtonRaphson(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    std::string str2 = std::string(this->tcERNewRapDerivtive->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction derFunction;
    derFunction.setFunction(str2);

    Real x0 = 0.0;
    this->tcERNewRapInitialGuess->GetValue().ToDouble(&x0);

    Numerical::NewtonRaphsonEquationRootFinder newtonRaphson;

    newtonRaphson.setFunction(&function);
    newtonRaphson.setTestingFunction(testFunction);
    newtonRaphson.setTestingStepSize(testStepSize);
    newtonRaphson.setMaxSteps(static_cast<UInt>(maxSteps));
    newtonRaphson.setTolerance(tolerance);
    newtonRaphson.setDerivative(&derFunction);
    newtonRaphson.setInitial(x0);
    newtonRaphson.setStoringPoints(true);

    newtonRaphson.execute();

    Data::Point3DObjVector points = newtonRaphson.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << newtonRaphson.getNumSteps() << _(" steps, the Newton-Raphson method reached the root value ") << newtonRaphson.getRoot() << _(".");

    if (newtonRaphson.isOk())
    {
        wxString msg = _("Newton-Raphson method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("Newton-Raphson method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootSteffensen(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    Real x0 = 0.0;
    this->tcERSteffInitialGuess->GetValue().ToDouble(&x0);

    Numerical::SteffensenEquationRootFinder steffensen;

    steffensen.setFunction(&function);
    steffensen.setTestingFunction(testFunction);
    steffensen.setTestingStepSize(testStepSize);
    steffensen.setMaxSteps(static_cast<UInt>(maxSteps));
    steffensen.setTolerance(tolerance);
    steffensen.setInitial(x0);
    steffensen.setStoringPoints(true);

    steffensen.execute();

    Data::Point3DObjVector points = steffensen.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << steffensen.getNumSteps() << _(" steps, the Steffensen method reached the root value ") << steffensen.getRoot() << _(".");

    if (steffensen.isOk())
    {
        wxString msg = _("Steffensen method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("Steffensen method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnExecuteEquationRootSecant(wxCommandEvent &WXUNUSED(event))
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));
    Numerical::InterpretedFunction function;
    function.setFunction(str);

    bool testFunction = this->cbERTestFunctionValue->GetValue();
    bool testStepSize = this->cbERTestStepSize->GetValue();

    unsigned long maxSteps = 0;
    this->tcERMaxSteps->GetValue().ToULong(&maxSteps);

    Real tolerance = 0.0;
    this->tcERTolerance->GetValue().ToDouble(&tolerance);

    Real x0 = 0.0;
    this->tcERSecGuess1->GetValue().ToDouble(&x0);
    Real x1 = 0.0;
    this->tcERSecGuess2->GetValue().ToDouble(&x1);

    Numerical::SecantEquationRootFinder secant;

    secant.setFunction(&function);
    secant.setTestingFunction(testFunction);
    secant.setTestingStepSize(testStepSize);
    secant.setMaxSteps(static_cast<UInt>(maxSteps));
    secant.setTolerance(tolerance);
    secant.setInitial(x0);
    secant.setInitial2(x1);
    secant.setStoringPoints(true);

    secant.execute();

    Data::Point3DObjVector points = secant.getPoints();

    this->MakeMesh(points, true, false);

    wxString text;

    text << _("After ") << secant.getNumSteps() << _(" steps, the secant method reached the root value ") << secant.getRoot() << _(".");

    if (secant.isOk())
    {
        wxString msg = _("Secant method converged.\n");
        msg << text;

        wxMessageBox(msg, _("Converged"), wxOK);
    }
    else
    {
        wxString msg = _("Secant method did not converge.\n");
        msg << text;

        wxMessageBox(msg, _("Error"), wxOK);
    }

    this->canvas->Refresh(false);
    this->canvas->Update();
}

/*void CRAbMesh::NumericalModule::OnCheckGeometry(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool stop = this->cbGStopOnFirst->GetValue();

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshInterceptionChecker *checker = new Tools::MeshInterceptionChecker();

			checker->setMesh((*iter).mesh);
			checker->setBoundary((*iter).boundary);
			checker->setStopOnFirst(stop);

			this->tools.push_back(checker);
		}

		this->pdGlobal = new wxProgressDialog(_("Checking interception in mesh"),
			_("Checking interception in mesh"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshInterceptionChecker *checker = new Tools::MeshInterceptionChecker();

		checker->setMesh(this->GetMesh());
		checker->setBoundary(this->GetBoundary());
		checker->setStopOnFirst(stop);

		this->tools.push_back(checker);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while checking interceptions.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnCheckTopology(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool stop = this->cbTStopOnFirst->GetValue();

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshTopologyChecker *checker = new Tools::MeshTopologyChecker();

			checker->setMesh((*iter).mesh);
			checker->setBoundary((*iter).boundary);
			checker->setFront((*iter).front);
#if USE_TECGRAF
			checker->setModel((*iter).model);
#endif //#if USE_TECGRAF
			checker->setStopOnFirst(stop);

			this->tools.push_back(checker);
		}

		this->pdGlobal = new wxProgressDialog(_("Checking topological consistency"),
			_("Checking topological consistency"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshTopologyChecker *checker = new Tools::MeshTopologyChecker();

		checker->setMesh(this->GetMesh());
		checker->setBoundary(this->GetBoundary());
		checker->setFront(this->GetFront());
#if USE_TECGRAF
		checker->setModel(this->GetModel());
#endif //#if USE_TECGRAF
		checker->setStopOnFirst(stop);

		this->tools.push_back(checker);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while checking consistency.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnCheckWaterTightness(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool stop = this->cbWStopOnFirst->GetValue();
	bool multiple = this->cbAllowMultiple->GetValue();

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryWaterTightnessChecker *checker = new Tools::BoundaryWaterTightnessChecker();

			checker->setBoundary((*iter).boundary);
			checker->setAllowMultiple(multiple);
			checker->setStopOnFirst(stop);

			this->tools.push_back(checker);
		}

		this->pdGlobal = new wxProgressDialog(_("Checking water tightness of boundaries"),
			_("Checking water tightness of boundaries"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryWaterTightnessChecker *checker = new Tools::BoundaryWaterTightnessChecker();

		checker->setBoundary(this->GetBoundary());
		checker->setAllowMultiple(multiple);
		checker->setStopOnFirst(stop);

		this->tools.push_back(checker);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while checking water tightness of boundary.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRemoveCracks(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::CrackRemover *remover = new Tools::CrackRemover();

			remover->setBoundary((*iter).boundary);
			remover->setMesh((*iter).mesh);

			this->tools.push_back(remover);
		}

		this->pdGlobal = new wxProgressDialog(_("Removing cracks"),
			_("Removing cracks"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::CrackRemover *remover = new Tools::CrackRemover();

		remover->setBoundary((*this->datum).boundary);
		remover->setMesh((*this->datum).mesh);

		this->tools.push_back(remover);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while removing cracks.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnCopyConvert(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryConversor *conversor = new Tools::BoundaryConversor();

			conversor->setOriginal((*iter).boundary);

			this->tools.push_back(conversor);
		}

		this->pdGlobal = new wxProgressDialog(_("Copying/converting boundaries"),
			_("Copying/converting boundaries"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryConversor *conversor = new Tools::BoundaryConversor();

		conversor->setOriginal((*this->datum).boundary);

		this->tools.push_back(conversor);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while copying/converting boundary.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnFixNormals(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryNormalFixer *fixer = new Tools::BoundaryNormalFixer();

			fixer->setBoundary((*iter).boundary);

			this->tools.push_back(fixer);
		}

		this->pdGlobal = new wxProgressDialog(_("Fixing boundary normals"),
			_("Fixing boundary normals"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryNormalFixer *fixer = new Tools::BoundaryNormalFixer();

		fixer->setBoundary((*this->datum).boundary);

		this->tools.push_back(fixer);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while fixing normals.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnInvertNormals(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryNormalInverter *inverter = new Tools::BoundaryNormalInverter();

			inverter->setBoundary((*iter).boundary);

			this->tools.push_back(inverter);
		}

		this->pdGlobal = new wxProgressDialog(_("Inverting boundary normals"),
			_("Inverting boundary normals"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryNormalInverter *inverter = new Tools::BoundaryNormalInverter();

		inverter->setBoundary((*this->datum).boundary);

		this->tools.push_back(inverter);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while inverting normals.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnIdentifyInnerSurfaces(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	long int direction = -1;

	this->tcInnerSurfaceSurfDirection->GetValue().ToLong(&direction);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::InnerSurfacesIdentifier *identifier = new Tools::InnerSurfacesIdentifier();

			identifier->setBoundary((*iter).boundary);
			identifier->setMesh((*iter).mesh);

			if (direction >= 0)
            {
                identifier->setSurfaceDirection(direction%3);
            }

			this->tools.push_back(identifier);
		}

		this->pdGlobal = new wxProgressDialog(_("Identifying inner surfaces"),
			_("Identifying inner surfaces"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::InnerSurfacesIdentifier *identifier = new Tools::InnerSurfacesIdentifier();

		identifier->setBoundary((*this->datum).boundary);
		identifier->setMesh((*this->datum).mesh);

		if (direction >= 0)
        {
            identifier->setSurfaceDirection(direction%3);
        }

		this->tools.push_back(identifier);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying inner surfaces.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnIdentifySilhouette(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real angle = 0.0;

	this->tcSilhouetteAngle->GetValue().ToDouble(&angle);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::SilhouetteIdentifier *identifier = new Tools::SilhouetteIdentifier();

			identifier->setBoundary((*iter).boundary);
			identifier->setAngle(angle);

			this->tools.push_back(identifier);
		}

		this->pdGlobal = new wxProgressDialog(_("Identifying silhouettes"),
			_("Identifying silhouettes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::SilhouetteIdentifier *identifier = new Tools::SilhouetteIdentifier();

		identifier->setBoundary((*this->datum).boundary);
		identifier->setAngle(angle);

		this->tools.push_back(identifier);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying silhouette.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnIdentifySurfaces(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real angle = 0.0;

	this->tcSurfacesAngle->GetValue().ToDouble(&angle);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::SurfacesIdentifier *identifier = new Tools::SurfacesIdentifier();

			identifier->setBoundary((*iter).boundary);
			identifier->setAngle(angle);

			this->tools.push_back(identifier);
		}

		this->pdGlobal = new wxProgressDialog(_("Identifying surfaces"),
			_("Identifying surfaces"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::SurfacesIdentifier *identifier = new Tools::SurfacesIdentifier();

		identifier->setBoundary((*this->datum).boundary);
		identifier->setAngle(angle);

		this->tools.push_back(identifier);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying surface.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnIdentifyVisibility(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	unsigned long int vertex = 0;

	this->tcVisibilityVertex->GetValue().ToULong(&vertex);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Data::Vertex *v = NULL;

			for (Data::Boundary::VertexIterator iter2 = (*iter).boundary->vBegin();
                 iter2 != (*iter).boundary->vEnd(); iter2++)
            {
                if ((*iter2)->getId() == vertex)
                {
                    v = (*iter2);

                    break;
                }
            }

            if (!v)
            {
                continue;
            }

			Tools::VisibilityIdentifier *identifier = new Tools::VisibilityIdentifier();

			identifier->setMesh(static_cast<MJMesh::Mesh *>((*iter).mesh));

			identifier->setBoundary(static_cast<MJMesh::Boundary *>((*iter).boundary));
			identifier->setVertex(static_cast<MJMesh::Vertex *>(v), static_cast<MJMesh::Mesh *>((*iter).mesh));

			this->tools.push_back(identifier);
		}

		this->pdGlobal = new wxProgressDialog(_("Identifying visibilities"),
			_("Identifying visibilities"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
	    Data::Vertex *v = NULL;

        for (Data::Boundary::VertexIterator iter2 = (*this->datum).boundary->vBegin();
             iter2 != (*this->datum).boundary->vEnd(); iter2++)
        {
            if ((*iter2)->getId() == vertex)
            {
                v = (*iter2);

                break;
            }
        }

        if (v)
        {
            Tools::VisibilityIdentifier *identifier = new Tools::VisibilityIdentifier();

            identifier->setMesh(static_cast<MJMesh::Mesh *>((*this->datum).mesh));

            identifier->setBoundary(static_cast<MJMesh::Boundary *>((*this->datum).boundary));
            identifier->setVertex(static_cast<MJMesh::Vertex *>(v), static_cast<MJMesh::Mesh *>((*this->datum).mesh));

            this->tools.push_back(identifier);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying visibility.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnIdentifyEdgeVisibility(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	unsigned long int edge = 0;

	this->tcVisibilityEdge->GetValue().ToULong(&edge);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Data::FrontElement *fel = NULL;

			for (Data::Boundary::FrontElementIterator iter2 = (*iter).boundary->eBegin();
                 iter2 != (*iter).boundary->eEnd(); iter2++)
            {
                if ((*iter2)->getId() == edge)
                {
                    fel = (*iter2);

                    break;
                }
            }

            if (!fel)
            {
                continue;
            }

			Tools::FrontElementVisibilityIdentifier *identifier = new Tools::FrontElementVisibilityIdentifier();

			identifier->setMesh(static_cast<MJMesh::Mesh *>((*iter).mesh));

			identifier->setBoundary(static_cast<MJMesh::Boundary *>((*iter).boundary));
			identifier->setFrontElement(static_cast<MJMesh::FrontElement *>(fel));

			this->tools.push_back(identifier);
		}

		this->pdGlobal = new wxProgressDialog(_("Identifying visibilities"),
			_("Identifying visibilities"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
	    Data::FrontElement *fel = NULL;

        for (Data::Boundary::FrontElementIterator iter2 = (*this->datum).boundary->eBegin();
             iter2 != (*this->datum).boundary->eEnd(); iter2++)
        {
            if ((*iter2)->getId() == edge)
            {
                fel = (*iter2);

                break;
            }
        }

        if (fel)
        {
            Tools::FrontElementVisibilityIdentifier *identifier = new Tools::FrontElementVisibilityIdentifier();

            identifier->setMesh(static_cast<MJMesh::Mesh *>((*this->datum).mesh));

            identifier->setBoundary(static_cast<MJMesh::Boundary *>((*this->datum).boundary));
            identifier->setFrontElement(static_cast<MJMesh::FrontElement *>(fel));

            this->tools.push_back(identifier);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying visibility.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnFindVisibilityBasedVertex(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	unsigned long int edge = 0;

	this->tcVisibilityBasedVertex->GetValue().ToULong(&edge);

	Tools::VisibilityBasedVertexFinder *identifier = new Tools::VisibilityBasedVertexFinder();

	identifier->setSearchVertex(false);

    for (MeshDataList::iterator iter = this->data.begin();
         iter != this->data.end(); iter++)
    {
        Data::FrontElement *fel = NULL;

        for (Data::Boundary::FrontElementIterator iter2 = (*iter).boundary->eBegin();
             iter2 != (*iter).boundary->eEnd(); iter2++)
        {
            if ((*iter2)->getId() == edge)
            {
                fel = (*iter2);

                break;
            }
        }

        if (!fel)
        {
            continue;
        }

        identifier->addInput(static_cast<MJMesh::Boundary *>((*iter).boundary), static_cast<MJMesh::FrontElement2D *>(fel), static_cast<MJMesh::Mesh *>((*iter).mesh));
    }

    this->tools.push_back(identifier);

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while identifying visibility.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

#if USE_NUMERICAL
void CRAbMesh::NumericalModule::OnFindCovarianceMatrix(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool frontElements = this->cbCovMatrFrontElements->GetValue();
	bool weighted = this->cbCovMatrWeighted->GetValue();
	enum Tools::CovarianceMatrixFinder::Mode mode = Tools::CovarianceMatrixFinder::NORMAL;

	switch (rbCovMatrMode->GetSelection())
    {
    case 0:
        mode = Tools::CovarianceMatrixFinder::NORMAL;
        break;
    case 1:
        mode = Tools::CovarianceMatrixFinder::ACCUMULATE;
        break;
#if USE_EXACT
    case 2:
        mode = Tools::CovarianceMatrixFinder::PSEUDO_EXACT;
        break;
#endif //#if USE_EXACT
    }

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
            if (!(*iter).boundary)
            {
                continue;
            }

			Tools::CovarianceMatrixFinder *finder = new Tools::CovarianceMatrixFinder();

			finder->setBoundary((*iter).boundary);

			finder->setFrontElementCentroids(frontElements);
			finder->setWeighted(weighted);
			finder->setMode(mode);

			this->tools.push_back(finder);
		}

		this->pdGlobal = new wxProgressDialog(_("Finding covariance matrices"),
			_("Covariance matrices"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
	    if ((*this->datum).boundary)
        {
            Tools::CovarianceMatrixFinder *finder = new Tools::CovarianceMatrixFinder();

            finder->setBoundary((*this->datum).boundary);

            finder->setFrontElementCentroids(frontElements);
			finder->setWeighted(weighted);
			finder->setMode(mode);

            this->tools.push_back(finder);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while finding covariance matrices.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnAnalyzePrincipalComponents(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool frontElements = this->cbCovMatrFrontElements->GetValue();
	bool weighted = this->cbCovMatrWeighted->GetValue();
	enum Tools::CovarianceMatrixFinder::Mode mode = Tools::CovarianceMatrixFinder::NORMAL;

	switch (rbCovMatrMode->GetSelection())
    {
    case 0:
        mode = Tools::CovarianceMatrixFinder::NORMAL;
        break;
    case 1:
        mode = Tools::CovarianceMatrixFinder::ACCUMULATE;
        break;
#if USE_EXACT
    case 2:
        mode = Tools::CovarianceMatrixFinder::PSEUDO_EXACT;
        break;
#endif //#if USE_EXACT
    }

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
            if (!(*iter).boundary)
            {
                continue;
            }

			Tools::PrincipalComponentsAnalyzer *analyzer = new Tools::PrincipalComponentsAnalyzer();

			analyzer->setBoundary((*iter).boundary);

			analyzer->setFrontElementCentroids(frontElements);
			analyzer->setWeighted(weighted);
			analyzer->setMode(mode);

			this->tools.push_back(analyzer);
		}

		this->pdGlobal = new wxProgressDialog(_("Finding covariance matrices"),
			_("Covariance matrices"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
	    if ((*this->datum).boundary)
        {
            Tools::PrincipalComponentsAnalyzer *analyzer = new Tools::PrincipalComponentsAnalyzer();

            analyzer->setBoundary((*this->datum).boundary);

            analyzer->setFrontElementCentroids(frontElements);
			analyzer->setWeighted(weighted);
			analyzer->setMode(mode);

            this->tools.push_back(analyzer);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while analyzing principal components.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnHotellingTransform(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool frontElements = this->cbCovMatrFrontElements->GetValue();
	bool weighted = this->cbCovMatrWeighted->GetValue();
	enum Tools::CovarianceMatrixFinder::Mode mode = Tools::CovarianceMatrixFinder::NORMAL;

	switch (rbCovMatrMode->GetSelection())
    {
    case 0:
        mode = Tools::CovarianceMatrixFinder::NORMAL;
        break;
    case 1:
        mode = Tools::CovarianceMatrixFinder::ACCUMULATE;
        break;
#if USE_EXACT
    case 2:
        mode = Tools::CovarianceMatrixFinder::PSEUDO_EXACT;
        break;
#endif //#if USE_EXACT
    }

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
            if (!(*iter).boundary)
            {
                continue;
            }

			Tools::HotellingTransformer *analyzer = new Tools::HotellingTransformer();

			analyzer->setBoundary((*iter).boundary);

			analyzer->setFrontElementCentroids(frontElements);
			analyzer->setWeighted(weighted);
			analyzer->setMode(mode);

			this->tools.push_back(analyzer);
		}

		this->pdGlobal = new wxProgressDialog(_("Finding covariance matrices"),
			_("Covariance matrices"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
	    if ((*this->datum).boundary)
        {
            Tools::HotellingTransformer *analyzer = new Tools::HotellingTransformer();

            analyzer->setBoundary((*this->datum).boundary);

            analyzer->setFrontElementCentroids(frontElements);
			analyzer->setWeighted(weighted);
			analyzer->setMode(mode);

            this->tools.push_back(analyzer);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while analyzing principal components.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}
#endif //#if USE_NUMERICAL

void CRAbMesh::NumericalModule::OnSmooth(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real factor = 0.0;

	this->tcSmoothFactor->GetValue().ToDouble(&factor);

	Real tolerance = 0.00001;

	bool normalTest = this->cbSmoothNormalTest->GetValue();

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryLaplacianSmoother *smoother =
				new Tools::BoundaryLaplacianSmoother(this->GetDimension(), (*iter).boundary, (*iter).mesh, true, factor);

			smoother->setTolerance(tolerance);
			smoother->setNormalTestEnabled(normalTest);

			this->tools.push_back(smoother);
		}

		this->pdGlobal = new wxProgressDialog(_("Smoothing boundary vertices"),
			_("Smoothing boundary vertices"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryLaplacianSmoother *smoother =
			new Tools::BoundaryLaplacianSmoother(this->GetDimension(), (*this->datum).boundary, (*this->datum).mesh, true, factor);

		smoother->setTolerance(tolerance);
		smoother->setNormalTestEnabled(normalTest);

		this->tools.push_back(smoother);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while smoothing vertices.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRefineSwap(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool noCracks = this->cbSwNoCrack->GetValue();
	bool normalTest = this->cbSwNormalTest->GetValue();
	bool changeSurfaceBoundary = this->cbSwChangeSurface->GetValue();

	Real size = 0.0;

	this->tcSwSizeProportion->GetValue().ToDouble(&size);

	Real threshold = 0.0;

	this->tcSwQualityThreshold->GetValue().ToDouble(&threshold);

	unsigned long int maxEdgesOnBoundary = 0;

	this->tcSwMaxEdgesOnBoundary->GetValue().ToULong(&maxEdgesOnBoundary);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*iter).boundary);
			MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*iter).mesh);

			if ((!mjboundary) && (!mjmesh))
			{
				continue;
			}

			Tools::EdgeSwapBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeSwapBoundaryAdapter(mjmesh) :
                new Tools::EdgeSwapBoundaryAdapter(mjboundary);

			adapter->setNoCrack(noCracks);
			adapter->setNormalTestEnabled(normalTest);
			adapter->setChangeSurfaceBoundaryEnabled(changeSurfaceBoundary);
			adapter->setMaxEdgesOnBoundary(maxEdgesOnBoundary);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);

			this->tools.push_back(adapter);
		}

		this->pdGlobal = new wxProgressDialog(_("Swapping edges"),
			_("Swapping edges"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->datum).boundary);
		MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*this->datum).mesh);

		if ((mjboundary) || (mjmesh))
		{
			Tools::EdgeSwapBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeSwapBoundaryAdapter(mjmesh) :
                new Tools::EdgeSwapBoundaryAdapter(mjboundary);

			adapter->setNoCrack(noCracks);
			adapter->setNormalTestEnabled(normalTest);
			adapter->setChangeSurfaceBoundaryEnabled(changeSurfaceBoundary);
			adapter->setMaxEdgesOnBoundary(maxEdgesOnBoundary);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);

			this->tools.push_back(adapter);
		}
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while swapping edge.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRefineCollapse(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool noCracks = this->cbCNoCrack->GetValue();
	bool inPlane = this->cbInPlane->GetValue();

	Real size = 0.0;

	this->tcCSizeProportion->GetValue().ToDouble(&size);

	Real threshold = 0.0;

	this->tcCQualityThreshold->GetValue().ToDouble(&threshold);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*iter).boundary);
			MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*iter).mesh);

			if ((!mjboundary) && (!mjmesh))
			{
				continue;
			}

			Data::Mesh *mesh = (*iter).mesh;

			Tools::EdgeCollapseBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeCollapseBoundaryAdapter(mjmesh) :
                new Tools::EdgeCollapseBoundaryAdapter(mjboundary, mesh);

			adapter->setNoCrack(noCracks);
			adapter->setOnlyInPlane(inPlane);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);

			this->tools.push_back(adapter);
		}

		this->pdGlobal = new wxProgressDialog(_("Collapsing boundary edges"),
			_("Collapsing boundary edges"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		//MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->boundary));
		MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->datum).boundary);
		MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*this->datum).mesh);

		if ((mjboundary) || (mjmesh))
		{
			Data::Mesh *mesh = (*this->datum).mesh;

			Tools::EdgeCollapseBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeCollapseBoundaryAdapter(mjmesh) :
                new Tools::EdgeCollapseBoundaryAdapter(mjboundary, mesh);

			adapter->setNoCrack(noCracks);
			adapter->setOnlyInPlane(inPlane);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);

			this->tools.push_back(adapter);
		}
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while collapsing edge.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRefineUniformly(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool noCrack = this->cbUNoCrack->GetValue();

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*iter).boundary);
			MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*iter).mesh);

			if ((!mjboundary) && (!mjmesh))
			{
				continue;
			}

			Data::Mesh *mesh = (*iter).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::UniformBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::UniformBoundaryAdapter(mjmesh) :
                new Tools::UniformBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCrack);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}

		this->pdGlobal = new wxProgressDialog(_("Refining boundaries uniformly"),
			_("Refining boundaries uniformly"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->datum).boundary);
		MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*this->datum).mesh);

		if ((mjboundary) || (mjmesh))
		{
			Data::Mesh *mesh = (*this->datum).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::UniformBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::UniformBoundaryAdapter(mjmesh) :
                new Tools::UniformBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCrack);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while refining boundary uniformly.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRefineSize(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool noCrack = this->cbSNoCrack->GetValue();

	Real size = 0.0;

	this->tcSSizeProportion->GetValue().ToDouble(&size);

	UInt maxAdaptations = 1;

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*iter).boundary);
			MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*iter).mesh);

			if ((!mjboundary) && (!mjmesh))
			{
				continue;
			}

			Data::Mesh *mesh = (*iter).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::SizeBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::SizeBoundaryAdapter(mjmesh) :
                new Tools::SizeBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCrack);
			adapter->setSizeProportion(size);
			adapter->setMaxAdaptations(maxAdaptations);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}

		this->pdGlobal = new wxProgressDialog(_("Refining boundary based in size"),
			_("Refining boundary based in size"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->datum).boundary);
		MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*this->datum).mesh);

		if ((mjboundary) || (mjmesh))
		{
			Data::Mesh *mesh = (*this->datum).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::SizeBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::SizeBoundaryAdapter(mjmesh) :
                new Tools::SizeBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCrack);
			adapter->setSizeProportion(size);
			adapter->setMaxAdaptations(maxAdaptations);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while refining based in size.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRefineEdge(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool noCracks = this->cbENoCrack->GetValue();

	Real size = 0.0;

	this->tcESizeProportion->GetValue().ToDouble(&size);

	Real threshold = 0.0;

	this->tcEQualityThreshold->GetValue().ToDouble(&threshold);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*iter).boundary);
			MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*iter).mesh);

			if ((!mjboundary) && (!mjmesh))
			{
				continue;
			}

			Data::Mesh *mesh = (*iter).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::EdgeBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeBoundaryAdapter(mjmesh) :
                new Tools::EdgeBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCracks);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}

		this->pdGlobal = new wxProgressDialog(_("Refining based in edges"),
			_("Refining based in edges"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		MJMesh::Boundary *mjboundary = dynamic_cast<MJMesh::Boundary *>((*this->datum).boundary);
		MJMesh::Mesh *mjmesh = dynamic_cast<MJMesh::Mesh *>((*this->datum).mesh);

		if ((mjboundary) || (mjmesh))
		{
			Data::Mesh *mesh = (*this->datum).mesh;

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(0, 3);

			if (mesh)
			{
				for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
					 iter != mesh->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				if ((!mjmesh) || (!mjmesh->isSurface()))
                {
                    for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                         iter != mesh->feEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(1))
                        {
                            idManager->setId(1, (*iter)->getId());
                        }
                    }
                }
                else
                {
                    for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                         iter != mesh->eEnd(); iter++)
                    {
                        if ((*iter)->getId() > idManager->getId(2))
                        {
                            idManager->setId(2, (*iter)->getId());
                        }
                    }
                }
			}
			else
			{
				for (Data::Boundary::ConstVertexIterator iter = mjboundary->vBegin();
					 iter != mjboundary->vEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(0))
					{
						idManager->setId(0, (*iter)->getId());
					}
				}

				for (Data::Boundary::ConstFrontElementIterator iter = mjboundary->eBegin();
					 iter != mjboundary->eEnd(); iter++)
				{
					if ((*iter)->getId() > idManager->getId(1))
					{
						idManager->setId(1, (*iter)->getId());
					}
				}
			}

			idManager->setId(0, idManager->getId(0) + 1);
			idManager->setId(1, idManager->getId(1) + 1);
			idManager->setId(2, idManager->getId(2) + 1);

			Tools::EdgeBoundaryAdapter *adapter = ((mjmesh) && (mjmesh->isSurface())) ?
                new Tools::EdgeBoundaryAdapter(mjmesh) :
                new Tools::EdgeBoundaryAdapter(mjboundary);

			adapter->setIdManager(idManager);
			adapter->setNoCrack(noCracks);
			adapter->setSizeProportion(size);
			adapter->setQualityThreshold(threshold);
			adapter->setMesh(((mjmesh) && (mjmesh->isSurface())) ? NULL : mesh);

			this->tools.push_back(adapter);
		}
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while refining based in edges.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnMergeMeshes(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.size() <= 1)
	{
		return;
	}

	Tools::MeshMerger *merger = new Tools::MeshMerger();

	Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(1, 3);

	Data::MeshList meshes;

	bool makeBoundary = false;

	for (MeshDataList::iterator iter = this->data.begin();
		 iter != this->data.end(); iter++)
	{
		meshes.push_back((*iter).mesh);

		if ((*iter).boundary)
        {
            makeBoundary = true;
        }
	}

	bool keepOldIds = this->cbMMKeepOldIds->GetValue();

	merger->setMeshes(meshes);
	merger->setShouldMakeBoundary(makeBoundary);
	merger->setKeepOldIds(keepOldIds);
	merger->setIdManager(idManager);

	this->tools.push_back(merger);

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while merging meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnSplitMeshes(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	bool shouldMakeBoundaries = true;

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshSplitter *splitter = new Tools::MeshSplitter();

			splitter->setShouldMakeBoundaries(shouldMakeBoundaries);
			splitter->setMesh((*iter).mesh);

			this->tools.push_back(splitter);
		}

		this->pdGlobal = new wxProgressDialog(_("Splitting meshes"),
			_("Splitting meshes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshSplitter *splitter = new Tools::MeshSplitter();

		splitter->setShouldMakeBoundaries(shouldMakeBoundaries);
		splitter->setMesh((*this->datum).mesh);

		this->tools.push_back(splitter);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while splitting meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnMergeVertices(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real tolerance = 0.0;

	this->tcTolerance->GetValue().ToDouble(&tolerance);

	if (tolerance < 0.0)
	{
		tolerance = 0.0;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::VertexMerger *merger = new Tools::VertexMerger();

			merger->setMesh((*iter).mesh);
			merger->setBoundary((*iter).boundary);
			merger->setUseTolerance(tolerance > 0);
			merger->setTolerance(tolerance);

			this->tools.push_back(merger);
		}

		this->pdGlobal = new wxProgressDialog(_("Merging vertices"),
			_("Merging vertices"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::VertexMerger *merger = new Tools::VertexMerger();

		merger->setMesh((*this->datum).mesh);
		merger->setBoundary((*this->datum).boundary);
		merger->setUseTolerance(tolerance > 0);
		merger->setTolerance(tolerance);

		this->tools.push_back(merger);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while merging vertices.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnMakeBoundary(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::BoundaryCreator *creator = new Tools::BoundaryCreator();

			Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(1, 1);

			if (!(*iter).boundary)
			{
				(*iter).boundary = creator->makeBoundary();
			}

			creator->setMesh((*iter).mesh);
			creator->setBoundary((*iter).boundary);
			creator->setIdManager(idManager);

			this->tools.push_back(creator);
		}

		this->pdGlobal = new wxProgressDialog(_("Making boundary"),
			_("Making boundary"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::BoundaryCreator *creator = new Tools::BoundaryCreator();

		Performer::SequentialIdManager *idManager = new Performer::SequentialIdManager(1, 1);

		if (!(*this->datum).boundary)
		{
			(*this->datum).boundary = creator->makeBoundary();
		}

		creator->setMesh((*this->datum).mesh);
		creator->setBoundary((*this->datum).boundary);
		creator->setIdManager(idManager);

		this->tools.push_back(creator);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while making boundary.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnColorizeMeshes(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshSplitter *splitter = new Tools::MeshSplitter();

			splitter->setDuplicate(false);
			splitter->setMesh((*iter).mesh);

			this->tools.push_back(splitter);
		}

		this->pdGlobal = new wxProgressDialog(_("Colorizing meshes"),
			_("Colorizing meshes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshSplitter *splitter = new Tools::MeshSplitter();

		splitter->setDuplicate(false);
		splitter->setMesh((*this->datum).mesh);

		this->tools.push_back(splitter);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while colorizing meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnColorizeElements(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	unsigned long int scheme = this->cColorScheme->GetSelection();
	UInt first = 0;

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshColorizer *colorizer = new Tools::MeshColorizer();

			colorizer->setColorizeVertices(false);
			colorizer->setColorizeFrontElements(false);
			colorizer->setColorizeElements(true);
			colorizer->setScheme(static_cast<enum Tools::MeshColorizer::ColorizationScheme>(scheme), first);
			colorizer->setMesh((*iter).mesh);

			this->tools.push_back(colorizer);

			if (scheme == Tools::MeshColorizer::UNIQUE)
            {
                first++;
            }
		}

		this->pdGlobal = new wxProgressDialog(_("Colorizing mesh elements"),
			_("Colorizing mesh elements"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshColorizer *colorizer = new Tools::MeshColorizer();

        colorizer->setColorizeVertices(false);
        colorizer->setColorizeFrontElements(false);
        colorizer->setColorizeElements(true);
        colorizer->setScheme(static_cast<enum Tools::MeshColorizer::ColorizationScheme>(scheme), first);
        colorizer->setMesh((*this->datum).mesh);

        this->tools.push_back(colorizer);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while colorizing mesh elements.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnTranslate(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real x, y, z;

	this->tcTranslate[0]->GetValue().ToDouble(&x);
	this->tcTranslate[1]->GetValue().ToDouble(&y);
	this->tcTranslate[2]->GetValue().ToDouble(&z);

	Data::Vector3D v(x, y, z);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

			transformer->setMesh((*iter).mesh);
			transformer->translate(v);

			this->tools.push_back(transformer);
		}

		this->pdGlobal = new wxProgressDialog(_("Translating meshes"),
			_("Translating meshes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

		transformer->setMesh((*this->datum).mesh);
		transformer->translate(v);

		this->tools.push_back(transformer);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while translating meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnScale(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real x, y, z;

	this->tcScale[0]->GetValue().ToDouble(&x);
	this->tcScale[1]->GetValue().ToDouble(&y);
	this->tcScale[2]->GetValue().ToDouble(&z);

	Data::Vector3D v(x, y, z);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

			transformer->setMesh((*iter).mesh);
			transformer->scale(v, false);

			this->tools.push_back(transformer);
		}

		this->pdGlobal = new wxProgressDialog(_("Scaling meshes"),
			_("Scaling meshes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

		transformer->setMesh((*this->datum).mesh);
		transformer->scale(v, false);

		this->tools.push_back(transformer);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while scaling meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnRotate(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real x, y, z, ang;

	this->tcRotate[0]->GetValue().ToDouble(&x);
	this->tcRotate[1]->GetValue().ToDouble(&y);
	this->tcRotate[2]->GetValue().ToDouble(&z);
	this->tcRotate[3]->GetValue().ToDouble(&ang);

	Data::Vector3D v(x, y, z);

	ang = M_PI*ang/180.0;

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

			transformer->setMesh((*iter).mesh);
			transformer->rotate(v, ang);

			this->tools.push_back(transformer);
		}

		this->pdGlobal = new wxProgressDialog(_("Rotating meshes"),
			_("Rotating meshes"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::MeshTransformer *transformer = new Tools::MeshTransformer();

		transformer->setMesh((*this->datum).mesh);
		transformer->rotate(v, ang);

		this->tools.push_back(transformer);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while rotating meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

void CRAbMesh::NumericalModule::OnCentralize(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	Real size;

	this->tcCentralize->GetValue().ToDouble(&size);

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Data::Vector3D min(DBL_MAX, DBL_MAX, DBL_MAX), max(-DBL_MAX, -DBL_MAX, -DBL_MAX);
            Data::Vector3D translate(0.0, 0.0, 0.0);
            Data::Vector3D scale(1.0, 1.0, 1.0);

            UInt dim = 0;

            for (Data::Mesh::VertexIterator iter2 = (*iter).mesh->vBegin();
                 iter2 != (*iter).mesh->vEnd(); iter2++)
            {
                if ((*iter2)->getPoint()->dimension() > dim)
                {
                    dim = (*iter2)->getPoint()->dimension();
                }

                for (UInt i = 0; i < (*iter2)->getPoint()->dimension(); i++)
                {
                    if ((*iter2)->getPoint()->getCoord(i) < min.getCoord(i))
                    {
                        min.setCoord(i, (*iter2)->getPoint()->getCoord(i));
                    }

                    if ((*iter2)->getPoint()->getCoord(i) > max.getCoord(i))
                    {
                        max.setCoord(i, (*iter2)->getPoint()->getCoord(i));
                    }
                }
            }

            Real maxSize = 0.0;

            for (UInt i = 0; i < dim; i++)
            {
                translate.setCoord(i, -0.5*(min.getCoord(i) + max.getCoord(i)));

                if (max.getCoord(i) - min.getCoord(i) > maxSize)
                {
                    maxSize = max.getCoord(i) - min.getCoord(i);
                }
            }

            Tools::MeshTransformer *transformer1 = new Tools::MeshTransformer();

			transformer1->setMesh((*iter).mesh);
			transformer1->translate(translate);

			this->tools.push_back(transformer1);

			if (maxSize > 0.0)
            {
                for (UInt i = 0; i < dim; i++)
                {
                    scale.setCoord(i, size/maxSize);
                }

                Tools::MeshTransformer *transformer2 = new Tools::MeshTransformer();

                transformer2->setMesh((*iter).mesh);
                transformer2->scale(scale);

                this->tools.push_back(transformer2);
            }
		}

		this->pdGlobal = new wxProgressDialog(_("Centralizing meshes"),
			_("Centralizing meshes"),
			static_cast<int>(this->tools.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Data::Vector3D min(DBL_MAX, DBL_MAX, DBL_MAX), max(-DBL_MAX, -DBL_MAX, -DBL_MAX);
        Data::Vector3D translate(0.0, 0.0, 0.0);
        Data::Vector3D scale(1.0, 1.0, 1.0);

        UInt dim = 0;

        for (Data::Mesh::VertexIterator iter2 = (*this->datum).mesh->vBegin();
             iter2 != (*this->datum).mesh->vEnd(); iter2++)
        {
            if ((*iter2)->getPoint()->dimension() > dim)
            {
                dim = (*iter2)->getPoint()->dimension();
            }

            for (UInt i = 0; i < (*iter2)->getPoint()->dimension(); i++)
            {
                if ((*iter2)->getPoint()->getCoord(i) < min.getCoord(i))
                {
                    min.setCoord(i, (*iter2)->getPoint()->getCoord(i));
                }

                if ((*iter2)->getPoint()->getCoord(i) > max.getCoord(i))
                {
                    max.setCoord(i, (*iter2)->getPoint()->getCoord(i));
                }
            }
        }

        Real maxSize = 0.0;

        for (UInt i = 0; i < dim; i++)
        {
            translate.setCoord(i, -0.5*(min.getCoord(i) + max.getCoord(i)));

            if (max.getCoord(i) - min.getCoord(i) > maxSize)
            {
                maxSize = max.getCoord(i) - min.getCoord(i);
            }
        }

        Tools::MeshTransformer *transformer1 = new Tools::MeshTransformer();

        transformer1->setMesh((*this->datum).mesh);
        transformer1->translate(translate);

        this->tools.push_back(transformer1);

        if (maxSize > 0.0)
        {
            for (UInt i = 0; i < dim; i++)
            {
                scale.setCoord(i, size/maxSize);
            }

            Tools::MeshTransformer *transformer2 = new Tools::MeshTransformer();

            transformer2->setMesh((*this->datum).mesh);
            transformer2->scale(scale);

            this->tools.push_back(transformer2);
        }
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while centralizing meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}

#if USE_TECGRAF
void CRAbMesh::NumericalModule::OnBuildBoundaries(wxCommandEvent &WXUNUSED(event))
{
#if (!USE_ONLY_MTOOLS)
	if (this->data.empty())
	{
		return;
	}

	if (this->ExecuteForAllMeshes())
	{
		for (MeshDataList::iterator iter = this->data.begin();
			 iter != this->data.end(); iter++)
		{
			Tools::TecGrafBoundaryBuilder *builder = new Tools::TecGrafBoundaryBuilder(true);

			builder->setModel((*iter).model);
			builder->setMakeMeshes(true);

			this->tools.push_back(builder);
		}

		this->pdGlobal = new wxProgressDialog(_("Building boundaries"),
			_("Building boundaries"),
			static_cast<int>(this->data.size()),
			this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}
	else
	{
		Tools::TecGrafBoundaryBuilder *builder = new Tools::TecGrafBoundaryBuilder(true);

		builder->setModel((*this->datum).model);
		builder->setMakeMeshes(true);

		this->tools.push_back(builder);
	}

	this->pdLocal = new wxProgressDialog(_("Please wait"),
		_("Please wait while rotating meshes.\nCheck the log for progress and errors"),
		100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

	this->ExecuteTools(false);
#endif //#if (!USE_ONLY_MTOOLS)
}
#endif //#if USE_TECGRAF*/

void CRAbMesh::NumericalModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	//if (this->data.empty())
	{
	    if (this->tcLog)
        {
            this->tcLog->Clear();
        }

		//return;
	}

	int ok = wxMessageBox(_("Are you sure you want to clear everything?"),
		_("Clearing"), wxYES_NO | wxICON_QUESTION);

	if (ok == wxYES)
	{
		this->Clear();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

/*void CRAbMesh::NumericalModule::OnPickedShow(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::const_iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		wxString str((*iter)->text().c_str(), wxMBConvLibc());

		Data::Vertex *v = dynamic_cast<Data::Vertex *>((*iter));

		if (v)
		{
			text << _("Vertex ");
		}
		else
		{
			Data::FrontElement *fe = dynamic_cast<Data::FrontElement *>((*iter));

			if (fe)
			{
				text << _("Front Element ");
			}
			else
			{
				Data::Element *e = dynamic_cast<Data::Element *>((*iter));

				if (e)
				{
					text << _("Element ");
				}
				else
				{
					text << _("Unrecognized ");
				}
			}
		}

		text << str << _("\n");
	}

	wxMessageBox(text, _("Picked item"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteFirstVertex(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		Data::Vertex *v = dynamic_cast<Data::Vertex *>((*iter));

		if (!v)
		{
			continue;
		}

		text = this->Delete(v);

		this->picked.erase(iter);

		break;
	}

	if (text.IsEmpty())
	{
		text << _("No vertex deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted vertex"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteFirstFrontElement(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		Data::FrontElement *fel = dynamic_cast<Data::FrontElement *>((*iter));

		if (!fel)
		{
			continue;
		}

		text = this->Delete(fel);

		this->picked.erase(iter);

		break;
	}

	if (text.IsEmpty())
	{
		text << _("No front element deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted front element"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteFirstElement(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		Data::Element *e = dynamic_cast<Data::Element *>((*iter));

		if ((!e) || (dynamic_cast<Data::FrontElement *>(*iter)))
		{
			continue;
		}

		text = this->Delete(e);

		this->picked.erase(iter);

		break;
	}

	if (text.IsEmpty())
	{
		text << _("No element deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted element"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteVertices(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end();)
	{
		Data::Vertex *v = dynamic_cast<Data::Vertex *>((*iter));

		if (!v)
		{
			iter++;

			continue;
		}

		if (!text.IsEmpty())
		{
			text << _("\n");
		}

		text << this->Delete(v);

		iter = this->picked.erase(iter);
	}

	if (text.IsEmpty())
	{
		text << _("No vertex deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted vertices"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteFrontElements(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end();)
	{
		Data::FrontElement *fel = dynamic_cast<Data::FrontElement *>((*iter));

		if (!fel)
		{
			iter++;

			continue;
		}

		if (!text.IsEmpty())
		{
			text << _("\n");
		}

		text << this->Delete(fel);

		iter = this->picked.erase(iter);
	}

	if (text.IsEmpty())
	{
		text << _("No front element deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted front elements"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteElements(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end();)
	{
		Data::Element *e = dynamic_cast<Data::Element *>((*iter));

		if ((!e) || (dynamic_cast<Data::FrontElement *>(*iter)))
		{
			iter++;

			continue;
		}

		if (!text.IsEmpty())
		{
			text << _("\n");
		}

		text << this->Delete(e);

		iter = this->picked.erase(iter);
	}

	if (text.IsEmpty())
	{
		text << _("No element deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted elements"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedDeleteSelected(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.empty())
	{
		return;
	}

	wxString text;

	while (!this->picked.empty())
	{
		Data::Drawable *d = this->picked.front();

		this->picked.pop_front();

		if (!text.IsEmpty())
		{
			text << _("\n");
		}

		Data::Vertex *v = dynamic_cast<Data::Vertex *>(d);

		if (v)
		{
			text << this->Delete(v);

			continue;
		}

		Data::FrontElement *fel = dynamic_cast<Data::FrontElement *>(d);

		if (fel)
		{
			text << this->Delete(fel);

			continue;
		}

		Data::Element *e = dynamic_cast<Data::Element *>(d);

		if (e)
		{
			text << this->Delete(e);

			continue;
		}

		text << this->Delete(d);
	}

	if (text.IsEmpty())
	{
		text << _("Nothing deleted");
	}
	else
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	wxMessageBox(text, _("Deleted selected"), wxOK);
}

void CRAbMesh::NumericalModule::OnPickedMergeVertices(wxCommandEvent &WXUNUSED(event))
{
	if (this->picked.size() <= 1)
	{
		return;
	}

	MJMesh::Vertex *v1 = NULL, *v2 = NULL;
	bool tolerance = false;

	for (Data::DrawableList::iterator iter = this->picked.begin();
		 iter != this->picked.end(); iter++)
	{
		MJMesh::Vertex *v = dynamic_cast<MJMesh::Vertex *>((*iter));

		if (!v)
		{
			continue;
		}

		if (!v1)
		{
			v1 = v;

			continue;
		}

		bool match = v->getPoint()->match(v1->getPoint());
		bool equal = false;

		if (!match)
		{
			equal = v->getPoint()->equal(v1->getPoint());
		}

		if ((!match) && (!equal))
		{
			continue;
		}

		if (equal)
		{
			tolerance = true;
		}

		v2 = v;

		this->picked.erase(iter);

		break;
	}

	if ((!v1) || (!v2))
	{
		return;
	}

	if ((!this->GetMesh()->have(v1)) || (!this->GetMesh()->have(v2)))
	{
		return;
	}

	Data::FrontElementList fadjs = this->GetMesh()->findAdjacentFrontElements(v2);
	Data::ElementList adjs = this->GetMesh()->adjacency(v2);

	while (!fadjs.empty())
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fadjs.front());

		fadjs.pop_front();

		fel->replace(v2, v1);

		v2->remove(fel);

		v1->add(fel);
	}

	while (!adjs.empty())
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>(adjs.front());

		adjs.pop_front();

		e->replace(v2, v1);

		v2->remove(e);

		v1->add(e);
	}

	(*this->datum).mesh->remove(v2);

	if ((this->GetBoundary()) && (this->GetBoundary()->have(v2)))
	{
		(*this->datum).boundary->remove(v2);
	}

	if ((this->GetFront()) && (this->GetFront()->have(v2)))
	{
		(*this->datum).front->remove(v2);
	}

	if (tolerance)
	{
        VECTORpp(v, v1->getPoint()->dimension(), v1->getPoint(), v2->getPoint());

		v.multiply(0.5);

		v1->getPoint()->sum(v);
	}

	delete v2;

	this->frame->GetViewModule()->ClearDisplayLists();

	this->FillTree();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::NumericalModule::OnPickedClearPickedList(wxCommandEvent &WXUNUSED(event))
{
    this->picked.clear();

    this->canvas->Refresh(false);
    this->canvas->Update();
}*/

/*bool CRAbMesh::NumericalModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
#if USE_TECGRAF
	minx = miny = minz = DBL_MAX;
	maxx = maxy = maxz = -DBL_MAX;

	if (this->GetModel())
	{
		Data::Box box = this->GetModel()->box();

		minx = box.getMin()->getCoord(0);
		miny = box.getMin()->getCoord(1);
		minz = box.getMin()->getCoord(2);

		maxx = box.getMax()->getCoord(0);
		maxy = box.getMax()->getCoord(1);
		maxz = box.getMax()->getCoord(2);

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}
#endif //#if USE_TECGRAF

	return CRAbMesh::MeshModule::BoundingBox(minx, miny, minz, maxx, maxy, maxz);
}*/

void CRAbMesh::NumericalModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::NumericalModule::GetDimension() const
{
	/*unsigned int dimension = 0;

	if ((dimension == 0) && (this->GetMesh()))
	{
		dimension = this->GetMesh()->dimension();
	}

	if ((dimension == 0) && (this->GetFront()))
	{
		dimension = this->GetFront()->dimension();
	}

	if ((dimension == 0) && (this->GetBoundary()))
	{
		dimension = this->GetBoundary()->dimension();
	}

#if USE_TECGRAF
	if ((dimension == 0) && (this->GetModel()))
	{
		dimension = this->GetModel()->dimension();
	}
#endif //#if USE_TECGRAF

	return dimension;*/

	return 3;
}

void CRAbMesh::NumericalModule::SetTree(Data::BoxTree */*tree*/)
{

}

const Data::BoxTree *CRAbMesh::NumericalModule::GetTree() const
{
	return NULL;
	//return this->debug;
}

void CRAbMesh::NumericalModule::SetBoundary(Data::Boundary */*boundary*/)
{

}

const Data::Boundary *CRAbMesh::NumericalModule::GetBoundary() const
{
    return NULL;
	//return (this->datum != this->data.end()) ? (*this->datum).boundary : NULL;
}

void CRAbMesh::NumericalModule::SetFront(Data::Front */*front*/)
{

}

const Data::Front *CRAbMesh::NumericalModule::GetFront() const
{
    return NULL;
	//return (this->datum != this->data.end()) ? (*this->datum).front : NULL;
}

void CRAbMesh::NumericalModule::SetMesh(Data::Mesh */*mesh*/)
{

}

const Data::Mesh *CRAbMesh::NumericalModule::GetMesh() const
{
    return this->mesh;
}

wxString CRAbMesh::NumericalModule::GetLoadedFilename() const
{
    return _("");
	//return (this->datum == this->data.end()) ? wxString() : wxString((*this->datum).filename.c_str(), wxMBConvLibc());
}

wxString CRAbMesh::NumericalModule::QualityString() const
{
	return _("");
}

/*#if USE_TECGRAF
void CRAbMesh::NumericalModule::ClearDisplayLists(bool force) const
{
	if (this->GetModel())
	{
		bool clearing = force;

		if (!clearing)
		{
			clearing = this->GetModel()->isOutdated();
		}

		if (!clearing)
		{
#if USE_C__11
			for (TecGraf::SurfaceHash::const_iterator iter = this->GetModel()->sBegin();
#else
			for (TecGraf::SurfaceHash::const_iterator iter = this->GetModel()->sBegin();
#endif //#if USE_C__11
				 iter != this->GetModel()->sEnd(); iter++)
			{
				if ((*iter)->isOutdated())
				{
					clearing = true;

					break;
				}
			}
		}

		if (!clearing)
		{
			for (TecGraf::RegionList::const_iterator iter = this->GetModel()->rBegin();
				 iter != this->GetModel()->rEnd(); iter++)
			{
				if ((*iter)->isOutdated())
				{
					clearing = true;

					break;
				}

				for (TecGraf::OrientedSurfaceList::iterator iter2 = (*iter)->begin();
					 iter2 != (*iter)->end(); iter2++)
				{
					if ((*iter2)->isOutdated())
					{
						clearing = true;

						break;
					}
				}

				if (clearing)
				{
					break;
				}
			}
		}

		if (clearing)
		{
			this->GetModel()->clearLists();
		}
	}

	MeshModule::ClearDisplayLists(force);
}*/

void CRAbMesh::NumericalModule::RenderLineRange(bool x, bool y, bool z) const
{
    if ((!x) && (!y) && (!z))
    {
        return;
    }

    bool line[3] = {x, y, z};
    double pt[3][2] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    int num[3] = {0, 0, 0};
    double h[3] = {0.0, 0.0, 0.0};

    for (int i = 0; i < 3; i++)
    {
        pt[i][0] = -10.0;
        pt[i][1] =  10.0;
        num[i] = 20;

        //this->tcMin[i]->GetValue().ToDouble(&pt[i][0]);
        //this->tcMax[i]->GetValue().ToDouble(&pt[i][1]);
        //this->tcNum[i]->GetValue().ToUInt(&num[i]);

        if (num[i] < 1)
        {
            num[i] = 1;
        }

        h[i] = (pt[i][1] - pt[i][0])/static_cast<double>(num[i]);
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        glLineWidth(1.0);
        //glColor3d(1.0, 1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

            //glLoadIdentity();
            glBegin(GL_LINES);
                for (int i = 0; i < 3; i++)
                {
                    if (!line[i])
                    {
                        continue;
                    }

                    double c[3] = {0.0, 0.0, 0.0};

                    c[i] = {1.0};

                    glColor3d(c[0], c[1], c[2]);

                    double ptmin[3] = {0.0, 0.0, 0.0};
                    double ptmax[3] = {0.0, 0.0, 0.0};

                    ptmin[i] = pt[i][0];
                    ptmax[i] = pt[i][1];

                    glVertex3d(ptmin[0], ptmin[1], ptmin[2]);
                    glVertex3d(ptmax[0], ptmax[1], ptmax[2]);

                    double pt1min[3] = {0.0, 0.0, 0.0};
                    double pt1max[3] = {0.0, 0.0, 0.0};
                    double pt2min[3] = {0.0, 0.0, 0.0};
                    double pt2max[3] = {0.0, 0.0, 0.0};

                    pt1min[(i+1)%3] = pt2min[(i+2)%3] = -h[i]*0.5;
                    pt1max[(i+1)%3] = pt2max[(i+2)%3] =  h[i]*0.5;

                    for (int j = 0; j <= num[i]; j++)
                    {
                        pt1min[i] = pt1max[i] = pt2min[i] = pt2max[i] = pt[i][0] + j*h[i];

                        glVertex3d(pt1min[0], pt1min[1], pt1min[2]);
                        glVertex3d(pt1max[0], pt1max[1], pt1max[2]);
                        glVertex3d(pt2min[0], pt2min[1], pt2min[2]);
                        glVertex3d(pt2max[0], pt2max[1], pt2max[2]);
                    }
                }
            glEnd();

        glPopMatrix();
    glPopAttrib();
}

void CRAbMesh::NumericalModule::RenderPlaneRange(bool xy, bool xz, bool yz) const
{
    if ((!xy) && (!xz) && (!yz))
    {
        return;
    }

    bool line[3] = {yz, xz, xy};
    double pt[3][2] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    int num[3] = {0, 0, 0};
    double h[3] = {0.0, 0.0, 0.0};

    for (int i = 0; i < 3; i++)
    {
        pt[i][0] = -10.0;
        pt[i][1] =  10.0;
        num[i] = 20;

        //this->tcMin[i]->GetValue().ToDouble(&pt[i][0]);
        //this->tcMax[i]->GetValue().ToDouble(&pt[i][1]);
        //this->tcNum[i]->GetValue().ToUInt(&num[i]);

        if (num[i] < 1)
        {
            num[i] = 1;
        }

        h[i] = (pt[i][1] - pt[i][0])/static_cast<double>(num[i]);
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        glLineWidth(1.0);
        //glColor3d(1.0, 1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

            //glLoadIdentity();

            for (int i = 0; i < 3; i++)
            {
                if (!line[i])
                {
                    continue;
                }

                double c[3] = {1.0, 1.0, 1.0};

                c[i] = {0.0};

                glColor3d(c[0], c[1], c[2]);

                //double ptmin[3] = {0.0, 0.0, 0.0};
                //double ptmax[3] = {0.0, 0.0, 0.0};

                //ptmin[(i+1)%3] = pt[(i+1)%3][0];
                //ptmin[(i+2)%3] = pt[(i+2)%3][0];
                //ptmax[(i+1)%3] = pt[(i+1)%3][1];
                //ptmax[(i+2)%3] = pt[(i+2)%3][1];

                //glVertex3d(ptmin[0], ptmin[1], ptmin[2]);
                //glVertex3d(ptmax[0], ptmax[1], ptmax[2]);

                double pt1[3] = {0.0, 0.0, 0.0};
                double pt2[3] = {0.0, 0.0, 0.0};
                double pt3[3] = {0.0, 0.0, 0.0};
                double pt4[3] = {0.0, 0.0, 0.0};

                for (int j = 0; j < num[(i+1)%3]; j++)
                {
                    for (int k = 0; k < num[(i+2)%3]; k++)
                    {
                        pt1[(i+1)%3] = pt[(i+1)%3][0] + j*h[(i+1)%3];
                        pt1[(i+2)%3] = pt[(i+2)%3][0] + k*h[(i+2)%3];

                        pt2[(i+1)%3] = pt[(i+1)%3][0] + (j+1)*h[(i+1)%3];
                        pt2[(i+2)%3] = pt[(i+2)%3][0] + k*h[(i+2)%3];

                        pt3[(i+1)%3] = pt[(i+1)%3][0] + (j+1)*h[(i+1)%3];
                        pt3[(i+2)%3] = pt[(i+2)%3][0] + (k+1)*h[(i+2)%3];

                        pt4[(i+1)%3] = pt[(i+1)%3][0] + j*h[(i+1)%3];
                        pt4[(i+2)%3] = pt[(i+2)%3][0] + (k+1)*h[(i+2)%3];

                        glBegin(GL_LINE_LOOP);
                            glVertex3d(pt1[0], pt1[1], pt1[2]);
                            glVertex3d(pt2[0], pt2[1], pt2[2]);
                            glVertex3d(pt3[0], pt3[1], pt3[2]);
                            glVertex3d(pt4[0], pt4[1], pt4[2]);
                        glEnd();
                    }
                }
            }

        glPopMatrix();
    glPopAttrib();
}


bool CRAbMesh::NumericalModule::ClipSegment(Real x1, Real y1, Real x2, Real y2, Real miny, Real maxy, Real coords[4]) const
{
    if (((y1 < miny) && (y2 < miny)) ||
        ((y1 > maxy) && (y2 > maxy)))
    {
        return false;
    }

    if ((y1 >= miny) && (y1 <= maxy) && (y2 >= miny) && (y2 <= maxy))
    {
        coords[0] = x1;
        coords[1] = y1;
        coords[2] = x2;
        coords[3] = y2;

        return true;
    }

    if (y1 < miny)
    {
        Real t = (miny - y1)/(y2 - y1);

        y1 = y1 + t*(y2 - y1);
        x1 = x1 + t*(x2 - x1);
    }

    if (y2 < miny)
    {
        Real t = (miny - y1)/(y2 - y1);

        y2 = y1 + t*(y2 - y1);
        x2 = x1 + t*(x2 - x1);
    }

    if (y1 > maxy)
    {
        Real t = (maxy - y1)/(y2 - y1);

        y1 = y1 + t*(y2 - y1);
        x1 = x1 + t*(x2 - x1);
    }

    if (y2 > maxy)
    {
        Real t = (maxy - y1)/(y2 - y1);

        y2 = y1 + t*(y2 - y1);
        x2 = x1 + t*(x2 - x1);
    }

    coords[0] = x1;
    coords[1] = y1;
    coords[2] = x2;
    coords[3] = y2;

    return true;
}

void CRAbMesh::NumericalModule::RenderEquationRootFunction() const
{
    std::string str = std::string(this->tcERFuncion->GetValue().mb_str(wxMBConvLibc()));

    Numerical::InterpretedFunction function;
    function.setFunction(str);

    Real minx = 0.0;
    this->tcDrawCoords[0][0]->GetValue().ToDouble(&minx);
    Real maxx = 0.0;
    this->tcDrawCoords[0][1]->GetValue().ToDouble(&maxx);

    Real miny = 0.0;
    this->tcDrawCoords[1][0]->GetValue().ToDouble(&miny);
    Real maxy = 0.0;
    this->tcDrawCoords[1][1]->GetValue().ToDouble(&maxy);

    unsigned long m = 0;
    this->tcDrawRefinement->GetValue().ToULong(&m);

    Real h = (maxx - minx)/static_cast<Real>(m);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        glLineWidth(1.0);

        glMatrixMode(GL_MODELVIEW);
        glBegin(GL_LINES);

        Real coords[4];

        Real px = minx;
        Real fpx = function.f(px);

        for (ULInt i = 1; i < m + 1; i++)
        {
            Real x = minx + static_cast<Real>(i)*h;
            Real fx = function.f(x);

            if (this->ClipSegment(px, fpx, x, fx, miny, maxy, coords))
            {
                glVertex3d(coords[0], coords[1], 0.0);
                glVertex3d(coords[2], coords[3], 0.0);
            }

            px = x;
            fpx = fx;
        }

        glEnd();
    glPopAttrib();
}

void CRAbMesh::NumericalModule::Render() const
{
    this->RenderLineRange(this->IsConfiguration(DRAW_X_RANGE), this->IsConfiguration(DRAW_Y_RANGE), this->IsConfiguration(DRAW_Z_RANGE));
    this->RenderPlaneRange(this->IsConfiguration(DRAW_XY_RANGE), this->IsConfiguration(DRAW_XZ_RANGE), this->IsConfiguration(DRAW_YZ_RANGE));

    if (this->IsConfiguration(DRAW_ER_FUNCTION))
    {
        this->RenderEquationRootFunction();
    }

    CRAbMesh::MeshModule::Render();

	/*if (this->IsDrawable())
	{
#if USE_SCREEENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREEENSHOT

		//if (((this->IsRegionsDrawable()) || (this->IsSurfacesDrawable())) && (this->GetModel()))
		if ((this->IsModelDrawable()) && (this->GetModel()))
		{
			this->GetModel()->setDrawingRegions(false);

			this->GetModel()->draw();
		}
	}*/
}

/*void CRAbMesh::NumericalModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	if (this->GetModel())
	{
		if (this->IsEnabled(REGIONS, TREE_ITEM))
		{
			TecGraf::RegionList regions = this->GetModel()->getRegions();
			regions.sort(Data::Identifiable::less);

			while (!regions.empty())
			{
				this->AddTreeItem(regions.front());

				regions.pop_front();
			}
		}

		if (this->IsEnabled(SURFACES, TREE_ITEM))
		{
			TecGraf::SurfaceHash hsurfaces = this->GetModel()->getSurfaces();
			TecGraf::SurfaceList surfaces(hsurfaces.begin(), hsurfaces.end());
			surfaces.sort(Data::Identifiable::less);

			while (!surfaces.empty())
			{
				this->AddTreeItem(surfaces.front());

				surfaces.pop_front();
			}
		}

		if (this->IsEnabled(VERTICES, TREE_ITEM))
		{
#if USE_C__11
			Data::VertexHash svertices;

			for (TecGraf::SurfaceHash::const_iterator iter = this->GetModel()->sBegin();
#else
			Data::VertexSet svertices;

			for (TecGraf::SurfaceSet::const_iterator iter = this->GetModel()->sBegin();
#endif //#if USE_C__11
				 iter != this->GetModel()->sEnd(); iter++)
			{
#if USE_C__11
				svertices.reserve(svertices.size() + (*iter)->verticesSize());
#endif //#if USE_C__11

				svertices.insert((*iter)->vBegin(), (*iter)->vEnd());
			}

			Data::VertexList vertices(svertices.begin(), svertices.end());

			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}

		if (this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM))
		{
#if USE_C__11
			Data::FrontElementHash sfels;

			for (TecGraf::SurfaceHash::const_iterator iter = this->GetModel()->sBegin();
#else
			Data::FrontElementSet sfels;

			for (TecGraf::SurfaceSet::const_iterator iter = this->GetModel()->sBegin();
#endif //#if USE_C__11
				 iter != this->GetModel()->sEnd(); iter++)
			{
#if USE_C__11
				sfels.reserve(sfels.size() + (*iter)->size());
#endif //#if USE_C__11

				sfels.insert((*iter)->eBegin(), (*iter)->eEnd());
			}

			Data::FrontElementList fels(sfels.begin(), sfels.end());

			fels.sort(Data::Identifiable::less);

			while (!fels.empty())
			{
				this->AddTreeItem(fels.front());

				fels.pop_front();
			}
		}
	}
	else
	{
		MeshModule::ForceFillTree(false);
	}
}

void CRAbMesh::NumericalModule::AddTreeItem(TecGraf::Region *r)
{
	if (this->IsEnabled(REGIONS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(r);
		this->tree->AppendItem(this->tiItems[REGIONS], item->GetText(), -1, -1, item);
	}
}

void CRAbMesh::NumericalModule::AddTreeItem(TecGraf::Surface *s)
{
	if (this->IsEnabled(SURFACES, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(s);
		this->tree->AppendItem(this->tiItems[SURFACES], item->GetText(), -1, -1, item);
	}
}
#endif //#if USE_TECGRAF*/

/*void CRAbMesh::NumericalModule::ExecuteTools(bool dialog)
{
	wxString message, title;

	if ((dialog) && (this->pdLocal))
	{
		message = this->pdLocal->GetMessage();
		title = this->pdLocal->GetTitle();

		this->pdLocal->Update(this->pdLocal->GetRange());

		delete this->pdLocal;

		this->pdLocal = NULL;
	}

	if (this->tools.empty())
	{
        if (this->pdGlobal)
		{
			this->pdGlobal->Update(this->pdGlobal->GetRange());

			delete this->pdGlobal;

			this->pdGlobal = NULL;
		}

		return;
	}

#if (!USE_ONLY_MTOOLS)
	dynamic_cast<Tools::MainDrive *>(this->drive)->setTool(this->tools.front());
#endif //#if (!USE_ONLY_MTOOLS)

	this->tools.pop_front();

	if (dialog)
	{
		this->pdLocal = new wxProgressDialog(title, message, 100, this->frame, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	}

	//same as OnExecute()
	this->Execute();

	this->frame->Execute();
	//end same as OnExecute()
}*/

//#if USE_ONLY_MTOOLS
//void CRAbMesh::NumericalModule::ShowMessage(Tools::Tool */*tool*/)
//#else
//void CRAbMesh::NumericalModule::ShowMessage(Tools::Tool *tool)
//#endif //#if USE_ONLY_MTOOLS
/*{
#if (!USE_ONLY_MTOOLS)
	if (this->ShowMessage(dynamic_cast<Tools::MeshInterceptionChecker *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::MeshTopologyChecker *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryWaterTightnessChecker *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::CrackRemover *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryConversor *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryNormalFixer *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryNormalInverter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryLaplacianSmoother *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::UniformBoundaryAdapter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::SizeBoundaryAdapter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::EdgeSwapBoundaryAdapter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::EdgeCollapseBoundaryAdapter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::EdgeBoundaryAdapter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::InnerSurfacesIdentifier *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::SilhouetteIdentifier *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::SurfacesIdentifier *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::VisibilityIdentifier *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::FrontElementVisibilityIdentifier *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::VisibilityBasedVertexFinder *>(tool)))
	{
		return;
	}

#if USE_NUMERICAL
	if (this->ShowMessage(dynamic_cast<Tools::CovarianceMatrixFinder *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::PrincipalComponentsAnalyzer *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::HotellingTransformer *>(tool)))
	{
		return;
	}
#endif //#if USE_NUMERICAL

	if (this->ShowMessage(dynamic_cast<Tools::MeshSplitter *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::MeshColorizer *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::MeshMerger *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::VertexMerger *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::BoundaryCreator *>(tool)))
	{
		return;
	}

	if (this->ShowMessage(dynamic_cast<Tools::MeshTransformer *>(tool)))
	{
		return;
	}

#if USE_TECGRAF
	if (this->ShowMessage(dynamic_cast<Tools::TecGrafBoundaryBuilder *>(tool)))
	{
		return;
	}
#endif //#if USE_TECGRAF
#endif //#if (!USE_ONLY_MTOOLS)
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshInterceptionChecker *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getInterceptions().empty())
	{
		wxMessageBox(_("Mesh does not intercept"), _("No interception detected"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Mesh intercepts itself"), _("Interception detected"), wxOK | wxICON_ERROR);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshTopologyChecker *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isConsistent())
	{
		wxMessageBox(_("Mesh is topologically consistent"), _("No topological inconsistency detected"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Mesh is not topologically consistent"), _("Topological inconsistency detected"), wxOK | wxICON_ERROR);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryWaterTightnessChecker *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getInconsistents().empty())
	{
		wxMessageBox(_("Boundary is water-tight"), _("No inconsistency detected"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Boundary is not water-tight"), _("Inconsistency detected"), wxOK | wxICON_ERROR);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::CrackRemover *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isAnyCrackRemoved())
	{
		wxMessageBox(_("Cracks removed"), _("Cracks removed"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxMessageBox(_("No crack detected in boundary"), _("No crack detected in boundary"), wxOK | wxICON_EXCLAMATION);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryConversor *tool)
{
	if (!tool)
	{
		return false;
	}

	wxMessageBox(_("Boundary copied/converted"), _("Boundary copied/converted"), wxOK | wxICON_EXCLAMATION);

	Data::Boundary *newBoundary = tool->getConverted();

	MeshData datum;
	datum.boundary = newBoundary;

	this->data.push_back(datum);

	this->EnsureExistenceOfMesh();

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryNormalFixer *tool)
{
	if (!tool)
	{
		return false;
	}

	if ((tool->getStatus().getType() == tool->success.getType()))
	{
		if (tool->isAnyInverted())
		{
			wxMessageBox(_("Boundary normals fixed"), _("Boundary normals fixed"), wxOK | wxICON_EXCLAMATION);

			this->frame->GetViewModule()->ClearDisplayLists();

			this->FillTree();

			this->canvas->Refresh(false);
			this->canvas->Update();
		}
		else
		{
			wxMessageBox(_("No fixing necessary"), _("No fixing necessary"), wxOK | wxICON_EXCLAMATION);
		}
	}
	else
	{
		wxMessageBox(_("Normal fixing failed"), _("Normal fixing failed"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryNormalInverter *tool)
{
	if (!tool)
	{
		return false;
	}

	wxMessageBox(_("Boundary normals inverted"), _("Boundary normals inverted"), wxOK | wxICON_EXCLAMATION);

	this->frame->GetViewModule()->ClearDisplayLists();

	this->FillTree();

	this->canvas->Refresh(false);
	this->canvas->Update();

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryLaplacianSmoother *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isSmoothed())
	{
		wxMessageBox(_("Boundary smoothed"), _("Boundary smoothed"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxMessageBox(_("No smoothing necessary"), _("No smoothing necessary"), wxOK | wxICON_EXCLAMATION);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::UniformBoundaryAdapter *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	wxMessageBox(_("Boundary refined uniformly"), _("Boundary refined uniformly"), wxOK | wxICON_EXCLAMATION);

	this->frame->GetViewModule()->ClearDisplayLists();

	this->FillTree();

	this->canvas->Refresh(false);
	this->canvas->Update();

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::SizeBoundaryAdapter *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	if (!tool->isOk())
	{
		wxMessageBox(_("Could not refine based in size"), _("Could not refine based in size"), wxOK | wxICON_ERROR);
	}
	else if (!tool->isAdapted())
	{
		wxMessageBox(_("No refinement necessary"), _("No refinement necessary"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Boundary refined based in size"), _("Boundary refined based in size"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::EdgeSwapBoundaryAdapter *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	if (!tool->isOk())
	{
		wxMessageBox(_("Could not swap edges"), _("Could not swap edges"), wxOK | wxICON_ERROR);
	}
	else if (!tool->isAdapted())
	{
		wxMessageBox(_("No swapping necessary"), _("No swapping necessary"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Edges swapped"), _("Edges swapped"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::EdgeCollapseBoundaryAdapter *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	if (!tool->isOk())
	{
		wxMessageBox(_("Could not collapse edges"), _("Could not collapse edges"), wxOK | wxICON_ERROR);
	}
	else if (!tool->isAdapted())
	{
		wxMessageBox(_("No collapsing necessary"), _("No collapsing necessary"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Edges collapsed"), _("Edges collapsed"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::EdgeBoundaryAdapter *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	if (!tool->isOk())
	{
		wxMessageBox(_("Could not refine based in edges"), _("Could not refine based in edges"), wxOK | wxICON_ERROR);
	}
	else if (!tool->isAdapted())
	{
		wxMessageBox(_("No refining necessary"), _("No refining necessary"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Boundary refined based in edges"), _("Boundary refined based in edges"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::InnerSurfacesIdentifier *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isManifold())
	{
		wxMessageBox(_("Inner surfaces identified"), _("Inner surfaces identified"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxMessageBox(_("Inner surfaces not identified"), _("Inner surfaces not identified\nBoundary is not manifold"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::SilhouetteIdentifier *tool)
{
	if (!tool)
	{
		return false;
	}

	Data::Mesh *silhouette = tool->getSilhouette();

	if (silhouette)
	{
		wxMessageBox(_("Silhouette identified"), _("Silhouette identified"), wxOK | wxICON_EXCLAMATION);

		MeshData datum;

		datum.mesh = silhouette;

		MJMesh::Boundary *boundary = new MJMesh::Boundary();

		for (Data::Mesh::VertexIterator iter = datum.mesh->vBegin();
			 iter != datum.mesh->vEnd(); iter++)
		{
			boundary->add((*iter));
		}

		for (Data::Mesh::FrontElementIterator iter = datum.mesh->feBegin();
			 iter != datum.mesh->feEnd(); iter++)
		{
			boundary->add((*iter));
		}

		datum.boundary = boundary;

		this->data.push_back(datum);

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}
	else
	{
		wxMessageBox(_("Silhouette not identified"), _("Silhouette not identified"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::SurfacesIdentifier *tool)
{
	if (!tool)
	{
		return false;
	}

	Data::MeshList surfaces = tool->getSurfaces();

	if (!surfaces.empty())
	{
		wxMessageBox(_("Surfaces identified"), _("Surfaces identified"), wxOK | wxICON_EXCLAMATION);

		for (Data::MeshList::iterator iter = surfaces.begin();
			 iter != surfaces.end(); iter++)
		{
			MeshData datum;

			datum.mesh = (*iter);

			this->data.push_back(datum);
		}

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}
	else
	{
		wxMessageBox(_("Surfaces not identified"), _("Surfaces not identified"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::VisibilityIdentifier *tool)
{
	if (!tool)
	{
		return false;
	}

	Tools::VisibilityIdentifier::Visibility visibility = tool->getVisibility();

	if ((!visibility.vertices.empty()) || (!visibility.edges.empty()))
	{
		wxMessageBox(_("Visibility identified"), _("Visibility identified"), wxOK | wxICON_EXCLAMATION);

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}
	else
	{
		wxMessageBox(_("Visibility not identified"), _("Visibility not identified"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::FrontElementVisibilityIdentifier *tool)
{
	if (!tool)
	{
		return false;
	}

	Tools::VisibilityIdentifier::Visibility visibility = tool->getVisibility();

	if ((!visibility.vertices.empty()) || (!visibility.edges.empty()))
	{
		wxMessageBox(_("Visibility identified"), _("Visibility identified"), wxOK | wxICON_EXCLAMATION);

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}
	else
	{
		wxMessageBox(_("Visibility not identified"), _("Visibility not identified"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::VisibilityBasedVertexFinder *tool)
{
	if (!tool)
	{
		return false;
	}

	if (!tool->found())
	{
		wxMessageBox(_("No vertex or edge found"), _("No vertex or edge found"), wxOK | wxICON_ERROR);
	}
	else if (tool->foundVertex())
	{
	    wxString s;

	    s << _("Vertex ") << tool->getFoundVertex() << _(" found");

		wxMessageBox(s, _("Vertex found"), wxOK | wxICON_EXCLAMATION);

		this->ShowMesh(this->sMeshes->GetValue());
	}
	else
    {
        wxString s;

	    s << _("Edge ") << tool->getFoundFrontElement() << _(" found with parameter ") << tool->getFoundFrontElementParameter();

		wxMessageBox(s, _("Edge found"), wxOK | wxICON_EXCLAMATION);

		this->ShowMesh(this->sMeshes->GetValue());
    }

	return true;
}

#if USE_NUMERICAL
bool CRAbMesh::NumericalModule::ShowMessage(Tools::CovarianceMatrixFinder *tool)
{
	if (!tool)
	{
		return false;
	}

	Numerical::SymmetricMatrix *matrix = tool->getMatrix();

	if (matrix)
	{
		wxString s = _("Covariance matrix:\n");

		wxString str(matrix->text().c_str(), wxMBConvLibc());

		s << str;

		wxMessageBox(s, _("Covariance matrix"), wxOK | wxICON_EXCLAMATION);

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}
	else
	{
		wxMessageBox(_("Covariance matrix not found"), _("Covariance matrix not found"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::PrincipalComponentsAnalyzer *tool)
{
	if (!tool)
	{
		return false;
	}

	Data::Mesh *mesh = tool->getMesh();

	if (mesh)
	{
	    MeshData datum;
        datum.boundary = const_cast<Data::Boundary *>(tool->getBoundary());
        datum.mesh = mesh;

        this->data.push_back(datum);

        this->ShowMesh(static_cast<int>(this->data.size()) - 1, static_cast<int>(this->data.size()));

        wxString s = _("Mid point:\n");
		wxString str(tool->getMid()->text().c_str(), wxMBConvLibc());
		s << str;

		s << _("Eigenvalues:\n");
		wxString str2(tool->getEigenvalues()->text().c_str(), wxMBConvLibc());
		s << str2;

		s << _("Eigenvectors:\n");
		wxString str3(tool->getEigenvectors()->text().c_str(), wxMBConvLibc());
		s << str3;

		wxMessageBox(s, _("Eigenvalues and eigenvectors"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Principal components not found"), _("Principal components not found"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::HotellingTransformer *tool)
{
	if (!tool)
	{
		return false;
	}

	Numerical::BasisChangeMatrix *matrix = tool->getMatrix();

	if (matrix)
	{
	    this->ShowMesh(static_cast<int>(this->data.size()) - 1, static_cast<int>(this->data.size()));

        wxString s = _("Mid point:\n");
		wxString str(tool->getMid()->text().c_str(), wxMBConvLibc());
		s << str;

		s << _("Eigenvalues:\n");
		wxString str2(tool->getEigenvalues()->text().c_str(), wxMBConvLibc());
		s << str2;

		s << _("Eigenvectors:\n");
		wxString str3(tool->getEigenvectors()->text().c_str(), wxMBConvLibc());
		s << str3;

		s << _("Basis Change Matrix:\n");
		wxString str4(matrix->text().c_str(), wxMBConvLibc());
		s << str4;

		wxMessageBox(s, _("PCA Based Hotelling Transform"), wxOK | wxICON_EXCLAMATION);
	}
	else
	{
		wxMessageBox(_("Principal components not found"), _("Principal components not found"), wxOK | wxICON_ERROR);
	}

	return true;
}
#endif //#if USE_NUMERICAL

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshSplitter *tool)
{
	if (!tool)
	{
		return false;
	}

	Data::MeshList meshes = tool->getMeshes();

	if (tool->isDuplicate())
	{
		if (meshes.empty())
		{
			wxMessageBox(_("Mesh was not split"), _("Mesh was not split"), wxOK | wxICON_EXCLAMATION);
		}
		else
		{
			wxMessageBox(_("Mesh was split"), _("Mesh was split"), wxOK | wxICON_EXCLAMATION);

			Data::BoundaryList boundaries = tool->getBoundaries();

			UInt s = static_cast<UInt>(std::max(meshes.size(), boundaries.size()));

			Data::MeshList::iterator miter = meshes.begin();
			Data::BoundaryList::iterator biter = boundaries.begin();

			for (UInt i = 0; i < s; i++)
			{
				MeshData datum;

				datum.mesh = (miter != meshes.end()) ? (*miter) : NULL;
				datum.boundary = (biter != boundaries.end()) ? (*biter) : NULL;

				this->data.push_back(datum);

				if (miter != meshes.end())
				{
					miter++;
				}

				if (biter != boundaries.end())
				{
					biter++;
				}
			}

			this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
		}
	}
	else
	{
		if (meshes.empty())
		{
			wxMessageBox(_("Mesh was not colorized"), _("Mesh was not colorized"), wxOK | wxICON_EXCLAMATION);
		}
		else
		{
			while (!meshes.empty())
			{
				Data::Mesh *mesh = meshes.front();

				meshes.pop_front();

				mesh->clear();

				delete mesh;
			}

			this->frame->GetViewModule()->ClearDisplayLists();

			this->FillTree();

			this->canvas->Refresh(false);
			this->canvas->Update();
		}
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshColorizer *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isColorized())
	{
        wxMessageBox(_("Mesh elements were colorized"), _("Mesh elements were colorized"), wxOK | wxICON_EXCLAMATION);

        this->frame->GetViewModule()->ClearDisplayLists();

        this->canvas->Refresh(false);
        this->canvas->Update();
	}
	else
	{
        wxMessageBox(_("Mesh elements were not colorized"), _("Mesh elements were not colorized"), wxOK | wxICON_EXCLAMATION);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshMerger *tool)
{
	if (!tool)
	{
		return false;
	}

	wxMessageBox(_("Meshes merged"), _("Meshes merged"), wxOK | wxICON_EXCLAMATION);

	MeshData datum;
	datum.boundary = tool->getBoundary();
	datum.mesh = tool->getMesh();

	this->data.push_back(datum);

	this->ShowMesh(static_cast<int>(this->data.size()) - 1, static_cast<int>(this->data.size()));

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::VertexMerger *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->isMerged())
	{
		wxMessageBox(_("Vertices merged"), _("Vertices merged"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxMessageBox(_("No merging necessary"), _("No merging necessary"), wxOK | wxICON_EXCLAMATION);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::BoundaryCreator *tool)
{
	if (!tool)
	{
		return false;
	}

	if (tool->getIdManager())
	{
		delete tool->getIdManager();

		tool->setIdManager(NULL);
	}

	Data::Boundary *b = tool->getBoundary();

	if (b)
	{
		wxMessageBox(_("Boundary made"), _("Boundary made"), wxOK | wxICON_EXCLAMATION);

		this->frame->GetViewModule()->ClearDisplayLists();

		this->FillTree();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxMessageBox(_("Boundary could not be made"), _("Boundary could not be made"), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CRAbMesh::NumericalModule::ShowMessage(Tools::MeshTransformer *tool)
{
	if (!tool)
	{
		return false;
	}

	this->frame->GetViewModule()->ClearDisplayLists();

	if (this->cbAutoFitCamera->GetValue())
    {
        this->ForceFitCamera();
    }

	this->ForceFillTree(true);

	this->canvas->Refresh(false);
	this->canvas->Update();

	return true;
}

#if USE_TECGRAF
bool CRAbMesh::NumericalModule::ShowMessage(Tools::TecGrafBoundaryBuilder *tool)
{
	if (!tool)
	{
		return false;
	}

	if (!tool->getModel())
	{
		return true;
	}

	Data::BoundaryList boundaries = tool->getBoundaries();

	if (boundaries.empty())
	{
		wxMessageBox(_("Boundaries were not built"), _("Boundaries were not built"), wxOK | wxICON_ERROR);
	}
	else
	{
		wxMessageBox(_("Boundaries were built"), _("Boundaries were built"), wxOK | wxICON_EXCLAMATION);

		Data::MeshList meshes = tool->getMeshes();

		UInt s = static_cast<UInt>(std::max(meshes.size(), boundaries.size()));

		Data::MeshList::iterator miter = meshes.begin();
		Data::BoundaryList::iterator biter = boundaries.begin();

		for (UInt i = 0; i < s; i++)
		{
			MeshData datum;

			datum.mesh = (miter != meshes.end()) ? (*miter) : NULL;
			datum.boundary = (biter != boundaries.end()) ? (*biter) : NULL;

			this->data.push_back(datum);

			if (miter != meshes.end())
			{
				miter++;
			}

			if (biter != boundaries.end())
			{
				biter++;
			}
		}

		this->ShowMesh(this->sMeshes->GetValue(), static_cast<int>(this->data.size()));
	}

	return true;
}
#endif //#if USE_TECGRAF

bool CRAbMesh::NumericalModule::ExecuteForAllMeshes()
{
	return (this->rbMeshRange->GetSelection() == 1);
}*/

/*void CRAbMesh::NumericalModule::ForceShowMesh()
{
	this->ShowMesh(0, static_cast<int>(this->data.size()));

	if (this->GetDimension() == 3)
	{
		this->frame->GetViewModule()->SetDepthTestEnabled(true);
	}

	this->ForceFitCamera();
}*/

void CRAbMesh::NumericalModule::ForceFitCamera()
{
	if (this->GetCamera())
	{
		this->GetCamera()->SetFitted(false);
	}

	this->canvas->FitCamera();
}

/*void CRAbMesh::NumericalModule::ShowMesh(int show, int max)
{
	if (max >= 0)
	{
		this->sMeshes->SetRange(0, max);
	}

	this->sMeshes->SetValue(show);

	wxScrollEvent evt;

	this->OnMeshes(evt);
}

bool CRAbMesh::NumericalModule::EnsureExistenceOfMesh()
{
	if (this->data.empty())
	{
		return false;
	}

	if ((!this->data.back().mesh) &&
		((this->data.back().boundary) || (this->data.back().front)))
	{
#if USE_C__11
		Data::VertexHash vertices;
		Data::FrontElementHash frontElements;
#else
		Data::VertexSet vertices;
		Data::FrontElementSet frontElements;
#endif //#if USE_C__11

		Data::Boundary *boundary = this->data.back().boundary;
		Data::Front *front = this->data.back().front;

		if (boundary)
		{
			Data::VertexList vs = boundary->getVertices();
			Data::FrontElementList fels = boundary->getElements();

#if USE_C__11
			vertices.reserve(vertices.size() + vs.size());
			frontElements.reserve(frontElements.size() + fels.size());
#endif //#if USE_C__11

			vertices.insert(vs.begin(), vs.end());
			frontElements.insert(fels.begin(), fels.end());
		}

		if (front)
		{
			Data::VertexList vs = front->getVertices();
			Data::FrontElementList fels = front->getAllElements();

#if USE_C__11
			vertices.reserve(vertices.size() + vs.size());
			frontElements.reserve(frontElements.size() + fels.size());
#endif //#if USE_C__11

			vertices.insert(vs.begin(), vs.end());
			frontElements.insert(fels.begin(), fels.end());
		}

		if ((vertices.size() != 0) || (frontElements.size() != 0))
		{
			MJMesh::Mesh *mesh = new MJMesh::Mesh();

			mesh->setVertices(Data::VertexList(vertices.begin(), vertices.end()));
			mesh->setFrontElements(Data::FrontElementList(frontElements.begin(), frontElements.end()));

			this->data.back().mesh = mesh;
			this->data.back().createdMesh = true;

			return true;
		}
	}

	return false;
}*/

//#if USE_ONLY_MTOOLS
//bool CRAbMesh::NumericalModule::SaveFile(Data::Boundary */*boundary*/, Data::Mesh */*mesh*/, const std::string &/*filename*/)
//#else
//bool CRAbMesh::NumericalModule::SaveFile(Data::Boundary *boundary, Data::Mesh *mesh, const std::string &filename)
//#endif //#if USE_ONLY_MTOOLS
/*{
	bool saved = false;

	for (unsigned int i = 0; ((!saved) && (i < MODULES_COUNT)); i++)
	{
		MeshModule *module = dynamic_cast<MeshModule *>(this->frame->modules[moduleNames[i]]);

		if ((module) && (module != this->frame->currentModule))
		{
#if (!USE_ONLY_MTOOLS)
			Performer::MainDrive *drive = this->drive;

			this->drive = module->drive;

			module->MakeMainDrive();

			module->SetDimension(this->GetDimension());
			module->SetBoundary(boundary);
			module->SetMesh(mesh);

			saved = module->SaveFile(filename);

			module->SetBoundary(NULL);
			module->SetMesh(NULL);

			delete module->drive;

			module->drive = this->drive;

			this->drive = NULL;
			this->drive = drive;
#endif //#if (!USE_ONLY_MTOOLS)
		}
	}

	return saved;
}

wxString CRAbMesh::NumericalModule::Delete(Data::Drawable *d)
{
	wxString text;

	wxString str(d->text().c_str(), wxMBConvLibc());

	text << _("Unrecognized type for ") << str;

	return text;
}

wxString CRAbMesh::NumericalModule::Delete(Data::Vertex *v)
{
	wxString text;

	wxString str(v->text().c_str(), wxMBConvLibc());

	text << _("Deleted vertex ") << str;

	Data::FrontElementList fadjs;
	Data::ElementList adjs;

	if (this->GetMesh())
	{
		fadjs = this->GetMesh()->findAdjacentFrontElements(v);
		adjs = this->GetMesh()->adjacency(v);

		(*this->datum).mesh->remove(v);

		for (Data::FrontElementList::iterator iter2 = fadjs.begin();
			 iter2 != fadjs.end(); iter2++)
		{
			Data::FrontElement *fel = (*iter2);

			wxString str(fel->text().c_str(), wxMBConvLibc());

			text << _("\nDeleted front element ");

			text << str;

			if (this->GetMesh()->have(fel))
			{
				(*this->datum).mesh->remove(fel);
			}

			if ((this->GetBoundary()) && (this->GetBoundary()->have(fel)))
			{
				(*this->datum).boundary->remove(fel);
			}

			if ((this->GetFront()) && (this->GetFront()->have(fel)))
			{
				(*this->datum).front->remove(fel);
			}
		}

		for (Data::ElementList::iterator iter2 = adjs.begin();
			 iter2 != adjs.end(); iter2++)
		{
			Data::Element *e = (*iter2);

			wxString str(e->text().c_str(), wxMBConvLibc());

			text << _("\nDeleted element ");

			text << str;

			if (this->GetMesh()->have(e))
			{
				(*this->datum).mesh->remove(e);
			}
		}
	}

	if ((this->GetBoundary()) && (this->GetBoundary()->have(v)))
	{
		(*this->datum).boundary->remove(v);

		if (fadjs.empty())
		{
			fadjs = this->GetBoundary()->adjacency(v);

			for (Data::FrontElementList::iterator iter2 = fadjs.begin();
				 iter2 != fadjs.end(); iter2++)
			{
				Data::FrontElement *fel = (*iter2);

				wxString str(fel->text().c_str(), wxMBConvLibc());

				text << _("\nDeleted front element ");

				text << str;

				if ((this->GetMesh()) && (this->GetMesh()->have(fel)))
				{
					(*this->datum).mesh->remove(fel);
				}

				if (this->GetBoundary()->have(fel))
				{
					(*this->datum).boundary->remove(fel);
				}

				if ((this->GetFront()) && (this->GetFront()->have(fel)))
				{
					(*this->datum).front->remove(fel);
				}
			}
		}
	}

	if ((this->GetFront()) && (this->GetFront()->have(v)))
	{
		(*this->datum).front->remove(v);
	}

	for (Data::FrontElementList::iterator iter = fadjs.begin();
		 iter != fadjs.end(); iter++)
	{
		this->picked.remove((*iter));

		delete (*iter);
	}

	for (Data::ElementList::iterator iter = adjs.begin();
		 iter != adjs.end(); iter++)
	{
		this->picked.remove((*iter));

		delete (*iter);
	}

	delete v;

	return text;
}

wxString CRAbMesh::NumericalModule::Delete(Data::FrontElement *fel)
{
	wxString text;

	wxString str(fel->text().c_str(), wxMBConvLibc());

	text << _("Deleted front element ") << str;

	Data::VertexList vertices;

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		vertices.push_back(fel->getVertex(i));
	}

	if ((this->GetMesh()) && (this->GetMesh()->have(fel)))
	{
		(*this->datum).mesh->remove(fel);
	}

	if ((this->GetBoundary()) && (this->GetBoundary()->have(fel)))
	{
		(*this->datum).boundary->remove(fel);
	}

	if ((this->GetFront()) && (this->GetFront()->have(fel)))
	{
		(*this->datum).front->remove(fel);
	}

	while (!vertices.empty())
	{
		Data::Vertex *v = vertices.front();

		vertices.pop_front();

		Data::FrontElementList fadjs;
		Data::ElementList adjs;

		if (this->GetMesh())
		{
			fadjs = this->GetMesh()->findAdjacentFrontElements(v);
			adjs = this->GetMesh()->adjacency(v);
		}
		else if (this->GetBoundary())
		{
			fadjs = this->GetBoundary()->adjacency(v);
		}

		if (((fadjs.size() > 1) || (!adjs.empty())) ||
			((fadjs.size() == 1) && (fadjs.front() != fel)))
		{
			continue;
		}

		wxString str(v->text().c_str(), wxMBConvLibc());

		text << _("\nDeleted vertex ") << str;

		fel->replace(v, NULL);

		if ((this->GetMesh()) && (this->GetMesh()->have(v)))
		{
			(*this->datum).mesh->remove(v);
		}

		if ((this->GetBoundary()) && (this->GetBoundary()->have(v)))
		{
			(*this->datum).boundary->remove(v);
		}

		if ((this->GetFront()) && (this->GetFront()->have(v)))
		{
			(*this->datum).front->remove(v);
		}

		this->picked.remove(v);

		delete v;
	}

	delete fel;

	return text;
}

wxString CRAbMesh::NumericalModule::Delete(Data::Element *e)
{
	wxString text;

	wxString str(e->text().c_str(), wxMBConvLibc());

	text << _("Deleted element ") << str;

	Data::VertexList vertices;
	Data::FrontElementList fels;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		vertices.push_back(e->getVertex(i));
	}

	fels = this->GetMesh()->findAdjacentFrontElements(e);

	for (Data::FrontElementList::iterator fiter = fels.begin();
		 fiter != fels.end(); fiter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*fiter));

		fel->remove(static_cast<MJMesh::Element *>(e));
	}

	if ((this->GetMesh()) && (this->GetMesh()->have(e)))
	{
		(*this->datum).mesh->remove(e);
	}

	while (!vertices.empty())
	{
		Data::Vertex *v = vertices.front();

		vertices.pop_front();

		Data::FrontElementList fadjs;
		Data::ElementList adjs;

		if (this->GetMesh())
		{
			fadjs = this->GetMesh()->findAdjacentFrontElements(v);
			adjs = this->GetMesh()->adjacency(v);
		}
		else if (this->GetBoundary())
		{
			fadjs = this->GetBoundary()->adjacency(v);
		}

		if (((adjs.size() > 1) || (!fadjs.empty())) ||
			((adjs.size() == 1) && (adjs.front() != e)))
		{
			continue;
		}

		wxString str(v->text().c_str(), wxMBConvLibc());

		text << _("\nDeleted vertex ") << str;

		e->replace(v, NULL);

		if ((this->GetMesh()) && (this->GetMesh()->have(v)))
		{
			(*this->datum).mesh->remove(v);
		}

		if ((this->GetBoundary()) && (this->GetBoundary()->have(v)))
		{
			(*this->datum).boundary->remove(v);
		}

		if ((this->GetFront()) && (this->GetFront()->have(v)))
		{
			(*this->datum).front->remove(v);
		}

		this->picked.remove(v);

		delete v;
	}

	delete e;

	return text;
}*/

void CRAbMesh::NumericalModule::MakeMesh(Data::Point3DObjVector points, bool edges, bool ordered)
{
    if (points.empty())
    {
        return;
    }

    if (this->mesh)
    {
        delete this->mesh;
    }

    this->mesh = new MJMesh::Mesh();

    if (ordered)
    {
        for (UInt i = 0; i < points.size(); i++)
        {
            UInt min = i;

            for (UInt j = i + 1; j < points.size(); j++)
            {
                if (points[j].getCoord(0) < points[min].getCoord(0))
                {
                    min = j;
                }
            }

            if (min == i)
            {
                continue;
            }

            Data::Point3D pt = points[min];
            points[min] = points[i];
            points[i] = pt;
        }
    }

    MJMesh::Vertex *oldv = NULL;

    for (UInt i = 0; i < points.size(); i++)
    {
        Data::Point3D pt = points[i];
        MJMesh::Vertex *v = new MJMesh::Vertex(pt.getCoord(0), pt.getCoord(1), pt.getCoord(2), i+1);
        this->mesh->add(v);

        if ((edges) && (oldv))
        {
            MJMesh::FrontElementSurface *fel = new MJMesh::FrontElementSurface(oldv, v, i);
            this->mesh->add(fel);
        }

        oldv = v;
    }
}

/*#if USE_TECGRAF
void CRAbMesh::NumericalModule::OnTreeMiddleClick(wxTreeEvent &event)
{
	wxTreeItemId item = event.GetItem();

	if (!item.IsOk())
    {
        return;
    }

	if ((this->IsEnabled(REGIONS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[REGIONS]))
	{
		const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

		Data::Box s = static_cast<const TecGraf::Region *>(d)->box();

		this->canvas->Center(&s);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else if ((this->IsEnabled(SURFACES, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[SURFACES]))
	{
	    const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

		Data::Box *s = static_cast<const TecGraf::Surface *>(d)->box();

		this->canvas->Center(s);

		delete s;

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		MeshModule::OnTreeMiddleClick(event);
	}
}
#endif //#if USE_TECGRAF*/

#endif //#if ((USE_GUI) && (USE_NUMERICAL) && (USE_EXPRTK))
