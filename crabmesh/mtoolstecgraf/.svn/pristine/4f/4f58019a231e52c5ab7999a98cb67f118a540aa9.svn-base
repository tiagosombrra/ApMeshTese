#include "CRAbMesh/PMJMesh2Module.h"

#if (USE_GUI)

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>
#include <wx/bookctrl.h>
#include "PMJMesh2/MainDrive.h"
#include "PMJMesh2/Communicator.h"
#include "PMJMesh2/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "Parallel/Task.h"
#include "Parallel/Partition.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"
#include "Data/BoxTreeCell.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/State.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/TreeItemData.h"

CRAbMesh::PMJMesh2Module::PMJMesh2Module(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MJMeshModule(frame, canvas, false)
{
    this->parallelismEnabled = true;
    this->numThreads = 0;
    this->numTasksPerThread = 1;
	this->heightDependentInLoad = false;
    this->removeLayersInLoad = 1;
    this->loadThreshold = 0.5;
    this->aPrioriInterfaceBadAngleCos = 0.95;
    this->aPrioriInterfaceShiftDelta = 2.0;
    this->creatingInnerFrontInDecomposition = true;
    this->rangeProportion = 1.0;
    this->aPrioriInterfaceElementSizeFactor = 1.0;
    this->improvementInSlavesEnabled = true;
    this->improvementLayers = 1;

	this->cbPartitionerType = NULL;
	this->partitionerTypes[_("Geometry based")] = PMJMesh2::PartitionerType::GEOMETRY_BASED_BSP;
	this->partitionerTypes[_("Load based")] = PMJMesh2::PartitionerType::LOAD_BASED_BSP;
#if USE_FILE_BASED_BSP
	this->partitionerTypes[_("File based")] = PMJMesh2::PartitionerType::FILE_BASED_BSP;
#endif //#if USE_FILE_BASED_BSP
	this->SetPartitionerType(PMJMesh2::PartitionerType::LOAD_BASED_BSP);

	this->cbInterfaceMeshGenerationType = NULL;
	this->interfaceMeshGenerationTypes[_("A Posteriori")] = PMJMesh2::InterfaceMeshGenerationType::A_POSTERIORI;
#if USE_TECGRAF_MSH_SURF
	this->interfaceMeshGenerationTypes[_("A Priori")] = PMJMesh2::InterfaceMeshGenerationType::A_PRIORI;
	this->interfaceMeshGenerationTypes[_("A Priori internal")] = PMJMesh2::InterfaceMeshGenerationType::A_PRIORI_INTERNAL;
	this->SetInterfaceMeshGenerationType(PMJMesh2::InterfaceMeshGenerationType::A_PRIORI);
#else
	this->SetInterfaceMeshGenerationType(PMJMesh2::InterfaceMeshGenerationType::A_POSTERIORI);
#endif //#if USE_TECGRAF_MSH_SURF

	this->cbMeshUpdateType = NULL;
	this->meshUpdateTypes[_("1 - Full mesh")] = PMJMesh2::MeshUpdateType::FULL;
	this->meshUpdateTypes[_("2 - Partial mesh")] = PMJMesh2::MeshUpdateType::PARTIAL;
	this->meshUpdateTypes[_("3 - Only front")] = PMJMesh2::MeshUpdateType::EMPTY;
	this->SetMeshUpdateType(PMJMesh2::MeshUpdateType::PARTIAL);

	this->cbFinalMeshState = NULL;
	this->finalMeshStates[_("1 - Joined")] = PMJMesh2::FinalMeshState::JOINED;
	this->finalMeshStates[_("2 - Distributed")] = PMJMesh2::FinalMeshState::DISTRIBUTED;
	this->finalMeshStates[_("3 - Redistributed")] = PMJMesh2::FinalMeshState::REDISTRIBUTED;
	this->SetFinalMeshState(PMJMesh2::FinalMeshState::DISTRIBUTED);

	PARTITIONS            = "Partitions";
	DRAW_PARTITIONS       = "Draw partitions";
	DRAW_PARTITION_STATES = "Draw partition states";
	DRAW_INNER_MESHES     = "Draw inner meshes";

	this->InsertOption(PARTITIONS,            TREE_CELLS,       TYPE_TREE_ITEM);
	this->InsertOption(DRAW_PARTITIONS,       DRAW_MESH,        TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_PARTITION_STATES, DRAW_PARTITIONS,  TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_INNER_MESHES,     DRAW_PARTITION_STATES, TYPE_MENU_AND_CHECK_ITEM);

	this->SetConfiguration(DRAW_PARTITIONS,       true,  false);
	this->SetConfiguration(DRAW_PARTITION_STATES, false, false);
	this->SetConfiguration(DRAW_INNER_MESHES,     true, false);

	this->SetOption(PARTITIONS,            true);
	this->SetOption(DRAW_PARTITIONS,       true, true);
	this->SetOption(DRAW_PARTITION_STATES, true, false);
	this->SetOption(DRAW_INNER_MESHES,     true, false);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();

		//this->EnableRangeProportionCtrl();
		//this->EnableAPrioriInterfaceElementSizeCtrl();
		//this->EnableMeshUpdateTypesCtrl();

		if (dynamic_cast<PMJMesh2::MainDrive *>(this->drive)->getComm()->isSlave())
		{
			this->SetStepByStep(false);
			this->SetProgressShown(false);

			this->SetBoundaryDrawable(false);
			this->SetTreeDrawable(false);
			this->SetFrontDrawable(false);
			this->SetMeshDrawable(false);
			this->SetInnerMeshDrawable(false);
			this->SetPartitionsDrawable(false);
		}
	}
}

CRAbMesh::PMJMesh2Module::~PMJMesh2Module()
{
	this->tcRangeProportion->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
	this->tcAPrioriInterfaceElementSizeFactor->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
	this->tcAPrioriInterfaceBadAngleCos->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
	this->tcAPrioriInterfaceShiftDelta->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
}

wxString CRAbMesh::PMJMesh2Module::Name() const
{
	wxString name = _("BSP PJMesh (Markos version)");

	PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

	if ((drive) && (drive->getComm()) && (drive->getComm()->isParallel()))
	{
		if (drive->getComm()->isMaster())
		{
			name << _(" Master");
		}
		else
		{
			name << _(" Slave ") << drive->getComm()->rank();
		}
	}

	return name;
}

void CRAbMesh::PMJMesh2Module::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

		//drive->setParallel(this->IsParallelismEnabled());
		static_cast<PMJMesh2::Communicator *>(drive->getComm())->setSharedParallelismEnabled(this->IsParallelismEnabled());
		drive->setNumThreads(this->GetNumThreads());
		drive->setNumTasksPerThread(this->GetNumTasksPerThread());
        drive->setHeightDependentInLoad(this->IsHeightDependentInLoad());
        drive->setRemoveLayersInLoad(this->GetRemoveLayersInLoad());
        drive->setLoadThreshold(this->GetLoadThreshold());
#if USE_REMOVED_LAYERS_PROPORTIONS
        //drive->setRemovedLayersProportions(0, 1.0);
        //drive->setRemovedLayersProportions(1, 1.0);
        //drive->setRemovedLayersProportions(2, 0.3);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
		drive->setPartitionerType(this->GetPartitionerType());
		drive->setInterfaceMeshGenerationType(this->GetInterfaceMeshGenerationType());
		drive->setCreatingInnerFrontInDecomposition(this->IsCreatingInnerFrontInDecomposition());
		drive->setRangeProportion(this->GetRangeProportion());
		drive->setAPrioriInterfaceBadAngleCos(this->GetAPrioriInterfaceBadAngleCos());
		drive->setAPrioriInterfaceShiftDelta(this->GetAPrioriInterfaceShiftDelta());
		drive->setAPrioriInterfaceElementSizeFactor(this->GetAPrioriInterfaceElementSizeFactor());
		drive->setImprovementInSlavesEnabled(this->IsImprovementInSlavesEnabled());
		drive->setImprovementLayers(this->GetImprovementLayers());
		drive->setUpdateType(this->GetMeshUpdateType());
		drive->setFinalMeshState(this->GetFinalMeshState());
	}

	MJMeshModule::SetParameters(firstTime);
}

