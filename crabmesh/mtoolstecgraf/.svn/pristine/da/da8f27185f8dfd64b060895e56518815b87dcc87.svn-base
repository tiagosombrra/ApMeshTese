#ifndef _TECGRAF_BOUNDARY_BUILDER_H_
#define _TECGRAF_BOUNDARY_BUILDER_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF

#include "MJMesh/BoundaryBuilder.h"

namespace TecGraf
{
	class BoundaryBuilder : public MJMesh::BoundaryBuilder
	{
	public:

		BoundaryBuilder(bool sorted);
		virtual ~BoundaryBuilder();

		virtual void setRegion(Region *region);

		virtual const Data::ULIntMap &getVIdMap() const;
		virtual const Data::ULIntMap &getFEIdMap() const;

		virtual void clear();

		virtual void execute();

    protected:

        typedef std::map<Data::Vertex *, Data::FrontElementList> VertexFrontElementListMap;
        typedef std::map<Data::Vertex *, VertexFrontElementListMap> EdgeFrontElementMap;

        typedef std::list<std::list<Data::FrontElementList> > FrontElementListList;

    protected:

        virtual bool buildMap(EdgeFrontElementMap &eMap) const;

        virtual bool makeCracks(const Surface *s, const EdgeFrontElementMap &eMap);

        virtual void adjustForCracks();

	protected:

		Region *region;

		Data::ULIntMap vidMap;
		Data::ULIntMap feidMap;

		ULInt lastIdVertex;
        ULInt lastIdFrontElement;

	};
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TECGRAF_BOUNDARY_BUILDER_H_
