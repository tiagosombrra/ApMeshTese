#include "CRAbMesh/ViewModule.h"

#if USE_GUI

#include "CRAbMesh/Camera.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/MeshModule.h"
#include "CRAbMesh/ColorDialog.h"
#include "Data/State.h"
#include "Data/Point3D.h"
#include "Data/Edge.h"
#include "Data/Mesh.h"

#include "point_png.cpp"
#include "edge_png.cpp"
#include "triangle_png.cpp"
#include "identifiers_png.cpp"
#include "point_size_png.cpp"
#include "line_width_png.cpp"
#include "shrink_png.cpp"
#include "explode_png.cpp"
#include "pexplode_png.cpp"
#include "quality_png.cpp"
#include "pick_point_png.cpp"
#include "pick_edge_png.cpp"
#include "pick_triangle_png.cpp"
#include "plane_right_png.cpp"
#include "plane_left_png.cpp"
#include "plane_top_png.cpp"
#include "plane_bottom_png.cpp"
#include "plane_front_png.cpp"
#include "plane_back_png.cpp"

CRAbMesh::ViewModule::ViewModule(Frame *frame, Canvas *canvas) :
	CRAbMesh::Module(frame, canvas)
{
	this->mouseOption = MOUSE_ROTATE;
	this->viewOption = VIEW_FRONT;
	this->animationOption = ANIMATION_NONE;
	this->projectionOption = PROJECTION_PERSPECTIVE;
	this->moduleOption = moduleNames[0];

	this->stereoscopyEnabled = false;

	this->depthTestEnabled = false;
	this->cullFaceEnabled = false;
	this->wireframeEnabled = true;
	this->triangleBorderEnabled = true;
	this->normalByVertexEnabled = false;
	this->normalInvertedEnabled = true;

	this->renderAxesEnabled = true;
	this->renderCrossbarEnabled = true;
	this->renderIconEnabled = true;

	//this->windowTreeEnabled = true;
	//this->windowConfEnabled = true;

	this->states[moduleNames[0]] = Data::State();

	this->defaultPointSize = this->states[moduleNames[0]].getPointSize();
	this->defaultLineWidth = this->states[moduleNames[0]].getLineWidth();

	this->MakeMenus();
	this->MakeToolbars();
}

CRAbMesh::ViewModule::~ViewModule()
{
	delete [] this->miModules;
}

wxString CRAbMesh::ViewModule::Name() const
{
	return _("");
}

#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
wxMenu *CRAbMesh::ViewModule::GetModulesMenu(wxString &title) const
{
	return this->GetMenu(0, title);
}
#endif //#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))

wxMenu *CRAbMesh::ViewModule::GetMouseMenu(wxString &title) const
{
#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	return this->GetMenu(0, title);
#else
	return this->GetMenu(1, title);
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
}

wxMenu *CRAbMesh::ViewModule::GetViewMenu(wxString &title) const
{
#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	return this->GetMenu(1, title);
#else
	return this->GetMenu(2, title);
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
}

wxAuiToolBar *CRAbMesh::ViewModule::GetMeshToolbar(wxString &title) const
{
	return this->GetToolbar(0, title);
}

wxMenu *CRAbMesh::ViewModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
	case 0:
		title = _("&Modules");
		break;
#endif //#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	case 0:
#else
	case 1:
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
		title = _("M&ouse");
		break;
#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
	case 1:
#else
	case 2:
#endif //#if ((USE_ONLY_MTOOLS == 2) || (USE_ONLY_MTOOLS == 3))
		title = _("&View");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::ViewModule::GetToolbar(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("M&esh modifiers");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->toolbars.size()) ? this->toolbars[i] : NULL;
}

wxPanel *CRAbMesh::ViewModule::GetPanel(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

bool CRAbMesh::ViewModule::LoadFile(const std::string &/*filename*/)
{
	return false;
}

bool CRAbMesh::ViewModule::SaveFile(const std::string &/*filename*/)
{
	return false;
}

void CRAbMesh::ViewModule::SetMouseOption(enum CRAbMesh::ViewModule::MouseOption option)
{
	this->mouseOption = option;

	switch (this->mouseOption)
	{
	/*case MOUSE_INSERT:
		this->miMouseInsert->Check(true);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;*/
	case MOUSE_PICK:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(true);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_ROTATE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(true);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_TURN:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(true);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_LOOK:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(true);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_TRANSLATE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(true);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_MOVE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(true);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_OPTICAL_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(true);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_DIGITAL_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(true);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_MERGED_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(true);
		break;
	}
}

enum CRAbMesh::ViewModule::MouseOption CRAbMesh::ViewModule::GetMouseOption() const
{
	return this->mouseOption;
}

void CRAbMesh::ViewModule::SetViewOption(enum CRAbMesh::ViewModule::ViewOption option)
{
	this->viewOption = option;

	this->canvas->ResetView();
}

enum CRAbMesh::ViewModule::ViewOption CRAbMesh::ViewModule::GetViewOption() const
{
	return this->viewOption;
}

void CRAbMesh::ViewModule::SetAnimationOption(enum CRAbMesh::ViewModule::AnimationOption option)
{
	this->animationOption = option;

	switch (this->animationOption)
	{
	case ANIMATION_NONE:
		this->miMouseNoAnimation->Check(true);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_ANY:
		this->miMouseAnimation->Check(true);
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_X:
		this->miMouseAnimationX->Check(true);
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_Y:
		this->miMouseAnimationY->Check(true);
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_Z:
		this->miMouseAnimationZ->Check(true);
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		break;
	}

	Data::getState().setAnimating(this->animationOption != ANIMATION_NONE);

	this->canvas->Refresh(false);
	this->canvas->Update();
}

enum CRAbMesh::ViewModule::AnimationOption CRAbMesh::ViewModule::GetAnimationOption() const
{
	return this->animationOption;
}

void CRAbMesh::ViewModule::ToggleAnimation()
{
	/*Data::getState().setAnimating(!this->IsAnimationEnabled());

	this->miMouseAnimation->Check(this->IsAnimationEnabled());

	this->canvas->Refresh(false);
	this->canvas->Update();*/

	this->SetAnimationOption(this->IsAnimationEnabled() ? ANIMATION_NONE : ANIMATION_ANY);
}

bool CRAbMesh::ViewModule::IsAnimationEnabled() const
{
	return Data::getState().isAnimating();
}

void CRAbMesh::ViewModule::SetProjectionOption(enum CRAbMesh::ViewModule::ProjectionOption option)
{
	this->projectionOption = option;

	switch (this->projectionOption)
    {
	case PROJECTION_PERSPECTIVE:
		this->miViewPerspective->Check(true);
		this->miViewOrthographic->Check(false);
		break;
	case PROJECTION_ORTHOGRAPHIC:
		this->miViewPerspective->Check(false);
		this->miViewOrthographic->Check(true);
		break;
    }

    this->canvas->ResetProjection();
}

enum CRAbMesh::ViewModule::ProjectionOption CRAbMesh::ViewModule::GetProjectionOption() const
{
	return this->projectionOption;
}

void CRAbMesh::ViewModule::SetModuleOption(std::string option)
{
	this->states[this->moduleOption] = Data::getState();

	if (this->states.find(option) == this->states.end())
	{
		this->states[option] = Data::getState();

		this->states[option].setBox(Data::Box());
		this->states[option].setMid(Data::Point3D());

		for (UInt i = 0; i < Data::State::COUNT; i++)
		{
			this->states[option].disablePlane(static_cast<Data::State::PlanePosition>(i));
		}

		this->bcbPlane->SetSelection(0);

		wxCommandEvent evt;

		this->OnSelectPlane(evt);
	}

	this->moduleOption = option;

	Data::setState(this->states[option]);

	this->frame->SetCurrentModule(this->moduleOption);

	wxString s(this->moduleOption.c_str(), wxMBConvLibc());

	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		this->Disconnect(this->miModules[i]->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnModule));

		this->miModules[i]->Check((this->miModules[i]->GetItemLabelText() == s));

		this->Connect(this->miModules[i]->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnModule));
	}

	this->SetGUIState();
}

std::string CRAbMesh::ViewModule::GetModuleOption() const
{
	return this->moduleOption;
}

