#ifndef _PMJMESH2_MESH_GENERATOR_H_
#define _PMJMESH2_MESH_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "Performer/MeshGenerator.h"

namespace PMJMesh2
{
    class MeshGenerator : public Performer::MeshGenerator
    {
    public:

        MeshGenerator();
        virtual ~MeshGenerator();

        virtual void setTree(Data::BoxTree *tree);
        virtual void setFront(PMJMesh::Front *front);
        virtual void setBox(Data::Box *box);

        virtual void execute();

    protected:

        Data::BoxTree *tree;

        PMJMesh::Front *front;

        Data::Box *box;

    };
}

#endif //#ifndef _PMJMESH2_MESH_GENERATOR_H_
