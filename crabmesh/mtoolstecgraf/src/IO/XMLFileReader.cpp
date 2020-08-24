#include "IO/XMLFileReader.h"

#if USE_TINYXML

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"
#include "TecGraf/Surface.h"
#include "TecGraf/Region.h"
#include "TecGraf/Model.h"

using namespace IO;

IO::XMLFileReader::XMLFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

#if USE_TECGRAF
	this->model = NULL;
#endif //#if USE_TECGRAF

	this->boundary = false;
	this->ismesh = false;

#if USE_TECGRAF
	this->forceModel = false;
#endif //#if USE_TECGRAF

	this->addFiletype(".xml");
}

IO::XMLFileReader::~XMLFileReader()
{

}

void IO::XMLFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

#if USE_TECGRAF
void IO::XMLFileReader::setForceModel(bool forceModel)
{
	this->forceModel = forceModel;
}
#endif //#if USE_TECGRAF

bool IO::XMLFileReader::isBoundary() const
{
	return this->boundary;
}

bool IO::XMLFileReader::isMesh() const
{
	return this->ismesh;
}

#if USE_TECGRAF
TecGraf::Model *IO::XMLFileReader::getModel() const
{
	return this->model;
}
#endif //#if USE_TECGRAF

bool IO::XMLFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::XMLFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::XMLFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

#if USE_TECGRAF
TecGraf::Model *IO::XMLFileReader::makeModel() const
{
	return new TecGraf::Model();
}
#endif //#if USE_TECGRAF

