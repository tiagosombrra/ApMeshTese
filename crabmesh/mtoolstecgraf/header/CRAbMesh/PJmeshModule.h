#ifndef _CRAB_MESH_PJMESH_MODULE_H_
#define _CRAB_MESH_PJMESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if (USE_GUI)

#include "CRAbMesh/PMJMeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "PJmesh/Definitions.h"
#include "PJmesh/MainDrive.h"

namespace CRAbMesh
{
	class PJmeshModule : public CRAbMesh::PMJMeshModule
	{
	public:

		PJmeshModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~PJmeshModule();

		virtual wxString Name() const;

		/*using Module::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using Module::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;*/
		using Module::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetVersion(wxString version);
		virtual void SetVersion(enum Jmesh::MainDrive::Versions version);
		virtual enum Jmesh::MainDrive::Versions GetVersion() const;
		virtual wxString GetVersionString() const;
		virtual wxArrayString GetVersions() const;

		virtual void SetGUIState();

	protected:

		virtual void MakeMainDrive();
		/*virtual void MakeMenus();
		virtual void MakeToolbars();*/
		virtual void MakePanels();

		virtual wxMenu *MakeConfigurationMenu();
		//virtual wxMenu *MakeExecuteMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();
		virtual wxDialog *MakeConfigurationDialog();

		virtual void DisableUnusedControls();

	protected:

		virtual void OnConfAdvanVersion(wxCommandEvent &event);
		virtual void OnConfSmallVersion(wxCommandEvent &event);
		virtual void OnConfVersions(wxCommandEvent &event);

		/*virtual void OnExecute(wxCommandEvent &event);
		virtual void OnClear(wxCommandEvent &event);

		virtual void OnResults(wxCommandEvent &event);*/

	protected:

		//virtual void OnTimer();

		virtual unsigned int GetDimension() const;

		/*virtual void SetTree(Data::BoxTree *tree);
		virtual const Data::BoxTree *GetTree() const;
		virtual void SetBoundary(Data::Boundary *boundary);
		virtual const Data::Boundary *GetBoundary() const;*/
		virtual void SetFront(Data::Front *front);
		virtual const Data::Front *GetFront() const;
		/*virtual void SetMesh(Data::Mesh *mesh);
		virtual const Data::Mesh *GetMesh() const;

		virtual wxString QualityString() const;*/

	protected:

		wxMenuItem *miConfAdvanVersion;
		wxMenuItem *miConfSmallVersion;

		/*wxMenuItem *miExecuteExecute;
		wxMenuItem *miExecuteClear;
		wxMenuItem *miExecuteResults;*/

		wxRadioBox *rbVersions;

		wxTextCtrl *tcLog;

		/*wxButton *bExecute;
		wxButton *bClear;*/

		mutable Redirector redirector;

	protected:
		///attributes for configuration

		typedef std::map<wxString, enum Jmesh::MainDrive::Versions> VersionMap;

		wxString version;
		VersionMap versions;

	protected:

		bool removedControls;
	};
}

#endif //#if (USE_GUI)

#endif //#ifndef _CRAB_MESH_PJMESH_MODULE_H_
