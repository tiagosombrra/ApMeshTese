#include "CRAbMesh/PMJMeshModule.h"

#if (USE_GUI)

#include <wx/gbsizer.h>
#include <wx/tokenzr.h>
#include <wx/propdlg.h>
#include <wx/bookctrl.h>
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/TreeItemData.h"
#include "PMJMesh/MainDrive.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/Front.h"
#include "Parallel/Communicator.h"
#include "Parallel/Task.h"
#include "Parallel/Partition.h"
#include "MJMesh/Mesh.h"
#include "Data/State.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

CRAbMesh::PMJMeshModule::PMJMeshModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MJMeshModule(frame, canvas, false)
{
	this->aPrioriEnabled = false;
	this->parallelismEnabled = true;
	this->minRefinementPostponed = false;
	this->usePartitioningThreshold = false;
	this->partitioningThreshold = 1024.0;
	this->percentageNonWorkingTask = 0.0;

	this->cbLoadBalancer = NULL;
	this->loadBalancers[_("Weighted greedy")] = Parallel::AvailableLoadBalancers::WEIGHTED_GREEDY_SERIAL_STATIC;
	this->loadBalancers[_("Greedy")] = Parallel::AvailableLoadBalancers::GREEDY_SERIAL_STATIC;
	this->loadBalancers[_("Spring")] = Parallel::AvailableLoadBalancers::SPRING_SERIAL_STATIC;
	this->loadBalancers[_("Demand")] = Parallel::AvailableLoadBalancers::DEMAND_PARALLEL_DYNAMIC;
	this->SetLoadBalancer(Parallel::AvailableLoadBalancers::DEMAND_PARALLEL_DYNAMIC);

	this->rangeProportion = 1.0;

	this->smoothInSlavesEnabled = true;
#if (!USE_DELETE_FELS)
	this->slaveUnsmoothingLayer = 0;
#endif //#if (!USE_DELETE_FELS)
	this->masterSmoothingLayer = 1;

	this->cbPartitionerType = NULL;
	this->partitionerTypes[_("Load based")] = PMJMesh::PartitionerType::BOX_LOAD;
	this->partitionerTypes[_("Number of partitions based")] = PMJMesh::PartitionerType::BOX_NUMBER;
	this->partitionerTypes[_("BSP")] = PMJMesh::PartitionerType::BSP_NUMBER;
	this->partitionerTypes[_("Smart BSP")] = PMJMesh::PartitionerType::SMART_BSP_NUMBER;
	this->SetPartitionerType(PMJMesh::PartitionerType::BOX_LOAD);

	this->cbMeshUpdateType = NULL;
	this->meshUpdateTypes[_("1 - Full mesh and front")] = PMJMesh::MeshUpdateType::FULL;
	this->meshUpdateTypes[_("2 - Partial mesh and full front")] = PMJMesh::MeshUpdateType::PARTIAL;
	this->meshUpdateTypes[_("3 - Partial mesh and full front (no boundary)")] = PMJMesh::MeshUpdateType::PARTIAL_NO_BOUNDARY;
	this->meshUpdateTypes[_("4 - Only front")] = PMJMesh::MeshUpdateType::EMPTY;
	this->meshUpdateTypes[_("5 - Only front (no boundary)")] = PMJMesh::MeshUpdateType::EMPTY_NO_BOUNDARY;
	this->meshUpdateTypes[_("6 - Only final front")] = PMJMesh::MeshUpdateType::FINAL;
	this->SetMeshUpdateType(PMJMesh::MeshUpdateType::EMPTY_NO_BOUNDARY);

	this->cbShiftType = NULL;
	this->shiftTypes[_("0 - 0 side (no shift)")] = PMJMesh::ShiftType::NONE;
	this->shiftTypes[_("1 - 1 side only")] = PMJMesh::ShiftType::LATERAL;
	this->shiftTypes[_("2 - 2 sides only")] = PMJMesh::ShiftType::DIAGONAL;
	this->shiftTypes[_("3 - 1 and 2 sides")] = PMJMesh::ShiftType::LATERAL_DIAGONAL;
	this->shiftTypes[_("4 - 3 sides only (3D)")] = PMJMesh::ShiftType::DIAGONAL_VERTEX;
	this->shiftTypes[_("5 - 1 and 3 sides (3D)")] = PMJMesh::ShiftType::LATERAL_DIAGONAL_VERTEX;
	this->shiftTypes[_("6 - 2 and 3 sides (3D)")] = PMJMesh::ShiftType::DIAGONAL_DIAGONAL_VERTEX;
	this->shiftTypes[_("7 - 1, 2 and 3 sides (3D)")] = PMJMesh::ShiftType::LATERAL_DIAGONAL_DIAGONAL_VERTEX;
	this->SetShiftType(PMJMesh::ShiftType::LATERAL);

	PARTITIONS            = "Partitions";
	DRAW_PARTITIONS       = "Draw partitions";
	DRAW_PARTITION_STATES = "Draw partition states";

	this->InsertOption(PARTITIONS,            TREE_CELLS,       TYPE_TREE_ITEM);
	this->InsertOption(DRAW_PARTITIONS,       DRAW_MESH,        TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_PARTITION_STATES, DRAW_PARTITIONS,  TYPE_MENU_AND_CHECK_ITEM);

	this->SetConfiguration(DRAW_PARTITIONS,       true,  false);
	this->SetConfiguration(DRAW_PARTITION_STATES, false, false);

	this->SetOption(PARTITIONS,            true);
	this->SetOption(DRAW_PARTITIONS,       true, true);
	this->SetOption(DRAW_PARTITION_STATES, true, false);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();

		this->EnableLoadCtrl();
		this->EnableTreeRefinementCtrl();
		this->EnableRangeProportionCtrl();
		this->EnableMeshUpdateTypesCtrl();
		this->EnableShiftTypesCtrl();

		if (dynamic_cast<PMJMesh::MainDrive *>(this->drive)->getComm()->isSlave())
		{
			this->SetStepByStep(false);
			this->SetProgressShown(false);

			this->SetBoundaryDrawable(false);
			this->SetTreeDrawable(false);
			this->SetFrontDrawable(false);
			this->SetMeshDrawable(false);
			this->SetPartitionsDrawable(false);
		}
	}
}