void CRAbMesh::ViewModule::SetStereoscopyEnabled(bool stereoscopyEnabled)
{
	this->stereoscopyEnabled = stereoscopyEnabled;

	this->miViewStereoscopy->Check(this->stereoscopyEnabled);

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsStereoscopyEnabled() const
{
	return this->stereoscopyEnabled;
}

void CRAbMesh::ViewModule::SetDepthTestEnabled(bool depthTestEnabled)
{
	this->depthTestEnabled = depthTestEnabled;

	this->miViewDepthTest->Check(this->depthTestEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsDepthTestEnabled() const
{
	return this->depthTestEnabled;
}

void CRAbMesh::ViewModule::SetCullFaceEnabled(bool cullFaceEnabled)
{
	this->cullFaceEnabled = cullFaceEnabled;

	this->miViewCullFace->Check(this->cullFaceEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsCullFaceEnabled() const
{
	return this->cullFaceEnabled;
}

void CRAbMesh::ViewModule::SetWireframeEnabled(bool wireframeEnabled)
{
	this->wireframeEnabled = wireframeEnabled;

	this->miViewWireframe->Check(this->wireframeEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsWireframeEnabled() const
{
	return this->wireframeEnabled;
}

void CRAbMesh::ViewModule::SetTriangleBorderEnabled(bool triangleBorderEnabled)
{
	this->triangleBorderEnabled = triangleBorderEnabled;

	Data::getState().setTriangleBorderDrawn(this->triangleBorderEnabled);

	this->miViewTriangleBorder->Check(this->triangleBorderEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsTriangleBorderEnabled() const
{
	return this->triangleBorderEnabled;
}

void CRAbMesh::ViewModule::SetNormalByVertexEnabled(bool normalByVertexEnabled)
{
	this->normalByVertexEnabled = normalByVertexEnabled;

	Data::getState().setNormalByVertex(this->normalByVertexEnabled);

	this->miViewNormalByVertex->Check(this->normalByVertexEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsNormalByVertexEnabled() const
{
	return this->normalByVertexEnabled;
}

void CRAbMesh::ViewModule::SetNormalInvertedEnabled(bool normalInvertedEnabled)
{
	this->normalInvertedEnabled = normalInvertedEnabled;

	Data::getState().setNormalInverted(this->normalInvertedEnabled);

	this->miViewNormalInverted->Check(this->normalInvertedEnabled);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsNormalInvertedEnabled() const
{
	return this->normalInvertedEnabled;
}

void CRAbMesh::ViewModule::SetRenderAxesEnabled(bool renderAxesEnabled)
{
	this->renderAxesEnabled = renderAxesEnabled;

	this->miViewRenderAxes->Check(this->renderAxesEnabled);

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsRenderAxesEnabled() const
{
	return this->renderAxesEnabled;
}

void CRAbMesh::ViewModule::SetRenderCrossbarEnabled(bool renderCrossbarEnabled)
{
	this->renderCrossbarEnabled = renderCrossbarEnabled;

	this->miViewRenderCrossbar->Check(this->renderCrossbarEnabled);

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsRenderCrossbarEnabled() const
{
	return this->renderCrossbarEnabled;
}

void CRAbMesh::ViewModule::SetRenderIconEnabled(bool renderIconEnabled)
{
	this->renderIconEnabled = renderIconEnabled;

	this->miViewRenderIcon->Check(this->renderIconEnabled);

	this->canvas->Refresh(false);
}

bool CRAbMesh::ViewModule::IsRenderIconEnabled() const
{
	return this->renderIconEnabled;
}

/*void CRAbMesh::ViewModule::SetWindowTreeEnabled(bool windowTreeEnabled)
{
	this->windowTreeEnabled = windowTreeEnabled;

	this->frame->UpdateConfiguration();
}

bool CRAbMesh::ViewModule::IsWindowTreeEnabled() const
{
	return this->windowTreeEnabled;
}

void CRAbMesh::ViewModule::SetWindowConfEnabled(bool windowConfEnabled)
{
	this->windowConfEnabled = windowConfEnabled;

	this->frame->UpdateConfiguration();
}

bool CRAbMesh::ViewModule::IsWindowConfEnabled() const
{
	return this->windowConfEnabled;
}*/

unsigned int CRAbMesh::ViewModule::GetDimension() const
{
	return 3;
}

void CRAbMesh::ViewModule::MakeMainDrive()
{
	this->drive = NULL;
}

void CRAbMesh::ViewModule::MakeMenus()
{
#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
	this->menus.push_back(this->MakeModulesMenu());
#endif //#if ((USE_ONLY_MTOOLS != 2) && (USE_ONLY_MTOOLS != 3))
	this->menus.push_back(this->MakeMouseMenu());
	this->menus.push_back(this->MakeViewMenu());
}

void CRAbMesh::ViewModule::MakeToolbars()
{
	this->toolbars.push_back(this->MakeMeshToolbar());
}

void CRAbMesh::ViewModule::MakePanels()
{
	//Module::MakePanels();
}

wxMenu *CRAbMesh::ViewModule::MakeMouseMenu()
{
	wxMenu *mMouse = new wxMenu();
	//this->miMouseInsert      = mMouse->AppendRadioItem(wxID_ANY, _("Insert\t1"),           _("Insert points"));
    this->miMousePick        = mMouse->AppendRadioItem(wxID_ANY, _("Pick\t1"),          _("Pick objects"));
    //mMouse->AppendSeparator();
    this->miMouseRotate      = mMouse->AppendRadioItem(wxID_ANY, _("&Rotate\t2"),       _("Rotate objects"));
    this->miMouseTurn        = mMouse->AppendRadioItem(wxID_ANY, _("T&urn\t3"),         _("Turn objects"));
    this->miMouseLook        = mMouse->AppendRadioItem(wxID_ANY, _("&Look\t4"),         _("Look objects"));
    this->miMouseTranslate   = mMouse->AppendRadioItem(wxID_ANY, _("&Translate\t5"),    _("Translate objects"));
    this->miMouseMove        = mMouse->AppendRadioItem(wxID_ANY, _("&Move\t6"),         _("Move camera front/back"));
    this->miMouseOpticalZoom = mMouse->AppendRadioItem(wxID_ANY, _("&Optical zoom\t7"), _("Optical zoom"));
    this->miMouseDigitalZoom = mMouse->AppendRadioItem(wxID_ANY, _("&Digital zoom\t8"), _("Digital zoom"));
    this->miMouseMergedZoom  = mMouse->AppendRadioItem(wxID_ANY, _("Merged &zoom\t9"),  _("Merged zoom"));
    mMouse->AppendSeparator();
    this->miMouseFront       = mMouse->Append(wxID_ANY, _("Front\tCtrl+2"),     _("Front View"));
    this->miMouseBack        = mMouse->Append(wxID_ANY, _("Back\tCtrl+3"),      _("Back View"));
    this->miMouseRight       = mMouse->Append(wxID_ANY, _("Right\tCtrl+4"),     _("Right View"));
    this->miMouseLeft        = mMouse->Append(wxID_ANY, _("Left\tCtrl+5"),      _("Left View"));
    this->miMouseTop         = mMouse->Append(wxID_ANY, _("Top\tCtrl+6"),       _("Top View"));
    this->miMouseBottom      = mMouse->Append(wxID_ANY, _("Bottom\tCtrl+7"),    _("Bottom View"));
    this->miMouseDiagonal    = mMouse->Append(wxID_ANY, _("Diagonal\tCtrl+8"),  _("Diagonal View"));
    mMouse->AppendSeparator();
    this->miMouseNoAnimation = mMouse->AppendRadioItem(wxID_ANY, _("Do not animate camera"), _("Do not animate camera"));
    this->miMouseAnimation   = mMouse->AppendRadioItem(wxID_ANY, _("Animate camera"),        _("Animate camera with a rotation"));
    this->miMouseAnimationX  = mMouse->AppendRadioItem(wxID_ANY, _("Animate camera (X)"),    _("Animate camera with a rotation around X axis"));
    this->miMouseAnimationY  = mMouse->AppendRadioItem(wxID_ANY, _("Animate camera (Y)"),    _("Animate camera with a rotation around Y axis"));
    this->miMouseAnimationZ  = mMouse->AppendRadioItem(wxID_ANY, _("Animate camera (Z)"),    _("Animate camera with a rotation around Z axis"));

    switch (this->GetMouseOption())
    {
	/*case MOUSE_INSERT:
		this->miMouseInsert->Check(true);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;*/
	case MOUSE_PICK:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(true);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_ROTATE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(true);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_TURN:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(true);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_LOOK:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(true);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_TRANSLATE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(true);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_MOVE:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(true);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_OPTICAL_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(true);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_DIGITAL_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(true);
		this->miMouseMergedZoom->Check(false);
		break;
	case MOUSE_MERGED_ZOOM:
		//this->miMouseInsert->Check(false);
		this->miMousePick->Check(false);
		this->miMouseRotate->Check(false);
		this->miMouseTurn->Check(false);
		this->miMouseLook->Check(false);
		this->miMouseTranslate->Check(false);
		this->miMouseMove->Check(false);
		this->miMouseOpticalZoom->Check(false);
		this->miMouseDigitalZoom->Check(false);
		this->miMouseMergedZoom->Check(true);
		break;
    }

    switch (this->GetAnimationOption())
	{
	case ANIMATION_NONE:
		this->miMouseNoAnimation->Check(true);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_ANY:
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(true);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_X:
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(true);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_Y:
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(true);
		this->miMouseAnimationZ->Check(false);
		break;
	case ANIMATION_Z:
		this->miMouseNoAnimation->Check(false);
		this->miMouseAnimation->Check(false);
		this->miMouseAnimationX->Check(false);
		this->miMouseAnimationY->Check(false);
		this->miMouseAnimationZ->Check(true);
		break;
	}

    //this->Connect(this->miMouseInsert->GetId(),      wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMousePick->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseRotate->GetId(),      wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseTurn->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseLook->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseTranslate->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseMove->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseOpticalZoom->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseDigitalZoom->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));
    this->Connect(this->miMouseMergedZoom->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseChanged));

    this->Connect(this->miMouseNoAnimation->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseAnimation));
    this->Connect(this->miMouseAnimation->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseAnimation));
    this->Connect(this->miMouseAnimationX->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseAnimation));
    this->Connect(this->miMouseAnimationY->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseAnimation));
    this->Connect(this->miMouseAnimationZ->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseAnimation));

    this->Connect(this->miMouseFront->GetId(),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseBack->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseRight->GetId(),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseLeft->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseTop->GetId(),         wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseBottom->GetId(),      wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));
    this->Connect(this->miMouseDiagonal->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnMouseView));

    return mMouse;
}

wxMenu *CRAbMesh::ViewModule::MakeModulesMenu()
{
	wxMenu *mModules = new wxMenu();

	this->miModules = new wxMenuItem*[MODULES_COUNT];

	this->miModulesNext = mModules->Append(wxID_ANY, _("Next module\tCtrl+Right"));
	this->miModulesPrevious = mModules->Append(wxID_ANY, _("Previous module\tCtrl+Left"));

	mModules->AppendSeparator();

	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		wxString name(moduleNames[i].c_str(), wxMBConvLibc());

		if (i < 8)
		{
			name << _("\tF") << (i + 2);
		}
		else if (i < 10)
		{
			name << _("\tF") << (i + 3);
		}
		else if (i < 21)
		{
			name << _("\tCtrl+F") << (i - 8);
		}

		this->miModules[i] = mModules->AppendRadioItem(wxID_ANY, name);

		this->Connect(this->miModules[i]->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnModule));
	}

	this->Connect(this->miModulesNext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnModuleNext));
	this->Connect(this->miModulesPrevious->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnModulePrevious));

	return mModules;
}

wxMenu *CRAbMesh::ViewModule::MakeViewMenu()
{
	wxMenu *mView = new wxMenu();
    this->miViewFullScreen      = mView->AppendCheckItem(wxID_ANY, _("Full screen\tCtrl+F"));
    this->miViewStereoscopy     = mView->AppendCheckItem(wxID_ANY, _("Stereoscopy\tCtrl+T"));
    mView->AppendSeparator();
    this->miViewPerspective     = mView->AppendRadioItem(wxID_ANY, _("Perspective projection"));
    this->miViewOrthographic    = mView->AppendRadioItem(wxID_ANY, _("Orthographic projection"));
    mView->AppendSeparator();
    this->miViewDepthTest       = mView->AppendCheckItem(wxID_ANY, _("Depth test\tCtrl+D"));
    this->miViewCullFace        = mView->AppendCheckItem(wxID_ANY, _("Face culling\tCtrl+C"));
    this->miViewWireframe       = mView->AppendCheckItem(wxID_ANY, _("Wireframe\tCtrl+W"));
    this->miViewTriangleBorder  = mView->AppendCheckItem(wxID_ANY, _("Front element border"));
    this->miViewNormalByVertex  = mView->AppendCheckItem(wxID_ANY, _("Normals by vertex"));
    this->miViewNormalInverted  = mView->AppendCheckItem(wxID_ANY, _("Inverted normals"));
    mView->AppendSeparator();
    this->miViewRenderAxes      = mView->AppendCheckItem(wxID_ANY, _("Render axes\tCtrl+A"));
    this->miViewRenderCrossbar  = mView->AppendCheckItem(wxID_ANY, _("Render crossbar\tCtrl+B"));
    this->miViewRenderIcon      = mView->AppendCheckItem(wxID_ANY, _("Render icon\tCtrl+I"));
    /*mView->AppendSeparator();
    this->miViewWindowTree  = mView->AppendCheckItem(wxID_ANY, _("Elements tree window\tF2"));
    this->miViewWindowConf  = mView->AppendCheckItem(wxID_ANY, _("Configuration window\tF3"));*/
    mView->AppendSeparator();
    this->miViewColors = mView->Append(wxID_ANY, _("Colors..."));

    this->miViewFullScreen->Check(this->frame->IsFullScreen());
    this->miViewStereoscopy->Check(this->IsStereoscopyEnabled());
    this->miViewDepthTest->Check(this->IsDepthTestEnabled());
    this->miViewCullFace->Check(this->IsCullFaceEnabled());
    this->miViewWireframe->Check(this->IsWireframeEnabled());
    this->miViewTriangleBorder->Check(this->IsTriangleBorderEnabled());
    this->miViewNormalByVertex->Check(this->IsNormalByVertexEnabled());
    this->miViewNormalInverted->Check(this->IsNormalInvertedEnabled());
    this->miViewRenderAxes->Check(this->IsRenderAxesEnabled());
    this->miViewRenderCrossbar->Check(this->IsRenderCrossbarEnabled());
    this->miViewRenderIcon->Check(this->IsRenderIconEnabled());

    switch (this->GetProjectionOption())
    {
	case PROJECTION_PERSPECTIVE:
		this->miViewPerspective->Check(true);
		this->miViewOrthographic->Check(false);
		break;
	case PROJECTION_ORTHOGRAPHIC:
		this->miViewPerspective->Check(false);
		this->miViewOrthographic->Check(true);
		break;
    }

    this->Connect(this->miViewFullScreen->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewFullScreen));
    this->Connect(this->miViewStereoscopy->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewStereoscopy));

    this->Connect(this->miViewPerspective->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewProjection));
    this->Connect(this->miViewOrthographic->GetId(),   wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewProjection));

    this->Connect(this->miViewDepthTest->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewDepthTest));
    this->Connect(this->miViewCullFace->GetId(),      wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewCullFace));
    this->Connect(this->miViewWireframe->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewWireframe));
    this->Connect(this->miViewTriangleBorder->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewTriangleBorder));
    this->Connect(this->miViewNormalByVertex->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewNormalByVertex));
    this->Connect(this->miViewNormalInverted->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewNormalInverted));

    this->Connect(this->miViewRenderAxes->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewRenderAxes));
    this->Connect(this->miViewRenderCrossbar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewRenderCrossbar));
    this->Connect(this->miViewRenderIcon->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewRenderIcon));

    //this->Connect(this->miViewWindowTree->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewWindowTree));
    //this->Connect(this->miViewWindowConf->GetId(),  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewWindowConf));

    this->Connect(this->miViewColors->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ViewModule::OnViewColors));

    return mView;
}

