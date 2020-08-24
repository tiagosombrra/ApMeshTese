#ifndef _CRAB_MESH_PMTOOLS_MODULE_H_
#define _CRAB_MESH_PMTOOLS_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if ((USE_MTOOLS) && (USE_GUI))

#include "CRAbMesh/PMJMeshModule.h"
#include "CRAbMesh/Redirector.h"
#include "PMTools/Definitions.h"
#include "PMTools/MainDrive.h"

namespace CRAbMesh
{
	class PMToolsModule : public CRAbMesh::PMJMeshModule
	{
	public:

		PMToolsModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~PMToolsModule();

		virtual wxString Name() const;

		/*using Module::GetMenu;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const;
		using Module::GetToolbar;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const;*/
		using Module::GetPanel;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const;

		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename);

		//virtual void SetParameters(bool firstTime);

		virtual void Execute();
		virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);

		virtual void SetGUIState();

	protected:

		virtual void MakeMainDrive();
		/*virtual void MakeMenus();
		virtual void MakeToolbars();*/
		virtual void MakePanels();

		//virtual wxMenu *MakeConfigurationMenu();
		//virtual wxMenu *MakeExecuteMenu();
		//virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeLogPanel();
		virtual wxDialog *MakeConfigurationDialog();

		virtual void DisableUnusedControls();

	protected:

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

		/*wxMenuItem *miExecuteExecute;
		wxMenuItem *miExecuteClear;
		wxMenuItem *miExecuteResults;*/

		wxTextCtrl *tcLog;

		/*wxButton *bExecute;
		wxButton *bClear;*/

		mutable Redirector redirector;

	protected:
		///attributes for configuration

		bool removedControls;
	};
}

#endif //#if ((USE_MTOOLS) && (USE_GUI))

#endif //#ifndef _CRAB_MESH_PMTOOLS_MODULE_H_
