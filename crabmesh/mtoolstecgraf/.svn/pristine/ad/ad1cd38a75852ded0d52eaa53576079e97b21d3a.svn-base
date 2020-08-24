#include "IO/NeutralFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::NeutralFileReader::NeutralFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->dimension = 0;
	this->boundary = false;

	this->addFiletype(".dat");
	this->addFiletype(".pos");
}

IO::NeutralFileReader::~NeutralFileReader()
{

}

void IO::NeutralFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

UInt IO::NeutralFileReader::getDimension() const
{
	return this->dimension;
}

bool IO::NeutralFileReader::isBoundary() const
{
	return this->boundary;
}

bool IO::NeutralFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::NeutralFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::NeutralFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::NeutralFileReader::read()
{
	bool ok = this->readDat();

    if (!ok)
	{
		this->boundaryBuilder->clear();

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		this->dimension = 0;
	}
}

bool IO::NeutralFileReader::readDat()
{
	this->dimension = 2;

    std::string str;

    do
    {
    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "%NODE.COORD"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    ULInt verticesCount;

	this->file >> verticesCount;

#if USE_C__11
	typedef std::unordered_map<ULInt, Data::R3Tuple> ULIntR3TupleHashMap;
#else
	typedef std::map<ULInt, Data::R3Tuple> ULIntR3TupleMap;
#endif //#if USE_C__11

#if USE_C__11
	ULIntR3TupleHashMap mapInput;
#else
	ULIntR3TupleMap mapInput;
#endif //#if USE_C__11

	for (ULInt i = 0; i < verticesCount; i++)
	{
		ULInt id;
		Real x, y, z;

		this->file >> id;
		this->file >> x;
		this->file >> y;
		this->file >> z;

		if (std::fabs(z) > Data::getTolerance())
		{
			this->dimension = 3;
		}

#if USE_C__11
		mapInput[id] = std::make_tuple(x, y, z);
#else
		Data::R3Tuple t;
		t.values[0] = x;
		t.values[1] = y;
		t.values[2] = z;
		mapInput[id] = t;
#endif //#if USE_C__11
	}

	do
    {
    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "%ELEMENT.T3") && (str != "%ELEMENT.Q4") && (str != "%ELEMENT.TETR4") && (str != "%ELEMENT.BRICK8"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    /*if (str.substr(9) == "T3")
    {
    	this->dimension = 2;
    }
    else if (str.substr(9) == "TETR4")
    {
    	this->dimension = 3;
    }*/

    bool brick = (str.substr(9) == "BRICK8");
    bool quad = (str.substr(9) == "Q4");

#if (!USE_BRICKS)
	if (brick)
	{
		this->file.close();

		return false;
	}
#endif //#if (!USE_BRICKS)

#if (!USE_QUADS)
	if (quad)
	{
		this->file.close();

		return false;
	}
#endif //#if (!USE_BRICKS)

    ULInt elementsCount;

    this->file >> elementsCount;

	if (this->dimension == 2)
	{
#if USE_C__11
		Data::VertexHashMap mapVertices;
#else
		Data::VertexMap mapVertices;
#endif //#if USE_C__11

		this->mesh = this->makeMesh(false);

		this->boundary = false;

#if USE_C__11
		for (ULIntR3TupleHashMap::iterator iter = mapInput.begin();
#else
		for (ULIntR3TupleMap::iterator iter = mapInput.begin();
#endif //#if USE_C__11
			 iter != mapInput.end(); iter++)
		{
			ULInt id = (*iter).first;
#if USE_C__11
			Real x = std::get<0>((*iter).second);
			Real y = std::get<1>((*iter).second);
#else
			Real x = (*iter).second.values[0];
			Real y = (*iter).second.values[1];
#endif //#if USE_C__11

			MJMesh::Vertex *v = new MJMesh::Vertex(x, y, id);

    		mapVertices[id] = v;

    		static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
		}

		for (ULInt i = 0; i < elementsCount; i++)
		{
			ULInt id, id1, id2, id3, ignore;

			this->file >> id;
			this->file >> ignore;
			this->file >> ignore;
			this->file >> ignore;
			this->file >> id1;
			this->file >> id2;
			this->file >> id3;

			MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[id1]);
			MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[id2]);
			MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[id3]);

			/*Data::Triangle2D *t = new Data::Triangle2D((Data::Point2D *)v1->getPoint(),
				(Data::Point2D *)v2->getPoint(), (Data::Point2D *)v3->getPoint());

			MJMesh::Element2D *el = new MJMesh::Element2D(t);*/

			MJMesh::Element2D *el = new MJMesh::Element2D(v1, v2, v3, id);

			static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
		}
	}
	else if (this->dimension == 3)
	{
#if USE_C__11
		Data::VertexHashMap mapVertices;
#else
		Data::VertexMap mapVertices;
#endif //#if USE_C__11

		this->boundary = (str.substr(9) == "T3");

		this->mesh = this->makeMesh(this->boundary);

#if USE_C__11
		for (ULIntR3TupleHashMap::iterator iter = mapInput.begin();
#else
		for (ULIntR3TupleMap::iterator iter = mapInput.begin();
#endif //#if USE_C__11
			 iter != mapInput.end(); iter++)
		{
			ULInt id = (*iter).first;
#if USE_C__11
			Real x = std::get<0>((*iter).second);
			Real y = std::get<1>((*iter).second);
			Real z = std::get<2>((*iter).second);
#else
			Real x = (*iter).second.values[0];
			Real y = (*iter).second.values[1];
			Real z = (*iter).second.values[2];
#endif //#if USE_C__11

			if (this->boundary)
			{
				this->boundaryBuilder->add(id, x, y, z);
			}
			else
			{
				MJMesh::Vertex *v = new MJMesh::Vertex(y, z, x, id);

				static_cast<MJMesh::Mesh *>(this->mesh)->add(v);

				mapVertices[id] = v;
			}
		}

		if (this->boundary)
		{
			for (ULInt i = 0; i < elementsCount; i++)
			{
#if USE_QUADS
				ULInt id, id1, id2, id3, id4, ignore;
#else
				ULInt id, id1, id2, id3, ignore;
#endif //#if USE_QUADS

				this->file >> id;
				this->file >> ignore;
				this->file >> ignore;
				this->file >> ignore;
				this->file >> id1;
				this->file >> id2;
				this->file >> id3;

#if USE_QUADS
				if (quad)
				{
					this->file >> id4;
				}

				if (quad)
				{
					this->boundaryBuilder->add(id, id1, id2, id3, id4);
				}
				else
#endif //#if USE_QUADS
				{
					this->boundaryBuilder->add(id, id1, id2, id3);
				}
			}
		}
		else
		{
			for (ULInt i = 0; i < elementsCount; i++)
			{
#if USE_BRICKS
				ULInt id, id1, id2, id3, id4, id5, id6, id7, id8, ignore;
#else
				ULInt id, id1, id2, id3, id4, ignore;
#endif //#if USE_BRICKS

				this->file >> id;
				this->file >> ignore;
				this->file >> ignore;
				this->file >> id1;
				this->file >> id2;
				this->file >> id3;
				this->file >> id4;

#if USE_BRICKS
				if (brick)
				{
					this->file >> id5;
					this->file >> id6;
					this->file >> id7;
					this->file >> id8;
				}
#endif //#if USE_BRICKS

				MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[id1]);
				MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[id2]);
				MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[id3]);
				MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(mapVertices[id4]);
#if USE_BRICKS
				MJMesh::Vertex *v5 = (brick) ? static_cast<MJMesh::Vertex *>(mapVertices[id5]) : NULL;
				MJMesh::Vertex *v6 = (brick) ? static_cast<MJMesh::Vertex *>(mapVertices[id6]) : NULL;
				MJMesh::Vertex *v7 = (brick) ? static_cast<MJMesh::Vertex *>(mapVertices[id7]) : NULL;
				MJMesh::Vertex *v8 = (brick) ? static_cast<MJMesh::Vertex *>(mapVertices[id8]) : NULL;
#endif //#if USE_BRICKS

				MJMesh::Element3D *el =
#if USE_BRICKS
					brick ? new MJMesh::Element3D(v1, v2, v3, v4, v5, v6, v7, v8, id) :
#endif //#if USE_BRICKS
					new MJMesh::Element3D(v1, v2, v3, v4, id);

				static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
			}
		}
	}

	return true;
}
