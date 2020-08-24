#ifndef _MTOOLS_ADV_FRONT3D_H_
#define _MTOOLS_ADV_FRONT3D_H_

#include "MTools/Definitions.h"

#if USE_MTOOLS

#include "AdvFront3D.hpp"

namespace MTools
{
	class AdvFront3D :
		public Msh3D::AdvFront3D
	{
	public:

		class AdjacentFaceError : public std::runtime_error {
            public:
                AdjacentFaceError() : runtime_error("adjacent face missing in front") {}
        } ;

		AdvFront3D(NodeHash *NodeTable,FaceHash *FaceTable, bool noCracks = false);
        AdvFront3D();
        virtual ~AdvFront3D();

        virtual void SetUseUniformRefinement(bool useUniformRefinement);
        virtual void SetUseUniformSize(bool useUniformSize);

        virtual const FTools::OctTree *GetOct_Tree() const;

        virtual Msh3D::AdvFront3D::FFaceHeap *GetFaceHeap() const;

        using Msh3D::AdvFront3D::IntNodeHash;
        using Msh3D::AdvFront3D::IntNodeHashIter;
        virtual IntNodeHash *GetIntNodeTable() const;

        using Msh3D::AdvFront3D::IntFaceHash;
        virtual IntFaceHash *GetIntFaceTable() const;

        virtual Msh3D::TetTopo *GetTet_Topo() const;

        using Msh3D::AdvFront3D::MateHash;
        using Msh3D::AdvFront3D::MateHashIter;
        virtual MateHash *GetMates() const;

        using Msh3D::AdvFront3D::TetHash;
        using Msh3D::AdvFront3D::TetHashIter;
        virtual TetHash *GetTetTable() const;

        virtual void SetDoEdgeSwapping(bool flag);
        virtual void SetDoFaceSwapping(bool flag);

        virtual void GenerateOctTree();

	protected:

		virtual void BuildOctTree();

		virtual void UpdateFront(int nodes[4],int exists,
                         int level,int use = 0,
                         bool check_valid = true,
                         int region = -1) ;

		/*virtual bool FindCenterCoord(
            FFace **bface,
            Dict<int,int> &ttfaces,
            Dict<int,int> &ttnodes,
            Dict<int,FFace *> &ff_table,
            Vec3D& center,
            Dict<int,int> &crossings) const ;*/

		virtual bool DoBacktrack(FFace* entry) ;

		virtual bool DeleteElement(int region,FFace *face,FFace **bface,
                           int nodes[4],bool check_first=false) ;

	private:

		AdvFront3D(const MTools::AdvFront3D &other);

	protected:

		//added by markos
		bool useUniformRefinement;
		bool useUniformSize;
		//end added by markos

	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _MTOOLS_ADV_FRONT3D_H_
