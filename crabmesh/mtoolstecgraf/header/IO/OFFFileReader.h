#ifndef _IO_OFF_FILE_READER_H_
#define _IO_OFF_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class OFFFileReader :
		public IO::MeshFileReader
	{
	public:

		OFFFileReader();
		virtual ~OFFFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readOFF();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;
	};
}

#endif //#ifndef _IO_OFF_FILE_READER_H_
