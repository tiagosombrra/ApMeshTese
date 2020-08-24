#ifndef _PMTOOLS_MSH_REGION3D_H_
#define _PMTOOLS_MSH_REGION3D_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "MTools/MshRegion3D.h"

namespace PMTools
{
	class MshRegion3D : public MTools::MshRegion3D
	{
	public:

		MshRegion3D(const MshOrder iorder = LINEAR, const int istart_id = 0, const int imat_id = 0);
        virtual ~MshRegion3D();

        virtual void SetAdvFront(Msh3D::AdvFront3D *AdvFront);
        virtual void SetFtree(const FTools::OctTree *ftree);
        virtual void SetStartElemId(int StartElemId);
        virtual int GetStartElemId() const;

        virtual void SetWorkerTask(bool workerTask);
        virtual void SetUseBoxTest(bool useBoxTest);
        virtual void SetEnableBackTracking(bool enableBackTracking);
        virtual void SetBoundingBox(const FTools::Vec3D &min, const FTools::Vec3D &max);
        virtual void SetTreeBox(const FTools::Vec3D &min, const FTools::Vec3D &max);
        virtual void SetRangeProportion(double rangeProportion);

        using MTools::MshRegion3D::BuildAdvFront;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        virtual void execute();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	protected:

		virtual Msh3D::AdvFront3D *MakeAdvFront();

	protected:

		const FTools::OctTree *ftree;

		bool workerTask;
		bool useBoxTest;
		bool enableBackTracking;
		FTools::Vec3D min;
		FTools::Vec3D max;
		FTools::Vec3D treeMin;
		FTools::Vec3D treeMax;
		double rangeProportion;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeGeneration;
		double timeImprovement;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_MSH_REGION3D_H_
