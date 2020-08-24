#include "PJmesh/InputData.h"

#include "Parallel/Communicator.h"

using namespace PJmesh;

PJmesh::InputData::InputData() :
	PMJMesh::InputData()
{
	this->setVer(Jmesh::MainDrive::ADVAN);

	this->setVersion(0);
	this->setFormat(0);
	this->setFactor(0.0);
	this->setTol(0.0);
	this->setInternal(0);
	this->setNumNode(0);
	this->setNodes(NULL);
	this->setTnodes(NULL);
	this->setNumFace(0);
	this->setFaces(NULL);
}

PJmesh::InputData::~InputData()
{

}

void PJmesh::InputData::setVer(Jmesh::MainDrive::Versions ver)
{
	this->ver = ver;
}

Jmesh::MainDrive::Versions PJmesh::InputData::getVer() const
{
	return this->ver;
}

void PJmesh::InputData::setVersion(int version)
{
	this->version = version;
}

int PJmesh::InputData::getVersion() const
{
	return this->version;
}

void PJmesh::InputData::setFormat(int format)
{
	this->format = format;
}

int PJmesh::InputData::getFormat() const
{
	return this->format;
}

void PJmesh::InputData::setFactor(double factor)
{
	this->factor = factor;
}

double PJmesh::InputData::getFactor() const
{
	return this->factor;
}

void PJmesh::InputData::setTol(double tol)
{
	this->tol = tol;
}

double PJmesh::InputData::getTol() const
{
	return this->tol;
}

void PJmesh::InputData::setInternal(int internal)
{
	this->internal = internal;
}

int PJmesh::InputData::getInternal() const
{
	return this->internal;
}

void PJmesh::InputData::setNumNode(int num_node)
{
	this->num_node = num_node;
}

int PJmesh::InputData::getNumNode() const
{
	return this->num_node;
}

void PJmesh::InputData::setNodes(double **nodes)
{
	this->nodes = nodes;
}

double **PJmesh::InputData::getNodes() const
{
	return this->nodes;
}

void PJmesh::InputData::setTnodes(int *tnodes)
{
	this->tnodes = tnodes;
}

int *PJmesh::InputData::getTnodes() const
{
	return this->tnodes;
}

void PJmesh::InputData::setNumFace(int num_face)
{
	this->num_face = num_face;
}

int PJmesh::InputData::getNumFace() const
{
	return this->num_face;
}

void PJmesh::InputData::setFaces(int **faces)
{
	this->faces = faces;
}

int **PJmesh::InputData::getFaces() const
{
	return this->faces;
}

UInt PJmesh::InputData::packSize(const Parallel::Communicator *comm) const
{
	UInt size = PMJMesh::InputData::packSize(comm);

	// unsigned - ver
	size += comm->packSize(1, Parallel::Communicator::UINT);

	// int - version, format, internal, num_node, tnodes, num_face, faces
	size += comm->packSize(5 + num_node + num_face*3, Parallel::Communicator::INT);

	// double - factor, tol, nodes;
	size += comm->packSize(2 + num_node*3, Parallel::Communicator::REAL);

	return size;
}

void PJmesh::InputData::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	PMJMesh::InputData::pack(comm, buffer, size, position);

	comm->pack(this->ver, buffer, size, position);

	comm->pack(this->version, buffer, size, position);

	comm->pack(this->format, buffer, size, position);

	comm->pack(this->factor, buffer, size, position);

	comm->pack(this->tol, buffer, size, position);

	comm->pack(this->internal, buffer, size, position);

	comm->pack(this->num_node, buffer, size, position);

	for (int i = 0; i < this->num_node; i++)
		comm->pack(this->nodes[i], 3, buffer, size, position);

	comm->pack(this->tnodes, this->num_node, buffer, size, position);

	comm->pack(this->num_face, buffer, size, position);

	for (int i = 0; i < this->num_face; i++)
		comm->pack(this->faces[i], 3, buffer, size, position);
}

void PJmesh::InputData::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	PMJMesh::InputData::unpack(comm, p, position);

	Int size = p.first;
	char *buffer = p.second;

	UInt ver = 0;

	comm->unpack(ver, buffer, size, position);

	this->ver = static_cast<Jmesh::MainDrive::Versions>(ver);

	comm->unpack(this->version, buffer, size, position);

	comm->unpack(this->format, buffer, size, position);

	comm->unpack(this->factor, buffer, size, position);

	comm->unpack(this->tol, buffer, size, position);

	comm->unpack(this->internal, buffer, size, position);

	comm->unpack(this->num_node, buffer, size, position);

	this->nodes = (double **)Msh3DCalloc(this->num_node, sizeof(double *));

	for (int i = 0; i < this->num_node; i++)
	{
		this->nodes[i] = (double *)Msh3DCalloc(3, sizeof(double));

		comm->unpack(this->nodes[i], 3, buffer, size, position);
	}

	this->tnodes = (int *)Msh3DCalloc(this->num_node, sizeof(int));

	comm->unpack(this->tnodes, this->num_node, buffer, size, position);

	comm->unpack(this->num_face, buffer, size, position);

	this->faces = (int **)Msh3DCalloc(this->num_face, sizeof(int *)) ;

	for (int i = 0; i < this->num_face; i++)
	{
		this->faces[i] = (int *)Msh3DCalloc(3, sizeof(int)) ;

		comm->unpack(this->faces[i], 3, buffer, size, position);
	}
}
