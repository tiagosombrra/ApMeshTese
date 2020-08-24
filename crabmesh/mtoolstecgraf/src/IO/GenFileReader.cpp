#include "IO/GenFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"

using namespace IO;

IO::GenFileReader::GenFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->dimension = 0;
	this->boundary = false;
	this->surface = false;

	this->addFiletype(".gen");
}

IO::GenFileReader::~GenFileReader()
{

}

void IO::GenFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

UInt IO::GenFileReader::getDimension() const
{
	return this->dimension;
}

bool IO::GenFileReader::isBoundary() const
{
	return this->boundary;
}

bool IO::GenFileReader::isSurface() const
{
	return this->surface;
}

bool IO::GenFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return true;
}

Data::AbstractMesh *IO::GenFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::GenFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::GenFileReader::read()
{
	bool ok = this->readGen();

    if (!ok)
	{
		if (this->boundaryBuilder)
        {
            this->boundaryBuilder->clear();
        }

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		this->dimension = 0;
		this->boundary = false;
	}
}

bool IO::GenFileReader::readGen()
{
    /* File format:

    dim //2 for 2D meshes, 2.5 for surface meshes, 3 for 3D meshes;
    nv
    v1_id v1_x v1_y[ v1_z]
    ...
    vnv_id vnv_x vnv_y[ vnv_z]
    nfe
    fe1_id fe1_nv fe1_idv1 ... fe1_idvfe1_nv
    ...
    fenfe_id fenfe_nv fenfe_idv1 ... fenfe_idvfenfe_nv
    ne
    e1_id e1_nv e1_idv1 ... e1_idve1_nv
    ...
    ene_id ene_nv ene_idv1 ... ene_idvene_nv
    //optionals
    [np //number of partitions
    [idp1
    p1_nv_owned
    p1_v_owned_1_id
    ...
    p1_v_owned_nv_owned_id
    p1_nfe_owned
    p1_fe_owned_1_id
    ...
    p1_fe_owned_nfe_owned_id
    p1_nf_owned
    p1_e_owned_1_id
    ...
    p1_e_owned_ne_owned_id

    p1_nv_ghost
    p1_v_ghost_1_id p1_v_ghost_1_id_owner
    ...
    p1_v_ghost_nv_ghost_id p1_v_ghost_nv_ghost_id_owner
    p1_nfe_ghost
    p1_fe_ghost_1_id p1_fe_ghost_1_id_owner
    ...
    p1_fe_ghost_nfe_ghost_id p1_fe_ghost_nfe_ghost_id_owner
    p1_nf_ghost
    p1_e_ghost_1_id p1_e_ghost_1_id_owner
    ...
    p1_e_owned_ne_owned_id p1_e_owned_ne_owned_id_owner]
    [...]
    ...
    ]
    */

	bool ok = false;

#if USE_C__11
	Data::VertexHashMap vertices;
#else
	Data::VertexMap vertices;
#endif //#if USE_C__11

    std::string dim;

    this->file >> dim;

    if (dim == "2")
    {
        this->dimension = 2;
        this->surface = false;
    }
    else if (dim == "2.5")
    {
        this->dimension = 3;
        this->surface = true;
    }
    else if (dim == "3")
    {
        this->dimension = 3;
        this->surface = false;
    }
    else
    {
        return ok;
    }

    ULInt vCount = 0;

    typedef std::list<Data::VertexList> VertexListList;

    VertexListList order;

    order.push_back(Data::VertexList());

	this->file >> vCount;

	bool incrIdV = false;

	for (ULInt i = 0; i < vCount; i++)
    {
        std::string str;

        std::getline(this->file, str);

        while (str.size() <= 1)
        {
            std::getline(this->file, str);
        }

        if ((this->dimension == 2) && (str == "-1"))
        {
            order.push_back(Data::VertexList());

            continue;
        }

        std::stringstream stream(str.c_str());

        ULInt id;

        stream >> id;

        if (id == 0)
        {
            incrIdV = true;
        }

        Real x = 0.0, y = 0.0, z = 0.0;

        stream >> x;
        stream >> y;

        if (this->dimension == 3)
        {
            stream >> z;
        }

        Data::Vertex *v = NULL;

        if (this->dimension == 2)
        {
           v = new MJMesh::Vertex(x, y, id);
        }
        else if (this->dimension == 3)
        {
            v = new MJMesh::Vertex(x, y, z, id);
        }

        vertices[id] = v;

        order.back().push_back(v);
    }

    ULInt feCount = 0;

    this->file >> feCount;

    Data::FrontElementList fels;

    bool incrIdFE = false;

    for (ULInt i = 0; i < feCount; i++)
    {
        std::string str;

        std::getline(this->file, str);

        while (str.size() <= 1)
        {
            std::getline(this->file, str);
        }

        std::stringstream stream(str.c_str());

        ULInt id;

        stream >> id;

        if (id == 0)
        {
            incrIdFE = true;
        }

        UInt nvs;

        stream >> nvs;

        if (((this->dimension == 2) && (nvs != 2)) ||
            ((this->dimension == 3) && (nvs != 2) && (nvs != 3) && (nvs != 4)))
        {
            continue;
        }

        MJMesh::Vertex *vs[nvs];

        for (UInt j = 0; j < nvs; j++)
        {
            ULInt idv;

            stream >> idv;

            vs[j] = static_cast<MJMesh::Vertex *>(vertices[idv]);
        }

        Data::FrontElement *fel = NULL;

        if ((this->dimension == 2) && (nvs == 2) && (!this->surface))
        {
            fel = new MJMesh::FrontElement2D(vs[0], vs[1], id);
        }
        else if ((this->dimension == 3) && (nvs == 2) && (this->surface))
        {
            fel = new MJMesh::FrontElementSurface(vs[0], vs[1], id);
        }
        else if ((this->dimension == 3) && (nvs == 3) && (!this->surface))
        {
            fel = new MJMesh::FrontElement3D(vs[0], vs[1], vs[2], id);
        }
#if USE_QUADS
        else if ((this->dimension == 3) && (nvs == 4) && (!this->surface))
        {
            fel = new MJMesh::FrontElement3D(vs[0], vs[1], vs[2], vs[3], id);
        }
#endif //#if USE_QUADS

        if (fel)
        {
            fels.push_back(fel);
        }
    }

    ULInt eCount = 0;

    this->file >> eCount;

    Data::ElementList els;

    bool incrIdE = false;

    for (ULInt i = 0; i < eCount; i++)
    {
        std::string str;

        std::getline(this->file, str);

        while (str.size() <= 1)
        {
            std::getline(this->file, str);
        }

        std::stringstream stream(str.c_str());

        ULInt id;

        stream >> id;

        if (id == 0)
        {
            incrIdE = true;
        }

        UInt nvs;

        stream >> nvs;

        if (((this->dimension == 2) && (nvs != 3) && (nvs != 4) && (!this->surface)) ||
            ((this->dimension == 3) && (nvs != 3) && (nvs != 4) && (this->surface)) ||
            ((this->dimension == 3) && (nvs != 4) && (nvs != 5) && (nvs != 6) && (nvs != 8) && (!this->surface)))
        {
            continue;
        }

        MJMesh::Vertex *vs[nvs];

        for (UInt j = 0; j < nvs; j++)
        {
            ULInt idv;

            stream >> idv;

            vs[j] = static_cast<MJMesh::Vertex *>(vertices[idv]);
        }

        Data::Element *e = NULL;

        if ((this->dimension == 2) && (nvs == 3) && (!this->surface))
        {
            e = new MJMesh::Element2D(vs[0], vs[1], vs[2], id);
        }
#if USE_QUADS
        else if ((this->dimension == 2) && (nvs == 4) && (!this->surface))
        {
            e = new MJMesh::Element2D(vs[0], vs[1], vs[2], vs[3], id);
        }
#endif //#if USE_QUADS
        else if ((this->dimension == 3) && (nvs == 3) && (this->surface))
        {
            e = new MJMesh::ElementSurface(vs[0], vs[1], vs[2], id);
        }
#if USE_QUADS
        else if ((this->dimension == 3) && (nvs == 4) && (this->surface))
        {
            e = new MJMesh::ElementSurface(vs[0], vs[1], vs[2], vs[3], id);
        }
#endif //#if USE_QUADS
        else if ((this->dimension == 3) && (nvs == 4) && (!this->surface))
        {
            e = new MJMesh::Element3D(vs[0], vs[1], vs[2], vs[3], id);
        }
#if USE_BRICKS
        else if ((this->dimension == 3) && (nvs == 5) && (!this->surface))
        {
            e = new MJMesh::Element3D(vs[0], vs[1], vs[2], vs[3], vs[4], id);
        }
        else if ((this->dimension == 3) && (nvs == 6) && (!this->surface))
        {
            e = new MJMesh::Element3D(vs[0], vs[1], vs[2], vs[3], vs[4], vs[5], id);
        }
        else if ((this->dimension == 3) && (nvs == 8) && (!this->surface))
        {
            e = new MJMesh::Element3D(vs[0], vs[1], vs[2], vs[3], vs[4], vs[5], vs[6], vs[7], id);
        }
#endif //#if USE_BRICKS

        if (e)
        {
            for (UInt i = 0; i < e->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                for (Data::FrontElementList::iterator iter = v->feBegin();
                     iter != v->feEnd(); iter++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    if (fel->haveAdjacent(static_cast<MJMesh::Element *>(e)))
                    {
                        continue;
                    }

                    bool foundAll = true;

                    for (UInt j = 0; j < fel->numVertices(); j++)
                    {
                        if (!e->have(fel->getVertex(j)))
                        {
                            foundAll = false;
                        }
                    }

                    if (foundAll)
                    {
                        fel->add(static_cast<MJMesh::Element *>(e));
                    }
                }
            }

            els.push_back(e);
        }
    }

    this->boundary = (eCount == 0);

    if (!this->boundary)
    {
        this->mesh = this->makeMesh(false);

        while (!order.empty())
        {
            while (!order.front().empty())
            {
                Data::Vertex *v = order.front().front();

                order.front().pop_front();

                if (incrIdV)
                {
                    v->setId(v->getId() + 1);
                }

                static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
            }

            order.pop_front();
        }

        while (!fels.empty())
        {
            Data::FrontElement *fel = fels.front();

            fels.pop_front();

            if (incrIdFE)
            {
                fel->setId(fel->getId() + 1);
            }

            static_cast<MJMesh::Mesh *>(this->mesh)->add(fel);
        }

        while (!els.empty())
        {
            Data::Element *e = els.front();

            els.pop_front();

            if (incrIdE)
            {
                e->setId(e->getId() + 1);
            }

            static_cast<MJMesh::Mesh *>(this->mesh)->add(e);
        }

        ok = true;
    }
    else
    {
        if (this->boundaryBuilder)
        {
            ULInt feId = 1;

            for (VertexListList::iterator iter = order.begin();
                 iter != order.end(); iter++)
            {
                for (Data::VertexList::iterator iter2 = (*iter).begin();
                     iter2 != (*iter).end(); iter2++)
                {
                    Data::VertexList::iterator prev = iter2;

                    if (iter2 != (*iter).begin())
                    {
                        prev--;
                    }

                    if (this->dimension == 2)
                    {
                        this->boundaryBuilder->add((*iter2)->getId(), (*iter2)->getPoint()->getCoord(0), (*iter2)->getPoint()->getCoord(1));
                    }
                    else
                    {
                        this->boundaryBuilder->add((*iter2)->getId(), (*iter2)->getPoint()->getCoord(0), (*iter2)->getPoint()->getCoord(1), (*iter2)->getPoint()->getCoord(2));
                    }

                    if (feCount == 0)
                    {
                        if (iter2 != (*iter).begin())
                        {
                            this->boundaryBuilder->add(feId++, (*prev)->getId(), (*iter2)->getId());
                        }

                        if (iter2 == --(*iter).end())
                        {
                            this->boundaryBuilder->add(feId++, (*iter2)->getId(), (*iter).front()->getId());
                        }
                    }
                }
            }

            for (Data::FrontElementList::iterator iter = fels.begin();
                 iter != fels.end(); iter++)
            {
                if ((this->dimension == 2) && (!this->surface))
                {
                    this->boundaryBuilder->add((*iter)->getId(), (*iter)->getVertex(0)->getId(), (*iter)->getVertex(1)->getId());
                }
                else if ((this->dimension == 3) && (this->surface))
                {
                    this->boundaryBuilder->addSurface((*iter)->getId(), (*iter)->getVertex(0)->getId(), (*iter)->getVertex(1)->getId());
                }
                else if ((this->dimension == 3) && (!this->surface))
                {
    #if USE_QUADS
                    if ((*iter)->numVertices() == 3)
    #endif //#if USE_QUADS
                    {
                        this->boundaryBuilder->add((*iter)->getId(),
                            (*iter)->getVertex(0)->getId(), (*iter)->getVertex(1)->getId(), (*iter)->getVertex(2)->getId());
                    }
    #if USE_QUADS
                    else if ((*iter)->numVertices() == 4)
                    {
                        this->boundaryBuilder->add((*iter)->getId(),
                            (*iter)->getVertex(0)->getId(), (*iter)->getVertex(1)->getId(), (*iter)->getVertex(2)->getId(), (*iter)->getVertex(3)->getId());
                    }
    #endif //#if USE_QUADS
                }

                delete (*iter);
            }

            this->boundaryBuilder->incrementIds(incrIdV, incrIdFE, 1);
        }
        else
        {
            while (!fels.empty())
            {
                delete fels.front();

                fels.pop_front();
            }
        }

        while (!order.empty())
        {
            while (!order.front().empty())
            {
                delete order.front().front();

                order.front().pop_front();
            }

            order.pop_front();
        }

        if ((!this->boundaryBuilder) || (this->boundaryBuilder->getBoundary()->size() == 0))
        {
            ok = false;
        }
        else
        {
            ok = true;
        }
    }

    return ok;
}