wxAuiToolBar *CRAbMesh::ViewModule::MakeMeshToolbar()
{
	wxAuiToolBar *tbMesh = new wxAuiToolBar(this->frame, wxID_ANY);

	wxSize s(16, 16);

	tbMesh->SetToolBitmapSize(s);

	wxImage img;
	wxBitmap bmp;

	//img.LoadFile(_("../../images/point.png"));
	img = IMAGE_PNG_FROM_DATA(point);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Draw vertices"), bmp, _("Draw vertices"), wxITEM_CHECK);
	this->tbiVertexDrawn = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	//img.LoadFile(_("../../images/edge.png"));
	img = IMAGE_PNG_FROM_DATA(edge);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Draw front elements"), bmp, _("Draw front elements"), wxITEM_CHECK);
	this->tbiFrontElementDrawn = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	//img.LoadFile(_("../../images/triangle.png"));
	img = IMAGE_PNG_FROM_DATA(triangle);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Draw elements"), bmp, _("Draw elements"), wxITEM_CHECK);
	this->tbiElementDrawn = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	//img.LoadFile(_("../../images/identifiers.png"));
	img = IMAGE_PNG_FROM_DATA(identifiers);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Draw ids"), bmp, _("Draw mesh identifiers"), wxITEM_CHECK);
	this->tbiIdDrawn = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sIdDrawn   = new wxSlider(tbMesh, wxID_ANY, std::max(this->Font(Data::getState().getGlutFont()), 1), 1, 7, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sIdDrawn);

	img = IMAGE_PNG_FROM_DATA(point_size);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Point size"), bmp, _("Change point size"), wxITEM_CHECK);
	this->tbiPointSize = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sPointSize   = new wxSlider(tbMesh, wxID_ANY, static_cast<int>(this->defaultPointSize), 1, 10, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sPointSize);

	img = IMAGE_PNG_FROM_DATA(line_width);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Line width"), bmp, _("Change line width"), wxITEM_CHECK);
	this->tbiLineWidth = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sLineWidth   = new wxSlider(tbMesh, wxID_ANY, static_cast<int>(this->defaultLineWidth), 1, 5, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sLineWidth);

	tbMesh->AddSeparator();

	//img.LoadFile(_("../../images/shrink.png"));
	img = IMAGE_PNG_FROM_DATA(shrink);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Shrink"), bmp, _("Shrink mesh elements"), wxITEM_CHECK);
	this->tbiShrink = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sShrink   = new wxSlider(tbMesh, wxID_ANY, 0, -90, 50, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sShrink);

	//img.LoadFile(_("../../images/explode.png"));
	img = IMAGE_PNG_FROM_DATA(explode);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Explode"), bmp, _("Explode mesh elements"), wxITEM_CHECK);
	this->tbiExplode = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sExplode   = new wxSlider(tbMesh, wxID_ANY, 0, -50, 100, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sExplode);

	//img.LoadFile(_("../../images/pexplode.png"));
	img = IMAGE_PNG_FROM_DATA(pexplode);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Explode by partition"), bmp, _("Explode by partition"), wxITEM_CHECK);
	this->tbiExplodeExternal = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sExplodeExternal   = new wxSlider(tbMesh, wxID_ANY, 0, -50, 100, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sExplodeExternal);

	tbMesh->AddSeparator();

	//this->bcbPlane       = new wxBitmapComboBox(tbMesh, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(48, 20), 0, NULL, wxCB_READONLY);
	//this->bcbPlane       = new wxBitmapComboBox(tbMesh, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	this->bcbPlane       = new wxBitmapComboBox(tbMesh, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0, NULL, wxCB_READONLY);
	tbMesh->AddControl(this->bcbPlane);
	//this->cbPlaneEnabled = new wxCheckBox(tbMesh, wxID_ANY, _(""));
	this->cbPlaneEnabled = new wxCheckBox(tbMesh, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	tbMesh->AddControl(this->cbPlaneEnabled);
	this->sPlaneCoord    = new wxSlider(tbMesh, wxID_ANY, 0, 0, 1, wxDefaultPosition, wxSize(100, 16));
	tbMesh->AddControl(this->sPlaneCoord);
	this->cbInPlane      = new wxCheckBox(tbMesh, wxID_ANY, _("In plane"));
	tbMesh->AddControl(this->cbInPlane);

	tbMesh->AddSeparator();

	//img.LoadFile(_("../../images/quality.png"));
	img = IMAGE_PNG_FROM_DATA(quality);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	tbMesh->AddTool(wxID_ANY, _("Quality range"), bmp, _("Select quality range"), wxITEM_CHECK);
	this->tbiQualityRange = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);
	this->sQualityRangeMin = new wxSlider(tbMesh, wxID_ANY, 0, 0, 10, wxDefaultPosition, wxSize(50, 16), wxSL_HORIZONTAL);
	tbMesh->AddControl(this->sQualityRangeMin);
	this->sQualityRangeMax = new wxSlider(tbMesh, wxID_ANY, 10, 0, 10, wxDefaultPosition, wxSize(50, 16), wxSL_HORIZONTAL);
	tbMesh->AddControl(this->sQualityRangeMax);

	tbMesh->AddSeparator();

	//img.LoadFile(_("../../images/pick_point.png"));
	img = IMAGE_PNG_FROM_DATA(pick_point);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Pick vertices"), bmp, _("Pick vertices"), wxITEM_CHECK);
	this->tbiVertexPickable = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	//img.LoadFile(_("../../images/pick_edge.png"));
	img = IMAGE_PNG_FROM_DATA(pick_edge);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Pick front elements"), bmp, _("Pick front elements"), wxITEM_CHECK);
	this->tbiFrontElementPickable = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	//img.LoadFile(_("../../images/pick_triangle.png"));
	img = IMAGE_PNG_FROM_DATA(pick_triangle);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

    tbMesh->AddTool(wxID_ANY, _("Pick elements"), bmp, _("Pick elements"), wxITEM_CHECK);
	this->tbiElementPickable = tbMesh->FindToolByIndex(static_cast<int>(tbMesh->GetToolCount()) - 1);

	this->tbiVertexDrawn->SetLongHelp(this->tbiVertexDrawn->GetShortHelp());
	this->tbiFrontElementDrawn->SetLongHelp(this->tbiFrontElementDrawn->GetShortHelp());
	this->tbiElementDrawn->SetLongHelp(this->tbiElementDrawn->GetShortHelp());
	this->tbiIdDrawn->SetLongHelp(this->tbiIdDrawn->GetShortHelp());
	this->tbiPointSize->SetLongHelp(this->tbiPointSize->GetShortHelp());
	this->tbiLineWidth->SetLongHelp(this->tbiLineWidth->GetShortHelp());
	this->tbiShrink->SetLongHelp(this->tbiShrink->GetShortHelp());
	this->tbiExplode->SetLongHelp(this->tbiExplode->GetShortHelp());
	this->tbiExplodeExternal->SetLongHelp(this->tbiExplodeExternal->GetShortHelp());
	this->tbiQualityRange->SetLongHelp(this->tbiQualityRange->GetShortHelp());
	this->tbiVertexPickable->SetLongHelp(this->tbiVertexPickable->GetShortHelp());
	this->tbiFrontElementPickable->SetLongHelp(this->tbiFrontElementPickable->GetShortHelp());
	this->tbiElementPickable->SetLongHelp(this->tbiElementPickable->GetShortHelp());

	this->sShrink->Enable(tbMesh->GetToolToggled(this->tbiShrink->GetId()));
	this->sExplode->Enable(tbMesh->GetToolToggled(this->tbiExplode->GetId()));
	this->sExplodeExternal->Enable(tbMesh->GetToolToggled(this->tbiExplodeExternal->GetId()));
	this->sIdDrawn->Enable(tbMesh->GetToolToggled(this->tbiIdDrawn->GetId()));
	this->sPointSize->Enable(tbMesh->GetToolToggled(this->tbiPointSize->GetId()));
	this->sLineWidth->Enable(tbMesh->GetToolToggled(this->tbiLineWidth->GetId()));
	this->sPlaneCoord->Enable(false);
	this->sQualityRangeMin->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));
	this->sQualityRangeMax->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));

	//img.LoadFile(_("../../images/plane_right.png"));
	img = IMAGE_PNG_FROM_DATA(plane_right);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Right"), bmp);

	//img.LoadFile(_("../../images/plane_left.png"));
	img = IMAGE_PNG_FROM_DATA(plane_left);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Left"), bmp);

	//img.LoadFile(_("../../images/plane_top.png"));
	img = IMAGE_PNG_FROM_DATA(plane_top);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Top"), bmp);

	//img.LoadFile(_("../../images/plane_bottom.png"));
	img = IMAGE_PNG_FROM_DATA(plane_bottom);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Bottom"), bmp);

	//img.LoadFile(_("../../images/plane_front.png"));
	img = IMAGE_PNG_FROM_DATA(plane_front);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Front"), bmp);

	//img.LoadFile(_("../../images/plane_back.png"));
	img = IMAGE_PNG_FROM_DATA(plane_back);
    img.Rescale(s.GetWidth(), s.GetHeight(), wxIMAGE_QUALITY_HIGH);
    bmp = wxBitmap(img);

	this->bcbPlane->Append(_("Back"), bmp);

	this->bcbPlane->SetSelection(0);

	this->cbInPlane->SetValue(Data::getState().isPlaneInclusive());

	tbMesh->Realize();

	this->Connect(this->tbiVertexDrawn->GetId(),       wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshVertexDrawn));
	this->Connect(this->tbiFrontElementDrawn->GetId(), wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshFrontElementDrawn));
	this->Connect(this->tbiElementDrawn->GetId(),      wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshElementDrawn));
	this->Connect(this->tbiShrink->GetId(),            wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->sShrink->GetId(),              wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshShrink));
	this->Connect(this->tbiExplode->GetId(),           wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->sExplode->GetId(),             wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshExplode));
	this->Connect(this->tbiExplodeExternal->GetId(),   wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->sExplodeExternal->GetId(),     wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshExplodeExternal));
	this->Connect(this->tbiIdDrawn->GetId(),           wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->sIdDrawn->GetId(),             wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshIdDrawn));
	this->Connect(this->tbiPointSize->GetId(),         wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->sPointSize->GetId(),           wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshPointSize));
	this->Connect(this->tbiLineWidth->GetId(),         wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->sLineWidth->GetId(),           wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshLineWidth));
	this->Connect(this->bcbPlane->GetId(),             wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(ViewModule::OnSelectPlane));
	this->Connect(this->cbPlaneEnabled->GetId(),       wxEVT_COMMAND_CHECKBOX_CLICKED,  wxCommandEventHandler(ViewModule::OnCheckPlaneEnabled));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->sPlaneCoord->GetId(),          wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderPlaneCoord));
	this->Connect(this->cbInPlane->GetId(),            wxEVT_COMMAND_CHECKBOX_CLICKED,  wxCommandEventHandler(ViewModule::OnCheckInPlane));
	this->Connect(this->tbiQualityRange->GetId(),      wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshQualityRange));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMin->GetId(),     wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMin));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_TOP,                wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_BOTTOM,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_LINEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_LINEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_PAGEUP,             wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_PAGEDOWN,           wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_THUMBTRACK,         wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_THUMBRELEASE,       wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->sQualityRangeMax->GetId(),     wxEVT_SCROLL_CHANGED,            wxScrollEventHandler(ViewModule::OnSliderMeshQualityRangeMax));
	this->Connect(this->tbiVertexPickable->GetId(),    wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshVertexPickable));
	this->Connect(this->tbiFrontElementPickable->GetId(), wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshFrontElementPickable));
	this->Connect(this->tbiElementPickable->GetId(),   wxEVT_COMMAND_TOOL_CLICKED,      wxCommandEventHandler(ViewModule::OnToolMeshElementPickable));

	return tbMesh;
}

