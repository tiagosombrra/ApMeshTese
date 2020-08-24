#ifndef _TOOLS_BOUNDARY_CREATOR_H_
#define _TOOLS_BOUNDARY_CREATOR_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/BoundaryCreator.h"

namespace Tools
{
	class BoundaryCreator :
		public Performer::BoundaryCreator,
		public Tools::Tool
	{
	public:

		BoundaryCreator(Data::Mesh *mesh = NULL);
		virtual ~BoundaryCreator();

		virtual Data::Boundary *makeBoundary() const;

		virtual void execute();

	protected:

		using Performer::BoundaryCreator::execute2D;

		virtual void execute2D(Data::Element *e, Performer::BoundaryCreator::DoneList2D &done);

		virtual void execute3D();
		virtual void execute3D(Data::Element *e, Performer::BoundaryCreator::DoneList3D &done);

		virtual Data::FrontElement *makeFrontElement(const Data::VertexVector &vertices, Data::Element *e) const;

    protected:

        bool surface;
	};
}

#endif //#ifndef _TOOLS_BOUNDARY_CREATOR_H_
