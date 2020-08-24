#include "PMJMesh/Partition.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/FrontElement.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Communicator.h"

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

using namespace PMJMesh;

PMJMesh::Partition::Partition() :
	Parallel::Partition(PMJMesh::TRANSFERABLE_PARTITION)
#if USE_GUI
	, Data::Drawable()
#endif //#if USE_GUI
{
	this->setBox(NULL);
	this->setSubfront(NULL);
}

PMJMesh::Partition::Partition(Data::Box *box, Real load) :
	Parallel::Partition(PMJMesh::TRANSFERABLE_PARTITION, load)
#if USE_GUI
	, Data::Drawable()
#endif //#if USE_GUI
{
	this->setBox(box);

	//this->setLoad(load);

	this->setSubfront(NULL);
}

PMJMesh::Partition::~Partition()
{
	if (this->subfront)
	{
		delete this->subfront;
	}

	if (this->box)
	{
		delete this->box;
	}
}

void PMJMesh::Partition::setBox(Data::Box *box)
{
	this->box = box;

#if USE_GUI
	if (this->box)
	{
        Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::PARTITION, r, g, b);
		this->box->setColor(r, g, b);
	}
#endif //#if USE_GUI
}

Data::Box *PMJMesh::Partition::getBox() const
{
	return this->box;
}

void PMJMesh::Partition::setSubfront(PMJMesh::Front *subfront)
{
	this->subfront = subfront;
}

PMJMesh::Front *PMJMesh::Partition::getSubfront() const
{
	return this->subfront;
}

Real PMJMesh::Partition::getLoad() const
{
	return (this->subfront) ? this->load : 0.0;
}

UInt PMJMesh::Partition::packSize(const Parallel::Communicator *comm,
	ULInt numVertices, ULInt numElements) const
{
	UInt size = 0;

	UInt dimension = this->box->dimension();

	//bool - front sorted
	size += comm->packSize(1, Parallel::Communicator::BOOL);

	//unsigned - type, dimension
	size += comm->packSize(2, Parallel::Communicator::UINT);
	//size += comm->packSize(6, Parallel::Communicator::UINT);

	//long - numvertices, numboundary, numelements, numrejected, numcrossing,
	//       id, vertices->id, frontelements->id, frontelements->vertices
	size += comm->packSize(6 + static_cast<UInt>(numVertices) + static_cast<UInt>(numElements) + dimension*static_cast<UInt>(numElements), Parallel::Communicator::ULINT);

	//real - load, min->coords, max->coords, vertices->coords
	size += comm->packSize(1 + dimension*(2 + static_cast<UInt>(numVertices)), Parallel::Communicator::REAL);

	return size;
}

UInt PMJMesh::Partition::packSize(const Parallel::Communicator *comm) const
{
	return this->packSize(comm, this->subfront->verticesSize(), this->subfront->size());
}

