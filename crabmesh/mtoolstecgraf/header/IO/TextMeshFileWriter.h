#ifndef _IO_TEXT_MESH_FILE_WRITER_H_
#define _IO_TEXT_MESH_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class TextMeshFileWriter :
		public IO::MeshFileWriter
	{
	public:

		TextMeshFileWriter();
		virtual ~TextMeshFileWriter();

		virtual void setDimension(UInt dimension);

	protected:

		virtual void write() const;

		virtual void writeText() const;

	protected:

		UInt dimension;
	};
}

#endif //#ifndef _IO_TEXT_MESH_FILE_WRITER_H_
