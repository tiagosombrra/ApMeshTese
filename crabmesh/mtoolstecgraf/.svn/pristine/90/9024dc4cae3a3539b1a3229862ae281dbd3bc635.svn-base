#ifndef _CRAB_MESH_PMJMESH_MODULE_H_
#define _CRAB_MESH_PMJMESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if (USE_GUI)

#include "CRAbMesh/MJMeshModule.h"
#include "PMJMesh/Definitions.h"

#include <wx/spinctrl.h>

namespace CRAbMesh
{
	class PMJMeshModule : public CRAbMesh::MJMeshModule
	{
	public:

		PMJMeshModule(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~PMJMeshModule();

		virtual wxString Name() const;

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		//virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);
		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void SetAPrioriEnabled(bool aPrioriEnabled);
		virtual bool IsAPrioriEnabled() const;

		virtual void SetParallelismEnabled(bool parallelismEnabled);
		virtual bool IsParallelismEnabled() const;

		virtual void SetMinRefinementPostponed(bool minRefinementPostponed);
		virtual bool IsMinRefinementPostponed() const;

		virtual void SetUsePartitioningThreshold(bool usePartitioningThreshold);
		virtual bool IsUsePartitioningThreshold() const;

		virtual void SetPartitioningThreshold(double partitioningThreshold);
		virtual double GetPartitioningThreshold() const;

		virtual void SetPercentageNonWorkingTask(double percentageNonWorkingTask);
		virtual double GetPercentageNonWorkingTask() const;

		virtual void SetLoadBalancer(wxString loadBalancer);
		virtual void SetLoadBalancer(enum Parallel::AvailableLoadBalancers loadBalancer);
		virtual enum Parallel::AvailableLoadBalancers GetLoadBalancer() const;
		virtual wxString GetLoadBalancerString() const;
		virtual wxArrayString GetLoadBalancers() const;

		virtual void SetRangeProportion(double rangeProportion);
		virtual double GetRangeProportion() const;

		virtual void SetSmoothInSlavesEnabled(bool smoothInSlavesEnabled);
		virtual bool IsSmoothInSlavesEnabled() const;

#if (!USE_DELETE_FELS)
		virtual void SetSlaveUnsmoothingLayer(unsigned int slaveUnsmoothingLayer);
		virtual unsigned int GetSlaveUnsmoothingLayer() const;
#endif //#if (!USE_DELETE_FELS)

		virtual void SetMasterSmoothingLayer(unsigned int masterSmoothingLayer);
		virtual unsigned int GetMasterSmoothingLayer() const;

		virtual void SetPartitionerType(wxString partitionerType);
		virtual void SetPartitionerType(enum PMJMesh::PartitionerType partitionerType);
		virtual enum PMJMesh::PartitionerType GetPartitionerType() const;
		virtual wxString GetPartitionerTypeString() const;
		virtual wxArrayString GetPartitionerTypes() const;

		virtual void SetMeshUpdateType(wxString meshUpdateType);
		virtual void SetMeshUpdateType(enum PMJMesh::MeshUpdateType meshUpdateType);
		virtual enum PMJMesh::MeshUpdateType GetMeshUpdateType() const;
		virtual wxString GetMeshUpdateTypeString() const;
		virtual wxArrayString GetMeshUpdateTypes() const;

		virtual void SetShiftType(wxString shiftType);
		virtual void SetShiftType(enum PMJMesh::ShiftType shiftType);
		virtual enum PMJMesh::ShiftType GetShiftType() const;
		virtual wxString GetShiftTypeString() const;
		virtual wxArrayString GetShiftTypes() const;

		virtual void SetPartitionsDrawable(bool partitionsDrawable);
		virtual bool IsPartitionsDrawable() const;

		virtual void SetPartitionsGeneratedStateDrawable(bool partitionsGeneratedStateDrawable);
		virtual bool IsPartitionsGeneratedStateDrawable() const;

		virtual bool IsDrawable() const;

	protected:

		virtual void MakeMainDrive();

		virtual wxMenu *MakeConfigurationMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxDialog *MakeConfigurationDialog();

	protected:

		virtual void OnConfigurationDialogOk(const wxDialog *dConf);
		virtual void OnConfEnableAPriori(wxCommandEvent &event);
		virtual void OnConfEnableParallelism(wxCommandEvent &event);
		virtual void OnConfPostponeMinRefinement(wxCommandEvent &event);

		virtual void OnConfUsePartitioningThreshold(wxCommandEvent &event);
		virtual void OnConfEnableSmoothInSlaves(wxCommandEvent &event);

		virtual void OnConfLoadBalancer(wxCommandEvent &event);
		virtual void OnConfMeshUpdateType(wxCommandEvent &event);
		virtual void OnConfShiftType(wxCommandEvent &event);

		virtual void OnKillFocus(wxFocusEvent &event);

		virtual void OnTreeMiddleClick(wxTreeEvent &event);

	protected:

		virtual void EnableLoadCtrl();
		virtual void EnableTreeRefinementCtrl();
		virtual void EnableRangeProportionCtrl();
		virtual void EnableMeshUpdateTypesCtrl();
		virtual void EnableShiftTypesCtrl();

		virtual void ForceFillTree(bool clear = true);
		using MJMeshModule::AddTreeItem;
		virtual void AddTreeItem(PMJMesh::Partition *p);

	protected:

		std::string PARTITIONS;
		std::string DRAW_PARTITIONS;
		std::string DRAW_PARTITION_STATES;

	protected:

		wxMenuItem *miConfEnableAPriori;
		wxMenuItem *miConfEnableParallelism;
		wxMenuItem *miConfPostponeMinRefinement;
		wxMenuItem *miConfUsePartitioningThreshold;
		wxMenuItem *miConfEnableSmoothInSlaves;

		wxTextCtrl *tcPartitioningThreshold;
		wxTextCtrl *tcRangeProportion;

		wxSpinCtrl *scPercentageNonWorkingTask;

		wxComboBox *cbPartitionerType;
		wxComboBox *cbLoadBalancer;
		wxComboBox *cbMeshUpdateType;
		wxComboBox *cbShiftType;

		wxCheckBox *cbEnableAPriori;
		wxCheckBox *cbEnableParallelism;
		wxCheckBox *cbPostponeMinRefinement;
		wxCheckBox *cbUsePartitioningThreshold;
		wxCheckBox *cbEnableSmoothInSlaves;

	protected:
		///attributes for configuration

		typedef std::map<wxString, enum Parallel::AvailableLoadBalancers> LoadBalancerMap;
		typedef std::map<wxString, enum PMJMesh::PartitionerType> PartitionerTypeMap;
		typedef std::map<wxString, enum PMJMesh::MeshUpdateType> UpdateTypeMap;
		typedef std::map<wxString, enum PMJMesh::ShiftType> ShiftTypeMap;

		bool aPrioriEnabled;
		bool parallelismEnabled;
		bool minRefinementPostponed;
		bool usePartitioningThreshold;
		double partitioningThreshold;
		double percentageNonWorkingTask;

		wxString loadBalancer;
		LoadBalancerMap loadBalancers;

		double rangeProportion;

		bool smoothInSlavesEnabled;
#if (!USE_DELETE_FELS)
		unsigned int slaveUnsmoothingLayer;
#endif //#if (!USE_DELETE_FELS)
		unsigned int masterSmoothingLayer;

		wxString partitionerType;
		PartitionerTypeMap partitionerTypes;

		wxString meshUpdateType;
		UpdateTypeMap meshUpdateTypes;

		wxString shiftType;
		ShiftTypeMap shiftTypes;
	};
}

#endif //#if (USE_GUI)

#endif //#ifndef _CRAB_MESH_PMJMESH_MODULE_H_
