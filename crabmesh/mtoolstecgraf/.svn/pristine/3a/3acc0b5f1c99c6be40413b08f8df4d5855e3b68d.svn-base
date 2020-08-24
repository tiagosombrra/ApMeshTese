#include "IO/TextMeshFileReader.h"

#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Element2D.h"

using namespace IO;

IO::TextMeshFileReader::TextMeshFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->dimension = 0;

	this->addFiletype(".txt");
}

IO::TextMeshFileReader::~TextMeshFileReader()
{

}

UInt IO::TextMeshFileReader::getDimension() const
{
	return this->dimension;
}

Data::AbstractMesh *IO::TextMeshFileReader::makeMesh() const
{
	return new MJMesh::Mesh();
}

void IO::TextMeshFileReader::read()
{
	bool ok = this->readText();

    if (!ok)
	{
		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		this->dimension = 0;
	}
}

bool IO::TextMeshFileReader::readText()
{
	bool ok = false;

    this->file >> this->dimension;

    if (this->dimension == 2)
    {
    	ULInt verticesCount;

    	this->file >> verticesCount;

    	Data::VertexMap map;

    	for (ULInt i = 0; i < verticesCount; i++)
    	{
    		ULInt id;
    		Real x, y;

    		this->file >> id;
    		this->file >> x;
    		this->file >> y;

			MJMesh::Vertex *v = new MJMesh::Vertex(x, y, id);

    		map[id] = v;

    		static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
    	}

    	ULInt elementsCount;

    	this->file >> elementsCount;

    	for (ULInt i = 0; i < elementsCount; i++)
    	{
    		ULInt id, id1, id2, id3;

    		this->file >> id;
    		this->file >> id1;
    		this->file >> id2;
    		this->file >> id3;

    		MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(map[id1]);
    		MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(map[id2]);
    		MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(map[id3]);

    		/*Data::Triangle2D *t = new Data::Triangle2D((Data::Point2D *)v1->getPoint(),
				(Data::Point2D *)v2->getPoint(), (Data::Point2D *)v3->getPoint(), id);

			MJMesh::Element2D *el = new MJMesh::Element2D(t);*/

			MJMesh::Element2D *el = new MJMesh::Element2D(v1, v2, v3, id);

    		static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
    	}

    	ok = true;
    }
    else if (this->dimension == 3)
    {
		ULInt verticesCount;

    	this->file >> verticesCount;

    	Data::VertexMap map;

    	for (ULInt i = 0; i < verticesCount; i++)
    	{
    		ULInt id;
    		Real x, y, z;

    		this->file >> id;
    		this->file >> x;
    		this->file >> y;
    		this->file >> z;

			MJMesh::Vertex *v = new MJMesh::Vertex(x, y, z, id);

    		map[id] = v;

    		static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
    	}

    	ULInt elementsCount;

    	this->file >> elementsCount;

    	for (ULInt i = 0; i < elementsCount; i++)
    	{
    		ULInt id, id1, id2, id3, id4;

    		this->file >> id;
    		this->file >> id1;
    		this->file >> id2;
    		this->file >> id3;
    		this->file >> id4;

    		MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(map[id1]);
    		MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(map[id2]);
    		MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(map[id3]);
    		MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(map[id4]);

    		/*Data::Triangle2D *t = new Data::Triangle2D((Data::Point2D *)v1->getPoint(),
				(Data::Point2D *)v2->getPoint(), (Data::Point2D *)v3->getPoint(), id);

			MJMesh::Element2D *el = new MJMesh::Element2D(t);*/

			MJMesh::Element3D *el = new MJMesh::Element3D(v1, v2, v3, v4, id);

    		static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
    	}

    	ok = true;
    }

    return ok;
}
