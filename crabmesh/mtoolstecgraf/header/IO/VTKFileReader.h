#ifndef _IO_VTK_FILE_READER_H_
#define _IO_VTK_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class VTKFileReader :
		public IO::MeshFileReader
	{
	public:

		VTKFileReader();
		virtual ~VTKFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

		virtual void read();

		virtual bool readVtk();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;
	};
}

#endif //#ifndef _IO_VTK_FILE_READER_H_