void CRAbMesh::ViewModule::SetParameters(bool /*firstTime*/)
{

}

void CRAbMesh::ViewModule::Execute()
{
	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		this->miModules[i]->Enable(false);
	}
}

void CRAbMesh::ViewModule::Ready(bool /*clearing*/, bool pending)
{
	//this->ClearDisplayLists();

	if (pending)
	{
		return;
	}

	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		this->miModules[i]->Enable(true);
	}
}

void CRAbMesh::ViewModule::Clear(bool onlyDrive, bool /*destroying*/)
{
	if (onlyDrive)
	{
		return;
	}

	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		this->miModules[i]->Enable(true);
	}
}

void CRAbMesh::ViewModule::ClearDisplayLists(bool force) const
{
	if ((this->frame) && (this->frame->GetCurrentModule()))
	{
		this->frame->GetCurrentModule()->ClearDisplayLists(force);
	}

	Data::getState().clearDisplayLists();
}

void CRAbMesh::ViewModule::SetGUIState()
{
	this->miViewTriangleBorder->Check(Data::getState().isTriangleBorderDrawn());
	this->miViewNormalByVertex->Check(Data::getState().isNormalByVertex());
	this->miViewNormalInverted->Check(Data::getState().isNormalInverted());

	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	tbMesh->ToggleTool(this->tbiVertexDrawn->GetId(), Data::getState().isVertexDrawn());

	tbMesh->ToggleTool(this->tbiFrontElementDrawn->GetId(), Data::getState().isFrontElementDrawn());

	tbMesh->ToggleTool(this->tbiElementDrawn->GetId(), Data::getState().isElementDrawn());

	tbMesh->ToggleTool(this->tbiIdDrawn->GetId(), Data::getState().isIdDrawn());
	this->sIdDrawn->Enable(tbMesh->GetToolToggled(this->tbiIdDrawn->GetId()));
	this->sIdDrawn->SetValue(std::max(this->Font(Data::getState().getGlutFont()), 1));

	tbMesh->ToggleTool(this->tbiPointSize->GetId(), Data::getState().getPointSize() != this->defaultPointSize);
	this->sPointSize->Enable(tbMesh->GetToolToggled(this->tbiPointSize->GetId()));
	this->sPointSize->SetValue(static_cast<int>(this->defaultPointSize));

	tbMesh->ToggleTool(this->tbiLineWidth->GetId(), Data::getState().getLineWidth() != this->defaultLineWidth);
	this->sLineWidth->Enable(tbMesh->GetToolToggled(this->tbiLineWidth->GetId()));
	this->sLineWidth->SetValue(static_cast<int>(this->defaultLineWidth));

	tbMesh->ToggleTool(this->tbiShrink->GetId(), (Data::getState().getShrink() != 1.0));
	this->sShrink->Enable(tbMesh->GetToolToggled(this->tbiShrink->GetId()));
	this->sShrink->SetValue(static_cast<int>(Data::getState().getShrink()*100.0 - 100.0));

	tbMesh->ToggleTool(this->tbiExplode->GetId(), (Data::getState().getExplode() != 1.0));
	this->sExplode->Enable(tbMesh->GetToolToggled(this->tbiExplode->GetId()));
	this->sExplode->SetValue(static_cast<int>(Data::getState().getExplode()*100.0 - 100.0));

	tbMesh->ToggleTool(this->tbiExplodeExternal->GetId(), (Data::getState().getExplodeExternal() != 1.0));
	this->sExplodeExternal->Enable(tbMesh->GetToolToggled(this->tbiExplodeExternal->GetId()));
	this->sExplodeExternal->SetValue(static_cast<int>(Data::getState().getExplodeExternal()*100.0 - 100.0));

	if (Data::getState().getBox().getMin())
	{
		this->cbPlaneEnabled->SetValue(Data::getState().isAnyPlaneEnabled());
		this->sPlaneCoord->Enable(this->cbPlaneEnabled->GetValue());
		this->sPlaneCoord->SetValue(static_cast<int>(Data::getState().getPlaneCoord(this->bcbPlane->GetSelection())*100.0));
		this->cbInPlane->SetValue(Data::getState().isPlaneInclusive());
	}
	else
	{
		this->cbPlaneEnabled->SetValue(false);
		this->sPlaneCoord->Enable(this->cbPlaneEnabled->GetValue());
		this->sPlaneCoord->SetValue(this->sPlaneCoord->GetMin());
		this->cbInPlane->SetValue(false);
	}

	tbMesh->ToggleTool(this->tbiQualityRange->GetId(), ((Data::getState().getQualityRangeMin() != 0.0) || (Data::getState().getQualityRangeMax() != 1.0)));
	this->sQualityRangeMin->SetValue(static_cast<int>(Data::getState().getQualityRangeMin()*(this->sQualityRangeMin->GetMax() - this->sQualityRangeMin->GetMin())));
	this->sQualityRangeMin->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));
	this->sQualityRangeMax->SetValue(static_cast<int>(Data::getState().getQualityRangeMax()*(this->sQualityRangeMax->GetMax() - this->sQualityRangeMax->GetMin())));
	this->sQualityRangeMax->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));

	tbMesh->ToggleTool(this->tbiVertexPickable->GetId(), Data::getState().isVertexPickable());
	tbMesh->ToggleTool(this->tbiFrontElementPickable->GetId(), Data::getState().isFrontElementPickable());
	tbMesh->ToggleTool(this->tbiElementPickable->GetId(), Data::getState().isElementPickable());

	if (this->frame->GetCurrentModule())
	{
		this->frame->GetCurrentModule()->SetGUIState();
	}
}

