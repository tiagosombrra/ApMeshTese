#ifndef _PMJMESH2_AUXILIARY_FRONT_GENERATOR_H_
#define _PMJMESH2_AUXILIARY_FRONT_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh/CrossingReplacer.h"
#include "Performer/Builder.h"

namespace PMJMesh2
{
    class AuxiliaryFrontGenerator : public Performer::Builder
    {
    public:

        AuxiliaryFrontGenerator();
        virtual ~AuxiliaryFrontGenerator();

        virtual void setDimension(UInt dimension);
        virtual void setCreateCrackInInnerFront(bool createCrackInInnerFront);
        virtual void setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront);
        virtual void setRangeProportion(Real rangeProportion);

        virtual void setBox(const Data::Box *box);
        virtual void setTree(const Data::BoxTree *tree);
        virtual void setFront(PMJMesh::Front *front);
#if USE_DELETE_FELS
        virtual void setOldFront(PMJMesh::Front *oldFront);
#endif //#if USE_DELETE_FELS

        virtual void setIdManager(Performer::IdManager *idManager);

        virtual bool createdInnerFront() const;

        virtual void execute();

    protected:

        virtual void createInnerFront();
        virtual void deleteInnerFront();

        virtual bool checkInnerFront(const Data::VertexList &vertices,
            const Data::FrontElementList &fels, Real size) const;

        virtual MJMesh::Vertex *makeVertex(Data::Point *p) const;
        virtual MJMesh::FrontElement *makeFrontElement(
            MJMesh::Vertex *v1, MJMesh::Vertex *v2) const;
        virtual MJMesh::FrontElement *makeFrontElement(
            MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3) const;

    protected:

        UInt dimension;
        Real rangeProportion;

		bool createCrackInInnerFront;
		bool displaceVerticesInInnerFront;

		const Data::Box *box;
		const Data::BoxTree *tree;
		PMJMesh::Front *front;

#if USE_DELETE_FELS
        PMJMesh::Front *oldFront;
#endif //#if USE_DELETE_FELS

		Performer::IdManager *idManager;

#if USE_C__11
		PMJMesh::CrossingReplacer::VertexReplacementHashMap createdInnerVertices;
		PMJMesh::CrossingReplacer::FrontElementReplacementHashMap createdInnerFels;
#else
		PMJMesh::CrossingReplacer::VertexReplacementMap createdInnerVertices;
		PMJMesh::CrossingReplacer::FrontElementReplacementMap createdInnerFels;
#endif //#if USE_C__11

    };
}

#endif //#ifndef _PMJMESH2_AUXILIARY_FRONT_GENERATOR_H_
