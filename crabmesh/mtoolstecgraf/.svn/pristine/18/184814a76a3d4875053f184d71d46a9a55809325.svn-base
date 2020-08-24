#include "CRAbMesh/SplinesModule.h"

#if USE_GUI
#if USE_SPLINES

#include <wx/gbsizer.h>
#include <wx/choicebk.h>
#include <wx/propgrid/advprops.h>

#include "Data/Point3D.h"
#include "Data/Box.h"
#include "Data/Spline/LagrangeCurve.h"
#include "Data/Spline/CubicLagrangeCurve.h"
#include "Data/Spline/LagrangeChebyshevCurve.h"
#include "Data/Spline/HermiteCurve.h"
#include "Data/Spline/C1HermiteCurve.h"
#include "Data/Spline/BezierCurve.h"
#include "Data/Spline/CubicBezierCurve.h"
#include "Data/Spline/C1CubicBezierCurve.h"
#include "Data/Spline/KochanekBartelsCurve.h"
#include "Data/Spline/CardinalCurve.h"
#include "Data/Spline/CatmullRomCurve.h"
#include "Data/Spline/BCurve.h"
#include "Data/Spline/CubicBCurve.h"
#include "Data/Spline/NURBCurve.h"
//#include "Data/Spline/LagrangeSurface.h"
//#include "Data/Spline/BicubicLagrangeSurface.h"
//#include "Data/Spline/HermiteSurface.h"
//#include "Data/Spline/C1HermiteSurface.h"
#include "Data/Spline/BezierSurface.h"
#include "Data/Spline/BicubicBezierSurface.h"
#include "Data/Spline/C1BicubicBezierSurface.h"
//#include "Data/Spline/KochanekBartelsSurface.h"
//#include "Data/Spline/CardinalSurface.h"
//#include "Data/Spline/CatmullRomSurface.h"
//#include "Data/Spline/BSurface.h"
//#include "Data/Spline/BicubicBSurface.h"
//#include "Data/Spline/NURBSurface.h"
#include "CRAbMesh/TreeItemData.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"

using namespace CRAbMesh;

CRAbMesh::SplinesModule::SplinesModule(Frame *frame, Canvas *canvas) :
	CRAbMesh::MeshModule(frame, canvas)
{
	CURVES = "Curves";
	CURVES_POINTS = "Curves points";
	SURFACES = "Surfaces";
	SURFACES_POINTS = "Surfaces points";

	DRAW_CURVES = "Draw curves";
	DRAW_SURFACES = "Draw surfaces";
	DRAW_CONTROL_POINTS = "Draw control points";
	DRAW_CONTROL_POLYGON = "Draw control polygon";
#if USE_NUMERICAL
	DRAW_SPACED_POINTS = "Draw spaced points";
	DRAW_BASES = "Draw local bases";
#endif //#if USE_NUMERICAL

	this->InsertOption(CURVES,               TREE_CELLS,           TYPE_TREE_ITEM);
	this->InsertOption(CURVES_POINTS,        CURVES_POINTS,        TYPE_TREE_ITEM);
	this->InsertOption(SURFACES,             SURFACES,             TYPE_TREE_ITEM);
	this->InsertOption(SURFACES_POINTS,      SURFACES_POINTS,      TYPE_TREE_ITEM);
	this->InsertOption(DRAW_CURVES,          DRAW_MESH,            TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_SURFACES,        DRAW_CURVES,          TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_CONTROL_POINTS,  DRAW_SURFACES,        TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_CONTROL_POLYGON, DRAW_CONTROL_POINTS,  TYPE_MENU_AND_CHECK_ITEM);
#if USE_NUMERICAL
	this->InsertOption(DRAW_SPACED_POINTS,   DRAW_CONTROL_POLYGON, TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_BASES,           DRAW_SPACED_POINTS,   TYPE_MENU_AND_CHECK_ITEM);
#endif //#if USE_NUMERICAL

	this->SetConfiguration(DRAW_CURVES,          true,  false);
	this->SetConfiguration(DRAW_SURFACES,        true,  false);
	this->SetConfiguration(DRAW_CONTROL_POINTS,  true,  false);
	this->SetConfiguration(DRAW_CONTROL_POLYGON, false, false);
#if USE_NUMERICAL
	this->SetConfiguration(DRAW_SPACED_POINTS,   true,  false);
	this->SetConfiguration(DRAW_BASES,           false, false);
#endif //#if USE_NUMERICAL

	this->SetOption(VERTICES, false);
	this->SetOption(ELEMENTS, false);
	this->SetOption(FRONT_ELEMENTS, false);
	this->SetOption(TREE_CELLS, false);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, false, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, false, false);

	this->SetOption(DRAW_NORMAL, false, false);
	this->SetOption(DRAW_DISCRETIZATION, false, false);

	this->SetOption(DRAW_BOUNDARY, false, false);
	this->SetOption(DRAW_TREE, false, false);
	this->SetOption(DRAW_FRONT, false, false);
	this->SetOption(DRAW_MESH, false, false);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	this->SetOption(CURVES,               true);
	this->SetOption(CURVES_POINTS,        true);
	this->SetOption(SURFACES,             true);
	this->SetOption(SURFACES_POINTS,      true);
	this->SetOption(DRAW_CURVES,          true, true);
	this->SetOption(DRAW_SURFACES,        true, true);
	this->SetOption(DRAW_CONTROL_POINTS,  true, true);
	this->SetOption(DRAW_CONTROL_POLYGON, true, true);
#if USE_NUMERICAL
	this->SetOption(DRAW_SPACED_POINTS,   true, true);
	this->SetOption(DRAW_BASES,           true, true);
#endif //#if USE_NUMERICAL

	this->splineNames[LAGRANGE] = "Lagrange";
	this->splineNames[CUBIC_LAGRANGE] = "Cubic Lagrange";
#if USE_NUMERICAL
	this->splineNames[LAGRANGE_CHEBYSHEV] = "Lagrange Chebyshev";
#endif //#if USE_NUMERICAL
	this->splineNames[HERMITE] = "Hermite";
	this->splineNames[C1_HERMITE] = "C1 Hermite";
	this->splineNames[BEZIER] = "Bezier";
	this->splineNames[CUBIC_BEZIER] = "Cubic Bezier";
	this->splineNames[C1_CUBIC_BEZIER] = "C1 Cubic Bezier";
	this->splineNames[KOCHANEK_BARTELS] = "Kochanek-Bartels";
	this->splineNames[CARDINAL] = "Cardinal";
	this->splineNames[CATMULL_ROM] = "Catmull-Rom";
	this->splineNames[B] = "B Splines";
	this->splineNames[CUBIC_B] = "Cubic B Splines";
	this->splineNames[NURB] = "NURBS";

	//if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}

	this->setCurve(this->curves.end());
	this->setSurface(this->surfaces.end());

	this->addingPoints = false;
}

CRAbMesh::SplinesModule::~SplinesModule()
{

}

bool CRAbMesh::SplinesModule::LoadFile(const std::string &/*filename*/)
{
	return false;
}

bool CRAbMesh::SplinesModule::SaveFile(const std::string &/*filename*/)
{
	return false;
}

wxString CRAbMesh::SplinesModule::Name() const
{
	return _("Splines");
}

wxMenu *CRAbMesh::SplinesModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("&Configuration");
		break;
	//case 1:
	//	title = _("&Picked");
	//	break;
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::SplinesModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::SplinesModule::GetPanel(unsigned int i, wxString &title) const
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

void CRAbMesh::SplinesModule::SetParameters(bool /*firstTime*/)
{

}

void CRAbMesh::SplinesModule::Execute()
{

}

void CRAbMesh::SplinesModule::Ready(bool /*clearing*/, bool /*pending*/)
{

}

void CRAbMesh::SplinesModule::Clear(bool onlyDrive, bool destroying)
{
	this->ClearTree();

	while (!this->curves.empty())
	{
		delete this->curves.front();

		this->curves.pop_front();
	}

	while (!this->surfaces.empty())
	{
		delete this->surfaces.front();

		this->surfaces.pop_front();
	}

	this->setCurve(this->curves.end());
	this->setSurface(this->surfaces.end());

	MeshModule::Clear(onlyDrive, destroying);
}

void CRAbMesh::SplinesModule::SetGUIState()
{

}

void CRAbMesh::SplinesModule::SetCurveDrawable(bool curveDrawable)
{
	this->SetConfiguration(DRAW_CURVES, curveDrawable);
}

bool CRAbMesh::SplinesModule::IsCurveDrawable() const
{
	return this->IsConfiguration(DRAW_CURVES);
}

void CRAbMesh::SplinesModule::SetSurfaceDrawable(bool surfaceDrawable)
{
	this->SetConfiguration(DRAW_SURFACES, surfaceDrawable);
}

