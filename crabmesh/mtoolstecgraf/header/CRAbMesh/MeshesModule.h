#ifndef _CRAB_MESH_MESHES_MODULE_H_
#define _CRAB_MESH_MESHES_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/progdlg.h>
#include "CRAbMesh/MeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "IO/Definitions.h"
#include "Tools/Definitions.h"
#include "TecGraf/Definitions.h"

namespace CRAbMesh
{
	class MeshesModule : public CRAbMesh::MeshModule
	{
	public:

		MeshesModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~MeshesModule();

		virtual bool SaveFile(const std::string &filename);

		virtual wxString Name() const;

		using CRAbMesh::MeshModule::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using CRAbMesh::MeshModule::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual wxMenu *GetPickedMenu(wxString &title) const;

		virtual bool LoadFile(const std::string &filename);

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetGUIState();

		virtual void ChangeFrom(const Module *module);
		virtual void ChangeTo(const Module *module);

		virtual void OpenFiles();
		virtual void SaveFiles();

		virtual void ForceShowMesh();

#if USE_TECGRAF
		virtual void SetModelDrawable(bool modelDrawable);
		virtual bool IsModelDrawable() const;

		/*virtual void SetRegionsDrawable(bool regionsDrawable);
		virtual bool IsRegionsDrawable() const;

		virtual void SetSurfacesDrawable(bool surfacesDrawable);
		virtual bool IsSurfacesDrawable() const;*/

		virtual bool IsDrawable() const;
#endif //#if USE_TECGRAF

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxMenu *MakePickedMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();

	protected:

		virtual void UpdatePDLocal();
		virtual void OnTimer();

		virtual void OnOpenFiles(wxCommandEvent &event);
		virtual void OnSaveFiles(wxCommandEvent &event);
		virtual void OnMeshes(wxScrollEvent &event);
		virtual void OnFitCamera(wxCommandEvent &event);

		virtual void OnCheckGeometry(wxCommandEvent &event);
		virtual void OnCheckTopology(wxCommandEvent &event);
		virtual void OnCheckWaterTightness(wxCommandEvent &event);

		virtual void OnRemoveCracks(wxCommandEvent &event);
		virtual void OnCopyConvert(wxCommandEvent &event);
		virtual void OnFixNormals(wxCommandEvent &event);
		virtual void OnInvertNormals(wxCommandEvent &event);
		virtual void OnIdentifyInnerSurfaces(wxCommandEvent &event);
		virtual void OnIdentifySilhouette(wxCommandEvent &event);
		virtual void OnIdentifySurfaces(wxCommandEvent &event);
		virtual void OnIdentifyVisibility(wxCommandEvent &event);
		virtual void OnIdentifyEdgeVisibility(wxCommandEvent &event);
		virtual void OnFindVisibilityBasedVertex(wxCommandEvent &event);
#if USE_NUMERICAL
		virtual void OnFindCovarianceMatrix(wxCommandEvent &event);
		virtual void OnAnalyzePrincipalComponents(wxCommandEvent &event);
		virtual void OnHotellingTransform(wxCommandEvent &event);
#endif //#if USE_NUMERICAL

		virtual void OnSmooth(wxCommandEvent &event);
		virtual void OnRefineSwap(wxCommandEvent &event);
		virtual void OnRefineCollapse(wxCommandEvent &event);

		virtual void OnRefineUniformly(wxCommandEvent &event);
		virtual void OnRefineSize(wxCommandEvent &event);
		virtual void OnRefineEdge(wxCommandEvent &event);

		virtual void OnMergeMeshes(wxCommandEvent &event);
		virtual void OnSplitMeshes(wxCommandEvent &event);
		virtual void OnMergeVertices(wxCommandEvent &event);
		virtual void OnMakeBoundary(wxCommandEvent &event);
		virtual void OnColorizeMeshes(wxCommandEvent &event);
		virtual void OnColorizeElements(wxCommandEvent &event);

		virtual void OnTranslate(wxCommandEvent &event);
		virtual void OnScale(wxCommandEvent &event);
		virtual void OnRotate(wxCommandEvent &event);
		virtual void OnCentralize(wxCommandEvent &event);

#if USE_TECGRAF
		virtual void OnBuildBoundaries(wxCommandEvent &event);
#endif //#if USE_TECGRAF

		virtual void OnClear(wxCommandEvent &event);

		virtual void OnPickedShow(wxCommandEvent &event);
		virtual void OnPickedDeleteFirstVertex(wxCommandEvent &event);
		virtual void OnPickedDeleteFirstFrontElement(wxCommandEvent &event);
		virtual void OnPickedDeleteFirstElement(wxCommandEvent &event);
		virtual void OnPickedDeleteVertices(wxCommandEvent &event);
		virtual void OnPickedDeleteFrontElements(wxCommandEvent &event);
		virtual void OnPickedDeleteElements(wxCommandEvent &event);
		virtual void OnPickedDeleteSelected(wxCommandEvent &event);
		virtual void OnPickedMergeVertices(wxCommandEvent &event);
		virtual void OnPickedClearPickedList(wxCommandEvent &event);

#if USE_TECGRAF
		virtual void OnTreeMiddleClick(wxTreeEvent &event);
#endif //#if USE_TECGRAF

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
#if USE_TECGRAF
		virtual void SetModel(TecGraf::Model *model);
		virtual const TecGraf::Model *GetModel() const;
#endif //#if USE_TECGRAF
		virtual wxString GetLoadedFilename() const;

