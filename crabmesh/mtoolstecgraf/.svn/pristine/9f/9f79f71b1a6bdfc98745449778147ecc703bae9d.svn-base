#include "Performer/UniformBoundaryAdapter.h"

#include "Performer/IdManager.h"
#include "Data/Mesh.h"
#include "Data/Boundary.h"
#include "Data/FrontElement.h"
#include "Data/Vertex.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector3D.h"

Performer::UniformBoundaryAdapter::UniformBoundaryAdapter(Data::Boundary *boundary) :
	Performer::BoundaryAdapter(boundary)
{
	this->dimension = 0;
	this->mesh = NULL;
}

Performer::UniformBoundaryAdapter::UniformBoundaryAdapter(Data::Mesh *surface) :
	Performer::BoundaryAdapter(surface)
{
	this->dimension = 0;
	this->mesh = NULL;
}

Performer::UniformBoundaryAdapter::~UniformBoundaryAdapter()
{

}

void Performer::UniformBoundaryAdapter::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

bool Performer::UniformBoundaryAdapter::adaptEdge2D(Data::/*Front*/Element *element, Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const
{
	Data::Vertex *v = element->getVertex(1);
	element->setVertex(1, mid);

	Data::VertexVector vertices(2);
	vertices[0] = mid;
	vertices[1] = v;

	Data::/*Front*/Element *newElement = this->makeElement(vertices);

	newElements.push_back(newElement);

	return true;
}

bool Performer::UniformBoundaryAdapter::adaptTriangle3D(Data::/*Front*/Element *element, Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements) const
{
	element->setVertex(1, mids[0]);
	element->setVertex(2, mids[2]);

	Data::VertexVector vertices(3);
	vertices[0] = mids[0];
	vertices[1] = vs[1];
	vertices[2] = mids[1];

	Data::/*Front*/Element *newElement = this->makeElement(vertices);
	newElements.push_back(newElement);

	vertices[0] = mids[2];
	vertices[1] = mids[1];
	vertices[2] = vs[2];

	newElement = this->makeElement(vertices);
	newElements.push_back(newElement);

	vertices[0] = mids[0];
	vertices[1] = mids[1];
	vertices[2] = mids[2];

	newElement = this->makeElement(vertices);
	newElements.push_back(newElement);

	return true;
}

bool Performer::UniformBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	if (this->dimension == 2)
	{
#if USE_EXPERIMENTAL_3
		Data::Vertex *mid = this->makeVertex(element->getShape()->mid2D());
#else
		Data::Vertex *mid = this->makeVertex(element->getShape()->mid());
#endif //#if USE_EXPERIMENTAL_3

		newVertices.push_back(mid);

		this->adaptEdge2D(element, mid, newElements);

		return true;
	}

	if (this->dimension == 3)
	{
		if (element->numVertices() == 3)
		{
			Data::Vertex *vs[3];

			for (UInt i = 0; i < 3; i++)
			{
				vs[i] = element->getVertex(i);
			}

			Data::Vertex *mids[3];

			for (UInt i = 0; i < 3; i++)
			{
				ULInt id1 = vs[i]->getId();
				ULInt id2 = vs[(i+1)%3]->getId();

				//mids[i] = this->mids[id1][id2];
				mids[i] = NULL;

				{
#if USE_C__11
					VertexMatrixHashMap::iterator it = this->mids.find(id1);
#else
					VertexMatrixMap::iterator it = this->mids.find(id1);
#endif //#if USE_C__11

					if (it != this->mids.end())
					{
#if USE_C__11
						std::unordered_map<ULInt, Data::Vertex *>::iterator it2 = (*it).second.find(id2);
#else
						std::map<ULInt, Data::Vertex *>::iterator it2 = (*it).second.find(id2);
#endif //#if USE_C__11

						if (it2 != (*it).second.end())
						{
							mids[i] = (*it2).second;
						}
					}
				}

				if (!mids[i])
				{
					Data::Vector3D vec(vs[i]->getPoint(), vs[(i+1)%3]->getPoint());
					vec.multiply(0.5);

#if USE_EXPERIMENTAL_3
					Data::Point3D pt = *vs[i]->getPoint();
#else
					Data::Point pt = *vs[i]->getPoint();
#endif //#if USE_EXPERIMENTAL_3
					pt.sum(vec);

					this->mids[id2][id1] = mids[i] = this->makeVertex(pt);

					newVertices.push_back(mids[i]);
				}
			}

			return this->adaptTriangle3D(element, vs, mids, newElements);
		}
	}

	return false;
}

void Performer::UniformBoundaryAdapter::execute()
{
	if (!this->getMesh())
	{
		return;
	}

	if (this->getMesh()->size() == 0)
	{
		return;
	}

	Data::/*Front*/ElementList newElements;
	Data::VertexList newVertices;

	this->dimension = this->getMesh()->dimension();

#if USE_C__11
	this->mids.reserve(this->getMesh()->verticesSize());
#endif //#if USE_C__11

	if (this->isBoundary())
    {
        for (Data::Boundary::FrontElementIterator iter = this->getBoundary()->eBegin();
             iter != this->getBoundary()->eEnd(); iter++)
        {
            Data::FrontElement *element = (*iter);

            this->adapt(element, newElements, newVertices);
        }
    }
    else
    {
        for (Data::Mesh::ElementIterator iter = this->getSurface()->eBegin();
             iter != this->getSurface()->eEnd(); iter++)
        {
            Data::Element *element = (*iter);

            this->adapt(element, newElements, newVertices);
        }
    }

    UInt n = this->isBoundary() ? 1 : 2;

	while (!newElements.empty())
	{
		Data::/*Front*/Element *element = newElements.front();

		newElements.pop_front();

		element->setId(this->idManager->next(n));

		this->add(element);

		if (this->mesh)
		{
			this->mesh->add(static_cast<Data::FrontElement *>(element));
		}
	}

	while (!newVertices.empty())
	{
		Data::Vertex *vertex = newVertices.front();

		newVertices.pop_front();

		vertex->setId(this->idManager->next(0));

		this->add(vertex);

		if (this->mesh)
		{
			this->mesh->add(vertex, false);
		}
	}

	this->mids.clear();
}

Data::Vertex *Performer::UniformBoundaryAdapter::makeVertex(const Data::Point &pt) const
{
	if (this->dimension == 2)
	{
		return new Data::Vertex(new Data::Point2D(pt));
	}

	if (this->dimension == 3)
	{
		return new Data::Vertex(new Data::Point3D(pt));
	}

#if USE_EXPERIMENTAL_3
	return NULL;
#else
	return new Data::Vertex(new Data::Point(pt));
#endif //#if USE_EXPERIMENTAL_3
}

/*Data::FrontElement *Performer::UniformBoundaryAdapter::makeElement(const Data::VertexVector &vertices) const
{
	if (this->dimension == 2)
	{
		Data::Edge2D *edge = new Data::Edge2D();

		Data::FrontElement *element = new Data::FrontElement(edge);

		for (UInt i = 0; i < 2; i++)
		{
			element->setVertex(i, vertices[i]);
		}

		return element;
	}

	if (this->dimension == 3)
	{
		if (vertices.size() == 3)
		{
			Data::Triangle3D *triangle = new Data::Triangle3D();

			Data::FrontElement *element = new Data::FrontElement(triangle);

			for (UInt i = 0; i < 3; i++)
			{
				element->setVertex(i, vertices[i]);
			}

			return element;
		}
	}

	return NULL;
}*/