bool CRAbMesh::SplinesModule::IsSurfaceDrawable() const
{
	return this->IsConfiguration(DRAW_SURFACES);
}

void CRAbMesh::SplinesModule::SetControlPointDrawable(bool controlPointDrawable)
{
	this->SetConfiguration(DRAW_CONTROL_POINTS, controlPointDrawable);
}

bool CRAbMesh::SplinesModule::IsControlPointDrawable() const
{
	return this->IsConfiguration(DRAW_CONTROL_POINTS);
}

void CRAbMesh::SplinesModule::SetControlPolygonDrawable(bool controlPolygonDrawable)
{
	this->SetConfiguration(DRAW_CONTROL_POLYGON, controlPolygonDrawable);
}

bool CRAbMesh::SplinesModule::IsControlPolygonDrawable() const
{
	return this->IsConfiguration(DRAW_CONTROL_POLYGON);
}

#if USE_NUMERICAL
void CRAbMesh::SplinesModule::SetSpacedPointDrawable(bool spacedPointDrawable)
{
	this->SetConfiguration(DRAW_SPACED_POINTS, spacedPointDrawable);
}

bool CRAbMesh::SplinesModule::IsSpacedPointDrawable() const
{
	return this->IsConfiguration(DRAW_SPACED_POINTS);
}

void CRAbMesh::SplinesModule::SetBaseDrawable(bool baseDrawable)
{
	this->SetConfiguration(DRAW_BASES, baseDrawable);
}

bool CRAbMesh::SplinesModule::IsBaseDrawable() const
{
	return this->IsConfiguration(DRAW_BASES);
}
#endif //#if USE_NUMERICAL

bool CRAbMesh::SplinesModule::IsDrawable() const
{
	return (MeshModule::IsDrawable()
			|| this->IsCurveDrawable()
			|| this->IsSurfaceDrawable()
			|| this->IsControlPointDrawable()
			|| this->IsControlPolygonDrawable()
#if USE_NUMERICAL
			|| this->IsSpacedPointDrawable()
			|| this->IsBaseDrawable()
#endif //#if USE_NUMERICAL
            );
}

void CRAbMesh::SplinesModule::MakeMainDrive()
{

}

void CRAbMesh::SplinesModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
	//this->menus.push_back(this->MakePickedMenu());
}

void CRAbMesh::SplinesModule::MakeToolbars()
{

}

void CRAbMesh::SplinesModule::MakePanels()
{
	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
}

wxPanel *CRAbMesh::SplinesModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	wxArrayString choices;

	for (UInt i = 0; i < NUM_SPLINES; i++)
	{
		wxString str(this->splineNames[i].c_str(), wxMBConvLibc());

		choices.Add(str);
	}

	this->cCurves = new wxChoice(pConfiguration, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	this->cCurves->SetSelection(0);

	this->bNewCurve      = new wxButton(pConfiguration, wxID_ANY, _("New curve"));
	this->bNewCurvePoint = new wxToggleButton(pConfiguration, wxID_ANY, _("Add point"));

	this->pgCurve = new wxPropertyGrid(pConfiguration, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);

	this->cSurfaces = new wxChoice(pConfiguration, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	this->cSurfaces->SetSelection(0);

	this->bNewSurface       = new wxButton(pConfiguration, wxID_ANY, _("New surface"));
	this->bNewSurfacePoint  = new wxToggleButton(pConfiguration, wxID_ANY, _("Add point"));
	this->scNewSurfacePoint = new wxSpinCtrl(pConfiguration, wxID_ANY);

	this->pgSurface = new wxPropertyGrid(pConfiguration, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);

	this->bClear = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

	wxStaticBoxSizer *sbsCurves = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Curves"));
    sbsCurves->Add(this->cCurves,        0, wxEXPAND | wxALL, 0);
    sbsCurves->Add(this->bNewCurve,      0, wxEXPAND | wxALL, 0);
    sbsCurves->Add(this->bNewCurvePoint, 0, wxEXPAND | wxALL, 0);
	sbsCurves->Add(this->pgCurve,        0, wxEXPAND | wxALL, 1);

    wxStaticBoxSizer *sbsSurfaces = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Surfaces"));
    sbsSurfaces->Add(this->cSurfaces,         0, wxEXPAND | wxALL, 0);
    sbsSurfaces->Add(this->bNewSurface,       0, wxEXPAND | wxALL, 0);
    sbsSurfaces->Add(this->bNewSurfacePoint,  0, wxEXPAND | wxALL, 0);
    sbsSurfaces->Add(this->scNewSurfacePoint, 0, wxEXPAND | wxALL, 0);
	sbsSurfaces->Add(this->pgSurface,         0, wxEXPAND | wxALL, 1);

	wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
	bsConf->Add(sbsCurves,    0, wxEXPAND | wxALL, 0);
	bsConf->Add(sbsSurfaces,  0, wxEXPAND | wxALL, 0);
	bsConf->Add(this->bClear, 0, wxEXPAND | wxALL, 0);
	bsConf->Add(sConf,        0, wxEXPAND | wxALL, 0);
    pConfiguration->SetSizer(bsConf, false);

    this->Connect(this->bNewCurve->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SplinesModule::OnNewCurve));
    this->Connect(this->bNewCurvePoint->GetId(), wxEVT_TOGGLEBUTTON, wxCommandEventHandler(SplinesModule::OnNewCurvePoint));
    this->Connect(this->pgCurve->GetId(),        wxEVT_PG_CHANGED,   wxPropertyGridEventHandler(SplinesModule::OnCurvePropertyChanged));

	this->Connect(this->bNewSurface->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SplinesModule::OnNewSurface));
	this->Connect(this->bNewSurfacePoint->GetId(), wxEVT_TOGGLEBUTTON, wxCommandEventHandler(SplinesModule::OnNewSurfacePoint));
	this->Connect(this->pgSurface->GetId(),        wxEVT_PG_CHANGED,   wxPropertyGridEventHandler(SplinesModule::OnSurfacePropertyChanged));

	this->Connect(this->bClear->GetId(),           wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SplinesModule::OnClear));

	return pConfiguration;
}

void CRAbMesh::SplinesModule::OnNewCurve(wxCommandEvent &WXUNUSED(event))
{
	int selection = this->cCurves->GetSelection();

	if ((selection == wxNOT_FOUND) || (selection < 0))
	{
		return;
	}

	Data::Spline::Curve *curve = this->MakeCurve(selection);

	if (curve)
	{
		Real r, g, b;

		Data::pallete(static_cast<UInt>(this->curves.size()), r, g, b);

		curve->setColor(r, g, b);

		this->curves.push_back(curve);

		this->AddTreeItem(curve);

		this->setCurve(--this->curves.end());

		this->GetCamera()->SetFitted(false);
	}
}

void CRAbMesh::SplinesModule::OnNewSurface(wxCommandEvent &WXUNUSED(event))
{
	int selection = this->cSurfaces->GetSelection();

	if ((selection == wxNOT_FOUND) || (selection < 0))
	{
		return;
	}

	Data::Spline::Surface *surface = this->MakeSurface(selection);

	if (surface)
	{
		Real r, g, b;

		Data::pallete(static_cast<UInt>(this->curves.size()), r, g, b);

		surface->setColor(r, g, b);

		this->surfaces.push_back(surface);

		this->AddTreeItem(surface);

		this->setSurface(--this->surfaces.end());

		this->GetCamera()->SetFitted(false);
	}
}

void CRAbMesh::SplinesModule::OnNewCurvePoint(wxCommandEvent &WXUNUSED(event))
{
	if (this->curve == this->curves.end())
	{
		this->bNewCurvePoint->SetValue(false);

		return;
	}

	int selection = this->cCurves->GetSelection();

	if ((selection == wxNOT_FOUND) ||
		(selection < 0) ||
		(!this->bNewCurvePoint->GetValue()) ||
		(this->SplineType((*this->curve)) != selection))
	{
		this->addingPoints = false;

		this->bNewCurve->Enable(true);
		this->bNewSurface->Enable(true);

		this->bNewSurfacePoint->Enable(true);

		this->cCurves->Enable(true);
		this->cSurfaces->Enable(true);

		this->bClear->Enable(true);
	}
	else
	{
		this->addingPoints = true;

		this->bNewCurve->Enable(false);
		this->bNewSurface->Enable(false);

		this->bNewSurfacePoint->Enable(false);

		this->cCurves->Enable(false);
		this->cSurfaces->Enable(false);

		this->bClear->Enable(false);
	}
}

