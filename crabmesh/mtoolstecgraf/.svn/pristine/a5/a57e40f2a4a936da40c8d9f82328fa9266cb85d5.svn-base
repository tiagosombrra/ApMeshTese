#ifndef _MTOOLS_MSH_REGION3D_H_
#define _MTOOLS_MSH_REGION3D_H_

#include "MTools/Definitions.h"

#if USE_MTOOLS

#include "Performer/Builder.h"
#include "MshRegion3D.hpp"
#include "OctTree.hpp"

namespace MTools
{
	class MshRegion3D :
		public Msh3D::MshRegion3D,
		public Performer::Builder
	{
	public:

		MshRegion3D(const MshOrder iorder = LINEAR, const int istart_id = 0, const int imat_id = 0);
        virtual ~MshRegion3D();

        virtual void SetNoCracks(bool noCracks);
        virtual void SetUseUniformRefinement(bool useUniformRefinement);
        virtual void SetUseUniformSize(bool useUniformSize);

        virtual const Msh3D::AdvFront3D *GetAdvFront() const;

        virtual const FTools::OctTree *GetOctTree() const;

        virtual const FTools::OctTree *GenerateOctTree();

        virtual void execute();

	protected:

		virtual Msh3D::AdvFront3D *BuildAdvFront();
		virtual Msh3D::AdvFront3D *MakeAdvFront();

	protected:

		bool noCracks;
		bool useUniformRefinement;
		bool useUniformSize;

	protected:

		enum
		{
			SUCCESS = 0,
			NOT_INIT,
			INIT,

			ERROR = 100,
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status success;
		static const Performer::Status error;

#if DEBUG_TIMING_PMTOOLS
	public:

		static double timeBuildAdvFront;
#endif //#if DEBUG_TIMING_PMTOOLS
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _MTOOLS_MSH_REGION3D_H_
