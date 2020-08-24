#ifndef _PMTOOLS_ADV_FRONT3D_H_
#define _PMTOOLS_ADV_FRONT3D_H_

#include "PMTools/Definitions.h"

#if USE_MTOOLS

#include "MTools/AdvFront3D.h"

namespace PMTools
{
	class AdvFront3D :
		public MTools::AdvFront3D
	{
	public:

        class BackTrackingFaceError : public std::runtime_error {
            public:
                BackTrackingFaceError() : runtime_error("face missing in back-tracking front") {}
        } ;

		AdvFront3D(NodeHash *NodeTable,FaceHash *FaceTable, bool noCracks = false);
        AdvFront3D();
        virtual ~AdvFront3D();

        virtual void SetOct_Tree(const FTools::OctTree *Oct_Tree);

        virtual void SetWorkerTask(bool workerTask);
        virtual void SetUseBoxTest(bool useBoxTest);
        virtual void SetEnableBackTracking(bool enableBackTracking);
        virtual void SetBoundingBox(const FTools::Vec3D &min, const FTools::Vec3D &max);
        virtual void SetTreeBox(const FTools::Vec3D &min, const FTools::Vec3D &max);
        virtual void SetRangeProportion(double rangeProportion);

        virtual void SetMaxBoundaryNodeId(int maxBoundaryNodeId);

        virtual void SetTet_Topo(Msh3D::TetTopo *Tet_Topo);

        virtual void SetTetTable(TetHash *TetTable);

        virtual void SetNextElemId(int StartId);

	protected:

		virtual bool PMshOutBoxPt(const FTools::Vec3D &pt, double tolerance);
		virtual bool PMshOutBox(const FTools::Vec3D &imin, const FTools::Vec3D &imax, double tolerance);

		virtual void GenerateMesh(int start_id, int mat_id, ElemHash *elem_table,
			bool pyramids_only = false, bool restart = false);
		virtual bool BoundaryContraction(bool pyramids_only);
		virtual bool ConstructStarShapedPoly(FFace **bface, Dict<int,int> &ttfaces,
			Dict<int,int> &ttnodes, Dict<int,FFace *> &ff_table, PointTree *point_tree,
			AdvFront3D::Node** trial);
		virtual bool DoBacktrack(FFace* entry);
		virtual void UpdateFront(int nodes[4],int exists,
                         int level,int use = 0,
                         bool check_valid = true,
                         int region = -1) ;

	private:

		AdvFront3D(const PMTools::AdvFront3D &other);

	protected:

		bool pmtoolsWorkerTask;
		bool pmtoolsUseBoxTest;
		bool pmtoolsEnableBackTracking;
		FTools::Vec3D pmtoolsBoxMin;
		FTools::Vec3D pmtoolsBoxMax;
		FTools::Vec3D pmtoolsTreeBoxMin;
		FTools::Vec3D pmtoolsTreeBoxMax;
		double pmtoolsRangeProportion;
		int pmtoolsMaxBoundaryNodeId;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeGeneration;
		double timeImprovement;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _PMTOOLS_ADV_FRONT3D_H_
