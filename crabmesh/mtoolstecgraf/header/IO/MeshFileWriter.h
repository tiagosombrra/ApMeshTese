#ifndef _IO_MESH_FILE_WRITER_H_
#define _IO_MESH_FILE_WRITER_H_

#include "IO/FileWriter.h"

namespace IO
{
	class MeshFileWriter :
		public IO::FileWriter
	{
	public:

		MeshFileWriter();
		virtual ~MeshFileWriter();

		virtual void setMesh(const Data::AbstractMesh *mesh);

	protected:

		virtual bool areStructuresCorrect() const;

	protected:

		const Data::AbstractMesh *mesh;
	};
}

#endif //#ifndef _IO_MESH_FILE_WRITER_H_
