#ifndef _IO_CDB_FILE_READER_H_
#define _IO_CDB_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class CDBFileReader :
		public IO::MeshFileReader
	{
	public:

		CDBFileReader();
		virtual ~CDBFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool isBoundary() const;

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readCDB();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		bool boundary;
	};
}

#endif //#ifndef _IO_CDB_FILE_READER_H_
