#ifndef _IO_MJMESH_FILE_READER_H_
#define _IO_MJMESH_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class MJMeshFileReader :
		public IO::MeshFileReader
	{
	public:

		MJMeshFileReader();
		virtual ~MJMeshFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual UInt getDimension() const;
		virtual Real getTreeFactor() const;
		virtual Real getTolerance() const;

	protected:

		virtual bool makeStructures();

		virtual Data::AbstractMesh *makeMesh() const;

		virtual void read();

		virtual bool readMJMesh();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		UInt dimension;
		Real treeFactor;
		Real tolerance;
	};
}

#endif //#ifndef _IO_MJMESH_FILE_READER_H_
