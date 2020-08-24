#ifndef _CRAB_MESH_SPLINES_MODULE_H_
#define _CRAB_MESH_SPLINES_MODULE_H_

#include "CRAbMesh/Definitions.h"
#include "Data/Spline/Definitions.h"

#if USE_GUI
#if USE_SPLINES

#include <wx/tglbtn.h>
#include <wx/spinctrl.h>
#include <wx/propgrid/propgrid.h>

#include "CRAbMesh/MeshModule.h"
#include "Data/Spline/Definitions.h"

namespace CRAbMesh
{
	class SplinesModule : public CRAbMesh::MeshModule
	{
	public:

		SplinesModule(Frame *frame = NULL, Canvas *canvas = NULL);
		virtual ~SplinesModule();

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		virtual wxString Name() const;

		using CRAbMesh::MeshModule::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetGUIState();

		virtual void SetCurveDrawable(bool curveDrawable);
		virtual bool IsCurveDrawable() const;

		virtual void SetSurfaceDrawable(bool surfaceDrawable);
		virtual bool IsSurfaceDrawable() const;

		virtual void SetControlPointDrawable(bool controlPointDrawable);
		virtual bool IsControlPointDrawable() const;

		virtual void SetControlPolygonDrawable(bool controlPolygonDrawable);
		virtual bool IsControlPolygonDrawable() const;

#if USE_NUMERICAL
		virtual void SetSpacedPointDrawable(bool spacedPointDrawable);
		virtual bool IsSpacedPointDrawable() const;

		virtual void SetBaseDrawable(bool baseDrawable);
		virtual bool IsBaseDrawable() const;
#endif //#if USE_NUMERICAL

		virtual bool IsDrawable() const;

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxPanel *MakeConfigurationPanel();

	protected:

		virtual void OnNewCurve(wxCommandEvent &event);
		virtual void OnNewCurvePoint(wxCommandEvent &event);

		virtual void OnNewSurface(wxCommandEvent &event);
		virtual void OnNewSurfacePoint(wxCommandEvent &event);

		virtual void OnClear(wxCommandEvent &event);

		virtual void OnTreeSelectionChanged(wxTreeEvent &event);

		virtual void OnCurvePropertyChanged(wxPropertyGridEvent &event);
		virtual void OnSurfacePropertyChanged(wxPropertyGridEvent &event);

	public:

		virtual bool OnLeftDown(int x, int y);
		virtual bool IsMouseMotionAllowed() const;

	public:

		enum SplineTypes
		{
			LAGRANGE = 0,
			CUBIC_LAGRANGE,
#if USE_NUMERICAL
			LAGRANGE_CHEBYSHEV,
#endif //#if USE_NUMERICAL
			HERMITE,
			C1_HERMITE,
			BEZIER,
			CUBIC_BEZIER,
			C1_CUBIC_BEZIER,
			KOCHANEK_BARTELS,
			CARDINAL,
			CATMULL_ROM,
			B,
			CUBIC_B,
			NURB,
			NUM_SPLINES
		};

	protected:

		virtual bool BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const;

		virtual void SetDimension(unsigned int dimension);
		virtual unsigned int GetDimension() const;

		virtual void SetTree(Data::BoxTree *tree);
		virtual const Data::BoxTree *GetTree() const;
		virtual void SetBoundary(Data::Boundary *boundary);
		virtual const Data::Boundary *GetBoundary() const;
		virtual void SetFront(Data::Front *front);
		virtual const Data::Front *GetFront() const;
		virtual void SetMesh(Data::Mesh *mesh);
		virtual const Data::Mesh *GetMesh() const;

		virtual wxString QualityString() const;

		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void ForceFillTree(bool clear = true);
		using MeshModule::AddTreeItem;
		virtual void AddTreeItem(Data::Spline::Curve *c);
		virtual void AddTreeItem(Data::Spline::Surface *s);

		virtual Data::Spline::Curve *MakeCurve(UInt i) const;
		virtual Data::Spline::Surface *MakeSurface(UInt i) const;

		virtual enum SplineTypes SplineType(Data::Spline::Curve *curve) const;
		virtual enum SplineTypes SplineType(Data::Spline::Surface *surface) const;

		virtual void setCurve(const Data::CurveList::iterator &curve);
		virtual void setSurface(const Data::SurfaceList::iterator &surface);

		virtual void RebuildCurveProperties();
		virtual void RebuildSurfaceProperties();

		virtual void ResetCurvePoints();
		virtual void ResetSurfacePoints();

		virtual void RebuildCurveKnotProperties();

	protected:

		std::string CURVES;
		std::string CURVES_POINTS;
		std::string SURFACES;
		std::string SURFACES_POINTS;

		std::string DRAW_CURVES;
		std::string DRAW_SURFACES;
		std::string DRAW_CONTROL_POINTS;
		std::string DRAW_CONTROL_POLYGON;
#if USE_NUMERICAL
		std::string DRAW_SPACED_POINTS;
		std::string DRAW_BASES;
#endif //#if USE_NUMERICAL

	protected:

		wxChoice *cCurves;
		wxChoice *cSurfaces;

		wxButton *bNewCurve;
		wxButton *bNewSurface;
		wxButton *bClear;

		wxPropertyGrid *pgCurve;
		wxPropertyGrid *pgSurface;

		wxToggleButton *bNewCurvePoint;
		wxToggleButton *bNewSurfacePoint;

		wxSpinCtrl *scNewSurfacePoint;

	protected:

		std::string splineNames[NUM_SPLINES];

		Data::CurveList curves;
		Data::SurfaceList surfaces;

		Data::CurveList::iterator curve;
		Data::SurfaceList::iterator surface;

		bool addingPoints;

	};
}

#endif //#if USE_SPLINES
#endif //#if USE_GUI

#endif //#endif //_CRAB_MESH_SPLINES_MODULE_H_