void PMJMesh::Partition::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	//Data::VertexList &vertices, Data::FrontElementList &boundary, Data::FrontElementList &elements,
	//Data::FrontElementList &rejected, Data::FrontElementList &crossing) const
	const Data::Front *front) const
{
	comm->pack(this->type, buffer, size, position);

	comm->pack(this->id, buffer, size, position);

	comm->pack(this->load, buffer, size, position);

	UInt dimension = this->box->dimension();

	comm->pack(dimension, buffer, size, position);

	for (UInt i = 0; i < dimension; i++)
	{
		comm->pack(this->box->getMin()->getCoord(i), buffer, size, position);
	}

	for (UInt i = 0; i < dimension; i++)
	{
		comm->pack(this->box->getMax()->getCoord(i), buffer, size, position);
	}

	comm->pack(front ? front->isSorted() : false, buffer, size, position);

	comm->pack(front ? front->verticesSize() : 0, buffer, size, position);

	if (front)
	{
		//while (!vertices.empty())
		for (Data::Front::ConstVertexIterator iter = front->vBegin();
			 iter != front->vEnd(); iter++)
		{
			//Data::Vertex *v = vertices.front();
			Data::Vertex *v = (*iter);

			//vertices.pop_front();

			comm->pack(v->getId(), buffer, size, position);

			for (UInt i = 0; i < dimension; i++)
			{
				comm->pack(v->getPoint()->getCoord(i), buffer, size, position);
			}
		}
	}

	comm->pack(front ? front->size(0) : 0, buffer, size, position);

	if (front)
	{
		//while (!boundary.empty())
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(0);
			 iter != front->eEnd(0); iter++)
		{
			Data::FrontElement *fel = (*iter);

			//boundary.pop_front();

			comm->pack(fel->getId(), buffer, size, position);

			for (UInt i = 0; i < dimension; i++)
			{
				comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
			}
		}
	}

	comm->pack(front ? front->size(1) : 0, buffer, size, position);

	if (front)
	{
		//while (!elements.empty())
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(1);
			 iter != front->eEnd(1); iter++)
		{
			//Data::FrontElement *fel = elements.front();
			Data::FrontElement *fel = (*iter);

			//elements.pop_front();

			comm->pack(fel->getId(), buffer, size, position);

			for (UInt i = 0; i < dimension; i++)
			{
				comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
			}
		}
	}

	comm->pack(front ? front->size(2) : 0, buffer, size, position);

	if (front)
	{
		//while (!rejected.empty())
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(2);
			 iter != front->eEnd(2); iter++)
		{
			//Data::FrontElement *fel = rejected.front();
			Data::FrontElement *fel = (*iter);

			//rejected.pop_front();

			comm->pack(fel->getId(), buffer, size, position);

			for (UInt i = 0; i < dimension; i++)
			{
				comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
			}
		}
	}

	comm->pack(front ? front->size(3) : 0, buffer, size, position);

	if (front)
	{
		//while (!crossing.empty())
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(3);
			 iter != front->eEnd(3); iter++)
		{
			//Data::FrontElement *fel = crossing.front();
			Data::FrontElement *fel = (*iter);

			//crossing.pop_front();

			comm->pack(fel->getId(), buffer, size, position);

			for (UInt i = 0; i < dimension; i++)
			{
				comm->pack(fel->getVertex(i)->getId(), buffer, size, position);
			}
		}
	}
}

void PMJMesh::Partition::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	//Data::VertexList vertices;
	//Data::FrontElementList /*boundary, elements, rejected, */crossing;

	/*if (this->subfront)
	{
		//vertices = this->subfront->getVertices();

		//boundary = this->subfront->getBoundary();
		//elements = this->subfront->getElements();
		//rejected = this->subfront->getRejected();
		//crossing = this->subfront->getCrossing();
	}*/

	//this->pack(comm, buffer, size, position, vertices, boundary, elements, rejected, crossing);
	this->pack(comm, buffer, size, position, this->subfront);
}

