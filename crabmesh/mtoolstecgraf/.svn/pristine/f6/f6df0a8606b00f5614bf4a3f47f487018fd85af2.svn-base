#ifndef _TOOLS_VISIBILITY_BASED_VERTEX_FINDER_H_
#define _TOOLS_VISIBILITY_BASED_VERTEX_FINDER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Tools/VisibilityIdentifier.h"

namespace Tools
{
    class VisibilityBasedVertexFinder : public Tools::ExternalTool
    {
    public:

        VisibilityBasedVertexFinder(const Tool *tool = NULL);
        virtual ~VisibilityBasedVertexFinder();

        ///The aux point will be the centroid of the front element
        virtual void addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel);
        virtual void addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, const Data::Point2D &aux);

#if USE_GUI
        virtual void addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, MJMesh::Mesh *mesh);
        virtual void addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, const Data::Point2D &aux, MJMesh::Mesh *mesh);
#endif //#if USE_GUI

        virtual void setSearchVertex(bool searchVertex);

        virtual bool found() const;

        virtual bool foundVertex() const;
        virtual ULInt getFoundVertex() const;

        virtual bool foundFrontElement() const;
        virtual ULInt getFoundFrontElement() const;
        virtual Real getFoundFrontElementParameter() const;

        virtual void execute();

        virtual void findNextBestVertexOrEdge();

    protected:

        typedef std::list<Tools::VisibilityIdentifier::Visibility> VisibilityList;
        typedef std::list<Data::VertexList> VLList;

    protected:

        virtual FrontElementVisibilityIdentifier *makeFEVisibilityIdentifier() const;
        virtual Tools::VisibilityIdentifier::Visibility findVisibility(Data::Boundary *boundary, Data::FrontElement *fel) const;

        virtual void findBestVertex();
        virtual void findBestEdge();
        virtual ULInt best(const VLList &vertices) const;

    protected:

        Data::BoundaryList boundaries;
        Data::FrontElementList frontElements;
        Data::Point2DObjList points;

#if USE_GUI
        Data::MeshList meshes;
#endif //#if USE_GUI

        bool searchVertex;

        VisibilityList visibilities;
        VisibilityList intersections;

        bool vFound;
        ULInt vertex;

        bool feFound;
        ULInt fel;
        Real param;

    };
}

#endif //#ifndef _TOOLS_VISIBILITY_BASED_VERTEX_FINDER_H_