void CRAbMesh::PMJMesh2Module::Execute()
{
	if (this->tcNumThreads)
	{
		unsigned long int numThreads = 0;

		this->tcNumThreads->GetValue().ToULong(&numThreads);

		this->SetNumThreads(static_cast<unsigned int>(numThreads));

		this->tcNumThreads->Enable(false);
	}

	if (this->tcNumTasksPerThread)
	{
		unsigned long int numTasksPerThread = 0;

		this->tcNumTasksPerThread->GetValue().ToULong(&numTasksPerThread);

		this->SetNumTasksPerThread(static_cast<unsigned int>(numTasksPerThread));

		this->tcNumTasksPerThread->Enable(false);
	}

	if (this->tcRemoveLayersInLoad)
	{
		unsigned long int removeLayersInLoad = 0;

		this->tcRemoveLayersInLoad->GetValue().ToULong(&removeLayersInLoad);

		this->SetRemoveLayersInLoad(static_cast<unsigned int>(removeLayersInLoad));

		this->tcRemoveLayersInLoad->Enable(false);
	}

	if (this->tcLoadThreshold)
	{
		double loadThreshold = 0;

		this->tcLoadThreshold->GetValue().ToDouble(&loadThreshold);

		this->SetLoadThreshold(loadThreshold);

		this->tcLoadThreshold->Enable(false);
	}

	if (this->tcRangeProportion)
	{
		double rangeProportion = 0.0;

		this->tcRangeProportion->GetValue().ToDouble(&rangeProportion);

		this->SetRangeProportion(rangeProportion);

		this->tcRangeProportion->Enable(false);
	}

	if (this->tcAPrioriInterfaceBadAngleCos)
	{
		double aPrioriInterfaceBadAngleCos = 0.0;

		this->tcAPrioriInterfaceBadAngleCos->GetValue().ToDouble(&aPrioriInterfaceBadAngleCos);

		this->SetAPrioriInterfaceBadAngleCos(aPrioriInterfaceBadAngleCos);

		this->tcAPrioriInterfaceBadAngleCos->Enable(false);
	}

	if (this->tcAPrioriInterfaceShiftDelta)
	{
		double aPrioriInterfaceShiftDelta = 0.0;

		this->tcAPrioriInterfaceShiftDelta->GetValue().ToDouble(&aPrioriInterfaceShiftDelta);

		this->SetAPrioriInterfaceShiftDelta(aPrioriInterfaceShiftDelta);

		this->tcAPrioriInterfaceShiftDelta->Enable(false);
	}

	if (this->tcAPrioriInterfaceElementSizeFactor)
	{
		double aPrioriInterfaceElementSizeFactor = 0.0;

		this->tcAPrioriInterfaceElementSizeFactor->GetValue().ToDouble(&aPrioriInterfaceElementSizeFactor);

		this->SetAPrioriInterfaceElementSizeFactor(aPrioriInterfaceElementSizeFactor);

		this->tcAPrioriInterfaceElementSizeFactor->Enable(false);
	}

	if (this->tcImprovementLayers)
	{
		unsigned long int improvementLayers;

		this->tcImprovementLayers->GetValue().ToULong(&improvementLayers);

		this->SetImprovementLayers(static_cast<unsigned int>(improvementLayers));

		this->tcImprovementLayers->Enable(false);
	}

	if (this->cbPartitionerType)
	{
		this->SetPartitionerType(this->cbPartitionerType->GetValue());

		this->cbPartitionerType->Enable(false);
	}

	if (this->cbInterfaceMeshGenerationType)
	{
		this->SetInterfaceMeshGenerationType(this->cbInterfaceMeshGenerationType->GetValue());

		this->cbInterfaceMeshGenerationType->Enable(false);
	}

	if (this->cbMeshUpdateType)
	{
		this->SetMeshUpdateType(this->cbMeshUpdateType->GetValue());

		this->cbMeshUpdateType->Enable(false);
	}

	if (this->cbFinalMeshState)
	{
		this->SetFinalMeshState(this->cbFinalMeshState->GetValue());

		this->cbFinalMeshState->Enable(false);
	}

	if (this->miConfEnableParallelism) this->miConfEnableParallelism->Enable(false);
	if (this->miConfHeightDependentInLoad) this->miConfHeightDependentInLoad->Enable(false);
	if (this->miConfCreatingInnerFrontInDecomposition) this->miConfCreatingInnerFrontInDecomposition->Enable(false);
	if (this->miConfEnableImprovementInSlaves) this->miConfEnableImprovementInSlaves->Enable(false);

	if (this->cbEnableParallelism) this->cbEnableParallelism->Enable(false);
	if (this->cbHeightDependentInLoad) this->cbHeightDependentInLoad->Enable(false);
	if (this->cbCreatingInnerFrontInDecomposition) this->cbCreatingInnerFrontInDecomposition->Enable(false);
	if (this->cbEnableImprovementInSlaves) this->cbEnableImprovementInSlaves->Enable(false);

#if USE_FILE_BASED_BSP
	if (this->bLoadBSPFile) this->bLoadBSPFile->Enable(false);
#endif //#if USE_FILE_BASED_BSP

	//if (this->gWait) this->gWait->Enable();

	MJMeshModule::Execute();
}

void CRAbMesh::PMJMesh2Module::Clear(bool onlyDrive, bool destroying)
{
	/*if ((!destrying) && (dynamic_cast<PMJMesh2::MainDrive *>(this->drive)->getComm()->isSlave()))
	{
		return;
	}*/

	//dynamic_cast<PMJMesh2::MainDrive *>(this->drive)->setDestroying(destroying);

	MJMeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		return;
	}

	if (this->miConfEnableParallelism) this->miConfEnableParallelism->Enable();
	if (this->miConfHeightDependentInLoad) this->miConfHeightDependentInLoad->Enable();
	if (this->miConfCreatingInnerFrontInDecomposition) this->miConfCreatingInnerFrontInDecomposition->Enable();
	if (this->miConfEnableImprovementInSlaves) this->miConfEnableImprovementInSlaves->Enable();

	if (this->cbEnableParallelism) this->cbEnableParallelism->Enable();
	if (this->cbHeightDependentInLoad) this->cbHeightDependentInLoad->Enable();
	if (this->cbCreatingInnerFrontInDecomposition) this->cbCreatingInnerFrontInDecomposition->Enable();
	if (this->cbEnableImprovementInSlaves) this->cbEnableImprovementInSlaves->Enable();

	this->tcNumThreads->Enable();
	this->tcNumTasksPerThread->Enable();
	this->tcRemoveLayersInLoad->Enable();
	this->tcLoadThreshold->Enable();
    this->tcRangeProportion->Enable();
    this->tcAPrioriInterfaceBadAngleCos->Enable();
    this->tcAPrioriInterfaceShiftDelta->Enable();
    this->tcAPrioriInterfaceElementSizeFactor->Enable();
    this->tcImprovementLayers->Enable();
    this->cbPartitionerType->Enable();
    this->cbInterfaceMeshGenerationType->Enable();
    this->cbMeshUpdateType->Enable();
    this->cbFinalMeshState->Enable();
#if USE_FILE_BASED_BSP
    if (this->bLoadBSPFile) this->bLoadBSPFile->Enable();
#endif //#if USE_FILE_BASED_BSP

	this->EnableNumThreadsCtrl();
	this->PropagateAPrioriInterfaceMeshGeneration();
}

void CRAbMesh::PMJMesh2Module::ClearDisplayLists(bool force) const
{
	CRAbMesh::MJMeshModule::ClearDisplayLists(force);

	PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

	Parallel::TaskVector tasks = drive->getTasks();

	for (Parallel::TaskVector::const_iterator iter = tasks.begin();
		 iter != tasks.end(); iter++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

		Data::Mesh *m = t->getMesh();

		if ((m) && ((force) || (m->isOutdated())))
		{
			m->clearLists();
		}

		Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMeshes();

		while (!meshes.empty())
		{
			m = meshes.front();

			if ((m) && ((force) || (m->isOutdated())))
			{
				m->clearLists();
			}

			meshes.pop_front();
		}

		Data::Front *f = t->getFront();

		if ((f) && ((force) || (f->isOutdated())))
		{
			f->clearLists();
		}

		const Data::Boundary *b = dynamic_cast<const PMJMesh::WorkerMainDrive *>(t->getBuilder())->getBoundary();

		if ((b) && (b != this->GetBoundary()) && ((force) || (b->isOutdated())))
		{
			b->clearLists();
		}

		/*PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(t->getPartition());

		p->boundary.clearLists();*/
	}

	/*if (!drive->getComm()->isMaster())
	{
		const Data::Mesh *m = drive->getUnsmoothedMesh();

		if ((m) && ((force) || (m->isOutdated())))
		{
			m->clearLists();
		}
	}*/
}

