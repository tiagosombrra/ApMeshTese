#ifndef _IO_STL_FILE_READER_H_
#define _IO_STL_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class STLFileReader :
		public IO::MeshFileReader
	{
	public:

		STLFileReader();
		virtual ~STLFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readAsciiStl();
		virtual bool readBinaryStl();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;
	};
}

#endif //#ifndef _IO_STL_FILE_READER_H_