void CRAbMesh::SplinesModule::OnNewSurfacePoint(wxCommandEvent &WXUNUSED(event))
{
	if (this->surface == this->surfaces.end())
	{
		this->bNewSurfacePoint->SetValue(false);

		return;
	}

	int selection = this->cSurfaces->GetSelection();

	if ((selection == wxNOT_FOUND) ||
		(selection < 0) ||
		(!this->bNewSurfacePoint->GetValue()) ||
		(this->SplineType((*this->surface)) != selection))
	{
		this->addingPoints = false;

		this->bNewCurve->Enable(true);
		this->bNewSurface->Enable(true);

		this->bNewCurvePoint->Enable(true);

		this->cCurves->Enable(true);
		this->cSurfaces->Enable(true);

		this->bClear->Enable(true);
	}
	else
	{
		this->addingPoints = true;

		this->bNewCurve->Enable(false);
		this->bNewSurface->Enable(false);

		this->bNewCurvePoint->Enable(false);

		this->cCurves->Enable(false);
		this->cSurfaces->Enable(false);

		this->bClear->Enable(false);
	}
}

void CRAbMesh::SplinesModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	int ok = wxMessageBox(_("Are you sure you want to clear the curves and surfaces?"),
		_("Clearing"), wxYES_NO | wxICON_QUESTION);

	if (ok == wxYES)
	{
		this->Clear();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

void CRAbMesh::SplinesModule::OnTreeSelectionChanged(wxTreeEvent &event)
{
	wxTreeItemId oldItem = event.GetOldItem();
    wxTreeItemId currentItem = event.GetItem();

    bool changed = false;

    if (oldItem.IsOk())
    {
    	if ((this->IsEnabled(CURVES, TREE_ITEM)) &&
			(this->tree->GetItemParent(oldItem) == (*this->tiItems.find(CURVES)).second))
		{
			if (this->curve != this->curves.end())
			{
				this->setCurve(this->curves.end());

				changed = true;
			}
		}
		else if ((this->IsEnabled(SURFACES, TREE_ITEM)) &&
			(this->tree->GetItemParent(oldItem) == (*this->tiItems.find(SURFACES)).second))
		{
			if (this->surface != this->surfaces.end())
			{
				this->setSurface(this->surfaces.end());

				changed = true;
			}
		}
		/*else if ((this->IsEnabled(CURVES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(oldItem) == (*this->tiItems.find(CURVES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(oldItem));

			drawable = data->GetItem();
		}
		else if ((this->IsEnabled(SURFACES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(oldItem) == (*this->tiItems.find(SURFACES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(oldItem));

			drawable = data->GetItem();
		}*/
    }

    if (currentItem.IsOk())
    {
    	if ((this->IsEnabled(CURVES, TREE_ITEM)) &&
			(this->tree->GetItemParent(currentItem) == (*this->tiItems.find(CURVES)).second))
		{
			bool set = false;

			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(currentItem));

			Data::Spline::Curve *curve = dynamic_cast<Data::Spline::Curve *>(data->GetItem());

			for (Data::CurveList::iterator iter = this->curves.begin();
				 iter != this->curves.end(); iter++)
			{
				if ((*iter) == curve)
				{
					this->setCurve(iter);

					set = true;

					changed = true;

					break;
				}
			}

			if (!set)
			{
				this->setCurve(this->curves.end());

				changed = true;
			}
		}
		else if ((this->IsEnabled(SURFACES, TREE_ITEM)) &&
			(this->tree->GetItemParent(currentItem) == (*this->tiItems.find(SURFACES)).second))
		{
			bool set = false;

			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(currentItem));

			Data::Spline::Surface *surface = dynamic_cast<Data::Spline::Surface *>(data->GetItem());

			for (Data::SurfaceList::iterator iter = this->surfaces.begin();
				 iter != this->surfaces.end(); iter++)
			{
				if ((*iter) == surface)
				{
					this->setSurface(iter);

					set = true;

					changed = true;

					break;
				}
			}

			if (!set)
			{
				this->setSurface(this->surfaces.end());

				changed = true;
			}
		}
		/*else if ((this->IsEnabled(CURVES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(currentItem) == (*this->tiItems.find(CURVES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(currentItem));

			drawable = data->GetItem();
		}
		else if ((this->IsEnabled(SURFACES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(currentItem) == (*this->tiItems.find(SURFACES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(currentItem));

			drawable = data->GetItem();
		}*/
    }

	if (changed)
	{
		this->frame->GetViewModule()->ClearDisplayLists();
	}

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::SplinesModule::OnCurvePropertyChanged(wxPropertyGridEvent &event)
{
	wxPGProperty *property = event.GetProperty();

	if (!property)
	{
		return;
	}

	wxAny value = property->GetValue();

	if (value.IsNull())
	{
		return;
	}

	bool refresh = false;

	wxPropertyCategory *category = this->pgCurve->GetPropertyCategory(property);

	if ((category) && (category->GetName() == _("Points")))
	{
		wxPGProperty *mainProperty = event.GetMainParent();

		if (mainProperty == property)
		{
			return;
		}

		wxString name = property->GetName();

		wxString point = name.Before('.');
		wxString dimension = name.After('.');

		unsigned long int pt, dim;

		point.ToULong(&pt);

		if (dimension == _("X"))
		{
			dim = 0;
		}
		else if (dimension == _("Y"))
		{
			dim = 1;
		}
		else if (dimension == _("Z"))
		{
			dim = 2;
		}
		else
		{
			dimension.ToULong(&dim);
		}

		float val;

		if (value.GetAs(&val))
		{
			Data::Point3D point = (*this->curve)->pt3d((*this->curve)->getPoint(static_cast<UInt>(pt)));

			point.setCoord(static_cast<UInt>(dim), static_cast<Real>(val));

			(*this->curve)->move(static_cast<UInt>(pt), point.getX(), point.getY(), point.getZ());

			this->ResetCurvePoints();

			refresh = true;
		}
	}
	else if (category)
	{
		enum SplineTypes type = this->SplineType((*this->curve));

		if (type < NUM_SPLINES)
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			if (category->GetName() != name)
			{
				type = NUM_SPLINES;
			}
		}

		switch (type)
		{
		case KOCHANEK_BARTELS:
			{
				if (property->GetName() == _("Tension"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsCurve *>((*this->curve))->setTension(static_cast<Real>(val));

						refresh = true;
					}
				}
				else if (property->GetName() == _("Bias"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsCurve *>((*this->curve))->setBias(static_cast<Real>(val));

						refresh = true;
					}
				}
				else if (property->GetName() == _("Continuity"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsCurve *>((*this->curve))->setContinuity(static_cast<Real>(val));

						refresh = true;
					}
				}

				break;
			}
		case CARDINAL:
			{
				if (property->GetName() == _("Tension"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::CardinalCurve *>((*this->curve))->setTension(static_cast<Real>(val));

						refresh = true;
					}
				}

				break;
			}
		case B:
			{
				if (property->GetName() == _("Degree"))
				{
					int val;

					if (value.GetAs(&val))
					{
						if (val > 0)
						{
							static_cast<Data::Spline::BCurve *>((*this->curve))->setDegree(val);

							this->RebuildCurveKnotProperties();

							refresh = true;
						}
						else
						{
							this->pgCurve->SetPropertyValue(property, static_cast<int>(static_cast<Data::Spline::BCurve *>((*this->curve))->getDegree()));
						}
					}
				}
				else
				{
					wxPGProperty *mainProperty = event.GetMainParent();

					if ((mainProperty != property) &&
						(mainProperty->GetName() == _("Knot vector")))
					{
						wxString name = property->GetName();
						wxString index = name.After('.');

						unsigned long int ind;

						index.ToULong(&ind);

						float val;

						if (value.GetAs(&val))
						{
							static_cast<Data::Spline::BCurve *>((*this->curve))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

							this->RebuildCurveKnotProperties();

							refresh = true;
						}
					}
				}

				break;
			}
		case CUBIC_B:
			{
				wxPGProperty *mainProperty = event.GetMainParent();

				if ((mainProperty) &&
					(mainProperty != property) &&
					(mainProperty->GetName() == _("Knot vector")))
				{
					wxString name = property->GetName();
					wxString index = name.After('.');

					unsigned long int ind;

					index.ToULong(&ind);

					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::CubicBCurve *>((*this->curve))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

						this->RebuildCurveKnotProperties();

						refresh = true;
					}
				}

				break;
			}
		case NURB:
			{
				if (property->GetName() == _("Degree"))
				{
					int val;

					if (value.GetAs(&val))
					{
						if (val > 0)
						{
							static_cast<Data::Spline::NURBCurve *>((*this->curve))->setDegree(val);

							this->RebuildCurveKnotProperties();

							refresh = true;
						}
						else
						{
							this->pgCurve->SetPropertyValue(property, static_cast<int>(static_cast<Data::Spline::NURBCurve *>((*this->curve))->getDegree()));
						}
					}
				}
				else
				{
					wxPGProperty *mainProperty = event.GetMainParent();

					if ((mainProperty) &&
						(mainProperty != property))
					{
						if (mainProperty->GetName() == _("Knot vector"))
						{
							wxString name = property->GetName();
							wxString index = name.After('.');

							unsigned long int ind;

							index.ToULong(&ind);

							float val;

							if (value.GetAs(&val))
							{
								static_cast<Data::Spline::NURBCurve *>((*this->curve))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

								this->RebuildCurveKnotProperties();

								refresh = true;
							}
						}
						else if (mainProperty->GetName() == _("Weights"))
						{
							wxString name = property->GetName();
							wxString index = name.After('.');

							unsigned long int ind;

							index.ToULong(&ind);

							float val;

							if (value.GetAs(&val))
							{
								static_cast<Data::Spline::NURBCurve *>((*this->curve))->setWeight(static_cast<UInt>(ind), static_cast<Real>(val));

								refresh = true;
							}
						}
					}
				}

				break;
			}
		default:
			break;
		}
	}
	else if (property->GetName() == _("Discretization"))
	{
		int val;

		if (value.GetAs(&val))
		{
			if (val > 0)
			{
				(*this->curve)->setDiscretization(val);

				refresh = true;
			}
			else
			{
				this->pgCurve->SetPropertyValue(property, static_cast<int>((*this->curve)->getDiscretization()));
			}
		}
	}

	if (refresh)
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->ForceFillTree(true);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}

	/*wxString name = property->GetName();

	wxString val;
	float fval = -1.0;

	if (value.GetAs(&fval))
	{
		val << fval;
	}
	else
	{
		value.GetAs(&val);
	}

	wxMessageBox(name << _(": ") << val, _("Property"));

	wxPGProperty *mainProperty = event.GetMainParent();

	if ((mainProperty) && (mainProperty != property) && (!mainProperty->GetValue().IsNull()))
	{
		wxString name = mainProperty->GetName();
		wxString val;

		wxAny value = mainProperty->GetValue();

		value.GetAs(&val);

		wxMessageBox(name << _(": ") << val, _("Main property"));
	}

	wxPropertyCategory *category = this->pgCurve->GetPropertyCategory(property);

	if (category)
	{
		wxString name = category->GetName();
		wxMessageBox(name, _("Category"));
	}*/
}

