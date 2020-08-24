#ifndef _CRAB_MESH_TECGRAF_MSH_2D_MODULE_H_
#define _CRAB_MESH_TECGRAF_MSH_2D_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI
#if USE_TECGRAF_MSH_2D

#include "CRAbMesh/MeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "IO/Definitions.h"
#include "TecGraf/Definitions.h"

namespace CRAbMesh
{
	class TecGrafMsh2DModule : public CRAbMesh::MeshModule
	{
	public:

		TecGrafMsh2DModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~TecGrafMsh2DModule();

		virtual wxString Name() const;

		using CRAbMesh::MeshModule::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);
		virtual bool LoadWarpFile(const std::string &filename);

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetGUIState();

		virtual void SetWarpBoundaryDrawable(bool warpBoundaryDrawable);
		virtual bool IsWarpBoundaryDrawable() const;

		virtual void SetWarpMeshDrawable(bool warpMeshDrawable);
		virtual bool IsWarpMeshDrawable() const;

		virtual void SetDeformedMeshDrawable(bool deformedMeshDrawable);
		virtual bool IsDeformedMeshDrawable() const;

		virtual bool IsDrawable() const;

		virtual const Data::Boundary *GetWarpBoundary() const;
		virtual const Data::Mesh *GetWarpMesh() const;

		virtual const Data::Mesh *GetDeformedMesh() const;

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();

	protected:

		virtual void OnLoadWarpBoundary(wxCommandEvent &event);
		virtual void OnDeformWarpMesh(wxScrollEvent &event);

		virtual void OnExecute(wxCommandEvent &event);
		virtual void OnClear(wxCommandEvent &event);

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

		virtual void SetWarpBoundary(Data::Boundary *boundary);
		virtual void SetWarpMesh(Data::Mesh *mesh);

		virtual wxString QualityString() const;

		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void ForceFillTree(bool clear = true);
		using CRAbMesh::MeshModule::AddTreeItem;
		virtual void AddTreeItem(Data::Vertex *v);
		virtual void AddTreeItem(Data::FrontElement *e);
		virtual void AddTreeItem(Data::Element *e);

	protected:

		std::string WARP_VERTICES;
		std::string WARP_FRONT_ELEMENTS;
		std::string WARP_ELEMENTS;
		std::string DRAW_WARP_BOUNDARY;
		std::string DRAW_WARP_MESH;
		std::string DRAW_DEFORMED_MESH;

	protected:

		wxBookCtrlBase *bcbAlgorithm;

		wxTextCtrl *tcBilinearElementType;
		wxTextCtrl *tcBilinearDiagonalType;
		wxTextCtrl *tcBilinearAngleTolerance;
		wxTextCtrl *tcBilinearNumberVerticesDirection1;
		wxTextCtrl *tcBilinearNumberVerticesDirection2;

		wxTextCtrl *tcTryBilinearElementType;
		wxTextCtrl *tcTryBilinearDiagonalType;

		wxTextCtrl *tcCollBilinearElementType;
		wxTextCtrl *tcCollBilinearDiagonalType;
		wxTextCtrl *tcCollBilinearAngleTolerance;
		wxTextCtrl *tcCollBilinearNumberVerticesDirection1;
		wxTextCtrl *tcCollBilinearNumberVerticesDirection2;

		wxTextCtrl *tcLoftElementType;
		wxTextCtrl *tcLoftDiagonalType;
		wxTextCtrl *tcLoftAngleTolerance;
		wxTextCtrl *tcLoftNumberVerticesDirection1;
		wxTextCtrl *tcLoftNumberVerticesDirection2;
		wxTextCtrl *tcLoftDirection;
		wxTextCtrl *tcLoftWeight;

		wxTextCtrl *tcCollLoftElementType;
		wxTextCtrl *tcCollLoftDiagonalType;
		wxTextCtrl *tcCollLoftAngleTolerance;
		wxTextCtrl *tcCollLoftNumberVerticesDirection1;
		wxTextCtrl *tcCollLoftNumberVerticesDirection2;
		wxTextCtrl *tcCollLoftWeight;

		wxTextCtrl *tcTrilinearElementType;
		wxTextCtrl *tcTrilinearAngleTolerance;
		wxTextCtrl *tcTrilinearNumberVerticesDirection;

		wxTextCtrl *tcTryTrilinearElementType;

		wxCheckBox *cbBoundContractionInternalVertices;
		wxCheckBox *cbBoundContractionInternalVerticesWithQuadtree;

		wxCheckBox *cbBoundContractionWarpInternalVertices;
		wxCheckBox *cbBoundContractionWarpInternalVerticesWithQuadtree;
		wxCheckBox *cbBoundContractionWarpBoundaryVertices;
		//wxCheckBox *cbBoundContractionWarpInvertReadOrderOnTxt;
		wxButton *bBoundContractionWarpLoadWarpBoundary;
		wxSlider *sBoundCountractionWarpDeformMesh;

		wxTextCtrl *tcQuadBoundElementType;
		wxCheckBox *cbQuadBoundRefineQuadtree;

		wxCheckBox *cbShapeInternalPoints;
		wxTextCtrl *tcShapeRefinementFactor;

		wxCheckBox *cbEdgeInternalPoints;
		wxTextCtrl *tcEdgeRefinementFactor;

#if USE_QUADS
		wxCheckBox *cbQuadSeamInternalPoints;
		wxTextCtrl *tcQuadSeamRefinementFactor;

		wxCheckBox *cbQuadSeamEdgeInternalPoints;
		wxTextCtrl *tcQuadSeamEdgeRefinementFactor;

		wxTextCtrl *tcTemplateNumberSides;
		wxTextCtrl *tcTemplateNumberSubdivision[4];
		wxCheckBox *cbTemplateSmooth;
		wxTextCtrl *tcTemplateU1;
		wxTextCtrl *tcTemplateV1;
		wxTextCtrl *tcTemplateU2;
		wxTextCtrl *tcTemplateV2;
#endif //#if USE_QUADS

		wxButton *bExecute;
		wxButton *bClear;

		wxTextCtrl *tcLog;

		mutable bool useRedirector;
		mutable Redirector redirector;

		MJMesh::Mesh *deformed;

	};
}

#endif //#if USE_TECGRAF_MSH_2D
#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_TECGRAF_MSH_2D_MODULE_H_
