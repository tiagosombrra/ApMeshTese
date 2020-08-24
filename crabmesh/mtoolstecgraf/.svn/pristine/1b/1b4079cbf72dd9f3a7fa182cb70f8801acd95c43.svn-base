#include "Tools/MeshColorizer.h"

#if USE_GUI

#include <typeinfo>
#include "Data/State.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Mesh.h"

Tools::MeshColorizer::MeshColorizer() :
    Tools::Tool::Tool()
{
    this->vertices = true;
    this->frontElements = true;
    this->elements = true;

    this->scheme = UNIQUE;
    this->first = 0;

    this->mesh = NULL;

    this->colorized = false;
}

Tools::MeshColorizer::~MeshColorizer()
{

}

void Tools::MeshColorizer::setColorizeVertices(bool vertices)
{
    this->vertices = vertices;
}

void Tools::MeshColorizer::setColorizeFrontElements(bool frontElements)
{
    this->frontElements = frontElements;
}

void Tools::MeshColorizer::setColorizeElements(bool elements)
{
    this->elements = elements;
}

void Tools::MeshColorizer::setScheme(enum ColorizationScheme scheme, UInt first)
{
    this->scheme = scheme;
    this->first = first;
}

void Tools::MeshColorizer::setMesh(Data::Mesh *mesh)
{
    this->mesh = mesh;
}

bool Tools::MeshColorizer::isColorized() const
{
    return this->colorized;
}

void Tools::MeshColorizer::execute()
{
    this->colorized = false;

    if (!this->mesh)
    {
        return;
    }

    this->setMeasure(0, 0);

    switch (this->scheme)
    {
    case UNIQUE:
        this->colorizeUnique();
        break;

    case QUALITY:
        this->colorizeQuality();
        break;

    case ORDER:
        this->colorizeOrder();
        break;

    case TYPE:
        this->colorizeType();
        break;

    default:
        break;
    }

    this->endMeasure();
}

void Tools::MeshColorizer::colorizeUnique()
{
    Real r, g, b;
    Data::pallete(this->first, r, g, b);

    if (this->vertices)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->verticesSize()));
    }

    if (this->frontElements)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->frontElementsSize()));
    }

    if (this->elements)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->size()));
    }

    if (this->vertices)
    {
        this->colorized = true;

        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
             iter != this->mesh->vEnd(); iter++)
        {
            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }

    if (this->frontElements)
    {
        this->colorized = true;

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++)
        {
            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }

    if (this->elements)
    {
        this->colorized = true;

        for (Data::Mesh::ElementIterator iter = this->mesh->eBegin();
             iter != this->mesh->eEnd(); iter++)
        {
            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }
}

void Tools::MeshColorizer::colorizeQuality()
{
    if (this->elements)
    {
        this->addMeasure(0, 1);

        this->colorized = true;

        Real scale[3][10];

        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_0, scale[0][0], scale[1][0], scale[2][0]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_1, scale[0][1], scale[1][1], scale[2][1]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_2, scale[0][2], scale[1][2], scale[2][2]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_3, scale[0][3], scale[1][3], scale[2][3]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_4, scale[0][4], scale[1][4], scale[2][4]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_5, scale[0][5], scale[1][5], scale[2][5]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_6, scale[0][6], scale[1][6], scale[2][6]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_7, scale[0][7], scale[1][7], scale[2][7]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_8, scale[0][8], scale[1][8], scale[2][8]);
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_9, scale[0][9], scale[1][9], scale[2][9]);

        Data::Mesh::Quality quality;

        quality.findQuality(this->mesh, 10, &scale[0][0]);

        this->addMeasure(1, 0);
    }
}

void Tools::MeshColorizer::colorizeOrder()
{
    if (this->vertices)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->verticesSize()));
    }

    if (this->frontElements)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->frontElementsSize()));
    }

    if (this->elements)
    {
        this->addMeasure(0, static_cast<UInt>(this->mesh->size()));
    }

    if (this->vertices)
    {
        this->colorized = true;

        UInt order = this->first;

        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
             iter != this->mesh->vEnd(); iter++, order++)
        {
            Real r, g, b;
            Data::pallete(order, r, g, b);

            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }

    if (this->frontElements)
    {
        this->colorized = true;

        UInt order = this->first;

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++, order++)
        {
            Real r, g, b;
            Data::pallete(order, r, g, b);

            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }

    if (this->elements)
    {
        this->colorized = true;

        UInt order = this->first;

        for (Data::Mesh::ElementIterator iter = this->mesh->eBegin();
             iter != this->mesh->eEnd(); iter++, order++)
        {
            Real r, g, b;
            Data::pallete(order, r, g, b);

            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }
}

void Tools::MeshColorizer::colorizeType()
{
	UInt order = this->first;

	if (this->frontElements)
    {
        this->colorized = true;

        StringColorMap map;

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++)
        {
        	std::string type(typeid(*(*iter)->getShape()).name());

        	std::array<Real, 3> arr;

        	std::pair<StringColorMap::iterator, bool> p = map.insert(std::make_pair(type, arr));

        	Real r, g, b;

        	if (p.second)
			{
				Data::pallete(order, r, g, b);

				(*p.first).second[0] = r;
				(*p.first).second[1] = g;
				(*p.first).second[2] = b;

				order++;
			}
			else
			{
				r = (*p.first).second[0];
				g = (*p.first).second[1];
				b = (*p.first).second[2];
			}

            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }

    if (this->elements)
    {
        this->colorized = true;

        StringColorMap map;

        for (Data::Mesh::ElementIterator iter = this->mesh->eBegin();
             iter != this->mesh->eEnd(); iter++)
        {
        	std::string type = typeid(*(*iter)->getShape()).name();

        	std::array<Real, 3> arr;

        	std::pair<StringColorMap::iterator, bool> p = map.insert(std::make_pair(type, arr));

        	Real r, g, b;

        	if (p.second)
			{
				Data::pallete(order, r, g, b);

				(*p.first).second[0] = r;
				(*p.first).second[1] = g;
				(*p.first).second[2] = b;

				order++;
			}
			else
			{
				r = (*p.first).second[0];
				g = (*p.first).second[1];
				b = (*p.first).second[2];
			}

            (*iter)->setColor(r, g, b);

            this->addMeasure(1, 0);
        }
    }
}

#endif //#if USE_GUI