void CRAbMesh::SplinesModule::OnSurfacePropertyChanged(wxPropertyGridEvent &event)
{
	wxPGProperty *property = event.GetProperty();

	if (!property)
	{
		return;
	}

	wxAny value = property->GetValue();

	if (value.IsNull())
	{
		return;
	}

	bool refresh = false;

	wxPropertyCategory *category = this->pgSurface->GetPropertyCategory(property);

	if ((category) && (category->GetName() == _("Points")))
	{
		wxPGProperty *mainProperty = event.GetMainParent();

		if (mainProperty == property)
		{
			return;
		}

		wxString name = property->GetName();

		wxString point = name.Before('.');
		wxString dimension = name.After('.');

		wxString pointi = name.Before(',');
		wxString pointj = name.After(',');

		unsigned long int pti, ptj, dim;

		pointi.ToULong(&pti);
		pointj.ToULong(&ptj);

		if (dimension == _("X"))
		{
			dim = 0;
		}
		else if (dimension == _("Y"))
		{
			dim = 1;
		}
		else if (dimension == _("Z"))
		{
			dim = 2;
		}
		else
		{
			dimension.ToULong(&dim);
		}

		float val;

		if (value.GetAs(&val))
		{
			Data::Point3D point = (*this->surface)->pt3d((*this->surface)->getPoint(static_cast<UInt>(pti), static_cast<UInt>(ptj)));

			point.setCoord(static_cast<UInt>(dim), static_cast<Real>(val));

			(*this->surface)->move(static_cast<UInt>(pti), static_cast<UInt>(ptj), point.getX(), point.getY(), point.getZ());

			this->ResetSurfacePoints();

			refresh = true;
		}
	}
	/*else if (category)
	{
		enum SplineTypes type = this->SplineType((*this->surface));

		if (type < NUM_SPLINES)
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			if (category->GetName() != name)
			{
				type = NUM_SPLINES;
			}
		}

		switch (type)
		{
		case KOCHANEK_BARTELS:
			{
				if (property->GetName() == _("Tension"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsSurface *>((*this->surface))->setTension(static_cast<Real>(val));

						refresh = true;
					}
				}
				else if (property->GetName() == _("Bias"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsSurface *>((*this->surface))->setBias(static_cast<Real>(val));

						refresh = true;
					}
				}
				else if (property->GetName() == _("Continuity"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::KochanekBartelsSurface *>((*this->surface))->setContinuity(static_cast<Real>(val));

						refresh = true;
					}
				}

				break;
			}
		case CARDINAL:
			{
				if (property->GetName() == _("Tension"))
				{
					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::CardinalSurface *>((*this->surface))->setTension(static_cast<Real>(val));

						refresh = true;
					}
				}

				break;
			}
		case B:
			{
				if (property->GetName() == _("Degree"))
				{
					int val;

					if (value.GetAs(&val))
					{
						if (val > 0)
						{
							static_cast<Data::Spline::BSurface *>((*this->surface))->setDegree(val);

							this->RebuildSurfaceKnotProperties();

							refresh = true;
						}
						else
						{
							this->pgSurface->SetPropertyValue(property, static_cast<int>(static_cast<Data::Spline::BSurface *>((*this->surface))->getDegree()));
						}
					}
				}
				else
				{
					wxPGProperty *mainProperty = event.GetMainParent();

					if ((mainProperty != property) &&
						(mainProperty->GetName() == _("Knot vector")))
					{
						wxString name = property->GetName();
						wxString index = name.After('.');

						unsigned long int ind;

						index.ToULong(&ind);

						float val;

						if (value.GetAs(&val))
						{
							static_cast<Data::Spline::BSurface *>((*this->surface))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

							this->RebuildSurfaceKnotProperties();

							refresh = true;
						}
					}
				}

				break;
			}
		case CUBIC_B:
			{
				wxPGProperty *mainProperty = event.GetMainParent();

				if ((mainProperty) &&
					(mainProperty != property) &&
					(mainProperty->GetName() == _("Knot vector")))
				{
					wxString name = property->GetName();
					wxString index = name.After('.');

					unsigned long int ind;

					index.ToULong(&ind);

					float val;

					if (value.GetAs(&val))
					{
						static_cast<Data::Spline::BicubicBSurface *>((*this->surface))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

						this->RebuildSurfaceKnotProperties();

						refresh = true;
					}
				}

				break;
			}
		case NURB:
			{
				if (property->GetName() == _("Degree"))
				{
					int val;

					if (value.GetAs(&val))
					{
						if (val > 0)
						{
							static_cast<Data::Spline::NURBSurface *>((*this->surface))->setDegree(val);

							this->RebuildSurfaceKnotProperties();

							refresh = true;
						}
						else
						{
							this->pgSurface->SetPropertyValue(property, static_cast<int>(static_cast<Data::Spline::NURBSurface *>((*this->surface))->getDegree()));
						}
					}
				}
				else
				{
					wxPGProperty *mainProperty = event.GetMainParent();

					if ((mainProperty) &&
						(mainProperty != property))
					{
						if (mainProperty->GetName() == _("Knot vector"))
						{
							wxString name = property->GetName();
							wxString index = name.After('.');

							unsigned long int ind;

							index.ToULong(&ind);

							float val;

							if (value.GetAs(&val))
							{
								static_cast<Data::Spline::NURBSurface *>((*this->surface))->setKnot(static_cast<UInt>(ind), static_cast<Real>(val));

								this->RebuildSurfaceKnotProperties();

								refresh = true;
							}
						}
						else if (mainProperty->GetName() == _("Weights"))
						{
							wxString name = property->GetName();
							wxString index = name.After('.');

							unsigned long int ind;

							index.ToULong(&ind);

							float val;

							if (value.GetAs(&val))
							{
								static_cast<Data::Spline::NURBSurface *>((*this->surface))->setWeight(static_cast<UInt>(ind), static_cast<Real>(val));

								refresh = true;
							}
						}
					}
				}

				break;
			}
		default:
			break;
		}
	}*/
	else if (property->GetName() == _("U Discretization"))
	{
		int val;

		if (value.GetAs(&val))
		{
			if (val > 0)
			{
				(*this->surface)->setDiscretization(val, (*this->surface)->getDiscretizationV());

				refresh = true;
			}
			else
			{
				this->pgSurface->SetPropertyValue(property, static_cast<int>((*this->surface)->getDiscretizationU()));
			}
		}
	}
	else if (property->GetName() == _("V Discretization"))
	{
		int val;

		if (value.GetAs(&val))
		{
			if (val > 0)
			{
				(*this->surface)->setDiscretization((*this->surface)->getDiscretizationU(), val);

				refresh = true;
			}
			else
			{
				this->pgSurface->SetPropertyValue(property, static_cast<int>((*this->surface)->getDiscretizationV()));
			}
		}
	}

	if (refresh)
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->ForceFillTree(true);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

bool CRAbMesh::SplinesModule::OnLeftDown(int x, int y)
{
	if (!this->addingPoints)
	{
		return false;
	}

	double mx, my, mz;

	this->camera->Unproject(x, y, mx, my, mz);

	Data::Point3D m(mx, my, mz);
	Data::Point3D p(this->camera->GetPosition(0), this->camera->GetPosition(1), this->camera->GetPosition(2));

	Data::Vector3D v(p, m);

	if (std::fabs(v.getZ()) < Data::getTolerance())
	{
		return false;
	}

	Real t = -p.getZ()/v.getZ();

	Data::Point3D c(p.getX() + t*v.getX(), p.getY() + t*v.getY(), 0.0);

	if ((this->curve != this->curves.end()) &&
		(this->bNewCurvePoint->GetValue()))
	{
		int selection = this->cCurves->GetSelection();

		if ((selection != wxNOT_FOUND) &&
			(selection >= 0) &&
			(this->SplineType((*this->curve)) == selection))
		{
			(*this->curve)->add(new Data::Point3D(c));

			this->ForceFillTree(true);

			this->RebuildCurveProperties();

			return true;
		}
	}

	if ((this->surface != this->surfaces.end()) &&
		(this->bNewSurfacePoint->GetValue()))
	{
		int selection = this->cSurfaces->GetSelection();

		if ((selection != wxNOT_FOUND) &&
			(selection >= 0) &&
			(this->SplineType((*this->surface)) == selection))
		{
			int row = this->scNewSurfacePoint->GetValue();

			if (row >= 0)
			{
				(*this->surface)->add(static_cast<UInt>(row), new Data::Point3D(c));

				this->ForceFillTree(true);

				this->RebuildSurfaceProperties();

				return true;
			}
		}
	}

	return false;
}

bool CRAbMesh::SplinesModule::IsMouseMotionAllowed() const
{
	return !this->addingPoints;
}

bool CRAbMesh::SplinesModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
	minx = miny = minz = DBL_MAX;
	maxx = maxy = maxz = -DBL_MAX;

	if ((this->curves.empty()) && (this->surfaces.empty()))
	{
		return false;
	}

	bool set = false;

	for (Data::CurveList::const_iterator iter = this->curves.begin();
		 iter != this->curves.end(); iter++)
	{
		if ((*iter)->numPoints() == 0)
		{
			continue;
		}

		set = true;

		Data::Box box = (*iter)->box();

		if (box.getMin()->getCoord(0) < minx)
		{
			minx = box.getMin()->getCoord(0);
		}

		if (box.getMin()->getCoord(1) < miny)
		{
			miny = box.getMin()->getCoord(1);
		}

		if ((box.dimension() > 2) && (box.getMin()->getCoord(2) < minz))
		{
			minz = box.getMin()->getCoord(2);
		}

		if (box.getMax()->getCoord(0) > maxx)
		{
			maxx = box.getMax()->getCoord(0);
		}

		if (box.getMax()->getCoord(1) > maxy)
		{
			maxy = box.getMax()->getCoord(1);
		}

		if ((box.dimension() > 2) && (box.getMax()->getCoord(2) > maxz))
		{
			maxz = box.getMax()->getCoord(2);
		}
	}

	for (Data::SurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		if ((*iter)->numPoints() == 0)
		{
			continue;
		}

		set = true;

		Data::Box box = (*iter)->box();

		if (box.getMin()->getCoord(0) < minx)
		{
			minx = box.getMin()->getCoord(0);
		}

		if (box.getMin()->getCoord(1) < miny)
		{
			miny = box.getMin()->getCoord(1);
		}

		if ((box.dimension() > 2) && (box.getMin()->getCoord(2) < minz))
		{
			minz = box.getMin()->getCoord(2);
		}

		if (box.getMax()->getCoord(0) > maxx)
		{
			maxx = box.getMax()->getCoord(0);
		}

		if (box.getMax()->getCoord(1) > maxy)
		{
			maxy = box.getMax()->getCoord(1);
		}

		if ((box.dimension() > 2) && (box.getMax()->getCoord(2) > maxz))
		{
			maxz = box.getMax()->getCoord(2);
		}
	}

	if (!set)
	{
		return false;
	}

	if (minz > maxz)
	{
		minz = maxz = 0.0;
	}

	this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

	return true;
}

