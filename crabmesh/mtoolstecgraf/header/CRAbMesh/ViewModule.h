#ifndef _CRAB_MESH_VIEW_MODULE_H_
#define _CRAB_MESH_VIEW_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include "CRAbMesh/Module.h"
#include "CRAbMesh/Frame.h"
#include <wx/clrpicker.h>
#include <wx/bmpcbox.h>

namespace CRAbMesh
{
	class ViewModule : public CRAbMesh::Module
	{
	friend class MeshModule;
	friend class BGColorDialog;

	public:
		///enums for configuration

		enum MouseOption
		{
			//MOUSE_INSERT = 1,
			MOUSE_PICK = 1,
			MOUSE_ROTATE,
			MOUSE_TURN,
			MOUSE_LOOK,
			MOUSE_TRANSLATE,
			MOUSE_MOVE,
			MOUSE_OPTICAL_ZOOM,
			MOUSE_DIGITAL_ZOOM,
			MOUSE_MERGED_ZOOM,
		};

		enum ViewOption
		{
			VIEW_FRONT = 1,
			VIEW_BACK,
			VIEW_RIGHT,
			VIEW_LEFT,
			VIEW_TOP,
			VIEW_BOTTOM,
			VIEW_DIAGONAL
		};

		enum AnimationOption
		{
			ANIMATION_NONE = 1,
			ANIMATION_ANY,
			ANIMATION_X,
			ANIMATION_Y,
			ANIMATION_Z,
		};

		enum ProjectionOption
		{
			PROJECTION_PERSPECTIVE = 1,
			PROJECTION_ORTHOGRAPHIC
		};

	public:

		ViewModule(Frame *frame = NULL, Canvas *canvas = NULL);
		virtual ~ViewModule();

		virtual wxString Name() const;

#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
		virtual wxMenu *GetModulesMenu(wxString &title) const;
#endif //#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
		virtual wxMenu *GetMouseMenu(wxString &title) const;
		virtual wxMenu *GetViewMenu(wxString &title) const;

		virtual wxAuiToolBar *GetMeshToolbar(wxString &title) const;

		using Module::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using Module::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using Module::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		virtual void SetMouseOption(enum MouseOption option);
		virtual enum MouseOption GetMouseOption() const;

		virtual void SetViewOption(enum ViewOption option);
		virtual enum ViewOption GetViewOption() const;

		virtual void SetAnimationOption(enum AnimationOption option);
		virtual enum AnimationOption GetAnimationOption() const;
		virtual void ToggleAnimation();
		virtual bool IsAnimationEnabled() const;

		virtual void SetProjectionOption(enum ProjectionOption option);
		virtual enum ProjectionOption GetProjectionOption() const;

		virtual void SetModuleOption(std::string option);
		virtual std::string GetModuleOption() const;

		virtual void SetStereoscopyEnabled(bool stereoscopyEnabled);
		virtual bool IsStereoscopyEnabled() const;

		virtual void SetDepthTestEnabled(bool depthTestEnabled);
		virtual bool IsDepthTestEnabled() const;

		virtual void SetCullFaceEnabled(bool cullFaceEnabled);
		virtual bool IsCullFaceEnabled() const;

		virtual void SetWireframeEnabled(bool wireframeEnabled);
		virtual bool IsWireframeEnabled() const;

		virtual void SetTriangleBorderEnabled(bool triangleBorderEnabled);
		virtual bool IsTriangleBorderEnabled() const;

		virtual void SetNormalByVertexEnabled(bool normalByVertexEnabled);
		virtual bool IsNormalByVertexEnabled() const;

		virtual void SetNormalInvertedEnabled(bool normalInvertedEnabled);
		virtual bool IsNormalInvertedEnabled() const;

		virtual void SetRenderAxesEnabled(bool renderAxesEnabled);
		virtual bool IsRenderAxesEnabled() const;

		virtual void SetRenderCrossbarEnabled(bool renderCrossbarEnabled);
		virtual bool IsRenderCrossbarEnabled() const;

		virtual void SetRenderIconEnabled(bool renderIconEnabled);
		virtual bool IsRenderIconEnabled() const;

