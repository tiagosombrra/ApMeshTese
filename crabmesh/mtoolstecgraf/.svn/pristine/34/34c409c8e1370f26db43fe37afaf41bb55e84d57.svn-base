#ifndef _IO_JMESH_FILE_READER_H_
#define _IO_JMESH_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class JmeshFileReader :
		public IO::MeshFileReader
	{
	public:

		JmeshFileReader();
		virtual ~JmeshFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool isInternal() const;
		virtual bool isFromone() const;
		virtual Real getTreeFactor() const;
		virtual Real getTolerance() const;

	protected:

		virtual bool makeStructures();

		virtual Data::AbstractMesh *makeMesh() const;

		virtual void read();

		virtual bool readDMsh();
		virtual bool readJMsh();
		virtual bool readWMsh();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		bool internal;
		bool fromone;
		Real treeFactor;
		Real tolerance;
	};
}

#endif //#ifndef _IO_JMESH_FILE_READER_H_
