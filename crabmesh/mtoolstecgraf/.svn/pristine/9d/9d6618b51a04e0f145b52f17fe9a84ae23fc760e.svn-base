#ifndef _PMJMESH_HOTELLING_TRANSFORMER_H_
#define _PMJMESH_HOTELLING_TRANSFORMER_H_

#include "PMJMesh/Definitions.h"
#include "MJMesh/HotellingTransformer.h"

#if USE_NUMERICAL

namespace PMJMesh
{
    class HotellingTransformer : public MJMesh::HotellingTransformer
    {
    public:

        HotellingTransformer();
        virtual ~HotellingTransformer();

        virtual void addMesh(Data::Mesh *mesh);

        virtual void executeBackOnMeshes();

    protected:

        Data::MeshList meshes;
    };
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _PMJMESH_HOTELLING_TRANSFORMER_H_