const Data::State *CRAbMesh::ViewModule::GetState(const std::string &option) const
{
    ModuleStateMap::const_iterator it = this->states.find(option);

    return (it == this->states.end()) ? NULL : &(*it).second;
}

void CRAbMesh::ViewModule::Render() const
{
	if (Data::getState().isAnyPlaneEnabled())
	{
		Data::Point3D min = *Data::getState().getBox().getMin();
		Data::Point3D max = *Data::getState().getBox().getMax();

		Data::Vector3D smin, smax;

		for (UInt i = 0; i < 3; i++)
		{
			smin.setCoord(i, max.getCoord(i) - min.getCoord(i));
			smax.setCoord(i, smin.getCoord(i));
		}

		smin.multiply(-0.01);
		smax.multiply(0.01);

		min.sum(smin);
		max.sum(smax);

		for (UInt i = 0; i < 3; i++)
		{
			if (Data::getState().isPlaneEnabled(2*i))
			{
				max.setCoord(i, Data::getState().getPlaneCoord(2*i));
			}

			if (Data::getState().isPlaneEnabled(2*i + 1))
			{
				min.setCoord(i, Data::getState().getPlaneCoord(2*i + 1));
			}
		}

		bool lighting = glIsEnabled(GL_LIGHTING);

		glDisable(GL_LIGHTING);

		Real shrink = Data::getState().getShrink();

		Data::getState().setShrink(Data::getState().getExplode());
		Data::getState().setShrink(Data::getState().getExplodeExternal());

		for (UInt i = 0; i < 3; i++)
		{
			if (Data::getState().isPlaneEnabled(2*i))
			{
				Data::Edge e1(max, max);
				Data::Edge e2(max, max);
				Data::Edge e3(min, min);
				Data::Edge e4(min, min);

				e1.getP2()->setCoord((i + 1)%3, min.getCoord((i + 1)%3));

				e2.getP1()->setCoord((i + 2)%3, min.getCoord((i + 2)%3));

				e3.getP1()->setCoord((i + 0)%3, max.getCoord((i + 0)%3));
				e3.getP1()->setCoord((i + 1)%3, max.getCoord((i + 1)%3));
				e3.getP2()->setCoord((i + 0)%3, max.getCoord((i + 0)%3));

				e4.getP1()->setCoord((i + 0)%3, max.getCoord((i + 0)%3));
				e4.getP2()->setCoord((i + 2)%3, max.getCoord((i + 2)%3));
				e4.getP2()->setCoord((i + 0)%3, max.getCoord((i + 0)%3));

				e1.draw();
				e2.draw();
				e3.draw();
				e4.draw();
			}

			if (Data::getState().isPlaneEnabled(2*i + 1))
			{
				Data::Edge e1(min, min);
				Data::Edge e2(min, min);
				Data::Edge e3(max, max);
				Data::Edge e4(max, max);

				e1.getP2()->setCoord((i + 1)%3, max.getCoord((i + 1)%3));

				e2.getP1()->setCoord((i + 2)%3, max.getCoord((i + 2)%3));

				e3.getP1()->setCoord((i + 0)%3, min.getCoord((i + 0)%3));
				e3.getP1()->setCoord((i + 1)%3, min.getCoord((i + 1)%3));
				e3.getP2()->setCoord((i + 0)%3, min.getCoord((i + 0)%3));

				e4.getP1()->setCoord((i + 0)%3, min.getCoord((i + 0)%3));
				e4.getP2()->setCoord((i + 2)%3, min.getCoord((i + 2)%3));
				e4.getP2()->setCoord((i + 0)%3, min.getCoord((i + 0)%3));

				e1.draw();
				e2.draw();
				e3.draw();
				e4.draw();
			}
		}

		Data::getState().setShrink(shrink);

		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
	}
}

