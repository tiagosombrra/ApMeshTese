#include "Performer/VertexMerger.h"

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

Performer::VertexMerger::VertexMerger() :
	Performer::Builder::Builder()
{
	this->mesh = NULL;
	this->boundary = NULL;
	this->useTolerance = false;
	this->merged = false;
	this->dimension = 0;
	this->tolerance = 0.0;
}

Performer::VertexMerger::~VertexMerger()
{

}

void Performer::VertexMerger::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::VertexMerger::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::VertexMerger::setUseTolerance(bool useTolerance)
{
	this->useTolerance = useTolerance;
}

void Performer::VertexMerger::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

bool Performer::VertexMerger::isMerged() const
{
	return this->merged;
}

void Performer::VertexMerger::mergeMesh(Data::Vertex *v, Data::Mesh::VertexIterator iter)
{
	VectorList vectors;
	UInt numVectors = 0;

	for (++iter; iter != this->mesh->vEnd();)
	{
		Data::Vertex *o = (*iter);

		bool match, equal;

		this->matchOrEqual(v, o, match, equal);

		if ((!match) && (!equal))
		{
			iter++;

			continue;
		}

		this->merged = true;

		this->add(match, equal, vectors, numVectors, v, o);

		Data::FrontElementList fadjs = this->mesh->findAdjacentFrontElements(o);

		while (!fadjs.empty())
		{
			Data::FrontElement *fel = fadjs.front();

			fadjs.pop_front();

			this->replace(fel, o, v);
		}

		Data::ElementList adjs = this->mesh->adjacency(o);

		while (!adjs.empty())
		{
			Data::Element *e = adjs.front();

			adjs.pop_front();

			this->replace(e, o, v);
		}

		if ((this->boundary) && (this->boundary->have(o)))
		{
			this->boundary->remove(o);
		}

		iter = this->mesh->erase(iter);

		delete o;
	}

	this->sum(v, vectors, numVectors);
}

void Performer::VertexMerger::mergeBoundary(Data::Vertex *v, Data::Boundary::VertexIterator iter)
{
	VectorList vectors;
	UInt numVectors = 0;

	for (++iter; iter != this->boundary->vEnd();)
	{
		Data::Vertex *o = (*iter);

		bool match, equal;

		this->matchOrEqual(v, o, match, equal);

		if ((!match) && (!equal))
		{
			iter++;

			continue;
		}

		this->merged = true;

		this->add(match, equal, vectors, numVectors, v, o);

		Data::FrontElementList fadjs = this->boundary->adjacency(o);

		while (!fadjs.empty())
		{
			Data::FrontElement *fel = fadjs.front();

			fadjs.pop_front();

			this->replace(fel, o, v);
		}

		iter = this->boundary->erase(iter);

		delete o;
	}

	this->sum(v, vectors, numVectors);
}

void Performer::VertexMerger::add(bool /*match*/, bool equal, VectorList &vectors,
	UInt &numVectors, Data::Vertex *v, Data::Vertex *o) const
{
	if (equal)
	{
#if USE_EXPERIMENTAL_3
		Data::Vector *vec = (this->dimension == 2) ?
            static_cast<Data::Vector *>(new Data::Vector2D(v->getPoint(), o->getPoint())) :
            static_cast<Data::Vector *>(new Data::Vector3D(v->getPoint(), o->getPoint()));
#else
		Data::Vector vec(v->getPoint(), o->getPoint());
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
		vec->multiply(0.5);
#else
		vec.multiply(0.5);
#endif //#if USE_EXPERIMENTAL_3

		vectors.push_back(vec);
	}

	numVectors++;
}

void Performer::VertexMerger::sum(Data::Vertex *v, VectorList &vectors, UInt numVectors)
{
	if (vectors.empty())
	{
		return;
	}

	Real den = 1.0/static_cast<Real>(numVectors);

	//Data::Vector sum(this->dimension);
	VECTORd(sum, this->dimension);

	while (!vectors.empty())
	{
		sum.sum(vectors.front());

#if USE_EXPERIMENTAL_3
        delete vectors.front();
#endif //#if USE_EXPERIMENTAL_3

		vectors.pop_front();
	}

	sum.multiply(den);

	v->getPoint()->sum(sum);
}

void Performer::VertexMerger::matchOrEqual(const Data::Vertex *v1, const Data::Vertex *v2, bool &match, bool &equal) const
{
	match = this->match(v1, v2);
	equal = false;

	if (!match)
	{
		equal = this->equal(v1, v2);
	}
}

void Performer::VertexMerger::execute()
{
	this->merged = false;

	if ((!this->mesh) && (!this->boundary))
	{
		return;
	}

	Real tol = Data::getTolerance();

	if (this->useTolerance)
	{
		Data::setTolerance(this->tolerance);
	}

	if (this->mesh)
	{
		this->dimension = this->mesh->dimension();

		for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
			 iter != this->mesh->vEnd(); iter++)
		{
			this->mergeMesh((*iter), iter);
		}
	}
	else
	{
		this->dimension = this->boundary->dimension();

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			this->mergeBoundary((*iter), iter);
		}
	}

	if (this->useTolerance)
	{
		Data::setTolerance(tol);
	}
}

bool Performer::VertexMerger::equal(const Data::Vertex *v1, const Data::Vertex *v2) const
{
	return this->useTolerance ? v1->getPoint()->equal(v2->getPoint()) : false;
}

bool Performer::VertexMerger::match(const Data::Vertex *v1, const Data::Vertex *v2) const
{
	return v1->getPoint()->match(v2->getPoint());
}

void Performer::VertexMerger::replace(Data::FrontElement *fel, Data::Vertex *vOld, Data::Vertex *vNew) const
{
	fel->replace(vOld, vNew);
}

void Performer::VertexMerger::replace(Data::Element *e, Data::Vertex *vOld, Data::Vertex *vNew) const
{
	e->replace(vOld, vNew);
}
