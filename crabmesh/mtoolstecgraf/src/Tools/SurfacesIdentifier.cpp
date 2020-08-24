#include "Tools/SurfacesIdentifier.h"

#include "Data/Point3D.h"
#include "Data/Edge3D.h"
#include "Data/Polygon.h"
#include "Data/Boundary.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Mesh.h"
#include "Performer/SequentialIdManager.h"

using namespace Tools;

Tools::SurfacesIdentifier::SurfacesIdentifier(const Tool *tool) :
	Tools::ExternalTool::ExternalTool(tool)
{
	this->boundary = NULL;
	this->angleThreshold = 0.0;
	this->idManager = NULL;
}

Tools::SurfacesIdentifier::~SurfacesIdentifier()
{

}

void Tools::SurfacesIdentifier::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Tools::SurfacesIdentifier::setAngle(Real angle)
{
	this->angleThreshold = angle;
}

const Data::MeshList &Tools::SurfacesIdentifier::getSurfaces() const
{
	return this->surfaces;
}

Data::Mesh *Tools::SurfacesIdentifier::makeMesh() const
{
	return new MJMesh::Mesh();
}

Data::Vertex *Tools::SurfacesIdentifier::makeVertex(const Data::Vertex *v, Data::VertexHashMap &map) const
{
	std::pair<Data::VertexHashMap::iterator, bool> p = map.insert(std::make_pair(v->getId(), static_cast<Data::Vertex *>(NULL)));

	if (p.second)
	{
		Data::Vertex *newv = new MJMesh::Vertex(new Data::Point3D(v->getPoint()), v->getId());

		(*p.first).second = newv;

		return newv;
	}

	return (*p.first).second;
}

Data::FrontElement *Tools::SurfacesIdentifier::makeFrontElement(Data::Vertex *v1, Data::Vertex *v2, Data::VertexHashMap &map, const Data::Vector3D &normal) const
{
	MJMesh::Vertex *newv1 = static_cast<MJMesh::Vertex *>(map[v1->getId()]);
	MJMesh::Vertex *newv2 = static_cast<MJMesh::Vertex *>(map[v2->getId()]);

	ULInt id = this->idMap[newv1->getId()][newv2->getId()];

	if (id == 0)
	{
		id = this->idManager->next(1);

		this->idMap[newv1->getId()][newv2->getId()] = id;
		this->idMap[newv2->getId()][newv1->getId()] = id;
	}

	MJMesh::FrontElementSurface *fel = new MJMesh::FrontElementSurface(newv1, newv2, id);

	fel->setNormal(normal);

	for (Data::ElementList::iterator iter = newv1->eBegin();
		 iter != newv1->eEnd(); iter++)
	{
		if ((*iter)->have(newv2))
		{
			fel->add(static_cast<MJMesh::Element *>((*iter)));

			break;
		}
	}

	return fel;
}

Data::Element *Tools::SurfacesIdentifier::makeElement(Data::/*Front*/Element *fel, Data::VertexHashMap &map) const
{
#if USE_QUADS
	if (fel->numVertices() == 4)
	{
		return new MJMesh::ElementSurface(
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(0)->getId()]),
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(1)->getId()]),
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(2)->getId()]),
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(3)->getId()]), fel->getId());
	}
#endif //#if USE_QUADS

	return new MJMesh::ElementSurface(
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(0)->getId()]),
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(1)->getId()]),
			static_cast<MJMesh::Vertex *>(map[fel->getVertex(2)->getId()]), fel->getId());
}

Real Tools::SurfacesIdentifier::angle(const Data::/*Front*/Element *fel1, const Data::/*Front*/Element *fel2) const
{
	Data::Vector3D v1 = static_cast<Data::Polygon *>(fel1->getShape())->normal();
	Data::Vector3D v2 = static_cast<Data::Polygon *>(fel2->getShape())->normal();

	return v1.angle(v2);
}

