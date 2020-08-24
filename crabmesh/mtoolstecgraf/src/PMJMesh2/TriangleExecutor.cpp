#include "PMJMesh2/TriangleExecutor.h"

#include "MJMesh/ElementSurface.h"
#include "MJMesh/Vertex.h"


PMJMesh2::TriangleExecutor::TriangleExecutor( Data::VertexList &vertexs, bool invertMesh )
{
    this->area = 0.0;
    this->numHoles = 0;
    this->numAreas = 0;
    this->pointHoles.clear();

    this->vertexs = vertexs;
    this->invertMesh = invertMesh;
}

PMJMesh2::TriangleExecutor::~TriangleExecutor()
{

}

void PMJMesh2::TriangleExecutor::execute()
{
    convertFrontToTriangle();
    executeTriangle();
    executeMeshBuilding();
}

void PMJMesh2::TriangleExecutor::convertFrontToTriangle()
{
    ///Converting vertexs
    Data::IntMap map;


    in.pointlist = NULL;
    in.pointattributelist = NULL;
    in.pointmarkerlist = NULL;
    in.numberofpoints = 0;
    in.numberofpointattributes = 0;

    in.trianglelist = NULL;
    in.triangleattributelist = NULL;
    in.trianglearealist = NULL;
    in.neighborlist = NULL;
    in.numberoftriangles = 0;
    in.numberofcorners = 0;
    in.numberoftriangleattributes = 0;

    in.segmentlist = NULL;
    in.segmentmarkerlist = NULL;
    in.numberofsegments = 0;

    in.holelist = NULL;
    in.numberofholes = 0;

    in.regionlist = NULL;
    in.numberofregions = 0;

    in.edgelist = NULL;
    in.edgemarkerlist = NULL;
    in.normlist = NULL;
    in.numberofedges = 0;

    out.pointlist = NULL;;
    out.pointattributelist = NULL;
    out.pointmarkerlist = NULL;
    out.numberofpoints = 0;
    out.numberofpointattributes = 0;

    out.trianglelist = NULL;
    out.triangleattributelist = NULL;
    out.trianglearealist = NULL;
    out.neighborlist = NULL;
    out.numberoftriangles = 0;
    out.numberofcorners = 0;
    out.numberoftriangleattributes = 0;

    out.segmentlist = NULL;
    out.segmentmarkerlist = NULL;
    out.numberofsegments = 0;

    out.holelist = NULL;
    out.numberofholes = 0;

    out.regionlist = NULL;
    out.numberofregions = 0;

    out.edgelist = NULL;
    out.edgemarkerlist = NULL;
    out.normlist = NULL;
    out.numberofedges = 0;


    int size = static_cast<int>(vertexs.size());

    if( size == 0 )
        std::cout << "ERROR: TRIANGLE VERTEXS SIZE = 0" << std::endl;

    this->vMap.clear();
    this->vMap.resize(size);

    //std::cout << "Num Pontos: " << size << std::endl;


    ///ENCONTRANDO O PLANO DOS VERTICES
    Data::Vertex *v1 = vertexs.front();
    Data::Vertex *v2 = vertexs.back();
    //int coords[3];
    int pos = 0;
    for( int x=0; x<3; x++ )
    {
        if( v1->getPoint()->getCoord(x) != v2->getPoint()->getCoord(x) )
            this->coords[pos++] = x;
        else
        {
            this->plane_coord = v1->getPoint()->getCoord(x);
            this->coords[2] = x;
        }
    }



    in.numberofpoints = size;

    in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
    int i = 0;

    for (std::list<Data::Vertex*>::iterator it = vertexs.begin(); it != vertexs.end(); it++)
    {
        Data::Vertex *v = (*it);

        in.pointlist[i * 2 + 0] = v->getPoint()->getCoord( coords[0] );
        in.pointlist[i * 2 + 1] = v->getPoint()->getCoord( coords[1] );

        this->vMap[i] = v;
        map[v->getId()] = i;

        ////debug
        //std::cout << "adding to this->vMap = " << v->text() << " in position " << i << std::endl;
        //std::cout << i << " " << in.pointlist[i * 2 + 0] << " " << in.pointlist[i * 2 + 1] << std::endl;
        ////endebug
        //std::cout << i+1 << "\t" << v->getPoint()->getCoord(0) << "\t" << v->getPoint()->getCoord(1) << std::endl;
        //std::cout << "setting vertex map, mjmesh vertex = " << v->getId() << " mapped to jmesh vertex " << i << std::endl;
        //std::cout << "vertex " << v->text() << " mapped to vertex " << i << std::endl;

        i++;
    }
}

bool PMJMesh2::TriangleExecutor::executeTriangle()
{
    std::stringstream params;

    /// SEE: http://www.cs.cmu.edu/~quake/triangle.switch.html
    params << "qzBDYYQ";
    //params << "S0";         ///NUMBER OF Steiner points

    /*if (this->area > 0.0001)
    {
        params << "a" << std::fixed << this->area;
    }*/

    triangulate(const_cast<char *>(params.str().c_str()), &in, &out, (struct triangulateio *) NULL);

    return true;
}