		/*///do not forget to call auiManager.Update()
		virtual void SetWindowTreeEnabled(bool windowTreeEnabled);
		virtual bool IsWindowTreeEnabled() const;

		///do not forget to call auiManager.Update()
		virtual void SetWindowConfEnabled(bool windowConfEnabled);
		virtual bool IsWindowConfEnabled() const;*/

		virtual void AnimateCamera();

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void SetGUIState();

		virtual const Data::State *GetState(const std::string &option) const;

	protected:

		virtual unsigned int GetDimension() const;

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxMenu *MakeMouseMenu();
		virtual wxMenu *MakeModulesMenu();
		virtual wxMenu *MakeViewMenu();

		virtual wxAuiToolBar *MakeMeshToolbar();

		virtual void SetParameters(bool firstTime);

		virtual void SetOpenGLStateBeforeRendering(/*const Camera *camera*/) const;
		virtual bool BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const;

		virtual void RenderPick() const;
		virtual unsigned int PickBufferSize() const;
		virtual Data::DrawableList Picked(int hits, GLuint buffer[]);

		virtual void *Font(int font) const;
		virtual int Font(void *font) const;

	protected:

		virtual void OnMouseChanged(wxCommandEvent &event);
		virtual void OnMouseAnimation(wxCommandEvent &event);
		virtual void OnMouseView(wxCommandEvent &event);

		virtual void OnViewFullScreen(wxCommandEvent &event);
		virtual void OnViewStereoscopy(wxCommandEvent &event);
		virtual void OnViewProjection(wxCommandEvent &event);
		virtual void OnViewDepthTest(wxCommandEvent &event);
		virtual void OnViewCullFace(wxCommandEvent &event);
		virtual void OnViewWireframe(wxCommandEvent &event);
		virtual void OnViewTriangleBorder(wxCommandEvent &event);
		virtual void OnViewNormalByVertex(wxCommandEvent &event);
		virtual void OnViewNormalInverted(wxCommandEvent &event);
		virtual void OnViewRenderAxes(wxCommandEvent &event);
		virtual void OnViewRenderCrossbar(wxCommandEvent &event);
		virtual void OnViewRenderIcon(wxCommandEvent &event);
		virtual void OnViewColors(wxCommandEvent &event);
		//virtual void OnWindowTree(wxCommandEvent &event);
		//virtual void OnWindowConf(wxCommandEvent &event);

		virtual void OnToolMeshVertexDrawn(wxCommandEvent &event);
		virtual void OnToolMeshFrontElementDrawn(wxCommandEvent &event);
		virtual void OnToolMeshElementDrawn(wxCommandEvent &event);
		virtual void OnToolMeshShrink(wxCommandEvent &event);
		virtual void OnSliderMeshShrink(wxScrollEvent &event);
		virtual void OnToolMeshExplode(wxCommandEvent &event);
		virtual void OnSliderMeshExplode(wxScrollEvent &event);
		virtual void OnToolMeshExplodeExternal(wxCommandEvent &event);
		virtual void OnSliderMeshExplodeExternal(wxScrollEvent &event);
		virtual void OnToolMeshIdDrawn(wxCommandEvent &event);
		virtual void OnSliderMeshIdDrawn(wxScrollEvent &event);
		virtual void OnToolMeshPointSize(wxCommandEvent &event);
		virtual void OnSliderMeshPointSize(wxScrollEvent &event);
		virtual void OnToolMeshLineWidth(wxCommandEvent &event);
		virtual void OnSliderMeshLineWidth(wxScrollEvent &event);
		virtual void OnSelectPlane(wxCommandEvent &event);
		virtual void OnCheckPlaneEnabled(wxCommandEvent &event);
		virtual void OnSliderPlaneCoord(wxScrollEvent &event);
		virtual void OnCheckInPlane(wxCommandEvent &event);
		virtual void OnToolMeshQualityRange(wxCommandEvent &event);
		virtual void OnSliderMeshQualityRangeMin(wxScrollEvent &event);
		virtual void OnSliderMeshQualityRangeMax(wxScrollEvent &event);
		virtual void OnToolMeshVertexPickable(wxCommandEvent &event);
		virtual void OnToolMeshFrontElementPickable(wxCommandEvent &event);
		virtual void OnToolMeshElementPickable(wxCommandEvent &event);

		virtual void OnModule(wxCommandEvent &event);
		virtual void OnModuleNext(wxCommandEvent &event);
		virtual void OnModulePrevious(wxCommandEvent &event);

