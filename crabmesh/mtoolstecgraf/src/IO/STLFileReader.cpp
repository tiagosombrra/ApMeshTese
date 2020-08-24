#include "IO/STLFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::STLFileReader::STLFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->addFiletype(".stl");
}

IO::STLFileReader::~STLFileReader()
{

}

void IO::STLFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::STLFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::STLFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::STLFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::STLFileReader::read()
{
	bool ok = this->readAsciiStl();

	if (!ok)
    {
        ok = this->readBinaryStl();
    }

    if (!ok)
	{
		this->boundaryBuilder->clear();

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}
	}
}

bool IO::STLFileReader::readAsciiStl()
{
	std::string str;

    do
    {
    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "facet"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    //this->file >> str;

#if USE_C__11
	typedef std::unordered_map<Real, std::unordered_map<Real, std::unordered_map<Real, ULInt> > > R3ULIntHashMap;
#else
    typedef std::map<Real, std::map<Real, std::map<Real, ULInt> > > R3ULIntMap;
#endif //#if USE_C__11

#if USE_C__11
	R3ULIntHashMap vMap;
#else
	R3ULIntMap vMap;
#endif //#if USE_C__11

    ULInt idV = 0, idFel = 0;

    do
    {
    	//this->file >> str;

    	if (str == "facet")
    	{
    		do
    		{
    			this->file >> str;
    		}
    		while ((!this->file.eof()) && (str != "vertex"));

			if (this->file.eof())
			{
				this->file.close();

				return false;
			}

    		ULInt vs[3] = {0, 0, 0};

    		UInt i = 0;

    		do
    		{
    			Real x, y, z;

    			this->file >> x;
    			this->file >> y;
    			this->file >> z;

    			ULInt v = vMap[x][y][z];

    			if (!v)
    			{
    				this->boundaryBuilder->add(++idV, x, y, z);

    				vMap[x][y][z] = v = idV;
    			}

    			if (i < 3)
    			{
					vs[i++] = v;
    			}

    			this->file >> str;
    		}
    		while ((!this->file.eof()) && (str != "endloop"));

			if (this->file.eof())
			{
				this->file.close();

				return false;
			}

    		this->boundaryBuilder->add(++idFel, vs[0], vs[1], vs[2]);
    	}

    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "endsolid"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

	return true;
}

bool IO::STLFileReader::readBinaryStl()
{
    if (this->file.is_open())
    {
        this->file.close();
    }

    this->file.open(this->filename.c_str(), std::ios_base::in | std::ios_base::binary);

    if (!this->file.is_open())
    {
        return false;
    }

    char header[80];

	this->file.read(header, 80);

	int numTriangles = this->binaryRead<int, 4>();

#if USE_C__11
	typedef std::unordered_map<Real, std::unordered_map<Real, std::unordered_map<Real, ULInt> > > R3ULIntHashMap;
#else
    typedef std::map<Real, std::map<Real, std::map<Real, ULInt> > > R3ULIntMap;
#endif //#if USE_C__11

#if USE_C__11
	R3ULIntHashMap vMap;
#else
	R3ULIntMap vMap;
#endif //#if USE_C__11

	ULInt idV = 0, idFel = 0;

	for (int i = 0; i < numTriangles; i++)
	{
		float vals[4][3];

		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				vals[j][k] = this->binaryRead<float, 4>();
			}
		}

		this->binaryRead<short, 2>();

		ULInt vs[3];

		for (UInt i = 0; i < 3; i++)
        {
            Real x = vals[i + 1][0];
            Real y = vals[i + 1][1];
            Real z = vals[i + 1][2];

            vs[i] = vMap[x][y][z];

            if (!vs[i])
            {
                this->boundaryBuilder->add(++idV, x, y, z);

                vMap[x][y][z] = vs[i] = idV;
            }
        }

        this->boundaryBuilder->add(++idFel, vs[0], vs[1], vs[2]);
	}

	this->file.close();

    return true;
}
