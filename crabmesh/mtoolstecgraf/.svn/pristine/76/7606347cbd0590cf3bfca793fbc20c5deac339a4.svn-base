#include "IO/GenFileWriter.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"

IO::GenFileWriter::GenFileWriter() :
	IO::MeshFileWriter::MeshFileWriter()
{
	this->type = BOUNDARY;
	this->dimension = 2;

	this->addFiletype(".gen");
}

IO::GenFileWriter::~GenFileWriter()
{

}

void IO::GenFileWriter::setType(enum Type type)
{
	this->type = type;
}

bool IO::GenFileWriter::write(const std::string &filename, const Data::Boundary *boundary, const Data::Mesh *mesh)
{
    size_t pos = filename.find_last_of('.');

	if ((pos == 0) || (pos == std::string::npos))
	{
		return false;
	}

	std::string extension = filename.substr(pos);

	if (extension.size() == 0)
	{
		return false;
	}

	std::string actualFilename = filename.substr(0, pos);

	size_t pos2 = actualFilename.find_last_of('.');

	std::string ext;

	if (pos != std::string::npos)
    {
        ext = actualFilename.substr(pos2);
    }

    if (ext != ".gen")
    {
        std::stringstream str;

        str << actualFilename << ".gen";

        actualFilename = str.str();
    }

    const Data::AbstractMesh *m = NULL;
    enum Type type;

    if ((extension == ".bbgen") || (extension == ".bgen"))
    {
        type = BOUNDARY;
        m = boundary;
    }
    else if (extension == ".bvgen")
    {
        type = VOLUME;
        m = boundary;
    }
    else if (extension == ".bsgen")
    {
        type = SURFACE;
        m = boundary;
    }
    else if (extension == ".mbgen")
    {
        type = BOUNDARY;
        m = mesh;
    }
    else if ((extension == ".mvgen") || (extension == ".mgen"))
    {
        type = VOLUME;
        m = mesh;
    }
    else if (extension == ".msgen")
    {
        type = SURFACE;
        m = mesh;
    }
    else
    {
        return false;
    }

    if (!m)
    {
        return false;
    }

    IO::GenFileWriter writer;

    writer.setFilename(actualFilename);
    writer.setType(type);
    writer.setMesh(m);

    if (!writer.areStructuresCorrect())
    {
        return false;
    }

    writer.execute();

    return true;
}

bool IO::GenFileWriter::areStructuresCorrect() const
{
    if (!this->mesh)
    {
        return false;
    }

    if (((dynamic_cast<const Data::Boundary *>(this->mesh)) &&
         (static_cast<const Data::Boundary *>(this->mesh)->dimension() == 2) &&
         (this->type == VOLUME)) ||
        ((dynamic_cast<const Data::Mesh *>(this->mesh)) &&
         (static_cast<const Data::Mesh *>(this->mesh)->dimension() == 3) &&
         (static_cast<const MJMesh::Mesh *>(this->mesh)->isSurface()) &&
         (this->type == VOLUME)))
    {
        return false;
    }

    return true;
}

void IO::GenFileWriter::write() const
{
    this->writeGen();
}

void IO::GenFileWriter::write(Data::Vertex *v) const
{
    Data::Point *p = v->getPoint();

    this->file << v->getId();

    UInt dim = std::min(p->dimension(), this->dimension);

    for (UInt i = 0; i < dim; i++)
    {
        this->file << " " << p->getCoord(i);
    }

    for (UInt i = dim; i < this->dimension; i++)
    {
        this->file << " " << 0.0;
    }

    this->file << std::endl;
}

void IO::GenFileWriter::write(Data::Element *e) const
{
    this->file << e->getId();

    this->file << " " << e->numVertices();

    for (UInt i = 0; i < e->numVertices(); i++)
    {
        this->file << " " << e->getVertex(i)->getId();
    }

    this->file << std::endl;
}

void IO::GenFileWriter::writeGen() const
{
    const MJMesh::Boundary *boundary = dynamic_cast<const MJMesh::Boundary *>(this->mesh);
    const MJMesh::Mesh *mesh = dynamic_cast<const MJMesh::Mesh *>(this->mesh);

    this->dimension = (this->type == SURFACE) ? 3 :
        ((boundary) ? boundary->dimension() : mesh->dimension());

    bool surface = (mesh) && (mesh->isSurface());
    bool boundarySurface = (boundary) && (boundary->isSurfaceBoundary());

    if ((this->type == SURFACE) ||
        ((this->type == BOUNDARY) && (boundarySurface)))
    {
        this->file << "2.5" << std::endl;
    }
    else
    {
        this->file << this->dimension << std::endl;
    }

    if (boundary)
    {
        this->file << boundary->verticesSize() << std::endl;
    }
    else
    {
        this->file << mesh->verticesSize() << std::endl;
    }

    if (boundary)
    {
        for (Data::Boundary::ConstVertexIterator iter = boundary->vBegin();
             iter != boundary->vEnd(); iter++)
        {
            this->write((*iter));
        }
    }
    else
    {
        for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin();
             iter != mesh->vEnd(); iter++)
        {
            this->write((*iter));
        }
    }

    if (boundary)
    {
        if ((boundary->dimension() == 2) || (this->type == BOUNDARY) || (boundarySurface))
        {
            this->file << boundary->size() << std::endl;

            for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
                 iter != boundary->eEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else
        {
            this->file << 0 << std::endl;
        }
    }
    else
    {
        if ((mesh->dimension() == 2) ||
            ((surface) && (this->type == SURFACE)) ||
            ((!surface) && (this->type != SURFACE)))
        {
            this->file << mesh->frontElementsSize() << std::endl;

            for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                 iter != mesh->feEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else if ((surface) && (this->type == BOUNDARY))
        {
            this->file << mesh->size() << std::endl;

            for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                 iter != mesh->eEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else
        {
            this->file << 0 << std::endl;
        }
    }

    if (boundary)
    {
        if ((boundary->dimension() == 3) && (this->type != BOUNDARY) && (!boundarySurface))
        {
            this->file << boundary->size() << std::endl;

            for (Data::Boundary::ConstFrontElementIterator iter = boundary->eBegin();
                 iter != boundary->eEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else
        {
            this->file << 0 << std::endl;
        }
    }
    else
    {
        if (((mesh->dimension() == 2) && (this->type != BOUNDARY)) ||
            ((surface) && (this->type == SURFACE)) ||
            ((!surface) && (this->type == VOLUME)))
        {
            this->file << mesh->size() << std::endl;

            for (Data::Mesh::ConstElementIterator iter = mesh->eBegin();
                 iter != mesh->eEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else if ((mesh->dimension() == 3) && (!surface) && (this->type == SURFACE))
        {
            this->file << mesh->frontElementsSize() << std::endl;

            for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin();
                 iter != mesh->feEnd(); iter++)
            {
                this->write((*iter));
            }
        }
        else
        {
            this->file << 0 << std::endl;
        }
    }
}
