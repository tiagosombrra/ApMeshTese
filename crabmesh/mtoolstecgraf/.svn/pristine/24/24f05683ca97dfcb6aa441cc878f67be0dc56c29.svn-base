#include "MJMesh/Element.h"

#include "Data/OStream.h"
#include "MJMesh/Vertex.h"

MJMesh::Element::Element(Data::GeometricShape *shape, ULInt id) :
	Data::Element(shape, id)
{

}

MJMesh::Element::~Element()
{
	/*if (this->getId() != 0)
	{
		std::stringstream str;
		str << "deleting element " << this->getId() << " at " << this;
		str << " vertices are at ";
		//for (UInt i = 0; i < this->numVertices(); i++) str << " " << this->vertices[i]->getId();
		for (UInt i = 0; i < this->numVertices(); i++) str << " " << this->vertices[i];
		//str << std::endl;
		//str << Data::trace();
		Data::cout() << str.str() << Data::endl;
	}*/

	/*while (!this->vertices.empty())
	{
		MJMesh::Vertex *v = (MJMesh::Vertex *)this->vertices.back();
		this->vertices.pop_back();

		v->remove(this);
	}*/

	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

#if USE_ARRAY
#if USE_EXPERIMENTAL
	if (this->vertices)
#endif //#if USE_EXPERIMENTAL
	{
		this->numVerts = 0;

		delete [] this->vertices;

		this->vertices = NULL;
	}
#endif //#if USE_ARRAY

	/*//debug
	if (!this->data.meshes.empty())
	{
		std::cout << "Element " << this->getId() << " still in mesh" << std::endl;
		std::cout << Data::trace() << std::endl;
	}
	//endebug*/
}

void MJMesh::Element::remove(MJMesh::Vertex *v)
{
#if USE_ARRAY
	if (!this->vertices)
	{
		return;
	}

	UInt found = this->numVerts;

	for (UInt i = 0; i < this->numVerts; i++)
	{
		if (this->vertices[i] == v)
		{
			found = i;

			break;
		}
	}

	if (found < this->numVerts)
	{
		Data::Vertex **vertices = this->vertices;

		this->vertices = new Data::Vertex*[this->numVerts - 1];

		for (UInt i = 0; i < found; i++)
		{
			this->vertices[i] = vertices[i];
		}

		for (UInt i = found; i < this->numVerts - 1; i++)
		{
			this->vertices[i] = vertices[i + 1];
		}

		delete [] vertices;

		this->numVerts--;
	}
#else
	if (this->vertices.empty())
	{
		return;
	}

	VertexVector::iterator it = std::find(this->vertices.begin(), this->vertices.end(), v);

	if (it != this->vertices.end())
	{
		this->vertices.erase(it);
	}
#endif //#if USE_ARRAY
}

bool MJMesh::Element::gammaGreater(Data::Element *e1, Data::Element *e2)
{
	return static_cast<MJMesh::Element *>(e1)->gamma() > static_cast<MJMesh::Element *>(e2)->gamma();
}

MJMesh::ElementSpecificData &MJMesh::Element::getData() const
{
	return this->data;
}

void MJMesh::Element::addMeshData(const MJMesh::Mesh *mesh, Data::Mesh::ElementIterator &meshIt) const
{
	this->data.meshes[mesh] = meshIt;
}

void MJMesh::Element::removeMeshData(const MJMesh::Mesh *mesh) const
{
	this->data.meshes.erase(mesh);
}

bool MJMesh::Element::isInMesh(const MJMesh::Mesh *mesh) const
{
	return (this->data.meshes.find(mesh) != this->data.meshes.end());
}

void MJMesh::Element::setMeshData(const MJMesh::Mesh *mesh, Data::Mesh::ElementIterator &meshIt) const
{
	(*this->data.meshes.find(mesh)).second = meshIt;
}

const Data::Mesh::ElementIterator &MJMesh::Element::getMeshIterator(const MJMesh::Mesh *mesh) const
{
	return (*this->data.meshes.find(mesh)).second;
}

Data::Mesh::ElementIterator MJMesh::Element::getAndRemoveMeshIterator(const MJMesh::Mesh *mesh) const
{
	ElementSpecificData::MeshDataMap::iterator iter = this->data.meshes.find(mesh);

	Data::Mesh::ElementIterator it = (*iter).second;

	this->data.meshes.erase(iter);

	return it;
}

void MJMesh::Element::swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	ElementSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);
	ElementSpecificData::MeshDataMap::iterator oit = this->data.meshes.find(other);

	Data::Mesh::ElementIterator tmp = (*mit).second;
	(*mit).second = (*oit).second;
	(*oit).second = tmp;
}

void MJMesh::Element::replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	ElementSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);

	this->data.meshes[other] = (*mit).second;

	this->data.meshes.erase(mit);
}

std::string MJMesh::Element::text(const Data::AbstractMesh *mesh) const
{
	std::stringstream s;

	s << Data::Element::text(mesh);

	s << "; " << this->quality();

	return s.str();
}