CRAbMesh::PMJMeshModule::~PMJMeshModule()
{
	this->tcPartitioningThreshold->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);

	this->scPercentageNonWorkingTask->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);

	this->tcRangeProportion->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);
}

wxString CRAbMesh::PMJMeshModule::Name() const
{
	wxString name = _("PJMesh (Markos version)");

	PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

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

void CRAbMesh::PMJMeshModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

		if (this->IsAPrioriEnabled())
		{
			drive->setAlgorithm(PMJMesh::A_PRIORI_INTERFACE_MESH);
			drive->setMinRefinementPostponed(false);
			drive->setRangeProportion(1.0);
		}
		else
		{
			drive->setAlgorithm(PMJMesh::GLOBAL_SHIFT);
			drive->setMinRefinementPostponed(this->IsMinRefinementPostponed());
			drive->setRangeProportion(this->GetRangeProportion());
		}

		drive->setParallel(this->IsParallelismEnabled());
		//drive->setMinRefinementPostponed(this->IsMinRefinementPostponed());
		drive->setPartitionerType(this->GetPartitionerType());
		drive->setUsePartitioningThreshold(this->IsUsePartitioningThreshold());
		drive->setSmoothInSlavesEnabled(this->IsSmoothInSlavesEnabled());
#if (!USE_DELETE_FELS)
		drive->setUnsmoothingLayer(this->GetSlaveUnsmoothingLayer());
#endif //#if (!USE_DELETE_FELS)
		drive->setSmoothingLayer(this->GetMasterSmoothingLayer());
		drive->setPartitioningThreshold(this->GetPartitioningThreshold());
		drive->setPercentageNonWorkingTask(this->GetPercentageNonWorkingTask());
		drive->setLoadBalancerType(this->GetLoadBalancer());
		drive->setUpdateType(this->GetMeshUpdateType());
		drive->setShiftType(this->GetShiftType());

		//drive->setRemoveLayersInLoad(2);
		drive->setRemoveLayersInLoad(0);
		drive->setHeightDependentInLoad(false);
		drive->setSkippingInnerInDecomposition(false);
	}

	MJMeshModule::SetParameters(firstTime);
}

void CRAbMesh::PMJMeshModule::Execute()
{
	if (this->tcPartitioningThreshold)
	{
		double partitioningThreshold = 0.0;

		this->tcPartitioningThreshold->GetValue().ToDouble(&partitioningThreshold);

		this->SetPartitioningThreshold(partitioningThreshold);

		this->tcPartitioningThreshold->Enable(false);
	}

	if (this->scPercentageNonWorkingTask)
	{
		double percentageNonWorkingTask = static_cast<double>(this->scPercentageNonWorkingTask->GetValue());

		this->SetPercentageNonWorkingTask(percentageNonWorkingTask*0.01);

		this->scPercentageNonWorkingTask->Enable(false);
	}

	if (this->cbLoadBalancer)
	{
		this->SetLoadBalancer(this->cbLoadBalancer->GetValue());

		this->cbLoadBalancer->Enable(false);
	}

	if (this->tcRangeProportion)
	{
		double rangeProportion = 0.0;

		this->tcRangeProportion->GetValue().ToDouble(&rangeProportion);

		this->SetRangeProportion(rangeProportion);

		this->tcRangeProportion->Enable(false);
	}

	if (this->cbPartitionerType)
	{
		this->SetPartitionerType(this->cbPartitionerType->GetValue());

		this->cbPartitionerType->Enable(false);
	}

	if (this->cbMeshUpdateType)
	{
		this->SetMeshUpdateType(this->cbMeshUpdateType->GetValue());

		this->cbMeshUpdateType->Enable(false);
	}

	if (this->cbShiftType)
	{
		this->SetShiftType(this->cbShiftType->GetValue());

		this->cbShiftType->Enable(false);
	}

	if (this->miConfEnableAPriori) this->miConfEnableAPriori->Enable(false);
	if (this->miConfEnableParallelism) this->miConfEnableParallelism->Enable(false);
	if (this->miConfPostponeMinRefinement) this->miConfPostponeMinRefinement->Enable(false);
	if (this->miConfUsePartitioningThreshold) this->miConfUsePartitioningThreshold->Enable(false);
	if (this->miConfEnableSmoothInSlaves) this->miConfEnableSmoothInSlaves->Enable(false);

	if (this->cbEnableAPriori) this->cbEnableAPriori->Enable(false);
	if (this->cbEnableParallelism) this->cbEnableParallelism->Enable(false);
	if (this->cbPostponeMinRefinement) this->cbPostponeMinRefinement->Enable(false);
	if (this->cbUsePartitioningThreshold) this->cbUsePartitioningThreshold->Enable(false);
	if (this->cbEnableSmoothInSlaves) this->cbEnableSmoothInSlaves->Enable(false);

	//if (this->gWait) this->gWait->Enable();

	MJMeshModule::Execute();
}

/*void CRAbMesh::PMJMeshModule::Ready(bool clearing, bool pending)
{
	Interface::Frame::Ready(clearing, pending);

	PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

	if (//(this->alwaysExecute) &&
		(drive->getComm()->isSlave()) &&
		(this->drive->getStatus().getType() != this->drive->success.getType()) &&
		(this->drive->getStatus().getType() < this->drive->error.getType()))
	{
		this->Execute();
	}
}*/