void Tools::SurfacesIdentifier::identifySurface(const Data::/*Front*/Element *fel, const AdjacenciesIdentifier::AdjacencyMap &adjacencies,
	Data::/*Front*/ElementHash &used, Data::/*Front*/ElementList &surface, Data::VertexList &edges, Data::VectorVector &normals) const
{
	std::pair<Data::/*Front*/ElementHash::iterator, bool> p = used.insert(const_cast<Data::/*Front*/Element *>(fel));

	if (!p.second)
	{
		return;
	}

	surface.push_back(const_cast<Data::/*Front*/Element *>(fel));

	AdjacenciesIdentifier::AdjacencyMap::const_iterator iter = adjacencies.find(const_cast<Data::/*Front*/Element *>(fel));

	if (iter == adjacencies.end())
	{
		return;
	}

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		Data::Vertex *v1 = fel->getVertex(i);
		Data::Vertex *v2 = fel->getVertex((i+1)%fel->numVertices());

		AdjacenciesIdentifier::VertexAdjacencyMap::const_iterator iter2 = (*iter).second.find(v1);

		if (iter2 == (*iter).second.end())
		{
			continue;
		}

		Data::/*Front*/Element *adj = (*iter2).second;

		Real angle = this->angle(fel, adj);

		if (angle >= this->angleThreshold)
		{
			edges.push_back(v1);
			edges.push_back(v2);

			Data::Vector3D n = static_cast<Data::Polygon *>(fel->getShape())->normal();
			Data::Vector3D v(static_cast<Data::Point3D *>(v1->getPoint()), static_cast<Data::Point3D *>(v2->getPoint()));

			Data::Vector3D cross = n.cross(v);

			cross.normalize();

			normals.push_back(new Data::Vector3D(cross));

			continue;
		}

		this->identifySurface(adj, adjacencies, used, surface, edges, normals);
	}

	this->addMeasure(1, 0);
}

void Tools::SurfacesIdentifier::execute()
{
	if ((!this->boundary) || (this->boundary->dimension() != 3))
	{
		return;
	}

	this->angleThreshold *= M_PI/180.0;

	Tools::AdjacenciesIdentifier *identifier = new Tools::AdjacenciesIdentifier(this);

	identifier->setBoundary(this->boundary);

	identifier->execute();

	AdjacenciesIdentifier::AdjacencyMap adjacencies;

	identifier->getAdjacenciesSwapping(adjacencies);

	delete identifier;

	this->idManager = new Performer::SequentialIdManager(1, 2);

	Data::/*Front*/ElementHash used;
	Data::VertexHashMap vertices;

	this->addMeasure(0, static_cast<UInt>(adjacencies.size()));

	for (AdjacenciesIdentifier::AdjacencyMap::iterator iter = adjacencies.begin();
		 iter != adjacencies.end(); iter++)
	{
		Data::/*Front*/ElementList surface;
		Data::VertexList edges;
		Data::VectorVector normals;

		this->identifySurface((*iter).first, adjacencies, used, surface, edges, normals);

		if (surface.empty())
		{
			continue;
		}

		Data::Mesh *mesh = this->makeMesh();

		Data::VertexHashMap vertices;

		for (Data::/*Front*/ElementList::iterator iter = surface.begin();
			 iter != surface.end(); iter++)
		{
			Data::/*Front*/Element *e = (*iter);

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				mesh->add(this->makeVertex(e->getVertex(i), vertices));
			}

			mesh->add(this->makeElement(e, vertices));
		}

		Data::VectorVector::iterator niter = normals.begin();

		for (Data::VertexList::iterator iter = edges.begin();
			 iter != edges.end(); iter++, niter++)
		{
			Data::Vertex *v1 = (*iter);
			iter++;
			Data::Vertex *v2 = (*iter);

			Data::FrontElement *fel = this->makeFrontElement(v1, v2, vertices, *(*niter));

			delete (*niter);

			mesh->add(fel);

			mesh->removeInner(fel->getVertex(0));
			mesh->removeInner(fel->getVertex(1));
		}

		this->surfaces.push_back(mesh);
	}

	delete this->idManager;

	this->endMeasure();
}
