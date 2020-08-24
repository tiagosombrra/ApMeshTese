#ifndef _PERFORMER_MESH_INTERCEPTION_CHECKER_H_
#define _PERFORMER_MESH_INTERCEPTION_CHECKER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class MeshInterceptionChecker : public Performer::Builder
	{
	public:

#if USE_C__11
		typedef FEFEHashHashMap InterceptionMap;
#else
		typedef FEFESetMap InterceptionMap;
#endif //USE_C__11

	public:

		MeshInterceptionChecker();
		virtual ~MeshInterceptionChecker();

		virtual void setBoundary(const Data::Boundary *boundary);

		virtual void setMesh(const Data::Mesh *mesh);

		virtual void setStopOnFirst(bool stopOnFirst);

		virtual const InterceptionMap &getInterceptions() const;

		virtual void execute();

	protected:

		virtual void makeFrontElements();

		virtual void checkInterceptions();

		virtual void clearNewFrontElements();

		virtual Data::FrontElement *makeFrontElement(const Data::VertexVector &vertices, Data::Element *el) const = 0;

		virtual void adjustTopology(Data::FrontElement *fel, Data::Element *el) const;

	protected:

		const Data::Boundary *boundary;

		const Data::Mesh *mesh;

		bool stopOnFirst;

		InterceptionMap interceptions;

#if USE_C__11
		Data::FrontElementHash newFels;
#else
		Data::FrontElementSet newFels;
#endif //#if USE_C__11

		UInt dimension;

	};
}

#endif //#ifndef _PERFORMER_MESH_INTERCEPTION_CHECKER_H_