void CRAbMesh::PMJMeshModule::Clear(bool onlyDrive, bool destroying)
{
	/*if ((!destrying) && (dynamic_cast<PMJMesh::MainDrive *>(this->drive)->getComm()->isSlave()))
	{
		return;
	}*/

	//dynamic_cast<PMJMesh::MainDrive *>(this->drive)->setDestroying(destroying);

	MJMeshModule::Clear(onlyDrive, destroying);

	if (onlyDrive)
	{
		return;
	}

	if (this->miConfEnableAPriori) this->miConfEnableAPriori->Enable();
	if (this->miConfEnableParallelism) this->miConfEnableParallelism->Enable();
	//if (this->miConfPostponeMinRefinement) this->miConfPostponeMinRefinement->Enable();
	if (this->miConfUsePartitioningThreshold) this->miConfUsePartitioningThreshold->Enable();
	if (this->miConfEnableSmoothInSlaves) this->miConfEnableSmoothInSlaves->Enable();

	if (this->scPercentageNonWorkingTask) this->scPercentageNonWorkingTask->Enable();

	if (this->cbEnableAPriori) this->cbEnableAPriori->Enable();
	if (this->cbEnableParallelism) this->cbEnableParallelism->Enable();
	if (this->cbPostponeMinRefinement) this->cbPostponeMinRefinement->Enable();
	if (this->cbUsePartitioningThreshold) this->cbUsePartitioningThreshold->Enable();
	if (this->cbEnableSmoothInSlaves) this->cbEnableSmoothInSlaves->Enable();

	if (this->cbLoadBalancer) this->cbLoadBalancer->Enable();
	if (this->cbPartitionerType) this->cbPartitionerType->Enable();
	if (this->cbMeshUpdateType) this->cbMeshUpdateType->Enable();
	if (this->cbShiftType) this->cbShiftType->Enable();

	this->EnableLoadCtrl();
	this->EnableTreeRefinementCtrl();
	this->EnableRangeProportionCtrl();
	this->EnableMeshUpdateTypesCtrl();
	this->EnableShiftTypesCtrl();
}

void CRAbMesh::PMJMeshModule::ClearDisplayLists(bool force) const
{
	CRAbMesh::MJMeshModule::ClearDisplayLists(force);

	PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

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

		Data::Front *f = t->getFront();

		if ((f) && ((force) || (f->isOutdated())))
		{
			f->clearLists();
		}
	}

	if (!drive->getComm()->isMaster())
	{
		const Data::Mesh *m = drive->getUnsmoothedMesh();

		if ((m) && ((force) || (m->isOutdated())))
		{
			m->clearLists();
		}
	}
}

void CRAbMesh::PMJMeshModule::Render() const
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

	PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

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
			Parallel::TaskVector notEmptyTasks;

			for (Parallel::TaskVector::const_iterator iter = tasks.begin();
				 iter != tasks.end(); iter++)
			{
				PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter)->getPartition());

				if (!p)
				{
					continue;
				}

				if (p->getLoad() > Data::getTolerance())
				{
					notEmptyTasks.push_back((*iter));

					continue;
				}

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

				//const Data::Box *b = p->getBox();

				/*static double val = 10.0;

				b->setColor(b->getR()/val, b->getG()/val, b->getB()/val);

				b->highlight();

				b->draw();

				b->setColor(b->getR()*val, b->getG()*val, b->getB()*val);

				b->unhighlight();*/

				//b->draw();

				static_cast<PMJMesh::MJMeshTask *>((*iter))->draw();

				//b->setBox(NULL, NULL);

				//debug
				//drawing the mesh
				if (this->IsMeshDrawable())
				{
					Data::Mesh *m = static_cast<PMJMesh::MJMeshTask *>((*iter))->getMesh();

					if (m)
					{
						m->draw();
					}
				}
				//endebug

				if (this->IsPartitionsGeneratedStateDrawable())
				{
					static_cast<PMJMesh::MJMeshTask *>((*iter))->drawGeneratedState();
				}

				Data::getState().resetMidExternal();
			}

			for (Parallel::TaskVector::const_iterator iter = notEmptyTasks.begin();
				 iter != notEmptyTasks.end(); iter++)
			{
				//drawing the box
				PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter)->getPartition());

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

				//drawing the mesh
				if (this->IsMeshDrawable())
				{
					Data::Mesh *m = static_cast<PMJMesh::MJMeshTask *>((*iter))->getMesh();

					if (m)
					{
						m->draw();
					}
				}

				//drawing the front
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

				if (this->IsPartitionsGeneratedStateDrawable())
				{
					static_cast<PMJMesh::MJMeshTask *>((*iter))->drawGeneratedState();
				}

				Data::getState().resetMidExternal();
			}
		}

		if ((!selected) && (!drive->getComm()->isMaster()) && (this->IsMeshDrawable()))
		{
			const Data::Mesh *m = drive->getUnsmoothedMesh();

			if (m)
			{
				m->draw();
			}
		}

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

				//drawing the front normals
				if (this->IsFrontDrawable())
				{
					PMJMesh::Front *f = p->getSubfront();

					if (f)
					{
						f->drawNormals(factor);
					}
				}

				Data::getState().resetMidExternal();
			}
		}
	}
}

void CRAbMesh::PMJMeshModule::SetAPrioriEnabled(bool aPrioriEnabled)
{
	this->aPrioriEnabled = aPrioriEnabled;

	this->miConfEnableAPriori->Check(this->aPrioriEnabled);
	this->cbEnableAPriori->SetValue(this->aPrioriEnabled);

	this->EnableTreeRefinementCtrl();
	this->EnableRangeProportionCtrl();
	this->EnableMeshUpdateTypesCtrl();
	this->EnableShiftTypesCtrl();
}

bool CRAbMesh::PMJMeshModule::IsAPrioriEnabled() const
{
	return this->aPrioriEnabled;
}

void CRAbMesh::PMJMeshModule::SetParallelismEnabled(bool parallelismEnabled)
{
	this->parallelismEnabled = parallelismEnabled;

	this->miConfEnableParallelism->Check(this->parallelismEnabled);
	this->cbEnableParallelism->SetValue(this->parallelismEnabled);
}

bool CRAbMesh::PMJMeshModule::IsParallelismEnabled() const
{
	return this->parallelismEnabled;
}

void CRAbMesh::PMJMeshModule::SetMinRefinementPostponed(bool minRefinementPostponed)
{
	this->minRefinementPostponed = minRefinementPostponed;

	this->miConfPostponeMinRefinement->Check(this->minRefinementPostponed);
	this->cbPostponeMinRefinement->SetValue(this->minRefinementPostponed);
}

bool CRAbMesh::PMJMeshModule::IsMinRefinementPostponed() const
{
	return this->minRefinementPostponed;
}

void CRAbMesh::PMJMeshModule::SetUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;

	this->miConfUsePartitioningThreshold->Check(this->usePartitioningThreshold);
	this->cbUsePartitioningThreshold->SetValue(this->usePartitioningThreshold);

	this->EnableLoadCtrl();
}

bool CRAbMesh::PMJMeshModule::IsUsePartitioningThreshold() const
{
	return this->usePartitioningThreshold;
}