Parallel::Package PMJMesh::Partition::pack(const Parallel::Communicator *comm) const
{
	//Data::VertexList vertices;
	//Data::FrontElementList /*boundary, elements, rejected, */crossing;

	/*if (this->subfront)
	{
		//vertices = this->subfront->getVertices();

		//boundary = this->subfront->getBoundary();
		//elements = this->subfront->getElements();
		//rejected = this->subfront->getRejected();
		//crossing = this->subfront->getCrossing();
	}*/

	Int size = this->packSize(comm, this->subfront ? this->subfront->verticesSize() : 0,
		this->subfront ? this->subfront->size(0) + this->subfront->size(1) + this->subfront->size(2) + this->subfront->size(3) : 0);

	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	//this->pack(comm, buffer, size, position, vertices, boundary, elements, rejected, crossing);
	this->pack(comm, buffer, size, position, this->subfront);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::Partition::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	Int size = p.first;
	char *buffer = p.second;

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	comm->unpack(this->id, buffer, size, position);

	comm->unpack(this->load, buffer, size, position);

	UInt dimension;

	comm->unpack(dimension, buffer, size, position);

	Data::Point *min = NULL, *max = NULL;

	if (dimension == 2)
	{
		min = new Data::Point2D();
		max = new Data::Point2D();
	}
	else
	{
		min = new Data::Point3D();
		max = new Data::Point3D();
	}

	for (UInt i = 0; i < dimension; i++)
	{
		Real coord = 0.0;

		comm->unpack(coord, buffer, size, position);

		min->setCoord(i, coord);
	}

	for (UInt i = 0; i < dimension; i++)
	{
		Real coord = 0.0;

		comm->unpack(coord, buffer, size, position);

		max->setCoord(i, coord);
	}

	this->box = new Data::Box(min, max);

	bool sorted = false;

	comm->unpack(sorted, buffer, size, position);

	ULInt numVertices = 0;

	comm->unpack(numVertices, buffer, size, position);

	Data::VertexList vertices;

#if USE_C__11
	Data::VertexHashMap vMap;

	vMap.reserve(numVertices);
#else
	Data::VertexMap vMap;
#endif //#if USE_C__11

	for (ULInt i = 0; i < numVertices; i++)
	{
		Data::Point *p = (dimension == 2) ?
			static_cast<Data::Point *>(new Data::Point2D()) :
			static_cast<Data::Point *>(new Data::Point3D());

		ULInt id = 0;

		comm->unpack(id, buffer, size, position);

		for (UInt i = 0; i < dimension; i++)
		{
			Real coord = 0.0;

			comm->unpack(coord, buffer, size, position);

			p->setCoord(i, coord);
		}

		MJMesh::Vertex *v = new MJMesh::Vertex(p, id);

		vertices.push_back(v);

		vMap[v->getId()] = v;
	}

	ULInt numElements;

	Data::FrontElementList boundary, elements, rejected, crossing;

	comm->unpack(numElements, buffer, size, position);

	for (ULInt i = 0; i < numElements; i++)
	{
		ULInt id = 0, idv1 = 0, idv2 = 0;

		comm->unpack(id, buffer, size, position);

		comm->unpack(idv1, buffer, size, position);

		comm->unpack(idv2, buffer, size, position);

		MJMesh::FrontElement *fel = NULL;

		if (dimension == 2)
		{
			fel = new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]), id);
		}
		else
		{
			ULInt idv3 = 0;

			comm->unpack(idv3, buffer, size, position);

			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]),
				static_cast<MJMesh::Vertex *>(vMap[idv3]), id);
		}

		boundary.push_back(fel);
	}

	comm->unpack(numElements, buffer, size, position);

	for (ULInt i = 0; i < numElements; i++)
	{
		ULInt id = 0, idv1 = 0, idv2 = 0;

		comm->unpack(id, buffer, size, position);

		comm->unpack(idv1, buffer, size, position);

		comm->unpack(idv2, buffer, size, position);

		MJMesh::FrontElement *fel = NULL;

		if (dimension == 2)
		{
			fel = new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]), id);
		}
		else
		{
			ULInt idv3 = 0;

			comm->unpack(idv3, buffer, size, position);

			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]),
				static_cast<MJMesh::Vertex *>(vMap[idv3]), id);
		}

		elements.push_back(fel);
	}

	comm->unpack(numElements, buffer, size, position);

	for (ULInt i = 0; i < numElements; i++)
	{
		ULInt id = 0, idv1 = 0, idv2 = 0;

		comm->unpack(id, buffer, size, position);

		comm->unpack(idv1, buffer, size, position);

		comm->unpack(idv2, buffer, size, position);

		MJMesh::FrontElement *fel = NULL;

		if (dimension == 2)
		{
			fel = new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]), id);
		}
		else
		{
			ULInt idv3 = 0;

			comm->unpack(idv3, buffer, size, position);

			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]),
				static_cast<MJMesh::Vertex *>(vMap[idv3]), id);
		}

		rejected.push_back(fel);
	}

	comm->unpack(numElements, buffer, size, position);

	for (ULInt i = 0; i < numElements; i++)
	{
		ULInt id = 0, idv1 = 0, idv2 = 0;

		comm->unpack(id, buffer, size, position);

		comm->unpack(idv1, buffer, size, position);

		comm->unpack(idv2, buffer, size, position);

		MJMesh::FrontElement *fel = NULL;

		if (dimension == 2)
		{
			fel = new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]), id);
		}
		else
		{
			ULInt idv3 = 0;

			comm->unpack(idv3, buffer, size, position);

			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vMap[idv1]), static_cast<MJMesh::Vertex *>(vMap[idv2]),
				static_cast<MJMesh::Vertex *>(vMap[idv3]), id);
		}

		crossing.push_back(fel);
	}

	this->subfront = new PMJMesh::Front(sorted);

	this->subfront->setVertices(vertices);
	this->subfront->setBoundary(boundary);
	this->subfront->setElements(elements);
	this->subfront->setRejected(rejected);
	this->subfront->setCrossing(crossing);
}

