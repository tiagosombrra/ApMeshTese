#ifndef _PJMESH2_TriangleExecutor_H_
#define _PJMESH2_TriangleExecutor_H_

#include "PMJMesh2/Definitions.h"


#include "PMJMesh2/Triangle.h"


namespace PMJMesh2
{
	class TriangleExecutor
	{
	public:

		TriangleExecutor( Data::VertexList &vertexs, bool invertMesh );
		virtual ~TriangleExecutor();

		Data::ElementList &getElements() { return this->elements; }
		Data::VertexList &getvertexs() { return this->vertexs; }

		virtual void execute();

	protected:

		virtual void convertFrontToTriangle();
		virtual bool executeTriangle();
		virtual bool executeMeshBuilding();

    private:
        bool invertMesh;
        Data::VertexVector vMap;

        Data::VertexList vertexs;
        Data::ElementList elements;

        int coords[3];
        Real plane_coord;

        Real area;
        struct triangulateio in, out;
        int numHoles, numAreas;
        std::vector<REAL> pointHoles;
	};
}

#endif //#ifndef _PJMESH2_TriangleExecutor_H_
