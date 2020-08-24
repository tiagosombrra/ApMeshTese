#ifndef _CRAB_MESH_PMJMESH2_MODULE_H_
#define _CRAB_MESH_PMJMESH2_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if (USE_GUI)

#include "CRAbMesh/MJMeshModule.h"
#include "PMJMesh2/Definitions.h"

#include <wx/spinctrl.h>

namespace CRAbMesh
{
	class PMJMesh2Module : public CRAbMesh::MJMeshModule
	{
	public:

		PMJMesh2Module(Frame *frame = NULL, Canvas *canvas = NULL, bool makeMainDrive = true);
		virtual ~PMJMesh2Module();

		virtual wxString Name() const;

		virtual void SetParameters(bool firstTime);

		virtual void Execute();
		//virtual void Ready(bool clearing, bool pending);
		virtual void Clear(bool onlyDrive = false, bool destroying = false);
		virtual void ClearDisplayLists(bool force = true) const;

		virtual void Render() const;

		virtual void SetParallelismEnabled(bool parallelismEnabled);
		virtual bool IsParallelismEnabled() const;

		virtual void SetNumThreads(unsigned int numThreads);
        virtual unsigned int GetNumThreads() const;

		virtual void SetNumTasksPerThread(unsigned int numTasksPerThread);
        virtual unsigned int GetNumTasksPerThread() const;

		virtual void SetHeightDependentInLoad(bool heightDependentInLoad);
        virtual bool IsHeightDependentInLoad() const;

        virtual void SetRemoveLayersInLoad(unsigned int removeLayersInLoad);
        virtual unsigned int GetRemoveLayersInLoad() const;

        virtual void SetLoadThreshold(double loadThreshold);
        virtual double GetLoadThreshold() const;

		virtual void SetPartitionerType(wxString partitionerType);
		virtual void SetPartitionerType(enum PMJMesh2::PartitionerType partitionerType);
		virtual enum PMJMesh2::PartitionerType GetPartitionerType() const;
		virtual wxString GetPartitionerTypeString() const;
		virtual wxArrayString GetPartitionerTypes() const;

		virtual void SetInterfaceMeshGenerationType(wxString interfaceMeshGenerationType);
		virtual void SetInterfaceMeshGenerationType(enum PMJMesh2::InterfaceMeshGenerationType interfaceMeshGenerationType);
		virtual enum PMJMesh2::InterfaceMeshGenerationType GetInterfaceMeshGenerationType() const;
		virtual wxString GetInterfaceMeshGenerationTypeString() const;
		virtual wxArrayString GetInterfaceMeshGenerationTypes() const;
		virtual bool IsInterfaceMeshGenerationAPriori() const;

		virtual void SetCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition);
        virtual bool IsCreatingInnerFrontInDecomposition() const;

		virtual void SetRangeProportion(double rangeProportion);
		virtual double GetRangeProportion() const;

		virtual void SetAPrioriInterfaceElementSizeFactor(double aPrioriInterfaceElementSizeFactor);
		virtual double GetAPrioriInterfaceElementSizeFactor() const;

		virtual void SetAPrioriInterfaceBadAngleCos(double aPrioriInterfaceBadAngleCos);
		virtual double GetAPrioriInterfaceBadAngleCos() const;

		virtual void SetAPrioriInterfaceShiftDelta(double aPrioriInterfaceShiftDelta);
		virtual double GetAPrioriInterfaceShiftDelta() const;

		virtual void SetImprovementInSlavesEnabled(bool improvementInSlavesEnabled);
		virtual bool IsImprovementInSlavesEnabled() const;

		virtual void SetMeshUpdateType(wxString meshUpdateType);
		virtual void SetMeshUpdateType(enum PMJMesh2::MeshUpdateType meshUpdateType);
		virtual enum PMJMesh2::MeshUpdateType GetMeshUpdateType() const;
		virtual wxString GetMeshUpdateTypeString() const;
		virtual wxArrayString GetMeshUpdateTypes() const;

		virtual void SetImprovementLayers(unsigned int improvementLayers);
        virtual unsigned int GetImprovementLayers() const;

		virtual void SetFinalMeshState(wxString finalMeshState);
		virtual void SetFinalMeshState(enum PMJMesh2::FinalMeshState finalMeshState);
		virtual enum PMJMesh2::FinalMeshState GetFinalMeshState() const;
		virtual wxString GetFinalMeshStateString() const;
		virtual wxArrayString GetFinalMeshStates() const;

		virtual void SetPartitionsDrawable(bool partitionsDrawable);
		virtual bool IsPartitionsDrawable() const;

		virtual void SetPartitionsGeneratedStateDrawable(bool partitionsGeneratedStateDrawable);
		virtual bool IsPartitionsGeneratedStateDrawable() const;

		virtual void SetInnerMeshDrawable(bool innerMeshesDrawable);
		virtual bool IsInnerMeshDrawable() const;