void CRAbMesh::PMJMesh2Module::Render() const
{
	PMJMesh::Partition *selected = NULL;

	wxTreeItemId item = this->tree->GetSelection();

	if ((item.IsOk()) &&
		(this->IsEnabled(PARTITIONS, TREE_ITEM)) &&
		(this->tree->GetItemParent(item) == (*this->tiItems.find(PARTITIONS)).second))
	{
		TreeItemData *data = static_cast<TreeItemData *>(this->tree->GetItemData(item));

		selected = static_cast<PMJMesh::Partition *>(data->GetItem());
	}

//#if USE_SCREEENSHOT
	if (!selected)
	{
		MJMeshModule::Render();
	}
//#endif //#if USE_SCREEENSHOT

	PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

	if (this->IsDrawable())
	{
		Parallel::TaskVector tasks = drive->getTasks();

#if USE_SCREEENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREEENSHOT

		Real factor = this->MakeNormalFactor();

		if (this->IsPartitionsDrawable())
		{
			for (Parallel::TaskVector::const_iterator iter = tasks.begin();
				 iter != tasks.end(); iter++)
			{
				PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter)->getPartition());

				if (!p)
				{
					continue;
				}

				if ((selected) && (selected != p))
				{
					continue;
				}

				//const Data::Box *b = p->getBox();

				/*static double val = 10.0;

				b->setColor(b->getR()/val, b->getG()/val, b->getB()/val);

				b->highlight();

				b->draw();

				b->setColor(b->getR()*val, b->getG()*val, b->getB()*val);

				b->unhighlight();*/

				//b->draw();

				if (Data::getState().getExplodeExternal() != 1.0)
                {
                    Data::Point3D mid;

                    if (p->getBox()->dimension() == 2)
                    {
                        Data::Point2D bmid = p->getBox()->mid2D();

                        mid.setCoord(0, bmid.getCoord(0));
                        mid.setCoord(1, bmid.getCoord(1));
                        mid.setCoord(2, 0.0);
                    }
                    else
                    {
                        Data::Point3D bmid = p->getBox()->mid3D();

                        mid.setCoord(0, bmid.getCoord(0));
                        mid.setCoord(1, bmid.getCoord(1));
                        mid.setCoord(2, bmid.getCoord(2));
                    }

                    Data::getState().setMidExternal(mid);
                }

				static_cast<PMJMesh::MJMeshTask *>((*iter))->draw();

				//b->setBox(NULL, NULL);

				//debug
				//drawing the mesh
				if (this->IsMeshDrawable())
				{
					if ((*iter)->getBuilder())
					{
						//Data::Mesh *m = static_cast<PMJMesh::GeometryFrontAdvancer *>((*iter)->getBuilder())->getMesh();
						const Data::Mesh *m = dynamic_cast<PMJMesh::WorkerMainDrive *>((*iter)->getBuilder())->getMesh();

						if (m)
						{
							m->draw();
						}
					}
					else
                    {
                        Data::Mesh *m = static_cast<PMJMesh::MJMeshTask *>((*iter))->getMesh();

                        if (m)
                        {
                            m->draw();
                        }
                    }
				}

				if (this->IsInnerMeshDrawable())
                {
                    Data::Point3D mid = Data::getState().getMidExternal();
                    Data::Point3DObjList mids = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMids();
                    Data::Point3D lmid;

                    if ((*iter)->getBuilder())
					{
						Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMeshes();

                        while (!meshes.empty())
                        {
                            Data::Mesh *m = meshes.front();

                            if (Data::getState().getExplodeExternal() != 1.0)
                            {
                                if (mids.empty())
                                {
                                    lmid = mid;
                                }
                                else
                                {
                                    lmid = mids.front();
                                    mids.pop_front();
                                }

                                Data::getState().setMidExternal(lmid);
                            }

                            if (m)
                            {
                                m->draw();
                            }

                            meshes.pop_front();
                        }
					}
					else
                    {
                        if ((!selected) || (selected != p))
                        {
                            Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMeshes();

                            while (!meshes.empty())
                            {
                                Data::Mesh *m = meshes.front();

                                if (Data::getState().getExplodeExternal() != 1.0)
                                {
                                    if (mids.empty())
                                    {
                                        lmid = mid;
                                    }
                                    else
                                    {
                                        lmid = mids.front();
                                        mids.pop_front();
                                    }

                                    Data::getState().setMidExternal(lmid);
                                }

                                if (m)
                                {
                                    m->draw();
                                }

                                meshes.pop_front();
                            }
                        }
                    }

                    if (Data::getState().getExplodeExternal() != 1.0)
                    {
                        Data::getState().setMidExternal(mid);
                    }
                }
				//endebug

				if (this->IsFrontDrawable())
				{
					Data::Front *f = static_cast<PMJMesh::MJMeshTask *>((*iter))->getFront();

					if ((selected) &&
						(selected == p) &&
						((!f) ||
						 ((f->isEmpty()) && ((*iter)->getBuilder()))))
					{
						f = const_cast<PMJMesh::Front *>(static_cast<const PMJMesh::Front *>(dynamic_cast<const PMJMesh::WorkerMainDrive *>((*iter)->getBuilder())->getFront()));
					}

					if (f)
					{
						f->draw();
					}
				}

				if (this->IsBoundaryDrawable())
				{
					const Data::Boundary *b = dynamic_cast<const PMJMesh::WorkerMainDrive *>((*iter)->getBuilder())->getBoundary();

					if ((b) && (b != this->GetBoundary()))
					{
						b->draw();
					}
				}

				if (this->IsPartitionsGeneratedStateDrawable())
				{
					static_cast<PMJMesh::MJMeshTask *>((*iter))->drawGeneratedState();
				}

				/*if (this->IsTreeDrawable())
                {
                    for (Data::GraphNodeList::iterator iter = p->cells.begin();
                         iter != p->cells.end(); iter++)
                    {
                        static_cast<Data::BoxTreeCell *>((*iter))->draw();
                    }
                }

                if (this->IsBoundaryDrawable())
                {
                    p->boundary.draw();
                }*/

                Data::getState().resetMidExternal();
			}
		}

		/*if ((!selected) && (!drive->getComm()->isMaster()) && (this->IsMeshDrawable()))
		{
			const Data::Mesh *m = drive->getUnsmoothedMesh();

			if (m)
			{
				m->draw();
			}
		}*/

#if USE_SCREEENSHOT
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
#endif //#if USE_SCREEENSHOT

		if ((this->IsPartitionsDrawable()) && (this->IsNormalDrawable()))
		{
			for (Parallel::TaskVector::const_iterator iter = tasks.begin();
				 iter != tasks.end(); iter++)
			{
				PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter)->getPartition());

				if ((selected) && (selected != p))
				{
					continue;
				}

				if (Data::getState().getExplodeExternal() != 1.0)
                {
                    Data::Point3D mid;

                    if (p->getBox()->dimension() == 2)
                    {
                        Data::Point2D bmid = p->getBox()->mid2D();

                        mid.setCoord(0, bmid.getCoord(0));
                        mid.setCoord(1, bmid.getCoord(1));
                        mid.setCoord(2, 0.0);
                    }
                    else
                    {
                        Data::Point3D bmid = p->getBox()->mid3D();

                        mid.setCoord(0, bmid.getCoord(0));
                        mid.setCoord(1, bmid.getCoord(1));
                        mid.setCoord(2, bmid.getCoord(2));
                    }

                    Data::getState().setMidExternal(mid);
                }

				//drawing the mesh normals
				if (this->IsMeshDrawable())
				{
					if ((*iter)->getBuilder())
					{
						//Data::Mesh *m = static_cast<PMJMesh::GeometryFrontAdvancer *>((*iter)->getBuilder())->getMesh();
						const Data::Mesh *m = dynamic_cast<PMJMesh::WorkerMainDrive *>((*iter)->getBuilder())->getMesh();

						if (m)
						{
							m->drawNormals(factor);
						}
					}
				}

				if (this->IsInnerMeshDrawable())
				{
					Data::Point3D mid = Data::getState().getMidExternal();
                    Data::Point3DObjList mids = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMids();
                    Data::Point3D lmid;

                    if ((*iter)->getBuilder())
					{
						Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMeshes();

                        while (!meshes.empty())
                        {
                            Data::Mesh *m = meshes.front();

                            if (Data::getState().getExplodeExternal() != 1.0)
                            {
                                if (mids.empty())
                                {
                                    lmid = mid;
                                }
                                else
                                {
                                    lmid = mids.front();
                                    mids.pop_front();
                                }

                                Data::getState().setMidExternal(lmid);
                            }

                            if (m)
                            {
                                m->drawNormals(factor);
                            }

                            meshes.pop_front();
                        }
					}
					else
                    {
                        if ((!selected) || (selected != p))
                        {
                            Data::MeshList meshes = static_cast<PMJMesh2::MJMeshTask *>((*iter))->getMeshes();

                            while (!meshes.empty())
                            {
                                Data::Mesh *m = meshes.front();

                                if (Data::getState().getExplodeExternal() != 1.0)
                                {
                                    if (mids.empty())
                                    {
                                        lmid = mid;
                                    }
                                    else
                                    {
                                        lmid = mids.front();
                                        mids.pop_front();
                                    }

                                    Data::getState().setMidExternal(lmid);
                                }

                                if (m)
                                {
                                    m->drawNormals(factor);
                                }

                                meshes.pop_front();
                            }
                        }
                    }

                    if (Data::getState().getExplodeExternal() != 1.0)
                    {
                        Data::getState().setMidExternal(mid);
                    }
				}

				//drawing the front normals
				if (this->IsFrontDrawable())
				{
					PMJMesh::Front *f = p->getSubfront();

					if (f)
					{
						f->drawNormals(factor);
					}
				}

				if (this->IsBoundaryDrawable())
				{
					const Data::Boundary *b = dynamic_cast<const PMJMesh::WorkerMainDrive *>((*iter)->getBuilder())->getBoundary();

					if ((b) && (b != this->GetBoundary()))
					{
						b->drawNormals(factor);
					}
				}

                Data::getState().resetMidExternal();
			}
		}
	}
}

void CRAbMesh::PMJMesh2Module::PropagateAPrioriInterfaceMeshGeneration()
{
    this->EnableRangeProportionCtrl();
    this->EnableAPrioriInterfaceElementSizeCtrl();
    this->EnableInnerFrontCtrl();
    //this->EnableMeshUpdateTypesCtrl();
}

