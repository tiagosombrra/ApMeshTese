#ifndef _IO_MJMESH_FILE_WRITER_H_
#define _IO_MJMESH_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class MJMeshFileWriter :
		public IO::MeshFileWriter
	{
	public:

		MJMeshFileWriter();
		virtual ~MJMeshFileWriter();

		virtual void setDimension(UInt dimension);
		virtual void setTreeFactor(Real treeFactor);
		virtual void setTolerance(Real tolerance);

	protected:

		virtual void write() const;

		virtual void writeMJMesh() const;

	protected:

		UInt dimension;
		Real treeFactor;
		Real tolerance;
	};
}

#endif //#ifndef _IO_MJMESH_FILE_WRITER_H_