void CRAbMesh::ViewModule::SetOpenGLStateBeforeRendering(/*const Camera *camera*/) const
{

}

bool CRAbMesh::ViewModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
	minx = miny = minz = DBL_MAX;
	maxx = maxy = maxz = -DBL_MAX;

	return false;
}

void CRAbMesh::ViewModule::RenderPick() const
{

}

unsigned int CRAbMesh::ViewModule::PickBufferSize() const
{
	return 0;
}

Data::DrawableList CRAbMesh::ViewModule::Picked(int /*hits*/, GLuint */*buffer[]*/)
{
	return Data::DrawableList();
}

void *CRAbMesh::ViewModule::Font(int font) const
{
	switch (font)
	{
	case 1:
		return GLUT_BITMAP_TIMES_ROMAN_10;
	case 2:
		return GLUT_BITMAP_HELVETICA_10;
	case 3:
		return GLUT_BITMAP_HELVETICA_12;
	case 4:
		return GLUT_BITMAP_8_BY_13;
	case 5:
		return GLUT_BITMAP_9_BY_15;
	case 6:
		return GLUT_BITMAP_HELVETICA_18;
	case 7:
		return GLUT_BITMAP_TIMES_ROMAN_24;
	}

	return NULL;
}

int CRAbMesh::ViewModule::Font(void *font) const
{
	if (font == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		return 1;
	}
	else if (font == GLUT_BITMAP_HELVETICA_10)
	{
		return 2;
	}
	else if (font == GLUT_BITMAP_HELVETICA_12)
	{
		return 3;
	}
	else if (font == GLUT_BITMAP_8_BY_13)
	{
		return 4;
	}
	else if (font == GLUT_BITMAP_9_BY_15)
	{
		return 5;
	}
	else if (font == GLUT_BITMAP_HELVETICA_18)
	{
		return 6;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		return 7;
	}

	return 0;
}