void CRAbMesh::PMJMesh2Module::SetParallelismEnabled(bool parallelismEnabled)
{
	this->parallelismEnabled = parallelismEnabled;

	this->miConfEnableParallelism->Check(this->parallelismEnabled);
	this->cbEnableParallelism->SetValue(this->parallelismEnabled);

	this->EnableNumThreadsCtrl();
}

bool CRAbMesh::PMJMesh2Module::IsParallelismEnabled() const
{
	return this->parallelismEnabled;
}

void CRAbMesh::PMJMesh2Module::SetHeightDependentInLoad(bool heightDependentInLoad)
{
	this->heightDependentInLoad = heightDependentInLoad;

	this->miConfHeightDependentInLoad->Check(this->heightDependentInLoad);
	this->cbHeightDependentInLoad->SetValue(this->heightDependentInLoad);
}

bool CRAbMesh::PMJMesh2Module::IsHeightDependentInLoad() const
{
	return this->heightDependentInLoad;
}

void CRAbMesh::PMJMesh2Module::SetNumThreads(unsigned int numThreads)
{
	this->numThreads = numThreads;

	wxString s;
    s << this->numThreads;
    this->tcNumThreads->SetValue(s);
}

unsigned int CRAbMesh::PMJMesh2Module::GetNumThreads() const
{
	if (this->numThreads == 0)
    {
        PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

		this->numThreads = static_cast<PMJMesh2::Communicator *>(drive->getComm())->getMaxThreads();
    }

	return this->numThreads;
}

void CRAbMesh::PMJMesh2Module::SetNumTasksPerThread(unsigned int numTasksPerThread)
{
	this->numTasksPerThread = (numTasksPerThread == 0) ? 1 : numTasksPerThread;

	wxString s;
    s << this->numTasksPerThread;
    this->tcNumTasksPerThread->SetValue(s);
}

unsigned int CRAbMesh::PMJMesh2Module::GetNumTasksPerThread() const
{
	return this->numTasksPerThread;
}

void CRAbMesh::PMJMesh2Module::SetRemoveLayersInLoad(unsigned int removeLayersInLoad)
{
	this->removeLayersInLoad = removeLayersInLoad;

	wxString s;
    s << this->removeLayersInLoad;
    this->tcRemoveLayersInLoad->SetValue(s);
}

unsigned int CRAbMesh::PMJMesh2Module::GetRemoveLayersInLoad() const
{
	return this->removeLayersInLoad;
}

void CRAbMesh::PMJMesh2Module::SetLoadThreshold(double loadThreshold)
{
	this->loadThreshold = loadThreshold;

	wxString s;
    s << this->loadThreshold;
    this->tcLoadThreshold->SetValue(s);
}

double CRAbMesh::PMJMesh2Module::GetLoadThreshold() const
{
	return this->loadThreshold;
}

void CRAbMesh::PMJMesh2Module::SetPartitionerType(wxString partitionerType)
{
	this->partitionerType = partitionerType;

	if (this->cbPartitionerType)
	{
		this->cbPartitionerType->SetStringSelection(this->partitionerType);
	}
}

void CRAbMesh::PMJMesh2Module::SetPartitionerType(enum PMJMesh2::PartitionerType partitionerType)
{
	for (PartitionerTypeMap::iterator iter = this->partitionerTypes.begin();
		 iter != this->partitionerTypes.end(); iter++)
	{
		if ((*iter).second == partitionerType)
		{
			this->SetPartitionerType((*iter).first);

			break;
		}
	}
}

enum PMJMesh2::PartitionerType CRAbMesh::PMJMesh2Module::GetPartitionerType() const
{
	return (*this->partitionerTypes.find(this->partitionerType)).second;
}

wxString CRAbMesh::PMJMesh2Module::GetPartitionerTypeString() const
{
	return this->partitionerType;
}

