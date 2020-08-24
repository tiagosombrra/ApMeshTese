#ifndef _TOOLS_BOUNDARY_CONVERSOR_H_
#define _TOOLS_BOUNDARY_CONVERSOR_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/BoundaryConversor.h"

namespace Tools
{
	class BoundaryConversor :
		public Performer::BoundaryConversor,
		public Tools::Tool
	{
	public:

		BoundaryConversor(Data::Boundary *original = NULL);
		virtual ~BoundaryConversor();

		virtual void execute();

	protected:

		virtual void addVertex(Real x, Real y, ULInt id);
		virtual void addVertex(Real x, Real y, Real z, ULInt id);
		virtual void addFrontElement(ULInt id1, ULInt id2, ULInt id);
		virtual void addFrontElement(ULInt id1, ULInt id2, ULInt id3, ULInt id);

		virtual Data::Boundary *consolidate() const;

	protected:

		MJMesh::BoundaryBuilder *builder;
	};
}

#endif //#ifndef _TOOLS_BOUNDARY_CONVERSOR_H_