		virtual bool IsDrawable() const;

	protected:

		virtual void MakeMainDrive();

		virtual wxMenu *MakeConfigurationMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxDialog *MakeConfigurationDialog();

	protected:

		virtual void OnConfigurationDialogOk(const wxDialog *dConf);

		virtual void OnConfEnableParallelism(wxCommandEvent &event);
		virtual void OnConfHeightDependentInLoad(wxCommandEvent &event);
		virtual void OnConfCreatingInnerFrontInDecomposition(wxCommandEvent &event);
		virtual void OnConfEnableImprovementInSlaves(wxCommandEvent &event);

		virtual void OnConfPartitionerType(wxCommandEvent &event);
		virtual void OnConfInterfaceMeshGenerationType(wxCommandEvent &event);
		virtual void OnConfMeshUpdateType(wxCommandEvent &event);
		virtual void OnConfFinalMeshState(wxCommandEvent &event);

#if USE_FILE_BASED_BSP
		virtual void OnLoadBSPFile(wxCommandEvent &event);
#endif //#if USE_FILE_BASED_BSP

		virtual void OnKillFocus(wxFocusEvent &event);

		virtual void OnTreeMiddleClick(wxTreeEvent &event);

	protected:

		virtual void EnableRangeProportionCtrl();
		virtual void EnableInnerFrontCtrl();
		virtual void EnableAPrioriInterfaceElementSizeCtrl();
		//virtual void EnableMeshUpdateTypesCtrl();
		virtual void EnableNumThreadsCtrl();
		virtual void PropagateAPrioriInterfaceMeshGeneration();

		virtual void ForceFillTree(bool clear = true);
		using MJMeshModule::AddTreeItem;
		virtual void AddTreeItem(PMJMesh::Partition *p);

	protected:

		std::string PARTITIONS;
		std::string DRAW_PARTITIONS;
		std::string DRAW_PARTITION_STATES;
		std::string DRAW_INNER_MESHES;

	protected:

		wxMenuItem *miConfEnableParallelism;
		wxMenuItem *miConfHeightDependentInLoad;
		wxMenuItem *miConfCreatingInnerFrontInDecomposition;
		wxMenuItem *miConfEnableImprovementInSlaves;

		wxTextCtrl *tcNumThreads;
		wxTextCtrl *tcNumTasksPerThread;
		wxTextCtrl *tcRemoveLayersInLoad;
		wxTextCtrl *tcAPrioriInterfaceBadAngleCos;
		wxTextCtrl *tcAPrioriInterfaceShiftDelta;
		wxTextCtrl *tcLoadThreshold;
		wxTextCtrl *tcRangeProportion;
		wxTextCtrl *tcAPrioriInterfaceElementSizeFactor;
		wxTextCtrl *tcImprovementLayers;

		wxComboBox *cbPartitionerType;
		wxComboBox *cbInterfaceMeshGenerationType;
		wxComboBox *cbMeshUpdateType;
		wxComboBox *cbFinalMeshState;

		wxCheckBox *cbEnableParallelism;
		wxCheckBox *cbHeightDependentInLoad;
		wxCheckBox *cbCreatingInnerFrontInDecomposition;
		wxCheckBox *cbEnableImprovementInSlaves;

#if USE_FILE_BASED_BSP
		wxButton *bLoadBSPFile;
#endif //#if USE_FILE_BASED_BSP

	protected:
		///attributes for configuration

		typedef std::map<wxString, enum PMJMesh2::PartitionerType> PartitionerTypeMap;
		typedef std::map<wxString, enum PMJMesh2::InterfaceMeshGenerationType> InterfaceMeshGenerationTypeMap;
		typedef std::map<wxString, enum PMJMesh2::MeshUpdateType> MeshUpdateTypeMap;
		typedef std::map<wxString, enum PMJMesh2::FinalMeshState> FinalMeshStateMap;

		bool parallelismEnabled;

		mutable unsigned int numThreads;

		unsigned int numTasksPerThread;

		bool heightDependentInLoad;

		unsigned int removeLayersInLoad;
		double loadThreshold;
		double aPrioriInterfaceBadAngleCos;
		double aPrioriInterfaceShiftDelta;

		wxString partitionerType;
		PartitionerTypeMap partitionerTypes;

		wxString interfaceMeshGenerationType;
		InterfaceMeshGenerationTypeMap interfaceMeshGenerationTypes;

		bool creatingInnerFrontInDecomposition;

		double rangeProportion;

		double aPrioriInterfaceElementSizeFactor;

		bool improvementInSlavesEnabled;

		wxString meshUpdateType;
		MeshUpdateTypeMap meshUpdateTypes;

		unsigned int improvementLayers;

		wxString finalMeshState;
		FinalMeshStateMap finalMeshStates;

	};
}

#endif //#if (USE_GUI)

#endif //#ifndef _CRAB_MESH_PMJMESH2_MODULE_H_
