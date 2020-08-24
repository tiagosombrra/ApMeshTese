#ifndef _CRAB_MESH_JMESH_MODULE_H_
#define _CRAB_MESH_JMESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/treectrl.h>

#include "CRAbMesh/MeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "Jmesh/Definitions.h"
#include "Jmesh/MainDrive.h"

namespace CRAbMesh
{
	class JmeshModule : public CRAbMesh::MeshModule
	{
	public:

		JmeshModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~JmeshModule();

		virtual wxString Name() const;

		using Module::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using Module::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;
		using Module::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetVersion(wxString version);
		virtual enum Jmesh::MainDrive::Versions GetVersion() const;
		virtual wxString GetVersionString() const;
		virtual wxArrayString GetVersions() const;

#if USE_NUMERICAL
		virtual void SetHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool IsHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void SetUsingUniformRefinement(bool usingUniformRefinement);
		virtual bool IsUsingUniformRefinement() const;

		virtual void SetGUIState();

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxMenu *MakeConfigurationMenu();
		virtual wxMenu *MakeExecuteMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();

	protected:

		virtual void OnConfAdvanVersion(wxCommandEvent &event);
		virtual void OnConfSmallVersion(wxCommandEvent &event);
		virtual void OnConfVersions(wxCommandEvent &event);
#if USE_NUMERICAL
		virtual void OnConfEnableHotellingTransform(wxCommandEvent &event);
#endif //#if USE_NUMERICAL
		virtual void OnConfUseUniformRefinement(wxCommandEvent &event);

		virtual void OnExecute(wxCommandEvent &event);
		virtual void OnClear(wxCommandEvent &event);

		virtual void OnResults(wxCommandEvent &event);

	protected:

		//virtual void OnTimer();

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

	protected:

		wxMenuItem *miConfAdvanVersion;
		wxMenuItem *miConfSmallVersion;
#if USE_NUMERICAL
		wxMenuItem *miConfEnableHotellingTransform;
#endif //#if USE_NUMERICAL
		wxMenuItem *miConfUseUniformRefinement;

		wxMenuItem *miExecuteExecute;
		wxMenuItem *miExecuteClear;
		wxMenuItem *miExecuteResults;

		wxRadioBox *rbVersions;

#if USE_NUMERICAL
		wxCheckBox *cbEnableHotellingTransform;
#endif //#if USE_NUMERICAL
		wxCheckBox *cbUseUniformRefinement;

		wxTextCtrl *tcLog;

		wxButton *bExecute;
		wxButton *bClear;

		mutable Redirector redirector;

	protected:
		///attributes for configuration

		typedef std::map<wxString, enum Jmesh::MainDrive::Versions> VersionMap;

		wxString version;
		VersionMap versions;

#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		bool usingUniformRefinement;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_JMESH_MODULE_H_