	protected:

		virtual void EnablePlaneSlider(Data::UInt selection);

	protected:

		//wxMenuItem *miMouseInsert;
		wxMenuItem *miMousePick;
		wxMenuItem *miMouseRotate;
		wxMenuItem *miMouseTurn;
		wxMenuItem *miMouseLook;
		wxMenuItem *miMouseTranslate;
		wxMenuItem *miMouseMove;
		wxMenuItem *miMouseOpticalZoom;
		wxMenuItem *miMouseDigitalZoom;
		wxMenuItem *miMouseMergedZoom;
		wxMenuItem *miMouseNoAnimation;
		wxMenuItem *miMouseAnimation;
		wxMenuItem *miMouseAnimationX;
		wxMenuItem *miMouseAnimationY;
		wxMenuItem *miMouseAnimationZ;
		wxMenuItem *miMouseFront;
		wxMenuItem *miMouseBack;
		wxMenuItem *miMouseRight;
		wxMenuItem *miMouseLeft;
		wxMenuItem *miMouseTop;
		wxMenuItem *miMouseBottom;
		wxMenuItem *miMouseDiagonal;

		wxMenuItem *miViewFullScreen;
		wxMenuItem *miViewStereoscopy;
		wxMenuItem *miViewPerspective;
		wxMenuItem *miViewOrthographic;
		wxMenuItem *miViewDepthTest;
		wxMenuItem *miViewCullFace;
		wxMenuItem *miViewWireframe;
		wxMenuItem *miViewTriangleBorder;
		wxMenuItem *miViewNormalByVertex;
		wxMenuItem *miViewNormalInverted;
		wxMenuItem *miViewRenderAxes;
		wxMenuItem *miViewRenderCrossbar;
		wxMenuItem *miViewRenderIcon;
		//wxMenuItem *miViewWindowTree;
		//wxMenuItem *miViewWindowConf;
		wxMenuItem *miViewColors;

		wxMenuItem *miModulesNext;
		wxMenuItem *miModulesPrevious;
		wxMenuItem **miModules;

		wxAuiToolBarItem *tbiVertexDrawn;
		wxAuiToolBarItem *tbiFrontElementDrawn;
		wxAuiToolBarItem *tbiElementDrawn;
		wxAuiToolBarItem *tbiIdDrawn;
		wxSlider *sIdDrawn;
		wxAuiToolBarItem *tbiPointSize;
		wxSlider *sPointSize;
		wxAuiToolBarItem *tbiLineWidth;
		wxSlider *sLineWidth;
		wxAuiToolBarItem *tbiShrink;
		wxSlider *sShrink;
		wxAuiToolBarItem *tbiExplode;
		wxSlider *sExplode;
		wxAuiToolBarItem *tbiExplodeExternal;
		wxSlider *sExplodeExternal;
		wxBitmapComboBox *bcbPlane;
		wxCheckBox *cbPlaneEnabled;
		wxSlider *sPlaneCoord;
		wxCheckBox *cbInPlane;
		wxAuiToolBarItem *tbiQualityRange;
		wxSlider *sQualityRangeMin;
		wxSlider *sQualityRangeMax;
		wxAuiToolBarItem *tbiVertexPickable;
		wxAuiToolBarItem *tbiFrontElementPickable;
		wxAuiToolBarItem *tbiElementPickable;

		typedef std::map<std::string, Data::State> ModuleStateMap;

		ModuleStateMap states;

	protected:
		///attributes for configuration

		enum MouseOption mouseOption;
		enum ViewOption viewOption;
		enum AnimationOption animationOption;
		enum ProjectionOption projectionOption;
		std::string moduleOption;

		bool stereoscopyEnabled;

		bool depthTestEnabled;
		bool cullFaceEnabled;
		bool wireframeEnabled;
		bool triangleBorderEnabled;
		bool normalByVertexEnabled;
		bool normalInvertedEnabled;

		bool renderAxesEnabled;
		bool renderCrossbarEnabled;
		bool renderIconEnabled;

		double defaultPointSize;
		double defaultLineWidth;

		//bool windowTreeEnabled;
		//bool windowConfEnabled;

	protected:

		Data::BSPTree *debug;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_VIEW_MODULE_H_
