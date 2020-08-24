#ifndef _PERFORMER_BOUNDARY_NORMAL_FIXER_H_
#define _PERFORMER_BOUNDARY_NORMAL_FIXER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryNormalFixer : public Performer::Builder
	{
	public:

		BoundaryNormalFixer();
		virtual ~BoundaryNormalFixer();

		virtual void setBoundary(Data::Boundary *boundary);

		virtual bool isAnyInverted() const;

		virtual void execute();

	public:

		enum
		{
			NOT_INIT = 0,
			INIT,
			SUCCESS,
			ERROR = 100
		};

		static const Performer::Status notInit;
		static const Performer::Status init;
		static const Performer::Status success;
		static const Performer::Status error;

	protected:

		virtual bool shouldInvert2D(const Data::FrontElement *adj,
			const Data::FrontElement *fel, const Data::Vertex *v) const;
		virtual bool shouldInvert3D(const Data::FrontElement *adj,
			const Data::FrontElement *fel,
			const Data::Vertex *v1, const Data::Vertex *v2) const;

		virtual void test2D(Data::FrontElement *fel);
		virtual void test3D(Data::FrontElement *fel);

		virtual bool test2D(Data::FrontElement *fel, Data::FrontElementList &toBeTested);
		virtual bool test3D(Data::FrontElement *fel, Data::FrontElementList &toBeTested);

		virtual void invert(Data::FrontElement *fel);

	protected:

		Data::Boundary *boundary;

		UInt dimension;

#if USE_C__11
		Data::FrontElementHash tested;
#else
		Data::FrontElementSet tested;
#endif //#if USE_C__11

		bool anyInverted;
	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_NORMAL_FIXER_H_
