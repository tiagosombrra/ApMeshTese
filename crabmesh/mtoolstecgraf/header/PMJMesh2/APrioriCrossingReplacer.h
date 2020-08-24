#ifndef _PMJMESH2_A_PRIORI_CROSSING_REPLACER_H_
#define _PMJMESH2_A_PRIORI_CROSSING_REPLACER_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/CrossingReplacer.h"

namespace PMJMesh2
{
    class APrioriCrossingReplacer : public PMJMesh2::CrossingReplacer
    {
    public:

        APrioriCrossingReplacer(UInt dimension, bool onlyVertices, bool byTask,
            const Data::BSPTree *bsp = NULL, const Data::BSPTreeCell *parent = NULL,
            MJMesh::Boundary *boundary = NULL, const Parallel::TaskVector *tasks = NULL,
            const Data::VertexSet *vertices = NULL);
        virtual ~APrioriCrossingReplacer();

        virtual void setOnlyVertices(bool onlyVertices);

        virtual void setVertices(const Data::VertexSet *vertices);

        using PMJMesh2::CrossingReplacer::backReplace;

    protected:

        virtual void backReplace(MJMesh::FrontElement *fel, Data::FrontElementList *temporaries, const Data::Boundary *delExcept = NULL);
		virtual void backReplace(MJMesh::Vertex *v, Data::VertexList *temporaries, const Data::Boundary *delExcept = NULL);

        virtual void replace();
        virtual void backReplace();

    protected:

        bool onlyVertices;
        const Data::VertexSet *vertices;

    };
}

#endif //#ifndef _PMJMESH2_A_PRIORI_CROSSING_REPLACER_H_
