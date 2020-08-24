#ifndef _IO_GEN_FILE_READER_H_
#define _IO_GEN_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class GenFileReader :
		public IO::MeshFileReader
	{
	public:

		GenFileReader();
		virtual ~GenFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual UInt getDimension() const;
		virtual bool isBoundary() const;
		virtual bool isSurface() const;

	protected:

		virtual bool makeStructures();

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readGen();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		UInt dimension;
		bool boundary;
		bool surface;
	};
}

#endif //#ifndef _IO_GEN_FILE_READER_H_