void CRAbMesh::PMJMeshModule::SetPartitioningThreshold(double partitioningThreshold)
{
	this->partitioningThreshold = partitioningThreshold;

	wxString s;
    s << this->partitioningThreshold;
    this->tcPartitioningThreshold->SetValue(s);
}

double CRAbMesh::PMJMeshModule::GetPartitioningThreshold() const
{
	return this->partitioningThreshold;
}

void CRAbMesh::PMJMeshModule::SetPercentageNonWorkingTask(double percentageNonWorkingTask)
{
	this->percentageNonWorkingTask = percentageNonWorkingTask;

	wxString s;
    s << static_cast<int>(this->percentageNonWorkingTask*100.0);
    this->scPercentageNonWorkingTask->SetValue(s);
}

double CRAbMesh::PMJMeshModule::GetPercentageNonWorkingTask() const
{
	return this->percentageNonWorkingTask;
}

void CRAbMesh::PMJMeshModule::SetLoadBalancer(wxString loadBalancer)
{
	this->loadBalancer = loadBalancer;

	if (this->cbLoadBalancer)
	{
		this->cbLoadBalancer->SetValue(this->loadBalancer);
	}
}

void CRAbMesh::PMJMeshModule::SetLoadBalancer(enum Parallel::AvailableLoadBalancers loadBalancer)
{
	for (LoadBalancerMap::iterator iter = this->loadBalancers.begin();
		 iter != this->loadBalancers.end(); iter++)
	{
		if ((*iter).second == loadBalancer)
		{
			this->SetLoadBalancer((*iter).first);

			break;
		}
	}
}

enum Parallel::AvailableLoadBalancers CRAbMesh::PMJMeshModule::GetLoadBalancer() const
{
	LoadBalancerMap::const_iterator it = this->loadBalancers.find(this->loadBalancer);

	return (it == this->loadBalancers.end()) ? Parallel::UNDEFINED : (*it).second;
}

wxString CRAbMesh::PMJMeshModule::GetLoadBalancerString() const
{
	return this->loadBalancer;
}

wxArrayString CRAbMesh::PMJMeshModule::GetLoadBalancers() const
{
	wxArrayString s;

	for (LoadBalancerMap::const_iterator iter = this->loadBalancers.begin();
		 iter != this->loadBalancers.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMeshModule::SetRangeProportion(double rangeProportion)
{
	this->rangeProportion = rangeProportion;

	wxString s;
    s << this->rangeProportion;
    this->tcRangeProportion->SetValue(s);
}

double CRAbMesh::PMJMeshModule::GetRangeProportion() const
{
	return this->rangeProportion;
}

void CRAbMesh::PMJMeshModule::SetSmoothInSlavesEnabled(bool smoothInSlavesEnabled)
{
	this->smoothInSlavesEnabled = smoothInSlavesEnabled;

	this->miConfEnableSmoothInSlaves->Check(this->smoothInSlavesEnabled);
	this->cbEnableSmoothInSlaves->SetValue(this->smoothInSlavesEnabled);
}

bool CRAbMesh::PMJMeshModule::IsSmoothInSlavesEnabled() const
{
	return this->smoothInSlavesEnabled;
}

#if (!USE_DELETE_FELS)
void CRAbMesh::PMJMeshModule::SetSlaveUnsmoothingLayer(unsigned int slaveUnsmoothingLayer)
{
	this->slaveUnsmoothingLayer = slaveUnsmoothingLayer;
}

unsigned int CRAbMesh::PMJMeshModule::GetSlaveUnsmoothingLayer() const
{
	return this->slaveUnsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

void CRAbMesh::PMJMeshModule::SetMasterSmoothingLayer(unsigned int masterSmoothingLayer)
{
	this->masterSmoothingLayer = masterSmoothingLayer;
}

unsigned int CRAbMesh::PMJMeshModule::GetMasterSmoothingLayer() const
{
	return this->masterSmoothingLayer;
}

void CRAbMesh::PMJMeshModule::SetPartitionerType(wxString partitionerType)
{
	this->partitionerType = partitionerType;

	if (this->cbPartitionerType)
	{
		this->cbPartitionerType->SetStringSelection(this->partitionerType);
	}
}

void CRAbMesh::PMJMeshModule::SetPartitionerType(enum PMJMesh::PartitionerType partitionerType)
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

enum PMJMesh::PartitionerType CRAbMesh::PMJMeshModule::GetPartitionerType() const
{
	return (*this->partitionerTypes.find(this->partitionerType)).second;
}

wxString CRAbMesh::PMJMeshModule::GetPartitionerTypeString() const
{
	return this->partitionerType;
}

wxArrayString CRAbMesh::PMJMeshModule::GetPartitionerTypes() const
{
	wxArrayString s;

	for (PartitionerTypeMap::const_iterator iter = this->partitionerTypes.begin();
		 iter != this->partitionerTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMeshModule::SetMeshUpdateType(wxString meshUpdateType)
{
	this->meshUpdateType = meshUpdateType;

	if (this->cbMeshUpdateType)
	{
		this->cbMeshUpdateType->SetValue(this->meshUpdateType);
	}
}

void CRAbMesh::PMJMeshModule::SetMeshUpdateType(enum PMJMesh::MeshUpdateType meshUpdateType)
{
	for (UpdateTypeMap::iterator iter = this->meshUpdateTypes.begin();
		 iter != this->meshUpdateTypes.end(); iter++)
	{
		if ((*iter).second == meshUpdateType)
		{
			this->SetMeshUpdateType((*iter).first);

			break;
		}
	}
}

enum PMJMesh::MeshUpdateType CRAbMesh::PMJMeshModule::GetMeshUpdateType() const
{
	return (*this->meshUpdateTypes.find(this->meshUpdateType)).second;
}

wxString CRAbMesh::PMJMeshModule::GetMeshUpdateTypeString() const
{
	return this->meshUpdateType;
}

wxArrayString CRAbMesh::PMJMeshModule::GetMeshUpdateTypes() const
{
	wxArrayString s;

	for (UpdateTypeMap::const_iterator iter = this->meshUpdateTypes.begin();
		 iter != this->meshUpdateTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMeshModule::SetShiftType(wxString shiftType)
{
	this->shiftType = shiftType;

	if (this->cbShiftType)
	{
		this->cbShiftType->SetValue(this->shiftType);
	}
}

void CRAbMesh::PMJMeshModule::SetShiftType(enum PMJMesh::ShiftType shiftType)
{
	for (ShiftTypeMap::iterator iter = this->shiftTypes.begin();
		 iter != this->shiftTypes.end(); iter++)
	{
		if ((*iter).second == shiftType)
		{
			this->SetShiftType((*iter).first);

			break;
		}
	}
}

enum PMJMesh::ShiftType CRAbMesh::PMJMeshModule::GetShiftType() const
{
	return (*this->shiftTypes.find(this->shiftType)).second;
}

wxString CRAbMesh::PMJMeshModule::GetShiftTypeString() const
{
	return this->shiftType;
}

wxArrayString CRAbMesh::PMJMeshModule::GetShiftTypes() const
{
	wxArrayString s;

	for (ShiftTypeMap::const_iterator iter = this->shiftTypes.begin();
		 iter != this->shiftTypes.end(); iter++)
	{
		s.Add((*iter).first);
	}

	return s;
}

void CRAbMesh::PMJMeshModule::SetPartitionsDrawable(bool partitionsDrawable)
{
	this->SetConfiguration(DRAW_PARTITIONS, partitionsDrawable);
}

bool CRAbMesh::PMJMeshModule::IsPartitionsDrawable() const
{
	return this->IsConfiguration(DRAW_PARTITIONS);
}

void CRAbMesh::PMJMeshModule::SetPartitionsGeneratedStateDrawable(bool partitionsGeneratedStateDrawable)
{
	this->SetConfiguration(DRAW_PARTITION_STATES, partitionsGeneratedStateDrawable);
}

bool CRAbMesh::PMJMeshModule::IsPartitionsGeneratedStateDrawable() const
{
	return this->IsConfiguration(DRAW_PARTITION_STATES);
}

bool CRAbMesh::PMJMeshModule::IsDrawable() const
{
	return (MJMeshModule::IsDrawable() ||
			this->IsPartitionsDrawable());
}

void CRAbMesh::PMJMeshModule::MakeMainDrive()
{
	this->drive = new PMJMesh::MainDrive();
}

wxMenu *CRAbMesh::PMJMeshModule::MakeConfigurationMenu()
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
	this->miConfEnableAPriori            = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("A priori interface mesh"));
	pos++;
	this->miConfEnableParallelism        = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Shared memory parallelism"));
	pos++;
	this->miConfPostponeMinRefinement    = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Tree refinement in slaves"));
	pos++;
	this->miConfUsePartitioningThreshold = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Use partitioning threshold"));
	pos++;
	this->miConfEnableSmoothInSlaves     = mConfiguration->InsertCheckItem(pos, wxID_ANY, _("Enable smooth in slaves"));

	this->miConfEnableAPriori->Check(this->IsAPrioriEnabled());
	this->miConfEnableParallelism->Check(this->IsParallelismEnabled());
	this->miConfPostponeMinRefinement->Check(this->IsMinRefinementPostponed());
	this->miConfUsePartitioningThreshold->Check(this->IsUsePartitioningThreshold());
	this->miConfEnableSmoothInSlaves->Check(this->IsSmoothInSlavesEnabled());

	this->Connect(this->miConfEnableAPriori->GetId(),            wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfEnableAPriori));
	this->Connect(this->miConfEnableParallelism->GetId(),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfEnableParallelism));
	this->Connect(this->miConfPostponeMinRefinement->GetId(),    wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfPostponeMinRefinement));
	this->Connect(this->miConfUsePartitioningThreshold->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfUsePartitioningThreshold));
    this->Connect(this->miConfEnableSmoothInSlaves->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfEnableSmoothInSlaves));

    return mConfiguration;
}

