#ifndef _IO_ABAQUS_FILE_WRITER_H_
#define _IO_ABAQUS_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class AbaqusFileWriter :
		public IO::MeshFileWriter
	{
	public:

		AbaqusFileWriter();
		virtual ~AbaqusFileWriter();

	protected:

		virtual void write() const;

		virtual void writeAbaqus() const;

	};
}

#endif //#ifndef _IO_ABAQUS_FILE_WRITER_H_