void IO::XMLFileReader::read()
{
	bool ok = this->readXML();

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

bool IO::XMLFileReader::readXML()
{
	/*bool ok = false;

	std::string str;

	this->file >> str;

	if (str == "<bsmio-mesh")
	{
		ok = this->readBSM();
	}

	this->file.close();

	return ok;*/

	this->file.close();

    TiXmlDocument file(this->filename);
    //TiXmlDocument file(this->filename.c_str());


	if (!file.LoadFile())
	{
		return false;
	}

	TiXmlHandle handle(&file);

	TiXmlElement *root = handle.FirstChildElement().Element();

	if (!root)
	{
		return false;
	}

	bool ok = false;

	if (std::string(root->Value()) == "bsmio-mesh")
	{
		ok = this->readBSM(root);
	}

	return ok;
}

template <typename T>
std::vector<T> parse(const std::string &str)
{
	std::vector<T> vec;

	std::stringstream stream;
	stream << str;

	while (!stream.eof())
	{
		T value;

		stream >> value;

		vec.push_back(value);
	}

	return vec;
}

bool IO::XMLFileReader::readBSM(TiXmlElement *bsmio)
{
	TiXmlHandle hBsmio(bsmio);

	Real divx = 1.0;
	Real divy = 1.0;
	Real divz = 1.0;

#if USE_C__11
	Data::VertexHashMap mapVertices;
#else
	Data::VertexMap mapVertices;
#endif //#if USE_C__11

	UInt region = 0;
	this->boundary = false;
	this->ismesh = false;

#if USE_TECGRAF
	if (this->forceModel)
	{
		region = 2;
	}
	else
#endif //#if USE_TECGRAF
	{
		for (TiXmlElement *elem = hBsmio.FirstChild().Element(); elem; elem = elem->NextSiblingElement())
		{
			std::string str(elem->Value());

#if USE_TECGRAF
			if (str == "interval")
			{
				region++;
			}
			else
#endif //#if USE_TECGRAF
				if (str == "element")
			{
				this->ismesh = true;
			}
		}
	}

	if (this->ismesh)
	{
		this->boundary = false;
		region = 0;
	}
	else if (region > 1)
	{
		this->boundary = false;
		this->ismesh = false;

#if USE_TECGRAF
		this->model = this->makeModel();
#endif //#if USE_TECGRAF
	}
	else
	{
		region = 0;
		this->boundary = true;
	}

#if USE_TECGRAF
#if USE_C__11
	typedef std::unordered_map<ULInt, TecGraf::Surface *> SurfaceMap;
#else
	typedef std::map<ULInt, TecGraf::Surface *> SurfaceMap;
#endif //#if USE_C__11

	SurfaceMap surfaces;
#endif //#if USE_TECGRAF

	for (TiXmlElement *elem = hBsmio.FirstChild().Element(); elem; elem = elem->NextSiblingElement())
	{
		std::string str(elem->Value());

		if (str == "mesh-units")
		{
			TiXmlHandle hElem(elem);

			for (TiXmlElement *elem = hElem.FirstChild().Element(); elem; elem = elem->NextSiblingElement())
			{
				str = std::string(elem->Value());

				size_t pos = str.find("-coordinate-unit");

				if (pos == std::string::npos)
				{
					continue;
				}

				std::string coord = str.substr(0, str.size() - pos);
				std::string value = elem->GetText();

				Real val = 1.0;

				if (value == "km")
				{
					val *= 1000.0;
				}
				else if (value == "hm")
				{
					val *= 100.0;
				}
				else if (value == "dam")
				{
					val *= 10.0;
				}

				if (coord.find("x") != std::string::npos)
				{
					divx = val;
				}

				if (coord.find("y") != std::string::npos)
				{
					divy = val;
				}

				if (coord.find("z") != std::string::npos)
				{
					divz = val;
				}
			}
		}
		else if (str == "surface")
		{
#if USE_TECGRAF
			if (this->model)
			{
				TiXmlElement *chElem = elem->FirstChildElement("id");

				std::string str = chElem->GetText();

				ULInt id = Data::convert<ULInt>(str);

				TecGraf::Surface *surface = new TecGraf::Surface(id);

				surfaces[id] = surface;

				chElem = elem->FirstChildElement("name");

				surface->setName(chElem->GetText());

				chElem = elem->FirstChildElement("type");

				surface->setType(chElem->GetText());

				chElem = elem->FirstChildElement("color");

				str = chElem->GetText();

				Data::RealVector color = parse<Real>(str);

#if USE_GUI
				surface->setColor(color[0], color[1], color[2]);
#endif //#if USE_GUI
			}
#endif //#if USE_TECGRAF
		}
		else if (str == "position")
		{
			ULInt id = 0;
			Real x = 0.0, y = 0.0, z = 0.0;

			if (elem->QueryValueAttribute("id", &id) != TIXML_SUCCESS)
			{
				return false;
			}

			if (elem->QueryValueAttribute("x", &x) != TIXML_SUCCESS)
			{
				return false;
			}

			if (elem->QueryValueAttribute("y", &y) != TIXML_SUCCESS)
			{
				return false;
			}

			if (elem->QueryValueAttribute("z", &z) != TIXML_SUCCESS)
			{
				return false;
			}

			if (this->boundary)
			{
				this->boundaryBuilder->add(id, x/divx, y/divy, z/divz);
			}
			else if (this->ismesh)
			{
				MJMesh::Vertex *v = new MJMesh::Vertex(x/divx, y/divy, z/divz, id);

				mapVertices[id] = v;

				static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
			}
#if USE_TECGRAF
			else
			{
				MJMesh::Vertex *v = new MJMesh::Vertex(x/divx, y/divy, z/divz, id);

				mapVertices[id] = v;

				TiXmlElement *chElem = elem->FirstChildElement("surface-ids");

				if (chElem)
                {
                    std::string str = chElem->GetText();

                    Data::UIntVector ids = parse<UInt>(str);

                    for (UInt i = 0; i < ids.size(); i++)
                    {
                        surfaces[ids[i]]->add(v);
                    }
                }
			}
#endif //#if USE_TECGRAF
		}
		else if (str == "face")
		{
			ULInt id = 0;

			if (elem->QueryValueAttribute("id", &id) != TIXML_SUCCESS)
			{
				return false;
			}

			TiXmlElement *nodeids = elem->FirstChildElement("node-ids");

			std::string str = nodeids->GetText();

			Data::ULIntVector ids = parse<ULInt>(str);

			UInt min = 3, max = 3;

#if USE_QUADS
			max = 4;
#endif //#if USE_QUADS

			//if ((count < min) || (count > max))
			if ((ids.size() < min) || (ids.size() > max))
			{
				return false;
			}

			if (this->boundary)
			{
#if USE_QUADS
				//if (count == 3)
				if (ids.size() == 3)
#endif //#if USE_QUADS
				{
					this->boundaryBuilder->add(id, ids[0], ids[1], ids[2]);
				}
#if USE_QUADS
				//else if (count == 4)
				else if (ids.size() == 4)
				{
					this->boundaryBuilder->add(id, ids[0], ids[1], ids[2], ids[3]);
				}
#endif //#if USE_QUADS
			}
			else if (this->ismesh)
			{
				MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[ids[0]]);
				MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[ids[1]]);
				MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[ids[2]]);

				MJMesh::FrontElement *fel = NULL;

#if USE_QUADS
				//if (count == 3)
				if (ids.size() == 3)
#endif //#if USE_QUADS
				{
					fel = new MJMesh::FrontElement3D(v1, v2, v3, id);
				}
#if USE_QUADS
				//else if (count == 4)
				else if (ids.size() == 4)
				{
					MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(mapVertices[ids[3]]);

					fel = new MJMesh::FrontElement3D(v1, v2, v3, v4, id);
				}
#endif //#if USE_QUADS

				static_cast<MJMesh::Mesh *>(this->mesh)->add(fel);
			}
#if USE_TECGRAF
			else
			{
				MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[ids[0]]);
				MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[ids[1]]);
				MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[ids[2]]);

				MJMesh::FrontElement *fel = NULL;

