#ifndef _TOOLS_FRONT_ELEMENT_VISIBILITY_IDENTIFIER_H_
#define _TOOLS_FRONT_ELEMENT_VISIBILITY_IDENTIFIER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Tools/VisibilityIdentifier.h"

namespace Tools
{
    class FrontElementVisibilityIdentifier : public Tools::ExternalTool
    {
    friend
        class VisibilityBasedVertexFinder;

    public:

        FrontElementVisibilityIdentifier(const Tool *tool = NULL);
        virtual ~FrontElementVisibilityIdentifier();

#if USE_GUI
        virtual void setMesh(MJMesh::Mesh *mesh);
#endif //#if USE_GUI

        virtual void setBoundary(const MJMesh::Boundary *boundary);
        virtual void setFrontElement(const MJMesh::FrontElement *fel);

        virtual Tools::VisibilityIdentifier::Visibility getVisibility() const;

        virtual void execute();

    protected:

        virtual Tools::VisibilityIdentifier::Visibility findVisibility(const MJMesh::Vertex *v) const;

        virtual Tools::VisibilityIdentifier::Visibility mergeVisibilities(
            const Tools::VisibilityIdentifier::Visibility &vis1, const Tools::VisibilityIdentifier::Visibility &vis2) const;
        virtual Data::EFRealVector mergeVisibilities(const Data::EFRealVector &vis1, const Data::EFRealVector &vis2) const;
        virtual Data::Position intersection(Data::Position pos1, Data::Position pos2) const;

    protected:

#if USE_GUI
        MJMesh::Mesh *mesh;
        Performer::SequentialIdManager *idManager;
#endif //#if USE_GUI

        const MJMesh::Boundary *boundary;
        const MJMesh::FrontElement *fel;

        Tools::VisibilityIdentifier::Visibility vis1;
        Tools::VisibilityIdentifier::Visibility vis2;
        Tools::VisibilityIdentifier::Visibility vis;

    };
}

#endif //#ifndef _TOOLS_FRONT_ELEMENT_VISIBILITY_IDENTIFIER_H_
