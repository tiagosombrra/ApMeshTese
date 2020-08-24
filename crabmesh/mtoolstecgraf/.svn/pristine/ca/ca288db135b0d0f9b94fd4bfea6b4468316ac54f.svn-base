#include "Tools/SilhouetteIdentifier.h"

#include "Data/Point3D.h"
#include "Data/Edge3D.h"
#include "Data/Polygon.h"
#include "Data/Boundary.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Mesh.h"
#include "Performer/SequentialIdManager.h"

using namespace Tools;

Tools::SilhouetteIdentifier::SilhouetteIdentifier(const Tool *tool) :
	Tools::ExternalTool::ExternalTool(tool)
{
	this->boundary = NULL;
	this->silhouette = NULL;
	this->angleThreshold = 0.0;
	this->idManager = NULL;
}

Tools::SilhouetteIdentifier::~SilhouetteIdentifier()
{

}

void Tools::SilhouetteIdentifier::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Tools::SilhouetteIdentifier::setAngle(Real angle)
{
	this->angleThreshold = angle;
}

Data::Mesh *Tools::SilhouetteIdentifier::getSilhouette()
{
	return this->silhouette;
}

Data::Mesh *Tools::SilhouetteIdentifier::makeMesh() const
{
	return new MJMesh::Mesh();
}

Data::Vertex *Tools::SilhouetteIdentifier::makeVertex(const Data::Vertex *v) const
{
	return new MJMesh::Vertex(new Data::Point3D(v->getPoint()), v->getId());
}

Data::FrontElement *Tools::SilhouetteIdentifier::makeFrontElement(Data::Vertex *v1, Data::Vertex *v2) const
{
	return new MJMesh::FrontElementSurface(static_cast<MJMesh::Vertex *>(v1), static_cast<MJMesh::Vertex *>(v2), this->idManager->next(1));
}

Real Tools::SilhouetteIdentifier::angle(const Data::FrontElement *fel1, const Data::FrontElement *fel2) const
{
	Data::Vector3D v1 = static_cast<Data::Polygon *>(fel1->getShape())->normal();
	Data::Vector3D v2 = static_cast<Data::Polygon *>(fel2->getShape())->normal();

	return v1.angle(v2);
}

void Tools::SilhouetteIdentifier::execute()
{
	if ((!this->boundary) || (this->boundary->dimension() != 3))
	{
		return;
	}

	this->addMeasure(0, static_cast<UInt>(this->boundary->size()));

	this->angleThreshold *= M_PI/180.0;

	Data::FrontElementHash used;
	Data::VertexHashMap vertices;

	this->silhouette = this->makeMesh();
	this->idManager = new Performer::SequentialIdManager(1, 2);

	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::Vertex *v1 = fel->getVertex(i);
			Data::Vertex *v2 = fel->getVertex((i+1)%fel->numVertices());

			Data::FrontElement *adj = NULL;

			Data::FrontElementList fels = this->boundary->adjacency(v1);

			while (!fels.empty())
			{
				Data::FrontElement *fel2 = fels.front();

				fels.pop_front();

				if (fel2 == fel)
				{
					continue;
				}

				if (fel2->have(v2))
				{
					adj = fel2;

					break;
				}
			}

			if (!adj)
			{
				continue;
			}

			if (used.find(adj) != used.end())
			{
				continue;
			}

			Real angle = this->angle(fel, adj);

			if (angle < this->angleThreshold)
			{
				continue;
			}

			Data::Vertex *newV1 = (*vertices.insert(std::make_pair(v1->getId(), static_cast<Data::Vertex *>(NULL))).first).second;

			if (!newV1)
			{
				newV1 = this->makeVertex(v1);

				vertices[v1->getId()] = newV1;

				this->silhouette->add(newV1);
			}

			Data::Vertex *newV2 = (*vertices.insert(std::make_pair(v2->getId(), static_cast<Data::Vertex *>(NULL))).first).second;

			if (!newV2)
			{
				newV2 = this->makeVertex(v2);

				vertices[v2->getId()] = newV2;

				this->silhouette->add(newV2);
			}

			Data::FrontElement *newFel = this->makeFrontElement(newV1, newV2);

			this->silhouette->add(newFel);
		}

		used.insert(fel);

		this->addMeasure(1, 0);
	}

	if (this->silhouette->verticesSize() == 0)
	{
		delete this->silhouette;

		this->silhouette = NULL;
	}

	delete this->idManager;

	this->endMeasure();
}
