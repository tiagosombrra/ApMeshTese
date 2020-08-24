#ifndef _TOOLS_INNER_SURFACES_IDENTIFIER_H_
#define _TOOLS_INNER_SURFACES_IDENTIFIER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "Performer/InnerSurfacesIdentifier.h"

namespace Tools
{
	class InnerSurfacesIdentifier :
		public Performer::InnerSurfacesIdentifier,
		public Tools::Tool
	{
	public:

		InnerSurfacesIdentifier();
		virtual ~InnerSurfacesIdentifier();

		virtual void execute();

	protected:

		virtual Data::Boundary *makeBoundary() const;
		virtual MJMesh::AnglePointInObjectDecider *makeDecider() const;
		virtual Data::Position position(const Data::Point *point, const Data::Boundary *boundary) const;

		using Performer::InnerSurfacesIdentifier::propagate;
		virtual void propagate();
		virtual void propagate(Data::FrontElement *fe, Surface *surface);

		using Performer::InnerSurfacesIdentifier::findCompleteBoundary2D;
		using Performer::InnerSurfacesIdentifier::findCompleteBoundary3D;
		virtual bool findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested);
		virtual bool findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested);

		virtual void findCompleteBoundaries();

	protected:

		MJMesh::AnglePointInObjectDecider *decider;
	};
}

#endif //#ifndef _TOOLS_INNER_SURFACES_IDENTIFIER_H_
