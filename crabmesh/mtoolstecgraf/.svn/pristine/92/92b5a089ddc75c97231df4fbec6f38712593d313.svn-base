#ifndef _TOOLS_VISIBILITY_IDENTIFIER_H_
#define _TOOLS_VISIBILITY_IDENTIFIER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Data/Exact.h"
#include "Data/Vector2D.h"

namespace Tools
{
    class VisibilityIdentifier : public Tools::ExternalTool
    {
    public:

        enum Operation
        {
            OPERATION_UNKNOWN = 0,
            UNION,
            INTERSECTION
        };

        enum AngleType
        {
            ANGLE_UNKNOWN = 0,
            COLINEAR,
            CONCAVE,
            CONVEX
        };

        struct EdgeVisibility
        {
            MJMesh::FrontElement2D *fel;
            Data::EFRealVector visibility;
        };

        typedef std::list<EdgeVisibility> EdgeVisibilityList;

        struct Visibility
        {
            EdgeVisibilityList edges;
            Data::VertexList vertices;
        };

        struct Intersection
        {
            Data::EFReal t;
            MJMesh::FrontElement2D *fel;

            bool operator<(const Intersection &intersection) const
            {
                return ((this->t < intersection.t) ||
                        ((this->t <= intersection.t) && (this->fel < intersection.fel)));
            };
        };

        typedef std::set<Intersection> IntersectionSet;

        struct Vtx
        {
            Data::Vertex *v;
            Data::EFReal angle;
            Data::EFReal dist;

            bool artificial;
            Data::EVector2D vec;
            mutable IntersectionSet intersections;

            bool operator<(const Vtx &vtx) const
            {
                return ((this->angle < vtx.angle) ||
                        ((this->angle <= vtx.angle) && (this->dist < vtx.dist)));
            };
        };

        typedef std::set<Vtx> VtxSet;

        struct EdgeIntersection
        {
            mutable const Vtx *vtx;
            Data::EFReal s;
            mutable enum Data::Position pos;
            mutable enum Data::Position prevPos;

            bool operator<(const EdgeIntersection &intersection) const
            {
                return (this->s < intersection.s);
            };
        };

        typedef std::set<EdgeIntersection> EdgeIntersectionSet;
        typedef std::unordered_map<Data::FrontElement *, EdgeIntersectionSet> FEEdgeIntersectionSetMap;

        struct EdgeSegment
        {
            Data::EFReal t1, t2;
            Data::Position pos;
        };

        struct VertexSegment
        {
            Data::EFReal t;
            Data::Position pos;
        };

        typedef std::list<EdgeSegment> EdgeSegmentList;
        typedef std::list<VertexSegment> VertexSegmentList;

        struct SegmentedEdge
        {
            EdgeSegmentList segments;
            VertexSegmentList vertices;
        };

        typedef std::unordered_map<Data::FrontElement *, SegmentedEdge> SegmentedEdgeMap;

    public:

        VisibilityIdentifier(const Tool *tool = NULL);
        virtual ~VisibilityIdentifier();

#if USE_GUI
        virtual void setMesh(MJMesh::Mesh *mesh);
#endif //#if USE_GUI

        virtual void setBoundary(const MJMesh::Boundary *boundary);
        virtual void setVertex(const MJMesh::Vertex *vertex, const MJMesh::Mesh *mesh = NULL);
        virtual void setVertex(const MJMesh::Vertex *vertex, const MJMesh::FrontElement2D *fel);
        virtual void setFrontElements(const MJMesh::FrontElement2D *fel1, const MJMesh::FrontElement2D *fel2);
        virtual void setOperation(enum Operation operation);

        virtual Visibility getVisibility() const;

        virtual void execute();

    protected:

        virtual void findVertex();   //determine the angle type -> colinear, concave or convex
        virtual void findAngleType();   //determine the angle type -> colinear, concave or convex
        virtual void findOperation();   //determine the operation given an angle type
        virtual void findVisibility(const MJMesh::FrontElement2D *fel, Visibility &vis);   //find the visibility of an edge (with no shadowing)
        virtual void operateVisibilities(); //unify or intersect the visibilities of two edges (with no shadowing)
        virtual void findIntersections();
        virtual void narrowVisibility();    //remove the edges that are completely hidden (some shadowing)

        virtual bool areCollinear(const Data::Point2D *p1, const Data::Point2D *p2, const Data::Point2D *p3) const;
        virtual bool isVisible(const Data::Point2D *p1, const Data::Point2D *p2, const Data::Point2D *p3, Data::EFReal t2, Data::EFReal t3, const Data::FrontElement *fel);

        virtual void determineVisibility(); //given the remaining edges, determine the parameterizations that are visible, if any (final shadowing)

        virtual void findIntersections(const Vtx *vtx, const Data::EVector2D &vec,
            const EdgeVisibilityList &edges, IntersectionSet &intersections, enum Data::Position pos, bool addToVisibility = true);
        virtual IntersectionSet findIntersections(const Vtx *vtx, const Data::EVector2D &vec,
            MJMesh::FrontElement2D *fel, enum Data::Position pos, bool addToVisibility = true);
        virtual void addToVisibility(MJMesh::FrontElement2D *fel, EdgeIntersection &ei);

    protected:

#if USE_GUI
        MJMesh::Mesh *mesh;
        Performer::SequentialIdManager *idManager;
#endif //#if USE_GUI

        const MJMesh::Boundary *boundary;
        const MJMesh::FrontElement2D *fel1;
        const MJMesh::FrontElement2D *fel2;
        enum Operation operation;

        UInt vertex;
        MJMesh::Vertex *v;
        MJMesh::Vertex *v1;
        MJMesh::Vertex *v2;

        enum AngleType angleType;

        Visibility vis1;
        Visibility vis2;

        VtxSet vertices;
        FEEdgeIntersectionSetMap edgeVisibility;
        SegmentedEdgeMap segments;

    };
}

#endif //#ifndef _TOOLS_VISIBILITY_IDENTIFIER_H_
