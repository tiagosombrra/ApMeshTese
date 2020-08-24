#include "IO/GmshMshFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Mesh.h"
#include "Data/Point3D.h"

using namespace IO;

IO::GmshMshFileReader::GmshMshFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->dimension = 0;

	this->addFiletype(".msh");
}

IO::GmshMshFileReader::~GmshMshFileReader()
{

}

void IO::GmshMshFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

UInt IO::GmshMshFileReader::getDimension() const
{
	return this->dimension;
}

bool IO::GmshMshFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::GmshMshFileReader::makeMesh() const
{
	return NULL;
}

void IO::GmshMshFileReader::read()
{
	bool ok = this->readMsh();

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

bool IO::GmshMshFileReader::readMsh()
{
    std::string label, endlabel;

#if USE_EXPERIMENTAL_3
    Data::Point3DObjList pts;
#else
    Data::PointObjList pts;
#endif //#if USE_EXPERIMENTAL_3
    Data::ULIntList ids;

    while (!this->file.eof())
    {
        std::string line;

        std::getline(this->file, line);

        if (line.empty())
        {
            break;
        }

        if (label.empty())
        {
            label = line;

            std::stringstream str;

            str << label[0];

            str << "End";

            str << label.substr(1);

            endlabel = str.str();
        }

        while (line != endlabel)
        {
            if (this->file.eof())
            {
                return false;
            }

            std::getline(this->file, line);

            if (label == "$Nodes")
            {
                std::stringstream str;

                str << line;

                UInt numEntities = 0;

                str >> numEntities;

                for (UInt i = 0; i < numEntities; i++)
                {
                    if (this->file.eof())
                    {
                        return false;
                    }

                    std::getline(this->file, line);

                    std::stringstream str;

                    str << line;

                    ULInt id;
                    Real x, y, z;

                    str >> id;
                    str >> x;
                    str >> y;
                    str >> z;

                    ids.push_back(id);

                    Data::Point3D pt;

                    pt.setCoord(0, x);
                    pt.setCoord(1, y);
                    pt.setCoord(2, z);

                    pts.push_back(pt);

                    if (((x > 0.0) || (x < 0.0)) &&
                        (this->dimension < 1))
                    {
                        this->dimension = 1;
                    }

                    if (((y > 0.0) || (y < 0.0)) &&
                        (this->dimension < 2))
                    {
                        this->dimension = 2;
                    }

                    if (((z > 0.0) || (z < 0.0)) &&
                        (this->dimension < 3))
                    {
                        this->dimension = 3;
                    }
                }

                if (this->file.eof())
                {
                    return false;
                }

                std::getline(this->file, line);

                for (UInt i = 0; i < numEntities; i++)
                {
                    ULInt id = ids.front();
                    ids.pop_front();

                    Data::Point3D pt = pts.front();
                    pts.pop_front();

                    if (this->dimension == 2)
                    {
                        this->boundaryBuilder->add(id, pt.getCoord(0), pt.getCoord(1));
                    }
                    else
                    {
                        this->boundaryBuilder->add(id, pt.getCoord(0), pt.getCoord(1), pt.getCoord(2));
                    }
                }
            }
            else if (label == "$Elements")
            {
                std::stringstream str;

                str << line;

                UInt numEntities = 0;

                str >> numEntities;

                for (UInt i = 0; i < numEntities; i++)
                {
                    if (this->file.eof())
                    {
                        return false;
                    }

                    std::getline(this->file, line);

                    std::stringstream str;

                    str << line;

                    ULInt id;
                    UInt type;
                    UInt numTags;

                    str >> id;
                    str >> type;
                    str >> numTags;

                    for (UInt i = 0; i < numTags; i++)
                    {
                        UInt ignore;

                        str >> ignore;
                    }

                    if ((type == 1) && (this->dimension == 2))
                    {
                        ULInt ids[2];

                        str >> ids[0];
                        str >> ids[1];

                        this->boundaryBuilder->add(id, ids[0], ids[1]);
                    }
                    else if (type == 2)
                    {
                        ULInt ids[3];

                        str >> ids[0];
                        str >> ids[1];
                        str >> ids[2];

                        this->boundaryBuilder->add(id, ids[0], ids[1], ids[2]);
                    }
#if USE_QUADS
                    else if (type == 3)
                    {
                        ULInt ids[4];

                        str >> ids[0];
                        str >> ids[1];
                        str >> ids[2];
                        str >> ids[3];

                        this->boundaryBuilder->add(id, ids[0], ids[1], ids[2], ids[3]);
                    }
#endif //#if USE_QUADS
                }

                if (this->file.eof())
                {
                    return false;
                }

                std::getline(this->file, line);
            }
        }

        label = endlabel = "";
    }

    return true;
}