void CRAbMesh::ViewModule::OnMouseChanged(wxCommandEvent &event)
{
	/*if (event.GetId() == this->miMouseInsert->GetId())
	{
		this->SetMouseOption(MOUSE_INSERT);
		this->frame->SetStatusText(this->miMouseInsert->GetHelp(), 1);
	}
	else */if (event.GetId() == this->miMousePick->GetId())
	{
		this->SetMouseOption(MOUSE_PICK);
		this->frame->SetStatusText(this->miMousePick->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseRotate->GetId())
	{
		this->SetMouseOption(MOUSE_ROTATE);
		this->frame->SetStatusText(this->miMouseRotate->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseTurn->GetId())
	{
		this->SetMouseOption(MOUSE_TURN);
		this->frame->SetStatusText(this->miMouseTurn->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseLook->GetId())
	{
		this->SetMouseOption(MOUSE_LOOK);
		this->frame->SetStatusText(this->miMouseLook->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseTranslate->GetId())
	{
		this->SetMouseOption(MOUSE_TRANSLATE);
		this->frame->SetStatusText(this->miMouseTranslate->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseMove->GetId())
	{
		this->SetMouseOption(MOUSE_MOVE);
		this->frame->SetStatusText(this->miMouseMove->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseOpticalZoom->GetId())
	{
		this->SetMouseOption(MOUSE_OPTICAL_ZOOM);
		this->frame->SetStatusText(this->miMouseOpticalZoom->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseDigitalZoom->GetId())
	{
		this->SetMouseOption(MOUSE_DIGITAL_ZOOM);
		this->frame->SetStatusText(this->miMouseDigitalZoom->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseMergedZoom->GetId())
	{
		this->SetMouseOption(MOUSE_MERGED_ZOOM);
		this->frame->SetStatusText(this->miMouseMergedZoom->GetHelp(), 1);
	}
}

void CRAbMesh::ViewModule::OnMouseAnimation(wxCommandEvent &event)
{
	//this->ToggleAnimation();

	if (event.GetId() == this->miMouseNoAnimation->GetId())
	{
		this->SetAnimationOption(ANIMATION_NONE);
	}
	if (event.GetId() == this->miMouseAnimation->GetId())
	{
		this->SetAnimationOption(ANIMATION_ANY);
	}
	if (event.GetId() == this->miMouseAnimationX->GetId())
	{
		this->SetAnimationOption(ANIMATION_X);
	}
	if (event.GetId() == this->miMouseAnimationY->GetId())
	{
		this->SetAnimationOption(ANIMATION_Y);
	}
	if (event.GetId() == this->miMouseAnimationZ->GetId())
	{
		this->SetAnimationOption(ANIMATION_Z);
	}
}

void CRAbMesh::ViewModule::OnMouseView(wxCommandEvent &event)
{
	if (event.GetId() == this->miMouseFront->GetId())
	{
		this->SetViewOption(VIEW_FRONT);
		this->frame->SetStatusText(this->miMouseFront->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseBack->GetId())
	{
		this->SetViewOption(VIEW_BACK);
		this->frame->SetStatusText(this->miMouseBack->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseRight->GetId())
	{
		this->SetViewOption(VIEW_RIGHT);
		this->frame->SetStatusText(this->miMouseRight->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseLeft->GetId())
	{
		this->SetViewOption(VIEW_LEFT);
		this->frame->SetStatusText(this->miMouseLeft->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseTop->GetId())
	{
		this->SetViewOption(VIEW_TOP);
		this->frame->SetStatusText(this->miMouseTop->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseBottom->GetId())
	{
		this->SetViewOption(VIEW_BOTTOM);
		this->frame->SetStatusText(this->miMouseBottom->GetHelp(), 1);
	}
	else if (event.GetId() == this->miMouseDiagonal->GetId())
	{
		this->SetViewOption(VIEW_DIAGONAL);
		this->frame->SetStatusText(this->miMouseDiagonal->GetHelp(), 1);
	}
}

void CRAbMesh::ViewModule::OnViewFullScreen(wxCommandEvent &WXUNUSED(event))
{
	/*static bool windowTreeEnabled = false;
	static bool windowConfEnabled = false;

	if (this->IsFullScreen())
	{
		this->conf->setWindowTreeEnabled(windowTreeEnabled);
		this->conf->setWindowConfEnabled(windowConfEnabled);
	}
	else
	{
		windowTreeEnabled = this->conf->isWindowTreeEnabled();
		windowConfEnabled = this->conf->isWindowConfEnabled();

		this->conf->setWindowTreeEnabled(false);
		this->conf->setWindowConfEnabled(false);
	}*/

	this->frame->ShowFullScreen(!this->frame->IsFullScreen(), wxFULLSCREEN_NOTOOLBAR | wxFULLSCREEN_NOSTATUSBAR | wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION);
	//this->ShowFullScreen(!this->IsFullScreen());

	this->canvas->Refresh(false);
}

void CRAbMesh::ViewModule::OnViewStereoscopy(wxCommandEvent &WXUNUSED(event))
{
	this->SetStereoscopyEnabled(!this->IsStereoscopyEnabled());
}

void CRAbMesh::ViewModule::OnViewProjection(wxCommandEvent &event)
{
	if (event.GetId() == this->miViewPerspective->GetId())
	{
		this->SetProjectionOption(PROJECTION_PERSPECTIVE);
	}
	else if (event.GetId() == this->miViewOrthographic->GetId())
	{
		this->SetProjectionOption(PROJECTION_ORTHOGRAPHIC);
	}

	this->canvas->Refresh(false);
}

void CRAbMesh::ViewModule::OnViewDepthTest(wxCommandEvent &WXUNUSED(event))
{
	this->SetDepthTestEnabled(!this->IsDepthTestEnabled());
}

void CRAbMesh::ViewModule::OnViewCullFace(wxCommandEvent &WXUNUSED(event))
{
	this->SetCullFaceEnabled(!this->IsCullFaceEnabled());
}

void CRAbMesh::ViewModule::OnViewWireframe(wxCommandEvent &WXUNUSED(event))
{
	this->SetWireframeEnabled(!this->IsWireframeEnabled());
}

void CRAbMesh::ViewModule::OnViewTriangleBorder(wxCommandEvent &WXUNUSED(event))
{
	this->SetTriangleBorderEnabled(!this->IsTriangleBorderEnabled());
}

void CRAbMesh::ViewModule::OnViewNormalByVertex(wxCommandEvent &WXUNUSED(event))
{
	this->SetNormalByVertexEnabled(!this->IsNormalByVertexEnabled());
}

void CRAbMesh::ViewModule::OnViewNormalInverted(wxCommandEvent &WXUNUSED(event))
{
	this->SetNormalInvertedEnabled(!this->IsNormalInvertedEnabled());
}

void CRAbMesh::ViewModule::OnViewRenderAxes(wxCommandEvent &WXUNUSED(event))
{
	this->SetRenderAxesEnabled(!this->IsRenderAxesEnabled());
}

void CRAbMesh::ViewModule::OnViewRenderCrossbar(wxCommandEvent &WXUNUSED(event))
{
	this->SetRenderCrossbarEnabled(!this->IsRenderCrossbarEnabled());
}

void CRAbMesh::ViewModule::OnViewRenderIcon(wxCommandEvent &WXUNUSED(event))
{
	this->SetRenderIconEnabled(!this->IsRenderIconEnabled());
}

void CRAbMesh::ViewModule::OnViewColors(wxCommandEvent &WXUNUSED(event))
{
	ColorDialog *dialog = new ColorDialog(this->frame, this->frame);

	if (dialog->ShowModal() == wxID_OK)
	{
		bool onlyBackground = true;

		if (this->frame->GetConfiguration().background.color != dialog->background->GetScheme())
        {
            this->frame->GetConfiguration().background.color = dialog->background->GetScheme();
        }

		if (this->frame->GetConfiguration().boundary.color != dialog->boundary->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().boundary.color = dialog->boundary->GetScheme();
        }

		if (this->frame->GetConfiguration().mesh.color != dialog->mesh->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().mesh.color = dialog->mesh->GetScheme();
        }

		if (this->frame->GetConfiguration().quality.color != dialog->quality->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().quality.color = dialog->quality->GetScheme();
        }

		if (this->frame->GetConfiguration().front.color != dialog->front->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().front.color = dialog->front->GetScheme();
        }

		if (this->frame->GetConfiguration().cell.color != dialog->cell->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().cell.color = dialog->cell->GetScheme();
        }

		if (this->frame->GetConfiguration().partition.color != dialog->partition->GetScheme())
        {
            onlyBackground = false;
            this->frame->GetConfiguration().partition.color = dialog->partition->GetScheme();
        }

		this->frame->ApplyConfiguration(onlyBackground);
	}

	delete dialog;
}

/*void CRAbMesh::ViewModule::OnViewWindowTree(wxCommandEvent &WXUNUSED(event))
{
	//this->conf->setWindowTreeEnabled(!this->conf->isWindowTreeEnabled());

	this->frame->UpdateAui();
}

void CRAbMesh::ViewModule::OnViewWindowConf(wxCommandEvent &WXUNUSED(event))
{
	//this->conf->setWindowConfEnabled(!this->conf->isWindowConfEnabled());

	this->frame->UpdateAui();
}*/

void CRAbMesh::ViewModule::AnimateCamera()
{
	Camera *camera = this->frame->GetCurrentOrViewCamera();

	switch (this->GetAnimationOption())
	{
	case ANIMATION_NONE:
		break;
	case ANIMATION_ANY:
		{
			if (camera->GetLastX() > camera->GetCanvasWidth())
			{
				camera->SetLast(0, camera->GetLastY());
			}
			else if (camera->GetLastX() < 0)
			{
				camera->SetLast(camera->GetCanvasWidth(), camera->GetLastY());
			}

			camera->Rotate(camera->GetLastX() - 10, camera->GetLastY());
		}
		break;
	case ANIMATION_X:
		{
			camera->Rotate(1.0, 0.0, 0.0, M_PI/120.0);
		}
		break;
	case ANIMATION_Y:
		{
			camera->Rotate(0.0, 1.0, 0.0, M_PI/120.0);
		}
		break;
	case ANIMATION_Z:
		{
			camera->Rotate(0.0, 0.0, 1.0, M_PI/120.0);
		}
		break;
	}
}

void CRAbMesh::ViewModule::OnToolMeshVertexDrawn(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	Data::getState().setVertexDrawn(tbMesh->GetToolToggled(this->tbiVertexDrawn->GetId()));

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshFrontElementDrawn(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	Data::getState().setFrontElementDrawn(tbMesh->GetToolToggled(this->tbiFrontElementDrawn->GetId()));

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshElementDrawn(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	Data::getState().setElementDrawn(tbMesh->GetToolToggled(this->tbiElementDrawn->GetId()));

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshShrink(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sShrink->Enable(tbMesh->GetToolToggled(this->tbiShrink->GetId()));

	if (!this->sShrink->IsEnabled())
	{
		Data::getState().setShrink(1.0);

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxScrollEvent evt;
		this->OnSliderMeshShrink(evt);
	}
}

void CRAbMesh::ViewModule::OnSliderMeshShrink(wxScrollEvent &WXUNUSED(event))
{
	double factor = (static_cast<double>(this->sShrink->GetValue()) + 100.0)/100.0;

	Data::getState().setShrink(factor);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshExplode(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sExplode->Enable(tbMesh->GetToolToggled(this->tbiExplode->GetId()));

	if (!this->sExplode->IsEnabled())
	{
		Data::getState().setExplode(1.0);

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxScrollEvent evt;
		this->OnSliderMeshExplode(evt);
	}
}

void CRAbMesh::ViewModule::OnSliderMeshExplode(wxScrollEvent &WXUNUSED(event))
{
	double factor = (static_cast<double>(this->sExplode->GetValue()) + 100.0)/100.0;

	Data::getState().setExplode(factor);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshExplodeExternal(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sExplodeExternal->Enable(tbMesh->GetToolToggled(this->tbiExplodeExternal->GetId()));

	if (!this->sExplodeExternal->IsEnabled())
	{
		Data::getState().setExplodeExternal(1.0);

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxScrollEvent evt;
		this->OnSliderMeshExplodeExternal(evt);
	}
}

void CRAbMesh::ViewModule::OnSliderMeshExplodeExternal(wxScrollEvent &WXUNUSED(event))
{
	double factor = (static_cast<double>(this->sExplodeExternal->GetValue()) + 100.0)/100.0;

	Data::getState().setExplodeExternal(factor);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshIdDrawn(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sIdDrawn->Enable(tbMesh->GetToolToggled(this->tbiIdDrawn->GetId()));

	if (!this->sIdDrawn->IsEnabled())
	{
		Data::getState().setIdDrawn(false);

		int font = this->sIdDrawn->GetValue();

		if (font != 0)
		{
			Data::getState().setGlutFont(this->Font(font));
		}
	}
	else
	{
		Data::getState().setIdDrawn(true);
	}

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnSliderMeshIdDrawn(wxScrollEvent &WXUNUSED(event))
{
	int font = this->sIdDrawn->GetValue();

	if (font != 0)
	{
		Data::getState().setGlutFont(this->Font(font));

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

void CRAbMesh::ViewModule::OnToolMeshPointSize(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sPointSize->Enable(tbMesh->GetToolToggled(this->tbiPointSize->GetId()));

	if (!this->sPointSize->IsEnabled())
	{
		Data::getState().setPointSize(this->defaultPointSize);
	}
	else
	{
	    int size = this->sPointSize->GetValue();

	    if (size > 0)
        {
            Data::getState().setPointSize(static_cast<double>(size));
        }
        else
        {
            Data::getState().setPointSize(1.0);
        }
	}

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnSliderMeshPointSize(wxScrollEvent &WXUNUSED(event))
{
	int size = this->sPointSize->GetValue();

	if (size > 0)
	{
		Data::getState().setPointSize(static_cast<double>(size));

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

void CRAbMesh::ViewModule::OnToolMeshLineWidth(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sLineWidth->Enable(tbMesh->GetToolToggled(this->tbiLineWidth->GetId()));

	if (!this->sLineWidth->IsEnabled())
	{
		Data::getState().setLineWidth(this->defaultLineWidth);
	}
	else
	{
	    int size = this->sLineWidth->GetValue();

	    if (size > 0)
        {
            Data::getState().setLineWidth(static_cast<double>(size));
        }
        else
        {
            Data::getState().setLineWidth(1.0);
        }
	}

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnSliderMeshLineWidth(wxScrollEvent &WXUNUSED(event))
{
	int size = this->sLineWidth->GetValue();

	if (size > 0)
	{
		Data::getState().setLineWidth(static_cast<double>(size));

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

void CRAbMesh::ViewModule::OnSelectPlane(wxCommandEvent &WXUNUSED(event))
{
	if (!Data::getState().getBox().getMin())
	{
		return;
	}

	int selection = this->bcbPlane->GetSelection();

	if (selection == wxNOT_FOUND)
	{
		return;
	}

	this->cbPlaneEnabled->SetValue(Data::getState().isPlaneEnabled(static_cast<UInt>(selection)));

	this->EnablePlaneSlider(static_cast<UInt>(selection));
}

void CRAbMesh::ViewModule::OnCheckPlaneEnabled(wxCommandEvent &WXUNUSED(event))
{
	if (!Data::getState().getBox().getMin())
	{
		return;
	}

	int selection = this->bcbPlane->GetSelection();

	if (selection == wxNOT_FOUND)
	{
		return;
	}

	//this->cbPlaneEnabled->SetValue(!this->cbPlaneEnabled->GetValue());

	this->EnablePlaneSlider(static_cast<UInt>(selection));
}

void CRAbMesh::ViewModule::OnSliderPlaneCoord(wxScrollEvent &WXUNUSED(event))
{
	if (!Data::getState().getBox().getMin())
	{
		return;
	}

	int selection = this->bcbPlane->GetSelection();

	if (selection == wxNOT_FOUND)
	{
		return;
	}

	int val = this->sPlaneCoord->GetValue();

	Data::getState().setPlaneCoord(selection, static_cast<Real>(val)/100.0);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnCheckInPlane(wxCommandEvent &WXUNUSED(event))
{
	Data::getState().setPlaneInclusive(this->cbInPlane->GetValue());

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshQualityRange(wxCommandEvent &WXUNUSED(event))
{
	wxString title;
	wxAuiToolBar *tbMesh = this->GetMeshToolbar(title);

	this->sQualityRangeMin->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));
	this->sQualityRangeMax->Enable(tbMesh->GetToolToggled(this->tbiQualityRange->GetId()));

	if (!this->sQualityRangeMin->IsEnabled())
	{
		Data::getState().setQualityRangeMin(0.0);
		Data::getState().setQualityRangeMax(1.0);

		this->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		wxScrollEvent evt;
		this->OnSliderMeshQualityRangeMin(evt);
		this->OnSliderMeshQualityRangeMax(evt);
	}
}

void CRAbMesh::ViewModule::OnSliderMeshQualityRangeMin(wxScrollEvent &WXUNUSED(event))
{
	int value = this->sQualityRangeMin->GetValue();

	if (value >= this->sQualityRangeMax->GetValue())
	{
		this->sQualityRangeMin->SetValue(value - 1);

		return;
	}

	double factor = static_cast<double>(value)/static_cast<double>(this->sQualityRangeMin->GetMax() - this->sQualityRangeMin->GetMin());

	Data::getState().setQualityRangeMin(factor);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnSliderMeshQualityRangeMax(wxScrollEvent &WXUNUSED(event))
{
	int value = this->sQualityRangeMax->GetValue();

	if (value <= this->sQualityRangeMin->GetValue())
	{
		this->sQualityRangeMax->SetValue(value + 1);

		return;
	}

	double factor = static_cast<double>(value)/static_cast<double>(this->sQualityRangeMax->GetMax() - this->sQualityRangeMax->GetMin());

	Data::getState().setQualityRangeMax(factor);

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::ViewModule::OnToolMeshVertexPickable(wxCommandEvent &WXUNUSED(event))
{
	Data::getState().setVertexPickable(!Data::getState().isVertexPickable());
}

void CRAbMesh::ViewModule::OnToolMeshFrontElementPickable(wxCommandEvent &WXUNUSED(event))
{
	Data::getState().setFrontElementPickable(!Data::getState().isFrontElementPickable());
}

void CRAbMesh::ViewModule::OnToolMeshElementPickable(wxCommandEvent &WXUNUSED(event))
{
	Data::getState().setElementPickable(!Data::getState().isElementPickable());
}

void CRAbMesh::ViewModule::OnModule(wxCommandEvent &event)
{
	for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		if (event.GetId() == this->miModules[i]->GetId())
		{
			this->SetModuleOption(moduleNames[i]);

			break;
		}
	}
}

void CRAbMesh::ViewModule::OnModuleNext(wxCommandEvent &WXUNUSED(event))
{
    for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		if (this->moduleOption == moduleNames[i])
		{
            this->SetModuleOption(moduleNames[(i+1)%MODULES_COUNT]);

			break;
		}
	}
}

void CRAbMesh::ViewModule::OnModulePrevious(wxCommandEvent &WXUNUSED(event))
{
    for (unsigned int i = 0; i < MODULES_COUNT; i++)
	{
		if (this->moduleOption == moduleNames[i])
		{
            this->SetModuleOption(moduleNames[(i+MODULES_COUNT-1)%MODULES_COUNT]);

			break;
		}
	}
}

void CRAbMesh::ViewModule::EnablePlaneSlider(UInt selection)
{
	unsigned int dim = 3;

	if (this->frame->GetCurrentModule())
	{
		dim = this->frame->GetCurrentModule()->GetDimension();
	}

	if (selection >= 2*dim)
	{
		this->cbPlaneEnabled->SetValue(false);

		Data::getState().setPlaneEnabled(selection, this->cbPlaneEnabled->GetValue());

		return;
	}

	Data::getState().setPlaneEnabled(selection, this->cbPlaneEnabled->GetValue());

	this->sPlaneCoord->Enable(Data::getState().isPlaneEnabled(selection));

	Real min = Data::getState().getBox().getMin()->getCoord(selection/2);
	Real max = Data::getState().getBox().getMax()->getCoord(selection/2);

	min *= 100.0;
	max *= 100.0;

	this->sPlaneCoord->SetRange(static_cast<int>(min - 1.0), static_cast<int>(max + 1.0));
	this->sPlaneCoord->SetValue(static_cast<int>(Data::getState().getPlaneCoord(selection)*100.0));

	this->ClearDisplayLists();

	this->canvas->Refresh(false);
	this->canvas->Update();
}

#endif //#if USE_GUI
