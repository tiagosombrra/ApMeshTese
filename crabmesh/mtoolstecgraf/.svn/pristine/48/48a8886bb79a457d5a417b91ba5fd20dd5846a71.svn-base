#ifndef _IO_ABAQUS_FILE_READER_H_
#define _IO_ABAQUS_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class AbaqusFileReader :
		public IO::MeshFileReader
	{
	public:

		AbaqusFileReader();
		virtual ~AbaqusFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool isBoundary() const;

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readAbaqus();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		bool boundary;
	};
}

#endif //#ifndef _IO_ABAQUS_FILE_READER_H_