#if USE_QUADS
				//if (count == 3)
				if (ids.size() == 3)
#endif //#if USE_QUADS
				{
					fel = new MJMesh::FrontElement3D(v1, v2, v3, id);
				}
#if USE_QUADS
				//else if (count == 4)
				else if (ids.size() == 4)
				{
					MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(mapVertices[ids[3]]);

					fel = new MJMesh::FrontElement3D(v1, v2, v3, v4, id);
				}
#endif //#if USE_QUADS

				if (fel)
				{
					TiXmlElement *chElem = elem->FirstChildElement("surface-ids");

					std::string str = chElem->GetText();

					Data::ULIntVector ids = parse<ULInt>(str);

					for (UInt i = 0; i < ids.size(); i++)
					{
						surfaces[ids[i]]->add(fel);

						for (UInt j = 0; j < fel->numVertices(); j++)
                        {
                            if (!surfaces[ids[i]]->have(fel->getVertex(j)))
                            {
                                surfaces[ids[i]]->add(fel->getVertex(j));
                            }
                        }
					}
				}
			}
#endif //#if USE_TECGRAF
		}
		else if (str == "element")
		{
			if (this->boundary)
			{
				this->mesh = this->makeMesh(false);

				Data::Boundary *b = this->boundaryBuilder->getBoundary();

				for (Data::Boundary::VertexIterator iter = b->vBegin();
					 iter != b->vEnd(); iter = b->erase(iter))
				{
					Data::Vertex *v = (*iter);

					mapVertices[v->getId()] = v;

					static_cast<MJMesh::Mesh *>(this->mesh)->add(v);
				}

				for (Data::Boundary::FrontElementIterator iter = b->eBegin();
					 iter != b->eEnd(); iter = b->erase(iter))
				{
					static_cast<MJMesh::Mesh *>(this->mesh)->add((*iter));
				}

				this->boundaryBuilder->clear();

				this->boundary = false;
			}

			ULInt id = 0;

			if (elem->QueryValueAttribute("id", &id) != TIXML_SUCCESS)
			{
				return false;
			}

			TiXmlElement *nodeids = elem->FirstChildElement("node-ids");

			std::string str = nodeids->GetText();

			Data::ULIntVector ids = parse<ULInt>(str);

			UInt min = 4, max = 4;

#if USE_BRICKS
			max = 8;
#endif //#if USE_BRICKS

			//if ((count != min) && (count != max))
			if ((ids.size() != min) && (ids.size() != max))
			{
				return false;
			}

			MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(mapVertices[ids[0]]);
			MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(mapVertices[ids[1]]);
			MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(mapVertices[ids[2]]);
			MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(mapVertices[ids[3]]);

			MJMesh::Element *el = NULL;

#if USE_BRICKS
			//if (count == 4)
			if (ids.size() == 4)
#endif //#if USE_BRICKS
			{
				el = new MJMesh::Element3D(v1, v2, v3, v4, id);
			}
#if USE_BRICKS
			//else if (count == 8)
			else if (ids.size() == 8)
			{
				MJMesh::Vertex *v5 = static_cast<MJMesh::Vertex *>(mapVertices[ids[4]]);
				MJMesh::Vertex *v6 = static_cast<MJMesh::Vertex *>(mapVertices[ids[5]]);
				MJMesh::Vertex *v7 = static_cast<MJMesh::Vertex *>(mapVertices[ids[6]]);
				MJMesh::Vertex *v8 = static_cast<MJMesh::Vertex *>(mapVertices[ids[7]]);

				el = new MJMesh::Element3D(v1, v2, v3, v4, v5, v6, v7, v8, id);
			}
#endif //#if USE_BRICKS

			static_cast<MJMesh::Mesh *>(this->mesh)->add(el);
		}
		else if (str == "interval")
		{
#if USE_TECGRAF
			if (this->model)
			{
				TecGraf::Region *r = new TecGraf::Region();

				TiXmlElement *chElem = elem->FirstChildElement("duration-value");

				std::string str = chElem ? chElem->GetText() : "";

				Real value = chElem ? Data::convert<Real>(str) : 0.0;

				r->setDuration(value);

				chElem = elem->FirstChildElement("surface-ids");

				str = chElem->GetText();

				Data::ULIntVector ids = parse<ULInt>(str);

				chElem = elem->FirstChildElement("surface-orientation");

				str = chElem->GetText();

				Data::IntVector ors = parse<Int>(str);

				for (UInt i = 0; i < ids.size(); i++)
				{
					TecGraf::Surface *s = surfaces[ids[i]];
					Int ori = ors[i];

					if (ori == 0)
                    {
                        r->add(s, 1);
                        r->add(s, -1);
                    }
                    else
                    {
                        r->add(s, ori);
                    }
				}

				this->model->add(r);
			}
#endif //#if USE_TECGRAF
		}
	}

	return true;
}

#endif //#if USE_TINYXML