void CRAbMesh::SplinesModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::SplinesModule::GetDimension() const
{
	return 3;
}

void CRAbMesh::SplinesModule::SetTree(Data::BoxTree */*tree*/)
{

}

const Data::BoxTree *CRAbMesh::SplinesModule::GetTree() const
{
	return NULL;
}

void CRAbMesh::SplinesModule::SetBoundary(Data::Boundary */*boundary*/)
{

}

const Data::Boundary *CRAbMesh::SplinesModule::GetBoundary() const
{
	return NULL;
}

void CRAbMesh::SplinesModule::SetFront(Data::Front */*front*/)
{

}

const Data::Front *CRAbMesh::SplinesModule::GetFront() const
{
	return NULL;
}

void CRAbMesh::SplinesModule::SetMesh(Data::Mesh */*mesh*/)
{

}

const Data::Mesh *CRAbMesh::SplinesModule::GetMesh() const
{
	return NULL;
}

wxString CRAbMesh::SplinesModule::QualityString() const
{
	return _("");
}

void CRAbMesh::SplinesModule::ClearDisplayLists(bool force) const
{
	MeshModule::ClearDisplayLists(force);
}

void CRAbMesh::SplinesModule::Render() const
{
	wxTreeItemId item = this->tree->GetSelection();

	Data::Drawable *drawable = NULL;

	if (item.IsOk())
	{
		if ((this->IsEnabled(CURVES, TREE_ITEM)) &&
			(this->tree->GetItemParent(item) == (*this->tiItems.find(CURVES)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(item));

			drawable = data->GetItem();
		}
		else if ((this->IsEnabled(SURFACES, TREE_ITEM)) &&
			(this->tree->GetItemParent(item) == (*this->tiItems.find(SURFACES)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(item));

			drawable = data->GetItem();
		}
		else if ((this->IsEnabled(CURVES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(item) == (*this->tiItems.find(CURVES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(item));

			drawable = data->GetItem();
		}
		else if ((this->IsEnabled(SURFACES_POINTS, TREE_ITEM)) &&
			(this->tree->GetItemParent(item) == (*this->tiItems.find(SURFACES_POINTS)).second))
		{
			TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(item));

			drawable = data->GetItem();
		}
	}

	CRAbMesh::MeshModule::Render();

	if (this->IsDrawable())
	{
/*#if USE_SCREENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREENSHOT*/

		glShadeModel(GL_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		if (this->IsCurveDrawable())
		{
			glDisable(GL_LIGHTING);

			for (Data::CurveList::const_iterator iter = this->curves.begin();
				 iter != this->curves.end(); iter++)
			{
				if ((*iter) == (*this->curve))
				{
					(*iter)->highlight();
				}
				else
				{
					(*iter)->unhighlight();
				}

				(*iter)->draw();

				if (this->IsControlPointDrawable())
				{
					(*iter)->drawPoints();
				}

				if (this->IsControlPolygonDrawable())
				{
					(*iter)->drawPolygon();
				}

#if USE_NUMERICAL
				if (this->IsSpacedPointDrawable())
				{
					(*iter)->drawSpacedPoints();
				}

				if (this->IsBaseDrawable())
				{
					(*iter)->drawBases();
				}
#endif //#if USE_NUMERICAL
			}
		}

		if (this->IsSurfaceDrawable())
		{
			glEnable(GL_LIGHTING);

			for (Data::SurfaceList::const_iterator iter = this->surfaces.begin();
				 iter != this->surfaces.end(); iter++)
			{
				if ((*iter) == (*this->surface))
				{
					(*iter)->highlight();
				}
				else
				{
					(*iter)->unhighlight();
				}

				(*iter)->draw();

				if (this->IsControlPointDrawable())
				{
					(*iter)->drawPoints();
				}

				if (this->IsControlPolygonDrawable())
				{
					(*iter)->drawPolygon();
				}

#if USE_NUMERICAL
				/*if (this->IsSpacedPointDrawable())
				{
					(*iter)->drawSpacedPoints();
				}*/

				if (this->IsBaseDrawable())
				{
					(*iter)->drawBases();
				}
#endif //#if USE_NUMERICAL
			}
		}

		if (drawable)
		{
			if (dynamic_cast<Data::Spline::Curve *>(drawable))
			{
				glDisable(GL_LIGHTING);
			}
			else if (dynamic_cast<Data::Spline::Surface *>(drawable))
			{
				glEnable(GL_LIGHTING);
			}

			drawable->highlight();

			drawable->draw();

			if (this->IsControlPointDrawable())
			{
				if (dynamic_cast<Data::Spline::Curve *>(drawable))
				{
					dynamic_cast<Data::Spline::Curve *>(drawable)->drawPoints();
				}
				else if (dynamic_cast<Data::Spline::Surface *>(drawable))
				{
					dynamic_cast<Data::Spline::Surface *>(drawable)->drawPoints();
				}
			}

			if (this->IsControlPolygonDrawable())
			{
				if (dynamic_cast<Data::Spline::Curve *>(drawable))
				{
					dynamic_cast<Data::Spline::Curve *>(drawable)->drawPolygon();
				}
				else if (dynamic_cast<Data::Spline::Surface *>(drawable))
				{
					dynamic_cast<Data::Spline::Surface *>(drawable)->drawPolygon();
				}
			}

#if USE_NUMERICAL
			if (this->IsSpacedPointDrawable())
			{
				if (dynamic_cast<Data::Spline::Curve *>(drawable))
				{
					dynamic_cast<Data::Spline::Curve *>(drawable)->drawSpacedPoints();
				}
				else if (dynamic_cast<Data::Spline::Surface *>(drawable))
				{
					//dynamic_cast<Data::Spline::Surface *>(drawable)->drawSpacedPoints();
				}
			}

			if (this->IsBaseDrawable())
			{
				if (dynamic_cast<Data::Spline::Curve *>(drawable))
				{
					dynamic_cast<Data::Spline::Curve *>(drawable)->drawBases();
				}
				else if (dynamic_cast<Data::Spline::Surface *>(drawable))
				{
					dynamic_cast<Data::Spline::Surface *>(drawable)->drawBases();
				}
			}
#endif //#if USE_NUMERICAL

			drawable->unhighlight();
		}
	}
}

void CRAbMesh::SplinesModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	for (Data::CurveList::iterator iter = this->curves.begin();
		 iter != this->curves.end(); iter++)
	{
		this->AddTreeItem((*iter));
	}

	for (Data::SurfaceList::iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		this->AddTreeItem((*iter));
	}

	MeshModule::ForceFillTree(false);
}

void CRAbMesh::SplinesModule::AddTreeItem(Data::Spline::Curve *c)
{
	if (this->IsEnabled(CURVES, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(c);
		this->tree->AppendItem(this->tiItems[CURVES], item->GetText(), -1, -1, item);
	}

	if (this->IsEnabled(CURVES_POINTS, TREE_ITEM))
	{
		for (UInt i = 0; i < c->numPoints(); i++)
		{
			TreeItemData *item = new TreeItemData(c->getPoint(i));
			this->tree->AppendItem(this->tiItems[CURVES_POINTS], item->GetText(), -1, -1, item);
		}
	}
}

void CRAbMesh::SplinesModule::AddTreeItem(Data::Spline::Surface *s)
{
	if (this->IsEnabled(SURFACES, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(s);
		this->tree->AppendItem(this->tiItems[SURFACES], item->GetText(), -1, -1, item);
	}

	if (this->IsEnabled(SURFACES_POINTS, TREE_ITEM))
	{
		for (UInt i = 0; i < s->numPoints(); i++)
		{
			if (s->getPoint(i))
			{
				TreeItemData *item = new TreeItemData(s->getPoint(i));
				this->tree->AppendItem(this->tiItems[SURFACES_POINTS], item->GetText(), -1, -1, item);
			}
		}
	}
}

Data::Spline::Curve *CRAbMesh::SplinesModule::MakeCurve(UInt i) const
{
	Data::Spline::Curve *curve = NULL;

	switch (i)
	{
	case LAGRANGE:
		curve = new Data::Spline::LagrangeCurve();
		break;
	case CUBIC_LAGRANGE:
		curve = new Data::Spline::CubicLagrangeCurve();
		break;
#if USE_NUMERICAL
	case LAGRANGE_CHEBYSHEV:
		curve = new Data::Spline::LagrangeChebyshevCurve();
		break;
#endif //#if USE_NUMERICAL
	case HERMITE:
		curve = new Data::Spline::HermiteCurve();
		break;
	case C1_HERMITE:
		curve = new Data::Spline::C1HermiteCurve();
		break;
	case BEZIER:
		curve = new Data::Spline::BezierCurve();
		break;
	case CUBIC_BEZIER:
		curve = new Data::Spline::CubicBezierCurve();
		break;
	case C1_CUBIC_BEZIER:
		curve = new Data::Spline::C1CubicBezierCurve();
		break;
	case KOCHANEK_BARTELS:
		curve = new Data::Spline::KochanekBartelsCurve();
		break;
	case CARDINAL:
		curve = new Data::Spline::CardinalCurve();
		break;
	case CATMULL_ROM:
		curve = new Data::Spline::CatmullRomCurve();
		break;
	case B:
		curve = new Data::Spline::BCurve();
		break;
	case CUBIC_B:
		curve = new Data::Spline::CubicBCurve();
		break;
	case NURB:
		curve = new Data::Spline::NURBCurve();
		break;
	}

	return curve;
}

Data::Spline::Surface *CRAbMesh::SplinesModule::MakeSurface(UInt i) const
{
	Data::Spline::Surface *surface = NULL;

	switch (i)
	{
	case LAGRANGE:
		//surface = new Data::Spline::LagrangeSurface();
		break;
	case CUBIC_LAGRANGE:
		//surface = new Data::Spline::BicubicLagrangeSurface();
		break;
#if USE_NUMERICAL
	case LAGRANGE_CHEBYSHEV:
		//surface = new Data::Spline::LagrangeChebyshevSurface();
		break;
#endif //#if USE_NUMERICAL
	case HERMITE:
		//surface = new Data::Spline::HermiteSurface();
		break;
	case C1_HERMITE:
		//surface = new Data::Spline::C1HermiteSurface();
		break;
	case BEZIER:
		surface = new Data::Spline::BezierSurface();
		break;
	case CUBIC_BEZIER:
		surface = new Data::Spline::BicubicBezierSurface();
		break;
	case C1_CUBIC_BEZIER:
		surface = new Data::Spline::C1BicubicBezierSurface();
		break;
	case KOCHANEK_BARTELS:
		//surface = new Data::Spline::KochanekBartelsSurface();
		break;
	case CARDINAL:
		//surface = new Data::Spline::CardinalSurface();
		break;
	case CATMULL_ROM:
		//surface = new Data::Spline::CartmullRomSurface();
		break;
	case B:
		//surface = new Data::Spline::BSurface();
		break;
	case CUBIC_B:
		//surface = new Data::Spline::BicubicBSurface();
		break;
	case NURB:
		//surface = new Data::Spline::NURBSurface();
		break;
	}

	return surface;
}

enum CRAbMesh::SplinesModule::SplineTypes CRAbMesh::SplinesModule::SplineType(Data::Spline::Curve *curve) const
{
	if (dynamic_cast<Data::Spline::CubicLagrangeCurve *>(curve))
	{
		return CUBIC_LAGRANGE;
	}

	if (dynamic_cast<Data::Spline::LagrangeCurve *>(curve))
	{
		return LAGRANGE;
	}

#if USE_NUMERICAL
	if (dynamic_cast<Data::Spline::LagrangeChebyshevCurve *>(curve))
	{
		return LAGRANGE_CHEBYSHEV;
	}
#endif //#if USE_NUMERICAL

	if (dynamic_cast<Data::Spline::C1HermiteCurve *>(curve))
	{
		return C1_HERMITE;
	}

	if (dynamic_cast<Data::Spline::HermiteCurve *>(curve))
	{
		return HERMITE;
	}

	if (dynamic_cast<Data::Spline::C1CubicBezierCurve *>(curve))
	{
		return C1_CUBIC_BEZIER;
	}

	if (dynamic_cast<Data::Spline::CubicBezierCurve *>(curve))
	{
		return CUBIC_BEZIER;
	}

	if (dynamic_cast<Data::Spline::BezierCurve *>(curve))
	{
		return BEZIER;
	}

	if (dynamic_cast<Data::Spline::CatmullRomCurve *>(curve))
	{
		return CATMULL_ROM;
	}

	if (dynamic_cast<Data::Spline::CardinalCurve *>(curve))
	{
		return CARDINAL;
	}

	if (dynamic_cast<Data::Spline::KochanekBartelsCurve *>(curve))
	{
		return KOCHANEK_BARTELS;
	}

	if (dynamic_cast<Data::Spline::NURBCurve *>(curve))
	{
		return NURB;
	}

	if (dynamic_cast<Data::Spline::CubicBCurve *>(curve))
	{
		return CUBIC_B;
	}

	if (dynamic_cast<Data::Spline::BCurve *>(curve))
	{
		return B;
	}

	return NUM_SPLINES;
}

enum CRAbMesh::SplinesModule::SplineTypes CRAbMesh::SplinesModule::SplineType(Data::Spline::Surface *surface) const
{
	/*if (dynamic_cast<Data::Spline::BicubicLagrangeSurface *>(surface))
	{
		return CUBIC_LAGRANGE;
	}

	if (dynamic_cast<Data::Spline::LagrangeSurface *>(surface))
	{
		return LAGRANGE;
	}

#if USE_NUMERICAL
	if (dynamic_cast<Data::Spline::LagrangeChebyshevSurface *>(surface))
	{
		return LAGRANGE_CHEBYSHEV;
	}
#endif //#if USE_NUMERICAL

	if (dynamic_cast<Data::Spline::C1HermiteSurface *>(surface))
	{
		return C1_HERMITE;
	}

	if (dynamic_cast<Data::Spline::HermiteSurface *>(surface))
	{
		return HERMITE;
	}*/

	if (dynamic_cast<Data::Spline::C1BicubicBezierSurface *>(surface))
	{
		return C1_CUBIC_BEZIER;
	}

	if (dynamic_cast<Data::Spline::BicubicBezierSurface *>(surface))
	{
		return CUBIC_BEZIER;
	}

	if (dynamic_cast<Data::Spline::BezierSurface *>(surface))
	{
		return BEZIER;
	}

	/*if (dynamic_cast<Data::Spline::CatmullRomSurface *>(surface))
	{
		return CATMULL_ROM;
	}

	if (dynamic_cast<Data::Spline::CardinalSurface *>(surface))
	{
		return CARDINAL;
	}

	if (dynamic_cast<Data::Spline::KochanekBartelsSurface *>(surface))
	{
		return KOCHANEK_BARTELS;
	}

	if (dynamic_cast<Data::Spline::NURBSurface *>(surface))
	{
		return NURB;
	}

	if (dynamic_cast<Data::Spline::BicubicBSurface *>(surface))
	{
		return CUBIC_B;
	}

	if (dynamic_cast<Data::Spline::BSurface *>(surface))
	{
		return B;
	}*/

	return NUM_SPLINES;
}

void CRAbMesh::SplinesModule::setCurve(const Data::CurveList::iterator &curve)
{
	this->curve = curve;

	this->RebuildCurveProperties();
}

void CRAbMesh::SplinesModule::setSurface(const Data::SurfaceList::iterator &surface)
{
	this->surface = surface;

	this->RebuildSurfaceProperties();
}

void CRAbMesh::SplinesModule::RebuildCurveProperties()
{
	this->pgCurve->Clear();

	if (this->curve == this->curves.end())
	{
		return;
	}

	this->pgCurve->Append(new wxIntProperty(_("Discretization"), wxPG_LABEL, (*this->curve)->getDiscretization()));

	this->pgCurve->Append(new wxPropertyCategory("Points"));

	for (UInt i = 0; i < (*this->curve)->numPoints(); i++)
	{
		Data::Point *p = (*this->curve)->getPoint(i);

		wxString label;
		label << i;

		wxPGProperty *pgPoint = this->pgCurve->Append(new wxStringProperty(label, wxPG_LABEL, "<composed>"));

		for (UInt j = 0; j < p->dimension(); j++)
		{
			wxString dim;

			if (j == 0)
			{
				dim << "X";
			}
			else if (j == 1)
			{
				dim << "Y";
			}
			else if (j == 2)
			{
				dim << "Z";
			}
			else
			{
				dim << j;
			}

			this->pgCurve->AppendIn(pgPoint, new wxFloatProperty(dim, wxPG_LABEL, p->getCoord(j)));
		}
	}

	enum SplineTypes type = this->SplineType((*this->curve));

	switch (type)
	{
	case KOCHANEK_BARTELS:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgCurve->Append(new wxPropertyCategory(name));

			this->pgCurve->Append(new wxFloatProperty(_("Tension"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsCurve *>(*this->curve)->getTension()));
			this->pgCurve->Append(new wxFloatProperty(_("Bias"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsCurve *>(*this->curve)->getBias()));
			this->pgCurve->Append(new wxFloatProperty(_("Continuity"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsCurve *>(*this->curve)->getContinuity()));

			break;
		}
	case CARDINAL:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgCurve->Append(new wxPropertyCategory(name));

			this->pgCurve->Append(new wxFloatProperty(_("Tension"), wxPG_LABEL, static_cast<Data::Spline::CardinalCurve *>(*this->curve)->getTension()));

			break;
		}
	case B:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgCurve->Append(new wxPropertyCategory(name));

			this->pgCurve->Append(new wxIntProperty(_("Degree"), wxPG_LABEL, static_cast<Data::Spline::BCurve *>(*this->curve)->getDegree()));

			wxPGProperty *pgKnot = this->pgCurve->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::BCurve *>(*this->curve)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgCurve->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::BCurve *>(*this->curve)->getKnot(i)));
			}

			break;
		}
	case CUBIC_B:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgCurve->Append(new wxPropertyCategory(name));

			wxPGProperty *pgKnot = this->pgCurve->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::CubicBCurve *>(*this->curve)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgCurve->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::CubicBCurve *>(*this->curve)->getKnot(i)));
			}

			break;
		}
	case NURB:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgCurve->Append(new wxPropertyCategory(name));

			this->pgCurve->Append(new wxIntProperty(_("Degree"), wxPG_LABEL, static_cast<Data::Spline::BCurve *>(*this->curve)->getDegree()));

			wxPGProperty *pgWeights = this->pgCurve->Append(new wxStringProperty(_("Weights"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::NURBCurve *>(*this->curve)->numPoints(); i++)
			{
				wxString label;
				label << i;

				this->pgCurve->AppendIn(pgWeights, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::NURBCurve *>(*this->curve)->getWeight(i)));
			}

			wxPGProperty *pgKnot = this->pgCurve->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::NURBCurve *>(*this->curve)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgCurve->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::NURBCurve *>(*this->curve)->getKnot(i)));
			}

			break;
		}
	default:
		break;
	}

	/*// One way to add category (similar to how other properties are added)
	this->pgCurve->Append( new wxPropertyCategory("Main") );
	// All these are added to "Main" category
	this->pgCurve->Append( new wxStringProperty("Name") );
	this->pgCurve->Append( new wxIntProperty("Age",wxPG_LABEL,25) );
	this->pgCurve->Append( new wxIntProperty("Height",wxPG_LABEL,180) );
	this->pgCurve->Append( new wxIntProperty("Weight") );
	// Another one
	this->pgCurve->Append( new wxPropertyCategory("Attributes") );
	// All these are added to "Attributes" category
	this->pgCurve->Append( new wxIntProperty("Intelligence") );
	this->pgCurve->Append( new wxIntProperty("Agility") );
	this->pgCurve->Append( new wxIntProperty("Strength") );

	this->pgCurve->Append( new wxPropertyCategory("Car Properties") );
	wxPGProperty* carProp = this->pgCurve->Append(new wxStringProperty("Car",wxPG_LABEL,"<composed>"));
	this->pgCurve->AppendIn(carProp, new wxStringProperty("Model",wxPG_LABEL,"Lamborghini Diablo SV"));
	this->pgCurve->AppendIn(carProp, new wxIntProperty("Engine Size (cc)",wxPG_LABEL,5707) );
	wxPGProperty* speedsProp = this->pgCurve->AppendIn(carProp,new wxStringProperty("Speeds",wxPG_LABEL,"<composed>"));
	this->pgCurve->AppendIn( speedsProp, new wxIntProperty("Max. Speed (mph)",wxPG_LABEL,290) );
	this->pgCurve->AppendIn( speedsProp, new wxFloatProperty("0-100 mph (sec)",wxPG_LABEL,3.9) );
	this->pgCurve->AppendIn( speedsProp, new wxFloatProperty("1/4 mile (sec)",wxPG_LABEL,8.6) );
	// This is how child property can be referred to by name
	this->pgCurve->SetPropertyValue( "Car.Speeds.Max. Speed (mph)", 300 );
	this->pgCurve->AppendIn(carProp, new wxIntProperty("Price ($)",wxPG_LABEL,300000) );
	// Displayed value of "Car" property is now very close to this:
	// "Lamborghini Diablo SV; 5707 [300; 3.9; 8.6] 300000"

	this->pgCurve->Append( new wxPropertyCategory("Diet Properties") );
	wxPGChoices chs;
	chs.Add("Herbivore", 40);
	chs.Add("Carnivore", 45);
	chs.Add("Omnivore", 50);
	// Let's add an item with bitmap, too
	chs.Add("None of the above", wxBitmap(), 60);
	this->pgCurve->Append( new wxEnumProperty("Primary Diet",wxPG_LABEL,chs) );
	// Add same choices to another property as well - this is efficient due
	// to reference counting
	this->pgCurve->Append( new wxEnumProperty("Secondary Diet",wxPG_LABEL,chs) );

	this->pgCurve->Append( new wxPropertyCategory("Window Properties") );
	const wxChar* flags_prop_labels[] = { wxT("wxICONIZE"),	wxT("wxCAPTION"), wxT("wxMINIMIZE_BOX"), wxT("wxMAXIMIZE_BOX"), NULL };
	// this value array would be optional if values matched string indexes
	long flags_prop_values[] = { wxICONIZE, wxCAPTION, wxMINIMIZE_BOX, wxMAXIMIZE_BOX };
	this->pgCurve->Append( new wxFlagsProperty("Window Style",wxPG_LABEL,flags_prop_labels,flags_prop_values,wxDEFAULT_FRAME_STYLE) );

	this->pgCurve->Append( new wxPropertyCategory("Specialized Properties") );
	// Date property.
	this->pgCurve->Append( new wxDateProperty("MyDateProperty",wxPG_LABEL,wxDateTime::Now()) );
	// Image file property. Wild card is auto-generated from available
	// image handlers, so it is not set this time.
	this->pgCurve->Append( new wxImageFileProperty("Label of ImageFileProperty","NameOfImageFileProp") );
	// Font property has sub-properties. Note that we give window's font as
	// initial value.
	this->pgCurve->Append( new wxFontProperty("Font",wxPG_LABEL,this->frame->GetFont()) );
	// Colour property with arbitrary colour.
	this->pgCurve->Append( new wxColourProperty("My Colour 1",wxPG_LABEL,wxColour(242,109,0) ) );
	// System colour property.
	this->pgCurve->Append( new wxSystemColourProperty("My SysColour 1",wxPG_LABEL,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)) );
	// System colour property with custom colour.
	this->pgCurve->Append( new wxSystemColourProperty("My SysColour 2",wxPG_LABEL,wxColour(0,200,160) ) );
	// Cursor property
	this->pgCurve->Append( new wxCursorProperty("My Cursor",wxPG_LABEL,wxCURSOR_ARROW));

	this->pgCurve->GetParent()->GetSizer()->Layout();
	this->pgCurve->GetParent()->FitInside();*/
}

