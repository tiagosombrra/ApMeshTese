#ifndef _PMJMESH_POSTERIORI_LOAD_CALCULATOR_H_
#define _PMJMESH_POSTERIORI_LOAD_CALCULATOR_H_

#include "PMJMesh/Definitions.h"

#include "PMJMesh/LoadCalculator.h"

namespace PMJMesh
{
	class PosterioriLoadCalculator : public PMJMesh::LoadCalculator
	{
	public:

		PosterioriLoadCalculator(const Data::BoxTree *tree,
			bool considerOn = true, bool heightDependent = false,
			UInt dimension = 0, UInt minLevel = 0, UInt numLayers = 1,
			bool adjustInTree = false, bool adjustInBox = false);
		virtual ~PosterioriLoadCalculator();

		virtual void setNumLayers(UInt numLayers);

		virtual void setAdjustInTree(bool adjustInTree);
		virtual void setAdjustInBox(bool adjustInBox);

	protected:

#if USE_C__11
		typedef std::unordered_map<ULInt, Data::UIntVector> ULIntUIntVectorHashMap;
#endif //#if USE_C__11

		typedef std::map<ULInt, Data::UIntVector> ULIntUIntVectorMap;

	protected:

		virtual bool shouldRemoveFromLoad(const Data::BoxTreeCell *cell, const Data::BoxTreeCell *from = NULL) const;

		virtual UInt layer(const Data::BoxTreeCell *cell, UInt layer, UInt side, ULIntUIntVectorMap &mapLayers) const;

		virtual void findNeighboringCells(Data::GraphNodeList cells, UInt side,
			Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers, const Data::BoxTreeCell *cell = NULL);

		virtual Real sumLoads(Data::GraphNodeSet descends[], const ULIntUIntVectorMap &mapLayers) const;

		virtual void findDescendants(const Data::BoxTreeCell *cell, UInt side,
			Data::GraphNodeList &cells, Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers) const;

		virtual void removeBoundingLoad(const Data::BoxTreeCell *cell);

		virtual void adjustTreeLoad();

		virtual void makeTreeLoad();

		//virtual bool inBox(const Data::BoxTreeCell *cell);

		virtual void findOnCells(const Data::BoxTreeCell *cell, Data::GraphNodeList ds[],
			Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers, bool interTest = true);

		virtual void adjustBoxLoad();

		virtual void makeBoxLoad();

	protected:

		UInt numLayers;

		bool adjustInTree;
		bool adjustInBox;
	};
}

#endif //ifndef _PMJMESH_POSTERIORI_LOAD_CALCULATOR_H_
