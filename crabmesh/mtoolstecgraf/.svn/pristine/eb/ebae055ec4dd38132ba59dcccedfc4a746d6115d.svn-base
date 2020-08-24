#include "IO/CDBFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::CDBFileReader::CDBFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->boundary = true;

	this->addFiletype(".cdb");
}

IO::CDBFileReader::~CDBFileReader()
{

}

void IO::CDBFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::CDBFileReader::isBoundary() const
{
	return this->boundary;
}

bool IO::CDBFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::CDBFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::CDBFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::CDBFileReader::read()
{
	bool ok = this->readCDB();

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

bool IO::CDBFileReader::readCDB()
{
	std::string str;

    do
    {
    	std::getline(this->file, str);

    	if ((str.size() > 6) && (str.substr(0, 6) == "NUMOFF"))
		{
			break;
		}
    }
    while (!this->file.eof());

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    ULInt lastVertId = 0, lastFEId = 0;

    while (true)
	{
		if (this->file.eof())
		{
			this->file.close();

			return false;
		}

		if (str.substr(7, 4) == "NODE")
		{
			std::stringstream stream;

			stream << str;

			std::string ignore;

			stream >> ignore;

			stream >> lastVertId;
		}
		else if (str.substr(7, 4) == "ELEM")
		{
			std::stringstream stream;

			stream << str;

			std::string ignore;

			stream >> ignore;

			stream >> lastFEId;
		}

		std::getline(this->file, str);

		if ((str.size() <= 6) ||
			(str.substr(0, 6) != "NUMOFF"))
		{
			break;
		}
	}

	if ((this->file.eof()) || (lastVertId == 0) || (lastFEId == 0))
	{
		this->file.close();

		return false;
	}

	//reading vertices
	do
    {
    	std::getline(this->file, str);

    	if ((str.size() > 6) && (str.substr(0, 6) == "NBLOCK"))
		{
			break;
		}
    }
    while (!this->file.eof());

    if (this->file.eof())
	{
		this->file.close();

		return false;
	}

	std::getline(this->file, str); //format

	UInt numExtras, sizeExtras, numCoords, sizeCoords, numPrecision;
	numExtras = sizeExtras = numCoords = sizeCoords = numPrecision = 0;
	std::string accum;

	for (UInt i = 0; i < str.size(); i++)
	{
		if (str[i] == '(')
		{
			continue;
		}

		if (str[i] == 'i')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> numExtras;

				accum.clear();
			}

			continue;
		}

		if (str[i] == ',')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> sizeExtras;

				accum.clear();
			}

			continue;
		}

		if (str[i] == 'e')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> numCoords;

				accum.clear();
			}

			continue;
		}

		if (str[i] == '.')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> sizeCoords;

				accum.clear();
			}

			continue;
		}

		if (str[i] == ')')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> numPrecision;

				accum.clear();
			}

			break;
		}

		accum.append(1, str[i]);
	}

	ULInt id = 0;

#if USE_C__11
	typedef std::unordered_map<ULInt, Data::R3Tuple> ULIntR3TupleHashMap;
#else
	typedef std::map<ULInt, Data::R3Tuple> ULIntR3TupleMap;
#endif //#if USE_C__11

#if USE_C__11
	ULIntR3TupleHashMap mapInput;

	mapInput.reserve(static_cast<size_t>(lastVertId));
#else
	ULIntR3TupleMap mapInput;
