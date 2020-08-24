#ifndef _PMJMESH_PARTITION_TREE_GENERATOR_H_
#define _PMJMESH_PARTITION_TREE_GENERATOR_H_

/*#include "PMJMesh/Definitions.h"

#include "Performer/TreeGenerator.h"

namespace PMJMesh
{
	class PartitionTreeGenerator : public Performer::TreeGenerator
	{
	public:

		PartitionTreeGenerator(UInt dimension, const Data::BoxTree *original,
#if USE_C__11
			const Data::RealHashMap *load,
#else
			const Data::RealMap *load,
#endif //#if USE_C__11
			Real maxLoad, LoadCalculator *loadCalculator, Data::ULIntVector *originalNewCells = NULL);
		virtual ~PartitionTreeGenerator();

		virtual void setDimension(UInt dimension);

		virtual void setOriginal(const Data::BoxTree *original);

#if USE_C__11
		virtual void setLoad(const Data::RealHashMap *load);
#else
		virtual void setLoad(const Data::RealMap *load);
#endif //#if USE_C__11

		virtual void setMaxLoad(Real maxLoad);

		virtual bool shouldSubdivide(Real load) const;
		virtual bool shouldSubdivide(const Data::BoxTreeCell *original) const;

		virtual void subdivide(Data::BoxTreeCell *c, const Data::BoxTreeCell *original);

		virtual void execute();

	protected:

		UInt dimension;

		LoadCalculator *loadCalculator;

		const Data::BoxTree *original;

#if USE_C__11
		const Data::RealHashMap *load;
#else
		const Data::RealMap *load;
#endif //#if USE_C__11

		Real maxLoad;

		Data::ULIntVector *originalNewCells;
	};
}*/

#endif //#ifndef _PMJMESH_PARTITION_TREE_GENERATOR_H_
