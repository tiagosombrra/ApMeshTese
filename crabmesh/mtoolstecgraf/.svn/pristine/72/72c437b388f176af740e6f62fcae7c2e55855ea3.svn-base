#ifndef _TOOLS_BOUNDARY_NORMAL_FIXER_H_
#define _TOOLS_BOUNDARY_NORMAL_FIXER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/BoundaryNormalFixer.h"

namespace Tools
{
	class BoundaryNormalFixer :
		public Performer::BoundaryNormalFixer,
		public Tools::Tool
	{
	public:

		BoundaryNormalFixer();
		virtual ~BoundaryNormalFixer();

		virtual void execute();

	protected:

		struct BoundaryTree;

		typedef std::list<BoundaryTree *> BoundaryTreeList;

		struct BoundaryTree
		{
			Data::FrontElementList fels;

			BoundaryTreeList children;
		};

	protected:

		static bool smallerCoords(const Data::Vertex *v1, const Data::Vertex *v2);

		virtual MJMesh::AnglePointInObjectDecider *makeDecider() const;

		using Performer::BoundaryNormalFixer::test2D;
		using Performer::BoundaryNormalFixer::test3D;

		virtual bool test2D(Data::FrontElement *fel, Data::FrontElementList &toBoTested);
		virtual bool test3D(Data::FrontElement *fel, Data::FrontElementList &toBoTested);

		virtual bool isInner(const BoundaryTree *node, BoundaryTree *&innermost) const;
		virtual void clear(BoundaryTree *node) const;

	protected:

		BoundaryTree *current;

		MJMesh::AnglePointInObjectDecider *decider;
	};
}

#endif //#ifndef _TOOLS_BOUNDARY_NORMAL_FIXER_H_
