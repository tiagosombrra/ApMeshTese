#ifndef _IO_NEUTRAL_FILE_WRITER_H_
#define _IO_NEUTRAL_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class NeutralFileWriter :
		public IO::MeshFileWriter
	{
	public:

		NeutralFileWriter();
		virtual ~NeutralFileWriter();

		virtual void setDimension(UInt dimension);
		virtual void setBoundary(bool boundary);

	protected:

		virtual void write() const;

		virtual void writeDat() const;

	protected:

		UInt dimension;
		bool boundary;
	};
}

#endif //#ifndef _IO_NEUTRAL_FILE_WRITER_H_