wxPanel *CRAbMesh::PMJMeshModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MJMeshModule::MakeConfigurationPanel();

	this->cbEnableAPriori            = new wxCheckBox(pConfiguration, wxID_ANY, _("A priori interface mesh"));
	this->cbEnableParallelism        = new wxCheckBox(pConfiguration, wxID_ANY, _("Shared memory parallelism"));
	this->cbPostponeMinRefinement    = new wxCheckBox(pConfiguration, wxID_ANY, _("Tree refinement in slaves"));
	this->cbUsePartitioningThreshold = new wxCheckBox(pConfiguration, wxID_ANY, _("Use partitioning threshold"));

	wxStaticText *stPartitionerType = new wxStaticText(pConfiguration, wxID_ANY, _("Partitioning type:"));
    this->cbPartitionerType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetPartitionerTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetPartitionerTypes(), wxCB_READONLY);

	wxStaticText *stPartitioningThreshold = new wxStaticText(pConfiguration, wxID_ANY, _("Partitioning threshold:"));
    this->tcPartitioningThreshold         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("1024"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stPercentageNonWorkingTask = new wxStaticText(pConfiguration, wxID_ANY, _("Minimum load (%):"));
    this->scPercentageNonWorkingTask         = new wxSpinCtrl(  pConfiguration, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()));

    wxStaticText *stLoadBalancer = new wxStaticText(pConfiguration, wxID_ANY, _("Load balancer:"));
    this->cbLoadBalancer         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetLoadBalancerString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetLoadBalancers(), wxCB_READONLY);

	wxStaticText *stRangeProportion = new wxStaticText(pConfiguration, wxID_ANY, _("Range proportion:"));
    this->tcRangeProportion         = new wxTextCtrl(  pConfiguration, wxID_ANY, _("1.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	this->cbEnableSmoothInSlaves = new wxCheckBox(pConfiguration, wxID_ANY, _("Enable smooth in slaves"));

    wxStaticText *stMeshUpdateType = new wxStaticText(pConfiguration, wxID_ANY, _("Mesh update type:"));
    this->cbMeshUpdateType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetMeshUpdateTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetMeshUpdateTypes(), wxCB_READONLY);

    wxStaticText *stShiftType = new wxStaticText(pConfiguration, wxID_ANY, _("Shift type:"));
    this->cbShiftType         = new wxComboBox(  pConfiguration, wxID_ANY,
		this->GetShiftTypeString(), wxDefaultPosition,
		wxSize(this->tcTreeFactor->GetSize().GetWidth(), wxDefaultSize.GetHeight()),
		this->GetShiftTypes(), wxCB_READONLY);

	this->cbEnableAPriori->SetValue(this->IsAPrioriEnabled());
	this->cbEnableParallelism->SetValue(this->IsParallelismEnabled());
	this->cbPostponeMinRefinement->SetValue(this->IsMinRefinementPostponed());
	this->cbEnableSmoothInSlaves->SetValue(this->IsSmoothInSlavesEnabled());
	this->cbUsePartitioningThreshold->SetValue(this->IsUsePartitioningThreshold());

	wxString s;

    s = _("");
    s << this->GetPartitioningThreshold();
    tcPartitioningThreshold->SetValue(s);

    s = _("");
    s << this->GetPercentageNonWorkingTask()*100.0;
    scPercentageNonWorkingTask->SetValue(s);

    s = _("");
    s << this->GetRangeProportion();
    tcRangeProportion->SetValue(s);

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
    sParallel->Add(this->cbEnableAPriori,            wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(this->cbEnableParallelism,        wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(this->cbPostponeMinRefinement,    wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stPartitionerType,                wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbPartitionerType,          wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	sParallel->Add(this->cbUsePartitioningThreshold, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stPartitioningThreshold,          wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcPartitioningThreshold,    wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stPercentageNonWorkingTask,       wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->scPercentageNonWorkingTask, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stLoadBalancer,                   wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbLoadBalancer,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(stRangeProportion,                wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->tcRangeProportion,          wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
    sParallel->Add(this->cbEnableSmoothInSlaves,     wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sParallel->Add(stMeshUpdateType,                 wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbMeshUpdateType,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	sParallel->Add(stShiftType,                      wxGBPosition(row,   0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sParallel->Add(this->cbShiftType,                wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 0);
	wxStaticBoxSizer *sbsParallelConf = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Parallel options"));
	sbsParallelConf->Add(sParallel, 0, wxEXPAND | wxALL, 1);

	sConf->Add(sbsSerialConf,   0, wxEXPAND | wxALL, 1);
	sConf->Add(sbsParallelConf, 0, wxEXPAND | wxALL, 1);
	sConf->Add(sDrawing,        0, wxEXPAND | wxALL, 1);
    sConf->Add(sExecute,        0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);

    sConf->Layout();

    this->tcPartitioningThreshold->Connect(   wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);
    this->scPercentageNonWorkingTask->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);
    this->tcRangeProportion->Connect(         wxEVT_KILL_FOCUS, wxFocusEventHandler(PMJMeshModule::OnKillFocus), NULL, this);

    this->Connect(this->cbEnableAPriori->GetId(),            wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMeshModule::OnConfEnableAPriori));
    this->Connect(this->cbEnableParallelism->GetId(),        wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMeshModule::OnConfEnableParallelism));
    this->Connect(this->cbPostponeMinRefinement->GetId(),    wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMeshModule::OnConfPostponeMinRefinement));
    this->Connect(this->cbUsePartitioningThreshold->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMeshModule::OnConfUsePartitioningThreshold));
    this->Connect(this->cbEnableSmoothInSlaves->GetId(),     wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PMJMeshModule::OnConfEnableSmoothInSlaves));

    this->Connect(this->cbLoadBalancer->GetId(),   wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfLoadBalancer));
    this->Connect(this->cbMeshUpdateType->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfMeshUpdateType));
    this->Connect(this->cbShiftType->GetId(),      wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(PMJMeshModule::OnConfShiftType));

    return pConfiguration;
}

wxDialog *CRAbMesh::PMJMeshModule::MakeConfigurationDialog()
{
	wxDialog *dConf = MJMeshModule::MakeConfigurationDialog();

	wxPropertySheetDialog *psdConf = static_cast<wxPropertySheetDialog *>(dConf);

	wxBookCtrlBase *nConf = psdConf->GetBookCtrl();

	wxPanel *pConf = new wxPanel(nConf);

	wxStaticText *stPartitioningThreshold = new wxStaticText(pConf, wxID_ANY, _("Partitioning threshold:"));
    wxTextCtrl *tcPartitioningThreshold   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    wxStaticText *stPercentageNonWorkingTask = new wxStaticText(pConf, wxID_ANY, _("Minimum load (%):"));
    wxSpinCtrl *scPercentageNonWorkingTask   = new wxSpinCtrl(  pConf, wxID_ANY);

    wxStaticText *stLoadBalancer = new wxStaticText(pConf, wxID_ANY, _("Load balancer:"));
    wxComboBox *cbLoadBalancer   = new wxComboBox(  pConf, wxID_ANY, this->GetLoadBalancerString(), wxDefaultPosition, wxDefaultSize, this->GetLoadBalancers(), wxCB_READONLY);

    wxStaticText *stRangeProportion = new wxStaticText(pConf, wxID_ANY, _("Range proportion:"));
    wxTextCtrl *tcRangeProportion   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

#if (!USE_DELETE_FELS)
	wxStaticText *stSlaveUnsmoothingLayer = new wxStaticText(pConf, wxID_ANY, _("Layers of unimproved elements in slave:"));
    wxTextCtrl *tcSlaveUnsmoothingLayer   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
#endif //#if (!USE_DELETE_FELS)

    wxStaticText *stMasterSmoothingLayer = new wxStaticText(pConf, wxID_ANY, _("Layers of improved elements in master:"));
    wxTextCtrl *tcMasterSmoothingLayer   = new wxTextCtrl(  pConf, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	wxStaticText *stMeshUpdateType = new wxStaticText(pConf, wxID_ANY, _("Mesh update type:"));
    wxComboBox *cbMeshUpdateType   = new wxComboBox(  pConf, wxID_ANY, this->GetMeshUpdateTypeString(), wxDefaultPosition, wxDefaultSize, this->GetMeshUpdateTypes(), wxCB_READONLY);

    wxStaticText *stShiftType = new wxStaticText(pConf, wxID_ANY, _("Shift type:"));
    wxComboBox *cbShiftType   = new wxComboBox(  pConf, wxID_ANY, this->GetShiftTypeString(), wxDefaultPosition, wxDefaultSize, this->GetShiftTypes(), wxCB_READONLY);

    wxString s;
    s = _("");
    s << this->GetPartitioningThreshold();
    tcPartitioningThreshold->SetValue(s);

    tcPartitioningThreshold->Enable(this->IsUsePartitioningThreshold());

    s = _("");
    s << this->GetPercentageNonWorkingTask()*100.0;
    scPercentageNonWorkingTask->SetValue(s);

#if (!USE_DELETE_FELS)
    s = _("");
    s << this->GetSlaveUnsmoothingLayer();
    tcSlaveUnsmoothingLayer->SetValue(s);
#endif //#if (!USE_DELETE_FELS)

    s = _("");
    s << this->GetRangeProportion();
    tcRangeProportion->SetValue(s);

    tcRangeProportion->Enable(!this->IsAPrioriEnabled());

    s = _("");
    s << this->GetMasterSmoothingLayer();
    tcMasterSmoothingLayer->SetValue(s);

    cbShiftType->Enable(!this->IsAPrioriEnabled());

    int row = 0;
    wxGridBagSizer *gbsConf = new wxGridBagSizer();
    gbsConf->SetFlexibleDirection(wxBOTH);
    gbsConf->Add(stPartitioningThreshold,    wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcPartitioningThreshold,    wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stPercentageNonWorkingTask, wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(scPercentageNonWorkingTask, wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stLoadBalancer,             wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbLoadBalancer,             wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stRangeProportion,          wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcRangeProportion,          wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
#if (!USE_DELETE_FELS)
    gbsConf->Add(stSlaveUnsmoothingLayer,    wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcSlaveUnsmoothingLayer,    wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
#endif //#if (!USE_DELETE_FELS)
    gbsConf->Add(stMasterSmoothingLayer,     wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(tcMasterSmoothingLayer,     wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
    gbsConf->Add(stMeshUpdateType,           wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbMeshUpdateType,           wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
	gbsConf->Add(stShiftType,                wxGBPosition(row, 0),   wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 0);
    gbsConf->Add(cbShiftType,                wxGBPosition(row++, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 0);
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

void CRAbMesh::PMJMeshModule::OnConfigurationDialogOk(const wxDialog *dConf)
{
	MJMeshModule::OnConfigurationDialogOk(dConf);

	const wxPropertySheetDialog *psdConf = static_cast<const wxPropertySheetDialog *>(dConf);

	double partitioningThreshold, percentageNonWorkingTask, rangeProportion;
	wxString loadBalancer, meshUpdateType, shiftType;
	unsigned long int
#if (!USE_DELETE_FELS)
	slaveUnsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
	masterSmoothingLayer;

	wxWindow *pConf = psdConf->GetBookCtrl()->GetPage(1);

	static_cast<wxTextCtrl *>(pConf->GetChildren()[1])->GetValue().ToDouble(&partitioningThreshold);
	percentageNonWorkingTask = static_cast<wxSpinCtrl *>(pConf->GetChildren()[3])->GetValue();
	loadBalancer = static_cast<wxComboBox *>(pConf->GetChildren()[5])->GetValue();
	static_cast<wxTextCtrl *>(pConf->GetChildren()[7])->GetValue().ToDouble(&rangeProportion);
#if USE_DELETE_FELS
	static_cast<wxTextCtrl *>(pConf->GetChildren()[9])->GetValue().ToULong(&masterSmoothingLayer);
	meshUpdateType = static_cast<wxComboBox *>(pConf->GetChildren()[11])->GetValue();
	shiftType = static_cast<wxComboBox *>(pConf->GetChildren()[13])->GetValue();
#else
	static_cast<wxTextCtrl *>(pConf->GetChildren()[9])->GetValue().ToULong(&slaveUnsmoothingLayer);
	static_cast<wxTextCtrl *>(pConf->GetChildren()[11])->GetValue().ToULong(&masterSmoothingLayer);
	meshUpdateType = static_cast<wxComboBox *>(pConf->GetChildren()[13])->GetValue();
	shiftType = static_cast<wxComboBox *>(pConf->GetChildren()[15])->GetValue();
#endif //#if USE_DELETE_FELS

	this->SetPartitioningThreshold(partitioningThreshold);
	this->SetPercentageNonWorkingTask(percentageNonWorkingTask*0.01);
	this->SetLoadBalancer(loadBalancer);
	this->SetRangeProportion(rangeProportion);
#if (!USE_DELETE_FELS)
	this->SetSlaveUnsmoothingLayer(static_cast<unsigned int>(slaveUnsmoothingLayer));
#endif //#if (!USE_DELETE_FELS)
	this->SetMasterSmoothingLayer(static_cast<unsigned int>(masterSmoothingLayer));
	this->SetMeshUpdateType(meshUpdateType);
	this->SetShiftType(shiftType);
}

void CRAbMesh::PMJMeshModule::OnConfEnableAPriori(wxCommandEvent &WXUNUSED(event))
{
	this->SetAPrioriEnabled(!this->IsAPrioriEnabled());
}

void CRAbMesh::PMJMeshModule::OnConfEnableParallelism(wxCommandEvent &WXUNUSED(event))
{
	this->SetParallelismEnabled(!this->IsParallelismEnabled());
}

void CRAbMesh::PMJMeshModule::OnConfPostponeMinRefinement(wxCommandEvent &WXUNUSED(event))
{
	this->SetMinRefinementPostponed(!this->IsMinRefinementPostponed());
}

void CRAbMesh::PMJMeshModule::OnConfUsePartitioningThreshold(wxCommandEvent &WXUNUSED(event))
{
	this->SetUsePartitioningThreshold(!this->IsUsePartitioningThreshold());
}

void CRAbMesh::PMJMeshModule::OnConfEnableSmoothInSlaves(wxCommandEvent &WXUNUSED(event))
{
	this->SetSmoothInSlavesEnabled(!this->IsSmoothInSlavesEnabled());
}

void CRAbMesh::PMJMeshModule::OnConfLoadBalancer(wxCommandEvent &event)
{
	if (event.GetId() == this->cbLoadBalancer->GetId())
	{
		this->SetLoadBalancer(this->cbLoadBalancer->GetValue());
	}
}

void CRAbMesh::PMJMeshModule::OnConfMeshUpdateType(wxCommandEvent &event)
{
	if (event.GetId() == this->cbMeshUpdateType->GetId())
	{
		this->SetMeshUpdateType(this->cbMeshUpdateType->GetValue());
	}
}

void CRAbMesh::PMJMeshModule::OnConfShiftType(wxCommandEvent &event)
{
	if (event.GetId() == this->cbShiftType->GetId())
	{
		this->SetShiftType(this->cbShiftType->GetValue());
	}
}

void CRAbMesh::PMJMeshModule::OnKillFocus(wxFocusEvent &event)
{
	if (event.GetId() == this->tcPartitioningThreshold->GetId())
	{
		double partitioningThreshold;

		this->tcPartitioningThreshold->GetValue().ToDouble(&partitioningThreshold);

		this->SetPartitioningThreshold(partitioningThreshold);

		event.Skip(true);
	}
	else if (event.GetId() == this->scPercentageNonWorkingTask->GetId())
	{
		double percentageNonWorkingTask = static_cast<double>(this->scPercentageNonWorkingTask->GetValue());

		this->SetPercentageNonWorkingTask(percentageNonWorkingTask*0.01);

		event.Skip(true);
	}
	if (event.GetId() == this->tcRangeProportion->GetId())
	{
		double rangeProportion;

		this->tcRangeProportion->GetValue().ToDouble(&rangeProportion);

		this->SetRangeProportion(rangeProportion);

		event.Skip(true);
	}
	else
	{
		MJMeshModule::OnKillFocus(event);
	}
}

void CRAbMesh::PMJMeshModule::OnTreeMiddleClick(wxTreeEvent &event)
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

void CRAbMesh::PMJMeshModule::EnableLoadCtrl()
{
	if ((this->cbUsePartitioningThreshold) &&
		(this->tcPartitioningThreshold) &&
		(this->cbUsePartitioningThreshold->IsEnabled()))
	{
		this->tcPartitioningThreshold->Enable(this->IsUsePartitioningThreshold());
	}
}

void CRAbMesh::PMJMeshModule::EnableTreeRefinementCtrl()
{
	if ((this->miConfPostponeMinRefinement) &&
		(this->cbPostponeMinRefinement))
	{
		if (this->IsAPrioriEnabled())
		{
			this->miConfPostponeMinRefinement->Enable(false);
			this->cbPostponeMinRefinement->Enable(false);
		}
		else
		{
			this->miConfPostponeMinRefinement->Enable(true);
			this->cbPostponeMinRefinement->Enable(true);
		}
	}
}

void CRAbMesh::PMJMeshModule::EnableRangeProportionCtrl()
{
	if (this->tcRangeProportion)
	{
		this->tcRangeProportion->Enable(!this->IsAPrioriEnabled());
	}
}

void CRAbMesh::PMJMeshModule::EnableMeshUpdateTypesCtrl()
{
	if (this->cbMeshUpdateType)
	{
		if (this->IsAPrioriEnabled())
		{
			if (this->GetMeshUpdateType() != PMJMesh::FULL)
			{
				this->SetMeshUpdateType(PMJMesh::EMPTY);
			}

			this->meshUpdateTypes.clear();

			this->meshUpdateTypes[_("1 - Full mesh and front")] = PMJMesh::MeshUpdateType::FULL;
			//this->meshUpdateTypes[_("2 - Partial mesh and full front")] = PMJMesh::MeshUpdateType::PARTIAL;
			//this->meshUpdateTypes[_("3 - Partial mesh and full front (no boundary)")] = PMJMesh::MeshUpdateType::PARTIAL_NO_BOUNDARY;
			this->meshUpdateTypes[_("4 - Only front")] = PMJMesh::MeshUpdateType::EMPTY;
			//this->meshUpdateTypes[_("5 - Only front (no boundary)")] = PMJMesh::MeshUpdateType::EMPTY_NO_BOUNDARY;
			//this->meshUpdateTypes[_("6 - Only final front")] = PMJMesh::MeshUpdateType::FINAL;
		}
		else
		{
			this->meshUpdateTypes.clear();

			this->meshUpdateTypes[_("1 - Full mesh and front")] = PMJMesh::MeshUpdateType::FULL;
			this->meshUpdateTypes[_("2 - Partial mesh and full front")] = PMJMesh::MeshUpdateType::PARTIAL;
			this->meshUpdateTypes[_("3 - Partial mesh and full front (no boundary)")] = PMJMesh::MeshUpdateType::PARTIAL_NO_BOUNDARY;
			this->meshUpdateTypes[_("4 - Only front")] = PMJMesh::MeshUpdateType::EMPTY;
			this->meshUpdateTypes[_("5 - Only front (no boundary)")] = PMJMesh::MeshUpdateType::EMPTY_NO_BOUNDARY;
			this->meshUpdateTypes[_("6 - Only final front")] = PMJMesh::MeshUpdateType::FINAL;
		}

		this->cbMeshUpdateType->Clear();

		this->cbMeshUpdateType->Append(this->GetMeshUpdateTypes());

		if ((this->IsAPrioriEnabled()) &&
			(this->GetMeshUpdateType() != PMJMesh::FULL))
		{
			this->SetMeshUpdateType(PMJMesh::EMPTY);
		}
	}
}

void CRAbMesh::PMJMeshModule::EnableShiftTypesCtrl()
{
	if (this->cbShiftType)
	{
		if (this->IsAPrioriEnabled())
		{
			if ((this->IsAPrioriEnabled()) && (this->GetShiftType() != PMJMesh::LATERAL))
			{
				this->SetShiftType(PMJMesh::LATERAL);
			}

			this->cbShiftType->Enable(false);
		}
		else
		{
			this->cbShiftType->Enable(true);
		}
	}
}

void CRAbMesh::PMJMeshModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	PMJMesh::MainDrive *drive = dynamic_cast<PMJMesh::MainDrive *>(this->drive);

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
			this->AddTreeItem(static_cast<PMJMesh::Partition *>((*iter)->getPartition()));
		}
	}

	MJMeshModule::ForceFillTree(false);
}

void CRAbMesh::PMJMeshModule::AddTreeItem(PMJMesh::Partition *p)
{
	if (this->IsEnabled(PARTITIONS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(p);
		this->tree->AppendItem(this->tiItems[PARTITIONS], item->GetText(), -1, -1, item);
	}
}

#endif //#if (USE_GUI)
