#ifndef _PMJMESH_LOAD_CALCULATOR_H_
#define _PMJMESH_LOAD_CALCULATOR_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Builder.h"

namespace PMJMesh
{
	class LoadCalculator : public Parallel::Builder
	{
	public:

		LoadCalculator(const Data::BoxTree *tree,
			bool considerOn = true, bool heightDependent = false,
			UInt dimension = 0, UInt minLevel = 0);
		virtual ~LoadCalculator();

		virtual void setTree(const Data::BoxTree *tree);

		virtual void setMinLevel(UInt minLevel);
		virtual void setDimension(UInt dimension);

		virtual void setConsiderOn(bool considerOn);
		virtual bool isConsideringOn() const;

		virtual void setHeightDependent(bool heightDependent);

		virtual void setBox(const Data::Box *box);

		virtual Real getLoad() const;

#if USE_C__11
		virtual const Data::RealHashMap &getTreeLoad() const;
#else
		virtual const Data::RealMap &getTreeLoad() const;
#endif //#if USE_C__11

		//virtual bool isConsideringOn() const;
		//virtual bool isAddingLoadForNodes() const;

		virtual Real loadFor(enum Data::Position pos) const;

		virtual void execute();

	protected:

		///this method assumes input cell is a leaf
		virtual Real loadFor(const Data::BoxTreeCell *cell, Real basicLoad) const;

		//virtual Real weight(Real load) const;

		virtual Real setLoad(const Data::BoxTreeCell *cell, Real load);

		virtual Real calculateTreeLoad(const Data::BoxTreeCell *cell);

		virtual void makeTreeLoad();

		virtual bool inBox(const Data::BoxTreeCell *cell);

		virtual Real calculateBoxLoad(const Data::BoxTreeCell *cell);

		virtual void makeBoxLoad();

	protected:

		bool considerOn;

		const Data::Box *box;

		Real load;

		UInt minLevel;
		UInt dimension;

		const Data::BoxTree *tree;

#if USE_C__11
		Data::RealHashMap treeLoad;
#else
		Data::RealMap treeLoad;
#endif //#if USE_C__11

		//bool propagateLoad;
		///Does not work for PosteriorLoadCalculator
		//bool addingLoadForNodes;

		bool heightDependent;
		UInt heightDependentMinLevel;
		UInt heightDependentMaxLevel;
		Real heightDependentFactor;
	};
}

#endif //#ifndef _PMJMESH_LOAD_CALCULATOR_H_