wxArrayString CRAbMesh::PMJMesh2Module::GetPartitionerTypes() const
{
	wxArrayString s;

	for (PartitionerTypeMap::const_iterator iter = this->partitionerTypes.begin();
		 iter != this->partitionerTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMesh2Module::SetInterfaceMeshGenerationType(wxString interfaceMeshGenerationType)
{
	this->interfaceMeshGenerationType = interfaceMeshGenerationType;

	if (this->cbInterfaceMeshGenerationType)
	{
		this->cbInterfaceMeshGenerationType->SetStringSelection(this->interfaceMeshGenerationType);
	}
}

void CRAbMesh::PMJMesh2Module::SetInterfaceMeshGenerationType(enum PMJMesh2::InterfaceMeshGenerationType interfaceMeshGenerationType)
{
	for (InterfaceMeshGenerationTypeMap::iterator iter = this->interfaceMeshGenerationTypes.begin();
		 iter != this->interfaceMeshGenerationTypes.end(); iter++)
	{
		if ((*iter).second == interfaceMeshGenerationType)
		{
			this->SetInterfaceMeshGenerationType((*iter).first);

			break;
		}
	}
}

enum PMJMesh2::InterfaceMeshGenerationType CRAbMesh::PMJMesh2Module::GetInterfaceMeshGenerationType() const
{
	return (*this->interfaceMeshGenerationTypes.find(this->interfaceMeshGenerationType)).second;
}

wxString CRAbMesh::PMJMesh2Module::GetInterfaceMeshGenerationTypeString() const
{
	return this->interfaceMeshGenerationType;
}

bool CRAbMesh::PMJMesh2Module::IsInterfaceMeshGenerationAPriori() const
{
    enum PMJMesh2::InterfaceMeshGenerationType interfaceMeshGenerationType = this->GetInterfaceMeshGenerationType();

    return (interfaceMeshGenerationType != PMJMesh2::A_POSTERIORI);
}

wxArrayString CRAbMesh::PMJMesh2Module::GetInterfaceMeshGenerationTypes() const
{
	wxArrayString s;

	for (InterfaceMeshGenerationTypeMap::const_iterator iter = this->interfaceMeshGenerationTypes.begin();
		 iter != this->interfaceMeshGenerationTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMesh2Module::SetCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition)
{
	this->creatingInnerFrontInDecomposition = creatingInnerFrontInDecomposition;

	this->miConfCreatingInnerFrontInDecomposition->Check(this->creatingInnerFrontInDecomposition);
	this->cbCreatingInnerFrontInDecomposition->SetValue(this->creatingInnerFrontInDecomposition);
}

bool CRAbMesh::PMJMesh2Module::IsCreatingInnerFrontInDecomposition() const
{
	return this->creatingInnerFrontInDecomposition;
}

void CRAbMesh::PMJMesh2Module::SetRangeProportion(double rangeProportion)
{
	this->rangeProportion = rangeProportion;

	wxString s;
    s << this->rangeProportion;
    this->tcRangeProportion->SetValue(s);
}

double CRAbMesh::PMJMesh2Module::GetRangeProportion() const
{
	return this->rangeProportion;
}

void CRAbMesh::PMJMesh2Module::SetAPrioriInterfaceElementSizeFactor(double aPrioriInterfaceElementSizeFactor)
{
	this->aPrioriInterfaceElementSizeFactor = aPrioriInterfaceElementSizeFactor;

	wxString s;
    s << this->aPrioriInterfaceElementSizeFactor;
    this->tcAPrioriInterfaceElementSizeFactor->SetValue(s);
}

double CRAbMesh::PMJMesh2Module::GetAPrioriInterfaceElementSizeFactor() const
{
	return this->aPrioriInterfaceElementSizeFactor;
}

void CRAbMesh::PMJMesh2Module::SetAPrioriInterfaceBadAngleCos(double aPrioriInterfaceBadAngleCos)
{
	this->aPrioriInterfaceBadAngleCos = aPrioriInterfaceBadAngleCos;

	wxString s;
    s << this->aPrioriInterfaceBadAngleCos;
    this->tcAPrioriInterfaceBadAngleCos->SetValue(s);
}

double CRAbMesh::PMJMesh2Module::GetAPrioriInterfaceBadAngleCos() const
{
	return this->aPrioriInterfaceBadAngleCos;
}

void CRAbMesh::PMJMesh2Module::SetAPrioriInterfaceShiftDelta(double aPrioriInterfaceShiftDelta)
{
	this->aPrioriInterfaceShiftDelta = aPrioriInterfaceShiftDelta;

	wxString s;
    s << this->aPrioriInterfaceShiftDelta;
    this->tcAPrioriInterfaceShiftDelta->SetValue(s);
}

double CRAbMesh::PMJMesh2Module::GetAPrioriInterfaceShiftDelta() const
{
	return this->aPrioriInterfaceShiftDelta;
}

void CRAbMesh::PMJMesh2Module::SetImprovementInSlavesEnabled(bool improvementInSlavesEnabled)
{
	this->improvementInSlavesEnabled = improvementInSlavesEnabled;

	this->miConfEnableImprovementInSlaves->Check(this->improvementInSlavesEnabled);
	this->cbEnableImprovementInSlaves->SetValue(this->improvementInSlavesEnabled);
}

bool CRAbMesh::PMJMesh2Module::IsImprovementInSlavesEnabled() const
{
	return this->improvementInSlavesEnabled;
}

void CRAbMesh::PMJMesh2Module::SetMeshUpdateType(wxString meshUpdateType)
{
	this->meshUpdateType = meshUpdateType;

	if (this->cbMeshUpdateType)
	{
		this->cbMeshUpdateType->SetValue(this->meshUpdateType);
	}
}

void CRAbMesh::PMJMesh2Module::SetMeshUpdateType(enum PMJMesh2::MeshUpdateType meshUpdateType)
{
	for (MeshUpdateTypeMap::iterator iter = this->meshUpdateTypes.begin();
		 iter != this->meshUpdateTypes.end(); iter++)
	{
		if ((*iter).second == meshUpdateType)
		{
			this->SetMeshUpdateType((*iter).first);

			break;
		}
	}
}

enum PMJMesh2::MeshUpdateType CRAbMesh::PMJMesh2Module::GetMeshUpdateType() const
{
	return (*this->meshUpdateTypes.find(this->meshUpdateType)).second;
}

wxString CRAbMesh::PMJMesh2Module::GetMeshUpdateTypeString() const
{
	return this->meshUpdateType;
}

wxArrayString CRAbMesh::PMJMesh2Module::GetMeshUpdateTypes() const
{
	wxArrayString s;

	for (MeshUpdateTypeMap::const_iterator iter = this->meshUpdateTypes.begin();
		 iter != this->meshUpdateTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMesh2Module::SetImprovementLayers(unsigned int improvementLayers)
{
    this->improvementLayers = improvementLayers;

	wxString s;
    s << this->improvementLayers;
    this->tcImprovementLayers->SetValue(s);
}

unsigned int CRAbMesh::PMJMesh2Module::GetImprovementLayers() const
{
    return this->improvementLayers;
}

void CRAbMesh::PMJMesh2Module::SetFinalMeshState(wxString finalMeshState)
{
	this->finalMeshState = finalMeshState;

	if (this->cbFinalMeshState)
	{
		this->cbFinalMeshState->SetValue(this->finalMeshState);
	}
}

void CRAbMesh::PMJMesh2Module::SetFinalMeshState(enum PMJMesh2::FinalMeshState finalMeshState)
{
	for (FinalMeshStateMap::iterator iter = this->finalMeshStates.begin();
		 iter != this->finalMeshStates.end(); iter++)
	{
		if ((*iter).second == finalMeshState)
		{
			this->SetFinalMeshState((*iter).first);

			break;
		}
	}
}

enum PMJMesh2::FinalMeshState CRAbMesh::PMJMesh2Module::GetFinalMeshState() const
{
	return (*this->finalMeshStates.find(this->finalMeshState)).second;
}

wxString CRAbMesh::PMJMesh2Module::GetFinalMeshStateString() const
{
	return this->finalMeshState;
}

wxArrayString CRAbMesh::PMJMesh2Module::GetFinalMeshStates() const
{
	wxArrayString s;

	for (FinalMeshStateMap::const_iterator iter = this->finalMeshStates.begin();
		 iter != this->finalMeshStates.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMesh2Module::SetPartitionsDrawable(bool partitionsDrawable)
{
	this->SetConfiguration(DRAW_PARTITIONS, partitionsDrawable);
}

bool CRAbMesh::PMJMesh2Module::IsPartitionsDrawable() const
{
	return this->IsConfiguration(DRAW_PARTITIONS);
}

void CRAbMesh::PMJMesh2Module::SetPartitionsGeneratedStateDrawable(bool partitionsGeneratedStateDrawable)
{
	this->SetConfiguration(DRAW_PARTITION_STATES, partitionsGeneratedStateDrawable);
}

bool CRAbMesh::PMJMesh2Module::IsPartitionsGeneratedStateDrawable() const
{
	return this->IsConfiguration(DRAW_PARTITION_STATES);
}

void CRAbMesh::PMJMesh2Module::SetInnerMeshDrawable(bool innerMeshesDrawable)
{
	this->SetConfiguration(DRAW_INNER_MESHES, innerMeshesDrawable);
}

bool CRAbMesh::PMJMesh2Module::IsInnerMeshDrawable() const
{
	return this->IsConfiguration(DRAW_INNER_MESHES);
}

bool CRAbMesh::PMJMesh2Module::IsDrawable() const
{
	return (MJMeshModule::IsDrawable() ||
			this->IsInnerMeshDrawable() ||
			this->IsPartitionsDrawable());
}

void CRAbMesh::PMJMesh2Module::MakeMainDrive()
{
	this->drive = new PMJMesh2::MainDrive();
}

wxMenu *CRAbMesh::PMJMesh2Module::MakeConfigurationMenu()
{
	wxMenu *mConfiguration = MJMeshModule::MakeConfigurationMenu();

	unsigned int pos = 0;

	for (; pos < mConfiguration->GetMenuItemCount(); pos++)
	{
		if (mConfiguration->FindItemByPosition(pos) == this->miConfShowProgress)
		{
			break;
		}
	}

	if (pos < mConfiguration->GetMenuItemCount())
	{
		pos++;
	}

	//menu items
	mConfiguration->InsertSeparator(pos);
	pos++;
	this->miConfEnableParallelism         = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Shared memory parallelism"));
	pos++;
	this->miConfHeightDependentInLoad     = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Load depends on height"));
	pos++;
	this->miConfCreatingInnerFrontInDecomposition = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Create front in inner subdomains"));
	pos++;
	this->miConfEnableImprovementInSlaves = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Enable improvement in slaves"));

	this->miConfEnableParallelism->Check(this->IsParallelismEnabled());
	this->miConfHeightDependentInLoad->Check(this->IsHeightDependentInLoad());
	this->miConfCreatingInnerFrontInDecomposition->Check(this->IsCreatingInnerFrontInDecomposition());
	this->miConfEnableImprovementInSlaves->Check(this->IsImprovementInSlavesEnabled());

	this->Connect(this->miConfEnableParallelism->GetId(),         wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfEnableParallelism));
	this->Connect(this->miConfHeightDependentInLoad->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfHeightDependentInLoad));
	this->Connect(this->miConfCreatingInnerFrontInDecomposition->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfCreatingInnerFrontInDecomposition));
    this->Connect(this->miConfEnableImprovementInSlaves->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfEnableImprovementInSlaves));

    return mConfiguration;
}

wxPanel *CRAbMesh::PMJMesh2Module::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MJMeshModule::MakeConfigurationPanel();

	this->cbEnableParallelism = new wxCheckBox(pConfiguration, wxID_ANY, _("Shared memory parallelism"));

	wxStaticText *stNumThreads = new wxStaticText(pConfiguration, wxID_ANY, _("Number of threads:"));
    this->tcNumThreads         = new wxTextCtrl(  pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stNumTasksPerThread = new wxStaticText(pConfiguration, wxID_ANY, _("Number of tasks per thread:"));
    this->tcNumTasksPerThread         = new wxTextCtrl(  pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	this->cbHeightDependentInLoad = new wxCheckBox(pConfiguration, wxID_ANY, _("Load depends on height"));

	wxStaticText *stRemoveLayersInLoad = new wxStaticText(pConfiguration, wxID_ANY, _("Layers removed in load:"));
    this->tcRemoveLayersInLoad         = new wxTextCtrl(  pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stLoadThreshold = new wxStaticText(pConfiguration, wxID_ANY, _("Load threshold:"));
    this->tcLoadThreshold         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stPartitionerType = new wxStaticText(pConfiguration, wxID_ANY, _("Partitioning type:"));
    this->cbPartitionerType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetPartitionerTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetPartitionerTypes(), wxCB_READONLY);

#if USE_FILE_BASED_BSP
    this->bLoadBSPFile = new wxButton(pConfiguration, wxID_ANY, _("Load BSP file"));
#endif //#if USE_FILE_BASED_BSP

    wxStaticText *stInterfaceMeshGenerationType = new wxStaticText(pConfiguration, wxID_ANY, _("Interface generation:"));
    this->cbInterfaceMeshGenerationType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetInterfaceMeshGenerationTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetInterfaceMeshGenerationTypes(), wxCB_READONLY);

    this->cbCreatingInnerFrontInDecomposition = new wxCheckBox(pConfiguration, wxID_ANY, _("Create front in inner subdomains"));

	wxStaticText *stRangeProportion = new wxStaticText(pConfiguration, wxID_ANY, _("Range proportion:"));
    this->tcRangeProportion         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("1.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stAPrioriInterfaceBadAngleCos = new wxStaticText(pConfiguration, wxID_ANY, _("Interface bad angle (cos):"));
    this->tcAPrioriInterfaceBadAngleCos         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("0.95"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stAPrioriInterfaceShiftDelta = new wxStaticText(pConfiguration, wxID_ANY, _("Interface shift delta factor:"));
    this->tcAPrioriInterfaceShiftDelta         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("2.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stAPrioriInterfaceElementSizeFactor = new wxStaticText(pConfiguration, wxID_ANY, _("Interface elem size factor:"));
    this->tcAPrioriInterfaceElementSizeFactor         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("1.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	this->cbEnableImprovementInSlaves = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable improvement in slaves"));

	wxStaticText *stImprovementLayers = new wxStaticText(pConfiguration, wxID_ANY, _("Layers improved:"));
    this->tcImprovementLayers         = new wxTextCtrl(  pConfiguration, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stMeshUpdateType = new wxStaticText(pConfiguration, wxID_ANY, _("Mesh update type:"));
    this->cbMeshUpdateType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetMeshUpdateTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetMeshUpdateTypes(), wxCB_READONLY);

    wxStaticText *stFinalMeshState = new wxStaticText(pConfiguration, wxID_ANY, _("Final mesh state:"));
    this->cbFinalMeshState         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetFinalMeshStateString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetFinalMeshStates(), wxCB_READONLY);

    this->cbEnableParallelism->SetValue(this->IsParallelismEnabled());
	this->cbHeightDependentInLoad->SetValue(this->IsHeightDependentInLoad());
	this->cbCreatingInnerFrontInDecomposition->SetValue(this->IsCreatingInnerFrontInDecomposition());
	this->cbEnableImprovementInSlaves->SetValue(this->IsImprovementInSlavesEnabled());

	wxString s;

	s = _("");
    s << this->GetNumThreads();
    this->tcNumThreads->SetValue(s);

    s = _("");
    s << this->GetNumTasksPerThread();
    this->tcNumTasksPerThread->SetValue(s);

    s = _("");
    s << this->GetRemoveLayersInLoad();
    this->tcRemoveLayersInLoad->SetValue(s);

    s = _("");
    s << this->GetLoadThreshold();
    this->tcLoadThreshold->SetValue(s);

    s = _("");
    s << this->GetRangeProportion();
    this->tcRangeProportion->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceBadAngleCos();
    this->tcAPrioriInterfaceBadAngleCos->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceShiftDelta();
    this->tcAPrioriInterfaceShiftDelta->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceElementSizeFactor();
    this->tcAPrioriInterfaceElementSizeFactor->SetValue(s);

    s = _("");
    s << this->GetImprovementLayers();
    this->tcImprovementLayers->SetValue(s);

    this->PropagateAPrioriInterfaceMeshGeneration();
    this->EnableNumThreadsCtrl();

    wxSizer *sConf = pConfiguration->GetSizer();
	wxSizer *sSerial = sConf->GetItem(static_cast<size_t>(0))->GetSizer();
	wxSizer *sDrawing = sConf->GetItem(1)->GetSizer();
	wxSizer *sExecute = sConf->GetItem(2)->GetSizer();
	sConf->Detach(sSerial);
	sConf->Detach(sDrawing);
	sConf->Detach(sExecute);

	wxStaticBoxSizer *sbsSerialConf = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Serial options"));
	sbsSerialConf->Add(sSerial, 0, wxEXPAND | wxALL, 1);

	int row = 0;
	wxGridBagSizer *sParallel = new wxGridBagSizer();
    sParallel->SetFlexibleDirection(wxBOTH);
    sParallel->AddGrowableCol(0);
	//sParallel->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    sParallel->Add(this->cbEnableParallelism,           wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stNumThreads,                        wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcNumThreads,                  wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stNumTasksPerThread,                 wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcNumTasksPerThread,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(this->cbHeightDependentInLoad,       wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stRemoveLayersInLoad,                wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcRemoveLayersInLoad,          wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stLoadThreshold,                     wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcLoadThreshold,               wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stPartitionerType,                   wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbPartitionerType,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
#if USE_FILE_BASED_BSP
    sParallel->Add(this->bLoadBSPFile,                  wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#endif //#if USE_FILE_BASED_BSP
	sParallel->Add(stInterfaceMeshGenerationType,       wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbInterfaceMeshGenerationType, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	sParallel->Add(this->cbCreatingInnerFrontInDecomposition, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stRangeProportion,                   wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcRangeProportion,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stAPrioriInterfaceBadAngleCos,       wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcAPrioriInterfaceBadAngleCos, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stAPrioriInterfaceShiftDelta,        wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcAPrioriInterfaceShiftDelta,  wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stAPrioriInterfaceElementSizeFactor,       wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcAPrioriInterfaceElementSizeFactor, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(this->cbEnableImprovementInSlaves,   wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stImprovementLayers,                 wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcImprovementLayers,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stMeshUpdateType,                    wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbMeshUpdateType,              wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	sParallel->Add(stFinalMeshState,                    wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbFinalMeshState,              wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	wxStaticBoxSizer *sbsParallelConf = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Parallel options"));
	sbsParallelConf->Add(sParallel, 0, wxEXPAND | wxALL, 1);

	sConf->Add(sbsSerialConf,   0, wxEXPAND | wxALL, 1);
	sConf->Add(sbsParallelConf, 0, wxEXPAND | wxALL, 1);
	sConf->Add(sDrawing,        0, wxEXPAND | wxALL, 1);
    sConf->Add(sExecute,        0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);

    sConf->Layout();

    this->tcNumThreads->Connect(        wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcNumTasksPerThread->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcRemoveLayersInLoad->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcLoadThreshold->Connect(     wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcRangeProportion->Connect(   wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcAPrioriInterfaceBadAngleCos->Connect(   wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcAPrioriInterfaceShiftDelta->Connect(   wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcAPrioriInterfaceElementSizeFactor->Connect(   wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);
    this->tcImprovementLayers->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMesh2Module::OnKillFocus), NULL, this);

    this->Connect(this->cbEnableParallelism->GetId(),         wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMesh2Module::OnConfEnableParallelism));
    this->Connect(this->cbHeightDependentInLoad->GetId(),     wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMesh2Module::OnConfHeightDependentInLoad));
    this->Connect(this->cbCreatingInnerFrontInDecomposition->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMesh2Module::OnConfCreatingInnerFrontInDecomposition));
    this->Connect(this->cbEnableImprovementInSlaves->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMesh2Module::OnConfEnableImprovementInSlaves));

    this->Connect(this->cbPartitionerType->GetId(),             wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfPartitionerType));
    this->Connect(this->cbInterfaceMeshGenerationType->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfInterfaceMeshGenerationType));
    this->Connect(this->cbMeshUpdateType->GetId(),              wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfMeshUpdateType));
    this->Connect(this->cbFinalMeshState->GetId(),              wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMesh2Module::OnConfFinalMeshState));

#if USE_FILE_BASED_BSP
    this->Connect(this->bLoadBSPFile->GetId(),                  wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PMJMesh2Module::OnLoadBSPFile));
#endif //#if USE_FILE_BASED_BSP

    return pConfiguration;
}

wxDialog *CRAbMesh::PMJMesh2Module::MakeConfigurationDialog()
{
	wxDialog *dConf = MJMeshModule::MakeConfigurationDialog();

	wxPropertySheetDialog *psdConf = static_cast<wxPropertySheetDialog *>(dConf);

	wxBookCtrlBase *nConf = psdConf->GetBookCtrl();

	wxPanel *pConf = new wxPanel(nConf);

	wxStaticText *stNumThreads = new wxStaticText(pConf, wxID_ANY, _("Number of threads:"));
    wxTextCtrl *tcNumThreads   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stNumTasksPerThread = new wxStaticText(pConf, wxID_ANY, _("Number of tasks per thread:"));
    wxTextCtrl *tcNumTasksPerThread   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stRemoveLayersInLoad = new wxStaticText(pConf, wxID_ANY, _("Layers removed in load:"));
    wxTextCtrl *tcRemoveLayersInLoad   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stLoadThreshold = new wxStaticText(pConf, wxID_ANY, _("Load threshold:"));
    wxTextCtrl *tcLoadThreshold   = new wxTextCtrl(  pConf, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stPartitionerType = new wxStaticText(pConf, wxID_ANY, _("Partitioning type:"));
    wxComboBox *cbPartitionerType   = new wxComboBox(  pConf, wxID_ANY, this->GetPartitionerTypeString(), wxDefaultPosition, wxDefaultSize, this->GetPartitionerTypes(), wxCB_READONLY);

	wxStaticText *stInterfaceMeshGenerationType = new wxStaticText(pConf, wxID_ANY, _("Interface generation:"));
    wxComboBox *cbInterfaceMeshGenerationType   = new wxComboBox(  pConf, wxID_ANY, this->GetInterfaceMeshGenerationTypeString(), wxDefaultPosition, wxDefaultSize, this->GetInterfaceMeshGenerationTypes(), wxCB_READONLY);

	wxStaticText *stRangeProportion = new wxStaticText(pConf, wxID_ANY, _("Range proportion:"));
    wxTextCtrl *tcRangeProportion   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stAPrioriInterfaceBadAngleCos = new wxStaticText(pConf, wxID_ANY, _("Interface bad angle (cos):"));
    wxTextCtrl *tcAPrioriInterfaceBadAngleCos   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stAPrioriInterfaceShiftDelta = new wxStaticText(pConf, wxID_ANY, _("Interface shift delta factor:"));
    wxTextCtrl *tcAPrioriInterfaceShiftDelta   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stAPrioriInterfaceElementSizeFactor = new wxStaticText(pConf, wxID_ANY, _("Interface element size factor:"));
    wxTextCtrl *tcAPrioriInterfaceElementSizeFactor   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stImprovementLayers = new wxStaticText(pConf, wxID_ANY, _("Layers improved:"));
    wxTextCtrl *tcImprovementLayers   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stMeshUpdateType = new wxStaticText(pConf, wxID_ANY, _("Mesh update type:"));
    wxComboBox *cbMeshUpdateType   = new wxComboBox(  pConf, wxID_ANY, this->GetMeshUpdateTypeString(), wxDefaultPosition, wxDefaultSize, this->GetMeshUpdateTypes(), wxCB_READONLY);

    wxStaticText *stFinalMeshState = new wxStaticText(pConf, wxID_ANY, _("Final mesh state:"));
    wxComboBox *cbFinalMeshState   = new wxComboBox(  pConf, wxID_ANY, this->GetFinalMeshStateString(), wxDefaultPosition, wxDefaultSize, this->GetFinalMeshStates(), wxCB_READONLY);

    wxString s;

    s = _("");
    s << this->GetNumThreads();
    tcNumThreads->SetValue(s);

    s = _("");
    s << this->GetNumTasksPerThread();
    tcNumTasksPerThread->SetValue(s);

    s = _("");
    s << this->GetRemoveLayersInLoad();
    tcRemoveLayersInLoad->SetValue(s);

    s = _("");
    s << this->GetLoadThreshold();
    tcLoadThreshold->SetValue(s);

    s = _("");
    s << this->GetRangeProportion();
    tcRangeProportion->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceBadAngleCos();
    tcAPrioriInterfaceBadAngleCos->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceShiftDelta();
    tcAPrioriInterfaceShiftDelta->SetValue(s);

    s = _("");
    s << this->GetAPrioriInterfaceElementSizeFactor();
    tcAPrioriInterfaceElementSizeFactor->SetValue(s);

    s = _("");
    s << this->GetImprovementLayers();
    tcImprovementLayers->SetValue(s);

    tcRangeProportion->Enable(!this->IsInterfaceMeshGenerationAPriori());
    tcAPrioriInterfaceBadAngleCos->Enable(this->IsInterfaceMeshGenerationAPriori());
    tcAPrioriInterfaceShiftDelta->Enable(this->IsInterfaceMeshGenerationAPriori());
    tcAPrioriInterfaceElementSizeFactor->Enable(this->IsInterfaceMeshGenerationAPriori());
    tcNumThreads->Enable(this->IsParallelismEnabled());

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
    gbsConf->Add(stNumThreads,                  wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcNumThreads,                  wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stNumTasksPerThread,           wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcNumTasksPerThread,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stRemoveLayersInLoad,          wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcRemoveLayersInLoad,          wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stLoadThreshold,               wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcLoadThreshold,               wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stPartitionerType,             wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbPartitionerType,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stInterfaceMeshGenerationType, wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbInterfaceMeshGenerationType, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stRangeProportion,             wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcRangeProportion,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stAPrioriInterfaceBadAngleCos, wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcAPrioriInterfaceBadAngleCos, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stAPrioriInterfaceShiftDelta,  wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcAPrioriInterfaceShiftDelta,  wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stAPrioriInterfaceElementSizeFactor, wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcAPrioriInterfaceElementSizeFactor, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stImprovementLayers,           wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcImprovementLayers,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stMeshUpdateType,              wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbMeshUpdateType,              wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stFinalMeshState,              wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbFinalMeshState,              wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->AddGrowableCol(0);
	gbsConf->AddGrowableCol(1);

	wxBoxSizer *sCond = new wxBoxSizer(wxVERTICAL);
	sCond->Add(gbsConf, 1, wxEXPAND | wxALL, 0);
	pConf->SetSizer(sCond);

	nConf->SetPageText(0, _("Serial"));
	nConf->AddPage(pConf, _("Parallel"), false);

	psdConf->LayoutDialog();

	return psdConf;
}

void CRAbMesh::PMJMesh2Module::OnConfigurationDialogOk(const wxDialog *dConf)
{
	MJMeshModule::OnConfigurationDialogOk(dConf);

	const wxPropertySheetDialog *psdConf = static_cast<const wxPropertySheetDialog *>(dConf);

	double rangeProportion, loadThreshold;
	wxString partitionerType, interfaceMeshGenerationType, meshUpdateType, finalMeshState;
	unsigned long int numThreads, numTasksPerThread, removeLayersInLoad, improvementLayers;

	wxWindow *pConf = psdConf->GetBookCtrl()->GetPage(1);

	if (this->IsParallelismEnabled()) static_cast<wxTextCtrl *>(pConf->GetChildren()[1])->GetValue().ToULong(&numThreads);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[3])->GetValue().ToULong(&numTasksPerThread);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[5])->GetValue().ToULong(&removeLayersInLoad);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[7])->GetValue().ToDouble(&loadThreshold);
	partitionerType = static_cast<wxComboBox *>(pConf->GetChildren()[9])->GetValue();
	interfaceMeshGenerationType = static_cast<wxComboBox *>(pConf->GetChildren()[11])->GetValue();
	static_cast<wxTextCtrl *>(pConf->GetChildren()[13])->GetValue().ToDouble(&rangeProportion);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[15])->GetValue().ToULong(&improvementLayers);
	meshUpdateType = static_cast<wxComboBox *>(pConf->GetChildren()[17])->GetValue();
	finalMeshState = static_cast<wxComboBox *>(pConf->GetChildren()[19])->GetValue();

	if (this->IsParallelismEnabled()) this->SetNumThreads(static_cast<unsigned int>(numThreads));
	this->SetNumTasksPerThread(static_cast<unsigned int>(numTasksPerThread));
	this->SetRemoveLayersInLoad(static_cast<unsigned int>(removeLayersInLoad));
	this->SetLoadThreshold(loadThreshold);
	this->SetPartitionerType(partitionerType);
	this->SetInterfaceMeshGenerationType(interfaceMeshGenerationType);
	this->SetRangeProportion(rangeProportion);
	this->SetImprovementLayers(static_cast<unsigned int>(improvementLayers));
	this->SetMeshUpdateType(meshUpdateType);
	this->SetFinalMeshState(finalMeshState);
}

void CRAbMesh::PMJMesh2Module::OnConfEnableParallelism(wxCommandEvent &WXUNUSED(event))
{
	this->SetParallelismEnabled(!this->IsParallelismEnabled());
}

void CRAbMesh::PMJMesh2Module::OnConfHeightDependentInLoad(wxCommandEvent &WXUNUSED(event))
{
	this->SetHeightDependentInLoad(!this->IsHeightDependentInLoad());
}

void CRAbMesh::PMJMesh2Module::OnConfCreatingInnerFrontInDecomposition(wxCommandEvent &WXUNUSED(event))
{
	this->SetCreatingInnerFrontInDecomposition(!this->IsCreatingInnerFrontInDecomposition());
}

void CRAbMesh::PMJMesh2Module::OnConfEnableImprovementInSlaves(wxCommandEvent &WXUNUSED(event))
{
	this->SetImprovementInSlavesEnabled(!this->IsImprovementInSlavesEnabled());
}

void CRAbMesh::PMJMesh2Module::OnConfPartitionerType(wxCommandEvent &event)
{
	if (event.GetId() == this->cbPartitionerType->GetId())
	{
		this->SetPartitionerType(this->cbPartitionerType->GetValue());
	}
}

void CRAbMesh::PMJMesh2Module::OnConfInterfaceMeshGenerationType(wxCommandEvent &event)
{
	if (event.GetId() == this->cbInterfaceMeshGenerationType->GetId())
	{
		this->SetInterfaceMeshGenerationType(this->cbInterfaceMeshGenerationType->GetValue());

		this->PropagateAPrioriInterfaceMeshGeneration();
	}
}

void CRAbMesh::PMJMesh2Module::OnConfMeshUpdateType(wxCommandEvent &event)
{
	if (event.GetId() == this->cbMeshUpdateType->GetId())
	{
		this->SetMeshUpdateType(this->cbMeshUpdateType->GetValue());
	}
}

void CRAbMesh::PMJMesh2Module::OnConfFinalMeshState(wxCommandEvent &event)
{
	if (event.GetId() == this->cbFinalMeshState->GetId())
	{
		this->SetFinalMeshState(this->cbFinalMeshState->GetValue());
	}
}

#if USE_FILE_BASED_BSP
void CRAbMesh::PMJMesh2Module::OnLoadBSPFile(wxCommandEvent &event)
{
    wxString wildcard;

    wildcard << _("BSP Files (*.aacop)|*.aacop");

    wxFileDialog *open = new wxFileDialog(this->frame, _("Open file"), this->frame->GetOpenPath(), _(""),
        wildcard,
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
#else
        wxOPEN | wxFILE_MUST_EXIST);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (open->ShowModal() == wxID_OK)
    {
        wxString path = open->GetPath();

        std::string inputFile(path.mb_str(wxMBConvLibc()));

		dynamic_cast<PMJMesh2::MainDrive *>(this->drive)->setPartitionBSPFileName(inputFile);
    }

    delete open;
}
#endif //#if USE_FILE_BASED_BSP

void CRAbMesh::PMJMesh2Module::OnKillFocus(wxFocusEvent &event)
{
	if (event.GetId() == this->tcNumThreads->GetId())
	{
		unsigned long int numThreads;

		this->tcNumThreads->GetValue().ToULong(&numThreads);

		this->SetNumThreads(static_cast<unsigned int>(numThreads));

		event.Skip(true);
	}
	else if (event.GetId() == this->tcNumTasksPerThread->GetId())
	{
		unsigned long int numTasksPerThread;

		this->tcNumTasksPerThread->GetValue().ToULong(&numTasksPerThread);

		this->SetNumTasksPerThread(static_cast<unsigned int>(numTasksPerThread));

		event.Skip(true);
	}
	else if (event.GetId() == this->tcRemoveLayersInLoad->GetId())
	{
		unsigned long int removeLayersInLoad;

		this->tcRemoveLayersInLoad->GetValue().ToULong(&removeLayersInLoad);

		this->SetRemoveLayersInLoad(static_cast<unsigned int>(removeLayersInLoad));

		event.Skip(true);
	}
	else if (event.GetId() == this->tcLoadThreshold->GetId())
	{
		double loadThreshold;

		this->tcLoadThreshold->GetValue().ToDouble(&loadThreshold);

		this->SetLoadThreshold(loadThreshold);

		event.Skip(true);
	}
	else if (event.GetId() == this->tcRangeProportion->GetId())
	{
		double rangeProportion;

		this->tcRangeProportion->GetValue().ToDouble(&rangeProportion);

		this->SetRangeProportion(rangeProportion);

		event.Skip(true);
	}
	else if (event.GetId() == this->tcAPrioriInterfaceBadAngleCos->GetId())
	{
		double aPrioriInterfaceBadAngleCos;

		this->tcAPrioriInterfaceBadAngleCos->GetValue().ToDouble(&aPrioriInterfaceBadAngleCos);

		this->SetAPrioriInterfaceBadAngleCos(aPrioriInterfaceBadAngleCos);

		event.Skip(true);
	}
	else if (event.GetId() == this->tcAPrioriInterfaceShiftDelta->GetId())
	{
		double aPrioriInterfaceShiftDelta;

		this->tcAPrioriInterfaceShiftDelta->GetValue().ToDouble(&aPrioriInterfaceShiftDelta);

		this->SetAPrioriInterfaceShiftDelta(aPrioriInterfaceShiftDelta);

		event.Skip(true);
	}
	else if (event.GetId() == this->tcAPrioriInterfaceElementSizeFactor->GetId())
	{
		double aPrioriInterfaceElementSizeFactor;

		this->tcAPrioriInterfaceElementSizeFactor->GetValue().ToDouble(&aPrioriInterfaceElementSizeFactor);

		this->SetAPrioriInterfaceElementSizeFactor(aPrioriInterfaceElementSizeFactor);

		event.Skip(true);
	}
	else if (event.GetId() == this->tcImprovementLayers->GetId())
	{
		unsigned long int improvementLayers;

		this->tcImprovementLayers->GetValue().ToULong(&improvementLayers);

		this->SetImprovementLayers(static_cast<unsigned int>(improvementLayers));

		event.Skip(true);
	}
	else
	{
		MJMeshModule::OnKillFocus(event);
	}
}

void CRAbMesh::PMJMesh2Module::OnTreeMiddleClick(wxTreeEvent &event)
{
	wxTreeItemId item = event.GetItem();

	const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

	if ((this->IsEnabled(PARTITIONS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[PARTITIONS]))
	{
		Data::GeometricShape *s = static_cast<const PMJMesh::Partition *>(d)->getBox();

		this->canvas->Center(s);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else
	{
		MJMeshModule::OnTreeMiddleClick(event);
	}
}

void CRAbMesh::PMJMesh2Module::EnableRangeProportionCtrl()
{
	if (this->tcRangeProportion)
	{
		this->tcRangeProportion->Enable(!this->IsInterfaceMeshGenerationAPriori());
	}
}

void CRAbMesh::PMJMesh2Module::EnableAPrioriInterfaceElementSizeCtrl()
{
    if (this->tcAPrioriInterfaceBadAngleCos)
	{
		this->tcAPrioriInterfaceBadAngleCos->Enable(this->IsInterfaceMeshGenerationAPriori());
	}

	if (this->tcAPrioriInterfaceShiftDelta)
	{
		this->tcAPrioriInterfaceShiftDelta->Enable(this->IsInterfaceMeshGenerationAPriori());
	}

	if (this->tcAPrioriInterfaceElementSizeFactor)
	{
		this->tcAPrioriInterfaceElementSizeFactor->Enable(this->IsInterfaceMeshGenerationAPriori());
	}
}

void CRAbMesh::PMJMesh2Module::EnableInnerFrontCtrl()
{
	if (this->miConfCreatingInnerFrontInDecomposition)
	{
		this->miConfCreatingInnerFrontInDecomposition->Enable(!this->IsInterfaceMeshGenerationAPriori());
	}

	if (this->cbCreatingInnerFrontInDecomposition)
	{
		this->cbCreatingInnerFrontInDecomposition->Enable(!this->IsInterfaceMeshGenerationAPriori());
	}
}

/*void CRAbMesh::PMJMesh2Module::EnableMeshUpdateTypesCtrl()
{
	if (this->cbMeshUpdateType)
	{
		if (this->IsInterfaceMeshGenerationAPriori())
		{
			if (this->GetMeshUpdateType() != PMJMesh2::FULL)
			{
				this->SetMeshUpdateType(PMJMesh2::EMPTY);
			}

			this->meshUpdateTypes.clear();

			this->meshUpdateTypes[_("1 - Full mesh and front")] = PMJMesh2::MeshUpdateType::FULL;
			//this->meshUpdateTypes[_("2 - Partial mesh and full front")] = PMJMesh2::MeshUpdateType::PARTIAL;
			//this->meshUpdateTypes[_("3 - Partial mesh and full front (no boundary)")] = PMJMesh2::MeshUpdateType::PARTIAL_NO_BOUNDARY;
			this->meshUpdateTypes[_("4 - Only front")] = PMJMesh2::MeshUpdateType::EMPTY;
			//this->meshUpdateTypes[_("5 - Only front (no boundary)")] = PMJMesh2::MeshUpdateType::EMPTY_NO_BOUNDARY;
			//this->meshUpdateTypes[_("6 - Only final front")] = PMJMesh2::MeshUpdateType::FINAL;
		}
		else
		{
			this->meshUpdateTypes.clear();

			this->meshUpdateTypes[_("1 - Full mesh and front")] = PMJMesh2::MeshUpdateType::FULL;
			this->meshUpdateTypes[_("2 - Partial mesh and full front")] = PMJMesh2::MeshUpdateType::PARTIAL;
			this->meshUpdateTypes[_("3 - Partial mesh and full front (no boundary)")] = PMJMesh2::MeshUpdateType::PARTIAL_NO_BOUNDARY;
			this->meshUpdateTypes[_("4 - Only front")] = PMJMesh2::MeshUpdateType::EMPTY;
			this->meshUpdateTypes[_("5 - Only front (no boundary)")] = PMJMesh2::MeshUpdateType::EMPTY_NO_BOUNDARY;
			this->meshUpdateTypes[_("6 - Only final front")] = PMJMesh2::MeshUpdateType::FINAL;
		}

		this->cbMeshUpdateType->Clear();

		this->cbMeshUpdateType->Append(this->GetMeshUpdateTypes());

		if ((this->IsInterfaceMeshGenerationAPriori()) &&
			(this->GetMeshUpdateType() != PMJMesh2::FULL))
		{
			this->SetMeshUpdateType(PMJMesh2::EMPTY);
		}
	}
}*/

void CRAbMesh::PMJMesh2Module::EnableNumThreadsCtrl()
{
	this->tcNumThreads->Enable(this->IsParallelismEnabled());
}

void CRAbMesh::PMJMesh2Module::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	PMJMesh2::MainDrive *drive = dynamic_cast<PMJMesh2::MainDrive *>(this->drive);

	Parallel::TaskVector tasks = drive->getTasks();

	//sorting
	if (!tasks.empty())
	{
		for (UInt i = 0; i < tasks.size() - 1; i++)
		{
			UInt min = i;

			for (UInt j = i + 1; j < tasks.size(); j++)
			{
				if (tasks[j]->getId() < tasks[min]->getId())
				{
					min = j;
				}
			}

			if (min != i)
			{
				Parallel::Task *t = tasks[i];

				tasks[i] = tasks[min];

				tasks[min] = t;
			}
		}

		for (Parallel::TaskVector::iterator iter = tasks.begin();
			 iter != tasks.end(); iter++)
		{
			if ((*iter)->getPartition())
            {
                this->AddTreeItem(static_cast<PMJMesh::Partition *>((*iter)->getPartition()));
            }
		}
	}

	MJMeshModule::ForceFillTree(false);
}

void CRAbMesh::PMJMesh2Module::AddTreeItem(PMJMesh::Partition *p)
{
	if (this->IsEnabled(PARTITIONS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(p);
		this->tree->AppendItem(this->tiItems[PARTITIONS], item->GetText(), -1, -1, item);
	}
}

#endif //#if (USE_GUI)

