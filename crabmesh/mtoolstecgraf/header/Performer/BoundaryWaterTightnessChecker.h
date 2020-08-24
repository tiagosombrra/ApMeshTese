#ifndef _PERFORMER_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_
#define _PERFORMER_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryWaterTightnessChecker : public Performer::Builder
	{
	public:

#if USE_C__11
		typedef std::unordered_map<Data::Vertex *, Data::FrontElementHash> VertexFEListMap;
		typedef std::unordered_map<Data::Vertex *, VertexFEListMap> AdjacencyMap;
#else
		typedef std::map<Data::Vertex *, Data::FrontElementSet> VertexFEListMap;
		typedef std::map<Data::Vertex *, VertexFEListMap> AdjacencyMap;
#endif //#if USE_C__11

	public:

		BoundaryWaterTightnessChecker();
		virtual ~BoundaryWaterTightnessChecker();

		virtual void setBoundary(const Data::Boundary *boundary);

		virtual void setAllowMultiple(bool allowMultiple);

		virtual void setStopOnFirst(bool stopOnFirst);

		virtual const AdjacencyMap &getInconsistents() const;

		virtual void execute();

	protected:

		virtual void buildAdjMap2D(AdjacencyMap &map);
		virtual void buildAdjMap3D(AdjacencyMap &map);

		virtual void check(AdjacencyMap &map);
		virtual void check(const AdjacencyMap::iterator &iter);
		virtual bool check(const VertexFEListMap::iterator &iter2);

	protected:

		const Data::Boundary *boundary;

		bool allowMultiple;

		bool stopOnFirst;

		AdjacencyMap inconsistents;

	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_WATER_TIGHTNESS_CHECKER_H_
