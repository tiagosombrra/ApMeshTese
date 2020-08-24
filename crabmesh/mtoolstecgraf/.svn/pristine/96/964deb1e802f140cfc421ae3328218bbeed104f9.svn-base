#ifndef _CRAB_MESH_TECGRAF_MSH_SURF_MODULE_H_
#define _CRAB_MESH_TECGRAF_MSH_SURF_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI
#if USE_TECGRAF_MSH_SURF

#include "CRAbMesh/MeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "IO/Definitions.h"
#include "TecGraf/Definitions.h"

namespace CRAbMesh
{
	class TecGrafMshSurfModule : public CRAbMesh::MeshModule
	{
	public:

		TecGrafMshSurfModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~TecGrafMshSurfModule();

		virtual wxString Name() const;

		using CRAbMesh::MeshModule::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetGUIState();

		virtual void SetSupportMeshDrawable(bool supportMeshDrawable);
		virtual bool IsSupportMeshDrawable() const;

		virtual bool IsDrawable() const;

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();

	protected:

		virtual void OnLoadSupportMesh(wxCommandEvent &event);

		virtual void OnTreeMiddleClick(wxTreeEvent &event);

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

		virtual void SetSupportMesh(Data::Mesh *mesh);
		virtual const Data::Mesh *GetSupportMesh() const;

		virtual wxString QualityString() const;

		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void ForceFillTree(bool clear = true);
		using CRAbMesh::MeshModule::AddTreeItem;
		virtual void AddTreeItem(Data::Vertex *v);
		virtual void AddTreeItem(Data::FrontElement *e);
		virtual void AddTreeItem(Data::Element *e);

	protected:

		std::string SUPPORT_VERTICES;
		std::string SUPPORT_FRONT_ELEMENTS;
		std::string SUPPORT_ELEMENTS;
		std::string DRAW_SUPPORT_MESH;

	protected:

		wxBookCtrlBase *bcbAlgorithm;

		wxTextCtrl *tcAnalyticalMaxElementSize;
		wxCheckBox *cbAnalyticalComputeCurvature;

		wxTextCtrl *tcSupportMaxElementSize;
		wxCheckBox *cbSupportComputeCurvature;
		wxCheckBox *cbSupportInvertMesh;
		wxButton *bSupportLoadSupportMesh;

		wxButton *bExecute;
		wxButton *bClear;

		wxTextCtrl *tcLog;

		mutable bool useRedirector;
		mutable Redirector redirector;

    public:

		static Redirector *mshSurfRedirector;

	};

	void mshSurfPrint(char *msg);
}

#endif //#if USE_TECGRAF_MSH_SURF
#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_TECGRAF_MSH_SURF_MODULE_H_