void CRAbMesh::SplinesModule::RebuildSurfaceProperties()
{
	this->pgSurface->Clear();

	if (this->surface == this->surfaces.end())
	{
		return;
	}

	this->pgSurface->Append(new wxIntProperty(_("U Discretization"), wxPG_LABEL, (*this->surface)->getDiscretizationU()));
	this->pgSurface->Append(new wxIntProperty(_("V Discretization"), wxPG_LABEL, (*this->surface)->getDiscretizationV()));

	this->pgSurface->Append(new wxPropertyCategory("Points"));

	for (UInt i = 0; i < (*this->surface)->numPointsV(); i++)
	{
		for (UInt j = 0; j < (*this->surface)->numPointsU(); j++)
		{
			Data::Point *p = (*this->surface)->getPoint(i, j);

			if (!p)
			{
				continue;
			}

			wxString label;
			label << i << _(",") << j;

			wxPGProperty *pgPoint = this->pgSurface->Append(new wxStringProperty(label, wxPG_LABEL, "<composed>"));

			for (UInt k = 0; k < p->dimension(); k++)
			{
				wxString dim;

				if (k == 0)
				{
					dim << "X";
				}
				else if (k == 1)
				{
					dim << "Y";
				}
				else if (k == 2)
				{
					dim << "Z";
				}
				else
				{
					dim << k;
				}

				this->pgSurface->AppendIn(pgPoint, new wxFloatProperty(dim, wxPG_LABEL, p->getCoord(k)));
			}
		}
	}

	/*enum SplineTypes type = this->SplineType((*this->surface));

	switch (type)
	{
	case KOCHANEK_BARTELS:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgSurface->Append(new wxPropertyCategory(name));

			this->pgSurface->Append(new wxFloatProperty(_("Tension"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsSurface *>(*this->surface)->getTension()));
			this->pgSurface->Append(new wxFloatProperty(_("Bias"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsSurface *>(*this->surface)->getBias()));
			this->pgSurface->Append(new wxFloatProperty(_("Continuity"), wxPG_LABEL, static_cast<Data::Spline::KochanekBartelsSurface *>(*this->surface)->getContinuity()));

			break;
		}
	case CARDINAL:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgSurface->Append(new wxPropertyCategory(name));

			this->pgSurface->Append(new wxFloatProperty(_("Tension"), wxPG_LABEL, static_cast<Data::Spline::CardinalSurface *>(*this->surface)->getTension()));

			break;
		}
	case B:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgSurface->Append(new wxPropertyCategory(name));

			this->pgSurface->Append(new wxIntProperty(_("Degree"), wxPG_LABEL, static_cast<Data::Spline::BSurface *>(*this->surface)->getDegree()));

			wxPGProperty *pgKnot = this->pgSurface->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::BSurface *>(*this->surface)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgSurface->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::BSurface *>(*this->surface)->getKnot(i)));
			}

			break;
		}
	case CUBIC_B:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgSurface->Append(new wxPropertyCategory(name));

			wxPGProperty *pgKnot = this->pgSurface->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::BicubicBSurface *>(*this->surface)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgSurface->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::BicubicBSurface *>(*this->surface)->getKnot(i)));
			}

			break;
		}
	case NURB:
		{
			wxString name(this->splineNames[type].c_str(), wxMBConvLibc());

			this->pgSurface->Append(new wxPropertyCategory(name));

			this->pgSurface->Append(new wxIntProperty(_("Degree"), wxPG_LABEL, static_cast<Data::Spline::BSurface *>(*this->surface)->getDegree()));

			wxPGProperty *pgWeights = this->pgSurface->Append(new wxStringProperty(_("Weights"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::NURBSurface *>(*this->surface)->numPoints(); i++)
			{
				wxString label;
				label << i;

				this->pgSurface->AppendIn(pgWeights, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::NURBSurface *>(*this->surface)->getWeight(i)));
			}

			wxPGProperty *pgKnot = this->pgSurface->Append(new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

			for (UInt i = 0; i < static_cast<Data::Spline::NURBSurface *>(*this->surface)->getKnotSize(); i++)
			{
				wxString label;
				label << i;

				this->pgSurface->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::NURBSurface *>(*this->surface)->getKnot(i)));
			}

			break;
		}
	default:
		break;
	}*/
}