#endif //#if USE_C__11

	do
	{
		std::getline(this->file, str);

		UInt i = 0;

		for (; i < numExtras*sizeExtras; i += sizeExtras)
		{
			if (i >= str.size())
			{
				break;
			}

			if (i == 0)
			{
				std::string s = str.substr(i, sizeExtras);

				std::stringstream stream;

				stream << s;

				stream >> id;
			}
		}

		Real coords[3] = {0.0, 0.0, 0.0};
		UInt use = 0;

		for (; i < numExtras*sizeExtras + numCoords*sizeCoords; i += sizeCoords)
		{
			if (i >= str.size())
			{
				break;
			}

			std::string s = str.substr(i, sizeCoords);

			std::stringstream stream;

			stream << s;

			stream >> coords[use];

			use++;

			if (use >= 3)
			{
				break;
			}
		}

#if USE_C__11
		mapInput[id] = std::make_tuple(coords[0], coords[1], coords[2]);
#else
		Data::R3Tuple t;
		t.values[0] = coords[0];
		t.values[1] = coords[1];
		t.values[2] = coords[2];
		mapInput[id] = t;
#endif //#if USE_C__11
	}
	while ((!this->file.eof()) && (id != lastVertId));

	if (this->file.eof())
	{
		this->file.close();

		return false;
	}

	//reading front elements
	do
    {
    	std::getline(this->file, str);

    	if ((str.size() > 6) && (str.substr(0, 6) == "EBLOCK"))
		{
			break;
		}
    }
    while (!this->file.eof());

    if (this->file.eof())
	{
		this->file.close();

		return false;
	}

	std::getline(this->file, str); //format

	numExtras = sizeExtras = 0;

	for (UInt i = 0; i < str.size(); i++)
	{
		if (str[i] == '(')
		{
			continue;
		}

		if (str[i] == 'i')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> numExtras;

				accum.clear();
			}

			continue;
		}

		if (str[i] == ')')
		{
			if (!accum.empty())
			{
				std::stringstream stream;

				stream << accum;

				stream >> sizeExtras;

				accum.clear();
			}

			break;
		}

		accum.append(1, str[i]);
	}

	id = 0;

	ULInt verts[numExtras - 11];

	bool first = true, all = false;
	bool addVertices = true;

#if USE_C__11
	Data::VertexHashMap mapVertices;
#else
	Data::VertexMap mapVertices;
#endif //#if USE_C__11

	do
	{
		std::getline(this->file, str);

		for (UInt i = 0; i < numExtras - 11; i++)
		{
			verts[i] = 0;
		}

		UInt use = 0;
		UInt i = 0;

		for (i = 0; i < numExtras*sizeExtras; i += sizeExtras)
		{
			if (i + sizeExtras > str.size())
			{
				break;
			}

			if (i == 10*sizeExtras)
			{
				std::string s = str.substr(i, sizeExtras);

				std::stringstream stream;

				stream << s;

				stream >> id;
			}
			else if (i > 10*sizeExtras)
			{
				std::string s = str.substr(i, sizeExtras);

				std::stringstream stream;

				stream << s;

				stream >> verts[use];

				use++;
			}
		}

		if (first)
		{
			if (i == numExtras*sizeExtras)
			{
				all = true;
			}

			first = false;
		}

		if ((!all) || ((all) && (i == numExtras*sizeExtras)))
		{
			if (use == 3)
			{
				if (addVertices)
				{
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

						this->boundaryBuilder->add(id, x, y, z);
					}

					addVertices = false;
				}

				this->boundaryBuilder->add(id, verts[0], verts[1], verts[2]);
			}
			else
			{
				if (this->boundary)
				{
					this->boundary = false;

					this->mesh = this->makeMesh(false);

#if USE_C__11
					mapVertices.reserve(static_cast<size_t>(lastVertId));

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

						MJMesh::Vertex *v = new MJMesh::Vertex(x, y, z, id);

						mapVertices[id] = v;

						static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
					}

					addVertices = false;
				}

				MJMesh::Element *el = NULL;

				MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[verts[0]]);
				MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[verts[1]]);
				MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[verts[2]]);
				MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(mapVertices[verts[3]]);

				if (use == 4)
				{
					el = new MJMesh::Element3D(v1, v2, v3, v4, id);
				}
#if USE_BRICKS
				else if (use == 8)
				{
					MJMesh::Vertex *v5 = static_cast<MJMesh::Vertex *>(mapVertices[verts[4]]);
					MJMesh::Vertex *v6 = static_cast<MJMesh::Vertex *>(mapVertices[verts[5]]);
					MJMesh::Vertex *v7 = static_cast<MJMesh::Vertex *>(mapVertices[verts[6]]);
					MJMesh::Vertex *v8 = static_cast<MJMesh::Vertex *>(mapVertices[verts[7]]);

					el = new MJMesh::Element3D(v1, v2, v3, v4, v5, v6, v7, v8, id);
				}
#else
				this->file.close();

				return false;
#endif //#if USE_BRICKS

				if (el)
				{
					static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
				}
			}
		}
		else
		{
			id = 0;
		}
	}
	while ((!this->file.eof()) && (id != lastFEId));

	if (this->file.eof())
	{
		this->file.close();

		return false;
	}

	this->file.close();

	return true;
}
