#ifndef _MJMESH_HOTELLING_TRANSFORMER_H_
#define _MJMESH_HOTELLING_TRANSFORMER_H_

#include "MJMesh/Definitions.h"
#include "Performer/HotellingTransformer.h"

#if USE_NUMERICAL

namespace MJMesh
{
    class HotellingTransformer : public Performer::HotellingTransformer
    {
    public:

        HotellingTransformer();
        virtual ~HotellingTransformer();

        virtual void execute();
        virtual void executeBack();

    protected:

        virtual void executePrincipalComponentsBasisFinder();
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _MJMESH_HOTELLING_TRANSFORMER_H_