#if USE_GUI
void PMJMesh::Partition::setR(Real r) const
{
	if (this->box)
	{
		this->box->setR(r);
	}
}

Real PMJMesh::Partition::getR() const
{
	return (this->box) ? this->box->getR() : 0.0;
}

void PMJMesh::Partition::setG(Real g) const
{
	if (this->box)
	{
		this->box->setG(g);
	}
}

Real PMJMesh::Partition::getG() const
{
	return (this->box) ? this->box->getG() : 0.0;
}

void PMJMesh::Partition::setB(Real b) const
{
	if (this->box)
	{
		this->box->setB(b);
	}
}

Real PMJMesh::Partition::getB() const
{
	return (this->box) ? this->box->getB() : 0.0;
}

void PMJMesh::Partition::highlight() const
{
	if (this->box)
	{
		this->box->highlight();
	}
}

void PMJMesh::Partition::unhighlight() const
{
	if (this->box)
	{
		this->box->unhighlight();
	}
}

bool PMJMesh::Partition::isHighlighted() const
{
	return (this->box) ? this->box->isHighlighted() : false;
}

void PMJMesh::Partition::draw() const
{
	/*if (!Data::getState().isInVisibleSpace(this->box))
	{
		return;
	}*/

	if (this->box)
	{
		this->box->draw();

		this->drawId();
	}
}

void PMJMesh::Partition::drawId() const
{
	this->drawId(this->getR(), this->getG(), this->getB());
}

void PMJMesh::Partition::drawId(Real r, Real g, Real b) const
{
	if (!Data::getState().isIdDrawn())
	{
		return;
	}

	if (!Data::getState().isInVisibleSpace(this->box))
	{
		return;
	}

	glPushMatrix();
		/*GLboolean logic = 0, lighting;

		glGetBooleanv(GL_COLOR_LOGIC_OP, &logic);
		glGetBooleanv(GL_LIGHTING, &lighting);

		if (lighting)
		{
			glDisable(GL_LIGHTING);
		}

		if (!logic)
		{
			glEnable(GL_COLOR_LOGIC_OP);
		}

		GLint opcode = GL_COPY;

		glGetIntegerv(GL_LOGIC_OP_MODE, &opcode);

		glLogicOp(GL_XOR);*/

		Data::glColor(r, g, b);

#if USE_EXPERIMENTAL_3
		if (this->box->dimension() == 2)
        {
            Data::Point2D mid = this->box->mid2D();

            this->explodeAndShrink(mid);

            Data::glTextId(mid.getCoord(0), mid.getCoord(1), 0.0, this->getId());
        }
		else
        {
            Data::Point3D mid = this->box->mid3D();

            this->explodeAndShrink(mid);

            Data::glTextId(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2), this->getId());
        }
#else
		Data::Point mid = this->box->mid();

		this->explodeAndShrink(mid);

		Data::glTextId(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2), this->getId());
#endif //#if USE_EXPERIMENTAL_3

		/*glLogicOp(opcode);

		if (!logic)
		{
			glDisable(GL_COLOR_LOGIC_OP);
		}

		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}*/
	glPopMatrix();
}

std::string PMJMesh::Partition::text() const
{
	std::stringstream str;

	str << this->getId();

	if (this->box)
	{
		str << " " << this->box->text();
	}

	str << " load: " << this->load;

	return str.str();
}
#endif //#if USE_GUI
