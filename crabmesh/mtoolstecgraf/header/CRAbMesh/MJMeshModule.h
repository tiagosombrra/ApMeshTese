#ifndef _CRAB_MESH_MJMESH_MODULE_H_
#define _CRAB_MESH_MJMESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/treectrl.h>

#include "CRAbMesh/MeshModule.h"
#include "MJMesh/Definitions.h"

namespace CRAbMesh
{
	class MJMeshModule : public CRAbMesh::MeshModule
	{
	public:

		MJMeshModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~MJMeshModule();

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

		virtual void SetTreeFactor(double treeFactor);
		virtual double GetTreeFactor() const;

		virtual void SetTolerance(double tolerance);
		virtual double GetTolerance() const;

		virtual void SetSeachTreesEnabled(bool searchTreesEnabled);
		virtual bool IsSearchTreesEnabled() const;

		//virtual void SetBoundaryAdapted(bool boundaryAdapted);
		//virtual bool IsBoundaryAdapted() const;

		virtual void SetBoundarySorted(bool boundarySorted);
		virtual bool IsBoundarySorted() const;

#if USE_NUMERICAL
		virtual void SetHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool IsHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void SetUsingUniformRefinement(bool usingUniformRefinement);
		virtual bool IsUsingUniformRefinement() const;

		virtual void SetLocalBackTrackingEnabled(bool localBackTrackingEnabled);
		virtual bool IsLocalBackTrackingEnabled() const;

		virtual void SetMeshSorted(bool meshSorted);
		virtual bool IsMeshSorted() const;

		virtual void SetNumImproves(unsigned int numImproves);
		virtual unsigned int GetNumImproves() const;

		virtual void SetMaxBackTracks(unsigned int maxBackTracks);
		virtual unsigned int GetMaxBackTracks() const;

		virtual void SetStepByStep(bool stepByStep);
		virtual bool IsStepByStep() const;

		virtual void SetProgressShown(bool progressShown);
		virtual bool IsProgressShown() const;

		virtual void SetGUIState();

	protected:

		virtual void MakeMainDrive();
		virtual void MakeMenus();
		virtual void MakeToolbars();
		virtual void MakePanels();

		virtual wxMenu *MakeConfigurationMenu();
		virtual wxMenu *MakeExecuteMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxDialog *MakeConfigurationDialog();

	protected:

		virtual void OnConfConfiguration(wxCommandEvent &event);
		virtual void OnConfigurationDialogOk(const wxDialog *dConf);

		//virtual void OnConfEnableBoundaryAdapt(wxCommandEvent &event);
		virtual void OnConfSortBoundaryElements(wxCommandEvent &event);
#if USE_NUMERICAL
		virtual void OnConfEnableHotellingTransform(wxCommandEvent &event);
#endif //#if USE_NUMERICAL
		virtual void OnConfUseUniformRefinement(wxCommandEvent &event);
		virtual void OnConfEnableLocalBackTracking(wxCommandEvent &event);
		virtual void OnConfSortMesh(wxCommandEvent &event);

		virtual void OnConfStepByStep(wxCommandEvent &event);
		virtual void OnConfShowProgress(wxCommandEvent &event);

		virtual void OnExecute(wxCommandEvent &event);
		virtual void OnClear(wxCommandEvent &event);

		virtual void OnResults(wxCommandEvent &event);

		virtual void OnKillFocus(wxFocusEvent &event);

	protected:

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

		wxMenuItem *miConfConfiguration;
		//wxMenuItem *miConfEnableBoundaryAdapt;
#if USE_NUMERICAL
		wxMenuItem *miConfEnableHotellingTransform;
#endif //#if USE_NUMERICAL
		wxMenuItem *miConfSortBoundaryElements;
		wxMenuItem *miConfUseUniformRefinement;
		wxMenuItem *miConfEnableLocalBackTracking;
		wxMenuItem *miConfSortMesh;
		wxMenuItem *miConfStepByStep;
		wxMenuItem *miConfShowProgress;

		wxMenuItem *miExecuteExecute;
		wxMenuItem *miExecuteClear;
		wxMenuItem *miExecuteResults;

		wxTextCtrl *tcTreeFactor;
		wxTextCtrl *tcTolerance;

		//wxCheckBox *cbEnableBoundaryAdapt;
		wxCheckBox *cbSortBoundaryElements;
#if USE_NUMERICAL
		wxCheckBox *cbEnableHotellingTransform;
#endif //#if USE_NUMERICAL
		wxCheckBox *cbUseUniformRefinement;
		wxCheckBox *cbEnableLocalBackTracking;
		wxCheckBox *cbSortMesh;
		wxCheckBox *cbStepByStep;
		wxCheckBox *cbShowProgress;

		wxButton *bExecute;
		wxButton *bClear;

		//wxGauge *gWait;

	protected:
		///attributes for configuration

		double treeFactor;
		double tolerance;

		bool searchTreesEnabled;

		//bool boundaryAdapted;
		bool boundarySorted;
#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		bool usingUniformRefinement;
		bool localBackTrackingEnabled;
		bool meshSorted;

		unsigned int numImproves;
		unsigned int maxBackTracks;

		bool stepByStep;
		bool progressShown;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_MJMESH_MODULE_H_
