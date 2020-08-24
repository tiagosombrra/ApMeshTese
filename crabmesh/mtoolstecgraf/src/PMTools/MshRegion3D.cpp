#include "PMTools/MshRegion3D.h"

#if USE_MTOOLS

#include "PMTools/AdvFront3D.h"

PMTools::MshRegion3D::MshRegion3D(const MshOrder iorder, const int istart_id, const int imat_id) :
	MTools::MshRegion3D::MshRegion3D(iorder, istart_id, imat_id)
{
	this->ftree = NULL;

	this->useBoxTest = false;
	this->workerTask = false;
	this->enableBackTracking = true;
	this->rangeProportion = 1.0;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeGeneration = this->timeImprovement = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PMTools::MshRegion3D::~MshRegion3D()
{

}

void PMTools::MshRegion3D::SetAdvFront(Msh3D::AdvFront3D *AdvFront)
{
	this->AdvFront = AdvFront;
}

void PMTools::MshRegion3D::SetFtree(const FTools::OctTree *ftree)
{
	this->ftree = ftree;
}

void PMTools::MshRegion3D::SetStartElemId(int StartElemId)
{
	this->StartElemId = StartElemId;
}

int PMTools::MshRegion3D::GetStartElemId() const
{
	return this->StartElemId;
}

void PMTools::MshRegion3D::SetUseBoxTest(bool useBoxTest)
{
    this->useBoxTest = useBoxTest;
}

void PMTools::MshRegion3D::SetWorkerTask(bool workerTask)
{
	this->workerTask = workerTask;
}

void PMTools::MshRegion3D::SetEnableBackTracking(bool enableBackTracking)
{
	this->enableBackTracking = enableBackTracking;
}

void PMTools::MshRegion3D::SetBoundingBox(const FTools::Vec3D &min, const FTools::Vec3D &max)
{
	this->min = min;
	this->max = max;
}

void PMTools::MshRegion3D::SetTreeBox(const FTools::Vec3D &min, const FTools::Vec3D &max)
{
	this->treeMin = min;
	this->treeMax = max;
}

void PMTools::MshRegion3D::SetRangeProportion(double rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
void PMTools::MshRegion3D::execute()
{
    this->timeGeneration = this->timeImprovement = 0.0;

    MTools::MshRegion3D::execute();

    if (this->AdvFront)
    {
        this->timeGeneration = static_cast<PMTools::AdvFront3D *>(this->AdvFront)->timeGeneration;
        this->timeImprovement = static_cast<PMTools::AdvFront3D *>(this->AdvFront)->timeImprovement;
    }
}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

Msh3D::AdvFront3D *PMTools::MshRegion3D::MakeAdvFront()
{
	PMTools::AdvFront3D *advFront = new PMTools::AdvFront3D(NodeTable,FaceTable,this->noCracks);

	advFront->SetOct_Tree(this->ftree);

	advFront->SetUseBoxTest(this->useBoxTest);
	advFront->SetWorkerTask(this->workerTask);
	advFront->SetEnableBackTracking(this->enableBackTracking);
	advFront->SetBoundingBox(this->min, this->max);
	advFront->SetTreeBox(this->treeMin, this->treeMax);
	advFront->SetRangeProportion(this->rangeProportion);

	return advFront;
}

#endif //#if USE_MTOOLS
