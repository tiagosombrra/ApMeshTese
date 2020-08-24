#ifndef _IO_NEUTRAL_FILE_READER_H_
#define _IO_NEUTRAL_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class NeutralFileReader :
		public IO::MeshFileReader
	{
	public:

		NeutralFileReader();
		virtual ~NeutralFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual UInt getDimension() const;
		virtual bool isBoundary() const;

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readDat();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		UInt dimension;
		bool boundary;
	};
}

#endif //#ifndef _IO_NEUTRAL_FILE_READER_H_