void CRAbMesh::SplinesModule::ResetCurvePoints()
{
	wxPGProperty *pPoints = this->pgCurve->GetProperty(_("Points"));

	wxPropertyGridIterator iter = this->pgCurve->GetIterator(wxPG_ITERATE_ALL, pPoints);

	iter.Next();

	for (UInt i = 0; i < (*this->curve)->numPoints(); i++)
	{
		Data::Point *p = (*this->curve)->getPoint(i);

		iter.Next();

		for (UInt j = 0; j < p->dimension(); j++, iter.Next())
		{
			this->pgCurve->SetPropertyValue(iter.GetProperty(), p->getCoord(j));
		}
	}
}

void CRAbMesh::SplinesModule::ResetSurfacePoints()
{
	wxPGProperty *pPoints = this->pgSurface->GetProperty(_("Points"));

	wxPropertyGridIterator iter = this->pgSurface->GetIterator(wxPG_ITERATE_ALL, pPoints);

	iter.Next();

	for (UInt i = 0; i < (*this->surface)->numPointsV(); i++)
	{
		for (UInt j = 0; j < (*this->surface)->numPointsU(); j++)
		{
			Data::Point *p = (*this->surface)->getPoint(i, j);

			if (!p)
			{
				continue;
			}

			iter.Next();

			for (UInt k = 0; k < p->dimension(); k++, iter.Next())
			{
				this->pgSurface->SetPropertyValue(iter.GetProperty(), p->getCoord(k));
			}
		}
	}
}

void CRAbMesh::SplinesModule::RebuildCurveKnotProperties()
{
	enum SplineTypes type = this->SplineType((*this->curve));

	if ((type != B) && (type != CUBIC_B) && (type != NURB))
	{
		return;
	}

	wxPGProperty *pgKnot = this->pgCurve->GetProperty(_("Knot vector"));

	if (!pgKnot)
	{
		return;
	}

	wxPropertyCategory *category = this->pgCurve->GetPropertyCategory(pgKnot);

	this->pgCurve->DeleteProperty(pgKnot);

	pgKnot = this->pgCurve->Insert(category, -1, new wxStringProperty(_("Knot vector"), wxPG_LABEL, "<composed>"));

	for (UInt i = 0; i < static_cast<Data::Spline::BCurve *>(*this->curve)->getKnotSize(); i++)
	{
		wxString label;
		label << i;

		this->pgCurve->AppendIn(pgKnot, new wxFloatProperty(label, wxPG_LABEL, static_cast<Data::Spline::BCurve *>(*this->curve)->getKnot(i)));
	}
}

#endif //#if USE_SPLINES
#endif //#if USE_GUI