		virtual wxString QualityString() const;

#if USE_TECGRAF
		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void ForceFillTree(bool clear = true);
		using MeshModule::AddTreeItem;
		//virtual void AddTreeItem(TecGraf::Model *m);
		virtual void AddTreeItem(TecGraf::Region *r);
		virtual void AddTreeItem(TecGraf::Surface *s);
#endif //#if USE_TECGRAF

#if USE_TECGRAF
	protected:

		std::string REGIONS;
		std::string SURFACES;
		std::string DRAW_MODEL;
		//std::string DRAW_REGIONS;
		//std::string DRAW_SURFACES;
#endif //#if USE_TECGRAF

	protected:

		virtual void ExecuteTools(bool dialog = true);

		virtual void ShowMessage(Tools::Tool *tool);
		virtual bool ShowMessage(Tools::MeshInterceptionChecker *tool);
		virtual bool ShowMessage(Tools::MeshTopologyChecker *tool);
		virtual bool ShowMessage(Tools::BoundaryWaterTightnessChecker *tool);
		virtual bool ShowMessage(Tools::CrackRemover *tool);
		virtual bool ShowMessage(Tools::BoundaryConversor *tool);
		virtual bool ShowMessage(Tools::BoundaryNormalFixer *tool);
		virtual bool ShowMessage(Tools::BoundaryNormalInverter *tool);
		virtual bool ShowMessage(Tools::BoundaryLaplacianSmoother *tool);
		virtual bool ShowMessage(Tools::UniformBoundaryAdapter *tool);
		virtual bool ShowMessage(Tools::SizeBoundaryAdapter *tool);
		virtual bool ShowMessage(Tools::EdgeSwapBoundaryAdapter *tool);
		virtual bool ShowMessage(Tools::EdgeCollapseBoundaryAdapter *tool);
		virtual bool ShowMessage(Tools::EdgeBoundaryAdapter *tool);
		virtual bool ShowMessage(Tools::InnerSurfacesIdentifier *tool);
		virtual bool ShowMessage(Tools::SilhouetteIdentifier *tool);
		virtual bool ShowMessage(Tools::SurfacesIdentifier *tool);
		virtual bool ShowMessage(Tools::VisibilityIdentifier *tool);
		virtual bool ShowMessage(Tools::FrontElementVisibilityIdentifier *tool);
		virtual bool ShowMessage(Tools::VisibilityBasedVertexFinder *tool);
#if USE_NUMERICAL
		virtual bool ShowMessage(Tools::CovarianceMatrixFinder *tool);
		virtual bool ShowMessage(Tools::PrincipalComponentsAnalyzer *tool);
		virtual bool ShowMessage(Tools::HotellingTransformer *tool);
#endif //#if USE_NUMERICAL
		virtual bool ShowMessage(Tools::MeshSplitter *tool);
		virtual bool ShowMessage(Tools::MeshColorizer *tool);
		virtual bool ShowMessage(Tools::MeshMerger *tool);
		virtual bool ShowMessage(Tools::VertexMerger *tool);
		virtual bool ShowMessage(Tools::BoundaryCreator *tool);
		virtual bool ShowMessage(Tools::MeshTransformer *tool);
#if USE_TECGRAF
		virtual bool ShowMessage(Tools::TecGrafBoundaryBuilder *tool);
#endif //#if USE_TECGRAF

		virtual bool ExecuteForAllMeshes();

		virtual void ForceFitCamera();
		virtual void ShowMesh(int show, int max = -1);

		virtual bool EnsureExistenceOfMesh();

		virtual bool SaveFile(Data::Boundary *boundary, Data::Mesh *mesh, const std::string &filename);

		virtual wxString Delete(Data::Drawable *d);
		virtual wxString Delete(Data::Vertex *v);
		virtual wxString Delete(Data::FrontElement *fel);
		virtual wxString Delete(Data::Element *e);

	protected:

		//wxButton *bOpenFiles;
		//wxButton *bSaveFiles;
		wxSlider *sMeshes;

		wxRadioBox *rbMeshRange;
#if USE_NUMERICAL
		wxRadioBox *rbCovMatrMode;
#endif //#if USE_NUMERICAL

		wxCheckBox *cbAutoFitCamera;

		wxButton *bFitCamera;

		wxBookCtrlBase *bcbTools;