bool PMJMesh2::TriangleExecutor::executeMeshBuilding()
{
	///Converting vertices
    ULInt id_vert = this->vertexs.size();
    int num_gen_nodes = out.numberofpoints - in.numberofpoints;
    this->vMap.resize(this->vMap.size() + num_gen_nodes);

    //std::cout << "GEROU pontos novos:  " << num_gen_nodes << std::endl;

    for (int i = 0; i < num_gen_nodes; i++)
    {
        Data::Point3D *p = NULL;

        p = new Data::Point3D();

        //p->setPosition(out.pointlist[i*2 + 0 + in.numberofpoints*2], out.pointlist[i*2 + 1 + in.numberofpoints*2]);

        p->setCoord( this->coords[0], out.pointlist[i*2 + 0 + in.numberofpoints*2] );
        p->setCoord( this->coords[1], out.pointlist[i*2 + 1 + in.numberofpoints*2] );
        p->setCoord( this->coords[2], this->plane_coord );


        MJMesh::Vertex *v = NULL;

        v = new MJMesh::Vertex(p, id_vert++);


        this->vMap[in.numberofpoints + i] = v;

        //if (debugrank == 0) std::cout << "adding to this->vMap = " << v->text() << " in position " << this->num_node + i << std::endl;

        //std::cout << "vertex " << this->num_node + i << " back-mapped to vertex " << v->text() << std::endl;

        this->vertexs.push_back( v );
    }


    ///Converting elements
    ULInt id_el = 0;

    //std::cout << "GEROU:  " << out.numberoftriangles << std::endl;

    for (int i = 0; i < out.numberoftriangles; i++)
    {
        Data::Vertex *vs[3];

        vs[0] = this->vMap[out.trianglelist[i*3 + 0]];
        vs[1] = this->vMap[out.trianglelist[i*3 + 1]];
        vs[2] = this->vMap[out.trianglelist[i*3 + 2]];


        //std::cout << "VALOR: " << i*3 + 0 << " " << i*3 + 1 << " " << i*3 + 2 << std::endl;
        //std::cout << "NUM: " << out.trianglelist[i*3 + 0] << " " << out.trianglelist[i*3 + 1] << " " << out.trianglelist[i*3 + 2] << std::endl;
        //std::cout << "V0: " << vs[0]->getId() << std::endl;
        //std::cout << "V1: " << vs[1]->getId() << std::endl;
        //std::cout << "V2: " << vs[2]->getId() << std::endl;

        MJMesh::ElementSurface *el = NULL;

        if( invertMesh )
            el = new MJMesh::ElementSurface(
                static_cast<MJMesh::Vertex *>(vs[2]),
                static_cast<MJMesh::Vertex *>(vs[1]),
                static_cast<MJMesh::Vertex *>(vs[0]),
                id_el++);
        else
            el = new MJMesh::ElementSurface(
                static_cast<MJMesh::Vertex *>(vs[0]),
                static_cast<MJMesh::Vertex *>(vs[1]),
                static_cast<MJMesh::Vertex *>(vs[2]),
                id_el++);


        elements.push_back( el );
    }

    if (in.pointlist) free(in.pointlist);
    if (in.pointattributelist) free(in.pointattributelist);
    if (in.pointmarkerlist) free(in.pointmarkerlist);
    if (in.trianglelist) free(in.trianglelist);
    if (in.triangleattributelist) free(in.triangleattributelist);
    if (in.trianglearealist) free(in.trianglearealist);
    if (in.neighborlist) free(in.neighborlist);
    if (in.segmentlist) free(in.segmentlist);
    if (in.segmentmarkerlist) free(in.segmentmarkerlist);
    if (in.holelist) free(in.holelist);
    if (in.regionlist) free(in.regionlist);
    if (in.edgelist) free(in.edgelist);
    if (in.edgemarkerlist) free(in.edgemarkerlist);
    if (in.normlist) free(in.normlist);

    if (out.pointlist) free(out.pointlist);
    if (out.pointattributelist) free(out.pointattributelist);
    if (out.pointmarkerlist) free(out.pointmarkerlist);
    if (out.trianglelist) free(out.trianglelist);
    if (out.triangleattributelist) free(out.triangleattributelist);
    if (out.trianglearealist) free(out.trianglearealist);
    if (out.neighborlist) free(out.neighborlist);
    if (out.segmentlist) free(out.segmentlist);
    if (out.segmentmarkerlist) free(out.segmentmarkerlist);
    if (out.holelist) free(out.holelist);
    if (out.regionlist) free(out.regionlist);
    if (out.edgelist) free(out.edgelist);
    if (out.edgemarkerlist) free(out.edgemarkerlist);
    if (out.normlist) free(out.normlist);


    return true;
}
