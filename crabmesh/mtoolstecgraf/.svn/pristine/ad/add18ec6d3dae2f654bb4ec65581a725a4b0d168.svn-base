#ifndef _PJMESH_INPUT_DATA_H_
#define _PJMESH_INPUT_DATA_H_

#include "PJmesh/Definitions.h"

#include "PMJMesh/InputData.h"
#include "Jmesh/MainDrive.h"

namespace PJmesh
{
	class InputData : public PMJMesh::InputData
	{
	public:

		InputData();
		virtual ~InputData();

		virtual void setVer(Jmesh::MainDrive::Versions ver);
		virtual Jmesh::MainDrive::Versions getVer() const;

		virtual void setVersion(int version);
		virtual int getVersion() const;

		virtual void setFormat(int format);
		virtual int getFormat() const;

		virtual void setFactor(double factor);
		virtual double getFactor() const;

		virtual void setTol(double tol);
		virtual double getTol() const;

		virtual void setInternal(int internal);
		virtual int getInternal() const;

		virtual void setNumNode(int num_node);
		virtual int getNumNode() const;

		virtual void setNodes(double **nodes);
		virtual double **getNodes() const;

		virtual void setTnodes(int *tnodes);
		virtual int *getTnodes() const;

		virtual void setNumFace(int num_face);
		virtual int getNumFace() const;

		virtual void setFaces(int **faces);
		virtual int **getFaces() const;

		virtual UInt packSize(const Parallel::Communicator *comm) const;
		using PMJMesh::InputData::pack;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

	protected:

		enum Jmesh::MainDrive::Versions ver;

		int version;
		int format;
		double factor;
		double tol;
		int internal;
		int num_node;
		double **nodes;
		int *tnodes;
		int num_face;
		int **faces;
	};
}

#endif //#ifndef _PJMESH_INPUT_DATA_H_