		wxCheckBox *cbGStopOnFirst;
		wxCheckBox *cbTStopOnFirst;
		wxCheckBox *cbAllowMultiple;
		wxCheckBox *cbWStopOnFirst;
		wxCheckBox *cbSmoothNormalTest;
		wxCheckBox *cbSwNoCrack;
		wxCheckBox *cbSwNormalTest;
		wxCheckBox *cbSwChangeSurface;
#if USE_NUMERICAL
		wxCheckBox *cbCovMatrFrontElements;
		wxCheckBox *cbCovMatrWeighted;
#endif //#if USE_NUMERICAL
		wxCheckBox *cbInPlane;
		wxCheckBox *cbENoCrack;
		wxCheckBox *cbUNoCrack;
		wxCheckBox *cbSNoCrack;
		wxCheckBox *cbCNoCrack;
		wxCheckBox *cbMMKeepOldIds;

		wxChoice *cColorScheme;

		wxButton *bCheckGeometry;
		wxButton *bCheckTopology;
		wxButton *bCheckWaterTightness;

		wxButton *bRemoveCracks;
		wxButton *bCopyConvert;
		wxButton *bFixNormals;
		wxButton *bInvertNormals;
		wxButton *bIdentifyInnerSurfaces;
		wxButton *bIdentifySilhouette;
		wxButton *bIdentifySurfaces;
		wxButton *bIdentifyVisibility;
		wxButton *bIdentifyEdgeVisibility;
		wxButton *bFindVisibilityBasedVertex;
#if USE_NUMERICAL
		wxButton *bFindCovarianceMatrix;
		wxButton *bAnalyzePrincipalComponents;
		wxButton *bHotellingTransorm;
#endif //#if USE_NUMERICAL

		wxButton *bSmooth;
		wxButton *bRefineCollapse;
		wxButton *bRefineSwap;

		wxButton *bRefineUniformly;
		wxButton *bRefineSize;
		wxButton *bRefineEdge;

		wxButton *bMergeMeshes;
		wxButton *bSplitMeshes;
		wxButton *bMergeVertices;
		wxButton *bMakeBoundary;
		wxButton *bColorizeMeshes;
		wxButton *bColorizeElements;

		wxButton *bTranslate;
		wxButton *bScale;
		wxButton *bRotate;
		wxButton *bCentralize;

#if USE_TECGRAF
		wxButton *bBuildBoundaries;
#endif //#if USE_TECGRAF

		wxTextCtrl *tcInnerSurfaceSurfDirection;
		wxTextCtrl *tcSilhouetteAngle;
		wxTextCtrl *tcSurfacesAngle;
		wxTextCtrl *tcVisibilityVertex;
		wxTextCtrl *tcVisibilityEdge;
		wxTextCtrl *tcVisibilityBasedVertex;
		wxTextCtrl *tcSmoothFactor;
		wxTextCtrl *tcSwSizeProportion;
		wxTextCtrl *tcSwQualityThreshold;
		wxTextCtrl *tcSwMaxEdgesOnBoundary;
		wxTextCtrl *tcCSizeProportion;
		wxTextCtrl *tcCQualityThreshold;
		wxTextCtrl *tcSSizeProportion;
		wxTextCtrl *tcESizeProportion;
		wxTextCtrl *tcEQualityThreshold;
		wxTextCtrl *tcTolerance;
		wxTextCtrl *tcTranslate[3];
		wxTextCtrl *tcScale[3];
		wxTextCtrl *tcRotate[4];
		wxTextCtrl *tcCentralize;

		wxButton *bClear;

		wxMenuItem *miPickedShow;
		wxMenuItem *miPickedDeleteFirstVertex;
		wxMenuItem *miPickedDeleteFirstFrontElement;
		wxMenuItem *miPickedDeleteFirstElement;
		wxMenuItem *miPickedDeleteVertices;
		wxMenuItem *miPickedDeleteFrontElements;
		wxMenuItem *miPickedDeleteElements;
		wxMenuItem *miPickedDeleteSelected;
		wxMenuItem *miPickedMergeVertices;
		wxMenuItem *miPickedClearPickedList;

		wxTextCtrl *tcLog;

		wxProgressDialog *pdGlobal;
		wxProgressDialog *pdLocal;

		mutable bool useRedirector;
		mutable Redirector redirector;

	protected:

		struct MeshData
		{
			MeshData() : boundary(NULL), front(NULL), mesh(NULL)
#if USE_TECGRAF
			, model(NULL)
#endif //#if USE_TECGRAF
            , originalModule(NULL), createdMesh(false)
			{};

			Data::Boundary *boundary;
			Data::Front *front;
			Data::Mesh *mesh;
#if USE_TECGRAF
			TecGraf::Model *model;
#endif //#if USE_TECGRAF
            const MeshModule *originalModule;
            bool createdMesh;
			std::string filename;
		};

		typedef std::list<MeshData> MeshDataList;

		MeshDataList data;
		MeshDataList::iterator datum;

		//bool sharedMesh;
		//bool createdSharedMesh;
		mutable bool continuingConsistencyCheck;

		//Data::BSPTree *debug;

		Tools::ToolList tools;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_MESHES_MODULE_H_
